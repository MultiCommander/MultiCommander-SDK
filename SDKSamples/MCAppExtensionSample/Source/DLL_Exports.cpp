// MCExtensionTemplate.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MCAppExtensionSample.h"
#include "MCFilePropSample.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////

// nID is the ID of the extensions that it should create. This is
extern "C" /*__declspec(dllexport)*/ PVOID APIENTRY Create( int nID )
{
  if( nID == 0 )
  {
    MCNS::MCAppExtensionSample* pExtension = new MCNS::MCAppExtensionSample();
    MCNS::IPluginInterface* pInterface = static_cast<MCNS::MCAppExtensionSample*>(pExtension);
    return pInterface;
  }

  if( nID == 1 )
  {
    MCNS::MCFilePropSample* pExtension = new MCNS::MCFilePropSample();
    MCNS::IFileProperties* pInterface = static_cast<MCNS::MCFilePropSample*>(pExtension);
    return pInterface;
  }

  return NULL;
}

// pModule is an pointer to and extension that was Created with Create( int nID ).
// nID is the ID of the extension that pModule is
extern "C" /*__declspec(dllexport)*/ bool APIENTRY Delete( MCNS::IPluginInterface* pModule , int nID )
{
  if( pModule == NULL )
    return false;

  if( nID == 0 )
  {
    MCNS::MCAppExtensionSample* pExtension = dynamic_cast<MCNS::MCAppExtensionSample*>(pModule);
    if( pExtension )
    {
      delete pExtension;
      return true;
    }

  }

  if( nID == 1 )
  {
    MCNS::MCFilePropSample* pExtension = dynamic_cast<MCNS::MCFilePropSample*>(pModule);
    if( pExtension )
    {
      delete pExtension;
      return true;
    }

  }

  return false;
}

/*
  GetExtensionInfo is called until it returns false when Multi Commander is enumerating what Extensions and PLugins exists in this DLL. 
  nID will increase for every call. And if a ExtensionInfo is returned then that nID will be connected to that extension.
  
  This way you can have many extensions in the same dll.
*/

extern "C" /*__declspec(dllexport) */bool APIENTRY GetExtensionInfo( int nID , MCNS::DLLExtensionInfo* pInfo )
{
  if( nID == 0 )
    return MCNS::MCAppExtensionSample::GetExtensionInfo( pInfo );

  if( nID == 1 )
    return MCNS::MCFilePropSample::GetExtensionInfo(pInfo);

  return false;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif
