#pragma once

#include "MCNamespace.h"

MCNSBEGIN

class IAppMenu
{
public:
  virtual bool AddMenuSeperator() = 0;
  // dwMenuItemFlags - MF_DISABLED | MF_CHECKED | MF_GRAYED
  virtual bool AddMenuItem(const WCHAR* szItemText, DWORD dwCommand, DWORD dwMenuItemFlags) = 0;

  virtual IAppMenu* AddSubMenu(const WCHAR* szItemText) = 0;
};

MCNSEND

