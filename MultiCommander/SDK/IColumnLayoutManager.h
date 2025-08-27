/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2025 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once

#include "MCNamespace.h"
MCNSBEGIN

// Option . Same as under IFilePropertiesManager
class __declspec(novtable) IColumnLayoutInterface
{
public:
  virtual ~IColumnLayoutInterface() {}
  virtual bool AddColumn( const WCHAR* name, UINT nWidth, bool bReadonly = true, BYTE nAlign = DT_LEFT, WORD nFlags = 0, DWORD nOptions = 0) = 0;
};


class __declspec(novtable) IColumnLayoutManager
{
public:
  virtual ~IColumnLayoutManager() {}

  virtual IColumnLayoutInterface* CreateColumnLayout() = 0;
  virtual void DeleteColumnLayout(IColumnLayoutInterface* pColumnLayout) = 0;

  virtual bool RegisterDefaultColumnLayout( IColumnLayoutInterface* pColumnLayout, const char* guid) = 0;

};

MCNSEND