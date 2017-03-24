set obj = wscript.CreateObject("Vis1cpp.Vis1cppPlugin", "ObjEvent_")
obj.Method1() 'метод генерирующий событие

Sub ObjEvent_OnMethod1()
	msgbox "script event"
End Sub