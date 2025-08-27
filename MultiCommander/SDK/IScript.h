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
MCNSBEGIN

class __declspec(novtable) IScriptFunctions
{
public:
  enum DataType
  {
    None,
    String,
    Num,
    Handle,
    Array,
  };

  /*
  nFunctionID    - Extension/Plugin unique ID to identify the function. is use in
  szFunctionName - Name of the script function that should be registered-
  returnType - Return type of the function

  */
  virtual bool  RegisterScriptFunction(DWORD nFunctionID, const WCHAR* szFunctionName, DataType returnType) = 0;
  virtual bool  RegisterScriptFunction(DWORD nFunctionID, const WCHAR* szFunctionName, DataType returnType, DataType param1) = 0;
  virtual bool  RegisterScriptFunction(DWORD nFunctionID, const WCHAR* szFunctionName, DataType returnType, DataType param1, DataType param2) = 0;
  virtual bool  RegisterScriptFunction(DWORD nFunctionID, const WCHAR* szFunctionName, DataType returnType, DataType param1, DataType param2, DataType param3) = 0;
  virtual bool  RegisterScriptFunction(DWORD nFunctionID, const WCHAR* szFunctionName, DataType returnType, DataType* pParams, DWORD nParamCount) = 0;
};

class IScriptFunctionParameter;

class __declspec(novtable) IScriptArray
{
public:
  virtual IScriptFunctions::DataType ArrayType() = 0;

  // return false if datatype of value is wrong
  virtual bool AddValue(INT64 nValue) = 0;
  virtual bool AddValue(const WCHAR* szValue) = 0;

  virtual const IScriptFunctionParameter* GetValue(DWORD nIdx) const = 0;

  virtual DWORD GetCount() const = 0;
};

class __declspec(novtable) IScriptHandle
{
public:
  virtual ~IScriptHandle() {}
  virtual void* GetHandle() = 0;

};

class __declspec(novtable) IScriptFunctionParameter
{
public:
  virtual IScriptFunctions::DataType  GetDataType() const = 0;  

  // will return false if datatype of parameter does not match
  virtual bool GetNumValue(INT64* pValue) const = 0;
  virtual bool GetStringValue(const WCHAR** szValue) const = 0;
  virtual const IScriptArray* GetArrayValues() const = 0;
  //virtual bool GetHandleValue(void** pHandle) const = 0;
  virtual bool GetHandleValue(IScriptHandle** pHandle) const = 0;
}; 

class __declspec(novtable) IScriptFunctionContext
{
public:

  enum Error
  {
    NoError = 0,
    InvalidNoParamers = 4,
    InvalidParameter = 5,
    InvalidData = 6,
    FunctionNotImplemented = 10,

  };

  virtual DWORD FunctionID() = 0;

  // Set error on why this function call did not work. execution of script will be aborted.
  virtual void ErrorCode(Error error) = 0;

  // Set Return Value
  virtual void ReturnValue(INT64 nValue) = 0;
  virtual void ReturnValue(const WCHAR* szValue) = 0;
  virtual void ReturnHandleValue(IScriptHandle* pHandle) = 0; // This should replace the void* above

  virtual IScriptArray* CreateArrayReturnValue(IScriptFunctions::DataType type) = 0;

  virtual DWORD Parameters() = 0;
  virtual IScriptFunctionParameter* GetParameter(DWORD nIdx) = 0;

  // To log an Error, Warning or Info 
  virtual void LogError(const WCHAR* szErrorMsg) = 0;
  virtual void LogWarning(const WCHAR* szWarningMsg) = 0;
  virtual void LogInfo(const WCHAR* szInfoMsg) = 0;

};

MCNSEND