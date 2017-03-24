;===========================================================================
; Сценарий установки скрипта code_beautifier.pl

Section "code_beautifier.pl"

	SectionIn 1 2

	!insertmacro OC_IF_PERLSCRIPT_INSTALLED 0 PS_Not_Installed 	
		SetOutPath "$INSTDIR\config\scripts\Редактирование"
		File "${OC_ConfigDir}\scripts\Редактирование\code_beautifier.pl"
		File "${OC_ConfigDir}\scripts\Редактирование\RunPerlScripts.vbs"
		goto end

	PS_Not_Installed:
		MessageBox MB_OK "В Вашей системе не установлен PerlScript Engine.$\r$\n\
			Скрипт code_beautifier.pl не будет установлен!"

	end:

SectionEnd