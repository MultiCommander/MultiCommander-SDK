/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2025 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 */

#ifndef _ISTATUSHANDLER_H_
#define _ISTATUSHANDLER_H_
#pragma once

#include "MCNamespace.h"


MCNSBEGIN

enum class OverwriteOption;

enum class OverwriteResult
{
  NotProcessed = -1,
  Overwrite = 1,
  DoNotOverwrite = 2,
  Retry = 3,
  Abort = 20
};

enum class FileOpErrorResult
{
  NotProcessed = -1,
  Overwrite = 1,
  Skip = 2,
  Retry = 3,
  Abort = 20,
  Ignore,
  Continue,
  SkipAll,
  RetryShared, // special
  OverwriteIf,
  OverwriteProtected,
  OverwriteAllProtected,
};

enum class FileOpErrorHint
{
  NoHint = 0,
  OpeningTargetFile, // Error happened when opening the target file
  OpeningSourceFile, // Error happened when opening the source file
  ReadingDataFromSourceFile, // Error happened when reading data from the source file
  ReadingDataFromTargetFile, // Error happened when reading data from the target file
  WritingDataToTargetFile, // Error happened when writing data to the target file
};

  //////////////////////////////////////////////////////////////////////////

#ifndef _MULTILOGLEVELS_
#define _MULTILOGLEVELS_
enum class LogLevel
{
  _DEBUG_      = 1, // Default Debug level
  //_SUCCESS = 7, // Something has Succeeded
  _INFO    = 10,// Info
  _WARNINIG= 20,// Warnings 
  _ERROR   = 30,// Errors
  _CRITICAL= 40,// Critical error
  _ALERT   = 50,//
  _FATAL   = 100,// Fatal Errors, program can not continue after this
  _NOLOG   = 500 // Set this as log level to NOT log anything
};
#endif

#define LOGWIN_DEBUG	0
#define LOGWIN_FILEOP	1

struct FileOperationErrorContext;
class IFileInfo;

class IMultiLogger
{
public:
  virtual void Log( UINT nLogID, LogLevel nLevel, const char*  strText, ... ) = 0;
  virtual void Log( UINT nLogID, LogLevel nLevel, const WCHAR* strText, ... ) = 0;

  virtual bool CreateLogWindow( UINT nLogID ) = 0;

};

//////////////////////////////////////////////////////////////////////////
// This interface can be used to show status in the mainwindows status bar
class IStatusHandler : public IMultiLogger // change name to GUI Handler ??
{
public:

  // Text that will be show on the bottom status bar.
  virtual void SetInfoTextA( const CHAR* strText ) = 0;
  virtual void SetInfoTextW( const WCHAR* strText ) = 0;

  // no other thread can change the progressbar values until it is unlocked
  virtual void LockProgressBar() = 0;
  virtual void UnlockProgressBar() = 0;

  // Progress Bar on status bar
  virtual void SetProgressMinMaxValue( DWORD nMin = 0 , DWORD nMax = 100 ) = 0;
  virtual void SetProgressValue( DWORD nCurrentValue ) = 0;

  // Loop ProgressBar
  virtual void StartLoopProgressBar() = 0;
  virtual void StopLoopProgressBar() = 0;

  // Should not be in this interface. Bad workaround
  //[[deprecated]] virtual DWORD ShowErrorDlg( int nErrorCode , DWORD dwBtnFlags[4] , WCHAR* strErrorText = NULL ) = 0;
  //virtual bool  ShowAskPasswordDlg(const WCHAR* szCaption, const WCHAR* szText, const WCHAR* szCueText, WCHAR* szPassword, int nPasswordLen) = 0;
};



// Running status.. used with passive thread notification
#define AR_RUNSTAT_RUNNING 10
#define AR_RUNSTAT_STOP    20
#define AR_RUNSTAT_SKIP    30
#define AR_RUNSTAT_ABORT   40
#define AR_RUNSTAT_PAUSE   50
//#define AR_RUNSTAT_FILEDONE 60

//
// When using InternalOperation in volume interface. Use this interface to communicate with progress bar
//
//
//
struct ZFILEINFO;

#define PATHMONITOR_DELETE      0x00001000L
#define PATHMONITOR_ADDED       0x00002000L
#define PATHMONITOR_CHANGED     0x00004000L
#define PATHMONITOR_RENAME      0x00008000L // Only valid in Ignore list. not annouce list

#define PATHMONITOR_AUTOREMOVE  0x00100000L // AutoRemove From Ignorelist when used

/*
Exmaple of usage

pProgress->EnableButtonSkip(FALSE);


pProgress->Set_From( strFromFile.c_str() );
pProgress->Set_To( strWriteFile );
if( pItem )
pProgress->Set_Current_Max( pItem->Get_FileSize() );
pProgress->Set_Current_Now( 0 );
pProgress->Operation_Start();
pProgress->Update();
pProgress->UpdateNames();

// To internal work..

if( pItem )
pProgress->Add_Total_Done( pItem->Get_FileSize() );

pProgress->Operation_Finished();
pProgress->Update();

*/
enum FileOperationResult : int;

// TODO: another name. It do alot more then just progress

// Suggested names : IFileOperationInteraction , 
//
// parts IFileOperationsProgress, IFileSystemActions (need better name)
class IProgress
{
public:

  // Prepare a new file in the progress
  virtual void Prepare(const wchar_t* szFrom, const wchar_t* szTo, UINT64 size) = 0;
  virtual void InitRebuild(const wchar_t* szText, UINT64 maxSize) = 0;

  virtual void Set_From(const WCHAR* strText) = 0;

  virtual const wchar_t* Get_From() = 0;
  virtual const wchar_t* Get_To() = 0;

  virtual void Set_To(const WCHAR* strText) = 0;

  virtual void Set_Current_Max(UINT64 nMax) = 0;
  virtual void Set_Current_Now(UINT64 nNow) = 0;

  // Will update total done, based on previous call, 
  virtual void Set_Total_Processed_By_Current(INT64 currentDone) = 0;

  // ADD nDone Value to both to both the Current and Total ,and update if needed
  virtual void Add_Done(UINT64 nDone, BOOL bContinuesViewUpdate = TRUE, BOOL bForceUpdateProgress = FALSE) = 0;

  // Will only add nDone to the Current counter. Will Update if needed
  virtual void Add_CurrentDone(UINT nDone, BOOL bContinuesViewUpdate = TRUE) = 0;

  virtual void Set_Total_Max(UINT64 nMax) = 0;
  virtual void Add_Total_Max(UINT64 nMax) = 0;
  virtual void Add_Total_Done(UINT64 nNow, BOOL bUpdate = FALSE) = 0;
  virtual void Set_Total_Done(UINT64 val) = 0;

  virtual void Add_Skipped(DWORD nItems) = 0;
  // Add Items Done.
  virtual void Add_ItemsDone(DWORD nItems) = 0;
  virtual void Add_TotalItems(DWORD nItems) = 0;

  // Init - New items Will preprare for new current item in progress bar.. (no need to call Operation_Start() )

  virtual void OnInitCurrentItem(UINT64 nCurrentItemMaxValue) = 0;
  // use this before and after a file is copied. Without them the copy speed and time will not be calculated correct
  virtual void Operation_Start() = 0;
  virtual void Operation_Update(DWORD dwBytesCount) = 0; // no of bytes been written/read . used to calculate read/write speed
  virtual void Operation_Finished(bool bUpdate = false, bool bIncItemsDone = true) = 0;

  // when information have changed and you want the progress dialog to update.. run this
  virtual void Update() = 0;
  virtual void UpdateNames() = 0;
  virtual void EnableButtonSkip(bool b) = 0;

  /*	Update Filesystem Cache and notify views about changes */

  ///// MOVE OVER TO A FILESYSTEMINTERFACE ///
  // strPathItem - Path to an path item that should update when UpdatePath() is run
  // bRemoved    - If item is removed
  // bChanged    - If Item is changed (in size/date/attributes)
  virtual void UpdatePathItem(const WCHAR* strPathItem, bool bRemove, bool bChanged, bool bRefresh) = 0;
  virtual void UpdatePathItem(const CHAR* strPathItem, bool bRemove, bool bChanged, bool bRefresh) = 0;
  virtual void UpdateParentPathItem(const WCHAR* strPathItem, bool bRemove, bool bChanged, bool bRefresh) = 0;
  virtual void UpdateParentPathItem(const CHAR* strPathItem, bool bRemove, bool bChanged, bool bRefresh) = 0;
  virtual void UpdatePath(bool bForce) = 0;

  // bStart - true to Start monitor a path, false to stop
  // used from BatchFileOperations_Start in IVolume. to stop unnecessary updates
  virtual void PathMonitor(const WCHAR* strPathItem, bool bStart) = 0;
  virtual void PathMonitor_IgnoreChangeOnFile(const WCHAR* strPathItem, DWORD nFlags, bool bSet = true) = 0;
  virtual void PathMonitor_AnnounceChangeOnFile(const WCHAR* strPathItem, DWORD nFlags) = 0;

  // <param name="bForce">Force a Update. path will be rescaned</param>
  // <param name="bTarget">Update Target view if true else Source view </param>
  virtual void UpdateViews(bool bForce, bool bTarget) = 0;

  // <param name="bForce">Force a Update. path will be rescaned</param>
  // <param name="strPath">Path to Update</param>
  virtual void UpdateViews(bool bForce, const WCHAR* strPath) = 0;

  // AR_RUNSTAT_RUNNING 
  // AR_RUNSTAT_STOP    
  // AR_RUNSTAT_SKIP    - Skip Requested
  // AR_RUNSTAT_ABORT   - Abort Requested
  // AR_RUNSTAT_PAUSE   - Paused Requested
  virtual int GetRunningStatus() = 0;
  virtual int HandlePause() = 0; // will HOLD if running state is AR_RUNSTAT_PAUSE, then return current running state.

  // is status is anything except OK. then set status for that fileoperation 
  virtual void SetResult(DWORD_PTR refID, FileOperationResult dwResult, DWORD dwError = 0) = 0;

  virtual HWND GetHWND() = 0;
};

enum class YNA
{
  Yes,
  No,
  Ask,
};

class IFileSystemOperation
{
public:
  virtual DWORD FileOpOptions() const = 0;

  virtual YNA  DeleteIncomplete() const = 0;

  virtual void SetKeepALLBroken(bool b) = 0;
  virtual void SetDeleteALLBroken(bool b) = 0;

  // check if option is enabled
  virtual bool ContinueOnError() const = 0;

};

class IFileOperationErrorHandling
{
public:
  virtual FileOpErrorResult OnHandleError(MCNS::FileOperationErrorContext& errorContext) = 0;

  // nIcon = MB_ICON____ 
  // dwBtnFlags
  // 
  //  DWORD dwBtnFlags[4];
  //  dwBtnFlags[0] = ED_RENAME;
  //  dwBtnFlags[1] = ED_NONE;
  //  dwBtnFlags[2] = ED_OVERWRITE;
  //  dwBtnFlags[3] = ED_ABORT;
  //
  // pButtons is ASSUMED to be 4xDWORD pointer. ( DWORD myButtons[4] )
  virtual int ShowCustomErrorDialog(DWORD* pButtons, const WCHAR* strErrorText, const WCHAR* strCaption = nullptr, UINT nIcon = 0) = 0;

  virtual DWORD ShowErrorDlg(int nErrorCode, DWORD* pButtons, const WCHAR* strErrorText, const WCHAR* strCaption, MCNS::ZFILEINFO* pFileInfo = nullptr, UINT nIcon = 0) = 0;

  // Windows Error code to MC error code
  virtual DWORD TranslateErrorCode(DWORD dwErrorCode) = 0;
};

class IDialogService
{
public:
  // dwBtnFlags
// 
//  DWORD dwBtnFlags[4];
//  dwBtnFlags[0] = ED_RENAME;
//  dwBtnFlags[1] = ED_NONE;
//  dwBtnFlags[2] = ED_OVERWRITE;
//  dwBtnFlags[3] = ED_ABORT;
//
// pButtons is ASSUMED to be 4xDWORD pointer. ( DWORD myButtons[4] )
  virtual DWORD ShowErrorDlg(int nErrorCode, DWORD dwBtnFlags[4], const WCHAR* strErrorText) = 0;
  virtual bool  ShowAskPasswordDlg(const WCHAR* szCaption, const WCHAR* szText, const WCHAR* szCueText, WCHAR* szPassword, int nPasswordLen) = 0;

  virtual DWORD ShowMessageBox(const WCHAR* strText, const WCHAR* strCaption, UINT nType) = 0;
};

class IAppConnection
{
public:
  virtual  ~IAppConnection() = default;

  // Set text that show up in info area in the statusbar of main window
  virtual void SetInfoText(const WCHAR* strText) = 0;

  virtual const WCHAR* GetMCText(long textId) = 0;
  virtual const WCHAR* GetVolumeText(short volumeId, long textId) = 0;

  virtual IMultiLogger* Logger() = 0;
  virtual IDialogService* DialogService() = 0;
};

class IFileOperationExtensionContext 
{
public:
  virtual                              ~IFileOperationExtensionContext() = default;

  // User have Request to abort/pause the file operation (copy/move/delete)
  virtual bool                         AbortRequested() = 0;
  virtual bool                         PauseRequested() = 0;

  virtual MCNS::IFileInfo*             CreateFileInfo(const TCHAR* szFilepath) = 0;
  virtual MCNS::IFileInfo*             CreateFileInfo(MCNS::ZFILEINFO* pFileInfo) = 0;

  virtual const wchar_t* GetExtensionText(long textID) = 0;

  virtual IProgress* Progress() = 0;
  virtual IFileSystemOperation*        FileSystemOperations() = 0;
  virtual IFileOperationErrorHandling* FileOperationErrorHandling() = 0;
  virtual IAppConnection*              AppConnection() = 0;

	// duplicated from IAppConnection
  virtual IMultiLogger* Logger() = 0;
  virtual IDialogService* DialogService() = 0;
};


MCNSEND

#endif