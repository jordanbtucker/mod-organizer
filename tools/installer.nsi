; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "Mod Organizer"

!include 'LogicLib.nsh'
!include 'Sections.nsh'
!include 'strloc.nsh'

!system "ExtractVersionInfo.exe"
!include "App-Version.txt"

; Main Install settings
Name "${APPNAME} ${Version}"
InstallDir "$PROGRAMFILES\Mod Organizer"
;InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "..\staging\ModOrganizer-${Version}.exe"
RequestExecutionLevel user
SetCompress auto
SetCompressionLevel 9
SetCompressor /SOLID lzma


; Modern interface settings
!include "MUI2.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\ModOrganizer.exe"
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of Mod Organizer ${Version}.$\r$\n$\r$\n\
															IMPORTANT!$\r$\n\
															Run this installer as administrator if you want to install to 'Program Files'.$\r$\n$\r$\n\
															You can install multiple instances of Mod Organizer if you want to use it with different games.$\r$\n$\r$\n\
															You can install over an existing installation of Mod Organizer to update/repair it. You won't lose data.$\r$\n$\r$\n\
															Click Next to continue."
															
; !define MUI_COMPONENTSPAGE_SMALLDESC

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "static_data\license\gpl-3.0.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "!Mod Organizer" Section1

	; Set Section properties
	SectionIn RO

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"
	File "..\staging\ModOrganizer\ModOrganizer.exe"
	File "..\staging\ModOrganizer\helper.exe"
	File "..\staging\ModOrganizer\hook.dll"
	File "..\staging\ModOrganizer\msvcp100.dll"
	File "..\staging\ModOrganizer\msvcr100.dll"
	File "..\staging\ModOrganizer\nxmhandler.exe"
	File "..\staging\ModOrganizer\proxy.dll"
	File "..\staging\ModOrganizer\uibase.dll"
	SetOutPath "$INSTDIR\plugins\"
	File "..\staging\ModOrganizer\plugins\installerBundle.dll"
	File "..\staging\ModOrganizer\plugins\diagnoseBasic.dll"
	CreateDirectory "$INSTDIR\logs"
	SetOutPath "$INSTDIR\license\"
	File "..\staging\ModOrganizer\license\*.txt"
	SetOutPath "$INSTDIR\DLLs\"
	File "..\staging\ModOrganizer\DLLs\7z.dll"
	File "..\staging\ModOrganizer\DLLs\archive.dll"
	File "..\staging\ModOrganizer\DLLs\boost_python-vc100-mt-1_??.dll"
SectionEnd

Section "Qt DLLs" Section2
	SetOutPath "$INSTDIR\dlls\"
	File "..\staging\ModOrganizer\DLLs\Qt*.dll"
	File "..\staging\ModOrganizer\DLLs\dlls.manifest"
	SetOutPath "$INSTDIR\imageformats\"
	File "..\staging\ModOrganizer\imageformats\*.dll"
SectionEnd

Section "Python" SectionPython
	SetOutPath "$INSTDIR\"
	File "..\staging\ModOrganizer\python27.dll"
	File "..\staging\ModOrganizer\python27.zip"
	SetOutPath "$INSTDIR\plugins\"
SectionEnd

SectionGroup "Plugins" PluginsGroup
  Section "Manual Installer"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\installerManual.dll"
	SectionEnd
	Section "Quick Installer"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\installerQuick.dll"
	SectionEnd
	Section "BAIN Installer"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\installerBAIN.dll"
	SectionEnd
	Section "FOMOD Installer"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\installerFomod.dll"
	SectionEnd
	Section "NCC Installer" Section4
		SetOutPath "$INSTDIR\NCC\"
		File /r "..\staging\ModOrganizer\NCC\*"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\installerNCC.dll"
	SectionEnd
	Section "Python Support" PluginPython
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\proxyPython.dll"
	SectionEnd
	Section "Legacy ini editor"
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\iniEditor.dll"
	SectionEnd
	Section "FNIS Checker" PluginFNIS
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\checkFNIS.dll"
	SectionEnd
	Section "FNIS Checker" PluginNMMImport
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\NMMImport.dll"
	SectionEnd
	Section "Configurator" PluginConfigurator
		SetOutPath "$INSTDIR\plugins\"
		File "..\staging\ModOrganizer\plugins\pyCfg.py"
		SetOutPath "$INSTDIR\plugins\data\"
		File "..\staging\ModOrganizer\plugins\data\pyCfg*.py"
		File "..\staging\ModOrganizer\plugins\data\settings.json"
	SectionEnd
SectionGroupEnd

Section "Translations" Section5
	SetOutPath "$INSTDIR\translations\"
	File "..\staging\ModOrganizer\translations\*.qm"
SectionEnd

Section "Tutorials" Section6
	SetOutPath "$INSTDIR\tutorials\"
	File "..\staging\ModOrganizer\tutorials\*.js"
	File "..\staging\ModOrganizer\tutorials\*.qml"
SectionEnd

Section "Stylesheets" Section7
	SetOverwrite on
	
	SetOutPath "$INSTDIR\stylesheets\"
	File "..\staging\ModOrganizer\stylesheets\*.qss" 
SectionEnd

Section /o "Handle Nexus Links" Section8
	WriteRegStr HKCU "Software\Classes\nxm\shell\open\command" "" '$INSTDIR\nxmhandler.exe "%1"'
SectionEnd

Section /o "Startmenu Shortcut" Section9
	CreateDirectory "$SMPROGRAMS\Mod Organizer"
	CreateShortCut "$SMPROGRAMS\Mod Organizer\Mod Organizer.lnk" "$INSTDIR\ModOrganizer.exe"
	CreateShortCut "$SMPROGRAMS\Mod Organizer\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

Section -FinishSection
;	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
;	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
;	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	; set path to instdir because this will be the cwd if the user chooses to run MO from the installer (seriously nsis???)
	SetOutPath "$INSTDIR\"
SectionEnd

Function .onSelChange
${If} ${SectionIsSelected} ${PluginPython}
	!insertmacro ClearSectionFlag ${SectionPython} ${SF_RO}
	!insertmacro ClearSectionFlag ${PluginConfigurator} ${SF_RO}
${Else}
	!insertmacro SetSectionFlag ${SectionPython} ${SF_RO}
  !insertmacro UnSelectSection ${SectionPython}
	!insertmacro SetSectionFlag ${PluginConfigurator} ${SF_RO}
  !insertmacro UnSelectSection ${PluginConfigurator}
${EndIf}
FunctionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} "Core Application"
	!insertmacro MUI_DESCRIPTION_TEXT ${Section2} "Qt DLLs version 4.8.5. These are REQUIRED for Mod Organizer to run."
	!insertmacro MUI_DESCRIPTION_TEXT ${SectionPython} "Required for some of the plugins. Disable if you have 32-bit python 2.7.x installed system-wide."
	!insertmacro MUI_DESCRIPTION_TEXT ${Section4} "NCC is required to install some mods. It depends on the Microsoft .Net Framework."
	!insertmacro MUI_DESCRIPTION_TEXT ${Section5} "(Partial) translations of the Mod Organizer Interface to the various languages."
	!insertmacro MUI_DESCRIPTION_TEXT ${Section6} "Tutorials demonstrating basic usage inside the UI"
	!insertmacro MUI_DESCRIPTION_TEXT ${Section7} "Additional themes for Mod Organizer"
	!insertmacro MUI_DESCRIPTION_TEXT ${Section8} "Have MO handle $\"Download (NMM)$\" links on Nexus. Happens automatically when using the Globe toolbar icon."
	!insertmacro MUI_DESCRIPTION_TEXT ${Section9} "Creates a starmenu shortcut for Mod Organizer and its uninstaller."
	!insertmacro MUI_DESCRIPTION_TEXT ${PluginsGroup} "Plugins extend MOs functionality. Some of these are rather essential, disable only if you know what you're doing."
	!insertmacro MUI_DESCRIPTION_TEXT ${PluginPython} "This plugin itself doesn't provide any functionality but it's required for some other plugins to work."
	!insertmacro MUI_DESCRIPTION_TEXT ${PluginFNIS} "Checks whether FNIS needs to be run every time you start the game. Doesn't hurt, even if you don't use FNIS."
	!insertmacro MUI_DESCRIPTION_TEXT ${PluginNMMImport} "Imports Mods from a Nexus Mod Manager installation."
	!insertmacro MUI_DESCRIPTION_TEXT ${PluginConfigurator} "More sophisticated ini editor. Requires the python plugin."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\Mod Organizer.lnk"
	Delete "$SMPROGRAMS\Mod Organizer\Mod Organizer.lnk"
	Delete "$SMPROGRAMS\Mod Organizer\Uninstall.lnk"

	; Clean up Mod Organizer
	Delete "$INSTDIR\ModOrganizer.exe"
	Delete "$INSTDIR\helper.exe"
	Delete "$INSTDIR\hook.dll"
	Delete "$INSTDIR\msvcp100.dll"
	Delete "$INSTDIR\msvcr100.dll"
	Delete "$INSTDIR\nxmhandler.exe"
	Delete "$INSTDIR\proxy.dll"
	Delete "$INSTDIR\uibase.dll"
	Delete "$INSTDIR\license\*.txt"
	Delete "$INSTDIR\DLLs\7z.dll"
	Delete "$INSTDIR\DLLs\archive.dll"
	Delete "$INSTDIR\DLLs\boost_python-vc100-mt-1_??.dll"
	
	; Clean up cache of integrated browser
	RMDir /r "$INSTDIR\webcache"
	
	; Clean up generated logs and dumps
	Delete "$INSTDIR\logs\*"
	Delete "$INSTDIR\ModOrganizer.exe.dmp"

	; Clean up qt dll option
	Delete "$INSTDIR\DLLs\Qt*.dll"
	Delete "$INSTDIR\imageformats\*.dll"
	Delete "$INSTDIR\DLLs\dlls.manifest"
	
	; Clean up stylesheet option
	Delete "$INSTDIR\stylesheets\*.qss"
	
	; Clean up python option
	Delete "$INSTDIR\python27.dll"
	Delete "$INSTDIR\python27.zip"
	
	; Clean up NCC option
	RMDir /r "$INSTDIR\NCC\"
	
	; Clean up plugin options
	Delete "$INSTDIR\plugins\*.dll"
	Delete "$INSTDIR\plugins\*.py"
	Delete "$INSTDIR\plugins\data\*"
	
	; Clean up tutorials option
	Delete "$INSTDIR\tutorials\*.js"
	Delete "$INSTDIR\tutorials\*.qml"

	; Clean up translations option
	Delete "$INSTDIR\translations\*.qm"
	
	; Clean up tutorials option
	Delete "$INSTDIR\tutorials\*.js"
	Delete "$INSTDIR\tutorials\*.qml"
	
	MessageBox MB_YESNO|MB_ICONQUESTION \
			"Do you want to remove all data from the Mod Organizer installation (Settings, Downloads, Installed Mods, Profiles)?" \
			IDNO noremovedata
		Delete "$INSTDIR\ModOrganizer.ini"
		Delete "$INSTDIR\downloads\*"
		RMDir /r "$INSTDIR\mods\*"
		RMDir /r "$INSTDIR\overwrite\*"
		RMDir /r "$INSTDIR\profiles\*"
	noremovedata:
	
	ReadRegStr $0 HKCU "Software\Classes\nxm\shell\open\command" ""
	${un.StrLoc} $1 $0 $INSTDIR ">"
	${If} $1 == 0
		DeleteRegValue HKCU "Software\Classes\nxm\shell\open\command" ""
		MessageBox MB_OK|MB_ICONINFORMATION \
			"This installation of Mod Organizer was set up to handle Nexus Download links. If you intend to use a different Mod Manager now you may have to associate it with those links. \
			 How that works differs depending on the application:$\r$\n\
			 - If it's a different MO instance, simply start it and hit the globe icon.$\r$\n\
			 - If it's Nexus Mod Manager, start it as Administrator, go to settings->general and enable the $\"Associate with NXM URLs$\" option"
	${Endif}


	; Remove remaining directories (IF they are empty)
	RMDir "$SMPROGRAMS\Mod Organizer"
	RMDir "$INSTDIR\logs\"
	RMDir "$INSTDIR\downloads\"
	RMDir "$INSTDIR\stylesheets\"
	RMDir "$INSTDIR\mods\"
	RMDir "$INSTDIR\overwrite\"
	RMDir "$INSTDIR\plugins\"
	RMDir "$INSTDIR\profiles\"
	RMDir "$INSTDIR\imageformats\"
	RMDir "$INSTDIR\license\"
	RMDir "$INSTDIR\DLLs\"
	RMDir "$INSTDIR\translations\"
	RMDir "$INSTDIR\tutorials\"
	RMDir "$INSTDIR\"

SectionEnd

; eof