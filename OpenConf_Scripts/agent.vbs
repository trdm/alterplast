$NAME Волшебник

' (c) Orefkov
' Попсовый пример использования
' SelfScript.AddNamedItem,
' SelfScript.Name,
' Configurator_OnIdle,
' Documents.EnumAllTypedObj

' Для работы должен быть установлен MS Agent
' По щелчку левой кнопкой выводит список последних редакируемых форм
' По щелчку левой кнопкой c Ctrl выводит список, что открыть
' По правой несколько "горячих" команд

Dim helper          ' Помощник
Dim AllObjects      ' Все открываемые объекты конфигурации
Dim LastX, LastY    ' Координаты для вывода меню
Dim RecentForms()   ' Массив с последними формами
Dim rcFormsCount    ' Количество элементов в массиве
Dim MaxRcForms      ' максимальное количество в списке последних форм
Dim SelectMode      ' Режим, в котором начата обработка меню

' Показать помощника
Sub ShowAgent()
'    helper.Show
'    helper.MoveTo 100, 100
End Sub

Sub Configurator_AllPluginsInit()
    ShowAgent
End Sub

' Вызывается OpenConfом при "простое", те в тот момент когда
' программа обработала все сообщения из своей очереди сообщений
Sub Configurator_OnIdle()
    ' Будем использовать это время для того, чтобы занести текущее
    ' активное окно в список, при этом,если оно уже есть в списке,
    ' надо переместить его наверх списка.
    ' Получим активное окно
    Set wnd = Windows.ActiveWnd
    If wnd Is Nothing Then Exit Sub
    ' Получим документ окна
    Set doc = wnd.Document
    ' Скрипт работает только с формами и модулями
    If doc = docUnknown Then Exit Sub
    ' Сформируем строку, описывающую документ
    docFull = CStr(doc.id) & "##"
    If doc.id < 0 Then
        ' Внешний файл
        docFull = docFull & doc.Path
    Else
        ' Объект конфигурации
        Name = doc.Name
        If doc.Kind = "GlobalData" Then Name = "Общие таблицы"
        docFull = docFull & Name & "##" & CStr(doc.Type) & "##" & CStr(doc.Kind) & "##" & doc.Path
    End If
    docFull = docFull & "##"
    ' Теперь поищем документ в списке последних
    For i = 0 To rcFormsCount - 1
        If InStr(RecentForms(i), docFull) = 1 Then
            For j = i + 1 To rcFormsCount - 1
                RecentForms(j - 1) = RecentForms(j)
            Next
            rcFormsCount = rcFormsCount - 1
            Exit For
        End If
    Next
    If rcFormsCount = MaxRcForms Then
        For i = 1 To rcFormCount - 1
            RecentForm(i - 1) = RecentForm(i)
        Next
        rcFormsCount = rcFormsCount - 1
    End If
    If doc = docWorkBook Then docFull = docFull & CStr(doc.ActivePage)
    RecentForms(rcFormsCount) = docFull
    rcFormsCount = rcFormsCount + 1
End Sub

' Инициализация агента
Function InitAgent(DummyParam)   ' Пустой параметр, чтобы не попадал в макросы
    InitAgent = False
    On Error Resume Next
    Set a = CreateObject("Agent.Control.2")
    If Err <> 0 Then MsgBox "Агент не установлен": Exit Function
    a.Connected = True
    ' Пытаемся загрузить персонаж Merlin
    a.Characters.Load "Merlin", "merlin.acs"
    If Err <> 0 Then
        Err.Clear
        ' Пытаемся загрузить персонаж по умолчанию
        a.Characters.Load "Merlin"
    End If
    If Err <> 0 Then MsgBox "Нет ни одного персонажа": Exit Function

    ' Добавляем созданный объект в пространство имен скрипта.
    ' Теперь можно будет в любом месте скрипта обратится к этому объекту
    ' по указанному имени и привязаться к его событиям (Аналог With Events в VB)
    ' Если вместо False указать True, то методы и свойства добавляемого объекта
    ' будут доступны напрямую, без указания его имени,
    ' как например, у объекта Configurator
    SelfScript.AddNamedItem "Agent", a, False

    Set helper = Agent.Characters("Merlin")
    helper.AutoPopupMenu = False
    InitAgent = True
End Function

' Обработчики событий от агента
' Вызывается по щелчку на агенте
Sub Agent_Click(id, btn, sh, x, y)
    Set cmd = helper.Commands
    cmd.RemoveAll
    helper.StopAll
    LastX = x
    LastY = y
    If btn = 1 Then
        If sh = 0 Then
            FillCommandsLeftClick cmd
        ElseIf sh = 2 Then
            FillCommandsLeftClickCtrl cmd
        End If
    ElseIf btn = 2 Then
        FillCommandsRightClick cmd
    End If
    cmd.Add "ShowHelp", "Обо мне...", , True, True
    helper.ShowPopupMenu x, y
End Sub

' Вызывается при выборе команды меню помощника
Sub Agent_Command(UserInput)
    PlayMagic = False
    If UserInput.Name = "ShowHelp" Then
        helper.Speak "Привет. Я твой помошник. Щелкни на мне левой кнопкой мыши, " & _
            "и я помогу открыть тебе последние редактируемые тобой формы и модули. " & _
            "Если при этом ты нажимаешь Ctrl, я помогу открыть тебе любую форму или модуль. " & _
            "По щелчку правой кнопкой мыши я смогу выполнить несколько полезных команд"
        Exit Sub
    End If
    Select Case SelectMode
        Case 1
            PlayMagic = OnSelectCmd1(UserInput)
        Case 2
            PlayMagic = OnSelectCmd2(UserInput)
        Case 3
            PlayMagic = OnSelectCmd3(UserInput)
    End Select
    If PlayMagic = True Then helper.Play "DoMagic1"
End Sub

Sub Agent_DragComplete(CharacterID, Button, Shift, x, y)
    helper.Speak "Туда, сюда, надоело!!!"
End Sub

' Заполнение списка команд при щелчке правой кнопкой
Sub FillCommandsRightClick(Commands)
    SelectMode = 1
    Names = Array("Conf", "GM", "CommonTables", "Synt", "Assist")
    captions = Array("Открыть Конфигурацию", "Глобальный модуль", "Общие таблицы", _
        "Полная синтакс-проверка", "Поиск в синтакс-помошнике")
    For i = 0 To 4
        Commands.Add Names(i), captions(i), , True, True
    Next
End Sub

' Обработка выбора команды при щелчке правой кнопкой
Function OnSelectCmd1(UserInput)
    OnSelectCmd1 = True
    Select Case UserInput.Name
        Case "Conf"
            SendCommand cmdOpenConfigWnd
        Case "Synt"
            SendCommand 33884   'FullSynt
        Case "Assist"
            SendCommand 33879   'Assist
        Case "GM"
            Documents("ГлобальныйМодуль").Open
        Case "CommonTables"
            Documents("Таблицы").Open
        Case Else
            OnSelectCmd1 = False
    End Select
End Function


Sub FillCommandsLeftClickCtrl(Commands)
    SelectMode = 2
    helper.Think "Что бы такого открыть?.."
    ' Получаем все открываемые объекты конфы
    ' Возвращает объект, в котором перечислены все формы и модули конфигурации
    ' Можно их перебрать напрямую, через Count
    ' For i=0 to AllObjects.Count-1
    '   Set doc=AllObjects(i)
    ' Но в этом случае объекты будут идти в том порядке,
    ' в котором они возвращаются самой 1С
    ' OpenConf позволяет перебрать объекты упорядоченно по
    ' типам объектов и алфавиту
    ' Для этого служат свойства
    ' ChildCount(Parent)        - Возвращает количество групп указанного родителя
    ' ChildsName(Parent,index)  - Возвращает имя дочерней группы с заданным номером
    ' ObjectCount(Parent)       - Возвращает количество объектов в ветке родителя
    ' Родитель - строка, в которой через точку перечислены имена групп
    ' Такую же структуру можно уведеть в окне TurboMD на вкладке "Метаданные",
    ' за исключением того, что не разделяются отдельно диалог и модуль формы
    Set AllObjects = Documents.EnumAllTypedObj()
    ' Перебираем группы объектов первого уровня
    For i = 0 To AllObjects.ChildsCount("") - 1
        Name = AllObjects.ChildName("", i)
        Commands.Add Name, Name, , True, True
    Next
End Sub

Function OnSelectCmd2(UserInput)
    OnSelectCmd2 = False
    helper.Think helper.Commands(UserInput.Name).Caption & "..."
    Set cmd = helper.Commands
    Parent = UserInput.Name
    Do
        cmd.RemoveAll
        Count = AllObjects.ChildsCount(Parent) - 1
        If Count < 0 Then
            LastAdded = Parent
        Else
            For i = 0 To Count
                Name = AllObjects.ChildName(Parent, i)
                FullName = Parent & "." & Name
                cmd.Add FullName, Name, , True, True
                LastAdded = FullName
            Next
        End If
        If cmd.Count = 1 Then Parent = LastAdded Else Exit Do
    Loop While 1 = 1
    If helper.Commands.Count < 1 Then
        Set doc = AllObjects(LastAdded)
        doc.Open
        OnSelectCmd2 = True
        Set AllObjects = Nothing
    End If
    If Not OnSelectCmd2 Then helper.ShowPopupMenu LastX, LastY
End Function


Sub FillCommandsLeftClick(Commands)
    SelectMode = 3
    For i = rcFormsCount - 1 To 0 Step -1
        Name = RecentForms(i)
        delimFirst = InStr(Name, "##") + 2
        delimNext = InStr(delimFirst, Name, "##")
        ShortName = Mid(Name, delimFirst, delimNext - delimFirst)
        Commands.Add Name, ShortName, , True, True
    Next
End Sub

Function OnSelectCmd3(UserInput)
    OnSelectCmd3 = True
    docFull = UserInput.Name
    del = InStr(docFull, "##")
    id = CLng(Left(docFull, del - 1))
    docFull = Mid(docFull, del + 2)
    Set doc = Nothing
    On Error Resume Next
    If id > 0 Then
        del = InStr(docFull, "##")
        Name = CLng(Left(docFull, del - 1))
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        typedeoc = CLng(Left(docFull, del - 1))
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        Kind = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        Path = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)
        Set doc = Documents.DocFromID(id, typedoc, Path, Kind)
    Else
        del = InStr(docFull, "##")
        Path = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)
        Set doc = Documents.Open(Path)
    End If
    If doc Is Nothing Then
        Message "Документ " & UserInput.Caption & " не открыт.", mRedErr
        OnSelectCmd3 = False
        For i = 0 To rcFormsCount - 1
            If RecentForms(i) = UserInput.Name Then
                For j = i + 1 To rcFormsCount - 1
                    RecentForms(j - 1) = RecentForms(j)
                Next
                rcFormsCount = rcFormsCount - 1
                Exit For
            End If
        Next
    Else
        doc.Open
        If doc = docWorkBook Then doc.ActivePage = CLng(docFull)
    End If
End Function


If InitAgent(0) = False Then Scripts.Unload SelfScript.Name
rcFormsCount = 0
MaxRcForms = 10
ReDim RecentForms(MaxRcForms)
