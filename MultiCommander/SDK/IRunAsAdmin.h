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
  // Helper for creating command that can be sent to RunAsAdmin
class __declspec(novtable) IDataBuilder
{
public:

  virtual bool PreAllocate(DWORD nSize) = 0;

  virtual bool AddData(const BYTE* pData, DWORD nLen) = 0;
  // Include NULL terminating character
  virtual bool AddData(const WCHAR* szWString) = 0;
  virtual bool AddData(const CHAR* szAString) = 0;
  virtual bool AddData(DWORD nValue) = 0;

  virtual const BYTE* Data() = 0;
  virtual DWORD Size() = 0;

  virtual void Release() = 0;
};

class __declspec(novtable) IDataParser
{
public:
  virtual void Init(const BYTE* pData, DWORD nLen) = 0;

  // return false if EOD
  virtual bool ParseData( BYTE* pData, DWORD nLen ) = 0;
  virtual bool ParseData(DWORD* pData) = 0;
  virtual bool ParseData(const WCHAR** pszWString) = 0;
  virtual bool ParseData(const CHAR** pszAString) = 0;

  virtual void Release() = 0;
};

class __declspec(novtable) IRunAdminCommand
{
public:
  virtual DWORD GetCommandID() = 0;

  virtual const BYTE* Data() = 0;
  virtual DWORD DataLen() = 0;


  virtual void SetResponse(DWORD nResult, DWORD ErrorCode, const BYTE* pData = NULL, DWORD nDataLen = 0) = 0;

  // When the Admin process has loaded a DLL and Created an Interface for it. this is cached in case more request are the extension are sent.
  // With UnloadExtensionFromAdmin you can tell the Admin System that you no longer what the extension dll to be loaded.
  virtual void UnloadExtensionFromAdmin() = 0;

  // helper
  virtual IDataBuilder* CreateDataBuilder() = 0;
  virtual IDataParser*  CreateDataParser() = 0;
};

class __declspec(novtable) IRunAsAdmin
{
public:

  // Before running any function as admin. Check user customization options.
  virtual bool RunAsAdminAllowed() = 0;   // if false. Then Do not run any commands AsAdmin. (will be stopped by the command to if you try)
  virtual bool AutoRetryAsAdmin() = 0;    // If try then you should check IsAdminModeRunning(). and if it is then you should run the command else you should show an error dialog with RetryAsAdmin option
  virtual bool AutoStartRunAsAdmin() = 0; // If true then you do not need to show error dialog before running the RunAsAdmin command

  // status 
  virtual bool IsAdminModeRunning() = 0;

  // ex.
  // hKey == HKEY_LOCAL_MACHINE 
  // szKeyPath == "Software\\MultiCommander\\Machine"
  virtual long RegKeyCreate(HKEY hKey, const WCHAR* szKeyPath) = 0;
  virtual long RegKeyDelete(HKEY hKey, const WCHAR* szKeyPath) = 0;

  // use RegValueSet for creating new values
  virtual long RegValueSet(HKEY hKey, const WCHAR* szKeyPath, const WCHAR* szValueName, short dwType, DWORD dataLen, const BYTE* pData) = 0;
  virtual long RegValueDelete(HKEY hKey, const WCHAR* szKeyPath, const WCHAR* szValueName) = 0;

  virtual long RegKeyCopyMove(HKEY hFrom, const WCHAR* szFromPath, HKEY hTo, const WCHAR* szToPath, bool bMove, bool bAutoRenameIfExists) = 0;
  /////////////////////////////////////////////////////////////////////////
  // Send Custom admin command to an extension

  // Send command to the same extension but loaded by in admin process
  virtual IDataBuilder* CreateDataBuilder() = 0;

  //
  // Can use IDataBuilder as ->  SendAdminCommand( nID, pDataBuilder->Data(), pDataBuilder->Size());
  //
  virtual long SendAdminCommand(DWORD nCommand, const BYTE* pData, DWORD nDataLen) = 0;

  // Release and delete interface. After this has been run the Interface pointer is no longer valid.
  virtual void Release() = 0;
};

MCNSEND