;===========================================================================
; —ценарий деинсталл€ции скрипта author.js

Section "un.author.js (јвторские комментарии)"

	SectionIn 1

	!insertmacro OC_STATUS "”даление скриптов | author.js..."

	Delete "$INSTDIR\config\scripts\–едактирование\author.js"

	Delete "$INSTDIR\config\docs\authorJS.readme.htm"
	!insertmacro OC_DEL_FILE_FROM_STARTMENU "author.js - —правка"

SectionEnd