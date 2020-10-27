
#include <Windows.h>
#include <Winuser.h> // SetWindowHookEx
#include <tchar.h>
#include <vector>
#include <memory>
#include <string>
#include <tlhelp32.h>


// MultiCommander WM_COPYDATA Flags
#define CPF_GOTOPATH       0x00000002L // Go to path
#define CPF_GETCURPATH     0x00000004L // Go current path - Result returned in WM_COPYDATA message from MC
#define CPF_GETCURITEM     0x00000008L // Get name (File/Folder) of Current item in focus
#define CPF_GETCURITEMFULL 0x00000010L // Get full path for (File/Folder) of Current item in focus

#define CPF_LEFT           0x00000100L // Go to the new path in on the left side
#define CPF_RIGHT          0x00000200L // Go to the new path in on the right side
#define CPF_SOURCE         0x00000400L // Go to the new path in the source panel side
#define CPF_TARGET         0x00000800L // Go to the new path in the target panel side
#define CPF_ANSI           0x00010000L // If not specified All string are assumed to be Unicode
#define CPF_NEWTAB         0x00020000L // Open a new side on the specified side.
#define CPF_FOCUSITEM      0x00040000L // Change the focus to the file item provided in the path
#define CPF_RESPONSE       0x00100000L // The message is a respone FROM MultiCommander (Set by MC in response)


namespace MultiCommander
{
  // Do not like this solution. But short on time
  class MCCopyDataHelper;
  MCCopyDataHelper* g_pCaller = nullptr;
  UINT g_MC_WM_SENDTOFRONT = RegisterWindowMessage( _T("MULTICMD_WM_SENDTOFRONT"));

  extern INT_PTR CALLBACK MCCopyDataSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  class MCCopyDataHelper
  {
  public:
    void GoToPath(const wchar_t* szPath, ULONG_PTR dwFlags, HWND hwndResponse)
    {
      std::wstring path = szPath;

      SendCopyData((const BYTE*) path.c_str(), sizeof(wchar_t) * (path.size() + 1), dwFlags | CPF_GOTOPATH, hwndResponse);
    }

    void GoToPath(const char* szPath, ULONG_PTR dwFlags, HWND hwndResponse)
    {
      std::string path = szPath;

      SendCopyData((const BYTE*) path.c_str(), sizeof(char) * (path.size() + 1), dwFlags | CPF_GOTOPATH | CPF_ANSI, hwndResponse);
    }

    std::string GetCurrentPathA(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseA = "";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURPATH | CPF_ANSI, hwndResponse);

      return m_responseA;
    }

    std::wstring GetCurrentPathW(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseW = L"";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURPATH, hwndResponse);

      return m_responseW;
    }

    std::wstring GetCurrentItemW(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseW = L"";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURITEM, hwndResponse);

      return m_responseW;
    }

    std::string GetCurrentItemA(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseA = "";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURITEM, hwndResponse);

      return m_responseA;
    }

    std::string GetCurrentItemFullA(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseA = "";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURITEMFULL, hwndResponse);

      return m_responseA;
    }

    std::wstring GetCurrentItemFullW(HWND hwndResponse, ULONG_PTR dwFlags)
    {
      m_responseW = L"";
      SendCopyData(nullptr, 0, dwFlags | CPF_GETCURITEMFULL, hwndResponse);

      return m_responseW;
    }
    
    void Subclass(HWND hwnd)
    {
      g_pCaller = this;
      m_pOldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (DWORD_PTR)MCCopyDataSubclassProc);
    }

    void ReleaseSubClass(HWND hwnd)
    {
      g_pCaller = nullptr;
      if (m_pOldWndProc)
      {
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (DWORD_PTR)m_pOldWndProc);
      }
      m_pOldWndProc = 0;
    }

    INT_PTR CallOldWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
      if (m_pOldWndProc != 0)
        return ::CallWindowProc(m_pOldWndProc , hWnd, uMsg, wParam, lParam);
      return FALSE;
    }

    void OnWMCopyData(HWND hWnd, COPYDATASTRUCT* pCDS)
    {
      // response from GetCurrentPathFromMC
      if (pCDS->dwData & CPF_RESPONSE)
      {
        if (pCDS->dwData & CPF_GETCURPATH || pCDS->dwData & CPF_GETCURITEM || pCDS->dwData & CPF_GETCURITEMFULL)
        {
          DWORD nLen = (pCDS->cbData + (1 * sizeof(wchar_t))); // make sure that a extera null termination fits in buffert
          auto pResponseData = std::make_unique<BYTE []>(nLen);
          ZeroMemory(pResponseData.get(), nLen);
          CopyMemory(pResponseData.get(), pCDS->lpData, pCDS->cbData);

          if (pCDS->dwData & CPF_ANSI)
          {
            m_responseW = L"";
            m_responseA = (const char*)pResponseData.get();
          }
          else
          {
            m_responseA = "";
            m_responseW = (const wchar_t*)pResponseData.get();
          }
        }
      }
    }

    std::wstring m_responseW;
    std::string m_responseA;
  private:
    WNDPROC m_pOldWndProc = 0;
  protected:
    void SendCopyData(const BYTE* pData, DWORD len, DWORD dwFlags, HWND hReponseHwnd)
    {
      DWORD pid = 0;
      HWND hWndMC = GetMainHWNDForMultiCommander(pid);
      if (hWndMC == 0)
        return;

      // Build package
      COPYDATASTRUCT cds;
      cds.dwData = dwFlags;
      if (pData == nullptr)
      {
        cds.cbData = 0;
        cds.lpData = nullptr;
      }
      else
      {
        cds.cbData = len;
        cds.lpData = const_cast<BYTE*>(pData);
      }

      // Allow MC to steal focus from US
      AllowSetForegroundWindow(pid);

      // Will not return until sender had processed message
      SendMessage(hWndMC, WM_COPYDATA, (WPARAM) hReponseHwnd, (LPARAM) (LPVOID) &cds);
    }

    DWORD GetFirstPIDForModuleName(const TCHAR* szModuleName)
    {
      PROCESSENTRY32 entry;
      entry.dwSize = sizeof(PROCESSENTRY32);
      DWORD dwProcessID = 0;
      HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

      if (Process32First(snapshot, &entry) == TRUE)
      {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
          if (_tcsicmp(entry.szExeFile, szModuleName) == 0)
          {
            dwProcessID = entry.th32ProcessID;
            break;
          }
        }
      }

      CloseHandle(snapshot);
      return dwProcessID;
    }

    struct EnumWindowsCallbackArgs
    {
      EnumWindowsCallbackArgs(DWORD p) : pid(p), bRetry(false) {}
      const DWORD pid;
      bool bRetry; // retry becuse MC might be minimized to tray
      std::vector<HWND> handles;
    };

    static BOOL CALLBACK EnumMCWindowsCallback(HWND hnd, LPARAM lParam)
    {
      EnumWindowsCallbackArgs *args = (EnumWindowsCallbackArgs *) lParam;

      DWORD windowPID;
      (void)::GetWindowThreadProcessId(hnd, &windowPID);
      if (windowPID == args->pid)
      {
        HWND hwndParent = GetParent(hnd);
        if (hwndParent != 0)
          return TRUE;

        TCHAR szClassName[255];
        GetClassName(hnd, szClassName, _countof(szClassName));

        // New MC 5.1 style
        if (_tcscmp(szClassName, _T("MCSysTrayWnd")) == 0)
        {
          ::PostMessage(hnd, g_MC_WM_SENDTOFRONT, (WPARAM) 128, 0);
          args->bRetry = true;
          return TRUE;
        }
        else if (_tcscmp(szClassName, _T("MultiCommander MainWnd")) == 0)
        {
          // top level window
          LONG style = GetWindowLong(hnd, GWL_STYLE);
          if (style & WS_VISIBLE)
            args->handles.push_back(hnd);
          else if (style & WS_MINIMIZE)
            args->handles.push_back(hnd);
          return TRUE;
        }

        // Fallback to old style - up to 5.0
        // Top Level window has no parent
        if (hwndParent != 0)
          return TRUE;

        if (_tcsncmp(szClassName, _T("Afx:"), 4) == 0) // Main Window Class starts with Afx:
        {
          LONG style = GetWindowLong(hnd, GWL_STYLE);
          if (style & WS_VISIBLE)
            args->handles.push_back(hnd);
          else if (style & WS_MINIMIZE)
            args->handles.push_back(hnd);
        }
      }
      return TRUE;
    }

    HWND GetToplevelMCWindow(DWORD pid)
    {
      EnumWindowsCallbackArgs args(pid);
      if (::EnumWindows(&EnumMCWindowsCallback, (LPARAM) &args) == FALSE)
      {
        return 0;
      }

      // retry if MC Tray window was found.
      if (args.bRetry == true)
      {
        Sleep(250);
        args.bRetry = false;
        args.handles.clear();
        if (::EnumWindows(&EnumMCWindowsCallback, (LPARAM) &args) == FALSE)
        {
          return 0;
        }
      }

      if (args.handles.empty())
        return 0;

      return args.handles[0];
    }

    HWND GetMainHWNDForMultiCommander(OUT DWORD& pid)
    {
      pid = GetFirstPIDForModuleName(_T("MultiCommander.exe"));
      if (pid != 0)
      {
        return GetToplevelMCWindow(pid);
      }

      return 0;
    }
  };

  // WinProc
    // Stupid solution.. do not like it
  INT_PTR CALLBACK MCCopyDataSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch (uMsg)
    {
      case WM_COPYDATA:
      {
        if (g_pCaller)
        {
          HWND hWnd = (HWND) wParam;
          COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*) lParam;
          g_pCaller->OnWMCopyData(hWnd, pCDS);
          return TRUE;
        }
      }
    }

    if (g_pCaller)
      return g_pCaller->CallOldWndProc(hWnd, uMsg, wParam, lParam);
    return FALSE;
  }
  
};
