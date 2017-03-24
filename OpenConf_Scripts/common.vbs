$NAME Common
' -----------------------------------------------------------------------------
'         Набор общих процедур для использования в скриптах
'
'         Мой e-mail: artbear@bashnet.ru
'         Мой ICQ: 265666057
' -----------------------------------------------------------------------------
'
' 	В своих скриптах можно использовать вот так:
'		CommonScripts = scripts("common")
'		set doc = CommonScripts.GetTextDoc(0)
'
' -----------------------------------------------------------------------------

Dim QuietMode
Dim LastErrorStr

QuietMode = false
LastErrorStr = ""

Function SetQuietMode(bNewMode)
	SetQuietMode = QuietMode
	QuietMode = bNewMode
End Function

Sub MyMsgBox(Text)
	If QuietMode Then
		LastErrorStr = Text
	Else
		MsgBox Text
	End If
End Sub

Function GetLastError(Param)
	GetLastError = LastErrorStr
End Function

Sub Echo(text)
  Message text, mNone
End Sub ' Echo

' -----------------------------------------------------------------------------
Sub Error(text)
  Message text, mError
End Sub ' Error

' -----------------------------------------------------------------------------
Sub EchoError(text)
  Message text, mRedErr
End Sub ' EchoError

' -----------------------------------------------------------------------------
' Получить модуль текущего окна (или текста), даже если закладка модуля не активна
' В случае успеха возвращает объект TextDoc
' В случае ошибки возвращает Nothing
' param  пустой параметр, чтобы не попадал в макросы
'
Function GetTextDoc(param)

  set GetTextDoc = Nothing

  If Windows.ActiveWnd Is Nothing Then
     MyMsgBox "Нет активного окна"
     Exit Function
  End If
  Set doc = Windows.ActiveWnd.Document
  If doc=docWorkBook Then Set doc=doc.Page(1)

  If doc<>docText Then
     MyMsgBox "Окно не текстовое"
     Exit Function
  End If

  set GetTextDoc = doc
End Function ' GetTextDoc

' -----------------------------------------------------------------------------
' Получить открытый модуль (формы или текста)
' В случае успеха возвращает объект TextDoc
' В случае ошибки возвращает Nothing
' param  пустой параметр, чтобы не попадал в макросы
' bActivateTextModule = 1 -> если не стоим в окне текста, переходим в него
' bActivateTextModule = 0 -> если не стоим в окне текста, выдается ошибка
'
' Удобно использовать в методах навигации по тексту (пример - Переходы по модулю.vbs)
'
Function GetTextDocIfOpened(bActivateTextModule)

  set GetTextDocIfOpened = Nothing

  If Windows.ActiveWnd Is Nothing Then
     MyMsgBox "Нет активного окна"
     Exit Function
  End If
  Set doc = Windows.ActiveWnd.Document
  If doc=docWorkBook Then
	 	if (bActivateTextModule = 0) and (Doc.ActivePage <> 1) then
    	MyMsgBox "Окно не текстовое"
    	Exit Function
    elseif (bActivateTextModule = 1) then
  		Doc.ActivePage = 1
  	end if
	 	Set doc=doc.Page(1)
  end if
  If doc<>docText Then
     MyMsgBox "Окно не текстовое"
     Exit Function
  End If

  set GetTextDocIfOpened = doc
End Function ' GetTextDocIfOpened

' -----------------------------------------------------------------------------
' получить плагин по названию
' если плагин отсутствует, возвращается Nothing
'
' почему-то конструкция
'          set GetPlugin = Plugins(PluginName)
' не хочет работать,  если PluginName - обычная строковая переменная
' или плагина под таким названием не существует
'
Function GetPluginByName(PluginName)
  set GetPluginByName = Nothing

  err.Clear
  on error resume next
'  temp = Plugins(PluginName)
  temp = Plugins(CStr(PluginName))
  iErrNumber = Err.Number
  on error goto 0
'  if err.Number = 438 then ' такой плагин действительно существует
  if iErrNumber = 438 then ' такой плагин действительно существует
    err.Clear
    set GetPluginByName = Plugins(CStr(PluginName))
    Exit Function
  elseif iErrNumber = 0 then
    set GetPluginByName = Plugins(CStr(PluginName))
  end if
End Function ' GetPluginByName

' -----------------------------------------------------------------------------
' получить скрипт по названию
' если скрипт отсутствует, возвращается Nothing
'
' почему-то конструкция
'          set GetScript = Scripts(ScriptName)
' не хочет работать,  если ScriptName - обычная строковая переменная
' или скрипта под таким названием не существует
'
Function GetScriptByName(ScriptName)
  set GetScriptByName = Nothing

  on error resume next
  Err.Clear
  temp = Scripts(CStr(ScriptName))
  iErrNumber = err.Number
  on error goto 0
'  if err.Number = 438 then ' такой скрипт действительно существует
  if iErrNumber = 438 then ' такой скрипт действительно существует
    err.Clear
    set GetScriptByName = Scripts(CStr(ScriptName))
    Exit Function
  elseif iErrNumber = 0 then
    set GetScriptByName = Scripts(CStr(ScriptName))
  end if
End Function ' GetScriptByName

Function GetScriptIndexByName(ScriptName)
	set GetScriptIndexByName = nothing
  iScriptCount=Scripts.Count-1
  for i=0 to iScriptCount
    sScriptName = Scripts.Name(i)
	if Lcase(sScriptName) = LCase(trim(ScriptName)) then
		GetScriptIndexByName = i
		Exit Function
	end if
  Next
End Function ' GetScriptIndexByName

' Инициализация скрипта. param - пустой параметр,
' чтобы не попадал в макросы
'
' При загрузке скрипта инициализируем его
'Init SelfScript, "Мастер 1С++", "Wizard1CPP", Wizard1CPP
'
Sub InitScript(Script, ScriptName, PrefixForEvents, ByRef ScriptVariable)
  'set ScriptVariable1 = GetPluginByName(CStr(ScriptName))
  set ScriptVariable1 = GetScriptByName(CStr(ScriptName))
  if not ScriptVariable1 Is Nothing Then
      ' Привязываем скрипт к событиям плагина
      set ScriptVariable = ScriptVariable1

      on error resume next
      Err.Clear
      Script.AddNamedItem PrefixForEvents, ScriptVariable, False

      if iErrNumber = -2147024809 then ' Имя уже зарегистрировано
        Echo "ОШИБКА в скрипте " & Script.Name & ": " & Err.Description & vbCRLF _
          & "Необходимо использовать другой префикс для событий!"
        ' Выгрузим и скрипт
        Scripts.Unload Script.Name
        Err.Clear
      end if
  else
      ' Плагин не загружен. Выгрузим и скрипт
      Scripts.Unload Script.Name
  End If
End Sub ' InitScript

' проверить на соответствие шаблону (регулярному выражению)
' регистр символов не важен
  Dim regExTest
Function RegExpTest(ByVal patrn, ByVal strng)
  if IsEmpty(regExTest) then
    Set regExTest = New RegExp         ' Create regular expression.
  end if
  regExTest.Pattern = patrn         ' Set pattern.
  regExTest.IgnoreCase = true      ' disable case sensitivity.
  RegExpTest = regExTest.Test(strng)      ' Execute the search test.
End Function

' существует ли в указанном скрипте макрос с таким именем
Function MacrosExists(ScriptName, MacrosName)
  MacrosExists = false

  on error resume next
  Err.Clear
  Set e = CreateObject("Macrosenum.Enumerator")
  if Err.Number <> 0 then
  	Echo "Не удалось создать объект Macrosenum.Enumerator" & vbCRLF & _
  		"Зарегистрируйте соответствующий dll(ocx) файл"
  	Exit Function
  end if
  on error goto 0

  Set script = GetScriptByName(ScriptName)
  if script is Nothing then
    Exit Function
  end if

  arr=e.EnumMacros(script)                        ' Получение массива макросов объекта
  for j=0 to ubound(arr)
    if trim(LCase(arr(j))) = trim(LCase(MacrosName)) then
      MacrosExists = True
      Exit Function
    End if
  Next

End Function ' MacrosExists

' Выполнить указанный макрос
Sub CallMacros(sScriptName, sMacrosName)
'    On Error Resume Next
    Cmd = "Scripts(""" & sScriptName & """)." & "" & sMacrosName & ""
'stop
    Eval "" & Cmd & ""
'	iErrNumber = Err.Number
'    On Error GoTo 0
End Sub ' CallMacros

' выполнить программу
Sub RunCommand(ProgramPath, sArguments, bWait)
	set wshShell = createObject("WScript.Shell")
	CmdLine = ProgramPath & " " & sArguments

	on Error resume next
	wshShell.Run CmdLine, 1, bWait
	if Err.Number <> 0 then
		WshShell.Popup("ОШИБКА: не удалoсь выполнить программу" & _
			vbCRLF & vbCRLF & CmdLine & vbCRLF & vbCRLF & _
			"Возможно, указанный файл или не существует, или не находится на пути поиска (если указано только имя программы)")
		Err.Clear
	end if
	on Error goto 0
End Sub ' RunCommand

' выполнить программу и подождать ее завершения
Sub RunCommandAndWait(ProgramPath, sArguments)
	RunCommand ProgramPath, sArguments, true
End Sub ' RunCommandAndWait

' послать команду для Конфигуратора
' Ввиду того, что OpenConf.SendCommand делает PostMessage(WM_COMMAND,число,0)
' нужно вызвать принудительную обработку сообщений Windows
' пока это сделано через вызов любой маленькой проги (cscript.exe)
'
Sub SendCommand(iCommand)
	Configurator.SendCommand(iCommand)

	set wshShell = createObject("WScript.Shell")
	CmdLine = "cscript "
	wshShell.Run CmdLine, 0, true
End Sub ' SendCommand


' пример использования:
'----------------------
'		Set Pos = CommonScripts.GetDocumentPosition(Doc)
'		какой-то код, который меняет позицию курсора
'		Pos.Restore
'----------------------

Class TDocumentPosition
	Private Doc
	Private Line1
	Private Col1
	Private Line2
	Private Col2

	Public Sub Save(aDoc)
		If aDoc Is Nothing then
			Exit Sub
		End if
		set Doc = aDoc
		Line1 = Doc.SelStartLine
		Col1 = Doc.SelStartCol
		Line2 = Doc.SelEndLine
		Col2 = Doc.SelEndCol
	End Sub

	Public Sub Restore()
		If not Doc Is Nothing Then
			Doc.MoveCaret Line1, Col1, Line2, Col2
		end if
	End Sub
End Class ' TDocumentPosition

Function GetDocumentPosition(Doc)
	Set GetDocumentPosition = new TDocumentPosition
	GetDocumentPosition.Save(Doc)
End Function ' GetDocumentPosition

Function SelectFileForRead(sFileName, sFilesFilter)
	SelectFileForRead = ""

    Set srv = CreateObject("Svcsvc.Service")
    'sFileName = srv.SelectFile(False, sFileName, "Текстовые файлы|*.txt|Все файлы|*", False)
    sDstFileName = srv.SelectFile(False, sFileName, sFilesFilter, False)
    If sDstFileName = "" Then
        Exit Function
    End If

    Set fs = CreateObject("Scripting.FileSystemObject")
    If Not fs.FileExists(sDstFileName) Then
        Error "Указанный файл не существует"
        Exit Function
    End If

	SelectFileForRead = sDstFileName
End Function ' SelectFileForRead

' ADirks
'Функция служит для выбора строки из набора вариантов. Для отображения окна списка используется ActiveX-сервер
' SelectValue.dll от IAm или svcsvc.dll от Александра Орефкова. Если ни один сервер не зарегистрирован, то
' выдаётся сообщение.
' - ValueList - строка, содержащая варианты для выбора. Варианты разделены символом vbCrLf или символом ','
' - Caption   - строка, содержащая заголовок окна выбора
' Возвращаемое значение: выбранная строка. Если ничего не выбрано (пользователь нажал Esc), то возвращается пустая строка
Function SelectValue(ValueList, Caption)
	SelectValue = ""
	Set SelObj = Nothing
	SelType = ""

	On Error Resume Next
	'Попробуем создать SvcSvc.Service, потом SelectValue.SelectVal, и если не получилось, то скажем доблестному программисту, что он отстал от жизни
	Set SelObj = CreateObject("SvcSvc.Service")
	If Not SelObj Is Nothing Then
		SelType = "SvcSvc"
	Else
		Set SelObj = CreateObject("SelectValue.SelectVal")
		If Not SelObj Is Nothing Then SelType = "SelectValue"
	End If
	On Error Goto 0

	If SelObj Is Nothing Then
		Message "Не установлен (или не зарегистрирован) ActiveX сервер для отображения списка выбора!", mExclamation
		Message "	Сервер SvcSvc.dll можно найти по адресу http://openconf.itland.ru/beta/", mInformation
		Message "	Сервер SelectValue.dll можно найти в наборе 'Intellisence for OpenConf' от IAm, по адресу http://1c.proclub.ru/modules/mydownloads/personal.php?cid=1002&lid=4229", mInformation
		Exit Function
	End If

	If InStr(ValueList, vbCrLf) = 0 Then
		ValueList = Replace(ValueList, ",", vbCrLf)
	End If
	If SelType = "SelectValue" Then
		SelectValue = SelObj.SelectPopUp(ValueList, Windows.ActiveWnd.HWnd, vbCrLf)
	Else
		SelectValue = -1
		On Error Resume Next
		SelectValue = SelObj.FilterValue(ValueList, 1, Caption)
		If SelectValue = -1 Then
			SelectValue = SelObj.SelectValue(ValueList, Caption, false)
		End If
		On Error Goto 0
	End If
End Function

' ADirks
'Функция добавляет к числу N нули слева до достижения ширины width
Private Function MyFormatNumber(N, width)
	MyFormatNumber = Trim(CStr(N))
	do while len(MyFormatNumber) < width
		MyFormatNumber = "0" & MyFormatNumber
	loop
End Function

' ADirks
'Простое форматирование даты
'В форматной строке можно задавать поля 'd', 'm', 'MMMM', 'y'
'	количество символов 'd', 'm' и 'y' может быть любым - соотв. часть даты будет
'	дополнена слева нулями до нужной длины
'	MMMM будет заменено на название месяца
Function FormatDate(aDate, Format)
	Dim format_d, format_m, format_y
	Dim d, m, y

	Set RE = New RegExp
	RE.IgnoreCase = true

	format_d = ""
	format_m = ""
	format_y = ""


	RE.Pattern = "[^d]*(d*)"
	RE.IgnoreCase = true
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_d = Matches(0).SubMatches(0)

	RE.Pattern = "[^m]*(m*)"
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_m = Matches(0).SubMatches(0)

	RE.Pattern = "[^y]*(y*)"
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_y = Matches(0).SubMatches(0)

	d = MyFormatNumber(DatePart("d", aDate), len(format_d))
	If format_m = "MMMM" Then
		m = MonthName(DatePart("m", aDate))
	Else
		m = MyFormatNumber(DatePart("m", aDate), len(format_m))
	End If
	y = MyFormatNumber(DatePart("yyyy", aDate), len(format_y))

	FormatDate = Format
	FormatDate = Replace(FormatDate, format_d, d)
	FormatDate = Replace(FormatDate, format_m, m)
	FormatDate = Replace(FormatDate, format_y, y)
End Function

' Orefkov
' Макрос переключает видимость различных панелей. Например,
' 	TogglePanel "Синтакс-Помощник"
' 	TogglePanel "Окно сообщений"
' 	TogglePanel "Список найденных вхождений"
' 	TogglePanel "Стандартная"
Sub TogglePanel(PanelName)
	Windows.PanelVisible(PanelName)=Not Windows.PanelVisible(PanelName)
End Sub

' Получить каталог ИБ в нормальном регистре (исправил баг Configurator.IBDir - все в маленьком регистре)
Function GetIBDir()

	set fso = CreateObject("Scripting.FileSystemObject")
 	set Folder = fso.GetFolder(IBDir)
 	GetIBDir = Folder.Path + "\"

End Function