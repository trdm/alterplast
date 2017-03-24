$NAME Выбор подсистемы
'
' Сергей Токарев aka GrayT mail: tswsoft@tamb.ru
'
'Данный скрипт и обработка предназначены для формирования списка подсистем
'(аналогично в восьмерке). Т.е. внутри одной конфигурации указываются
'отдельные логические участки - учет материалов, расчеты с клиентами, учет
'готовой продукции и т.п. К каждой подсистеме относятся определенные
'справочники, документы, отчеты и т.д.
'При работе в конфе появляется быстрый доступ к модулям из определенных
'подсистем
'
'При работе обработки и скрипта используется
'ActivX от Александра Орефкова - svcsvc.dll.
'
'Порядок работы:
'1.Зарегистрить svcsvc.dll (regsvr32 svcsvc.dll)
'2.Запустить обработку DescriptSubSystem.ert. С ее помощью сформировать
'список подсистем и объектов конфигурации, относящихся к данным подсистемам.
'Список сохраняется в каталоге конфигурации в файле DescriptSubSystem.xml
'3.В конфигураторе запускаем ChoiceSubSystem.vbs:.. дальше разберетесь :)

Dim tree

Sub ReadFromXML(Pusto)
set Objc= CreateObject("MSXML2.DOMDocument")
If not Objc.Load(IbDir & "DescriptSubSystem.xml") then
	MsgBox ("Нет файла описания подсистем DescriptSubSystem.xml" & vbCrLf &_
                "Для формирования файла используйте" & vbCrLf &_
                "обработку DescriptSubSystem.ert ")
	Exit sub
End if

set root=Objc.ChildNodes.item(1)
set ListSubRoot = root.ChildNodes
maxCount = ListSubRoot.length-1
for i=0 to maxCount
 set SubRoot = ListSubRoot.item(i)
 tree = tree & SubRoot.GetAttribute("Description") & vbCrLf

 set ListSubRoot2 = SubRoot.ChildNodes
 maxCount2 = ListSubRoot2.length-1
 for j=0 to maxCount2
   set SubRoot2 = ListSubRoot2.item(j)
   tree = tree & vbTab & SubRoot2.GetAttribute("Tip") & vbCrLf
   set ListSubRoot3 = SubRoot2.ChildNodes
   maxCount3 = ListSubRoot3.length-1
   for k=0 to maxCount3
    set SubRoot3 = ListSubRoot3.item(k)
    tree = tree & vbTab & vbTab & SubRoot3.GetAttribute("Object") & vbCrLf
   next
 next
next
End Sub

Sub ShoiceSubSystem ()
	tree = ""
	ReadFromXML(0)
	Set srv=CreateObject("Svcsvc.Service")
	Cmd = srv.SelectInTree(tree,"Открыть....",false)
	ln = Len(Cmd)
	If Ln = 0 Then
		Exit sub
	End if
	fs = InStr(1, Cmd, "\")
	Cmd = Right(Cmd, Ln-fs)

	ln = Len(Cmd)
	fs = InStr(1, Cmd, "\")
	Cmd1 = Left(Cmd, fs-1)
	Cmd  = Right(Cmd, Ln-fs)

	Select case Cmd1
		Case "Справочники"
			Cmd = "Справочник." & Cmd
		Case "Документы"
			Cmd = "Документ." & Cmd
		Case "Отчеты"
			Cmd = "Отчет." & Cmd
		Case "Обработки"
			Cmd = "Обработка." & Cmd
		Case "Журналы"
			Cmd = "Журнал." & Cmd
		Case "ЖурналыРасчетов"
			Cmd = "ЖурналРасчетов." & Cmd
		Case "ВидыРасчетов"
			Cmd = "ВидРасчетов." & Cmd
		Case else
			MsgBox "Пока не придумал что с этим делать :("
			Exit sub
	End select

	ListDoc = ""
	Set AllObjects = Documents.EnumAllTypedObj()
	Count = AllObjects.ChildsCount(Cmd) - 1

	'If Count >= 0 Then
	If Count > 0 Then ' artbear
		For i = 0 To Count
	    	Name = AllObjects.ChildName(Cmd, i)
	        FullName = Cmd & "." & Name

			Count = AllObjects.ChildsCount(FullName) - 1

			If Count >= 0 Then
				For j = 0 To Count
			    	Name2 = AllObjects.ChildName(FullName, j)
			        FullName2 = FullName & "." & Name2
					ListDoc = ListDoc & Name & "." & Name2 & "| |" & FullName2 & vbCrLf
			    Next
			Else
					ListDoc = ListDoc & Name & "| |" & FullName & vbCrLf
			End If
	    Next
		CurentDoc = srv.PopupMenu(ListDoc,0) ' artbear
		
	ElseIf Count = 0 Then ' сразу открыть форму -- artbear
    	Name = AllObjects.ChildName(Cmd, i)
        FullName = Cmd & "." & Name 
		CurentDoc = FullName
	Else
		  MsgBox "Странно но ни чего нет"
		  Exit sub
	End If

	Set Doc = AllObjects(CurentDoc)
	Doc.Open
End sub
 
' добавление от Александра Орефкова
' Макрос по ОЛЕ запускает текущую базу в режиме предприятия и открывает обработку модально
Sub SetupSubSystem()
	Set v7=CreateObject("V77.Application")
	v7.Initialize v7.rmtrade,"/d""" & ibdir & """","NO_SPLASH_SHOW"
	'v7.EvalExpr "ОткрытьФормуМодально(""Отчет"",,""" & BinDir & "config\DescriptSubSystem.ert"")"
	v7.EvalExpr "ОткрытьФормуМодально(""Отчет"",,""" & BinDir & "config\Scripts\Чужие\DescriptSubSystem.ert"")"
End Sub