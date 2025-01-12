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

#pragma once
#include "MCNamespace.h"
MCNSBEGIN

  // FileManager Option
#define FMO_SHOWFILESIZETYPE  100
#define FMO_FILESIZETYPE      101
#define FMO_TAGFOLDERS        110
#define FMO_FOLDERTAGINSIZE   111
#define FMO_FORWARDKEYS       112
#define FMO_DATEMODE          113
#define FMO_SHOWLINKTARGET    114 // Show (Symlink/Junction) target in filename ( eg myfile -> [s] c:\users\myfile )
#define FMO_SHOWHIDDEN        115
#define FMO_SHOWSYSTEM        116
#define FMO_RBUTTONDRAG       117 // Enable dragging using right mouse button. Will disable toggle selection that are default for right button
#define FMO_RBUTTONDRAG_CTRL  118 // Enable dragging using right mouse button with holding done CTRL key.
#define FMO_UNSELECT_ONCOPY   119
#define FMO_RESCAN_ALWAYS     120 // Always rescan when going into a cached folder
#define FMO_DATEADJUSTDTS     121 // adjust datetime for dts

#define FMO_VIEWMODE          125 // ViewMode
#define FMO_ICONTYPE          130 // Icon type.  Small, Large, XLarge, Jumbo.
#define FMO_THUMBSIZE         131
#define FMO_THUMNAILSETTINGS  132
#define FMO_FLATFSVIEW        133
#define FMO_SHOWCONTEXTONDOTDOT 134
#define FMO_AUTOSCALE         135 // Autoscale the icons size

#define FMO_VIEWMODE_ICONS        1  // Not Supported yet
#define FMO_VIEWMODE_LIST         2  // List mode Only first column is show
#define FMO_VIEWMODE_DETAIL       3  // Detail view mode - Multi columns
#define FMO_VIEWMODE_THUMBLIST    4  // Thumbnail list mode - Large Thumbnail with 1 row of text (first column)
#define FMO_VIEWMODE_THUMBDETAIL  5  // Thumbnail detail mode. Large Thumbnail with ALL column as top->bottom rows to the right of the thumbnail


#define THUMBMODE_WHEELZOOM       0x00000010
#define THUMBMODE_PICFRAME        0x00000020
#define THUMBMODE_NOTEXT          0x00000100
#define THUMBMODE_TEXTFILENAME    0x00000200
#define THUMBMODE_TEXTFILESIZE    0x00000400
#define THUMBMODE_IMGALIGN_TOP    0x00100000 // only thumbnail detailed mode
#define THUMBMODE_IMGALIGN_CENTER 0x00200000 // only thumbnail detailed mode
#define THUMBMODE_IMGALIGN_BOTTOM 0x00400000 // only thumbnail detailed mode
#define THUMBMODE_HIGHLIGHTNAME   0x00800000 // only thumbnail detailed mode
#define THUMBMODE_OVERLAY_PICSIZE 0x01000000 // Overlay image size on thumbnail
#define THUMBMODE_OVERLAY_PICDATE    0x02000000 // Overlay image size on thumbnail
#define THUMBMODE_OVERLAY_PHOTOINFO1 0x04000000 // Overlay image size on thumbnail

#define FLATVIEW_AUTOADDPATHCOLUMN 0x00000001
#define FLATVIEW_INCLUDEFOLDERS    0x00000002

class __declspec(novtable) IFileManagerViewInterface
{
public:
  // Set filesize type
  /*
  ZFM_FILESIZE_BYTES
  ZFM_FILESIZE_KBYTES
  ZFM_FILESIZE_MBYTES
  ZFM_FILESIZE_AUTO
  */

  // How to show the filesize. Auto, Bytes, KBytes, MegaBytes, GigaBytes
  virtual void FM_Set_FilesizeType( ZHANDLE hFMGrid , short iType ) = 0;

  // How to Show The FileSize unit, None, Mini, Short, Long ( M/MB/Megabytes )
  virtual void FM_Set_ShowFilesizeUnit( ZHANDLE hFMGrid , short nUnitType ) = 0;

  // How many decimals to show for the filesize. (Not valid if filesize is shown as Bytes. )
  // 0 , 1 or 2, allowed. 
  virtual void FM_Set_ShowFilesizeDecimals( ZHANDLE hFMGrid , short nDecimals ) = 0;

  // 1 = Show Both , 3 = Show Ext in Ext only , 2			 = show only on filename
  virtual void FM_Set_ShowFileExt( ZHANDLE hFMGrid , short iType ) = 0;

  virtual long FM_InsertItemsFromItem( ZHANDLE hFMGrid , ZHANDLE hItem ) = 0;
  // strPath must a WCHAR str[1024]; . its assumes that strpath can be up to 1024 char long
  virtual long FM_GetPathFromItem( ZHANDLE hFMGrid , ZHANDLE hItem , WCHAR* strPath ) = 0;

  virtual long FM_GetCheckedItems( ZHANDLE hFMGrid , IFileItemCollection* pFileItems ) = 0;

};

class __declspec(novtable) IFileViewInterface
{
public:	
  virtual void Set_Option( DWORD dwOption , DWORD dwParam ) = 0;
  virtual DWORD Get_Option( DWORD dwOption ) const = 0;

  virtual bool ChangeViewMode(DWORD nViewMode, DWORD dwOptions, DWORD dwSizes, int nShowFileExt, bool bVertical, short nFirstColWidth = -1) = 0;

  virtual bool CalculateRowHeight() = 0;
  virtual void Set_TooltipMode( DWORD dwTooltipMode, short delay = 1500, short timeout = 7000 ) = 0;

  virtual BOOL UnBindKey( UINT nVKCode , UINT nFlags ) = 0;
  virtual BOOL BindKey( UINT nVKCode , UINT nFlags , DWORD dwCmd , DWORD_PTR lParam) = 0;

  virtual BOOL BindKey(DWORD hotKey , DWORD dwCmd , DWORD_PTR lParam) = 0;
  virtual BOOL UnBindKey(DWORD hotKey, bool bUnbindAny = false) = 0;

  virtual BOOL BindAsciiKey( TCHAR nChar , UINT nFlags , DWORD dwCmd , DWORD_PTR lParam ) = 0;

  virtual BOOL BindMouseCommand(UINT nMouseKey, UINT nMouseCommand, ZHANDLE hAction) = 0;

  virtual IFileItem* GetFileItemInFocus() = 0;
  virtual bool GetFileItems( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool GetSelection( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool SetSelection( IFileItemCollection* pFileItems, DWORD dwFlags ) = 0;
  virtual bool SetSelectionByFilter( const WCHAR* strFilter, DWORD dwFlags ) = 0;
  virtual bool SetSelectionByAdvFilter(MCNS::ZHANDLE hFilter, DWORD dwFlags) = 0;
  virtual bool SetSelectionAll( bool bSelect ) = 0;
  virtual bool InvertSelection() = 0;
  //////////////////////////////////////////////////////////////////////////
  // Action to preform on marked files or file in focus
  //
  // The functions are Asyncrone and will return direct.
  //////////////////////////////////////////////////////////////////////////
  // if bHideDialog is TRUE the dialog is not shown and default parameters is used
  virtual BOOL DoCopy( DWORD dwFlags = 0 ) = 0;
  virtual BOOL DoQuickCopy( DWORD dwFlags = 0 ) = 0;
  virtual BOOL DoMove( DWORD dwFlags = 0 ) = 0;
  virtual BOOL DoDelete( DWORD dwFlags = 0 ) = 0;
  virtual BOOL DoMakeDir(const WCHAR* szFoldername, bool bSilent = false, bool bGoToCreatedPath = false) = 0;
  virtual BOOL DoMakeDirOnTarget(bool bSilent = false) = 0;
  virtual BOOL DoRenameCurrent() = 0;

  virtual BOOL DoPackFiles(DWORD dwFlags = 0) = 0;

  // bForceAll = Force sizing of ALL folder. not just the selected.
  // bOnlyFocus = Size only folder that are in focus
  virtual BOOL DoCalcFolderSize(bool bForceAll, bool bOnlyFocus) = 0;

  // remove
  virtual BOOL DoInvertSelection() = 0;


  // REMOVE THEM.....
  // if bHideDialog is true then strSelection is used. else a dialog is shown.
  virtual BOOL DoAddSelection( BOOL bHideDialog = FALSE , const WCHAR* strSelection = NULL ) = 0;
  virtual BOOL DoRemoveSelection( BOOL bHideDialog = FALSE , const WCHAR* strSelection = NULL ) = 0;
  //////////////////////////////////////////////////////////////////////////
  virtual bool AddColumn(const WCHAR* szColumnName) = 0;
  virtual bool RemoveColumn(const WCHAR* szColumnName) = 0;
};

#define FTREEVIEW_EXPANDCURRENT       0x00000001    // Expand current selected folder. (Else only make sure that tree is expanded until parent folder)
#define FTREEVIEW_EXPANDONHOVERDRAG   0x00000002    // Expand Folder when hovering during drag and drop
#define FTREEVIEW_SMARTEXPANDBOX      0x00000004    // Use Smart Expand box that only show expandbox if item have subfolders
#define FTREEVIEW_AUTOHORZSCROLL      0x00000008    // Auto horizontal scroll to show as much of name as possible
#define FTREEVIEW_HIGHLIGHT_CURRENT   0x00000010    // Highlight the current focus item
#define FTREEVIEW_HIGHLIGHT_PARENT    0x00000020    // Highlight parent of item that is highlighted

// Highlight style
#define FTREEVIEW_HIGHLIGHT_BOLD       0x00000001
#define FTREEVIEW_HIGHLIGHT_UNDERLINE  0x00000002
#define FTREEVIEW_HIGHLIGHT_COLOR      0x00000004

class __declspec(novtable) IFolderTreeView
{
public:
  virtual HWND    GetHWND() = 0;

  virtual void UpdateView() = 0;
  virtual bool ShowPath(const WCHAR* szPath) = 0;
  // Clear everything from the folder tree
  virtual bool Clear() = 0;

  // Color Configuration
  virtual void SetColor_DefaultActiveBG(COLORREF cr) = 0;
  virtual void SetColor_DefaultInactiveBG(COLORREF cr, short nOffsetColor = 0) = 0;
  virtual void SetColor_HighlightActiveBackground(bool b) = 0;

  // -1 for BG to use default bg
  virtual void SetColor_NormalItem(COLORREF cr, COLORREF crBG = (COLORREF)-1) = 0;
  virtual void SetColor_FocusItem(COLORREF cr, COLORREF crBG = (COLORREF)-1) = 0;

  virtual void SetColor_Highlight(COLORREF cr, COLORREF crBG = (COLORREF)-1) = 0;

  // must be false if Normal/Focus items background color is to be used.
  // set to true and the background color of Items will be ignored
  virtual void SetColor_OverrideBackgroundColor(bool b) = 0;

  virtual void SetIconSize(MCIconSize nIconSize, bool bAutoScale = false) = 0;
  virtual void SetRowPadding(long lPadding) = 0;

  virtual BOOL SetFont( const WCHAR* FaceName , int Size , long FontFlags ) = 0;

  virtual void  SetOptions( DWORD dwOptions ) = 0;
  virtual DWORD GetOptions() = 0;

  virtual void  SetHighlightStyle(DWORD dwOptions) = 0;
  virtual DWORD GetHighlightStyle() = 0;

  virtual void SetDeviceFilter(const WCHAR* szDeviceFilter, DWORD dwDeviceTypes) = 0; // dwDeviceTypes == 0 for default
  virtual void GetDeviceFilter(WCHAR* szDeviceFilter, DWORD dwLen) = 0;

  virtual void SetFocusNotification(bool bEnable) = 0;

  virtual void SetUseCustomBGColors(bool bUseCustomBGForFiles, bool bUseCustomBGForFolders) = 0;
  virtual void AllowRuleBasedColoring(bool bRuleBasedColoring) = 0;
  virtual bool AllowRuleBasedColoring() = 0;

  virtual void LanguageAwareSort(bool b) = 0;

  virtual bool DoRenameCurrent() = 0;

  virtual void Collapse() = 0;

  virtual IFileItem* GetFileItemInFocus() const = 0;
};

MCNSEND