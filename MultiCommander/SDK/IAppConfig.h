/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
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

// Application Configuration Interface
// Use to get/set config

/*
  WCHAR szValue1[100];
  szValue1[0] = '\0';
  IAppConfig* pConfig = m_pAppInterface->GetAppConfig();
  ZHANDLE hConfigRoot = pConfig->GetConfigElement( NULL ,  L"config" );
  pConfig->GetConfigValue(hConfigRoot, L"setting1", L"value", szValue1, _countof(szValue1));
*/

struct FontAndIconSettings
{
  wchar_t szFontName[100];
  long fontStyle = 0;
  int fontSize = 0;
  int cxIconSize = 0;
};


class IAppConfig
{
public:

  virtual ZHANDLE  OpenConfig( long ModuleID = -1 , BOOL bSet = TRUE , BOOL bGetConfigElement = TRUE) = 0;
  virtual BOOL     CloseConfig( ZHANDLE hHandle ) = 0;

  virtual bool     VerifyConfig() = 0;

  // Validate existance of a config file, The path MUST be under the MC config path or the "restore" default file will not work
  virtual bool     ValidateExistence(const wchar_t* szFilename, bool bRestoreIfMissing) = 0;
  // if bForceWrite is true then conifg is saved even if no change are detected.
  virtual BOOL     SaveConfig( long ModuleID , bool bForceWrite = false ) = 0; 
  virtual bool     IsConfigChanged(long ModuleID) = 0;
  virtual BOOL     CreateConfig() = 0;

  virtual bool     SetConfigElementMatching( const WCHAR* strElementPath , short nMatchType , const WCHAR* strMatchName , const WCHAR* strMatchValue ) = 0;

  virtual int      CountConfigGroupValues( ZHANDLE hHandle ,  const WCHAR* sGrp ) = 0;

  virtual ZHANDLE  CreateConfigElement( ZHANDLE hParent , const WCHAR* strElement ) = 0;
  virtual bool     RemoveConfigElement( ZHANDLE hElement ) = 0;

  virtual ZHANDLE  GetConfigElement( ZHANDLE hHandle , const WCHAR* strElement , bool bCreate = false, bool bSilent = false) = 0;
  virtual ZHANDLE  GetNextElement( ZHANDLE hHandle , const WCHAR* strElement ) = 0;

  virtual BOOL     GetConfigText( ZHANDLE hHandle , const WCHAR* strElement , WCHAR* strOut , long lstrSize ) = 0;
  virtual BOOL     GetConfigText( ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName, const WCHAR* strValue, WCHAR* strContent, DWORD dwContentLen ) = 0;
  virtual BOOL     SetConfigText( ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName, const WCHAR* strValue, const WCHAR* strContent ) = 0;

  virtual BOOL     GetConfigValue( ZHANDLE hHandle , const WCHAR* strElement , int nIndex , /*out*/WCHAR* sKey , /*out*/ WCHAR* sValue , long MaxSize ) = 0;
  virtual BOOL     GetConfigValue( ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , WCHAR* OutString , long MaxSize ) = 0;

  // If bCreate is true, Value will be added if not found. And default value will be set and returned.
  virtual BOOL     GetConfigValue_String( ZHANDLE zHandle , const WCHAR* strElement , const WCHAR* strName , WCHAR* /*IN & OUT*/strValue , int lMaxSize , bool bCreate = false) = 0;
  virtual BOOL     GetConfigValue_Long( ZHANDLE zHandle , const WCHAR* strElement , const WCHAR* strName , /*OUT*/ long& lValue , long lDefault = 0 , bool bCreate = false ) = 0;
  virtual BOOL     GetConfigValue_CR( ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , COLORREF &nCR , COLORREF crDefault = -1, bool bCreate = false ) = 0;
  virtual COLORREF GetConfigValue_CR( ZHANDLE zHandle, const WCHAR* strElement , const WCHAR* strName, COLORREF crDefault = -1, bool bCreate = false ) = 0;

  virtual bool     SetConfigValue(ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , const WCHAR* strValue , bool bCreate = false ) = 0;
  virtual bool     SetConfigValue(ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , long  nValue , bool bCreate = false ) = 0;
  virtual bool     SetConfigValue(ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , DWORD nValue , bool bCreate = false ) = 0;
  virtual bool     SetConfigValue(ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , INT64 nValue , bool bCreate = false ) = 0;
  virtual bool     SetConfigValue_CR(ZHANDLE hHandle , const WCHAR* strElement , const WCHAR* strName , COLORREF nColorRef , bool bCreate = false ) = 0;

  virtual bool GetExplorerPanelFontAndIconSettings(MCNS::FontAndIconSettings* pFontAndIcon) = 0;

};
MCNSEND