'========================================================================================
' Выгрузка в файл командной строки  Copyright (c) 2004 Артур Аюханов")
' e-mail: artbear@bashnet.ru ICQ: 265666057
'========================================================================================

Option Explicit

' следующие две строки для вывода отладочных сообщений
' эти две строки можно и удалить
'Dim DebugFlag 'обязательно глобальная переменная
'DebugFlag = True 'Разрешаю вывод отладочных сообщений

WScript.Quit(main())

'********************************************************************
' Возвращает 1 при успехе, 0 - при неудаче
Function main( )
  main = 0

  'Make sure the host is csript, if not then abort
  VerifyHostIsCscript()

    Call Echo("__________________________________________________________________________")
    Call Echo(" Выгрузка в файл командной строки  Copyright (c) 2004 Артур Аюханов")
    Call Echo("")

  Dim wshShell
  Dim fso 'as FileSystemObject

  set wshShell = wScript.createObject("wScript.shell")
  Set fso = CreateObject("Scripting.FileSystemObject")

  Dim ParamFileName
  ParamFileName = Replace(LCase(WScript.ScriptFullName),".vbs",".lst")
	Debug "ParamFileName", ParamFileName

	Dim ParamFile 'As TextStream

	On Error Resume Next
	Set ParamFile = fso.CreateTextFile(ParamFileName, true)
	if err.Number<>0 then
		Err.Clear()
		echo "Ini-файл "& ParamFileName &" не удалось открыть!"
		Exit Function
	end if
	on error goto 0
	Dim objArgs
	Set objArgs = WScript.Arguments
	Dim I
	For I = 0 to objArgs.Count - 1
		ParamFile.WriteLine objArgs(I)
	Next
	ParamFile.Close

	main = 1
end Function 'main

Sub Echo(text)
  WScript.Echo(text)
	On Error Resume Next
		If IsObject(LogFile) then        'LogFile should be a file object
			LogFile.WriteLine text
		end if
	On Error goto 0
End Sub'Echo

Sub Debug(ByVal title, ByVal msg)
on error resume next
  DebugFlag = DebugFlag
  if err.Number<>0 then
    err.Clear()
    on error goto 0
    Exit Sub
  end if
  if DebugFlag then
    if not (IsEmpty(msg) or IsNull(msg)) then
      msg = CStr(msg)
    end if
    if not (IsEmpty(title) or IsNull(title)) then
      title = CStr(title)
    end if
    If msg="" Then
      Echo(title)
    else
      Echo(title+" - "+msg)
    End If
  End If
on error goto 0
End Sub'Debug

'********************************************************************
'*
'* Sub      VerifyHostIsCscript()
'*
'* Purpose: Determines which program is used to run this script.
'*
'* Input:   None
'*
'* Output:  If host is not cscript, then an error message is printed
'*          and the script is aborted.
'*
'********************************************************************
Sub VerifyHostIsCscript()

    ON ERROR RESUME NEXT

    'Define constants
    CONST CONST_ERROR                   = 0
    CONST CONST_WSCRIPT                 = 1
    CONST CONST_CSCRIPT                 = 2

    Dim strFullName, strCommand, i, j, intStatus

    strFullName = WScript.FullName

    If Err.Number then
        Call Echo( "Error 0x" & CStr(Hex(Err.Number)) & " occurred." )
        If Err.Description <> "" Then
            Call Echo( "Error description: " & Err.Description & "." )
        End If
        intStatus =  CONST_ERROR
    End If

    i = InStr(1, strFullName, ".exe", 1)
    If i = 0 Then
        intStatus =  CONST_ERROR
    Else
        j = InStrRev(strFullName, "\", i, 1)
        If j = 0 Then
            intStatus =  CONST_ERROR
        Else
            strCommand = Mid(strFullName, j+1, i-j-1)
            Select Case LCase(strCommand)
                Case "cscript"
                    intStatus = CONST_CSCRIPT
                Case "wscript"
                    intStatus = CONST_WSCRIPT
                Case Else       'should never happen
                    Call Echo( "An unexpected program was used to " _
                                       & "run this script." )
                    Call Echo( "Only CScript.Exe or WScript.Exe can " _
                                       & "be used to run this script." )
                    intStatus = CONST_ERROR
                End Select
        End If
    End If

    If intStatus <> CONST_CSCRIPT Then
        Call Echo( "Please run this script using CScript." & vbCRLF & _
             "This can be achieved by" & vbCRLF & _
             "1. Using ""CScript SystemAccount.vbs arguments"" for Windows 95/98 or" _
             & vbCRLF & "2. Changing the default Windows Scripting Host " _
             & "setting to CScript" & vbCRLF & "    using ""CScript " _
             & "//H:CScript //S"" and running the script using" & vbCRLF & _
             "    ""SystemAccount.vbs arguments"" for Windows NT/2000/XP." )
        WScript.Quit(0)
    End If
End Sub 'VerifyHostIsCscript