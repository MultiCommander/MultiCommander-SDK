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

// Change to void*
typedef DWORD_PTR ZHANDLE;

typedef DWORD_PTR XHANDLE;

typedef long    ZRETURN; // return code
typedef DWORD_PTR   ZCMDID;

class __declspec(novtable) IHObject
{
public:
  virtual ZHANDLE GetHandle() = 0;
  virtual HWND    GetHWND() = 0;
  // Might be useful when connecting SDK created controls with WTL
  virtual BOOL    PreTranslateMessage(MSG* /*pMsg*/) { return FALSE; }
};

// Macros for handling TextID's
#define GROUPID(l)      (char)LOWORD(l)
#define ITEMID(l)     HIWORD(l)
#define MAKETEXTID(c,w)   MAKELONG((WORD)c,w)

#define GETACCEL_KEY(l)     (WORD)LOWORD(l)
#define GETACCEL_MOD(m)     (BYTE)HIWORD(m)
#define MAKEACCELKEY(k,m)   MAKELONG((WORD)k,m)

#ifndef _MC_MAXPATH_
#define _MC_MAXPATH_    4096
#endif

MCNSEND

