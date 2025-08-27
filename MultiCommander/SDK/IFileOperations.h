/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2025 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once

#include "MCNamespace.h"

MCNSBEGIN

#define FOS_FINISHED  1
#define FOS_PAUSED    2
#define FOS_RUNNING   3

#define FOIS_DELETED  0x00000010L
#define FOIS_COPIED   0x00000020L
#define FOIS_MOVED    0x00000040L
#define FOIS_RENAMED  0x00000080L

#define FOIS_SUCCESS  0x00000001L
#define FOIS_FAILED   0x00000002L

// File operation - Overwrite attributes flags
#define FO_OWF_OVERWRITE_READONLY 0x00010000L
#define FO_OWF_OVERWRITE_SYSTEM   0x00020000L
#define FO_OWF_OVERWRITE_HIDDEN   0x00040000L
#define FO_OWF_OVERWRITE_ALLFLAGS 0x00070000L

// overwrite by condition flags
enum class OverwriteOption
{
  Abort = -1,
  Ask = 0,
  SkipThis,
  RenameThis,
  AppendThis,
  OverwriteThis,
  AutoRenameThisNew, // Not imp yet
  AutoRenameThisExisting, // Not imp yet

  // all from here will affect all conflicts in the queue
  SkipAll = 10,
  OverwriteAll,
  OverwriteIfNewer,
  OverwriteIfOlder,
  OverwriteIfNewerAndSizeDiffers,
  OverwriteIfSizeDiffers,
  OverwriteIfSizeLarger,
  OverwriteIfSizeSmaller,
  OverwriteKeepBothAutoRenameNew,
  OverwriteKeepBothAutoRenameExisting,
};

enum class ProtectedFileOption // if file has ReadOnly/System/Hidden attribute set
{
  Abort = -1,
  Ask = 0,
  DeleteOrOverwriteAll,
  SkipAll,
};

enum class FileSystemLinkOptions
{
  FollowLinks = 0, // Follow links to real file/folder
  IgnoreLinks, // Ignore links and treat them as normal files/folders
  CopyMoveLinks, // Copy/Move links as links (For Copy/Move FileOperations)
  DeleteLinks, // Delete links (For Delete FileOperations) 
};

class __declspec(novtable) IRenameItem
{
public:
  virtual IFileItem* GetFileItem() = 0;
  virtual const WCHAR* GetOldName() = 0;
  virtual const WCHAR* GetNewName() = 0;

  virtual DWORD        GetOptions() = 0;

  virtual DWORD_PTR    GetRefData() = 0;

  virtual DWORD        GetResultCode() = 0;
  //virtual DWORD        RenameResult() = 0; // error code from rename operation
};

class __declspec(novtable) IRenameItems
{
public:
  virtual IRenameItem* Add(IFileItem* pFileItem, const WCHAR* newName, DWORD_PTR nRefPtr = 0) = 0;

  virtual IRenameItem* Add(const WCHAR* oldName, const WCHAR* newName, DWORD_PTR nRefPtr = 0) = 0;

  virtual DWORD Count() = 0;

  virtual IRenameItem* GetAt(DWORD nIndex) = 0;

  virtual void  SetOptions(DWORD dwOptions) = 0;
  virtual DWORD GetOptions() = 0;

  virtual void  Release() = 0;

  // Result state 
  virtual DWORD ItemsRenamed() = 0;
  virtual DWORD ItemsFailed() = 0;
};

class __declspec(novtable) IFileOpStatusCallback
{
public:
  virtual       ~IFileOpStatusCallback() = default;
  virtual DWORD GetOperationStatus() = 0;
  virtual void  SetOperationStatus(DWORD nStatus) = 0;
  virtual void  SetStatus( DWORD_PTR nRefItem , DWORD nStatus ) = 0;
};


#define IFO_COPY          0x00000001L // Copy
#define IFO_MOVE          0x00000002L // Move
#define IFO_DELETE        0x00000004L // Delete
#define IFO_RENAME        0x00000008L // ? Isn't rename same as move ?
#define IFO_NO_RECURSIVE  0x00000100L // If source is a folder. DO not process recursive into folder
#define IFO_VIRTUAL       0x00010000L // Item is virtual. source object does not exists.


class __declspec(novtable) IFileOperationItem
{
public:

  // eg. "c:\temp\" path
  virtual void  SetTarget( const WCHAR* strTarget ) = 0;      
  // eg. "Kalle.tmp" name
  virtual void  SetTargetName( const WCHAR* strTargetName ) = 0;

  // Full path to source item, eg. "d:\nisse\myfile.txt"
  virtual void  SetSource( const WCHAR* strSource ) = 0;
  virtual void  SetOptions( DWORD dwOptions ) = 0;

  // Get Taget Path
  virtual WCHAR*  GetTarget( WCHAR* str , UINT nMaxLenght ) = 0;
  // Get Target Name
  virtual WCHAR*  GetTargetName( WCHAR* str , UINT nMaxLenght ) = 0;
  // Get Source Item Path
  virtual WCHAR*  GetSource( WCHAR* str , UINT nMaxLenght ) = 0;
  virtual const WCHAR* GetSource() const = 0;

  virtual DWORD   GetOptions() = 0;

  // Set/Get custom RefValue. (Sent to plugin via callback)
  virtual void        SetRefValue( DWORD_PTR nValue ) = 0;
  virtual DWORD_PTR   GetRefValue() = 0;
};

class __declspec(novtable) IFileOperation
{
public:
  virtual IFileOperationItem* AddNewFileOperation() = 0;

  virtual DWORD Count() = 0;
  virtual IFileOperationItem* GetAt( DWORD nIndex ) = 0;

  virtual void    SetTargetPath( const WCHAR* str ) = 0;
  virtual WCHAR*  GetTargetPath( WCHAR* str , UINT nMaxLength ) = 0;

  // FileOperations Options.. (Valid For all items)
  virtual DWORD GetOptions() = 0;
  virtual void  SetOptions( DWORD dwOptions ) = 0;

  virtual void SetFilter(const WCHAR* strFilter) = 0;
};

//=================================================================================
// NEW FileOperation Interface - DO not use IFileOperation/IFileOperationItem with the interfaces below
enum class FileOperationStatus
{
  Finished = FOS_FINISHED,
  Paused = FOS_PAUSED,
  Running = FOS_RUNNING,
};

enum class FileOperationMethod
{
  Copy,
  Move,
  Delete,
  Pack,   // Not used yet
  Unpack, // Not used yet
  Rename, // Not used yet
};

class __declspec(novtable) IFileOperationQueueItem
{
public:
  virtual const wchar_t* ItemPath() const = 0;
  virtual bool Recursive() const = 0;

};

class __declspec(novtable) IFileOperationProcess
{
public:
  virtual FileOperationStatus GetStatus() = 0;

};

class __declspec(novtable) IFileOperationQueue
{
public:

  virtual void SetTargetPath(const WCHAR* strTargetPath) = 0;
  // this is used to determain what folder structure from item should be create in target path
  virtual void SetSourcePath(const WCHAR* strSourcePath) = 0;

  virtual const wchar_t* GetTargetPath() const = 0;
  virtual const wchar_t* GetSourcePath() const = 0;

  virtual bool QueueItem(const wchar_t* szItem, bool recursive) = 0;
  virtual size_t Count() const = 0;
  virtual IFileOperationQueueItem* GetAt(size_t nIndex) const = 0;

  // No more items can be added to the queue
  virtual void LockQueue() = 0;
  virtual void UnlockQueue() = 0;
  
};

class __declspec(novtable) IFileOperationOptions
{
public:


  // Setter methods
  virtual void ShowProgressUI(bool bShow) = 0;
  virtual void Filter(const wchar_t* szFilter) = 0;
  virtual void OverwriteOption(OverwriteOption option) = 0;
  virtual void ProtectedFileOption(ProtectedFileOption option) = 0;
  virtual void KeepIncompleteBrokenFiles(bool bKeep) = 0;
  virtual void KeepDateTime(bool bKeep) = 0;
  virtual void KeepAttributes(bool bKeep) = 0;
  virtual void KeepSecurity(bool bKeep) = 0; 
  virtual void DoNotCopyEmptyFolders(bool bDoNotCopy) = 0;
  virtual void AutoClose(bool bAutoClose) = 0;
  virtual void AutoCloseOnlyFastOperation(bool bAutoClose) = 0;
  virtual void ShowErrorReportWhenFinihsed(bool bShow) = 0;
  virtual void ShowErrorReportOnlyOnErrorOrSkipped(bool bShow) = 0;

  // Getter methods
  virtual bool IsShowProgressUIEnabled() const = 0;
  virtual const wchar_t* GetCurrentFilter() const = 0;
  virtual MCNS::OverwriteOption GetCurrentOverwriteOption() const = 0;
  virtual MCNS::ProtectedFileOption GetCurrentProtectedFileOption() const = 0;
  virtual bool IsKeepIncompleteBrokenFilesEnabled() const = 0;
  virtual bool IsKeepDateTimeEnabled()  const = 0;
  virtual bool IsKeepAttributesEnabled() const = 0;
  virtual bool IsKeepSecurityEnabled() const = 0;
  virtual bool IsDoNotCopyEmptyFoldersEnabled() const = 0;
  virtual bool IsAutoCloseEnabled() const = 0;
  virtual bool IsAutoCloseOnlyFastOperationEnabled() const = 0;
  virtual bool IsShowErrorReportWhenFinihsedEnabled() const = 0;
  virtual bool IsShowErrorReportOnlyOnErrorOrSkippedEnabled() const = 0;
  
//  virtual void Links(FileSystemLinkOptions linkOption) = 0;

};


class __declspec(novtable) IFileOperationContext
{
public:
  // Craete a new Queue. a Context can have multiple queues.
  // All files to the same target root path should have there own queue.
  virtual IFileOperationQueue*  Queue() = 0;

  virtual IFileOperationOptions* Options() = 0;

  virtual size_t Queues() = 0;

  virtual void Release() = 0;
};


class __declspec(novtable) IFileOperationService
{
public:
  virtual IFileOperationContext* CreateContext(FileOperationMethod method) = 0;


  virtual IFileOperationProcess* Start(IFileOperationContext* pContext) = 0;

  // will delete and cleanup everything with the FileOperation.
  virtual void Release() = 0;

};


MCNSEND