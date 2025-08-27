/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2025 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once
#include "MCNamespace.h"
MCNSBEGIN

// Use to send data between two extension of different types

#define ZIMPORT_FALSE    0
#define ZIMPORT_SUCCESS    1
#define ZIMPORT_USEIMPOBJECT 2

struct IXData  //  Import / Export Data
{

  long   nDataFormat;           // ZDF_FILE  ,  ZFILEITEM , IMAGE , AUDIO , TEXT 
  WCHAR  wsDataType[10];        // "exe" , "jpg" , "gif" , "mp3" , "txt"
  long   lDataFlags;
  long   lDataCount;            // How many items to Export/Import/Are in Collection
  DWORD_PTR   hData;            // ZHANDLE to a Collection of data
  long   hDestinationParam;     //
  ZHANDLE hSourceModule;        //
  ZHANDLE hDestinationModule;   //

};

struct IXItem
{
  WCHAR* ItemName;
  long nItemType;
  long nParam1;
  long nParam2;
};

class __declspec(novtable) IExportData
{
public:

  virtual void Init( IXData *pXData , long Buffersize ) = 0;
  virtual void DeInit() = 0;

  virtual long Get_ExportCount() = 0; // Return the no of items awaiting export

  virtual XHANDLE OpenObject( int nItem , IXItem *pXItem ) = 0;

  // write data to the module
  virtual long ReadData( XHANDLE hHandle , void* Buffer , long Buffersize ) = 0;

  virtual long CloseObject( XHANDLE hHandle ) = 0;
};

class __declspec(novtable) IImportData
{
public:

  virtual void Init( IXData *pXData , long Buffersize ) = 0;
  virtual void DeInit() = 0;

  virtual XHANDLE OpenObject( IXItem *pXItem  ) = 0;

  // write data to the module
  virtual long WriteData( XHANDLE hHandle , void* Buffer , long Buffersize ) = 0;
  //
  virtual long CloseObject( XHANDLE hHandle ) = 0;
};

MCNSEND