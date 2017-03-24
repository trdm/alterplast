;===========================================================================
; Сценарий деинсталляции плагина Подсистемы (Сергей Ушаков aka Phoenix)

Section un."Подсистемы"

	!insertmacro OC_STATUS "Удаление плагинов | Подсистемы ..."

    Delete "$INSTDIR\config\SubSystems.dll"
    Delete "$INSTDIR\config\SubSystems.tlb"
    Delete "$INSTDIR\config\ClipboardHistory.exe"

    Delete "$INSTDIR\config\scripts\SubSystemsManage.vbs"

	;; Файлы с персональными настройками самостоятельно удалять не будем, только с разрешения
	!insertmacro OC_USERFRIENDLY_DELETE "$INSTDIR\config\SubSystemsData.mdb"

	;; Если SubSystems.mdb все-таки был удален, удалим и сопутствующий *.ldb-файл
	IfFileExists "$INSTDIR\config\SubSystemsData.mdb" +2 0
		Delete "$INSTDIR\config\SubSystemsData.ldb"

	Delete "$INSTDIR\config\docs\SubSystems.readme.doc"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Подсистемы - Справка"

	Delete "$INSTDIR\config\docs\ClipboardHistory.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "ClipboardHistory - Описание"

SectionEnd