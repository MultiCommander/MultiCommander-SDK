/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once
#include "MCNamespace.h"
MCNSBEGIN

  //
  // Get FileProperties from a filename
  //

class IFilePropertiesRetriever
{
public:
  // If FileProperty is a date property and pFT is NULL then value will be converted into a date string and set to szText
  virtual int GetFileProp(WCHAR* szText, int nLen, const WCHAR* szProp, FILETIME* pFT, const WCHAR* szFilename) = 0;

  // szPropName is be a extension name like MCPictureProp then only properties for that extension will be returned.
  // if it is nullptr then all will be returned.
  virtual int GetProperties(IKeyValCollection* pCollection, const WCHAR* szPropName = nullptr) = 0;

  virtual void Release() = 0;
};
MCNSEND