@ECHO OFF

for /F "tokens=1-3* delims=." %%a in ('cscript.exe //nologo filever.vbs ..\output\ModOrganizer.exe') do @set version=%%a.%%b.%%c

echo "Packagin version %version%"

echo "creating archive"

cd ..\staging

e:\7-zip\7z.exe a -mx9 -r ModOrganizer_v%version%.7z ModOrganizer\*

cd ..\tools


echo "creating installer"

cd ..\staging\ModOrganizer

e:\7-zip\7z.exe a -mx9 -r ..\temp.7z *

cd ..

copy /B E:\7-zip\7zsd.sfx + ..\tools\config.txt + temp.7z ..\staging\ModOrganizer_v%version%_setup.exe

del temp.7z

cd ..\tools

echo "TODO: create patch"
