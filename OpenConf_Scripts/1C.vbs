' в случае появления окна "Реорганизация информации", где написано, что
' "Изменения в метаданных не вызвали изменений в данных",
' автоматом нажимаю "Принять"  и окно не появляется на экране.
' Мелочь, а так удобно :)
'
' также есть пример перебора дочерних окон - метод EnumChildWindows
'
' Авторы: 
'	Артур Аюханов aka artbear	( e-mail:	artbear@bashnet.ru )
'	MetaEditor					( e-mail: shotfire@inbox.ru )
'
Sub Configurator_OnDoModal(Hwnd, Caption, Answer)
	If Caption = "Реорганизация информации" then
		LB_GETCOUNT		= &H018B
		LB_GETTEXTLEN	= &H018A
		LB_GETTEXT		= &H0189
		
		Set Wrapper = CreateObject("DynamicWrapper")
		Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
		ListBoxHandle = Wrapper.FindWindowExA (Hwnd,0,"ListBox",0)
		
		Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
		ListboxCount = Wrapper.SendMessage (ListBoxHandle, LB_GETCOUNT, 0, 0)
		if ListboxCount <> 2 then ' требуется листбокс, где всего два элемента
			exit Sub
		end if
		cnt = Wrapper.SendMessage (ListBoxHandle, LB_GETTEXTLEN, 1, 0)
		if cnt <> Len("Изменения в метаданных не вызвали изменений данных.") then		
			exit Sub ' вдруг не та строка
		end if
		
		Wrapper.Register "USER32.DLL",   "SendMessage",    "I=lllr", "f=s", "r=l"
		TextBuff = Space(240)
		
		cnt = Wrapper.SendMessage (ListBoxHandle, LB_GETTEXT, 1, TextBuff)
	    If cnt > 0 Then
	        TextBuff = Left(cstr(TextBuff), cnt)
	    End If

		' из-за бага в DynamicWrapper (но не всегда!! ) - при получении более 50 символов ВОЗМОЖНО возвращается всякая фигня
		if TextBuff= "Ч‘Ш‘Щ‘Ъ‘Ы‘Ь‘Э‘Ю‘Я‘а‘б‘в‘г‘" then ' Изменения в метаданных не вызвали изменений в данных
			' автоматом нажимаем кнопку "Принять"
			Answer = mbaOK 
		else TextBuff= "Изменения в метаданных не вызвали изменений данных." ' если все как обычно (баг не сработал)
			' автоматом нажимаем кнопку "Принять"
			Answer = mbaOK 
		end if
		
		Wrapper = 0
	end if
	
End Sub
       
' Дать инфу об окне
sub GetWindowInfo(hwnd, Group)
	Const WM_GETTEXT = &HD
	
	message Group & "hwnd <" & Hex(hwnd) & ">"
	
	Set Wrapper = CreateObject("DynamicWrapper")
	
 	Wrapper.Register "USER32.DLL",   "SendMessage",    "I=lllr", "f=s", "r=l"
	Title = Space(240)
	cnt = Wrapper.SendMessage(hwnd, WM_GETTEXT ,240,Title) ' (заголовок окна)
    If cnt > 0 Then
        Title = Left(cstr(Title), cnt)
		message Group & "Caption <" & Title & ">"
	else
		message Group & "cnt <" & cnt & ">"
    End If
	
 	Wrapper.Register "USER32.DLL",   "GetClassName",    "I=lrl", "f=s", "r=l"
	ClassName = Space(240)
	cnt = Wrapper.GetClassName(hwnd, ClassName, 240) ' класс окна
    If cnt > 0 Then
        ClassName = Left(cstr(ClassName), cnt)
		message Group & "Class <" & ClassName & ">"
	else
		message Group & "cnt <" & cnt & ">"
    End If
   
	Wrapper = 0
end sub

' Пример перебора дочерних окон
sub EnumChildWindows(hwnd, Group)
	
	Set Wrapper = CreateObject("DynamicWrapper")
	                     
	Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llll", "f=s", "r=l"
	
	Handle = Wrapper.FindWindowExA (Hwnd,0,0,0)
	while Handle <> 0 
		GetWindowInfo Handle, Group
		EnumChildWindows Handle, Group & "--- "
		Handle = Wrapper.FindWindowExA (hwnd,handle,0,0)
	wend
     
	Wrapper = 0

End Sub

Sub Configurator_OnMsgBox(text, style, def, Answer)
	                          
	if InStr(text, "Общая файловая ошибка при доступе к") > 0 then
		Message "Событие OnMsgBox - " & text, mRedErr
		Answer = mbaOK
    	Exit Sub
	end if
	
End Sub
