$NAME Телепат
' (c) Orefkov
' Скрипт, работающий в паре с плагином "Телепат"
' и обрабатывающий события от него
' разместите в bin\config\scripts

' Обработка события "Вставка шаблона".
' Возникает при вставке шаблона перед обработкой его 1С.
' Позволяет изменить текст замены, скорректировав его по ситуации.
' name - имя вставляемого шаблона.
' text - текст замены шаблона. Можно изменить его.
' cancel - флаг отмены. При установке а True вставка шаблона отменяется.
'
Sub Telepat_OnTemplate(Name, Text, Cancel)
    Select Case Name
        Case "Вставка даты"
            Text = "// " & Trim(CStr(Now()))
        Case "Заполнение дока"
            Cancel = FillDoc(Text)
        Case "Заполнение ТЧ"
            Cancel = FillTablePart(Text)
        Case "Заполнение регистра"
            Cancel = FillDocRegister(Text)
        Case "Новый элемент"
            Cancel = FillNewRefItem(Text)
        Case "Новая группа"
            Cancel = FillNewRefGroup(Text)
		Case "Из дока в док"
			Cancel=CopyDocFromDoc(Text)
    End Select
	Text = DatePatternReplace(Text)
End Sub

''-----------------------------------------------------------------------------
'' Обработчик события "Вызов меню шаблонов"
'' Позволяет динамически добавить команды в меню шаблонов.
'' Для этого должен вернуть строку-описатель добавляемых пунктов.
'' Каждый добавляемый пункт должен располагаться на отдельной строке.
'' Вложенные группы команд определяются по табуляции в начале строки.
'' После названия команды через символ | можно указать символы
'' d или D (от Disabled) - недоступный пункт
'' c или C (от Checked)  - пункт с "галочкой"
'' Далее через символ | можно указать идентификатор команды.
'' В этом случае в событие OnCustomMenu вместо названия пункта меню
'' будет передан этот идентификатор
'' Для создания разделителей укажите имя "-"
''
'Function Telepat_GetMenu()
'    Telepat_GetMenu = _
'        "Группа неких команд" & vbCrLf & _
'        vbTab & "Недоступная команда|d" & vbCrLf & _
'        vbTab & "Помеченный пункт|c|command1" & vbCrLf & _
'        vbTab & "Просто пункт меню| |command2" & vbCrLf & _
'        vbTab & "-" & vbCrLf & _
'        vbTab & "Еще команда" & vbCrLf & _
'        "Еще одна команда" & vbCrLf & _
'        "И еще одна" & vbCrLf & _
'        "-" & vbCrLf & _
'        "Недоступная помеченная|dc"
'End Function
'
'' Обработчик события OnCustomMenu.
'' Вызывается при выборе пользователем пункта меню,
'' добавленного в "GetMenu"
'' Cmd - название (или идентификатор) выбранного пункта меню.
''
'Sub Telepat_OnCustomMenu(Cmd)
'    Message Cmd, mNone
'End Sub
''-----------------------------------------------------------------------------

' Обработчик события вставки текста из списка завершения
' Позволяет изменить текст вставки.
' InsertType - тип вставляемого значения (пояснения ниже)
' InsertName - имя вставляемого значения (как оно пишется в списке завершения)
' Text - вставляемый текст
' Во вставляемом тексте местоположение знака "!" определяет размещение
' курсора после вставки. (работает корректно только для однострочных вставок)
' Если положение курсора не указано, то он устанавливается в конце текста.
' При вставке шаблона из списка завершений данный обработчик не вызывается.
' Для примера показано, как вместо И,ИЛИ,НЕ вставлять и,или,не
'
Sub Telepat_OnInsert(InsertType, InsertName, Text)
    Select Case InsertType
        Case 0          ' Экспортируемая переменная текущего модуля
        Case 1          ' Неэкспортируемая переменная текущего модуля
        Case 2          ' Экспортируемый метод текущего модуля
        Case 3          ' Неэкспортируемый метод текущего модуля
        Case 4          ' Реквизит диалога
        Case 5          ' Экспортируемая переменная глобального модуля
        Case 6          ' Неэкспортируемая переменная глобального модуля
        Case 7          ' Экспортируемый метод глобального модуля
        Case 8          ' Неэкспортируемый метод глобального модуля
        Case 9          ' Имя глобального атрибута (типа РазделительСтрок, Перечисление и тп)
        Case 10         ' Метод 1С
            ' Когда еще от Орефкова дождешься нормального СоздатьОбъект...
'            If InsertName = "СоздатьОбъект" Then Text = "СоздатьОбъект(""!"");"
        Case 11         ' Ключевое слово
'            If InStr(",ИЛИ,НЕ,", InsertName) > 0 Then Text = LCase(Text)
        Case 12         ' Локальная переменная
        Case 13         ' Атрибут метаданных текущего модуля
        Case 14         ' Предопределенные блоки
'        	If InsertName="Процедура_КонецПроцедуры" or InsertName="Функция_КонецФункции" Then
'        		ProcName=InputBox("Введите имя метода","Метод", "гл")
'        		Text=Replace(Text,"!(",ProcName & "(!") & vbTab & "// " & ProcName
'        	End If
        Case 15         ' Подстановка в строке
    End Select
End Sub

'функция ищет все вхождения строки  {ДАТА(#dd.mm.yyyy)} и заменяет их на текущую дату
Function DatePatternReplace(Text)
	Set CS = CreateObject("OpenConf.CommonServices")
	Set RE = New RegExp

	RE.Pattern = "{ДАТА#([^)]*)}"
	RE.Global = true

	DatePatternReplace = ""
	pos = 1
	Set Matches = RE.Execute(Text)
	For Each Match in Matches
		format = Match.SubMatches(0)
		DatePatternReplace = DatePatternReplace & Mid(Text, pos, Match.FirstIndex - pos + 1) & CS.FormatDate(Now(), format)
		pos = Match.FirstIndex + Match.Length + 1
	Next
	DatePatternReplace = DatePatternReplace & Mid(Text, pos)
End Function


' Шаблон Заполнения документа
Function FillDoc(Text)
    FillDoc = True      ' По умолчанию отменим вставку шаблона
    ' Позволим пользователю выбрать вид документа
    DocKind = ""
    Set MetaDoc = SelectMetaObj(DocKind, "Документ", "Укажите вид документа")
    If MetaDoc Is Nothing Then Exit Function    ' Документ не выбран

    ' Дадим пользователю выбрать, как назвать переменную
    docName = InputBox("Укажите имя переменной документа", "Заполнение документа", "док" & DocKind)
    If Len(docName) = 0 Then Exit Function
    FillDoc = False ' Не будем отменять вставку шаблона

    ' Подставим в шаблон выбранный вид документа
    Text = Replace(Text, "!selDoc!", DocKind)
    ' Подставим в шаблон выбранное имя переменной
    Text = Replace(Text, "!selVar!", docName)

    ' Теперь дополним шаблон динамической информацией
    Set Head = MetaDoc.Childs("РеквизитШапки")
    Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
    Set CommonRekv = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
    ' Сосчитаем количество строк в полученном шаблоне
    ' с учетом комментариев
    ReDim Lines(2 + CommonRekv.Count + Head.Count + Table.Count)

    docName = docName & "."

    iCurLine = 0        ' Индекс заполняемой строки
    ' Заполним общие реквизиты документа
    FillNamesLines "// Общие реквизиты", docName, CommonRekv, Lines, iCurLine
    ' Заполним реквизиты шапки
    FillNamesLines "// Реквизиты шапки", docName, Head, Lines, iCurLine
    ' Заполним реквизиты ТЧ
    FillNamesLines "// Реквизиты табличной части", docName, Table, Lines, iCurLine
    ' Объединим и вернем полученный текст
    Text = Text & Join(Lines, vbCrLf)
End Function

' Обработка шаблона заполнения ТЧ документа
' Формирует строки вида Приемник.ИмяРеквизитаТЧ = Источник.ИмяРеквизитаТЧ;
Function FillTablePart(Text)
    FillTablePart = True    ' По умолчанию отменим вставку шаблона
    ' Позволим пользователю выбрать вид документа
    DocKind = ""
    Set MetaDoc = SelectMetaObj(DocKind, "Документ", "Укажите вид документа")
    If MetaDoc Is Nothing Then Exit Function
    ' Получим MetaArray c описанием реквизитов ТЧ выбранного документа
    Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
    iCountLines = Table.Count
    If iCountLines = 0 Then
        MsgBox "У документа """ & DocKind & """ нет реквизитов ТЧ", , "Телепат"
        Exit Function
    End If

    ' Запросим у пользователя имя переменной источника
    selFrom = InputBox("Укажите, откуда копировать значения", "Заполнение ТЧ", "тз")
    If Len(selFrom) = 0 Then Exit Function
    ' Запросим у пользователя имя переменной приемника
    selTo = InputBox("Укажите, куда копировать значения", "Заполнение ТЧ", "док")
    If Len(selTo) = 0 Then Exit Function

    FillTablePart = False   ' Пользователь все выбрал. Разрешим вставку шаблона
    ' Подставим в шаблон имя переменной источника
    Text = Replace(Text, "!selFrom!", selFrom)
    ' Заполним динамическую часть шаблона реквизитами ТЧ
    ReDim Lines(iCountLines - 1)
    selTo = vbTab & selTo & "."
    iMaxNameLen = MaxNameLen(Table, 0) + Len(selTo)
    selFrom = " = " & selFrom & "."
    For i = 0 To iCountLines - 1
        Lines(i) = selTo & Table(i).Name
        Lines(i) = Lines(i) & Space(iMaxNameLen - Len(Lines(i))) & selFrom & Table(i).Name & ";"
    Next
    ' Подставим в шаблон сформированную динамическую часть
    Text = Replace(Text, "!dynamic_part!", Join(Lines, vbCrLf))
End Function

' Обрабока шаблона "Заполнения регистра"
Function FillDocRegister(Text)
    FillDocRegister = True  ' По умолчанию отменим вставку шаблона
    ' Позволим пользователю выбрать вид регистра
    RegName = ""
    Set MetaReg = SelectMetaObj(RegName, "Регистр", "Укажите вид регистра")
    If MetaReg Is Nothing Then Exit Function

    ' Проверим, откуда вызывается шаблон
    Set TextDoc = Windows.ActiveWnd.Document
    If TextDoc = docWorkBook Then Set TextDoc = TextDoc.Page(1)
    docVarName = ""
    If TextDoc.Kind <> "Transact" Then
        ' Если шаблон вызывается не из модуля документа,
        ' надо получить имя переменной документа
        docVarName = InputBox("Укажите имя переменной документа", "Заполнение регистров", "Конт")
        If Len(docVarName) > 0 Then docVarName = docVarName & "."
    End If
    ' Позволим пользователю выбрать имя переменной для обозначения регистра
    regVarName = InputBox("Укажите имя переменной регистра", "Заполнение регистров", "рег" & RegName)
    If Len(regVarName) = 0 Then Exit Function
    ' Пользователь все выбрал, разрешим вставку шаблона
    FillDocRegister = False
    ' Подставим в шаблон выбранную информацию
    Text = Replace(Text, "!regVar!", regVarName)
    Text = Replace(Text, "!docVar!", docVarName)
    Text = Replace(Text, "!regName!", RegName)
    ' Получим данные о регистре
    Set Fields = MetaReg.Childs("Измерение")
    Set Resurs = MetaReg.Childs("Ресурс")
    Set Rekv = MetaReg.Childs("Реквизит")

    ' Подготовим массив для строк
    ReDim Lines(2 + Fields.Count + Resurs.Count + Rekv.Count)

    regVarName = vbTab & regVarName & "."

    iCurLine = 0        ' Индекс заполняемой строки
    ' Заполним измерения
    FillNamesLines vbTab & "// Измерения регистра " & RegName, regVarName, Fields, Lines, iCurLine
    ' Заполним реквизиты
    FillNamesLines vbTab & "// Реквизиты регистра " & RegName, regVarName, Rekv, Lines, iCurLine
    ' Заполним ресурсы
    FillNamesLines vbTab & "// Ресурсы регистра " & RegName, regVarName, Resurs, Lines, iCurLine
    ' Подставим в шаблон динамическую часть
    Text = Replace(Text, "!dynamic_part!", Join(Lines, vbCrLf))
End Function

Function CopyDocFromDoc(Text)
	CopyDocFromDoc=True
	docKindTo=""
	Set MetaTo=SelectMetaObj(docKindTo,"Документ","Укажите вид заполняемого документа")
	if MetaTo Is Nothing Then Exit Function

	docKindFrom=""
	Set MetaFrom=SelectMetaObj(docKindFrom,"Документ","Укажите вид заполняющего документа")
	if MetaFrom Is Nothing Then Exit Function

	docTo="док" & docKindTo
	docFrom="док" & docKindFrom
	if docKindFrom=docKindTo Then
		docTo=docTo & "1"
		docFrom=docFrom & "2"
	End If
	docTo=InputBox("Переменная заполняемого дока","Из дока в док",docTo)
	if len(docTo)=0 then exit function
	docFrom=InputBox("Переменная заполняющего дока","Из дока в док",docFrom)
	if len(docFrom)=0 then exit function
	if docTo=docFrom Then
		MsgBox "Источник и приемник совпадают",,"Телепат"
		exit function
	end if

	CopyDocFromDoc=False
	Text=Replace(Text,"!docTo!",docTo)
	Text=Replace(Text,"!docFrom!",docFrom)
	docTo=docTo & "."
	docFrom=docFrom & "."
	' Заполним общие реквизиты
    Set CommonRekv = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
    FillHead=""
    ReDim Lines(CommonRekv.Count)
    Lines(0)="// Общие реквизиты документа"
    iMaxNameLen=0
    for i=0 to CommonRekv.Count-1
    	Name=CommonRekv(i).Name
    	Lines(i+1)=docTo & Name & "! = "  & docFrom & Name & ";"
    	if len(Name)>iMaxNameLen Then iMaxNameLen=Len(Name)
    next
    iMaxNameLen=iMaxNameLen+Len(docTo)+1
    for i=1 to CommonRekv.Count
    	line=Lines(i)
    	l=InStr(line,"!")
    	Lines(i)=Replace(line,"!",Space(iMaxNameLen-l))
    next
    FillHead=Join(Lines,vbCrLf) & vbCrLf
    ' Заполним совпадающие реквизиты шапки
    Set HeadTo = MetaTo.Childs("РеквизитШапки")
    Set HeadFrom = MetaFrom.Childs("РеквизитШапки")
    ReDim Lines(HeadTo.Count)
    Lines(0)="// Реквизиты шапки"
    iMaxNameLen=0
    On Error Resume Next
    for i=0 to HeadTo.Count-1
    	Name=HeadTo(i).Name
    	RightPart=""
    	if len(Name)>iMaxNameLen Then iMaxNameLen=Len(Name)
    	Lines(i+1)=docTo & Name & "! = "
    	Set rekv=Nothing
    	Set rekv=HeadFrom(CStr(Name))
    	if not rekv is nothing then
    		TypeTo=HeadTo(i).Props("Тип")
    		KindTo=HeadTo(i).Props("Вид")
    		TypeFrom=rekv.Props("Тип")
    		KindFrom=rekv.Props("Вид")
    		if TypeTo=TypeFrom and (KindFrom=KindTo or KindTo="") Then RightPart = docFrom & Name
    	End If
    	Lines(i+1)=Lines(i+1) & RightPart & ";"
	next
    iMaxNameLen=iMaxNameLen+Len(docTo)+1
    for i=1 to HeadTo.Count
    	line=Lines(i)
    	l=InStr(line,"!")
    	Lines(i)=Replace(line,"!",Space(iMaxNameLen-l))
    next
    Text=Replace(Text,"!fill_head!",FillHead & Join(Lines,vbCrLf))
    ' Заполним реквизиты тч
    Set TableTo = MetaTo.Childs("РеквизитТабличнойЧасти")
    Set TableFrom = MetaFrom.Childs("РеквизитТабличнойЧасти")
    docTo=vbTab & docTo
    ReDim Lines(TableTo.Count)
    Lines(0)=vbTab & "// Реквизиты табличной части"
    iMaxNameLen=0
    On Error Resume Next
    for i=0 to TableTo.Count-1
    	Name=TableTo(i).Name
    	RightPart=""
    	if len(Name)>iMaxNameLen Then iMaxNameLen=Len(Name)
    	Lines(i+1)=docTo & Name & "! = "
    	Set rekv=Nothing
    	Set rekv=TableFrom(CStr(Name))
    	if not rekv is nothing then
    		TypeTo=TableTo(i).Props("Тип")
    		KindTo=TableTo(i).Props("Вид")
    		TypeFrom=rekv.Props("Тип")
    		KindFrom=rekv.Props("Вид")
    		if TypeTo=TypeFrom and (KindFrom=KindTo or KindTo="") Then RightPart = docFrom & Name
    	End If
    	Lines(i+1)=Lines(i+1) & RightPart & ";"
	next
    iMaxNameLen=iMaxNameLen+Len(docTo)+1
    for i=1 to TableTo.Count
    	line=Lines(i)
    	l=InStr(line,"!")
    	Lines(i)=Replace(line,"!",Space(iMaxNameLen-l))
    next
    Text=Replace(Text,"!fill_table!",Join(Lines,vbCrLf))
End Function



' Заполнение шаблона "Новый элемент справочника"
Function FillNewRefItem(Text)
    FillNewRefItem = FillNewReference(Text, False)
End Function
' Заполнение шаблона "Новая группа справочника"
Function FillNewRefGroup(Text)
    FillNewRefGroup = FillNewReference(Text, True)
End Function

' Обработка шаблона "Новый элемент/группа справочника"
Function FillNewReference(Text, IsGroup)
    FillNewReference = True ' Отменяем вставку шаблона
    refKind = ""
    Set MetaRef = SelectMetaObj(refKind, "Справочник", "Укажите вид справочника")
    If MetaRef Is Nothing Then Exit Function
    LevelsCount = CLng(MetaRef.Props("КоличествоУровней"))
    If IsGroup = True And LevelsCount = 1 Then
        MsgBox "У справочника """ & refKind & """ не может быть групп!", , "Телепат"
        Exit Function
    End If
    varName = InputBox("Укажите имя переменной", "Новый элемент/группа", "спр" & refKind)
    If Len(varName) = 0 Then Exit Function
    FillNewReference = False    ' Разрешаем вставку шаблона

    Text = Replace(Text, "!varName!", varName)
    Text = Replace(Text, "!refKind!", refKind)
    varName = varName & "."

    ' Получим реквизиты справочника
    Set Fields = MetaRef.Childs("Реквизит")
    iMaxNameLen = 0
    iLineCount = Fields.Count
    ReDim Lines(iLineCount)
    Lines(0) = "// Реквизиты справочника"
    iCurLine = 1
    ' Выясним, надо ли добавить Код
    If CLng(MetaRef.Props("ДлинаКода")) > 0 Then
        iLineCount = iLineCount + 1
        ReDim Preserve Lines(iLineCount)
        Lines(1) = varName & "Код!"
        iCurLine = 2
        iMaxNameLen = 3
    End If
    ' Выясним, надо ли добавить Наименование
    If CLng(MetaRef.Props("ДлинаНаименования")) > 0 Then
        iLineCount = iLineCount + 1
        ReDim Preserve Lines(iLineCount)
        Lines(iCurLine) = varName & "Наименование!"
        iCurLine = iCurLine + 1
        iMaxNameLen = 12
    End If
    ' Заполним массив имен реквизитов с их комментариями
    HavePeriodicVal = False
    For i = 0 To Fields.Count - 1
        Set Rekv = Fields(i)
        UseIn = Rekv.Props("Использование")
        If (UseIn = "ДляГруппы" And IsGroup = False) _
            Or (UseIn = "ДляЭлемента" And IsGroup = True) Then
            iLineCount = iLineCount - 1
        Else
            If Rekv.Props("Периодический") = "1" Then HavePeriodicVal = True
            Name = Rekv.Name
            Lines(iCurLine) = varName & Name & "!"
            If Len(Name) > iMaxNameLen Then iMaxNameLen = Len(Name)
            Descr = Rekv.Descr
            If Len(Descr) > 0 And Descr <> Name Then
                Lines(iCurLine) = Lines(iCurLine) & " // " & Descr
            End If
            iCurLine = iCurLine + 1
        End If
    Next
    ' Дополним строки выравненными знаками =
    iMaxNameLen = iMaxNameLen + Len(varName) + 1
    ReDim Preserve Lines(iLineCount)
    For i = 1 To iLineCount
        Line = Lines(i)
        l = InStr(Line, "!")
        Lines(i) = Replace(Line, "!", Space(iMaxNameLen - l) & " = ;")
    Next
    Text = Replace(Text, "!dynamic_part!", Join(Lines, vbCrLf))

    ' Выясним, подчиненный ли справочник, имеет ли он перидиодич.
    ' реквизиты, сколько в нем уровней
    ParentPeriodic = ""
    If HavePeriodicVal = True Then ParentPeriodic = varName & _
        "ИспользоватьДату(ТекущаяДата());" & vbCrLf
    If Len(MetaRef.Props("Владелец")) > 0 Then ParentPeriodic = _
        ParentPeriodic & varName & "ИспользоватьВладельца(...); //" & MetaRef.Props("Владелец") & vbCrLf
    If LevelsCount > 1 Then ParentPeriodic = ParentPeriodic & _
        varName & "ИспользоватьРодителя(...);" & vbCrLf
    Text = Replace(Text, "!parent_periodic!", ParentPeriodic)
End Function

' Функция выбора средствами 1С различных видов метаданных
Function SelectMetaObj(objName, objType, Title)
    ' Для выбора воспользуемся методом телепата ConvertTemplate
    ' Данный метод отрабатывает обработку переданного
    ' текста шаблона стандартным 1С-методом
    ' и возвращает получившийся текст
    ' В данном случае формируется текст одной из управляющих
    ' функций шаблонов для выбора объекта метаданных,
    ' например <?"Укажите документ",Документ>
    objName = Telepat.ConvertTemplate("<?""" & Title & """," & objType & ">")
    If Len(objName) = 0 Then Set SelectMetaObj = Nothing: Exit Function
    ' Вернем описание метаданных для выбранного объекта
    Set SelectMetaObj = MetaData.TaskDef.Childs(CStr(objType))(CStr(objName))
End Function

' Функция определяет максимальную длину идентификатора
' в массиве метаданных. Используется для выравнивания знаков =
Function MaxNameLen(MetaArray, OldMaxLen)
    MaxNameLen = OldMaxLen
    For i = 0 To MetaArray.Count - 1
        If Len(MetaArray(i).Name) > MaxNameLen Then MaxNameLen = Len(MetaArray(i).Name)
    Next
End Function

' Функция заполняет массив строк, начиная указанной,
' строками вида
' LeftPart ИмяРеквизита = ; // Комментарий реквизита
' с выравниванием знаков =
Sub FillNamesLines(Title, LeftPart, MetaRekvArray, Lines(), iCurLine)
    iMaxNameLen = MaxNameLen(MetaRekvArray, 0) + Len(LeftPart)
    Lines(iCurLine) = Title
    iCurLine = iCurLine + 1
    For i = 0 To MetaRekvArray.Count - 1
        Line = LeftPart & MetaRekvArray(i).Name
        Line = Line & Space(iMaxNameLen - Len(Line)) & " = ;"
        Descr = MetaRekvArray(i).Descr
        If Len(Descr) > 0 Then
            If Descr <> MetaRekvArray(i).Name Then
                Line = Line & " // " & Descr
            End If
        End If
        Lines(iCurLine) = Line
        iCurLine = iCurLine + 1
    Next
End Sub

' Инициализация скрипта. param - пустой параметр,
' чтобы не попадал в макросы
'
Sub Init(param)
    Set t = Plugins("Телепат")  ' Получаем плагин
    If Not t Is Nothing Then    ' Если "Телепат" загружен
        ' Привязываем скрипт к событиям плагина
        SelfScript.AddNamedItem "Telepat", t, False
        ' Теперь плагин доступен как объект Telepat
        ' Настроим его параметры
        ' Раскомментируйте нужные строки
        'Telepat.Components = 1 + 2 + 4                  ' Используемые компоненты. 1-Торговля, 2 - Бухгалтерия, 4 - Расчет
        'Telepat.Language = 1 + 2                        ' Используемые языки. 1- Английский, 2 - Русский
        'Telepat.UseStdMethodDlg = False                 ' Использовать стандартный диалог "Методы модуля"
        'Telepat.NoOrderMethodDlg = False                ' Не сортировать список в диалоге "Методы модуля"
        Telepat.FilterMethodDlg = True                  ' Фильтровать список в диалоге "Методы модуля"
        Telepat.AutoParamInfo = False                   ' Автоактивация подсказки о параметрах
        Telepat.ParamInfoAddMethDescr = True            ' В подсказке о параметрах выводить описание метода
        Telepat.ParamInfoAddParamDescr = True           ' В подсказке о параметрах выводить описание параметра
        Telepat.AutoActiveCountSymbols = 2              ' Количество символов в идентификаторе для автозавершения
        Telepat.DisableTemplateInRemString = 1 + 2      ' Отключить шаблоны. 1-в комментариях, 2-в строках
        Telepat.AddTemplate = True                      ' Добавлять шаблоны в список завершений
    Else
        ' Плагин не загружен. Выгрузим и скрипт
        Scripts.Unload SelfScript.Name
    End If
End Sub

' При загрузке скрипта инициализируем его
Init 0