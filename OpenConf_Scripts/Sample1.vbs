Sub Connect1COLE()
    Set v77 = CreateObject("V77S.Application")
    v77.Initialize v77.RMTrade, "", "NO_SPLASH_SHOW"
End Sub

Sub TestMDOChildsProps()
    PrintMDO MetaData.TaskDef, 0
End Sub

Sub PrintMDO(mdo, level)
    txt = ""
    For i = 1 To level
        txt = txt & vbTab
    Next
    Message txt & mdo.Name & " " & mdo.ID, mNone
    txt = txt & vbTab
    For i = 0 To mdo.Props.Count - 1
        Message txt & mdo.Props.Name(i, True) & "=" & mdo.Props(i), mNone
    Next
    Set childs = mdo.childs
    txt = txt & vbTab
    For i = 0 To childs.Count - 1
        Message txt & childs.Name(i, True), mMetaData
        Set mda = childs(i)
        c = mda.Count - 1
        For j = 0 To c
            PrintMDO mda(j), level + 3
        Next
    Next
End Sub

Sub TestMDOChildsProps1()
    Set atr = MetaData.TaskDef.childs("справочник")("Проверка").childs("Реквизит")(0)
    Message atr.Props.Name(0, True) & "=" & atr.Props(4), mNone
    Message MetaData.TaskDef.Props("ОсновнаяФормаПланаСчетов"), mNone
End Sub

Sub TestMainHWND()
    Message "HWND=" & Windows.hWnd, mNone
End Sub

Sub TestOpenOLEForm()
    Set exc = OpenOLEForm("Excel.Sheet", "Test")
  Set exc=exc.WorkSheets(1)
    exc.Cells(1, 1).value = "Hello !!!"
End Sub

Sub GMText()
    Message GlobalModule.Name, mRedErr
    'GlobalModule.Line(1)=vbTab & "Hello"
    'GlobalModule.Open
End Sub

Sub Configurator_ConfigWindowCreate()
'    Message "Open Config Wnd", mExclamation
End Sub

Sub Configurator_AllPluginsInit()
''    Message "Init All", mExclamation
End Sub

Private Sub Configurator_OnDoModal(hWnd, Caption, Answer)
'    'Message Caption, mNone
''    Answer = 1
'    Message Caption+"-"+ CSTR(Answer), mNone
End Sub

Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
End Sub

Sub Configurator_OnMsgBox(text, style, def, a)
''    Message text, mInfo
'    Message text+"-"+ CSTR(style)+"-"+ CSTR(def)+"-"+ CSTR(a), mInfo
''    a = 3
End Sub

Sub TestExitConf()
    Quit False
End Sub

Sub TestWriteLog()
    LogMessageWrite ecAdmin, "Проверка"
    LogMessageWrite ecInfo, "Проверка2", "Пользовательские", "Информация"
End Sub

Sub TestPlugins()
    For i = 0 To Plugins.Count - 1
        Message Plugins.Name(i) & "  ---  " & Plugins.Path(i), mNone
    Next
    Plugins("userplugin").Macros1
    Message Plugins.Path("userplugin"), mNone
    'Должен сообщить об ошибке
    Message Plugins.Name(MetaData), mNone
End Sub

Sub TestScripts()
    Scripts.Load BinDir + "config\sample.vbs"
    Scripts.ReLoad ("Sample1")
End Sub

Sub TestTD()
    MetaData.TaskDef.Round15_2 = True
    MsgBox MetaData.TaskDef.Round15_2
End Sub

Sub TestCopyrights()
    Message MetaData.LongCopyright, mMetaData
    Message MetaData.ShortCopyright, mMetaData
End Sub

Sub TestMigrationSave()
    fname = IBDir & "migration.rul"
  set fso=CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(fName, 2, True,0)
    Dim ch(2)
    ch(0) = "Константа"
    ch(1) = "Справочник"
    ch(2) = "Документ"

    For j = 0 To 2
        Set childs = MetaData.TaskDef.childs(CStr(ch(j)))
        For i = 0 To childs.Count - 1
            Set mdo = childs(i)
            Set mgr = MetaData.migration(mdo.ID)
            f.WriteLine mdo.FullName & "#" & mgr.RuleOfMigration & "#" & mgr.AutoRegister & "#" & mgr.AdditDBSigns
        Next
    Next
  f.Close
    OpenFile fname
End Sub

Sub TestMigrationSaveToExcel()
  set fso=OpenOLEForm("Excel.Sheet","Правила миграции") 'CreateObject("Scripting.FileSystemObject")
  set exc=fso.WorkSheets(1)

    Dim ch(2)
    ch(0) = "Константа"
    ch(1) = "Справочник"
    ch(2) = "Документ"
  exc.Cells(1,1) = "Тип"
  exc.Cells(1,2) = "Вид"
  exc.Cells(1,3) = "Правило миграции"
  exc.Cells(1,4) = "Авторегистрация"
  exc.Cells(1,5) = "Дополнительно"
  row=2
    For j = 0 To 2
        Set childs = MetaData.TaskDef.childs(CStr(ch(j)))
        For i = 0 To childs.Count - 1
            Set mdo = childs(i)
            Set mgr = MetaData.Migration(mdo.ID)
      exc.Cells(row,1) = ch(j)
      exc.Cells(row,2) = mdo.Name
      exc.Cells(row,3) = mgr.RuleOfMigration
      exc.Cells(row,4) = mgr.AutoRegister
      exc.Cells(row,5) = mgr.AdditDBSigns
      row = row + 1
        Next
    Next
  fso.SaveAs IBDir & "migration.xls"
End Sub

Sub TestLoadMigrationFromExcel()
  set fso=CreateObject("Excel.Application")
  fso.Visible=true
  set exc=fso.WorkBooks.Open(IBDir & "migration.xls")
  set exc=exc.WorkSheets(1)
  row=2
  On Error Resume Next
  While exc.Cells(row,1) <> Empty
    Set mdo = Nothing
    name="" & exc.Cells(row,1) & "." & exc.Cells(row,2)
    Set mdo=Metadata.TaskDef.Childs(CStr(exc.Cells(row,1)))(CStr(exc.Cells(row,2)))
    if mdo Is Nothing Then
      Message name & " не найден в метаданных",mRedErr
    Else
      Message "Загружен " & name, mInformation
      Set mgr=MetaData.Migration(mdo.id)
      mgr.RuleOfMigration = exc.Cells(row,3)
      mgr.AutoRegister = exc.Cells(row,4)
      mgr.AdditDBSigns = exc.Cells(row,5)
    End If
    row=row+1
  Wend
End Sub

Sub TestSaveMMDS()
  MetaData.SaveMMDS IBDir & "mmds.mds"
  OpenFile IBDir & "mmds.mds"
End Sub

Sub TestTypes()
  Set ref=MetaData.TaskDef.Childs("Справочник")("Проверка")
  set atr=ref.Childs("Реквизит")(0)
  Set t=atr.Type
  Message t.FullName,mNone
  t.Type=ttReference
  t.ID=ref.id
  Message t.FullName,mNone
  atr.Type=t
End Sub
