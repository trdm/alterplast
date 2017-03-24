' (c) Orefkov
' небольшие изменения artbear@bashnet.ru
'

Dim LoadedMD	' Будет использоваться для автозагрузки мдшника, собранного GComp
LoadedMD = ""

Dim cvs

' Запуск GComp с заданной строкой
Sub RunGComp(cmd)
	Message cmd,mInformation
    On Error Resume Next
    ret = Plugins("1CVS").GComp.Run(cmd)
    If Err <> 0 Then
        MsgBox "Произошла ошибка " & Err.Description
    Else
        Message "GComp завершил работу с кодом " & CStr(ret),mInformation
    End If
End Sub

' Обработчики событий при автозагрузке мдшника.
' НЕ ПРОВЕРЯЛИСЬ
Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
    If LoadedMD <> "" Then
        FileName = LoadedMD
        Answer = mbaOK
    End If
End Sub

Sub Configurator_OnMsgBox(Text, Style, DefAnswer, Answer)
    If LoadedMD <> "" Then
        Message Text, mMetaData
        Message DefAnswer, mMetaData
        Answer = DefAnswer
    End If
End Sub

Sub Configurator_OnDoModal(hWnd, Caption, Answer)
    If LoadedMD <> "" Then anwser = mbaOK
End Sub

Sub DeCompileFile(FileName)
'    RunGComp ("-d -DD " & cvs.gcomp.SRCPath & " --meta-data -F """ & FileName & """")
    RunGComp ("-d -DD " & """" & cvs.gcomp.SRCPath & """" & " --meta-data -F """ & FileName & """")
End Sub

' Артур
'   gcomp -c -DD "W:\Kp\New\kp102031md\Src_GComp Новая" --meta-data -F 1cv7new.md
Sub CompileFile(FileName)
    RunGComp ("-c -DD " & """" & cvs.gcomp.SRCPath & """" & " --meta-data -F """ & FileName & """")
End Sub
' завершение

'Обработка события сохранения метаданных
Sub Configurator_MetaDataSaved(FileName)
    'If MsgBox("Разобрать GComp'ом?", vbYesNo, "CVS") = vbYes Then
    '    'RunGComp "-d -F """ & FileName & """"
	'	DeCompileFile FileName
    'End If
End Sub

Sub TestCVSRun()
    On Error Resume Next
    Plugins("1CVS").CVS.Run ("checkout config")
    If Err <> 0 Then MsgBox Err.Description
End Sub

Sub TestRunGUI()
' Артур	- не ждем окончания работы CVS.GUI
'	Plugins("1CVS").CVS.RunGUI True
'	MsgBox "Close GUI"
    Plugins("1CVS").CVS.RunGUI False
End Sub

' Декомпиляция мдшника "на лету". Не требуется сохранять мдшник
Sub TestRunGCompDeCompile()
	Fname="1cv7.md"
    If MetaData.Modified <> 0 Then
        Status "Сохраняется свежая версия метаданных"
        If MetaData.SaveMDToFile(IBDir & "new.md", False) = False Then
            Message "Не удалось сохранить метаданные", mBlackErr
            Exit Sub
        End If
		Fname="new.md"
    End If
    Status "Разбор метаданных..."

    'Message "-d -D "& cvs.gcomp.SRCPath & " --meta-data -F """ & IBDir & Fname & """", mMetaData

    'RunGComp ("-d -D " & cvs.gcomp.SRCPath & " --meta-data -F """ & IBDir & Fname & """")
	DeCompileFile IBDir & Fname
End Sub

' Сборка мдшника из сорцов
' Надо бы еще автозагрузку сделать
Sub TestRunGCompCompile()
    On Error Resume Next

' -- Артур -- 
'    RunGComp "-c"
'   gcomp -c -DD "W:\Kp\New\kp102031md\Src_GComp Новая" --meta-data -F 1cv7new.md
    CompileFile("1cv7new.md")
' -- завершение

End Sub

Sub GCompHelp()
	RunGComp ""
End Sub

Sub Init(param)
	set cvs=Plugins("1cvs")
	if Not cvs Is Nothing Then
		cvs.cvs.CVSPath = "C:\Program Files\cvsnt\cvs.exe"
		cvs.cvs.CVSRoot = ":local:T:\Repository"
		cvs.cvs.GUIPath = "C:\Program Files\WinCvs 1.3\wincvs.exe"

	  Set fso = CreateObject("Scripting.FileSystemObject")
	  if fso.FileExists("D:\Cmd\gcomp.exe") then
			cvs.gcomp.GCompPath = "D:\Cmd\gcomp.exe"
		elseif fso.FileExists("C:\Cmd\gcomp.exe") then
			cvs.gcomp.GCompPath = "C:\Cmd\gcomp.exe"
		end if

		cvs.gcomp.SRCPath = "src_GComp"
	End If
End Sub

Sub Configurator_AllPluginsInit()
	Init(0)
End Sub

Init(0)