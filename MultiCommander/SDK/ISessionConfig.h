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
#include "Collections.h"
MCNSBEGIN
/*
   Get/Set Session Config

   SessionConfig is configuration that are not impotent to keep.
   Data like. last shown value in edit field X, Last 10 values of the combobox,  Last position and stuff.

   set bHistoryValue to TRUE if the session config value is a history value like. Last used value.
   Difference is that they are stored in different way and user can choose to cleanup only history values
*/
class ISessionConfig
{
public:
  virtual bool  GetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , bool &bValue ) = 0;
  virtual bool  GetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , DWORD &bValue ) = 0;
  virtual bool  GetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , WCHAR* strValue , DWORD nLen ) = 0;
  virtual bool  GetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , BYTE* pData , DWORD nDataLen ) = 0;

  virtual bool  SetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , bool bValue ) = 0;
  virtual bool  SetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , DWORD bValue ) = 0;
  virtual bool  SetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , const WCHAR* strValue ) = 0;
  virtual bool  SetSessionConfigValue(bool bHistoryValue, const WCHAR* strKey , const WCHAR* strName , BYTE* pData , DWORD nDataLen ) = 0;

  // Get/set arrays of values
  virtual bool  GetSessionConfigValues( const WCHAR* strKey , IKeyValCollection* pColl ) = 0;
  virtual bool  SetSessionConfigValues( const WCHAR* strKey , IKeyValCollection* pColl ) = 0;
  virtual bool  GetSessionConfigValueItems(const WCHAR* strKey , IValueCollection* pColl ) = 0;
  virtual bool  SetSessionConfigValueItems(const WCHAR* strKey , IValueCollection* pColl , DWORD nMaxCount = 0 ) = 0;

  virtual bool  ClearSessionData(const WCHAR* strKey) = 0;
};
MCNSEND
