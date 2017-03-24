;===========================================================================
; Сценарий установки плагина Телепат 2.0 (Александр Орефков)

SubSection "Телепат 2.0"

	Section -"TelepatBeforeInstall"
		!insertmacro OC_STATUS "Установка плагинов | Телепат 2.0..."
	SectionEnd

	Section "Плагин, документация"

	    SectionIn 1 2

    	SetOutPath "$INSTDIR\config"
	    File "${OC_ConfigDir}\telepat.dll"
	    File "${OC_ConfigDir}\1cv7srct.st"

	    SetOutPath "$INSTDIR\config\scripts"
	    File "${OC_ConfigDir}\scripts\telepat.vbs"

		SetOutPath "$INSTDIR\config\docs"

		File "${OC_ConfigDir}\docs\Telepat.history.txt"	
		!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "Telepat.history.txt" "Телепат 2.0 - История изменений"
		
		File "${OC_ConfigDir}\docs\Telepat.chm"
		!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "Telepat.chm" "Телепат 2.0 - Справка"

	SectionEnd

	Section "Использовать альтернативную библиотеку иконок"

		SetOutPath "$INSTDIR"
		File "${OC_Bindir}\telepat.icl"

	SectionEnd

	Section "Утилита xml2tls"

		SectionIn 1 2

		CreateDirectory "$INSTDIR\config\system\xml2tls"
		SetOutPath "$INSTDIR\config\system\xml2tls"

		File "${OC_ConfigDir}\system\xml2tls\1cpplang.xml"
		File "${OC_ConfigDir}\system\xml2tls\xml2tls.exe"

		File "${OC_ConfigDir}\system\xml2tls\readme.txt"
		!insertmacro OC_ADD_FILE_TO_STARTMENU "config\system\xml2tls\readme.txt" "Документация\Телепат 2.0 - Утилита xml2ts"

	SectionEnd

SubSectionEnd