;===========================================================================
; Сценарий установки плагина OC2VBA (Александр Орефков)

Section "OC2VBA (VBA для OpenConf)"

    SectionIn 1 2

	!insertmacro OC_STATUS "Установка плагинов | OC2VBA..."

	;; Проверка на наличие установленного APC
	StrCpy $R0 ""
	ExpandEnvStrings $R0 "%CommonProgramFiles%"

	StrCmp $R0 "" APC_Not_Installed 0
		IfFileExists "$R0\Microsoft Shared\VBA\VBA6\apc6*.dll" APC_Installed APC_Not_Installed

	APC_Not_Installed: ;; APC не найден нами в системе

		MessageBox	MB_OK|MB_ICONINFORMATION \
			"Для работы плагина OC2VBA требуется наличие$\r$\n\
			установленного Application Programmability Components.$\r$\n\
			Этот компонент не обнаружен у Вас в системе,$\n$\r\
			поэтому плагин OC2VBA установлен не будет.$\r$\n\
			Подробнее об установке OC2VBA можно прочитать в файле$\r$\n\
			$INSTDIR\config\docs\Oc2Vba.readme.txt"

		;; Файл справки устанавливаем всегда, поскольку только что
		;; мы пообещали пользователю, что в ней (справке) он сможет 
		;; прочитать об установке OC2VBA и требуемых компонент подробнее

		goto Install_Readme

	APC_Installed: ;; APC установлен

	    SetOutPath "$INSTDIR\config"
    	File "${OC_ConfigDir}\oc2vba.dll"

		;; Возможно это переустановка, или установка "поверх", поэтому
		;; если файл проекта уже существует, не будем его затирать
		IfFileExists "$INSTDIR\openconf.ocp" Install_Readme 0
			SetOutPath "$INSTDIR"
			File "${OC_BinDir}\openconf.ocp"

	Install_Readme: 
	
		SetOutPath "$INSTDIR\config\docs"
		File "${OC_ConfigDir}\docs\Oc2Vba.readme.txt"
		!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "Oc2Vba.readme.txt" "Oc2Vba - Справка"

SectionEnd ;; OC2VBA
