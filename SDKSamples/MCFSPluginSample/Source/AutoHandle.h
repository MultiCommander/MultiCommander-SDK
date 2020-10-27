#pragma once

class AutoHandle
{
public:
	AutoHandle(HANDLE h = INVALID_HANDLE_VALUE) 
    : m_hHandle(h)
	{

	}
	~AutoHandle()
	{
		Release();
	}

  void Release()
  {
    if(isValid())
    {
      CloseHandle(m_hHandle);
      m_hHandle = INVALID_HANDLE_VALUE;
    }
  }

	bool isValid()
	{
		if(m_hHandle == INVALID_HANDLE_VALUE)
			return false;

		return true;
	}

	AutoHandle& operator=(const HANDLE& handle)
	{
    Release();
		m_hHandle = handle;
		return *this;
	}
	
	operator HANDLE()
	{
		return m_hHandle;
	}

private:
	HANDLE m_hHandle;

};