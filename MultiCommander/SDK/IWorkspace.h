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

#include "IFileOperations.h"
#include "IEventCallback.h"

#include "MCNamespace.h"
MCNSBEGIN

  // WorkspaceItemFlags
#define WIF_VIRTUALFOLDER 0x0010

class __declspec(novtable) IWorkspaceItem
{
public:
  virtual const WCHAR* GetName(WCHAR* str , UINT nMaxLength) = 0;
  virtual const WCHAR* GetPath(WCHAR* str , UINT nMaxLength) = 0;
  virtual const WCHAR* GetRefPath(WCHAR* str , UINT nMaxLength) = 0;
  virtual UINT64    GetAttributes() = 0;
  virtual DWORD    GetFlags() = 0;
  virtual IFileItem*   GetRefItem() = 0;
};


// WorkspaceView Styles
#define WVS_MAINROOT    0x00000100L //  Have a main root item.
#define WVS_DRAGDROP    0x00001000L //  Allow Drag 'n' Drop of items ( if RegisterDragDropTarget(false) is called then only drag of files OUT of ctrl is allowed )
#define WVS_ACCEPTFILES 0x00002000L //  Allow files to be dropped and added to the workspace ( Wrong ?!, Allow drop of files on empty space in the view or if a drop must be on an item)
#define WVS_READONLY    0x00010000L //  Will NOT allow any modification of any items.
#define WVS_FLATVIEW    0x00020000L //  Draw flat view. No treestyle.
#define WVS_NOSTRECH    0x00040000L //  No Auto Streching of Filename or Path column
#define WVS_PATHCOLUMN  0x00100000L //  Add a path column

#define WVS_DRAGDATA_INTERNAL   0x0001  // Allows rearrange of items in the workspace.
#define WVS_DRAGDATA_FILEPATHS  0x0002  // Allows files to be dragged OUT of the controller
#define WVS_DRAGDATA_INTERNALFORCECOPY  0x0004  // Force 'Copy' drag actions as default for internal drags to filebrowser


class __declspec(novtable) IFileWorkspaceViewInterface : public IHObject, public ISetEvent
{
public:
  // Create layout
  virtual bool  Init( DWORD nOptions = 0 , DWORD nDragDataModes = WVS_DRAGDATA_INTERNAL) = 0;
  virtual bool  Clear() = 0;

  // Insert items in Workspace
  virtual ZHANDLE InsertItem( ZHANDLE hParent , const WCHAR* strText = NULL , DWORD dwOptions = 0 , IFileItem* pRefItem = NULL ) = 0;

  virtual bool  RemoveCurrentItem( bool bRemoveSubItems ) = 0;
  virtual bool  RemoveItem( DWORD nIndex , bool bRedraw = true ) = 0;

  virtual IWorkspaceItem* GetWorkspaceItem( DWORD nIndex ) = 0;

  // The handles in the IHCollection is IWorkspaceItems
  virtual bool  GetCheckedWorkspaceItems( IHCollection* pValues ) = 0;
  // return how many items are checked.
  virtual DWORD GetCheckCount() = 0; 

  // Modify workspace items
  virtual bool    Update( bool bRedrawOnly = true ) = 0;

  // Return Workspace relative path of an item
  virtual WCHAR*  GetItemPath( WCHAR* strPath , UINT nMaxLenght , UINT nIndex ) = 0;

  virtual bool  SelectItems( const WCHAR* strMatch , bool bSelect , bool bRecursive ) = 0;

  virtual IFileItem* GetFileItemInFocus() = 0;
  virtual bool GetFileItems( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool GetSelection( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool SetSelection( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool SetSelectionByFilter( const WCHAR* strFilter, DWORD dwFlags ) = 0;
  virtual bool SetSelectionAll( bool bSelect ) = 0;
  virtual bool InvertSelection() = 0;

  virtual bool  Load( const WCHAR* strFilename ) = 0;
  virtual bool  Save( const WCHAR* strFilename , const WCHAR* szRootName = NULL) = 0;

  virtual void  GetStatus( INT64 &nTotalSize , DWORD &nTotalFiles , DWORD &nTotalFolders , INT64 &nSelectedSize , DWORD &nSelectedFiles , DWORD &nSelectedFolders ) = 0;

  virtual bool  OnCopy( bool bSilent = false ) = 0;
  virtual bool  OnMove( bool bSilent = false ) = 0;
  virtual bool  OnDelete( bool bSilent = false , IFileOpStatusCallback* pCallback = NULL ) = 0;

  virtual bool  CopyToClipboard() = 0;

  virtual DWORD FindIndexByItem( IWorkspaceItem* pWorkspaceItem ) = 0;
};

MCNSEND
