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

  /*
   
   FileProperties Interface 

   Inherit and create and exhaustions using IFileProperties to add support for extended file properties that can be viewed in column, multi-rename and more.

  */

// forward deceleration
class IMultiAppInterface;
class IFileItem;

struct __declspec(novtable) ExecuteInfo
{
  int size; // sizeof(ExecuteInfo) 

  IFileItem* pFileItem;
  WORD propType;
  POINT pt;
  HWND hHwndParent;

  DWORD dwResult;
};

class __declspec(novtable) IFileProperties
{
public:

  // Override this and return GUID
  virtual char* Get_ModuleID() = 0;

  // Run when MC is starting up.
  // Override and add code to register property types in here
  virtual long PreStartInit( IMultiAppInterface* /*pInterface*/ ) { return 0; }


  // Open file to get properties for
  // return true if successfully or false if failed
  virtual bool Open(IFileItem* pParentFileItem) = 0;
  virtual bool Open(const WCHAR* szParentPath) = 0;

  virtual bool Close() = 0;

  // Please note for properties that takes long time to get 
  //
  // If the property that is return can take long time to get. then try to check
  // the pAbort parameter periodically
  //
  // if(pAbort && *pAbort == true) and then abort the process of getting the property
  //
  // pAbort is set to true if user is browsing away from the current folder
  //

  // return a value with GetDisplayValue if the property has a special value for presentation.
  // Example. a Num Property might be presented with a Text for display, so instead of showing 3, 5, it can show "Novice" , "Advanced". but Internally
  // it is a number. When doing sorting on a column the GetPropStr / GetPropNum will be used. and there the num value are returned.

  // if no display value is used then GetPropStr/GetPropNum is called and if it is a number will just be converted into a string for display

  // Why no just show all values as string then? you ask, because we want to be able to use extended number properties
  // from search, script and so on. and also so that sorting is correct, and if a property is a number we know we can do > (bigger then) == equal.
  //
  // Exception to the rule - FILEPROP_DATE properties are not called for DisplayValues. GetPropNum are always called on them. And we use the
  // user defined date/time settings so show the date
  
  virtual bool GetDisplayValue(IFileItem* pFileItem, WCHAR* propData, WORD nLen, WORD propType, const volatile bool* pAbort) = 0; // Remove volatile - Not needed 

  // called for property that are FILEPROP_STRING
  virtual bool GetPropStr(IFileItem* pFileItem, WCHAR* propData, WORD nLen, WORD propType, const volatile bool* pAbort) = 0;// Remove volatile - Not needed 

  // called if property is FILEPROP_NUM or FILEPROP_DATE  if DATE the INT64 is a FILETIME 
  virtual bool GetPropNum(IFileItem* pFileItem, INT64* propData, WORD propType, const volatile bool* pAbort) = 0;// Remove volatile - Not needed 

  // called if property is FILEPROP_DOUBLE
  virtual bool GetPropDouble(IFileItem* pFileItem, double* propData, WORD propType, const volatile bool* pAbort) = 0;// Remove volatile - Not needed 

  // optional
  // Format a cached raw property value. If false is return GetDisplayValue will be called and if that failed raw value is converted to string and used
  virtual bool FormatDisplayValue(WCHAR* /*szDisplayValue*/, WORD /*nLen*/, double /*dValue*/, WORD /*propType*/) { return false;  }
  virtual bool FormatDisplayValue(WCHAR* /*szDisplayValue*/, WORD /*nLen*/, INT64 /* nValue*/, WORD /*propType*/) { return false;  }

  virtual bool SetProp(IFileItem* pFileItem, WORD propType, const BYTE* propData) = 0;

  // override to support execute action on property
  // return true if handled. else something else will get the execute command
  // Make sure "pExecuteInfo->size == sizeof(ExecuteInfo)"
  virtual bool Execute(ExecuteInfo* /*pExecuteInfo*/) { return false; }
protected:

};
MCNSEND