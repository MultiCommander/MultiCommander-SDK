// MCExtensionTemplate.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "FSSampleStream.h"
#include "FSSampleInternal.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH: break;
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    case DLL_PROCESS_DETACH:
      {
        MCNS::MCFSSampleStream::MemFSClear();
        MCNS::MCFSSampleInternal::MemFSClear();
      }
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
    MCNS::MCFSSampleStream* pExtension = new MCNS::MCFSSampleStream();
    MCNS::IVolume* pIVolume = static_cast<MCNS::MCFSSampleStream*>(pExtension);
    return pIVolume;
  }

  if( nID == 1 )
  {
    MCNS::MCFSSampleInternal* pExtension = new MCNS::MCFSSampleInternal();
    MCNS::IVolume* pIVolume = static_cast<MCNS::MCFSSampleInternal*>(pExtension);
    return pIVolume;
  }

  return NULL;
}

// pModule is an pointer to and extension that was Created with Create( int nID ).
// nID is the ID of the extension that pModule is
extern "C" /*__declspec(dllexport)*/ bool APIENTRY Delete( MCNS::IVolume* pModule , int nID )
{
  if( pModule == NULL )
    return false;

  if( nID == 0 )
  {
    MCNS::MCFSSampleStream* pFSPlugin = dynamic_cast<MCNS::MCFSSampleStream*>(pModule);
    if( pFSPlugin )
    {
      delete pFSPlugin;
      return true;
    }

  }
  if( nID == 1 )
  {
    MCNS::MCFSSampleInternal* pFSPlugin = dynamic_cast<MCNS::MCFSSampleInternal*>(pModule);
    if( pFSPlugin )
    {
      delete pFSPlugin;
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
    return MCNS::MCFSSampleStream::GetExtensionInfo( pInfo );

  if( nID == 1 )
    return MCNS::MCFSSampleInternal::GetExtensionInfo( pInfo );

   return false;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif
