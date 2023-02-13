/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2022 All Rights Reserved , http://multicommander.com
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
  // all from there will affect all conflicts in the queue
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


class __declspec(novtable) IRenameItem
{
public:
  virtual IFileItem*   GetFileItem() = 0;
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
  virtual IRenameItem* Add( IFileItem* pFileItem , const WCHAR* newName, DWORD_PTR nRefPtr = 0) = 0;

  virtual IRenameItem* Add( const WCHAR* oldName, const WCHAR* newName, DWORD_PTR nRefPtr = 0) = 0;

  virtual DWORD Count() = 0;

  virtual IRenameItem*  GetAt( DWORD nIndex ) = 0;

  virtual void  SetOptions(DWORD dwOptions) = 0;
  virtual DWORD GetOptions() = 0;

  virtual void  Release() = 0;

  // Result state 
  virtual DWORD ItemsRenamed() = 0;
  virtual DWORD ItemsFailed() = 0;
};

MCNSEND