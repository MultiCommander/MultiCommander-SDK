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
/*
  Event Callback interface for some UI components.

  Inherit from IEventCallback and send that pointer to the UI component. The OnEvent function will be call when a event happen and you can act on it. 

  Example : (ILabelControl)  
  m_pLabel->SetEventCallback(IEventCallback::Callback_ButtonDoubleClick, new CEvent_LabelDblClick(this));

  If user now the user double click on the label the CEvent_LabelDblClick::OnEvent(..) function will be called.

*/
class CEventCallbackData
{
public:
  CEventCallbackData()
  {
    ZeroMemory(this,sizeof(CEventCallbackData));
  }
  HWND    hWnd;
  int     nItem;
  int     nSubItem;
  POINT   pt;

  DWORD_PTR nParam;
};

class __declspec(novtable) IEventCallback
{
public:

  // Callback event that controllers can support. not All types are supported by all controllers.
  enum  EventCallbackType
  { 
    Callback_Unknown = 0,

    // Any area of the controller
    Callback_ContextMenu = 1,       // Right mouse button down 
    Callback_CtrlContextMenu,       // Right mouse button down 
    Callback_LeftMouseButtonClick,
    Callback_RightMouseButtonClick,
    Callback_MiddleMouseButtonClick,
    Callback_LeftMouseButtonDblClick,
    Callback_RightMouseButtonDblClick,
    Callback_MiddleMouseButtonDblClick,
    Callback_ButtonClick,
    Callback_ButtonDoubleClick,

    // Click on special item inside the controller
    Callback_ItemClick,
    Callback_ItemCtrlClick,       // ctrl button holded down and Clicked on item
    Callback_ItemShiftCtrlClick,  // shift+ctrl holded down and clicked on item
    Callback_NoneItemClick,       // Controller has Clickable items. but user clicked outside of the item.

    Callback_RMB_ItemClick,  // Clicking on item using right mouse button. (can conflict with Callback_ContextMenu if controller support both)
    Callback_RMB_ItemCtrlClick,       
    Callback_RMB_ItemShiftCtrlClick,  
    Callback_RMB_NoneItemClick, 

    // special
    Callback_FilesDropped,
    Callback_OnEditChange
  };

  virtual ~IEventCallback()
  {
  }

  virtual void Delete() = 0; // Event is no longer needed. delete it.
  virtual INT OnEvent( EventCallbackType dwEventType , CEventCallbackData* pCallbackData ) = 0;

  virtual MCNS::ZHANDLE Command() { return 0; }
};

//////////////////////////////////////////////////////////////////////////

class __declspec(novtable) ISetEvent
{
public:
  virtual bool SetEventCallback( MCNS::IEventCallback::EventCallbackType EventType , MCNS::IEventCallback* pEvent ) = 0;
  virtual bool SetEventMessage( MCNS::IEventCallback::EventCallbackType EventType , MCNS::ZHANDLE hCmd) = 0;
  virtual bool RemoveEventCallback(MCNS::IEventCallback::EventCallbackType EventType) = 0;
};

MCNSEND