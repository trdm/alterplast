<<NSH;
;===========================================================================
;; Сценарий установки компонентов

Section "Компоненты (COM-dll, WSC)"
	
	SectionIn 1 2 3
	
	!insertmacro OC_STATUS "Установка компонентов | Копирование файлов..."
	
	;; Справка по компонентам
	SetOutPath "\$INSTDIR\\config\\docs"
	File "\${OC_ConfigDir}\\docs\\System.readme.txt"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "System.readme.txt" "Компоненты - Описание"

	SetOutPath "\$INSTDIR\\config\\system"

	;; Утилита fecho.exe Александра Орефкова
	File "\${OC_ConfigDir}\\system\\fecho.exe"

	;; COM-dll
@{[`$flst_bat $dir \\.dll\$`]}

	;; Скриптлеты (*.wsc)
@{[`$flst_bat $dir \\.wsc\$`]}

	;; Скрипты для регистрации и отмены регистрации компонентов
	File "$dir\\regfiles.js"
	File "$dir\\regall.bat"
	File "$dir\\unregall.bat"

	;; Dynamic Wrapper для ОС семейства Win9x
	SetOutPath "\$INSTDIR\\config\\system\\DynWin9x"
	File "\${OC_ConfigDir}\\system\\DynWin9x\\dynwrap.dll"
	File "\${OC_ConfigDir}\\system\\DynWin9x\\readme.txt"

	ClearErrors
	
	!insertmacro OC_STATUS "Установка компонентов | Регистрация компонентов..."
	
	;; Запускаем скрипт регистрации компонентов
	ExecWait 'wscript.exe //nologo "\$INSTDIR\\config\\system\\regfiles.js" /I /S /L'
	
	;; Если в результате выполнения команды код возврата != 0, то сообщаем об ошибке
	IfErrors 0 end
		MessageBox MB_OK|MB_ICONEXCLAMATION \\
			"Некоторые из компонентов (*.wsc- или *.dll-файлы)\$\\r\$\\n\\
			не были зарегестрированы корректно.\$\\r\$\\n\\
			Какие компоненты не были зарегистрированы можно узнать из файла\$\\r\$\\n\\
			\$INSTDIR\\config\\system\\regfiles.log.\$\\r\$\\n\\
			Подробную информацию об устранении этой ошибки можно найти в файле\$\\r\$\\n\\
			\$INSTDIR\\config\\docs\\System.readme.txt"
			
	end:
	
	!insertmacro OC_STATUS "Установка компонентов | Готово"
	
SectionEnd ;; "Компоненты"

;===========================================================================
;; Сценарий деинсталляции компонентов

Section "un.Компоненты (COM-dll, WSC)"

	!insertmacro OC_STATUS "Удаление компонентов | Отмена регистрации..."

	;; Запускаем скрипт отмены регистрации компонентов
	IfFileExists "\$INSTDIR\\config\\system\\regfiles.js" 0 Delete_Files
		ExecWait 'wscript.exe //nologo "\$INSTDIR\\config\\system\\regfiles.js" /U /S'
		
	Delete_Files:

	!insertmacro OC_STATUS "Удаление компонентов | Удаление файлов..."
	
	;; Удаляем служебные скрипты
	Delete "\$INSTDIR\\config\\system\\regfiles.log"
	Delete "\$INSTDIR\\config\\system\\regfiles.js"
	Delete "\$INSTDIR\\config\\system\\regall.bat"
	Delete "\$INSTDIR\\config\\system\\unregall.bat"

	;; Утилита fecho.exe Александра Орефкова
	Delete "\$INSTDIR\\config\\system\\fecho.exe"
	
	;; COM-dll
@{[`$flst_bat $dir \\.dll\$ Delete \$INSTDIR\\config\\system`]}

	;; Скриптлеты (*.wsc)
@{[`$flst_bat $dir \\.wsc\$ Delete \$INSTDIR\\config\\system`]}

	Delete "\$INSTDIR\\config\\system\\DynWin9x\\dynwrap.dll"
	Delete "\$INSTDIR\\config\\system\\DynWin9x\\readme.txt"
	RMDir "\$INSTDIR\\config\\system\\DynWin9x"

	;; Удаляем справку
	Delete "\$INSTDIR\\config\\docs\\System.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Компоненты - Описание"

	;; Если директория system пустая, удаляем ее
	RMDir "\$INSTDIR\\config\\system"	

	!insertmacro OC_STATUS "Удаление компонентов | Готово"
	
SectionEnd ;; un."Компоненты"
NSH