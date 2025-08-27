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

#include "IEventCallback.h"
#include "IFileItem.h"
#include "MCNamespace.h"
MCNSBEGIN

  //////////////////////////////////////////////////////////////////////////
class __declspec(novtable) IEditControl : public IHObject
{
public:
  virtual BOOL AppendText( const WCHAR* strText ) = 0;
  virtual BOOL SetText( const WCHAR* strText ) = 0;
  virtual BOOL GetText( WCHAR* strText , DWORD nLen ) = 0;

  // The same as calling EnableChangeNotification(false); SetText(..); EnableChangeNotification(true);
  virtual BOOL SetTextSilent( const WCHAR* strText ) = 0;

  virtual void SetFocus() = 0;

  // If SetText is called a Chagne notification is sent. But sometimes you do not want to send that if 
  // you calling SetText your self, by default it is on
  virtual void EnableChangeNotification(bool b) = 0;

  virtual void EnableWindow(bool b) = 0;

  virtual void SetFont(const WCHAR* FaceName, int Size, long FontFlags) = 0;
};

class __declspec(novtable) IButtonControl : public IHObject, public ISetEvent
{
public:
  // tmp solution...
  virtual void FixColors() = 0;

  // Set Button Icon
  virtual void  SetText(const WCHAR* text) = 0;
  virtual BOOL  SetIcon( HICON hIn , HICON hOut ) = 0;
  virtual BOOL  SetGreyDisabledIcon(int cx, int cy) = 0;
  virtual void  SetTooltip( const WCHAR* tooltip) = 0;

  virtual void  Enable(bool b) = 0;
  // --- Draw Styles
  // Draw border
  virtual DWORD DrawBorder(BOOL bDrawBorder = TRUE, BOOL bRepaint = TRUE) = 0;

  // If a Command has been added to the event handling it will add the HotKey in the tooltip
  virtual void  UpdateToolTip(IEventCallback::EventCallbackType EventType = IEventCallback::Callback_ButtonClick) = 0;

  //  Defined in ISetEvent
  //   bool SetEventCallback( IEventCallback::EventCallbackType EventType , IEventCallback* pEvent );
  //   bool SetEventMessage( IEventCallback::EventCallbackType EventType , ZHANDLE hCmd);
  //   bool RemoveEventCallback( IEventCallback::EventCallbackType EventType);

};

class __declspec(novtable) ILabelControl : public IHObject, public ISetEvent
{
public:
  virtual void SetFont( const WCHAR* FaceName , int Size , long FontFlags ) = 0;
  virtual void SetTextPadding( int iLeft , int iRight ) = 0;
  virtual void SetTextColor( COLORREF crText ) = 0;
  virtual void SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BOOL bGradient = FALSE ) = 0;
  virtual void SetText(const WCHAR* strText ) = 0;

  virtual void SetFocusOnClick( BOOL b ) = 0; // Stupid

  virtual void GetText( WCHAR* strText , long nMaxSize ) = 0;

  // Emulate user action
  virtual void DoContextMenu() = 0;
  virtual void DoEditText() = 0;
};

//////////////////////////////////////////////////////////////////////////
// Flags for SetStyle(...);
//
#define CBC_EDITOUTSIDE   0x00000010  // Click must be outside of crumb items.
#define CBC_EDITONCLK_L   0x00000100  // Enter edit mode on Left Click
#define CBC_EDITONDBL_L   0x00000200  // Enter edit mode on Left DblClick
#define CBC_EDITONCLK_R   0x00000400  // Enter edit mode on Right Click
#define CBC_EDITONDBL_R   0x00000800  // Enter edit mode on Right DblClick
#define CBC_EDITONCLK_M   0x00001000  // Enter edit mode on Middle Click
#define CBC_EDITONDBL_M   0x00002000  // Enter edit mode on Middle DblClick
#define CBC_LASTITEMCOLD  0x00010000  // Show no hover action for the last item
#define CBC_LASTITEMNOCLK 0x00020000  // Do not generate click action for the last item
#define CBC_LASTITEMNODELIMITER 0x00040000  // Do not draw a delimiter for the last item
#define CBC_DONOTSTRIPDELIMITER	0x00080000  // Do not strip double delimiter.

// SetDelimiter modeFlags
#define CBC_MODE_STRIP    0 // Strip the delimiter char
#define CBC_MODE_LEFT     1 // Show Delimiter in the Left part 
#define CBC_MODE_RIGHT    2 // Show Delimiter in the right part 

class __declspec(novtable) ICrumbBarControl : public IHObject, public ISetEvent
{
public:
  virtual void  SetStyle(DWORD dwStyle) = 0;
  virtual DWORD GetStyle() = 0;

  virtual void SetFont(const WCHAR* FaceName, int Size, long FontFlags) = 0;
  virtual void SetMargin(int iLeftMargin, int iRightMargin) = 0;

  virtual void SetDefaultColor(COLORREF crColor) = 0;
  virtual void SetHotColor(COLORREF crColor) = 0;
  virtual void SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BOOL bGradient = FALSE ) = 0;

  virtual void SetDelimiter(const WCHAR* strDelimiter, DWORD dwMode) = 0;
  virtual void SetPath(const WCHAR* strText) = 0;
  virtual void GetPath(WCHAR* strText, long nMaxSize ) = 0;

  virtual void SetFocusOnClick( BOOL b ) = 0;

  // Emulate user action
  virtual void DoContextMenu() = 0;
  virtual void DoEditText() = 0;
};

class IHotKeyControl : public IHObject
{

};



class __declspec(novtable) IComboBoxControl : public IHObject
{
public:
  // 
  // if the hCmd is used then the TextID or Text parameter can be 0/NULL  
  // then it will use the text assign to the hCmd
  //
  //  hCmd is a command that will be sent to extension via OnNotify 
  //
  virtual int  AddString( long TextID , ZHANDLE hCmd = 0) = 0;
  virtual int  AddString( const WCHAR* Text , ZHANDLE hCmd = 0) = 0;

  virtual BOOL SetSelected( int nIndex ) = 0;
  virtual int  GetSelected() = 0;

  virtual bool GetItem(WCHAR* text, DWORD len, ZHANDLE* hCmd, int nIndex) = 0;

  virtual void Clear() = 0;
  virtual BOOL ShowDropDown() = 0;

  virtual int  Count() = 0;
};

class __declspec(novtable) IDeviceComboCtrl : public IComboBoxControl
{
public:

  virtual void SetDeviceFilter(const WCHAR* szDeviceFilter) = 0;

  virtual void ShowFreeSpace(bool bShowFreeSpace, int size1k = 1000) = 0;
  virtual void UpdateFreeSpaceWhenShowing(bool bUpdateFreeSpaceOnShow) = 0;

  // It will set the item best suited for the path as active.
  virtual BOOL SetCurrentPath( const WCHAR* strPath ) = 0;

  virtual BOOL GetPath(WCHAR* szPath, DWORD nLen, DWORD nIdx) = 0;

  virtual BOOL IsCurrentPathUNC() = 0;
  virtual void RememberLastSubFolder(bool b) = 0;
};

class IMultiWindow;

class __declspec(novtable) ISplitterViewInterface : public IHObject
{
public:
  //
  // CreateParam
  //
  // #define ZCP_FIXEDSIZE    0x00010000L // This active splitter view will not be resizeable
  // #define ZCP_DYNAMICKEEP  0x00020000L // This active splitter view will not be resizeable
  // #define ZCP_NOBORDER     0x00040000L

  // Create Child views.
  virtual ZHANDLE	CreateView( long CreateType , long CreateParam , int row , int col , int minSize , BOOL bDefaultFocus = FALSE, DWORD_PTR param1 = 0, DWORD_PTR param2 = 0, DWORD dwInitValues = 0) = 0;
  virtual void*	CreateViewQI( long CreateType , long CreateParam , int row , int col , int minSize , BOOL bDefaultFocus = FALSE, DWORD_PTR param1 = 0, DWORD_PTR param2 = 0, DWORD dwInitValues = 0) = 0;

  virtual ZHANDLE AddHWNDToPane(long lCreateparam, HWND hWnd, int row, int col, int minSize, BOOL bDefaultFocus = FALSE ) = 0;
  virtual ZHANDLE AddWindowToPane(long lCreateparam, IMultiWindow* pWnd, int row, int col, int minSize, BOOL bDefaultFocus = FALSE) = 0;

  // Create a new splitter in a cell.
  virtual ZHANDLE CreateSplitter( long Createparam , int nMaxRow , int nMaxCol , int row , int col , int minSize , BOOL bDefaultFocus = FALSE ) = 0;
  virtual void*   CreateSplitterQI( long lCreateparam , int nMaxRow , int nMaxCol , int row , int col , int minSize , BOOL bDefaultFocus = FALSE ) = 0;


  // obsolete
  virtual ZHANDLE CreateControlAsView( long CreateType , long CreateParam , int row , int col , int minSize , int cmd ) = 0;

  // Must be run if anything is changed in the splitter.
  virtual BOOL	RecalcLayout() = 0;
  virtual BOOL  Refresh() = 0;

  //  cxSplitter / cySplitter = The Track Splitter that is drawn when splitting 
  //  cxGap      / cyGap      = The Gap between the views.
  //  cxBorder   / cyBorder   = How large borders to draw on gap.
  virtual BOOL    SetLayout( int cxSplitter=7 , int cySplitter=7  , int cxGap=7 , int cyGap=7 , int cxBorder=2 , int cyBorder=2 ) = 0;

  // 
  virtual void	UseTrackerLimiter( BOOL b ) = 0;

  virtual int GetColWidth(int nCol) = 0;
  virtual int GetRowHeight(int nRow) = 0;

  // width
  virtual bool SetColWidth(int nCol, int nWidth) = 0;
  virtual bool SetRowHeight(int nRow, int nHeight) = 0;

  virtual bool IsRowVisible( int nRow ) = 0;
  virtual bool IsColVisible( int nCol ) = 0;

  // Set if a row or column show be hidden or shown. 
  // RecalcLayout() must be run if any column is changes
  virtual void ShowRow( int nRow , bool bShow ) = 0;
  virtual void ShowCol( int nCol , bool bShow ) = 0;

  virtual void DestroyWindow() = 0;
};

class __declspec(novtable) IStatusBarViewInterface : public IHObject
{
public:
  // add controller to the end of a status panel
  virtual ZHANDLE InsertControl( ZHANDLE hCtrl, int cxWidth, BOOL bStretch, LPRECT pMargin = NULL, const WCHAR* tooltip = NULL ) = 0;
  // remove controller 
  virtual bool RemoveControl(ZHANDLE hCtrl) = 0;
};

//////////////////////////////////////////////////////////////////////////
// Used by 
//  IToolbarViewInterface 
//  IDeviceBarInterface
//
#define   TextOption_None         -1
#define   TextOption_TextLabels    0
#define   TextOption_TextOnRight   1
#define   TextOption_NoTextLabels  2

#define   IconOption_None        -1
#define   IconOption_SmallIcons   0
#define   IconOption_LargeIcons   1
#define   IconOption_MediumIcons  2
#define   IconOption_XLargeIcons  3

// ToolBarButton - Flags for InsertButton(..)
#define TBBTN_MODULE_CONTEXTMENU    0x00000010  // Forward ContextMenu Request to Module that owns the command ( Handle AM_SHOWCMDCONTEXTMENU )
#define TBBTN_MODULE_DEVICEITEM     0x00000100  
class __declspec(novtable) IToolbarViewInterface : public IHObject
{
public:
  virtual ZHANDLE InsertControl( ZHANDLE hCtrl ) = 0;
  virtual ZHANDLE InsertButton( ZHANDLE hCmd , int nIndex = -1, DWORD dwFlags = 0 ) = 0;
  virtual void    InsertSeperator() = 0;

  virtual int   AddIcon( HICON ) = 0;

  virtual int  ChangeIcon(ZHANDLE hCmd, int nIndex ) = 0;

  virtual void SetIconOption( int iconOptions , bool bUpdate) = 0;
  virtual void SetTextOption( int textOptions , bool bUpdate) = 0;
  virtual void SetCustomIconSize( int xyIconSize, bool bUpdate) = 0;

  virtual void ModifyStyle( DWORD dwRemove , DWORD dwAdd ) = 0;
};

class __declspec(novtable) IDeviceBarInterface : public IHObject
{
public:
  virtual ZHANDLE GetClickedDevice() = 0;

  virtual void SetDeviceFilter(const WCHAR* szDeviceFilter) = 0;
  virtual void SetDeviceFilter(DWORD dwDeviceTypes) = 0;

  virtual void TextOption(int nOption) = 0;
  virtual int  TextOption() = 0;

  virtual void IconOption(int nOption) = 0;
  virtual int  IconOption() = 0;

  // Disconnect the device bar from the device manager.
  virtual bool Disconnect() = 0;
};

class __declspec(novtable) ICommandBarContextCallback
{
public:

  virtual HICON GetIcon(DWORD iconResID, const SIZE& iconXY) = 0;

  virtual void Release() = 0;
};

// a CommandBarsController have an multiple Bars that then have command items
// Like like different Commands Groups
class __declspec(novtable) ICommandBarsInterface : public IHObject
{
public:
  virtual bool SetIconSize(int iconXY) = 0;

  virtual ZHANDLE AddCommandBar() = 0;
  
  virtual int Bars() = 0;
  
  virtual ZHANDLE BarAtIdx(size_t idx) = 0;

  virtual void SetFont(const WCHAR* FaceName, int Size, long FontFlags) = 0;

  virtual void EnableContextMenuHandling() = 0;
};

class __declspec(novtable) ICommandGroupBarInterface : public IHObject
{
public:
  // Commandbar interface will call Release() on context when it is finished so interface need to delete it self 
  virtual void SetContextCallback(MCNS::ICommandBarContextCallback* pContext) = 0;
  
  virtual bool AddCommand(MCNS::ZHANDLE hCmd, int nIconResID, DWORD dwExtraData = 0) = 0;
  virtual bool AddSeperator() = 0;
  virtual int  LoadIcons() = 0;
  virtual int  ChangeIcon(ZHANDLE hCmd, int nIconResID) = 0;

  // This are not used yet
  virtual ZHANDLE InsertControl(MCNS::ZHANDLE hCtrl, int cxWidth, bool bStretch, LPRECT pMargin, const WCHAR* tooltip) = 0;
  virtual ZHANDLE InsertButton(ZHANDLE hCmd, int nIndex = -1, DWORD dwFlags = 0) = 0;
  virtual void    InsertSeperator() = 0;
  
  virtual int   AddIcon(HICON) = 0;

  // Not Used
  virtual void SetIconOption(int iconOptions, bool bUpdate) = 0;
  virtual void SetTextOption(int textOptions, bool bUpdate) = 0;

  virtual void ModifyStyle(DWORD dwRemove, DWORD dwAdd) = 0;
};

class __declspec(novtable) IDataPreviewControl : public IHObject
{
public:
  virtual void  OnShowPreview(const MCNS::IFileItem* pFileItem) = 0;
  virtual void  OnShowPreview(const wchar_t* szFilePath) = 0;

  virtual void SetFont(const WCHAR* szFaceName, int size, long fontFlags) = 0;
};

enum class MultiDataTextViewMode
{
  Auto = 0,
  Ascii = 1,
  Unicode = 2,
  UniocodeBe = 3,
  UTF8 = 4,
  Binary = 5,
  Hex = 6  
};

class __declspec(novtable) IMultiDataTextView : public IHObject
{
public:

  virtual void SetFont(const WCHAR* szFaceName, int size, long fontFlags) = 0;
  
  virtual void SetBufferSize(int bytes) = 0;
  virtual void InitMemoryView(MultiDataTextViewMode viewMode) = 0;

  virtual void AddText(const wchar_t* szText) = 0;
  virtual void AddText(const char* szText) = 0;
};


MCNSEND