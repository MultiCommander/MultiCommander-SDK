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
#include "Multi_Type.h"
MCNSBEGIN


class IFileItem;

// Collection that holds XHANDLE items
class __declspec(novtable) IHCollection
{
public:
  virtual void Add( XHANDLE h ) = 0;

  // enum
  virtual void  Reset() = 0; // set internal next pointer to first
  virtual XHANDLE Next()  = 0;
  //
  virtual DWORD    Count() = 0;
  virtual XHANDLE GetAt( DWORD nIndex ) = 0;
  virtual void  Clear();

  // deletes instance
  virtual void Release() = 0;

};

// Collection that holds points items
class __declspec(novtable) IPCollection
{
public:
  virtual void Add(PHANDLE h) = 0;

  // enum
  virtual void  Reset() = 0; // set internal next pointer to first
  virtual PHANDLE Next() = 0;
  //
  virtual DWORD    Count() = 0;
  virtual PHANDLE GetAt(DWORD nIndex) = 0;
  virtual void  Clear();

  // deletes instance
  virtual void Release() = 0;

};

// Text => Value - map like collection
class __declspec(novtable) ITextValueCollection
{
public:
  virtual void  Add( const WCHAR* strText, INT64 value ) = 0;

  virtual bool Exists(const WCHAR* strText) = 0;
  virtual DWORD Count() = 0;
  virtual void  Clear() = 0;

  virtual BOOL  GetAt(DWORD nIndex, WCHAR* strText, int strMaxLen,INT64& value) = 0;

  // deletes instance
  virtual void Release() = 0;

};

// Text => Text - map like collection
class __declspec(novtable) IKeyValCollection
{
public:
  // Multiple KEYS are allowed.
  virtual void  Add( const WCHAR* strKey , const WCHAR* strVal ) = 0;

  virtual void  Remove(const WCHAR* strKey) = 0;
  // replace a key (if it exists) with a new value
  virtual void  Replace( const WCHAR* strKey , const WCHAR* strVal ) = 0;

  virtual bool  KeyExists(const WCHAR* szKey, bool bCaseSensative = true) = 0;
  virtual int   GetKeyIdx(const WCHAR* strKey, bool bCaseSensative = true) = 0;

  virtual DWORD Count() = 0;
  virtual BOOL  GetAt( DWORD nIndex , WCHAR* strKey , WCHAR* strVal , int strMaxLen ) = 0;
  // get the value that matches strKey ( not case sensitive )
  virtual BOOL  Get( const WCHAR* strKey , WCHAR* strVal , int strMaxLen , BOOL bCaseSensative ) = 0;

  virtual void  Clear() = 0;
  // deletes instance
  virtual void Release() = 0;

};

// Text collection
class __declspec(novtable) IValueCollection
{
public:
  virtual void  Add_Last( const WCHAR* strVal ) = 0;

  // Will replace all existing item with the one in pItems
  virtual DWORD CopyFrom(const IValueCollection* pItems) = 0;
  virtual DWORD MoveFrom(IValueCollection* pItems) = 0;

  virtual void  Insert( const WCHAR* strVal , DWORD nIndex ) = 0;

  virtual void  Remove( DWORD nIndex ) = 0;
  virtual void  Remove( const WCHAR* strVal ) = 0;
  virtual void  RemoveAll() = 0;

  virtual bool  Find( const WCHAR* strVal , /*[out]*/ DWORD* pIndex) = 0;
  virtual bool  FindNoCase( const WCHAR* strVal , /*[out]*/ DWORD* pIndex) = 0;

  virtual DWORD Count() = 0;
  virtual bool  GetAt( DWORD nIndex , WCHAR* strVal , int strMaxLen ) = 0;

  // Reverse the data in the Collection
  virtual void  Reverse() = 0;

  // Sort the collection
  virtual void  Sort() = 0;

  virtual void  RemoveDuplicate(bool bIgnoreCase) = 0;
  // Emprt the collection.
  virtual void  Clear() = 0;

  // deletes instance
  virtual void Release() = 0;

};

// IFileItem Collection
class IFileItem;
class __declspec(novtable) IFileItemCollection
{
public:
  virtual ~IFileItemCollection() { }

  virtual void     Add( IFileItem* hFileItem ) = 0;
  virtual void     AddFront( IFileItem* hFileItem ) = 0;
  virtual DWORD    Add( IFileItemCollection* pFileItems) = 0;

  // Add all item that not alredy exists from pFileItems 
  // It will check if every item exists before adding it
  virtual DWORD    AddUniqe(IFileItemCollection* pFileItems) = 0;

  virtual DWORD    Count() const = 0;
  virtual IFileItem* GetAt( DWORD nIndex ) = 0;
  virtual const IFileItem* GetAt(DWORD nIndex) const = 0;
  virtual void    Clear() = 0;

  // Count how many items we got with matching attribute flag.
  virtual DWORD   CountMatchingItems(DWORD dwAttribute) = 0;

  // returns (DWORD)-1 if item is NOT found
  virtual DWORD       Find( IFileItem* pFileItem ) = 0;
  virtual IFileItem*  Find( const WCHAR* strFilename , bool bCaseSensative = false, bool bRemove = false ) = 0;

  // Return how many items it found that it inserted into pFoundItems
  // bClearMatches will set found items that are inserted into pFoundItems to NULL. (thread safe, But not if you are removing.)
  virtual DWORD       FindItems(IFileItemCollection* pFoundItems, const WCHAR* strMatch, bool bRemove = false , bool bClearMatching = false, bool bFastCheck = false) = 0;

  virtual bool    Remove( IFileItem* pFileItem ) = 0;

  // Remove all matching items (by ptr)
  virtual DWORD    Remove( IFileItemCollection* pItems ) = 0;

  // deletes instance
  virtual void Release() = 0;
};

MCNSEND