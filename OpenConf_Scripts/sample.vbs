' (c) Orefkov
' Пример скрипта, демонстрирующего некоторые возможности OpenConf

' Чтение информации о Метаданных
' Каждый объект метаданных имеет набор именованных потомков и свойств,
' доступных через свойство Childs и Props
' Корневым объектом метаданных является TaskDef, начиная с которого
' можно обойти все дерево метаданных
Sub TestMDOChildsProps()
    PrintMDO MetaData.TaskDef, 0
End Sub

Sub PrintMDO(mdo, level)
    txt = ""
    For i = 1 To level
        txt = txt & vbTab
    Next
    ' Выведем имя объекта
    Message txt & mdo.Name & " " & mdo.ID, mNone
    txt = txt & vbTab
    ' Выведем все свойства объекта
    For i = 0 To mdo.Props.Count - 1
        Message txt & mdo.Props.Name(i, True) & "=" & mdo.Props(i), mNone
    Next
    ' Переберем всех его потомков
    Set Childs = mdo.Childs
    txt = txt & vbTab
    For i = 0 To Childs.Count - 1
        ' Выведем имя потомков
        Message txt & Childs.Name(i, True), mMetaData
        ' Получим набор объектов-потомков данного вида
        Set mda = Childs(i)
        c = mda.Count - 1
        For j = 0 To c
            ' Выведем информацию о потомке
            PrintMDO mda(j), level + 3
        Next
    Next
End Sub

' Небольшой пример доступа к объекту метаданных
Sub TestMDOChildsProps1()
    Set atr = MetaData.TaskDef.Childs("справочник")("Проверка").Childs("Реквизит")(0)
    Message atr.Props.Name(0, True) & "=" & atr.Props(4), mNone
    Message MetaData.TaskDef.Props("ОсновнаяФормаПланаСчетов"), mNone
End Sub

Sub TestMigrationSaveToExcel()
    Set fso = OpenOLEForm("Excel.Sheet", "Правила миграции")
    Set exc = fso.Worksheets(1)

    Dim ch(2)
    ch(0) = "Константа"
    ch(1) = "Справочник"
    ch(2) = "Документ"
    exc.Cells(1, 1) = "Тип"
    exc.Cells(1, 2) = "Вид"
    exc.Cells(1, 3) = "Правило миграции"
    exc.Cells(1, 4) = "Авторегистрация"
    exc.Cells(1, 5) = "Дополнительно"
    Row = 2
    For j = 0 To 2
        Set Childs = MetaData.TaskDef.Childs(CStr(ch(j)))
        For i = 0 To Childs.Count - 1
            Set mdo = Childs(i)
            Set mgr = MetaData.Migration(mdo.ID)
            exc.Cells(Row, 1) = ch(j)
            exc.Cells(Row, 2) = mdo.Name
            exc.Cells(Row, 3) = mgr.RuleOfMigration
            exc.Cells(Row, 4) = mgr.AutoRegister
            exc.Cells(Row, 5) = mgr.AdditDBSigns
            Row = Row + 1
        Next
    Next
    fso.SaveAs IBDir & "migration.xls"
End Sub

Sub TestLoadMigrationFromExcel()
    Set fso = CreateObject("Excel.Application")
    fso.Visible = True
    Set exc = fso.Workbooks.Open(IBDir & "migration.xls")
    Set exc = exc.Worksheets(1)
    Row = 2
    On Error Resume Next
    While exc.Cells(Row, 1) <> Empty
        Set mdo = Nothing
        Name = "" & exc.Cells(Row, 1) & "." & exc.Cells(Row, 2)
        Set mdo = MetaData.TaskDef.Childs(CStr(exc.Cells(Row, 1)))(CStr(exc.Cells(Row, 2)))
        If mdo Is Nothing Then
            Message Name & " не найден в метаданных", mRedErr
        Else
            Message "Загружен " & Name, mInformation
            Set mgr = MetaData.Migration(mdo.ID)
            mgr.RuleOfMigration = exc.Cells(Row, 3)
            mgr.AutoRegister = exc.Cells(Row, 4)
            mgr.AdditDBSigns = exc.Cells(Row, 5)
        End If
        Row = Row + 1
    Wend
End Sub

Sub TestSaveMMDS()
    MetaData.SaveMMDS IBDir & "mmds.mds"
    OpenFile IBDir & "mmds.mds"
End Sub

Sub TestTypes()
    Set ref = MetaData.TaskDef.Childs("Справочник")(0)
    Set atr = ref.Childs("Реквизит")(0)
    Set t = atr.Type
    Message t.FullName, mNone
    t.Type = ttReference
    t.ID = ref.ID
    Message t.FullName, mNone
    atr.Type = t
End Sub

Sub TestConst()
    Set con = MetaData.Constants
    cnt = con.Count
    Message "Констант - " & CStr(cnt), mNone
    For i = 0 To cnt - 1
        Message con.Name(i), mNone
    Next
    Message con(0).Periodic, mNone
    Set nConst = con.Add("Cnst", "Добавлено автоматом")
    Message "new id=" & CStr(nConst.ID), mNone
    Set t = nConst.Type
    t.Type = ttReference
    t.ID = MetaData.TaskDef.Childs("Справочник")(0).ID
    nConst.Type = t
    nConst.Periodic = True
    MetaData.RefreshMDWnd
End Sub

Sub TestWindow()
    Set w = Windows.ActiveWnd
    If Not w Is Nothing Then
        Set mdo = w.Document
        If Not mdo Is Nothing Then
            Message "Path=" & mdo.Path, mMetaData
            Message "Type=" & CStr(mdo.Type), mMetaData
            Message "ID=" & CStr(mdo.ID), mMetaData
            Message "Kind=" & mdo.Kind, mMetaData
            If mdo.ID <> -1 Then
                Set mdo = MetaData.FindObject(mdo.ID)
                Message mdo.FullName, mMetaData
            End If
        End If
    End If
End Sub

' Пример открытия в Конфигураторе документа ActiveDocument
' и работы с ним
Sub TestOpenOLEForm()
    Set exc = OpenOLEForm("Excel.Sheet", "Test")
    Set exc = exc.Worksheets(1)
    exc.Cells(1, 1).Value = "Hello !!!"
End Sub

' Пример работы с внешними файлами
' а также со страницами формы, и текстовыми документами
Sub TestOpenERT()
    ' Dim ert As OpenConf.Workbook, modul As TextDoc
    ' VBScript не понимает Dim As, поэтому перед переносом
    ' из VB надо закомментировать
    ' Открываем внешний файл (здесь подставьте свой путь)
    Set ert = Documents.Open("d:\Visual Studio\Project\CONFIG\COMPOUND.ERT")
    If Not ert Is Nothing Then      ' Открыли удачно
        If ert = docWorkBook Then   ' Проверим, что открыли именно форму
            Set modul = ert.Page(1) ' Получаем документ с закладки 1 (Модуль)
            ' Вставляем в начало модуля текст
            modul.Range(0, 0, 0, 0) = "// Open " & CStr(Date) & " at " & CStr(Time) & vbCrLf
            ' Переключаем на страницу модуля
            ert.ActivePage = 1
            ' Получаем страницу описания
            Set modul = ert.Page("Описание")
            Message modul.text, mInformation
        End If
    End If
End Sub

' Пример доступа к модулям, формам, таблицам
Sub TestModules()
    Set m = Documents("Документ.Новый1.Модуль Документа")
    ' В качестве имени можно использовать
    '   ГлобальныйМодуль
    '   Справочник.ИмяСправочника.<Форма|Форма Группы|ФормаСписка.ИмяФормыСписка>
    '   Документ.ИмяДокумента.<Форма|Модуль Документа>
    '   Отчет.ИмяОтчета
    '   Обработка.ИмяОбработки
    '   ВидРасчета.ИмяВидаРасчета
    '   Журнал.ИмяЖурнала.ИмяФормыСписка
    '   ЖурналРасчетов.ИмяЖурналаРасчетов.ИмяФормыСписка
    '   ПланСчетов.<Форма|ФормаСписка.ИмяФормыСписка>
    '   Операция.<Форма|ФормаСписка.ИмяФормыСписка>
    '   Проводка.ФормаСписка.ИмяФормыСписка
    '   Описание.ИДОбъектаМетаданных
    '   Таблицы
    ' Для модулей и описаний возвращается TextDoc, для форм WorkBook
    m.Open
End Sub

' Запись в журнал регистрации
Sub TestWriteLog()
    LogMessageWrite ecAdmin, "Проверка"
    LogMessageWrite ecInfo, "Проверка2", "Пользовательские", "Информация"
End Sub

Sub TestDocs()
    Set refs = MetaData.TaskDef.Childs("Справочник")
    For i = 0 To refs.Count - 1
        Set doc = Documents("Справочник." & refs(i).Name & ".Форма")
        doc.Open
    Next
End Sub

' Сохранение открытых окон конфигуратора
Sub SaveOpenDocs()
    fName = IBDir & "wnds.txt"
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(fName, 2, True, 0)
    f.WriteLine (IsConfigWndOpen())
    Set w = Windows.FirstWnd
    While Not w Is Nothing
        Set d = w.Document
        If d <> docUnknown Then
            If d.ID > 0 And d = docWorkBook Then Set d = d.Page(d.ActivePage)
            stat = CStr(d.ID) & "##" & CStr(d) & "##" & d.Kind & "##" & d.Path & "##"
            If d.ID < 0 And d = docWorkBook Then stat = stat & CStr(d.ActivePage)
            f.WriteLine (stat)
        End If
        Set w = Windows.NextWnd(w)
    Wend
    f.Close
End Sub

' Открытие запомненных окон
Sub OpenSavedDocs()
    fName = IBDir & "wnds.txt"
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(fName, 1, False, 0)
    Line = f.ReadLine
    Line = LCase(Line)
    If Line = "true" Then SendCommand cmdOpenConfigWnd
    While Not f.AtEndOfStream
        Line = f.ReadLine

        del = InStr(Line, "##")
        ID = CLng(Left(Line, del - 1))
        Line = Mid(Line, del + 2)

        del = InStr(Line, "##")
        typedeoc = CLng(Left(Line, del - 1))
        Line = Mid(Line, del + 2)

        del = InStr(Line, "##")
        Kind = Left(Line, del - 1)
        Line = Mid(Line, del + 2)

        del = InStr(Line, "##")
        Path = Left(Line, del - 1)
        Line = Mid(Line, del + 2)
        On Error Resume Next
        Set doc = Nothing
        If ID > 0 Then
            Set doc = Documents.DocFromID(ID, typedoc, Path, Kind)
            If Not doc Is Nothing Then
                doc.Open
            End If
        Else
            Set doc = Documents.Open(Path)
            If Not doc Is Nothing Then
                If doc = docWorkBook Then doc.ActivePage = CLng(Line)
            End If
        End If
        If Err <> 0 Then
            Message "Документ " & Path & " не открыт.", mRedErr
            Err.Clear
        End If
    Wend
End Sub

Sub TestNewDoc()
    Documents.New docText
    Documents.New docTable
    Set wb = Documents.New(docWorkBook)
    wb.AddPage "Testing"
    wb.ActivePage = "Testing"
End Sub

' Найти ссылки на объект метаданных
Sub TestRefers()
    Set ref = MetaData.TaskDef.Childs("Справочник")(0)
    Message ref.FullName, mInformation
    Set arr = MetaData.TestRefers(ref.ID, False)
    For i = 0 To arr.Count - 1
        Message arr(i).FullName, mMetaData
    Next
End Sub

Sub TestVersion()
    Message Version, mInformation
End Sub


' Пример работы с таймером
TimerAutoSave = 0
Sub StartAutoSave()
    StopAutoSave
    TimerAutoSave = CfgTimer.SetTimer(60000, False)
End Sub

Sub StopAutoSave()
    If TimerAutoSave <> 0 Then CfgTimer.KillTimer TimerAutoSave
End Sub

TimerText = 0
Sub StartTextTimer()
    StopTextTimer
    TimerText = CfgTimer.SetTimer(100, False)
End Sub
Sub StopTextTimer()
    If TimerText <> 0 Then CfgTimer.KillTimer TimerText
End Sub

Sub Configurator_OnTimer(timerID)
    If timerID = MyTimer Then
        Status "Автосохранение метаданных..."
        MetaData.SaveMDToFile IBDir & "1cv7.md.bak", False
        Status ""
    End If
    If timerID = TimerText Then
        Set w = Windows.ActiveWnd
        If Not w Is Nothing Then
            Set d = w.Document
            If d = docText Then
                Message d.CurrentWord, mNone
            End If
        End If
    End If
End Sub

Sub Configurator_ConfigWindowCreate()
'    Documents("ГлобальныйМодуль").Open
End Sub

Sub Configurator_AllPluginsInit()
    'TestOpenERT
  SendCommand cmdOpenConfigWnd
End Sub

Private Sub Configurator_OnDoModal(hWnd, Caption, Answer)
    'Message Caption, mNone
End Sub

Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
End Sub

Sub Configurator_OnMsgBox(text, style, def, a)
End Sub

Sub Configurator_MetaDataSaved(FileName)
End Sub

sub testTable()
  set t=Documents("Таблицы").Page(0)
  t.SaveToFileAs Ibdir & "test.xls",tbExc
  'Documents.Open Ibdir & "test.mxl"
end sub

sub TestName()
  set d=Windows.ActiveWnd.Document
  Message d.Name,mNone
  if d=docWorkBook Then
    Message d.Page(d.ActivePage).Name,mNone
  end if
end sub

sub TestDialog()
  Set d=Documents("Справочник.Новый1.Форма").Page(0)
  Message d.Stream,mNone
end sub

sub TestLoadDialog()
  Set d=Documents("Справочник.Новый1.Форма").Page(0)
  d.Open
  d.LoadFromFile IBDir & "test.frm"
end sub

sub TestEnum()
  set Enums=MetaData.Enums
  'Message Name,mNone
  'Enums.Remove 0
  set n=Enums.Add("gfgfgfg")
  'Message n.ID,mNone
  MetaData.RefreshMDWnd
end sub

sub TestEnumVals()
  set e=MetaData.Enums(0)
  Message e.Name,mNone
  Message "Count vals " & CStr(e.ValsCount),mNone
  for i=0 to 10
    e.Add "znach" & CStr(i)
  next
  e.Remove 0
end sub
