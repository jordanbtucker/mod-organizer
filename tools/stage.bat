@echo off

rmdir /s /q ..\staging\ModOrganizer
mkdir ..\staging\ModOrganizer

for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do @set version=%%a_%%b_%%c

copy /y ..\output\ModOrganizer.pdb ..\pdbs\ModOrganizer_%version%.pdb

xcopy /y /I ..\output\ModOrganizer.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\helper.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\uibase.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\hook.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\proxy.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\msvcp100.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\msvcr100.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\proxy.dll ..\staging\ModOrganizer\
xcopy /y /s /I ..\output\stylesheets ..\staging\ModOrganizer\stylesheets
xcopy /y /s /I ..\output\tutorials ..\staging\ModOrganizer\tutorials
xcopy /y /s /I ..\output\translations ..\staging\ModOrganizer\translations
xcopy /y /s /I ..\output\dlls ..\staging\ModOrganizer\dlls
xcopy /y /s /I ..\output\plugins ..\staging\ModOrganizer\plugins
xcopy /y /s /I static_data\* ..\staging\ModOrganizer