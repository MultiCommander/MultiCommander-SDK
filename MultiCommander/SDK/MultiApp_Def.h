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

#include <tchar.h>

#ifndef _MULTIAPP_DEF_H_
#define _MULTIAPP_DEF_H_

#include "Multi_Type.h"


enum class MCImageListSize
{
  None = 0,
  Small = 1,   // 16x16  100%
  Medium = 2,   // 24x24  150%
  Large = 3,   // 32x32  200%
  XLarge = 4    // 48x48 
};

enum class MCIconSize
{
  None = 0,
  Small = 1,   // 100%
  Medium = 2,   //150%
  Large = 3,   // 200%
  XLarge = 4    // 250%
};


#include "MCNamespace.h"
MCNSBEGIN

//////////////////////////////////////////////////////////////////////////
// IPluginInterface - Module Flags - returned by GetModuleFlags()
#define ZMIF_INTERNAL       0x0000100L  // Internal Module (Only use this for module that are built into MultiCommander.exe )
#define ZMIF_RESERVED       0x0001000L  // Reserved
#define ZMIF_SENDTO         0x0002000L  // Support Items (fileitems) to be Sent To Module  // Is it really used anymore ?
#define ZMIF_QUERYSENDTO    0x0004000L  // Query the module every time for the SendTo request  // OBSOLETE
//*******************************************************************************

// GetTabSettings -> Flags
#define TABSETTING_GRADIENT        0x00000001 // Button will be shown using gradient color, Without it solid color is drawn
#define TABSETTING_HOTWHENACTIVE   0x00000002 // Show the Active tab with the color use when hovering over the tab. (Background is slightly lighter then set color)
//#define TABSETTING_ICON            0x00000004 // 
//#define TABSETTING_HIGHLIGHTACTIVE 0x00000000 // Highlight the active tab by showing it with a lighter color

// Controller type ID - Used in
// CreateControl(...), QueryInterface(...), CreateView(..) CreateViewQI(...)

//  ZCTRL_ are windows controls that can be added to toolbars, statusbar, other UI that can store UI components.
#define ZCTRL_COMBOBOX         1 // ComboBox Control
#define ZCTRL_EDIT             2 // Edit Control
#define ZCTRL_STATIC           3 // Static Control
#define ZCTRL_LABEL            4 // Label Control (Like Static but with more options )
#define ZCTRL_BUTTON           5 // Button Control
#define ZCTRL_MENU             9 // Menu 
#define ZCTRL_HOTKEY          10 // HotKey Control
#define ZCTRL_CRUMBBAR        11 // Crumb bar control
#define ZCTRL_DRVCOMBO        20 // Drive ComboBox  ( Obsolete ?, use VolumeDeviceCombo Instead )
#define ZCTRL_DEVCOMBO        30 // Volume Device ComboBox
#define ZCTRL_DEVCOMBO2       31 // Volume Device ComboBox2 (custom drawm box, support darkmode)

// Views
#define ZCV_TREEVIEW              100  // Tree List View
#define ZCV_RICHEDIT              200  // RichEdit View   -  OBSOLETE - Not used
#define ZCV_EDITVIEW              300  // Dummy Edit view -  OBSOLETE - Not used
#define ZCV_LISTVIEW              400  // ListCtrl view
#define ZCV_MULTILISTVIEW         500  // MultiList view
#define ZCV_FILESYSTEMBROWSERVIEW 600  // FileSystemBrowser View
#define ZCV_FOLDERTREEVIEW        610  // FolderTree view
#define ZCV_FILEWORKSPACEVIEW     650  // FileWorkspace view

#define ZCV_TOOLBAR               700  // A Toolbar view
#define ZCV_COMMANDBAR            710  // A Commandbar view
#define ZCV_STATUSBAR             750  // Status Bar (also used as area to place other UI components on, like Buttons, texts, edit fields and so on)
#define ZCV_DEVICEBAR             760  // Device bar (Shows button for devices)

#define ZCV_SPLITTERVIEW        1000   // Split view - Dynamic or static splitting - Use for example to place multiple UI components inside a tab panel
#define ZCV_GSETTINGSVIEW       1500   // Generic settings view

#define ZCV_CORE                2800   // Core - Only valid if module is Internal

// Object  - Used to Query a data object from the interface layer.
#define ZOBJ_FILESYSTEM         3000 // FileSystemManager interface..  Interact with the internal filesystem
#define ZOBJ_FILEMONITOR        3010 // Notify file system about changes. 
#define ZOBJ_PROPMANGER         3100 
#define ZOBJ_COLUMNMANAGER      3200
#define ZOBJ_RUNASADMIN         3250 
#define ZOBJ_RESERVED           3300

// Flags for SetImageList
#define ZIMGLST_FILEITEM      0x0010  // Set The imagelist with cached FileItem icons
#define ZIMGLST_CREATE        0x0100  // Create Image list
#define ZIMGLST_1616          0x0200  // Create new 16x16 Image list
#define ZIMGLST_3232          0x0400  // Create new 32x32 Image list

// ZCTRL_COMBOBOX - Style flags
#define ZCCF_CB_DROPLIST    0x0000001L  // Make combobox have a DropList

 // ZCTRL_LABEL - Style flags
#define ZCP_LABEL_ALIGN_LEFT    0x00000100L
#define ZCP_LABEL_ALIGN_RIGHT   0x00000200L
#define ZCP_LABEL_ALIGN_CENTER  0x00000400L
#define ZCP_LABEL_NOWORDBREAK   0x00000800L
#define ZCP_LABEL_PATHELLIPSIS  0x00001000L
#define ZCP_LABEL_END_ELLIPSIS  0x00002000L
#define ZCP_LABEL_VCENTER       0x00004000L
#define ZCP_LABEL_CLICKANDEDIT  0x00008000L

// ZCTRL_BUTTON - Style flags
#define ZCP_BUTTON_CHECK        0x00000100L // Button is of Check type

// ZCV_TOOLBAR - Style flags
#define ZCP_TB_TOOLTIP        0x00000800L // Use the Tooltip enhanched version of Controllers ( DO not use this if toolbar is added to tabbed toolbar in main app )
#define ZCP_TB_BORDER_BOTTOM  0x00001000L // Add a Bottom Border
#define ZCP_TB_BORDER_TOP     0x00002000L // Add a Top Border
#define ZCP_TB_BORDER_3D      0x00004000L // 3D Boarder
#define ZCP_TB_FLAT           0x00008000L // Flat Style

// ZCV_SPLITTERVIEW - Style flags
#define ZCP_FIXEDSIZE       0x00010000L // This active splitter view will not be resizeable
#define ZCP_DYNAMICKEEP     0x00020000L // This active splitter view will not be resizeable
#define ZCP_NOBORDER        0x00040000L

// Tab Panel constants - Used in many places
#define ZCP_TAB_LEFT        0x01000000L // Put the created view in the left tab
#define ZCP_TAB_RIGHT       0x02000000L // Put the created view in the right tab
#define ZCP_TAB_ACTIVE      0x04000000L // Put The created view in the Active tab
#define ZCP_TAB_NOTACTIVE   0x08000000L // Put The created view in the none active tab

// FileSystem Icon handler flags
#define FSICON_GETSMALL         0x00000001L
#define FSICON_GETLARGE         0x00000002L
#define FSICON_GETMEDIUM        0x00000004L
#define FSICON_GETXLARGE        0x00000008L

#define ZFSICON_ASSOCIATED      0x00000010L // Get Accosiated Fileitem
#define ZFSICON_EXE             0x00000020L // Get Icon from .EXE (.cur , and ico )
#define ZFSICON_LINK            0x00000040L // Get the Icon from shortcut that the link points to
#define ZFSICON_OVERLAY         0x00000100L // Get Overlay icon
#define ZFSICON_OVERLAY_REMOTE  0x00000200L // Get Overlay icon if fileitem is on remote device

  /*
  #define FSICONSIZE_SMALL        1 // 16x16   - SHIL_SMALL
  #define FSICONSIZE_MEDIUM       2 // 32x32   - SHIL_LARGE
  #define FSICONSIZE_LARGE        3 // 32x32   - SHIL_LARGE
  #define FSICONSIZE_XLARGE       4 // 48x48   - SHIL_EXTRALARGE
  */
  //#define FSICONSIZE_JUMBO        4 // 256x256 - SHIL_JUMBO


// MultiList View / FileSystemBrowser View - Style Flags
#define MLVS_CHECKBOXES       0x00000100L // Show checkboxes at the beginning of every row item
#define MLVS_VGRIDLINES       0x00000200L // Draw vertical grid lines
#define MLVS_HGRIDLINES       0x00000400L // Draw horizontal grid lines
#define MLVS_CHKBOXINHERIT    0x00000800L // Inherit Checkbox status. if unchecking one subitem the parent item will be grayed.  if parent is uncheck no check mark will be shown on sub items.
#define MLVS_ITEMICONS        0x00001000L // Show item icon at the beginning of every row item
#define MLVS_TREELINES        0x00002000L // Draw Tree Lines and indentions
#define MLVS_TREECOLLAPSE     0x00004000L // Draw Collapse/Expand symbol for treelines
#define MLVS_ADJUSTSTRINGSIZE 0x00008000L // "..." at the end of string if they do not fit in column
#define MLVS_UNDERLINEHOT     0x00010000L // Underline hot item
#define MLVS_NOFOCUSRECT      0x00020000L // Don't Draw a Focus rect (CHANGE THIS.. Never have negative options. )
#define MLVS_FULLROWFOCUSRECT 0x00040000L // Draw the focus rect for the full row
#define MLVS_AUTOSIZECOLUMNS  0x00080000L // Auto size columns
#define MLVS_NOBORDER         0x00100000L // Draw no border
#define MLVS_INNEREDGE        0x00200000L // Draw Inner edge
#define MLVS_ANYKEYSEARCH     0x00400000L // Enable any key search
#define MLVS_SEARCHBOX        0x00800000L // Show Search box for inline search for items
#define MLVS_PROPERTIES_MODE  0x01000000L // The Controller is in properties mode. focus is always on second column and is always entering edit mode
#define MLVS_CHKIFSUBITEMS    0x02000000L // Only for checkbox item, If it has subitems and IS a root item (req ZGDF_CHECKBOX also to be set )
#define MLVS_SELONNAVIGATE    0x04000000L // Auto check when doing multi selection
#define MLVS_UNSELONSINGLESEL 0x08000000L // Auto Uncheck all when doing single selection
#define MLVS_SELECTIONRECT    0x10000000L // Enable selection using selection rectangle
#define MLVS_HIGHLIGHTSORTCOL 0x20000000L // Highlight sort column
#define MLVS_DBGSELECTION     0x80000000L // Enable focus/selection status debugging.

/*
// Selection Rect Flags
#define ZSGR_ENABLE       0x0001    // Enable Mouse Selection Rectangle
#define ZSGR_CHECKITEM    0x1000    // Check affected items  (not used yet)
#define ZSGR_SELECTITEM   0x2000    // Select affected items (not used yet)
*/

// MultiList PreItem Layout items
#define MLPI_CHECKBOX      1
#define MLPI_IMAGE         2
#define MLPI_TREEIMAGES    4

// ZGSettingsView Flags 
#define ZGSV_APPLY    0x001 //  Add a Apply Button
#define ZGSV_CANCEL   0x002 //  Add a Cancel Button
#define ZGSV_SAVE     0x004 //  Add a Save Button
#define ZGSV_DEFAULT  0x008 //  Add a 'set default' button


///////////////////////////////////////////////////////////////////
// MultiList View -  Column Style Flags
///////////////////////////////////////////////////////////////////

// PreItem Order - IMultiListViewInterface::SetLayout(..) - In what order the preitem should be drawn
#define ZSGPI_CHECKBOX    1
#define ZSGPI_IMAGE       2
#define ZSGPI_TREEIMAGES  4

// Column Text Draw style - IMultiListViewInterface::AddColumns
#define ZSGC_LEFT         0x00000010L // Left Align Column 
#define ZSGC_RIGHT        0x00000020L // Right Align Column
#define ZSGC_CENTER       0x00000040L // Center Column text
#define ZSGC_TEXTFIRST    0x00000080L // if a Controller can be both text and the control and this flags is set 
                                      // Then the Text is draw before the controler otherwise it is draw after
                                      // ComboBox can have this option and (Button and CheckBox(not yet))

// Column Draw style - IMultiListViewInterface::AddColumns ( Can be combined with ZSGC_ above. )
#define IMLC_OVERDRAW         0x0200 // This column should overdraw if next column text is empty
#define IMLC_ALLOW_OVERDRAW   0x0400 // Allow this column to be overdraw by previous if this column text is empty
#define IMLC_SORT_DECENDING   0x0800 // if not set it will sort ascending descending as default
#define IMLC_STRETCH          0x1000 // AutoSize only this column ( if ZGHF_AUTOSIZECOL need to be active )
#define IMLC_HIDE_THUMBDETAIL 0x4000 // Do not draw this column in thumbnail detail mode

//////////////////////////////////////////////////////////////////////////
// MultiList Notification
//////////////////////////////////////////////////////////////////////////
#define NML_EDIT_CHANGED	1007	// Content of and item of 'edit' type has changed


// Cell Click Edit Modes
#define MLC_EDIT_MUSTBESELECTED  0x0001  // Item need already be selected.
// - Else when user click on a new cell it will enter edit mode directly.
#define MLC_EDIT_CLICKANDWAIT    0x0002  // Enter Edit mode of cells in a Click and Wait mode.
#define MLC_EDIT_DBLCLICK        0x0004  // Go into edit mode if user dblclick on cell.
//#define MLC_EDIT_DISABLE         0x0010  // Disable editing of cell with edit type.

#define ML_SORTMODE_TEXT      1 // Normal Sorting
#define ML_SORTMODE_NATURAL   2 // Natural/Logical Sorting. (will sort  1,2,3,10,20 instead of 1,10,2,20,3 )
#define ML_SORTMODE_NUM       3 // Number sorting 
#define ML_SORTMODE_CALLBACK  4 // Handle sorting your self.

///////////////////////////////////////////////////////////////////
// MultiList Column Cell Type 
//-----------------------------------------------------------------
// MLCT_TEXT can be combined with any other Control
//
//
#define MLCT_EMPTY              0x00000100L
#define MLCT_TEXT               0x00000200L // Control is a text only control
#define MLCT_STATIC             0x00000400L // Static Text ( Never Editable )
#define MLCT_COMBOBOX_DROPLIST  0x00000800L // ComboBox - With Drop list
#define MLCT_COMBOBOX_DROPDOWN  0x00001000L // ComboBox - Widht Dropdown list
#define MLCT_CHECKBOX           0x00002000L // CheckBox
#define MLCT_DATEPICK           0x00004000L // Date Picker
#define MLCT_COLORPICK          0x00008000L // Color Picker
#define MLCT_FONTPICK           0x00010000L // Font Pick ComboBox
#define MLCT_BUTTON             0x00020000L // Button
#define MLCT_ICON               0x00040000L // Icon
#define MLCT_IMAGE              0x00080000L // Image
#define MLCT_RADIO              0x00100000L // Radio
#define MLCT_NUMSPIN            0x00200000L // Number control with up/ down arrows in ( OBSOLETE - Not used )

// Button Flags ( OBSOLETE - not used )
#define MLBS_NORMAL       0x00000001L // Normal Push Button
#define MLBS_IMAGE        0x00000002L // Image Button
#define MLBS_CD_FONT      0x00000100L // Open Common Font Dialog on Click
#define MLBS_CD_FILEOPEN  0x00000200L // Open Common FileOpen Dialog
#define MLBS_CD_FILESAVE  0x00000400L // Open Common FileSave Dialog
#define MLBS_CD_SELFOLDER 0x00000800L // Open Select Folder Dialog

// MultiList Cell Style
#define MLCS_CENTER        0x00000001L // Force center alignment
#define MLCS_RIGHT         0x00000002L // Force right alignment
#define MLCS_LEFT          0x00000004L // Force left alignment
#define MLCS_TEXTSTRIKEOUT 0x00000400L
#define MLCS_TEXTBOLD      0x00100000L
#define MLCS_TEXTITALIC    0x00200000L
#define MLCS_TEXTUNDERLINE 0x00400000L
#define MLCS_GROUP         0x00800000L // Add this control to group
#define MLCS_READONLY      0x01000000L // Cell is read only.
#define MLCS_CHECKED       0x02000000L // if cell is checkable then its is checked
#define MLCS_UNCHECKED     0x04000000L // if cell is checkable then its is unchecked
#define MLCS_CHECKEDGRAY   0x08000000L // if cell is checkable then its is checkedgray
#define MLCS_EDGE          0x10000000L // Draw Edge .
#define MLCS_REQALIGN      0x20000000L // For customized Column alignment. will request GetTextFormat to get alignment
#define MLCS_OVERDRIDE_OD  0x40000000L // Override overdraw even if column is empty
#define MLCS_CTRLBG        0x80000000L // Draw ctrl background


// ReadWriteItem Object - Open Flags
/*
#define ZRW_CREATE_NEW      0x0000001L // Creates a new file. The function fails if the specified file already exists
#define ZRW_CREATE_ALWAYS   0x0000002L // Creates a new file. If the file exists, the function overwrites the file and clears the existing attributes.
#define ZRW_OPEN_EXISTING   0x0000004L // Opens the file. The function fails if the file does not exist. 
#define ZRW_ALWAYS          0x0000008L // Opens the file, if it exists. If the file does not exist, the function creates the file as if it was ZRW_CREATE_NEW 
#define ZRW_OPENREAD        0x0000100L // Open the item for READ operation
#define ZRW_OPENWRITE       0x0000200L // Open The Item for Write Operations
#define ZRW_PATHRELATIVE    0x0000400L // Path of item name is relative to the create path
*/

//////////////////////////////////////////////////////////////////////////
// Registered Command flags and styles

// Command Flags
#define ZCF_ENABLE              0x00000001L // This Command should be Enabled
#define ZCF_GRAYED              0x00000002L // Gray the Menus and Buttons acocieted with this command
#define ZCF_DISABLE             0x00000004L // Disable this command 
//#define ZCF_CHECKABLE           0x00000008L // This command is checkabled
#define ZCF_UNCHECKED           0x00000010L // This command should be presented as UnChecked 
#define ZCF_CHECKED             0x00000020L // This Command should be presented as Checked
#define ZCF_GRAYCHECK           0x00000040L // This command should be presented as grayed Checked.. 
   // Flags for how commands should be sent to the Extensions and plugins
#define ZCF_TARGET_ACTIVE       0x00000100L //  Send to ActiveTab only (if Modules ID matches ) (First Found Modules with matching ID of loaded modules)
                                            //  ZCF_TARGET_ACTIVE is not 100% correct. if how this works.
                                            //  _ACTIVE is the tab that has the ACTIVE marking. (almost same as Focus..)
#define ZCF_TARGET_NEW          0x00000200L //  Send to A New instance and create the new in the tab that have focus
#define ZCF_TARGET_NOTACTIVE    0x00000400L //  Send To Non ActiveTab instance that id matches
#define ZCF_TARGET_ANY          0x00000800L //  Send To Any..Start with Instances that are Active and focus
#define ZCF_TARGET_FOCUS        0x00001000L //  Instance Must be in Focus . 
#define ZCF_TARGET_THIS         0x00002000L //  Send ONLY to the instance connected to this instance of the module
#define ZCF_TARGET_IGNORE_MID   0x00004000L //  Combine with almost any of above. Modules ID will Be ignored when doing matching. ( _FOCUS or _ANY is properly only good combinations )
#define ZCF_INTERNAL            0x00010000L //  The Cmd Should be handled internally not sent to and module
#define ZCF_SYSTEM              0x00020000L // The command is a system command.. 
#define ZCF_USERDEFINEDCOMMAND  0x00040000L // the command is a user defined command
#define ZCF_QUERY_CMDUI         0x00100000L // Query about the command state, Disable/Check/UnChecked and so on

#define ZCF_SHOWONLYCHKINMENU   0x01000000L // If cmd is shown in menu. and cmd is checked. Only show the check for menu item
#define ZCF_CMDTYPE_FILE        0x10000000L // This command is a FILE item command that act on selected files or are releated to files/folders  (Commands with this flags are shown in Custom ContextMenu)
                                            // (having this flag make it possible for the user to see the command when customized the custom context menu )

//////////////////////////////////////////////////////////////////////////
/*
// Defined in winuser.h
#define FVIRTKEY  TRUE          // Assumed to be == TRUE
#define FNOINVERT 0x02
#define FSHIFT    0x04
#define FCONTROL  0x08
#define FALT      0x10
*/
#define FCUSTOMIZABLE 0x40  //(own Extended flag for hotkeys) If hotkey should be user customizable
#define FFORCE        0x80
#define HOTKEY_FFORCE 0x00800000 // FFORCE set to to Hiword for Joined VkKey + vkFlag -> DWORD

// FileManager Columns
#define ZFV_FILENAME    1 // The FileName Field will be draw in the filemanager
#define ZFV_EXT         2 // The File Extetion Field will be drawn
#define ZFV_SIZE        3
#define ZFV_DATE        4
#define ZFV_ATTRIB      5
#define ZFV_EXTRA       6
#define ZFV_PATH        7

// Extended Columns
#define ZFVX_FILEVERSION 10
#define ZFVX_PRODVERSION 11
#define ZFVX_COMPANY     12
#define ZFVX_COPYRIGHT   13
#define ZFVX_DESCRIPTION 14
#define ZFVX_COMMENT     15
#define ZFVX_OSTYPE      16 // 32 or 64bit
#define ZFVX_SHORTNAME   19

#define ZFVX_DATECREATE  21
#define ZFVX_DATEMODIFY  22
#define ZFVX_DATEACCESS  23

#define ZFVX_FRAGMENTS   25
#define ZFVX_HARDLINKCOUNT 28
#define ZFVX_FILEINDEX     29
#define ZFVX_FILESEQUENCE 30
#define ZFVX_FILEVOLSN    31
#define ZFVX_NAMELEN      32
#define ZFVX_PATHLEN      33
#define ZFVX_COLORRULENAME     34
#define ZFVX_COLORRULEID      35
#define ZFVX_DBGFILEITEMINFO       37
#define ZFVX_DBGFILEATTRIBUTERAW   38
#define ZFVX_DBGFILEITEMFLAGSRAW   39

#define ZFVX_SVN_STATUS	    40
#define ZFVX_SVN_STATUS_ID  41
#define ZFVX_SVN_REVISION	  42
#define ZFVX_SVN_URL    	  43
#define ZFVX_SVN_URLSHORT	  44
#define ZFVX_SVN_AUTHOR  	  45
#define ZFVX_SVN_MIMETYPE	  46
#define ZFVX_SVN_LOCKOWNER	47

#define ZFVX_EXPANEL_ROW    60  // Explorer Panel - Row No

// Filemanager column Flags
#define ZFV_CF_NONE             0x0000
#define ZFV_CF_OVERDRAW         0x0200 // this column should overdraw if next column text is empty
#define ZFV_CF_ALLOW_OVERDRAW   0x0400 // allow this column to be overdraw by previous if this column text is empty
#define ZFV_CF_SORT_DECENDING   0x0800 // if set it will sort descending as default
#define ZFV_CF_STRECH           0x1000


// Filemanager Filesize type
#define ZFM_FILESIZE_BYTES    0
#define ZFM_FILESIZE_KBYTES   1
#define ZFM_FILESIZE_MBYTES   2
#define ZFM_FILESIZE_GBYTES   3
#define ZFM_FILESIZE_AUTO     4

//  AddToolBarTab flags
#define TTAB_TEXTNONE           0x0001  // No Text
#define TTAB_TEXTLABELS         0x0002  // Show Text Label, ( Under Icon )
#define TTAB_TEXTONRIGHT        0x0004  // Show Text Label to the right of the icon. ( else it is under )
#define TTAB_TEXTNOTEXTLABELS   0x0008  // Show NO Text Label

// 0x0010 - 0x01000 reserved
#define TOOLBAR_DEVICE          0x1000

// 
// Messages
// ==================================================================================
//
// Extensions Messages
//
// This are the messages that the MultiCommander Core can send to the Extensions/Plugins
// ** DO NOT send this message your self using windows SendMessage or PostMessage APIs - The result will not be good **
//
// Make sure your extensions message ids doesn't collide with message from the core. 
// All Extension Message/Command ID should be between 30000 -> 50000
//
//  20000 -> 21199 - Command Messages for Extensions/Plugins
//  21200 -> 21500 - Notification Messages
//  30000 -> 50000 - Extension/Plugin created messages
//  52001 -> 55000 - UI Notifications from MulitList,Workspace,FileView command/notify messages  (Todo - Move to 2xxxx range )
//
//
#define AM_INIT             20000 // Module have just been created
#define AM_DESTORY          20001 // Module is about to be destroyed
#define AM_CLOSE            20002 // Request have been made to close this modules
#define AM_RECENT_BACK      20003
#define AM_RECENT_FORWARD   20004
#define AM_RECENT_HISTORY   20005
#define AM_GETAUTOLOADPARAM 20006 // param1 and param2 are 2 sting that modules can file with some value.. MAX 2048 WCHAR big..
#define AM_POSTSTART        20007
#define AM_AUTOLOAD         20010 // Module is requested to auto load.. So do your autoload stuff
#define AM_SYSCOLORCHANGE   20020 // System colors have change so update colors on UI if needed.
#define AM_TABPANEL_CHANGE  20025 // a HWND located inside a tab for the Extension has change TabPanel. wParam is the HWND, lParam is the new panel.

#define AM_FRAMEWNDCLOSING  20309 // return FALSE to abort closing
#define AM_FRAMEWNDCLOSE    20310 // A FrameWnd was Closed.. Now clean up all you interfaces and handles to controller you put into that frame
#define AM_UITHRAEDINIT     20400 // UI Thread created by extension is ready to be initialized. (ready for gui to be added to it. ) 
                                  // This message will be sent from the NEW UI thread. So be carefull what you do when act on it.

#define AM_CONTEXTMENU      20450 // Command from the application contextmenu (like tab menu)
#define AM_SHOWCMDCONTEXTMENU  20455 // A Command request that a Context Menu to be shown. (See TBBTN_MODULE_CONTEXTMENU flag for InsertButton for Toolbarinterface )
#define AM_COMMANDUI        20500 // CommandUI. lParam is the MESSAGEID , wParam is a DWORD* that you enter return value into.
#define AM_SECUI_COMMAND    20600 // Command sent from Secondary UI thread. When extension get this command be aware that you are now NOT running in the main thread.

#define AM_URL_CHANGED      21000 // The User have enter a text and pressed enter in the URL bar
#define AM_URL_SELECTED     21001 // The user have selected an item in the drop down menu

#define AM_FSUPDATE         21012 // Filesystem is changed. so update the view.. lparam is item that should be focus IF it is not null

#define AM_RUNCUSTOMCMD     21018 // A Custom command that you have configured a controller for, Notifies back with this command 

#define AM_SETFOCUS         21020 // This Module have been set to focus.. Don't relay on this blindly
#define AM_KILLFOCUS        21021
#define AM_ACTIVATE         21022 // Application is Activated/Deactivated lparam TRUE/FALSE tell if activate on/off
#define AM_TIMER            21023

#define AM_COPYDATA         21100 // CopyData is requested
#define MM_GETCONTENT       21101 
#define AM_GETHISTORY       21102 // Get The History interface that the extension has. WPARAM is a IHistoryManager** 

// lPARAM Flags for AM_GETSELECTEDITEMS / AM_GETFILEITEMS messages below
#define MF_ALL           0x00000000 // (Default. both folders and files)
#define MF_FOLDERS       0x00000001 // Folder
#define MF_FILES         0x00000002
#define MF_INCFOCUS      0x00000010 // include focus item if not checked items are found
#define MF_ONLYFOCUS     0x00000020 // include only the focused item
#define MF_SELFIRST      0x00000040 // Only first selected item. If no selected then zero items are returned (0 or 1 items returned)
#define MF_INCFOCUSDOT   0x00001000 // include ".." item if focus is on that item (Must be combined with GF_ONLYFOCUS or GF_INCFOCUS )
#define MF_INCFOCUSFIRST 0x00002000 // include Focus item FIRST in the response. even if focused item is NOT selected.

// file items messages
#define AM_GETFILEITEMS           21110 // Request to get file items from current module ( WPARAM = IFileItemCollection* , LPARAM = Flags ? )
#define AM_GETFOCUSITEM		        21111
#define AM_GETFOCUSITEMPATH       21112 // wparam target wchar* , lparam = maxlen
#define AM_GETFOCUSITEMNAME       21113 // wparam target wchar* , lparam = maxlen
#define AM_GETSELECTEDITEMS       21215 // WParam = IFileItemsCollection* lPARAM = flags 
#define AM_SELECTFILEITEMS        21120// Request to select file items in current module ( WPARAM = IFileItemCollection* , LPARAM = 1 for Check, 0 for Uncheck )
#define AM_SELECTFILEITEMSEX      21121// Request to select file items in current module ( WPARAM = WCHAR* (Filter), LPARAM = 1 for Check, 0 for Uncheck )
#define AM_SELECTFILEITEMS_ALL		21122 // Request to Select/unselect All items , if WPARAM == TRUE then Select else unselect
#define AM_SELECTFILEITEMS_INVERT 21123 // request to Invert selections of items.
#define AM_SELECTFILEITEMS_ADVF   21130 // Request to select file items in current module using advanced filter( WPARAM = ZHANDLE to Filter , LPARAM = 1 for Check, 0 for Uncheck )

#define AM_REMEMBERSELECTED       21140 // If Plugin support it, Remember current selected items
#define AM_PACKFILEOPSTART        21141 // Pack/Unpack/Test file operation has started.

#define AM_EXECUTE				21150 // 
#define AM_VIEW           21151 // View file. (Will only be sent if a module is registered as a viewer )
#define AM_EDIT           21152 // Edit file (Will only be sent if a module is registered as a editor)
#define AM_VIEWEX         21153

// Notification Message.. this message might be handled or not.
// ComboBox Changed
#define NM_CMB_CHANGED        21200 // ComboBox Changed 
#define NM_DEVCMB_CHANGED     21201 // Device ComboBox Changed , wParam = DeviceItem handle, lParam = Index of selected item
#define NM_EDIT_OKCANCEL      21202 // User pressed return or esc. ( see lparam 1 if escape )
#define NM_EDIT_CHANGED       21203 // Text in Edit ctrl have changed

#define AM_SETTINGS_CHANGED             21500 // Settings have been changed.. Please Reload settings (Will be sent to ALL instances of the extension)
#define AM_SETTINGS_CHANGED_SHARED      21501 // (Sent before AM_SETTINGS_CHANGED is sent) Settings have been changed.. Please Reload settings (Will be sent only to the FIRST instance of the extension, Used to update shared settings for all instances of that extension)
#define AM_SETTINGS_CHANGED_SHARED_POST 21502 // Same as AM_SETTINGS_CHANGED_SHARED but are sent AFTER all the AM_SETTINGS_CHANGED have been sent.
#define AM_SETTINGS_CHANGED_TABCOLORS  21503 
#define AM_KEYS_CUSTOMIZED          21510 // User has customized keys. update your hotkeys.
#define AM_ADDCOLUMNTOVIEW          21700 // Add a column to view.  WPARAM is a pointer to a wchar_t with the column name.
#define AM_REMOVECOLUMNFROMVIEW     21701 
#define AM_RESETCOLUMNSINVIEW       21702

// Global Messages ( I know. Not a very good way of doing this. but.... )
#ifndef CMD_BASE
#define CMD_BASE        WM_APP
#define WM_ZCMD         CMD_BASE +  95
#endif

// Data eXchange flags between modules
#define ZXF_FILE        0x0001  // Windows File Object (?)
#define ZXF_ZFILEITEM   0x0002  // MultiApp File Items Objects
#define ZXF_IMAGE       0x0004  // Image Data
#define ZXF_AUDIO       0x0008  // Audio Data

//////////////////////////////////////////////////////////////////////////
//
// ZAction (Obsolete - Not used anymore ) - Action flags and types.
//
#define ZAEF_GLOBAL 0x0001 // Handle this command by globally by the framework
#define ZAEF_MODULE	0x0002 // Module of matching module ID should handle it
#define ZAEF_OWNER  0x0004 // if Owner of the control and module id is matching and ZAEF_MODULE. then send it direct to the owner module
//#define ZAEF_LEFT   0x0100 // Send to matching module on the LEFT side
//#define ZAEF_RIGHT  0x0200 // Send to matching module on the RIGHT side


//  - Defines what type of data is in the lParam Value
enum eContentDataType { 
  CDT_UNKNOWN = 0  
  , CDT_FILEITEM = 1  // a IFileItem 
  , CDT_FILEITEMS // a IFIleItemsCollection 
  , CDT_FILEPATH  // Path to a File
  , CDT_FILEPATHS // List of file Paths
  , CDT_FOLDERPATH  // Path to a Folder
  , CDT_FOLDERPATHS // List of folder paths
  , CDT_PATH    // Path to a File or a Folder
  , CDT_PATHS   // list of Paths to a File or a Folder
  , CDT_CURRENTPATH 
  , CDT_STRING  
  , CDT_INDEX     // Index of Item
  , CDT_PARAM     // Custom Parameter
  , CDT_PARAMS    // Custom Parameters 
  , CDT_KEYPARAM  // The param for the bindKey that was pressed to activate this command ( use if same command so attach to many keys )
  , CDT_NONE
};

#define ACTION_CMD_START 22000 // Command set to to ActionEvent must be higher or equal to this value
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//  KeyBinder
//
#define ZKB_NONE   0x0000 // None Extra Key
#define ZKB_SHIFT  0x0010 // Shift key 
#define ZKB_ALT    0x0020 // Alt Key
#define ZKB_CTRL   0x0040 // Ctrl Key
#define ZKB_ANY    0x0100 // Any Combo Key will result in a HIT. 
#define ZKB_FORCE  0x0800 // If Key Is already Bind. then unbind it and bind the new one

#define ZKB_CMD_MODULECMD 1100 // Will call the owner module   <IPluginInterface>::OnMessage( <command> )
                               // Assign Return for just the ListCtrl to send CMD_GOTO to the module
                               // Eg: m_pListCtrl->BindKey( VK_RETURN, ZKB_NONE | ZKB_FORCE , ZKB_CMD_MODULECMD, CMD_GOTO); 


//////////////////////////////////////////////////////////////////////////
// File Manager View - Key and Mouse Commands

// A lot of following command are not used and are obsolete.
// In 9 of 10 cases there are better way to do the things you want to do

#define ZM_FM_START       52000 // FileManager Command start here

// Commands that FileManager view support

// MultiList notifications - MANY are not sent. Many are obsolete and not used anymore (Todo - Cleanup )
#define ZM_FM_MOVEUP         52001 // Move Activecell Up
#define ZM_FM_MOVEDOWN       52002 // Move ActiveCell Down
#define ZM_FM_MOVELEFT       52003 // Move ActiveCell Left
#define ZM_FM_MOVERIGHT      52004 // Move ActiveCell Right

#define ZM_FM_MOVEUP_PAGE    52005 // Move Page Up
#define ZM_FM_MOVEDOWN_PAGE  52006 // Move Page Down
#define ZM_FM_MOVETOP        52007 // Move Page Top
#define ZM_FM_MOVEBOTTOM     52008 // Move Page Bottom

#define ZM_FM_REFRESH        52009 // Refresh/redraw view
// Check
#define ZM_FM_CHECK          52010 // Check/Mark Item
#define ZM_FM_TOGGLE_CHECK   52011 // Toggle Check
#define ZM_FM_UNCHECK        52012 // Uncheck Item
#define ZM_FM_CHECK_ALL      52013 // Check All
#define ZM_FM_UNCHECK_ALL    52014 // UnCheck All
#define ZM_FM_SCHECK_ALL     52015 // Check All Including Subitems
#define ZM_FM_SUNCHECK_ALL   52016 // Uncheck All Including Subitmes

#define ZM_FM_DELETE         52030 // Request to Delete Active Item
#define ZM_FM_EDIT           52031 // Request to Edit Item 
#define ZM_FM_RENAME         52032 // Request to Rename Item
#define ZM_FM_COPY           52033 // Request to Copy Item
#define ZM_FM_MOVE           52034 // Request to Move Item
#define ZM_FM_EXECUTE        52035 // Request To Execute Item
#define ZM_FM_MKDIR          52036 // Request to create folder

#define ZM_FM_SHELLMENU      52100 // Request Shell Menu on Item
#define ZM_FM_STOP           52199 // FileManager Command Stops Here

// File Manager mouse events
#define ZMM_FM_LB_CLICK       52200 // LeftButton Click
#define ZMM_FM_RB_CLICK       52201 // RightButton Click
#define ZMM_FM_LD_CLICK       52202 // LeftButton double Click
#define ZMM_FM_RD_CLICK       52203 // RightButton double Click
#define ZMM_FM_RB_DRAG        52204 // RightButton Drag
#define ZMM_FM_LB_DRAG        52205 // LeftButton Drag
#define ZMM_FM_LB_HOLD        52206 // Left Button hold down for 1 sec
#define ZMM_FM_RB_HOLD        52207 // Right Button hold down for 1 sec
#define ZMM_FM_LB_CLICKWAIT   52208 // LeftButtonClick and waited for 1sec without moving mouse 
#define ZMM_FM_RB_CLICKWAIT   52209 // RightButtonClick and waited for 1sec without moving mouse

// File Manager View - Notifications events sent back to the extension/plugin
// Notify Events for FileSystemView, MultiList, Workspace, 
#define ZMN_SELECTIONUPDATE   52300 // Selection Have Been Updated 
#define ZMN_CHECKUPDATE       52301 // Checkbox state updated
#define ZMN_PATHCHANGING      52302 // Filemanager path is about to be changed
#define ZMN_PATHCHANGED       52303 // Filemanager path have changed (only FileManager view )
#define ZMN_PATHUPDATED       52304 // Path was AutoUpdated. user did not do something for this to change. was update bacuse of file monitoring.
#define ZMN_ITEMSREMOVED      52305
#define ZMN_ITEMSDROPPED      52306 // Items dropped and added to MultiList
#define ZMN_ITEMSMOVED        52307 // Items Moved within MultiList
#define ZMN_ITEMCHANGED       52308
#define ZMN_PATHLOCKED        52309 // Path is locked. (the locked path is sent in wparam as WCHAR*)
#define ZMN_PATHUNLOCKED      52310 // Path got auto unlocked.  locked path disapared

#define ZMN_LOADDATA          52312
#define ZMN_SAVEDATA          52313
#define ZMN_CREATENEW         52315 //   Request the extension to create a new instance of it self. (lparam true for target, else source pane)

#define ZMN_PATHCHANGED2      52316  // same as  ZMN_PATHCHANGED BUT will not steal focus
#define ZMN_FILTERAUTOCHANGED 52317

// Contextmenu action Create Folder has been performed. So refresh folder WPARAM = TCHAR* (path) and set item (LPARAM*) (WCHAR*) as rename mode
#define ZMN_CMNEWFOLDER       52320

#define ZMN_MULTICHECKBEGIN   52330
#define ZMN_MULTICHECKFINISH  52331

// Workspace Notification Events
#define WSC_ADDSELECTION        52400 // Request to Add Selection
#define WSC_SUBSTRACTSELECTION  52401 // Request to Subtract Selection

// Module Messages
#define ZM_FM_GET_CUR_PATH      53220 // Return Current Path - Can be sent to any module to return that modules current path.

// Core General Message 
#define ZM_GM_SETTINGS      55000 // Start The Config and ModuleID as param1 if you want just for that module
#define ZM_GM_SETTINGSSHOW  55010


//////////////////////////////////////////////////////////////////////////
// ShowErrorDialog (..., DWORD dwOptions)
#define SED_SHOWRETRYASADMIN  0x00000100L
#define SED_SYSTEMERROR       0x00000200L // The ErrorCode is a Windows System Error Code. 

// Result flags from the ShowErrorDialog(..., DWORD dwOptions)
// 
// Some of the return codes are only valid for special errors.

#define SEDR_ABORT               0x00000001L // User Pressed Abort
#define SEDR_RESUME              0x00000002L // Resume file
#define SEDR_APPEND              0x00000004L // Append to the end of existing file

#define SEDR_SKIP                0x00000010L // Skip file
#define SEDR_SKIP_ALL            0x00000020L // Skip ALL
#define SEDR_SKIP_ALL_NOT_EMPTY  0x00000040L // Skip All 'Cant delete! Folder not empty.'
#define SEDR_SKIP_ALL_PATHTOLONG 0x00000080L // Skip all 'Path to Long' error message

#define SEDR_OVERWRITE              0x00000100L  // Overwrite File
#define SEDR_OVERWRITE_ALL          0x00000200L // Overwrite ALL files  ( Do not ask again )
#define SEDR_OVERWRITE_ALL_IF_NEWER 0x00000400L // Overwrite all if source file is newer  (Version + date/time check ) ( Do not ask again )
#define SEDR_OVERWRITE_ALL_SIZEDIFF 0x00000800L // Overwrite ALL if size differs (Version + date/time check ) ( Do not ask again )

#define SEDR_RENAME              0x00001000L   // Rename File. with the name in AlternativeFilename parameter
#define SEDR_AUTORENAME          0x00002000L  // Auto Rename
#define SEDR_AUTORENAME_ALL      0x00004000L  // Auto Rename All files that need it ( Do not ask again )

#define SEDR_REMOVE              0x00010000L   // Remove File
#define SEDR_REMOVE_ALL          0x00020000L   // Remove All ( Do not ask again) Also during copy if the ask to delete broken/incompletes.
#define SEDR_REMOVE_TARGET       0x00040000L  // Remove target file  

#define SEDR_REMOVE_READONLY     0x00100000L  // Remove File With Readonly Attribute
#define SEDR_REMOVE_READONLY_ALL 0x00200000L  // Remove ALL files with Readonly attribute ( Do not ask again )
#define SEDR_REMOVE_SH           0x00400000L  // Remove file with System or Hidden Attribute
#define SEDR_REMOVE_SH_ALL       0x00800000L  // Remove ALL files with SYSTEM/HIDDEN attribute

#define SEDR_FORCEALL_DELETE_SH 0x01000000L  // Force delete all System / hidden files 
#define SEDR_FORCEALL_DELETE_RO 0x02000000L  // force delete all Readonly files 

#define SEDR_REMOVE_BROKEN      0x08000000L  // Remove Broken.. dont ask
#define SEDR_KEEP_ALL           0x10000000L  // Keep all broken files.. dont ask..

#define SEDR_RETRY              0x40000000L  // Retry
#define SEDR_RETRY_ASADMIN      0x80000000L  // Retry as Administrator.

MCNSEND
#endif
