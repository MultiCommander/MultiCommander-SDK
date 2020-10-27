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

#define FILEPROP_STRING       0x00000100L
#define FILEPROP_NUM          0x00000200L
#define FILEPROP_DATE         0x00000400L
#define FILEPROP_DOUBLE       0x00000800L

#define FILEPROP_FORMATDISP   0x00001000L // Format a display value from a cached raw property value.

#define FILEPROP_CUSTOMIZABLE 0x00010000L // The FileProperty item is customizable. The User can add/remove this column (not just enable/disable)
                                          // If this is not set. It can only be set by a module using the ColumnLayout

#define FILEPROP_ASYNC        0x00020000L // Get this column data asyncrone. (If it takes time to fetch this column information adding this flag is recommeded)
#define FILEPROP_NOTINMENU    0x00040000L // Items that are Customizable and this flag will NOT be shown directly in the context menu.
                                          // user have to go into the "customize..." option to find them
                                          // Add this to properties that are not often used. Because we do not want the entire screen to be covered when showing the contextmenu

#define FILEPROP_DONOTCACHEASDISPLAY  0x00100000L // Do not cache value as display value.. Value will be featch every thing it is draw.. !! warnign very slow
#define FILEPROP_EDITABLE     0x01000000L // - Not supported yet
#define FILEPROP_EXECUTE      0x02000000L // - If set and user Ctrl + DoubleClick on property in the explorerPanel
                                          //   then IFileProperties->Execute(IFileItem* pFileItem , DWORD propType) is called

struct FilePropData
{
  WORD propType;
  short IdealWidth;
  short columnFlags; // See IMLC_ for IMLC_OVERDRAW, IMLC_ALLOW_OVERDRAW, IMLC_SORT_DECENDING, IMLC_STRETCH
  BYTE  Align;
  BYTE  SubCategory;
  DWORD dwOptions; // FILEPROP_ASYNC , FILEPROP_.... , ...
  const WCHAR* szPropName;
  const WCHAR* szDisplayName;
  const WCHAR* szColumnName;
  const WCHAR* szCategoryName;
  const WCHAR* szDescription;
};

class __declspec(novtable) IFilePropertiesManager
{
public:

  //	propType - Unique Identifier for property
  //  propName - name identifier
  //  displayName - The name shown to the user
  //  categoryName - Category name shown to the user
  //
  //
  // Category and displayName together is uses to build strings to show to user..
  // eg just displayName is shown in column header
  // But when selection columns to show the CategoryName is also shown  "<Category> - <displayName>"

  virtual bool RegisterProperty(FilePropData* pFilePropertyData) = 0;

  // Must be run before registering properties. 
  // No need to run it if only querying properties value
  virtual bool Init(const char* guid) = 0;

};

MCNSEND