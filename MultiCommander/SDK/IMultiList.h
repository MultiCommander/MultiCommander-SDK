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
#include "MultiApp_Def.h"
#include "MultiControllers.h"

#include "MCNamespace.h"
MCNSBEGIN


class IHCollection;

// MultiListView Row Callback proxy interface
class __declspec(novtable) IMLRowCallBack
{
public:
  virtual DWORD_PTR GetCustomParam( DWORD_PTR lParam , int nCol , DWORD nColumnID ) = 0;
  virtual long    GetColumnFlags( DWORD_PTR lParam , int nCol , DWORD nColumnID ) = 0;
  virtual DWORD   GetTextFormat( DWORD_PTR lParam , int nCol , DWORD nColumnID  ) = 0; 
  virtual DWORD   GetColumnCount(DWORD_PTR lParam) const = 0;
  virtual BOOL    GetControlType(DWORD_PTR lParam , int nCol, DWORD nColumnID , DWORD& dwType ) const = 0;
  virtual DWORD_PTR GetControlData(DWORD_PTR lParam , int iSubItem, DWORD nColumnID  ) const = 0;
  virtual COLORREF GetColumnFGColor( DWORD_PTR lParam ,int nCol, DWORD nColumnID  ) = 0;
  virtual COLORREF GetColumnBGColor( DWORD_PTR lParam , int nCol, DWORD nColumnID  ) = 0;
  virtual int     GetImage(DWORD_PTR lParam) const = 0;
  virtual long    Get_RowHeight(DWORD_PTR lParam) = 0;
  virtual BOOL    GetItemText(DWORD_PTR lPatam , int nCol , DWORD nColumnID , WCHAR* strText , const DWORD nStrMaxSize ) = 0;
  virtual BOOL    SetItemText(DWORD_PTR lPatam , int nCol , DWORD nColumnID , const WCHAR* strText) = 0;
};

//////////////////////////////////////////////////////////////////////////
// Default MultiListColorProfile
//
#define MCLPM_SELECTED       0x0001  // Set/Get the colors for selected items
#define MCLPM_CELLFOCUS      0x0002  // Set/Get the colors for cell in focus
#define MCLPM_FOCUS          0x0004  // Set/Get the colors for items in focus
#define MCLPM_NORMAL         0x0008  // Set/Get the normal color (not selected,not in focus, not checked)
#define MCLPM_CHECKED        0x0010  // Set/Get the color for checked items
#define MCLPM_CHECKEDFOCUS   0x0020  // Set/Get the color for checked items in focus
#define MCLPM_GRIDLINE       0x0100  // Set/Get the color for the gridlines
#define MCLPM_FOCUSRECT      0x0200  // Set/Get the color for the focus rectangle
#define MCLPM_ACTIVEBG       0x1000  // Set/Get the color for the Active Background
#define MCLPM_ACTIVEALTBG    0x2000  // Set/Get the color for the Active Alternative background
#define MCLPM_INACTIVEBG     0x4000  // Set/Get the color for the Inactive Background
#define MCLPM_INACTIVEALTBG  0x8000  // Set/Get the color for the Inactive Alternative background
#define MCLPM_ALL            0xFFFF  // Set/Get All colors

// MulitListColorProfile Flags
#define MLCPF_HIGHLIGHTACTIVEBG 0x00000100L // Allow different background color for Active / Inactive panels
#define MCLPF_OVERRIDEITEMBG    0x00000200L // Override the item specified BG color, The specified background color will ALWAYS be used. for ALL items..
#define MCLPF_ALTERNATIVEBG     0x00000400L // Allow alternating background colors. where odd/even rows has different background colors.
#define MCLPF_COLORRULES        0x00001000L // Color rules are active
#define MCLPF_RESETCOLOR        0x01000000L // Will reset all colors to default before using the colors in the profile to set


struct MultiListColorProfile
{
  SHORT     Mask;

  DWORD     dwFlags;

  // (COLORREF)-1 = use Default color (only valid for background colors)
  // (COLORREF)-1 on Forground color, Then that color will not be set.
  COLORREF crSelectedFG;
  COLORREF crSelectedBG;

  COLORREF crCellFocusFG;
  COLORREF crCellFocusBG;

  COLORREF crFocusFG;
  COLORREF crFocusBG;

  COLORREF crNormalFG;
  COLORREF crNormalBG;

  COLORREF crCheckedFG;
  COLORREF crCheckedBG;

  COLORREF crCheckedFocusFG;
  COLORREF crCheckedFocusBG;

  COLORREF crDefaultActiveBG;
  COLORREF crDefaultInActiveBG;

  COLORREF crDefaultActiveAltBG;
  COLORREF crDefaultInActiveAltBG;

  COLORREF crGridlinesV;
  COLORREF crGridlinesH;

  COLORREF crFocusRect;
  bool bForceFocusRectColor;
};

class IMultiList_ComboboxItems;
class __declspec(novtable) IMultiListViewInterface : public IHObject , public ISetEvent
{
public:

  // Obsolete. DO NOT USE
  virtual void	SetListMode( USHORT nMode ) = 0;
  virtual USHORT  GetListMode() = 0;

  virtual void AcceptDropFiles( bool b ) = 0;

  // Allow drag 'n' drop internal or between modules. 
  // ON by default
  virtual void RegisterDragDropTarget( bool b ) = 0;

  // ZSGS_ flags
  virtual BOOL	SetStyle( long lStyleFlags ) = 0;
  virtual long	GetStyle() = 0;
  virtual long	ToggleStyle( long lStyleFlag ) = 0;

  virtual void  SetEditOptions(DWORD dwOptions) = 0;
  virtual DWORD GetEditOptions() = 0;

  virtual BOOL  SetImageList( ZHANDLE hCtrl , DWORD dwImgType , BOOL bShared , BOOL bSmallIcons) = 0; // OBSOLETE. DO NOT USE 
  virtual bool  SetImageList( DWORD dwImgType , BOOL bShared , MCIconSize iconSize) = 0;

  // 1 = SMALL 16x16 , 2 = Large 32x32
  virtual void         SetIconType(MCIconSize nIconType) = 0;
  virtual MCIconSize   GetIconType() = 0;

  virtual void  SetSortMode(int nSortMode) = 0;
  virtual int   GetSortMode() = 0;

  virtual BOOL	SetLayout( int nItem1 , int nItem2 = 0 , int nItem3 = 0, int nItem4 = 0, int nItem5 = 0 ) = 0;
  virtual void  SetMargin( int nLeft = -1, int nRight = -1 , int nTop = -1 , int nBottom = -1 ) = 0;

  virtual void  SetRowPadding(long lPadding) = 0;
  // Set to 0 to use dynamic height. 
  virtual void	SetFixedHeight( DWORD nHeight ) = 0;

  // TODO - Rename to UseRowCallbackInterface()
  virtual BOOL    SetUseCallBackInterface( BOOL b , IMLRowCallBack* pProxy ) = 0;

  //									Normal FG / BG             Foucs FG / BG                CellFocus GB/BG                Selected Item FG/BG
  virtual BOOL	SetDefaultColors(  COLORREF fg, COLORREF bg ,COLORREF ffg  , COLORREF fbg, COLORREF cffg , COLORREF cfbg, COLORREF sfg  , COLORREF sbg)=0;
  virtual void	SetDefaultBG( COLORREF bg , bool bOverrideBGColors ) = 0;

  virtual BOOL  GetColorProfile(MultiListColorProfile* pColorProfile) = 0;
  virtual BOOL  SetColorProfile(MultiListColorProfile* pColorProfile) = 0;

  // If bForFiles is true the colorprofile will be set as the configuration is for files
  // If bForFiles is false the colorprofile will be set as the configuration is for folders
  virtual BOOL  LoadExplorerPanelColorProfile(bool bForFiles, MultiListColorProfile* pColorProfile) = 0;

  // Will call LoadExplorerPanelColorProfile and SetColorProfile in one call
  virtual BOOL  SetExplorerPanelColors(bool bForFiles) = 0;
  virtual BOOL  SetExplorerPanelFontAndIconSettings(bool bSetSharedFileItemImageList = true) = 0;

  virtual void    SetRedraw( BOOL bRedraw ) = 0;

  virtual BOOL UnBindKey( UINT nVKCode , UINT nFlags ) = 0;
  virtual BOOL BindKey( UINT nVKCode , UINT nFlags , DWORD dwCmd , DWORD_PTR lParam) = 0;
  virtual BOOL BindAsciiKey( TCHAR nChar , UINT nFlags , DWORD dwCmd , DWORD_PTR lParam ) = 0;

  // Column Function
  virtual BOOL	AddColumns( int nCol , long lWitdh , long lFlags , long lTextId , const WCHAR* strText = NULL, BYTE nColID = 0 ) = 0;
  virtual DWORD GetColumnFlags( int nCol ) = 0;
  virtual BOOL  SetColumnFlags( int nCol , DWORD nFlags ) = 0;
  virtual int   GetColumnFromColID( BYTE nColID ) = 0;
  // Must be run after # of column been changed
  virtual BOOL	UpdateColumns() = 0;

  // Row Functions
  virtual ZHANDLE CreateRowItem( DWORD_PTR dwCustomData = 0 ) = 0;
  virtual BOOL	SetColumnItem( ZHANDLE hRow , int nColIdx , long lCtrlFlags , long TextID , DWORD_PTR lParam=0 ) = 0;
  virtual BOOL	SetColumnItem( ZHANDLE hRow , int nColIdx , long lCtrlFlags , const WCHAR* strText , DWORD_PTR lParam=0 , DWORD_PTR lDataParam=0 , COLORREF crFG=-1 , COLORREF crBG = -1 ) = 0;	
  virtual bool	SetColumnText( ZHANDLE hRow , int nCol , const WCHAR* strText ) = 0;
  virtual bool	SetColumnParam( ZHANDLE hRow , int nCol , DWORD_PTR lParam ) = 0;
  virtual bool	SetColumnDataParam( ZHANDLE hRow , int nCol , DWORD_PTR lParam ) = 0;


  virtual BOOL	SetRowImage( ZHANDLE hRow , int nIndex ) = 0;

  virtual ZHANDLE	InsertRow( ZHANDLE hRow ) = 0;
  virtual ZHANDLE	InsertRow( ZHANDLE hRow , ZHANDLE hParentBranch ) = 0;

  virtual BOOL    RemoveRow( ZHANDLE hBranch , BOOL bUpdate = TRUE ) = 0;

  // Rows need to be inserted FIRST before the group function can be run
  virtual BOOL	AddToGroup( const WCHAR* strGroup , ZHANDLE hRow , int col ) = 0;
  //	virtual BOOL    RemoveGroup( int groupID ) = 0;
  //	virtual BOOL    RemoveGroup( int groupID , ZHANDLE hRow , int col ) = 0;


  // If a Combobox item is assinged to a rowdata. then it will delete it. else you need to delete it
  // your self
  virtual IMultiList_ComboboxItems* QueryInterface_ComboboxItems( ZHANDLE hItems ) = 0;
  virtual void					  ReleaseInterface_ComboboxItems( IMultiList_ComboboxItems* pItems ) = 0;

  virtual ZHANDLE CreateComboBoxItem( long lWidth , const WCHAR* wsText , long lFlags ) = 0;
  virtual BOOL	ComboBoxItem_AddItem( ZHANDLE hCmbItems , const WCHAR* strText , long kValue , long lCmd  ) = 0;
  virtual BOOL	ComboBoxItem_AddItem( ZHANDLE hCmbItems , const WCHAR* strText , const WCHAR* strValue , long lCmd  ) = 0;

  // crText = -1 if only a color select popup. and not a text & bk selection
  virtual ZHANDLE CreateColorPickItem( COLORREF cr, COLORREF crText, long lWidth ,  const WCHAR* wsCustomText , const WCHAR* wsDefaultText ) = 0;
  virtual BOOL    GetColorData( ZHANDLE hColorData , long* plWidth , COLORREF *cr ) = 0;

  virtual ZHANDLE CreateButtonData( long lWidth , long lButtonType , const WCHAR* wsButtonText , long lParam1=0 , long lParam2=0 , const WCHAR* wsParam1=NULL , const WCHAR* wsParam2=NULL ) = 0;
  virtual BOOL    GetButtonData( ZHANDLE hButtonData , long* plWidth , long *plButtonType , long* plParam1, long* plParam2 , WCHAR* wsParam1 , WCHAR* wsParam2 , int strMaxSize ) = 0;

  virtual int     AddIcon(  const WCHAR* Filename , int nIndex ) = 0;
  virtual int     AddIcon(  const HICON hIcon ) = 0;

  virtual BOOL	SetFont( const WCHAR* FaceName , int Size , long FontFlags ) = 0;
  // 1 = 16x16 (small), 2 = 32x32 (Large)
  virtual BOOL  SetIconSize(MCIconSize nIconSize) = 0;
  
  // Set PreItem like CheckBox, TreeImage size.
  virtual void SetPreItemSize(int nXY) = 0;

  virtual void  SetFocusNotification(bool b) = 0;
  virtual void	SetFocus() = 0;

  // if bRedrawOnly is FALSE scrollbar will also be adjusted and it will recalculate internal data
  // just Update( FALSE ); when you have inserted many rows at once.
  virtual BOOL    Update( BOOL bRedrawOnly = TRUE ) = 0;
  virtual BOOL	UpdateItem( ZHANDLE hRow ) = 0;

  virtual BOOL  ResizeColumns() = 0;
  virtual void  Sort(BOOL bUpdate) = 0;

  virtual BOOL	ExpandAll() = 0;
  virtual BOOL	CollapseAll() = 0;
  virtual BOOL  Expand(ZHANDLE hRow , bool bRecursive) = 0;
  virtual BOOL  Collapse(ZHANDLE hRow , bool bRecursive) = 0;
  virtual BOOL	Clear() = 0;

  // if nColIdx is -1 it will find the correct colid based on nColumnID
  virtual INT   EditCell(ZHANDLE hRow, int nColIdx, int nColumnID) = 0;
  //=========================================================
  // Get Functions
  //=========================================================
  //	Return How many items there exits in the MultiListCtrl
  virtual long	GetItemCount( ) = 0;	
  //	Get an Row Handle by Index Position
  virtual ZHANDLE GetItem(  long nIndex ) = 0;

  // Return ListItem of a Index Position
  virtual ZHANDLE GetItemData( DWORD nIndex ) = 0;

  virtual ZHANDLE GetItemByCustomData( DWORD_PTR dwCustomData ) = 0;

  // Get Handle to Row Data from a Row Handled ( returned by InsertRow )
  virtual ZHANDLE GetRowData( ZHANDLE hListItem ) = 0;

  virtual ZHANDLE GetParentItem( ZHANDLE hListItem ) = 0;

  virtual DWORD_PTR GetCustomParam( ZHANDLE hBranch ) = 0;
  virtual DWORD_PTR	GetCustomParamByRowIndex( DWORD nIndex ) = 0;

  virtual int		GetFocusIndex() = 0; // return Index for item that are in focus
  virtual bool	SetFocusIndex( DWORD nIndex ) = 0; // Set Item at Index X as focus.
  virtual bool  SetFocusAndSelection(DWORD nIndex) = 0; // You probably want to use this instead of just SetFocusIndex(...)

  virtual DWORD GetRowIndex( ZHANDLE hRow ) = 0;

  virtual void  SetItemCheckAll(BOOL bCheck) = 0;
  virtual void  SetItemCheck( ZHANDLE hRow , BOOL bCheck ) = 0;

  // 0 = Not Checked, 1 = Checked , 2 = Partial Checked 
  virtual INT		GetItemCheck( ZHANDLE hRow ) = 0;

  virtual BOOL  GetColumnData( ZHANDLE hRow , int nCol , long* lCtrlFlags=NULL, DWORD_PTR* lParam=NULL , DWORD_PTR* lDataParam=NULL , COLORREF* crFG=NULL,COLORREF* crBG = NULL ) = 0;
  virtual BOOL  GetColumnFlags( ZHANDLE hRow , int nCol , long* lCtrlFlags=NULL ) = 0;
  virtual BOOL  GetColumnColors( ZHANDLE hRow , int nCol , COLORREF* crFG=NULL,COLORREF* crBG = NULL ) = 0;
  virtual BOOL	GetColumnParam( ZHANDLE hRow , int nCol , DWORD_PTR* lParam=NULL , DWORD_PTR* lDataParam=NULL )  = 0;
  virtual BOOL	GetColumnText( ZHANDLE hRow , int nCol , WCHAR* strText , long strSize ) = 0;

  // Fill a collection with the items that are check, 
  // if bDataParam is TRUE fill the collection with the CustomData Parameter for column 0
  // if bChilds also do Childs items
  virtual long    GetCheckItems( IHCollection *pCollection , BOOL bDataParam , BOOL bChilds , BOOL bIgnore3State = FALSE , BOOL b3StateChildsOnly = FALSE ) = 0;

  virtual void	GetCheckStatus( DWORD& nItems , DWORD& nChecked , DWORD& n3StateChecked , BOOL bChilds ) = 0;

  // Move Selected/Focused items(s)
  virtual bool OnMoveUp() = 0;
  virtual bool OnMoveDown() = 0;
  virtual bool OnMoveTop() = 0;
  virtual bool OnMoveBottom() = 0;
  virtual bool OnMoveAfter(MCNS::ZHANDLE hRowItem) = 0;
  virtual bool OnMoveBefore(MCNS::ZHANDLE hRowItem) = 0;
};

class __declspec(novtable) IMultiList_ComboboxItems : public IHObject
{
public:

  // Text to show before or after the combobox.
  virtual void SetLabelText( const WCHAR* strText ) = 0;
  virtual bool GetLabelText( WCHAR* strText , int nLength ) = 0;

  // Width of the combobox. -1 and it will use entire column
  virtual void SetWidth( int nWidht ) = 0;
  virtual int  GetWidth() = 0;

  virtual void SetSelected( int nIndex ) = 0;
  virtual int  GetSelected( ) = 0;

  virtual bool GetText( int nIndex , WCHAR* strText , int nLength ) = 0;
  virtual bool GetValue( int nIndex , WCHAR* strValue , int nLength ) = 0;
  virtual bool GetValue( int nIndex , /*[out]*/long* lValue ) = 0;

  // Only valid if combobox is in DropDown Mode and there is a edit ctrl
  virtual void SetEditText( const WCHAR* strText ) = 0;
  virtual bool GetEditText( WCHAR* strText , int nLength ) = 0;

  virtual long Count() = 0;

  virtual int  FindItemByText( const WCHAR* strText , bool bCaseSensative = false ) = 0;
  virtual int  FindItemByValue( const WCHAR* strText , bool bCaseSensative = false ) = 0;
  virtual int  FindItemByValue( long lValue ) = 0;

  // Slower.
  virtual bool AddItemFront( const WCHAR* strText , const WCHAR* strValue , long lCommand ) = 0;
  virtual bool AddItemFront( const WCHAR* strText , long lValue , long lCommand ) = 0;

  // Faster - will add to back
  virtual bool AddItem( const WCHAR* strText , const WCHAR* strValue , long lCommand ) = 0;
  virtual bool AddItem( const WCHAR* strText , long lValue , long lCommand ) = 0;

  virtual void Clear() = 0;
};
MCNSEND