$NAME Разбор командной строки Конфигуратора
'========================================================================================
'	1) Запуск 1С-Предприятия из Конфигуратора в монопольном и разделенном режимах,
'		скрипт умеет определять состояние базы, 
' 		и при невозможности работы в одном режиме предлагает другой режим
'	Пример. 
'	- Если пользователь запускает 1С в разделенном режиме, а необходима переиндексация,
'		скрипт предлагает запуститься в монопольном режиме.
'	- Если пользователь запускает 1С в монопольном режиме, а в базе уже кто-то работает,
'		скрипт предлагает запуститься в разделенном режиме.
'      
'		макросы: RunInExclusiveMode и RunInSharedMode
'
'	Рекомендую эти макросы посадить на F11 и  F10
'
'	2) Загрузка файлов при запуске Конфигуратора из командной строки  
'                                                           
'	Примечание. для правильной работы скрипта требуется установленный скритлет "1S.StatusIB.wsc"
'
'	Copyright (c) 2004 Артур Аюханов")
'	e-mail: artbear@bashnet.ru 
'	ICQ: 265666057
'
'	Версия: $Revision: 1.15 $ 
'
'========================================================================================

' следующие две строки для вывода отладочных сообщений
' эти две строки можно и удалить
Dim DebugFlag 'обязательно глобальная переменная
'DebugFlag = True 'Разрешаю вывод отладочных сообщений

	sParamName = "/Open:"
  'RunFileName = BinDir+"Config\CmdLine\RunParseCmdLine.vbs"
  
Sub RunEnterpise(bInShared)

  Dim wshShell
  Dim fso 'as FileSystemObject

  set wshShell = CommonScripts.WSH
 
	CmdLine = CommandLine + " "
    Debug "1C CommandLine", CommandLine
	
	CmdLine = RegReplaceText(CmdLine, "\s+config\s+", " enterprise ")
 	if InStr(UCase(CmdLine), " ENTERPRISE ") = 0 then
		CmdLine = CmdLine + " enterprise "
	end if
	
	strExclusive = "/m"
	CmdLine = RegReplaceText(CmdLine, "\s+"+strExclusive+"\s+", " ")
	
	' добавлю путь к базе
	IBDirPath = CommonScripts.IBDir
	
 	if InStr(UCase(CmdLine), " /D") = 0 then
		CmdLine = CmdLine + " /D"+""""+IBDirPath+""""
	end if
           
	' добавлю имя пользователя, если позволяет версия Опенконф
	on Error resume next
	if InStr(UCase(CmdLine), " /N") = 0 then
		sUserName = AppProps(appUserName)
		CmdLine = CmdLine + " /N" + sUserName
	end if
	on Error goto 0

    Debug "CmdLine", CmdLine
	
	' теперь нужно проверить состояние базы
	bNeedRun = true
	bNeedSharedMode = bInShared
	
	set ob = CreateObject("1S.IBState")
	iIBState = ob.IBState(IBDirPath)

		'-1	-->>	"Выполнен некорректный выход из 1С, нужна переиндексация"
		'0	-->>	"В базе никого нет"
		'1	-->>	"База работает в монопольном режиме"
		'2	-->>	"В базе работает один пользователь в разделенном режиме"
		'3	-->>	"1С работает в разделенном режиме"

	if (iIBState = -1) and bInShared then '
		' задать вопрос о необходимости переиндексации в монопольном режиме
		' TODO ? запускать переиндексацию в пакетном режиме, а потом заходить в разделенном ?
		strAnswer = "ВНИМАНИЕ: " & ob.IBStateToString(iIBState) & vbCrLf & vbCrLf & _
				"Вы хотите зайти в монопольный режим с переиндексацией ?"
		if MsgBox(strAnswer, vbOKCancel, SelfScript.Name) = vbOK then
			bNeedSharedMode = false
		else
			bNeedRun = false
		end if
	end if

	if (iIBState = 1)  then
		' сообщить о невозможности работы
		strAnswer = "ВНИМАНИЕ: " & ob.IBStateToString(iIBState)
		MsgBox(strAnswer)
		bNeedRun = false
	end if

	if (iIBState = 2) and not bInShared then
		' задать вопрос о невозможности работы в монопольном режиме и спросить о разделенном режиме
		strAnswer = "ВНИМАНИЕ: " & ob.IBStateToString(iIBState) & vbCrLf & _
				"Работа в монопольном режиме невозможна" & vbCrLf & vbCrLf & _
				"Вы хотите зайти в разделенном режиме ?"
		if MsgBox(strAnswer, vbOKCancel, SelfScript.Name) = vbOK then
			bNeedSharedMode = true
		else
			bNeedRun = false
		end if
	end if

	if (iIBState = 3) and not bInShared then
		' задать вопрос о невозможности работы в монопольном режиме и спросить о разделенном режиме
		strAnswer = "ВНИМАНИЕ: " & ob.IBStateToString(iIBState) & vbCrLf & _
				"Работа в монопольном режиме невозможна" & vbCrLf & vbCrLf & _
				"Вы хотите зайти в разделенном режиме ?"
		if MsgBox(strAnswer, vbOKCancel, SelfScript.Name) = vbOK then
			bNeedSharedMode = true
		else
			bNeedRun = false
		end if
	end if     
	
  	if not bNeedSharedMode then
		CmdLine = RegReplaceText(CmdLine, "\s+enterprise\s+", " enterprise " & strExclusive & " ")
	end if
    Debug "CmdLine", CmdLine
	
	if bNeedRun then
  		wshShell.Run CmdLine, 3, false
  	end if

End Sub ' RunEnterpise

' запустить 1С не в монопольном режиме для текущего пользователя
' рекомендую посадить на F10
Sub RunInSharedMode()
	RunEnterpise true
End Sub ' RunInSharedMode

' запустить 1С в монопольном режиме для текущего пользователя
' рекомендую посадить на F11
Sub RunInExclusiveMode()
	RunEnterpise false
End Sub ' RunInExclusiveMode

' Открыть внешний файл, указанный в командной строке Конфигуратора через "/Open: "
Sub OpenExternalFileFromCommandLine()

	CmdLine = CommandLine
	Debug "CmdLine", CmdLine

	set CmdLineDict = CommonScripts.CommandLineToDictionary(CmdLine)
	arguments = CmdLineDict.Items
	For i = 0 To CmdLineDict.Count -1
		s = arguments(i)
		if InStr(LCase(s), LCase(sParamName)) = 1 then
			ErtName = Mid(s, Len(sParamName)+1)
			if CommonScripts.fso.FileExists(ErtName) then
				Debug "ErtName", ErtName

				Set ert = Documents.Open(ErtName)
				If ert Is Nothing Then      ' Открыли неудачно
					'If ert = docWorkBook Then   ' Проверим, что открыли именно форму
					CommonScripts.Error("Не удалось открыть файл "+ErtName)
				End If
			end if
		end if
	Next

End Sub ' OpenExternalFileFromCommandLine

Sub Configurator_AllPluginsInit()
  OpenExternalFileFromCommandLine()
End Sub

Dim regEx ' Create variables.
Set regEx = New RegExp            ' Create regular expression.

Function RegReplaceText(srcStr, patrn, replStr)
  regEx.Pattern = patrn            ' Set pattern.
  regEx.IgnoreCase = True            ' Make case insensitive.
  RegReplaceText = regEx.Replace(srcStr, replStr)   ' Make replacement.
End Function ' RegReplaceText

Sub Debug(ByVal title, ByVal msg)
on error resume next
  DebugFlag = DebugFlag
  if err.Number<>0 then
    err.Clear()
    on error goto 0
    Exit Sub
  end if
on error goto 0

	CommonScripts.SetQuietMode(not DebugFlag)
	CommonScripts.Debug title, msg
End Sub'Debug

Sub EnableDebugMessages()
	DebugFlag = true
End Sub

Sub DisableDebugMessages()
	DebugFlag = false
End Sub

'
' Процедура инициализации скрипта
'
Sub Init(dummy) ' Фиктивный параметр, чтобы процедура не попадала в макросы
    Set c = Nothing
    On Error Resume Next
    Set c = CreateObject("OpenConf.CommonServices")
    On Error GoTo 0
    If c Is Nothing Then
        Message "Не могу создать объект OpenConf.CommonServices", mRedErr
        Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    c.SetConfig(Configurator)
	SelfScript.AddNamedItem "CommonScripts", c, False
End Sub
 
Init 0 ' При загрузке скрипта выполняем инициализацию
