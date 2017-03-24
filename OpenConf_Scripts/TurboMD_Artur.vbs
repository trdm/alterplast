'Пример скрипта, позволяющего выгрузить в файл
'текущую редактируемую форму и модуль
'(либо просто модуль проведения или вида расчета)
'во внешний файл.
'При этом выгружается текущее состояние формы/модуля
'без необходимости сохранять конфигурацию

Dim BaseDir
BaseDir = IBDir & "unpack\" ' Базовый каталог для выгрузки

Dim TurboMdPrmName
TurboMdPrmName = IBDir & "TurboMd.prm"

Dim Collection

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

' функции работы со списком модулей
Sub AddDocToList(ModuleName, FileName)
		if Collection.Exists(ModuleName) then
			Collection.Remove ModuleName
		end if
		Collection.Add ModuleName, FileName
end sub 'AddDocToList

Sub RemoveDocFromList(doc)
		if Collection.Exists(doc.Name) then
			Collection.Remove doc.Name
		end if
End Sub 'RemoveDocFromList()

' Анализ файла turbomd.prm
Sub AnalyzeTurboMDPrm(NullParam) ' параметр нужен, чтобы не был виден в списке макросов
    set Collection = CreateObject("Scripting.Dictionary")

    Set fso = CreateObject("Scripting.FileSystemObject")
    set file = fso.OpenTextFile(TurboMdPrmName, 1, true) 'чтение

		Do While file.AtEndOfStream <> True
      CurrLine = file.ReadLine
      pos = InStr(CurrLine,"=")
      if pos <> 0 then
      	ModuleName = Left(CurrLine, pos-1)
      	FileName = Mid(CurrLine, pos+1)
      	AddDocToList ModuleName, FileName
			end if
		Loop
		file.Close
end sub 'AnalyzeTurboMDPrm

' добавление соответствия в файл TurboMD.prm
Sub WriteToTurboMDPrm(NullParam)
    Set fso = CreateObject("Scripting.FileSystemObject")
    set file = fso.CreateTextFile(TurboMdPrmName, 2) 'запись

		items = Collection.Items
		keys = Collection.Keys
		For i = 0 To Collection.Count -1
			file.WriteLine keys(i) & "=" & items(i)
		Next
		file.Close
end sub 'WriteToTurboMDPrm

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

    AddDocToList doc.Name, "unpack\" & Replace(doc.Name, ".", "\") & ".txt"

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

    'анализ файла turbomd.prm
    AnalyzeTurboMDPrm 1

    If d = docText Then     ' Просто модуль
        UnloadDoc d
    Else
        If d = docWorkBook Then ' Форма
            If MsgBox("Выгрузить форму?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
            	UnloadDoc d.Page(0) ' Выгружаем диалог
            End If
            If MsgBox("Выгрузить модуль?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
            	UnloadDoc d.Page(1) ' Выгружаем модуль
            End If
        End If
    End If

    WriteToTurboMDPrm 1

End Sub

'Собственно макрос для выгрузки активного окна
Sub RemoveLinkToCurrentWnd()
    Set w = Windows.ActiveWnd
    If w Is Nothing Then
        MsgBox "Нет активного окна", vbOKOnly, "TurboMD"
        Exit Sub
    End If
    Set d = w.Document
    If d.ID < 1 Then
        MsgBox "Окно ни форма, ни модуль", vbOKOnly, "TurboMD"
        Exit Sub
    End If

    'анализ файла turbomd.prm
    AnalyzeTurboMDPrm 1

    If d = docText Then     ' Просто модуль
    	RemoveDocFromList d
    Else
        If d = docWorkBook Then ' Форма
		    	RemoveDocFromList d.Page(0) ' Выгружаем диалог
    			RemoveDocFromList d.Page(1) ' Выгружаем модуль
        End If
    End If

    WriteToTurboMDPrm 1
End Sub 'RemoveLink_to_CurrentWnd

Sub ClearAllLinks () 'чтобы все загружалось из мд
	Set fso = CreateObject("Scripting.FileSystemObject")
    Set file = fso.CreateTextFile(TurboMdPrmName, 2) 'запись
    file.Write ""
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
