$NAME Выбор реквизитов Спр/Док
'/* Предоставляет выбор из список реквизитов конкретного справочника либо документа */
Dim CommonScripts
Dim doc			' Редактируемый документ

Dim CountOfDots	' Кол-во точек, введенных в текст. 
Dim StartCol	' Начальная колонка
Dim BeginOfField 'Позиция начала поля дока
Dim EndOfField 'Позиция конца поля дока
Dim FieldBeforeIns ' Диапазон символов, где появится поле дока
Dim TimerIns		' Таймер
 
Sub OpenList(param)
'	SendCommand 22500	' by ctrl-spc
	SendCommand 22510	' Открыть список вставки
End Sub

Sub Configurator_OnTimer(timerID)
    If timerID = TimerIns Then
	    Line1 = doc.SelStartLine
	    FieldAfterIns = doc.Range(Line1, BeginOfField, Line1, EndOfField)
	    If FieldBeforeIns <> FieldAfterIns Then
		    doc.Range(Line1, StartCol, Line1, BeginOfField) = ""
		    EndCol = StartCol+Len(FieldAfterIns)	' Позиция после поля, куда поставить курсор
		    doc.MoveCaret Line1, EndCol		' Поставим туда
			CfgTimer.KillTimer TimerIns		' И грохнем таймер
	    End If
    End If
End Sub

Sub Telepat_OnInsert(InsertType, InsertName, Text)
	If CountOfDots <> 0 Then
		If CountOfDots < 2 Then
			Text = Text + "."
			BeginOfField = BeginOfField + Len(Text)
			CountOfDots = 2
			OpenList(0) 
		Elseif CountOfDots = 2 Then
		    Line1 = doc.SelStartLine
			EndOfField = BeginOfField + Len(Text)
    		FieldBeforeIns = doc.Range(Line1, BeginOfField, Line1, EndOfField)
    		CountOfDots = 0
			'Теперь повиснем на таймере
			TimerIns = CfgTimer.SetTimer(10, False)
		End If
	End If
End Sub

Sub Attrs(MetaType)
    Set doc = CommonScripts.GetTextDoc(0)
    If doc Is Nothing Then Exit Sub

    Line1 = doc.SelStartLine
    Col1 = doc.SelStartCol
    Col2 = doc.LineLen(doc.SelStartLine)

    quStr=""""+MetaType+"."	' Строка запроса
	BeginOfField = doc.SelStartCol + Len(quStr)

    CurrentText = doc.Range(Line1, Col1, Line1, Col2)
	CurrentText=+quStr+CurrentText

    doc.Range(Line1, Col1, Line1, Col2) = CurrentText

    doc.MoveCaret Line1, Col1+Len(quStr)
	CountOfDots = 1
	StartCol = Col1

	OpenList(0)
End Sub ' CodeReplace

Sub SprField()
	Attrs("Справочник")
End Sub

Sub DocField()
	Attrs("Документ")
End Sub

' Инициализация скрипта. param - пустой параметр,
' чтобы не попадал в макросы
'
Sub Init(param)
    'Set CommonScripts = Scripts("Common")
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
	
    Set t = Plugins("Телепат")  ' Получаем плагин
    If Not t Is Nothing Then    ' Если "Телепат" загружен
        ' Привязываем скрипт к событиям плагина
        SelfScript.AddNamedItem "Telepat", t, False
        Telepat.DisableTemplateInRemString = 1      ' Отключить шаблоны. 1-в комментариях, 2-в строках
    Else
        ' Плагин не загружен. Выгрузим и скрипт
        Scripts.Unload SelfScript.Name
    End If

	c.SetConfig(Configurator)
	SelfScript.AddNamedItem "CommonScripts", c, False

End Sub

' При загрузке скрипта инициализируем его
Init 0