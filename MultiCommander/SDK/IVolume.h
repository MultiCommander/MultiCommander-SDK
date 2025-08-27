/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2025 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * IVolume - Interface to inherit from when creating a FileSystem Plugin
 * 
 * 
 **/

#ifndef _ZIVOLUME_IF_
#define _ZIVOLUME_IF_

#pragma once

#include <WTypes.h>

#include "MultiApp_Def.h"

#include "IStatus.h"
#include "IFileItem.h"
#include "IScript.h"
#include "IFileOperations.h"

#include "MCErrorCodes.h"
#include "MCNamespace.h"

MCNSBEGIN

class IVolumeContextMenu;
class IVolumeCommandResult;

// CreateFile Flags
// Access Mode Flags
#define VA_WRITE    0x0200L
#define VA_READ     0x0400L
#define VA_ASADMIN  0x8000L  // Sent to CreateFile to indicate that user want to open this file as Admin via the IRunAsAdmin interface

// Create Modes
#define VC_CREATE_NEW          1  // If File Already Exists. Return NULL and set last error code to VERROR_ALREADY_EXISTS
#define VC_CREATE_ALWAYS       2  // Return valid handler, overwrite old file
#define VC_OPEN_EXISTING       3  // Open Existing file. if file does not exists return NULL and set error
#define VC_OPEN_ALWAYS         4  // Open existing file, if it does not exists Create it IF VA_WRITE was specified. else return error
#define VC_TRUNCATE_EXISTING   5

// Seek Flags
#define VSEEK_FILE_BEGIN    0
#define VSEEK_FILE_CURRENT  1
#define VSEEK_FILE_END      2

// dwExecuteOptions Flags in many of the call.
#define EXO_ISFILE               0x00000001 // Referenced file is a file   ( Only used by the DeleteItem(...) Api )
#define EXO_ISFOLDER             0x00000002 // Referenced file is a folder ( Only used by the DeleteItem(...) Api )
#define EXO_ISREPARSEPOINT       0x00000008 // Referenced file is a ReparsePoint. So file might need to be opended with that flag
#define EXO_EXTRALONGPATHALLOWED 0x00000100 // Allow extra long if supported. ( User have given OK for extra long path )
#define EXO_ASADMIN              0x80000000 // Run this command as administrator if possible

// Return result flags for Execute(...)
#define FSEXECUTE_REFRESHITEM             0x00000001L // The pFileItem has changed. refresh it.
#define FSEXECUTE_REFRESHITEMPATH         0x00000002L // Refresh the fileitem Path
#define FSEXECUTE_REDIRCTPATH             0x00000004L // Redirect to a new path suppled in PARAM
#define FSEXECUTE_REFRESHPARENT           0x00000010L // Refresh parent item
#define FSEXECUTE_INLINERENAME_ITEM       0x00000020L // item to set to inline renamed is in IVolumeCommandResult
#define FSEXECUTE_ASYNC_UITHREAD   0x00000100L // ASYNC Executing - Redirect to a secondary UI Thread and call Execute again
#define FSEXECUTE_ASYNC_WORKTHREAD 0x00000200L // ASYNC Executing - Redirect to a Workerthread and call Execute again

#define FSEXECUTE_SUCCESSFUL      0x00010000L // Execute was successful, do nothing else
#define FSEXECUTE_FAILED          0x00020000L // Execute was not able to processed.


#define FSDROP_REFRESHPATH    0x00000010L // Refresh current path

// GetOptionalFunctions() flags
#define IVF_GETSIZE          0x00000002  // Has GetSize function (or IVF_GETDEVICEITEMSIZE below)
#define IVF_VIRTUAL_GETICON  0x00000004  // Virtual Item do not need to be Real file items. it can be registry entries, or meta information or what ever.
// Then when showing it in the filebrowser, No icon will will be shown since it is not files. 
// So if file items are Virtual and This flag is set on the volume then the volume will be able to 
// provide a ICON.. both HICON and a Icon Matcher. 

#define IVF_EXECUTEFILE       0x00000008  // IVolume->Execute(..) supported
#define IVF_CONNECT           0x00000010  // Volume support the Connect()/DisConnect/IsConnected functions. (Eg FTP Volume)
#define IVF_ASSIGN2NUMDEVICE  0x00000020  // Auto Assign device to a Num Device ( 0: -> 9: ) (Require IVG_CONNECT to )
#define IVF_CMDLINE           0x00000040  // Volume support it own command line commands.
#define IVF_HASPACKCONFIGDLG  0x00010000  // if set then ShowPackOptionDlg(...) can be called 
#define IVF_HASSETTINGSUI     0x00100000  // Volume can show its own Settings UI     (Menu option for this will be auto added under Menu>Configuration>FileSystems)
#define IVF_GENERICSETTINGS   0x00200000  // Volume support the generic Settings UI. (Menu option for this will be auto added under Menu>Configuration>FileSystems)
#define IVF_STORAGEINFO       0x01000000  // Volume support FreeSpace information
#define IVF_DROPHANDLING      0x02000000  // Volume support RAW drop Handling  OnAcceptDrop(..), OnDrop(..)
#define IVF_GETDEVICEITEMSIZE 0x04000000  // Volume support GetDeviceItemSize(..) function
//===========================================================================================================
// Volume Supported Operations Flags

// Browse/List volume content
#define VSO_BROWSE        0x00000000 // Volume support browsing  // REMOVED (set to 0) ALL FileSystem MUST support browse so no need for a Flag for that

// Stream operations   ( ReadFile / WriteFile To/From a memory buffer, Like the standard Read/Write operations )
#define VSO_STREAM_READ       0x00000001 // Volume support stream read operations
#define VSO_STREAM_WRITE      0x00000002 // Volume support stream write operations

// Get/Put a file item into/from the volume. from/to local disk. (Can NOT be combined with VSO_BATCH_xxx )
#define VSO_GETFILE           0x00000004 // Volume support read operation of a single item to a local filepath
#define VSO_PUTFILE           0x00000008 // Volume support write operation of a single item from a item located on a local filepath

#define VSO_DIRECT_DELETE     0x00000010 // Volume can delete item directly.

// Handle operations in batches. (Archives (zip,rar,..) will use this so that the extraction/creating can be optimized )
// Batch uses Local filesystem as Target/Source locations.
#define VSO_BATCH_GETITEMS    0x00000020 // Volume needs to handle the Read operations
#define VSO_BATCH_INSERTITEMS 0x00000040 // Volume needs to handle the Write operations
#define VSO_BATCH_DELETEITEMS 0x00000080 // Volume needs to handle the Delete operations

// Modules can support both STREAM and INTERNAL.  Stream is used as default.
// If VSO_BATCH_TARGETLOCAL is set then INTERNAL will be used if multiple files to be read.
// This is good if the filesystem is very slow on seeking.
// However - THIS is Only valid if the other volume (target) is a stream FS like LocalFS 

// Modify content support
#define VSO_CREATENEW         0x00000100 // Volume supports Creation of new file item/add file to archive
// VSO_CREATENEWEX -- See below
#define VSO_DELETE            0x00000200 // Volume supports Delete operations. (VSO_DIRECT_DELETE or VSO_BATCH_DELETEITEMS must also be set)
#define VSO_MKDIR             0x00000400 // Volume supports Create Folder operations
#define VSO_RENAME            0x00000800 // Volume supports Rename operations
#define VSO_CHATTRIBUTE       0x00001000 // Volume supports Change Attribute operations
#define VSO_SETDATETIME       0x00002000 // Volume supports Changed date / time of item operations
#define VSO_TESTVOLUME        0x00004000 // Volume support Test. (eg for Zip, Rar volume. )
#define VSO_CREATEVOLUME      0x00008000 // Volume support Create new volumes. eg create new zip file

//extended options
#define VSO_NORCREATE         0x00010000 // No Recursive Create. 
// If we creating file c:\my.zip\myfolder\myfile.txt  
// Do not create the the myfolder directory first. Some archives (eg Zip) don't need entries for all the empty folders

#define VSO_EXTRACT_NOFOLDER  0x00020000 // When extracting to Localfile system. do not queue folders when extracting. (with BatchFileOperations_QueueGetItem(..)  
// since they will be created automatically by the volume
// (Require that VSO_BATCH_INSERTITEMS also is set )

#define VSO_CHK_OWRITE        0x00040000 // Volume support overwrite of item. If not then files that need to be overwriten will first be deleted then copied. (Eg Zip, Delete item from zip before adding the replacment)
#define VSO_NOUPDIR           0x00080000 // ".." folder is NOT returned from this volume. This flag makes the system add it by it self

#define VSO_NOMEMBUFFER       0x00100000 // This volume does only support read's operation to be written to file, not to a memory buffer 
// eg. if file c:\myrar.rar\myfile.txt is to be read the RAR-Volume must 
// extract the file to a file on the HD it self.. not to a buffer for MultiApp
// This flag also need VSO_BATCH_GETITEMS flags

#define VSO_MODULE_SAME_RW    0x00200000 // Do internal Handling when Source/Target of a file operation is same volume type.
// Eg Read from one Zip and Write to another zip. Then the read does not need to Unpack the data, but can read the raw zip data for that file entry
// and insert it into the target zip.

#define VSO_SLOW              0x00400000 // Volume is slow. 
// When doing copy/move operation the path will not be requested to be updated after every change.
// update will happen after file operation is completed.

#define VSO_BATCH_TARGETLOCAL 0x00800000 // Use Batch method for copy if target volume is Local filesystem, AND we got multiple items are in queue.
// Only valid if volume support both stream and batch mode. When unpacking multiple files it can be faster if they are done internally because
// the order of what file that should be unpacked can be determined by the volume it self. (So they can be unpack in the order they are in the archive)
// Else the order is determined by how the files are queued for copy

#define VSO_RUNASADMIN_SUPPORTED     0x01000000 // If a function fails with VERROR_ACCESS_DENIED or some of the special ACCESS_DENIED error codes.
// and this flag is set then the user can choose to retry using as administrator. and a second call will be run
// with one of the flag parameters for that function set to xxx_ASADMIN (flag name are different depending on function call)

// Not used yet - For future support
#define VSO_RUNASADMIN_USETEMPSTORAGE 0x02000000 // For internal operation. When unpacking. The unpacking should be done to a tmp folder where access is allowed,
// MultiCommander should then move the files to correct location as administrator by it self.

#define VSO_RUNASADMIN_INTERNAL_COPY  0x04000000 // If source and target are the same modules.. (eg both is REG: ) then use direct copy operation.
// BUT only if RetryAsAdmin has been requested.

#define VSO_IS_FILEARCHIVE 0x10000000 // Volume is a file archive. Stored file inside a file (Zip, rar, 7zip, and so on, virtual devices like FTP, WPD is NOT this )

#define VSO_CREATENEWCTX     0x20000000 // Volume supports Creation of new file item using CreateItemContext
// Not used yet - For future support
//#define VSO_READ_PASSWORD      0x10000000  // Volume support Password for reading (unpacking) items. (eg. password protected Zip, 7Zip archive for example)
//#define VSO_WRITE_PASSWORD     0x20000000  // Volume support Password for writing (packing) items. (eg. password protected Zip, 7Zip archive for example)

#define VSO_FREESPACE_NOTAVAIL 0x40000000  // Free space information is not available for this volume ( BUT if 
// Only valid if you have a Volume that is connected to a device like FTP:, REG: or somethings.
// 
//===========================================================================================================

// Volume overwrite support
#define VOW_APPEND   0x00000001L // Append to existing file
#define VOW_RENAME   0x00000002L // Rename existing file
#define VOW_RESUME   0x00000004L // Resume existing file

//===========================================================================================================

// OBOSLETE:.. MCIconSize used instead
// VFICON_
#define VFICON_SMALL    0x000000001
#define VFICON_LARGE    0x000000002
#define VFICON_MEDIUM   0x000000004
#define VFICON_XLARGE   0x000000008

// Volume Module open mode
enum eVOPEN_MODE    
{ 
  VOM_NOTOPEN = 0, 
  VOM_READ    = 1, 
  VOM_WRITE   = 2 , 
  VOM_BROWSE  = 3 
};

// Volume Open Flags
#define VOF_PARSECONTENT          0x00000001L // Content must be parsed when Open returns.
#define VOF_IGNOREBROWSERERRORS   0x00000002L // Ignore browse errors. This error was found when we browsed recently. do not bug users about it again

//
// Flag for CREATING a Volume. eg. creating a .ZIP
//
// VCM_NEW    = Create NEW
// VCM_ALWAYS = Create Always. If it exists. Overwrite It.
// VCM_EXISTING=Create New. If Exists append To it.
enum eVCREATE_MODE  { VCM_NEW = 1, VCM_ALWAYS = 2, VCM_EXISTING = 3 };

/*
VOM_READ  VOM_WRITE VOM_BROWSE   <- already in mode
VOM_READ    OK      S/V     OK
VOM_WRITE   S/V     S/V     OK
VOM_BROWSE    OK      OK      OK      S/V = Sharing violation error
^
'--  Requested open mode
*/


// Flags for for BatchFileOperations_Init
#define FILEOP_KEEPBROKEN    0x00000100L // Keep Broken Files ( eg If disk get full or user abort operations ) // as default it will NOT Keep broken files
#define FILEOP_ASK_KEEP      0x00000200L // Ask If User want to keep broken file.. // as default it will not ask
#define FILEOP_DELETEBROKEN  0x00000400L // Delete Broken files.. Dont Ask
#define FILEOP_DKEEP_ATTRIB  0x00001000L // Do not keep Original Attributes // As Default it will keep attributes
#define FILEOP_DKEEP_TIME    0x00002000L // Do not keep Original Date/Time  // As Default it will keep the date

// BatchFileOperations_QueueGetItem
// BatchFileOperations_QueueInsertItem Flags
#define INTERNALOP_ADD                  0x00001000L  // Add  Files to Volume   (Write to volume)
#define INTERNALOP_READ                 0x00002000L  // Read Files from Volume (Read from volume)
#define INTERNALOP_DELETE               0x00004000L  // Delete Files from Volume
#define INTERNALOP_MODIFY               0x00008000L  // Modify Files in Volume
#define INTERNALOP_IGNORE_BROWSE_ERROR  0x00010000L  // Ignore browse error.. do not raise any GUI.

// Dropdata Type
#define DROPDATATYPE_MOZ_URL  1
#define DROPDATATYPE_IE_URL   2

//Error Dialog Buttons to show

#define ED_NONE        0x00000000L

#define ED_OK          0x00000001L
#define ED_YES         0x00000002L
#define ED_NO          0x00000004L
#define ED_CANCEL      0x00000008L
#define ED_OKCANCEL  ED_OK | ED_CANCEL
#define ED_YESNO     ED_YES | ED_NO

#define ED_SKIP        0x00000010L
#define ED_RETRY       0x00000020L
#define ED_ABORT       0x00000040L
#define ED_OVERWRITE    0x00000080L // overwrite file

#define ED_RESUME       0x00000100L
#define ED_APPEND       0x00000200L
#define ED_RENAME       0x00000400L  // Not support for delete.
#define ED_CONTINUE     0x00000800L

#define ED_DELETE       0x00001000L
#define ED_DELETEALL    0x00002000L
#define ED_ADMINRETRY   0x00004000L
#define ED_RETRYSHARED  0x00008000L // Retry, Try open shared

#define ED_KEEP         0x00010000L
#define ED_KEEPBROKEN   0x00020000L
#define ED_KEEPALL      0x00040000L

#define ED_ALL          0x00100000L
#define ED_SKIPALL      0x00200000L
#define ED_OVERWRITE_IF 0x00400000L // overwrite with contidtion (check overwrite option for what condition)
#define ED_OWRITE_ALL   0x00800000L  // so not use - only for dialog
#define ED_ARENAME_ALL  0x01000000L // Auto rename all.. truncate name so it will fit. ( but keep Extension )

#define ED_IGNORE       0x04000000L
#define ED_IGNOREALL    0x08000000L

#define ED_DEFAULT      0x10000000L // The button with this flag will be set as the default button to be highlighted
#define ED_FINDPROCESS  0x40000000L // Show Find Process (Used for Sharing violation errors )

// tmp until fixed in ohter way - 
//#define ED_OW_IFLARGER 0x00040000L
#define ED_OW_IFSMALLER 0x20000000L
#define ED_OW_IFLARGER  0x80000000L

// return Code from error messages dialog
#define R_SKIP_FILE     100
#define R_DELETE_FILE   110
#define R_RETRY         120
#define R_ABORT         140
#define R_CANCEL        141
#define R_OK              1
#define R_ERROR          -1
//
// ExtractQueue Flags for the Options parameter
#define VEQ_ABORT       ED_ABORT
#define VEQ_SKIPALL     ED_SKIPALL     // If file already exists Skip All ( Do not show any GUI asking )
#define VEQ_OWRITE_ALL  ED_OWRITE_ALL  // If file already exists Overwrite All ( Do not show any GUI asking )
#define VEQ_OWRITE_OLD  ED_OWRITE_OLD  // If file Already exists Overwrite only if newer ( Do not show any GUI asking )
#define VEQ_OWRITE_ISD  ED_OWRITE_ISD  // if file already exists overwirte only if size differs
#define VEQ_KEEPALL     ED_KEEPALL
#define VEQ_DEL_BROKEN  ED_DELETEALL

// Also used as parameter for StartInternalOperation of the operation is a DELETE.
#define VINTDEL_DELBEFOREREPLACE 0x20000000L // Hint that this delete operation is Becuse of a delete before replace (Since the volume did not support replace)

// File Operation result
enum FileOperationResult : int
{
  FOR_NOTSET    = 0,
  FOR_OK        = 1,
  FOR_UNKNOWN   = 2,
  FOR_SKIPPED   = 10,
  FOR_SKIPPED_DELFORMOVE = 11, // Skipped the delete of source file for move operation
  FOR_OVERWRITE = 20,
  /*
  FOR_OVERWRITE_IF_NEWER = 21, // This are just the decicend for WHY it was overwritten.. 
  FOR_OVERWRITE_IF_OLDER = 22,
  FOR_OVERWRITE_IF_LARGER = 23,
  FOR_OVERWRITE_IF_SMALLER = 24,
  FOR_OVERWRITE_SIZE_DIFF = 25,
  */
  FOR_OVERWRITE_KEEPBOTH_RENAME_NEW = 25,
  FOR_OVERWRITE_KEEPBOTH_RENAME_OLD = 25,
  FOR_RENAMED   = 30,
  FOR_DELETED   = 35,
  FOR_ABORTED   = 40,
  FOR_FAILED    = 50, // See ResultError for error code
  FOR_TESTFAILED = 100, // Test Operation Failed
};

enum BatchMode
{
  BatchModeGet    = 1, // Read files from volume
  BatchModePut    = 2, // Write files into the volume
  BatchModeDelete = 3, // Delete files from volume
  BatchModeRename = 4, // Rename files in volume
};

struct ZFILEINFO
{
  UINT64    dwFileAttributes;
  FILETIME  ftCreationTime;
  FILETIME  ftLastAccessTime;
  FILETIME  ftLastWriteTime;
  INT64     fsFileSize;

  WCHAR     strFilePath[_MC_MAXPATH_];
};

class IFileInfo
{
public:
  virtual const WCHAR* FullPathName() const = 0;

  virtual const WCHAR* Name() const = 0;
  virtual const WCHAR* Path() const = 0;
  virtual const WCHAR* Extension() const = 0;

  virtual INT64  Size() const = 0;
  virtual INT64  Attributes() const = 0;
  virtual DWORD  Flags() const = 0;
  virtual const  FILETIME* FileTimeLastWrite() = 0;
  virtual const  FILETIME* FileTimeCreated() = 0;
  virtual const  FILETIME* FileTimeLastAccess() = 0;

  virtual const  FILETIME* GetDisplayFileTime() = 0;

  virtual void Release() = 0;
};

struct FileOperationErrorContext
{
public:
  ~FileOperationErrorContext()
  {
    if(pSourceFile)
    {
      pSourceFile->Release();
      pSourceFile = nullptr;
      
    }
    if(pTargetFile)
    {
      pTargetFile->Release();
      pTargetFile = nullptr;
    }
  }

  // DO NOT TO CALL pSourceFile->Release(); will be called in destructor
  MCNS::IFileInfo* pSourceFile = nullptr; 

  // DO NOT TO CALL pSourceFile->Release(); will be called in destructor
  MCNS::IFileInfo* pTargetFile = nullptr; 

  DWORD ErrorCode = VERROR_UNKNOWN_ERROR;

  // Replace this with MCNS::FileOpErrorResult 
  DWORD Response = ED_ABORT;
  MCNS::FileOpErrorResult ErrorResult = MCNS::FileOpErrorResult::NotProcessed;

  MCNS::OverwriteOption owOption = MCNS::OverwriteOption::Ask;

  // if error was 'file alredy exists' and user selected an overwrite option that require processing
  OverwriteResult ProcessedOvewriteResult = OverwriteResult::NotProcessed; // 0 do not overwrite, 1 overwrite. -1 no processing done
  bool AllowInternalOverwriteEvaluation = true; // If pSourceFile and pTargetFile is not complete set to false, if true file properties will be evaluated and ProcessedOverwriteResult will be set 

  bool AllowRename = false;
  bool AllowAppend = false;
  bool AllowResume = false;

  // Enables the "Retry Opening file shared" option - Applicable for VERROR_SHARING_VIOLATION error encountered while opening a file for reading.
  // This situation can occur if the file is already open in WRITE mode. (Note: This might not be ideal as the file could be currently written to.)
  bool AllowRetrySharedOpening = false; 
  bool Reserved1 = false;
  MCNS::FileOpErrorHint ErrorHint = MCNS::FileOpErrorHint::NoHint;
  DWORD_PTR RefId = 0;
};

// If IVolume Interface is supporting Stream Read/Write then this is returned from IVolume::CreateFile
class __declspec(novtable) IRWFile
{
public:
  virtual  ~IRWFile(){}
  virtual BOOL WriteFile( LPVOID lpBuffer , DWORD nBytesToWrite , LPDWORD lpBytesWritten ) = 0;

  // write cached data if any exists. this is called after all normal WriteFile(..) calls to let the file know that no more data should be written
  // so flush your caches. if no internal caches are used and extension does not need this. just return TRUE.
  virtual BOOL WriteFlush() = 0;

  // return TRUE on success. If there is no more data to read, Then return TRUE with lpBytesRead == 0
  virtual BOOL ReadFile( LPVOID lpBuffer , DWORD nBytesToRead , LPDWORD lpBytesRead ) = 0;

  // If Write or Read failed (returned false), The ErrorCode will be retrieved using this GetLastError
  virtual DWORD GetLastError() = 0;

  // Internal Error code is only used to show in Log and for user so that it is easier to report back issue to developer.
  virtual DWORD GetInternalError() = 0;

  virtual DWORD Seek( INT64 nPos , DWORD nMode ) = 0;

  // Return the filesize
  virtual INT64 GetFileSize() = 0;

  // Get or set the filetime (if supported )
  virtual BOOL SetFileTime( FILETIME ftCreate, FILETIME ftAccess , FILETIME ftWrite ) = 0;
  virtual BOOL GetFileTime( FILETIME &ftCreate, FILETIME &ftAccess , FILETIME &ftWrite ) = 0;

  // Not used ? - Not called automaticlly anymore. Call it your self from IVolume::CloseFile(...)
  virtual void PostClose() = 0; // is called after the file handle has been closed

};

class ITextValueCollection;
class IKeyValCollection;
class IMultiAppInterface;
class IFileItemCollection;

// QueryPackOptions / ShowPackerOptions flags
#define PACKOPTIONS_CREATEARCHIVE   0x0001  // Do not combine with PACKOPTIONS_EXTRACTARCHIVE
#define PACKOPTIONS_EXTRACTARCHIVE  0x0002  // Do not combine with PACKOPTIONS_CREATEARCHIVE
#define PACKOPTIONS_DEFAULTPROFILE  0x0010  // The Query operation is for default profile (recommended is to add "default" to the name, eg "Internal Zip (default)" )

struct VolumePackOptions
{
public:
  WCHAR strPackName[128];
  WCHAR strPackExt[10];

  // Key - OptionName
  // Val - OptionValue
  IKeyValCollection*  pOptions;

  DWORD_PTR           dwCustomParam;  
  DWORD               dwFlags;
  BOOL                bDefault;
};


class __declspec(novtable) IVolumeFindCallback
{
public:
  virtual      ~IVolumeFindCallback() = default;

  // Use this to get access to logging, show error dialog and so on. Not all interfaces are valid from file scanning. (Progress() will return nullptr)
  virtual bool AbortRequested() const = 0;
  virtual void ItemsFound(const size_t nItems) = 0; // set found items 
  virtual void ItemsFoundInc() = 0;           // increment found items with +1
  
};

class __declspec(novtable) IGetDeviceItemSizeCallback
{
public:
  virtual ~IGetDeviceItemSizeCallback() = default;

  virtual void Report(const UINT64 nSize) = 0;
  virtual void Report(const UINT64 nSize,  const UINT64 numFolders, const UINT64 numFiles) = 0;

  virtual bool AbortRequested() = 0;
};

#pragma warning( push )
#pragma warning( disable : 4100 ) // warning C4100: '<parameter>' : unreferenced formal parameter

struct CreateItemContext
{
  const wchar_t* szFilename = nullptr;
  DWORD dwAccessMode = 0;
  DWORD dwCreateMode = 0;
  UINT64 dwAttributes = 0;
  DWORD dwRWFlags = 0;
  const FILETIME* ftlastModified = nullptr;
  const FILETIME* ftLastAccess = nullptr;
  const FILETIME* ftCreate = nullptr;
  UINT64 FileSize = 0;
};

class __declspec(novtable) IVolume
{
public:

  IVolume()
  {
    m_nRef = 0;
    m_nVolumeID = 0;
    m_nOpenMode = VOM_BROWSE;
  }

  //////////////////////////////////////////////////////////////////////////
  // Do not override this.
  inline DWORD AddRef()      
  { 
     InterlockedIncrement(&m_nRef);	
    return m_nRef; 
  }
  inline DWORD Release()     
  { 
    InterlockedDecrement(&m_nRef);
    return m_nRef; 
  }
  inline DWORD GetRefCount() 
  { 
    return m_nRef;
  }

  void  SetApplicationConnection(IAppConnection* pAppConnection) { m_pAppConnection = pAppConnection; }
  
  short GetVolumeID() const { return m_nVolumeID; }
  void  SetVolumeID( short nVolumeID ) { m_nVolumeID = nVolumeID; }

  int   GetOpenMode() const { return m_nOpenMode; }

  //////////////////////////////////////////////////////////////////////////
  // Override this and return GUID
  virtual char* Get_ModuleID() = 0;

  ///////////////////[ Override the functions that you supported depending of VSO_ flags ]////////////////////////////

  // Called once during startup. Load your config and store that in a singleton. since this instance will be deleted after the call
  virtual long PreStartInit(IMultiAppInterface* /*pInterface*/) { return 0; }

  // Only use PostStartInit if you need to init things from the Main UI thread.. else use PreStartInit
  virtual long PostStartInit(IMultiAppInterface* /*pInterface*/) { return 0; }

  virtual DWORD DataBuffertWriteAlignment(const WCHAR* szPath = nullptr) { return 0; }

  virtual bool ShowPackOptionDlg(DWORD /*dwFlags*/, VolumePackOptions* /*pOptions*/, IMultiAppInterface* /*pInterface*/) { return false; }
  virtual bool QueryPackOptions(DWORD /*nIdx*/, DWORD /*dwFlags*/, VolumePackOptions* /*pOptions*/, IMultiAppInterface* /*pInterface*/) { return false; }
  virtual bool SetPackOptions(VolumePackOptions* /*pPackOptions*/) { return false; };

  // if IVF_VIRTUAL_GETICON then this function will be called when icons are fetch
  // str NEED to be a ID string that can NOT be a filename
  // Recommended is "?{<guid>}-<type>:" since this a invalid as a filename it will not be any mixups in the icon cache
  virtual BOOL GetIconIdentifier(WCHAR* /*str*/, DWORD /*nLen*/, IFileItem* /*pFileitem*/) { return FALSE; }

  // Override if your volume need to support connect/disconnect (eg. FTP ) IVF_CONNECT
  virtual BOOL IsConnected(const WCHAR* /*path*/) { return FALSE; }
  virtual BOOL Connect(const WCHAR* /*path*/, OUT WCHAR* /*szRemotePath*/, DWORD /*len*/)    { return FALSE; }
  virtual BOOL Disconnect(const WCHAR* /*path*/)  { return FALSE; }

  // Override if you want to be able to Assign a Path in your volume to a num device (eg. "5:\pub\sys" ) 
  // ( flag IVF_ASSIGN2NUMDEVICE is required for this to be called )
  // 
  // path = Full path
  // szInternalDevicePath = OUT parameter on what the internal path for the num device
  // szDevicePath         = OUT parameter on the path for you volume.
  // len                  = IN  parameter of the len of the string for szInternalDevicePath/szDevicePath, use same size strings.
  //
  // Example
  // path = FTP:\\mysite\pub\sys   
  // szInternalDevicePath = FTP:\\Mysite
  // szDevicePath = \pub\sys
  // szDisplayName = "MySite (FTP)"
  // pIcon = Icon for the item
  virtual BOOL GetAssign2NumDevicePaths(const WCHAR* /*path*/, WCHAR* OUT /*szInternalDevicePath*/, WCHAR* OUT /*szDevicePath*/, WCHAR* OUT /*szDisplayName*/,  DWORD /*nLen*/, HICON* OUT /*pIcon*/, OUT DWORD& dwExtraDeviceFlagInfo) { return FALSE; }

  ///////////////////[ Override this functions if the IVolume is of type EXT_VDEVICEFS ]////////////////////////////

  // if IVF_EXECUTEFILE then overload this. and return TRUE if handled, if FALSE is returned 
  // MC will continue to try to execute item
  // dwResult is a out parameter that can have FSEXECUTE_XXXXX flags
  // szNewPath is valid if FSEXECUTE_REDIRECTPATH is returned in dwResult. ( szNewPath has a max size of 2048 )
  // This function is ALSO called if the fileitem has ZFF_VOLCOMMAND attribute.
  virtual BOOL Execute(IMultiAppInterface* /*pInterface*/, const WCHAR* /*path*/, IFileItem* /*pFileItem*/, DWORD* /*pdwResult*/, OUT WCHAR* /*szNewPath*/) { return FALSE; }

  // NOTE! Remove OnContextMenu/OnContextCommand ? Do not like them. bad solution.
  // When context menu is about to be shown. Since the volume is a EXT_VDEVICEFS it is not a REAL files so none
  // of the normal items will be there, with OnContextMenu the volume can add it own items.
  virtual BOOL OnContextMenu(IVolumeContextMenu* /*pContextmenu*/, IFileItemCollection* pFileItems) { return FALSE; }
  virtual BOOL OnContextCommand(IMultiAppInterface* /*pInterface*/, DWORD /*nCommand*/, IFileItemCollection* /*pFileItems*/, IVolumeCommandResult* /*pResult*/) { return FALSE; }

  // Requested to set the file items in pFileitems thats belonging to your volume into clipboard
  virtual BOOL SetClipboardData(IFileItemCollection* /*pFileItems*/)    { return FALSE; }
  virtual BOOL GetClipboardData(IFileItem* /*pParent*/, DWORD* /*pResult*/)  { return FALSE; }

  // return TRUE if command line was handled. Else it will be handled by the default command line process
  // Only called if IVF_CMDLINE in GetOptionalFunctions() is set
  virtual BOOL ExecuteCommandLine(const WCHAR* /*szCommand*/, const WCHAR* /*szParameters*/, const WCHAR* /*szWorkPath*/){ return FALSE; }


  ///////////////////[ Override following functions - REQUIRED ]////////////////////////////

  // Return an icon in pIcon for the file extension ( one that is associated with this Volume Interface )
  // This is optional. return false and system will use default. ( Only called if window shell does not return any icon )
  // Destroy icon will be called on pIcon if GetIcon(..) returns true. when the icon has been copied and added to the internal cache
  // dwFlags = VFICON_xxxx - Icon size
  virtual bool  GetIcon(HICON* pIcon, const WCHAR* fileExtension, MCIconSize iconSize) = 0;

  // Return VSO_ flags
  virtual DWORD GetSupportedOperations() = 0;

  //If the volume is busy. return true.
  virtual bool  IsBusy() = 0;

  // DO NOT keep a copy of the pOptions pointer. it will be invalid after the function return.
  // extract the options you need from it 
  virtual BOOL CreateVolume(const WCHAR* szVolume, const WCHAR* szMountPath = nullptr, eVCREATE_MODE nCreateMode = VCM_NEW, VolumePackOptions* pOptions = nullptr, DWORD nItemHint = 0) = 0;

  // Test a volume of this type. eg a Zip file ?
  // use the IProgress to return the progress of the test
  // dwResult is the result of the test  VERROR_FILE_CORRUPT , or other VERROR code
  // return : TRUE if test been preformed. FALSE of not.. (If test Fails return TRUE. becuse a TEST has been done. )
  virtual BOOL TestVolume(const WCHAR* /*szVolume*/, IFileOperationExtensionContext* /*pContext*/, DWORD& /*dwResult*/) { return FALSE; }

  /*
  BOOL Open(const WCHAR* szVolume , const WCHAR* szMountPath = NULL , eVOPEN_MODE nOpenMode = VOM_READ, DWORD dwFlags = 0)

  Open Volume and connect to data source and parse the content

  Example of paths with has redirected to numdevice

  szVolume     : c:\kalle\myfile.zip   ftp:\mysite\rootfolder\subfolder\      reg:    c:\myrar.rar
  szMountPath  : c:\kalle\myfile.zip   3:\                                    reg:    c:\myrar.rar
                                  ^--  3:\rootfolder should be handled as ftp:\mysite\rootfolder

  szVolume    - What  file or virtual path that should be opened.
  szMountPath - Under what path the volume has been 'mounted'.

  The volume needs to remember this, all paths that are sent to the volume is the virtual path. So the volume need to convert them to internal paths.

  dwFlags = Volume Open Flags.  VOF_PARSECONTENT (Must parse content if not parsed, else content might be parsed when data is required)
  */
  virtual BOOL Open(const WCHAR* szVolume , const WCHAR* szMountPath = nullptr, eVOPEN_MODE nOpenMode = VOM_READ, DWORD dwFlags = 0) = 0;

  // nOverrideMode is to Reopen to nOpenMode even if CurrentMode is nOrverrideMode
  virtual BOOL ReOpen(int nOpenMode , int nOverrideMode = 0, DWORD dwOpenFlags = 0) = 0;

  // Close volume. Unpack/Disconnect
  virtual BOOL Close() = 0;

  // Needs a call to open with VOF_PARSECONTENT
  virtual bool NeedParseContent() = 0;

  // Not Used.. I Think... Remove this
  virtual BOOL isOpen() = 0;

  // Return VERROR_NOERROR if path is Okey, return VERROR_PATH_TO_LONG if it is too long. or some other error code
  // if it is invalid
  virtual DWORD VerifyPath(const WCHAR* szPath) = 0;

  // GetRootPath()  ( eg if open with  Open( "c:\myzipvolume.zip" ); then GetRootPath should return "c:\myzipvolume.zip\"
  // if open with c:\kalle\nisse  then root path is "c:\" 
  // if open with c:\myzipvolume.zip\source\crypt then root path is "c:\myzipvolume.zip\"

  virtual const WCHAR* GetMountPath() = 0;  // Return the path to where this volume is mounted into the virtual file system
  virtual const WCHAR* GetVolumePath() = 0; // return the real volume path

  // Initialize a scan of the filesystem
  // if pattern is null *.* is assumed
  // dwFlags - are the FileItem flags of the root item. Can be used as a hint if scanning should be done in some special why.
  // pbAbort and pCount are optional.
  // But if a IVolume can be slow when parsing the data source they should be used.
  // Example. If we create a volume that will expose the archive of type .XYZ and when InitFind is called,
  // it needs to open that and parse the entire archive, That can take time. Then pState and pCount should be used and handled. 
  // pbAbort - if it is 3 then the parsing should be aborted.
  //           but a valid ZHANDLE should be returned so that the items that is did parse can be shown. (if possible)
  // pCount - is number of items founds. It do not need to be the same as the no of item that will be shown when finished.
  //          it is used to inform the user that something is happening. And that we are working
  virtual ZHANDLE InitFind( const WCHAR* szPath , const WCHAR* szPattern = nullptr, DWORD dwFlags = 0, IVolumeFindCallback* pFindCallback = nullptr) = 0;

  // Find Next Item in Scan
  // Set pItem with filedata the same way that is done in SetFileData.
  // BUT only if pItem is valid. IT CAN BE NULL so do a NULL check. return TRUE if there is a new item. (even if pItem is NULL)
  virtual BOOL FindNext( ZHANDLE h ,  /*[out]*/ IFileItem *pItem = nullptr) = 0;

  // dwAttribute only needs basic attributes like ZFF_HIDDEN , ZFF_SYSTEM
  // used to skip Hidden,system item
  virtual BOOL FindNext( ZHANDLE h , /*[out]*/ WCHAR* szFileName , int nameLen, UINT64& dwAttributes ) = 0;

  // if bUpdate is TRUE then return Update flags ZFU_ 
  virtual DWORD SetFileData( ZHANDLE h , /*[out]*/ IFileItem *pItem , BOOL bUpdate = FALSE ) = 0;
  // Close Scan
  virtual BOOL FindClose( ZHANDLE h ) = 0;

  // check if file/path exists in this volume
  virtual BOOL Exists( const WCHAR* szPath ) = 0;

  virtual BOOL Makedir( const WCHAR* szPath , const FILETIME* pFileTime = nullptr, DWORD dwExecuteOptions = 0 /* EXO_ */) = 0;

  // Move/Rename a file/folder on current volume
  virtual BOOL MoveItem( const WCHAR* szExistingFile , const WCHAR* szNewFilename, DWORD dwExecuteOptions  /* EXO_ */) = 0;

  // call if VSO_INTERNAL_COPY or VSO_RUNASADMIN_INTERNAL_COPY is set
  virtual BOOL CopyItem( const WCHAR* szExistingName, const WCHAR* szTargetName, DWORD dwOptions, DWORD dwExecuteOptions = 0) { return FALSE; }

  // Delete file
  // set bFile to FALSE to strExistingFile is a folder
  virtual BOOL DeleteItem( const WCHAR* szExistingFile , DWORD dwExecuteOptions = EXO_ISFILE) = 0;

  // Get/Set file attribute
  // return INVALID_FILE_ATTRIBUTES if szFilename does not exists on volume.
  virtual UINT64 GetAttributes( const WCHAR *szFilename ) = 0;
  virtual BOOL  SetAttributes( const WCHAR *szFilename , UINT64 dwAttribute, DWORD dwExecuteOptions /* EXO_ */) = 0;

  virtual BOOL  SetFileTime( const WCHAR* szFilename , const FILETIME *lpCreationTime, const FILETIME *lpLastAccessTime, const FILETIME *lpLastWriteTime, DWORD dwExecuteOptions /* EXO_ */ ) = 0;

  // Get file info
  virtual BOOL  GetFileInfo( const WCHAR *szFilename , ZFILEINFO &info ) = 0;

  // 1 = YES , 0 = NO , -1 Dont Know ( will mostly be threated as NO during delete then.)
  virtual INT  IsFolderEmpty( const WCHAR* szFolderPath ) = 0;

  virtual DWORD GetLastError() = 0;
  virtual DWORD GetLastError_InternalErrorCode() = 0; // Use to show user an errorcode if the GetLastError is an Unkown error.

  // if VSO_NOMEMBUFFER then when extracting file this function will be called instead.
  // and all internal stuff for writing will be done with the same volume
  virtual DWORD ExtractFile( const WCHAR* szFilename , const WCHAR* szWritePath, DWORD dwFlags = 0 ) = 0;

  /*
  dwAccessMode : VA_WRITE | VA_READ
  dwCreateMode : VC_CREATE_NEW , VC_CREATE_ALWAYS , VC_OPEN_EXISTING , VC_OPEN_ALWAYS , VC_TRUNCATE_EXISTING
  */
  virtual IRWFile* CreateItem( const WCHAR* szrFilename, DWORD dwAccessMode, DWORD dwCreateMode, UINT64 dwAttributes = 0, DWORD dwRWFlags = 0, const FILETIME* ftFileTime = nullptr, INT64 nFileSize = 0) = 0;
  virtual IRWFile* CreateItemEx(CreateItemContext& createItemContext) { return nullptr; }

  // bAbort is TRUE CloseFile is because of a UserRequested abort
  virtual BOOL   CloseItem(IRWFile *pIFile, bool bAbort) = 0;

  // Is called on Copy/Move if source / target is the same volume id. this is to better be able to prepare the write
  // operation. by checking local data IRWFile.  both pSource and pTarget is create by the same kind of IVolume. 
  // but might not be created by the same instance.
  // Example FSReg is using it when coping registry keys/values to other places in the registry
  virtual BOOL PrepareReadWrite(IRWFile* /*pSource*/, IRWFile* /*pTarget*/) { return TRUE; }

  // Override if VSO_READ_PASSWORD/VSO_WRITE_PASSWORD option is supported.
  virtual BOOL SetPassword(const WCHAR* /*szPassword*/, DWORD /*dwReserved*/ ) { return TRUE; }

  //// OPTIONAL !!
  // Optional Function MUST exists but if you don't want to support them used make them 
  // empty and return FALSE. and also make GetOptionalFucntions() return the right values so program know 
  // if you have them or not..

  // return IVF_<text> to check if Volume support optional function.
  virtual DWORD GetOptionalFunctions() = 0;

  // return size (in size parameter) of file or folder. if folder it should scan recursive and use filter
  // return TRUE if successful or FALSE if not
  // you should check if bAbort goes FALSE. if it does user have chosen to break this operation
  virtual BOOL GetSize( const WCHAR* szPath , const WCHAR* szFilter , /*[out]*/ INT64& size, bool followLinks, volatile bool *bAbort ) = 0;

  // call if device support IVF_GETDEVICEITEMSIZE
  virtual BOOL GetSizeEx(const WCHAR* szPath, const WCHAR* szFilter, bool followLinks, IGetDeviceItemSizeCallback* pCallback) { return false; }

  virtual BOOL ShowConfigDlg(MCNS::IMultiAppInterface* pInterface) = 0;

  ///========================================================================================================
  // Internal Batch file operations methods. 
  // They are called if volume module support VSO_BATCH_GETITEMS , VSO_BATCH_INSERTITEMS or VSO_BATCH_DELETEITEMS
  //
  // Return (ZHANDLE)-1 if InitInternalOperations fails.
  // use RefID to set result status 
  // dwFlags is FILEOP_ flags. That contains some user settings on how copy/move/delete should work
  // nBatchMode = 
  virtual ZHANDLE BatchFileOperations_Init(IProgress* /*pProgress*/, DWORD /*dwFlags*/ = 0, BatchMode /*mode*/ = BatchModePut) {  return (ZHANDLE)0; }
  virtual ZHANDLE BatchFileOperations_Init2(IFileOperationExtensionContext* /*pFOEContext*/, DWORD /*dwFlags*/ = 0, BatchMode /*mode*/ = BatchModePut) { return (ZHANDLE)0; }

  // dwFlags = INTERNALOP_ flags - Hints in why the item is queue If it is for a Add/Delete/Extract reason.
  // dwWriteFlags - Write flags to use according to read/write strategy, Must not be used. only recommended flag. 
  // refID - Used by 7Zip and RAR - Need doc on why and how it is used. 
  virtual BOOL    BatchFileOperations_QueueGetItem(ZHANDLE /*hIntOp*/   , const WCHAR* /*szFilename*/, const WCHAR* /*szWriteName*/, DWORD /*dwFlags*/ = 0, DWORD /*dwWriteFlags*/ = 0, DWORD_PTR /*refID*/ = 0) {  return FALSE; }
  virtual BOOL    BatchFileOperations_QueueInsertItem(ZHANDLE /*hIntOp*/, const IFileItem* /*pItem*/ , const WCHAR* /*szWriteName*/, DWORD /*dwFlags*/ = 0, DWORD /*dwWriteFlags*/ = 0, DWORD_PTR /*refID*/ = 0) {  return FALSE; }
  virtual DWORD   BatchFileOperations_QueueDeleteItem(ZHANDLE /*hIntOp*/, const WCHAR* szFilename , DWORD dwFlags = 0, DWORD_PTR refID = 0 )  { return VERROR_VOLUME_OPERATION_NOTSUPPORTED; }

  // dwOPtions = VEQ_ flags
  virtual BOOL    BatchFileOperations_Start(ZHANDLE /*hIntOp*/, DWORD &dwOptions, MCNS::OverwriteOption& owOption, BatchMode mode) { return FALSE; }
  virtual BOOL    BatchFileOperations_Release(ZHANDLE /*hIntOp*/) { return FALSE; }
  ///========================================================================================================

  // Alternative new way to insert/get one file at a time from an volume.
  // Require that flag VSO_GETFILE / VSO_PUTFILE, (Eg used by FSPortable )
  virtual DWORD   PutFile(const WCHAR* szLocalFile, const WCHAR* szTargetPath, DWORD dwFlags, IProgress* pProgress) { return VERROR_NOTIMPLEMENTED; }
  virtual DWORD   GetFile(const WCHAR* szLocalFile, const WCHAR* szSourceFile, DWORD dwFlags, IProgress* pProgress) { return VERROR_NOTIMPLEMENTED; }

  virtual DWORD   PutFileEx(const WCHAR* szLocalFile, const WCHAR* szTargetPath, DWORD dwFlags, IFileOperationExtensionContext* /*pFOEContext*/) { return VERROR_NOTIMPLEMENTED; }
  virtual DWORD   GetFileEx(const WCHAR* szLocalFile, const WCHAR* szSourceFile, DWORD dwFlags, IFileOperationExtensionContext* /*pFOEContext*/) { return VERROR_NOTIMPLEMENTED; }

  // When the user select to configure settings for Volume type. It is up to the Volume to show a configuration UI.
  // Only called if volume has flag
  virtual BOOL OnConfigureSettings(IMultiAppInterface* /*pInterface*/)  { return FALSE; }

  // If using the generic settings view for configuring settings. This is called when the user apply new settings
  virtual BOOL OnSettingsChanged(IMultiAppInterface* /*pInterface*/) { return FALSE; }

  // If the voume needs a password.
  // Eg.
  // Will be called if CreateFile return nullptr and error code is VERROR_OPEN_FAILED_BADPASSWORD
  // Error handling will then ask for password and call SetPassword.
  virtual void         SetPassword(const WCHAR* szPassword) { }
  virtual const WCHAR* GetPassword() { return nullptr;  }

  // Override and set add IVF_STORAGEINFO for Optional function support
  // Return free and avail space of the specified path
  // return false if Free/Avail is not availible,
  // return true if free and avail space is set.
  // always check pFreeSpace and pAvailSpace for null before settings them, they can be nullptr
  virtual BOOL GetStorageInformation(const WCHAR* szPath, INT64* pFreeSpace, INT64* pAvailSpace) { return FALSE; }

  // IVF_DROPHANDLING 
  // 0 = NO , 1 = Allowed , -1 Default
  // szOverPath - Is the path that the drag is over (Most of the time this is the parent is OverItem
  // bOverItem - true if the szOverPath is an "item" that we are over, false if it is in "white space" of that path. (outside of item)
  // dwResultFlags - FSDROP_xxxx flags
  virtual int OnAllowDrop(const WCHAR* szOverPath, bool bOverItem) { return -1; }

  // pDropItems - List of file items. (can be nullptr if pData is Not nullptr)
  // pData      - Raw Byte data of drop data
  // nDataLen   - Number of bytes there is in pData
  // nDataType  - Type of Data
  virtual int OnDrop(const WCHAR* szOverItem, IFileItemCollection* pDropItems, const BYTE* pData, DWORD nDataLen, DWORD dwDataType, DWORD& dwResultFlags) { return -1; }

  // Return what overwrite support this volume supports.. VOW_APPEND, VOW_RENAME, VOW_
  virtual DWORD GetOverwriteSupport() { return 0; }

protected:
  IAppConnection* m_pAppConnection = nullptr;

  eVOPEN_MODE   m_nOpenMode;

  // DO NOT change the value of these your self.
  DWORD         m_nRef = 0;
  short         m_nVolumeID = 0;
};

#pragma warning( pop ) 

MCNSEND

#endif

