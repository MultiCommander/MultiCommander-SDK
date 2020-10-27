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

class IFileItem;

// Generic so move to ownfile
enum class OperationState
{
  Ready = 1, // Awaiting start. Ready to start
  Starting,  // In the process of starting up the operation
  Running,   // Operation is running
  Pausing,   // In the processes of pausing operation
  Paused,    // Operation paused
  Stopping,  // in the process of stopping operation
  Stopped,   // Operation Stopped
  Finished,  // Operations Finsihed, (and stopped)
};


class __declspec(novtable) IOperationControl
{
public:
  virtual void Operation_Stop() = 0;
  virtual void Operation_TogglePause() = 0;
  virtual bool Operation_IsPaused() = 0;
};

//////////////////////////////////////////////////////////////////////////
// the IFileSearchResult is created by the extension and sent to the 
// framework with the search request. the Framework will call this interface
// when it find a search result. this call are sent syncronious. 
class __declspec(novtable) IFileSearchCallback
{
public:

  virtual ~IFileSearchCallback() {}

  // Search operation starting, return false to abort start
  virtual bool OnStart() = 0;
  virtual void Stopped( bool bInterupted ) = 0; // Is called when the search request is completed or stopped
  virtual void Paused() = 0;                    // is called when file search process is paused.

  //virtual bool OnNewSearchStarting() = 0; // A new file search is about to start. Prepare the UI (eg. clear old results) Return false to abort
  virtual bool OnFileSearchBegin(bool appendResult) = 0; // above function is obeslote

  virtual void OnStateChange(OperationState state) = 0;
  virtual bool IsAbortRequested() = 0;
  // Is called to let extension know what path us currently being scanned. return false to skip that folder.
  // nCount is a Timer how many time this has been called with current path
  virtual bool Searching(const WCHAR* strPath, DWORD nCount ) = 0; 

  virtual void FoundMatch( IFileItem* pItem ) = 0;  //  A fileitem matching the search criteria has been found.
  virtual void ProcessedItems(size_t processedItems) = 0;
  virtual void OnFinsihed(bool bUserAborted) = 0; // find files process is completed
  
  virtual void SetOperationControl(MCNS::IOperationControl* pControl) = 0;

  virtual void Release() = 0;
};

// File search filter - flags
#define FSF_FINDMODE_CONTAINS 1
#define FSF_FINDMODE_WILDCARD 2
#define FSF_FINDMODE_REGEXP   3

#define FSF_SIZE_SMALLERTHEN  1   //  Smaller Then X
#define FSF_SIZE_BIGGERTHEN   2   //  Bigger then X
#define FSF_SIZE_EQUAL        3   //  Same Size as X
#define FSF_SIZE_AROUND       4   //  Around the same size as X..  X +/- 5%

#define FSF_CONTENT_CASE    0x0001  // Case Sensitive
#define FSF_CONTENT_WORD    0x0002  // Whole word
#define FSF_CONTENT_NOT     0x0004  // Search is true if content is NOT found
#define FSF_CONTENT_HEX     0x0008  // Input string is Hex value
#define FSF_CONTENT_REGEX   0x0010  // Input string is an RegExp
#define FSF_CONTENT_AUTO    0x0100
#define FSF_CONTENT_ASCII   0x0200  // force ASCII matching
#define FSF_CONTENT_UNICODE 0x0400  // force UniCode matching
#define FSF_CONTENT_UTF8    0x0800  // force UTF8 matching
#define FSF_CONTENT_BINARY  0x1000  // force Binary matching


// OBSOLETE
class __declspec(novtable) IFileSearchHandler
{
public:
  // Clear all cached data
  virtual void Clear() = 0;

  virtual void ClearSearchCriteria() = 0;

  //  " "(space) separated list with or without wild cards for search for
  virtual void SearchFor( const WCHAR* strSearchFor , DWORD dwLookForMode) = 0;

  // ; separated list where to search.
  virtual void SearchIn( const WCHAR* strSearchIn ) = 0;
  virtual void SubLevelLimit( int nLimit ) = 0;
  virtual void SetLookInFilter( const WCHAR* strFilter ) = 0;

  virtual void SetFindContent( const WCHAR* strFindContent ) = 0;
  virtual void SetFindContentFlags( DWORD nFlags ) = 0;

  virtual void SetSearchCallback( IFileSearchCallback* pResult ) = 0;

  virtual void SetDateFrom( const FILETIME* ftFrom ) = 0;
  virtual void SetDateTo( const FILETIME* ftTo ) = 0;
  virtual void SetOlderThen( UINT nOld , short nUnit ) = 0;

  // Attribute matching is in two part. the direct match or not match
  // and some attributes can be whatever. ( set or unset to be a match )
  virtual void SetAttributes_Match(UINT64 dwAttributes ) = 0;
  virtual void SetAttributes_Whatever(UINT64 dwAttributes) = 0;

  virtual void SetFileSize( INT64 nFileSizeLimit , DWORD nFlags ) = 0;

  virtual void SetSearchInResult( bool b ) = 0;
  virtual void SetSearchInCache( bool b ) = 0;

  virtual bool HasResult() = 0;
  virtual bool HasCache() = 0;
};

// Content Matching Flags = CMF
typedef DWORD CMFLAGS;
const CMFLAGS CMF_AUTO    = 0x00000010L;
const CMFLAGS CMF_ASCII   = 0x00000020L;
const CMFLAGS CMF_UNICODE = 0x00000040L;
const CMFLAGS CMF_UTF8    = 0x00000080L;
const CMFLAGS CMF_BINARY  = 0x00000100L; // 0x1f0 & to filter out bits for format
const CMFLAGS CMF_WHOLEWORDS = 0x00002000L;
const CMFLAGS CMF_IGNORECASE = 0x00004000L;
const CMFLAGS CMF_CONTENT    = 0x01000000L;
const CMFLAGS CMF_REGEX      = 0x02000000L;
const CMFLAGS CMF_HEX        = 0x04000000L;

// Bit masking to get flags groups
const CMFLAGS CM_FORMATS   = 0x1F0;
const CMFLAGS CM_MATCHTYPE = 0x0F000000L;
const CMFLAGS CM_OPTIONS   = 0x0000F000L;


// bNot variable is to invert the result of the match.
// If a rule is not a match.. Then it will be a match.
class __declspec(novtable) IFindFilesFilter
{
public:
  enum MatchType
  {
    Wildcard = 1,
    Contains,
    RegEx,
  };

  virtual void SubLevelLimit(int n) = 0;
  virtual int  SubLevelLimit() = 0;

  // Multiple paths should be separated with ; | or ""
  virtual void ExcludeFolders(const wchar_t* szFolder) = 0;

  // Multiple paths should be separated with ; | or ""
  virtual void SearchInFolders(const wchar_t* szFolders) = 0;

  // Multiple matches should be separated with ; | or "" (Regex do not allow for multiple)
  virtual void AddFileNameMatch(MatchType matchType, const wchar_t* szMatch, bool bNot) = 0;

  virtual void AddFileContentMatch(const wchar_t* szContent, CMFLAGS dwFlags, bool bNot) = 0;

  virtual void AddFileSize_Min(size_t nMin, bool bNot) = 0;
  virtual void AddFileSize_Max(size_t nMax, bool bNot) = 0;
  
  virtual void AddFileTime_Min(SYSTEMTIME* pST, bool bNot) = 0;
  virtual void AddFileTime_Max(SYSTEMTIME* pST, bool bNot) = 0;
  virtual void AddFileTime_Range(int n, bool bNot) = 0;

  // "TODAY" , "YESTERDAY" , "THISWEEK" , "THISMONTH", "THISYEAR", 
  virtual void AddFileTime_Constant(const wchar_t* szTime, bool bNot) = 0;

  // ZFF_  Attribute flags
  // bSet if those flag should be set, bSet == false if the flags should NOT be set
  virtual void AddFileAttribute(DWORD dwAttribute, bool bSet, bool bNot) = 0;

  // Relese object. NOT need if filter is sent to IFindFilesHandler::SetFilter
  // then it owns it.
  virtual void Release();
    
};

// Shows FindFiles UI
class __declspec(novtable) IFindFilesHandler
{
public:
  virtual void Init(HWND hParent, IFileSearchCallback* pCallback, const TCHAR* szInitPath, const WCHAR* szFindFilesFiltersFilename) = 0;
  virtual bool IsInitilized() = 0;
  virtual void ResultInVirtualDevice(bool b) = 0;
  virtual void ShowFindFilesDlg(HWND hParent) = 0;

  virtual void ShowHideFindFilesDlg(bool bShow) = 0;
  virtual void ToggleShowHideFindFilesDlg(bool bReloadFilter = false, MCNS::ZHANDLE hCenterWin = 0) = 0;

  virtual void HideDialog(bool b) = 0;
  virtual bool IsDialogVisible() = 0;

  virtual IFindFilesFilter* CreateFindFilesFilter() = 0;
  virtual void SetFilter(IFindFilesFilter* pFilter) = 0;

  // Get the Match Content text
  virtual bool GetMatchContentText(wchar_t* szText, size_t len) = 0;
  
  // Request to start/stop/pause/continue findfiles operation. 
  // This call is async. operation enter that state when it can sometimes after the call have returned.
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Pause() = 0;
  virtual void Continue() = 0;

  // return true if a search is running/pause, Return false only if state is ready/stopped/finished
  virtual bool IsSearchActive() = 0;
  virtual OperationState State() = 0;

  virtual void Release() = 0;
};


MCNSEND
