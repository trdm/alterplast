$NAME NavigationTools

'==================================================================================================
'	Набор процедур для навигации по конфигурации
'
' Версия: $Revision: 1.28 $
'
'	Автор: MetaEditor
'	e-mail: shotfire@inbox.ru
'==================================================================================================

'ИЗМЕНЕНИЯ И ДОПОЛНЕНИЯ:

'	[*]	Объединены в один скрипты GotoFormula, GotoMdTreeItem
'	(имена процедур сохранены для совместимости с Навигацией и др.)
'	[*]	Переработан код
'	[*]	Общие процедуры переделаны через CommonScripts
'	[*]	Добавлено следующее поведение GotoFormula:
'	Если в формуле присутствует "#" то появляется выбор предопределённых процедур ПриОткрытии,
'	ПриЗаписи, ПриЗакрытии, для документов также ОбработкаПроведения и ОбработкаУдаленияПроведения.
'
'	[*]	#ЗАГРУЗИТЬИЗФАЙЛА / глобальные / предопределённые / процедуры из модуля проведения -
'	работают в любом сочетании
'
'	[*]	Добавлен опциональный параметр поиска в ГМ если в текущем модуле процедуры не обнаружено
'	(по умолчанию выключен, устанавливается в процедуре Init)
'	[*] Вроде бы стало работать с длинными формулами
'
'	[*]	Если формула совпадает с шаблоном "ТипМетаданного.ВидМетаданного" то при наличии такого объекта
'	в конфигурации происходит переход к нему.
'	В связи с этим реализован следующий механизм:
'	Если в окне свойств элемента никакой элемент не сфокусирован то ищется закладка "Тип"
'	и в ней читается текст из комбобокса с типом.
'		В общем, текст формулы считывается из любого сфокусированного элемента окна свойств
'	(Формула, идентификатор, тип, Тип Значения (в диалоге свойств реквизита документа например))
'	если такого нет то пробуем определить тип и перейти к нему
'	(ещо проще, по хоткею на закладке "Тип" переходим к дереву МД)
'
'	[*]	Макрос GoToMDTreeItem исправлен для работы со всеми типами метаданных
'
'	[*]	Также добавлен небольшой скрипт устанавливающий фокус ввода в поле поиска
'	в тулбаре "Стандартная" (например, Ctrl+F без открытия диалога поиска)

'22.04.05
'	[+] GotoFormula : добавлен поиск формулы из неактивного пришпиленного окна свойств с помощью АртВин.Вин
'	[*] GotoMDTreeItem :
'	- переделана на Опенконф 1.0.1.9
'	- добавлен третий параметр ShowChilds - имеет значение только если передали путь
'	  состоящий из двух частей (например "Справочник.Номенклатура") И переданное
'	  метаданное имеет подуровни (реквизиты, шапка, значения)  - вызывается список этих подуровней
'	  иначе сразу осуществляется переход
'04.05.05
'	[+] переход к типу выделенного элемента в дереве МД
'
'05.05.05
'	[+] отображение типа реквизитов в дереве(SelectInTree) выбранного элемента МД, отключается в Init - flShowTypes
'
'28.05.05
'	[*] GotoFormula: Получение формулы c помощью нового функционала OpenConf 1.0.2.0
'
'29.05.05
'	[+] Для процедур типа:
'	Функция ПриВыбореЗначения(Об="",ДопПарам="")
'		// Рекурсивная Функция обработки всех экранных объектов.
'		// CASE По реквизитам документа
'		Если Об="" Тогда Об=Форма.АктивныйЭлемент(); КонецЕсли;
'
'		Если Об="ДатаДок" Тогда
'			...
'		ИначеЕсли Об="Склад" Тогда
'			...
'		ИначеЕсли Об="Номенклатура" Тогда
'			...
'		Конецесли
'	КонецФункции
'	Добавлено следующее поведение:
'	Если в первых 15 строках - не комментариях тела процедуры есть "=Форма.АктивныйЭлемент()"
'	и идентификатор активного  элемента формы например = "Номенклатура"	тогда
'	осуществляется переход к строке вида "ИначеЕсли Об="Номенклатура""
'	отключается в Init - flTryGotoActiveElementStatement
'
'10.06.05
'	[*] Поправлено рег. выражение для распознавания строк, например:
'	Если (Об="Пк1") ИЛИ (Об="Пк2") ИЛИ (Об="спВидовТехКарт") И (Пар = 10) Тогда
'
'21.06.05
'	[+] Для формул вида "ОткрытьФорму("Справочник.Номенклатура",Конт)"
'	Осуществляется переход к дереву МД к "Справочник.Номенклатура"
'	[+]в макросе SelectMetadataAndGotoObj убран из списка вариант "Форма.Модуль" там где он ненужен,
'      добавлена проверка, открыто ли окно конфигурации, если нет, то открывается.
'
'18.07.05
'	[+] Новый макрос GotoControlWithFormula, обратный GotoFormula
'	берёт слово под курсором и ищет все контролы где в формуле есль это слово
'	выдаёт список этих контролов, и при выборе активизирует его на форме
'
'17.03.06
'	[+] Новый макрос GotoLastSelectedMDTreeItem
'	переход к элементу в дереве метаданных, который был выделен перед вызовом макроса
'	GoToSelectedMDTreeItemType
'	удобно повесить на Alt+Left (переход назад по стеку телепата), если открыто дерево метаданых
'	выполнится этот макрос, иначе стандартное действие телепата
'==================================================================================================


Const WM_CLOSE = &H10
Const WM_GETTEXT = &HD
Const WM_SETTEXT = &H000C
Const WM_COMMAND =  &H111
Const WM_LBUTTONDOWN =  &H0201
Const WM_SETFOCUS = &H0007

Const BM_CLICK = &H00F5

Const TVM_GETNEXTITEM = 4362
Const TVM_SELECTITEM  = 4363
Const TVM_EXPAND      = 4354

Const TVE_EXPAND = 2
Const TVE_TOGGLE = 3

Const TVGN_ROOT =  0
Const TVGN_NEXT =  1
Const TVGN_CHILD = 4
Const TVGN_CARET = 9
Const GW_CHILD = 5
Const EM_SETSEL = &H00B1

Const cmdOpenCfgWindow   = 33188 'открыть окно конфигурации
Const cmdOpenPropWindow  = 32844 'открыть окно свойств элемента
Const cmdOpenEditWindow  = 33711 'открыть окно редактирования элемента
Const cmdEditDescription = 33718 'открыть окно редактирования описания элемента
Const cmdSaveAsExternal  = 33239 'открыть диалог "Сохранить как внешний..."(для отчётов и обработок)

Dim flSearchInGM 'флаг дополнительного поиска в ГМ (отключается в процедуре Init)
Dim flShowTypes  'показывать типы реквизитов (отключается в процедуре Init)
Dim flTryGotoActiveElementStatement 'искать в теле процедуры "Форма.АктивныйЭлемент" (отключается в процедуре Init)

Dim Wrapper
Dim MDDict
Dim strLastMDTreeSelection
Dim svc


'=========================================================================================
'GOTOFORMULA
'==========================================================================================
Function GoToFormula()
	GoToFormula = false
	Formula = GetFormula()
	Identifier = GetActiveCtrlIdentifier()
	'message "ActiveCtrlIdentifier: " & Identifier
	if Formula="" then Exit Function

	'stop
	if GotoMdTreeItem (Formula, 0, false) then
  		GoToFormula = true
  		exit Function
  	end if

  	if TryToOpenForm (Formula) then
  		GoToFormula = true
  		exit Function
  	end if

	set doc = CommonScripts.GetTextDocIfOpened(1)
	if doc is nothing then exit Function

	If Instr(Formula,"#") > 0 then
 		list = "ПриЗакрытии()" & vbCrLf & "ПриОткрытии()" 	& vbCrLf & "ПриЗаписи()"
 		if Instr(doc.Name,"Документ.") = 1 then
 			list = list & vbCrLf & "ОбработкаПроведения()" & vbCrLf & "ОбработкаУдаленияПроведения()"
 		end if
		'Formula = CommonScripts.SelectValue(list)
		Formula = svc.FilterValue(list,1 + 256 + 512,"",0,0,1)
 		if Formula = "" then exit Function

 		if Formula = "ОбработкаПроведения()" or Formula = "ОбработкаУдаленияПроведения()" then
			CurrDocName = split(doc.Name,".")
			if not IsArray(CurrDocName) then exit Function
			if UBound(CurrDocName) < 1 then exit Function
			set doc = Documents(CurrDocName(0) & "."  & CurrDocName(1) & ".Модуль документа")
			doc.Open
 		end if
	End If

	if Instr(Formula,"(") = 0 then Formula = Formula & "()" 'Exit Sub

	FirstLine = doc.Range(0)
	If UCase(left(FirstLine,18)) = "#ЗАГРУЗИТЬИЗФАЙЛА " then
		Name = trim(mid(FirstLine,19))
    	If left(name,2)="\\" or mid(name,2,1)=":" Then
			On Error Resume Next
			set doc = Documents.Open(Name) 'абсолютный путь
			On Error GoTo 0
			if doc is nothing then message "Ошибка загрузки файла из директивы #ЗАГРУЗИТЬИЗФАЙЛА" : exit Function
		Else
			On Error Resume Next
			set doc = Documents.Open (IBDir & Name) ' путь тносительно каталога ИБ
			On Error GoTo 0
			if doc is nothing then message "Ошибка загрузки файла из директивы #ЗАГРУЗИТЬИЗФАЙЛА" : exit Function
		End If
	End If

	If Left(UCase(Formula),2) = "ГЛ" Then ' если это глобальная процедура/функция
		set doc = Documents("ГлобальныйМодуль")
		doc.Open
	End If

    Formula = Left(Formula,Instr(Formula,"(")-1) 'имя процедуры/функции без скобок

	If Instr(Formula,"[") > 0 Then 'уберём знак "[" (если это, например, шаблон в отчёте)
		Formula = Trim(Replace(Formula,"[",""))
	End If

	if Formula="" then Exit Function

	'Если внутри есть точка, то наверное это вызывается метод класса?
	If Instr(Formula, ".") > 0 Then
		Set Navigation = Scripts("Навигация")
		'doc.ActivePage = "Модуль"
		If Navigation.TryClassMethodJump(Formula) Then Exit Function
		'Exit Function
	End If

	Status "Поиск процедуры " & Formula & " ..."
	pos = CommonScripts.FindProc(doc,Formula)
	Status ""
	if pos > -1 then
		doc.MoveCaret pos, 0, pos, doc.LineLen(pos)
		TryGotoActiveElementStatement doc,Identifier
	else
		if flSearchInGM then
			set doc1 = Documents("ГлобальныйМодуль")
			Status "Дополнительный поиск процедуры " & Formula & "в ГЛОБАЛЬНОМ МОДУЛЕ..."
			pos1 = CommonScripts.FindProc(doc1,Formula)
			Status ""
			if pos1 > -1 then
				doc1.Open
				doc1.MoveCaret pos1, 0, pos1, doc1.LineLen(pos1)
				GoToFormula = true
				exit Function
			end if
		end if

		CreateProcedure doc,Formula
	end if
	GoToFormula = true
End Function

'==========================================================================================
Private Function GetActiveCtrlIdentifier()
	GetActiveCtrlIdentifier = ""
	Set Form = Nothing
	On error resume next
	set Form = Windows.ActiveWnd.Document.Page(0)
	on error goto 0

	if Form is Nothing then exit function

	Set Identifier = Nothing
	On error resume next
	Identifier = Form.ctrlProp(cInt(Form.Selection),cpStrID)
	on error goto 0

	if Identifier is Nothing then exit function

	GetActiveCtrlIdentifier = Identifier
End Function

'==========================================================================================
Private Function TryToOpenForm(Formula)
	TryToOpenForm = false
	patt = "открытьформу\(""([\wА-я¬и\d]+\.[\wА-я¬и\d]+)"""
	set Matches = CommonScripts.RegExpExecute(patt,Formula)
	if not Matches is Nothing then
		For Each Match In Matches
			GotoMdTreeItem Match.SubMatches(0), 0, false
			TryToOpenForm = true
			exit for
		Next
	end if
End Function

'==========================================================================================
Private Function TryGotoActiveElementStatement(doc,Identifier)
	if not flTryGotoActiveElementStatement then exit function
	if Identifier = "" then exit function
	cnt = 0
	l = doc.SelStartLine + 1

	patt = "([\wА-я¬и\d]+)\s*=\s*Форма\.АктивныйЭлемент\(\)"
	while cnt < 15
		sLine = doc.Range(l)
		if CommonScripts.RegExpTest("конецпроцедуры|конецфункции",sLine) then exit function
		if CommonScripts.RegExpTest("^(\s*)[\r\n]*$|^(\s*//.*)",sLine) then
			'message "sLine " & sLine
			l=l+1
		else
			set Matches = CommonScripts.RegExpExecute(patt, sLine)
			'message "try to find active element statement"
			'message "sLine " & sLine
			if not Matches is Nothing then
				For Each Match In Matches
					param = Match.SubMatches(0)
					exit for
				Next
				'message "found param " & param
				'message "in string " & sLine
				cnt = 16
			else
				'message "param not found"
				cnt=cnt+1
			end if
		end if
	Wend

	for i = l to doc.LineCount
		sLine = doc.Range(i)
	    if CommonScripts.RegExpTest("конецпроцедуры|конецфункции",sLine) then exit function
		patt = "Если\s+[ -я]*\(*" & param & "\s*=\s*""" & Identifier & """[ -я]*\s+Тогда"
		if CommonScripts.RegExpTest(patt,sLine) Then
			'message sLine
			doc.MoveCaret i, 0, i, doc.LineLen(i)
			exit for
		end if
	next
End Function

'==========================================================================================
Private Function GetFormula()
	GetFormula = ""
	Title = Space(20)
	Formula = Space(128)
	ForegroundWnd = Wrapper.GetForegroundWindow

 	Wrapper.Register "USER32.DLL",   "SendMessage",    "I=lllr", "f=s", "r=l"
	'cnt = Wrapper.SendMessage(ForegroundWnd, WM_GETTEXT ,20,Title) ' (заголовок окна)
	Title = svc.GetWindowText(ForegroundWnd)

	Title = UCase(CStr(Title))
	If Left(Title,9)="СВОЙСТВА " Then 'если это окно свойств
    	ActiveControl = Wrapper.GetFocus 'элемент окна свойств с фокусом ввода

		Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
		TabControl = Wrapper.FindWindowExA (ForegroundWnd,0,"SysTabControl32",NULL) 'TabControl с закладками

		'если имеется фокус ввода то считываем текст из него иначе пытаемся найти закладку "Тип"
		if (ActiveControl = ForegroundWnd) or (ActiveControl = TabControl) then
    		Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llls", "f=s", "r=l"
			TypeTab = Wrapper.FindWindowExA(ForegroundWnd,0,NULL,"Тип ") 'диалог закладки "Тип"

			if TypeTab = 0 then
				Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
	  			Wrapper.SendMessage ForegroundWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
  				Exit Function
			end if
			Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
			TypeCBBox = Wrapper.FindWindowExA(TypeTab,0,"COMBOBOX",NULL)
			if TypeCBBox = 0 then
				Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
	  			Wrapper.SendMessage ForegroundWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
  				Exit Function
			end if
			'cnt = Wrapper.SendMessage (TypeCBBox, WM_GETTEXT ,128, Formula) 'получим текст из комбобокса
			Formula = svc.GetWindowText(TypeCBBox) 'получим текст из комбобокса
		else
			'cnt = Wrapper.SendMessage (ActiveControl, WM_GETTEXT ,128, Formula) '  прочитаем формулу
			Formula = svc.GetWindowText(ActiveControl)
		end if

		GetFormula =  Trim(Cstr(Formula))

		Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llss", "f=s", "r=l"
	  	OKButtonHandle = Wrapper.FindWindowExA(ForegroundWnd,0,"Button","Обновить") 'найдём кнопку "Обновить"
	  	Wrapper.SendMessage OKButtonHandle, BM_CLICK ,0 ,0 'нажмём на неё
	 	Wrapper.SendMessage ForegroundWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств

	ElseIf ForegroundWnd=Windows.MainWnd.HWND Then 'если это окно конфигуратора
		If Windows.ActiveWnd.Document <> docWorkBook Then Exit Function
		If Windows.ActiveWnd.Document.ActivePage <> 0 Then Exit Function

		If Version >= 1020 Then
			set Form = Windows.ActiveWnd.Document.Page(0)
			Formula = Form.ctrlProp(cInt(Form.Selection),cpFormul)
			'если в формуле ничего нет, пробуем взять идентификатор контрола
			if Trim(Formula) = "" then Formula = GetActiveCtrlIdentifier()
		end if

		if Trim(CStr(Formula)) = "" then
			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
			Wrapper.SendMessage Windows.MainWnd.HWND, WM_COMMAND ,cmdOpenPropWindow, NULL 'откроем свойства элемента

			PropWnd = Wrapper.GetForegroundWindow 'Окно свойств элемента

			if PropWnd = Windows.MainWnd.HWND then 'окно свойств открыто но неактивно
				Set WinFinder = CreateObject("ArtWin.Win")
				PropWnd = WinFinder.Find("Свойства ")
				Wrapper.Register "USER32.DLL",   "GetClassName",    "I=lrl", "f=s", "r=l"
				ClassName = Space(128)
				cnt = Wrapper.GetClassName(PropWnd, ClassName, 128)
				if Left(cstr(ClassName), cnt) <> "#32770" then PropWnd = 0
			end if

			if PropWnd = 0 then Exit Function

			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=lllr", "f=s", "r=l"
			'cnt = Wrapper.SendMessage(PropWnd, WM_GETTEXT ,20,Title) ' (заголовок окна)
			Title = svc.GetWindowText(PropWnd)
			Title = UCase(CStr(Title))

			If Left(UCase(Title),9) <> "СВОЙСТВА " then
				Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
		  		Wrapper.SendMessage PropWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
		  		Exit Function
	  		End If

			Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
			TabControl = Wrapper.FindWindowExA (PropWnd,0,"SysTabControl32",NULL) 'найдём TabControl с закладками

			'нажмём на "Дополнительно" (вместо повторного открытия в пред. версии)
			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
			Wrapper.SendMessage TabControl,WM_LBUTTONDOWN,0,cLng(125 + 10 * &H10000)

			Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llls", "f=s", "r=l"
			DopTab = Wrapper.FindWindowExA(PropWnd,0,NULL,"Дополнительно") 'диалог закладки "Дополнительно"

			if DopTab = 0 then 'нет закладки "Дополнительно"
				Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
		  		Wrapper.SendMessage PropWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
				Exit Function
			end if

			Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
			FormulaEdit = Wrapper.FindWindowExA(DopTab,0,"EDIT",NULL)	'хэндл поля ввода формулы

			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=lllr", "f=s", "r=l"
			'cnt = Wrapper.SendMessage (FormulaEdit, WM_GETTEXT ,128, Formula) 'получим текст формулы
            Formula = svc.GetWindowText(FormulaEdit)
	    	if Trim(Cstr(Formula)) = "" then
				Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
		  		Wrapper.SendMessage PropWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
				Exit Function
			End If

			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
		  	Wrapper.SendMessage PropWnd, WM_CLOSE ,NULL, NULL 'закроем окно свойств
	  	'Else
		' 	set Form = Windows.ActiveWnd.Document.Page(0)
		'	Formula = Form.ctrlProp(cInt(Form.Selection),cpFormul)
	  	End If

  		GetFormula = Trim(CStr(Formula))
	End If
End Function

'========================================================================
Private Sub CreateProcedure(Doc,Formula)
	ProcOrFunc = InputBox("Процедура или Функция" & vbCrLF & Formula & " - не обнаружена." & vbCrLF & vbCrLF & _
	"Что создавать: 1 - Процедуру, 2 - Функцию ", "Введите значение", "1")

	If ProcOrFunc="1" then
		ReplValue1 = "Процедура " : ReplValue2 = "КонецПроцедуры // "
	ElseIf ProcOrFunc="2" then
		ReplValue1 = "Функция " : ReplValue2 = "КонецФункции // "
	Else
		Exit Sub
	End If

	ModuleText = split(doc.Text, vbCrLf)
	If not IsArray(ModuleText) then exit sub
	if UBound(ModuleText) > 0 Then
		doc.MoveCaret UBound(ModuleText), 0 'спустимся в конец модуля
		for i = doc.SelStartLine to 0 step -1 'найдём последнюю процедуру/функцию
			sText = lTrim(UCase(ModuleText(i)))
			if Instr(sText,"КОНЕЦПРОЦЕДУРЫ") = 1 or Instr(sText,"КОНЕЦФУНКЦИИ") = 1 then
				doc.MoveCaret i, 0
				Exit For
			end if
		next

		If i+1=doc.LineCount then 'если конец модуля, то добавим строку
			doc.Range(i,Len(sText),i,Len(sText))=vbCrLF
			doc.MoveCaret i+1, 0
		Else
			doc.MoveCaret i+1, 0
		End If
	End If

	Text=vbCrLF & "//" & String(70,"=") & vbCrLF & _
	ReplValue1 & Formula & "()" & vbCrLF & vbTab & vbCrLF & _
	ReplValue2 & Formula & "()" & vbCrLF
	doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelEndLine, doc.SelEndCol) = Text
	doc.MoveCaret i+4, 1
End Sub

'========================================================================
'GOTOMDTREEITEM
'========================================================================
'Справочник.Номенклатура
Private Function BuildTree(MDO,level)
	txt = ""
	For i = 1 To level
    	txt = txt & vbTab
	Next
	if flShowTypes then
		on error resume next
		sType = " [" & mdo.Type.Fullname & "]"
		on error goto 0
		if sType = " []" then sType = ""
	end if
	txt =  txt + mdo.Name & sType & "|e" & vbCrLf
	Set Childs = mdo.Childs
	For i = 0 To Childs.Count - 1
		txt = txt & vbTab & Childs.Name(i, True) & vbCrLf
		Set mda = Childs(i)
		For j = 0 To mda.Count - 1
			txt = txt + BuildTree (mda(j), level + 2)
		Next
	Next
	BuildTree = txt
End Function

'========================================================================
Private Function PrepareMDPath(Path) ', flDeleteInvisibleChilds
	PrepareMDPath = Path

	PathParts = Split(Path,"\")

	ItemType = UCase(PathParts(0))

	If ItemType = "КОНСТАНТА"      then
		PathParts(0) = "Константы"

	ElseIf ItemType = "СПРАВОЧНИК"     then
		PathParts(0) = "Справочники"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(2) = Replace(PathParts(2),"Реквизит","#")
			'end if
		end if

	ElseIf	ItemType = "ДОКУМЕНТ"       then
		PathParts(0) = "Документы"
		if UBound(PathParts) > 1 then
			PathParts(2) = Replace(PathParts(2),"РеквизитШапки","Шапка")
			PathParts(2) = Replace(PathParts(2),"РеквизитТабличнойЧасти","Табличная часть")
		end if
	ElseIf	ItemType = "ЖУРНАЛ"         then
		PathParts(0) = "Журналы документов"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(2) = Replace(PathParts(2),"Графа","#")
			'end if
		end if

	ElseIf ItemType = "ПЕРЕЧИСЛЕНИЕ"   then
		PathParts(0) = "Перечисления"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(2) = Replace(PathParts(2),"Значение","#")
			'end if
		end if

	ElseIf ItemType = "ОТЧЕТ"          then
		PathParts(0) = "Отчеты"

	ElseIf ItemType = "ОБРАБОТКА"      then
		PathParts(0) = "Обработки"

	ElseIf ItemType = "ПЛАНСЧЕТОВ"     then
		PathParts(0) = "Планы Счетов"

	ElseIf ItemType = "ВИДСУБКОНТО"    then
		PathParts(0) = "Виды Субконто"

	ElseIf ItemType = "ОПЕРАЦИЯ"       then
		PathParts(0) = "Операция"

	ElseIf ItemType = "ПРОВОДКА"       then
		PathParts(0) = "Проводка"

	ElseIf ItemType = "РЕГИСТР"        then
		PathParts(0) = "Регистры"
		if UBound(PathParts) > 1 then
			PathParts(2) = Replace(PathParts(2),"Измерение","Измерения")
			PathParts(2) = Replace(PathParts(2),"Ресурс","Ресурсы")
			PathParts(2) = Replace(PathParts(2),"Реквизит","Реквизиты")
		end if

	ElseIf ItemType = "ЖУРНАЛРАСЧЕТОВ" then
		PathParts(0) = "Журналы расчетов"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(2) = Replace(PathParts(2),"Реквизит","#")
			'end if
		end if

	ElseIf ItemType = "ВИДРАСЧЕТА"     then
		PathParts(0) = "Виды расчетов"

	ElseIf ItemType = "ГРУППАРАСЧЕТОВ"  then
		PathParts(0) = "Группы расчетов"

	ElseIf ItemType = "КАЛЕНДАРЬ"      then
		PathParts(0) = "Календари"

	ElseIf ItemType = "НУМЕРАТОР"      then
		PathParts(0) = "Документы\Нумераторы"

	ElseIf ItemType = "ПОСЛЕДОВАТЕЛЬНОСТЬ"      then
		PathParts(0) = "Документы\Последовательности"

	ElseIf ItemType = "ОБЩИЙРЕКВИЗИТДОКУМЕНТА"      then
		PathParts(0) = "Документы\Общие реквизиты"

	'   --------------
	'- обратный перевод -
	'   --------------
	ElseIf ItemType = "КОНСТАНТЫ"      then
		PathParts(0) = "Константа"

	ElseIf ItemType = "СПРАВОЧНИКИ"     then
		PathParts(0) = "Справочник"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(1) = PathParts(1) & "\Реквизит"
			'end if
		end if

	ElseIf ItemType = "ДОКУМЕНТЫ"       then
		PathParts(0) = "Документ"
		if UBound(PathParts) > 1 then
			PathParts(2) = Replace(PathParts(2),"Шапка","РеквизитШапки")
			PathParts(2) = Replace(PathParts(2),"Табличная часть","РеквизитТабличнойЧасти")
		end if

	ElseIf ItemType = "ЖУРНАЛЫ ДОКУМЕНТОВ" then
		PathParts(0) = "Журнал"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(1) = PathParts(1) & "\Графа"
			'end if
		end if

	ElseIf ItemType = "ПЕРЕЧИСЛЕНИЯ"   then
		PathParts(0) = "Перечисление"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(1) = PathParts(1) & "\Значение"
			'end if
		end if

	ElseIf ItemType = "ОТЧЕТЫ"          then
		PathParts(0) = "Отчет"

	ElseIf ItemType = "ОБРАБОТКИ"      then
		PathParts(0) = "Обработка"

	ElseIf ItemType = "ПЛАНЫ СЧЕТОВ"     then
		PathParts(0) = "Плансчетов"

	ElseIf ItemType = "ВИДЫ СУБКОНТО"    then
		PathParts(0) = "Видсубконто"

	ElseIf ItemType = "ОПЕРАЦИЯ"       then
		PathParts(0) = "Операция"

	ElseIf ItemType = "ПРОВОДКА"       then
		PathParts(0) = "Проводка"

	ElseIf ItemType = "РЕГИСТРЫ"        then
		PathParts(0) = "Регистр"
		if UBound(PathParts) > 1 then
			PathParts(2) = Replace(PathParts(2),"Измерения","Измерение")
			PathParts(2) = Replace(PathParts(2),"Ресурсы","Ресурс")
			PathParts(2) = Replace(PathParts(2),"Реквизиты","Реквизит")
		end if

	ElseIf ItemType = "ЖУРНАЛЫ РАСЧЕТОВ" then
		PathParts(0) = "Журналрасчетов"
		if UBound(PathParts) > 1 then
			'if flDeleteInvisibleChilds = true then
				PathParts(1) = PathParts(1) & "\Реквизит"
			'end if
		end if

	ElseIf ItemType = "ВИДЫ РАСЧЕТОВ"     then
		PathParts(0) = "Видрасчета"

	ElseIf ItemType = "ГРУППЫ РАСЧЕТОВ"  then
		PathParts(0) = "Группарасчетов"

	ElseIf ItemType = "КАЛЕНДАРИ"      then
		PathParts(0) = "Календарь"

	ElseIf ItemType = "НУМЕРАТОРЫ"      then
		PathParts(0) = "Нумератор"
	End If

	If UBound(PathParts) > 0 then
		If PathParts(1) = "Последовательности"  then
			PathParts(1) = "Последовательность"
		ElseIf PathParts(1) = "Общие реквизиты"      then
			PathParts(1) = "ОбщийРеквизитДокумента"
		End If
	End If

	If UBound(PathParts) = 0 then
		Path = PathParts(0)
	else
		Path = Join(PathParts,"\")
	end if

	Path =  Replace(Path,"\#\","\")
	PrepareMDPath = Path
End Function

'========================================================================
'дебуг
'Константа.ПоказыватьАртикул
'Справочник.Номенклатура
'Справочник.ЗначенияСвойств
'Документ.ПКО
'Журнал.ЖурналОбщий
'Перечисление.КодыОпераций
'Отчет.АнализПродаж
'Обработка.ДеревоДокументов
'Счет.Основной
'Плансчетов.Основной
'ВидСубконто.ИсточникиПоступления
'Регистр.Касса
'Регистр.Резерв
'ЖурналРасчетов.Зарплата
'ВидРасчета.ВытесняемаяДоплата05
'ГруппаРасчетов.НерегистрируемыеВНачисленииУдержании
'Календарь.Сменный12
'========================================================================
Function GoToMDTreeItem(ByVal Path,Action,ShowChilds)
	GoToMDTreeItem = false

	if Version < 1019 then
		message "Требуется версия Опенконфа не менее 1.0.1.9"
		exit Function
	end if

	Path = Replace(Path,".","\")

	if Right(Path,1) = "\" then Path = Left(Path,Len(Path)-1)

	ItemType = ""
	ItemKind = ""

	if InStr(Path,"\") > 0 then
		PathParts = Split(Path,"\")
		ItemType = UCase(PathParts(0))
		ItemKind = UCase(PathParts(1))
	else
		ItemType = Path
	end if

	if ItemType = "СЧЕТ" then ItemType = "ПЛАНСЧЕТОВ"


	set tmp = nothing
	On Error Resume Next
	if ItemKind = "" then
		Set tmp = MetaData.TaskDef.Childs(CStr(ItemType))
	else
		Set tmp = MetaData.TaskDef.Childs(CStr(ItemType))(CStr(ItemKind))
	end if
	On Error Goto 0
	if tmp is nothing then	exit Function


	if Instr("СПРАВОЧНИК,ДОКУМЕНТ,ЖУРНАЛ,ПЕРЕЧИСЛЕНИЕ,РЕГИСТР,ЖУРНАЛРАСЧЕТОВ",ItemType) = 0 then
		'только у этих объектов есть подуровни
		Exit Function
	end if

	if ShowChilds = true then
		if IsArray(PathParts) then
			if UBound(PathParts) = 1 then
				if Instr("СПРАВОЧНИК,ДОКУМЕНТ,ЖУРНАЛ,ПЕРЕЧИСЛЕНИЕ,РЕГИСТР,ЖУРНАЛРАСЧЕТОВ",ItemType) <> 0 then
					'у этих объектов есть подуровни
					Vals =  BuildTree(MetaData.TaskDef.Childs(CStr(ItemType))(CStr(ItemKind)),0)
					Vals = ItemType & "\" & Vals
					if Instr("СПРАВОЧНИК,ЖУРНАЛ,ЖУРНАЛРАСЧЕТОВ",ItemType) <> 0 then
						pos = Instr(Vals,"ФормаСписка") 'уберём из дерева Формы Списков
						Vals = Left(Vals,pos-1)
					end if
					Path = svc.SelectInTree(Vals,"",False,False)
					if Instr(Path,"[") > 0 then	Path = Left(Path,Instr(Path,"[")-1)
					if Path = "" then
						GoToMDTreeItem = true
						Exit Function
					end if
				end if
			end if
		end if
	end if

	Path = PrepareMDPath(Path) ',true
	MDWnd.DoAction Path, Action

	GoToMDTreeItem = true
End Function


'========================================================================================
Function GoToSelectedMDTreeItemType()
	GoToSelectedMDTreeItemType = false
	if Instr(Windows.ActiveWnd.Caption,"Конфигурация")=1 then
		if MDWnd.ActiveTab = 0 then
			path = MDWnd.GetSelected()
			strLastMDTreeSelection = path
			path = PrepareMDPath(path)
			if InStr(1,path,"ОбщийРеквизитДокумента") > 0 then Path = Replace(Path,"Документ\","")
			parts = Split(Path,"\")
			on error resume next
			set mdo = MetaData.TaskDef.Childs(CStr(Parts(0)))(CStr(Parts(1))).Childs(CStr(Parts(2)))(CStr(Parts(3)))
			itemtype = mdo.Props("Тип") & "." & mdo.Props("Вид")
			if itemtype = vbEmpty then
				Err.Clear
				set mdo = MetaData.TaskDef.Childs(CStr(Parts(0)))(CStr(Parts(1)))
				itemtype = mdo.Props("Тип") & "." & mdo.Props("Вид")
			end if
			if Err.Number = 0 then
				GotoMDTreeItem itemtype, 0, false
				GoToSelectedMDTreeItemType = true
			end if
			on error goto 0
		end if
	end if
End Function

'========================================================================================
' выбор нужного метаданного из списка метаданных и переход к нему
'========================================================================================
Sub SelectMetadataAndGotoObj()
	on Error resume next
	Set childs = MetaData.TaskDef.childs

	For i = childs.Count - 1 To 0 Step -1
		Set mda = childs(i)
		For j = mda.Count - 1 To 0 Step -1
			lname = mda(j).Name
			If Err.Number = 0	then
				'vals = vals & vbCrLf & childs.Name(i, True) & "." &  mda(j).Name
				str = childs.Name(i, True) & "." &  mda(j).Name
				If not MDDict.Exists(str) then MDDict.Add str, ""
			else
				Err.Clear
			end if
		Next
	Next

	Scripts("ChangeKbdLayout").ActivateRusLayout()
	on Error goto 0

	vals = ""
	keys = MDDict.Keys
	for i = MDDict.Count - 1 to 0 step -1
		vals = vals & keys(i) & vbCrLf
	next

	'fStr = CommonScripts.SelectValue(Vals,"")
	fStr = svc.FilterValue(Vals,1 + 512,"",0,0,1)


	if fStr = "" then Exit Sub

	MDDict.Remove fStr
	MDDict.Add fStr, ""

	'кусок из "Навигации"
	WhatToOpen = "Форма.Модуль"
	NameToOpen = fStr
	ListForms = WhatToOpen
	Str = NameToOpen
	ObjType = Mid(fStr,1,Instr(fstr,".")-1)
	ObjName = Mid(fStr,Instr(fstr,".")+1)

	If ObjType = "Справочник" or ObjType = "Журнал" Then
		ListForms = Scripts("Навигация").GetListForms(ObjType, ObjName, WhatToOpen)
		If ObjType = "Справочник" Then
			ListForms = WhatToOpen & "," & Replace(WhatToOpen, "Форма.", "Форма группы.") & "," & ListForms
		ElseIf ObjType = "Журнал" Then
			ListForms = ListForms
		Else
			ListForms = WhatToOpen & "," & ListForms
		End If
	ElseIf ObjType = "Документ" Then
		ListForms = ListForms & "," & "Модуль Документа"
	End If

	'If Instr("Перечисление,Регистр",ObjType) > 0 Then
	'	ListForms = Replace(ListForms,"Форма.Модуль","")
	'End If

	Set Doc = Nothing
	On Error Resume Next
	Set Doc = Documents(ObjType & "." & ObjName & ".Форма.Модуль")
	On Error Goto 0
	if doc is Nothing then
		ListForms = Replace(ListForms,"Форма.Модуль","")
	end if

	List = ListForms & ",Найти в дереве,Свойства,Редактировать"
	List = Replace(List, ",", vbCrLf)

	'WhatToOpen = CommonScripts.SelectValue(List, fStr, vbCrLf, true)
	WhatToOpen = svc.FilterValue(List, 1 + 32 + 512, fStr,0,0,1)
	If WhatToOpen <> "" Then
		If not IsConfigWndOpen Then
			Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
			Wrapper.SendMessage Windows.MainWnd.HWND, WM_COMMAND ,cmdOpenCfgWindow, NULL
		end if
		if WhatToOpen = "Найти в дереве" then
			GoToMDTreeItem fStr, 0, false

		elseif	WhatToOpen = "Свойства" then
			GoToMDTreeItem fStr, mdaProps, false

		elseif	WhatToOpen = "Редактировать" then
			GoToMDTreeItem fStr, cmdOpenEditWindow, false

		else ModuleName = NameToOpen & "." & WhatToOpen
			Documents(ModuleName).Open
			Set Doc = Documents(ModuleName)
			If Not Doc Is Nothing Then
				If Doc.IsOpen Then
					nLine = CommonScripts.FindProc(Doc, "ПриОткрытии", "ПриЗаписи","ОбработкаПроведения")
					If nLine < 0 Then nLine = 0
					CommonScripts.Jump nLine, -1, -1, -1, ModuleName
				End If
			End If
		end if
	End If
End Sub ' SelectMetadataAndGotoObj

'========================================================================================
'ACTIVATEFINDEDIT  (Ctrl + F)
'========================================================================================
Sub ActivateFindEdit()
	Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llsl", "f=s", "r=l"
	combo = Wrapper.FindWindowExA(Windows.MainWnd.Hwnd,0,"AfxControlBar42",NULL)

	Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llls", "f=s", "r=l"
	combo = Wrapper.FindWindowExA(combo,0,NULL,"Стандартная")

	Wrapper.Register "USER32.DLL",   "FindWindowExA",  "I=llss", "f=s", "r=l"
	combo = Wrapper.FindWindowExA(combo,0,"ComboBox","")

	Wrapper.Register "USER32.DLL",   "SendMessage",    "I=llll", "f=s", "r=l"
	Wrapper.SendMessage combo,WM_SETFOCUS,0,0

	'установим в окно поиска текст из буфера
	Wrapper.Register "USER32.DLL",   "GetWindow",    "I=ll", "f=s", "r=l"
	edit = Wrapper.GetWindow(combo,GW_CHILD)

	ClipBrdText = CommonScripts.GetFromClipboard
	svc.SetWindowText edit, ClipBrdText
	Wrapper.SendMessage edit, EM_SETSEL, 0, len(ClipBrdText)

	'Set doc = CommonScripts.GetTextDoc(true,2)
	'if doc is nothing then exit sub
	'doc.MoveCaret 0,0,0,0
End Sub

'========================================================================================
'GOTOCONTROLWITHFORMULA
'========================================================================================
Function GotoControlWithFormula_(quiet)
	GotoControlWithFormula_ = false

	If Windows.ActiveWnd.Document <> docWorkBook Then Exit Function
	If Windows.ActiveWnd.Document.ActivePage <> 1 Then Exit Function

	set Form = Windows.ActiveWnd.Document.Page(0)
	set doc = Windows.ActiveWnd.Document.Page(1)
	CurrWord = doc.CurrentWord

	if CurrWord = "" then exit Function

	Set FrmDict = CreateObject("Scripting.Dictionary")
	for i = 0 to Form.ctrlCount - 1
		Formula = Form.ctrlProp(i,cpFormul)

		if Instr(Formula,CurrWord) > 0 then
			Type_ = Form.ctrlType(i)
			Capt = Form.ctrlProp(i,cpTitle)
			strID = Form.ctrlProp(i,cpStrID)
			ControlString = Trim(cStr(i) & " - " & Type_ & ": " & Capt & " Ид: " & strID)
			FrmDict.Add ControlString, i
		End If
	next

	if FrmDict.Count = 0 then
		If not quiet Then
			msgbox "Формула """ & CurrWord & """ не задействована в элементах управления формы!++ " & quiet
		End If
		Exit Function
	end if

	if FrmDict.Count > 1 then
		ControlPos = CommonScripts.SelectValue(FrmDict)
	else
		ControlPos = FrmDict.Item(ControlString)
	end if

	if ControlPos = "" then exit Function

	Windows.ActiveWnd.Document.ActivePage = 0

	Form.LayerVisible(Form.ctrlProp(ControlPos,cpLayer)) = true

	Form.Selection = cStr(ControlPos)

	GotoControlWithFormula_ = true
End Function

Function GotoControlWithFormula()
	GotoControlWithFormula = GotoControlWithFormula_(false)
End Function


'========================================================================================
'GotoControlOrFormula
'
' В зависимости от контекста (форма или модуль) выполняется переход
'		- если активна форма(диалог),  выполняется переход
'				на формулу текущего элемента (GotoFormula)
'		- если активен модуль,  выполняется переход на элемент,
'				в формуле которого есть текущая процедура (GotoControlWithFormula)
'========================================================================================
Sub GotoControlOrFormula()
	If Windows.ActiveWnd.Document <> docWorkBook Then Exit Sub

	If Windows.ActiveWnd.Document.ActivePage = 0 Then
		GotoFormula

	ElseIf Windows.ActiveWnd.Document.ActivePage = 1 Then
		GotoControlWithFormula

	end if
End Sub

'======================================================================
'вешать на Alt+Left
'======================================================================
Sub GotoLastSelectedMDTreeItem()
	set doc = Windows.ActiveWnd.Document
	if not doc is nothing then
		if (doc.Type = 0) and (doc.Name = "CMDTabDoc::Конфигурация") then
			if (MDWnd.ActiveTab = 0) and (strLastMDTreeSelection <> "") then
				MDWnd.DoAction strLastMDTreeSelection, 0
			else
				CancelHotKey = true
			end if
		else
			CancelHotKey = true
		end if
	else
		CancelHotKey = true
	end if
End Sub

'========================================================================================
'INIT
'========================================================================================
Private Sub Init()
    Set c = Nothing
    On Error Resume Next
    Set c = CreateObject("OpenConf.CommonServices")
    On Error GoTo 0
    If c Is Nothing Then
        Message "Не могу создать объект OpenConf.CommonServices", mRedErr
        Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    c.SetConfig(Configurator)
	SelfScript.AddNamedItem "CommonScripts", c, False

	Set Wrapper = CreateObject("DynamicWrapper")

	Wrapper.Register "USER32.DLL",   "GetFocus",                 "f=s", "r=l"
	Wrapper.Register "USER32.DLL",   "GetForegroundWindow",      "f=s", "r=l"
	Wrapper.Register "USER32.DLL",   "GetParent",         "I=l", "f=s", "r=l"

	flSearchInGM = true 'false/true
	flShowTypes = true 'false/true
	flTryGotoActiveElementStatement = true 'false/true
	Set MDDict = CreateObject("Scripting.Dictionary")
	Set svc = CreateObject("Svcsvc.Service")
End Sub

'========================================================================================
Init 'При загрузке скрипта выполняем инициализацию