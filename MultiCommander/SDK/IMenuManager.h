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

  // Add/Remove menu item from main menu

class IMenuManager
{
public:
  // TextID is the ID of a test string 
  virtual ZHANDLE FindMenu( long TextId , BOOL bCreateIfNotFound = FALSE ,BOOL bModuleTextID = TRUE ) = 0;
  virtual ZHANDLE FindMenu( ZHANDLE hMenu , long TextId , BOOL bCreateIfNotFound = FALSE ,BOOL bModuleTextID = TRUE ) = 0;
  virtual ZHANDLE FindMenu( ZHANDLE hMenuParent, const WCHAR* szMenuName, BOOL bCreateIfNotFound = FALSE) = 0;

  
  virtual ZHANDLE AddMenu( long TextId ) = 0;
  virtual ZHANDLE AddMenu( const WCHAR* strMenuName, BOOL bIgnoreIfExists = TRUE) = 0;
  virtual ZHANDLE AddMenu( ZHANDLE hParentMenu , long TextID , BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE AddMenu( ZHANDLE hParentMenu , const WCHAR* szText, BOOL bIgnoreIfExists = TRUE ) = 0;

  virtual ZHANDLE AddMenuBefore( long BeforeTextId , long TextId, BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE AddMenuBefore( const WCHAR* strBeforeMenuName , const WCHAR* strMenuName, BOOL bIgnoreIfExists = TRUE) = 0;
  virtual ZHANDLE AddMenuBefore( ZHANDLE hMenu, long TextId, BOOL bIgnoreIfExists = TRUE ) = 0;

  virtual ZHANDLE AddMenuAfter( long AfterTextId , long TextId, BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE AddMenuAfter( const WCHAR* strAfterMenuName , const WCHAR* strMenuName, BOOL bIgnoreIfExists = TRUE) = 0;
  
  virtual long  AddMenuItem_Force(ZHANDLE hMenu, ZHANDLE hCmd, BOOL bIgnoreIfExists = TRUE, HICON hIcon = 0) = 0;
  virtual long  AddMenuItem( ZHANDLE hMenu , ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE , HICON hIcon = 0 ) = 0;
  // Add MenuItem After The Cmd hAfterCmd. if it is 0 Then add it on Top. to add at bottom use 'AddMenuItem'
  virtual long  AddMenuItemAfter( ZHANDLE hMenu, ZHANDLE hAfterCmd , ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE , HICON hIcon = 0 ) = 0;
  virtual long  AddMenuItemBefore( ZHANDLE hMenu, ZHANDLE hAfterCmd , ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE , HICON hIcon = 0 ) = 0;
  virtual long  AddMenuItemAfterItem( ZHANDLE hMenu, ZHANDLE hMenuItem , ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE , HICON hIcon = 0 ) = 0;
  virtual long  AddMenuItemBeforeItem( ZHANDLE hMenu, ZHANDLE hMenuItem , ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE , HICON hIcon = 0 ) = 0;

  // Add Separator
  virtual long  AddMenuSeperator( ZHANDLE hParentMenu , ZHANDLE hAfterMenu = 0 ) = 0;

  virtual void  RebuildMenu()=0;

  // To Be used with Toolbar buttons for example.
  virtual ZHANDLE CreateDynamicMenu( DWORD nID ) = 0;
  virtual ZHANDLE AddDynamicMenu( ZHANDLE hMenu ,const WCHAR* strMenuName , BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE AddDynamicMenuItem( ZHANDLE hMenuMan , ZHANDLE hMenu ,  const WCHAR* strMenuItemName  , BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE AddDynamicMenuItem( ZHANDLE hMenuMan , ZHANDLE hMenu ,  ZHANDLE hCmd , BOOL bIgnoreIfExists = TRUE ) = 0;
  virtual ZHANDLE ReBuildDynamicMenu( ZHANDLE hMenu ) = 0;

  // virtual void Release() = 0;

};

MCNSEND