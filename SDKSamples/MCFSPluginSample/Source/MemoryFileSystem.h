#pragma once

#include <vector>
#include <memory>

// Both FSSampleInternal and FSSampleStream is using this MemoryFileSystem.
const INT64 BLOCKSIZE = 32*1024;

class MemoryFile
{
public:
  MemoryFile();

  const WCHAR*  Name() const      { return m_Name.c_str();  }
  void  Name(const WCHAR* szName) { m_Name = szName; }

  INT64  Size()   const      { return m_Size; }
  bool IsFolder() const      { return (m_Attributes & ZFF_FOLDER) > 0; }

  UINT64 Attributes() const               { return m_Attributes;        }
  void  Attributes(UINT64 dwAttribute) { m_Attributes = dwAttribute; }

  void  SetFileTime(const FILETIME* ft) { m_FileTime = *ft; }
  void  GetFileTime(FILETIME* ft) const { *ft = m_FileTime; }
  INT64 GetFileSize() const             { return m_Size;    }
  INT64 CalculateFolderSize() const;

  void  Init(const WCHAR* szName, DWORD attribute, INT64 size, const FILETIME& time);

  bool  ReserveContentSize(INT64 nContentSize);
  INT64 ContentCapacity();

  DWORD SetFileData(MCNS::IFileItem* pFileItem);
  bool WriteBytes(const BYTE* pData, DWORD dwLen, DWORD* pWritten);
  bool ReadBytes(BYTE* pData, DWORD dwLen, DWORD* pWritten);
  void Seek(INT64 nSeekPos);
  
  bool DeleteFileItem(std::shared_ptr<MemoryFile>& pFile);
  // Find Subitems.
  size_t SubItems() { return m_Files.size(); }
  std::shared_ptr<MemoryFile> At(size_t idx);

  std::shared_ptr<MemoryFile> FindByName(const WCHAR* szName);
  std::shared_ptr<MemoryFile> CreateFolderItem(const WCHAR* szName);
  std::shared_ptr<MemoryFile> CreateNewFile(const WCHAR* szName);
  bool AddFileItem(std::shared_ptr<MemoryFile>& pFile);

  DWORD WriteToDisk(const WCHAR* szTargetName);

protected:
  DWORD AddContent(const BYTE* pData, DWORD dwLen);
  BYTE* GetBlockData(DWORD& nBlockCapacity);
  std::wstring m_Name;
  UINT64       m_Attributes;
  FILETIME     m_FileTime;

  INT64        m_Size;
  INT64        m_RWCursor; // next read/write pos - This should not be stored here. Only 1 thread can now read a file. Move to StreamRWFile

  // We do not allocate 1 entire block for the file
  // Because for large file the system might not be able to return large blocks.
  std::vector< std::unique_ptr<BYTE[]> > m_vContent;

  // Only valid if m_Attributes & ZFF_FOLDER
  std::vector< std::shared_ptr<MemoryFile> > m_Files;
};

class MemoryFileSystem
{
public:
  MemoryFileSystem();
  ~MemoryFileSystem();

  void Clear();

  std::shared_ptr<MemoryFile> FindByPath(const WCHAR* szPath, bool bFindParent = false);
  std::shared_ptr<MemoryFile> FindCreateItemByPath(const WCHAR* szPath); // Create item by path 
  std::shared_ptr<MemoryFile> FindParentByPath(const WCHAR* szPath);
  std::shared_ptr<MemoryFile> CreateFolderItem(const WCHAR* szPath);
  
protected:
  std::shared_ptr<MemoryFile> m_pRoot;
};

class MemoryFileEnumerator
{
public:
  MemoryFileEnumerator(std::shared_ptr<MemoryFile>& pParent);
  ~MemoryFileEnumerator();

  void Reset() { m_nNextIdx = 0; }

  std::shared_ptr<MemoryFile> GetNext();
  std::shared_ptr<MemoryFile> PeakNext(); // Get Next, But do not advance m_nNextIdx
  
  std::shared_ptr<MemoryFile> GetCurrent();

protected:
  size_t m_nNextIdx;
  std::shared_ptr<MemoryFile> m_pCurrent;
  std::shared_ptr<MemoryFile> m_pParent;
};
