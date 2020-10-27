#include "stdafx.h"
#include "FSSampleInternal.h"
#include "InternalFileOperations.h"

using namespace MCNS;

// This GUID MUST be unique, in VS2005+ go to Tools->CreateGUID and select "Registry formt" and remove { } and - 
#pragma message(" ** If you create your own Plugin then change this GUID \n ** In visual studio go to Menu > Tools > Create GUID. And remove all ")
char MCFSSampleInternal::m_GuidString[34] = "D88EF1959F744837B4A8C14D063F3023";

MemoryFileSystem MCFSSampleInternal::m_MemFS;

namespace
{
  const WCHAR* GetLastPathPart(const WCHAR* szPath)
  {
    if(szPath == NULL)
      return NULL;

    const WCHAR* szLast = wcsrchr(szPath, '\\');
    if(szLast)
      ++szLast;
    else
      return szPath; // if szLast is NULL then \ was not found. so it must be root item.

    return szLast;
  }

  const WCHAR* RemoveDeviceFromPath(const WCHAR* szPath)
  {
    if(_wcsnicmp(szPath, L"smp2:\\" , 6) == 0)
      szPath = szPath + 6;
    else if(_wcsnicmp(szPath, L"smp2:" , 5) == 0)
      szPath = szPath + 5;

    return szPath;
  }
}

bool MCFSSampleInternal::GetExtensionInfo( DLLExtensionInfo* pInfo )
{
  if( pInfo == NULL )
    return false;

  wcsncpy( pInfo->wsName , L"FSSample-Internal" , 100 );
  wcsncpy( pInfo->wsPublisher , L"Mathias Svensson" , 100 );
  wcsncpy( pInfo->wsURL , L"http:\\\\www.multicommander.com" , 100 );
  wcsncpy( pInfo->wsDesc , L"Sample of a filesystem plugin using Internal Method." , 160 );
  wcsncpy( pInfo->wsBaseName , L"FSSampleInternal" , 100 );
  strncpy( pInfo->strVersion , "1.0.1.0" , 10 );
  strncpy( pInfo->strGuid , m_GuidString , 34 );

  pInfo->dwFlags = EXT_TYPE_FS | EXT_PREINIT | EXT_NOLANGFILE | EXT_VDEVICEFS; // EXT_VDEVICEFS == FileSystem is a Device  xxx: instead of a filesystem inside a file.

#ifdef _UNICODE
  pInfo->dwFlags |= EXT_OS_UNICODE;
#else
  pInfo->dwFlags |= EXT_OS_ANSI;
#endif

  pInfo->dwInitOrder = 2100;
  pInfo->dwInterfaceVersion = MULTI_INTERFACE_VERSION;

  // Default parameters
  pInfo->dwDefaultFlags[0] = 0; // Default Read Flags
  pInfo->dwDefaultFlags[1] = 0; // Default Write Flags
  wcsncpy( pInfo->wsDefaultValues[0] , L"smp2" , 512 ); // smp1:
  wcsncpy( pInfo->wsDefaultValues[1] , L""     , 512 );

  return true;
}

MCFSSampleInternal::MCFSSampleInternal()
{

}

MCFSSampleInternal::~MCFSSampleInternal()
{

}

long MCFSSampleInternal::PreStartInit( IMultiAppInterface* pInterface )
{
  // Register FileSystem handler, So this extension will get all "smp1:*" requests
  pInterface->RegisterFSURLType(L"smp2:*");

  // Add "SMP1:" as a virtual device and with Icon in the Device list. This will make it show up in DeviceCombo and Device Toolbar
  IFileSystemManager* pFSManager = (IFileSystemManager*)pInterface->QueryInterface( ZOBJ_FILESYSTEM , 0 );

  // Load icon med ID 348 from the MCIcons.dll - 348 is a Movies icon..  
  HICON hAppIconSmall = pInterface->LoadIcon_ToolbarSize( (HINSTANCE)-1, 327 , false);
  HICON hAppIconLarge = pInterface->LoadIcon_ToolbarSize( (HINSTANCE)-1, 327 , true );

  pFSManager->AddVirtualDevice( _T("SMP2 (Internal)"), L"SMP2:" , hAppIconSmall, hAppIconLarge, NULL, DEVF_FREESPACE_EMPTY);
  DestroyIcon(hAppIconSmall);
  DestroyIcon(hAppIconLarge);

  pInterface->ReleaseInterface((ZHANDLE)pFSManager, ZOBJ_FILESYSTEM);

  return 0;
}

bool MCFSSampleInternal::GetIcon(HICON* /*pIcon*/, const WCHAR* /*fileExtension*/, MCIconSize::MCIconSize /*iconSize*/)
{
  /*
  // If FSPlugin was an archive. then you can provide an icon for your extension. 
  // The icon is stored in gSmallIcon/gLargeIcon that was set in PreStartInit
  if(wcscmp(fileExtension, L".7z") == 0)
  {
    if(dwFlags & VFICON_SMALL)
      *pIcon = DuplicateIcon(gSmallIcon, 16, 16);
    else
      *pIcon = DuplicateIcon(gLargeIcon, 16, 16);
    return true;
  }
  */
  return 0;
}

DWORD MCFSSampleInternal::GetSupportedOperations()
{
  return VSO_BROWSE     // Support Browsing
    | VSO_DELETE        // Support Delete requests
    | VSO_RENAME        // Support Rename 
    | VSO_CREATENEW     // Supported adding a new files
    | VSO_MKDIR         // Support Creating of Folders
    | VSO_CHATTRIBUTE   // Support Change of attributes
    | VSO_NOUPDIR       // Will not return its own Updir folder.
    | VSO_BATCH_GETITEMS  
    | VSO_BATCH_INSERTITEMS 
    | VSO_BATCH_DELETEITEMS
    ;
}

bool MCFSSampleInternal::IsBusy()
{
  return false;
}

BOOL MCFSSampleInternal::CreateVolume( const WCHAR* szVolume, const WCHAR* szMountPath /*= NULL*/, eVCREATE_MODE nCreateMode /*= VCM_NEW*/, VolumePackOptions* pOptions /*= NULL*/, DWORD nItemHint /*= 0*/ )
{
  UNREFERENCED_PARAMETER(szVolume);
  UNREFERENCED_PARAMETER(szMountPath);
  UNREFERENCED_PARAMETER(nCreateMode);
  UNREFERENCED_PARAMETER(pOptions);
  UNREFERENCED_PARAMETER(nItemHint);

  return FALSE;
}

BOOL MCFSSampleInternal::Open( const WCHAR* szVolume , const WCHAR* szMountPath /*= NULL */, eVOPEN_MODE nOpenMode /*= VOM_READ*/, DWORD dwFlags /*= 0*/ )
{
  UNREFERENCED_PARAMETER(dwFlags);

  if( szMountPath != NULL )
  {
    if(_wcsnicmp(szMountPath, L"smp2:" , 5) == 0)
      m_strMountPath = L"SMP2:";
  }
  else 
  {
    if(_wcsnicmp(szVolume, L"smp2:" , 5) == 0)
      m_strMountPath = L"SMP2:";
  }

  m_nOpenMode = nOpenMode;
  return TRUE;
}

BOOL MCFSSampleInternal::ReOpen( int nOpenMode , int nOverrideMode /*= 0*/, DWORD dwOpenFlags /*= 0*/ )
{
  UNREFERENCED_PARAMETER(nOpenMode);
  UNREFERENCED_PARAMETER(nOverrideMode);
  UNREFERENCED_PARAMETER(dwOpenFlags);

  return TRUE;
}

BOOL MCFSSampleInternal::Close()
{
  return TRUE;
}

BOOL MCFSSampleInternal::isOpen()
{
  return FALSE;
}

DWORD MCFSSampleInternal::VerifyPath( const WCHAR* szPath )
{
  szPath = RemoveDeviceFromPath(szPath);
  if( wcslen( szPath ) > _MC_MAXPATH_)
    return VERROR_PATH_TO_LONG;

  return VERROR_NOERROR;

}

const WCHAR* MCFSSampleInternal::GetMountPath()
{
  return m_strMountPath.c_str();
}

const WCHAR* MCFSSampleInternal::GetVolumePath()
{
  return m_strMountPath.c_str();
}

ZHANDLE MCFSSampleInternal::InitFind( const WCHAR* szPath , const WCHAR* szPattern /*= NULL */, DWORD dwFlags /*= 0*/, IVolumeFindCallback* pFindCallback)
{
  UNREFERENCED_PARAMETER(szPattern);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(pFindCallback);

  ResetLastError();

  szPath = RemoveDeviceFromPath(szPath);

  // Find path item
  std::shared_ptr<MemoryFile> pFolder = m_MemFS.FindByPath(szPath);

  if(pFolder)
  {
    // Use shared_ptr and store the shared ptr in a local vector. So we know what pointer we dealt out so we can clean up better
    MemoryFileEnumerator* pEnum = new MemoryFileEnumerator(pFolder);
    return (ZHANDLE)pEnum;
  }

  return 0;
}

BOOL MCFSSampleInternal::FindNext( ZHANDLE h , /*[out]*/ IFileItem *pItem /*= NULL */ )
{
  ResetLastError();
  MemoryFileEnumerator* pEnum = (MemoryFileEnumerator*)h;
  if(pEnum)
  {
    std::shared_ptr<MemoryFile> pFile= pEnum->GetNext();
    if(pFile)
    {
      if(pItem)
        pFile->SetFileData(pItem);

      return TRUE;
    }
  }
  return FALSE;
}

BOOL MCFSSampleInternal::FindNext( ZHANDLE h , /*[out]*/ WCHAR* szFileName , int nameLen, UINT64& dwAttributes )
{
  ResetLastError();
  MemoryFileEnumerator* pEnum = (MemoryFileEnumerator*)h;
  if(pEnum)
  {
    std::shared_ptr<MemoryFile> pFile= pEnum->GetNext();
    if(pFile)
    {
      wcsncpy(szFileName, pFile->Name(), nameLen);
      dwAttributes = pFile->Attributes();
      return TRUE;
    }
  }

  return FALSE;
}

DWORD MCFSSampleInternal::SetFileData( ZHANDLE h , /*[out]*/ IFileItem *pItem , BOOL bUpdate /*= FALSE */ )
{
   UNREFERENCED_PARAMETER(bUpdate);
 
  MemoryFileEnumerator* pEnum = (MemoryFileEnumerator*)h;
  if(pEnum)
  {
    std::shared_ptr<MemoryFile> pFile = pEnum->GetCurrent();
    if(pFile)
    {
      return pFile->SetFileData(pItem);
    }
  }

  return 0;
}

BOOL MCFSSampleInternal::FindClose( ZHANDLE h )
{
  MemoryFileEnumerator* pEnum = (MemoryFileEnumerator*)h;
  if(pEnum)
    delete pEnum;

  return TRUE;
}

BOOL MCFSSampleInternal::Exists( const WCHAR* szPath )
{
  szPath = RemoveDeviceFromPath(szPath);

  if(szPath[0] == '\0') // ROOT
    return TRUE;

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szPath);
  if(pFile)
    return TRUE;

  return FALSE;
}

BOOL MCFSSampleInternal::Makedir( const WCHAR* szPath , const FILETIME* pFileTime /*= NULL*/, DWORD dwExecuteOptions /*= 0 /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(pFileTime);
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  ResetLastError();

  szPath = RemoveDeviceFromPath(szPath);

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szPath);
  if(pFile)
  {
    m_nLastError = VERROR_ALREADY_EXISTS;
    return FALSE;
  }

  auto pFolder = m_MemFS.CreateFolderItem(szPath);
  if(pFolder)
    return TRUE;

  m_nLastError = VERROR_UNKNOWN_ERROR;
  return FALSE;
}

BOOL MCFSSampleInternal::MoveFile( const WCHAR* szExistingFile , const WCHAR* szNewFilename, DWORD dwExecuteOptions /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  ResetLastError();
  szExistingFile = RemoveDeviceFromPath(szExistingFile);
  szNewFilename = RemoveDeviceFromPath(szNewFilename);

  if(szExistingFile == NULL || szNewFilename == NULL)
    return FALSE;

  if(szNewFilename == '\0')
    return FALSE;

  std::shared_ptr<MemoryFile> pFileExisting = m_MemFS.FindByPath(szExistingFile);
  if(pFileExisting)
  {

    std::shared_ptr<MemoryFile> pTargetFolder = m_MemFS.FindByPath(szNewFilename, true);
    if(pTargetFolder == NULL)
    {
      m_nLastError = VERROR_PATH_NOT_FOUND;
      return FALSE;
    }

    // Remove file from parent
    std::shared_ptr<MemoryFile> pFolder = m_MemFS.FindByPath(szExistingFile, true);
    if(pFolder)
      pFolder->DeleteFileItem(pFileExisting); // Since we are using smart ptr. it is not really deleted

    pFileExisting->Name(GetLastPathPart(szNewFilename));
    pTargetFolder->AddFileItem(pFileExisting);
    return TRUE;
  }
  else
    m_nLastError = VERROR_FILE_NOT_FOUND;

  return FALSE;
}

BOOL MCFSSampleInternal::DeleteFile( const WCHAR* szExistingFile , DWORD dwExecuteOptions /*= EXO_ISFILE*/ )
{
  UNREFERENCED_PARAMETER(szExistingFile);
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  return FALSE;
}

UINT64 MCFSSampleInternal::GetAttributes( const WCHAR* szFilename )
{
  szFilename = RemoveDeviceFromPath(szFilename);
  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
  if(pFile)
    return pFile->Attributes();

  return INVALID_FILE_ATTRIBUTES;

}

BOOL MCFSSampleInternal::SetAttributes( const WCHAR* szFilename , UINT64 dwAttribute, DWORD dwExecuteOptions /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(dwAttribute);
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  return FALSE;
}

BOOL MCFSSampleInternal::SetFileTime( const WCHAR* szFilename , const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime, DWORD dwExecuteOptions /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(lpCreationTime);
  UNREFERENCED_PARAMETER(lpLastAccessTime);
  UNREFERENCED_PARAMETER(lpLastWriteTime);
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  return FALSE;
}

BOOL MCFSSampleInternal::GetFileInfo( const WCHAR* szFilename , ZFILEINFO& info )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(info);
  return FALSE;
}

INT MCFSSampleInternal::IsFolderEmpty( const WCHAR* szFolderPath )
{
  ResetLastError();

  szFolderPath = RemoveDeviceFromPath(szFolderPath);
  std::shared_ptr<MemoryFile> pFolder = m_MemFS.FindByPath(szFolderPath);

  if(pFolder)
  {
    if(pFolder->SubItems() > 0)
      return 1;
    else
      return 0;
  }

  return -1;
}

DWORD MCFSSampleInternal::GetLastError()
{
  return m_nLastError;
}

DWORD MCFSSampleInternal::GetLastError_InternalErrorCode()
{
  return 0;
}

DWORD MCFSSampleInternal::ExtractFile( const WCHAR* szFilename , const WCHAR* szWritePath, DWORD dwFlags /*= 0 */ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(szWritePath);
  UNREFERENCED_PARAMETER(dwFlags);
  return 0;
}

IRWFile* MCFSSampleInternal::CreateFile( const WCHAR* szFilename , DWORD dwAccessMode , DWORD dwCreateMode , UINT64 dwAttributes /*= 0 */, DWORD dwRWFlags /*= 0 */, const FILETIME* ftFileTime /*= NULL */, INT64 /*nFileSize*/ /*= 0*/ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(dwAccessMode);
  UNREFERENCED_PARAMETER(dwCreateMode);
  UNREFERENCED_PARAMETER(dwAttributes);
  UNREFERENCED_PARAMETER(dwRWFlags);
  UNREFERENCED_PARAMETER(ftFileTime);

  return NULL;
}

BOOL MCFSSampleInternal::CloseFile( IRWFile* pIFile, bool bAbort )
{
  UNREFERENCED_PARAMETER(pIFile);
  UNREFERENCED_PARAMETER(bAbort);
  return FALSE;
}

DWORD MCFSSampleInternal::GetOptionalFunctions()
{
  return IVF_GETSIZE; // Support of Folder sizing.
}

BOOL MCFSSampleInternal::GetSize( const WCHAR* szPath , const WCHAR* szFilter , /*[out]*/ INT64& size , volatile bool* bAbort)
{
  UNREFERENCED_PARAMETER(bAbort);
  UNREFERENCED_PARAMETER(szFilter);

  szPath = RemoveDeviceFromPath(szPath);
  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szPath);

  // Ignore filter for now. Normal folder sizing does not set filters.

  // If you have a loop for recursive sizing and your FileSystem is slow then check if *bAbort == true if it is. then Abort sizing.
  // Set the size you calculated so far. but abort ASAP.

  if(pFile->IsFolder())
    size = pFile->CalculateFolderSize();
  else
    size = pFile->GetFileSize();

  return TRUE;
}

BOOL MCFSSampleInternal::ShowConfigDlg( DWORD& dwConfigValue , BOOL bReadConfig /*= TRUE */, BOOL* bSaveAsDefault /*= NULL */ )
{
  UNREFERENCED_PARAMETER(dwConfigValue);
  UNREFERENCED_PARAMETER(bReadConfig);
  UNREFERENCED_PARAMETER(bSaveAsDefault);
  return FALSE;
}

// dwFlags = ZAF_ flags.
ZHANDLE MCFSSampleInternal::BatchFileOperations_Init( IProgress* pProgress , DWORD dwFlags /*= 0*/, BatchMode mode/*= BatchModePut*/ )
{
  if(mode == BatchModeGet)
  {
    InternalReadFileOperations* pReader = new InternalReadFileOperations(pProgress, dwFlags);
    return (ZHANDLE)pReader;
  }
  else if (mode == BatchModePut)
  {
    InternalWriteFileOperations* pWriter = new InternalWriteFileOperations(pProgress, dwFlags);
    return (ZHANDLE)pWriter;
  }

  return 0;
}

BOOL MCFSSampleInternal::BatchFileOperations_QueueGetItem( ZHANDLE hIntOp , const WCHAR* szFilename , const WCHAR* szWriteName , DWORD dwFlags /*= 0*/, DWORD dwWriteFlags /*= 0*/, DWORD_PTR refID /*= 0 */ )
{
  UNREFERENCED_PARAMETER(szWriteName);
  UNREFERENCED_PARAMETER(dwWriteFlags);
  UNREFERENCED_PARAMETER(refID);

  ResetLastError();

  if(dwFlags & INTERNALOP_DELETE) // Add file (pItem) to our file system
  {
    szFilename = RemoveDeviceFromPath(szFilename);
    std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
    if(pFile == NULL)
    {
      m_nLastError = VERROR_FILE_NOT_FOUND;
      return FALSE;
    }

    InternalWriteFileOperations* pWriteOperation = (InternalWriteFileOperations*)hIntOp;
    if( pWriteOperation == NULL )
      return FALSE;

    pWriteOperation->AddDeleteItem(szFilename);
    return TRUE;
  }
  else if (dwFlags & INTERNALOP_READ)
  {
     InternalReadFileOperations* pReadOperation = (InternalReadFileOperations*)hIntOp;
     if( pReadOperation == NULL )
       return FALSE;
   
     szFilename = RemoveDeviceFromPath(szFilename);
     std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
     if(pFile == nullptr)
     {
       m_nLastError = VERROR_FILE_NOT_FOUND;
       return FALSE;
     }
     pReadOperation->Add(pFile, szWriteName);
     return TRUE;
  }

  return FALSE;
}

BOOL MCFSSampleInternal::BatchFileOperations_QueueInsertItem( ZHANDLE hIntOp , const IFileItem* pItem, const WCHAR* szWriteName , DWORD dwFlags /*= 0*/, DWORD dwWriteFlags /*= 0*/, DWORD_PTR /*refID*/ /*= 0 */ )
{
   UNREFERENCED_PARAMETER(dwWriteFlags);

  if(dwFlags & INTERNALOP_ADD) // Add file (pItem) to our filesystem
  {
    szWriteName = RemoveDeviceFromPath(szWriteName);
    std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szWriteName);
    if(pFile)
    {
      // ?
      m_nLastError = VERROR_ALREADY_EXISTS;
      return FALSE;
    }

    InternalWriteFileOperations* pWriteOperation = (InternalWriteFileOperations*)hIntOp;
    if( pWriteOperation == NULL )
      return FALSE;

    pWriteOperation->AddItem(pItem, szWriteName);
    return true;
    
  }
  return FALSE;
}

DWORD   MCFSSampleInternal::BatchFileOperations_QueueDeleteItem(ZHANDLE hIntOp, const WCHAR* szFilename, DWORD dwFlags /*= 0*/, DWORD_PTR refID /*= 0*/)
{
  UNREFERENCED_PARAMETER(hIntOp);
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(refID);
  return VERROR_VOLUME_OPERATION_NOTSUPPORTED;
}

// dwOptions  = VEQ_ flags
BOOL MCFSSampleInternal::BatchFileOperations_Start( ZHANDLE hIntOp , DWORD& dwOptions, MCNS::OverwriteOption& owOption, BatchMode mode )
{
  UNREFERENCED_PARAMETER(dwOptions);

  if( mode == BatchModePut)
  {
    InternalWriteFileOperations* pWriteOperation = (InternalWriteFileOperations*)hIntOp;
    if( pWriteOperation == NULL )
      return FALSE;

    pWriteOperation->StartFileOperations(&m_MemFS);

    return TRUE;
    
  }
  else if (mode == BatchModeGet)
  {
    InternalReadFileOperations* pReadOperations = (InternalReadFileOperations*)hIntOp;
    if( pReadOperations == NULL )
      return FALSE;

    DWORD dwErrors = pReadOperations->WriteQueuedItemsToDisk();
    if (dwErrors != ERROR_SUCCESS)
    {
      m_nLastError = dwErrors;
      // In real extension. you need to keep the error in a variable.
      // MC will query GetLastError(), GetLastError_InternalErrorCode()
      // if there was an error..
      return FALSE;
    }

    return TRUE;
  }
  return FALSE;
}

BOOL MCFSSampleInternal::BatchFileOperations_Release( ZHANDLE hIntOp )
{
  InternalFileOperation* pFileOperation = (InternalFileOperation*)hIntOp;
  if( pFileOperation )
    delete pFileOperation;
    
  return TRUE;
}

void MCFSSampleInternal::ResetLastError()
{
  m_nLastError = VERROR_NOERROR;
}