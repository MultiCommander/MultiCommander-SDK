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
class IMultiWindow
{
public:
  virtual HWND GetHWND() = 0;
  virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;

  virtual BOOL DestroyWindow() = 0;
  virtual void PostNcDestroy() = 0;
};

MCNSEND