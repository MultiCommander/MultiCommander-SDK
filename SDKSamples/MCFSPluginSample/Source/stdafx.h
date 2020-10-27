// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include <targetver.h>


// Windows Header Files:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

// STL
#include <string>
#include <vector>

// Multi Commander SDK
#include "..\..\..\MultiCommander\SDK\FileSystemPlugin.h"     // FileSystem Plugin
#include "..\..\..\MultiCommander\SDK\IFileItem.h"
#include "..\..\..\MultiCommander\SDK\IFileSystemManager.h"
#include "..\..\..\MultiCommander\SDK\IAppInterface.h"
#include "..\..\..\MultiCommander\SDK\IVolumeContextMenu.h"
