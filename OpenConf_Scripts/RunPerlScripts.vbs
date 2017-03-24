$NAME Запускатель Перловых скриптов

Sub CodeBeautifier()
	Set Doc = CommonScripts.GetTextDocIfOpened(0)
	If Doc Is Nothing Then Exit Sub
	Set Pos = CommonScripts.GetDocumentPosition(Doc)
	If Doc.SelStartLine <> Doc.SelEndLine or Doc.SelStartCol <> Doc.SelEndCol Then
		Text = Doc.Range(Doc.SelStartLine, Doc.SelStartCol, Doc.SelEndLine, Doc.SelEndCol)
		Text = Scripts("CodeBeautifier").Beautify_OpenConf(Text)
		Doc.Range(Doc.SelStartLine, Doc.SelStartCol, Doc.SelEndLine, Doc.SelEndCol) = Text
	Else
		Doc.Text = Scripts("CodeBeautifier").Beautify_OpenConf(Doc.Text)
	End If
	Pos.Restore
End Sub

'Вставка кода в текущем модуле
Sub CodeIns()
	Set Doc = CommonScripts.GetTextDocIfOpened(0)
	If Doc Is Nothing Then Exit Sub

	DoCodeIns(Doc)
End Sub

Sub DoCodeIns(Doc)
	mod_type = GetModuleType(Doc)
	If mod_type = "" Then Exit Sub

	names = split(Doc.Name, ".")
	Text = Scripts("CodeIns").CodeIns(Doc.Text, names(1), IBDir & mod_type & ".pl")

	If Text <> Doc.Text Then
		If Doc.IsOpen Then Set Pos = CommonScripts.GetDocumentPosition(Doc)
		Doc.Text = Text
		If Doc.IsOpen Then Pos.Restore
	End If
End Sub

'Вставка кода во всех справочниках
Sub CodeInsReferences()
	Set Refs = MetaData.TaskDef.Childs("Справочник")
	For i = 0 To Refs.Count - 1
		Message Refs(i).Name, mMetaData
		
		Set Doc = Documents("Справочник." & Refs(i).Name & ".Форма.Модуль")
		If not Doc Is Nothing Then DoCodeIns(Doc)
		
		Set Doc = Documents("Справочник." & Refs(i).Name & ".Форма группы.Модуль")
		If not Doc Is Nothing Then DoCodeIns(Doc)

		Set ListForms = Refs(i).Childs("Формасписка")
		For j = 0 To ListForms.Count - 1
			Set Doc = Documents("Справочник." & Refs(i).Name & ".ФормаСписка." & ListForms(j).Name & ".Модуль")
			If not Doc Is Nothing Then DoCodeIns(Doc)
		Next
	Next
End Sub

'Вставка кода во всех документах
Sub CodeInsDocuments()
	Set Docs = MetaData.TaskDef.Childs("Документ")
	For i = 0 To Docs.Count - 1
		Message Docs(i).Name, mMetaData

		Set Doc = Documents("Документ." & Docs(i).Name & ".Форма.Модуль")
		If not Doc Is Nothing Then DoCodeIns(Doc)
		Set Doc = Documents("Документ." & Docs(i).Name & ".Модуль Документа")
		If not Doc Is Nothing Then DoCodeIns(Doc)
	Next

	Set Journals = MetaData.TaskDef.Childs("Журнал")
	For i = 0 To Journals.Count - 1
		Message Journals(i).Name, mMetaData

		Set ListForms = Journals(i).Childs("Формасписка")
		For j = 0 To ListForms.Count - 1
			Set Doc = Documents("Журнал." & Journals(i).Name & ".Форма." & ListForms(j).Name & ".Модуль")
			If not Doc Is Nothing Then DoCodeIns(Doc)
		Next
	Next
End Sub

'Вставка кода во всех справочниках и документах
Sub CodeInsToAll()
	CodeInsReferences()
	CodeInsDocuments()
End Sub

Function GetModuleType(Doc)
	GetModuleType = ""
	names = split(lcase(Doc.Name), ".")
	
	last = Ubound(names)
	If names(0) = "документ" Then
		If names(last) = "модуль документа" Then
			GetModuleType = "МодульДокумента"
		Else
			GetModuleType = "ФормаДокумента"
		End If
	ElseIf names(0) = "справочник" Then
		If names(last-1) = "форма" Then
			GetModuleType = "ФормаЭлемента"
		ElseIf names(last-1) = "форма группы" Then
			GetModuleType = "ФормаГруппы"
		ElseIf names(last-2) = "формасписка" Then
			GetModuleType = "ФормаСписка"
		End If
	ElseIf names(0) = "журнал" Then
		GetModuleType = "ФормаЖурнала"
	ElseIf names(0) = "очет" Then
		GetModuleType = "Отчет"
	ElseIf names(0) = "обработка" Then
		GetModuleType = "Обработка"
	End If		
End Function
	
' Процедура инициализации скрипта
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
	SelfScript.AddNamedItem "CommonScripts", c, False
End Sub
 
Init ' При загрузке скрипта выполняем инициализацию
