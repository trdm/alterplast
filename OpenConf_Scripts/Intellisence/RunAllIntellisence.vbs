' скрипт для объединения работы Интеллисенс.вбc и дотс.вбc
' для работы после точки

Dim gTimerIns		' Таймер включаем после вставки текста OnInsert

' Запускает скрипты из серии Intellisence с помощью Telepat_OnShowMemberList(Line, Col)
Sub ShowTooltip()
	Set Intellisence = Scripts("Intellisence")
	Intellisence.ShowTooltip(0)
End Sub

Function DelComment(line)
	DelComment = line
	PosKomment = InStr(1,line,"//")
	if PosKomment>0 Then
		if PosKomment = 1 Then
			DelComment = ""
		else
			DelComment	= Mid(line,1,PosKomment-1)
		End If
	End If
	DelComment = Trim(DelComment)
End Function

 'точка не должна работать в комментариях
Function VerifyIfInnerComment(Line, CommaPos)
	VerifyIfInnerComment = false

	PosKomment = InStr(1,line,"//")
	if PosKomment = 0 Then
		Exit Function
	end if

	if PosKomment < CommaPos Then
		VerifyIfInnerComment = true
	End If

End Function ' VerifyIfInnerComment

'[+]metaeditor 01.05.2006, перенёс из Intellisence.vbs
Sub Telepat_OnInsert(InsertType, InsertName, Text)
	Select Case InsertType
		Case 10
			If InsertName = "СоздатьОбъект" Then
				Text = "СоздатьОбъект(""!"");"
				gTimerIns = CfgTimer.SetTimer(1, True)
			end if
	End Select
End Sub
Sub Configurator_OnTimer(timerID)
	If timerID = gTimerIns Then
		CfgTimer.KillTimer gTimerIns
		Scripts("Intellisence").MethodsList
	End If
End Sub
'[+]_

function Telepat_OnShowMemberList(Line, Col)
	Telepat_OnShowMemberList=""
	ShowMethodList
End Function

' Макрос для использования связки Intell + Dots в "пассивном" режиме (Ctrl+I)
Sub ShowMethodList()

	set doc = CommonScripts.GetTextDocIfOpened(0)

	if doc is Nothing then 		Exit Sub
	If doc.LineCount = 0 Then	Exit Sub

	If (doc.SelStartLine<>0) And (doc.SelStartCol>0) Then

		if doc.Range(doc.SelStartLine,doc.SelStartCol-1,doc.SelEndLine,doc.SelEndCol) = "." then
			StrBezKomment = DelComment(doc.Range(doc.SelStartLine))
			if Len(StrBezKomment)=0 Then
				ShowTooltip()
				Exit Sub
			End IF

			 'точка не должна работать в комментариях
			if VerifyIfInnerComment(doc.Range(doc.SelStartLine), doc.SelStartCol-1) then
				Set wshShell = CreateObject("wScript.Shell")
				wshShell.sendKeys "{ESC}"
				Exit Sub
			end if

			'не нужно, упускаем некоторые возможности
			'не нужно - добавим контроль вызова скрипта Intellisence только если после точки ничего нет...
			'не нужно if Len( Trim(doc.Range(doc.SelStartLine,doc.SelStartCol))) = 0 Then
				Set Intellisence = Scripts("Intellisence")
				Ret = Intellisence.MethodsList()
				Select Case Ret
					Case -1: ' нажата ESC, пользователь передумал, значит и Dots вызывать смысла нет
						exit Sub
					Case  1: ' Все Ок, выбор из списка автодополнения произведен
						ShowTooltip()
						exit Sub
					Case Else: ' Интел не смог составить список для автодополнения, придется работать Dots'у
				End Select
			'не нужно End If
			if Doc.Name <> "Глобальный модуль" Then
				SuccessfulWork = Scripts("dots").IsSuccessfulWork()
				if SuccessfulWork Then
					ShowTooltip()
					exit Sub
				End If
			End If
		else
			exit Sub
		End If
	End If
End Sub

Private Function ShouldHandleTelepatOnShowMemberListEvent()
	ShouldHandleTelepatOnShowMemberListEvent = false

	Set fso = CreateObject("Scripting.FileSystemObject")
	intell_ini = fso.BuildPath(BinDir, "config\Intell\intell.ini")

	If Not fso.FileExists(intell_ini) Then Exit Function

	Set ini = CreateObject("OpenConf.RegistryIniFile")
	ini.SetConfig(Configurator)
	ini.UsedIniFile = true
	ini.IniFile = intell_ini

	If ini.Param(Null,"TELEPAT") = "ДА" Then
		ShouldHandleTelepatOnShowMemberListEvent = true
	End If
End Function

'
' Процедура инициализации скрипта
'
Private Sub Init()
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

	' ВНИМАНИЕ: следующие строки обязательно в таком порядке, иначе не получается !

	If ShouldHandleTelepatOnShowMemberListEvent() Then
		c.AddPluginToScript SelfScript, "Телепат", "Telepat", Telepat
	End If

	SelfScript.AddNamedItem "CommonScripts", c, False
End Sub

Init
