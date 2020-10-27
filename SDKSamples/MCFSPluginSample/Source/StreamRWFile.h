#pragma once

#include <memory>

class MemoryFile;

MCNSBEGIN

class StreamRWFile : public IRWFile
{
public:
  StreamRWFile(std::shared_ptr<MemoryFile>& pMemFile);
  ~StreamRWFile();

  //////////////////////////////////////////////////////////////////////////
  // Overridden from IRWFile
  virtual BOOL WriteFile( LPVOID lpBuffer , DWORD nBytesToWrite , LPDWORD lpBytesWritten ) override;
  virtual BOOL WriteFlush() override;
  virtual BOOL ReadFile( LPVOID lpBuffer , DWORD nBytesToRead , LPDWORD lpBytesRead ) override;
  virtual DWORD GetLastError() override;
  virtual DWORD GetInternalError() override;
  virtual DWORD Seek( INT64 nPos , DWORD nMode ) override;
  virtual INT64 GetFileSize() override;
  virtual BOOL SetFileTime( FILETIME ftCreate, FILETIME ftAccess , FILETIME ftWrite ) override;
  virtual BOOL GetFileTime( FILETIME &ftCreate, FILETIME &ftAccess , FILETIME &ftWrite ) override;
  virtual void PostClose() override;
  //////////////////////////////////////////////////////////////////////////

  void PrepareForWriting(UINT64 dwAttributes, DWORD dwRWFlags, const FILETIME* ftFileTime, INT64 nFileSize);
protected:
  std::shared_ptr<MemoryFile> m_pFile;
};

MCNSEND