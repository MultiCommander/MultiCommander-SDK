/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once

#include "MCNamespace.h"
MCNSBEGIN

  
 // What type of extension/plugun it is. Only one (1) of the flags can be set per extension interface.
  
#define EXT_TYPE_APP     0x00000100  // Application extension
#define EXT_TYPE_FS      0x00000200  // Filesystem plugin (Eg. Zip, FTP, Registry)
#define EXT_TYPE_FOP     0x00000400  // FileOperations plugin extension
#define EXT_TYPE_PROP    0x00000800  // File properties plugin
#define EXT_TYPE_THUMB   0x00000080  // File Thumbnail plugin
#define EXT_TYPE_ALL     0x00000FF0  // ALL Extension Type


#define EXT_OS_ANSI     0x00001000 // The extension is build for the ANSI API of Windows
#define EXT_OS_UNICODE  0x00002000 // The extension is build for the UNICODE API of Windows. (Unicode is recommended)

  // Default values
#define EXT_AUTOLOAD    0x00010000 // Autoload extension when MultiCommander is starting. (only valid if also EXT_TYPE_APP is set )
#define EXT_LOCALFS     0x00020000 // DO NOT USE - Internal flag for LocalFileSystem extension so that there can be some special handling for it make it faster.
#define EXT_NOLANGFILE  0x00040000 // Do not try to load language file for this module.
#define EXT_VIRTUALFFS  0x00060000 // DO NOT USE - Internal flag for LocalFileSystem extension so that there can be some special handling for it make it faster.
#define EXT_CANTDISABLE 0x00100000 // The Extension CANT be disabled. ( some internal modules can't be disabled. normally do NOT use this flag )
#define EXT_PREINIT     0x00200000 // Call PreStartInit at startup for none APP extensions.. Needed for FileSystem/FileProp plugin if you want PreStartInit to be called (However PreStartInit is not called from main thread..)
#define EXT_VDEVICEFS   0x00400000 // Virtual Device Filesystem. A virtual filesystem that is not located in a file(.zip/.rar) but in a Device (reg: , ftp: ).
                                   // eg. REG: FTP: .. ( the wsDefaultValues[0] is then not for matching extension but for matching devices ( eg "reg:")
#define EXT_POSTINIT    0x01000000 // Call PostStartInit at the end of the startup phase. this call is called from the main ui thread, ( EXT_PREINIT is not )

struct DLLExtensionInfo
{
  WCHAR  wsName[100];       // Name of extension
  WCHAR  wsPublisher[100];  // Publishers , Author
  WCHAR  wsURL[100];        // URL to homepage for extension 
  WCHAR  wsDesc[250];       // Short description that will be shown in Extension manager
  WCHAR  wsBaseName[100];   // Base filename for config and language files, Example if basename is "MyExt" then MyExt.xml is config , MyExt_lang_en.xml is loaded as language file
  char   strVersion[10];    // Version of the extension ( use format "5.3.2.12" )

  char   strGuid[34];     // Guid of extension
  DWORD  dwFlags;         // EXT_ Flags
  DWORD  dwInterfaceVersion;
  DWORD  dwInitOrder;     // Initialization Order. Lower -> Higher. User value for 1500 > 
  DWORD  dwLocalizedName; // TextID for localized name
  DWORD  dwLocalizedDesc; // TextID for localized description
  HICON  hIcon;

  // TODO - THIS NEED TO be Changed - how default parameters works.
  
  // If modules is EXT_TYPE_APP and EXT_AUTOLOAD and the first time the module is run it should use this as default values
  // For EXT_TYPE_APP it will have 4 "Key,Value,Key,Value" lists. and every list is a new Instance of the extension.
  WCHAR wsDefaultValues[4][512];
  DWORD dwDefaultFlags[2];

  // If the extension is of EXT_TYPE_FS (filesystem plugin)
  // Then register file ext and bytemarks as
  //   wcsncpy( pInfo->wsDefaultValues[0] , "rar,r??,r00" , 512);      // pInfo->wsDefaultValues[0] == Default File Extensions
  //   wcsncpy( pInfo->wsDefaultValues[1] , "HEX:526172211A"   , 512 );// pInfo->wsDefaultValues[1] == Default bytemarks for filesystem

};

MCNSEND

