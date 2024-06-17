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

class IMultiAppInterface;
class IFileItem;

#define MCTHUMB_QUALITY_BEST    0x00000100    // Slowest but best quality (Bilinear?) 
#define MCTHUMB_QUALITY_AVR     0x00000200
#define MCTHUMB_QUALITY_WORST   0x00000400    // Fastest 

class __declspec(novtable) IMCThumbnail
{
public:
  // Return the module ID that is a GUID string
  virtual char* Get_ModuleID() = 0;

  virtual long PreStartInit( IMultiAppInterface* /*pInterface*/) { return 0; }

  virtual HBITMAP GetThumbnail(const WCHAR* szFullPath, int cxThumbSize, int cyThumbSize, DWORD dwFlags = 0) = 0;
};

MCNSEND
