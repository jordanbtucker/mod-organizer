echo "Compiling everything"
@echo off

set OLDDIR=%CD%

set BOOSTPATH=E:\boost\boost_1_55_0
set PYTHONPATH=E:\Python278
set PATH=%PATH%;%QTDIR%\Tools\QtCreator\bin;%PYTHONPATH%;%PYTHONPATH%\Lib\site-packages\PyQt4
set ZLIBPATH=E:\Documents\Projects\zlib-1.2.7
set SEVENZIPPATH=E:\Documents\Projects\7zip
set LOOTPATH=C:\Users\Tannin\Documents\Projects\loot_api


set /p REBUILD="Rebuild? (y/n)" %=%

if /i {%REBUILD%}=={n} (goto :skipbuild)

rmdir /s /q ..\staging_prepare
mkdir ..\staging_prepare

call "E:\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
call "E:\Qt\4.8.6\bin\qtvars.bat" vsvars

cd ..\staging_prepare
qmake.exe ..\source\ModOrganizer.pro -r -spec win32-msvc2010 CONFIG+=release CONFIG-=debug
jom.exe

:skipbuild

set /p REBUILD="Fetch Translations? (y/n)" %=%
if /i {%REBUILD%}=={n} (goto :skipfetch)

rmdir /s /q ..\translations
mkdir ..\translations

cd ..\source
..\tools\tx -q pull -a --minimum-perc=20
for /r %%f in (*.ts) do copy %%f ..\translations
cd ..\translations
del *_en.ts
for %%f in (*.ts) do lrelease %%f

chdir /d %OLDDIR%

:skipfetch

rmdir /s /q ..\staging\ModOrganizer
mkdir ..\staging\ModOrganizer

for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do @set version=%%a_%%b_%%c

mkdir ..\pdbs\plugins_%version%

copy /y ..\output\ModOrganizer.pdb ..\pdbs\ModOrganizer_%version%.pdb
copy /y ..\output\hook.pdb ..\pdbs\hook_%version%.pdb
copy /y ..\output\uibase.pdb ..\pdbs\uibase_%version%.pdb
rem copy /y ..\output\dlls\boss.pdb ..\pdbs\boss_%version%.pdb
copy /y ..\output\plugins\*.pdb ..\pdbs\plugins_%version%

xcopy /y /I ..\output\ModOrganizer.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\helper.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\nxmhandler.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\uibase.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\hook.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\proxy.dll ..\staging\ModOrganizer\
xcopy /y /s /I ..\output\stylesheets ..\staging\ModOrganizer\stylesheets
xcopy /y /s /I ..\output\tutorials ..\staging\ModOrganizer\tutorials
xcopy /y /I ..\staging_trans\*.qm ..\staging\ModOrganizer\translations
del ..\staging\ModOrganizer\translations\*_en.qm
xcopy /y /I ..\output\plugins\*.dll ..\staging\ModOrganizer\plugins\
xcopy /y /I ..\output\plugins\*.py ..\staging\ModOrganizer\plugins\
xcopy /y /s /I /EXCLUDE:exclude.txt ..\output\plugins\data ..\staging\ModOrganizer\plugins\data
xcopy /y /s /I ..\output\NCC ..\staging\ModOrganizer\NCC
xcopy /y /s /I ..\output\loot ..\staging\ModOrganizer\loot
xcopy /y /s /I static_data\* ..\staging\ModOrganizer
xcopy /y /I ..\output\dlls\archive.dll ..\staging\ModOrganizer\dlls
rem xcopy /y /I ..\output\dlls\boss.dll ..\staging\ModOrganizer\dlls
xcopy /y /I ..\output\dlls\dlls.manifest ..\staging\ModOrganizer\dlls
xcopy /y /s /I ..\translations\*.qm ..\staging\ModOrganizer\translations

cd ..\staging\ModOrganizer

rem windeployqt ModOrganizer.exe --no-translations --no-plugins --libdir dlls --release
rem windeployqt uibase.dll --no-translations --no-plugins --libdir dlls --release

chdir /d %OLDDIR%

for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do echo %%a.%%b.%%c > ..\staging\version.txt
exit /B

exit /B