
#include "stdafx.h"
#include "MCCopyDataSendGetSample.h"
#include <string>
#include "MCCopyData.h"
#include <atlconv.h>


// Global Variables:
HINSTANCE hInst;								// current instance
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  MSG msg;
  BOOL ret;

  HWND hDlg;
  hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
  ShowWindow(hDlg, nCmdShow);

  while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
  {
    if (ret == -1)
      return -1;

    if (!IsDialogMessage(hDlg, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }


  return (int) msg.wParam;
}

std::wstring GetDlgText(HWND hDlg, UINT nDlgID)
{
  wchar_t szText[1024] = { 0 };
  GetDlgItemText(hDlg, nDlgID, szText, _countof(szText));
  return szText;
}
void OnCancel(HWND hDlg)
{
   SendMessage(hDlg, WM_CLOSE, 0, 0);
}
DWORD GetOptionsFlags(HWND hDlg)
{
  DWORD dwFlags = 0;
  if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
    dwFlags |= CPF_LEFT;
  else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
    dwFlags |= CPF_LEFT;
  else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
    dwFlags |= CPF_SOURCE;
  else if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED)
    dwFlags |= CPF_TARGET;
  
  if (IsDlgButtonChecked(hDlg, IDC_CHECK_FOCUS) == BST_CHECKED)
    dwFlags |= CPF_FOCUSITEM;
  if (IsDlgButtonChecked(hDlg, IDC_CHECK_NEWTAB) == BST_CHECKED)
    dwFlags |= CPF_NEWTAB;

  return dwFlags;
}

void OnButton_GetCurrentItemFullPath(HWND hDlg)
{
  MultiCommander::MCCopyDataHelper MCCopyData;

  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentItemFullW(hDlg, GetOptionsFlags(hDlg));
  MCCopyData.ReleaseSubClass(hDlg);
  
  if (path[0] != '\0')
  {
    SetDlgItemText(hDlg, IDC_EDIT_PATH, path.c_str());
  }

}

void OnButton_GetCurrentItem(HWND hDlg)
{
  MultiCommander::MCCopyDataHelper MCCopyData;

  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentItemW(hDlg, GetOptionsFlags(hDlg));
  MCCopyData.ReleaseSubClass(hDlg);
  
  if (path[0] != '\0')
  {
    SetDlgItemText(hDlg, IDC_EDIT_PATH, path.c_str());
  }
}

void OnButton_GetCurrentPath(HWND hDlg)
{
  USES_CONVERSION;
  MultiCommander::MCCopyDataHelper MCCopyData;

  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentPathW(hDlg, GetOptionsFlags(hDlg));
  MCCopyData.ReleaseSubClass(hDlg);

  if (path[0] != '\0')
  {
    SetDlgItemText(hDlg, IDC_EDIT_PATH, path.c_str());
  }
  
}

void OnButton_GoToPath(HWND hDlg)
{
  TCHAR szPath[1024] = { 0 };
  GetDlgItemText(hDlg, IDC_EDIT_PATH, szPath, _countof(szPath));

  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.GoToPath(szPath, GetOptionsFlags(hDlg) , hDlg);
}


void OnInitDialog(HWND hDlg)
{
  SetDlgItemText(hDlg, IDC_EDIT_PATH, L"C:\\Windows\\System32");
  CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO3);
}
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_CLOSE:
    {
      DestroyWindow(hDlg);
      return TRUE;
    }

    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return TRUE;
    }

    case  WM_INITDIALOG :
    {
      OnInitDialog(hDlg);
      return TRUE;
    }
    case WM_COPYDATA:
    {
      // IF you did not subclass then you need to call it your self
      if (MultiCommander::g_pCaller)
      {
       HWND hWnd = (HWND) wParam;
        COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*) lParam;
        MultiCommander::g_pCaller->OnWMCopyData(hWnd, pCDS);
      }
      return TRUE;
    }
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case IDCANCEL: OnCancel(hDlg); return TRUE;
        case IDC_BUTTON1: OnButton_GetCurrentItemFullPath(hDlg); return TRUE;
        case IDC_BUTTON2: OnButton_GetCurrentItem(hDlg); return TRUE;
        case IDC_BUTTON3: OnButton_GetCurrentPath(hDlg); return TRUE;
        case IDC_BUTTON4: OnButton_GoToPath(hDlg); return TRUE;
      }
      break;
    }
  }
  return FALSE;
}

