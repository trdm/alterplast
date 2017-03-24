;===========================================================================
;; Сценарии установки/деинсталляции плагинов

;===========================================================================
;; Субсекция установки

SubSection /e "Плагины"

	Section -"PluginsBeforeInstall"
		!insertmacro OC_STATUS "Установка плагинов..."
	SectionEnd
	
	!include "Telepat20.nsh"
	!include "HistoryPlugin.nsh"
	!include "SubSystems.nsh"
	!include "ExtFormsTree.nsh"
	!include "Oc2Vba.nsh"
	!include "Vis1Cpp.nsh"
	!include "ClassesWizard.nsh"
	!include "FDSubst.nsh"
	!include "Inspector2.nsh"

	Section -"PluginsAfterInstall"
		!insertmacro OC_STATUS "Установка плагинов | Готово"
	SectionEnd

SubSectionEnd

;===========================================================================
;; Субсекция деинсталляции

!ifdef OC_Uninstall
SubSection un."Плагины"

	Section un."PluginsBeforeUnInstall"
		!insertmacro OC_STATUS "Удаление плагинов..."
	SectionEnd

	!include "un.Telepat20.nsh"
	!include "un.HistoryPlugin.nsh"
	!include "un.SubSystems.nsh"
	!include "un.ExtFormsTree.nsh"
	!include "un.Oc2Vba.nsh"
	!include "un.Vis1Cpp.nsh"
	!include "un.ClassesWizard.nsh"
	!include "un.FDSubst.nsh"
	!include "un.Inspector2.nsh"

	Section un."AfterUninstallPlugins"
		;; Если директория system пустая, удалим ее по завершении 
		;; процесса деинсталляции всех плагинов
		RMDir "$INSTDIR\config\system"
		!insertmacro OC_STATUS "Удаление плагинов | Готово"
	SectionEnd

SubSectionEnd
!endif ;; OC_Uninstall
