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

/*


History 
--------


*/
#ifndef _ZILOGGER_IF_
#define _ZILOGGER_IF_

#pragma once

#include "MCNamespace.h"
#include "IStatus.h"
MCNSBEGIN

namespace LogOptions
{
  enum Encoding { ASCII = 1, Unicode= 2, UTF8 = 3 };
};

class __declspec(novtable) ILogger
{
public:

  // Connect the logger to the exiting FileSystemLog

  virtual void InitFileSystemLog(); 
  // bCreateNew : true if it already should create a new. if a log with existing name already exist a new log with same name will be created. else it will use same log.
  // szLogFilename : Filename of the log. (NO path should be giving. the log will be stored in the log folder. )
  virtual bool Init(bool bCreateNew, const WCHAR* szLogName, DWORD nLogSizeKB, bool bSaveToFile, const TCHAR* szLogFilename = NULL, LogOptions::Encoding encoding = LogOptions::ASCII) = 0;

  // Show the Log view. (There is many log windows in the tab log window. this will bring the current logwindow onfront)
  virtual void ShowLog() = 0;

  // Hide/Show the entire log window
  virtual void ShowLogWindow(bool b) = 0;

  virtual void TimeStamp(bool bTimeStamp, bool bIncludeMilliseconds = false) = 0;
  virtual bool TimeStamp() = 0;

  virtual bool Log(const WCHAR* szMessage, ...) = 0;
  virtual bool Log(const CHAR* szMessage, ...) = 0;

  virtual bool Log(LogLevel level, const WCHAR* szMessage, ...) const = 0;
  virtual bool Log(LogLevel level, const CHAR* szMessage, ...) const = 0;

  // force a redraw now
  virtual void Redraw() = 0;

  // release and delete object
  virtual void Release() = 0;

};

MCNSEND

#endif