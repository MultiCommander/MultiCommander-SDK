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

//
//  Allows Virtual devices to add items to the context menu
//
#pragma once
#include "MCNamespace.h"
MCNSBEGIN

class  __declspec(novtable) IVolumeContextMenu
{
public:

  // dwCommandID must be minimum of 11000 and Maximum of 39000
  virtual MCNS::ZHANDLE AddSubMenu(MCNS::ZHANDLE hParentMenu, const WCHAR* label) = 0;
  virtual ZHANDLE AddMenu(ZHANDLE hParentMenu, const WCHAR* label, DWORD dwCommandID, DWORD dwFlags) = 0;
  virtual ZHANDLE AddSeperator(ZHANDLE hParentMenu) = 0;
};


// Not a nice solution.. change this
class  __declspec(novtable) IVolumeCommandResult
{
public:
  // FSEXECUTE_
  virtual DWORD FileSystemExecute() = 0;
  virtual void  FileSystemExecute(DWORD dwResult) = 0;

  virtual void  AddParameter(const WCHAR* szParam) = 0;

};

MCNSEND