/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 2013-10-21 - ADDED - GetTabSettings
 */

#pragma once
#include "Collections.h"
#include "Dialogs.h"
#include "IXMLHelper.h"
#include "ILogger.h"
#include "IFileSearch.h"
#include "Actions.h"
#include "IScript.h"
#include "DataTransfer.h"
#include "IRecentManager.h"
#include "IFavoritesManager.h"
#include "ExtensionInfo.h"
#include "IStatus.h"
#include "IHTTPRequest.h"
#include "IMenuManager.h"
#include "ISessionConfig.h"
#include "IAppConfig.h"
#include "ICommandManager.h"
#include "IRestartRecoveryDataCollector.h"

#include "MCNamespace.h"
MCNSBEGIN

// IMultiAppInterface is the Main interface for extension. From this interface you can get most of the other interfaces.

class IFilePropertiesRetriever;
class IEventCallback;
class IFrameWindow;
class IDataViewerWindow;

// Flags for CreateZipArchive(...)
#define MC_CZA_LOG2APPLOG   0x00000100L // Log creating of zip to AppLog.
#define MC_CZA_LOGERRORONLY 0x00000800L // Log only errors

// flags for FormatBytes(...)
#define FMTBYTES_1000       0x00000100 // If set then 1KB == 1000 bytes. else 1KB == 1024 

#define FMTBYTES_UNIT_NONE  0x00001000
#define FMTBYTES_UNIT_MINI  0x00002000
#define FMTBYTES_UNIT_SHORT 0x00004000
#define FMTBYTES_UNIT_LONG  0x00008000

#define FMTBYTES_SIZE_BYTES 0x00010000
#define FMTBYTES_SIZE_KB    0x00020000
#define FMTBYTES_SIZE_MB    0x00040000
#define FMTBYTES_SIZE_GB    0x00080000
#define FMTBYTES_SIZE_TB    0x00100000
#define FMTBYTES_SIZE_AUTO  0x00200000

#define FMTBYTES_DECIMALS0  0x01000000 // 0
#define FMTBYTES_DECIMALS1  0x02000000 // 0
#define FMTBYTES_DECIMALS2  0x03000000 // 0


class /*__declspec(novtable)*/ IMultiAppInterface 
{
public:

  virtual ~IMultiAppInterface() = default;
  // WARNING ! a persistent interface is limited in what it can do. 
  // It should NOT be used to get/create UI components. only be use it for none GUI API.
  // And ReleasePersistenAppInterface() should be run at program exist. example from DLL_PROCESS_DETECH
  virtual IMultiAppInterface* GetPersistentAppInterface() = 0;
  // Delete the Persistent AppInterface.
  virtual void                ReleasePersistentAppInterface() = 0;

  // Close / Unload module.. 
  virtual BOOL  Close() = 0;  // 
  virtual BOOL  CloseEx() = 0;  // Will not try recove tabfocus and stuff. Only use if the module did NOT create any UI inside a tab.

                              // Close / Unload all module/extension of current type.
  virtual bool  CloseAll() = 0;

  // Get HWND of Main application widow
  virtual HWND GetOwnerHWND() = 0;

  // Get the INTERNAL id the extension that created this AppInterface.
  // This ID can be different from every time MultiCommander starts.
  virtual long Get_ModuleID() = 0;

  // Convert a PLugin/Extension GUID to Local ID - Use this to figure out what the local id of a known extension is.
  // Can be used if you want to be able to send message to and from extensions
  virtual WORD ModuleIDStrToID( const char *strModuleId ) = 0;
  virtual WORD ModuleIDStrToID( const WCHAR *strModuleId ) = 0;

  virtual bool ModuleIDToStr(wchar_t* szModule, int nMaxLen, long lModuleID) = 0;

  // Get the full path for the extension into strPath, ( it is assumed that strPath is at least 512 characters long. )
  virtual bool  GetModulePath( WCHAR* strPath ) = 0;

  // Get the build no of MultiCommander.
  virtual DWORD GetBuildNo() = 0;

  // Return true if a UI component hView is located on the right/top or left/bottom tab
  virtual bool  IsLeft( ZHANDLE hView ) = 0;
  virtual bool  IsRight( ZHANDLE hView ) = 0;

  // Is the tab that has the UI component view active ? (Must be a view that is directly located in the tab. not a child view item.)
  virtual bool  IsTabActive( ZHANDLE hView ) = 0;
  virtual bool  IsSource() = 0;

  // true if any of the requested side are active/in focus
  virtual bool  IsLeftSideActive() = 0;
  virtual bool  IsRightSideActive() = 0;
    
  // TextLanguage
  // Use the one below instead.. will be removed in future
  virtual BOOL    GetText( long ModuleID , long TextID ,/*[out]*/ WCHAR* str , int StrSize ) = 0;
  virtual BOOL    PutText( long ModuleID , long TextID , const WCHAR* str ) = 0;

  // is 0 for moduleid for text from MultiCommander_lang_xx.xml
  virtual const WCHAR* GetText(long ModuleID, long textID) = 0;
  virtual const WCHAR* GetDesc(long ModuleID, long textID) = 0;

  virtual const WCHAR* TranslateText(const WCHAR* text) = 0;
  virtual const WCHAR* TranslateText(const WCHAR* text, long lModuleID) = 0;
  virtual const WCHAR* GetText(long textID) = 0;
  virtual const WCHAR* GetDesc(long textID) = 0;

  // Translated Key Combo text..like "Ctrl+V" and so on
  // wKey VK_Key or any letter
  // modflags can be FSHIFT ,FALT , FCONTROL
  // bPreTabText - Add \t (tab) before the key text.
  virtual WCHAR* GetKeyText(WCHAR* szOutput, DWORD dwLen, WORD wKey, BYTE modflags, bool bPreTabText) = 0;

  virtual bool  StartTimer( DWORD dwInternalCmd , UINT ms ) = 0;
  virtual bool  KillTimer( DWORD dwInternalCmd ) = 0;

  // OBSOLETE
  virtual ZHANDLE CreateAction( UINT dwSourceID , eContentDataType eAType , DWORD dwFlags , DWORD dwCmd = ACTION_CMD_START ) = 0;

  // return TRUE if we are currently on the MAIN UI Thread.
  virtual bool  IsMainThread() = 0;

  // true if MultiCommander is shutting down
  virtual bool  IsShuttingDown() = 0;

  virtual void  PostMessage( DWORD dwMessage , WPARAM dwModuleID , LPARAM wParam ) = 0;
  virtual BOOL  RedirectMessageViaMainThread(DWORD dwMessage, WPARAM wParam , LPARAM lParam ) = 0;
  virtual BOOL  RedirectMessageViaGUIThread(DWORD dwMessage, WPARAM wParam , LPARAM lParam ) = 0;

  virtual bool  RunUserDefinedCommand(const WCHAR* szGuid, DWORD dwReserved = 0) = 0;

  virtual DWORD SendCommandDirect( DWORD nModuleID , DWORD nCmd , long lFlags , WPARAM wParam = 0, LPARAM lParam = 0 ) = 0;

  virtual DWORD SendMessageToSource( DWORD dwMessage, WPARAM wParam = 0, LPARAM lParam = 0,DWORD dwFlags = 0) = 0;
  virtual DWORD SendMessageToTarget( DWORD dwMessage, WPARAM wParam = 0, LPARAM lParam = 0,DWORD dwFlags = 0) = 0;

  // send notification code to all matching modules,
  virtual void  SendNotificationToAll(const DWORD dwModuleId, void* from, const DWORD dwNotifyCode, const WPARAM wParam = 0, const LPARAM lParam = 0, void* pExtra = nullptr) const = 0;

  //////////////////////////////////////////////////////////////////////////
  // Register/Find/Remove Commands. Command can the be assigned to Menus/Hotkey. 
  // Are like dynamic messages that can be send/posted to modules.
  virtual ICommandManager*  GetCommandManager() = 0;
  virtual IMenuManager*     GetMenuManager() = 0;
  virtual ICustomCommands*  GetCustomCommands(const char* strNamespace = NULL) = 0;
  virtual IScriptFunctions* GetScriptFunctions() = 0;
  virtual IFilePropertiesRetriever* CreateFilePropertiesRetriever() = 0;
  virtual IRecentManager*    GetRecentManager() = 0;
  virtual IHistoryManager*   CreateHistoryManager() = 0;
  virtual IFavoritesManager* GetFavoritesManager() = 0;
  virtual IXMLHelper*        CreateXMLHelper() = 0;

  // Config - Read/write Extension Config 
  virtual IAppConfig*     GetAppConfig() = 0;
  // Session Config - Read/Write Session configuration
  virtual ISessionConfig* GetSessionConfig() = 0;

  // Store a temporary string in a temp storage. This is so that Strings can be send using POST method.
  // dwHoldTime is time in ms that the string is valid for. after this time the string CAN be removed from the storage.
  virtual DWORD StoreTemporaryString(const WCHAR* sz, DWORD dwHoldTime) = 0;
  virtual void  RemoveTempraryString(DWORD nID) = 0;
  virtual BOOL  GetTemporaryString(DWORD nID, WCHAR* sz, DWORD len) = 0;

  virtual DWORD StoreTemporaryData(const BYTE* pData, DWORD len, DWORD dwHoldTime) = 0;
  virtual BOOL  GetTemporaryData(DWORD nID, OUT BYTE* pData, DWORD len, bool bRemove) = 0;

  //////////////////////////////////////////////////////////////////////////
  // HotKeys
  // Register Extension specific hot key. This is only valid if the hot keys should be customizable 
  // and then they should be registered in PreStartInit() 
  // Return TRUE (1) if hot key was registered.. FALSE if it failed ( already registered (-1).  or hot key conflict (-2), Unknown error (0) )
  virtual int   RegisterExtensionHotKey(ULONG lCmdID, DWORD hotKey, const WCHAR* displayName, const WCHAR* description) = 0;
  virtual int   ReserveExtensionHotKey(DWORD hotKey) = 0; // Tell system that no other extension key may be changed to THIS key.
  virtual DWORD GetExtensionHotKey(ULONG lCmdID) = 0;
  virtual const WCHAR* GetExtensionHotKeyName(ULONG lCmdID, WCHAR* szText, int nLen) = 0;

  virtual BOOL  UnBindAllKeys() = 0;
  virtual BOOL  UnBindKey( UINT nVKCode , UINT nFlags ) = 0;
  virtual BOOL  BindKey( UINT nVKCode , UINT nFlags , ZHANDLE lCmd ) = 0;
  virtual BOOL  BindAsciiKey( TCHAR nChar , UINT nFlags , ZHANDLE lCmd ) = 0;
  virtual BOOL  BindKey(DWORD hotKey, ZHANDLE lCmd) = 0; // Combined VKKEY and MOD

  // Simulate Key Press. Will trigger command will hotkey is assigned to key
  //virtual BOOL  OnKeyDown( long nVKCode , long nFlags ) = 0;

  // Register an extension as a viewer/editor/launcher
  //
  // Viewer/Editor/Launcher will be available for the user in the file type setup
  // strFilter is the extensions filter to use as default.
  //
  // Options
  // TYPETYPE_DEFAULT   - Require "*.*" filter, Will overwrite existing.. 
  //
  // - User configure filetype can overwrite extension registered.
  virtual BOOL  RegisterAsViewer(const WCHAR* szName, const WCHAR* strFilter, DWORD dwOptions = 0 ) = 0;
  virtual BOOL  RegisterAsEditor(const WCHAR* szName, const WCHAR* strFilter, DWORD dwOptions = 0 ) = 0;
  virtual BOOL  RegisterAsLauncher(const WCHAR* szName, const WCHAR* strFilter, DWORD dwOptions = 0 ) = 0;

  virtual BOOL  RegisterAsPanelApplication(ZHANDLE hCmd) = 0;

  // use nullptr to register it as the default handle. (Only valid for build in preview handler)
  virtual bool  RegisterAsPreviewHandler(const wchar_t* szFileFilter) = 0;

  // Not used yet.
  virtual void RegisterUsage(const DWORD nID) const = 0;

  // Query interface for UI or Other Objects
  // See the ZOBJ_FILESYSTEM and ZOBJ_xxxx constants for interface to query
  virtual void* QueryInterface( long iid ) = 0;
  virtual void* QueryInterface( long iid , ZHANDLE hHandle ) = 0;  
  virtual BOOL  ReleaseInterface( ZHANDLE , long iid ) = 0;

  //////////////////////////////////////////////////////////////////////////
  // Icon
  virtual HICON MainAppLoadIcon( UINT nIDResource ) = 0;

  virtual HICON  LoadIconFromPath(const WCHAR* strFilename, int nIndex, int nSize) = 0;
  // DestroyIcon( HICON ); need to be run to destroy icon or you get resource leak
  virtual HICON LoadIcon(const WCHAR* strFilename , int nIndex , int nSize = 16 ) = 0;
  // If hInstance is 0, then current app is used.
  virtual HICON LoadIconEx(HINSTANCE hInstance, UINT nID, int cx= 16, int cy = 16 ) = 0;

  // If hInstance is 0, then current app is used.
  // is iconSize is MCIconSize::Small then 16x16 size is used (IF MC decided that size should be used for small icons)
  // It can load higher if needed if DPI settings are higher
  virtual HICON LoadIconEx(HINSTANCE hInstance, UINT nID, MCIconSize iconSize) = 0;

  // Load Icon from stored in MCIcons.dll
  virtual HICON LoadIconMC(UINT nID, int cx = 16, int cy = 16) = 0;
  virtual HICON LoadIconMC(UINT nID, MCIconSize iconSize) = 0;
  virtual HICON LoadIconMC(UINT nID, const SIZE& iconSize) = 0;
  // if bSmallIconSize is set to false, Large icon is loaded. and the size of the large icon is 
  // specified by the toolbar icon size settings, set hInstance to -1 to load from MCIcons.dll
  virtual HICON LoadIcon_ToolbarSize(HINSTANCE hInstance, UINT nID, bool bLargeIcon) = 0;

  // if bDPIScale if true the icon size is scaled up according to the DPI settings
  virtual int  GetIconSize(MCIconSize iconSize, bool bDPIScale) = 0;

  //////////////////////////////////////////////////////////////////////////
  // CommandBar
  virtual MCNS::ZHANDLE CreateMainCommandBar(int& nTabID, const WCHAR* strTabName, long TextID = 0, DWORD dwFlags = 0, const WCHAR* szToolbarID = nullptr) = 0;

  
  virtual ZHANDLE CreateDeviceCommandBar(int& nTabID, const WCHAR* strTabName, long TextID = 0, ZHANDLE hCommand = 0,
                                         const WCHAR* szCommandBarID = nullptr, bool ownRow = false, DWORD deviceTypes = 0, const TCHAR* szDeviceFilter = nullptr, DWORD dwExtraData = 0) = 0;

  virtual ZHANDLE DeviceCommandBar_GetLastActiveItem() = 0;
  virtual ZHANDLE DeviceCommandBar_GetCustomData(ZHANDLE hBarItem) = 0;
  
  // Will add ZHANDLE to list of returned handle. Use ReleaseZHandle if you do not store the handle in a member
  virtual ZHANDLE GetCommandBarByName(const WCHAR* szToolbarID) = 0;

  virtual MCIconSize GetMainToolbarIconSize() = 0;
  virtual void    GetMainToolbarIconSize(int& cx, int& cy, MCIconSize iconSize) = 0;

  // not working
  virtual bool    RefreshToolBar(ZHANDLE hToolbar) = 0;

  // strType = "cd *" or "http:\\*"
  // lTarget = 
  // dwParam = Custom parameter 
  virtual BOOL    RegisterURLType(const WCHAR* strType , long lTarget , DWORD dwParam = 0 ) = 0; // eg http:* , www.* ?:*  *.zip 
  // Register a URL type that should be used in explorer view (used so FS Device Extensions can register devices to for explorer)
  virtual BOOL    RegisterFSURLType(const WCHAR* strType, const WCHAR* szPreFix = NULL) = 0; // eg http:* , www.* ?:*  *.zip 

  // register File extension handling.
  // dwParam can only be bit flags.. ZCP_TAB_LEFT/ZCP_TAB_RIGHT is set to is.. other flags are custom
  virtual BOOL    RegisterFileExtHandler( const WCHAR* strFileExt , long lTarget , DWORD dwParam = 0 ) = 0;

  // Set/Append text to the commandline field
  virtual BOOL    SetCommandLine(const WCHAR* szCommand, bool bAppend = false, bool bTakeInputFocus = false) = 0;

  // Create UI View (Will be located inside a Tab. )
  virtual ZHANDLE Create( long CreateType , long CreateParam , long TextID , long IconIdx , DWORD_PTR param1=0 , DWORD_PTR param2=0, ZHANDLE hParent=0 , DWORD dwInitValues = 0) = 0;
  // Create UI View and return matching Interface 
  virtual void*   CreateQI(long CreateType , long CreateParam , long TextID , long IconIdx , DWORD_PTR param1=0 , DWORD_PTR param2=0, ZHANDLE hParent=0 ) = 0;
  virtual BOOL    Destory(long lCreateType , ZHANDLE hView ) = 0;

  virtual ZHANDLE CreateRaw(UINT nType, DWORD dwCreateFlags, DWORD_PTR dwParam1, DWORD_PTR dwParam2, HWND hwndParent = 0, DWORD dwStyle = WS_CHILD, UINT nID = 0) = 0;

  virtual ZHANDLE CreateControl( long CtrlType , long CreateParam , long cmdId , RECT rt , ZHANDLE parent=0 ) = 0;
  virtual BOOL    DestoryControl( long lCtrlType , ZHANDLE hCtrl ) = 0;

  virtual void ReleaseZHandle(ZHANDLE h) = 0;

  virtual IFrameWindow* CreateFrameWindow(long CreateParam, DWORD dwReserved1, DWORD dwReserved2) = 0;

  // OBSOLETE - Do not use
  // only new GUI elements that are placed in a parent that has the GUI as an owner. (eg FrameWindow can do this. ) 
  // or create own dialog without the help of the MCInterface
  // A new message is send to the extension AM_THREADINIT. create you GUI when you receives this.
  virtual DWORD CreateGUIThread(DWORD dwParam) = 0;
  virtual void  DeleteGUIThread() = 0;

  // Will create the MC Secondary UI thread if not alredy created. 
  // Then it will call back to the extension with AM_SECONDARY_UI  with <PARAM> as parameter
  virtual DWORD   RedirectToSecondaryUIThread(DWORD dwParam) = 0;
  virtual bool    SecondaryUIThread_ConnectHWND(HWND hWnd) = 0;    // Not used yet. Will be used for callback to module if UIThread is closing and there is still connected HWND. It will notify the extension
  virtual bool    SecondaryUIThread_DisconnectHWND(HWND hWnd) = 0;


  virtual bool    GetTabLabel( ZHANDLE h , WCHAR* strLabel , UINT nMaxLenght ) = 0;
  virtual void    SetTabLabel( ZHANDLE h , const WCHAR* strLabel, const WCHAR* strTooltip = NULL, DWORD dwFlags = 0 ) = 0;
  virtual void    SetTabIcon( ZHANDLE h , HICON hIcon ) = 0;
  virtual void    SetTabIconAndLabel(ZHANDLE h, HICON hIcon, const WCHAR* strLabel, const WCHAR* strToolTip = NULL, DWORD dwFlags = 0 ) = 0;
  virtual void    SetTabTooltip( ZHANDLE h , const WCHAR* strTooltip) = 0;

  // set color to -1 to use default  
  // bUseHotOnActive is true then hot (hover) color will be used if tab is active
  virtual void    SetTabColor(ZHANDLE h, COLORREF crBackgroundColor, COLORREF crTextColor, bool bGradient, bool bUseHotOnActive) = 0;
  virtual void    SetTabColorEx(ZHANDLE h, COLORREF crBackgroundColor, COLORREF crTextColor, COLORREF crBackgroundColorInactive, COLORREF crTextColorInactive, bool bGradient, bool bUseHotOnActive) = 0;

  // Get Tab Settings for extension. (If there are any settings for this extension, if not then this function will return false)
  // dwFlags are TABSETTING_xxx flags
  virtual bool GetTabSettings(COLORREF& crBackground, COLORREF& crText, DWORD& dwFlags) = 0;
  virtual bool GetTabSettingsEx(COLORREF& crBackground, COLORREF& crText, COLORREF& crBackgroundInactive, COLORREF& crTextInactive, DWORD& dwFlags) = 0;

  virtual void    SetButtonMenu( ZHANDLE hButton , ZHANDLE hMenuMan , ZHANDLE hParent ) = 0;

  //////////////////////////////////////////////////////////////////////////
  // SysInfo
  // MOVE TO Own interface - ISysInfo
  virtual bool IsElevated() = 0;  // true if running elevated on Vista/Win7/2008

  virtual bool IsMinWin60()  = 0;  // true if Minimum Windows API Version is 6.0 (Windows Vista+)
  virtual bool IsMinWin61()  = 0;  // true if Minimum Windows API Version is 6.1 (Win7+)
  virtual bool IsMinWin62()  = 0;  // true if Minimum Windows API Version is 6.2 (Win8+)
  virtual bool IsMinWin63()  = 0;  // true if Minimum Windows API Version is 6.3 (Win8.1+) // Not sure 

  // nPathID - 
  // 0 = Path that MultiCommander has started from
  // 1 = User data path
  // 2 = Config path 
  // 11 = Temp folder
  virtual bool  GetPath(DWORD nPathID, WCHAR* szPath, UINT nMaxLength) = 0;
  virtual bool  GetExtensionInfo(long nExtensionID, DLLExtensionInfo* pInfo) = 0;
  // /*  c:\Documents and Settings\<user>\Application Data\MultiCommander\UserData\<Extension>\   */
  virtual bool  GetUserExtensionDataFolder( WCHAR* strPath , UINT nMaxLength ) = 0;

  // Copy Data to other module
  virtual long     CopyData( long lDataFormat , const WCHAR* strDataType , long lDataFlags , long lDataItemCount , DWORD_PTR hData ) = 0;


  virtual bool     ShowPath( const WCHAR* strPath, const WCHAR* strFocusItem, bool bLeftTab, bool bNew ) = 0;

  // Adds child items of pFileItem  to the pCol Collection
  virtual long     FM_AddSubItems( IFileItem* pFileItem , IHCollection **pCol ) = 0; // OBSOLETE.. do not use

  // OBSOLETE !! Do not use. Will be removed.  Use GetTargetPath(..) below instead
  virtual bool  GetDestinationPath( WCHAR* strPath , DWORD nMaxLen ) = 0;

  // Get Current Source/Target path. Source is the extension in focus and target the one opposite that.
  // Since path in MC can be longer then 256 is it recommended that you us _MC_MAXPATH_ as length if you want to support longer paths
  virtual bool  GetTargetPath(WCHAR* szPath, DWORD nLen) = 0;
  virtual bool  GetSourcePath(WCHAR* szPath, DWORD nLen) = 0;
  virtual bool  GetTargetFocusItem(WCHAR* szPath, DWORD nLen, bool bFullPath = true) = 0;
  virtual bool  GetSourceFocusItem(WCHAR* szPath, DWORD nLen, bool bFullPath = true) = 0;
  
  virtual long  TargetModuleID() = 0;
  virtual long  SourceModuleID() = 0;

  virtual IFileItemCollection* CreateFileItemCollection() = 0;
  virtual IFileItemCollection* CreateFileItemCollectionEx() = 0; // Will call AddRef/DelRef automatically. (Use if IFileItem are used in worker threds) 
  virtual IHCollection*        CreateHCollection() = 0;
  virtual IKeyValCollection*   CreateKeyValCollection() = 0;
  virtual IValueCollection*    CreateValueCollection() = 0;

  // Must be run to delete IFileItem objects
  // Objects return from Eg. FM_GetCheckedItems()
  virtual BOOL  DeleteFileItems( IFileItemCollection* pFileItems ) = 0;
  virtual bool  DeleteFileItemsGC( IFileItemCollection* pFileItems ) = 0;

  // Create Custom logger.
  virtual ILogger*  CreateLogger() = 0;

  // nLogID = 0 Application Log, 1 File Operation Logs, For custom log views use CreateLogger() and ILogger* above
  virtual void LogFmt( UINT nLogID , LogLevel nLevel ,  const char*  strText , ... ) = 0;
  virtual void LogFmt( UINT nLogID , LogLevel nLevel ,  const WCHAR* strText , ... ) = 0;
  virtual void Log( UINT nLogID , LogLevel nLevel ,  const char*  strText ) = 0;
  virtual void Log( UINT nLogID , LogLevel nLevel ,  const WCHAR* strText ) = 0;

  // Generic Dialogs
  virtual IProgressDlg*  CreateProgressDialog() = 0; // Recommended to use IProgress2Dlg instead.
  virtual IProgress2Dlg* CreateProgress2Dialog() = 0;
  
  // Similar to progress dialog. But there no known end of the progress. WorkingDialog has a Progress bar that loops around.
  // and there is only an Abort button (Optional). 
  virtual IWorkingDlg*   CreateWorkingDialog() = 0; 
  virtual IMessageBox*   CreateMessageBox() = 0;

  // bWaitForAnyKey  =  0 - F-Keys
  //            1 - Digit
  //            2 - AnyKey
  virtual UINT  ShowMessage( HWND hParent, const WCHAR* strMessage , DWORD nTimeout , DWORD nWaitForAnyKey ) = 0;
  virtual UINT  ShowMessageBox( const WCHAR* strTitle, const WCHAR* strMessage, DWORD nFlags ) = 0;
  virtual BOOL  ShowTextComboDlg( const WCHAR* strCaption , const WCHAR* strTitle , const WCHAR* strTag , short nMaxItems , WCHAR* strResult , int nResStrLen ) = 0;
  virtual bool  ShowGenericComboDlg( GENERICCOMBOBOX* pComboBox ) = 0;
  virtual bool  ShowMultiComboDlg( MULTICOMBOBOXDLG* pMultiComboDlg) = 0;
  virtual bool  ShowAskTextDlg(const WCHAR* caption, const WCHAR* title, const WCHAR* text, const WCHAR* cuebanner, WCHAR* szResult, int nResLen, HWND hwdParent = 0, bool bPasswordMode = false) = 0;

  // Options .. SED_
  virtual UINT      ShowErrorDialog(DWORD nErrorCode, const WCHAR* szItemName, DWORD dwOptions) = 0;

  // Show "nValues" no of item. and fill the item from pValues. bSet true/false if SetValue should be set if set is pressed
  virtual UINT      ShowSelectWindow( SelectWindowData* pSWData ) = 0;

  // Show Favorites Windows/Popup
  virtual UINT      ShowFavoritesWindow(int nSection = -1) = 0;
  virtual UINT      ShowFavoritesWindowEx(POINT pos, int nSection = -1, DWORD dwFlags = 0) = 0;
  virtual UINT      ShowFavoritesPopup(POINT pos) = 0;

  // Show settings panel for a specific Module, set szGuid to nullptr for current module
  virtual UINT      ShowSettingPanel(const char* szGuid) = 0;
  
  // Show dialog where user can pick a text and background color. 
  // return true if a color was picked. false if dailog was canceled.
  // Color is set in pColorPickInfo dialog
  virtual bool      ShowColorPickDlg(MCNS::COLORPICKINFO* pColorPickInfo) = 0;

  // Show Pick Filter dialog. User can specify a simple filter. or pick a defined global filter
  // a FilterHandle is returned
  virtual bool      ShowPickFilterDlg(MCNS::PICKFILTERINFO* pPickFilterInfo) = 0;

  // Show DefineFilterDlg, 
  // Returns the ZHANDLE to the defined filter
  virtual bool      ShowDefineFilterDlg(OUT WCHAR* szFilterID, int nFilterIDLen, MCNS::ZHANDLE hCurrentFilter, HWND hParent, bool bAllowPickAnyFilter) = 0;
  virtual bool      GetFilterNameFromFilterID(OUT WCHAR* szFilterName, int nFilterNameLen, const WCHAR* szFilterID) = 0;
  
  // If create a DataViewer of a named instance that that instance already exists
  // a interface to that instance is returned.
  // Depending on viewer. The viewer might be located in its own thread so the Viewer might not be ready
  // when IDataViewerWindow is returned. Do IsCreated() to query if it is ready to be used. else you need to wait.
  // if bAutoDelete is true then the IDataViewerWindow will auto release when disconnected from the viewer.
  // Else you need to call Release(); and keep an track of the IDataViewerWindow pointer.
  virtual IDataViewerWindow* CreateDataViewerWindow(const WCHAR* szNamedInstance = NULL, bool bAutoDelete = true) = 0;

  // Start a Search.
  // TODO - Change so that start/Stop/Continue is done from the IFileSarchHandler interface
  virtual IFileSearchHandler* CreateFileSearchHandler() = 0;
  virtual DWORD StartFileSearchOperation( IFileSearchHandler* p ) = 0;
  virtual bool  StopFileSearchOperation( IFileSearchHandler* p ) = 0;
  virtual bool  PauseFileSearchOperation( IFileSearchHandler* p ) = 0;
  virtual bool  ContinueFileSearchOperation( IFileSearchHandler* p ) = 0;
  virtual void  ReleaseFileSearchHandler( IFileSearchHandler* p )= 0;

  // Replaces IFileSearchHandler above
  virtual IFindFilesHandler* CreateFindFilesHandler() = 0;

  // call ->Release() on it to delete it
  virtual IHTTPRequest* CreateHTTPRequester() = 0;

  // Move to another interface eg INetworkMisc 
  // Change to use url  <protocol><host>[:port]</path> 

  // return Http code.. 200 is OKEY All values of 100 and above is the http response code, 0-99 are internal error codes
  virtual DWORD HTTPSendFile_POST(const WCHAR* szUserAgent, const WCHAR* szHost, const WCHAR* szRemotePath, const WCHAR* szFilename, IKeyValCollection* pExtraHeader) = 0;

  virtual bool SendToClipboard(const char* szText) = 0;
  virtual bool SendToClipboard(const WCHAR* szText) = 0;

  // TODO : move to another interface
  // dwFlags == MC_CZA_xxxxx 
  virtual bool CreateZipArchive(const WCHAR* szFilename, IKeyValCollection* pFilesToZip, DWORD dwFlags) = 0;
  virtual bool UnzipArchive(const WCHAR* szZipArchive, const WCHAR* szTargetPath, bool bOverwrite ) = 0;

  // DPIHelper
  virtual int DPIHelperScaleY(int y) = 0;
  virtual int DPIHelperScaleX(int x) = 0;
  virtual int DPIHelperUnscaleY(int y) = 0;
  virtual int DPIHelperUnscaleX(int x) = 0;

  virtual int DPIHelperDPIX() = 0;
  virtual int DPIHelperDPIY() = 0;

  // Fonts.
  virtual int DPIHelperPoints2Pixels(int pt) = 0;
  virtual int DPIHelperPixels2Points(int px) = 0;

  // input requested draw size and out the scaled sized.
  virtual MCIconSize  DPIScaleIconSize(MCIconSize iconSize) = 0;

  virtual bool DarkModeActive() = 0;
  // with darkmode translation (ONLY some colorid are translated)
  virtual COLORREF GetSysColor(int nColorId) = 0;

  //////////////////////////////////////////////////////////////////////////
  // IMiscUtils
  // 
  // Will format bytes 
  // Example
  // m_pAppInterface->FormatBytes(szBytes, _countof(szBytes), nBytes, FMTBYTES_SIZE_BYTES|FMTBYTES_UNIT_LONG|FMTBYTES_DECIMALS0);
  virtual bool FormatBytes(WCHAR* szOut, DWORD nLen, INT64 nSize, DWORD dwFlags) = 0;

  virtual bool ReadToString(WCHAR* szBuffer, DWORD nMaxLen, const WCHAR* szFilename) = 0;
  virtual bool ReadToString(char* szBuffer, DWORD nMaxLen, const WCHAR* szFilename) = 0;

  virtual void KeepSystemAlive(bool alive) = 0;

  virtual void AddRestoreTabInfo(const wchar_t* szDisplayName, IKeyValCollection* pTabParameters) = 0;

  // if szfilename is nullptr, then version info for MultiCommander is returned
  virtual void GetVersionInfo(VersionInfo* versionInfo, const wchar_t* szFilename) = 0;
};

MCNSEND