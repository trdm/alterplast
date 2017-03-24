;===========================================================================
;; —ценарий установки скриптов

SubSection /e "—крипты"

	!include "scripts\scripts.auto.nsh"

SubSectionEnd ;; —крипты

;===========================================================================
;; —ценарий деинсталл€ции скриптов

!ifdef OC_Uninstall
SubSection /e un."—крипты"

	!include "scripts\un.scripts.auto.nsh"

SubSectionEnd ;; un.—крипты
!endif ;; OC_Uninstall