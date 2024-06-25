@echo off

SET MCPATH=D:\Projects\MultiCommander\EXE\x64\UDebug\

rem Set The folder name of the Extension
SET EXTENSIONNAME=MCFSSamples

if not exist "%MCPATH%Extensions\%EXTENSIONNAME%" mkdir "%MCPATH%Extensions\%EXTENSIONNAME%"

if not exist "%MCPATH%Extensions\%EXTENSIONNAME%\Languages" mkdir "%MCPATH%Extensions\%EXTENSIONNAME%\Languages"

echo Copy Extension
copy /y %1 "%MCPATH%Extensions\%EXTENSIONNAME%\"

echo Copy Configuration for Extension
xcopy /S  /Y "%~dp0Config\*" "%MCPATH%Extensions\%EXTENSIONNAME%\"

exit 0