/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Changes
 * ---------------------
 * 
 */

#ifndef _PLUGININTERFACE_
#define _PLUGININTERFACE_

#pragma once

#include <windows.h>
#include <time.h>
#include <io.h>
#include "MultiApp_Def.h"

#include "MCNamespace.h"
MCNSBEGIN

  // Generic Settings Viewer.
class __declspec(novtable) IGSettingsViewInterface : public IHObject
{
public:
  virtual void SetMessageCommands( DWORD dwSettingsChange, DWORD dwOnPresetClicked ) = 0; 
  
  // if szFont is NULL default font will be used. So it is possible to just change size
  virtual bool SetFont_List(const TCHAR* szFont, long nSize, long lStyle) = 0;
  virtual bool SetFont_Buttons(const TCHAR* szFont, long nSize, long lStyle) = 0;

  virtual BOOL Init( DWORD_PTR pConfigDataReader , DWORD_PTR pConfigLayoutReader , DWORD_PTR pConfigTextReader = 0 ,  BOOL bAutoDelete = FALSE ) = 0;
  virtual BOOL Load( long lModuleID ) = 0;
  virtual BOOL Save() = 0;

  virtual void GoToAndHighlightItem(const wchar_t* szSettingsItem) = 0;
};

MCNSEND
#endif
