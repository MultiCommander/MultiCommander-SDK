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

#include "SDKVersion.h"
#include "Multi_Type.h"
#include "ExtensionInfo.h"
#include "MultiApp_Def.h"

#include "IAppInterface.h"

#include "MCNamespace.h"
MCNSBEGIN


class IRunAdminCommand;
/*
All Application extension must inherit from IPluginInterface
TODO - Rename IPluginInterface to IAppExtension
*/
class IAppMenu;

struct CloseContext
{
  bool Shutdown;
  bool DoNotAsk;
  bool ClosingMany;
};


class __declspec(novtable) IPluginInterface
{
public:
  virtual ~IPluginInterface() = default;
  // MultiApp is Initialized
  virtual long PreStartInit(  IMultiAppInterface* pInterface )= 0;

  // This function is the first to be called right after the module is loaded
  // this function should return some Flags, like what type of module this is and so on..
  // But all connections between this module and framework is not done yet so any interface function my not be called yet
  virtual long GetModuleFlags() = 0;

  // This function is always run after CreateParam() is run
  virtual BOOL Init(  IMultiAppInterface* pInterface ) = 0;

  // Return the module ID that is a GUID string
  virtual char* Get_ModuleID() = 0;

  // If created control can send notifications to the module. those notification message will be received here
  virtual bool OnNotify( ZHANDLE hFrom , DWORD nNotifyMsg , WPARAM wParam = 0 , LPARAM lParam = 0 , void* pExtra = NULL ) = 0;

  // Messages sent from the Core framework to the module.
  // If hView is not 0 then it is the handle of a UI Component created by the extension.
  virtual BOOL OnMessage( long msg , ZHANDLE hView , WPARAM param1 , LPARAM param2 ) = 0;

  // Registered Action is activated. and it is up to the module to handle it 
  virtual BOOL OnAction( IZAction* /*pAction*/ ) { return FALSE; }

  // An custom command that this extension has registered is activated. Lets handle it.
  virtual BOOL OnCustomCommand(ICustomCommand* pCustomCommand) = 0;

  // Called from The AdminHelper Process if a command is sent using the IRunAsAdmin Interface,
  // The caller should send everything the function need in the RunAsAdmin data. since the IMultiAppInterface is not available
  // and the init function is NOT Called. Think of this function as a static function.
  // And pAdminCommand->SetResponse(...); should ALWAYS be run. else a result is not returned back to MultiCommand and that
  // function might sit and wait for an response forever and lockup.. (or until timeout happens)
  virtual BOOL OnAdminCommand(DWORD /*nCommand*/, IRunAdminCommand* /*pAdminCommand*/) { return FALSE; }

  // A Register script function is about to be run. (Optional)
  virtual BOOL OnScriptFunction(IScriptFunctionContext* /*pScriptFuncContext*/) { return FALSE; }

  // Add Context menu items to a application context menu.
  // nContextMenuID = 1 : Tab ContextMenu 
  //
  // If a extension command is chosen a AM_CONTEXTMENU message is sent with nContextMenuID as lParam has the extension command value 
  virtual BOOL OnAddContextMenuItems(DWORD /*nContextMenuID*/, IAppMenu* /*pMenu*/) { return FALSE; }

  // Modules is requested to close. return TRUE if cleanup was successfully or FALSE if modules should not close down
  // bShutdownClose is TRUE if MultiCommander is shutting down. 
  virtual BOOL OnClose(CloseContext& context) = 0;

  // Import/Export Interface for transferring data between extensions.
  // return 0 : False                   ZIMPORT_FALSE
  // return 1 : Successful              ZIMPORT_SUCCESS
  // return 2 : Use ImportData Object.. ZIMPORT_USEIMPOBJECT
  virtual int       DoImportData( IXData *pXData ) = 0;

  virtual IImportData*  Get_ImportObject() = 0;
  virtual IExportData*  Get_ExportObject() = 0;

private:   

};


MCNSEND