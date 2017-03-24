;===========================================================================
; Сценарий деинсталляции скрипта navgator.js

Section "un.navigator.js"

	Delete "$INSTDIR\config\scripts\Навигация\navigator.js"

	Delete "$INSTDIR\config\docs\navigatorJS.readme.txt"
	!insertmacro OC_DEL_FILE_FROM_STARTMENU "navigator.js - Описание"

SectionEnd
