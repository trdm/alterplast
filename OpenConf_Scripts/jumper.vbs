'Попрыгунчик.....
'Примочка для быстрого перемещения по тексту модуля.
'Позволяет бегать по строкам текста модуля, переходя с одного статмента 
'(Если, КонецЕсли, процедура, КонецПроцедуры, IF) на другой вверх или вниз. 
'Статменты стоящие в тексте за другими словами (не статментами) игнорируются. Иначе говоря,
'при поиске статмента анализируется только первое слово в строке.
'
'Есть 3 варианта переходов;
'   1) Переход к предыдущему/следующему ключевому слову - методы GoUp, GoDown
'   2) Переход к предыдущему началу конструкции или к следующему окончанию конструкции - методы GoUp2, GoDown2
'   3) Переход к началу/завершению родительской конструкции. При этом, если при поиске родительской 
'      конструкции попадается начало/конец процедуры, то прыгаем туда.  Методы GoUp3, GoDown3
'
'Пример:
'<s1>Процедура Пример()
'	<s2>Если ... Тогда
'		.....
'		<s3>Если ... Тогда
'		<s4>КонецЕсли;
'		<s5>Пока ... Цикл
'			......
'			...<p1>...
'		<s6>КонецЦикла;
'		.....
'		<p2>
'	КонецЕсли
'КонецПроцедуры
'
'1) При движении вверх из точки <p1> последовательно попадаем в точки <s6>, <s5>, <s4>, <s3>, <s2>, <s1>
'2) При движении вверх из точки <p1> последовательно попадаем в точки <s5>, <s3>, <s2>, <s1>
'3) При движении вверх из точки <p1> последовательно попадаем в точки <s2>, <s1>.
'   При движении из точки <p1> последовательно попадаем в точки <s5>, <s2>, <s1>
'
'==================================================================================================
' Плод совместного творчества следующих товарищей:
'    Трошин Д. ака trdm, 2004
'    Артур Аюханов aka artbear
'    Алексей Диркс aka ADirks
'==================================================================================================


Dim CommonScripts
Dim ArrStatement, ArrStatementUp, ArrStatementDn
Dim OpenParents, CloseParents
Dim ProcStarts, ProcEnds

Sub Init(dummy)
	Set CommonScripts = Nothing
	On Error Resume Next
	Set CommonScripts = CreateObject("OpenConf.CommonServices")
	On Error GoTo 0
	If CommonScripts Is Nothing Then
		Message "Не могу создать объект OpenConf.CommonServices", mRedErr
		Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
		Scripts.UnLoad SelfScript.Name
	Else
		CommonScripts.SetConfig(Configurator)
	End If
	
	ArrStatement = Array("Процедура", "Procedure", "КонецПроцедуры", "EndProcedure", _
		"Функция", 	"Function", "КонецФункции", "EndFunction", _
		"Если", "If", "Тогда", "Then", "ИначеЕсли", "ElsIf", "Иначе", "Else", "КонецЕсли", "EndIf", _
		"Пока", "While", "Цикл", "Do", "КонецЦикла", "EndDo", "Для", "For", _
		"Попытка", "Try", "Исключение", "Except", "КонецПопытки", "EndTry")

	ArrStatementUp = Array("Процедура", "Procedure", _
		"Функция", 	"Function", "КонецФункции", "EndFunction", _
		"Если", "If", "Тогда", "Then", "ИначеЕсли", "ElsIf", _
		"Пока", "While", "Цикл", "Do", "Для", "For", _
		"Попытка", "Try")
	ArrStatementDn = Array("КонецПроцедуры", "EndProcedure", _
		"Функция", 	"Function", "КонецФункции", "EndFunction", _
		"ИначеЕсли", "ElsIf", "Иначе", "Else", "КонецЕсли", "EndIf", _
		"КонецЦикла", "EndDo", _
		"КонецПопытки", "EndTry")

	OpenParents = Array("Процедура", "Procedure", _
		"Функция", 	"Function", _
		"Если", "If", _
		"Пока", "While", "Цикл", "Do", "Для", "For", _
		"Попытка", "Try")
	CloseParents = Array("КонецПроцедуры", "EndProcedure", _
		"КонецФункции", "EndFunction", _
		"КонецЕсли", "EndIf", _
		"КонецЦикла", "EndDo", _
		"КонецПопытки", "EndTry")
		
	ProcStarts = Array("Процедура", "Procedure", "Функция", "Function")
	ProcEnds = Array("КонецПроцедуры", "EndProcedure", "КонецФункции", "EndFunction")
End Sub

Init(0)
'=======================================================================


Sub GoUp()
	GoToStatement ArrStatement, -1, false
End Sub

Sub GoDown()
	GoToStatement ArrStatement, 1, false
End Sub

Sub GoUp2()
	GoToStatement ArrStatementUp, -1, false
End Sub

Sub GoDown2()
	GoToStatement ArrStatementDn, 1, false
End Sub

Sub GoUp3()
	GoToStatement ArrStatement, -1, true
End Sub

Sub GoDown3()
	GoToStatement ArrStatement, 1, true
End Sub

Sub Jump(nLine, nCol)
	TelepatJump = true
	Err.Clear
	On Error Resume Next
	Set Telepat = Plugins("Телепат")  ' Получаем плагин
	Telepat.Jump nLine, nCol, nLine, nCol, ModuleName
	If Err.Number <> 0 Then TelepatJump = false
	On Error Goto 0
	
	If Not TelepatJump Then
		Set Doc = CommonScripts.GetTextDocIfOpened()
		Doc.MoveCaret nLine, nCol
	End If
End Sub

Sub GoToStatement(ArrStatement, nDirection, bToOuterStatement)
    Set Doc = CommonScripts.GetTextDocIfOpened()
	If Doc Is Nothing Then Exit Sub
	If Doc.LineCount  = 0 Then Exit Sub

	nDirection = nDirection / Abs(nDirection)
	StartLine = Doc.SelStartLine + nDirection
	If nDirection < 0 Then
		EndLine = 1
	Else
		EndLine = Doc.LineCount
	End If
	Level = 0
	
	For nLine = StartLine To EndLine Step nDirection
		If bToOuterStatement Then
			Delta = AdjustLevel(Doc, nLine)
			Level = Level + Delta
			If Delta <> 0 Then
				FirstWord = GetFirstWord(Doc, nLine)
				'На границах процедур/функций будем останавливаться независимо от уровня
				If WordInArray(ProcStarts, FirstWord) OR WordInArray(ProcEnds,   FirstWord) Then
					'Doc.MoveCaret nLine, 0
					Jump nLine, 0
					Exit Sub
				End If
			End If

			If LevelHasChanged(nDirection, Level) Then
				Pos = CheckPlaceStatement(Doc, ArrStatement, nLine)
				'Doc.MoveCaret nLine, Pos-1
				Jump nLine, Pos-1
				Exit Sub
			End If
			
		Else
			Pos = CheckPlaceStatement(Doc, ArrStatement, nLine)
			IF Pos >= 0 Then
				'Doc.MoveCaret nLine, Pos-1
				Jump nLine, Pos-1
				Exit Sub
			End If
		End If
	Next
End Sub

'анализирует ключевое слово, и возвращает изменение уровня вложенности
Function AdjustLevel(Doc, nLine)
	AdjustLevel = 0
	Pos = CheckPlaceStatement(Doc, CloseParents, nLine)
	IF Pos >= 0 Then
		AdjustLevel = - 1
	Else
		Pos = CheckPlaceStatement(Doc, OpenParents, nLine)
		If Pos >= 0 Then
		AdjustLevel =  1
		End If
	End If
End Function

'В зависимости от направления движения определяет, достигнут ли нужный уровень вложенности
Function LevelHasChanged(nDirection, Level)
	LevelHasChanged = false
	If nDirection < 0 Then
		If Level > 0 Then
			LevelHasChanged = true
		End If
	Else
		If Level < 0 Then
			LevelHasChanged = true
		End If
	End If
End Function

'Определяет, является ли первое слово в строке ключевым, и возвращает 
'начальную позицию этого слова
'Если слово не ключевое, то возвращает -1
Function CheckPlaceStatement(Doc, ArrStatement, nLine)
	CheckPlaceStatement = -1

	FirstWord = GetFirstWord(Doc, nLine)
	If FirstWord = "" Then Exit Function

	sLine = Doc.Range(nLine)
	sLine = UCase(sLine)
	
	If WordInArray(ArrStatement, FirstWord) Then
		CheckPlaceStatement = InStr(1, sLine, FirstWord)
		Exit Function
	End IF	
End Function

'выделяет первое слово в указанной строке
Function GetFirstWord(Doc, nLine)
	GetFirstWord = ""

	sLine = Doc.Range(nLine)
	sLine = UCase(sLine)
	sLine = Replace(sLine, vbTab," ")
	sLine = Replace(sLine, ";"," ")
	sLine = Trim(sLine)
	ArrWord = Split(sLine)
	
	If UBound(ArrWord) >= 0 Then GetFirstWord = Trim(ArrWord(0))
End Function

'проверяет, есть ли слово в массиве
Function WordInArray(Arr, Word)
	WordInArray = false
	For i = 0 To UBound(Arr) Step 1
		If UCase(Arr(i)) = Word Then
			WordInArray = true
			Exit Function
		End IF	
	Next
End Function