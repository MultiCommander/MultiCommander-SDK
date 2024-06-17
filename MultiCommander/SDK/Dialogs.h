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

#define IPDLG_BTN_OK			1
#define IPDLG_BTN_CANCEL		2
#define IPDLG_BTN_ABORT			3
#define IPDLG_BTN_PAUSE			4
#define IPDLG_BTN_CONTINUE		5 // Should not be used by user.. will be set if button pause, and is pressed

  // This Progress Thread will be run in its own GUI Thread. 
  //
  //
class __declspec(novtable) IProgressDlg : public IHObject
{
public:
  virtual BOOL    Show() = 0;
  virtual BOOL    Hide() = 0;

  virtual void    SetAlwaysOnTop( BOOL b ) = 0;

  virtual void    SetMaxItems( DWORD nMax ) = 0;    // Thread safe
  virtual void    SetDoneItems( DWORD nVal ) = 0;   // Thread safe

  virtual void    SetPrepareCount( DWORD nCount ) = 0;
  virtual void    SetPreparing(bool b) = 0;

  virtual void    SetCurrentItem(const WCHAR* szCurrentItem) = 0; // Filename / path of current item being processed ( thread safe ? )

  virtual void    SetCaption( const WCHAR* strCaption ) = 0; // NOT safe.

  virtual void    SetText( const WCHAR* strText ) = 0; // Thread Safe.

  // A text that will the added infront of all SetText calls.Thread ( eg "Processing : " and the SetText(...) is only called with the filename
  virtual void    SetPreText( const WCHAR* strText ) = 0;  // Thread safe

  virtual void    SetMaxUpdateRate( DWORD ms ) = 0;
  virtual void    SetButtonType( short nOKButton = IPDLG_BTN_PAUSE , short nCancelButton = IPDLG_BTN_ABORT ) = 0;

  // time in millisec. 0 to stop autoupdate of progress ( Thread safe )
  virtual void    SetAutoUpdate(DWORD ms) = 0;

  virtual void    Update( BOOL bForce = FALSE ) = 0;

  // Thread Safe
  virtual BOOL    IsPaused() = 0;
  virtual BOOL    IsAbortRequested() = 0;

  // Destroys window and delete object
  virtual void    Release() = 0;
};

//////////////////////////////////////////////////////////////////////////
//
// IProgress2 Dialog - Can show two progress items.  
//
//  Message Text ( MultiLine )
//  
//  Text Item 1
//  Text Item 2
//  [ Progress bar ................... ]
//  Progress Text 
//
// Example
// 
//  Comparing Two Files
//  
//  File 1 : C:\MyFile1.dat
//  File 2 : C:\MyFile2.dat
//  [ Progress bar ................... ]
//     123 MB of 233 MB Compared
#define IPROG2_OPT_PROGRESSINFO_BYTES    0x00000100L
#define IPROG2_OPT_PROGRESSINFO_ITEMS    0x00000200L
class __declspec(novtable) IProgress2Dlg : public IHObject
{
public:
  virtual BOOL    Show() = 0;
  virtual BOOL    Hide() = 0;

  virtual void    SetAlwaysOnTop( BOOL b ) = 0;

  virtual void    SetOptions(DWORD dwOptions ) = 0; // IPROG2_OPT_PROGRESSINFO_xxxxx
  virtual DWORD   GetOptions() = 0;

  virtual void    SetProgressMax( UINT64 nMax ) = 0;    // Thread safe
  virtual void    SetProgressCurrent( UINT64 nVal ) = 0;   // Thread safe

  // -------------------Remove ?
  virtual void    SetPrepareCount( DWORD nCount ) = 0;
  virtual void    SetPreparing(bool b) = 0;
  // -------------------

  virtual void    SetCaption( const WCHAR* szCaption ) = 0; // NOT thread safe. - Must be called from thred that create it
  virtual void    SetMessageText( const WCHAR* szText ) = 0;       // Thread Safe.

  virtual void    SetItem1(const WCHAR* szItemText) = 0;
  virtual void    SetItem2(const WCHAR* szItemText) = 0;

  // A text that will the inserted in-front of all SetItem1/2 text.
  // Eg "Processing : " or "To : " , "From : " , 
  // This Text will be kept even if new text are set using SetItem1
  virtual void    SetPreItem1Text( const WCHAR* strText ) = 0;  // Thread safe
  virtual void    SetPreItem2Text( const WCHAR* strText ) = 0;  // Thread safe

  // Auto Refresh state.
  virtual void    SetMaxUpdateRate( DWORD ms ) = 0;
  // time in millisec. 0 to stop autoupdate of progress 
  virtual void    SetAutoUpdate(DWORD ms) = 0; // Thread safe

  virtual void    SetButtonType( short nOKButton = IPDLG_BTN_PAUSE , short nCancelButton = IPDLG_BTN_ABORT ) = 0;

  // Force update of UI
  virtual void    Update( BOOL bForce = FALSE ) = 0; // NOT thread safe. 

  //////////////////////////////////////////////////////////////////////////
  // Pause / Continue
  /*
  if(pProgressDlg->IsPauseRequested())
  {
    pProgressDlg->OnSetStatePause();
    while(pPorgressDlg->IsPaused())
      Sleep(200); 
    pProgressDlg->OnSetStateContinue();
  }
  
  Or call 
  pProgressDlg->OnHandlePause(); // If pause is requested it will halt inside pause until continue/abort is clicked.
  */
  
  virtual bool    IsPauseRequested() = 0;   // Thread Safe
  virtual bool    IsAbortRequested() = 0;   // Thread Safe
  virtual bool    IsPaused() = 0;           // Thread Safe
  virtual void    OnSetStatePause() = 0;    // Thread Safe - Call this when you are pausing..
  virtual void    OnSetStateContinue() = 0; // Thread Safe - Call this after you have paused and are continuing work
  virtual void    OnHandlePause() = 0;      // Thread Safe

  // Destroys window and delete object
  virtual void    Release() = 0;
};

// Used with IWorkingDlg as a callback for user request
class __declspec(novtable) IUserCallback
{
public:
    // Abort of work requested
    virtual void AbortRequested() = 0;
    // Pause of work requested
    virtual void PauseRequested() = 0;
};

class __declspec(novtable) IWorkingDlg
{
public:
  virtual BOOL    Show() = 0;
  virtual BOOL    Hide() = 0;
  virtual void    SetAlwaysOnTop( BOOL b ) = 0;

  virtual void    SetCaption( const WCHAR* szCaption ) = 0;   // NOT thread safe - Must be called from thread that create it
  virtual void    SetMessageText( const WCHAR* szText ) = 0;  // Thread Safe.
  virtual void    SetInfoText(const WCHAR* szInfoText) = 0;

  virtual void    UpdateText() = 0;

  virtual void    Start() = 0;
  virtual void    Stop() = 0;

  virtual void    Release() = 0;

  virtual void    UserCallback(IUserCallback* pHook) = 0;
  // 
  // Destroys window and delete object
};

//////////////////////////////////////////////////////////////////////////
//
// IMessageBox 
//
class __declspec(novtable) IMessageBox
{
public:
  virtual bool Init(int nButtons) = 0; // Max 4

  virtual bool SetButtonInfo(int nButton, const WCHAR* szText, int nValue, DWORD nFlags) = 0;

  virtual void Caption(const WCHAR* szText) = 0;
  virtual void Message(const WCHAR* szText, ...) = 0;
  virtual int  DoModal() = 0;
  virtual int  DoModeless() = 0;

  virtual void Release();
};

struct GENERICCOMBOBOX
{
  GENERICCOMBOBOX()
  {
    ZeroMemory( this, sizeof(GENERICCOMBOBOX) );
    nSelectedValue = -1;
  }
  void AddTitle( const WCHAR* str )
  {
    wcsncpy( strTitle , str , 64 );
  }

  void AddText( const WCHAR* str )
  {
    wcsncpy( strText , str , 256 );
  }

  void SetSessionConfigTag( const WCHAR* str )
  {
    wcsncpy( strSessionPath , str , 256 );
  }
  void SetDefaultText( const WCHAR* str )
  {
    wcsncpy( strDefaultValue , str , 256 );
  }

  void AddString( const WCHAR* str )
  {
    wcsncpy( strDefaultValue , str , 256 );
  }

  const WCHAR* GetResult() { return strResult; }

  // If -1 then dialog was canceled and no item was selected
  int			GetSelectedItem() const { return nSelectedValue; }

  HWND   hwndParent{};

  WCHAR strTitle[64]{};
  WCHAR strText[256]{};
  WCHAR strChkText[256]{};

  WCHAR strSessionPath[256]{};  // Session Key - used to auto fill the dialog with items.
  WCHAR strDefaultValue[256]{}; // Default text in the combobox when it starts. (NOT when bDropList is true)
  WCHAR strResult[256]{};

  // Set to set an item as selected before dialog is shown or get to know what item was selected when dlg was closed
  int		nSelectedValue;	// Index of the selected. 

  WCHAR strSelectedValue[256]{}; // The Text of the selected item after dialog was closed

  bool bDropList{}; // set to true to show a droplist instead  ( instead of the editable dropdown type )

  bool  bShowCheckBox = false; // Show Checkbox., (Link "Remember this" )
  bool  bCheck = false; // state of checkbox

  IValueCollection*  pValues{}; // Array of string that the combobox will be populated with
  IKeyValCollection* pKeyValues{};
};

struct MULTICOMBOBOXDLG : public GENERICCOMBOBOX
{
  MULTICOMBOBOXDLG()
  {
    ZeroMemory( this, sizeof(MULTICOMBOBOXDLG) );
    nSelectedValue = -1;
  }
  void AddText2( const WCHAR* str )
  {
    wcsncpy( strText2 , str , 256 );
  }

  WCHAR strText2[256]{};

  WCHAR strSessionPath2[256]{};  // Session Key - used to auto fill the dialog with items.
  WCHAR strDefaultValue2[256]{}; // Default text in the combobox when it starts. (NOT when bDropList is true)
  WCHAR strResult2[256]{};


  int nSelectedValue2 = -1;

  WCHAR strSelectedValue2[256]{};
  bool  bDropList2 = false; // Second combo should be droplist

  // Secondary values
  IValueCollection*  pValues2{};
  IKeyValCollection* pKeyValues2{};
};


class IEventCallback;
struct SelectWindowData 
{
  SelectWindowData()
  {
    ZeroMemory(this,sizeof(SelectWindowData));
  }
  HWND hWndParent{};

  WCHAR strTitle[64]{};
  WCHAR strFooter[64]{};


  // The icons will be copied so destroy them after setting them
  HICON hIconSet{}; 
  HICON hIconClear{};
  WCHAR strSetToolTip[128]{};   //Tooltip for the set icon-button
  WCHAR strClearToolTip[128]{}; //Tooltip for the Clear icon-button

  POINT pt{};     // Where to show the window ( upper left corner )
  UINT  nWidth{}; // Width of the window

  IValueCollection* pValues{}; // Values to show
  DWORD             nValues{}; // Values to show

  IEventCallback* pClickCallback{}; // OnItemClick Callback.
};

#ifndef CLR_NONE
  #define CLR_NONE                0xFFFFFFFFL
#endif

struct COLORPICKINFO
{
  COLORPICKINFO()
  {
  }

  HWND hWndParent = 0;

  bool bAllowWindowsColorPage = true;
  bool bAllowTextColorPick = true;
  bool bAllowBackgroundColorPick = true;
  bool bAllowDefaultBackground = true;

  COLORREF  crText = RGB(255,255,255);
  COLORREF  crBackground = CLR_NONE;
  COLORREF  crDefaultBackground = RGB(0, 0, 255);

  // If Windows Color was set/used
  int crTextID = -1;
  int crBackgroundID = -1;
  bool bTextColorID = false;
  bool bBackgroundColorID = false;
};

struct PICKFILTERINFO
{
  PICKFILTERINFO()
  {
    ZeroMemory(this, sizeof(PICKFILTERINFO));
  }
  void Caption(const WCHAR* str)
  {
    wcsncpy(szCaption, str, _countof(szCaption));
  }
  void Title(const WCHAR* str)
  {
    wcsncpy(szTitle, str, _countof(szTitle));
  }
  
  void DefaultText(const WCHAR* str)
  {
    wcsncpy(szDefaultValue, str, _countof(szDefaultValue));
  }
  void SessionConfigTag(const WCHAR* str)
  {
    wcsncpy(szSessionPath, str, _countof(szSessionPath));
  }

  void FilterResult(const WCHAR* sz)
  {
    wcsncpy(szResult, sz, _countof(szResult));
  }

  HWND hWndParent = 0;

  // Options
  bool bAllowAdvFilters = false;

  // Not respected yet
  bool bAdvFilterAllowContent = false;   // Allow/Disallow selection of advanced filters using "content" matching. (Content match can be very slow and must be run in background)
  

  // Setting
  // Selection
  WCHAR szCaption[64]{}; // Caption Text
  WCHAR szTitle[256]{};  // Title Text  eg: "Enter selection filters. "
  WCHAR szDefaultValue[256]{}; 

  // Session Key - used to auto fill the dialog with items.
  WCHAR szSessionPath[256]{}; // Key used to load/save history data from/to

  WCHAR   szResult[256]{};
  ZHANDLE hFilter{};
};


MCNSEND