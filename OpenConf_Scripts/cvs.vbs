$NAME CVS & GComp Script
' (c) А.С. Орефков
' Набор макросов для работы с gcomp & cvs из Конфигуратора
' Скрипт предполагает следующую структуру хранения исходников:
' КаталогИБ - не под контролем
'    Src	- корневой каталог исходников, импортируется в CVS
'		md	- разобранный GComp'ом мдшник
'		Остальные папки, зеркала папок в КаталогеИБ, только с разобранными
'			внешними отчетами, например, ExtForms, Progs, Classes, Unpuck и тп.
'
'Версия: $Revision: 1.4 $
'

' Настройки
' Предполагается, что каталог с GComp прописан в PATH. Если это не так, укажите полный путь.
GCompPath = "gcomp.exe "
' Предполагается, что каталог с CVS прописан в PATH. Если это не так, укажите полный путь.
CVSPath = "cvs.exe "
' Имя каталога для разобранных модулей
SRCPath = IBDir & "SRC\"
' Режим отображения вывода CVS и GComp
OutPutTo = 2 ' 0-не выводить, 1 - в окно сообщений, 2 - открывать в окне
' Перенаправлять stderr в stdout
ErrToOut = True

' не задавать вопросы при открытии конфигурации или ее сохранении
bDontAskAboutConfig = True

' Имя модуля в репозитарии
Module = ""
'Репозитарий
CVSROOT = ""

' Общие объекты
Set fso = CreateObject("Scripting.FileSystemObject")
Set shell = CreateObject("WScript.Shell")
Set svc = CreateObject("Svcsvc.Service")

TempDir = shell.ExpandEnvironmentStrings("%temp%") & "\"
cmdFName = TempDir & "cvsScriptRun.cmd"
outFName = TempDir & "cvsScriptOut.txt"

' Текстовый документ для вывода сообщений
Set LastOutDoc = Nothing

Sub SplitPath(Path, Dir)
    delim = InStrRev(Path, "\")
    Dir = Mid(Path, delim + 1)
    Path = Left(Path, delim - 1)
End Sub

Function GetRepository(Dummy)
	If Len(CVSROOT) = 0 Then
		GetRepository = InputBox("Укажите Репозиторий", SelfScript.Name, "")
	Else
		GetRepository = CVSROOT
	End If
End Function

Function GetModule(Dummy)
    If Len(Module) = 0 Then
        If fso.FolderExists(SRCPath & "cvs") Then
            Set rep = fso.OpenTextFile(SRCPath & "cvs\Repository", 1)
            Module = rep.ReadLine()
            rep.Close
        Else
            SplitPath Left(IBDir, Len(IBDir) - 1), Module
            Module = InputBox("Укажите название модуля в репозитарии", SelfScript.Name, Module)
        End If
    End If
    GetModule = Module
End Function

Function RunCVS(WorkDir, CmdLine)
    ' Процедура запуска cvs с указанными параметрами командной строки
    ' в заданном каталоге. Весь вывод перенаправляется в файл cvs.out
    ' во временном каталоге. В зависимости от флага OutPutTo файл либо
    ' не отображается, либо выводится в окно сообщений, либо открывается
    ' в окне.
    ' Командная строка может быть многострочной.
    ' На каждую строку выполняется запуск cvs
    If fso.FileExists(outFName) Then fso.DeleteFile outFName
    ' Для начала сформируем командный файл для запуска
    Set CmdFile = fso.CreateTextFile(cmdFName, True)
    CmdFile.WriteLine "@Echo off"
    WorkDir=svc.AnsiToOEM(WorkDir)
    CmdFile.WriteLine Left(WorkDir, 2)                 ' Сменим диск
    CmdFile.WriteLine "cd """ & WorkDir & """"         ' Перейдем в каталог
    if errToOut Then CmdFile.WriteLine "cd | fecho >> """ & outFName & """" ' Выведем текущий каталог

    CmdLines = Split(CmdLine, vbCrLf)
    For i = 0 To UBound(CmdLines)
        ' Чтобы cvs понимал русские каталоги, конвертнем строку в OEM
        Line=svc.AnsiToOem(CmdLines(i))
    	if len(Line)>0 Then
	        ' Для каждой строки сформируем строку запуска cvs
	        if errToOut Then
	        	' Выведем командную строку на экран и в файл вывода
	        	CmdFile.WriteLine "echo " & Line & " | fecho >> """ & outFName & """"
	        End If
	        Line = CVSPath & Line
	        If ErrToOut Then Line = Line & " 2>&1"  ' Перенаправим stderr в stdout
	        ' Вывод будем отображать на экран и в файл. Так как руские каталоги cvs выдает в ANSI,
	        ' для fecho зададим параметр конвертить вывод в OEM
	        Line = Line & " | fecho -o >> """ & outFName & """"
	        CmdFile.WriteLine Line
	    end if
    Next
    CmdFile.Close
    ' Сформируем строку запуска
    strrun = "cmd.exe /c """ & cmdFName & """"
    ' Запустим сформированный файл
    shell.Run strrun, 1, True ' Запускаем видимым, что бы если что, можно было прибить.
    RunCVS = LastOutPut
End Function

Function RunGComp(WorkDir, CmdLine)
    If fso.FileExists(outFName) Then fso.DeleteFile outFName
    ' Для начала сформируем командный файл для запуска
    Set CmdFile = fso.CreateTextFile(cmdFName, True)
    CmdFile.WriteLine "@Echo off"
    WorkDir=svc.AnsiToOEM(WorkDir)
    CmdFile.WriteLine Left(WorkDir, 2)                  ' Сменим диск
    CmdFile.WriteLine "cd """ & WorkDir & """"          ' Перейдем в каталог
    if errToOut Then CmdFile.WriteLine "cd | fecho >> """ & outFName & """" ' Выведем текущий каталог
    CmdLines = Split(CmdLine, vbCrLf)
    For i = 0 To UBound(CmdLines)
        ' Для каждой строки сформируем строку запуска gcomp
        Line=svc.AnsiToOEM(CmdLines(i))
        if len(Line)>0 Then
        	if ErrToOut Then
	        	CmdFile.WriteLine "echo " & Line & " | fecho >> """ & outFName & """"
	        end if
	        Line = GCompPath & Line
	        If ErrToOut Then Line = Line & " 2>&1"  ' Перенаправим stderr в stdout
	        ' Вывод будем отображать на экран и в файл.
	        Line = Line & " | fecho >> """ & outFName & """"
	        CmdFile.WriteLine Line
	    end if
    Next
    CmdFile.Close
    ' Сформируем строку запуска
    strrun = "cmd.exe /c """ & cmdFName & """"
    ' Запустим сформированный файл
    shell.Run strrun, 1, True ' Запускаем видимым, что бы если что, можно было прибить.
    RunGComp = LastOutPut
End Function


sub RunCVSCmdLine()
    Dir=svc.SelectFolder("Укажите каталог",SRCPath,1,SRCPath)
    if Len(Dir)=0 Then Exit Sub
    cmdLine=InputBox("Введите командную строку")
    if len(cmdLine)=0 Then Exit Sub
    RunCVS Dir,CmdLine
end sub

Function GetCurrentDocument(Dummy)
    Set GetCurrentDocument = Nothing
    If Windows.ActiveWnd Is Nothing Then
        MsgBox "Нет активного окна", , SelfScript.Name
        Exit Function
    End If
    Set GetCurrentDocument = Windows.ActiveWnd.Document
End Function

Function DocToSrcPath(doc)
    ' Здесь должна быть функция, которая по имени объекта метаданных
    ' возвращает путь в разобранном мдшнике
    DocToSrcPath = ""
    If doc = docUnknown Then Exit Function
    If doc.ID < 0 Then Exit Function
    Names = Split(doc.Name, ".")
    Select Case Names(0)
        Case "Документ"
            DocToSrcPath = "Документы"
        Case "Справочник"
            DocToSrcPath = "Справочники"
    End Select
    If Len(DocToSrcPath) > 0 Then DocToSrcPath = DocToSrcPath + "\" + Names(1)
    'Message DocToSrcPath,mNone
End Function

Sub ShowStatus()
    Set doc = GetCurrentDocument(0)
    If doc Is Nothing Then Exit Sub
    Path = DocToSrcPath(doc)
    If Len(Path) = 0 Then Exit Sub
    If Not fso.FolderExists(SRCPath & Path) Then
        Message "Каталог " & Path & " не существует.", mRedErr
        Exit Sub
    End If
    RunCVS SRCPath, "status """ & Path & """"
End Sub

Sub ImportSrcToRepository()
    If Not fso.FolderExists(SRCPath) Then
        MsgBox "Нет каталога с разобранными файлами", , SelfScript.Name
        Exit Sub
    End If
    If fso.FolderExists(SRCPath & "cvs") Then
        MsgBox "Каталог с разобранными файлами уже под контролем", , SelfScript.Name
        Exit Sub
    End If
    msgText = "Импортировать каталог SRC в репозитарий?"
    If MsgBox(msgText, vbYesNo, SelfScript.Name) = vbNo Then
        Exit Sub
    End If
    On Error GoTo 0
    ' Процедура ставит под контроль каталог SRC
    CVSRepository = GetRepository(0)
    Module = GetModule(0)
    If Len(Module) = 0 Then Exit Sub
    Vendor = InputBox("Укажите метку производителя", SelfScript.Name, "vendor")
    Rel = InputBox("Укажите метку релиза", SelfScript.Name, "release")
    msg = ""
    While Len(msg) = 0
        msg = InputBox("Укажите комментарий", SelfScript.Name, "Ставим под контроль")
        If Len(msg) = 0 Then
            If MsgBox("Отменить импорт?", vbYesNo, SelfScript.Name) = vbYes Then Exit Sub
        End If
    Wend
    ' Импортнем src
    Message "Помещаем в модуль каталог " & SRCPath, mMetaData
    If Len(CVSRepository) = 0 Then
    	RunCVS SRCPath, "import -C -m """ & msg & """ """ & Module & """ " & Vendor & " " & Rel
    Else
    	RunCVS SRCPath, "-d " & CVSRepository & " import -C -m """ & msg & """ """ & Module & """ " & Vendor & " " & Rel
  	End If
End Sub

Sub DecompileMD()
    fName = IBDir & "1cv7.md"
    If MetaData.Modified > 0 Then
        If MsgBox("Файл метаданных изменен. Сохранить во временный файл?", vbYesNo, SelfScript.Name) = vbNo Then
            Exit Sub
        Else
            fName = IBDir & "1cv7md.tmp"
            Status "Сохранение метаданных в файл " & fName & " ..."
            MetaData.SaveMDToFile fName, False
            Status ""
        End If
    End If
    RunGComp IBDir, "-v -d -F """ & fName & """ -D """ & SRCPath & "md"""
End Sub

' Инициализирует конфигурацию.
Sub InitConfiguration()
	DecompileMD
	DeCompileExtForms
	ImportSrcToRepository
End sub


sub Decomp(s,d,cmdtext)
    Set sf = s.SubFolders
    For Each sSubFolder In sf
        dName = d.Path & "\" & sSubFolder.Name
        If Not fso.FolderExists(dName) Then
            Set dSubFolder = fso.CreateFolder(dName)
        Else
            Set dSubFolder = fso.GetFolder(dName)
        End If
        Decomp sSubFolder, dSubFolder, cmdtext
    Next
    Set sf = s.Files
    For Each file In sf
        If LCase(Right(file.Name, 4)) = ".ert" Then
            strrun = "-d --no-empty-mxl -F """ & file.Path & """ -DD """ & d.Path & "\" & file.Name & """"
            cmdtext=cmdtext & vbCrLf & strrun
        Else
            On Error Resume Next
            fso.CopyFile file.Path, d.Path & "\" & file.Name, True
            If Err <> 0 Then
                Message "Не удалось скопировать " & file.Path, mRedErr
            End If
            On Error GoTo 0
        End If
    Next
end sub

Sub DeCompileExtForms()
	If Not fso.FolderExists(SRCPath) Then MsgBox "Некуда разбирать.",,SelfScript.Name:Exit Sub
	Set Folder=fso.GetFolder(IBDir)
	sel=""
	For Each f In Folder.SubFolders
		if InStr("/new_stru/src/syslog/usrdef/,", "/" & LCase(f.Name) & "/")=0 Then
			if Len(sel)>0 Then sel=sel+vbCrLf
			sel=sel+f.Name
			if fso.FolderExists(SRCPath & f.Name) Then sel=sel & "|c"
		end if
	Next
	if len(sel)=0 Then MsgBox "Не найдено подходящих каталогов",,SelfScript.Name:Exit Sub
	sel=svc.SelectValue(sel,"Укажите разбираемые каталоги",True)
	if len(sel)=0 Then Exit Sub
	fNames=Split(sel,vbCrLf)
	cmdlines=""
	for i=0 to UBound(fNames)
		Set Source=fso.GetFolder(IBDir & fNames(i))
		dName=SrcPath & fNames(i)
		if fso.FolderExists(dName) Then
			Set Dest=fso.GetFolder(dName)
		else
			set Dest=fso.CreateFolder(dName)
		end if
		Decomp Source,Dest,cmdlines
	next
	if Len(cmdlines)>0 Then RunGComp IBDir, cmdlines
End Sub

Sub CheckOutSrc()
    CheckOutFolder ""
End Sub

Sub CheckOutRefs()
    CheckOutFolder "Справочники"
End Sub

Sub CheckOutFolder(Dir)
    Message "Извлекаем рабочую копию в " & SRCPath & Dir & " ...", mMetaData
    If fso.FolderExists(SRCPath & Dir) Then fso.DeleteFolder SRCPath & Dir, True
    Modul = GetModule(0)
    CVSRepository = GetRepository(0)
    If Len(CVSRepository) = 0 Then
    	RunCVS IBDir, "-r co -d """ & SRCPath & Dir & """ """ & Modul & Dir & """"
    Else
    	RunCVS IBDir, "-d" & CVSRepository & " -r co -d """ & SRCPath & Dir & """ """ & Modul & Dir & """"
    End If
End Sub

Sub Configurator_MetaDataSaved(FileName)
	if bDontAskAboutConfig then
    	Exit Sub
    end if

    ' Предложим разобрать GComp'ом
    msgText = "Разобрать файл GComp'ом?"
    If MsgBox(msgText, vbOkCancel, SelfScript.Name) = vbOK Then
        Message "Разбираем файл метаданных...", mMetaData
        RunGComp IBDir, "-v -d -F """ & FileName & """ -D """ & SRCPath & "md"""
    Else
        Exit Sub
    End If
    ' Проверим, находится ли каталог SRC под контролем CVS
    If Not fso.FolderExists(SRCPath & "\cvs") Then
        ImportSrcToRepository
    End If
End Sub

Sub BuildMDAndLoad()
	err.Raise 0,SelfScript.Name,"Функция не написана"
End Sub

Sub UpdateFolder(Dir)
    Message "Обновление в каталоге " & Dir, mMetaData
    RunCVS Dir, "update"
End Sub

Sub Update()
    Path = svc.SelectFolder("Что обновим?", SRCPath,1 + &h4000 , SRCPath)
    UpdateFolder Path
End Sub

Sub Commit()
    Message "Фиксация изменений", mMetaData
    RunCVS SRCPath, "commit"
End Sub

Sub Configurator_AllPluginsInit()
		If CVSROOT = "" Then
			CVSROOT = shell.ExpandEnvironmentStrings("%$CVSROOT%")
			If CVSROOT = "%$CVSROOT%" Then
				CVSROOT = ""
			End If
		End If
    MdExist=fso.FileExists(IBDir & "1cv7.md")
    SrcExist=fso.FolderExists(SRCPath)
    SrcInCVS=False
    if SrcExist Then SrcInCVS=fso.FolderExists(SRCPath & "cvs")

    If SrcExist Then
    	' Есть каталог разбора
        If SrcInCVS Then
        	' Он под контролем. Предложим обновить
            Set rep = fso.OpenTextFile(SRCPath & "cvs\Repository", 1)
            Module = rep.ReadLine()
            rep.Close
			if not bDontAskAboutConfig then
	            If MsgBox("Обновить SRC из репозитария " & Module & "?", vbOkCancel, SelfScript.Name) = vbOK Then
	                UpdateFolder SRCPath
	            End If
		    end if
        Else
        	' Не под контролем. Предложим поставить
        End If
    Else
    	' Нет каталога разбора
        If MdExist Then
        	' Есть мдшник. Предложим разобрать
        Else
        	' Нет мдшника. Предложим извлечь из репозитария
        End If
    End If

' -- Артур -- неудобно (для этого отдельный скрипт)
'    Windows.MainWnd.Caption = IBName

End Sub

Sub srvSwitchOutPut()
    OutPutTo = (OutPutTo + 1) Mod 3
    Select Case OutPutTo
        Case 0
            txt = "Нет вывода"
        Case 1
            txt = "В окно сообщений"
        Case 2
            txt = "В текстовое окно"
    End Select
    Message "Текущий режим вывода: " & txt, mNone
End Sub

Sub srvSwitchErrToOut()
    ErrToOut = Not ErrToOut
    If ErrToOut Then txt = "" Else txt = " НЕ "
    Message "Сообщения об ошибках " & txt & "выводятся", mNone
End Sub

Function LastOutPut()
    svc.FileO2A outFName
    Set outFile = fso.OpenTextFile(outFName, 1)
    LastOutPut = outFile.ReadAll()
    outFile.Close
    If OutPutTo = 1 Then
        Message LastOutPut, mNone
    ElseIf OutPutTo = 2 Then
        On Error Resume Next
        dType = LastOutDoc
        If Err <> 0 Then Set LastOutDoc = Documents.New(docText)
        On Error GoTo 0
        LastOutDoc.Text = LastOutDoc.Text & vbCrLf & LastOutPut
    End If
End Function

sub DecompileCurrentExtForms()
    If Windows.ActiveWnd Is Nothing Then Exit Sub
    If Windows.ActiveWnd.Document <> docWorkBook Then Exit Sub
    Dim doc ' As WorkBook
    Set doc = Windows.ActiveWnd.Document
    If doc.ID > 0 Then MsgBox "Это не внешний отчет", ,SelfScript.Name: Exit Sub
    If Len(doc.Path) = 0 Then MsgBox "Файл не записан", , SelfScript.Name: Exit Sub
    Path = LCase(doc.Path)
    If InStr(Path, LCase(IBDir)) <> 1 Then
        MsgBox "Процедура работает только с отчетами в каталоге ИБ", , SelfScript.Name
        Exit Sub
    End If
    doc.Save
    newPath = Replace(Path, IBDir, IBDir & "src\")
    strrun = "-d --no-empty-mxl -F """ & Path & """ -DD """ & newPath & """"
    RunGComp IBDir, strrun
end sub

' Выберите нужный вариант расположения WinCVS
sub RunWinCVS()
	shell.Run """c:\Program Files\GNU\WinCvs 1.3\wincvs.exe""", 1, False
end sub

sub RunWinCVS2()
	shell.Run """c:\Program Files\GNU\WinCvs 2.0\wincvs.exe""", 1, False
end sub
