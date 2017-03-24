
Section "un.Меню макросов из файла.vbs"
	
	Delete "$INSTDIR\config\scripts\Разное\Меню макросов из файла.vbs"
	Delete "$INSTDIR\config\Macros_all.ini"
	!insertmacro OC_USERFRIENDLY_DELETE "$INSTDIR\config\Macros.ini"

SectionEnd