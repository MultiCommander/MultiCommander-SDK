
Multi Commander SDK v0.9
========================
( This document is far from complete.)

About
------
Multi Commander has a intensive plugin/extension architecture. Almost everything you see in Multi Commander is from an extension.
The Zip Filesystem is an Extension. The Explorer Panel is an extension, UI part of Search is an extension, and more.
Some of the extension are built into MultiCommander.exe but they are using the same interfaces that you find in the SDK.
You will see all of the extension of you go to the  Extension Manager in the configuration menu in Multi Commander.


Extension /Plugins types
------------------------
There are a wide vararity of extension/plugins that are supported

 
    Application Extension  -  This can create UI inside a panel tab, or a new windows. They can add Commands and Menu options, 
                              Extend CustomCommands or Multi-Script with new command.
                              (Eg MultiRename, CheckSum (sfv) Tools)

    Volume Plugins         - Add Support for virtual filesystem. Like archives or other things that can be shown as a file system
                             ( Eg Zip, FTP, Registry )

    FileProperties Extension - Add support for extended file preoperty data. that can be shown in columns in Explorer Panel or used by MultiRename or other extensions
                              ( ExiF Image Tags, IDv3 Mp3 Tags )

    FileOperationPlugins -  Plugin that are call before/during file copy. Allowing the plugin do changes to the file being copyied.
                            (Not fully supported for 3d part usage yet)



How Multi Commander loads extensions
----------------------------------------
When MultiCommander starts it will scan the "Extensions/" subfolder and for all *.dll files it finds there it will 
check if it is a MultiCommander Extension. If it is it will check the configuration if it should be loaded or not.
If there is not configuration to exclude that extension it will register the extension in MultiCommander.
If the Extension is of APP type it will create an instance of the extension an call PreInitStart
this allows the extension to register commands and menus and other stuff into the MultiCommander core system.
It will then destory the created instance. and continue with startup.

When is an App-Extension instance created 
------------------------------------------
The App Extension registered some commands for its functions during PreInitStart. and when one of thouse commands is run 
an instance of the extension is created and the command is then pass to the extension so it can do its work.

When is a Volume-Plugin created
-----------------------------------------
MultiCommander will register the Volume plugin during start up. And when a filesystem of that type is requested an instance of it is created.

Volume plugin do not have access to the entire App Interface as App-Extension do. So depending on your command
