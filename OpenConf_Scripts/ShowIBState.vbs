'	Показ статуса информационной базы 1С.
'	Используются следующие варианты:
'	1)	"В базе никого нет"
'	2)	"База работает или в монопольном режиме или один пользователь работает в разделенном режиме"
'	3)	"Выполнен некорректный выход из 1С, нужна переиндексация"
'	4)	"1С работает в разделенном режиме"
'	
'	Примечание. 
'	1) скрипт работает и как обычный скрипт WSH, и как специальный скрипт для OpenConf
'	2) скриплет работает только для DBF-баз ( пока !!)
'	3) пока не удается разделить монопольный режим или работу одного пользователя в разделенном режиме
'   4) скрипт основан на работе скриптлета "1S.StatusIB.wsc" ( объект "1S.IBState" )
'	
'	Copyright (c) Артур Аюханов aka artbear, 2004
'	E-mail:		<artbear@bashnet.ru>
'	ICQ UIN:	265666057
'

strIBDir = "W:\Дополнительные конфигурации\Разработка 1С++\"

on Error resume next
temp = WScript.Version
iErrNumber = Err.Number
on Error goto 0
if iErrNumber = 0 then
	ShowIBStatus()
	WScript.Quit(0)
end if

strIBDir = IBDir ' openconf

Sub ShowIBStatus()
	set ob = CreateObject("1S.IBState")
	Echo "Статус базы - "+ob.StringIBState(strIBDir)
End Sub

Sub Echo(str)

	on Error resume next
	temp = WScript.Version
	iErrNumber = Err.Number
	on Error goto 0
	if iErrNumber = 0 then
		WScript.Echo CStr(str) ' обычный скрипт
	else
		Message CStr(str) ' openconf-скрипт
	end if
End Sub
