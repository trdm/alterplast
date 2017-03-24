my $ints = dostowin(join "", @{[`$flst_bat $dir \\.ints\$ Delete \$INSTDIR\\config\\Intell`]});

<<NSH;
Section un."Intellisence.vbs"
	
	!insertmacro OC_STATUS "Удаление скриптов | Intellisence.vbs"
	
	Delete "\$INSTDIR\\config\\scripts\\Intellisence\\Intellisence.vbs"

	!insertmacro OC_USERFRIENDLY_DELETE "\$INSTDIR\\config\\Intell\\Intell.ini"
	
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Intellisence - Справка"
	Delete "\$INSTDIR\\config\\docs\\Intellisence.readme.txt"

	!insertmacro OC_STATUS "Удаление скриптов | Удаление *.ints файлов"

$ints

	RMDir "\$INSTDIR\\config\\Intell"

SectionEnd ;; un."Intellisence.vbs"
NSH