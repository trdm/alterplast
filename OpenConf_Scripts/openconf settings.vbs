' -----------------------------------------------------------------------------
'		Тимошин Вячеслав aka slavka
'		e-mail: webmastert@mail.ru
'		ICQ:286-688-594
'       Версия: $Revision: 1.1 $
' -----------------------------------------------------------------------------
'Скрипт для переноса хоткеев Опенконфа /вернее макросов/
'Для работы скрипта необходимо:
'	1-мой скрипт WorkWithReestr.vbs
'	2-Svcsvc.Service
'	3-OpenConf.CommonServices
'Макрос UnloadOpenconfSettings - выгружает настройки в файл, местоположение и имя которого
'можно указать /по умолчанию прелагается BinDir & "Config\openconf_settings.set"/.
'Макрос LoadOpenconfSettings - загружает настройки из файла. При загрузке если скрипт указанный 
'в файле не находится, то его хоткеи не переносятся
'Благодарность за помощь при разработке скрипта MetaEditor, a13x, artbear.
' -----------------------------------------------------------------------------

Dim reg
Dim srv
Dim CommonScripts
Dim kolscr

Sub UnloadOpenconfSettings()
path = BinDir & "Config\"
stop
	kuda = srv.SelectFile(True,path+"openconf_settings.set","Все файлы|*")
	If kuda = "" Then
		Exit Sub
	End If
	If (CommonScripts.fso.FileExists(kuda)) Then
      If MsgBox ("Файл существует!!! Перезаписать?",vbOKCancel,"Manage Settings script")  = 2 Then
		Exit Sub
      End If
    End If	
   	Set f = CommonScripts.fso.CreateTextFile(kuda, true)
   	Dim Vals()
   	GetSubKey "HKCU", "Software\1C\1Cv7\7.7\OpenConf\HotKeys", Names1
   	i=0
	Do
	If InStr(Names1, ";") <> 0 Then
		pos = InStr(Names1, ";")
		rasdelname = left(Names1, pos-1)
		status rasdelname
			i=i+1
			ReDim Preserve Vals(i)
			Vals(i) = rasdelname
		Names1 = Mid(Names1, pos+1)
	Else 
		Exit Do
	End If			
	Loop
	k = i
	i = 0
	Do While i <>  k
		i=i+1
		Name = ""
		Names = Vals(i)
		GetSubKey "HKCU", "Software\1C\1Cv7\7.7\OpenConf\HotKeys" & "\" & Names, Name
		Do
		If InStr(Name, ";")<> 0 Then
			If Name <> ";"  Then
				k=k+1
				pos = InStr(Name, ";")
				rasdelname = left(Name, pos-1)
				ReDim Preserve Vals(k)
				status rasdelname
				Vals(k) = Names & "\" & rasdelname
				Name = Mid(Name, pos+1)
			Else
				Exit Do	
			End If
		Else
			Exit Do
		End If	
		Loop
	Loop
	For Each s In Vals
	Name = ""
		'следующие 5 строчек в принципе никакой роли не играют, просто у меня не возвращалось значение пареметра
		'если в разделе находился только один параметр без имени
		'/например HKEY_CURRENT_USER\Software\1C\1Cv7\7.7\<<IBName>>\AddTemplate\(по умолчанию REG_DWORD 1)
		If s = "" Then
			st = "Software\1C\1Cv7\7.7\OpenConf\HotKeys" & s & "tt"
		Else
			st = "Software\1C\1Cv7\7.7\OpenConf\HotKeys" & s & "\tt"
		End If		
		b = reg.RegWrite("HKCU\" & st,1, "REG_DWORD")
		b = reg.RegDelete("HKCU\" & st)
		b = reg.EnumParamValues("HKCU", "Software\1C\1Cv7\7.7\OpenConf\HotKeys" & s, Name)
		If Name <> ",;" Then
		status s
			f.WriteLine(s & "|" & Name)
		End If	
	Next
	Status "Complite"
	Message "Выгрузка завершена.", mInformation
End Sub   	

Sub LoadOpenconfSettings()
	path1 = BinDir & "Config\openconf_settings.set"
	path = srv.SelectFile(false,path1,"Файлы настроек (*.set)|*.set|Все файлы|*",false)
	If path <> "" Then
		Set f = CommonScripts.fso.OpenTextFile(path,1, True)
	Else
		Exit Sub
	End If
	Do While f.AtEndOfStream <> True
		str = f.readline
		If InStr(str,"IsScript=1")<>0 Then
		   'скрипт
		   vid = 1 '1-скрипт, 2-плагин
		   ScrExist = GetScriptName(str)
			If ScrExist = true Then
				'stop
				WriteReg str,vid	
			Else
				'message "Скрипт " 
			End If
		Else
		   'плагинs пока не проверяю
			'Set plg = CommonScripts.GetPluginByName(left(str,instr(str,"|")-1)&".dll")
'			message left(str,instr(str,"|")-1),mbNone
			'If Not plg Is Nothing Then
			'	message "ok!!"
			'Else	
			'	message "Плагин "+left(str,instr(str,"|")-1)+" не найден!",mrederr
			'End If	
		End If 
	Loop
	message "Перенос завершен.",mBlueTriangle
	message "Всего обработано: " & kolscr & " записей",mInformation		
	status ""
End Sub

Function GetScriptName(str)
'получим имя скрипта и проверим есть ли он
	GetScriptName = false
	posstart = InStr(str,"modul=")+6
	posend = InStr(posstart,str,",")
    ScriptName = Mid(str,posstart,posend-posstart)
	Set scrip = CommonScripts.GetScriptByName(ScriptName)
		status ScriptName
		kolscr=kolscr+1
		If Not scrip Is Nothing Then
			Message "Скрипт " & ScriptName & " ok", mbNone
			GetScriptName = true
		Else
			Message "Скрипт " & ScriptName & " не найден. Записи в реeстр не перенесены.", mrederr
			GetScriptName = false
		End If
End Function

Function GetSubKey(root, path, Names1)
b = reg.EnumSubKey(root, path, Names1)
End Function

Sub WriteReg(str,vid)
	podrasdel1 = left(str,instr(str,"\"))
	str = replace(str,podrasdel1,"")
	podrasdel2 = left(str,instr(str,"|")-1)
	str = replace(str,podrasdel2&"|","")
	If vid= 1 Then
		s1 = "HKCU\Software\1C\1Cv7\7.7\OpenConf\HotKeys" & podrasdel1 &  podrasdel2
	Else
		s1 = "HKCU\Software\1C\1Cv7\7.7\OpenConf\HotKeys"
	End If
	Do While instr(str,";") <> 0 
		p1 = instr(str,";")
		'выделим запись и разберем ее
		str1 =left(str,p1) 
		par1 = left(str,instr(str1,"=")-1) 'имя
		sc1 = replace(str1,par1+"=","")
		par2 = left(sc1,instr(sc1,",")-1)   'значение
		sc1 = replace(sc1,par2+",","")
		par3 = replace(sc1,";","")          'тип
		str=replace(str,str1,"")             
		reg.RegWrite s1+"\"+par1, par2, par3
	Loop	
End Sub

Sub Init(param)
  Set reg = Scripts("WorkWithReestr")
  Set srv = CreateObject("Svcsvc.Service")
  Set CommonScripts = CreateObject("OpenConf.CommonServices")
  CommonScripts.SetConfig(Configurator)
  kolscr = 0
End Sub

Init 0