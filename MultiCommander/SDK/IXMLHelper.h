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

// Helper interface for reading and writing XML files

typedef DWORD_PTR xmlNodeHandle;

class __declspec(novtable) IXMLHelper
{
public:

  virtual xmlNodeHandle CreateXMLDoc( const WCHAR* szRootNodeName ) = 0;
  virtual bool      FreeXMLDoc(xmlNodeHandle h) = 0;

  virtual xmlNodeHandle Load( const WCHAR* szFilename, bool bKeepBlanks = true) = 0;
  virtual bool      Save( xmlNodeHandle hNode , const WCHAR* szFilename , bool bIndent = true , const char* szEncoding = nullptr) = 0;
  virtual bool      GetEncoding(xmlNodeHandle hNode, char* szEncoding, DWORD dwEncodingLength) = 0; // If xml document was loaded. the get the encoding it was in

  virtual xmlNodeHandle AddNode( xmlNodeHandle hParent , const WCHAR* szName , const WCHAR* szContent = nullptr) = 0;
  virtual xmlNodeHandle AddNodeAndValue( xmlNodeHandle hParent , const WCHAR* szName , const WCHAR* szAttributeName , INT64 nValue , const WCHAR* szContent = nullptr) = 0;
  virtual xmlNodeHandle AddNodeAndValue( xmlNodeHandle hParent , const WCHAR* szName , const WCHAR* szAttributeName , DWORD nValue , const WCHAR* szContent = nullptr) = 0;
  virtual xmlNodeHandle AddNodeAndValue( xmlNodeHandle hParent , const WCHAR* szName , const WCHAR* szAttributeName , const WCHAR* szValue , const WCHAR* szContent = nullptr) = 0;

  virtual bool RemoveNode(xmlNodeHandle hNode) = 0;

  virtual xmlNodeHandle FindElement( xmlNodeHandle hParent , const WCHAR* szElementPath , const WCHAR* szAttributeName = nullptr, const WCHAR* szValue = nullptr) = 0;
  virtual xmlNodeHandle FindChildElement( xmlNodeHandle hParent , const WCHAR* szElementPath, const WCHAR* szAttributeName = nullptr, const WCHAR* szValue = nullptr) = 0;
  virtual xmlNodeHandle FindFirstChildElement( xmlNodeHandle hParent ) = 0;

  virtual xmlNodeHandle GetNextElement(xmlNodeHandle hNode) = 0;
  virtual xmlNodeHandle GetNextElement(xmlNodeHandle hNode, const WCHAR* szElementPath) = 0;

  virtual bool    GetElementName(xmlNodeHandle node, WCHAR* szName, DWORD len) = 0;
  virtual bool    GetContent(xmlNodeHandle hNode, WCHAR* szContent , DWORD nContentMaxSize ) = 0;
  virtual bool    GetContent(xmlNodeHandle hNode, const WCHAR* szElementPath, WCHAR* szContent , DWORD nContentMaxSize ) = 0;

  virtual WCHAR*  GetValueStr( WCHAR* szValue , DWORD nLen , xmlNodeHandle hNode , const WCHAR* szAttributeName ) = 0;
  virtual long    GetValueLong( xmlNodeHandle hNode , const WCHAR* szAttributeName ) = 0;
  virtual INT64   GetValueInt64( xmlNodeHandle hNode , const WCHAR* szAttributeName ) = 0;
  virtual COLORREF GetValueColor(MCNS::xmlNodeHandle hNode, const WCHAR* szAttributeName) = 0;

  virtual bool    GetValue(long* OUT ptValue, xmlNodeHandle hNode, const WCHAR* szAttributeName) = 0;
  virtual bool    GetValue(INT64* OUT ptValue, xmlNodeHandle hNode, const WCHAR* szAttributeName) = 0;

  virtual bool SetValue( xmlNodeHandle node , const WCHAR* szAttributeName , const WCHAR* szValue , bool bCreate = false ) = 0;
  virtual bool SetValue( xmlNodeHandle node , const WCHAR* szAttributeName , DWORD dwValue , bool bCreate = false ) = 0;
  virtual bool SetValue( xmlNodeHandle node , const WCHAR* szAttributeName , long lValue , bool bCreate = false ) = 0;
  virtual bool SetValue( xmlNodeHandle node , const WCHAR* szAttributeName , int  iValue , bool bCreate = false ) = 0;
  virtual bool SetValue( xmlNodeHandle node , const WCHAR* szAttributeName , INT64 nValue , bool bCreate = false ) = 0;
  virtual bool SetValueColor( xmlNodeHandle node , const WCHAR* szAttributeName , DWORD dwValue ,bool bPrefix, bool bCreate = false ) = 0;

  virtual void Release() = 0;
};

MCNSEND
