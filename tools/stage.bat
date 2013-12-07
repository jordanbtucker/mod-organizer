echo "Compiling everything"

call "E:\Qt\4.8.5\bin\qtvars.bat" vsvars

set /p REBUILD="Rebuild? (y/n)" %=%

if /i {%REBUILD%}=={n} (goto :skipbuild)

set OLDDIR=%CD%
rmdir /s /q ..\staging_prepare
mkdir ..\staging_prepare
call "E:\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
set BOOSTPATH=E:\boost\boost_1_49_0
set PYTHONPATH=E:\Python27_32bit
set PATH=%PATH%;E:\Qt\qtcreator2.8.1\bin\;%PYTHONPATH%
set ZLIBPATH=D:\Tannin_Documents\Projects\zlib-1.2.7
cd ..\staging_prepare
qmake.exe ..\source\ModOrganizer.pro -r -spec win32-msvc2010
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
	wget -P ..\staging_trans http://translate.tannin.eu/%%F/mo/export/zip
	e:\7-zip\7z.exe x -o..\staging_trans ..\staging_trans\mo-%%F.zip
	chdir ..\staging_trans
	ren *.ts ????????????????????_%%F.ts.x
	chdir %OLDDIR%
)
chdir ..\staging_trans
ren *.ts.x *.
for %%F IN (*.ts) DO lrelease %%F
chdir %OLDDIR%

:skiptrans


set /p TRANSLATIONS="Continue?" %=%



for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do @set version=%%a_%%b_%%c

mkdir ..\pdbs\plugins_%version%

copy /y ..\output\ModOrganizer.pdb ..\pdbs\ModOrganizer_%version%.pdb
copy /y ..\output\hook.pdb ..\pdbs\hook_%version%.pdb
copy /y ..\output\plugins\*.pdb ..\pdbs\plugins_%version%

xcopy /y /I ..\output\ModOrganizer.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\helper.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\nxmhandler.exe ..\staging\ModOrganizer\
xcopy /y /I ..\output\uibase.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\hook.dll ..\staging\ModOrganizer\
xcopy /y /I ..\output\proxy.dll ..\staging\ModOrganizer\
xcopy /y /s /I ..\output\stylesheets ..\staging\ModOrganizer\stylesheets
xcopy /y /s /I ..\output\tutorials ..\staging\ModOrganizer\tutorials
rem xcopy /y /s /I ..\output\translations ..\staging\ModOrganizer\translations
xcopy /y /I ..\staging_trans\*.qm ..\staging\ModOrganizer\translations
del ..\staging\ModOrganizer\translations\*_en.qm
xcopy /y /I ..\output\plugins\*.dll ..\staging\ModOrganizer\plugins\
xcopy /y /I ..\output\plugins\*.py ..\staging\ModOrganizer\plugins\
xcopy /y /s /I /EXCLUDE:exclude.txt ..\output\plugins\data ..\staging\ModOrganizer\plugins\data
xcopy /y /s /I ..\output\NCC ..\staging\ModOrganizer\NCC
xcopy /y /s /I static_data\* ..\staging\ModOrganizer
xcopy /y /I ..\output\dlls\archive.dll ..\staging\ModOrganizer\dlls
xcopy /y /I ..\output\dlls\dlls.manifest ..\staging\ModOrganizer\dlls
