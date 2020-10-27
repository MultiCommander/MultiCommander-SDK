#pragma once

#include "MemoryFileSystem.h"

// This is a sample of a file system plugin for Multi Commander that support the INTERNAL mode of reading/writing files.
// A FileSystem Plugin that support internal mode can NOT read/write file using bytes of data sent to it.
// When adding files to a FSPlugin supporting internal mode path to file on the local filesystem is sent to the plugin and 
// it will handle all of the adding of them INTERNALLY
// Example of Streaming FileSystems are Rar, 7Zip, 
// 
// Read FS-Sample-ReadMe.txt first.

MCNSBEGIN

class MCFSSampleInternal : public IVolume
{
public:
  static bool GetExtensionInfo( DLLExtensionInfo* pInfo );

  MCFSSampleInternal();
  ~MCFSSampleInternal();

  char* Get_ModuleID() override { return m_GuidString; }

  long PreStartInit(IMultiAppInterface* pInterface) override;

  bool  GetIcon(HICON* pIcon, const WCHAR* fileExtension, MCIconSize::MCIconSize iconSize) override;
  DWORD GetSupportedOperations() override;
  bool  IsBusy() override;
  BOOL CreateVolume(const WCHAR* szVolume, const WCHAR* szMountPath = NULL, eVCREATE_MODE nCreateMode = VCM_NEW, VolumePackOptions* pOptions = NULL, DWORD nItemHint = 0) override;

  BOOL Open(const WCHAR* szVolume , const WCHAR* szMountPath = NULL , eVOPEN_MODE nOpenMode = VOM_READ, DWORD dwFlags = 0) override;

  // nOverrideMode is to Reopen to nOpenMode even if CurrentMode is nOrverrideMode
  BOOL ReOpen(int nOpenMode , int nOverrideMode = 0, DWORD dwOpenFlags = 0) override;

  // Close volume. Unpack/Disconnect
  BOOL Close() override;

  BOOL isOpen() override;

  // Return VERROR_NOERROR if path is Okey, return VERROR_PATH_TO_LONG if it is too long. or some other error code if it is invalid
  DWORD VerifyPath(const WCHAR* szPath) override;

  // GetRootPath()  ( eg if open with  Open( "c:\myzipvolume.zip" ); then GetRootPath should return "c:\myzipvolume.zip\"
  // if open with c:\kalle\nisse  then root path is "c:\" 
  // if open with c:\myzipvolume.zip\source\crypt then root path is "c:\myzipvolume.zip\"
  const WCHAR* GetMountPath() override;  // Return the path to where this volume is mounted into the virtual file system
  const WCHAR* GetVolumePath() override; // return the real volume path

  // Enum Volume Content
  ZHANDLE InitFind( const WCHAR* strPath , const WCHAR* szPattern = NULL , DWORD dwFlags = 0, IVolumeFindCallback* pFindCallback = nullptr) override;
  BOOL FindNext( ZHANDLE h ,  /*[out]*/ IFileItem *pItem = NULL ) override ;
  BOOL FindNext( ZHANDLE h , /*[out]*/ WCHAR* szFileName , int nameLen, UINT64& dwAttributes ) override;
  //return 0xfffffff or (unsigned long)-1 if error.. else update flags
  // date that is set is LOCALTIME..
  DWORD SetFileData( ZHANDLE h , /*[out]*/ IFileItem *pItem , BOOL bUpdate = FALSE ) override;
  BOOL FindClose( ZHANDLE h ) override;
  //////////////////////////////////////////////////////////////////////////

  // check if file/path exists in this volume
  BOOL Exists( const WCHAR* szPath ) override;

  BOOL Makedir( const WCHAR* szPath , const FILETIME* pFileTime = NULL, DWORD dwExecuteOptions = 0 /* EXO_ */) override;
  BOOL MoveFile( const WCHAR* szExistingFile , const WCHAR* szNewFilename, DWORD dwExecuteOptions  /* EXO_ */) override;
  BOOL DeleteFile( const WCHAR* strExistingFile , DWORD dwExecuteOptions = EXO_ISFILE) override;

  UINT64 GetAttributes( const WCHAR* szFilename ) override; // return INVALID_FILE_ATTRIBUTES if strFilename does not exists on volume.
  BOOL  SetAttributes( const WCHAR* szFilename , UINT64 dwAttribute, DWORD dwExecuteOptions /* EXO_ */) override;

  BOOL  SetFileTime( const WCHAR* szFilename , const FILETIME *lpCreationTime, const FILETIME *lpLastAccessTime, const FILETIME *lpLastWriteTime, DWORD dwExecuteOptions /* EXO_ */ ) override;

  // Get file info
  BOOL  GetFileInfo( const WCHAR* szFilename , ZFILEINFO &info ) override;

  // 1 = YES , 0 = NO , -1 Dont Know, maybe ( -1 will mostly be threated as NO during delete then.)
  INT  IsFolderEmpty( const WCHAR* szFolderPath ) override;

  DWORD GetLastError() override;
  DWORD GetLastError_InternalErrorCode() override; // Use when GetLastError is VERROR_UNKNOWN, Then it will show an internal ErrorCode

  DWORD ExtractFile( const WCHAR* strFilename , const WCHAR* strWritePath, DWORD dwFlags = 0 ) override;  // VSO_NOMEMBUFFER 

  IRWFile* CreateFile( const WCHAR* strFilename , DWORD dwAccessMode , DWORD dwCreateMode , UINT64 dwAttributes = 0 , DWORD dwRWFlags = 0 , const FILETIME* ftFileTime = NULL , INT64 nFileSize = 0) override;
  BOOL     CloseFile(IRWFile *pIFile, bool bAbort) override;

  DWORD GetOptionalFunctions() override; // IVF_xxxxx

  // return size (in size parameter) of file or folder. if folder it should scan recursive and use filter
  // return TRUE if successful or FALSE if not
  // you should check if bAbort goes FALSE. if it does user have chosen to break this operation
  BOOL GetSize( const WCHAR* szPath , const WCHAR* szFilter , /*[out]*/ INT64& size , volatile bool *bAbort ) override;

  BOOL ShowConfigDlg( DWORD& dwConfigValue , BOOL bReadConfig = TRUE , BOOL* bSaveAsDefault = NULL ) override;

  ZHANDLE BatchFileOperations_Init( IProgress* pProgress , DWORD dwFlags = 0, BatchMode mode = BatchModePut) override;
  BOOL    BatchFileOperations_QueueGetItem( ZHANDLE hIntOp , const WCHAR* szFilename , const WCHAR* szWriteName , DWORD dwFlags = 0, DWORD dwWriteFlags = 0, DWORD_PTR refID = 0 ) override;
  BOOL    BatchFileOperations_QueueInsertItem( ZHANDLE hIntOp , const IFileItem* pItem, const WCHAR* szWriteName , DWORD dwFlags = 0, DWORD dwWriteFlags = 0, DWORD_PTR refID = 0 ) override;
  DWORD   BatchFileOperations_QueueDeleteItem( ZHANDLE hIntOp, const WCHAR* szFilename , DWORD dwFlags = 0, DWORD_PTR refID = 0) override;

  BOOL    BatchFileOperations_Start( ZHANDLE hIntOp , DWORD &dwOptions, MCNS::OverwriteOption& owOption, BatchMode mode) override;
  BOOL    BatchFileOperations_Release( ZHANDLE hIntOp ) override;

  static void MemFSClear()
  {
    m_MemFS.Clear();
  }
protected:
   void ResetLastError();

  static char  m_GuidString[34];
  std::wstring m_strMountPath;

  DWORD m_nLastError;

  // MCFSSampleInternal will be deleted when refcounter is 0. 
  // Since this "fake" FS is memory based. We do not want to loose it. so use a static
  // Just make sure that you clean it up in DllMain when DLL_PROCESS_DETACH is sent.
  static MemoryFileSystem m_MemFS;
};

MCNSEND

