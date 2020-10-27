===========================================================================
== Sample of how to Send and Get Data from MultiCommander using WM_COPYDATA
===========================================================================


MCCopyData.h is a helper class that will help you to Send/Get data To/From MC


To be able to recieve data from MultiCommander the caller must be a program 
that has a HWND window to recieve message.

Sending GOTO command to MultiCommander do not sends a response back so GOTO can
be send from program without a HWND.

If the path you are sending are ASCII/ANSI then the flag CPF_ANSI must be specified.
However if posible only send UNICODE (wide char) string. Since that works better with
none english characters.

The Helper class (MCCopyData) will send the command to the first MultiCommander
that it find. If you need a special instance of MultiCommander then you need to write your 
own code to find that.

Examples if using the include help class

  # Send GOTO command to Source Panel.
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.GoToPath(L"C:\\Windows\\System32", CPF_SOURCE, 0);

  # Send GOTO command to Source Panel, open as new tab
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.GoToPath(L"C:\\Windows\\System32", CPF_SOURCE|CPF_NEWTAB, 0);

  # Send GOTO command to Source Panel, open as new tab
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.GoToPath(L"C:\\Windows\\System32", CPF_SOURCE|CPF_NEWTAB, 0);

  # Send GOTO command to Source Panel, and also set MyTxt.txt as the focused item
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.GoToPath(L"D:\\temp\\MyTxt.txt", CPF_SOURCE|CPF_FOCUSITEM, 0);

# To Get Path/Item Name from MultiCommander, You need to response to the WM_COPYDATA message.
  If your program is a pure win32 (not MFC) you can use the included subclass functions in the helper class
  and let it process the message.

  # Get CurrentPath from source panel 
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentPathW(hDlg, CPF_SOURCE);
  MCCopyData.ReleaseSubClass(hDlg);

  
  # Get CurrentItem name from source panel (name of item in focus. No path)
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentItemW(hDlg, CPF_SOURCE);
  MCCopyData.ReleaseSubClass(hDlg);

  # Get full path to Current item in focus in source panel 
  MultiCommander::MCCopyDataHelper MCCopyData;
  MCCopyData.Subclass(hDlg);
  std::wstring path = MCCopyData.GetCurrentItemFullW(hDlg, CPF_SOURCE);
  MCCopyData.ReleaseSubClass(hDlg);

