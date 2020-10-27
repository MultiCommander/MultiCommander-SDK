
#pragma once

// IPluginInterface in located inside the MultiCommander namespace. and since all MC SDK types and interface are inside that namespace it is
// easier to just place your extension inside that interface too. 
// (If you do not want that. you need to inherit from MCNS::IPluginInterface. and also add MCNS:: infront of alot of type and interface below..)
//  ONLY put "using namespace MCNS;" in the .cpp file.. 
MCNSBEGIN

class MCAppExtensionSample : public IPluginInterface
{
public:
  MCAppExtensionSample(void);
  ~MCAppExtensionSample(void);

  static bool GetExtensionInfo( DLLExtensionInfo* pInfo );
public:

  //////////////////////////////////////////////////////////////////////////
  // [ Start - MultiCommander Extension API ]
  //////////////////////////////////////////////////////////////////////////
  long PreStartInit(  IMultiAppInterface* pAppInterface ) override;
  long GetModuleFlags() override;
  BOOL Init(  IMultiAppInterface* pAppInterface ) override;
  char* Get_ModuleID()  override { return m_GuidString; }
  bool OnNotify( ZHANDLE hFrom , DWORD nNotifyMsg , WPARAM wParam = 0 , LPARAM lParam = 0 , void* pExtra = NULL ) override;
  BOOL OnMessage( long msg , ZHANDLE hView , WPARAM param1 , LPARAM param2 ) override;
  
  BOOL OnCustomCommand( ICustomCommand* pCustomCommand ) override;

  BOOL OnClose(bool bShutDown, bool bDoNotAsk) override;

  // Use to send data between different kind of extensions
  int       DoImportData( IXData *pXData ) override;
  IImportData*  Get_ImportObject() override;
  IExportData*  Get_ExportObject() override;

  // OPTIONAL - This function is only required to be overridden if you adding MultiScript functions
  BOOL OnScriptFunction(IScriptFunctionContext* pScriptFuncContext) override;
  //////////////////////////////////////////////////////////////////////////
  // [ End - MultiCommander Extension API ]
  //////////////////////////////////////////////////////////////////////////
private:
  void OnCommand1();
  void OnCommand2();
  void OnCommand3();

  static char         m_GuidString[34];
  IMultiAppInterface* m_pAppInterface;

  BOOL OnCommandUI(DWORD nCommandID , DWORD* pFlags);

};

MCNSEND

