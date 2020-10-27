#include "stdafx.h"
#include "MCFilePropSample.h"

using namespace MCNS;

#define MCFILEPROP_SAMPLE1 10
#define MCFILEPROP_SAMPLE2 11

// This GUID MUST be unique, in VS2005+ go to Tools->CreateGUID and select "Registry formt" and remove { } and - 
#pragma message(" ** If you create your own Plugin then change this GUID \n ** In visual studio go to Menu > Tools > Create GUID. And remove all ")
char MCFilePropSample::m_GuidString[34] = "AB5FED6DA9AA4D91B16FAB52BB3465A4";   // THIS MUST BE CHANGED TO A UNIQUE ID WHEN CREATING YOU OWN

WORD MCFilePropSample::m_ExtensionID = 0;

MCFilePropSample::MCFilePropSample()
{

}

MCFilePropSample::~MCFilePropSample()
{

}

bool MCFilePropSample::GetExtensionInfo( DLLExtensionInfo* pInfo )
{
  if( pInfo == NULL )
    return false;

  wcsncpy( pInfo->wsName , L"FileProperties Sample" , 100 );           // Name of the Extension      (Shown in Extension Manager)
  wcsncpy( pInfo->wsPublisher , L"<Name of publisher>" , 100 );      // Name of the Publisher      (Shown in Extension Manager)
  wcsncpy( pInfo->wsURL , L"http:\\\\<webpage>" , 100 );             // URL to Publishers WebPage  (Shown in Extension Manager)
  wcsncpy( pInfo->wsDesc , L"This is a sample of file properties plugin" , 160 ); // Description of what this extension are doing  (Shown in Extension Manager)
  wcsncpy( pInfo->wsBaseName , L"FilePropSample" , 100 );              // The basename for language and config files. 
  // eg if it is "MyExt" then it will look for MyExt_lang_en.xml and MyExt.xml

  strncpy( pInfo->strVersion , "1.1.0.0" , 10 );                              // Version no of Your Extension
  strncpy( pInfo->strGuid , m_GuidString , 34 );                              // The Unique quid that this extension has.

  // PreStartInit is normally not called for FileProperties plugins.. add EXT_PREINIT flag if you want PreStartInit to be called during initialization
#ifdef _UNICODE
  pInfo->dwFlags = EXT_TYPE_PROP | EXT_PREINIT | EXT_OS_UNICODE;
#else
  pInfo->dwFlags = EXT_TYPE_PROP | EXT_PREINIT | EXT_OS_ANSI;
#endif

  pInfo->dwInitOrder = 2010; // All 3de party extension must start use 2000 or more

  pInfo->dwInterfaceVersion = MULTI_INTERFACE_VERSION;  // What version of MultiCommander Extension Interface this extensions is built for
  return true;
}

char* MCFilePropSample::Get_ModuleID()
{
    return m_GuidString;
}

long MCFilePropSample::PreStartInit( IMultiAppInterface* pAppInterface )
{
  IFilePropertiesManager* pPropMan = (IFilePropertiesManager*)pAppInterface->QueryInterface(ZOBJ_PROPMANGER, 0);

  if(pPropMan)
  {
    pPropMan->Init(m_GuidString);

    FilePropData fpd;
    ZeroMemory(&fpd, sizeof(FilePropData));

    std::wstring strCategory = pAppInterface->GetText(MAKETEXTID('p',200));
    fpd.szCategoryName = strCategory.c_str();
    fpd.szDescription = NULL;

    fpd.propType = MCFILEPROP_SAMPLE1;
    fpd.szPropName = L"SampleProp1";  // Machine name. only use Latin chars. This name is not shown to user. 
    fpd.szDisplayName = pAppInterface->GetText(MAKETEXTID('p',201));
    fpd.dwOptions =  FILEPROP_STRING|FILEPROP_ASYNC|FILEPROP_CUSTOMIZABLE;
    fpd.IdealWidth = 100;
    fpd.Align = DT_LEFT;
    pPropMan->RegisterProperty(&fpd);

    fpd.propType = MCFILEPROP_SAMPLE2;
    fpd.szPropName = L"SampleProp2";  // Machine name. only use Latin chars. This name is not shown to user. 
    fpd.szDisplayName = pAppInterface->GetText(MAKETEXTID('p',202));
    fpd.dwOptions =  FILEPROP_NUM|FILEPROP_ASYNC|FILEPROP_CUSTOMIZABLE | FILEPROP_EXECUTE; // If user Ctrl+DblClick on this column MCFilePropSample::Execute(..) is called.
    fpd.IdealWidth = 40;
    fpd.Align = DT_CENTER;
    pPropMan->RegisterProperty(&fpd);


    // pPropMan->Release(); 
    pAppInterface->ReleaseInterface((ZHANDLE)pPropMan, ZOBJ_PROPMANGER);

    m_ExtensionID = (WORD)pAppInterface->ModuleIDStrToID(m_GuidString);
  }

  return 0;
}

// pParentFileItem is the fileitem to the folder/parent volume that we are about to ask for file properties.
bool MCFilePropSample::Open( IFileItem* /*pParentFileItem*/ )
{
    return false;
}

bool MCFilePropSample::Open( const WCHAR* /*szParentPath*/ )
{
    return false;
}

// We no are no longer getting file property for the Parent sent in Open(..)
bool MCFilePropSample::Close()
{
    return true;
}

// If the file property has a special Display text. If this function return false. GetPropStr or GetPropNum is called.
// This can be used if a NUM property should be displayed in a text way. but sorted as NUM.
bool MCFilePropSample::GetDisplayValue( IFileItem* /*pFileItem*/, WCHAR* /*propData*/, WORD /*nLen*/, WORD /*propType*/, const volatile bool* /*pAbort*/ )
{
    return false;
}

// Get Property for properties of FILEPROP_STRING type
bool MCFilePropSample::GetPropStr( IFileItem* pFileItem, WCHAR* propData, WORD nLen, WORD propType, const volatile bool* /*pAbort*/ )
{
  WCHAR szFilename[_MC_MAXPATH_];
  pFileItem->Get_FullPath(szFilename, _countof(szFilename));

  //////////////////////////////////////////////////////////////////////////
  // Extras... (This part is not required)
  // Before you get the property you can check if it is already set. (Well if it is then this call should not have happened )
  // But you might have stored another property you need before getting the requested one. or something.
  ExtraProp prop;
  ZeroMemory(&prop, sizeof(ExtraProp));
  if(pFileItem->GetExtraPropData(m_ExtensionID, (WORD)propType, &prop))
  {
    if(prop.Flag & ZFXP_DISPLAYNAME) // If the stored property has display name set. then use it
    {
      wcsncpy(propData, (const WCHAR*)prop.szDisplayName, nLen);
      return true;
    }
  }
  //////////////////////////////////////////////////////////////////////////
  
  std::wstring strPropData;
  if(propType == MCFILEPROP_SAMPLE1)
  {
    // Get the property for szFilename.
    // But since we are got fetching any file value we will only take the filename and reverse it

    strPropData = pFileItem->Get_Name(szFilename, _countof(szFilename));
    std::reverse(strPropData.begin(),strPropData.end());
    wcsncpy(propData, strPropData.c_str(), nLen);
    return true;
  }
  return false;
}

// Get Property for properties of FILEPROP_NUM type
bool MCFilePropSample::GetPropNum( IFileItem* pFileItem, INT64* propData, WORD propType, const volatile bool* /*pAbort*/ )
{
  WCHAR szFilename[_MC_MAXPATH_];
  pFileItem->Get_FullPath(szFilename, _countof(szFilename));

  if(propType == MCFILEPROP_SAMPLE2)
  {
    // Get the property for szFilename.
    // But since we are got fetching any file value we will only take the filename and reverse it
    std::wstring strPropData;
    strPropData = pFileItem->Get_Name(szFilename, _countof(szFilename));

    *propData = strPropData.size();    
    return true;
  }

  return false;
}

bool MCFilePropSample::GetPropDouble(IFileItem* pFileItem, double* propData, WORD propType, const volatile bool* pAbort)
{
  UNREFERENCED_PARAMETER(pFileItem);
  UNREFERENCED_PARAMETER(propData);
  UNREFERENCED_PARAMETER(propType);
  UNREFERENCED_PARAMETER(pAbort);

  return false;
}


// Called if user change a property (Not Supported yet)
bool MCFilePropSample::SetProp( IFileItem* /*pFileItem*/, WORD /*propType*/, const BYTE* /*propData*/ )
{
  return false;
}

bool MCFilePropSample::Execute(ExecuteInfo* pExecuteInfo)
{
  WCHAR szFilename[_MC_MAXPATH_];
  szFilename[0] = '\0';

  if(pExecuteInfo && pExecuteInfo->pFileItem)
    pExecuteInfo->pFileItem->Get_Name(szFilename, _countof(szFilename));

  MessageBox(NULL, szFilename, _T("FileProp Sample"), MB_OK);
  return true;
}
