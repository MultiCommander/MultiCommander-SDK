#include "stdafx.h"
#include "MemoryFileSystem.h"
#include "AutoHandle.h"
#include <memory>
namespace
{
  std::vector<std::wstring> SplitString(const std::wstring& input, const std::wstring& delimiter)
  {
    size_t iPos = 0;
    size_t newPos = (size_t)-1;
    size_t sizeS2 = delimiter.size();
    size_t isize = input.size();
    std::vector<std::wstring> results;

    std::vector<unsigned int> positions;

    newPos = input.find(delimiter, 0);

    if(newPos == std::wstring::npos)
    {
      if(input.empty())
        return results;

      // only 1 item
      results.push_back(input);
      return results;
    }

    int numFound = 0;

    while( newPos > iPos || newPos == 0) // newPos can be 0 if there is a delimiter at first position'
    {
      numFound++;
      positions.push_back((unsigned int)newPos);
      iPos = newPos;
      newPos = input.find(delimiter, iPos+sizeS2+1);
    }

    for( DWORD i=0; i < positions.size(); i++ )
    {
      std::wstring s;
      if( i == 0 )
      { 
        s = input.substr(i, positions[i]); 
      }
      else
      {
        size_t offset = positions[i-1] + sizeS2;

        if( offset < isize )
        {
          if( i == positions.size() )
          {
            s = input.substr(offset);
          }
          else if( i > 0 )
          {
            s = input.substr( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2 );
          }
        }
      }
      if( s.size() > 0 )
      {
        results.push_back(s);
      }
    }

    return results;
  }

  std::vector<std::wstring> GetPathParts(const WCHAR* szPath)
  {
    std::vector<std::wstring> vParts = SplitString(szPath, _T("\\"));
    return std::move(vParts);
  }


}

std::shared_ptr<MemoryFile> MemoryFileSystem::FindByPath( const WCHAR* szPath , bool bFindParent)
{
  if(m_pRoot == NULL)
    return NULL;

  // Split Path..
  std::vector<std::wstring> vPath = GetPathParts(szPath);

  if(bFindParent && vPath.size() > 0)
    vPath.pop_back();

  std::shared_ptr<MemoryFile> pCurrent = m_pRoot;

  for(auto it = vPath.begin(); it != vPath.end(); ++it)
  {
    // Can only search in folders.
    if(pCurrent->IsFolder() == false)
      return NULL;

    pCurrent = pCurrent->FindByName((*it).c_str());
    if(pCurrent == NULL)
      return NULL;
  }

  return pCurrent;
}

std::shared_ptr<MemoryFile> MemoryFileSystem::FindParentByPath(const WCHAR* szPath)
{
  return FindByPath(szPath, true);
}

std::shared_ptr<MemoryFile> MemoryFileSystem::CreateFolderItem(const WCHAR* szPath)
{
  if(m_pRoot == NULL)
    return NULL;

  // Split Path..
  std::vector<std::wstring> vPath = GetPathParts(szPath);

  std::shared_ptr<MemoryFile> pCurrent = m_pRoot;

  for(auto it = vPath.begin(); it != vPath.end(); ++it)
  {
    // Can only search in folders.
    if(pCurrent->IsFolder() == false)
      return NULL;

    std::wstring name = (*it);
    auto pFolder = pCurrent->FindByName(name.c_str());
    if(pFolder == NULL)
    {
      pCurrent = pCurrent->CreateFolderItem(name.c_str());
    }
    else
      pCurrent = pFolder;
  }

  return pCurrent;

}

std::shared_ptr<MemoryFile> MemoryFileSystem::FindCreateItemByPath(const WCHAR* szPath)
{
  if(m_pRoot == NULL)
    return NULL;

  // Split Path..
  std::vector<std::wstring> vPath = GetPathParts(szPath);
  size_t nItems = vPath.size();

  std::shared_ptr<MemoryFile> pCurrent = m_pRoot;

  for(auto it = vPath.begin(); it != vPath.end(); ++it, --nItems)
  {
    // Can only search in folders.
    if(pCurrent->IsFolder() == false)
      return NULL;

    std::wstring name = (*it);
    auto pItem = pCurrent->FindByName(name.c_str());
    if(pItem == NULL)
    {
      if(nItems > 1)
        pCurrent = pCurrent->CreateFolderItem(name.c_str());
      else
        pCurrent = pCurrent->CreateNewFile(name.c_str());
    }
    else
      pCurrent = pItem;
  }

  return pCurrent;
}

MemoryFileSystem::MemoryFileSystem()
{
  m_pRoot = std::make_shared<MemoryFile>();
  FILETIME ft;
  m_pRoot->Init(L"?ROOT?", ZFF_FOLDER, 0, ft);
}

MemoryFileSystem::~MemoryFileSystem()
{
  int xBreakPos = 0;
  xBreakPos++;
}

void MemoryFileSystem::Clear()
{
  // Smart ptr so everything should clean up.
  m_pRoot = NULL;
}

//////////////////////////////////////////////////////////////////////////
MemoryFile::MemoryFile()
{
  m_Attributes = 0;
  m_FileTime.dwHighDateTime = 0;
  m_FileTime.dwLowDateTime = 0;
  m_Size = 0;
  m_RWCursor = 0;
}

INT64 MemoryFile::CalculateFolderSize() const
{
  if(IsFolder() == false)
    return m_Size;

  INT64 size = 0;
  for(auto it = m_Files.begin(); it != m_Files.end(); ++it)
  {
    size += (*it)->CalculateFolderSize();
  }

  return size;
}

void MemoryFile::Init(const WCHAR* szName, DWORD attribute, INT64 size, const FILETIME& time)
{
  m_Name = szName;
  m_Attributes = attribute;
  m_Size = size;
  m_FileTime = time;
}

bool  MemoryFile::ReserveContentSize(INT64 nContentSize)
{
  INT64 nBlocks = nContentSize / BLOCKSIZE;
  while(nBlocks * BLOCKSIZE < nContentSize)
    ++nBlocks;
  
  m_vContent.reserve(nBlocks);

  // if content is already larger do not add any. and do not remove any.
  for(INT64 n = m_vContent.size(); n < nBlocks; ++n)
  {
    std::unique_ptr<BYTE[]> p = std::unique_ptr<BYTE[]>(new BYTE[BLOCKSIZE]);
    m_vContent.push_back(std::move(p));
  }

  return true;
}

INT64 MemoryFile::ContentCapacity()
{
  return (BLOCKSIZE * m_vContent.size());
}

DWORD MemoryFile::SetFileData(MCNS::IFileItem* pFileItem)
{
  return pFileItem->Set_FileData(m_Name.c_str(),  NULL, m_Size, m_Attributes, &m_FileTime, &m_FileTime, &m_FileTime );
}

bool MemoryFile::WriteBytes(const BYTE* pData, DWORD dwLen, DWORD* pWritten)
{
  // 1. Check so that data fits in content
  if(m_Size + dwLen > ContentCapacity())
  {
    //ExpandContent(dwLen);
  }

  DWORD totalAdded = 0;
  while(dwLen > 0)
  {
    DWORD dwAdded = AddContent(pData, dwLen);
    dwLen -= dwAdded;
    pData += dwAdded;
    totalAdded += dwAdded;
  }
  *pWritten = totalAdded;
  return true;
}

// Add content at m_RWCursor position. 
DWORD MemoryFile::AddContent(const BYTE* pData, DWORD dwLen)
{
  DWORD nCapacity = 0;
  BYTE* pBlockData = GetBlockData(nCapacity);
  int nCopyLen = min(nCapacity, dwLen);
  CopyMemory(pBlockData, pData, nCopyLen);
  m_Size += nCopyLen;
  m_RWCursor += nCopyLen;
  return nCopyLen;
}

// Get the block and BYTE position at m_RWCursor, nBlockCapacity is a out parameter that return how much data is left in the block
BYTE* MemoryFile::GetBlockData(DWORD& nBlockCapacity)
{
  INT64 nBlock = m_RWCursor / BLOCKSIZE;
  INT64 nBlockOffset = m_RWCursor - (nBlock * BLOCKSIZE);
  nBlockCapacity = (DWORD)(BLOCKSIZE - nBlockOffset);
  return m_vContent.at(nBlock).get();
}

bool MemoryFile::ReadBytes(BYTE* pData, DWORD dwLen, DWORD* pWritten)
{
  bool bContinue = true;
  DWORD nTotalRead = 0;
  while(dwLen > 0 && bContinue)
  {
    DWORD nCapacity = 0;
    BYTE* pBlockData = GetBlockData(nCapacity);
    
    // Make sure we do not read past valid data
    if(m_RWCursor + nCapacity > m_Size)
      nCapacity = (DWORD)(m_Size - m_RWCursor);

    if(nCapacity == 0)
    {
      bContinue = false;
      continue;
    }

    int nCopyLen = min(nCapacity, dwLen);
    CopyMemory(pData, pBlockData, nCopyLen);
    m_RWCursor += nCopyLen;
    dwLen -= nCopyLen;
    pData += nCopyLen;
    nTotalRead += nCopyLen;
  }

  *pWritten = nTotalRead;
  return true;
}

void MemoryFile::Seek(INT64 nSeekPos)
{
  if(nSeekPos < m_Size)
    m_RWCursor = nSeekPos;
}

bool MemoryFile::DeleteFileItem(std::shared_ptr<MemoryFile>& pFile)
{
  for(auto it = m_Files.begin(); it != m_Files.end(); ++it)
  {
    if(pFile.get() == (*it).get())
    {
      m_Files.erase(it);
      return true;
    }
  }

  return false;
}

std::shared_ptr<MemoryFile> MemoryFile::At(size_t idx)
{
  if(idx >= m_Files.size())
    return NULL;

  return m_Files.at(idx);
}

std::shared_ptr<MemoryFile> MemoryFile::FindByName( const WCHAR* szName )
{
  for(auto it = m_Files.begin(); it != m_Files.end(); ++it)
  {
    if(_wcsicmp((*it)->Name(), szName) == 0)
      return (*it);
  }

  return NULL;
}

std::shared_ptr<MemoryFile> MemoryFile::CreateFolderItem(const WCHAR* szName)
{
  SYSTEMTIME st;
  FILETIME ft;
  GetLocalTime(&st);
  SystemTimeToFileTime(&st, &ft);

  auto pItem = std::make_shared<MemoryFile>();
  pItem->Init(szName, ZFF_FOLDER, 0, ft);

  m_Files.push_back(pItem);
  return pItem;
}

std::shared_ptr<MemoryFile> MemoryFile::CreateNewFile(const WCHAR* szName)
{
  if(szName == NULL)
    return NULL;

  auto pItem = std::make_shared<MemoryFile>();
  SYSTEMTIME st;
  FILETIME ft;
  GetLocalTime(&st);
  SystemTimeToFileTime(&st, &ft);
  pItem->Init(szName, 0, 0, ft);

  m_Files.push_back(pItem);
  return pItem;
}

bool MemoryFile::AddFileItem(std::shared_ptr<MemoryFile>& pFile)
{
  m_Files.push_back(pFile);
  return true;
}

DWORD MemoryFile::WriteToDisk(const WCHAR* szTargetName)
{
  AutoHandle hFile(::CreateFile(szTargetName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, 0));
  if (hFile.isValid() == false)
    return GetLastError();

  DWORD dwBufferSize = 1024 * 1024;
  auto pBuffer = std::unique_ptr<BYTE[]>(new BYTE[dwBufferSize]);

  // Move current pos for this item to start
  Seek(0);

  DWORD dwBytesWritten = 0;
  DWORD dwBufferContentSize = 0;
  while(ReadBytes(pBuffer.get(), dwBufferSize, &dwBufferContentSize) && dwBufferContentSize > 0)
  {
    if (WriteFile(hFile, pBuffer.get(), dwBufferContentSize, &dwBytesWritten, nullptr) == FALSE)
    {
      return GetLastError();
    }
  }
  
  ::SetFileTime(hFile, &m_FileTime, &m_FileTime, &m_FileTime);
  
  // Need to close file before we can set file attribute using that api
  hFile.Release();

  DWORD dwWinAttribute = m_Attributes & ZFF_WIN_ATTRIBUTES;
  SetFileAttributes(szTargetName, dwWinAttribute);
  return ERROR_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
MemoryFileEnumerator::MemoryFileEnumerator( std::shared_ptr<MemoryFile>& pParent )
{
  m_pParent = pParent;
  Reset();
}

MemoryFileEnumerator::~MemoryFileEnumerator()
{

}

std::shared_ptr<MemoryFile> MemoryFileEnumerator::GetCurrent()
{
  return m_pCurrent;
}

std::shared_ptr<MemoryFile> MemoryFileEnumerator::GetNext()
{
  auto pItem = PeakNext();
  if(pItem)
    ++m_nNextIdx;

  m_pCurrent = pItem;
  return pItem;
}

std::shared_ptr<MemoryFile> MemoryFileEnumerator::PeakNext()
{
  if(m_nNextIdx >= m_pParent->SubItems())
    return nullptr;

  return m_pParent->At(m_nNextIdx);
}
