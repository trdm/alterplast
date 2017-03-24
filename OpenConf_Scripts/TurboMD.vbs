' (c) Orefkov
'Пример скрипта, позволяющего выгрузить в файл
'текущую редактируемую форму и модуль
'(либо просто модуль проведения или вида расчета)
'во внешний файл.
'При этом выгружается текущее состояние формы/модуля
'без необходимости сохранять конфигурацию
'
' Автор: Александр Орефков
'
' artbear: Есть вставка кода
' Автор - Николай Гаврилов <shootnick2000@mail.ru>
' Отлавливает момент сохранения md и переименовывает turboMD.prm в bak.
'
Dim BaseDir
BaseDir = IBDir & "unpack\" ' Базовый каталог для выгрузки

' Процедура создания ветки каталогов
Sub MakeDir(Dir)
    Set fso = CreateObject("Scripting.FileSystemObject")
    If Left(Dir, 2) = "\\" Then
        'UNC Path
        pos = InStr(3, Dir, "\")    'Server name
        p = Left(Dir, pos)
        Dir = Mid(Dir, pos + 1)
    Else
        p = ""
    End If
    pos = 1
    While pos <> 0
        pos = InStr(Dir, "\")
        If pos = 0 Then
            p = p & Dir
        Else
            p = p & Left(Dir, pos)
            Dir = Mid(Dir, pos + 1)
        End If
        If fso.FolderExists(p) = False Then fso.CreateFolder p
    Wend
End Sub

'Выгрузка документа
Sub UnloadDoc(doc)
	'Формируем имя файла
    fName = BaseDir & Replace(doc.Name, ".", "\")
    If doc = docTable Then fName = fName & ".mxl" Else fName = fName & ".txt"
	'Из имени файла выделяем каталог
    lastdec = InStrRev(fName, "\")
    Dir = Left(fName, lastdec - 1)
	'И создаем этот каталог
    MakeDir Dir
	' Выгружаем документ в файл
    doc.SaveToFile fName
	' Проверяем, есть ли этот документ в turbomd.prm
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 1, True)
    If f.AtEndOfStream = True Then t = "" Else t = f.ReadAll()
    If InStr(t, doc.Name) = 0 Then
        If MsgBox("Добавить " & doc.Name & " в turbomd.prm ?", vbQuestion + vbYesNo) = vbYes Then
			' Добавляем запись в turbomd.prm
            f.Close
            Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 8)
            If Len(t) > 0 And Right(t, 2) <> vbCrLf Then f.WriteBlankLines 1
            f.WriteLine doc.Name & " = " & Replace(fName, IBDir, "")
        End If
    End If
    f.Close 
	Status "TurboMD: выгрузили документ <" & doc.Name & ">"
End Sub

'Собственно макрос для выгрузки активного окна
Sub UnloadCurrentWnd()
    Set w = Windows.ActiveWnd
    If w Is Nothing Then
        MsgBox "Нет активного окна", vbOKOnly, "TurboMD"
        Exit Sub
    End If
    Set d = w.Document
    If d.ID < 2 Then
        MsgBox "Окно ни форма, ни модуль", vbOKOnly, "TurboMD"
        Exit Sub
    End If
    If d = docText Then     ' Просто модуль
        UnloadDoc d
    Else
        If d = docWorkBook Then ' Форма
            If MsgBox("Выгрузить форму?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
                UnloadDoc d.Page(0) ' Выгружаем диалог
            End If
			'ModuleText = d.Page(1).text
			'For i = 2 To d.CountPages - 1
				'UnloadMXL d.Page(i), ModuleText
				'UnloadDoc d.Page(i)
			'Next
            If MsgBox("Выгрузить модуль?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
				'strOldText = d.Page(1).text
				'd.Page(1).text = ModuleText
                UnloadDoc d.Page(1) ' Выгружаем модуль
				'd.Page(1).text = strOldText
            End If
        End If
    End If
End Sub

'Макрос для загрузки всех распакованных файлов обратно в мдшник
Sub LoadFromFilesToMD()
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 1, True)
    On Error Resume Next
    While f.AtEndOfStream = False
        t = f.ReadLine()
        eq = InStr(t, "=")
        If eq > 0 Then
            dName = Trim(Left(t, eq - 1))
            fName = Trim(Mid(t, eq + 1))
            If Mid(fName, 2, 1) <> ":" And Left(fName, 2) <> "\\" Then fName = IBDir & fName
            Set doc = Documents(dName)
            If Err <> 0 Then
                Message Err.Description, mRedErr
                Err.Clear
            Else
                If doc.LoadFromFile(fName) <> True Then
                    Message "Не удалось загрузить " & doc.Name & " из " & fName, mBlackErr
                Else
                    Message doc.Name & " загружен из " & fName, mInformation
                End If
                If Err <> 0 Then
                    Message Err.Description, mRedErr
                    Err.Clear
                End If
            End If
        End If
    Wend
End Sub

' Макрос для быстрого открытия файла TurboMD.prm
Sub OpenTurboMDPrm()
    Documents.Open IBDir & "turbomd.prm"
End Sub

Sub SaveMD()
    MetaData.SaveMDToFile IBDir & "1cv7new.md", False
End Sub

'artbear
'Автор - Николай Гаврилов <shootnick2000@mail.ru>
' отлавливает момент сохранения md и переименовывает turboMD.prm в bak.
Sub RenameTurboMdPRM2Bak()

	Set FSO = CreateObject("Scripting.FileSystemObject")

	If FSO.FileExists(IBDir & "turbomd.prm") Then
		FSO.CopyFile IBDir & "turbomd.prm", IBDir & "turbomd.prm.bak", True
		FSO.DeleteFile(IBDir & "turbomd.prm")
		'MsgBox "turbomd.prm переименован в turbomd.prm.bak"
	End If

End Sub

'После сохранения - переделка prm
'------------------------------------------------------------------
Sub Configurator_MetaDataSaved(FileNAme)
	RenameTurboMdPRM2Bak()
End Sub

' выгрузка таблиц и замена ссылки типа 
' 	Таб.ИсходнаяТаблица("ИмяВнутреннейТаблицы");
' на
'	Таб.ИсходнаяТаблица("ПутьКВнешнемуФайлуОтТурбоМД");
'
Sub UnloadMXL(docMXL, ModuleText)
  
Message "Выгружаю таблицу "+docMXL.Name ' Возвращает для мокселевких страниц мусор,
'Причем для каждого вызова, разное.
    fName = BaseDir & Replace(docMXL.Name, ".", "\")
       
    lastdec = InStrRev(fName, "\")
    MXLName = Right(fName, Len(fName) - lastdec)
    
    fName = fName + ".mxl"
  
    Set RegExp = CreateObject("VBScript.RegExp")
    
    RegExp.MultiLine = 1
    RegExp.IgnoreCase = 1
    RegExp.Global = 1
    'RegExp.MultiLine = true
    'RegExp.IgnoreCase = true
    'RegExp.Global = true
    
    RegExp.Pattern = "\.ИсходнаяТаблица\(""" + MXLName + """\)"
    ModuleText = RegExp.Replace(ModuleText, ".ИсходнаяТаблица("""+ fName + """)")
'Message ModuleText

End Sub