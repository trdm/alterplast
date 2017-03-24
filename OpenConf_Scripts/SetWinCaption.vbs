' идея - trdm (c) 2004
' 
' Устанавливает заголовок основного окна в виде "Конфигуратоp - (<Каталог базы данных>)"
' Полезно, когда открыто много конфигураторов.
' 
'  artbear Таймер вроде бы не нужен
' 
'
'TimerWindowText = 0
'Sub StartChangeWindowText()
'    StopChangeWindowText
'    TimerWindowText = CfgTimer.SetTimer(3000, False)
'End Sub
'
'Sub StopChangeWindowText()
'    If TimerWindowText <> 0 Then CfgTimer.KillTimer TimerWindowText
'End Sub
'
'
'Sub Configurator_OnTimer(timerID)
'    If timerID = TimerWindowText Then
'		Set pMainWnd = Windows.MainWnd
'			pCaption = pMainWnd.Caption
'			IF InStr(1,pCaption,"Конфигуратор")>0 Then
'				pCaption = "Конфигуратоp - (" & IBDir & ")"
'				pMainWnd.Caption = pCaption
'		    End If
'    End If
'End Sub
'StartChangeWindowText

Windows.MainWnd.Caption = "Конфигуратор - < " & IBDir & " >" ' artbear
