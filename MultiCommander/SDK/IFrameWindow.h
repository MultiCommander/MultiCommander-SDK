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

#pragma once

#include "Multi_Type.h"
#include "MCNamespace.h"
MCNSBEGIN

#define FW_TOOLBAR    0x00000004
#define FW_STATUSBAR  0x00000008

#ifndef SBPS_NORMAL
#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar
#endif

// OBSOLETE - Not used

class IFrameWindow
{
public:
  virtual ZHANDLE CreateMainView(DWORD nViewID, long createParam, long param1, long param2) = 0;

  // A child window is a dlg. We must know that. (so we can pass message to IDialogMessage)
  virtual void AddDlgWindow(HWND hWnd) = 0;

  virtual void SetCaption(const WCHAR* szCaption) = 0;

  // Create the frame window. A MainView must have been added to the frame window, or this will fail
  virtual bool Create() = 0;

  // after calling this. this interface is not valid anymore
  virtual bool DeleteFrameWindow() = 0;

  virtual bool LoadWindowPos(const WCHAR* szSettingsName, int defaultHeight, int defaultWidth) = 0;
  virtual bool SaveWindowPos(const WCHAR* szSettingsName) = 0;
  //
  virtual void RecalcLayout() = 0;

  // Post Message that will close this frame when it can
  virtual void PostCloseMessage() = 0;

  // nID = The id a a statusbar item.
  // return idx for item if pos is -1
  virtual int  AddStatusBarIndicator(int pos, UINT nID, UINT nStyle = SBPS_NORMAL) = 0;
  virtual void SetStatusBarPaneWidth(int pos, int width) = 0;
  virtual void SetStatusBarPaneStyle(int pos, unsigned int style) = 0;
  virtual void SetStatusBarPaneText(UINT nID, const WCHAR* szText) = 0;
};

MCNSEND