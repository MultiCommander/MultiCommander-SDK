#include "StdAfx.h"

#include "MCAppExtensionSample.h"

#include "AppExtensionCmd.h"

using namespace MCNS;

#define APPSAMPLE_MULTISCRIPT_FUNC1   1
#define APPSAMPLE_MULTISCRIPT_FUNC2   2
#define APPSAMPLE_MULTISCRIPT_FUNC3   3
#define APPSAMPLE_MULTISCRIPT_FUNC4   4
#define APPSAMPLE_MULTISCRIPT_FUNC5   5
#define APPSAMPLE_MULTISCRIPT_FUNC6   6

// This GUID MUST be unique, in VS2005+ go to Tools->CreateGUID and select "Registry formt" and remove { } and - 
#pragma message(" ** If you create your own Plugin then change this GUID \n ** In visual studio go to Menu > Tools > Create GUID. And remove all ")
char MCAppExtensionSample::m_GuidString[34] = "abb1e6997f4040faa0f46ed47818a74c";   // THIS MUST BE CHANGED TO A UNIQUE ID WHEN CREATING YOU OWN

MCAppExtensionSample::MCAppExtensionSample(void)
 : m_pAppInterface(NULL)
{
}

MCAppExtensionSample::~MCAppExtensionSample(void)
{
}


bool MCAppExtensionSample::GetExtensionInfo( DLLExtensionInfo* pInfo )
{
  if( pInfo == NULL )
    return false;

  wcsncpy( pInfo->wsName , L"AppExtension Sample" , 100 );           // Name of the Extension      (Shown in Extension Manager)
  wcsncpy( pInfo->wsPublisher , L"<Name of publisher>" , 100 );      // Name of the Publisher      (Shown in Extension Manager)
  wcsncpy( pInfo->wsURL , L"http:\\\\<webpage>" , 100 );             // URL to Publishers WebPage  (Shown in Extension Manager)
  wcsncpy( pInfo->wsDesc , L"This is a sample of extension" , 160 ); // Description of what this extension are doing  (Shown in Extension Manager)
  wcsncpy( pInfo->wsBaseName , L"AppExtSample" , 100 );              // The basename for language and config files. 
                                                                     // eg if it is "MyExt" then it will look for MyExt_lang_en.xml and MyExt.xml
  
  strncpy( pInfo->strVersion , "1.0.0.0" , 10 );                              // Version no of Your Extension
  strncpy( pInfo->strGuid , m_GuidString , 34 );                              // The Unique quid that this extension has.

  // dwFlags contains what type of extension this is.
  // EXT_TYPE_APP - for an "application" extension
  // EXT_TYPE_FS  - for an Filesystem extension ( eg. zip browser )
  // EXT_TYPE_CP  - for an FileOperations extensions
#ifdef _UNICODE
  pInfo->dwFlags = EXT_TYPE_APP | EXT_OS_UNICODE;
#else
  pInfo->dwFlags = EXT_TYPE_APP | EXT_OS_ANSI;
#endif

  pInfo->dwInitOrder = 2000; // All 3de party extension must start use 2000 or more

  pInfo->dwInterfaceVersion = MULTI_INTERFACE_VERSION;  // What version of MultiCommander Extension Interface this extensions is built for
  return true;
}

//////////////////////////////////////////////////////////////////////////
// [ START OF MULTICOMMANDER EXTENSIONS API ]
//////////////////////////////////////////////////////////////////////////


// PreStartInit is call when MultiCommander is starting during the initialization phase.
// In there you can register command and setup menu options and command that MultiCommander
// should show. But do not do any heavy work in here. after PreStartInit is run, the Extension instance will be deleted.
long MCAppExtensionSample::PreStartInit( IMultiAppInterface* pAppInterface )
{

  pAppInterface->LogFmt(0, LogLevel::_DEBUG_, L"Loading... %s", L"Parameter");

  // Lets register some commands and add them to the menu
  IMenuManager* pMenuManager = pAppInterface->GetMenuManager();
  
  // We want to create a submenu under the tools menu

  // Get Handle to the Tools menu that always exists.  Find the menu by name, but since the name can be different depending on language. 
  // Use the MultiLanguge ID for the name  
  ZHANDLE hMenuTools = pMenuManager->FindMenu( MAKETEXTID( 'm' , 2004 ), TRUE, FALSE);

  if(hMenuTools == 0)
    return 0; // Failed to find Tools menu.. that strange. something is wrong

  ZHANDLE hMenuSample = pMenuManager->FindMenu( hMenuTools , MAKETEXTID( 'm' , 1 ), TRUE);
  if(hMenuSample == 0)
    return 0;

  // Register a command
  ICommandManager* pCommandManager = pAppInterface->GetCommandManager();

  ZHANDLE hCmd = 0;
  // Register a command with the value of APPEXT_CMD_COMMAND1 (If the command is already registered a handle to that command is returned)
  // The command should always be enable by default, and the command can be sent to ANY instance of a matching extension (So of two instances of MCAppExtensionSample is created the command can be sent to any of them)
  // For the Name and description of the command use texts from the language file
  // Do not assign any default hotkey, By allow the command do be customizable be the user (If you set a default hotkey. make sure it does not collide with default keys )
  // When user trigger a command the command is sent to OnMessage(..)
  hCmd = pCommandManager->RegisterCommand( APPEXT_CMD_COMMAND1 , ZCF_ENABLE | ZCF_TARGET_ANY ,  MAKETEXTID( 'c' , 100 ) , MAKETEXTID( 'c' , 100 ), MAKEACCELKEY(0, FCUSTOMIZABLE) );
  // Add this command to the menu
  pMenuManager->AddMenuItem(hMenuSample, hCmd);

  hCmd = pCommandManager->RegisterCommand( APPEXT_CMD_COMMAND2 , ZCF_ENABLE | ZCF_TARGET_ANY ,  MAKETEXTID( 'c' , 101 ) , MAKETEXTID( 'c' , 101 ), MAKEACCELKEY(0, FCUSTOMIZABLE) );
  pMenuManager->AddMenuItem(hMenuSample, hCmd);

  hCmd = pCommandManager->RegisterCommand( APPEXT_CMD_COMMAND3 , ZCF_ENABLE | ZCF_TARGET_ANY ,  MAKETEXTID( 'c' , 102 ) , MAKETEXTID( 'c' , 102 ), MAKEACCELKEY(0, FCUSTOMIZABLE) );
  pMenuManager->AddMenuItem(hMenuSample, hCmd);


  // Register so the COMMAND1 and 2 can be called from script using CustomCommands.
  ICustomCommands* pCustomCommands = pAppInterface->GetCustomCommands("MC.AppSample");
  if( pCustomCommands && pCustomCommands->GetInitialized() == false )
  {
    ICustomCommandDef* pDef = NULL;

    // MC.AppSample.Command1 OPTION1 OPTION2="Option Value"
    pDef = pCustomCommands->DefineCommand( "Command1", APPEXT_CMD_COMMAND1, CCO_QUITE, ZCF_TARGET_ANY );
    if( pDef )
    {
      pDef->SetDescription( pAppInterface->GetText(MAKETEXTID('c',100)));
      pDef->SetOption( "OPTION1" , CCOF_BOOL , pAppInterface->GetText(MAKETEXTID('c',112)) );
      pDef->SetOption( "OPTION2" , CCOF_TEXT , pAppInterface->GetText(MAKETEXTID('c',113)) );
    }

    // When custom command "MC.AppSample.Command1" is called. OnCustomCommand(...) is called in this extension
    
    pCustomCommands->SetInitialized(true);
  }

  // Register a MultiScript functions
  IScriptFunctions* pScriptFunctions = pAppInterface->GetScriptFunctions();
  if(pScriptFunctions)
  {
    /*
    MultiScript
    =======================
    import("AppExtSample"); // <<- Same name as you defined in BaseName in DLLExtensionInfo
    @var $v = SampleTest1();
    @var $b = SampleTest2($v);
    @var $arr = SampleText3();
    @var $c = SampleText4($arr);
    */
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC1, L"SampleTest1", IScriptFunctions::Num);
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC2, L"SampleTest2", IScriptFunctions::Num, IScriptFunctions::Num);
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC3, L"SampleTest3", IScriptFunctions::Array);
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC4, L"SampleTest4", IScriptFunctions::Num, IScriptFunctions::Array);
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC5, L"SampleTest5", IScriptFunctions::Handle);
    pScriptFunctions->RegisterScriptFunction(APPSAMPLE_MULTISCRIPT_FUNC6, L"SampleTest6", IScriptFunctions::Num, IScriptFunctions::Handle);
  }

  return 0;
}

// This function is the first to be run ( even before Init ) this should return flags on what type of 
// special conditions this extensions support.  ( THIS might be removed. )
long MCAppExtensionSample::GetModuleFlags()
{
  return ZMIF_SENDTO;
}

// This is called an Instance is created after MC has started.
// pAppInterface is a pointer to the AppInterface.
// Keep a hold of this pointer until this instance is deleted..
// it is through this pointer functions are accessed in MultiCommander
BOOL MCAppExtensionSample::Init( IMultiAppInterface* pAppInterface )
{
  m_pAppInterface = pAppInterface;
  return TRUE;
}

// If this extension creates any GUI, this function is called so you can interact on Notification from the GUI Controls.
bool MCAppExtensionSample::OnNotify( ZHANDLE /*hFrom*/ , DWORD /*nNotifyMsg*/ , WPARAM /*wParam*/ /*= 0 */, LPARAM /*lParam*/ /*= 0 */, void* /*pExtra*/ /*= NULL */ )
{
  return false;
}

// MultiCommander will send messages to Extension to informs them of different events. Also
// it is here you will receive message from own registered messages when they are activated.
BOOL MCAppExtensionSample::OnMessage( long msg , ZHANDLE /*hView*/ , WPARAM param1 , LPARAM param2 )
{
  switch( msg )
  {
      // AM_CLOSE is sent if user specifically choose to close and extensions.
    case AM_CLOSE:
    {
      CloseContext context = {false,false, false};
      return OnClose(context);
    }
      
      // Handle CommandID on Some Internal command and own command (menus command)
      // if they should be visible, grayed , checked and so on
      case AM_COMMANDUI : return OnCommandUI( (DWORD)param1 , (DWORD*)param2 ); break;


      // Extension Specific commands
      case APPEXT_CMD_COMMAND1 : OnCommand1(); return TRUE; break;
      case APPEXT_CMD_COMMAND2 : OnCommand2(); return TRUE; break;
      case APPEXT_CMD_COMMAND3 : OnCommand3(); return TRUE; break;
  }

  return FALSE;
}

BOOL MCAppExtensionSample::OnCustomCommand( ICustomCommand* pCustomCommand )
{
  if( pCustomCommand->GetID() == APPEXT_CMD_COMMAND1)
  {
    const WCHAR* szOption2Value = NULL;
    bool bHasOption1 = pCustomCommand->HasOption("OPTION1");
    if(pCustomCommand->HasOption("OPTION2"))
    {
      szOption2Value = pCustomCommand->GetOptionValue("OPTION2");
    }

    bHasOption1 = !bHasOption1; // Just so we do not get a waring about it not being used.

    OnCommand1();

    return TRUE;
  }

  return FALSE;
}

BOOL MCAppExtensionSample::OnScriptFunction(IScriptFunctionContext* pScriptFuncContext)
{
  if(!pScriptFuncContext)
    return FALSE;

  DWORD nFunctionID = pScriptFuncContext->FunctionID();
  if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC1) // Return Num
  {
    pScriptFuncContext->ReturnValue(5);
    return TRUE;
  }
  else if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC2) // Return Num, Take 1 parameter of type Num
  {
    IScriptFunctionParameter* pParam = pScriptFuncContext->GetParameter(0);
    if(pParam)
    {
      INT64 nValue = 0;
      if(!pParam->GetNumValue(&nValue))
        return FALSE;

      nValue = nValue * 4;
      pScriptFuncContext->ReturnValue(nValue);
      return TRUE;
    }
  }
  else if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC3) // Return Array of num
  {
    IScriptArray* pArray = pScriptFuncContext->CreateArrayReturnValue(IScriptFunctions::Num);
    pArray->AddValue(1);
    pArray->AddValue(2);
    pArray->AddValue(4);
    pArray->AddValue(8);
    return TRUE;

  }
  else if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC4)// Return num , take parameter of array
  {
    IScriptFunctionParameter* pParam = pScriptFuncContext->GetParameter(0);
    if(pParam)
    {
      INT64 nValueSum = 0;
      const IScriptArray* pArray = pParam->GetArrayValues();
      if(pArray)
      {
        DWORD nCount = pArray->GetCount();
        for(DWORD n = 0; n < nCount; ++n)
        {
          const IScriptFunctionParameter* pArrayValue = pArray->GetValue(n);
          if(pArrayValue)
          {
            if(pArrayValue->GetDataType() != IScriptFunctions::Num)
              continue;

            INT64 nValue = 0;
            if(pArrayValue->GetNumValue(&nValue))
              nValueSum += nValue;
          }
        }

        pScriptFuncContext->ReturnValue(nValueSum);
        return TRUE;
      }
    }
    return FALSE;
  }
  else if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC5) // return Handle.. 
  {
    // Create a class that inherit from IScriptHandle and store your pointer in that 
    // And delete it when MyScriptHandle is deleted.
    /*
    MyObject* pObject = new MyObject;
    MyScriptHandle* pHandle = new MyScriptHandle(pObject);
    pScriptFuncContext->ReturnHandleValue(pHandle);
    return TRUE;
    */
    return FALSE;
  }
  else if(nFunctionID == APPSAMPLE_MULTISCRIPT_FUNC6) // return Num, Parameter taking Handle
  {
    IScriptFunctionParameter* pParam = pScriptFuncContext->GetParameter(0);
    if(pParam)
    {
      IScriptHandle* p = NULL;
      if(pParam->GetHandleValue(&p))
      {
        /*
        MyScriptHandle* pScriptHandle = (MyScriptHandle*)p;
        MyObject* pMyObject = (MyObject*)pScriptHandle->Handle();

        delete pMyObject; // since we KNOW this will not be used anymore.
        pScriptHandle->Handle(NULL);
        */
        pScriptFuncContext->ReturnValue(1);
        return TRUE;
      }
    }
  }
  return FALSE;
}

// This Extension is about to be close (deleted). 
// return TRUE if cleanup was successfully or FALSE if modules can not close down. 
BOOL MCAppExtensionSample::OnClose(CloseContext& context)
{
  UNREFERENCED_PARAMETER(context);
  return FALSE;
}

int MCAppExtensionSample::DoImportData( IXData* /*pXData*/ )
{
  return 0;
}

IImportData* MCAppExtensionSample::Get_ImportObject()
{
  return NULL;
}

IExportData* MCAppExtensionSample::Get_ExportObject()
{
  return NULL;
}

//////////////////////////////////////////////////////////////////////////
// [ END OF MULTICOMMANDER EXTENSIONS API ]
//////////////////////////////////////////////////////////////////////////
BOOL MCAppExtensionSample::OnCommandUI( DWORD /*nCommandID*/ , DWORD* /*pFlags*/ )
{
/*
//  switch( nCommandID )
//  {
    // Most of this commands are only valid if you have created any GUI in MultiCommander
    
    // If Clipboard Cut is Supported
    case IM_CLIP_CUT   : *pFlags = ZCF_ENABLE; return TRUE; break;
    
    // If Clipboard copy is Supported
    case IM_CLIP_COPY  : *pFlags = ZCF_ENABLE; return TRUE; break;
    
    // If Clipboard paste is Supported
    case IM_CLIP_PASTE : *pFlags = ZCF_ENABLE; return TRUE; break;
    
    // If Back Button is Supported
    case IM_GO_BACK    : *pFlags = ZCF_ENABLE; return TRUE; break;
    
    // If Forward Button is Supported
    case IM_GO_FORWARD : *pFlags = ZCF_ENABLE; return TRUE; break;
    
    // If History Button is Supported
    case IM_GO_HISTORY : *pFlags = ZCF_ENABLE; return TRUE; break;

    // If Refresh Button is Supported
    case IM_REFRESH    : *pFlags = ZCF_ENABLE; return TRUE; break;

    // You will also be able to change the state of own register commands that
    // are assigned to menys / buttons
//  }
  */

  // return only TRUE if you have change any value in pFlags
  return FALSE;
}

void MCAppExtensionSample::OnCommand1()
{

  // Get a collection of all the selected item from the source view..
   IFileItemCollection* pFileItems = m_pAppInterface->CreateFileItemCollection();
   m_pAppInterface->SendMessageToSource( AM_GETSELECTEDITEMS, (WPARAM)pFileItems, MF_INCFOCUS);

   std::wstring files;

   WCHAR szPath[_MC_MAXPATH_];

   DWORD nCount = pFileItems->Count();
   for(DWORD n = 0; n < nCount; ++n)
   { 
     IFileItem* pItem = pFileItems->GetAt(n);
     if(pItem)
     {
       pItem->Get_FullPath(szPath, _countof(szPath));
       if(files.empty() == false)
         files += _T("\r\n");

       files += szPath;
     }
   }

   pFileItems->Release();

   MessageBox(m_pAppInterface->GetOwnerHWND(), files.c_str(), _T("Selected Files"), MB_OK);
}

void MCAppExtensionSample::OnCommand2()
{
  // Load From Config
  IAppConfig* pConfig = m_pAppInterface->GetAppConfig();

  ZHANDLE hConfigRoot = pConfig->GetConfigElement( NULL ,  L"config" );
  WCHAR szValue1[100];
  szValue1[0] = '\0';

  WCHAR szValue2[100];
  szValue2[0] = '\0';

  pConfig->GetConfigValue(hConfigRoot, L"setting1", L"value", szValue1, _countof(szValue1));

  ZHANDLE hLayout = pConfig->GetConfigElement(hConfigRoot, L"layout", false);
  if(hLayout)
  {
    pConfig->GetConfigValue(hLayout, L"settingitem2", L"value", szValue2, _countof(szValue2));
  }

  std::wstring str;
  str += L"setting1 : ";
  str += szValue1;
  str += L"\r\n";
  str += L"settingitem2 : ";
  str += szValue2;
  str += L"\r\n";
  MessageBox(m_pAppInterface->GetOwnerHWND(), str.c_str(), _T("Config Values"), MB_OK);
}

void MCAppExtensionSample::OnCommand3()
{
  // Get a collection of all the selected item from the source view..
  IFileItemCollection* pFileItems = m_pAppInterface->CreateFileItemCollection();
  IFileItemCollection* pFileItemsSelected = m_pAppInterface->CreateFileItemCollection();
  m_pAppInterface->SendMessageToSource( AM_GETFILEITEMS, (WPARAM)pFileItems, 0);

  
  DWORD nCount = pFileItems->Count();
  for(DWORD n = 0; n < nCount; ++n)
  { 
    IFileItem* pItem = pFileItems->GetAt(n);
    if(pItem && n % 2)
    {
      pFileItemsSelected->Add(pItem);
    }
  }

  m_pAppInterface->SendMessageToSource( AM_SELECTFILEITEMS, (WPARAM)pFileItemsSelected, 1);
  pFileItemsSelected->Release();
  pFileItems->Release();

}
