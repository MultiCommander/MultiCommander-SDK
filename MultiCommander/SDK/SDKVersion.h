
/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
  Defines what interface version the extensions/plugins are built with.

  If interfaces and MultiCommander has different version they may NOT be able to load them
  Minor changes to interface might be allowed anyway.

  MultiCommander will check the SDK Version an extension is build with and will not load the extension if the 
  SDK version mismatch on FrameWork or Major version part.

  Version Number Description
  
  Version  1.2.3.4 

  1 = Interface Framework version for MultiCommander. if this change plugins or extension will need major rewrites to work again.
      
  2 = Major Interface change. Most likely the extension/plugins just need to be rebuilt to work again.
      A major changes has happened to some interfaces. like functions have been removed, rearrange. or parameters for existing functions have changed.
      Or even complete interfaces have been removed. Anything that will change the layout of the interface.
      
  3 = Minor Interface changes
      None critical change to the layout has accrued. New functions has been added to the end of existing interface. (Extension using the previous layout will still work).
      Or NEW interfaces added. 
      
  4 = (Will load extension even if this is different)
      none critical change has accrued. Like a function or parameter got a name change, but they are still at the same position in 
      the interface so existing extensions/plugins should still work
      
      summary

     Version No    Change has been made              Loaded?  Need to fix                      
     ---------------------------------------------------------------------------------------------------------------------
         1.x.x.x | MC Interface Framework Changed   |  No  | Rebuild and major Rewrites of Extension/Plugin required
         x.2.x.x | Major Interface Changes          |  No  | Rebuild of Extension/Plugin should fix any problems
         x.x.3.x | Minor Interface Changes          | Yes  | Should work but Rebuild is recommended
         x.x.x.4 | Insignificant Interface Changes  | Yes  | No rebuild is required.
     ---------------------------------------------------------------------------------------------------------------------
   
*/
#pragma once

// MIV = MULTI_INTERFACE_VERSION
#define MIV_FRAMEWORK   1

#define MIV_MAJOR       100
#define MIV_MINOR       0
#define MIV_MICRO       0

//  91300         // 1.2.3.4 => 01.02.03.04 => 1020304 ,  90930 = 0.9.9.30
#define MULTI_INTERFACE_VERSION (DWORD)((MIV_FRAMEWORK * 1000000) + (MIV_MAJOR * 10000) +(MIV_MINOR * 100) + MIV_MICRO)
#define MULTI_INTERFACE_VERSION_DOTTED  "1.100.0.0"  

