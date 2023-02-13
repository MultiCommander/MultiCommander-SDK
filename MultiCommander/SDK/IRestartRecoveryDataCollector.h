#pragma once
#include "MCNamespace.h"
MCNSBEGIN


class __declspec(novtable) IRestartRecoveryDataCollector
{
public:
  virtual ~IRestartRecoveryDataCollector() {}

  virtual void AddValue(const wchar_t* key, const wchar_t* value) = 0;
  virtual bool AddData(const wchar_t* key, const BYTE* pData, DWORD dataLen) = 0;

  virtual void Clear() = 0;
  virtual const wchar_t* GetKeyValue(const wchar_t* key) = 0;
  
};

MCNSEND