#include "stdafx.h"
#include "FSSampleStream.h"
#include "StreamRWFile.h"

using namespace MCNS;

// This GUID MUST be unique, in VS2005+ go to Tools->CreateGUID and select "Registry formt" and remove { } and - 
#pragma message(" ** If you create your own Plugin then change this GUID \n ** In visual studio go to Menu > Tools > Create GUID. And remove all ")
char MCFSSampleStream::m_GuidString[34] = "8997A0421F6A4048855B037D457C8D82";

MemoryFileSystem MCFSSampleStream::m_MemFS;

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
    if(_wcsnicmp(szPath, L"smp1:\\" , 6) == 0)
      szPath = szPath + 6;
    else if(_wcsnicmp(szPath, L"smp1:" , 5) == 0)
      szPath = szPath + 5;

    return szPath;
  }
}

bool MCFSSampleStream::GetExtensionInfo( DLLExtensionInfo* pInfo )
{
  if( pInfo == NULL )
    return false;

  wcsncpy( pInfo->wsName , L"FSSample-Stream" , 100 );
  wcsncpy( pInfo->wsPublisher , L"Mathias Svensson" , 100 );
  wcsncpy( pInfo->wsURL , L"http:\\\\www.multicommander.com" , 100 );
  wcsncpy( pInfo->wsDesc , L"Sample of a filesystem plugin using Stream Method." , 160 );
  wcsncpy( pInfo->wsBaseName , L"FSSampleStream" , 100 );
  strncpy( pInfo->strVersion , "1.1.0.0" , 10 );
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
  pInfo->dwDefaultFlags[0] = 0;
  pInfo->dwDefaultFlags[1] = 0;
  wcsncpy( pInfo->wsDefaultValues[0] , L"smp1" , 512 ); // smp1:
  wcsncpy( pInfo->wsDefaultValues[1] , L""     , 512 );

  return true;
}

MCFSSampleStream::MCFSSampleStream()
{

}

MCFSSampleStream::~MCFSSampleStream()
{
  int called = 0;
  ++called;
}

long MCFSSampleStream::PreStartInit( IMultiAppInterface* pInterface )
{
  // Register FileSystem handler, So this extension will get all "smp1:*" requests
  pInterface->RegisterFSURLType(L"smp1:*");

  // Add "SMP1:" as a virtual device and with Icon in the Device list. This will make it show up in DeviceCombo and Device Toolbar
  IFileSystemManager* pFSManager = (IFileSystemManager*)pInterface->QueryInterface( ZOBJ_FILESYSTEM , 0 );

  // Load icon med ID 347 from the MCIcons.dll - 347 is a Movies icon..  
  HICON hAppIconSmall = pInterface->LoadIcon_ToolbarSize( (HINSTANCE)-1, 357 , false);
  HICON hAppIconLarge = pInterface->LoadIcon_ToolbarSize( (HINSTANCE)-1, 357 , true );

  pFSManager->AddVirtualDevice( _T("SMP1 (Stream)"), L"SMP1:" , hAppIconSmall, hAppIconLarge, NULL, DEVF_FREESPACE_EMPTY);
  DestroyIcon(hAppIconSmall);
  DestroyIcon(hAppIconLarge);

  pInterface->ReleaseInterface((ZHANDLE)pFSManager, ZOBJ_FILESYSTEM);

  return 0;
}

bool MCFSSampleStream::GetIcon(HICON* /*pIcon*/, const WCHAR* /*fileExtension*/, MCIconSize::MCIconSize /*iconSize*/)
{
  return 0;
}

DWORD MCFSSampleStream::GetSupportedOperations()
{
  return VSO_BROWSE     // Support Browsing
    | VSO_DELETE        // Support Delete requests
    | VSO_DIRECT_DELETE // When doing delete.. allow direct delete method
    | VSO_RENAME        // Support Rename 
    | VSO_CREATENEW     // Supported adding a new files
    | VSO_MKDIR         // Support Creating of Folders
    | VSO_CHATTRIBUTE   // Support Change of attributes
    | VSO_NOUPDIR       // Will not return its own Updir folder.
    | VSO_STREAM_READ
    | VSO_STREAM_WRITE
    ;
}

bool MCFSSampleStream::IsBusy()
{
  // If the volume is a archive. then you might need to return true so that it does not open a archive that is being created
  return false;
}

BOOL MCFSSampleStream::CreateVolume( const WCHAR* szVolume, const WCHAR* szMountPath /*= NULL*/, eVCREATE_MODE nCreateMode /*= VCM_NEW*/, VolumePackOptions* pOptions /*= NULL*/, DWORD nItemHint /*= 0*/ )
{
  UNREFERENCED_PARAMETER(szVolume);
  UNREFERENCED_PARAMETER(szMountPath);
  UNREFERENCED_PARAMETER(nCreateMode);
  UNREFERENCED_PARAMETER(pOptions);
  UNREFERENCED_PARAMETER(nItemHint);
  
  // Create a new volume file if supported. eg a new .ZIP file. 
  return FALSE;
}

BOOL MCFSSampleStream::Open( const WCHAR* szVolume , const WCHAR* szMountPath /*= NULL */, eVOPEN_MODE nOpenMode /*= VOM_READ*/, DWORD dwFlags /*= 0*/ )
{
  UNREFERENCED_PARAMETER(dwFlags);

  if( szMountPath != NULL )
  {
    if(_wcsnicmp(szMountPath, L"smp1:" , 5) == 0)
      m_strMountPath = L"SMP1:";
  }
  else 
  {
    if(_wcsnicmp(szVolume, L"smp1:" , 5) == 0)
      m_strMountPath = L"SMP1:";
  }

  m_nOpenMode = nOpenMode;
  return TRUE;
}

// Reopen a volume from eg WRITE MODE TO READ MODE. But this volume support read/write without changing.
// But archive format like zip might need to handle it.
BOOL MCFSSampleStream::ReOpen( int /*nOpenMode*/ , int /*nOverrideMode*/ /*= 0*/, DWORD /*dwOpenFlags*/ /*= 0*/ )
{
  return TRUE;
}

BOOL MCFSSampleStream::Close()
{
  return TRUE;
}

BOOL MCFSSampleStream::isOpen()
{
  return FALSE;
}

// Check that the path is valid. Not to long, have valid chars and so on.
// This is NOT a check if the path items Exists. 
DWORD MCFSSampleStream::VerifyPath( const WCHAR* szPath )
{
  szPath = RemoveDeviceFromPath(szPath);
  if( wcslen( szPath ) > _MC_MAXPATH_)
    return VERROR_PATH_TO_LONG;

  return VERROR_NOERROR;
}

const WCHAR* MCFSSampleStream::GetMountPath()
{
  return m_strMountPath.c_str();
}

// for FileSystem Plugins that are Devices. the mountpath is always the same as volume path
const WCHAR* MCFSSampleStream::GetVolumePath()
{
  return m_strMountPath.c_str();
}

ZHANDLE MCFSSampleStream::InitFind( const WCHAR* szPath , const WCHAR* szPattern /*= NULL */, DWORD dwFlags /*= 0*/,  IVolumeFindCallback* pFindCallback /*= nullptr*/ )
{
  UNREFERENCED_PARAMETER(szPath);
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

// This FindNext is called if you folder scan is needed when doing delete/copy
// pItem CAN BE NULL - THen 
BOOL MCFSSampleStream::FindNext( ZHANDLE h , /*[out]*/ IFileItem *pItem /*= NULL */ )
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

BOOL MCFSSampleStream::FindNext( ZHANDLE h , /*[out]*/ WCHAR* szFileName , int nameLen, UINT64& dwAttributes )
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

DWORD MCFSSampleStream::SetFileData( ZHANDLE h , /*[out]*/ IFileItem *pItem , BOOL bUpdate /*= FALSE */ )
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

BOOL MCFSSampleStream::FindClose( ZHANDLE h )
{
  MemoryFileEnumerator* pEnum = (MemoryFileEnumerator*)h;
  if(pEnum)
    delete pEnum;

  return TRUE;
}

BOOL MCFSSampleStream::Exists( const WCHAR* szPath )
{
  szPath = RemoveDeviceFromPath(szPath);

  if(szPath[0] == '\0') // ROOT
    return TRUE;

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szPath);
  if(pFile)
    return TRUE;

  return FALSE;
}

BOOL MCFSSampleStream::Makedir( const WCHAR* szPath , const FILETIME* pFileTime /*= NULL*/, DWORD dwExecuteOptions /*= 0 /* EXO_ */ )
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

// Rename is also using this
BOOL MCFSSampleStream::MoveFile( const WCHAR* szExistingFile , const WCHAR* szNewFilename, DWORD dwExecuteOptions /* EXO_ */ )
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

BOOL MCFSSampleStream::DeleteFile( const WCHAR* szExistingFile , DWORD dwExecuteOptions /*= EXO_ISFILE*/ )
{
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  ResetLastError();
  szExistingFile = RemoveDeviceFromPath(szExistingFile);
  if(szExistingFile == NULL || szExistingFile[0] == '\0')
  {
    m_nLastError = VERROR_FILE_NOT_FOUND;
    return FALSE;
  }

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szExistingFile);
  bool bDeleted = false;
  if(pFile)
  {
    std::shared_ptr<MemoryFile> pFolder = m_MemFS.FindByPath(szExistingFile, true);
    if(pFolder)
    {
      bDeleted =pFolder->DeleteFileItem(pFile);
      if(bDeleted == false)
        m_nLastError = VERROR_FILE_NOT_FOUND;
    }
    else
      m_nLastError = VERROR_PATH_NOT_FOUND;
  }
  else
    m_nLastError = VERROR_FILE_NOT_FOUND;

  return bDeleted;
}

UINT64 MCFSSampleStream::GetAttributes( const WCHAR* szFilename )
{
   szFilename = RemoveDeviceFromPath(szFilename);
   std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
   if(pFile)
     return pFile->Attributes();

  return INVALID_FILE_ATTRIBUTES;
}

BOOL MCFSSampleStream::SetAttributes( const WCHAR* szFilename , UINT64 dwAttribute, DWORD dwExecuteOptions /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(dwExecuteOptions);
  ResetLastError();

  szFilename = RemoveDeviceFromPath(szFilename);
  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);

  if(pFile)
  {
    bool bIsFolder = pFile->IsFolder();
    if(bIsFolder)
      pFile->Attributes(dwAttribute | ZFF_FOLDER); // Makesure that FOLDER attribute is set
    else
      pFile->Attributes(dwAttribute);

    return TRUE;
  }

  m_nLastError = VERROR_FILE_NOT_FOUND;
  return FALSE;
}

BOOL MCFSSampleStream::SetFileTime( const WCHAR* szFilename , const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime, DWORD dwExecuteOptions /* EXO_ */ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(lpCreationTime);
  UNREFERENCED_PARAMETER(lpLastAccessTime);
  UNREFERENCED_PARAMETER(lpLastWriteTime);
  UNREFERENCED_PARAMETER(dwExecuteOptions);

  return FALSE;
}

BOOL MCFSSampleStream::GetFileInfo( const WCHAR* szFilename , ZFILEINFO& info)
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(info);

  return FALSE;
}

INT MCFSSampleStream::IsFolderEmpty( const WCHAR* szFolderPath )
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

DWORD MCFSSampleStream::GetLastError()
{
  return m_nLastError;
}

DWORD MCFSSampleStream::GetLastError_InternalErrorCode()
{
  return 0;
}

// Unpack files using internal method
DWORD MCFSSampleStream::ExtractFile( const WCHAR* szFilename , const WCHAR* szWritePath, DWORD dwFlags /*= 0 */ )
{
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(szWritePath);
  UNREFERENCED_PARAMETER(dwFlags);

  return 0;
}

IRWFile* MCFSSampleStream::CreateFile( const WCHAR* szFilename , DWORD dwAccessMode , DWORD dwCreateMode , UINT64 dwAttributes /*= 0 */, DWORD dwRWFlags /*= 0 */, const FILETIME* ftFileTime /*= NULL */, INT64 nFileSize /*= 0*/ )
{
  ResetLastError();
  szFilename = RemoveDeviceFromPath(szFilename);

  if(dwAccessMode & VA_WRITE)
  {
    return CreateFileWrite(szFilename, dwAccessMode, dwCreateMode, dwAttributes, dwRWFlags, ftFileTime, nFileSize);
  }
  else if( dwAccessMode & VA_READ)
  {
    return CreateFileRead(szFilename, dwAccessMode, dwCreateMode, dwAttributes, dwRWFlags, ftFileTime, nFileSize);
  }

  m_nLastError = VERROR_PATH_NOT_FOUND;
  return NULL;
}

IRWFile* MCFSSampleStream::CreateFileWrite( const WCHAR* szFilename , DWORD dwAccessMode , DWORD dwCreateMode , UINT64 dwAttributes /*= 0 */, DWORD dwRWFlags /*= 0 */, const FILETIME* ftFileTime /*= NULL */, INT64 nFileSize /*= 0*/ )
{
  UNREFERENCED_PARAMETER(dwAccessMode);

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
  if(pFile)
  {
    // file already exists.  only open existing file if dwCreateMode is VC_OPEN_EXISTING or VC_CREATE_ALWAYS
    // VC_CREATE_ALWAYS will truncate the file.
    if(dwCreateMode == VC_OPEN_EXISTING)
    {
      StreamRWFile* pRWFile = new StreamRWFile(pFile);
      return pRWFile;
    }
    else if(dwCreateMode == VC_CREATE_ALWAYS)
    {

    }

    m_nLastError = VERROR_ALREADY_EXISTS;
    return NULL;
  }


  if(dwCreateMode == VC_CREATE_NEW)
  {
     std::shared_ptr<MemoryFile> pFolder = m_MemFS.FindParentByPath(szFilename);
     if(!pFolder)
     {
       m_nLastError = VERROR_PATH_NOT_FOUND;
       return NULL;
     }

     pFile = pFolder->CreateNewFile(GetLastPathPart(szFilename));
     if(pFile)
     {
       StreamRWFile* pRWFile = new StreamRWFile(pFile);
       pRWFile->PrepareForWriting(dwAttributes, dwRWFlags, ftFileTime, nFileSize);
       return pRWFile;
     }
  }

  return NULL;
}

IRWFile* MCFSSampleStream::CreateFileRead( const WCHAR* szFilename , DWORD dwAccessMode , DWORD dwCreateMode , UINT64 dwAttributes /*= 0 */, DWORD dwRWFlags /*= 0 */, const FILETIME* ftFileTime /*= NULL */, INT64 nFileSize /*= 0*/ )
{
  UNREFERENCED_PARAMETER(dwAccessMode);
  UNREFERENCED_PARAMETER(dwCreateMode);
  UNREFERENCED_PARAMETER(dwAttributes);
  UNREFERENCED_PARAMETER(dwRWFlags);
  UNREFERENCED_PARAMETER(ftFileTime);
  UNREFERENCED_PARAMETER(nFileSize);

  std::shared_ptr<MemoryFile> pFile = m_MemFS.FindByPath(szFilename);
  if(pFile == NULL)
  {
    m_nLastError = VERROR_FILE_NOT_FOUND;
    return NULL;
  }

  StreamRWFile* pRWFile = new StreamRWFile(pFile);
  return pRWFile;
}

BOOL MCFSSampleStream::CloseFile( IRWFile *pIFile, bool /*bAbort*/ )
{
  ResetLastError();
  StreamRWFile* pRWFile = (StreamRWFile*)pIFile;
  if(pRWFile)
  {
    delete pRWFile;
    return TRUE;
  }
  return FALSE;
}

DWORD MCFSSampleStream::GetOptionalFunctions()
{
  return IVF_GETSIZE; // Support of Folder sizing.
}

BOOL MCFSSampleStream::GetSize( const WCHAR* szPath , const WCHAR* szFilter , /*[out]*/ INT64& size , volatile bool* /*bAbort */)
{
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

BOOL MCFSSampleStream::ShowConfigDlg( DWORD& dwConfigValue , BOOL bReadConfig /*= TRUE */, BOOL* bSaveAsDefault /*= NULL */ )
{
  UNREFERENCED_PARAMETER(dwConfigValue);
  UNREFERENCED_PARAMETER(bReadConfig);
  UNREFERENCED_PARAMETER(bSaveAsDefault);

  return FALSE;
}

ZHANDLE MCFSSampleStream::BatchFileOperations_Init( IProgress* pProgress , DWORD dwFlags /*= 0*/, BatchMode mode /*= BatchModePut*/ )
{
  UNREFERENCED_PARAMETER(pProgress);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(mode);

  return 0;
}

BOOL MCFSSampleStream::BatchFileOperations_QueueGetItem( ZHANDLE hIntOp , const WCHAR* szFilename , const WCHAR* szWriteName , DWORD dwFlags /*= 0*/, DWORD dwWriteFlags /*= 0*/, DWORD_PTR refID /*= 0 */ )
{
  UNREFERENCED_PARAMETER(hIntOp);
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(szWriteName);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(dwWriteFlags);
  UNREFERENCED_PARAMETER(refID);

  return FALSE;
}

BOOL MCFSSampleStream::BatchFileOperations_QueueInsertItem( ZHANDLE hIntOp , const IFileItem* pItem, const WCHAR* szWriteName , DWORD dwFlags /*= 0*/, DWORD dwWriteFlags /*= 0*/, DWORD_PTR refID /*= 0 */ )
{
  UNREFERENCED_PARAMETER(hIntOp);
  UNREFERENCED_PARAMETER(pItem);
  UNREFERENCED_PARAMETER(szWriteName);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(dwWriteFlags);
  UNREFERENCED_PARAMETER(refID);

  return FALSE;
}

DWORD MCFSSampleStream::BatchFileOperations_QueueDeleteItem(ZHANDLE hIntOp, const WCHAR* szFilename, DWORD dwFlags /*= 0*/, DWORD_PTR refID /*= 0*/)
{
  UNREFERENCED_PARAMETER(hIntOp);
  UNREFERENCED_PARAMETER(szFilename);
  UNREFERENCED_PARAMETER(dwFlags);
  UNREFERENCED_PARAMETER(refID);
  return VERROR_VOLUME_OPERATION_NOTSUPPORTED;
}

BOOL MCFSSampleStream::BatchFileOperations_Start( ZHANDLE hIntOp , DWORD& dwOptions, MCNS::OverwriteOption& owOption, BatchMode mode )
{
  UNREFERENCED_PARAMETER(hIntOp);
  UNREFERENCED_PARAMETER(dwOptions);
  UNREFERENCED_PARAMETER(mode);
  UNREFERENCED_PARAMETER(owOption);

  return FALSE;
}

BOOL MCFSSampleStream::BatchFileOperations_Release( ZHANDLE hIntOp )
{
  UNREFERENCED_PARAMETER(hIntOp);
  return FALSE;
}

void MCFSSampleStream::ResetLastError()
{
  m_nLastError = VERROR_NOERROR;
}
