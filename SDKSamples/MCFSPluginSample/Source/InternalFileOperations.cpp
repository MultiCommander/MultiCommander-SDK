#include "stdafx.h"
#include "InternalFileOperations.h"
#include "MemoryFileSystem.h"
#include "AutoHandle.h"

using namespace MCNS;

namespace
{
  const DWORD BUFFERSIZE = 1024*1024;

  bool GetPathParent( TCHAR* strPath /*[in/out]*/)
  {
    TCHAR* pos1 = _tcsrchr( strPath , '\\' );
    TCHAR* pos2 = _tcsrchr( strPath , '/' );
    TCHAR* pos = max(pos1,pos2);
    if( pos )
    {
      if( (pos - strPath)+1 == (int)_tcslen(strPath) )
      {
        *pos = '\0';
        return GetPathParent( strPath );
      }

      if( pos > strPath && *(pos-1) == ':' )
        pos++;
      *pos = '\0';
      return true;
    }
    return false;     
  }

}

InternalReadFileOperations::InternalReadFileOperations( MCNS::IProgress* pProgress , DWORD dwFlags )
  : InternalFileOperation(IFOBJ_READ)
  , m_pProgress(pProgress)
  , m_dwFlags(dwFlags)
{

}

DWORD InternalReadFileOperations::WriteQueuedItemsToDisk()
{
  for (auto&& p : m_vItems)
  {
    DWORD dwError = WriteItem(p.get());
    if (dwError != ERROR_SUCCESS)
      return dwError;   
  }

  return ERROR_SUCCESS;
}

DWORD InternalReadFileOperations::WriteItem(WriteToDiskItem* pItem)
{
  return pItem->pFile->WriteToDisk(pItem->m_strTargetName.c_str());
}

//////////////////////////////////////////////////////////////////////////

InternalWriteFileOperations::InternalWriteFileOperations( MCNS::IProgress* pProgress , DWORD dwFlags )
  : InternalFileOperation(IFOBJ_WRITE)
  , m_pMemFS(NULL)
  , m_pProgress(pProgress)
  , m_dwFlags(dwFlags)
{

}

bool InternalWriteFileOperations::AddItem(const IFileItem* pItem, const WCHAR* szTargetName)
{
  m_vItems.push_back(WriteItem(pItem, szTargetName));
  return true;  
}

bool InternalWriteFileOperations::AddDeleteItem(const WCHAR* szDeleteFileItem)
{
  m_vDeleteItems.push_back(DeleteItem(szDeleteFileItem));
  return true;  
}

DWORD InternalWriteFileOperations::StartFileOperations(MemoryFileSystem* pFS)
{
  m_pMemFS = pFS;
  DWORD dwError = VERROR_NOERROR;

  if(m_vItems.size() > 0)
    dwError = StartCopyFileOperations();

  if(dwError == VERROR_NOERROR && m_vDeleteItems.size() > 0)
    dwError = StartDeleteFileOperations();

  return dwError;
}

DWORD InternalWriteFileOperations::StartDeleteFileOperations()
{
  std::wstring targetPath;
  DWORD nItems = CountDeleteItems();
  bool bAbort = false;

  // Set this to make the progress window poll for updates.
  // else you need to call m_pProgress->Update your self. and if the updates are very fast
  // It might slow things down.
  m_pProgress->StartAutoUpdate(250);

  m_pProgress->Set_Current_Max(nItems);
  m_pProgress->Set_Current_Now(0);

  int nDeleted = 0;
  for(DWORD n = 0; n < nItems; ++n)
  {
    if(bAbort)
      break;

    const std::wstring& strDeleteItem = GetDeleteItemAt(n);
    if(strDeleteItem.empty() == false)
    {
      m_pProgress->Operation_Start();
      //m_pProgress->Update();

      DWORD dwError = DeleteFileItem(strDeleteItem);

       m_pProgress->Operation_Finished();

      if(dwError != VERROR_NOERROR)
      {
        if(dwError == VERROR_ABORT_FILEOPERATION)
          bAbort = true;
        else
        {
          // Log Error
        }
      }
      else
      {
        ++nDeleted;
        m_pProgress->Set_Current_Now(nDeleted);
      }

    }
  }

  m_pProgress->StopAutoUpdate();

  return 0;
}

DWORD InternalWriteFileOperations::StartCopyFileOperations()
{
  // We do not support SKIP so disable that button in the progress UI
  m_pProgress->EnableButtonSkip(false);

  std::wstring targetPath;
  DWORD nItems = CountItems();
  bool bAbort = false;
  
  if(m_pBuffer == NULL)
    m_pBuffer = std::unique_ptr<BYTE[]>(new BYTE[BUFFERSIZE]);

  // Set this to make the progress window poll for updates.
  // else you need to call m_pProgress->Update your self. and if the updates are very fast
  // It might slow things down.
  m_pProgress->StartAutoUpdate(250);

  for(DWORD n = 0; n < nItems; ++n)
  {
    if(bAbort)
      break;

    std::wstring strTargetPath = TargetNameAt(n);
    const IFileItem* pSourceItem  = SourceItemAt(n);
    if(pSourceItem == NULL || strTargetPath.empty())
      continue;

    DWORD dwError = CopyFileItem(pSourceItem, strTargetPath);
   
    if(dwError != VERROR_NOERROR)
    {
      if(dwError == VERROR_ABORT_FILEOPERATION)
        bAbort = true;
      else
      {
        // Log Error
      }
    }
  }

  m_pProgress->StopAutoUpdate();
  m_pProgress->EnableButtonSkip(true);
  return TRUE;
}

DWORD InternalWriteFileOperations::DeleteFileItem(const std::wstring& strDeleteItem)
{
  std::shared_ptr<MemoryFile> pFile = m_pMemFS->FindByPath(strDeleteItem.c_str());
  if(pFile)
  {
    m_pProgress->Set_FromW(strDeleteItem.c_str());
    m_pProgress->UpdateNames();

    std::shared_ptr<MemoryFile> pFolder = m_pMemFS->FindByPath(strDeleteItem.c_str(), true);
    if(pFolder)
    {
      if(pFolder->DeleteFileItem(pFile) == false)
        return VERROR_FILE_NOT_FOUND;

      NotifyPathChange(strDeleteItem, true);
      return VERROR_NOERROR;
    }
    return VERROR_PATH_NOT_FOUND;
  }

  return VERROR_FILE_NOT_FOUND;
}

DWORD InternalWriteFileOperations::CopyFileItem(const MCNS::IFileItem* pSourceItem, const std::wstring& strTargetPath)
{
  WCHAR szSourcePath[_MC_MAXPATH_];

  SetProgressTargetPath(strTargetPath);
  SetProgressSourceInfo(pSourceItem, szSourcePath, _MC_MAXPATH_);

  m_pProgress->Operation_Start();  // Start timing operation so estimated / speed are shown.
  m_pProgress->UpdateNames();      // Update To/From names in UI

  if(pSourceItem->isFolder())
  {
    std::shared_ptr<MemoryFile> pFile = m_pMemFS->CreateFolderItem(strTargetPath.c_str());
    if(pFile == NULL)
      return VERROR_ALREADY_EXISTS; 

    SetFileDateAndAttributes(pFile, pSourceItem);
    NotifyPathChange(strTargetPath);
    return VERROR_NOERROR;
  }

  // Open Source File
  AutoHandle hFile(::CreateFile(szSourcePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0));
  if(hFile.isValid() == false)
  {
    //m_pProgress->ShowFileOperationsErrorDlg();
    return VERROR_FILE_NOT_FOUND; // can be other error - This should have been checked before we got here.but can be deleted since then
  }

  // Create Target File
  std::shared_ptr<MemoryFile> pFile = m_pMemFS->FindCreateItemByPath(strTargetPath.c_str());
  if(pFile == NULL)
  {
    return VERROR_ACCESS_DENIED;
  }

  // Prepare target file for write.
  pFile->ReserveContentSize(pSourceItem->Get_Size());

  DWORD dwBytesRead = 0;
  DWORD dwBytesWritten = 0;
  while(ReadFile(hFile, m_pBuffer.get(), BUFFERSIZE, &dwBytesRead, 0) && dwBytesRead > 0)
  {
    if(pFile->WriteBytes(m_pBuffer.get(), dwBytesRead, &dwBytesWritten) == false)
    {
      return VERROR_WRITEERROR;
    }

    // Read & write data
    m_pProgress->Add_Done(dwBytesWritten);

    // Check if pause is requested .. Check for AR_RUNSTAT_SKIP if you want to know if Pause button has been pressed
    while(m_pProgress->GetRunningStatus() == AR_RUNSTAT_PAUSE)
    {
      Sleep(200);
    }

    if(m_pProgress->GetRunningStatus() == AR_RUNSTAT_ABORT)
    {
      return VERROR_ABORT_FILEOPERATION;
    }
  }
  
  SetFileDateAndAttributes(pFile, pSourceItem);

  m_pProgress->Operation_Finished(true);
  
  // Since target path is not monitored and the MCCore system does not know when this filesystem has change.
  // You must let MC know that the path has change so that MC will rescan/update
  // However. if you copy operation is long... Then MC might want to refresh during the copy operations
  // So make sure that you write thread safe code. or that you do not allow browse of filesystem until it is done.
  // Alternative is to only send the m_pProgress->UpdatePathItem(...); when you are done. and not during the copy process.
  // But it depends on what your filesystem support.
  NotifyPathChange(strTargetPath);

  return VERROR_NOERROR;
}

void InternalWriteFileOperations::NotifyPathChange(const std::wstring& pathItem, bool bDelete)
{
  TCHAR szTargetPath[_MC_MAXPATH_];
  wcscpy(szTargetPath, _T("SMP2:\\")); 
  wcsncpy(szTargetPath+6, pathItem.c_str(), _MC_MAXPATH_-7); 
  GetPathParent(szTargetPath); // Will cut last part. so we are left with the parent path.

  if(bDelete)
    m_pProgress->UpdatePathItem(szTargetPath , true, false , false );
  else
    m_pProgress->UpdatePathItem(szTargetPath , false , true , false );
}

void InternalWriteFileOperations::SetProgressTargetPath(const std::wstring& strTargetPath)
{
  if(m_pProgress)
  {
    std::wstring targetPath = _T("SMP2:\\");
    targetPath += strTargetPath;
    m_pProgress->Set_ToW(targetPath.c_str());
  }
}

void InternalWriteFileOperations::SetProgressSourceInfo(const MCNS::IFileItem* pSourceItem, WCHAR* szSourcePath, size_t len)
{
  pSourceItem->Get_FullPath(szSourcePath, (DWORD)len);
  szSourcePath[len - 1] = '\0';
  m_pProgress->Set_FromW(szSourcePath);

  m_pProgress->Set_Current_Now(0);
  m_pProgress->Set_Current_Max(pSourceItem->Get_Size());
}

DWORD InternalWriteFileOperations::CountItems()
{
  return (DWORD)m_vItems.size();
}

const MCNS::IFileItem* InternalWriteFileOperations::SourceItemAt(DWORD n)
{
  if(n >= m_vItems.size())
    return NULL;

  return m_vItems.at(n).m_pSourceItem;
}

std::wstring     InternalWriteFileOperations::TargetNameAt(DWORD n)
{
  if(n >= m_vItems.size())
    return NULL;

  return m_vItems.at(n).m_strTargetName;
}

DWORD InternalWriteFileOperations::CountDeleteItems()
{
  return (DWORD)m_vDeleteItems.size();
}

const std::wstring& InternalWriteFileOperations::GetDeleteItemAt(DWORD n) const
{
  if(n >= m_vDeleteItems.size())
    return m_Empty;

  return m_vDeleteItems.at(n).m_strDeleteItem;
}

void InternalWriteFileOperations::ClearDeleteItems()
{
  m_vDeleteItems.clear();
}

// Set the FileDate and Attributes from pFileItem to pFile
void InternalWriteFileOperations::SetFileDateAndAttributes(std::shared_ptr<MemoryFile>& pFile, const MCNS::IFileItem* pFileItem)
{
  // Do not set Time if the flag ZAF_DKEEP_TIME is set
  if((m_dwFlags & FILEOP_DKEEP_TIME) == 0 )
  {
    FILETIME ft;
    pFileItem->Get_DateWrite(&ft);
    pFile->SetFileTime(&ft);
  }

  if((m_dwFlags & FILEOP_DKEEP_ATTRIB) == 0 )
  {
    DWORD dwAttributes = pFileItem->Get_Attributes();
    dwAttributes ^= dwAttributes & ZFF_MC_EXTENDED; // Remove all MC Extended (of any)
    pFile->Attributes(dwAttributes);
  }

}
