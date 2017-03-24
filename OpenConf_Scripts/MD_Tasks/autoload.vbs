' Скрипт предназначен для автоматической загрузки
' измененной конфигурации на основе параметра в
' командной строке.
' Для автозагрузки в командной строке запуска
' должен быть указан ключ "/q ИмяФайлаМД".
' Для простоты ключ должен идти последним в строке.

Dim loadMD  ' Имя загружаемого файла
loadMD = ""

Sub Configurator_AllPluginsInit()
    loadMD = InStr(CommandLine, "/q") ' Ищем ключ /q
    If loadMD <> 0 Then ' Если есть, получаем имя файла
        loadMD = Mid(CommandLine, loadMD + 3)
        '   Если выполняются эти строки, значит надо загружать мдшник
        '   Для начала надо выгрузить все остальные скрипты,
        '   чтобы не мешались их обработчики событий
        k = 0
        While Scripts.Count <> 1
            n = Scripts.Name(k)
            If LCase(n) <> "autoload" Then
                Message "Unload " & n, mMetaData
                Scripts.Unload k
            Else
                k = 1
            End If
        Wend
        ' Возможно другие скрипты успели открыть какиенить окна.
        ' Надо их закрыть.
        While Not Windows.ActiveWnd Is Nothing
        	Windows.ActiveWnd.Close
        Wend
        '   Теперь дадим команду на загрузку МД
        SendCommand cmdLoadMD
    Else
        ' Иначе выгружаем скрипт, как ненужный
        'Message "Not Autoload", mNone
        Scripts.Unload "autoload"
    End If
End Sub

Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
    '   После команды на загрузку МД, 1С запросит имя файла.
    If loadMD <> "" Then
        FileName = loadMD
        Answer = mbaOK
    End If
End Sub

Sub Configurator_ConfigWindowCreate()
	MsgBox "AutoLoad Trap Event"
    '   Раз окно открылось, значит все загрузилось, надо
    '   сохранять и выходить.
    If loadMD <> "" Then
        Message "Quit", mExclamation3
        Quit True
    End If
End Sub

Sub Configurator_OnMsgBox(Text, Style, DefAnswer, Answer)
    '   Во время всего действа, 1С периодически будет спрашивать
    '   разные глупые вещи, и сообщать никому не нужную информацию.
    '   Но мы в себе уверенны, потому на все вопросы будем отвечать
    '   по умолчанию.
    If loadMD <> "" Then
        Answer = DefAnswer
        Message Text, mExclamation
        '   Впрочем, если вы не совсем уверены, то можете
        '   вставить здесь свои проверки на то, что спрашивает 1С.
        Text = LCase(Text)
        If InStr(Text, "файл") <> 0 And InStr(Text, "не найден") <> 0 Then Quit False
    End If
End Sub

Sub Configurator_OnDoModal(Hwnd, Caption, Answer)
    '   Аналогично предыдущей процедуре.
    If loadMD <> "" Then
        Message Caption, mInformation
        Answer = mbaOK
    End If
End Sub
