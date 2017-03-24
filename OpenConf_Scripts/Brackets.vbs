'Скрипт,  позволяющий  обрамлять выделенный текст в документе выбранной 
'конструкцией, а с помощью правок, сделанных MetaEditor-ом, даже выделять 
'текст не нужно.
'
'Версия: $Revision: 1.7 $
'Автор: trdm и MetaEditor (c) 2005 г.
'
'Пример: Есть текст:"Если ТраЛяЛя = ДругоеТраЛяЛя Тогда"
'        
'Вы хотите заключить переменную "ДругоеТраЛяЛя" в кавычки.
'Переместите курсор на ДругоеТраЛяЛя и вызовите скрипт, Вам предложат 
'список "обрамителей": "."; (.); СтрДлина(.); и прочей беды. Выберите "." 
'и Ваш текст будет в ковычках ).
'
'Примечание: 
'- точка в конструкциях является местом вставки текста выделенного фрагмента 
'или текущего слова, конструкция; 
'- "¦" является шаблоном который будет выделен после подстановки "Обрамителей".
'
'Список текущих "Обрамителей":
'""".""", 
'"(.)", 
'"СтрДлина(.)", 
'"ПустаяСтрока(.)", 
'"ПустоеЗначение(.) = 1",
'"?(.,¦,)",
'"СокрЛ(.)",
'"СокрП(.)", 
'"СокрЛП(.)", 
'"Лев(.,¦)",
'"Прав(.,¦)",
'"Сред(,¦)",
'"Найти(.,¦)",
'"СтрЗаменить(.,¦,)",
'"СтрЧислоВхождений(.,¦)",
'"СтрКоличествоСтрок(.)",
'"Врег(.)", 
'"Нрег(.)",
'"OemToAnsi(.)",
'"AnsiToOem(.)",
'"Симв(.)",
'"КодСимв(.)", 
'"ТипЗначенияСтр(.)"
'... разумеется вы всегда можете их отредактировать на свой вкус...

'31.10.2005
'  Скрипт порядком переписан, на предмет использования в качестве скобок
'  операторов Если_Тогда_КонецЕсли, Для_Цикл, Пока_Цикл...

Dim glBracket
Dim BracketsDict
Dim tTextOfKomment
Dim tTextOfKommentWithDade

NameDelimiters = " .,;:|#=+-*/%?<>\()[]{}!~@$^&'""" & vbTab
glBracket = """.""" 
tTextOfKomment = ""
tTextOfKommentWithDade = ""

'========================================================================================
Function GetCurrentWordBorders(Line, CursorPos, Delimiters)
	Dim Col1, Col2, TextLen
	
	GetCurrentWordBorders = ""
	TextLen = Len(Line)
	Col1 = CursorPos
	
	do while Col1 > 0
		If InStr(Delimiters, Mid(Line, Col1, 1)) > 0 Then
			Col1 = Col1 + 1
			Exit Do
		End If
		Col1 = Col1 - 1
	loop
	
	If Col1 = 0 Then Col1 = 1
	
	Col2 = Col1
	do while Col2 <= TextLen
		If InStr(Delimiters, Mid(Line, Col2, 1)) > 0 Then
			Col2 = Col2 - 1
			Exit Do
		End If
		Col2 = Col2 + 1
	loop
	
	If Col2 > TextLen Then Col2 = TextLen
	GetCurrentWordBorders = Array(Col1, Col2 - Col1 + 1)
	
End Function 'GetCurrentWordBorders 

'========================================================================================
Sub AddBracket()
    Dim flMultiLine
	
	set doc = CommonScripts.GetTextDocIfOpened(0)
	If doc Is Nothing Then Exit Sub
  
	flMultiLine = false
	locBracket = glBracket
	dY = 0
	if InStr(1,locBracket,"_") > 0 then
		for i = 1 to Len(locBracket)
			if Mid(locBracket,i,1) = "_" then dY = dY + 1
		next
		locBracket = Replace(locBracket,"_",vbCrLf)
		flMultiLine = true
	end if	
	ArrOfBracket = Split(locBracket,".")

	flCurrWord = false
	if (Doc.SelStartCol = Doc.SelEndCol) and (Doc.SelStartLine = Doc.SelEndLine) then flCurrWord = true
	
	If (Doc.SelStartLine <> Doc.SelEndLine) Or InStr(1,glBracket,"_") Then 'Если нам предстоит многострочный кооментарий.....
		if doc.SelEndCol <> 0 Then
			Doc.MoveCaret doc.SelStartLine, 0, doc.SelEndLine, doc.LineLen(doc.SelEndLine)
		Else
			Doc.MoveCaret doc.SelStartLine, 0, doc.SelEndLine-1, doc.LineLen(doc.SelEndLine-1)
		End If
	End If

	Komment = ""
	if ((tTextOfKomment = glBracket) Or (tTextOfKommentWithDade = glBracket))And Not flCurrWord Then
		Komment = InputBox("комментарий","Введите комментарий","")
		if Len(Komment)>0 And glBracket = tTextOfKommentWithDade Then 
			ArrOfBracket(0) = Replace(ArrOfBracket(0),"date", Trim(CStr(Now())))
			ArrOfBracket(1) = Replace(ArrOfBracket(1),"date", Trim(CStr(Now()))+ " ")
		elseIf Len(Komment) = 0 Then 'отказались от ввода.....
			' Просто убираем "date" из текста....
			ArrOfBracket(0) = Replace(ArrOfBracket(0),"date", "")
			ArrOfBracket(1) = Replace(ArrOfBracket(1),"date", " ")
		End If
		ArrOfBracketOne = Split(ArrOfBracket(0),vbcrlf) 
		ArrOfBracketOne(0) = ArrOfBracketOne(0) & Komment
		ArrOfBracket(0) = Join(ArrOfBracketOne,vbcrlf)
	End If
	
	if flCurrWord = true then	
		Borders = GetCurrentWordBorders (Doc.Range(Doc.SelStartLine), Doc.SelStartCol, NameDelimiters)	
		tText = doc.Range(Doc.SelStartLine, Borders(0)-1, Doc.SelEndLine,  Borders(0) + Borders(1) - 1)
		tText = ArrOfBracket(0) & tText & ArrOfBracket(1)
		Doc.Range(Doc.SelStartLine , Borders(0)-1 , Doc.SelEndLine , Borders(0) + Borders(1) - 1) = tText
	else
		tText = Doc.Range(Doc.SelStartLine , Doc.SelStartCol , Doc.SelEndLine , Doc.SelEndCol)
		tText = ArrOfBracket(0) & tText & ArrOfBracket(1) & Komment 
		Doc.Range(Doc.SelStartLine , Doc.SelStartCol , Doc.SelEndLine , Doc.SelEndCol) = tText
	end if
  
	if flCurrWord then	
		Doc.MoveCaret Doc.SelStartLine, Borders(0)-1 , Doc.SelEndLine + dY ,Borders(0) + Borders(1) + Len(ArrOfBracket(0)) + Len(ArrOfBracket(1)) - 1
	else
		Doc.MoveCaret Doc.SelStartLine , Doc.SelStartCol , Doc.SelEndLine + dY, Doc.SelEndCol+2+Len(ArrOfBracket(0))-1
	end if
	
	if flMultiLine then Doc.FormatSel
	
	for i = Doc.SelStartLine to Doc.SelEndLine
		PositionMoveKaret = InStr(1,Doc.Range(i),"¦")
		if PositionMoveKaret > 1 then
			Doc.Range(i,  PositionMoveKaret - 1, i, PositionMoveKaret) = ""
			Doc.MoveCaret i , PositionMoveKaret - 1
			exit for
		end if	
	next
	
End Sub 'AddBracket  

'========================================================================================
Sub ChoiseTypeBracket()	
	If CommonScripts.GetTextDocIfOpened(0) Is Nothing Then Exit Sub
	tText = CommonScripts.SelectValue(BracketsDict,"")  
	If Len(tText) > 0 Then
		glBracket = tText 
		AddBracket()
	End If  
End Sub  'ChoiseTypeBracket

'========================================================================================
Private Sub InitDict()
	Set BracketsDict = CreateObject("Scripting.Dictionary")
	
	BracketsDict.Add """ """, """."""
	BracketsDict.Add "( )","(.)"  
	BracketsDict.Add "[ ]","[.]"  
	BracketsDict.Add "Строка()", "Строка(.)"
	BracketsDict.Add "Дата()", "Дата(.)"
	BracketsDict.Add "Число()", "Число(.)"
	BracketsDict.Add "Сообщить()", "Сообщить(.)"
	BracketsDict.Add "СтрДлина()", "СтрДлина(.)" 
	BracketsDict.Add "ПустаяСтрока()","ПустаяСтрока(.)" 
	BracketsDict.Add "ПустоеЗначение() = 1", "ПустоеЗначение(.) = 1"
	BracketsDict.Add "?(,,)", "?(.,¦,)"
	BracketsDict.Add "СокрЛ()", "СокрЛ(.)"
	BracketsDict.Add "СокрП()","СокрП(.)" 
	BracketsDict.Add "СокрЛП()","СокрЛП(.)" 
	BracketsDict.Add "Лев()", "Лев(.,¦)"
	BracketsDict.Add "Прав()", "Прав(.,¦)"
	BracketsDict.Add "Сред()", "Сред(,¦)"
	BracketsDict.Add "Найти()", "Найти(.,¦)"
	BracketsDict.Add "СтрЗаменить()","СтрЗаменить(.,¦,)" 
	BracketsDict.Add "СтрЧислоВхождений()","СтрЧислоВхождений(.,¦)" 
	BracketsDict.Add "СтрКоличествоСтрок()", "СтрКоличествоСтрок(.)"
	BracketsDict.Add "Врег()", "Врег(.)"
	BracketsDict.Add "Нрег()", "Нрег(.)"
	BracketsDict.Add "OemToAnsi()", "OemToAnsi(.)"
	BracketsDict.Add "AnsiToOem()", "AnsiToOem(.)"
	BracketsDict.Add "Симв()", "Симв(.)"
	BracketsDict.Add "КодСимв()", "КодСимв(.)"
	BracketsDict.Add "ТипЗначенияСтр()", "ТипЗначенияСтр(.)"
	BracketsDict.Add "Окр()", "Окр(.)"
	BracketsDict.Add "Цел()","Цел(.)" 
	BracketsDict.Add "Мин()", "Мин(.,¦)"
	BracketsDict.Add "Макс()", "Макс(.,¦)"
	BracketsDict.Add "Шаблон()", "Шаблон(.)"
	BracketsDict.Add "Для - Цикл - КонецЦикла", "Для ¦= По  Цикл_._КонецЦикла;"
	BracketsDict.Add "Пока - Цикл - КонецЦикла", "Пока ¦ Цикл_._КонецЦикла;"
	BracketsDict.Add "Попытка - Исключение - КонецПопытки", "Попытка_._¦_Исключение_КонецПопытки;"
	BracketsDict.Add "Процедура - КонецПроцедуры", "//" & String(70,"=") & "_Процедура ¦()_._КонецПроцедуры; //"
	BracketsDict.Add "Функция - КонецФункции","Функция ¦()_._КонецФункции;"
	BracketsDict.Add "Комментарий","//¦_._//"
	BracketsDict.Add "Комментарий + дата","// date ¦_._//"
	BracketsDict.Add "Транзакция","НачатьТранзакцию();_._ЗафиксироватьТранзакцию();¦"
	BracketsDict.Add "Если - Тогда - Иначе - КонецЕсли", "Если ¦ Тогда_._Иначе__КонецЕсли;"
	BracketsDict.Add "Если * Тогда - КонецЕсли", "Если . ¦ Тогда__КонецЕсли;"
	BracketsDict.Add "Если - Тогда * КонецЕсли", "Если ¦ Тогда_._КонецЕсли;"
	
	tTextOfKomment = "//¦_._//"
	tTextOfKommentWithDade = "// date ¦_._//"
End Sub 'InitDict

'========================================================================================
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
	InitDict()
End Sub

'========================================================================================
Init ' При загрузке скрипта выполняем инициализацию
