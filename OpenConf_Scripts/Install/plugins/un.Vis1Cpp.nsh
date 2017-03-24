;===========================================================================
; Сценарий деинсталляции плагина Visual 1C++ (Александр Орефков)	  

Section un."Visual 1C++"

	!insertmacro OC_STATUS "Удаление плагинов | Visual 1C++..."

    Delete "$INSTDIR\config\vis1cpp.dll"
	
	Delete "$INSTDIR\config\docs\Vis1Cpp.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Visual 1C++ - Описание"

	Delete "$INSTDIR\config\docs\Vis1Cpp.readme.doc"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Visual 1C++ - Справка"

	Delete "$INSTDIR\config\docs\Vis1Cpp.history.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Visual 1C++ - История изменений"	

SectionEnd