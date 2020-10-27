Version 0.2 - Copyright 2020 - Mathias Svensson

  Introduction
  =====================================================

  This DLL contains 2 File System plugins in the same dll.
  One FileSystem Plugin that are using Streaming mode and one that support internal mode.

  A FSPlugin can support both. 

  FSPlugin can also be 2 different types. It can be Virtual Device FileSystem or File Based FileSystem.
  File Base FileSystem is what you see when you browse into a .zip or other around then your path can be c:\folder\myarchive.zip\subfolder
  As you see, the zip is "mounted" in the file system.
  Virtual Device FS are like Registry or FTP plugin. you browse them by accessing a virtual device like REG: FTP:
  The difference is just how they are mounted and that with file based filesystem you need to be extra carefull when converting paths
  from MC Paths to local paths. BUt everything on how plugins are read/written are the same.
    
  All FSPlugins are inherited from IVolume. When you look at this first it can look scary but you do not have do support all of the 
  features that you see in IVolume. Some of the functions are only used for Streaming mode and some for Internal Mode.

  Internal Mode plugin are the easiest. But Internal mode plugin can only work aginst the Local Filesystem 
  (That also include network path \\server\share\, Everything the built in filesystem in Windows support ) 
  All the work to Add/Extract/Remove/Rename is done internally. If the user read a file the plugin will get a path where to save to file to.
  (Eg used by FSRAR, FS7Zip )

  With Streaming Mode plugin a user can copy from FTP directory to a Streaming Mode FileSystem plugin without data being stored locally first.
  If the user read a file the plugin will be asked to return a RWFile interface, and then ReadFile/WriteFile is used to read/write bytes from the file.
  This is similar to how the filesystem API works for files in Windows.
  (Eg used by FSLocal, FSFTP )

  A plugin can use a mix of both streaming and internal mode.
  FSZip is using both. If you read one file from a zip streaming mode is used.
  But if you are copying many files internal mode is used IF the target is local filesystem (C:\, \\server\share ) Because when using internal mode
  we can extract the file in the order they are stored inside the zip and it will be faster. 
  But if the user copy files from inside a Zip to FTP then streaming mode is used.

  With IVolume::GetSupportedOperations() you return what operation your plugin support

  Also one think to keep in mind when create a FileSystem Plugin is that Multi Commander is Multi-Threaded.
  All file copy operation is run in background threads, So it is possible to get call from different threads into the same instance of the plugin.
  So make sure you protect importent data with locks.
  (The FileSystem Plugin Sample are NOT thread safe. )

  Also all FileSystem Plugins are reference counted. And a instace of a FSPlugin is deleted when reference returns to zero (0).
  MC Core system only holds a reference when it absolutley needs to. When you browse away from the filesystem it "might" be removed.
  So do not store data that must survive a delete of the instance inside the IVolume inherited class.

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
