Version 0.3 - Copyright 2025 - Mathias Svensson

  Introduction
  =====================================================

  This DLL contains 2 extennsion/plugins in the same dll.
  An App Extension and a FileProperties Plugin for Multi Commander.

  With the AppExtension sample you can see and understanding on how Application Extensions for Multi Commander are created and how they work.
  An Application Extension can add Commands/Menu/Toolbars and build UI in tabs and interact with existing UI, and with the core system.


  Setup and Build
  =====================================================

  To be able to debug you need to have Multi Commander portable. Both 32 and 64bit is recommended so you can test for both.

  * Change the PostBuild_x64.bat and PostBuild_win32.bat so that the path to the Portable version of MultiCommander is correct.

  * Change the paths to the MultiCommander SDK in stdafx.h so it matches where you have the SDK.

  * Configure the project to Launch the portable version of Multi Commander when you select to debug the DLL. (Assuming Portable version is in E:\MCPortable\64Bit)
       * Show the Properties for MCAppExtensionSample project
       * Select Debugging
       * Set "Command" to "E:\MCPortable\64bit\MultiCommander.exe"
       * Set "Working dir" to E:\MCPortable\64bit\
    (For 32bit build configuration you should ofcourse setup so that the 32bit version of MultiCommander.exe is run )

  The Sample project is setup to build both the 32bit adn 64bit versions.
  and it is setup to link the CRT static. If you include other 3de party libraries in your code you might need to change that
  but then remember to include the dynamic CRT with your plugin. Shipped version of MC is static linked so it require no external dependencies so 
  you need to provide your on dependencies if you have any.


 Create you own extension
 ========================================================
   You can use this as a base for you own extension
  
   However you need to change the project and class names and also the extension name
   The most importent thing you MOST change is the GUID.

   * Rename the MCAppExtensionSample.h/cpp and Class name to suite your purpose.
   * Also change the Library name in DLL_Exports.def or you will get a big fat warning
   * in DLL_Exports.cpp make sure Create/Delete/GetExtensionInfo is calling your extensions.
   * Fill out the information in the GetExtensionInfo in the extension class
   * Change the Extension GUID
   * Build both 32 and 64bit
   * Create a subfolder <MultiCommand>/Extensions/<Your Product Name>/
   * Copy the .dll and your config and language files there.

   For help use the forum
   http://forum.multicommander.com/forum/index.php/board,9.0.html
