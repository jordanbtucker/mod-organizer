echo "Compiling everything"
@echo off

call "E:\Qt\4.8.6\bin\qtvars.bat" vsvars

set /p REBUILD="Rebuild? (y/n)" %=%

if /i {%REBUILD%}=={n} (goto :skipbuild)

set OLDDIR=%CD%
rmdir /s /q ..\staging_prepare
mkdir ..\staging_prepare
call "E:\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
set BOOSTPATH=E:\boost\boost_1_55_0
set PYTHONPATH=E:\Python276
set PATH=%PATH%;E:\Qt\qtcreator2.8.1\bin\;%PYTHONPATH%
set ZLIBPATH=E:\Documents\Projects\zlib-1.2.7
set SEVENZIPPATH=E:\Documents\Projects\7zip
set LOOTPATH=E:\Documents\Projects\loot_src

cd ..\staging_prepare
qmake.exe ..\source\ModOrganizer.pro -r -spec win32-msvc2010 CONFIG+=release CONFIG-=debug
jom.exe
chdir /d %OLDDIR%

:skipbuild


rmdir /s /q ..\staging\ModOrganizer
mkdir ..\staging\ModOrganizer


set /p TRANSLATIONS="Fetch Translations? (y/n)" %=%

if /i {%TRANSLATIONS%}=={n} (goto :skiptrans)

set OLDDIR=%CD%
rmdir /s /q ..\staging_trans
mkdir ..\staging_trans

call config_translations.cmd

del /Q ..\staging_trans\*
FOR %%F IN (%TRANSLATIONS%) DO (
	wget -q -P ..\staging_trans http://translate.tannin.eu/%%F/mo/export/zip
	e:\7-zip\7z.exe x -o..\staging_trans ..\staging_trans\mo-%%F.zip
	chdir ..\staging_trans
  FOR %%T IN (*.ts) DO (
    set FILENAME=%%T
    set LANG=%%F
    Call :UpdateTranslation
  )
	ren *.ts ????????????????????_%%F.ts.x
  copy *.ts.x ..\source\%PROJNAME%\*. >nul
	chdir %OLDDIR%
)
chdir ..\staging_trans
ren *.ts.x *.
for %%F IN (*.ts) DO lrelease %%F >nul
chdir %OLDDIR%

:skiptrans


set /p TRANSLATIONS="Continue?" %=%



for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do @set version=%%a_%%b_%%c

mkdir ..\pdbs\plugins_%version%

copy /y ..\output\ModOrganizer.pdb ..\pdbs\ModOrganizer_%version%.pdb
copy /y ..\output\hook.pdb ..\pdbs\hook_%version%.pdb
copy /y ..\output\dlls\boss.pdb ..\pdbs\boss_%version%.pdb
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
xcopy /y /I ..\output\dlls\boss.dll ..\staging\ModOrganizer\dlls
xcopy /y /I ..\output\dlls\dlls.manifest ..\staging\ModOrganizer\dlls

for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do echo %%a.%%b.%%c > ..\staging\version.txt
exit /B


:UpdateTranslation
FOR /F "delims=" %%A in ("%FILENAME%") DO (
  set PROJNAME=%%~nA
)

if exist ..\source\%PROJNAME% (
  copy %FILENAME% ..\source\%PROJNAME%\%PROJNAME%_%LANG%.ts
) else (
  copy %FILENAME% ..\source\plugins\%PROJNAME%\%PROJNAME%_%LANG%.ts
)


exit /B