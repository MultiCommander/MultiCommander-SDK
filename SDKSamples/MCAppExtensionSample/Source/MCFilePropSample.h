
#pragma once

// IPluginInterface in located inside the MultiCommander namespace. and since all MC SDK types and interface are inside that namespace it is
// easier to just place your extension inside that interface too. 
// (If you do not want that. you need to inherit from MCNS::IPluginInterface. and also add MCNS:: infront of alot of type and interface below..)
//  ONLY put "using namespace MCNS;" in the .cpp file.. 
MCNSBEGIN

class MCFilePropSample : public IFileProperties
{
public:
  MCFilePropSample();
  ~MCFilePropSample();

  static bool GetExtensionInfo( DLLExtensionInfo* pInfo );
  //////////////////////////////////////////////////////////////////////////
  // IFileProperties overrides
  // //////////////////////////////////////////////////////////////////////////
  char* Get_ModuleID() override;

  long PreStartInit( IMultiAppInterface* pAppInterface ) override;

  bool Open(IFileItem* pParentFileItem) override;
  bool Open(const WCHAR* szParentPath) override;

  bool Close() override;

  bool GetDisplayValue(IFileItem* pFileItem, WCHAR* propData, WORD nLen, WORD propType, const volatile bool* pAbort) override;
  bool GetPropStr(IFileItem* pFileItem, WCHAR* propData, WORD nLen, WORD propType, const volatile bool* pAbort) override;
  bool GetPropNum(IFileItem* pFileItem, INT64* propData, WORD propType, const volatile bool* pAbort) override;
  bool GetPropDouble(IFileItem* pFileItem, double* propData, WORD propType, const volatile bool* pAbort) override;

  bool SetProp(IFileItem* pFileItem, WORD propType, const BYTE* propData) override;

  // Optional - Do not need to be overriden
  bool Execute(ExecuteInfo* pExecuteInfo) override;
  //////////////////////////////////////////////////////////////////////////
protected:
  IFileItem*          m_pCurrentFileItem; // We will cache the fileitem locally becuse so we can 

  static char         m_GuidString[34];
  static WORD         m_ExtensionID;  // Internal Extension ID. Can be different between run so query it in PreStartInit
};

MCNSEND
