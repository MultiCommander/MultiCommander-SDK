/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once

#include "IExternalFile.h"
#include "Collections.h"

#include "MCNamespace.h"
MCNSBEGIN

class IFileOperation;
class IRenameItems;

// flags for SetFolderDateTime(...)
#define FSM_SFDT_SYNC 0x00000010L // Wait for process to complete before returning

// DeviceType flags for AttachToolbar
#define DEVICETYPE_FLOPPY       0x00000001 //
#define DEVICETYPE_HD           0x00000002 // Fixed Hard drives.
#define DEVICETYPE_CDDVD        0x00000004 //
#define DEVICETYPE_REMOTE       0x00000008 //
#define DEVICETYPE_UNC          0x00000010 // path that are \\server\share
#define DEVICETYPE_REMOVABLEHD  0x00000020 // USB / FireWire
#define DEVICETYPE_MACHINE     (DEVICETYPE_FLOPPY | DEVICETYPE_HD | DEVICETYPE_CDDVD | DEVICETYPE_REMOTE)
#define DEVICETYPE_DESKTOP      0x00000100 // Desktop Folder
#define DEVICETYPE_MYDOCUMENTS  0x00000200 // My Documents
#define DEVICETYPE_SDOCUMENTS   0x00000400 // Shared documents
#define DEVICETYPE_DOWNLOADS    0x00000800 // Downloads - Only Windows Vista/2008/Win7 and newer
#define DEVICETYPE_PICTURES     0x00001000 // Pictures -  Only Windows Vista/2008/Win7 and newer
#define DEVICETYPE_VIDEO        0x00002000 // Video -  Only Windows Vista/2008/Win7 and newer
#define DEVICETYPE_MUSIC        0x00004000 // Music -  Only Windows Vista/2008/Win7 and newer
#define DEVICETYPE_ONEDRIVE     0x00010000
#define DEVICETYPE_DROPBOX      0x00020000
#define DEVICETYPE_GOOGLEDRIVE  0x00040000

//#define DEVICETYPE_GDRIVE       0x00004000 // Goggle Drive - Not supported yet
#define DEVICETYPE_VDEVICE      0x00100000 // Virtual Devices FTP:, REG:, NET:, ...
#define DEVICETYPE_REFRESH      0x10000000 // Special
#define DEVICETYPE_SPECIALFOLDERS          (DEVICETYPE_DESKTOP | DEVICETYPE_MYDOCUMENTS | DEVICETYPE_SDOCUMENTS | DEVICETYPE_DOWNLOADS | DEVICETYPE_PICTURES | DEVICETYPE_VIDEO | DEVICETYPE_MUSIC)
#define DEVICETYPE_SPECIALFOLDERS_COMMON   (DEVICETYPE_DESKTOP | DEVICETYPE_MYDOCUMENTS | DEVICETYPE_DOWNLOADS )
#define DEVICETYPE_SPECIALFOLDERS_EXTENDED (DEVICETYPE_SDOCUMENTS | DEVICETYPE_PICTURES | DEVICETYPE_VIDEO | DEVICETYPE_MUSIC)
#define DEVICETYPE_CLOUD                   (DEVICETYPE_ONEDRIVE|DEVICETYPE_DROPBOX|DEVICETYPE_GOOGLEDRIVE)
#define DEVICETYPE_ALL          0xFFFFFFFF

// Device Flags
#define DEVF_FREESPACE_EMPTY    0x0010
#define DEVF_FREESPACE_NOTAVAIL 0x0020

// Used in IFileSystemManager::RenameFiles (...)
class __declspec(novtable) IRenameResultCallback
{
public:
  virtual void OnResult(DWORD_PTR dwRef, DWORD dwResult) = 0;
};

/*
  Work with the internal virtual filesystem in MultiCommander
*/
class __declspec(novtable) IFileSystemManager
{
public:

  virtual void RefreshAllFileSystems() = 0;

  virtual ZHANDLE GetDeviceItemHandle( const WCHAR* strPath ) = 0;

  // strPath can be a real path or virtual that is supported as a filesystem plugin like FTP , ISO file. 
  // bUpdate if true. Update the value or get cached value..
  // nSize [OUT] will give Total Size of Volume
  // nFree [OUT] will give Free size on volume
  // strName[OUT] will give VolumeName, must be a preallocated 128 WCHAR array
  virtual bool GetVolumeInformation( const WCHAR* strPath , BOOL bUpdate , INT64* nSize , INT64* nFree , WCHAR* strName , DWORD* pdwDeviceFlags = NULL ) = 0;
  // hDeviceItem is a HANDLE for the internal deviceitem data
  virtual bool GetVolumeInformation( const ZHANDLE hDeviceItem ,  BOOL bUpdate , INT64* nSize , INT64* nFree , WCHAR* strName, DWORD* pdwDeviceFlags = NULL ) = 0;

  // Get The Device name
  virtual bool GetDeviceName( const ZHANDLE hDeviceItem , WCHAR* strName ) = 0;
  virtual bool GetDevicePath( const ZHANDLE hDeviceItem , WCHAR* strPath ) = 0;

  // Get what character the hDeviceItem is mounted to.. C:A: 0 9  return 0 if it does not have any
  virtual char GetMountChar( const ZHANDLE hDeviceItem ) = 0;
  virtual bool GetVirtualDeviceName(WCHAR* strDeviceName, DWORD nLen, const ZHANDLE hDeviceItem ) = 0;

  // Add UNC Device to virtual filesystem ( \\server\share\folder\file )
  virtual bool AddUNCDevice( const WCHAR* strPath ) = 0;

  // Add virtual device to the DeviceManagment system ( will show the device in the DeviceCombo or DeviceToolbar )
  // DO NOT use hIconSmall, hIconLarge here.pass 0 here and use AddVirtualDeviceIcons below to set icon for all 4 supported sizes
  virtual bool AddVirtualDevice( const WCHAR* displayName, const WCHAR* rootPath, HICON hIconSmall, HICON hIconLarge, const WCHAR* tooltip = NULL, DWORD dwDeviceFlags = 0 ) = 0;

  // Add Icon for virtual device
  virtual int   AddVirtualDeviceIcon(const WCHAR* szDevice, MCIconSize iconSize, HICON hIcon) = 0;
  virtual bool  AddVirtualDeviceIcons(const WCHAR* szDevice, HICON hIconSmall, HICON hIconMedium, HICON hIconLarge, HICON hIconXLarge) = 0;

  // Attach a Toolbar to the DeviceManager. it will keep a drive toolbar update. NO other buttons and other item my be added to the same toolbar.
  // hToolbar = handle to a empty toolbar
  // hCmd is the command that will be sent when use click on drives on this toolbar
  virtual bool AttachToolbar( const ZHANDLE hToolBar , const ZHANDLE hCmd , DWORD dwDeviceTypeFlags = 0, const WCHAR* szDeviceFilter = nullptr) = 0;
  virtual DWORD GetDeviceTypes(const ZHANDLE hToolbar) = 0;
  virtual DWORD GetDeviceTypes2(const MCNS::ZHANDLE hCommandBar) = 0;

  // if bShowDlg is FALSE the other values will be used. else it values from the dlg will be used
  virtual bool ChangeAttributeAndDateTime( IFileItemCollection* pItems , BOOL bShowDlg = TRUE , BOOL bResursive = FALSE , DWORD dwAttribSet = 0, DWORD dwAttribUnSet = 0 , FILETIME* pFTModify = NULL , FILETIME* pFTAccess = NULL , FILETIME* pFTCreate = NULL ) = 0;

  // Is Local Filesystem ( if local filesystem plugin is handling strPath , so even \\server\path is local)
  virtual BOOL IsLocalFileSystem( const WCHAR* strPath ) = 0;

  virtual BOOL IsDeviceVolume(const WCHAR* device) = 0;

  // bCreate == TRUE if item should be created if it is not found in cache
  virtual IFileItem* GetFileItemFromPath(const WCHAR* strPath , BOOL bCreate ) = 0; // BROKEN - bCreate do not always create.  use GetFileItemFromPath2(...)

  // Get Item from cache and if not there it is created. RefCount is added to FileItem so use ReleaseFileItem afterwards.
  // bOnlyRealFileItem - Only allow real file item to be created. No virtual 
  virtual IFileItem* GetFileItemFromPath2(const WCHAR* strPath, bool bOnlyRealFileItem) = 0;
  virtual bool       ReleaseFileItem(IFileItem* pFileItem) = 0;
  virtual bool       ReleaseFileItem(IFileItemCollection* pFileItems) = 0;
  ////////////////////////////////////////////////////////////////

  // item must be cached. but if the path comes from MultiCommander it is most likely cached.
  virtual bool GetRootVolume(WCHAR* szRootVolume, DWORD nLength, const WCHAR* szPath) = 0;

  // Get a IFileItem object for a path. and the Item should be a clone. Not the real cached object.
  // bVirtual == true if the file item does not have to exist on local filesystem. eg FTP/ZIP item
  virtual IFileItem* GetFileItemClone( const WCHAR* szPath , bool bVirtual = false) = 0;
  virtual IFileItem* GetFileItemClone( IFileItem* pFileItem) = 0;
  virtual bool       DeleteFileItemClone(IFileItem* pItem, bool bGC = true) = 0;
  ////////////////////////////////////////////////////////////////

  // pExternalFile = Connect the executed process to the monitor for this external file
  virtual BOOL SHExecute( const WCHAR* strFile , const WCHAR* strParameters , const WCHAR* strStartFolder , DWORD dwFlags , IExternalFile* pExternalFile = NULL) = 0;

   // View file using viewer specified in FileType Setup
  virtual BOOL ViewFile( const WCHAR* strFile , IKeyValCollection* pOptions ) = 0;
  virtual BOOL ViewFiles( IValueCollection* pFiles ) = 0;

  // Edit file using editor specified in FileType Setup
  virtual bool EditFile(const WCHAR* szFilename, DWORD dwFlags = 0) = 0;

  // Run file using launcher in filetype setup
  virtual bool RunFile(const WCHAR* szFilename, DWORD dwReserved = 0) = 0;

  // Get Icon Index for a icon in the Shared Cache of fileitem icons
  virtual int   GetIconIndex( const WCHAR* strFilename , bool bSmallIcon ) = 0;
  virtual int   GetIconIndex( const WCHAR* strFilename, MCIconSize size) = 0;

  virtual int   GetDefaultFolderIconIndex(MCIconSize iconSize);
  virtual int   GetDefaultFolderIconIndex(bool bSmallIcon) = 0;
  virtual int   GetDefaultFileIconIndex(bool bSmallIcon) = 0;

  // Get Icon from internal IconCache
  virtual HICON GetFileIcon( const WCHAR* strFilename, bool bSmallIcon) = 0;
  virtual HICON GetDeviceIcon( CHAR chDevice, bool bSmallIcon) = 0;
  virtual HICON GetVirtualDeviceIcon(const WCHAR* strDeviceName, bool bSmallIcon) = 0;

  virtual HICON GetFileIcon( const WCHAR* strFilename, MCIconSize size) = 0;
  virtual HICON GetDeviceIcon( CHAR chDevice, MCIconSize size) = 0;
  virtual HICON GetVirtualDeviceIcon(const WCHAR* strDeviceName, MCIconSize size) = 0;

  // Get a copy of the image list (cast a HIMAGELIST* to ZHANDLE, not defined here as HIMAGELIST because dose not 
  // want all extension to require to include commctrl.h)
  // Remember to destory the image list your self using ImageList_Destroy(...);
  virtual bool  GetIcons(ZHANDLE hImageList, MCIconSize size = MCIconSize::Small) = 0;

  // Get ALL subitems of pFileItem in a pCollection (Recursive)
  // pFileItem should be a CLONED item. so that not a real item is delete by accident.
  virtual int   GetSubItems( IFileItem* pFileItem , IFileItemCollection* pCollection ) = 0;

  // Get only the Child item under pFileItem. NOT Recursive.. )
  virtual int	  GetChildItems( IFileItem* pFileItem , IFileItemCollection* pCollection ) = 0;

  virtual int    GetFoldersForPath(const WCHAR* szPath, IValueCollection* pValues, bool bRefresh) = 0;

  //////////////////////////////////////////////////////////////////////////
  // Icon Extraction and update.

  virtual bool  AddItemForIconExtraction(IFileItem* pFileItem , DWORD nID, bool bGetSmallIcon) = 0;
  virtual bool  AddItemForIconExtraction(const WCHAR* strFilePath , DWORD nID, bool bGetSmallIcon) = 0;

  virtual bool  AddItemForIconExtraction(IFileItem* pFileItem , DWORD nID, MCIconSize iconSize) = 0;
  virtual bool  AddItemForIconExtraction(const WCHAR* strFilePath , DWORD nID, MCIconSize iconSize) = 0;

  // Start Icon fetching thread
  virtual void  SetIconHandlerOptions(DWORD dwOptions) = 0;
  virtual bool  StartIconFetching( IFileItem* pFileItem , bool bStart = true ) = 0;
  virtual bool  StartIconFetching( DWORD nID ) = 0;
  virtual bool  StopIconExtraction( bool bWait = true ) = 0;

  // hView will be notified and refreshed when icons for pParentItem are updated.
  virtual bool  RegisterForIconRefresh( ZHANDLE hView , DWORD_PTR nID , bool bAutoUnregister = false , bool bAllowMultiple = false ) = 0;
  virtual bool  UnRegisterForIconRefresh( ZHANDLE hView , DWORD_PTR nID ) = 0;

  virtual bool  RegisterForIconRefresh( ZHANDLE hView , IFileItem* pParentItem , bool bAutoUnregister = false ) = 0;
  virtual bool  UnRegisterForIconRefresh( ZHANDLE hView , IFileItem* pParentItem ) = 0;

  //////////////////////////////////////////////////////////////////////////
  virtual IExternalFile*  CreateExternalFile( const WCHAR* strFilename , DWORD dwFlags ) = 0;
  virtual bool            ExternalFile_Finished( IExternalFile* pExFile ) = 0;

  //////////////////////////////////////////////////////////////////////////
  //  IFileOperation
  virtual IFileOperation* CreateFileOperationItems(DWORD nType=0) = 0;
  virtual void DeleteFileOperationItems(IFileOperation* pItems) = 0;
  virtual bool StartFileOperation(IFileOperation* pItems) = 0;
  virtual bool StartFileOperationDirect(IFileOperation* pItems) = 0;


  virtual bool IsUsedInFileOperation(const WCHAR* szPath) = 0;

  virtual bool Disconnect(const WCHAR* szPath) = 0;

  virtual IRenameItems* CreateRenameItems() = 0;
  virtual void DeleteRenameItems(IRenameItems* pItems) = 0;

  virtual DWORD RenameItems(IRenameItems* pItems, IRenameResultCallback* pRenameResultCallback = NULL) = 0;
  virtual DWORD RenameItemsUndo(IRenameItems* pItems, IRenameResultCallback* pRenameResultCallback = NULL) = 0;

  virtual DWORD DeleteItems(IFileItemCollection* pFileItems) = 0;

  // If there are more item then nMaxItems under szPath that match the szPattern.
  // Then delete the oldest so that there only are nMaxItems left.
  // Eg DeleteOldestFiles( L"c:\\temp\\MyFolder\\", L"*.zip" , 10 );
  // return how many files got deleted
  virtual DWORD DeleteOldestFiles(const WCHAR* szPath, const WCHAR* szPattern, DWORD nMaxItems) = 0;

  virtual DWORD GetFilesSorted(IValueCollection* pItems, const WCHAR* szPath, const WCHAR* szPattern, bool bAscending, bool bFullPath) = 0;

  // Sync
  // Rename ( NOT MOVE.. newName can not contain any path or ..\ parts
  // If you want to rename AND move a file use MoveFile
  //
  // eg. Rename( "c:\\MyFiles\\MyFile.txt" , MyFile1.txt );
  virtual DWORD RenameFile(IFileItem* pFileItem, const WCHAR* strNewName) = 0;
  virtual DWORD RenameFile(const WCHAR* strOldName, const WCHAR* strNewName) = 0;

  // Sync
  virtual DWORD DeleteFile(const WCHAR* strFilePath) = 0;

  // Create a directory. for any supported volumes  like Local, FTP, 
  virtual bool CreateFolder(const WCHAR* szDirectoryPath, bool bRecursive) = 0;

  //////////////////////////////////////////////////////////////////////////
  // TODO - Move to another location , Maybe IMultiAppInterface
  virtual bool ShowFindProcessDlg(const WCHAR* szFind) = 0;
  virtual bool ShowWGetDownload() = 0;

  // dwFlags = FSM_SFDT_ flags
  virtual bool SetFolderDateTime(IFileItemCollection* pFolders, DWORD dwFlags, const WCHAR* szIgnore, const WCHAR* szIncludeOnly) = 0;

};

MCNSEND