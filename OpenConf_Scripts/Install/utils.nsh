;===========================================================================
; Вспомогательные функции и макросы
;===========================================================================

;===========================================================================
;; Проверяет, установлен ли Perl Script Engine
!macro OC_IF_PERLSCRIPT_INSTALLED goto_IF_INSTALLED goto_IF_NOT_INSTALLED
	ReadRegStr $R0 HKEY_CLASSES_ROOT "PerlScript" ""
	;MessageBox MB_OK "-$R0-"
	StrCmp $R0 "" ${goto_IF_NOT_INSTALLED} ${goto_IF_INSTALLED}
!macroend

;===========================================================================
;; Вывод соообщения о статусе установки/деинсталляции

!macro OC_STATUS MSG
	SetDetailsPrint textonly
	DetailPrint "${MSG}"
	SetDetailsPrint listonly
!macroend

;===========================================================================
;; "Безопасное" удаление файла (с предварительным запросом на удаление)

!macro OC_USERFRIENDLY_DELETE FILEPATHNAME
	IfFileExists "${FILEPATHNAME}" 0 +3
		MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 \
			"Файл ${FILEPATHNAME}$\r$\nможет хранить Ваши \
			персональные настройки!$\r$\nУдалить этот файл \
			независимо от этого?" IDNO +2  
			Delete "${FILEPATHNAME}"
!macroend

;===========================================================================
;; Макрос-обертка для функции AddFileToStartMenu (см. ниже)

!macro OC_ADD_FILE_TO_STARTMENU FILENAME LINKNAME
	!ifdef OC_StartMenuPage
		Push "${FILENAME}"
		Push "${LINKNAME}"
		Call AddFileToStartMenu
	!endif
!macroend

;===========================================================================
;; Удаление ярлыка на файл из программной группы в меню Пуск

!macro OC_DEL_FILE_FROM_STARTMENU LINKNAME
	!ifdef OC_StartMenuPage
		Delete "$SMPROGRAMS\$STARTMENU_FOLDER\${LINKNAME}.lnk"
	!endif
!macroend

;===========================================================================
;; Макрос-обертка для функции AddFileToStartMenu (см. ниже)

!macro OC_ADD_DOCFILE_TO_STARTMENU FILENAME LINKNAME
	!ifdef OC_StartMenuPage
		Push "config\docs\${FILENAME}"
		Push "Документация\${LINKNAME}"
		Call AddFileToStartMenu
	!endif
!macroend

;===========================================================================
;; Удаление ярлыка на файл документации из программной группы в меню Пуск

!macro OC_DEL_DOCFILE_FROM_STARTMENU LINKNAME
	!ifdef OC_StartMenuPage
		Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Документация\${LINKNAME}.lnk"
	!endif
!macroend

;===========================================================================
;; Добавление ярлыка в программнуюю группу меню Пуск

Function AddFileToStartMenu
	Pop $0 ; [путь\]имя ссылки
	Pop $1 ; [путь\]имя файла (относительно каталога установки, т.е. BinDir)
	IfFileExists "$INSTDIR\$1" 0 end
		CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$0.lnk" "$INSTDIR\$1"
	end:
FunctionEnd

;===========================================================================
; GetParent
; input, top of stack  (e.g. C:\Program Files\Poop)
; output, top of stack (replaces, with e.g. C:\Program Files)
; modifies no other variables.
;
; Usage:
;   Push "C:\Program Files\Directory\Whatever"
;   Call GetParent
;   Pop $R0
;   
; at this point $R0 will equal "C:\Program Files\Directory"

Function GetParent

   Exch $R0
   Push $R1
   Push $R2
   Push $R3

   StrCpy $R1 0
   StrLen $R2 $R0

   loop:
     IntOp $R1 $R1 + 1
     IntCmp $R1 $R2 get 0 get
     StrCpy $R3 $R0 1 -$R1
     StrCmp $R3 "\" get
     Goto loop

   get:
     StrCpy $R0 $R0 -$R1

     Pop $R3
     Pop $R2
     Pop $R1
     Exch $R0

FunctionEnd ;; GetParent

;===========================================================================
