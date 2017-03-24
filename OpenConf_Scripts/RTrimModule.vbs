'$ENGINE {B54F3741-5B07-11CF-A4B0-00AA004A55E8}

'Author: Valera e-mail: wash_ai@mail333.com
'Скрипт для удаления пробелов в концах строк программных модулей
'Кроме того из нескольких идущих подряд пустых строк остается только одна
'Позволяет несколько облегчить MD-шник и внешние обработки
'Добавлено переформатирование текущего открытого модуля

Dim NumOfStrings

'Обрезание лишних пробелов в строках выбранного документа
Function TrimThisDoc(doc, mode)
	Dim TextStrings
	TextStrings = Split(doc.Text, Chr(13) & Chr(10))
	s1 = 0
	s2 = 0
	x = UBound(TextStrings, 1)
	For i = 0 To x
		NumOfStrings = NumOfStrings + 1
		T = TextStrings(i)
		s1 = s1 + Len(T)
		'If mode = "All" Then
			' по идее здесь должен быть код удаляющий повторяющиеся пробелы+табуляции
            ' т.е. типа этого:
            '
            'prevLen = Len(T) + 2
            'Do While prevLen <> Len(T)
            ' prevLen = Len(T)
            ' T = Replace(T, Chr(32) & Chr(32), Chr(32))
            ' T = Replace(T, Chr(9) & Chr(9), Chr(9))
            ' T = Replace(T, Chr(32) & Chr(9), Chr(9))
            ' T = Replace(T, Chr(9) & Chr(32), Chr(9))
            'Loop
            '
            ' Это срабатывает неплохо, НО!!!! если у вас используется где-нибудь в коде присвоение переменной
            ' строки из нескольких пробелов, то, увы, теперь будет присваиваться только один
            ' Так что здесь нужен другой код, который бы отслеживал появление кавычек и исключал всё, что м/у ними
            ' Пока мне это писать влом - может позже, а может и кто другой сообразит...
        'End If
        If (mode = "Right") Or (mode = "All") Then
            flag = 0
            Do While (Len(T) > 0) And (flag = 0)
                If flag = 0 Then
                	If (Right(T, 1) = Chr(9)) Or (Right(T, 1) = Chr(32)) Then
                        T = Left(T, Len(T) - 1)
                    Else
                        flag = 1
                    End If
                End If
            Loop
            T = RTrim(T)
        End If
        If (mode = "Left") Or (mode = "All") Then
            flag = 0
            Do While (Len(T) > 0) And (flag = 0)
                If flag = 0 Then
                    If (Left(T, 1) = Chr(9)) Or (Left(T, 1) = Chr(32)) Then
                        T = Right(T, Len(T) - 1)
                    Else
                        flag = 1
                    End If
                End If
            Loop
            T = LTrim(T)
        End If
        s2 = s2 + Len(T)
        TextStrings(i) = T
    Next
    doc.Text = Join(TextStrings, Chr(13) & Chr(10))
    prevLen = Len(doc.Text) + 2
    Do While prevLen <> Len(doc.Text)
        prevLen = Len(doc.Text)
        doc.Text = Replace(doc.Text, Chr(13) & Chr(10) & Chr(13) & Chr(10), Chr(13) & Chr(10))
    Loop
    TrimThisDoc = s1 - s2
End Function

'Переформатирование открытого модуля
Sub ReFormatCurrentWnd()
    NumOfStrings = 0
	
	Set d = CommonScripts.GetTextDocIfOpened(false) ' не активировать закладку модуля
	If d is Nothing Then
		Exit Sub
	End If	
	
	docName = d.Name
	s0 = TrimThisDoc(d, "All")
	d.MoveCaret 0, 0, d.LineCount	
    d.FormatSel
    'Message "Обработано строк: " & NumOfStrings, mInformation
End Sub

'Удаление пробелов в текущем открытом модуле
Sub RTrimCurrentWnd()
    NumOfStrings = 0
	
	Set d = CommonScripts.GetTextDocIfOpened(false) ' не активировать закладку модуля
	If d is Nothing Then
		Exit Sub
	End If	
    
	s0 = TrimThisDoc(d, "Right")
    docName = d.Name
	
    Message "Обработано строк: " & NumOfStrings, mInformation
    If s0 = 0 Then
        Message "Не найдено лишних пробелов в " & docName, mInformation
    Else
        Message "В модуле '" & docName & "' удалены лишние пробелы размер уменьшился на " & s0, mInformation
    End If
End Sub

'Удаление пробелов в Глобальном модуле, в Документах, а также во встроенных Отчетах и Обработках
Sub RTrimGlobalModuleDocReportsAndCalcVars()
    NumOfStrings = 0
    s0 = 0
    Set doc = Documents("ГлобальныйМодуль.Модуль")
    s0 = s0 + TrimThisDoc(doc, "Right")
    Set refs = MetaData.TaskDef.Childs("Документ")
    For i = 0 To refs.Count - 1
        Message refs(i).Name, mInformation
        Set doc = Documents("Документ." & refs(i).Name & ".Форма.Модуль")
        s0 = s0 + TrimThisDoc(doc, "Right")
        Set doc = Documents("Документ." & refs(i).Name & ".Модуль Документа.Модуль")
        s0 = s0 + TrimThisDoc(doc, "Right")
    Next
    Set refs = MetaData.TaskDef.Childs("Отчет")
    For i = 0 To refs.Count - 1
        Set doc = Documents("Отчет." & refs(i).Name & ".Форма.Модуль")
        s0 = s0 + TrimThisDoc(doc, "Right")
    Next
    Set refs = MetaData.TaskDef.Childs("Обработка")
    For i = 0 To refs.Count - 1
        Set doc = Documents("Обработка." & refs(i).Name & ".Форма.Модуль")
        s0 = s0 + TrimThisDoc(doc, "Right")
    Next
    Message "Обработано строк: " & NumOfStrings, mInformation
    Message "Всего удалено " & s0 & " символов", mInformation
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

