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

class __declspec(novtable) IRecentManager
{
public:
  virtual ~IRecentManager() {}

  virtual void  Init( DWORD nRecentCount ) = 0;

  virtual void  Add(const WCHAR* szDisplayName, const WCHAR* szValue, DWORD_PTR dwCustomParam) = 0;
  virtual void  ChangeCurrentValue(const WCHAR* szValue, DWORD_PTR dwCustomParam) = 0;

  virtual DWORD GetLastSelected() = 0;
  virtual void  SetLastSelected(DWORD nIdx) = 0;

  virtual DWORD       Count() = 0;

  virtual bool        GetName( WCHAR* str ,DWORD nTextLen, DWORD nIdx ) = 0;
  virtual bool        GetValue( WCHAR* str ,DWORD nTextLen, DWORD nIdx ) = 0;
  virtual DWORD_PTR   GetCustomParam( DWORD nIdx ) = 0;

  // Lock / UnLock recent manager for Add's. if locked then if Add is call nothing is added.
  virtual void Lock() = 0;
  virtual void UnLock() = 0;

  virtual void Clear() = 0;

};

class __declspec(novtable) IHistoryManager
{
public:
  virtual void Add(const WCHAR* szItem) = 0;
  virtual bool GetHistoryItem( WCHAR* str ,DWORD nTextLen, DWORD nIdx ) = 0;

  // Call to cleanup
  virtual void Release() = 0;
};

MCNSEND