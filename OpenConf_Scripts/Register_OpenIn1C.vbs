'
' Скрипт регистрирует утилитку OpenIn1C.exe в виндовозном реестре, и ассоциирует её с расширением .ert
' В качестве параметра можно указать полный путь к OpenIn1C.exe. Например: 
'     Register_OpenIn1C.vbs "c:\tools\OpenIn1C.exe"
' По умолчанию регистрируется OpenIn1C.exe из текущего каталога
'
'

Const ForReading = 1, ForWriting = 2, ForAppending = 8

Set WshShell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

Dim ExeName
Set Args = WScript.Arguments
If Args.Count > 0 Then
	ExeName = Args(0)
Else
	ExeName = fso.GetAbsolutePathName(".\OpenIn1C.exe")
	'Wscript.Echo ExeName
End If

ExeName = Replace(ExeName, "\", "\\")


text = "REGEDIT4" & vbCrLf & _
"[HKEY_CLASSES_ROOT\1C.ExternalReportOpen]" & vbCrLf & _
"""EditFlags""=dword:00010000" & vbCrLf & _
"""AlwaysShowExt""=""""" & vbCrLf & _
"@=""Внешний отчёт 1С""" & vbCrLf & _
"[HKEY_CLASSES_ROOT\1C.ExternalReportOpen\DefaultIcon]" & vbCrLf & _
"@=""<OpenIn1C_exe>,0""" & vbCrLf & _
"[HKEY_CLASSES_ROOT\1C.ExternalReportOpen\shell]" & vbCrLf & _
"@=""Open""" & vbCrLf & _
"[HKEY_CLASSES_ROOT\1C.ExternalReportOpen\shell\Open]" & vbCrLf & _
"[HKEY_CLASSES_ROOT\1C.ExternalReportOpen\shell\Open\command]" & vbCrLf & _
"@=""<OpenIn1C_exe> \""%1\""""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Applications\OpenIn1C.exe]" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Applications\OpenIn1C.exe\shell]" & vbCrLf & _
"@=""Open""" & vbCrLf & _
"""FriendlyCache""=""OpenIn1C""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Applications\OpenIn1C.exe\shell\Open]" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Applications\OpenIn1C.exe\shell\Open\command]" & vbCrLf & _
"@=""<OpenIn1C_exe> \""%1\""""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\1C.ExternalReportOpen]" & vbCrLf & _
"""EditFlags""=dword:00010000" & vbCrLf & _
"""AlwaysShowExt""=""""" & vbCrLf & _
"""BrowserFlags""=dword:00000008" & vbCrLf & _
"@=""Внешний отчёт 1С""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\1C.ExternalReportOpen\DefaultIcon]" & vbCrLf & _
"@=""<OpenIn1C_exe>,0""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\1C.ExternalReportOpen\shell]" & vbCrLf & _
"@=""Open""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\1C.ExternalReportOpen\shell\Open]" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\1C.ExternalReportOpen\shell\Open\command]" & vbCrLf & _
"@=""<OpenIn1C_exe> \""%1\""""" & vbCrLf & _
"[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\.ert]" & vbCrLf & _
"@=""1C.ExternalReportOpen""" & vbCrLf


text = Replace(text, "<OpenIn1C_exe>", ExeName)

Set f = fso.OpenTextFile("OpenIn1C.reg", ForWriting, true, TristateFalse)
f.Write(text)
f.Close

WshShell.Run "regedit /s OpenIn1C.reg", 1, true

fso.DeleteFile "OpenIn1C.reg"


'Зарегистрируем ещё и для .mxl
text = Replace(text, "\.ert", "\.mxl")

Set f = fso.OpenTextFile("OpenIn1C.reg", ForWriting, true, TristateFalse)
f.Write(text)
f.Close

WshShell.Run "regedit /s OpenIn1C.reg", 1, true

fso.DeleteFile "OpenIn1C.reg"
