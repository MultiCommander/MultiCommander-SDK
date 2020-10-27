/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 
 */

/*
Register Commands that can be assigned to Menus/Buttons/or other.

Example

// Create Command that by default also is assign to F7 hotkey (by default. since the FCUSTOMIZABLE is can be change by user)

ZHANDLE hCmd = pAppInterface->RegisterCommand(CMD_MYCMD1, ZCF_ENABLE | ZCF_TARGET_THIS, L"My Command", "My Command" , MAKEACCELKEY(VK_F7,FVIRTKEY|FCUSTOMIZABLE) )

// Add Command to menu
pMenuManager->AddMenuItem(hMyMenu, hCmd);

*/

#pragma once
#include "MCNamespace.h"
MCNSBEGIN

class ICommandManager
{
public:
  // lCmdID = Extension Internal Command ID.
  // lFlags = ZCF_
  virtual ZHANDLE RegisterCommand( ULONG lCmdID , long lFlags , long Title_ID , long ToolTip_ID=0 , DWORD hotKey=0 , ZHANDLE hAction=0) = 0;
  virtual ZHANDLE RegisterCommand( ULONG lCmdID , long lFlags , const WCHAR* pTitle , const WCHAR* pToolTip=NULL , DWORD hotKey=0 , ZHANDLE hAction=0) = 0;

  // if you KNOW that the lCmdID is resisted with ZCF_TARGET_THIS, then set bThisCommand to true.
  // It will find the correct cmd if it is registered as ZCF_TARGET_THIS and bThisCommand as false. but it is slower.
  virtual ZHANDLE FindCommand( ULONG lCmdID, bool bThisCommand = false ) = 0;
  virtual ZHANDLE FindCoreCommand(ULONG lCmdID) = 0;
  virtual ZHANDLE FindCommand(ULONG lInternalCmdID, long lExtID) = 0;

  // Enable / Disable command  ( like grey then for menu / toolbars )
  virtual BOOL    EnableCommand( ZHANDLE hCmd , BOOL bEnable ) = 0;
  // Check/Uncheck command ( like check a menu item )
  virtual BOOL    CheckCommand( ZHANDLE hCmd , BOOL bChecked ) = 0;

  virtual long  GetCommandFlags( ZHANDLE hCmd ) = 0;
  virtual BOOL  SetCommandFlags( ZHANDLE hCmd , long lFlags ) = 0;

  // set lExtID == -1 for core command
  // nAddHotKey = 0, none , 1 Adds hotkey to the end, 2 adds hotkey to the end. using \t as text-hotkey separator. (useful for menu/contextmenus)
  virtual bool  GetCommandDisplayText(WCHAR* szText, UINT nTextLen, ULONG lCmdID, long nExtID, int nAddHotKey = 0) = 0;

  virtual DWORD GetActiveCommandHotKey(ULONG lCmdID) = 0;
};
MCNSEND
