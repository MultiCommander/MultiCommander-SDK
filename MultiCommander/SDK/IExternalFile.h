/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 
 */

/*

IExternalFile will get an file from an "archive" filesystem ( eg. zip file ) and unpack it to temp folder.
and then automatically delete it when it is not used anymore.

Example
IFileSystemManager* pFileSysMan = GetIFileSystemManager();

IExternalFile* pVFile = pFileSysMan->CreateExternalFile( L"c:\MyZipfile.zip\MyTxtFile.txt" , VF_TRACKCHANGED | VF_UPDATE | VF_DELETE | VF_FINALTHREAD );
if( pVFile )
{
WCHAR strExternalFilename[1024];
if( !pVFile->GetExternalFilename( strExternalFilename ) )
return FALSE;

bRetVal = pFileSysMan->SHExecute( L"notepad.exe" , strExternalFilename , NULL , 0 , pVFile );
}

*/

#pragma once

#include "MCNamespace.h"

MCNSBEGIN

  // Options
#define VF_TRACKCHANGED 0x0001  // Only Update if temporary external file is changed
#define VF_UPDATE       0x0002  // Update original file 
#define VF_DELETE       0x0004  // Delete the temporary external file
#define VF_DONT_ASK     0x0100  // Don't ask if it should replace 
#define VF_SILENT       0x0200  
#define VF_FINALTHREAD  0x1000  // Do the finalizing in separate thread

  //////////////////////////////////////////////////////////////////////////
class __declspec(novtable) IExternalFile
{
public:
  // filepath to the external file. where it is temporary copied to.
  virtual bool	GetExternalFilename( WCHAR* strFilename, int nMaxLen);

  // Original filepath reference
  virtual bool  GetRefFilename(WCHAR* szFilename, int nLen);

  virtual HANDLE GetProcessHandle() = 0;
  virtual HANDLE GetThreadHandle() = 0;
  virtual DWORD  GetProcessID() = 0;
  virtual DWORD  GetThreadId() = 0;
};

MCNSEND
