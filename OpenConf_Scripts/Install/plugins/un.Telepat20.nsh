;===========================================================================
; Сценарий деинсталляции плагина Телепат 2.0 (Александр Орефков)

SubSection un."Телепат 2.0r"

	Section un."TelepatBeforeUnInstall"
		!insertmacro OC_STATUS "Удаление плагинов | Телепат 2.0..."
	SectionEnd

	Section un."Плагин, документация"
	    Delete "$INSTDIR\config\telepat.dll"
	    Delete "$INSTDIR\config\scripts\telepat.vbs"
		Delete "$INSTDIR\config\docs\Telepat.history.txt"
		Delete "$INSTDIR\config\docs\Telepat.chm"
		Delete "$INSTDIR\telepat.icl"
		!insertmacro OC_USERFRIENDLY_DELETE "$INSTDIR\config\1cv7srct.st"
		!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Телепат 2.0 - История изменений"
		!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Телепат 2.0 - Справка"
	SectionEnd

	Section un."Утилита xml2tls"
		Delete "$INSTDIR\config\system\xml2tls\1cpplang.xml"
		Delete "$INSTDIR\config\system\xml2tls\readme.txt"
		Delete "$INSTDIR\config\system\xml2tls\xml2tls.exe"
		!insertmacro OC_DEL_FILE_FROM_STARTMENU "Документация\Телепат 2.0 - Утилита xml2ts"
		RMDir "$INSTDIR\config\system\xml2tls"
	SectionEnd

SubSectionEnd