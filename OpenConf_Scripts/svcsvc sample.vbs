' Пример использования
Set srv=CreateObject("Svcsvc.Service")
Vals=""
for i=1 to 99
	vals=vals & "Значение" & CStr(i) & vbCrLf
next

Vals=Vals & "Значение100"
MsgBox srv.FilterValue(Vals,1)

MsgBox srv.SelectFolder("Укажите что либо","c:\",0)
MsgBox srv.SelectFolder("Укажите каталог","c:\",1)
MsgBox srv.SelectFolder("Укажите каталог или файл","c:\",1 + &h4000,"c:\")

MsgBox srv.SelectFile(false,"c:\1","Все файлы|*",true)

MsgBox srv.SelectValue(Vals,"Выберите, плиз",False)

Vals=Array("Значение 1","Значение 2|c","Значение 3")
Vals=Join(Vals,vbCrLf)
MsgBox srv.SelectValue(Vals,"Укажите нужные варианты",True)

Vals= _
"Папка 1|e" & vbCrLf & _
vbTab & "Значение 1_1" & vbCrLf & _
vbTab & "Подпапка 1_2" & vbCrLf & _
vbTab & vbTab & "Значение 1_2_1" & vbCrLf & _
"Папка 2" & vbCrLf & _
vbTab & "Значение 2_1" & vbCrLf & _
vbTab & "Значение 2_2" & vbCrLf & _
"Значение 3"

MsgBox srv.SelectInTree(Vals,"Выберите значение",False)
MsgBox srv.SelectInTree(Vals,"Выберите значение или папку",False,False)

Vals= _
"Папка 1|ce" & vbCrLf & _
vbTab & "Значение 1_1" & vbCrLf & _
vbTab & "Подпапка 1_2|c" & vbCrLf & _
vbTab & vbTab & "Значение 1_2_1" & vbCrLf & _
"Папка 2|e" & vbCrLf & _
vbTab & "Значение 2_1|c" & vbCrLf & _
vbTab & "Значение 2_2" & vbCrLf & _
"Значение 3"
MsgBox srv.SelectInTree(Vals,"Отметьте нужные пункты",True)

Vals= _
"Папка 1" & vbCrLf & _
vbTab & "Значение 1_1|d" & vbCrLf & _
vbTab & "Подпапка 1_2|c" & vbCrLf & _
vbTab & vbTab & "Значение 1_2_1|dc" & vbCrLf & _
"Папка 2" & vbCrLf & _
vbTab & "Значение 2_1| |val21" & vbCrLf & _
vbTab & "Значение 2_2|c|val22" & vbCrLf & _
"Значение 3"
MsgBox srv.PopupMenu(vals,0)
MsgBox srv.PopupMenu(vals,1)
MsgBox srv.PopupMenu(vals,2,300,300)