#pragma once

#include <memory> // std::unique_ptr

class MCNS::IProgress;
class MemoryFileSystem;
class MemoryFile;

class WriteItem
{
public:
  WriteItem(const MCNS::IFileItem* pItem, const WCHAR* szTargetName)
    : m_pSourceItem(pItem)
  {
    if(szTargetName)
      m_strTargetName = szTargetName;
  }

  const MCNS::IFileItem* m_pSourceItem;   // Source item to add into the plugin filesystem
  std::wstring     m_strTargetName; // Target path relative to plugin filesystem
};

class DeleteItem
{
public:
  DeleteItem(const WCHAR* szDeleteItem)
  {
    if(szDeleteItem)
      m_strDeleteItem = szDeleteItem;
  }

  std::wstring m_strDeleteItem;
};

#define IFOBJ_READ   0x00001000L
#define IFOBJ_WRITE  0x00010000L

class WriteToDiskItem
{
public:
  WriteToDiskItem() 
  {
  }
  WriteToDiskItem(std::shared_ptr<MemoryFile>& pSourceFile, const WCHAR* szTargetName) 
    : pFile(pSourceFile)
    , m_strTargetName(szTargetName)
  {
  }

  std::shared_ptr<MemoryFile> pFile;
  std::wstring m_strTargetName;
};

class InternalFileOperation
{
public:
  InternalFileOperation(DWORD t)
    : ObjType(t)
  {

  }
  virtual ~InternalFileOperation()
  {

  }

  DWORD ObjType = 0;
};

// Read FROM out internal filesystem
class InternalReadFileOperations : public InternalFileOperation
{
public:
  InternalReadFileOperations(MCNS::IProgress* pProgress , DWORD dwFlags);
  virtual ~InternalReadFileOperations()
  {

  }
  void Add(std::shared_ptr<MemoryFile>& pFile, const WCHAR* szWriteName)
  {
    m_vItems.push_back(std::make_unique<WriteToDiskItem>(pFile, szWriteName));
  }
  DWORD WriteQueuedItemsToDisk();
protected:
  DWORD WriteItem(WriteToDiskItem* pItem);

  std::vector<std::unique_ptr<WriteToDiskItem>> m_vItems;
  MCNS::IProgress* m_pProgress;
  DWORD m_dwFlags;

};

// Write TO out Internal filesystem
class InternalWriteFileOperations: public InternalFileOperation
{
public:
   InternalWriteFileOperations(MCNS::IProgress* pProgress , DWORD dwFlags);
   virtual ~InternalWriteFileOperations()
  {

  }

   bool AddItem(const MCNS::IFileItem* pItem, const WCHAR* szTargetName);
   bool AddDeleteItem(const WCHAR* szDeleteFileItem);
   
   DWORD StartFileOperations(MemoryFileSystem* pFS);

   // Copy Operations
   DWORD CountItems();
   const MCNS::IFileItem* SourceItemAt(DWORD n);
   std::wstring           TargetNameAt(DWORD n);

   //
   DWORD CountDeleteItems();
   const std::wstring& GetDeleteItemAt(DWORD n) const;
   void ClearDeleteItems();

private:
  void NotifyPathChange(const std::wstring& pathItem, bool bDelete = false) const;

  DWORD StartCopyFileOperations();
  DWORD StartDeleteFileOperations();
  DWORD DeleteFileItem(const std::wstring& strDeleteItem);

  DWORD CopyFileItem(const MCNS::IFileItem* pSourceItem, const std::wstring& strTargetPath);
  void  SetProgressTargetPath(const std::wstring& strTargetPath);
  void  SetProgressSourceInfo(const MCNS::IFileItem* pSourceItem, WCHAR* szSourcePath, size_t len);

  void SetFileDateAndAttributes(std::shared_ptr<MemoryFile>& pFile, const MCNS::IFileItem* pFileItem);

  MemoryFileSystem* m_pMemFS;
  MCNS::IProgress*  m_pProgress;
  DWORD m_dwFlags;

  std::wstring m_Empty;
  std::vector<WriteItem>  m_vItems;
  std::vector<DeleteItem> m_vDeleteItems;

  std::unique_ptr<BYTE[]> m_pBuffer;
};