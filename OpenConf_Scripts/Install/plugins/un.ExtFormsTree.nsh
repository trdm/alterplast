;===========================================================================
; Сценарий деинсталляции плагина ExtFormsTree (Сергей Ушаков aka Phoenix)

Section un."ExtFormsTree"

	!insertmacro OC_STATUS "Удаление плагинов | ExtFormsTree..."

    Delete "$INSTDIR\config\ExtFormsTree.dll"
    Delete "$INSTDIR\config\ExtFormsTree.tlb"
    Delete "$INSTDIR\config\scripts\ExtFormsTreeManage.vbs"

	;; Файлы с персональными настройками самостоятельно удалять не будем, только с разрешения
	!insertmacro OC_USERFRIENDLY_DELETE "$INSTDIR\config\ExtFormsTree.txt"

	Delete "$INSTDIR\config\docs\ExtFormsTree.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "ExtFormsTree - Описание"

SectionEnd