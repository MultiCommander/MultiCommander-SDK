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
#include "MCNamespace.h"
MCNSBEGIN

#define FSM_MODIFIED  0x0001
#define FSM_ADDED     0x0002
#define FSM_REMOVED 	0x0004
#define FSM_RENAMED 	0x0008

class __declspec(novtable) IFileSystemMonitor
{
public:

  // Refresh this path, (No rescan. refreshed existing items) and notify all views to reload
  virtual void RefreshPath(const WCHAR* szPath) = 0;

  // Force a reload of path
  virtual void RescanPath(const WCHAR* szPath) = 0;

  // Remove item from FileSystem Cache, and notify all views that the item is gone.
  virtual void FileSystemItemRemoved(const WCHAR* szPath) = 0;
  /*
  virtual void	InvokeChange(const WCHAR* strPath, DWORD dwFlags) = 0;

  virtual bool  AddIgnoreChange(const WCHAR* strPath, DWORD dwFlags) = 0;
  virtual bool  RemoveIgnoreChange(const WCHAR* strPath, DWORD dwFlags) = 0;

  // Not sure about this
  // Turn Notifications on/off
  virtual bool  SetViewChangeNotification( ZHANDLE hView, bool bAcceptNotification ) = 0;
  */
};

MCNSEND