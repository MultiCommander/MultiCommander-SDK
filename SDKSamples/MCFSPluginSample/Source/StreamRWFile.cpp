#include "stdafx.h"
#include "StreamRWFile.h"
#include "MemoryFileSystem.h"

using namespace MCNS;

StreamRWFile::StreamRWFile(std::shared_ptr<MemoryFile>& pMemFile)
  : m_pFile(pMemFile)
{
  m_pFile->Seek(0);
}

StreamRWFile::~StreamRWFile()
{

}

BOOL StreamRWFile::WriteFile( LPVOID lpBuffer , DWORD nBytesToWrite , LPDWORD lpBytesWritten )
{
  if(m_pFile->WriteBytes((const BYTE*)lpBuffer, nBytesToWrite, lpBytesWritten))
    return TRUE;

  return FALSE;
}

BOOL StreamRWFile::WriteFlush()
{
  return TRUE;
}

BOOL StreamRWFile::ReadFile( LPVOID lpBuffer , DWORD nBytesToRead , LPDWORD lpBytesRead )
{
  if(m_pFile->ReadBytes((BYTE*)lpBuffer, nBytesToRead, lpBytesRead))
    return TRUE;

  return FALSE;
}

DWORD StreamRWFile::GetLastError()
{
  return VERROR_NOERROR;
}
DWORD StreamRWFile::GetInternalError()
{
  return ERROR_SUCCESS;
}
// Resume a copy of file
DWORD StreamRWFile::Seek( INT64 /*nPos*/ , DWORD /*nMode*/ )
{
//   if(nMode == VSEEK_FILE_END)
//   {
// 
//   }
  return FALSE;
}

INT64 StreamRWFile::GetFileSize()
{
  m_pFile->GetFileSize();
  return FALSE;
}

BOOL StreamRWFile::SetFileTime( FILETIME /*ftCreate*/, FILETIME /*ftAccess*/ , FILETIME ftWrite )
{
  m_pFile->SetFileTime(&ftWrite);
  return TRUE;
}

BOOL StreamRWFile::GetFileTime( FILETIME &ftCreate, FILETIME &ftAccess , FILETIME &ftWrite )
{
  m_pFile->GetFileTime(&ftCreate);
  m_pFile->GetFileTime(&ftAccess);
  m_pFile->GetFileTime(&ftWrite);
  return TRUE;
}

void StreamRWFile::PostClose()
{
  //
  // Todo - Remove LOCK. So that file can be opened again.
  //
  int x = 0;
  x++;
}

//////////////////////////////////////////////////////////////////////////
void StreamRWFile::PrepareForWriting(UINT64 /*dwAttributes*/, DWORD /*dwRWFlags*/, const FILETIME* /*ftFileTime*/, INT64 nFileSize)
{
  // nFileSize is a hit on how much data might be sent. (It might be less, it might be more. it is only a hit )
  m_pFile->ReserveContentSize(nFileSize);
}
