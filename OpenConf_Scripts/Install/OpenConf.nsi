;===========================================================================
;
;	Copyright (c) 2004,2005 OpenConf Community, http://openconf.itland.ru
;   
;	NSIS-скрипт для сборки инсталлятора OpenConf'а, скриптов и плагинов
;
;	Авторы:
;    
;	Александр Кунташов aka a13x <kuntashov@yandex.ru> icq#338758861  
;
;===========================================================================

	!include "Sections.nsh"

	!include "parameters.nsh"

	!ifndef OC_VerFile
		!define OC_VerFile "_setup"
	!endif

	!ifndef OC_VerDisplay
		!define OC_VerDisplay ""
	!endif

;===========================================================================

	;Выходной файл инсталлятора
	OutFile "${OC_ReleaseDir}\oc${OC_VerFile}.exe"
	;Максимальная степень сжатия
	SetCompressor lzma

	;Типы установки
	InstType "Полный"
	InstType "Стандартный"
	InstType "Минимальный"

	;Директория установки по умолчанию
	InstallDir "$PROGRAMFILES\1Cv77\BIN"
	InstallDirRegKey HKLM "Software\OpenConf\Install" ""

;===========================================================================

	;Используем Modern UI
	!include "MUI.nsh"

;===========================================================================

	Name "OpenConf"
	Caption "Установка OpenConf ${OC_VerDisplay} для 1С:Предприятия 7.7"

;===========================================================================
;Глобальные переменные

	;;Var MUI_TEMP
	Var STARTMENU_FOLDER
	Var hwnd
	Var UPDATE

;===========================================================================
;Вспомогательные функции и макросы

	!include "utils.nsh"

;===========================================================================
;Интерфейс

!define MUI_ABORTWARNING

;FIXME Надо попросить Лохматого нарисовать лого и сплешик для OC и Инсталлера
;!define MUI_HEADERIMAGE
;!define MUI_WELCOMEFINISHPAGE_BITMAP "${OC_HeaderBitmap}"

;FIXME описания
!define MUI_COMPONENTSPAGE_NODESC
;!define MUI_COMPONENTSPAGE_SMALLDESC

;Первая страница
!define MUI_WELCOMEPAGE_TITLE "Вас приветствует мастер установки OpenConf ${OC_VerDisplay}!"
;; TODO загружать текст из внешнего файла (если можно)
!define MUI_WELCOMEPAGE_TEXT \
		"Этот мастер поможет вам установить OpenConf ${OC_VerDisplay} \
		для 1С:Предприятия 7.7.\r\n\r\n  \
		OpenConf - это программный продукт, разработанный Александром Орефковым, расширяющий \
		стандартные возможности Конфигуратора 1С:Предприятия.\r\n\r\n  \
		OpenConf предоставляет объектно - ориентированный интерфейс, позволяющий \
		автоматизировать рутинную работу по конфигурированию с помощью скриптов на \
		любых языках, поддерживающих технологию Active Scripting (например, VBScript \
		и JScript) и плагинов на языках высокого уровня (С++, Delphi и т.п.) с \
		использованием технологии COM.\r\n\r\n  $_CLICK"
!insertmacro MUI_PAGE_WELCOME

;Лицензия
!ifdef OC_LicenseFile
	!insertmacro MUI_PAGE_LICENSE "${OC_LicenseFile}"
!endif

;Расширенная страница выбора каталога установки
Page custom MyDirectoryPageEnter MyDirectoryPageLeave

!insertmacro MUI_PAGE_COMPONENTS

;Страница выбора программной группы (в меню Пуск)
!ifdef OC_StartMenuPage
	!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
	!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\1C\1Cv7\7.7\OpenConf"
	!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
	!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!endif

;Страница прогресса установки
!insertmacro MUI_PAGE_INSTFILES

;Последняя страница ("Готово")
!define MUI_FINISHPAGE_LINK "Посетите веб-сайт проекта Открытый Конфигуратор!"
!define MUI_FINISHPAGE_LINK_LOCATION "http://openconf.itland.ru/"
!define MUI_FINISHPAGE_NOREBOOTSUPPORT ;Без каких либо перезагрузок
!insertmacro MUI_PAGE_FINISH

!ifdef OC_Uninstall
	;Деинсталляция
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
!endif

;===========================================================================

;Russian only, sorry
!insertmacro MUI_LANGUAGE "Russian"

;===========================================================================
;Основная секция

Section "OpenConf ${OC_VerDisplay} (обязательно)" SecOpenConf

	SectionIn 1 2 3 RO

	SetOutPath "$INSTDIR"

	!insertmacro OC_STATUS "Установка OpenConf..."

	;;Создаем bin\config и bin\config\scripts
	CreateDirectory "$INSTDIR\config"
	CreateDirectory "$INSTDIR\config\docs"
	CreateDirectory "$INSTDIR\config\scripts"

	;;Копируем оригинальный 1С'овский config.dll в bin\config,
	;;если только это не режим обновления
	StrCmp $UPDATE "1" core_files 0
		!insertmacro OC_STATUS "Установка OpenConf | Перемещение config.dll..."
		CopyFiles "$INSTDIR\config.dll" "$INSTDIR\config"

	core_files:
		!insertmacro OC_STATUS "Установка OpenConf | Копироание файлов..."
		File "${OC_BinDir}\config.dll"
		;; TODO проверять и корректировать ссылку на tlb в реестре
		File "${OC_BinDir}\config.tlb"
		File "${OC_BinDir}\OpenConf.chm"

	;; Дабы не смущать пользователя тем, что справки по OpenConf'у 
	;; [в каталоге docs] нет, но на самом-то деле, как и суслик, она есть
	;; [в каталоге BIN, как того требует OpenConf], то создадим в docs 
	;; хотя бы ярлык на нее
	CreateShortCut "$INSTDIR\config\docs\OpenConf.lnk" "$INSTDIR\OpenConf.chm"

	;; readme- и history- файлы
	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ResDir}\readme.txt"
	File "${OC_ConfigDir}\docs\OpenConf.history.txt"
    File "${OC_ConfigDir}\docs\WhatsNew.txt"

	;Имя каталога установки сохраним в реестре
	WriteRegStr HKLM "Software\1C\1Cv7\7.7\OpenConf\Install" "" $INSTDIR

	!ifdef OC_Uninstall
		; Создаем анинсталлер
		WriteUninstaller "$INSTDIR\config\Uninstall.exe"
	!endif

	!ifdef OC_StartMenuPage
		!insertmacro OC_STATUS "Установка OpenConf | Создание программной группы и ярлыков..."
		!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
		; Ярлыки
    	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
		CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER\Документация"
		; для справки по ОпенКонфу и readme-файла
		CreateShortCut	"$SMPROGRAMS\$STARTMENU_FOLDER\Справка по OpenConf.lnk" \
						"$INSTDIR\OpenConf.chm"
		CreateShortCut	"$SMPROGRAMS\$STARTMENU_FOLDER\ReadMe.lnk" \
						"$INSTDIR\config\docs\readme.txt"
		CreateShortCut	"$SMPROGRAMS\$STARTMENU_FOLDER\История изменений OpenConf.lnk" \
						"$INSTDIR\config\docs\OpenConf.history.txt"
		CreateShortCut	"$SMPROGRAMS\$STARTMENU_FOLDER\Что нового.lnk" \
						"$INSTDIR\config\docs\WhatsNew.txt"
		!ifdef OC_Uninstall
			;для деинсталятора
			CreateShortCut	"$SMPROGRAMS\$STARTMENU_FOLDER\Удаление OpenConf.lnk" \
							"$INSTDIR\config\Uninstall.exe"
		!endif
    	!insertmacro MUI_STARTMENU_WRITE_END	
	!endif

SectionEnd ;; OpenConf

;===========================================================================
;Дополнительные секции

;; Плагины
!ifdef OC_Plugins
	!include "${OC_Plugins}"
!endif

;; Скрипты
!ifdef OC_Scripts
	!include "${OC_Scripts}"
!endif

;; Клавиатурные сокращения и прочий сервис
!ifdef OC_AddOns
	!include "${OC_AddOns}"
!endif

;; Дополнительная документация (скрипты, компоненты etc)
!ifdef OC_Docs
	!include "${OC_Docs}"
!endif

;; Дополнительные компоненты (для скриптов)
!ifdef OC_System
	!include "${OC_System}"
!endif

;===========================================================================
;Секция деинсталлятора
!ifdef OC_Uninstall
Section "Uninstall"

	!insertmacro OC_STATUS "Удаление OpenConf..."

	;; Проверяем, а ОпенКонф ли это? (на случай, если вдруг кто-то
	;; уже произвел деинсталляцию ОпенКонф'а вручную)
	GetDllVersion "$INSTDIR\config.dll" $R0 $R1
	IntOp $R2 $R0 / 0x00010000
	IntOp $R3 $R0 & 0x0000FFFF
	IntOp $R4 $R1 / 0x00010000
	IntOp $R5 $R1 & 0x0000FFFF	

	;; Если версия конфиг.длл ниже 7й, то считаем, что это ОпенКонф
	IntCmpU $R2 7 delete_files 0 delete_files

		!insertmacro OC_STATUS "Удаление OpenConf | Удаление файлов..."
		Delete "$INSTDIR\config.dll"
		Delete "$INSTDIR\config.tlb"

		!insertmacro OC_STATUS "Удаление OpenConf | Перемещение config.dll..."
		CopyFiles "$INSTDIR\config\config.dll" "$INSTDIR"
		Delete "$INSTDIR\config\config.dll"
  
	delete_files:

	!insertmacro OC_STATUS "Удаление OpenConf | Удаление файлов..."
	;; удалим остальные файлы (справку, ридми и сам деинсталлер)
	Delete "$INSTDIR\OpenConf.chm"
	Delete "$INSTDIR\config\docs\OpenConf.history.txt"
	Delete "$INSTDIR\config\docs\readme.txt"
	Delete "$INSTDIR\config\docs\OpenConf.lnk"
	Delete "$INSTDIR\config\docs\WhatsNew.txt"
	Delete "$INSTDIR\config\Uninstall.exe"

	;; если создавалась программная группа в меню Пуск, ее тоже надо удалить
	!ifdef OC_StartMenuPage		
		!insertmacro OC_STATUS "Удаление OpenConf | Удаление ярлыков и программной группы..."
		Delete	"$SMPROGRAMS\$STARTMENU_FOLDER\История изменений OpenConf.lnk"
		Delete	"$SMPROGRAMS\$STARTMENU_FOLDER\ReadMe.lnk"
		Delete	"$SMPROGRAMS\$STARTMENU_FOLDER\Справка по OpenConf.lnk"
		Delete	"$SMPROGRAMS\$STARTMENU_FOLDER\Удаление OpenConf.lnk"	
		Delete	"$SMPROGRAMS\$STARTMENU_FOLDER\Что нового.lnk"
		;; пытаемся удалить саму группу (если только она полностью пуста)
		RMDir "$SMPROGRAMS\$STARTMENU_FOLDER\Документация"
		RMDir "$SMPROGRAMS\$STARTMENU_FOLDER"		
	!endif

	;; пробуем удалить (если только папки полностью пусты)
	RMDir "$INSTDIR\config\docs"
	RMDir "$INSTDIR\config\scripts"
	RMDir "$INSTDIR\config"

	!insertmacro OC_STATUS "Удаление OpenConf | Удаление записи в реестре..."

	DeleteRegKey /ifempty HKLM "Software\1C\1Cv7\7.7\OpenConf\Install"
	DeleteRegKey /ifempty HKLM "Software\1C\1Cv7\7.7\OpenConf"

	!insertmacro OC_STATUS "Готово"

SectionEnd ;; Uninstall
!endif

;===========================================================================
;Обработчики событий

Function .onInit
	!insertmacro MUI_INSTALLOPTIONS_EXTRACT_AS "OpenConf.ini" "OpenConf.ini"
FunctionEnd

;===========================================================================

Function un.onInit
	;; Читаем информацию о пути установки из реестра
	;; FIXME нужна ли поддержка нескольких инсталляций?
	ReadRegStr $INSTDIR HKLM "Software\1C\1Cv7\7.7\OpenConf\Install" ""
	!ifdef OC_StartMenuPage	
		!insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
	!endif
FunctionEnd

;===========================================================================
;; Страница расширенного диалога выбора каталога установки

LangString SEC_DIR_TITLE ${LANG_RUSSIAN} "Выбор каталога установки OpenConf"
LangString SEC_DIR_SUBTITLE ${LANG_RUSSIAN} "Выберите инсталляцию 1С:Предприятия, в \
	которую следует установить OpenConf или укажите каталог установки вручную"

Function MyDirectoryPageEnter
	
	push $R0
	push $R1
	push $R2
	push $R3

	StrCpy $R0 ""
	StrCpy $R1 "0"
	StrCpy $R2 ""
	StrCpy $R3 ""

	;; Может быть, мы просто вернулись по кнопке "Назад" со следующей страницы?
	ReadINIStr $R0 "$PLUGINSDIR\OpenConf.ini" "Field 7" "State"
	StrCmp $R0 "" 0 init_page

	;; пытаемся прочитать данные об установке 1С из реестра
	read_next_key:
		EnumRegKey $R0 HKLM "SOFTWARE\1C\1Cv7\7.7" "$R1"
		IntOp $R1 $R1 + 1
		;; служебные разделы не должны попадать в список
		StrCmp $R0 "Components" read_next_key ;; служебный подраздел
		StrCmp $R0 "OpenConf" read_next_key ;; подраздел, создаваемый инсталлером
		StrCmp $R0 "" end_loop
		StrCmp $R2 "" 0 +4
		StrCpy $R2 $R0
		StrCpy $R3 $R0
		goto +2
		StrCpy $R2 "$R2|$R0"
		goto read_next_key
	end_loop:

	WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 4" "ListItems" $R2
         
	;; доступная инсталляция по умолчанию
	WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 4" "State" $R3

	;; выбор каталога установки вручную - значение по умолчанию
	ReadRegStr $R3 HKLM "SOFTWARE\1C\1Cv7\7.7\$R3" "1CPath"
	push $R3
	call GetParent
	pop $R3
	WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 7" "State" $R3

	pop $R3
	pop $R2
	pop $R1
	pop $R0

	init_page:	

	!insertmacro MUI_HEADER_TEXT "$(SEC_DIR_TITLE)" "$(SEC_DIR_SUBTITLE)"
	!insertmacro MUI_INSTALLOPTIONS_INITDIALOG "OpenConf.ini"

	pop $hwnd ; сохраняем хэндл окна, он нам будет нужен

	!insertmacro MUI_INSTALLOPTIONS_SHOW

FunctionEnd ;; MyDirectoryPageEnter

;===========================================================================

Function MyDirectoryPageLeave

	ReadINIStr $0 "$PLUGINSDIR\OpenConf.ini" "Settings" "State"
	StrCmp $0 0 onValidate
	StrCmp $0 1 onRadioButtonClick
	StrCmp $0 2 onRadioButtonClick
	StrCmp $0 4 onListChange
	StrCmp $0 7 onDirRequestChange
	Abort

	onListChange:
	Abort

	onRadioButtonClick:
		IntOp $0 $0 - 1
		GetDlgItem $1 $hwnd 1206 ; DirRequest  (1200 + Field 7 - 1)
		EnableWindow $1 $0
		GetDlgItem $1 $hwnd 1207 ; ... кнопка
		EnableWindow $1 $0
		IntOp $0 1 - $0
		GetDlgItem $1 $hwnd 1203 ; DropList  (1200 + Field 4 - 1 )
		EnableWindow $1 $0

		;; запоминаем состояние контролов на форме, чтобы возвращаясь со следующей странице
		;; по кнопке "Назад" все выглядело точно так же, как перед тем, как мы на нее перешли

		;; FIXME Пустые строки значимые! Используется абсолютная адресация строк (StrCmp и goto)!

		StrCmp $0 1 0 +8

		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 1" "State" "1"
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 2" "State" ""
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 4" "Flags" "NOTIFY"
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 7" "Flags" "DISABLED|NOTIFY"

		goto +6
	
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 1" "State" ""
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 2" "State" "1"
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 4" "Flags" "DISABLED"
		WriteINIStr "$PLUGINSDIR\OpenConf.ini" "Field 7" "Flags" "NOTIFY"

	  Abort

	onDirRequestChange:
	Abort

	onValidate:
		ReadINIStr $0 "$PLUGINSDIR\OpenConf.ini" "Field 1" "State"
		StrCmp $0 1 0 read_reqdir_field

		ReadINIStr $0 "$PLUGINSDIR\OpenConf.ini" "Field 4" "State"
		ReadRegStr $0 HKLM "SOFTWARE\1C\1Cv7\7.7\$0" "1CPath"
		push $0
		call GetParent
		pop $0
		goto validate_dir

	read_reqdir_field:
		ReadINIStr $0 "$PLUGINSDIR\OpenConf.ini" "Field 7" "State"

	validate_dir:
		;; проверяем, является ли заданная директория каталогом установки 1С
		IfFileExists "$0\1cv7*.exe" +3 0
		MessageBox MB_OK|MB_ICONEXCLAMATION "Указанный каталог не является каталогом установки 1С!"
		Abort

		;; каталог установки
		StrCpy $INSTDIR $0

		;; проверяем версию 1Ски
		;; по экзешнику проверить не можем, т.к. не знаем его точное имя,
		;; а по конфиг.длл - потому что возможно, что ОпенКонф уже установлен

		GetDllVersion "$0\basic.dll" $R0 $R1
		IntOp $R2 $R0 / 0x00010000
		IntOp $R3 $R0 & 0x0000FFFF
		IntOp $R4 $R1 / 0x00010000
		IntOp $R5 $R1 & 0x0000FFFF
		IntCmpU $R2 7  +1 +3 +3
		IntCmpU $R3 70 +1 +2 +1
		IntCmpU $R5 14 version_ok +1 version_ok
		MessageBox MB_OK|MB_ICONEXCLAMATION "Для работы ОпенКонфа требуется 1С 7.7 не ниже 14-го релиза (7.70.014)!"
		Abort

    version_ok:
		;; проверяем версию ОпенКонфа (а зодно и то, установлен ли он)
		GetDllVersion "$0\config.dll" $R0 $R1
		IntOp $R2 $R0 / 0x00010000
		IntOp $R3 $R0 & 0x0000FFFF
		IntOp $R4 $R1 / 0x00010000
		IntOp $R5 $R1 & 0x0000FFFF

		GetDLLVersionLocal "${OC_ConfigDir}\config.dll" $0 $1
		IntOp $2 $0 / 0x00010000
		IntOp $3 $0 & 0x0000FFFF
		IntOp $4 $1 / 0x00010000
		IntOp $5 $1 & 0x0000FFFF

		StrCpy $UPDATE "0"
		;; если версия конфиг.длл ниже 7й, то считаем, что это ОпенКонф
		IntCmpU $R2 "7" end 0 end
		;; ОпенКонф установлен, проверим, надо ли его обновлять
		IntCmpU $R2 $2 0 update update_warning
		IntCmpU $R3 $3 0 update update_warning
		IntCmpU $R4 $4 0 update update_warning
		IntCmpU $R5 $5 update_warning 0 update_warning

	update:
		StrCpy $UPDATE "1"
		goto end

	update_warning:
		;; FIXME ???
		MessageBox MB_OK|MB_ICONEXCLAMATION "Обнаружена установка ОпенКонфа такой же или более поздней версии!"	 
		!insertmacro UnselectSection "0" ;; отказываемся переустанавливать ядро

	end:

FunctionEnd ;; MyDirectoryPageLeave

;===========================================================================
;; EOF
