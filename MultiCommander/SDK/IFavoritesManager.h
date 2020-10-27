#pragma once

#include "MCNamespace.h"

MCNSBEGIN

class __declspec(novtable) IFavoritesManager
{
public:
  virtual ~IFavoritesManager() {}
  
  virtual bool SetQuickPath(int nIdx, const WCHAR* szPath, DWORD dwReserved = 0) = 0;
};

MCNSEND
