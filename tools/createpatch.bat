@echo off

pushd .
cd ..\staging\ModOrganizer
@set STAGE_PATH=%CD%
popd


for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs %STAGE_PATH%\ModOrganizer.exe') do (
	@set VERSION=%%a.%%b.%%c
)

echo Current Version: %VERSION%
set /p REFERENCE_VERSION=Reference Version: 

rmdir /s /q ..\staging\ModOrganizer_patch
del ..\staging\ModOrganizer_v%REFERENCE_VERSION%_update.7z
mkdir ..\staging\ModOrganizer_patch

for /F "delims=" %%a in ('cscript //nologo strlen.vbs "%STAGE_PATH%"') do @set LENGTH=%%a

for /F %%f in ('dir /B /S /A-D %STAGE_PATH%') do (
  @set "absPath=%%f"
  setlocal EnableDelayedExpansion
  @set relPath=!absPath:~%LENGTH%!

	fc %STAGE_PATH%!relPath! ..\staging\ModOrganizer_v%REFERENCE_VERSION%!relPath! > nul
	if errorlevel 1 (
		xcopy %STAGE_PATH%!relPath! ..\staging\ModOrganizer_patch!relPath!*
	)

  endlocal
)

pushd .
cd ..\staging\ModOrganizer_patch
e:\7-zip\7z.exe a -mx9 -r ..\ModOrganizer_v%version%_update.7z *
popd
