; Script generated with the Venis Install Wizard

RequestExecutionLevel admin

!include installerBase.nsi

Section "SetupInstance" SectionSetupInstance
	IfFileExists "$INSTDIR\INSTANCE" InstanceExists NoInstanceYet
	
	InstanceExists:
		Goto PastInstanceCheck
	
	NoInstanceYet:
		StrCpy $2 ""
		badrange:
				System::Call 'advapi32::SystemFunction036(*i0r0,i1)'
				IntCmpU $0 127 "" ""  badrange ; Limit to ASCII, IsCharAlphaNumeric is locale specific
				System::Call 'user32::IsCharAlphaNumericA(ir0)i.r1'
				StrCmp $1 0 badrange
				IntFmt $0 "%c" $0
				StrCpy $2 "$2$0"
				StrLen $0 $2
				IntCmpU $0 10 "" badrange
	
		FileOpen $3 "$INSTDIR\INSTANCE" w
		FileWrite $3 "$2"
		FileClose $3
	
		PastInstanceCheck:
	
	SectionEnd
	
	; eof