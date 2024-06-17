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

/*
  A Command definition.

  This interface describes how the command works and what kind of parameters it accepts.
  The command can then be used from Script (CustomCommand or from MultiScript)
  
*/

#include "MCNamespace.h"
#include "MultiApp_Def.h"
MCNSBEGIN

// This flag is what option to give the user when creating a custom command

//  Include data into the command when it is run. 
//  If set the then use pCustomCommand->GetContentData(); to get the data that is set
//  (Most of The CCO_PARAM_ are OBSOLETE.) 
#define CCO_PARAM_FROM_SOURCE 0x00000000L // Will take parameters from Source view ( Default, do not need to specify )
#define CCO_PARAM_FROM_TARGET 0x00000001L // Will take parameters from Target view
#define CCO_PARAM_FILEITEM    0x00000010L // Accept FILEITEM  as parameter ( IFileItem* )
#define CCO_PARAM_FILEITEMS   0x00000020L // Accept FILEITEMS as parameter ( IFileItemCollection* )
#define CCO_PARAM_FILEPATH    0x00000040L // Accept PATH to a file as parameter
#define CCO_PARAM_FOLDERPATH  0x00000080L // Accept PATH to a folder as parameter
#define CCO_PARAM_PATH        0x00000100L // Accept PATH to file or folder as parameter
#define CCO_PARAM_FILES       CCO_PARAM_PATH|CCO_PARAM_FILEITEMS|CCO_PARAM_FILEITEM // Accept All 
#define CCO_PARAM_SINGELFILE  CCO_PARAM_PATH|CCO_PARAM_FILEITEM // Accept All 

#define CCO_RUN_INDIVIDUALLY  0x00001000L // Allow command to be run on ALL selected Items individually. ( command will loop all selected items and run then one by one)
#define CCO_ASK_BEFORE_RUN    0x00002000L // Ask if user are sure before running command.
#define CCO_CONTINUE_ON_ERROR 0x00004000L // If running multiple command. and one command fails. then continue anyway.
#define CCO_QUITE             0x00100000L // Allow option to run in Quite mode.
#define CCO_NOGUI             0x00200000L // Allow option to run without GUI
//#define CCO_SINGLEITEM  0x00004000L
  

// Custom Command option types ( Customized options )
#define CCOF_BOOL     0x0000010L    // Option is a Bool . enter it is there or Not
#define CCOF_TEXT     0x0000020L    // Option has a Text value as parameter ( eg FILTER= )
#define CCOF_VALUE    0x0000040L    // Option has a numeric value as parameter ( eg MAX=34 )
#define CCOF_COLOR    0x0000080L    // Option is a color ( eg BGCOLOR=#443321 )
#define CCOF_REQUIRED 0x0001000L    // This options is required to use the Command

/*

Example

pDef = m_pCustomCommands->DefineCommand( "MyCmd", CMD_MY_CMD, CCO_QUITE, ZCF_TARGET_ANY);
if( pDef )
{
  pDef->SetDescription( pAppInterface->GetText(MAKETEXTID('c',120)) );
  pDef->SetOption( "FILE"  , CCOF_TEXT , pAppInterface->GetText(MAKETEXTID('c',111)));  // FILE="<text>" parameter
}

// 
BOOL MyExt::OnCustomCommand( ICustomCommand* pCustomCommand )
{
  if( pCustomCommand->GetID() == CMD_MY_CMD )
  {
     if( pCustomCommand->HasOption( "FILE" ) )
     {
       STLString strPath = pCustomCommand->GetOptionValue( "FILE" );
       OnMyCmd(strPath);
     }
  }

*/

class __declspec(novtable) ICustomCommand
{
public:
  virtual bool          HasOption( const char* strOption ) const = 0;
  virtual const WCHAR*  GetOptionValue( const char* option ) = 0;

  virtual const char*   GetName() const = 0;
  virtual DWORD         GetID() const = 0;    // Extension command ID.
  virtual const WCHAR*  GetDescription() const = 0;

  virtual eContentDataType  GetContentType() const = 0;
  virtual DWORD_PTR         GetContentData() const = 0;

  virtual void SetErrorCode(DWORD dwError) = 0;

  virtual void SetScriptResult(const WCHAR* szValue) = 0;
};

class __declspec(novtable) ICustomCommandDef
{
public:

  virtual bool SetOption( const char* strOption, DWORD nFlags , const WCHAR* strDesc ) = 0;
  virtual void SetDescription( const WCHAR* description ) = 0;
};

class __declspec(novtable) ICustomCommands
{
public:
  virtual bool      SetNamespace( const char* strNamespace ) = 0;

  // Return an interface to the Added command. NULL if failed or command already existed.
  virtual ICustomCommandDef*  DefineCommand( const char* strCommand , DWORD dwCommand , DWORD dwOptions = 0 , DWORD dwTargetFlags = 0 ) = 0;

  //virtual DWORD     GetCommand( const char* strCommand , short nParam=0  , DWORD dwFlags = 0) = 0;

  // Set to true when you have Initialized all command so you do not need to do it again.
  virtual void      SetInitialized( bool b ) = 0;
  virtual bool      GetInitialized() = 0;
};

//OBSOLETE - Remove me
class __declspec(novtable) IZAction
{
public:
  virtual UINT     GetActionID() = 0;

  virtual eContentDataType  GetActionType() = 0;
  virtual DWORD       GetTargetFlags() = 0;

  virtual DWORD_PTR  GetActionData() = 0;
  virtual UINT     GetActionDataCount() = 0;
  virtual DWORD    GetActionDataFlags() = 0;
};

MCNSEND
