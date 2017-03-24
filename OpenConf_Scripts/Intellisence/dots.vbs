'dots.vbs
'
'Версия: $Revision: 1.16 $
'
'Intellisence для OpenConf
'	(© trdm)	Трошин Д. В.
'Соавторы:
'	Артур Аюханов aka artbear
'
'Расширения файлов, для которых не нужно применять интеллисенс
Const NotIntellisenceExtensions = ".vbs.js.wsc.prm"

const cnstNameTypeSZ = "СписокЗначений"
const cnstNameTypeTZ = "ТаблицаЗначений"
const cnstNameTypeTabl = "Таблица"
const BagKatch		= False 'Ловим букашек?
const cnstRExWORD = "a-zA-Zа-яА-Я0-9_"
const KakVibiraem = 2
FindFirstInFindInStrEx = False
'1 - Выбираем с пом svcsvc.dll SelectValue
'2 - Выбираем с пом svcsvc.dll FilterValue
'3 - Выбираем с пом SelectValue.dll FilterValue

'Словарь в котором содержатся объекты внешних компонент и
'файлы их словарей, для получения атрибутов
Set vk_dict_CreateObject = CreateObject("Scripting.Dictionary")

'Парсим ГМ для вытяжки типов глобальных переменных  0 = еще не парсили 1 = парсили и не работали в нем  (для исключения репарсинга)
GlobalModuleParse = 0
' Тут мы храним списки глабальных переменных и их типов в формате  Переменная = Тип.Вид# Переменная = Тип.Вид#
Set GlobalVariableType = CreateObject("Scripting.Dictionary")
Set GlobalVariableNoCase = CreateObject("Scripting.Dictionary")
Set FSO = CreateObject("Scripting.FileSystemObject")

Set UniMetodsAndRekv = CreateObject("Scripting.Dictionary")

Dim GlobalModule 'Объект TheModule для Глобального модуля
Dim LocalModule	 'Объект TheModule для модуля текущего объекта
Dim glDoc		 'Объект который нужно отсканировать (иногда в процедурах скеанирования нужно перейти из одного модуля в другой)
glDoc = ""
Dim glStartLileEnd 'Верхушка до которой сканируем текст модуля

ttextModule = ""
ttextModuleGM = ""
ttextModuleGM_PriNachRabSys = ""
Dim GRegExp, regEx, Match, Matches, glRWord

' Содержит параметры, а то их так много накопилось, что голова кругом...
Class AWord					'Анализируемое слово
	Public RW				'Очищенное от всех лишних символов слово

	Public RWAll			'Первоначальное слово для определения надо ли ставить точку перед выбором

	Public RWOld			'Результирующая строка, которую надо распознать, возможно с точкой...
	Public RWFullStr		'Полная строка текста, в анализируемой строке, служит например для того,
							'что-бы убирать точку с запятой...
	Public TypeVid			'результирующий тип/вид слова
	Public TypeVid2			'Дополнительный тип/вид слова например: Документ+ТаблицаЗначений
	Public AddWord			'Массив слов, от первого типизированного до конечного у которого все запрашиваем...

	Public AddTypeVid		'Массив типов для этих слов.

	Public BTObj			'Объект токена, чей метод запрашивается
	Public BTMeth			'Метод или массив методов, чец объект запрашивается..
	Public BTNumberParams	'Номер параметра, в котором запрашивается слово...
	Public IsBetweenToken	'Запрашиваемая позиция находится в скобках
	Public IsIcvalVid		'Идет сравление слова с видами чегото
	Public IsNeedBrasket	'Нужны скобки (для фразы Переменная.Вид() = "!!")

	Public AsCreateObj		'Нужны скобки (для фразы Переменная.Вид() = "!!")


	Public RecvOfForms		'переменная - реквизит формы

	Public LineText			'строка текста в которой вызвали
	Public LineStartToCaret	'строка текста с начала и до курсора
	Public LineCaretToEnd	'строка текста с курсора и до конца

	Public FindAtribute		'(искать атрибуты) флаг отмены поиска, если он проставлен, значит уже ясно, что нам надо...

	Public TempArray		'Временный масссив для всяких манипуляций.....

	Public doc_SelStartLine		'Строка документа, где начались разборки
	Public doc_CurParseLine		'Текущая обрабатываемая строка


	Public doc_IsObject			'Объект документа Докусент/Справочник
	Public doc_ObjectType		'Тип объекта документа
	Public doc_ObjectVid		'Вид объекта документа
	Public doc_ObjectHaveAtrib	'Объект документа имеет атрибуты

	'Массивы...
	Public ArrProcFuncFromTheWord	'Процедуры и функции в которых возможно есть инициализация объекта...
								'Пример глНайтиВнешниеПечатныеФормыОбъекта
	Public ArrProcFuncNumbParam	'Номер параметра для процедуры или функции.
								'Если значение возвернуто функцией - тогда здесь "-1"


	Private Sub Class_Initialize
		doc_IsObject = false
		doc_ObjectHaveAtrib = false

		RW  = ""
		RWAll	= ""
		RWOld = ""
		RWFullStr = ""
		AddWord = "" :	AddTypeVid = "" : BTObj = "" : BTMeth = "" :
		TypeVid = ""
		TypeVid2=""
		BTNumberParams = ""
		IsBetweenToken = false
		IsIcvalVid = false
		IsNeedBrasket = false
		FindAtribute = true
		AsCreateObj = false

		RecvOfForms	= 0

		LineText			= ""
		LineStartToCaret	= ""
		LineCaretToEnd		= ""
		doc_IsObject		= false
		doc_ObjectType		= ""
		doc_ObjectVid		= ""
		doc_ObjectHaveAtrib	= ""
		doc_SelStartLine	= 0
		doc_CurParseLine	= 0

		ProcFuncFromTheWord = ""
		ArrProcFuncFromTheWord	= ""
		ArrProcFuncNumbParam	= ""

	End Sub

	Sub GetDocInfo(doc)
		DocName = doc.Name
		Arr = Split(DocName,".")
		doc_SelStartLine	= doc.SelStartLine
		doc_CurParseLine	= 0
		if UBound(Arr)>0 Then
			if instr(1,"/документ/справочник/отчет/обработка/журнал/",lcase(Arr(0)))>0 Then
				doc_IsObject = true
				doc_ObjectType = Arr(0)
				doc_ObjectVid = Arr(1)
				if instr(1,"/документ/справочник/журнал/",lcase(Arr(0)))>0 Then
					doc_ObjectHaveAtrib = true
				End IF
			End IF
		End IF
	End Sub
End Class

'Класс для переменной
Class TheVariable
	Public V_Vid	'Вид переменной
	Public V_Type	'Тип переменной
	Public Words	'Массив дополнительных переменных
	Public WordsCnt	'Размер массива слов

	Function Verify() ' проверить инициализацию..
		Verify = false
		If (Len(V_Vid)>0) And (Len(V_Type)>0) Then
			Verify = true
		End If
	End Function

	Function VerifyAgr() ' проверить инициализацию на агрегатные объекты
		VerifyAgr = false
		If (Len(V_Type)>0) Then
			if InStr(1,"/документ/справочник/регистр/перечисление/отчет/обработка/","/" & V_Type & "/") Then
				VerifyAgr = true
			End If
		End If
	End Function

	Sub ExtractDef(ttext)
		ttext = lcase(ttext)
		if Len(ttext)> 0 Then
			if InStr(1, ttext, ".")	> 0 Then
				Words = Split(ttext,".")
			Else
				Words = Array(ttext)
			End If
			WordsCnt = UBound(Words)
			if WordsCnt<>-1 Then
				V_Type = Words(0)
			End If
			if WordsCnt>0 Then
				V_Vid = Words(1)
			End If
		End If

	End Sub
	Private Sub Class_Initialize
		V_Vid = ""
		V_Type = ""
	End Sub

End Class


Dim StartScanLineText	'Откуда начинаем сканировать текст....
Dim GlobalStr			'Где продолжаем сканировать текст....

Dim SoobshitType 'Сообщить тип/вид вместо того, чтобы выдать список реквизитов/методов......


'*********************************************************************
' Оболочка для WordOfCaret(), позволяет переключить режим подстановки
' вычисленного выражения или показать его значения....
Sub TypeWordOfCaret()
	SoobshitType = True :	WordOfCaret()	:	SoobshitType = False
End Sub

'Индексировать ли модули для поиска поцедур и функций...
Dim IndexModule
IndexModule = True
'IndexModule = False ' - для отладки, чтобы не ждеть каждый раз когда модуть отсчитается...

'Индексировать ли модули для поиска поцедур и функций...
Dim NeedCheckModule
NeedCheckModule = True
'NeedCheckModule = False 'Отключаем эту возможность...

Dim SuccessfulWork
Function IsSuccessfulWork()
	WordOfCaret()
	IsSuccessfulWork = SuccessfulWork
End Function

'*********************************************************************
'Ключевая процедура
Sub WordOfCaret()
	Set RWord = new AWord
	Set var = new TheVariable
	glDoc = "" : glStartLileEnd = -1
	SuccessfulWork = false
	Doc = ""
	If Not CheckWindow(Doc) Then
		Exit Sub
	End IF

	IF NeedCheckModule Then
		SyntaxCheckModule()
	End IF
'stop
	Rword.GetDocInfo(doc)
	'Глобальная переменая RWord, пока не переработаем все модули для параметрирования...
	Set glRWord = RWord

	'еще раз проверим, не находимся ли мы в глобальнике и установим флаг сканирования
	'локального текста если побывали в Глобальнике ....
	Configurator_OnActivateWindow Windows.ActiveWnd,true

	StartScanLineText	= doc.SelStartLine
	IF (GlobalModuleParse = 0) And IndexModule Then
		ParseGlobalModule(doc)
	End If
	'Выделяем само слово или фразу с точками
	RWord.RW = GetWordFromCaret(RWord.RWAll,RWord.RWFullStr,RWord.LineCaretToEnd,  RWord)
	If Not (RWord.IsIcvalVid) Then
		RWord.IsBetweenToken = BetweenToken(doc.SelStartCol,RWord.BTObj, RWord.BTMeth, RWord.BTNumberParams, doc,RWord)
		if RWord.IsBetweenToken Then
			If Len(RWord.BTMeth)>0 Then
				RWord.IsBetweenToken = CheckIsBetweenToken(RWord.BTMeth, RWord.BTNumberParams, RWord, doc)
				If RWord.IsBetweenToken Then
					If Len(RWord.BTObj)<>0 Then
						RWord.RW		= RWord.BTObj
						RWord.RWAll	= ""
					End If
				End If
			ElseIf (Len(RWord.BTMeth)=0) And (Len(RWord.RW)=0) Then
				Exit Sub
			End If
		End If
	End If
	if (RWord.RW = "?") Then
		Exit Sub
	End If
	LocalModule	= ""
	If Doc.Name <> "Глобальный модуль" Then
		Set LocalModule = New TheModule
		LocalModule.SetDoc(Doc)
		LocalModule.InitializeModule(1)
	Else
		If Not IsEmpty(GlobalModule) Then
			Set LocalModule = GlobalModule
		End If
	End If

	GlobalStr = Doc.SelStartLine - 1
	RWord.RW = Replace(RWord.RW,vbCr,".")
	LastTipeVid = ""
	aroff = 0

	'Простое распознавание (поиск реквизиты объекта которому принадлежит модуль...)
	TypeVid1 = GetTypeVid(RWord.RW,LastTipeVid, Doc, RWord.RecvOfForms,RWord.RW)
	TypeVid  = TypeVid1
	IF Len(TypeVid1)>0 And Len(RWord.AddWord)>0 Then
		TypeVid1 = GetTypeVid(RWord.AddWord,TypeVid1, Doc, RWord.RecvOfForms,RWord.RW)
	End If
	If SimpleType(TypeVid1) Then
		Status RWord.RWAll & "->" & TypeVid1
		Exit Sub
	End If
	TypeVid  = TypeVid1
	NameOfTableFromTable = ""
	'Вот это - OR (lCase(TypeVid) = "документ") OR (lCase(TypeVid) = "справочник")
	'Это когда данный элемент на форме и имеет вид справочник или документ
	If (len(TypeVid) = 0) OR (lCase(TypeVid) = "документ") OR (lCase(TypeVid) = "справочник")  Then	'Значит не получилось :(
		' Попробуем отпарсить текст
		if RWord.RW <> "!" Then
			TypeVid = GetTypeFromTextRegExps(RWord.RW,RWord.AddWord)
			If Len(TypeVid) = 0 And Len(glRWord.TypeVid)<>0 Then
				TypeVid = glRWord.TypeVid
			End If
			If ((lCase(TypeVid1) = "документ") OR (lCase(TypeVid1) = "справочник")) And (Len(TypeVid) = 0) Then 'And InStr(1,TypeVid,"#")>0 Then
				TypeVid = TypeVid1
			End If
		End If
	End If

	IF InStr(1,UC("/константа/документ/справочник/Журнал/Перечисление/Отчет/Обработка/ПланСчетов/Регистр/"),UC(RWord.RW))>0 And (Len(TypeVid)=0) Then
		TypeVid = RWord.RW
		RWord.FindAtribute = False
		if Len(RWord.AddWord)> 0 Then
			TypeVid = GetTypeVid(RWord.AddWord,RWord.RW, Doc, RWord.RecvOfForms,RWord.RW)
		End If
	End If
	IF InStr(1,TypeVid,"+")>0 Then
		aroff = Split(TypeVid,"+")
		RWOrd.TypeVid  = aroff(0)
		RWOrd.TypeVid2  = aroff(1)
		TypeVid = aroff(0)
		TypeVid1 = aroff(1)
	End If

	ttext = ""
	strRekv =""
	if (Len(ttext) = 0) And (Not RWord.IsBetweenToken) then
		IF Len(TypeVid) = 0 Then
			If ((INStr(1,UCase(Doc.Name),UCase("CWBModuleDoc::"))>0) And (INStr(1,UCase(Doc.Name),UCase("ert"))>0)) _
			OR (INStr(1,UCase(Doc.Name),UCase("Отчет."))>0) OR (INStr(1,UCase(Doc.Name),UCase("Обработка."))>0) Then 'Внешний отчет
				if RWord.RW = "!" Then
					'TypeVid = "ФОРМА"
				End IF
			End IF
		Else
			ttt = Split(TypeVid, ".")
			if UBound(ttt) = 2 Then
				TypeVid = GetTypeVid(ttt(2),ttt(0)&"."&ttt(1), Doc, RWord.RecvOfForms,ttt(2))
			End IF
		End IF
	End IF
	IF SoobshitType Then
		IF Len(TypeVid)>0 Then
			MsgBox TypeVid
		Else
			MsgBox "Не определен :("
		End If
		Exit Sub
	End If
	if ((instr(1,"/строка/число/дата/", lcase(TypeVid))>0) Or (Len(TypeVid) = 0)) And (Not RWord.IsBetweenToken) And (Len(RWord.RW) = 0) Then
		If Len(TypeVid) = 0 Then exit sub
		MsgBox TypeVid
		exit sub
	End If

	If (Len(TypeVid)<>0) And (RWord.FindAtribute) And Not (Ucase(RWord.BTMeth) = UCase("Цвет")) And Not RWord.IsIcvalVid Then
		IF Instr(1,TypeVid,".")>0 Then
			Arr = Split(TypeVid,".")
			if UBound(Arr) = 1 Then
				SMetodami = 1
				if (RWord.IsBetweenToken) OR (rword.IsIcvalVid) Then
					SMetodami = 0
				End IF
				strRekv = GetStringRekvizitovFromObj(Arr(0), Arr(1),SMetodami,RWord.RecvOfForms, RWord.BTMeth,	RWord.BTNumberParams, RWord)
				if Len(strRekv)<>0 Then
					ttext = SelectFrom(strRekv, Caption)
					IF Len(ttext) = 0 Then
						SuccessfulWork = true
						Exit Sub
					End If
				End If
			End If
		Else
			IF RWord.IsBetweenToken Then
				SMethodami = 0
			else
				SMethodami = 1
			End If
			TypeVid1 = TypeVid
			IF InStr(1,TypeVid, "#КолонкаТаблицыЗначений#")>0 Then
				SMethodami = 0
				ArrTtext2 = Split(TypeVid,"#")
				RWord.RW = ArrTtext2(0)
				TypeVid = "ТаблицаЗначений"
			End If

			strRekv = GetMethodAndRekvEx(TypeVid, RWord.RW, SMethodami,RWord.RecvOfForms, RWord.IsBetweenToken, NameOfTableFromTable)
			If Lcase(TypeVid) = "документ" Then
				strRekv0 = GetORD()
				if lcase(RWord.RW) = "документ" Then
					Set MetaDoc  = MetaData.TaskDef.Childs(CStr(TypeVid))
					For tt = 0 To MetaDoc.Count-1
						strRekv0 = strRekv0 &  vbCrLf & MetaDoc(tt).Name
					Next
				End If
				strRekv = strRekv0 & vbCrLf & strRekv
			End If
			if len(strRekv) <> 0 Then
				IF InStr(1,TypeVid1, "#КолонкаТаблицыЗначений#")>0 Then
					Arr = Split(strRekv,vbCrLf)
					If UBound(Arr)<>-1 Then
						strRekv = ""
						For cntWords = 0 To UBound(Arr)
							if cntWords = 0 Then
								strRekv = " = """ & Arr(cntWords) & """"
							Else
								strRekv = strRekv & vbCrLf & " = """ & Arr(cntWords) & """"
							End If
						Next
					End If
				End If
				ttext = SelectFrom(strRekv, Caption)
				ttext = PodrabotkaVibora(strRekv, ttext)
				if len(ttext) = 0 Then
					SuccessfulWork = true
					Exit Sub
				End If
			End If
		End If
	End If
	IF RWOrd.FindAtribute And Not SoobshitType And Not RWord.IsIcvalVid Then
		IF (UCase(RWord.RW) = UCase("Константа")) Then
			ttext = GetConstantExA()
		End If

		IF RWord.IsBetweenToken And Len(ttext) = 0 Then
			IF (Ucase(RWord.BTMeth) = UCase("Цвет")) Or (Ucase(RWord.BTMeth) = UCase("Color")) Then
				ttext = CStr(VibColor)
			End If
		End If


		if (Len(ttext) = 0) And (Right(RWord.RWAll,1)<>".") And (Not RWord.IsBetweenToken) And (UCASE(TypeVid) = "ФОРМА") then
			ArrDocPresent = Split(Doc.Name,".")
			IF UBound(ArrDocPresent)>=2 Then
				IF (UCase(ArrDocPresent(0)) = "ДОКУМЕНТ") OR (UCase(ArrDocPresent(0)) = "СПРАВОЧНИК") Then
					strRekv = GetStringRekvizitovFromObj(ArrDocPresent(0), ArrDocPresent(1),1,1, RWord.BTMeth,	RWord.BTNumberParams, RWord)
					if Len(strRekv)<>0 Then
						tType = ""
						If EtoFormaDokumenta(tType) Then
							if tType = "Документ" Then
								strRekv = strRekv & vbCrLf &  "* только агрегатные"
								strRekv = strRekv & vbCrLf &  "* только агрегатные общие"
								strRekv = strRekv & vbCrLf &  "* только агрегатные шапка"
								strRekv = strRekv & vbCrLf &  "* только агрегатные таблица"
								strRekv = strRekv & vbCrLf &  "* только НЕ агрегатные"
								strRekv = strRekv & vbCrLf &  "* словарь"
							Else
								strRekv = strRekv & vbCrLf &  "* только агрегатные"
								strRekv = strRekv & vbCrLf &  "* только НЕ агрегатные"
								strRekv = strRekv & vbCrLf &  "* словарь"
							End If
						End If
						ttext = SelectFrom(strRekv, Caption)
						ttext = PodrabotkaVibora(strRekv, ttext)
						if Len(ttext) = 0 Then
							SuccessfulWork = true
							Exit Sub
						End If
					End If
				End IF
			End IF
		End IF

		If RWord.IsBetweenToken And (Len(ttext) = 0) Then '2004.12.03 Добавим реакцию на ОткрытьПодбор
			RWord.RWAll  = ""
			IF UC(RWord.BTMeth) = UC("ОткрытьПодбор") Then
				If InStr(1, UC(RWord.BTObj),UC("Справочник."))>0 Then
					ArrBTObj = Split(RWord.BTObj,".")
					If UBound(ArrBTObj) = 1 Then
						Set MetaRef  = MetaData.TaskDef.Childs(CStr(ArrBTObj(0)))(CStr(ArrBTObj(1)))
						Set CHI = MetaRef.Childs
						IF CHI.Item(1).Count<> 0 Then
							strRekv = ""
							For iii = 0 To CHI.Item(1).Count - 1
								iF Len(strRekv) = 0 Then
									strRekv = CHI.Item(1).Item(iii).Name
								Else
									strRekv = strRekv & vbCrLf & CHI.Item(1).Item(iii).Name
								End IF
							Next
							if Len(strRekv)<>0 Then
								ttext = SelectFrom(strRekv, Caption)
								if Len(ttext) = 0 Then
									SuccessfulWork = true
									Exit Sub
								End If
							End If
						End IF
					End IF
				End IF
			ElseIF (UC(RWord.BTMeth) = UC("ПравоДоступа")) Or (UC(RWord.BTMeth) = UC("AccessRight")) Then
				If UCase(RWord.TypeVid) = UCase("#РеквизитФормы#") Then
					'Возъмем реквизиты формы
				End IF
			ElseIF (UC(RWord.BTMeth) = UC("ПравоДоступа")) Or (UC(RWord.BTMeth) = UC("AccessRight")) Then
				IF RWord.BTNumberParams = 1 Then
					strRekv = GetStrAccessRight()
					ttext = SelectFrom(strRekv, Caption)
					if Len(ttext) = 0 Then
						SuccessfulWork = true
						Exit Sub
					End If
				End IF
			ElseIF UC(RWord.BTMeth) = UC("Итог") Or UC(RWord.BTMeth) = UC("Total") Then
				if (RWord.RW = "!") And (RWord.doc_ObjectType = "Документ") Then
'Stop
					TekObj = "" : strRekv = ""
					If Not GetTekObj(TekObj) Then
						Exit Sub
					End If
					Set ch = TekObj.Childs("РеквизитТабличнойЧасти")
					For cnmr = 0 To ch.Count-1
						Set MetaRekv = ch(cnmr)
						if MetaRekv.Props(9) = "1" Then	 'MetaRekv.Props.Name(9)	"ИтогПоКолонке"	String
							AddToString strRekv, MetaRekv.Name,vbCrLf
						End IF
					Next
					ttext = SelectFrom(strRekv, Caption)
					if Len(ttext) = 0 Then
						SuccessfulWork = true
						Exit Sub
					End If
				End IF
			End IF
		End IF
	End If
	If Len(ttext) = 0 Then '{ trdm 2005 01 22
		If InStr(1,UC("/константа/документ/справочник/Журнал/Перечисление/Отчет/Обработка/ПланСчетов/Регистр/"),UC("/"&RWord.RW&"/")) Or (RWord.IsIcvalVid)   Then
		'If (UC(RWord.RW) =  UC("отчет")) OR (UC(RWord.RW) =  UC("обработка")) Or (RWord.IsIcvalVid)   Then
			ttext = RWord.RW
			if RWord.IsIcvalVid Then
				if Len(TypeVid)>0 Then
					var.ExtractDef(TypeVid)
					if var.VerifyAgr() Then
						ttext = var.V_Type
					End If
				End If
			End If
			if Len(ttext)>0 Then
				var.ExtractDef(ttext)
				if var.VerifyAgr() Then
					strRekv = ""
					if var.VerifyAgr() And (Lcase(var.V_Type) = "перечисление") Then
						'Set MetaDoc  = MetaData.TaskDef.Childs(CStr(var.V_Type))(CStr(var.V_Vid))
						strRekv = GetStringRekvizitovFromObj(var.V_Type, var.V_Vid,0,0, "",	"", RWord)
					Else
						Set MetaDoc  = MetaData.TaskDef.Childs(CStr(ttext))
						For tt = 0 To MetaDoc.Count-1
							IF Len(strRekv) = 0 Then
								strRekv = MetaDoc(tt).Name
							Else
								strRekv = strRekv &  vbCrLf & MetaDoc(tt).Name
							End If
						Next
					End If
					if Len(strRekv)>0 Then
						ttext = SelectFrom(strRekv, Caption)
						if Len(ttext) = 0 Then
							SuccessfulWork = true
							Exit Sub
						End If
						' Если у нас ttext - выбранное значение перечисления, тады дополняем фразу так:
						if var.VerifyAgr() And (Lcase(var.V_Type) = "перечисление") Then
							ttext = "Перечисление." & var.V_Vid &"."& ttext
						End If
					End If
				Else
					ttext = ""
				End If
			End If
		End If
	End If ' }trdm 2005 01 22

	if Len(ttext) = 0  then
		StrObjekta = ""
		If RWord.AsCreateObj Then
			StrObjekta = Get_file_vk_dict(TypeVid)
		End IF
		for fff = 0 To 4
			if fff = 0 Then strRekv = LoadMethodFromFile(StrObjekta)
			if fff = 1 Then strRekv = LoadMethodFromFile(RWord.RWFullStr)
			if fff = 2 Then strRekv = LoadMethodFromFile(RWord.RW)
			if fff = 3 Then strRekv = LoadMethodFromFile(RWord.BTMeth)
			if fff = 4 Then strRekv = LoadMethodFromFile(TypeVid)

			If Not IsEmpty(strRekv) Then
				strRekv = Trim(strRekv)
				strRekv = Replace(strRekv,"(f)","(<?>)")
				ttext = SelectFrom(strRekv, Caption)
				if len(ttext) = 0 Then
					SuccessfulWork = true
					Exit Sub
				Else
					TypeVid = ""
				End If
				Exit for
			End If
		Next
	End If

	if Len(ttext)<>0 then
		DotIsEpsent = False
		IcvalPrezent = False
		IF InStr(1, ttext, "=") > 0 Then 'Если у нас выражение типа "= Перечисление.ВидыТоваров.Услуга" здесь точка не нужна...
			IcvalPrezent = True
		End IF
		RWord.LineCaretToEnd = DelKomment(RWord.LineCaretToEnd)
		RWord.LineCaretToEnd = Trim(RWord.LineCaretToEnd)
		if Len(RWord.LineCaretToEnd)>0 Then
			If InStr(1,ttext,";")>0 Then
				ttext = Replace(ttext,";","")
			End If
		End If
		IF (Not RWord.IsBetweenToken) And (RWord.RW <> "!") And Not IcvalPrezent And not RWord.IsIcvalVid Then
			if Len(RWord.RWAll)>0 Then 'Проверим точку
				If Not (Mid(RWord.RWOld,Len(RWord.RWOld))=".") Then
					If (RWord.RWAll<>"!") And (Not RWord.IsBetweenToken) Then
						DotIsEpsent = True
						ttext = "." & ttext
					end if
				end if
			end if
			if Not DotIsEpsent Then
				LinefromText = doc.range(doc.SelStartLine)
				RWord.RWFullStr = mid(LinefromText,1,doc.SelStartCol)
				RWord.RWFullStr = Trim(RWord.RWFullStr)
				ls = Len(RWord.RWOld)
				if Mid(RWord.RWOld,ls,1) <> "." Then
					ttext = "." & ttext
				end if
			end if
		end if
		if RWord.IsIcvalVid Then
			'Если у нас сравнивается перечисление и мы попытаемся заменить текст типа "перечисление.вид" на
			' возможно существующую глобальную переменную этого (перечисление.вид) вида
			if (StrCountOccur(ttext, ".") = 2) And (StrCountOccur(lcase(ttext),"перечисление.") = 1) Then
				ExtraktGlobalVariableName ttext
			end if
			if RWord.IsNeedBrasket Then
				If Not ((Mid(trim(RWord.LineStartToCaret),Len(trim(RWord.LineStartToCaret)),1) = """") OR (Mid(trim(RWord.LineCaretToEnd),1,1) = """")) Then
					ttext = """" & ttext & """"
				end if
			end if
		end if

		Pos = InStr(ttext,"<?>")
		if InStr(ttext,"<?>") = 0 Then
			Pos = len(ttext)
		else
			Pos = Pos - 1
		end if
		ttext = Replace(ttext, "<?>", "")
		doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelEndLine,doc.SelEndCol ) = ttext
		'doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelStartLine ,doc.SelStartCol) = ttext
		doc.MoveCaret doc.SelStartLine, doc.SelStartCol+Pos
		Status "dots"
		SuccessfulWork = true
	End If
	Status RWord.RWAll & "->" & TypeVid

end sub

Function GetNextMetaDataChild(TheWord, ReturnWord)
end Function
Function GetMetaObjectChild(TheWord, ReturnWord)
end Function


Private Function PodrabotkaVibora(strRekvIn, ttext)
	PodrabotkaVibora = ttext
	strRekvIn = "#" & Replace(strRekvIn, vbCrLf,"#") & "#"
	strRekv = ""
	SformirovaliZdes = False
	If InStr(1, ttext, "*")>0 Then
		PodrabotkaVibora = ""
		If InStr(1, ttext, "только")>0 Then
			TekObj = ""
			If Not GetTekObj(TekObj) Then
				Exit function
			End If
			If Instr(1,TekObj.FullName,"Документ.") Then
				For tt = 0 To 2
					if tt=0 Then Set ch = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
					if tt=1 Then Set ch = TekObj.Childs("РеквизитШапки")
					if tt=2 Then Set ch = TekObj.Childs("РеквизитТабличнойЧасти")
					For cnmr = 0 To ch.Count-1
						Set MetaRekv = ch(cnmr)
						If ttext = "* только НЕ агрегатные" Then
							strRekvIn = ReplaceEx(strRekvIn, Array("#" & MetaRekv.Name & "#" ,"#", "#ДатаДок#" ,"#", "#НомерДок#" ,"#", "#Операция#" ,"#"))
						else
							AddedText = ""
							If ttext = "* только агрегатные" Then
								AddedText = MetaRekv.Name
							ElseIf (ttext = "* только агрегатные общие") And (tt = 0) Then
								AddedText = MetaRekv.Name
							ElseIf (ttext = "* только агрегатные шапка") And (tt = 1) Then
								AddedText = MetaRekv.Name
							ElseIf (ttext = "* только агрегатные таблица") And (tt = 2) Then
								AddedText = MetaRekv.Name
							End IF
							If Len(AddedText)>0 Then
								If InStr(1,strRekvIn,"#" & AddedText & "#") > 0 Then
									AddToString strRekv, MetaRekv.Name,vbCrLf
								End IF
							End IF
							SformirovaliZdes = True
						End IF
					Next
				Next
			ElseIf Instr(1,TekObj.FullName,"Справочник.") Then
				Set ch = TekObj.Childs("Реквизит")
				For cnmr = 0 To ch.Count-1
					Set MetaRekv = ch(cnmr)
					If ttext = "* только НЕ агрегатные" Then
						strRekvIn = Replace(strRekvIn, "#" & MetaRekv.Name & "#" ,"#")
					else
						AddedText = ""
						If ttext = "* только агрегатные" Then
							AddedText = MetaRekv.Name
						End IF
						If Len(AddedText)>0 Then
							If InStr(1,strRekvIn,"#" & AddedText & "#") > 0 Then
								AddToString strRekv, MetaRekv.Name,vbCrLf
							End IF
						End IF
						SformirovaliZdes = True
					End IF
				Next
			End If
		ElseIf ttext = "* словарь" Then
			SformirovaliZdes = True
			strRekv = LoadSlovar()
			strRekv = Trim(strRekv)
		End If
	Else
		Exit Function
	End If
	if Not SformirovaliZdes Then
		strRekvIn = ReplaceEx(strRekvIn, Array("#* только агрегатные#" ,"#", "#* только агрегатные общие#" ,"#", "#* только агрегатные шапка#" ,"#", "#* только агрегатные таблица#" ,"#", "#* только НЕ агрегатные#" ,"#"))
		Do While Instr(1,strRekvIn,"##")>0
			strRekvIn = Replace(strRekvIn, "##" ,"#")
		Loop
		strRekvIn = Mid(strRekvIn,2,Len(strRekvIn)-2)
		ArrRekv = Split(strRekvIn,"#")
		strRekvIn = ""
		If UBound(ArrRekv)<> -1 Then
			For tt = 0 To UBound(ArrRekv)
				If Not (InStr(1,ArrRekv(tt),"(")>0) Then
					AddToString strRekvIn, ArrRekv(tt),vbCrLf
				End If
			Next
		End If
	Else
		strRekvIn = strRekv
	End If
	If Len(strRekvIn)>0 Then
		ttext = SelectFrom(strRekvIn, Caption)
		if len(ttext) > 0 Then
			PodrabotkaVibora = ttext
		End If
	End If

end Function



Private Function AddToString(Str, AddStr,Spl)
	If Len(Str)>0 Then
		Str = Str & Spl & AddStr
	Else
		Str = AddStr
	End IF
	AddToString = Str
end Function

Private Function AddToStringUni(Str, AddStr,Spl)
	AddToStringUni = False
	If Len(Str)>0 Then
		ArrUniStr = Split(Str,Spl)
		For qq = 0 To UBound(ArrUniStr)
			if Trim(Lcase(ArrUniStr(qq))) = Trim(Lcase(AddStr)) Then
				Exit Function
			End IF
		Next
		Str = Str & Spl & AddStr
	Else
		Str = AddStr
	End IF
	AddToStringUni = True
end Function



Private Function GetTekObj(TekObj)
	GetTekObj = False
	Doc = ""
	If CheckWindow(Doc) Then
		NameDoc = Doc.Name
		ArrName = Split(NameDoc,".")
		If UBound(ArrName) > 1 Then
			if (ArrName(0) = "Документ") OR (ArrName(0) = "Справочник") Then
				Set TekObj = MetaData.TaskDef.Childs(CStr(ArrName(0)))(CStr(ArrName(1)))
				GetTekObj = True
			End If
		End If
	End If
End Function



Private Sub ParseGlobalModule(doc)
	GlobalModuleParse = 1 ' Типа отпарсили и нам повотрный парсинг не нужен..

	GlobalVariableType.RemoveAll()
	GlobalVariableNoCase.RemoveAll()
	Set DocGM = Documents("Глобальный модуль")
	ttextGM = DocGM.text
	if Doc.Name <> "Глобальный модуль" Then
		Set GlobalModule = New TheModule
		GlobalModule.SetDoc(DocGM)
		GlobalModule.InitializeModule(1)
	End IF

	IF Len(ttextGM) = 0 Then
		Exit Sub
	End IF
	ttextObjiavlVARIABLE = ""
	For iii = 0 To DocGM.LineCount 	'Вычисляем переменные
		ttext = DocGM.Range(iii)
		ttext = Trim(ttext)
		IF (InStr(1,lcase(ttext),"процедура") = 1) Or (InStr(1,lcase(ttext),"функция") = 1) or (InStr(1,lcase(ttext),"function") = 1 )or (InStr(1,lcase(ttext),"procedure") = 1 )  Then
			Exit For
		End IF
		ttext = DelKomment(ttext)
		If Len(ttext)>0 Then
			ttextObjiavlVARIABLE = ttextObjiavlVARIABLE &" "& ttext
		End IF
	Next
	ttextObjiavlVARIABLE = lcase(ttextObjiavlVARIABLE)
	ttextObjiavlVARIABLE = ReplaceEx(ttextObjiavlVARIABLE, Array(" экспорт","", vbTab&"экспорт","", "перем ","", "перем"&vbTab,"", ";",",", " ","", vbCrLf,"", vbCr,"", vbTab,"" ) )
	ttextObjiavlVARIABLE = trim(ttextObjiavlVARIABLE)
	ttextObjiavlVARIABLE = mid(ttextObjiavlVARIABLE,1,len(ttextObjiavlVARIABLE)-1)
	ArrGlobVARIABLE = split(ttextObjiavlVARIABLE,",")
	For iii = 0 To UBound(ArrGlobVARIABLE)
		IF Not GlobalVariableType.Exists(ArrGlobVARIABLE(iii)) Then
			GlobalVariableType.Add ArrGlobVARIABLE(iii),""
			GlobalVariableNoCase.Add ArrGlobVARIABLE(iii),""
		End IF
	Next
	' Список переменных заготовили, смотрим что у нас что
	' Начинаем разматывать от конца модуля секцию инициализации переменных

	ttextInicializeGM = ""
	For iii = DocGM.LineCount  To 0 Step -1
		ttext = DocGM.Range(iii)
		ttext = Trim(ttext)
		IF (InStr(1,lcase(ttext),"конецпроцедуры") = 1) Or (InStr(1,lcase(ttext),"конецфункции") = 1)  Then
			Exit For
		End IF
		ttext = DelKomment(ttext)
		if Len(ttext)>0 then
			ttextInicializeGM = ttext & vbCrLf & ttextInicializeGM
		End IF
	Next
	ttextModuleGM = ttextInicializeGM
	ttextInicializeGM = ReplaceEx(ttextInicializeGM, Array(vbTab,"",  " ",""))
	ParseTextFromGlobalVariable ttextInicializeGM, GlobalVariableType
	'Попарсим текст в отдельной процедуре

	'Теперь найдем процедуру ПриНачалеРаботыСистемы
	Patern = "(процедура)+[\s]+(приначалеработысистемы)+[\s]*[\(]+[^~]+(конецпроцедуры)+"
	ttext = FindInStrEx(Patern,ttextGM)
	ttextInicializeGM = ""
	ArrGlobVARIABLE = split(ttext,vbCrLf)
	For iii = 0 To UBound(ArrGlobVARIABLE)
		tt = ArrGlobVARIABLE(iii)
		tt = DelKomment(tt)
		if Len(ttext)>0 then
			ttextInicializeGM = ttextInicializeGM & vbCrLf & tt
		End IF
		IF (InStr(1,lcase(tt),"конецпроцедуры") = 1) Then
			Exit For
		End IF
	Next
	ttextModuleGM_PriNachRabSys = ttextInicializeGM
	ParseTextFromGlobalVariable ttextInicializeGM, GlobalVariableType
end sub

Private Sub ParseTextFromGlobalVariable(textForParse, Dict)
	ArrStrforParse = split(textForParse,vbCrLf)
	' Идея такая: нам передан кусок модуля (процедура, функция или секция инициализации модуля),
	' мы его просматриваем на предмет инициализации переменных.
	' Есть локальный список переменных, созданных в этом модуле, есть список глобальных
	' запоминаем атрибуты и тип вид локальной переменной, если идет присвоение глобальной,
	' фиксируем тип вид и если есть колонки.
	Set PNRSDict = CreateObject("Scripting.Dictionary") 'Локальный словарь
	for iii = 0 To ubound(ArrStrforParse)
		VarName = ""
		ttext = ArrStrforParse(iii)
		ttext = Replace(ttext,vbTab,"")

		if Len(ttext) > 1 Then
			posIcval = InStr(1,ttext,"=")
			if posIcval<>0 Then
				VarName = Mid(ttext,1,posIcval)
				VarName = Replace(VarName,"=","")
				VarNameNoCase = VarName
				VarName = lcase(VarName)
				Expr = mid(ttext,posIcval+1)
				Expr = lcase(Expr)
				' проанализируем Expr, если в нем только присвоение строки, нафиг нам это надо :(
				'а как - просто посмотрим что присваивается, если строка, тады нафиг
				If Not Mid(Expr,1,1) = """" Then
					IF InStr(1,VarName,".") = 0 Then
						If (InStr(1,Expr,"константа.") = 1) Then
							Expr = Replace(Expr,"константа.","")
							Expr = Replace(Expr,";","")
							On Error Resume Next
							set Obj = MetaData.TaskDef.Childs("Константа")(CStr(Expr))
							If len(Obj.Props(3))>0 Then 'Тип, если это константа неопределенного вида, нафиг она нужна
								If InStr(1,"справочник.перечисление.документ",lcase(Obj.Props(3)))>0 Then
									OstavimVariable = True
									If Obj.Props(4)> 0 Then
										If Dict.Exists(VarName) Then
											Dict.Remove(VarName)
											Dict.Add VarName, Obj.Props(3) &"."&Obj.Props(4)
											if GlobalVariableNoCase.Exists(VarName) Then
												GlobalVariableNoCase.Remove(VarName)
											End IF
											GlobalVariableNoCase.Add VarName, VarNameNoCase
											'Message "VarName6=" & VarName & " Expr = " & Obj.Props(3) &"."&Obj.Props(4)
										End IF
									End IF
								End IF
							End IF
						ElseIf  (InStr(1,Expr,"перечисление.") = 1) Then
							Expr = Replace(Expr,"перечисление.","")
							Expr = Replace(Expr,";","")
							If InStr(1,Expr,".") = 0 Then 'Оставим только фразы Перечисление.Такоето
								'On Error Resume Next
								IF Dict.Exists(VarName) Then
									Dict.Remove(VarName)
									Dict.Add VarName, "перечисление." & Expr
									'Message "VarName5=" & VarName & " Expr = " & "перечисление." & Expr
									if GlobalVariableNoCase.Exists(VarName) Then
										GlobalVariableNoCase.Remove(VarName)
									End IF
									GlobalVariableNoCase.Add VarName, VarNameNoCase
								Else
									If PNRSDict.Exists(VarName) Then
										PNRSDict.Remove(VarName)
									End IF
									PNRSDict.Add VarName, Expr
								End IF
							End IF
						ElseIf (InStr(1,Expr,"создатьобъект(") = 1) Or (InStr(1,Expr,"createobject(.") = 1) Then
							Expr = ReplaceEx(Expr, Array("создатьобъект(","",  "createobject(","", ";","", ")","", """","" ))
							ArrExprWord = Split(Expr,".")
							IF UBound(ArrExprWord) <> -1 Then
								If InStr(1,"таблицазначений.операция.списокзначений.бухгалтерскиеитоги.справочник.документ.регистр.addin.",lcase(ArrExprWord(0)))>0 Then
									if Dict.Exists(VarName) Then
										Dict.Remove(VarName)
										Dict.Add VarName, Expr
										if GlobalVariableNoCase.Exists(VarName) Then
											GlobalVariableNoCase.Remove(VarName)
										End IF
										GlobalVariableNoCase.Add VarName, VarNameNoCase
									Else
										If PNRSDict.Exists(VarName) Then
											PNRSDict.Remove(VarName)
										End IF
										PNRSDict.Add VarName, Expr
									End IF
								End IF
							End IF
						Else
							If GlobalVariableType.Exists(VarName) Then
								TypeVarName = GlobalVariableType.Item(VarName)
								TypeVarName = trim(TypeVarName)
								Expr = Replace(Expr," ","")
								ArrWordExpr = Split(Expr,".")
								IF UBound(ArrWordExpr)=1 Then 'Выражения типа "спр.текущийэлемент();"
									If (InStr(1,ArrWordExpr(1),"текущийэлемент();")=1) Then
										If Dict.Exists(ArrWordExpr(0)) Then
											ittext = Dict.Item(ArrWordExpr(0))
											if (InStr(1,ittext,"справочник")=1) And (InStr(1,ittext,".")>0) And (InStr(1,ittext,"+")=0) Then
												If GlobalVariableType.Exists(VarName) Then
													GlobalVariableType.Remove(VarName)
													GlobalVariableType.Add VarName, ittext
													If GlobalVariableNoCase.Exists(VarName) Then
														GlobalVariableNoCase.Remove(VarName)
													End IF
													GlobalVariableNoCase.Add VarName, VarNameNoCase
													'Message "VarName4=" & VarName & " ittext = " & ittext
												End IF
											End IF
										ElseIf PNRSDict.Exists(ArrWordExpr(0)) Then
											ittext = PNRSDict.Item(ArrWordExpr(0))
											if (InStr(1,ittext,"справочник")=1) And (InStr(1,ittext,".")>0) And (InStr(1,ittext,"+")=0) Then
												If GlobalVariableType.Exists(VarName) Then
													GlobalVariableType.Remove(VarName)
													GlobalVariableType.Add VarName, ittext
													If GlobalVariableNoCase.Exists(VarName) Then
														GlobalVariableNoCase.Remove(VarName)
													End IF
													GlobalVariableNoCase.Add VarName, VarNameNoCase
													'Message "VarName4=" & VarName & " ittext = " & ittext
												End IF
											End IF

										End IF
									End IF
								End IF
							End IF
						End IF
					End IF
				End IF
			End IF
		End IF
	 next
end sub

Private Function DelKomment(ttext)
	PosKomment = InStr(1,ttext, "//")
	IF PosKomment>0 Then
		If PosKomment <> 1 Then
			ttext = Mid(ttext,1,PosKomment-1)
			ttext = Trim(ttext)
		Else
			ttext = ""
		End IF
	End IF
	DelKomment = ttext
end Function

Private Function CheckIsBetweenToken(BTMeth, BTNumberParams, RWord, doc)
	IsBetweenToken = False
	ArrMethods = Array(UCase("Свернуть"), UCase("GroupBy"),_
		UCase("Группировка"), UCase("Group"), _
		UCase("Цвет"), UCase("Color"), _
		UCase("ЭтоГруппа"), UCase("IsItAGroup"),_
		UCase("СоздатьОбъект"), UCase("CreateObject"),_
		UCase("ИспользоватьСлой"), UCase("UseLayer"),_
		UCase("ВывестиСекцию"), UCase("PutSection"),_
		UCase("ВидимостьКолонки"), UCase("ColumnVisibility"),_
		UCase("ПрисоединитьСекцию"), UCase("AttachSection"), _
		UCase("ПравоДоступа"), UCase("AccessRight")) '2004.12.30
	MethodsForBetweenToken = "#"& Join(ArrMethods,"#") & "#"
	Patern = "[#]+(" & UC(BTMeth) & ")+[#]+"
	FindStr = FindInStrEx(Patern,MethodsForBetweenToken)
	IF Len(FindStr)>0 Then
		IsBetweenToken = True
	End If
	IF (BTNumberParams = 1) Then
		IF	UC(BTMeth) = UC("НоваяКолонка") OR _
			UC(BTMeth) = UC("NewColumn") OR _
			UC(BTMeth) = UC("Итог") OR _
			UC(BTMeth) = UC("Total") OR _
			UC(BTMeth) = UC("Активизировать") OR _
			UC(BTMeth) = UC("Activate") OR _
			UC(BTMeth) = UC("УстановитьПараметрыКолонки") OR _
			UC(BTMeth) = UC("SetColumnParameters") OR _
			UC(BTMeth) = UC("УстановитьЗначениеФильтра") OR _
			UC(BTMeth) = UC("SetFilterValue") OR _
			UC(BTMeth) = UC("Сортировать") OR _
			UC(BTMeth) = UC("Sort") OR _
			UC(BTMeth) = UC("ПолучитьАтрибут") OR _
			UC(BTMeth) = UC("GetAttrib") OR _
			UC(BTMeth) = UC("УстановитьАтрибут") OR _
			UC(BTMeth) = UC("SetAttrib") OR _
			UC(BTMeth) = UC("ВыбратьЭлементыПоРеквизиту") OR _
			UC(BTMeth) = UC("SelectItemsByAttribute") OR _
			UC(BTMeth) = UC("ВыводитьПиктограммы") OR _
			UC(BTMeth) = UC("ShowImages") OR _
			UC(BTMeth) = UC("ВидимостьКолонки") OR _
			UC(BTMeth) = UC("ColumnVisibility") Then
			IsBetweenToken = True
			IF UC(BTMeth) = UC("Активизировать") OR UC(BTMeth) = UC("Activate")  Then
				glRWord.TypeVid = "#РеквизитФормы#"
			End If
		End If
	ElseIF (BTNumberParams = 2) Then
		IF UC(BTMeth) = UC("ОткрытьПодбор") OR UC(BTMeth) = UC("OpenPermanentChoice") OR _
			UC(BTMeth) = UC("Выбрать") OR UC(BTMeth) = UC("Choose") OR _
			UC(BTMeth) = UC("ВыгрузитьТабличнуюЧасть") OR UC(BTMeth) = UC("UnloadTable")  Then
			IsBetweenToken = True
			Set tvar = new TheVariable
			tvar.ExtractDef(Doc.Name)
			if tvar.VerifyAgr() And (UC(BTMeth) = UC("ВыгрузитьТабличнуюЧасть") OR UC(BTMeth) = UC("UnloadTable")) Then
				if lcase(tvar.V_Type) = lcase("Документ") Then
					'RWord.BTObj = tvar.V_Type & "." & tvar.V_Vid
					RWord.RW = "ТекущийДокумент();"
				else
					IsBetweenToken = false
				End If
			else
				IsBetweenToken = True
			End If
		End If
	ElseIF (BTNumberParams = 3) Then
		IF UC(BTMeth) = UC("НайтиЗначение") OR UC(BTMeth) = UC("FindValue") Then
			IsBetweenToken = True
		End If
	ElseIF (BTNumberParams = 4) Then
		IF UC(BTMeth) = UC("Заполнить") OR UC(BTMeth) = UC("Fill") OR _
			UC(BTMeth) = UC("Выгрузить") OR UC(BTMeth) = UC("Unload")  Then
			IsBetweenToken = True
		End If
	End If
	if (Ucase(RWord.BTMeth) = UCase("СоздатьОбъект")) OR (Ucase(RWord.BTMeth) = UCase("CreateObject")) Then
		If Len(RWord.RW)>0 Then
			IsBetweenToken = true
			RWord.IsIcvalVid = FALSE
			RWord.FindAtribute = FALSE
		End If
	End If
	CheckIsBetweenToken = IsBetweenToken
End Function


Private Function GetMethodAndRekvEx(TypeVid, ResultWord, SMethodami,AsRecvOfForms, IsBetweenToken, NameOfTableFromTable)
	ttext = ""
	'Если у нас тип без точки ЗАПРОС, Таблица, ТаблицаЗначений, СписокЗначений
	If UCase(TypeVid) = "ЗАПРОС" Then
		strRekv = GetVariableAndFunctionZapros(ResultWord,0)
		if SMethodami = 1 Then
			strRekv = strRekv & vbCrLf & GetZaprosMet()
		End If
	ElseIf UCase(TypeVid) = "ТАБЛИЦАЗНАЧЕНИЙ" Then
		strRekv = GetColumnsFromTZ(ResultWord,0)
		if SMethodami = 1 Then
			strRekv	= strRekv & vbCrLf & GetMethodsOfTablicaZnacheniy(0)
		End If
	ElseIf UCase(TypeVid) = UCase("ФормаПодбораСправочник") Then
		strRekv = strRekv & vbCrLf & GetMethodsOfFormPodbor()
	ElseIf UCase(TypeVid) = "ФОРМА" Then
		strRekv = GetTableRecvFromForms(0,IsBetweenToken)
		if SMethodami = 1 Then
			strRekv = strRekv & vbCrLf & GetMethodsOfForm()
		End If
		tType = ""
		If EtoFormaDokumenta(tType) Then
			if tType = "Документ" Then
				strRekv = strRekv & vbCrLf &  "* только агрегатные"
				strRekv = strRekv & vbCrLf &  "* только агрегатные общие"
				strRekv = strRekv & vbCrLf &  "* только агрегатные шапка"
				strRekv = strRekv & vbCrLf &  "* только агрегатные таблица"
				strRekv = strRekv & vbCrLf &  "* только НЕ агрегатные"
			Else
				strRekv = strRekv & vbCrLf &  "* только агрегатные"
				strRekv = strRekv & vbCrLf &  "* только НЕ агрегатные"
			End If
		End If
	ElseIf UCase(TypeVid) = UCase("#РеквизитФормы#") Then
		strRekv = GetMethodsOfFormRecv()
	ElseIf UCase(TypeVid) = UCase("СписокЗначений") Then
		strRekv = GetMethodsOfListBox(AsRecvOfForms)
	ElseIf UCase(TypeVid) = UCase("Документ") Then
		strRekv = GetDocMet(0)
		strRekv = SortStringForList(strRekv, vbCrLf)
	ElseIf UCase(TypeVid) = UCase("Регистры") Then
		strRekv = GetListKindVariable("Регистр")
		if SMethodami = 1 Then
			strRekv = strRekv & vbCrLf & GetMethodsRegisters()
		End If
	ElseIf UCase(TypeVid) = UCase("Регистр") Then
		strRekv = GetListKindVariable("Регистр")
	ElseIf UCase(TypeVid) = UCase("Текст") Then
		strRekv = GetMethodsText("")
	ElseIf UCase(TypeVid) = UCase("Таблица") Then
		if IsBetweenToken Then
			strRekv = GetListMethodAndRekvtablica(ResultWord,0,NameOfTableFromTable)
		else
			strRekv = GetListMethodAndRekvtablica(ResultWord,SMethodami,NameOfTableFromTable)
		End If
	ElseIf UCase(TypeVid) = UCase("Перечисление") Then
		strRekv = GetEnum("")
	'Else
	'	strRekv = LoadMethodFromFile(TypeVid)
	End If
	GetMethodAndRekvEx = strRekv
End Function



Private Function GetEnum(par)
	GetEnum = "" : tGetEnum = ""
	Set Enums = MetaData.TaskDef.Childs(CStr("Перечисление"))
	For tt = 0 To Enums.Count - 1
		Set mdo = Enums(tt)
		AddToString tGetEnum, mdo.Name,vbCrLf
	Next
	GetEnum = tGetEnum
End Function


Private Function EtoFormaDokumenta(tType)
	EtoFormaDokumenta = False
	tdoc = ""
	if CheckWindow(tdoc) Then
		ArrN = Split(tdoc.Name,".")
		If UBound(ArrN)<> -1 Then
			If (ArrN(0) = "Документ") Or (ArrN(0) = "Справочник") Then
				EtoFormaDokumenta = True
				tType = ArrN(0)
			End IF
		End IF
	End IF
End Function

Private Function LoadMethodFromFile(FileNm)
	LoadMethodFromFile = ""
	FileName = BinDir + "\Config\Intell\" + FileNm + ".ints"
	FileExists = FSO.FileExists(FileName)
	if FileExists = False then
		FileName = BinDir + "\Config\Intell\1С++\" + FileNm + ".ints"
		FileExists = FSO.FileExists(FileName)
	end if

	if FileExists = True then
		Set Fl = FSO.GetFile(FileName)
		Set FileStream = Fl.OpenAsTextStream()
		if FileStream.AtEndOfStream = true then  Exit Function

		AllMeth = FileStream.ReadAll()
		AllMethods = Split(AllMeth, vbCrLf)
		for i = 0 to UBound(AllMethods)
			Methods = Methods + vbCrLf + picMeth + Mid(AllMethods(i), 6)
		next
	end if
	LoadMethodFromFile = Methods
End Function


'Возвращает из выражения типа "ТЗЗапроса.Свернуть("Литры, Категория","");"
'Объект, в д.с.: "ТЗЗапроса"
'Метод, в д.с.: "Свернуть"
'И номер метода в котором стоит курсор в (BTNumberParams)
'теперь можно определиться что с этим добром делать :)
Private Function BetweenToken(SelStartCol,BTObj, BTMeth, BTNumberParams, doc, RWord)
	LinefromText = doc.range(doc.SelStartLine)
	BetweenToken = False
	RWord.BTNumberParams = 0
	'Patern = "[" & cnstRExWORD & "]+[\s]*[.]*[\s]*[" & cnstRExWORD & "]+[\s]*[\(]+[\s]*[" & cnstRExWORD & ",.\s\(\)""\+]*[\s]*[\)]+[\s]*[;]*"
	Patern = "[" & cnstRExWORD & "]+[\s]*[.]*[\s]*[" & cnstRExWORD & "]+[\s]*[\(]+.*[\)]+"
	ttext = FindInStrEx(Patern,LinefromText)
	ttextAll = ttext
	'Нашли выражение со скобками
	if Len(ttext)>0 Then
		'Определимся с именем переменной и методом
		Patern = "[" & cnstRExWORD & "]+[\s]*[.]*[\s]*[" & cnstRExWORD & "]+[\s]*"
		FindFirstInFindInStrEx = True
		VariableAndMethod = FindInStrEx(Patern,ttext)
		VariableAndMethod = Replace(VariableAndMethod, vbTab, " ")
		Do While InStr(1,VariableAndMethod,"  ")>0
			VariableAndMethod = Replace(VariableAndMethod,"  "," ")
		Loop
		VariableAndMethod = Trim(VariableAndMethod)
		'Оставим только то что с точкой
		Patern = "[" & cnstRExWORD & "]{1,1}[\s]*[.]+[\s]*[" & cnstRExWORD & "]{1,1}[\s]*"
		pttext = FindInStrEx(Patern,VariableAndMethod)
		tArr = split(pttext,vbCrLf)
		If UBound(tArr) <> -1 Then
			pttext = ""
			For ee = 0 To UBound(tArr)
				VariableAndMethod = Replace(VariableAndMethod,tArr(ee), Left(tArr(ee),1) & "."& Right(tArr(ee),1))
			Next
		End IF
		tArr = split(VariableAndMethod," ")
		If UBound(tArr)>0 Then
			VariableAndMethod = tArr(UBound(tArr))
		End IF


		'Patern2 - Для доставания наименования объекта и метода с точкой
		Patern2 = "[" & cnstRExWORD & "]+[\s]*[.]*[\s]*[" & cnstRExWORD & "]+[\s]*[\(]+"
		VariableAndMethodSoSkobkoy = FindInStrEx(Patern2,ttext)
		FindFirstInFindInStrEx = False
		if Len(VariableAndMethodSoSkobkoy) = 0 Then
			Exit Function
		End IF
		If Len(VariableAndMethod)>0 Then
			VariableAndMethod = Replace(VariableAndMethod," ","")
			IF InStr(1,VariableAndMethod,".")>0 Then 'Значит есть у нас и переменная и метод
				tArr = Split(VariableAndMethod,".")
				RWord.BTObj = Trim(tArr(0))
				RWord.BTMeth = Trim(tArr(1))
			else
				RWord.BTMeth = Trim(VariableAndMethod)
			End If
		Else
			Exit Function
		End If
		'Проверим находится ли слева наименование метода и объекта (VariableAndMethod),
		'вдруг мы пляшем на нем а не на скобках
		KursorNaMeste = True
		StrokaGdeIchem = Left(LinefromText,doc.SelStartCol)
		If InStr(1,StrokaGdeIchem,VariableAndMethodSoSkobkoy) = 0 Then
			'Значит курсор стоим гдето на методе или объекте
			KursorNaMeste = False
		End IF
		Patern = "[\s]*[\)]+[\s]*[;]*"
		ZaSkobkoy = FindInStrEx(Patern,LinefromText)

		'Проверим не находится ли курсор за скобкой
		StrokaGdeIchem = Mid(LinefromText,doc.SelStartCol)
		If InStr(1,StrokaGdeIchem,ZaSkobkoy) = 0 Then
			'Значит курсор стоим гдето на методе или объекте
			KursorNaMeste = False
		End IF
		if Not KursorNaMeste Then
			BetweenToken	= False
			RWord.BTObj			= ""
			RWord.BTMeth			= ""
			Exit Function
		End IF
		'Ищем что у нас в скобках
		Patern = "[\(]+.*[\)]+"

		InBrasket = FindInStrEx(Patern,LinefromText)

		If Len(InBrasket)>0 Then
			'Если нашли, убираем скобки и сплитуем
			Patern = "[\(]+.*[\)]+"
			InBrasket = FindInStrEx(Patern,InBrasket)
			If Len(InBrasket)>0 Then
				Params = GetParams(InBrasket)
				'MsgBox Params
				ArrParams = Split(Params, vbCrLf)
				NumberMeth = 1
				if UBound(ArrParams)>0 Then
					'Сейчас можно выяснить где находимся (место курсора в методе)
					'Добавляем с конца массива итемы в строку и ищем справа от курсора,
					'если не находим очередной, тогда значит мы находимся в нем :)
					StrokaGdeIShem = Mid(LinefromText,doc.SelStartCol+1)
					StrokaKotoruIShem = ""
					For tt = UBound(ArrParams) To 0 Step -1
						StrokaKotoruIShem = ArrParams(tt) & StrokaKotoruIShem
						If InStr(1,StrokaGdeIShem,StrokaKotoruIShem) = 0 Then
							NumberMeth = tt + 1
							Exit For
						End If
					Next
					'MsgBox BTNumberParams
				End If
				RWord.BTNumberParams = NumberMeth
			End If
		End If
	End If
	If (RWord.BTNumberParams<>0) And (RWord.BTMeth<>"") Then
		BetweenToken = True
		'2004.12.03 - добавляем реакцию на ОткрытьПодбор
		If UBound(ArrParams)>0 Then
			IF (RWord.BTNumberParams = 2) And (UC(RWord.BTMeth) = UC("ОткрытьПодбор")) Then
				If (InStr(1,uC(ArrParams(0)),"СПРАВОЧНИК.")>0) And (InStr(1,uC(ArrParams(0)),"+")=0) Then
					RWord.BTObj = Replace(ArrParams(0),"""","")
				End If
			End If
		End If
	Else
		BetweenToken = False
	End If
	RWord.IsBetweenToken = BetweenToken

End Function

'А вот как разделить параметры это загадка ведь выражения в ковычках ч\з запятую ни кто не отменял.
'Попробуем их выделить и заменить
'Смысл: есть открывающие и закрывающие конструкции, а между ними запятая,
'если конструкция открыта запятая не является разделителем медодов, а разделяет что-то
'внутри кавычек например. С этой точки зрения попытаемся разобрать параметры
'OpenBraskets = "" -  у нас открытые токены типа ""; ()
Private Function GetParams(InBrasket)
	Params = ""
	InBrasket2 = ""
	For tt = 2 To Len(InBrasket)-1 'Берем без закрывающих скобок
		Char = Mid(InBrasket,tt,1)
		IF (Char = """") Or (Char = "(") Or (Char = ")") Then
			IF Len(Braskets)>0 Then 'Есть открытая конструкция
				IF (Char = """") Then
					IF Right(Braskets,1) = Char Then 'Закрывается текущий токен
						Braskets = Left(Braskets,Len(Braskets)-1)
					End If
				ElseIF (Char = "(") Or (Char = ")") Then
					IF (Char = "(") Then
						Braskets = Braskets & Char
					ElseIF Char = ")" Then
						IF Right(Braskets,1) =  "(" Then 'Закрывается текущий токен
							Braskets = Left(Braskets,Len(Braskets)-1)
						End If
					End If
				End If
			Else
				Braskets = Braskets & Char
			End If
		End If
		'InBrasket2 = InBrasket2 & Mid(InBrasket,tt,1)
		if (Len(Braskets) = 0) And (Char = ",") Then
			IF Len(Params)>0 Then
				Params = Params & vbCrLf & InBrasket2
			Else
				Params = InBrasket2
			End IF
			InBrasket2 = ""
			InBrasket2 = InBrasket2 & Mid(InBrasket,tt,1)
		else
			InBrasket2 = InBrasket2 & Mid(InBrasket,tt,1)
		End IF
	Next
	if Len(InBrasket2)>0 Then
		IF Len(Params)>0 Then
			Params = Params & vbCrLf & InBrasket2
		Else
			Params = InBrasket2
		End IF
		InBrasket2 = ""
	End IF
	if Char = "," Then
		Params = Params & vbCrLf
	End IF
	GetParams = Params
End Function



'Private Function GetWordFromCaret()
Function GetWordFromCaret(OldResultWord,ResultWordFull,ResultWordEnd,  RWord)
	'FindInStrEx(Patern,ResultWordFull)
	ResultWordFull = ""
	doc = ""
	if Not CheckWindow(doc) then Exit Function

	BadWord = ""
	ResultWord = ""
	LinefromText = doc.range(doc.SelStartLine)
	RWord.LineText = LinefromText
	SelStartCol = doc.SelStartCol
	If Len(LinefromText)<>0 Then
		If Len(LinefromText)>(SelStartCol+1) then
			RWord.LineCaretToEnd = Mid(LinefromText,SelStartCol+1)
		End IF
		RWord.LineStartToCaret = Mid(LinefromText,1,SelStartCol)
	End IF
	tmpStrOfBadSimbols = " " & vbTab & vbCr & vbCrLf &"""()-=?,;+<>|/*"
	if (SelStartCol = 0) Then
		For i = (doc.SelStartCol+1) To Len(LinefromText)
			Char = Mid(LinefromText, i,1)
			if InStr(1,tmpStrOfBadSimbols,Char)>0  Then
				Exit For
			End If
			ResultWord = ResultWord + Char
		next
	else
		'Выуживаем полное слово для считывания из файлов ints
		ResultWordFull = mid(LinefromText,1,doc.SelStartCol)
		ResultWordFull = Trim(ResultWordFull)
		Do While InStr(1,ResultWordFull,"  ")>0
			ResultWordFull = Replace(ResultWordFull,"  "," ")
		Loop
		ResultWordEnd = Replace(LinefromText, ResultWordFull, "")

		for ttt = 0 to 3
			If ttt = 0 Then
				Patern = "[\s]*[\.]+[\s]*"
			ElseIf ttt = 1 Then	Patern = "[\s]*[\(]+[\s]*[""]+"
			ElseIf ttt = 2 Then	Patern = "[\s]*[""]+[\s]*[\)]+"
			ElseIf ttt = 3 Then	Patern = "[\(]+[\s]*[""]+[" & cnstRExWORD & "]+[""]+[\s]*[\)]+"
			End If
			ttextZameni = FindInStrEx(Patern,ResultWordFull)
			If ttextZameni<>"" Then
				ArrWordZameni = Split(ttextZameni,vbCrLf)
				If UBound(ArrWordZameni)<> -1 Then
					For iii = 0 to UBound(ArrWordZameni)
						itZameni = ArrWordZameni(iii)
						If Len(itZameni)<>0 Then
							If ttt = 0 Then
								ResultWordFull = Replace(ResultWordFull,itZameni,".")
							ElseIf ttt = 1 Then	ResultWordFull = Replace(ResultWordFull,itZameni,"(""")
							ElseIf ttt = 2 Then	ResultWordFull = Replace(ResultWordFull,itZameni,""")")
							ElseIf ttt = 3 Then	ResultWordFull = Replace(ResultWordFull,itZameni,"")
							End If
						End If
					Next
				End If
			End If
		Next
		LinefromText = ResultWordFull
		LenLinefromText = Len(LinefromText)+1
		If Len(ResultWordFull)<>0 Then
			ArrWord = Split(ResultWordFull)
			ResultWordFull = ""
			If Ubound(ArrWord)<> -1 Then
				ResultWordFull = ArrWord(Ubound(ArrWord))
			End If
		End If
		ResultWordFull = Replace(ResultWordFull,".","")
		'LenLinefromText
		'For i = doc.SelStartCol+1 To Len(LinefromText)
		For i = LenLinefromText To Len(LinefromText)
			Char = Mid(LinefromText, i,1)
			if InStr(1,tmpStrOfBadSimbols,Char)>0  Then
				Exit For
			End If
			BadWord = BadWord + Char
		next
		if (LenLinefromText <> 0) Then
			'for i = (doc.SelStartCol-1) To 1 Step -1
			for i = (LenLinefromText-1) To 1 Step -1
				Char = Mid(LinefromText,i,1)
				if  InStr(1,tmpStrOfBadSimbols,Char)>0  Then 'tmpStrOfBadSimbols
					Exit For
				End If
				ResultWord = Char + ResultWord
			next
		End If
	end if
	if (len(ResultWord) = 0 ) Then
		ResultWord = "!"
	ElseIF WordInArray(GetListOFStatments(), ResultWord) Then
		ResultWord = "!"
	end if
	OldResultWord = ResultWord
	If Len(ResultWord) > 0 Then	'Отбрасываем точку если она есть в конце слова
		If Mid(ResultWord,Len(ResultWord),1) = "." Then
			ResultWord = Mid(ResultWord,1,Len(ResultWord)-1)
		End IF
	End IF
	If InStr(1,ResultWord,".")>0 Then
		DimResultWordAdd = split(ResultWord,".")
	Else
		DimResultWordAdd = Array(ResultWord)
	End IF
	if InStr(ResultWord,".")>0 Then
	RWord.RWAll = ResultWord
	' Если у нас есть точки в слове, тогда ищем первую часть слова, а оставщиеся переливаем в массив
		tempWords = ""
		Dim tempDimResultWord
		tempDimResultWord = split(ResultWord, ".")
		for i = 0 to UBound(tempDimResultWord)
			if Len(tempDimResultWord(i))>0 Then
				if i = 0 Then
					ResultWord = tempDimResultWord(i)
				else
					if (Len(RWord.AddWord)>0) then
						RWord.AddWord = RWord.AddWord & "." & tempDimResultWord(i)
					else
						RWord.AddWord = tempDimResultWord(i)
					end if
				end if
			end if
		Next
		' Ь=Теперь в массиве DimResultWordAdd сидит все вплоть до "ТекущийДокумент()" или атрибутов реквизита.
	end if
	GetWordFromCaret = ResultWord
	if Len(BadWord)>0 Then
		GetWordFromCaret = "?"
	end if
	GetWordFromCaret = Trim(GetWordFromCaret)
	RWord.RWFullStr = ResultWordFull
	RWOrd.RWOld = OldResultWord

	'Посмотрим сравниваем ли мы что-то с чем-то
	'Проверяем на сравнеие с Итем.Итем.Вид() = [<!>|"<!>"]
	ttext = FindInStrEx("[" & cnstRExWORD &"\.]+\s*\.+\s*(вид|Kind)+\(+\s*\)+\s*\=+\s*[""""]*",RWord.LineStartToCaret)
	if Len(ttext) = 0 Then
		RWord.IsNeedBrasket = false
		ttext2 = FindInStrEx("[" & cnstRExWORD &"\.]+\s*=+\s*[""""]*",RWord.LineStartToCaret)
		ttext = ttext2
	else
		RWord.IsNeedBrasket = true
	End IF
	' FindInStrEx - может вернуть несколько значений, разделенных vbCrLf, тут надо брать последнее
	if InStr(1,ttext, vbCrLf)>0 Then
		tempDimResultWord = Split(ttext,vbCrLf)
		ttext = tempDimResultWord(UBound(tempDimResultWord))
	End IF
	'Только макрос вызван в толчке сравнения "= [<!>|"<!>"]"
	If InStr(1,StrReverse(RWord.LineStartToCaret),StrReverse(ttext)) = 1 Then
		' Убираем "вид()"
		ttext2 = FindInStrEx("\s*\.+\s*(вид|Kind)+\(+\s*\)+\s*",ttext)
		if Len(ttext2)>0 Then
			ttext = Replace(ttext,ttext2,"")
		End IF

		if Len(ttext)>0 Then
			ttext = FindInStrEx("[" & cnstRExWORD &"\.]+\s*",ttext)
			if Len(ttext) = 0 Then
				RWord.IsNeedBrasket = false
				ttext = FindInStrEx("[" & cnstRExWORD &"\.]+",ttext2)
			End IF
			ttext = Trim(ttext)
			if Len(ttext)>0 Then
				ttext = ReplaceEx(ttext, Array(" ","", vbTab, "", "=",""))
				if (Right(ttext,1) = ".") Then
					ttext = Left(ttext,len(ttext)-1)
				End IF
				if (InStr(1,ttext,".") > 0) Then
					tempDimResultWord = Split(ttext,".")
					if UBound(tempDimResultWord)<> -1 Then
						ttext = tempDimResultWord(0)
					End IF
					if UBound(tempDimResultWord)>0 Then
						RWord.AddWord = ""
						For e=1 To UBound(tempDimResultWord)
							if Len(RWord.AddWord)>0 Then
								RWord.AddWord = RWord.AddWord & "." & tempDimResultWord(e)
							Else
								RWord.AddWord = tempDimResultWord(e)
							End IF
						Next
					End IF
				End IF
				RWord.IsIcvalVid = true
				RWOrd.BTObj = ttext
				GetWordFromCaret = RWOrd.BTObj
			End IF
		End IF
	End IF
	'Проверяем на сравнеие с ТипЗначенияСтр(Словечко) = [<!>|"<!>"]
	'ttext = FindInStrEx("[" & cnstRExWORD &"\.]+\s*\.+\s*(вид|Kind)+\(+\s*\)+\s*\=+\s*[""""]*",RWord.LineStartToCaret)
	ttext = FindInStrEx("\s+(ТипЗначенияСтр|ValueTypeStr)+\s*\(+[" & cnstRExWORD &"\.]+\s*\)+\s*\=+\s*[""""]*",RWord.LineStartToCaret)
	If Len(ttext)>0 Then
		RWord.IsIcvalVid = true
		RWOrd.BTObj = ttext
		RWOrd.BTMeth = "ТипЗначенияСтр"
		'	Посмотрим, есть ли у нас ковычки в тексте куда мы собираемся подставить
		'	Тип и если их нет, скажем программе что они нам нужны...
		IF InStr(1,ttext,"""") = 0 Then
			IF InStr(1,RWord.LineCaretToEnd,"""") = 0 Then
				RWord.IsNeedBrasket = True
			End IF
		End IF
	End IF
End Function

'Выдает реквизиты и возможно методы Таблицы
'NameOfTableFromTable - наименование исходной таблицы
Private Function GetListMethodAndRekvTablica(ResultWord,SMethodami,NameOfTableFromTable)
	GetListMethodAndRekvTablica = ""
	If SMethodami Then
		GetListMethodAndRekvTablica = GetMethodsOfTablica("")
	End If

	doc = ""
	if Not CheckWindowOnWorkbook(doc) then Exit Function

	'пока сделаем заглушку
	' Отпарсим текст в поисках конструкций:
	' ИмяПеременнойТаблицы.ВС = ИмяСекции1, имясекции2 [, имяСекцииN]
	' ИмяПеременнойТаблицы.ГС = ИмяСекции1, имясекции2 [, имяСекцииN]
	If SMethodami = 0 Then
		TextMod = GetLocationText()
		Patern = "[\s|^]*(//)+[\s]*(" & ResultWord & ")+[\s]*[.]+[\s]*(ВС|ГС)+[\s]*[=]+[\s]*[" & cnstRExWORD & "\s,]+[\s]*"
		ttext = FindInStrEx(Patern,TextMod)
		ArrFindStrok = split(ttext,vbCrLf)
		If UBound(ArrFindStrok)<> -1 Then
			For ee = 0 To UBound(ArrFindStrok)
				ttext = ArrFindStrok(ee)
				if Len(ttext)>0 Then
					Patern = "(ВС|ГС)+[\s]*[=]+"
					ttextTypeSekc = FindInStrEx(Patern,ttext)
					if Len(ttext)>0 Then
						Patern = "(ВС|ГС)+[\s]*"
						ttextTypeSekc = FindInStrEx(Patern,ttextTypeSekc)
						if Len(ttext) <> 0 Then
							ttextTypeSekc = Trim(ttextTypeSekc)
							ttTypeSekc = UCase(ttextTypeSekc)
							Patern = "[=]+[\s]*[" & cnstRExWORD & "\s,]+[\s]*"
							ttextSekc = FindInStrEx(Patern,ttext)
							if Len(ttextSekc) > 0 Then
								ttextSekc = ReplaceEx(ttextSekc, Array("=","", " ",""))
								ArrSech = split(ttextSekc,",")
								Rezult = join(ArrSech, vbCrLf)
								if ttTypeSekc = "ГС" Then
									Rezult = "|"& Replace(Rezult, vbCrLf,vbCrLf&"|")
								End If
								If Len(GetListMethodAndRekvTablica)>0 Then
									GetListMethodAndRekvTablica = GetListMethodAndRekvTablica & vbCrLf & Rezult
								Else
									GetListMethodAndRekvTablica = Rezult
								End If
							End If
						End If
					End If
				End If
			Next
		End If
	End If
End Function

'Типизирует переменную, возвращает фразу "тип.вид"
Private Function GetTypeVidA(Variable)
	Set Var = New TheVariable
	Var.ExtractDef(Variable)

End Function

'Функция типизирует переменную "ResultWord" в зависимости
'от переданного LastTipeVid, который может быть пустым, значт надо искать
'по тексту или в форме
'Doc - документ из которого происходит поиск
' Возвращаем "" если не смогли типи зировать и Тип.Вид или Тип.Вид.СО (СО - объект созданный СоздатьОбъектом)
Private Function GetTypeVid(Variable,LastTipeVid, Doc, AsRecvOfForms, FirstWord)
	GetTypeVid	= ""
	If (Len(LastTipeVid) = 0) And (LCase(Variable) = "форма") Then
		If Len(glRWord.AddWord)>0 Then
			LastTipeVid	= "форма"
			Variable = glRWord.AddWord
		End If
	End If
	'Если у нас Variable с точками, тогда рекурсивно вызваем себя для пипизации всех позиций...
	If InStr(1,Variable,".")>0 Then
		ttt = Split(Variable,".")
		tRezult = LastTipeVid
		For qqq = 0 To UBound(ttt)
			tRezult = GetTypeVid(ttt(qqq),tRezult, Doc, AsRecvOfForms, FirstWord)
		Next
		if Len(tRezult)>0 Then
			GetTypeVid	= tRezult
		End IF
		Exit Function
	End IF
	if (LCase(glRWord.AddWord) = LCAse("Закладки")) And (CompareNoCase(Variable,"Форма",1)) Then
		GetTypeVid = cnstNameTypeSZ
		Exit Function
	End IF
	AsRecvOfForms = 0
	LastTipe	= ""
	LastVid		= ""
	IF InStr(1,LastTipeVid,".")>0 Then
		ttt = Split(LastTipeVid,".")
		if UBound(ttt)=1 Then
			LastTipe	= ttt(0)
			LastVid		= ttt(1)
			IF false And (UCase(LastTipe) = UCase("регистр")) Then
				GetTypeVid = LastTipeVid
				Exit Function
			End IF
		End IF
	End IF
	On Error Resume Next
	If (Len(glRWord.AddWord)>0) And (lcase(Variable) = "константа") Then
		IF StrCountOccur(glRWord.AddWord, ".") = 0 Then
			Set kon  = MetaData.TaskDef.Childs(CStr("константа"))
			For tt = 0 To kon.Count-1
				if (Lcase(kon(tt).Name) = trim(lcase(glRWord.AddWord))) Then
					GetTypeVid = kon(tt).Type.FullName
					Exit Function
				End IF
			Next
		End IF
	End IF

	If LastTipeVid = "" Then
		If Is1CObject(Variable) Then
			GetTypeVid = Variable
			if Len(FirstWord)>0 Then
				if FirstWord = Variable Then
					If Len(glRWord.AddWord)<>0 And 	CompareNoCase(Variable,"Перечисление",1) Then
						IF InStr(1,glRWord.AddWord,".") = 0 Then
							GetTypeVid = "Перечисление." & glRWord.AddWord
							Exit Function
						End IF
					End IF
				Else
					ttt = Split(FirstWord,".")
					if UBound(ttt)=1 Then
						LastTipe	= ttt(0)
						LastVid		= ttt(1)
						IF (UCase(LastTipe) = UCase("регистр")) Then
							GetTypeVid = LastTipeVid
							Exit Function
						End IF
					End IF
				End IF
			End IF
		Else
			'Сначала определимся с каким объектом работаем
			Patern = "(^)+(Документ.|Журнал.|Справочник.|Отчет.|Обработка.|CWBModuleDoc::)+"
			If Len(FindInStrEx (Patern, Doc.Name))>0 Then 'Мы в модуле объекта
				if InStr(1,Doc.Name,"CWBModuleDoc::")>0 Then
					LastTipe = "CWBModuleDoc::"
				Else
					StrMetaObj = Doc.Name
					ttt = Split(StrMetaObj,".")
					LastTipe	= ttt(0)
					AsRecvOfForms = 1
					if Ubound(ttt)>0 Then
						LastVid = ttt(1)
					End IF
				End IF
			End IF
		End IF
	Else 'IF InStr(1,LastTipeVid,".") > 0 Then 'Значит переменная
		If UCase(LastTipeVid) = "ФОРМА" Then
			IF UCase(Variable) = UC("Закладки") Then
				GetTypeVid = cnstNameTypeSZ
			Else
				GetTypeVid = "#РеквизитФормы#"
			End IF
		ElseIf (UCase(LastTipeVid) = UCase("Перечисление")) And (Len(Variable)<>0) Then
			GetTypeVid = LastTipeVid & "." & Variable 'Попробуем....
		End IF
	End IF
	If UC(LastTipe) = UC("Документ") Then
		if Len(LastVid)>0 Then
			IF UCase(Variable) = UCase("Регистр") Then
				GetTypeVid = "Регистр"
			ElseIF UCase(Variable) = UCase("ТекущийДокумент();") Then
				GetTypeVid = LastTipe & "." & LastVid
			Else
				Set MetaDoc  = MetaData.TaskDef.Childs(CStr(LastTipe))(CStr(LastVid))
				'Тут надо взять идентификаторы всех атрибутов объекта и сравнить с нашим
				For tt = 0 To 2
					if tt=0 Then Set ch = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
					if tt=1 Then Set ch = MetaDoc.Childs("РеквизитШапки")
					if tt=2 Then Set ch = MetaDoc.Childs("РеквизитТабличнойЧасти")
					For cnmr = 0 To ch.Count-1
						Set MetaRekv = ch(cnmr)
						If UCase(MetaRekv.Name) = UCase(Variable) Then
							GetTypeVid = MetaRekv.type.FullName
							if AsRecvOfForms = 1 Then
								AsRecvOfForms = 2
							End IF
							glRWord.RecvOfForms = AsRecvOfForms
							Exit Function
						End IF
					Next
				Next
			End IF
		End IF
	ElseIF UC(LastTipe) = UC("Регистр") Then
		if Len(LastVid)>0 Then
			If UCase("ТекущийДокумент") = UCase(Variable) Then
				GetTypeVid = "Документ"
				Exit Function
			Else
				Set MetaReg  = MetaData.TaskDef.Childs(CStr(LastTipe))(CStr(LastVid))
				'Тут надо взять идентификаторы всех атрибутов объекта и сравнить с нашим
				For tt = 0 To 2
					if tt=0 Then Set ch = MetaReg.Childs("Измерение")
					if tt=1 Then Set ch = MetaReg.Childs("Ресурс")
					if tt=2 Then Set ch = MetaReg.Childs("Реквизит")
					For cnmr = 0 To ch.Count-1
						Set MetaRekv = ch(cnmr)
						If UCase(MetaRekv.Name) = UCase(Variable) Then
							GetTypeVid = MetaRekv.type.FullName
							if AsRecvOfForms = 1 Then
								AsRecvOfForms = 2
							End IF
							Exit Function
						End IF
					Next
				Next
			End IF
		End IF

	ElseIF UC(LastTipe) = UC("Справочник") Then
		if Len(LastVid)>0 Then
			IF UCase(Variable) = "РОДИТЕЛЬ" Then
				GetTypeVid = LastTipe &"."& LastVid
				Exit Function
			End If
			Set MetaDoc  = MetaData.TaskDef.Childs(CStr(LastTipe))(CStr(LastVid))
			IF UCase(Variable) = "ВЛАДЕЛЕЦ" Then
				'MetaDoc.Props(3)	" [Справочник.Номенклатура]"	String
				GetTypeVid = ReplaceEx(MetaDoc.Props(3), Array("[","", "]","", " ",""))
				Exit Function
			End If
			if Instr(1,Variable,".")>0 Then
				ArrWOfVariable = Split(Variable,".")
				for q = 0 To UBound(ArrWOfVariable)
					LastTipeVid = GetTypeVid(ArrWOfVariable(q),LastTipeVid, Doc, AsRecvOfForms, FirstWord)
				Next
				GetTypeVid = LastTipeVid
			Else
				'Тут надо взять идентификаторы всех атрибутов объекта и сравнить с нашим
				Set ch = MetaDoc.Childs("Реквизит")
				For cnmr = 0 To ch.Count-1
					Set MetaRekv = ch(cnmr)
					If UCase(MetaRekv.Name) = UCase(Variable) Then
						GetTypeVid = MetaRekv.type.FullName
						if AsRecvOfForms = 1 Then
							AsRecvOfForms = 2
						End IF
						Exit Function
					End IF
				Next
			End IF
		End IF
	ElseIF UC(LastTipe) = UC("Журнал") Then
		Set MetaDoc  = MetaData.TaskDef.Childs(CStr(LastTipe))(CStr(LastVid))
		'Тут надо взять идентификаторы всех атрибутов объекта и сравнить с нашим
		Set ch = MetaDoc.Childs("Графа")
		For cnmr = 0 To ch.Count-1
			Set MetaRekv = ch(cnmr)
			If UCase(MetaRekv.Name) = UCase(Variable) Then
				Set Prop = MetaRekv.Props
				' Тут в Prop № 3 сидят типы, которые нас интересуют,
				' Возьмем первый
				StrTypes = Prop(3)
				GrafaItems = Split(StrTypes,",")
				if UBound(GrafaItems)>0 Then
					GrafaItem = GrafaItems(0)
					ttt = Split(GrafaItem,".")
					If Ubound(ttt) = 2 Then
						GetTypeVid = GetTypeVid(ttt(2),ttt(0)&"."&ttt(1), Doc, AsRecvOfForms)
					End IF
				End IF
				for iii = 0 To Prop.count - 1
					'Message Prop(iii)
				Next
				'GetTypeVid = MetaRekv.type.FullName
				if AsRecvOfForms = 1 Then
					AsRecvOfForms = 2
				End IF
				Exit Function
			End IF
		Next
	End IF
	if (UCase(LastTipeVid) = "ЗАПРОС") OR (UCase(LastTipeVid) = "ТАБЛИЦАЗНАЧЕНИЙ") Then
		IF UCase(LastTipeVid) = "ЗАПРОС" Then
			ttextVar = GetVariableAndFunctionZapros(FirstWord,1) 	'FirstWord
		ElseIf UCase(LastTipeVid) = "ТАБЛИЦАЗНАЧЕНИЙ" Then
			ttextVar = GetColumnsFromTZ(FirstWord,1)
		End IF
		ttextVar = ReplaceEx(ttextVar, Array(",","", ";","", " ",""))
		ArrVar = Split(ttextVar,vbCrLf)
		if UBound(ArrVar)<> -1 Then
			For iii=0 To UBound(ArrVar)
				ttt = Split(ArrVar(iii),"=")
				if UBound(ttt) > 0 Then
					IF (UCase(ttt(0))=UCase(Variable)) Then
						ttt2 = Split(ttt(1),".")
						if UBound(ttt2) = 2 Then
							GetTypeVid = GetTypeVid(ttt2(2),ttt2(0)&"."&ttt2(1), Doc, AsRecvOfForms, FirstWord)
							Exit Function
						ElseIf UBound(ttt2) = 1 Then
							GetTypeVid = ttt(1)
						End IF
					End IF
				End IF
			Next
		End IF
	End IF
	if (UCase(LastTipeVid) = UCase("Регистр")) Or (UCase(LastTipeVid) = UCase("Регистры")) Then
		If UCase(LastTipeVid) = UCase("Регистры") Then
			LastTipeVid = "Регистр"
		End IF

		Set MetaReg  = MetaData.TaskDef.Childs(CStr(LastTipeVid))
		'Тут надо взять идентификаторы всех атрибутов объекта и сравнить с нашим
		For tt = 0 To MetaReg.Count-1
			Set Register = MetaReg(tt)
			IF UCase(Register.Name) = UCase(Variable ) Then
				GetTypeVid = "Регистр." & Register.Name
				Exit Function
			End IF
		Next
	End IF
	'IF (UC(LastTipe) = UC("Отчет")) Or (UC(LastTipe) = UC("Обработка")) Or (UC(LastTipe) = UC("CWBModuleDoc::")) Then
	IF Len(GetTypeVid)=0 Then
		'Тутачки мы в форме
		TableRekv = GetTableRecvFromForms(2,False)
		ListRekv = Split(TableRekv,vbCrLf)
		if UBound(ListRekv)<> -1 Then
			For eee = 0 To UBound(ListRekv)
				ITRekv = ListRekv(eee)
				if InStr(1,ITRekv," ")>0 Then
					ttt = Split(ITRekv," ")
					if UBound(ttt) = 1 Then
						If UCase(Variable) = UCase(ttt(0)) Then
							GetTypeVid = ttt(1)
							Exit For
						End IF
					End IF
				ElseIf Len(UCase(Trim(ITRekv))) > 0 Then
					If UCase(Trim(ITRekv)) = UCase(Variable) Then
						GetTypeVid = "#РеквизитФормы#"
						Exit For
					End IF
				End IF
			Next
		End IF
	End IF
End Function


Private Function GetORD()
	ReturnString = ""
	Set CommonRekv = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
	CommonRekvCount = CommonRekv.Count - 1
	For CRC = 0 To (CommonRekvCount)
	  	If Len(ReturnString)>0 Then
	  		ReturnString = ReturnString  &  vbCrLf & CommonRekv(CRC).Name
	  	Else
	  		ReturnString = CommonRekv(CRC).Name
	  	End IF
	Next
	GetORD = ReturnString
End Function


'SMetodami = 1 Выдаем еще и методы объекта 0 - только реквизиты
'AsRecvOfForms = 1 переменная реквизхит формы, иначе может быть создана "СоздатьОбъектом"
Private Function GetStringRekvizitovFromObj(TypeObj, NameObj,SMetodami,AsRecvOfForms, BTMeth,	BTNumberParams, RWord)
	ReturnString = ""
	MethodsSring = ""
	Find = False
	if (UC(TypeObj) = UC("Справочник")) Or(UC(TypeObj) = UC("Документ")) Or(UC(TypeObj) = UC("Регистр")) Or (UC(TypeObj) = UC("Перечисление")) Then
	    Set Childs = MetaData.TaskDef.Childs(CStr(TypeObj))
	    For i = 0 To Childs.Count - 1
	        Set mdo = Childs(i)
	        if UC(TypeObj) = UC("Регистр") Then 'Документ
				'MetaData.TaskDef.Childs(CStr(TypeObj))(CStr(NameObj))
				if UCase(NameObj) = UCase(mdo.Name) Then
					Find = True
					Set MetaReg = MetaData.TaskDef.Childs(CStr("Регистр"))(CStr(NameObj))
					'Добавим обработку ситуации когда мы имеем выгруженную из регистра таблицу....
					'Признаки: RWord.TypeVid2	= "таблицазначений" и RWord.BTMeth	= "Выгрузитьитоги"
					DasIsTZ = false

					if (CompareNoCase(RWord.TypeVid2, "таблицазначений",1))_
						 And ((CompareNoCase(RWord.BTMeth, "выгрузитьитоги",1))_
						 or (CompareNoCase(RWord.BTMeth, "RetrieveTotals",1))) Then
						DasIsTZ = true
					End IF
					for regBench = 0 To 2
						NameBench = ""
						TypeBenchStr = ""
						if regBench = 0 Then
						    Set HeadBench = MetaReg.Childs("Измерение")
						Elseif regBench = 1 Then
						    Set HeadBench = MetaReg.Childs("Ресурс")
						Elseif regBench = 2 Then
							if DasIsTZ Then Exit For
						    Set HeadBench = MetaReg.Childs("Реквизит")
						end if
	  					for hd_cnt = 0 To HeadBench.Count - 1
	  						set hd_chld = HeadBench(hd_cnt)
	  						If Len(ReturnString)>0 Then
	  							ReturnString = ReturnString  &  vbCrLf & hd_chld.Name
	  						Else
	  							ReturnString = hd_chld.Name
	  						End IF
	  						If SMetodami = 2 Then
	  							ReturnString = ReturnString & "=" & hd_chld.Type.FullName
	  						End IF
	  					Next
	  				Next
	  				if DasIsTZ Then
	  					ReturnString = ReturnString & vbCrLf & GetMethodsOfTablicaZnacheniy(0)
	  				else
	  					IF (SMetodami = 1) And (Len(MethodsSring) = 0) Then
		  					MethodsSring = vbCrLf & GetMethodOfRegister(1, mdo)
		  				end if
					end if
	  			end if
				If 	Find Then
	  				Exit For
	  			end if
	        end if
	        if UC(TypeObj) = UC("Документ") Then 'Документ
				if UC(NameObj) = UC(mdo.Name) Then
					Find = True
					Set CommonRekv = MetaData.TaskDef.Childs("ОбщийРеквизитДокумента")
					CommonRekvCount = CommonRekv.Count - 1
					For CRC = 0 To (CommonRekvCount)
	  					If Len(ReturnString)>0 Then
	  						ReturnString = ReturnString  &  vbCrLf & CommonRekv(CRC).Name
	  					Else
	  						ReturnString = CommonRekv(CRC).Name
	  					End IF
					Next

				  	Set Head = mdo.Childs("РеквизитШапки")
				  	Set Table = mdo.Childs("РеквизитТабличнойЧасти")
	  				for hd_cnt = 0 To Head.Count - 1
	  					set hd_chld = Head(hd_cnt)
	  					If Len(ReturnString)>0 Then
	  						ReturnString = ReturnString  &  vbCrLf & hd_chld.Name
	  					Else
	  						ReturnString = hd_chld.Name
	  					End IF
	 				Next

	 				if ((RWord.IsBetweenToken) And (CompareNoCase(RWord.BTMeth, "выгрузитьтабличнуючасть", 1))) OR (trim(lcase(RWord.TypeVid2)) = "таблицазначений") Then
	 					ReturnString = ""
	 				End IF
	  				for tbl_cnt = 0 To Table.Count - 1
	  					set tbl_chld = Table(tbl_cnt)
	  					If Len(ReturnString)>0 Then
	  						ReturnString = ReturnString  &  vbCrLf & tbl_chld.Name
	  					Else
	  						ReturnString = tbl_chld.Name
	  					End IF
	  				Next
	  			end if
	  			IF (SMetodami = 1) And (Len(MethodsSring) = 0) And Not (trim(lcase(RWord.TypeVid2)) = "таблицазначений") Then
	  				MethodsSring = vbCrLf & GetDocMet(1)
				end if
				IF (trim(lcase(RWord.TypeVid2)) = "таблицазначений") Then
					ReturnString = ReturnString & GetMethodsOfTablicaZnacheniy(0)
				end if
				If 	Find Then
	  				Exit For
	  			end if
	        end if
	        if UC(TypeObj) = UC("Справочник") Then
	          	if UC(NameObj) = UC(mdo.Name) Then
	          		Find = True
	          		if (Clng(mdo.Props("ДлинаКода"))>0) Then
	 					ReturnString = "Код"
	          		End if
	          		if (Clng(mdo.Props("ДлинаНаименования"))>0) Then
	  					If Len(ReturnString)>0 Then
	  						ReturnString = ReturnString  & vbCrLf & "Наименование"
	  					End IF
	          		End if
	          		Set Ref = mdo.Childs("Реквизит")
	          		For r = 0 To Ref.Count -1
	          			Set RefChild = Ref(r)
	  					If Len(ReturnString)>0 Then
	  						ReturnString = ReturnString  &  vbCrLf & RefChild.Name
	  					Else
	  						ReturnString = RefChild.Name
	  					End IF
	          		next
	          		ReturnString = SortStringForList(ReturnString, vbCrLf)
	          	End if
				If Find Then
		  			IF (SMetodami = 1) And (Len(MethodsSring) = 0) Then
	  					MethodsSring = vbCrLf & GetSprMet(1)
					end if
					If (Lcase(BTMeth) = Lcase("Выбрать")) OR (Lcase(BTMeth) = Lcase("Выбрать")) Then
						If (BTNumberParams	= 2) Then
							ReturnString = GetFormListsOfSprav(TypeObj,NameObj)
							MethodsSring = ""
						end if
					end if
	  				Exit For
	  			end if
	        End if
       		if (UC(TypeObj) = UC("Перечисление")) And (NameObj <> "`") then
	          	if UC(NameObj) = UC(mdo.Name) Then
	          		Find = True
	          		Set Ref = mdo.Childs("Значение")
	          		For r = 0 To Ref.Count -1
	          			Set RefChild = Ref(r)
	          			if Len(ReturnString)>0 Then
	          				ReturnString = ReturnString & vbCrLf & RefChild.Name
	          			Else
	          				ReturnString =  RefChild.Name
	          			End if
	          			'if glRWord.IsIcvalVid
						'ReturnString = " = " & TypeObj & "." & NameObj & "." & RefChild.Name & vbCrLf & ReturnString

	 					' Получилась строка НаименованиеРеквизита,РС,ПолныйТипТип
	          		next
	          		ReturnString = SortStringForList(ReturnString, vbCrLf)
    			    ReturnString = ReturnString & vbCrLf &  LoadMethodFromFile(TypeObj)
	          	End if
				If 	Find Then
	  				Exit For
	  			end if

			end if
	    Next
	Elseif (UC(TypeObj) = UC("Форма")) Or (UC(TypeObj) = UC("Обработка")) Or (UC(TypeObj) = UC("Отчет")) Or (UC(TypeObj) = UC("CWBModuleDoc::")) Then
		ReturnString = GetTableRecvFromForms(0,False)
	Elseif (TypeObj = cnstNameTypeSZ) Then
		ReturnString = GetMethodsOfListBox("")
	Elseif (TypeObj = cnstNameTypeTZ) Then
		ReturnString = GetMethodsOfTablicaZnacheniy("")
	Elseif (TypeObj = cnstNameTypeTabl) Then
		ReturnString = GetMethodsOfTablica("")
	Elseif (TypeObj = UC("Текст")) Then
		ReturnString = GetMethodsText("")
	End if
	'ReturnString = SortStringForList(ReturnString, vbCrLf)
	GetStringRekvizitovFromObj = ReturnString & MethodsSring
End Function


Private Function GetFormListsOfSprav(TypeObj,NameObj)
	GetFormListsOfSprav = ""
	Set MetaRef  = MetaData.TaskDef.Childs(CStr(TypeObj))(CStr(NameObj))
	Set CHI = MetaRef.Childs
	IF CHI.Item(1).Count<> 0 Then
		For iii = 0 To CHI.Item(1).Count - 1
			If Len(GetFormListsOfSprav)>0 Then
				GetFormListsOfSprav = GetFormListsOfSprav & vbCrLf & CHI.Item(1).Item(iii).Name
			Else
				GetFormListsOfSprav = CHI.Item(1).Item(iii).Name
			End IF
		Next
	End IF

End Function



Private Function GetListKindVariable(tmpType)
	GetListKindVariable = ""
	If Len(tmpType)>0 Then
		strListKind = ""
	    Set Childs = MetaData.TaskDef.Childs(CStr(tmpType))
	    For i = 0 To Childs.Count - 1
	        Set mdo = Childs(i)
	        if Len(strListKind)>0 Then
				strListKind = strListKind & vbCrLf & mdo.Name
			else
				strListKind =  mdo.Name
			End IF
		next
		GetListKindVariable = SortStringForList(strListKind, vbCrLf)
	End IF
End Function


Private Function GetTableRecvFromForms(STipom,IsBetweenToken) ' Извлекаем реквизиты из формы
	IDDimRekvOfForms = ""
	IDDimRekvOfFormsTCH = ""
	FormMarkers = Array("{""Fixed"",", "{""Controls"",", "{""Cnt_Ver"",""10001""}}") 'Массив маркеров с которых начинаЮтся секци контролов, с нужным форматированием.
	ShablonEndControl = Array(""",""{"""""  ,  """"","""""  ,  """""}""},") 'Шаблон окончания контрола из секции <{"Controls",>
	GetTableRecvFromForms = ""
 	If Windows.ActiveWnd Is Nothing Then
 		Exit Function
 	End If
	Set doc = Windows.ActiveWnd.Document
	If doc<>docWorkBook Then
		Exit Function
	End If
	Set docForm = Windows.ActiveWnd.Document.Page(0)
	DlgText = docForm.Stream
	Dim DimDSFormStrims
	if IsBetweenToken Then
		Patern = "[^]+[{]+[" & cnstRExWORD & """]+[,]+[0-9""]+[}]{1,2}[,]+"
		ttext = FindInStrEx(Patern,DlgText)'IsBetweenToken
		If Len(ttext)>0 Then
			IDDimRekv = ""
			Sloi = split(ttext,vbCrLf)
			If UBound(Sloi)<> -1 Then
				For ee = 0 To UBound(Sloi)
					OneSloy	= Sloi(ee)
					OneSloy = Replace(OneSloy,"""","")
					OneSloyArr = Split(OneSloy,",")
					if Len(IDDimRekv)>0 Then
						IDDimRekv = IDDimRekv & vbCrLf & OneSloyArr(0)
					else
						IDDimRekv = OneSloyArr(0)
					End if
				Next
			End if
			GetTableRecvFromForms  = IDDimRekv
			Exit Function
		End if
	End if
	DimDSFormStrims = split(DlgText,vbCrLf)
	'начинаем проверять с 4-й строки.
	LastStatus = 0 'Последний нужный пройденный керн в файле.
	for i=4 To UBound(DimDSFormStrims)
		StringDialogStrmia = DimDSFormStrims(i)
		For FM = 0 To Ubound(FormMarkers)
			ItamFM = FormMarkers(FM)
			if InStr(1, StringDialogStrmia, ItamFM) = 1 Then
				'Нашли либо "Фиксед" либо "контроль" 'Fixed	Controls
				if InStr(1,ItamFM,"Fixed")>0 Then
					LastStatus = 1
				Elseif InStr(1,ItamFM,"Controls")>0 Then
					LastStatus = 2
				Elseif InStr(1,ItamFM,"Cnt_Ver")>0 Then
					LastStatus = 0
				End If
			End If
		Next
		DSElementsStrin = split(DimDSFormStrims(i), """,""")
		cntItem = Ubound(DSElementsStrin)
		if ((cntItem > 14 ) And (Mid(DimDSFormStrims(i),1,2) = "{""")) Then
			If LastStatus = 1 Then 'многострочная
				IF Len(DSElementsStrin(7)) <> 0 Then
					IndexID = 7
					IDRekv = DSElementsStrin(7)
					Do While InStr(1,IDRekv,"""")>0
						IndexID = IndexID +1
					    Exit Do
					Loop
					IDRekv = DSElementsStrin(IndexID)
					if Len(IDRekv)>0 Then
						If Len(IDDimRekvOfFormsTCH) = 0 Then
							IDDimRekvOfFormsTCH = Trim(DSElementsStrin(7)) '& GetRusTypeAtribOfForms(DSElementsStrin(3))
						Else
							IDDimRekvOfFormsTCH = IDDimRekvOfFormsTCH & vbCrLf & Trim(DSElementsStrin(7)) '& GetRusTypeAtribOfForms(DSElementsStrin(3))
						End if
						if STipom = 1 Then
							IDDimRekvOfFormsTCH = IDDimRekvOfFormsTCH & " " & DSElementsStrin(3)
						elseIf STipom = 2 Then
						End if
					End if
					if UBound(DSElementsStrin)>= (13+IndexID-7) Then
						IDMetaObj = DSElementsStrin(9+IndexID-7)
						IDMetaObj2 = DSElementsStrin(13+IndexID-7) 'ID вида значения (Вид справочника, документа, перечисления или счета)
					End if

				End if
			ElseIf LastStatus = 2 Then 'что меня интересовало, собственно контролы на форме
				If Len(DSElementsStrin(12))>0 Then
					IndexID = 12
					prirashenie = 0
					IDRekv = DSElementsStrin(12)
					Do While InStr(1,IDRekv,"""")>0
						prirashenie = prirashenie + 1
					    Exit Do
					Loop
					IndexID = IndexID + prirashenie

					IDRekv = DSElementsStrin(IndexID)
					if Len(IDRekv)>0 Then
						If Len(IDDimRekvOfForms) = 0 Then
							IDDimRekvOfForms = Trim(IDRekv) '& GetRusTypeAtribOfForms(DSElementsStrin(1))
						Else
							IDDimRekvOfForms = IDDimRekvOfForms & vbCrLf & Trim(IDRekv) '& GetRusTypeAtribOfForms(DSElementsStrin(1))
						End if
					End if
					if STipom = 1 Then
						IDDimRekvOfForms = IDDimRekvOfForms & " " & DSElementsStrin(1)
					elseIf STipom = 2 Then
						aaa = CLng(DSElementsStrin(17))
						if (aaa<>0) Then
							Set obj  = MetaData.FindObject(aaa)
							IDDimRekvOfForms = IDDimRekvOfForms & " " & obj.FullName
						ElseIF (DSElementsStrin(1) = "LISTBOX") OR (DSElementsStrin(1) = "COMBOBOX") Then
							IDDimRekvOfForms = IDDimRekvOfForms & " " & "СписокЗначений"
						ElseIF DSElementsStrin(1) = "TABLE" Then
							IDDimRekvOfForms = IDDimRekvOfForms & " " & "ТаблицаЗначений"
						ElseIF DSElementsStrin(14) = "B" Then
							IDDimRekvOfForms = IDDimRekvOfForms & " " & "Справочник" '& "Справочник."
						ElseIF DSElementsStrin(14) = "O" Then
							IDDimRekvOfForms = IDDimRekvOfForms & " " & "Документ"	'& "Документ."
						End if
					End if

					if UBound(DSElementsStrin)>= (13+IndexID-7) Then
						IDMetaObj2 = DSElementsStrin(13+IndexID-7) 'ID вида значения (Вид справочника, документа, перечисления или счета)
					End if
				End if
			end if
		end if
	Next
	iii = 12
	IDDimRekv = IDDimRekvOfForms & vbCrLf &  IDDimRekvOfFormsTCH
	GetTableRecvFromForms  = IDDimRekv
End Function

Private Function GetMethodsText(GGG)
	MethodsTextArr = Array("КоличествоСтрок()", _
	"ПолучитьСтроку(<?>)", _
	"Открыть(<?>);", _
	"Шаблон(<?>)", _
	"ФиксШаблон(<?>)", _
	"ВставитьСтроку(<?>,);", _
	"ДобавитьСтроку(<?>);", _
	"ЗаменитьСтроку(<?>,);", _
	"УдалитьСтроку(<?>);", _
	"ТолькоПросмотр(<?>)", _
	"Показать(<?>,);", _
	"Очистить();", _
	"КодоваяСтраница(<?>)", _
	"Записать(<?>);")
	MethodsText = Join(MethodsTextArr,vbCrLf)
	GetMethodsText = SortStringForList(MethodsText, Spliter)

End Function
private Function GetMethodsOfFormRecv()
	MethodsOfFormRecv = Array("Видимость(<?>);", _
	"Доступность(<?>);", _
	"Редактирование(<?>);", _
	"Цвет(<?>);", _
	"Заголовок(<?>);", _
	"ВыборГруппы(<?>);", _
	"ВыполнятьФормулуТолькоПриИзменении(<?>);", _
	"УстановитьТип(<?>);", _
	"НазначитьТип(<?>);", _
	"НеИзменятьВид(<?>);", _
	"РасположениеФайла(<?>,);")
	GetMethodsOfFormRecv = Join(MethodsOfFormRecv,vbCrLf)
	MethodsOfFormRecv = SortStringForList(GetMethodsOfFormRecv, Spliter)
End Function

private Function GetMethodsOfForm()
	MethodsOfFormRecv = Array("ТолькоПросмотр(<?>)", _
	"Параметр", _
	"СтрокаДействийФормы", _
	"Обновить(<?>);", _
	"ИспользоватьЗакладки(<?>)", _
	"ИспользоватьСлой(""<?>"",2)", _
	"Закладки", _
	"Закладки.ДобавитьЗначение(<?>,);", _
	"Заголовок(<?>,)", _
	"ПанельИнструментов(<?>);", _
	"КнопкаПоУмолчанию(<?>);", _
	"ОбработкаВыбораСтроки(<?>);", _
	"УстановитьЗначениеВПодборе(<?>,);", _
	"ПолучитьЗначениеИзПодбора(<?>)", _
	"Активизировать(<?>,);", _
	"АктивизироватьОбъект(<?>);", _
	"ВыполнитьВыбор(<?>);", _
	"РежимВыбора();", _
	"МодальныйРежим()", _
	"ПолучитьАтрибут(<?>);", _
	"АктивныйЭлемент()", _
	"ТекущаяКолонка();", _
	"Закрыть();")
	GetMethodsOfForm = Join(MethodsOfFormRecv,vbCrLf)
	GetMethodsOfForm = SortStringForList(GetMethodsOfForm, Spliter)
End Function

private Function GetMethodsOfFormPodbor()
	GetMethodsOfFormPodborRecv = Array("ТолькоПросмотр(<?>)", _
	"Параметр", _
	"СтрокаДействийФормы", _
	"Обновить(<?>);", _
	"ИспользоватьДату(<?>);", _
	"ИспользоватьРодителя(<?>,);", _
	"ИспользоватьВладельца(<?>,);", _
	"ИерархическийСписок(<?>,)", _
	"РедактироватьВДиалоге(<?>,);", _
	"СохранениеПериодическихРеквизитов(<?>,);", _
	"Сортировка(<?>,);", _
	"УстановитьОтбор(<?>,);", _
	"ПолучитьОтбор(<?>,);", _
	"ВидыОтбора(<?>)", _
	"ЗакладкиОтбора(<?>,);", _
	"ИспользоватьСписокЭлементов(<?>);", _
	"ВыборГруппы(<?>);", _
	"ПросмотрИстории(<?>);")
	GetMethodsOfFormPodbor = Join(GetMethodsOfFormPodborRecv,vbCrLf)
	GetMethodsOfFormPodbor = SortStringForList(GetMethodsOfFormPodbor, Spliter)
End Function



private Function GetMethodsOfTablica(InForms)
	MethodsOfTablica = Array("ТекущийОбъект",_
	"ИсходнаяТаблица(<?>);",_
	"Открыть(<?>);",_
	"Вывести();",_
	"ПолучитьСекцию(<?>);",_
	"ВывестиСекцию(<?>);",_
	"ПрисоединитьСекцию(<?>);",_
	"НоваяСтраница(<?>);",_
	"НоваяКолонка(<?>);",_
	"ШиринаТаблицы()",_
	"ВысотаТаблицы()",_
	"ШиринаСекции(<?>)",_
	"ВысотаСекции(<?>)",_
	"ТолькоПросмотр(<?>)",_
	"Показать(<?>);",_
	"Очистить();",_
	"Защита(<?>)",_
	"Записать(<?>,);",_
	"Область(<?>)",_
	"ПовторятьПриПечатиСтроки(<?>,);",_
	"ПовторятьПриПечатиСтолбцы(<?>,);",_
	"Опции(<?>);",_
	"ПараметрыСтраницы(<?>,);",_
	"КоличествоЭкземпляров(<?>)",_
	"Напечатать(<?>);")
	GetMethodsOfTablica = Join(MethodsOfTablica,vbCrLf)
	GetMethodsOfTablica = SortStringForList(GetMethodsOfTablica, Spliter)
End Function




private Function GetMethodsOfTablicaZnacheniy(InForms)
	if Len(InForms) = 0 Then
		MethOfListBox = Array("НомерСтроки", _
		"КоличествоКолонок(<?>)", _
		"НоваяКолонка(<?>);", _
		"ВставитьКолонку(<?>);", _
		"УдалитьКолонку(<?>);", _
		"УстановитьПараметрыКолонки(<?>);", _
		"ПолучитьПараметрыКолонки(<?>);", _
		"КоличествоСтрок()", _
		"НоваяСтрока();", _
		"УдалитьСтроку(<?>);", _
		"УдалитьСтроки();", _
		"ВыбратьСтроки();", _
		"ПолучитьСтроку() = 1", _
		"ВыбратьСтроку(<?>);", _
		"ПолучитьСтрокуПоНомеру(<?>);", _
		"СдвинутьСтроку(<?>,);", _
		"УстановитьЗначение(<?>);", _
		"ПолучитьЗначение(<?>,);", _
		"НайтиЗначение(<?>);", _
		"Сортировать(<?>,);", _
		"Очистить();", _
		"Итог(<?>);", _
		"Заполнить(<?>,);", _
		"Свернуть(<?>,);", _
		"Выгрузить(<?>,);", _
		"Загрузить(<?>);")
	Else
		MethOfListBox = Array("НомерСтроки", _
		"КоличествоКолонок(<?>)", _
		"НоваяКолонка(<?>);", _
		"ВставитьКолонку(<?>);", _
		"УдалитьКолонку(<?>);", _
		"УстановитьПараметрыКолонки(<?>,);", _
		"ПолучитьПараметрыКолонки(<?>,);", _
		"КоличествоСтрок()", _
		"НоваяСтрока(<?>);", _
		"УдалитьСтроку(<?>);", _
		"УдалитьСтроки();", _
		"ВыбратьСтроки();", _
		"ПолучитьСтроку()", _
		"ВыбратьСтроку(<?>);", _
		"ПолучитьСтрокуПоНомеру(<?>);", _
		"СдвинутьСтроку(<?>,);", _
		"УстановитьЗначение(<?>);", _
		"ПолучитьЗначение(<?>,);", _
		"НайтиЗначение(<?>);", _
		"Сортировать(<?>,);", _
		"Очистить();", _
		"Итог(<?>);", _
		"Заполнить(<?>,);", _
		"Свернуть(<?>,);", _
		"Выгрузить(<?>,);", _
		"Загрузить(<?>);", _
		"ТекущаяСтрока();", _
		"ТекущаяКолонка(<?>,);", _
		"ВидимостьКолонки(<?>);", _
		"Фиксировать(<?>,);", _
		"ВыводитьПиктограммы(<?>,);")
	End if
	GetMethodsOfTablicaZnacheniy = Join(MethOfListBox,vbCrLf)
	GetMethodsOfTablicaZnacheniy = SortStringForList(GetMethodsOfTablicaZnacheniy, Spliter)
End Function


private Function GetMethodsOfListBox(Par)
	if UCase(Par) = "ЗАКЛАДКИ" Then
		MethOfListBox = Array("ДобавитьЗначение(<?>,);", _
		"ВставитьЗначение(<?>,);", _
		"РазмерСписка()", _
		"УстановитьЗначение(<?>,);", _
		"Установить(<?>,);", _
		"Принадлежит(<?>)", _
		"НайтиЗначение(<?>)", _
		"ТекущаяСтрока();", _
		"УдалитьЗначение(<?>);", _
		"УдалитьВсе();", _
		"СдвинутьЗначение(<?>,);", _
		"Выгрузить(<?>);")
	Else
		MethOfListBox = Array("ДобавитьЗначение(<?>);", _
		"ВставитьЗначение(<?>,);", _
		"РазмерСписка()", _
		"УстановитьЗначение(<?>,);", _
		"Установить(<?>,);", _
		"Сортировать(<?>,);", _
		"СортироватьПоПредставлению(<?>);", _
		"Принадлежит(<?>)", _
		"НайтиЗначение(<?>)", _
		"ПолучитьЗначение(<?>);", _
		"Получить(<?>)", _
		"ИзСтрокиСРазделителями(<?>)", _
		"ВСтрокуСРазделителями()", _
		"ВыбратьЗначение(<?>)", _
		"ОтметитьЗначения(<?>,)", _
		"Пометка(<?>,)", _
		"ТекущаяСтрока(<?>);", _
		"УдалитьЗначение(<?>);", _
		"УдалитьВсе();", _
		"СдвинутьЗначение(<?>,);", _
		"Выгрузить(<?>);")
	End if
	GetMethodsOfListBox = Join(MethOfListBox,vbCrLf)
	GetMethodsOfListBox = SortStringForList(GetMethodsOfListBox, Spliter)
End Function

private Function GetRusTypeAtribOfForms(Param)
	RetValue = "<Пусто>"
	Param = Trim(Param)
	if Len(Param) <> 0 Then
		If Param = "1CEDIT" Then
			RetValue = "  <Поле ввода (строка/число/дата)>"
		ElseIf Param = "STATIC" Then 			RetValue = "  <Текст>"
		ElseIf Param = "BMASKED" Then
			RetValue = "  <Выбор значения>"
		ElseIf Param = "BUTTON" Then
			RetValue = "  <Кнопка>"
		ElseIf Param = "PICTURE" Then
			RetValue = "  <Картинка>"
		ElseIf Param = "COMBOBOX" Then
			RetValue = "  <Выпадающий список>"
		ElseIf Param = "TABLE" Then
			RetValue = "  <Таблица значений>"
		ElseIf Param = "TABLE" Then
			RetValue = "  <Таблица значений>"
		ElseIf Param = "1CGROUPBOX" Then
			RetValue = "  <Рамка группы>"
		ElseIf Param = "CHECKBOX" Then
			RetValue = "  <Флажок>"
		ElseIf Param = "RADIO" Then
			RetValue = "  <Переключатель>"
		ElseIf Param = "LISTBOX" Then
			RetValue = "  <Переключатель>"
		End If
	End If
	GetRusTypeAtribOfForms = RetValue
End Function

private Function WordIsAStatment(ResultWord)
	RetValue = False
	ArrStatments = GetListOFStatments()
	Find = 0
	For iii = 0 To UBound(ArrStatments)
		If UCase(Trim(ResultWord)) = UCase(Trim(ArrStatments(iii))) Then
			RetValue = True
			Exit For
		End IF
	Next
	WordIsAStatment = RetValue
End Function

Private Function WordInArray(Arr, Word) GetListOFStatments()
	WordInArray = False
	For qq = 0 To UBound(Arr)
		IF UCase(Arr(qq)) = UCase(Word) Then
			WordInArray = True
			Exit For
		End IF
	Next
End Function

private Function GetListOFStatments()
	MethOfListBox = Array("Процедура", "Procedure", "КонецПроцедуры", "EndProcedure", _
	"Функция", 	"Function", "КонецФункции", "EndFunction", _
	"Если", "If", "Тогда", "Then", "ИначеЕсли", "ElsIf", "Иначе", "Else", "КонецЕсли", "EndIf", _
	"Пока", "While", "Цикл", "Do", "КонецЦикла", "EndDo", "Для", "For", "По", "To", _
	"Попытка", "Try", "Исключение", "Except", "КонецПопытки", "EndTry", "ВызватьИсключение", "Raise", _
	"Перейти", 	"Goto", "Продолжить", "Continue", "Прервать", "Break", "Возврат", 	"Return", _
	"ЗагрузитьИзФайла", "LoadFromFile", _
	"Далее", "Forward", "Экспорт", 	"Export", _
	"РазделительСтраниц", "PageBreak", "РазделительСтрок", "LineBreak", "СимволТабуляции", 	"TabSymbol")
	GetListOFStatments = MethOfListBox
End Function

Private Function GetMethodOfRegister(ASCreateObj, mdo)
	'"-<-----Общие----->-", _
	MethodOfRegister = Array("Приход", _
	"Расход", _
	"Вид()", _
	"ПредставлениеВида();", _
	"НазначитьТип(<?>,)", _
	"УстановитьАтрибут(<?>,);", _
	"ПолучитьАтрибут(<?>);", _
	"ВыбратьДвижения(<?>);", _
	"ВыбратьДвиженияДокумента(<?>);", _
	"ОбратныйПорядок(<?>);", _
	"ПолучитьДвижение();", _
	"ТекущийДокумент();", _
	"НомерСтроки();", _
	"ВыбратьИтоги();", _
	"ПолучитьИтог();", _
	"ВременныйРасчет(<?>);", _
	"ВыгрузитьИтоги(<?>);", _
	"УстановитьФильтр(<?>,);", _
	"УстановитьЗначениеФильтра(<?>);")
	GetMethodOfRegister = Join(MethodOfRegister,vbCrLf)

	if mdo.Props(3) = "Остатки" Then
		'mdo.Props(3)	"Остатки"	String
		'"-<-----Регистр остатков----->-", _
		MethodOfRegister = Array("Остаток(<?>,)", _
		"СводныйОстаток(<?>,)", _
		"Остатки(<?>,);", _
		"СводныеОстатки(<?>,);", _
		"ОстаткиПолучить();", _
		"ВыбратьДвиженияСОстатками(<?>,);", _
		"ДвижениеПриход(<?>,);", _
		"ДвижениеРасход(<?>,);", _
		"ДвижениеПриходВыполнить();", _
		"ДвижениеРасходВыполнить();")
	else
	'"-<-----Оборотный----->-", _
		MethodOfRegister = Array("ИспользоватьПериод(<?>);", _
		"Итог(<?>)", _
		"Итоги(<?>,);", _
		"ИтогиПолучить();",_
		"Движение(<?>,);", _
		"ДвижениеВыполнить();")
	End IF
	GetMethodOfRegister = GetMethodOfRegister & vbCrLf & Join(MethodOfRegister,vbCrLf)

	'"-<-----Системные----->-", _
	MethodOfRegister = Array("РассчитатьРегистрыНа(<?>,);", _
	"РассчитатьРегистрыПо(<?>,);", _
	"ПривязыватьСтроку(<?>);")
	GetMethodOfRegister = GetMethodOfRegister & vbCrLf & Join(MethodOfRegister,vbCrLf)
End Function


private Function SortStringForList(SortStr, Delimiter)
	SortStringForList = CommonScripts.SortString(SortStr, Delimiter)
End Function


private Function GetSprMet(AsCreateObj)
	'IF AsCreateObj = 1 Then
	Method_Spr = Array("Родитель",_
	"Код", _
	"Наименование", _
	"Владелец", _
	"Получить(<?>)", _
	"Установить(<?>,)", _
	"Вид()", _
	"ПредставлениеВида();", _
	"Уровень()", _
	"УстановитьАтрибут(<?>,);", _
	"ПолучитьАтрибут(<?>);", _
	"ЭтоГруппа()", _
	"ПринадлежитГруппе(<?>)", _
	"Выбран()", _
	"Выбрать(<?>,)", _
	"ВидыДляВыбора(<?>)", _
	"ВыборГруппы(<?>)", _
	"ТекущийЭлемент()", _
	"ПолныйКод()", _
	"ПолноеНаименование()", _
	"НайтиЭлемент(<?>)", _
	"НайтиПоКоду(<?>,);", _
	"НайтиПоНаименованию(<?>);", _
	"НайтиПоРеквизиту(<?>);", _
	"ВыбратьЭлементы(<?>)", _
	"ВыбратьЭлементыПоРеквизиту(<?>,)", _
	"ОбратныйПорядок(<?>)", _
	"ПолучитьЭлемент() = 1", _
	"ИспользоватьДату(<?>);", _
	"ИспользоватьВладельца(<?>,);", _
	"ИспользоватьРодителя(<?>,);", _
	"ВключатьПодчиненные(<?>);", _
	"ПорядокКодов();", _
	"ПорядокНаименований();", _
	"ПорядокРеквизита(<?>);", _
	"Новый();", _
	"НоваяГруппа();", _
	"ПрефиксКода(<?>)", _
	"УстановитьНовыйКод(<?>);", _
	"НазначитьТип(<?>,);", _
	"Записать();", _
	"Удалить(<?>);", _
	"Блокировка(<?>);", _
	"СнятьПометкуУдаления();", _
	"ПометкаУдаления()", _
	"ИспользоватьДату(<?>);", _
	"ИспользоватьРодителя(<?>,);", _
	"ИспользоватьВладельца(<?>,);", _
	"ИерархическийСписок(<?>,)", _
	"РедактироватьВДиалоге(<?>,);", _
	"СохранениеПериодическихРеквизитов(<?>,);", _
	"Сортировка(<?>,);", _
	"УстановитьОтбор(<?>,);", _
	"ПолучитьОтбор(<?>,);", _
	"ВидыОтбора(<?>)", _
	"ЗакладкиОтбора(<?>,);", _
	"ИспользоватьСписокЭлементов(<?>);", _
	"ВыборГруппы(<?>)", _
	"ПросмотрИстории(<?>);")
	GetSprMet=Join(Method_Spr,vbCrLf)
End Function

private Function GetDocMet(ASRekvOfForm)
	Method_Doc = Array("АвтоВремяКонецДня(); ", _
	"АвтоВремяНачалоДня(); ", _
	"АвтоВремяОтключить(); ", _
	"АвтоВремяПослеТА(); ", _
	"АвтоВремяТекущее(); ", _
	"АктивизироватьСтроку(<?>); ", _
	"Блокировка(<?>); ", _
	"Вид() ", _
	"ВидыДляВыбора(<?>) ", _
	"Выбран() ", _
	"Выбрать(<?>) ", _
	"ВыбратьДокументы(<?>,) ", _
	"ВыбратьПодчиненныеДокументы(<?>) ", _
	"ВыбратьПоЗначению(<?>,) ", _
	"ВыбратьПоНомеру(<?>) ", _
	"ВыбратьПоПоследовательности(<?>) ", _
	"ВыбратьСтроки() ", _
	"ВыгрузитьТабличнуюЧасть(<?>,); ", _
	"Графа(<?>); ", _
	"ГрупповаяОбработка() ", _
	"ДатаДок ", _
	"ЗагрузитьТабличнуюЧасть(<?>); ", _
	"Записать(); ", _
	"Записать(); ", _
	"ИзменениеПорядкаСтрок(<?>); ", _
	"ИспользоватьЖурнал(<?>,); ", _
	"Итог(<?>) ", _
	"ИтогиАктуальны() ", _
	"КоличествоСтрок() ", _
	"Модифицированность() ", _
	"НазначитьТип(<?>,) ", _
	"НайтиДокумент(<?>) ", _
	"НайтиПоНомеру(<?>) ", _
	"НеПроводитьДокумент(); ", _
	"НоваяСтрока(); ", _
	"Новый(); ", _
	"НомерДок ", _
	"ОбратныйПорядок(<?>); ", _
	"Операция ", _
	"ОчиститьДвижения(<?>); ", _
	"ПолучитьАтрибут(<?>); ", _
	"ПолучитьВремя(<?>) ", _
	"ПолучитьДокумент() ", _
	"ПолучитьПозицию(); ", _
	"ПолучитьСтроку() ", _
	"ПолучитьСтрокуПоНомеру(<?>) ", _
	"ПометкаУдаления() ", _
	"ПредставлениеВида(); ", _
	"ПрефиксНомера(<?>) ", _
	"ПривязыватьСтроку(<?>); ", _
	"ПриЗаписиПерепроводить(<?>); ", _
	"ПринадлежитПоследовательности(<?>,); ", _
	"Проведен() ", _
	"Провести(<?>,); ", _
	"ПроводитьПослеТА(<?>,); ", _
	"СделатьНеПроведенным(); ", _
	"СнятьПометкуУдаления(); ", _
	"СортироватьСтроки(<?>); ", _
	"СравнитьТА() ", _
	"ТекущийДокумент() ", _
	"Удалить(<?>); ", _
	"УдалитьСтроки(); ", _
	"УдалитьСтроку(); ", _
	"УстановитьАтрибут(<?>,); ", _
	"УстановитьВремя(<?>); ", _
	"УстановитьНовыйНомер(<?>); ", _
	"УстановитьРеквизитСправочника(<?>); ", _
	"УстановитьФильтр(<?>);")
	GetDocMet=Join(Method_Doc,vbCrLf)
End Function

private Function GetZaprosMet()
	Method_Zapros = Array("вНачалоВыборки()", _
		"ЭтоГруппа(<?>)", _
		"ВНачалоВыборки()", _
		"Группировка()", _
		"НачалоПериода()", _
		"КонецПериода()", _
		"Выполнить(<?>)", _
		"Получить(<?>)", _
		"Выгрузить(<?>)", _
		"ЗначениеУпорядочивания(<?>,)", _
		"ПолучитьАтрибут(<?>)")
	GetZaprosMet=Join(Method_Zapros,vbCrLf)
End Function

Private Function GetMethodsRegisters()
	GetMethodsRegisters = ""
	ArrMethodsRegisters = Array("ПолучитьАтрибут(<?>);", _
	"РассчитатьРегистрыНа(<?>);", _
	"РассчитатьРегистрыПо(<?>);", _
	"Актуальность(<?>);")
	GetMethodsRegisters = Join(ArrMethodsRegisters,vbCrLf)
End Function

Private Function FindReturnValueFunction(tAllText)
	FindReturnValueFunction = ""
	tempType = FindInStrEx("(;|^|\s)+(Return|Возврат)+\s+[" & cnstRExWORD & "]+\s*[;]+", tAllText)
	If Len(tempType)>0 Then
		ArrFind = Split(tempType,vbCrLf)
		If UBound(ArrFind)<>-1 Then
			For qq = 0 To UBound(ArrFind)
				tempType = FindInStrEx("(;|^|\s)+(Return|Возврат)+\s+", ArrFind(qq))
				ArrFind(qq) = Replace(ArrFind(qq),tempType,"")
				ArrFind(qq) = ReplaceEx(ArrFind(qq),Array(";","", vbTab," "))
				ArrFind(qq) = Trim(ArrFind(qq))
				if Len(ArrFind(qq))>0 Then
					FindReturnValueFunction = ArrFind(qq)
					Exit For
				End IF
			Next
		End IF
	End IF
End Function



'В AddWords можно помеситьть допустим Выражение из
'"Таб.ИсходнаяТаблица("Сформировать");" AddWords = Сформировать
Private Function GetTypeFromTextRegExps(ResultWord, AddWords)
	'message ResultWord & " + " & AddWords
	GetTypeFromTextRegExps = ""
	if ResultWord = "?" Then
		Exit Function
	End If
	If GlobalVariableType.Exists(lcase(ResultWord)) Then
		GetTypeFromTextRegExps = GlobalVariableType.Item(lcase(ResultWord))
		IF Len(GetTypeFromTextRegExps)>0 Then
			Exit Function
		End If
	End If
	if (InStr(1, ResultWord,".") > 0) And False Then
		Set nVar = New TheVariable
		nVar.ExtractDef(ResultWord)
		ResultWord = nVar.V_Type
		'if (nVar.WordsCnt>1)
	End If

	doc = ""
	If glDoc <> "" Then
		Set doc = glDoc
	Else
		if Not CheckWindow(doc) then Exit Function
	End If

	AddText = ""
	RezTempTypeAdd = "" 'Результат нечеткого сравнения..... Используется если ничего путного с парсингом не вышло....
	'virajenieCO = "[\s|^|;]+(" & ResultWord & ")[\s]*["

	'План такой, если мы в глобальном модуле, тогда выбираем только текст данной процедуры
	'иначе, сначала выбираем только текст процедуры, потом, если не помогает, тогда
	'весь текст с начала и до конца
	'ПОТОМ Посылаем переменную в процедуру анализа и ждем реакции

	StartLileEnd = 0
	nctStep	= 1
	if doc.Name = "Глобальный модуль" Then
		nctStep = 0 'В глобальнике не берем весь текст
		if (glDoc<>"") And (glStartLileEnd<>-1) Then
			StartLileEnd = glStartLileEnd
		End IF
	End IF
	Rezult = ""
	For cStep = 0 To nctStep
		If cStep = 0 Then
			StartLile = StartScanLineText
		Else
			StartLile = Doc.LineCount
		End If
		For i = StartLile To StartLileEnd Step -1
			StartScanLineText = i
			ttext = " " & Trim(Doc.range(i))
			If InStr(1,UC(ttext),UC(ResultWord))>0 Then
				If inStr(1,ttext,"//")>0 Then
					Patern = "(\s|^)*(//)+(\s)*(" & ResultWord& ")(\s)*[=]+(\s)*[""]*(\s)*[a-zA-zа-яА-Я0-9_.]+(\s)*[""]*(\s)*" '(\+)*(\s)*(таблицазначений)*"
					tempType = FindInStrEx(Patern, ttext)
					If Len(tempType) = 0 Then
						Patern = "(\s|^)*(//)+(\s)*(" & ResultWord & "." & AddWords	 & ")(\s)*[=]+(\s)*[""]*(\s)*[a-zA-zа-яА-Я0-9_.]+(\s)*[""]*(\s)*"' (\+)*(\s)*(таблицазначений)*"
						tempType = FindInStrEx(Patern, ttext)
					End If
					If Len(tempType)>0 Then
						RezultStr = tempType
						tempType = Replace(tempType,"""", "")
						tempType = FindInStrEx("[=]+(\s)*[a-zA-zа-яА-Я0-9_.]+", tempType)
						RezultStr = Replace(RezultStr,tempType,"")
						RezultStr = ReplaceEx(RezultStr,Array("//","", vbTab,""," ",""))
						If Len(tempType)>0 Then
							tempType = FindInStrEx("[a-zA-zа-яА-Я0-9_.]+", tempType)
							If Len(tempType)>0 Then
								tempType = Trim(tempType)
								tempTypeArr = Split(tempType,".")
								if UBound(tempTypeArr) = 1 Then
									IF (UCase(tempTypeArr(0)) = UCase("Документ")) OR (UCase(tempTypeArr(0)) = UCase("Справочник")) OR (UCase(tempTypeArr(0)) = UCase("Регистр")) Then
										if CompareNoCase(ResultWord,RezultStr,1) Then
											If Len(AddWords)<>0 Then
												tempType = GetTypeVid(AddWords,tempType, Doc,0,"")
											End If
										End If
										if Len(tempType)<>0 Then
											AddWords = ""
											Rezult = tempType
											Exit For
										End If
									End If
								Else
									AddWords = ""
									Rezult = tempType
									Exit For
								End If
							End If
						End If
					End If
					ttext = Mid(ttext,1,inStr(1,ttext,"//"))
				End If
				' Сканируем модуль на предмет вызова процедур/функций например для "ТаблДопИнформации" из
				'глНайтиВнешниеПечатныеФормыОбъекта("Документ.РасходнаяНакладная", ТаблДопИнформации);
				RezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "]+\s*\(+.*("&ResultWord&")+.*\)+",ttext) 'Переменная находится в параметре
				tRezultStr = FindInStrEx("(\s|^|;)+("&ResultWord&")+\s*\=+\s*[" & cnstRExWORD & "]+\(+.*\)+",ttext)	' для "ТаблДокумента" из -> "ТаблДокумента = глПодготовитьТаблицуДокумента(Контекст);"

				if Len(RezultStr)>0 Or Len(tRezultStr)>0 Then
					Parameters = -1 'Где сидит слово, которое надо распознать... -1 означает возвращаемое значение..
					FindPF = false : FindPFInGlobalModule = False
					' в RezultStr помещаем имя
					' в tRezultStr помещаем параметры
					Shema = 1
					if Len(RezultStr)>0 Then 'Если сработала ПЕРВАЯ схема
						RezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "]+\s*\(+.*("&ResultWord&")+.*\)+",RezultStr)
						tRezultStr = FindInStrEx("\(+.*("&ResultWord&")+.*\)+",RezultStr)
						'Извлекаем имя..
						RezultStr = Replace(RezultStr,tRezultStr,"")
						RezultStr = ReplaceEx(RezultStr, Array(vbTab,"", vbCrLf,"", vbCr,""))
						tempTypeArr = Split(GetParams(tRezultStr),vbCrLf)
						If UBound(tempTypeArr)<>-1 Then
							For qq = 0 To UBound(tempTypeArr)
								'Stop
								If InStr(1,LCase(tempTypeArr(qq)),LCase(ResultWord))>0 Then
									Parameters = qq + 1
									Exit For
								End If
							Next
						End If
					Elseif Len(tRezultStr)>0 Then 'Если сработала ВТОРАЯ схема
						Shema = 2
						RezultStr = FindInStrEx("\=+\s*[" & cnstRExWORD & "]+\(+",tRezultStr)
						RezultStr = ReplaceEx(RezultStr, Array(vbTab,"", vbCrLf,"", vbCr,"", "=","", "(",""))
						tRezultStr = FindInStrEx("\(+.*\)+",tRezultStr)
					End If
					RezultStr = Trim(RezultStr)
					Procedure = ""
					If Not IsEmpty(LocalModule) Then
						On Error Resume Next
						if LocalModule.IsProcedure(RezultStr, "", "", "","",Procedure) Then
							FindPF = True
						End If
					End If
					IF Not IsEmpty(GlobalModule) And GlobalModule<>"" Then
						if LocalModule.ModuleName<>GlobalModule.ModuleName Then
							if GlobalModule.IsProcedure(RezultStr, "", "", "","",Procedure) Then
								FindPF = True : FindPFInGlobalModule = True
							End If
						End If
					End If
					if FindPF Then
						tRezultStr = GetParams(tRezultStr)
						tempTypeArr = Split(tRezultStr,vbCrLf)
						If UBound(tempTypeArr)<>-1 Then
							For qq = 0 To UBound(tempTypeArr)
								'Stop
								If InStr(1,LCase(tempTypeArr(qq)),LCase(ResultWord))>0 Or Shema = 2 Then
									tRezultStr = glRWord.ArrProcFuncFromTheWord
									glRWord.ArrProcFuncFromTheWord = tRezultStr
									If AddToStringUni(tRezultStr, RezultStr,",") Then
										tRezultStr = glRWord.ArrProcFuncNumbParam
										if Shema = 1 Then
											AddToStringUni tRezultStr, ""&(qq+1),","
										Else
											AddToStringUni tRezultStr, ""&(-1),","
										End IF
										glRWord.ArrProcFuncNumbParam = tRezultStr
										End If
									Exit For
								End If
							Next
						End If
					End If
					'Если нашли в глобальнике, тогда что-же делать, надо посмотреть там...
					'переключаемся на глобальник и смотрим там...
					if FindPFInGlobalModule Then
						'Stop
							tRezultStr = FindReturnValueFunction(Procedure.text)
							Procedure.RetValueStr = tRezultStr
						if Parameters <> -1 Then 'Это типа функция
							tRezultStr = Procedure.GetParamNumber(Parameters)
						End If
						If Len(tRezultStr)>0 Then
							'Запоминаем позиции сканирования
							LastglStartLileEnd = glStartLileEnd
							LastStartScanLineText = StartScanLineText
							glStartLileEnd = Procedure.LineStart
							StartScanLineText = Procedure.LineEnd

							Set glDoc = Documents("Глобальный модуль")
							Set tLastDoc = Doc
							tRezultStr = GetTypeFromTextRegExps(tRezultStr, AddWords)
							'Восстанавливаем позиции сканирования
							glDoc = ""
							Set Doc = tLastDoc
							LastglStartLileEnd = glStartLileEnd
							LastStartScanLineText = StartScanLineText
							If Len(tRezultStr)>0 Then
								Rezult = tRezultStr
								AddWords = ""
								Exit For
							End If

						End If
					End If
				End If

				'GetColumnsFromTZ(ResultWord, STipom)
				RezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "]+(\s)*[.]+(\s)*(ВыгрузитьТабличнуюЧасть|UnloadTable)+(\s)*[\(]+(\s)*(" & ResultWord &")+.*[\)]+(\s|^|;)", ttext)
				tRezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "\s\.]+(\s)*[.]+(\s)*(ВыгрузитьТабличнуюЧасть|UnloadTable)+(\s)*[\(]+(\s)*(" & ResultWord &")+.*[\)]+(\s|^|;)", ttext)
				if Len(RezultStr)> 0 Then
					RezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "]+(\s)*[.]+", RezultStr)
					if Len(RezultStr)> 0 Then
						RezultStr = FindInStrEx("[" & cnstRExWORD & "]+", RezultStr)
						if Len(RezultStr)> 0 Then
							RezultStr = GetTypeFromTextRegExps(RezultStr,"")
							if Len(RezultStr)> 0 Then
								IF Len(AddWords)>0 Then
									RezultStr = GetTypeVid(AddWords,RezultStr, Doc,0,"")
									if Len(RezultStr)>0 Then
										Rezult = RezultStr
									End If
								Else
								'Stop
									glRWord.BTMeth = "выгрузитьтабличнуючасть"
									Rezult = RezultStr & "+таблицазначений"
								End If
								exit For
							End If
						End If
					End If
				Elseif Len(tRezultStr)> 0 Then
					'Stop
					tRezultStr = FindInStrEx("(\s|^|;)+[" & cnstRExWORD & "\s\.]+(\s)*[.]+", tRezultStr)
					if Len(tRezultStr)> 0 Then
						tRezultStr = FindInStrEx("[" & cnstRExWORD & "]+", tRezultStr)
						RezultStr = ""
						if InStr(1,tRezultStr,vbCrLf)>0 Then
							tempTypeArr	= Split(tRezultStr,vbCrLf)
							tRezultStr = ""
							for qqq = 0 To UBound(tempTypeArr)
								tempTypeArr(qqq) = trim(tempTypeArr(qqq))
								if qqq > 0 Then
									if Len(tRezultStr)> 0 Then
										tRezultStr = tRezultStr &"."&tempTypeArr(qqq)
									else
										tRezultStr = tempTypeArr(qqq)
									End IF
								else
									RezultStr = tempTypeArr(qqq)
								End IF
							Next
						End IF
						if Len(tRezultStr&RezultStr)> 0 Then
							tRezultStr = GetTypeFromTextRegExps(RezultStr,tRezultStr)
							if Len(AddWords)>0 Then
								tRezultStr = GetTypeVid(AddWords,tRezultStr, Doc,0,"")
								if Len(tRezultStr)> 0 Then
									AddWords = ""
									Rezult = tRezultStr
									exit For
								End If
							Else
								if Len(tRezultStr)> 0 Then
								'Stop
									if InStr(1,tRezultStr,"таблицазначений") = 0 Then
										Rezult = tRezultStr & "+таблицазначений"
									Else
										Rezult = tRezultStr
									End If
									exit For
								End If
							End If

						End If
					End If
				End If


				Patern = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*(Выгрузить|Unload)+(\s)*[\(]+(\s)*(" & ResultWord &")+.*[\)]+(\s|^|;)"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then					'Откопаем переменную запроса
					tRezultStr = ""
					textVarZapros = FindInStrEx("(\s)*[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*",RezultStr)
					'Здесь косяк - данная переменна может быть и не запросом :(
					' Исправить............
					If Len(textVarZapros)>0 Then						'почистим
						textVarZapros = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+",textVarZapros)
					End IF
					Patern = "[\(]+(\s)*(" & ResultWord &")+.*[\)]+"
					RezultStr = FindInStrEx(Patern,RezultStr)
					RezultStr = ReplaceEx(RezultStr,Array("(","",")",""," ",""))
					RezultStr = FindInStrExA("[" & cnstRExWORD & "]+",RezultStr)
					If UC(RezultStr) = UC(ResultWord) Then
						tempType = GetTypeFromTextRegExps(textVarZapros,"")
						if InStr(1,tempType,"+таблицазначений")>0 Then
							tempType = Replace(tempType,"+таблицазначений","")
						End if
						if (UC(tempType) = "ЗАПРОС") Then
							RezultStr = GetVariableAndFunctionZapros(textVarZapros, 1)
						Elseif (UC(tempType) = UC(cnstNameTypeTZ)) Then
							RezultStr = GetColumnsFromTZ(textVarZapros, 1)
						ElseIf Instr(1,Lcase(tempType),LCase("Регистр."))>0 Then
							tempTypeArr = Split(tempType,".")
							glRWord.TypeVid2 = "таблицазначений"
							RezultStr = GetStringRekvizitovFromObj(tempTypeArr(0), tempTypeArr(1),2,0, "",	0, glRWord)
						End if
						'AddWords - не забыть
						if Len(RezultStr)>0 Then
							tempTypeArr = Split(RezultStr,vbCrLf)
							tRezultStr = "" : Arr2 = "" : Arr3 = ""
							if InStr(1,AddWords,".")>0 Then
								Arr3 = Split(AddWords,".")
							Else
								Arr3 = Array(AddWords)
							End IF
							if UBound(tempTypeArr)>0 Then
								For qqq = 0 To UBound(tempTypeArr)
									RezultStr = tempTypeArr(qqq)
									if inStr(1,RezultStr,"=")>0 Then
										Arr2 = Split(RezultStr,"=")
										if UBound(Arr2)>0 Then ' Вот эта строка нам и нужна..
											IF Lcase(Trim(Arr2(0))) = Trim(LCase(Arr3(0))) Then 'AddWords
												Arr2(1) = Replace(Arr2(1),";",",")
												Arr2(0) = Split(Arr2(1),",")
												if UBound(Arr2(0))<>-1 Then
													tRezultStr = trim(Arr2(0)(0))
													Exit For
												End if
											End if
										End if
									End if
								Next
								if Len(tRezultStr)>0 Then
									Arr2 = Split(tRezultStr,".")
									if UBound(Arr2) = 2 Then
										tRezultStr = GetTypeVid(Arr2(2),Arr2(0)&"."&Arr2(1), Doc,0,"")
									ElseIf UBound(Arr3)>0 Then
										'Подработка множества слов в AddWords
										AddWords = ""
										For qqq = 1 To UBound(Arr3)
											tRezultStr = GetTypeVid(Arr3(qqq),tRezultStr, Doc,0,"")
										Next
									End if
									if Len(tRezultStr)>0 Then
										Rezult = tRezultStr
										AddWords = ""
										Exit For
									End if
									'Exit Function
								End if
								'Rezult = Rezult & vbCrLf &  RezultStr
							End if
						End if
						'Exit For
					End if
				End if


				' Для вычисления сравнений для переменной например "ВидДок" можно отпарсить конструкцию типа
				'" 		ИЛИ (ВидДок=""РасходныйОрдерТБ"")" понять по ней какого типа идет сравнение и врубиться
				' с каким типом сравниваем...
				If glRWord.IsIcvalVid Then
					if InStr(1,ResultWord,".") = 0 Then
						FindFirstInFindInStrEx = True 'берем только первое вхождение
						tempType = FindInStrEx("[\s|;|\(]+(" &ResultWord&")+\s*\=+\s*[""""]+[" & cnstRExWORD & "]+[""""]", ttext)
						FindFirstInFindInStrEx = false
						if Len(tempType)>0 Then
							tempType = FindInStrEx("\=+\s*[""""]+[" & cnstRExWORD & "]+[""""]", tempType)
							if Len(tempType)>0 Then
								tempType = ReplaceEx(tempType, Array("""","", "=",""))
								if (Len(tempType)>0) And (InStr(1,tempType,".")=0) Then
									tempType = ReplaceEx(tempType, Array("""","", "=",""))
									tempType = GetTypeFromVid(tempType)
									if Len(tempType)>0 Then
										rezult = tempType
										Exit For
									End If
								End If
							End If
						End If
					End If
				End If

				'Ищем в функциях
				Patern = "(\s|^)*[^.][" & cnstRExWORD & "]+[\(]+[^@]+[\)]+(\s)*(;)+" ' "
				tempType = FindInStrEx(Patern, ttext)
				if Len(tempType)>0 Then
					tempType = AnalizProcedure(tempType,ResultWord)
					if Len(tempType)> 0 Then
						Rezult = tempType
						Exit For
					End If
				End If

				'Вычислим тип реквизита исполдьзуя нечеткое соспоставление по "уникальным" реквизитам/методам
				'UniMetodsAndRekv
				Patern = "(\s|;)+(" & ResultWord &  ")+[\s]*[.]+[\s]*[" & cnstRExWORD & "]+[\s]*[\(|;|=|\.|<]+" ' "
				tempTypeAdd = FindInStrEx(Patern, ttext)
				if Len(tempTypeAdd)>0 Then
					Patern = "(\s|;)+(" & ResultWord &  ")+[\s]*[.]+[\s]*[" & cnstRExWORD & "]+" ' "
					tempTypeAdd = FindInStrEx(Patern, tempTypeAdd)
					if Len(tempTypeAdd)> 0 Then
						Patern = "[.]+[\s]*[" & cnstRExWORD & "]+" ' "
						tempTypeAdd = FindInStrEx(Patern, tempTypeAdd)
						if Len(tempTypeAdd)> 0 Then
							Patern = "[" & cnstRExWORD & "]+" ' "
							tempTypeAdd = FindInStrEx(Patern, tempTypeAdd)
							tempTypeAdd = lcase(Trim(tempTypeAdd))
							If UniMetodsAndRekv.Exists(tempTypeAdd) Then
								RezTempTypeAdd = UniMetodsAndRekv.Item(tempTypeAdd)
								glRWord.TypeVid2 = RezTempTypeAdd
							End If
						End If
					End If
				End If


				'Если выражение типа "ОснОсн.Вид() = "ПриемВРемонт"" можно поискать справочник/документ/Регистр тагоко вида
				'Patern = "(\s|^)*(" & ResultWord & ")+(\s)*[.]+(\s)*(Вид|Kind)+(\s)*\((\s)*\)(\s)*(=|<>)+(\s)*[""][" & cnstRExWORD & "]+[""]"
				Patern = "(\s|^)*(" & ResultWord & ")+(\s)*[.]+(\s)*(Вид|Kind)+\s*\(+\s*\)+\s*(=|<>)+\s*[""][" & cnstRExWORD & "]+[""]"
				tempType = FindInStrEx(Patern, ttext)
				if (Len(tempType) = 0) And (Len(AddWords)>0) Then
					Patern = "(\s|^)*(" & ResultWord & "." & AddWords & ")+(\s)*[.]+(\s)*(Вид|Kind)+\s*\(+\s*\)+\s*(=|<>)+\s*[""][" & cnstRExWORD & "]+[""]"
					tempType = FindInStrEx(Patern, ttext)
				End IF
				if Len(tempType)> 0 Then
					Patern = "(=|<>)+\s*[""][" & cnstRExWORD & "]+[""]"
					tempType = FindInStrEx(Patern, tempType)
					if Len(tempType)> 0 Then
						Patern = "[""][" & cnstRExWORD & "]+[""]"
						tempType = FindInStrEx(Patern, tempType)
						if Len(tempType)> 0 Then
							tempType = Trim(tempType)
							tempType = Replace(tempType,"""","")
							ArrTempType = Split(tempType,vbCrLf)
							tempType = ArrTempType(0)
							ArrTipes = Array("Документ","Справочник","Регистр")
							For tttt = 0 To 2
								Set MDObjekts = Metadata.TaskDef.Childs(CStr(ArrTipes(tttt)))
								For cntdoc = 0 To MDObjekts.Count - 1
									Set MDObj = MDObjekts(cntdoc)
									if (UCase(MDObj.Name) = UCase(tempType)) Then
										Rezult = ArrTipes(tttt) & "." & tempType
										if Len(AddWords)>0 Then
											tRezultStr = GetTypeVid(AddWords,Rezult, Doc,0,"")
										End If
										if Len(tRezultStr)>0 Then
											GetTypeFromTextRegExps	= tRezultStr
										Else
											GetTypeFromTextRegExps	= Rezult
										End If
										AddWords = ""
										Exit Function
									End If
								Next
							Next
						End If
					End If
				End If

				'Если выражение типа "Если ТипЗначенияСтр(ЗначениеРасшифровки) = "СписокЗначений" Тогда
				Patern = "(\s)+(ТипЗначенияСтр|ValueTypeStr)+[\s]*[\(]+[\s]*(" & ResultWord & ")+[\s]*[\)]+[\s]*[=]+[\s]*[""]+(СписокЗначений|ТаблицаЗначений|Справочник|Документ|Регистр)+[""]+[\s]+"
				tempType = FindInStrEx(Patern, ttext)
				if Len(tempType)> 0 Then
					tRezultStr = ""
					If InStr(1,lcase(tempType),lcase("СписокЗначений"))>0 Then
						tRezultStr	= cnstNameTypeSZ
					ElseIf InStr(1,lcase(tempType),lcase("ТаблицаЗначений"))>0 Then
						tRezultStr	= cnstNameTypeTZ
					ElseIf InStr(1,lcase(tempType),lcase("Справочник"))>0 Then
						tRezultStr	= "Справочник"
					ElseIf InStr(1,lcase(tempType),lcase("Документ"))>0 Then
						tRezultStr	= "Документ"
					ElseIf InStr(1,lcase(tempType),lcase("Регистр"))>0 Then
						tRezultStr	= "Регистр"
					End IF
					if Len(tRezultStr)>0 Then
						glRWord.TypeVid = tRezultStr
						'2005 02 07 Пока ремим, попытаемся найти атрибуты если что....
						'Exit Function
					End If
				End If

				'Если выражение типа "ВидДок = Конт.Вид(); можно поискать справочник/документ/Регистр тагоко вида
				' в AddText у нас сидят сравнения с видами в формате "[переменная=ВидЧегото]РазделительСтрок"
				' переменная - полученная очевидно из фразы типа "ВидДок = Конт.Вид();
				'Patern = "(\s|^)*(" & ResultWord & ")+(\s)*[.]+(\s)*(Вид|Kind)+(\s)*\((\s)*\)(\s)*[=](\s)*[""][" & cnstRExWORD & "]+[""]"
				If Len(AddText)>0 Then
					Patern = "[\s]+[" & cnstRExWORD & "]+[\s]*[=]+[\s]*(" & ResultWord & ")+[\s]*[.]+[\s]*(Вид|Kind)+[\s]*[\(]+[\s]*[\)]+"
					tempType = FindInStrEx(Patern, ttext)
					if Len(tempType)> 0 Then
						Patern = "[\s]+[" & cnstRExWORD & "]+[\s]*[=]+"
						tempType = FindInStrEx(Patern, tempType)
						if Len(tempType)> 0 Then
							Patern = "[" & cnstRExWORD & "]+"
							tempType = FindInStrEx(Patern, tempType)
							if Len(tempType)> 0 Then
								' Тут у нас переменная содержащая Вид объекта
								tempType = Trim(tempType)
								ArrAddText = Split(AddText,vbCrLf)
								If Ubound(ArrAddText)<>-1 Then
									For qqq = 0 To Ubound(ArrAddText)
										Vids = Split(ArrAddText(qqq),"=")
										If UBound(Vids)>0 Then
											if LCase(Vids(0)) = LCase(tempType) Then
												Vid = Vids(1)
												ArrTipes = Array("Документ","Справочник","Регистр")
												For tttt = 0 To 2
													Set MDObjekts = Metadata.TaskDef.Childs(CStr(ArrTipes(tttt)))
													For cntdoc = 0 To MDObjekts.Count - 1
														Set MDObj = MDObjekts(cntdoc)
														if (UCase(MDObj.Name) = UCase(Vid)) Then
															Rezult = ArrTipes(tttt) & "." & Vid
															If Len(AddWords)>0 Then
																Rezult	= GetTypeVid(AddWords,Rezult, Doc,0,"")
															End If
															If Len(Rezult)>0 Then
																AddWords = ""
																GetTypeFromTextRegExps	= Rezult
																Exit Function
															End If
														End If
													Next
												Next
												exit For
											End If
										End If
									Next
								End If
							End If
						End If
					End If
				End If


				Patern = "(\s|^)(Процедура|Функция|Procedure|Function)(\s)+[a-zA-zа-яА-Я0-9_]+\([a-zA-zа-яА-Я0-9_,=""\s]*[\)]*"
				ProcFunc = FindInStrEx(Patern, ttext)
				If Len(ProcFunc)>0 Then
				'Если не нашли пока ничего, попробуем проанализировать не находится
				'ли переменная в такой конструкции "Процедура ВводНаОсновании(ДокОснование)"
				'если нашли, тогда смотрим списки оснований объекта ()
					Patern = "(\s|^)(Процедура|Функция|Procedure|Function)(\s)+(ВводНаОсновании)+(\s)*\((\s)*("&ResultWord&")(\s)*[\)]+"
					ProcFunc = FindInStrEx(Patern, ttext)
					If Len(ProcFunc)>0 Then
						ThisDoc = Split(Doc.name,".") 					'Здесь выбираем список оснований (если не одно)
						if UBound(ThisDoc)>0 Then
							Rezult = Trim(GetListOsnovaniyDoca(ThisDoc(1)))
						End If
						if InStr(1,Rezult," ") = 0 Then			'Убираем знак # так как пробелов нет, значит документ один.
							Rezult = Mid(Rezult,2)
						End If
					End If
					if nctStep = 0 Then
						Exit For
					End If
				End If

				Patern	 = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*(СоздатьОбъект|CreateObject)+[\s]*[\(]+["& cnstRExWORD &"\.""]+[\)]+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					RezultStr = FindInStrEx("\((.*?)\)", RezultStr)
					if Len(RezultStr)>0 Then
						If (InStr(1,RezultStr,"""")>0) And (InStr(1,RezultStr,"+") = 0) Then
							Rezult = FindInStrEx("\""(.*?)\""", RezultStr)
							Rezult = Replace(Rezult,"""","")
							glRWord.AsCreateObj = true
							if Len(AddWords)>0 Then
								RezultStr = GetTypeVid(AddWords,Rezult, Doc,0,"")
								if Len(RezultStr)<>0 Then
									AddWords = ""
									Rezult = RezultStr
								End if
							End if
							'If lcase(Rezult) = "регистры" Then :	Rezult = "Регистр"	: End if

							Exit For
						End if
					End if
				End if

				Patern	 = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*(ТекущийДокумент|CurrentDocument|ТекущийЭлемент|CurrentItem)+[\s]*[\(]+[\s]*[\)]+[\s]*[;]+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					ArrRezultStr = split(doc.Name,".")
					if UBound(ArrRezultStr)>0 Then
						Rezult = ArrRezultStr(0) & "." & ArrRezultStr(1)
					End if
				End if

				If (InStr(1, Doc.Name,	"Журнал.") = 1) 	Then
					Patern	 = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*(ТекущийДокумент|CurrentDocument)+[\s]*[\(]*[\s]*[\)]*[\s]*[;]+"
					RezultStr = FindInStrEx(Patern, ttext)
					if (Len(RezultStr)> 0) And (InStr(1,RezultStr,"(") =0 ) Then
						Rezult = "Документ"
					End if
				End IF

				RezultStr2 = FindInStrEx(Patern, ttext)

				'РегЗапчасти.ВыгрузитьИтоги(ТаблИтогов);
				Patern	 =	"[\s|^|;]+[" & cnstRExWORD & "]+[\s]*[\.]+[\s]*(ВыгрузитьИтоги|RetrieveTotals)+[\s]*[\(]+[\s]*(" & ResultWord & ")+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					Patern	 = "[\s]*[\.]+[\s]*(ВыгрузитьИтоги|RetrieveTotals)+[\s]*[\(]+[\s]*(" & ResultWord & ")+"
					AddRezultStr = FindInStrEx(Patern, RezultStr)
					if Len(AddRezultStr)>0 Then
						RezultStr = Replace(RezultStr,AddRezultStr,"")
						Patern = "[" & cnstRExWORD & "]+"
						RezultStr = FindInStrEx(Patern, RezultStr)
						tRezultStr = AddWords
						RezultStr = GetTypeFromTextRegExps(RezultStr, AddWords)
						if Len(RezultStr)>0 Then
						IF Len(tRezultStr) = 0 Then
							Rezult = RezultStr & "+таблицазначений"
						Else
							Rezult = RezultStr
						End if
							glRWord.BTMeth = "ВыгрузитьИтоги"
							glRWord.AddWord = ""
							if Len(AddWords)>0 Then
								AddWords = ""
							End if
							Exit For
						End if

					End if
				End if

				Patern	 = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*[" & cnstRExWORD & "]+[\s]*[;]+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					Patern	 = "[=]+[\s]*[" & cnstRExWORD & "]+[\s]*[;]+"
					RezultStr = FindInStrEx(Patern, ttext)
					if Len(RezultStr)> 0 Then
						RezultStr = ReplaceEx(RezultStr,Array( "=","", ";",""))
						RezultStr = Trim(RezultStr)
						tRezultStr = GetTypeVid(RezultStr,"", Doc, 0,RezultStr)
						'tRezultStr = GetTypeFromTextRegExps(RezultStr, AddWords)
						if Len(tRezultStr)>0 Then
							Rezult = tRezultStr
							Exit For
						End if
					End if
				End if

				Patern	 = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*[" & cnstRExWORD & "\s\.\(\)]+[\s]*[;]+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					Patern	 = "[=]+[\s]*[" & cnstRExWORD & "\s\.\(\)]+[\s]*[;]+"
					RezultStr = FindInStrEx(Patern, ttext)
					if Len(RezultStr)> 0 Then
						RezultStr = ReplaceEx(RezultStr, Array("=","", ";","", vbTab," ", " ",""))
						RezultStr = Trim(RezultStr) '"ПриходнаяН.ТекущийДокумент()"
						' Подработка фраз типа "Род = Спр.Владелец.ТекущийЭлемент();" в Род = Спр.Владелец;; для "спр"..
						RezultStr = Replace(RezultStr,FindInStrEx("[\.]+\s*(ТекущийЭлемент|ТекущийДокумент)+\s*\(+\s*\)+", ttext),"")
						ArrRezultStr = Split(RezultStr,".")
						IF UBound(ArrRezultStr) = 1 Then
							tRezultStr = GetTypeVid(ArrRezultStr(1),ArrRezultStr(0), Doc, 0,RezultStr)
							if Len(tRezultStr)>0 Then
								Rezult = tRezultStr
								Exit For
							End IF
						ElseIf UBound(ArrRezultStr) = 0 Then
							tRezultStr = GetTypeVid(ArrRezultStr(0),"", Doc, 0,RezultStr)
							if Len(tRezultStr)>0 Then
								Rezult = tRezultStr
								Exit For
							End IF
						End IF
						IF UBound(ArrRezultStr)>0 Then
							tRezultStr = GetTypeVid(ArrRezultStr(0),"", Doc, 0,RezultStr)
							IF Len(tRezultStr) = 0 Then
								Patern = ""
								for qwqe = 1 To UBound(ArrRezultStr)
									IF Patern = "" Then
										Patern = ArrRezultStr(qwqe)
									Else
										Patern = Patern & "." & ArrRezultStr(qwqe)
									End if
								Next
								tRezultStr = GetTypeVid(AddWords,RezultStr, Doc,0,"")
								if Len(tRezultStr) = 0 Then
									tRezultStr = GetTypeFromTextRegExps(ArrRezultStr(0), Patern)
									tRezultStr0 = tRezultStr
									If (lcase(tRezultStr) = "регистры") And (Len(Patern)>0) Then
										tRezultStr = "регистр" &"."&Patern
									elseif Len(Patern)>0 Then
										if Instr(1,Patern,".")>0 Then
											ArrWOfVariable = Split(Patern,".")
											for q = 0 To UBound(ArrWOfVariable)
												tRezultStr = GetTypeVid(ArrWOfVariable(q),tRezultStr, Doc,0,"")
											Next
										else
											tRezultStr = GetTypeVid(Patern,tRezultStr, Doc,0,"")
										End if
									End if
									Patern = ""
									if Len(AddWords)>0 Then
										if Instr(1,AddWords,".")>0 Then
											ArrWOfVariable = Split(AddWords,".")
											for q = 0 To UBound(ArrWOfVariable)
												tRezultStr = GetTypeVid(ArrWOfVariable(q),tRezultStr, Doc,0,"")
											Next
										else
											tRezultStr = GetTypeVid(AddWords,tRezultStr, Doc,0,"")
										End if
									End if
								End if
								AddWords = ""
								if Len(tRezultStr)>0 Then
									Rezult = tRezultStr
									Exit For
								End if

								Patern = ""
							ElseIf (UC(tRezultStr) = UC("ТаблицаЗначений")) And ( UC(ArrRezultStr(1)) = UC("ТекущаяКолонка()")) Then
								tRezultStr = ArrRezultStr(0) & "#КолонкаТаблицыЗначений#"
							End if

						End if
						if Len(tRezultStr)>0 Then
							if InStr(1,tRezultStr,".")>0 Then
								tRezultStr = GetTypeVid(ArrRezultStr(1),tRezultStr, Doc, 0,"")
								if Len(tRezultStr)<>0 Then
									Rezult = tRezultStr
								End if
							Else
								Rezult = tRezultStr
							End if
							if Len(Rezult)>0 Then
								Exit For
							End if
						End if
					End if
				End if

				Patern = "[\s|^]+(" & ResultWord & ")+[\s]*[.]+[\s]*(ИсходнаяТаблица|SourceTable)+[\s]*[\(]+[\s]*[" & cnstRExWORD & """]+[\)]+[\s]*[;]+[\s]*"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					Patern = "[\(]+[\s]*[" & cnstRExWORD & """]+[\)]+"
					RezultStr = FindInStrEx(Patern, RezultStr)
					if Len(RezultStr)>0 Then
						Patern = "[" & cnstRExWORD & """]+"
						RezultStr = FindInStrEx(Patern, RezultStr)
						if Len(RezultStr)>0 Then
							RezultStr = Replace(RezultStr,"""","")
							AddWords = RezultStr
						End if

					End if
				End if

				'Patern = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*[" & cnstRExWORD & ".]+[\s$;]+"
				'RezultStr = FindInStrEx(Patern, ttext)


				Patern = "[\s|^|;]+(" & ResultWord & ")+[\s]*[=]+[\s]*[" & cnstRExWORD & ".]+[\s$;]+"
				RezultStr = FindInStrEx(Patern, ttext)
				if Len(RezultStr)> 0 Then
					RezultStr = FindInStrEx("[\s]*[=]+[\s]*[" & cnstRExWORD & ".]+", RezultStr)
					if Len(RezultStr)>0 Then
						RezultStr = FindInStrEx("[" & cnstRExWORD & ".]+", RezultStr)
						if Len(RezultStr)>0 Then
							IF InStr(1,RezultStr,".") > 0 Then
								tmpArrVidType = Split(Trim(RezultStr),".")
								if UBound(tmpArrVidType)>0 Then
									tmpType	 = UCase(Trim(tmpArrVidType(0)))
									If Not Is1CObject(tmpType) Then
										AddWords = ""
										if UC(tmpType) <> UC(RezultStr) Then
											tRezultStr = GetTypeFromTextRegExps(tmpType,AddWords)
											If Is1CObject(tRezultStr) Then
												Rezult = Trim(tRezultStr)
												For uu = 1 To UBound(tmpArrVidType)
													Rezult = Rezult & "." & tmpArrVidType(uu)
												Next
											Else
												Rezult = RezultStr
											End if
										End if
									else
										Rezult = RezultStr
									End if
								End if
							End if
						End if
					End if
				End if
			End if
			FindRegim = 1  ' ВидДок = "Диагностика"
			patrn = "(\s|\()+[" & cnstRExWORD & "]+[\s]*[=]+[\s]*[""]+[" & cnstRExWORD & "]+[""]+"
			ttext0 = FindInStrEx (patrn, ttext)
			if Len(ttext0) = 0 Then
				patrn = "(\s|\()+[""]+[" & cnstRExWORD & "]+[""]+[\s]*[=]+[\s]*[" & cnstRExWORD & "]+"
				ttext0 = FindInStrEx (patrn, ttext)
				FindRegim = 2 '"Диагностика" = ВидДок
			End if
			if Len(ttext0)>0 Then
				ttext0 = ReplaceEx(ttext0,Array(" ","", vbTab,"", "(","",  """",""))
				Arrttext0 = Split(ttext0,"=")
				If UBound(Arrttext0)=1 Then
					IF FindRegim = 2 Then
						ttext0 = Arrttext0(1)&"="&Arrttext0(0)
					End if
					If Len(AddText) = 0 Then
						AddText = ttext0
					Else
						AddText = AddText & vbCrLf & ttext0
					End if
				End if
			End if
		Next
		if Len(Rezult)>0 Then
			Exit For
		End if
	Next

	if Len(Rezult)>0 Then
		if Mid(Rezult,Len(Rezult),1) = "." Then
			Rezult = Mid(Rezult,1,Len(Rezult)-1)
		End If
	Else
		if Len(RezTempTypeAdd)>0 Then
			Rezult = RezTempTypeAdd
		End If
	End If
	Rezult = Replace(Rezult,"""","")
	IF InStr(1,Rezult,".")> 0 Then
	End If
	GetTypeFromTextRegExps	= Rezult
End Function

Private Function Is1CObject(NameObj)
	Is1CObject	 = False
	If UCase(NameObj) = UCase("Регистр") Then
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("РегистрЫ") Then
		NameObj =	"Регистр"
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("Справочник") Then
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("Форма") Then
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("Документ") Then
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("Перечисление") Then
		Is1CObject	 = True
	ElseIf UCase(NameObj) = UCase("Константа") Then
		Is1CObject	 = True
	End If
End Function


Private Function AnalizProcedure(tempType, RezultWord)
	AnalizProcedure = ""
	Patern = "[^\s][^.][" & cnstRExWORD & "]+[\(]+" ' "
	NamePF = FindInStrEx(Patern, tempType)
	if Len(NamePF)>0 Then
		Patern = "[\(]+[^@]+[\)]+" ' "
		ParametrsPF = FindInStrEx(Patern, tempType)
		ParametrsPF = Replace(ParametrsPF," ","")
		ParametrsPFArr = split(ParametrsPF,",")
		NamePF = Trim(Replace(NamePF,"(",""))
		IF UCase(NamePF) = UCase("ОткрытьПодбор") Then
			If UBound(ParametrsPFArr)> 2 Then
				If UCase(ParametrsPFArr(2)) = UCase(RezultWord) Then
					if InStr(1,UCase(ParametrsPFArr(0)),UCase("Справочник")) >0 Then
						AnalizProcedure	= "ФормаПодбораСправочник"
						Exit Function
					End If
				End If
			End If
		End If
	End If
End Function

'ttextModuleGM = ""
'ttextModuleGM_PriNachRabSys = ""
'Медленно работает, проблемы
Private Function GetColumnsFromTZ(ResultWord, STipom)
	GetColumnsFromTZ = ""
' 	If Windows.ActiveWnd Is Nothing Then
' 		Exit Function
' 	End If
	OldGlobalStr = GlobalStr
	GetColumnsFromTZ = GetColumnsFromTZA(ResultWord, STipom, ttextModuleGM)
	If Len(GetColumnsFromTZ)<>0 Then
		Exit Function
	End If
	GetColumnsFromTZ = GetColumnsFromTZA(ResultWord, STipom, ttextModuleGM_PriNachRabSys)
	If Len(GetColumnsFromTZ)<>0 Then
		Exit Function
	End If
	TextAll = ""
	'ArrProcFuncFromTheWord	= ""
	'ArrProcFuncNumbParam	= ""
	'Dim GlobalModule 'Объект TheModule для Глобального модуля
	'Dim LocalModule	 'Объект TheModule для модуля текущего объекта



	'Позиционируемся на глобальные и локальные функции в надежде получить колонки таблицы значений..
	if Len(glRWord.ArrProcFuncFromTheWord)>0 Then
		ArrProcFuncFromTheWord = Split(glRWord.ArrProcFuncFromTheWord,",")
		ArrProcFuncNumbParam = Split(glRWord.ArrProcFuncNumbParam,",")
		For ee=0 To UBound(ArrProcFuncFromTheWord)
			If GlobalModule.IsProcedure(ArrProcFuncFromTheWord(ee),"","","",TextAll, TheProc) Then
				On Error Resume Next
				'if (ArrProcFuncNumbParam(ee) = "-1")
				If (TheProc.TypeItem = 2) And (ArrProcFuncNumbParam(ee) = "-1") Then
					GetColumnsFromTZ = GetColumnsFromTZA(TheProc.RetValueStr, STipom, TheProc.Text)
				Else
					ParamNumber = TheProc.GetParamNumber(ArrProcFuncNumbParam(ee))
					GetColumnsFromTZ = GetColumnsFromTZA(ParamNumber, STipom, TheProc.Text)
				End If
				If Len(GetColumnsFromTZ)<>0 Then
					Exit Function
				End If
			End If
		Next
	End If
	GlobalStr = OldGlobalStr
	doc = ""
	if Not CheckWindow(doc) then Exit Function

	'Выгружается табличная часть по месту в форме дока
	virajenieCO1 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_.]*[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)[\s]*[(][\s]*" & ResultWord &"[\s]*[)](\s|^|;)"
	'Идея такая сигнальная строка типа //ТЗПеременная=Документ.ТакойТО или //ТЗПеременная=Регистр.ТакойТО
	virajenieCO3 = 	"(//)+[\s]*(" & ResultWord &")[\s]*[=]+[\s]*(Документ|Регистр)+[\s]*[.]+[\s]*[a-zA-Zа-яА-Я0-9_.]+"
	TextAll = ""
	'План такой, если мы в глобальном модуле, тогда выбираем только текст данной процедуры
	'иначе, сначала выбираем только текст процедуры, потом, если не помогает, тогда
	'весь текст с начала и до конца
	'ПОТОМ Посылаем переменную в процедуру анализа и ждем реакции
	Rezult = ""
	' Попробуем в 2 прохода, если в местной локализации не найдем, поищем во всем модуле
	EndPosotion = 1
	If Windows.ActiveWnd.Document.Name = "Глобальный модуль" Then
		EndPosotion = 0
	End If
	For yyy = 0 To EndPosotion
		FStartPosition = 0
		IF yyy = 0 Then
			FStartPosition = GlobalStr
		Else
			FStartPosition = Doc.LineCount
		End IF
		For i = FStartPosition To 0 Step -1
			ttext = Trim(Doc.range(i))
			GlobalStr = i
			TextAll = ttext & TextAll
			if inStr(1,ttext,"//")>0 Then
				ttext = Mid(ttext,1,inStr(1,ttext,"//"))
			End If
			IF yyy = 0 Then
				If Len(FindInStrEx("(\s|^)+(Процедура|Функция|Procedure|Function)", ttext))>0 Then
					Exit For
				End If
			End If
			If InStr(1,UC(ttext),UC(ResultWord)) > 0 Then

				'Выгружается запрос в таблицу значений
				virajenieCO0 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*(Выгрузить|Unload)+(\s)*[\(]+(\s)*(" & ResultWord &")+.*[\)]+(\s|^|;)"
				RezultStr = FindInStrEx(virajenieCO0, ttext)
				if Len(RezultStr)> 0 Then
					'Откопаем переменную запроса
					textVarZapros = FindInStrEx("(\s)*[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*",RezultStr)
					'Здесь косяк - данная переменна может быть и не запросом :(
					' Исправить............
					If Len(textVarZapros)>0 Then
						'почистим
						textVarZapros = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+",textVarZapros)
					End IF
					Patern = "[\(]+(\s)*(" & ResultWord &")+.*[\)]+"
					ttt = FindInStrEx(Patern,RezultStr)
					ttt = Replace(Replace(Replace(ttt,"(",""),")","")," ","")
					if InStr(1,ttt,",")>0 Then
						eeee = Split(ttt,",")
						If Ubound(eeee)<> -1 Then
							ttt = eeee(0)
						End if
					End if
					If UC(ttt) = UC(ResultWord) Then
						TypeVid = GetTypeFromTextRegExps(textVarZapros,"")
						'Stop
						if InStr(1,TypeVid,"+таблицазначений")>0 Then
							TypeVid = Replace(TypeVid,"+таблицазначений","")
						End if
						RezultStr = ""
						if (UC(TypeVid) = "ЗАПРОС") Then
							RezultStr = GetVariableAndFunctionZapros(textVarZapros, STipom)
						Elseif (UC(TypeVid) = UC(cnstNameTypeTZ)) Then
							RezultStr = GetColumnsFromTZ(textVarZapros, STipom)
						ElseIf Instr(1,Lcase(TypeVid),LCase("Регистр."))>0 Then
							eeee = Split(TypeVid,".")
							RezultStr = GetStringRekvizitovFromObj(eeee(0), eeee(1),0,0, "",	0, glRWord)
							'GetStringRekvizitovFromObj(TypeObj, NameObj,SMetodami,AsRecvOfForms, BTMeth,	BTNumberParams, RWord)
						End if
						if Len(RezultStr)>0 Then
							Rezult = Rezult & vbCrLf &  RezultStr
						End if
						Exit For
					End if
				End if

				'Выгружается итоги по регистру в таблицу значений
				virajenieCO0 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*(ВыгрузитьИтоги|RetrieveTotals)+(\s)*[\(]+(\s)*(" & ResultWord &")+(\s)*(,)*[" & cnstRExWORD & "\(\)]*(,)*[0-3]*(,)*(\s)*[\)]+(\s|^|;)"
				RezultStr = FindInStrEx(virajenieCO0, ttext)
				if Len(RezultStr)> 0 Then
					'Откопаем переменную регистр
					textVarRegistr = FindInStrEx("(\s)*[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*",RezultStr)
					If Len(textVarRegistr)>0 Then
						'почистим
						textVarRegistr = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+",textVarRegistr)
					End IF
					If Len(textVarRegistr)>0 Then
						AddWords = ""
						ttipvid = GetTypeFromTextRegExps(textVarRegistr,AddWords)
						if Len(ttipvid)>0 Then
							Arrttipvid = Split(ttipvid,".")
							if UBound(Arrttipvid) = 1 Then
								If STipom = 1 Then
									RezultStr = GetStringRekvizitovFromObj(Arrttipvid(0), Arrttipvid(1),2,0,"",0)
								Else
									RezultStr = GetStringRekvizitovFromObj(Arrttipvid(0), Arrttipvid(1),0,0,"",0)
								End IF
							End IF
						End IF
					End IF
					if Len(RezultStr)>0 Then
						Rezult = Rezult & vbCrLf &  RezultStr
					End if
					Exit for
				End if

				'Ищем новую добавление колонок в ТЗ
				virajenieCO2 = 	"(\s|^|;)+(" & ResultWord &")[\s]*[.]+[\s]*(ВставитьКолонку|InsertColumn|НоваяКолонка|NewColumn)\((.*?)\)"
				RezultStr = FindInStrEx(virajenieCO2, ttext)
				if Len(RezultStr)> 0 Then
					RezultStr = FindInStrEx("\((.*?)\)", RezultStr)
					if Len(RezultStr)>0 Then
						If (InStr(1,RezultStr,"""")>0) And (InStr(1,RezultStr,"+") = 0) And (InStr(1,RezultStr,")") <> 0) And (InStr(1,RezultStr,"(") <> 0) Then
							RezultStr = ReplaceEx(RezultStr,Array("(","", ")","", """",""))
							tttt = split(RezultStr,",")
							if UBound(tttt) > 0 Then
								'В индексе (0) сидит имя, в (1) сидит тип
								IF STipom = 1 Then
									RezultStr = tttt(0) & "=" & tttt(1)
								Else
									RezultStr = tttt(0) '& "#" & tttt(1)
								End if
							elseif UBound(tttt) = 0 Then
								RezultStr = tttt(0)
							else
								RezultStr = ""
							End if
							if (Len(Rezult)>0) And (Len(RezultStr)>0) Then
								Rezult = Rezult & vbCrLf & RezultStr
							ElseIF (Len(RezultStr)>0) Then
								Rezult = RezultStr
							End if
						End if
					End if
				End if
				'Выгружается табличная часть по месту в форме дока
				'virajenieCO1 = 	"(\s|^|;)*[a-zA-Zа-яА-Я0-9_.]*[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)[\s]*[(][\s]*" & ResultWord &"[\s]*[)](\s|^|;)"
				RezultStr = FindInStrEx(virajenieCO1, ttext)
				if Len(RezultStr)> 0 Then
					'Берем просто у документа описание табличной части
					VidDoca = Array("Документ")
					RezultStr = FindInStrEx("(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+[\s]*[.]+[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)", RezultStr)
					if Len(RezultStr)>0 Then
						RezultStr = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+[\s]*[.]+", RezultStr)
						RezultStr = Replace(RezultStr,".","")
						RezultStr = Trim(RezultStr)
						AddWords = ""
						if Len(RezultStr)>0 Then
							OldStartScanLineText = StartScanLineText
							StartScanLineText = i
							tttVidDoca = GetTypeFromTextRegExps(RezultStr,AddWords)
							StartScanLineText = OldStartScanLineText
							if InStr(1,tttVidDoca,".") > 0 Then
								VidDoca = split(tttVidDoca, ".")
							End if
						End if
					Else
						VidDoca = split(Doc.Name, ".")
					End if
					if UBound(VidDoca)>0 Then
						IF ObjectExist("Документ",VidDoca(1)) Then
							Set MetaDoc = MetaData.TaskDef.Childs(CStr("Документ"))(CStr(VidDoca(1)))
							Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
							for tbl_cnt = 0 To Table.Count - 1
								set tbl_chld = Table(tbl_cnt)
								if Len(Rezult)>0 Then
									Rezult = Rezult &  vbCrLf & tbl_chld.Name
								else
									Rezult = tbl_chld.Name
								End if
								if STipom = 1 Then
									Rezult = Rezult & "=" & tbl_chld.Type.FullName
								End if
							Next
							Exit for
						End if
					End if
				End if
			End if
		Next
		RezultStr = FindInStrEx(virajenieCO3, TextAll)
		if Len(Rezult) > 0 Then
			Exit For 'Зачем нам проходить лишний проход?
		End If
	Next 'Проходы

	RezultStr = FindInStrEx(virajenieCO3, TextAll)
	if Len(RezultStr) > 0 Then
		IF InStr(1,RezultStr,vbCrLf) Then
			nnnn = Split(RezultStr,vbCrLf)
			If Len(nnnn(0))>0 Then
				RezultStr = nnnn(0)
			End IF
		End IF
		patern = 	"[\s|^]+(Регистр|Документ)+[\s]*[.]+[\s]*[a-zA-Zа-яА-Я0-9_.]+"
		RezultStr = FindInStrEx(patern, RezultStr)
		if Len(RezultStr)>0 Then
			VidDoca = split(UCase(RezultStr), ".")
			if UBound(VidDoca)>0 Then
				if (VidDoca(0) = UCase("Документ")) Then
					Set MetaDoc = MetaData.TaskDef.Childs(CStr("Документ"))(CStr(VidDoca(1)))
					Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
		 			for tbl_cnt = 0 To Table.Count - 1
		 				set tbl_chld = Table(tbl_cnt)
		 				if Len(Rezult)>0 Then
		 					Rezult = Rezult &  vbCrLf & tbl_chld.Name
		 				else
		 					Rezult = tbl_chld.Name
		 				End if
		 			Next
			 	ElseIF (VidDoca(0) = UCase("Регистр")) Then
			 		Rezult = GetStringRekvizitovFromObj("Регистр", CStr(VidDoca(1)),0,0,"",0)
				End if
			End if
		End If
	End if


	'Подработаепм на дублирование
	if STipom = 0 Then
		if 1=2 Then
			Patern = "(" & ResultWord & ")+[\s]*[.]+[\s]*(Свернуть|GroupBy)+[\s]*[\(]+[\s]*[" & cnstRExWORD & "\""\s,]+[\s]*[\)]+"
			ttt = FindInStrEx(Patern,TextAll)
			IF Len(ttt)>0 Then
				Patern = "[\(]+[\s]*[" & cnstRExWORD & "\""\s,]+[\s]*[\)]+"
				ttt = FindInStrEx(Patern,ttt)
				IF Len(ttt)>0 Then
					Rezult = ReplaceEx(ttt,Array("(","", ")","", """","", vbCrLf,",", ",,",","))
				End if
			End if
		End if
		ArrRezult = Split(Rezult,vbCrLf)
		tmpRezult = ""
		if UBound(ArrRezult)<> -1 Then
			For ee = 0 To UBound(ArrRezult)
				patern = "(\s|^|,)+(" & ArrRezult(ee) & ")+(\s|^|,|$)+"
				if Len(FindInStrEx(patern,tmpRezult)) = 0 Then
					 If Len(tmpRezult)>0 Then
						tmpRezult = tmpRezult & "," & ArrRezult(ee)
						else
						tmpRezult = ArrRezult(ee)
					End IF
				End IF
			Next
		End IF
		Rezult = Replace(tmpRezult,",",vbCrLf)
	End IF

	GetColumnsFromTZ	= Rezult
End Function

'ttextModuleGM = ""
'ttextModuleGM_PriNachRabSys = ""
'Медленно работает, проблемы
Private Function GetColumnsFromTZA(ResultWord, STipom, textModule)
	GetColumnsFromTZA = ""
' 	If Windows.ActiveWnd Is Nothing Then
' 		Exit Function
' 	End If
	ArrtextModule = split(textModule, vbCrLf)
	'Выгружается табличная часть по месту в форме дока
	virajenieCO1 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_.]*[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)[\s]*[(][\s]*" & ResultWord &"[\s]*[)](\s|^|;)"
	'Идея такая сигнальная строка типа //ТЗПеременная=Документ.ТакойТО или //ТЗПеременная=Регистр.ТакойТО
	virajenieCO3 = 	"(//)+[\s]*(" & ResultWord &")[\s]*[=]+[\s]*(Документ|Регистр)+[\s]*[.]+[\s]*[a-zA-Zа-яА-Я0-9_.]+"
	TextAll = ""
	'План такой, если мы в глобальном модуле, тогда выбираем только текст данной процедуры
	'иначе, сначала выбираем только текст процедуры, потом, если не помогает, тогда
	'весь текст с начала и до конца
	'ПОТОМ Посылаем переменную в процедуру анализа и ждем реакции
	Rezult = ""
	' Попробуем в 2 прохода, если в местной локализации не найдем, поищем во всем модуле
	EndPosotion = 1
	If Windows.ActiveWnd.Document.Name = "Глобальный модуль" Then
		EndPosotion = 0
	End If
	For i = UBound(ArrtextModule) To 0 Step -1
		ttext = Trim(ArrtextModule(i))
		GlobalStr = i
		TextAll = ttext & TextAll
		if inStr(1,ttext,"//")>0 Then
			ttext = Mid(ttext,1,inStr(1,ttext,"//"))
		End If
		If InStr(1,UC(ttext),UC(ResultWord)) > 0 Then

			'Выгружается запрос в таблицу значений
			virajenieCO0 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*(Выгрузить|Unload)+(\s)*[\(]+(\s)*(" & ResultWord &")+(\s)*(,)*[" & cnstRExWORD & "\(\)]*(,)*[0-3]*(,)*(\s)*[\)]+(\s|^|;)"
			RezultStr = FindInStrEx(virajenieCO0, ttext)
			if Len(RezultStr)> 0 Then
				'Откопаем переменную запроса
				textVarZapros = FindInStrEx("(\s)*[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*",RezultStr)
				If Len(textVarZapros)>0 Then
					'почистим
					textVarZapros = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+",textVarZapros)
				End IF
				Patern = "[\(]+(\s)*(" & ResultWord &")+(\s)*(,)*[" & cnstRExWORD & "\(\)]*(,)*[0-3]*(,)*(\s)*[\)]+"
				ttt = FindInStrEx(Patern,RezultStr)
				ttt = ReplaceEx(ttt,Array("(","",")",""," ",""))
				if InStr(1,ttt,",")>0 Then
					eeee = Split(ttt,",")
					If Ubound(eeee)<> -1 Then
						ttt = eeee(0)
					End if
				End if
				If UC(ttt) = UC(ResultWord) Then
					TypeVid = GetTypeFromTextRegExps(textVarZapros,"")
					if (UC(TypeVid) = "ЗАПРОС") Then
						RezultStr = GetVariableAndFunctionZapros(textVarZapros, STipom)
					Elseif (UC(TypeVid) = UC(cnstNameTypeTZ)) Then
						RezultStr = GetColumnsFromTZA(textVarZapros, STipom,textModule)
					End if
					if Len(RezultStr)>0 Then
						Rezult = Rezult & vbCrLf &  RezultStr
					End if
					Exit For
				End if
			End if

			'Выгружается итоги по регистру в таблицу значений
			virajenieCO0 = 	"(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*(ВыгрузитьИтоги|RetrieveTotals)+(\s)*[\(]+(\s)*(" & ResultWord &")+(\s)*(,)*[" & cnstRExWORD & "\(\)]*(,)*[0-3]*(,)*(\s)*[\)]+(\s|^|;)"
			RezultStr = FindInStrEx(virajenieCO0, ttext)
			if Len(RezultStr)> 0 Then
				'Откопаем переменную регистр
				textVarRegistr = FindInStrEx("(\s)*[a-zA-Zа-яА-Я0-9_]+(\s)*[.]+(\s)*",RezultStr)
				If Len(textVarRegistr)>0 Then
					'почистим
					textVarRegistr = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+",textVarRegistr)
				End IF
				If Len(textVarRegistr)>0 Then
					AddWords = ""
					ttipvid = GetTypeFromTextRegExps(textVarRegistr,AddWords)
					if Len(ttipvid)>0 Then
						Arrttipvid = Split(ttipvid,".")
						if UBound(Arrttipvid) = 1 Then
							RezultStr = GetStringRekvizitovFromObj(Arrttipvid(0), Arrttipvid(1),0,0,"",0)
						End IF
					End IF
				End IF
				if Len(RezultStr)>0 Then
					Rezult = Rezult & vbCrLf &  RezultStr
				End if
				Exit for
			End if

			'Ищем новую добавление колонок в ТЗ
			virajenieCO2 = 	"(\s|^|;)+(" & ResultWord &")[\s]*[.]+[\s]*(ВставитьКолонку|InsertColumn|НоваяКолонка|NewColumn)\((.*?)\)"
			RezultStr = FindInStrEx(virajenieCO2, ttext)
			if Len(RezultStr)> 0 Then
				RezultStr = FindInStrEx("\((.*?)\)", RezultStr)
				if Len(RezultStr)>0 Then
					If (InStr(1,RezultStr,"""")>0) And (InStr(1,RezultStr,"+") = 0) And (InStr(1,RezultStr,")") <> 0) And (InStr(1,RezultStr,"(") <> 0) Then

						RezultStr = ReplaceEx(RezultStr,Array("(","", ")","", """",""))
						tttt = split(RezultStr,",")
						if UBound(tttt) > 0 Then
							'В индексе (0) сидит имя, в (1) сидит тип
							IF STipom = 1 Then
								RezultStr = tttt(0) & "=" & tttt(1)
							Else
								RezultStr = tttt(0) '& "#" & tttt(1)
							End if
						elseif UBound(tttt) = 0 Then
							RezultStr = tttt(0)
						else
							RezultStr = ""
						End if
						if (Len(Rezult)>0) And (Len(RezultStr)>0) Then
							Rezult = Rezult & vbCrLf & RezultStr
						ElseIF (Len(RezultStr)>0) Then
							Rezult = RezultStr
						End if
					End if
				End if
			End if
			'Выгружается табличная часть по месту в форме дока
			'virajenieCO1 = 	"(\s|^|;)*[a-zA-Zа-яА-Я0-9_.]*[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)[\s]*[(][\s]*" & ResultWord &"[\s]*[)](\s|^|;)"
			RezultStr = FindInStrEx(virajenieCO1, ttext)
			if Len(RezultStr)> 0 Then
				'Берем просто у документа описание табличной части
				VidDoca = Array("Документ")
				RezultStr = FindInStrEx("(\s|^|;)+[a-zA-Zа-яА-Я0-9_]+[\s]*[.]+[\s]*(ВыгрузитьТабличнуюЧасть|UnloadTable)", RezultStr)
				if Len(RezultStr)>0 Then
					RezultStr = FindInStrEx("[a-zA-Zа-яА-Я0-9_]+[\s]*[.]+", RezultStr)
					RezultStr = Replace(RezultStr,".","")
					RezultStr = Trim(RezultStr)
					AddWords = ""
					if Len(RezultStr)>0 Then
						tttVidDoca = GetTypeFromTextRegExps(RezultStr,AddWords)
						if InStr(1,tttVidDoca,".") > 0 Then
							VidDoca = split(tttVidDoca, ".")
						End if
					End if
				Else
					VidDoca = split(Doc.Name, ".")
				End if
				if UBound(VidDoca)>0 Then
					Set MetaDoc = MetaData.TaskDef.Childs(CStr("Документ"))(CStr(VidDoca(1)))
					Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
					for tbl_cnt = 0 To Table.Count - 1
						set tbl_chld = Table(tbl_cnt)
						if Len(Rezult)>0 Then
							Rezult = Rezult &  vbCrLf & tbl_chld.Name
						else
							Rezult = tbl_chld.Name
						End if
						if STipom = 1 Then
							Rezult = Rezult & " " & tbl_chld.Type.FullName
						End if
					Next
					Exit for
				End if
			End if
		End if
	Next
	RezultStr = FindInStrEx(virajenieCO3, TextAll)
	if Len(RezultStr) > 0 Then
		IF InStr(1,RezultStr,vbCrLf) Then
			nnnn = Split(RezultStr,vbCrLf)
			If Len(nnnn(0))>0 Then
				RezultStr = nnnn(0)
			End IF
		End IF
		patern = 	"[\s|^]+(Регистр|Документ)+[\s]*[.]+[\s]*[a-zA-Zа-яА-Я0-9_.]+"
		RezultStr = FindInStrEx(patern, RezultStr)
		if Len(RezultStr)>0 Then
			VidDoca = split(UCase(RezultStr), ".")
			if UBound(VidDoca)>0 Then
				if (VidDoca(0) = UCase("Документ")) Then
					Set MetaDoc = MetaData.TaskDef.Childs(CStr("Документ"))(CStr(VidDoca(1)))
					Set Table = MetaDoc.Childs("РеквизитТабличнойЧасти")
		 			for tbl_cnt = 0 To Table.Count - 1
		 				set tbl_chld = Table(tbl_cnt)
		 				if Len(Rezult)>0 Then
		 					Rezult = Rezult &  vbCrLf & tbl_chld.Name
		 				else
		 					Rezult = tbl_chld.Name
		 				End if
		 			Next
			 	ElseIF (VidDoca(0) = UCase("Регистр")) Then
			 		Rezult = GetStringRekvizitovFromObj("Регистр", CStr(VidDoca(1)),0,0,"",0)
				End if
			End if
		End If
	End if


	'Подработаепм на дублирование
	if STipom = 0 Then
		if 1=2 Then
			Patern = "(" & ResultWord & ")+[\s]*[.]+[\s]*(Свернуть|GroupBy)+[\s]*[\(]+[\s]*[" & cnstRExWORD & "\""\s,]+[\s]*[\)]+"
			ttt = FindInStrEx(Patern,TextAll)
			IF Len(ttt)>0 Then
				Patern = "[\(]+[\s]*[" & cnstRExWORD & "\""\s,]+[\s]*[\)]+"
				ttt = FindInStrEx(Patern,ttt)
				IF Len(ttt)>0 Then
					Rezult = ReplaceEx(ttt,Array("(","", ")","", """","", vbCrLf,",", ",,",","))
				End if
			End if
		End if
		ArrRezult = Split(Rezult,vbCrLf)
		tmpRezult = ""
		if UBound(ArrRezult)<> -1 Then
			For ee = 0 To UBound(ArrRezult)
				patern = "(\s|^|,)+(" & ArrRezult(ee) & ")+(\s|^|,|$)+"
				if Len(FindInStrEx(patern,tmpRezult)) = 0 Then
					 If Len(tmpRezult)>0 Then
						tmpRezult = tmpRezult & "," & ArrRezult(ee)
						else
						tmpRezult = ArrRezult(ee)
					End IF
				End IF
			Next
		End IF
		Rezult = Replace(tmpRezult,",",vbCrLf)
	End IF

	GetColumnsFromTZA	= Rezult
End Function

Function GetConstantExA()
	Set Childs = MetaData.TaskDef.Childs(CStr("Константа"))
	GetConstantExA = ""
	StrKO = ""
	For i = 0 To Childs.Count - 1
		Set mdo = Childs(i)
		if Len(StrKO) = 0 Then
			StrKO = mdo.Name
		else
			StrKO = StrKO & vbCrLf & mdo.Name
		End if
	next
	IF Len(StrKO) = 0 Then
		Exit Function
	End If
	GetConstantExA = SelectFrom(StrKO, Caption)

End Function

Function GetConstantEx()
	Dim TypeConstArr
	GetConstantEx = ""
	TypeConstStr = ""
	tree = ""
	Set Childs = MetaData.TaskDef.Childs(CStr("Константа"))
	For i = 0 To Childs.Count - 1
		Set mdo = Childs(i)
		if Len(TypeConstStr) = 0 Then
			TypeConstStr = mdo.Type.FullName & "##"
		elseIf InStr(1,TypeConstStr,mdo.Type.FullName&"##")=0 Then
			TypeConstStr = TypeConstStr & mdo.Type.FullName & "##"
		End if
	next
	TypeConstArr = split(TypeConstStr,"##")
	if UBound(TypeConstArr)<>-1 Then
		For i = 0 To UBound(TypeConstArr)-1
			tree = tree & TypeConstArr(i)& vbCrLf
			For tt = 0 To Childs.Count - 1
				Set mdo = Childs(tt)
				if (mdo.Type.FullName = TypeConstArr(i)) Then
					tree = tree & vbTab & mdo.Name& vbCrLf
				End if
			next
		next
	End if
	Set srv=CreateObject("Svcsvc.Service")
	Cmd = srv.SelectInTree(tree,"Открыть....",false)
	ln = Len(Cmd)
	If Ln = 0 Then
		Exit Function
	else
		rrrr = split(Cmd,"\")
		if UBound(rrrr)<> -1 Then
			GetConstantEx = rrrr(UBound(rrrr))
		End if
	End if
End Function

Sub ViewConstantEx()
	AAA = GetConstantEx()
End Sub

Private Function SelectFrom(VLStr, Caption)
	textTo = ""
	if KakVibiraem = 1 Then
		Set srv=CreateObject("Svcsvc.Service")
		textTo = srv.SelectValue(VLStr,Caption,False)
	Elseif KakVibiraem = 2 Then
		Set srv=CreateObject("Svcsvc.Service")
		textTo = srv.FilterValue(VLStr,5+16,Caption)
	Elseif KakVibiraem = 3 Then
		Set SelObj = CreateObject("SelectValue.SelectVal")
		textTo = SelObj.SelectPopUp(VLStr, Windows.ActiveWnd.HWnd, vbCrLf)
	End If
	SelectFrom = textTo
End Function

Private Function GetListOsnovaniyDoca(NameDoc)
	'18 свойство(нумерация с 1) - у документа "является основанием для"
	'следовательно, если нужно найти документы на основании
	'которых вводится сАм - нужно перебрать все документы
	'и посмотреть у них в этом свойстве
	Rezult = ""
	Set Docs = Metadata.TaskDef.Childs("Документ")
	For cntdoc = 0 To Docs.Count - 1
		Set Doc = Docs(cntdoc)
		Set PropDoc = Doc.Props
		patrn = "(Документ."&NameDoc&")[,]*"
		text = FindInStrEx (patrn, PropDoc(18))
		if Len(text)>0 Then
			Rezult = Rezult&"Документ."&PropDoc(0)&" "
		End if
	Next
	GetListOsnovaniyDoca = "#"&Rezult
End Function
'GetMethodsAndRekvOfZapros 'GetVariableAndFunctionZapros

Private Function GetVariableAndFunctionZapros(NameVariable, STipom)
	GetVariableAndFunctionZapros = ""
	Rezult = ""
	textAllmodule = GetLocationText()
	TextCurentProcFunc = GetTextCurentProcFunc()

	For qq = 0 To 1 'Два прохода
		'сначала скормим ей текущую процедуру-функцию, если не получится, тады весь текст...
		'Выясним что за переменная отвечает за текст запроса
		IF qq=0 Then
			textmodule = TextCurentProcFunc
		Else
			textmodule = textAllmodule
		End IF
		varTextOfZapros = ""

		strPatrn = "(\s|^)*(" & NameVariable & ")+(\s)*(.Выполнить|.Execute)+(\s)*(\()+(\s)*[a-zA-zа-яА-Я0-9_]+(\s)*\)"
		text = FindInStrEx (strPatrn, textmodule)
		if Len(text)>0 Then
			'Если нашли данную "фразу" выделим выражение (переменнаяСТекстомЗапроса)
			strPatrn = "(\()(\s)*[a-zA-zа-яА-Я0-9_]+(\s)*\)"
			text = FindInStrEx (strPatrn, text)
			if Len(text)>0 Then
				'уберем скобки
				strPatrn = "[a-zA-zа-яА-Я0-9_]+"
				text = FindInStrEx (strPatrn, text)
				if Len(text)>0 Then
					'если все удачно вот переменная
					varTextOfZapros = text
					Exit For
				End if
			End if
		End if
	Next
	if Len(varTextOfZapros) = 0 Then
		Exit Function
	End if
	varTextZaprosa = ""
	'вытянем исходный текст запроса
	strPatrn =	"(\s|^)*" & varTextOfZapros & "(\s)*(=)(\s)*("")(\s)*[^#]+(\s)*("")[a-zA-zа-яА-Я0-9_|,=;\s^+\(\)/{}]*(\s)*(;)"
	text = FindInStrEx (strPatrn, textmodule)
	if Len(text)>0 Then
		'уберем из текста запроса присвоение текста самого запроса переменной "Текстзапроса"
		strPatrn = "(\s|^)*" & varTextOfZapros & "(\s)*(=)(\s)*"
		textVZ = FindInStrEx (strPatrn, text)
		if Len(textVZ)>0 Then
			text = Replace(text,textVZ,"")
			'Здесь у нас чистый текст запроса, но есть еще и коментарии и дополнительные присвоения
			'к тексту запроса
			varTextZaprosa = text
		End if
	End if

	'Ищем дополнительные присвоения к тексту запроса
	strPatrn = "(\s|^)*" & varTextOfZapros & "(\s)*(=)(\s)*"& varTextOfZapros & "(\s)*(\+)(\s)*("")(\s)*[^#]+(\s)*(;)+(\s)*("")+(\s)*(;)+"
	text = FindInStrEx (strPatrn, textmodule)
	if Len(text)>0 Then
		strPatrn = "(\s|^)*" & varTextOfZapros & "(\s)*(=)(\s)*"& varTextOfZapros & "(\s)*(\+)"
		pptext = FindInStrEx (strPatrn, textmodule)
		if Len(pptext)>0 Then
			Text = replace(text,pptext,"")
			varTextZaprosa = varTextZaprosa & text
		End if
	End if
	varTextZaprosa = Replace(varTextZaprosa,vbTab,"")
	varTextZaprosa = Replace(varTextZaprosa,"|","")

	'Вычленяем переменные запроса
	strPatrn = "[a-zA-zа-яА-Я0-9_|]+(\s)*(=)(\s)*[a-zA-zа-яА-Я0-9_,\s^\.]+(;)"
	text = FindInStrEx (strPatrn, varTextZaprosa)
	if Len(text)>0 Then
		'Отсекаем по самые "равно" :)
		if STipom = 0 Then
			strPatrn = "[a-zA-zа-яА-Я0-9_|]+(\s)*(=)"
			text = FindInStrEx (strPatrn, text)
			if Len(text)>0 Then
				'Убираем эти "по самые равно" :)
				strPatrn = "[a-zA-zа-яА-Я0-9_|]+"
				text = FindInStrEx (strPatrn, text)
				if Len(text)>0 Then
					'Кушайте переменные запроса
					Rezult = text
				End if
			End if
		else
			'Берем первый описатель переменной из запроса
			strPatrn = "[a-zA-zа-яА-Я0-9_|]+(\s)*(=)(\s)*[a-zA-zа-яА-Я0-9_.]+(\s)*[,|;]"
			text = FindInStrEx (strPatrn, text)
			if Len(text)>0 Then
				Rezult = text
			End if
		End if
	End if
	if STipom = 0 Then 'Функции нам для типизированя не нужны
		'Вычленяем функции запроса
		strPatrn = "(Функция|Function)(\s)+[a-zA-zа-яА-Я0-9_]+(\s)*(=)"
		text = FindInStrEx (strPatrn, varTextZaprosa)
		if Len(text)>0 Then
			'Убираем слово Функция
			strPatrn = "(\s)+[a-zA-zа-яА-Я0-9_]+(\s)*(=)"
			text = FindInStrEx (strPatrn, text)
			if Len(text)>0 Then
				'Убираем пробелы и равно
				strPatrn = "[a-zA-zа-яА-Я0-9_]+"
				text = FindInStrEx (strPatrn, text)
				if Len(text)>0 Then
					'получите и распишитесь :)
					Rezult = Rezult & vbCrLf & text
				End if
			End if
		End if
		'Подработка (убираем не уникальные функции, вдруг гдето сидят в комментариях)
		ArrRezult = split(Rezult,vbCrLf)
		Rezult = " "
		If UBound(ArrRezult)<> -1 Then
			For ii = 0 To UBound(ArrRezult)
				ttt = Trim(ArrRezult(ii))
				patern = "(\s)+(" & ttt & ")(\s)+"
				text = FindInStrEx (patern,Rezult)
				if Len(text) = 0 Then
					'получите и распишитесь :)
					Rezult = Rezult & " " & ttt & " "
				End if
			Next
		End If
		Rezult = Trim(Rezult)
		Rezult = Replace(Rezult, " ", vbCrLf)
	End if

	GetVariableAndFunctionZapros = Rezult
End Function


Private Function GetLocationText()
	'Выбираем текст от строки где находимся до начала процедуры-функции
	GetLocationText = ""
	Rezult = ""
	doc = ""
	if Not CheckWindow(doc) then Exit Function
	if doc.Name <> "Глобальный модуль" Then
		Rezult = Doc.Text
	Else
		virajenieRgExp = "(\s|^|$)+(Процедура|Функция|Procedure|Function)(\s)+[a-zA-zа-яА-Я0-9_]+\("
		for lncnt = doc.SelStartLine To 0 Step -1
			if Len(FindInStrEx (virajenieRgExp, doc.Range(lncnt)))>0 Then
				Exit For
			End if
			Rezult = doc.Range(lncnt) & vbCrLf & Rezult
		Next
	End if
	GetLocationText = Rezult
End Function

Private Function GetTextCurentProcFunc()
	'Выбираем текст от строки где находимся до начала процедуры-функции
	GetTextCurentProcFunc = ""
	Rezult = ""
	doc = ""
	if Not CheckWindow(doc) then Exit Function
	if doc.Name <> "Глобальный модуль" Then
		'Попробуем передать текст текущей процедуры, найдем её по glRWord.doc_SelStartLine,
		'где сидит начальная строка документа с которой началось сканирование..
		If Not IsEmpty(LocalModule) Then
			IF LocalModule.CounItem<>-1 Then
				For qq = 0 To UBound(LocalModule.ArrItem)
					If Not IsEmpty(LocalModule.ArrItem(qq)) Then
						IF (LocalModule.ArrItem(qq).LineStart<glRWord.doc_SelStartLine) And (LocalModule.ArrItem(qq).LineEnd>glRWord.doc_SelStartLine) Then
							Rezult	= LocalModule.ArrItem(qq).text
							Rezult = DellKommentForText(Rezult)
							Exit For
						End IF
					End IF
				Next
			End IF
		End IF
	End if
	GetTextCurentProcFunc = Rezult
End Function

'=============================================================================================
'doc - вернем текст
Function CheckWindowOnWorkbook(doc)
	CheckWindowOnWorkbook = False
	If Windows.ActiveWnd Is Nothing Then
    	Exit Function
	End If
	Set doc = Windows.ActiveWnd.Document

    if CommonScripts.CheckDocOnExtension(doc, NotIntellisenceExtensions) Then Exit Function

	If doc = docWorkBook Then
		if (Doc.ActivePage <> 1) Then
			Exit Function
		End If
		Set doc=doc.Page(1)
		CheckWindowOnWorkbook = True
	Else
		Exit Function
	End If
	If doc.LineCount = 0 Then
    	Exit Function
	End If
	CheckWindowOnWorkbook = True
End Function

'=============================================================================================
Function CheckWindow(doc)
	CheckWindow = False

	set doc = CommonScripts.GetTextDocIfOpened(0)
	if doc is nothing then exit Function

    if CommonScripts.CheckDocOnExtension(doc, NotIntellisenceExtensions) Then Exit Function

	If doc.LineCount = 0 Then
    	Exit Function
	End If
	CheckWindow = True
End Function

' Выбор метода документа
' Надо разделить методы по областям применения и зависимости от СоздатьОбъект()
Private Sub ChoiceDocMet()
	doc = ""
	if Not CheckWindowOnWorkbook(doc) then Exit Sub

	Vals_Method_Doc=GetDocMet(0)
	Vals_Method_Doc = SortStringForList(Vals_Method_Doc, vbCrLf)
	textTo = SelectFrom(Vals_Method_Doc, "Выберите, плиз")
	if (Len(textTo) = 0) Then
		Exit Sub
	End if
	doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelStartLine,doc.SelStartCol) = textTo
	doc.MoveCaret doc.SelStartLine, doc.SelStartCol+Len(textTo)
End Sub

' Выбор Метода справочника.
' Надо разделить методы по областям применения и зависимости от СоздатьОбъект()
Private Sub ChoiceSprMet()
	doc = ""
	if Not CheckWindowOnWorkbook(doc) then Exit Sub

	Vals_Method_Spr=GetSprMet(1)
	Vals_Method_Spr = SortStringForList(Vals_Method_Spr, vbCrLf)
	textTo = SelectFrom(Vals_Method_Spr, "Выберите, плиз")
	if (Len(textTo) = 0) Then
		Exit Sub
	End if
	doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelStartLine,doc.SelStartCol) = textTo
	doc.MoveCaret doc.SelStartLine, doc.SelStartCol+Len(textTo)
End Sub

Private Function UC(Word)
	UC = UCase(Word)
End Function

'Ищет и удаляет комментарии в тексте......
Private Function DellKommentForText(ttextBlock)
	DellKommentForText = ttextBlock
	IF len(ttextBlock) = 0 Then	Exit Function
	patern = "(//)+[\S \t\-]*"
	ttext0 = FindInStrEx(patern,ttextBlock)
	If Len(ttext0)>0 Then
		ArrTText0 = Split(ttext0, vbCrLf)
		For u = 0 To Ubound(ArrTText0)
			'Message ArrTText0(u)
			ttextBlock = Replace(ttextBlock,ArrTText0(u) & vbCrLf,"")
		Next
	End IF
	While InStr(1,ttextBlock, vbCrLf&vbCrLf)>0
		ttextBlock = Replace(ttextBlock,vbCrLf&vbCrLf,vbCrLf)
	Wend
	DellKommentForText = ttextBlock
End Function

'Задача - ГЛОБАЛЬНО Получить список процедур/функций модуля, + научиться позиционироваться на них
' научиться распознавать вызовы процедуры/функции, определять её "рабочие"  переменные
' и соот-но отпарсить
' ЛОКАЛЬНО - получить список процедур/функций модуля с координатами - строка начала/конца
' и определить является ли конструкция ТаблСтрок = глСформироватьТаблицуДокумента(Контекст)
' вызовом функци из конкретного модуля

Class TheModuleItem
	' Содержит процедуру/функцию модуля и её параметры....
	Public TypeItem			' Тип итема:	0 - описание переменных и предописание процедур и функций..
							'				1 - процедра
							'				2 - функция
							'				-1 - не распознан
	Public Name				' Имя процедуры / функции.
	Public NameFull			' Имя полное процедуры / функции.
	Public LineStart		' Строка, где начинается функция/процедура
	Public LineEnd			' Строка, где заканчивается

	Public LenthText		' Длинна текста

	Public Parameters		' Строка переменных с именами параметров ч/з запятую

	Public Text				' Полный текст функции
	Public TextBezKommet	' Полный текст функции, но без комментариев..

	Public RetValueStr		' Имя переменной возвращаемой функцией...
	Private Sub Class_Initialize
		TypeItem	= -1
		Name		= ""
		LineStart	= 0
		LineEnd		= 0
		Parameters	= ""
		Text		= ""
		TextBezKommet=""
		RetValueStr = ""
	End Sub
	Function GetParamNumber(NumberP)
		tNumberP = Int(NumberP)
		tNumberP = tNumberP - 1
		GetParamNumber = ""
		if Len(Parameters)>0 And tNumberP>0 Then
			ArrParameters = Split(Parameters,",")
			If UBound(ArrParameters)<>-1 Then
				If UBound(ArrParameters)<=tNumberP Then
					GetParamNumber = ArrParameters(tNumberP)
				End If
			End If
		End If
	End Function
End Class

Class TheModule
	Public ModuleName	' Имя модуля
	Public CounItem		'Количество процедур/функций
	Public CounLine		'Количество строк в модуле когда он отсканирован...
	Public ArrItem		'Массив процедур/функций модуля класс "TheModuleItem"
	Public ModuleDoc	'Документ модуля (типа "Text")
	Public NamesAllProcFunk	' Все процедуры функции модуля в формате /ИмяПроц/ИмяПроц2/.../и т.д.
	Public ModuleTextArray	'Документ модуля (типа "Text")

	Private Sub Class_Initialize
		ModuleName		= ""	' Имя модуля
		CounItem		= "" 'Количество процедур/функций
		ArrItem			= Array("")
		NamesAllProcFunk = "/"
	End Sub

	Function IsProcedure(NameProc, LineS, LineE, TypePF, TextProcFunk,TheProc)
		IsProcedure = False
		if IsArray(ArrItem) Then
			If Len(NameProc)>0 Then
				If InStr(1,LCase(NamesAllProcFunk),LCase("/" & TRim(NameProc) & "/"))>0 Then
					For qqq = 0 To UBound(ArrItem)
						IF LCase(ArrItem(qqq).Name) = LCase(TRim(NameProc)) Then
							'Проверим не сбился ли у нас нумератор строк в итеме...
							If (InStr(1, LCase(ModuleDoc.Range(ArrItem(qqq).LineStart-1)), LCase(TRim(NameProc)))>0) OR (InStr(1, LCase(ModuleDoc.Range(ArrItem(qqq).LineStart)), LCase(TRim(NameProc)))>0) Then
								IsProcedure = True
								LineS		= ArrItem(qqq).LineStart
								LineE		= ArrItem(qqq).LineEnd
								TextProcFunk = ArrItem(qqq).Text
								Set TheProc = ArrItem(qqq)
							Else
								Message "Модуль откорректировали, нужна переинициализация!"
							End IF
						End IF
					Next
				End IF
			End IF
		else
			Message "Модуль не инициализирован!"
		End IF
		'ArrItem			= Array("")
	End Function

	Sub SetDoc(Doc)
		Set ModuleDoc = Doc
		ModuleName = Doc.Name
		ModuleTextArray = Split(Doc.Text,  vbCrLf)
	End Sub

	Sub InitializeModule(ExtractAll)
		GetAllProcFunc(ModuleDoc.Text)
		if (ExtractAll=1) Then
			ExtractNameAndOther()
		End IF
	End Sub

	Sub ReDimModule (cnt)
		ReDim ArrItem(cnt)
	End Sub

	Sub Listing ()
		For ee = 0 To UBound(ArrItem)
			If Not IsEmpty(ArrItem(ee)) Then
				Message ArrItem(ee).LineStart & " " & ArrItem(ee).LineEnd & " " & ArrItem(ee).Parameters & " "
			End If
		Next
	End Sub

	Sub VerifyPosition(FuncProcName, Position)
		MinPos = 0
		MaxPos = ModuleDoc.LineCount

		SkipPosition = Position
		'Position = -1 'Проверка окончилась пшиком
		For qq = 0 To 50
			If qq = 0 Then
			ElseIf qq/2 = qq\2 Then 'Кратно двум проверяем сверху
				SkipPosition = Position - qq
			Else
				SkipPosition = Position + qq
			End IF
			If (SkipPosition>=MinPos) And (SkipPosition<=MaxPos) Then
ProfilerEnter("ModuleDoc.Range")
' супер ускорение artbear !!
'				ttext = ModuleDoc.Range(SkipPosition)
				ttext = ModuleTextArray(SkipPosition)
ProfilerLeave("ModuleDoc.Range")
				If InStr(1,LCase(ttext),LCase(FuncProcName))=1 Then
					Position = SkipPosition
					Exit For
				End IF
			End IF
		Next
	End Sub
	'==========================================
	'Попытка выровнять смещения процерур и функций... актульно для глобального модуля..
	Sub VerifyAndRecalcPosition()
		MinPos = 0
		MaxPos = ModuleDoc.LineCount
		AddDeleteLines = ModuleDoc.LineCount - MaxPos
		If CounLine <> ModuleDoc.LineCount And False Then ' Пока отложим..
			'Прямой проход... для "Правильных сначала"
			For ee = 0 To UBound(ArrItem)

				If Not IsEmpty(ArrItem(ee)) Then
					'ArrItem(ee).NameFull
				End IF
			Next
		End IF
		CounLine = ModuleDoc.LineCount
	End Sub


	Sub ExtractNameAndOther()
	'Stop
		For ee = 0 To UBound(ArrItem)
			If Not IsEmpty(ArrItem(ee)) Then
				' Выделяем имя и тип процедура или функция
				ArrItem(ee).NameFull = ArrItem(ee).Name
				tName = FindInStrEx("(Функция|Function|Процедура|Procedure)+[\s]+[" & cnstRExWORD &"]+[\s]*[\(]+",ArrItem(ee).Name)
				tType = FindInStrEx("(Функция|Function|Процедура|Procedure)+[\s]+",tName)
				tName = ReplaceEx(tName, Array(tType,"", "(",""))
				tName = Trim(tName)
				tType = UCAse(Trim(tType))
				ArrItem(ee).Name = tName
				NamesAllProcFunk = NamesAllProcFunk & lCase(tName) & "/"
				Select case tType
				case "ФУНКЦИЯ":					ArrItem(ee).TypeItem = 2
				case "FUNCTION":    			ArrItem(ee).TypeItem = 2
				case "ПРОЦЕДУРА"				ArrItem(ee).TypeItem = 1
				case "PROCEDURE"				ArrItem(ee).TypeItem = 1
				end select
				' Выделяем параметры...
				tName = FindInStrEx("(Функция|Function|Процедура|Procedure)+[\s]+[" & cnstRExWORD &"]+[\s]*[\(]+",ArrItem(ee).NameFull)
				'Легкий путь
				tName =  "(" &Replace(ArrItem(ee).NameFull,tName,"")
				tName = ReplaceEx(tName, Array("(","", ")",""))
				' Находим присвоения ТраЛяЛя = "что-то"
				tStr = FindInStrEx("\=+\s*("")+\s*[" & cnstRExWORD &",\.\\\/\s]*\s*("")+",tName)
				If Len(tStr)>0 Then
					tArr = Split(tStr,vbCrLf)
					tName = ClearInSring(tName, tArr)
				End If
				tStr = FindInStrEx("\=+\s*[""""]+",tName)
				If Len(tStr)>0 Then
					tArr = Split(tStr,vbCrLf)
					tName = ClearInSring(tName, tArr)
				End If
				tStr = FindInStrEx("\=+\s*[0-9\.]+",tName)
				If Len(tStr)>0 Then
					tArr = Split(tStr,vbCrLf)
					tName = ClearInSring(tName, tArr)
				End If
				tName = LCase(tName)
				tArr = Split(tName,",")
				If UBound(tArr)<>-1 Then
					For qq = 0 To UBound(tArr)
						tArr(qq) = Trim(tArr(qq))
						If Len(tArr(qq))>0 Then
							tArr(qq) = Replace(tArr(qq), "знач ","")
						End If
					Next
					ArrItem(ee).Parameters	= Join(tArr,",")
				End If
			End If
		Next
	End Sub

Dim Profiler
Private Sub ProfilerEnter(name)
'	Set Profiler = CreateObject("SHPCE.Profiler")
'	Profiler.StartPiece(name)
End Sub

Private Sub ProfilerLeave(name)
'	Set Profiler = CreateObject("SHPCE.Profiler")
'	Profiler.EndPiece(name)
End Sub

Private Sub GetAllProcFunc(textModule) ',ArrNameProcFunc, ArrTextProcFunc)
'Set Profiler = CreateObject("SHPCE.Profiler")

ProfilerEnter("GetAllProcFunc")
ProfilerEnter("0")

		CounLine = ModuleDoc.LineCount
		Patern = "(Функция|Function|Процедура|Procedure)+[\s]+[" & cnstRExWORD &"]+[\s]*[\(]+[" & cnstRExWORD &"=, \t""]*[\)]*" '\s*(Экспорт|Expotr)*\s*(Далее|Forward)*" '-плоховато выходит
''		ttextPF = FindInStrEx(Patern,textModule)
''
''		ArrNameProcFunc  = Array("")
''stop
'		ttextPF = FindInStrEx(Patern,textModule)
'			ArrNameProcFunc = Split(ttextPF, vbCrLf)
'Message UBound(ArrNameProcFunc)
'if UBound(ArrNameProcFunc)-2 > -1 then Message ArrNameProcFunc(UBound(ArrNameProcFunc)-2)
'if UBound(ArrNameProcFunc)-1 > -1 then Message ArrNameProcFunc(UBound(ArrNameProcFunc)-1)

		ArrNameProcFunc = FindInStrExArtur(Patern,textModule)
		textModuleSrc = textModule
'Message UBound(ArrNameProcFunc)
'if UBound(ArrNameProcFunc)-2 > -1 then Message ArrNameProcFunc(UBound(ArrNameProcFunc)-2)
'if UBound(ArrNameProcFunc)-1 > -1 then Message ArrNameProcFunc(UBound(ArrNameProcFunc)-1)

		ArrForLocText = ""
		PosithionInModule = 0
		ttextPFCurent = ""	'ttextPF = Replace(ttextPF,"(","")
'		If Len(ttextPF)>0 Then
		If UBound(ArrNameProcFunc)>0 Then
'			ArrNameProcFunc = Split(ttextPF, vbCrLf)
			If UBound(ArrNameProcFunc)>0 Then
				ReDimModule UBound(ArrNameProcFunc)
				Dim ItemsCount
				ItemsCount = UBound(ArrNameProcFunc)
				'For ee = 0 To UBound(ArrNameProcFunc)-1 : Status "Поиск фукц/проц.... " & GetProcent(ee, UBound(ArrNameProcFunc))
ProfilerLeave("0")
ProfilerEnter("For")
				For ee = 0 To ItemsCount-1
ProfilerEnter("Status")
					Status "1 Поиск фукц/проц.... " & GetProcent(ee, ItemsCount)
ProfilerLeave("Status")

					'Идея такая берем итем N из массива и итем N+1 и Выгрызаем текст между ними.
					'итем представляет собой заголовок процедуры

					' отключим пока...
					'Message ArrNameProcFunc(ee)
ProfilerEnter("1")
					Set ItemModule = New TheModuleItem
'					Pos1 = InStr(1, textModule,ArrNameProcFunc(ee))
					Pos1 = FindInStrExArturPositionArray(ee)
					if ee = 0 Then
						'Первое вхождение в процедуру - Вычислим смещение строки
'						ttextPFCurent = Mid(textModule,1,pos1)
						ttextPFCurent = Mid(textModuleSrc,1,pos1)

						ArrForLocText = Split(ttextPFCurent,vbCrLf)
						PosithionInModule = UBound(ArrForLocText)
						if (PosithionInModule = -1) Then
							PosithionInModule = 1
						Else
							PosithionInModule = PosithionInModule + 1
						End IF
					End IF
ProfilerLeave("1")
ProfilerEnter("2")
ProfilerEnter("2.0")
'					VerifyPosition ArrNameProcFunc(ee), PosithionInModule
ProfilerLeave("2.0")
ProfilerEnter("2.1")
'					Pos2 = InStr(1, textModule,ArrNameProcFunc(ee+1))
					Pos2 = CLng(FindInStrExArturPositionArray(ee+1))
					if Pos2 = 0 then
						Pos2 = Len(textModuleSrc)
					end if
'					ttextPFCurent = Mid(textModule,pos1,Pos2-pos1)
					ttextPFCurent = Mid(textModuleSrc,pos1,Pos2-pos1)

					ItemModule.LenthText = Pos2-pos1
					ItemModule.Text = ttextPFCurent
ProfilerLeave("2.1")
ProfilerEnter("2.2")
					ArrForLocText = Split(ttextPFCurent,vbCrLf)
ProfilerLeave("2.2")
ProfilerEnter("2.3")
					ItemModule.LineStart = PosithionInModule
					PosithionInModule = PosithionInModule + UBound(ArrForLocText)
					ItemModule.LineEnd	= PosithionInModule-1
					ItemModule.Name = ArrNameProcFunc(ee)
ProfilerLeave("2.3")
ProfilerEnter("2.4")
					Set ArrItem(ee) = ItemModule
ProfilerLeave("2.4")
ProfilerEnter("2.5")

					textModule = Mid(textModule,Pos2)
ProfilerLeave("2.5")
ProfilerLeave("2")
				Next
ProfilerLeave("For")
			End IF
		End if
ProfilerEnter("3")
		Set ItemModule = New TheModuleItem

		ItemModule.Text = textModule
		ArrForLocText = Split(textModule,vbCrLf)
		ItemModule.LineStart = PosithionInModule
		PosithionInModule = PosithionInModule + UBound(ArrForLocText)
		ItemModule.LineEnd	= PosithionInModule
		ItemModule.Name = ArrNameProcFunc( UBound(ArrNameProcFunc))
		Set ArrItem(ee) = ItemModule
		Status ""
ProfilerLeave("3")
ProfilerLeave("GetAllProcFunc")
'Set Results = Profiler.Results
'For Each R In Results
''	Message R & "/" & TypeName(R) & "/" & IsObject(R) & "/" & VarType(R)
'	Message R
'Next
	End Sub
End Class

Sub NewInicializeModule()
'Демонстрация инициализации модуля..
	Set LocalModule = New TheModule
	doc = ""
	if Not CheckWindow(doc) then Exit Sub

	LocalModule.SetDoc(doc)
	LocalModule.InitializeModule(1)
	'LocalModule.ExtractNameAndOther
	LocalModule.Listing
End Sub




Sub InicializeModule()
'stop
	Set LocalModule = New TheModule
	TextModuleInTxtFilesProc()
	LocalModule.ExtractNameAndOther
	'LocalModule.Listing
End Sub


Private Sub TextModuleInTxtFilesProc()
	doc = ""
	if Not CheckWindow(doc) then Exit Sub
	'Вычислим заголовки всех процедур и функций модуля
	ttext = doc.text
	ArrTtextOfPF  = Array("")
	ArrPF		  =  Array("")
	GetAllProcFunc ttext, ArrPF, ArrTtextOfPF
	For ee = 0 To Ubound(ArrTtextOfPF) ': Status "Замена комментария..."
		'ttextProc = DellKommentForText(ArrTtextOfPF(ee))
		ttextProc = ArrTtextOfPF(ee)
	Next :	Status "Замена комментария...ОК!"
End Sub

Private Function GetProcent(p1, p2)
	GetProcent = " %"
	proc = p1 * 100 / p2
	proc = "" & proc
	Arr = Split(proc,",")
	GetProcent = "" & Arr(0) & GetProcent
End Function

Private Function ClearInSring(tStr, tArr)
	ClearInSring = tStr
	if IsArray(tArr) Then
		If UBound(tArr)<>-1 Then
			For qq = 0 To UBound(tArr)
				If Len(tArr(qq))>0 Then
					tStr = Replace(tStr, tArr(qq), "")
				End If
			Next
		End If
	Else
		tStr = Replace(tStr, tArr, "")
	End If
	ClearInSring = tStr
End Function

Private Function GetStrAccessRight()
	ArrstrRekv = Array("Чтение", "АдминистративныеФункции", "СохранениеИВыгрузкаДанных", "УправлениеОперативнымиИтогами", _
	 "УправлениеБухгалтерскимиИтогами", "Монитор", "OLEAutomationServer", "УдалениеПомеченныхОбъектов", "ПоискСсылокНаОбъекты", "ИспользованиеЛюбыхВнешнихОтчетов", _
	 "ИспользованиеОбщихВнешнихОтчетов", "ИспользованиеФункцийВТабло", "ГрупповоеПроведениеДокументов", "ИспользованиеТаблоСчетов", _
	 "МонопольныйРежим", "АвтообменВРаспределеннойИБ", _
	 "Корректировка", "ЛюбыеИзменения",	 "ВводНового",	 "Удаление", "ПометкаНаУдаление", _
	 "СнятиеПометкиНаУдаление", 	 "Выбор", 	 "ПросмотрПодчиненныхДокументов", _
	 "ПроведениеДокумента", 	 "ИзменениеПроведенныхДокументов", 	 "ИзменениеДокументовБезПерепроведения", _
	 "ПроведениеДокументовЗаднимЧислом", "Использование", 	 "ПросмотрСписка", _
	 "ВводОперацииБезПроверкиПроводок",  "ВключениеПроводокОперации", 	 "ВыключениеПроводокОперации", _
	 "ПросмотрСпискаКорректныхПроводок",  "РедактированиеСпискаКорректныхПроводок",  "РедактированиеПраздников")
	 GetStrAccessRight = Join(ArrstrRekv,vbCrLf)
	 GetStrAccessRight = SortStringForList(GetStrAccessRight, vbCrLf)

End Function


Private Function FindInStrExA (patrn, strng)
	FindFirstInFindInStrEx = True
	FindInStrExA = FindInStrEx (patrn, strng)
	FindFirstInFindInStrEx = false
End Function

Private Function FindInStrEx (patrn, strng)
  regEx.Pattern = patrn			' Set pattern.
  Set Matches = regEx.Execute(strng)	' Execute search.
  RetStr = ""
  For Each Match in Matches		' Iterate Matches collection.
	if Len(RetStr)>0 Then
		RetStr = RetStr & vbCrLf & Match.Value
	else
		RetStr = Match.Value
    End if
    if (FindFirstInFindInStrEx = True) Then
		Exit For
    End if
  Next
  FindInStrEx = RetStr
End Function

' artbear
Dim FindInStrExDict
'set FindInStrExDict = CreateObject("Scripting.Dictionary")
Dim FindInStrExArturNamesArray, FindInStrExArturPositionArray

Private Function FindInStrExArtur(patrn, strng)
  regEx.Pattern = patrn			' Set pattern.
  Set Matches = regEx.Execute(strng)	' Execute search.
'  RetStr = ""
  Redim FindInStrExArturNamesArray(Matches.Count)
  Redim FindInStrExArturPositionArray(Matches.Count)
  Dim i
  i = 0
  For Each Match in Matches		' Iterate Matches collection.
'	if Len(RetStr)>0 Then
'		RetStr = RetStr & vbCrLf & Match.Value
'	else
'		RetStr = Match.Value
'    End if
		FindInStrExArturNamesArray(i) = Match.Value
		FindInStrExArturPositionArray(i) = Match.FirstIndex + 1
		i = i + 1
    if (FindFirstInFindInStrEx = True) Then
		Exit For
    End if
  Next
'  FindInStrEx = RetStr
  FindInStrExArtur = FindInStrExArturNamesArray
End Function 'FindInStrExArtur


Set regEx	= New RegExp : regEx.IgnoreCase = True : regEx.Global = True
Set GRegExp = New RegExp : GRegExp.IgnoreCase = True : GRegExp.Global = True
FindFirstInFindInStrEx = False

Private Function VibColor()	'Выброр цвета
	VibColor = "-1"
	set CD = CreateObject("MSComDlg.CommonDialog")
	CD.ShowColor()
	VibColor = CD.Color
End Function

Sub AddWordToSlovar()
	Doc = ""
	If CheckWindow(Doc) Then
		ttext = doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelEndLine,doc.SelEndCol)
		If (Len(ttext) > 0) Then'And (InStr(1,ttext," ") = 0) Then
			AddToSlovar(ttext)
		End IF
	End IF
End Sub

Sub InsertFromSlovar()
	Doc = ""
	If Not CheckWindow(doc) Then
		Exit sub
	End IF
	ttext  = GetWordFromSlovar()
	If Len(ttext) = 0 Then
		Exit sub
	End IF
	Pos = len(ttext)
	doc.range(doc.SelStartLine,doc.SelStartCol, doc.SelStartLine,doc.SelStartCol) = ttext
	doc.MoveCaret doc.SelStartLine, doc.SelStartCol+Pos
End Sub


Sub AddToSlovar(Word)
	If Len(Word)<1 Then
		Exit Sub
	End IF
	ttext = LoadSlovar()
	If Len(ttext)>0 Then
		If InStr(1,lcase(ttext),lcase(Word))>0 Then
			Exit Sub
		End IF
	End IF

	DictFileName = BinDir + "DictDots.txt"
	FileExists = FSO.FileExists(DictFileName)
	IF Not FileExists Then
		FSO.CreateTextFile(DictFileName)
	End IF
	FileExists = FSO.FileExists(DictFileName)
	IF Not FileExists Then
		Exit Sub
	End IF
	Set Fl = FSO.OpenTextFile(DictFileName,8)
	Fl.WriteLine(Word)
	Fl.Close
End sub

Function LoadSlovar()
	LoadSlovar = ""
	DictFileName = BinDir + "DictDots.txt"
	FileExists = FSO.FileExists(DictFileName)
	IF FileExists Then
		Set Fl = FSO.GetFile(DictFileName)
		Set FileStream = Fl.OpenAsTextStream()
		If Fl.Size<> 0 Then
			LoadSlovar = FileStream.ReadAll()
		End IF
	End IF
End Function

Function GetTypeFromVid(tempNameObj)
	GetTypeFromVid = ""
	ArrTipes = Array("Документ","Справочник","Регистр","Константа", "Перечисление")
	For tttt = 0 To 4
		Set MDObjekts = Metadata.TaskDef.Childs(CStr(ArrTipes(tttt)))
		For cntdoc = 0 To MDObjekts.Count - 1
			Set MDObj = MDObjekts(cntdoc)
			if (UCase(MDObj.Name) = UCase(tempNameObj)) Then
				GetTypeFromVid = ArrTipes(tttt)
				tempNameObj = ""
				Exit Function
			End If
		Next
	Next
End Function

Function GetWordFromSlovar()
	ttext = LoadSlovar()
	ttext = SelectFrom(ttext,"")
	If Len(ttext) <> 0 Then
		GetWordFromSlovar = ttext
	End IF
End Function

Private Sub AddToDict(Dict, ttextAdd, ttextObj, Spliter)
	ttextAdd = lcase(ttextAdd)
	ArrText = Split(ttextAdd,Spliter)
	For qq = 0 To Ubound(ArrText)
		Dict.Add ArrText(qq), ttextObj
	Next
End Sub
Private Function CompareNoCase(str1, str2, IsTrim)
	CompareNoCase = false
	Tstr1 = str1 : 	Tstr2 = str2
	if IsTrim = 1 Then
		Tstr1 = Trim(str1) : 	Tstr2 = Trim(str2)
	End IF
	if LCase(Tstr1) = LCase(Tstr2) Then
		CompareNoCase = true
	End IF

End Function

'Возвращает число вхождений строки шаблона поиска в строку поиска.
Private Function StrCountOccur(str1, str2)
	StrCountOccur = 0
	Tstr1 = lcase(str1) : 	Tstr2 = lcase(str2)
	find = InStr(1,Tstr1,Tstr2)
	Do While find
		StrCountOccur = StrCountOccur + 1
		find = InStr(find + Len(str2),Tstr1,Tstr2)
	Loop
End Function

Sub Configurator_OnActivateWindow(Wnd,bActive) 'As ICfgWindow, ByVal bActive As Boolean)
' artbear - устраню ошибку, возникающую при закрытии всех окон (через меню) - если переданное окно уже не существует
	caption = ""
	on error resume next
	Caption = W.Caption
	on error goto 0
	If Caption = "" Then Exit Sub

	On Error Resume Next
    Set doc134 = Wnd.Document
    iErrNumber = err.Number
	On Error GoTo 0
    If iErrNumber <> 0 Then
        Exit Sub
    End If

	if Not bActive Then
		Exit Sub
	End IF
	on error resume next
	iErrNumber = Err.Number
	on error goto 0
	Set doc = Wnd.Document
	if doc.Type = 0 Then
		Exit Sub
	End IF
	If Doc.Name = "Глобальный модуль" Then
		GlobalModuleParse = 0
	End IF
End Sub

'================================================
Private Function ReplaceEx(ttext, Arr1)
	ReplaceEx = ttext
	For i=0 To UBound(Arr1) / 2
		ttext = Replace(ttext, Arr1(i*2), Arr1(i*2+1))
	Next
	ReplaceEx = ttext
End Function

' Идея такая, вместо подстановки длинной фразы:
' = Перечисление.видыавансовыхотчетов.ОплатаПоставщику
' можем поискать в глобальных переменных переменную типа
' Перечисление.видыавансовыхотчетов и заменить на имя этой
' переменной. Получится компактнее
Sub ExtraktGlobalVariableName(ttextIns)
	Arrttext = Split(ttextIns,".")
	if UBound(Arrttext) = 2 Then
		ttext = Arrttext(0) & "." & Arrttext(1)
		ttext = lcase(ttext)
		a = GlobalVariableType.Items             ' Ищем в итемах данное перечисление
		PosFind =  -1
		For i = 0 To GlobalVariableType.Count -1
			if Trim(lcase(a(i))) = ttext Then
				PosFind = i
				Exit For
			End IF
		Next
		if PosFind<>-1 Then
			s = GlobalVariableNoCase.Items
			ttextIns = s(PosFind) & "." & Arrttext(2)
		End IF
	End IF
End Sub

Function SimpleType(TypeVid1)
	SimpleType = False
	TypeVid = UCase(trim(TypeVid1))
	if Len(TypeVid)>0 Then
		Select case TypeVid
		case "СТРОКА":	SimpleType = True
		case "STRING":	SimpleType = True
		case "ЧИСЛО":   SimpleType = True
		case "NUMBER":   SimpleType = True
		case "ДАТА":	SimpleType = True
		case "DATE":	SimpleType = True
		end select
	End IF

End Function

Function ObjectExist(TType, tVid)
	ObjectExist = false
	if InStr(1,lcase("/константа/Документ/Справочник/регистр/отчет/обработка/"), lcase("/" & TType & "/"))>0 Then
		Set MetaObjcts = MetaData.TaskDef.Childs(CStr(TType))
		For tt = 0 To MetaObjcts.Count-1
			if LCase(MetaObjcts(tt).Name) = LCase(tVid) Then
				ObjectExist = true
				exit for
			End IF
		Next
	End IF
End Function

'====================================
'Работа со словарями внешних компонент...
Function Get_file_vk_dict(StrObjekta)
	Get_file_vk_dict = ""
	lcStrObjekta = lcase(StrObjekta)
	if vk_dict_CreateObject.Exists(lcStrObjekta) Then
		Get_file_vk_dict = vk_dict_CreateObject.Item(lcStrObjekta)
	End IF
End Function

Sub Loadvk_dict_CreateObject()
	'Считываем словарь внешних компонент..
	vk_dict_CreateObject.RemoveAll()
	File_vk_dictName = BinDir & "config\Intell\vk_dict_CreateObject.Dict"
	FileExists = FSO.FileExists(File_vk_dictName)
	if FileExists = True then
		Set Fl = FSO.GetFile(File_vk_dictName)
		Set FileStream = Fl.OpenAsTextStream()
		AllMeth = FileStream.ReadAll()
		AllMethods = Split(AllMeth, vbCrLf)
		for i = 0 to UBound(AllMethods)
			IF Len(AllMethods(i))>0 Then
				Arr2 = Split(AllMethods(i)," ")
				if UBound(Arr2)>0 Then
					Arr2(0) = trim(Arr2(0))
					Arr2(1) = trim(Arr2(1))
					Arr2(1) = Replace(Arr2(1),".ints","")
					if vk_dict_CreateObject.Exists(lcase(Arr2(0))) Then
						message "Дублирование объектов в файле" & File_vk_dictName
					Else
						vk_dict_CreateObject.Add lcase(Arr2(0)), Arr2(1)
					end if
				end if
			end if
		next
	end if
End Sub

Sub Init(dummy) ' Фиктивный параметр, чтобы процедура не попадала в макросы
	InitScript(0) ' инициализация общего скритплета

	'*************************Документ**********************
	ttextObjekts = "Документ"
	ttext = "ДатаДок,DocDate,НомерДок,DocNum,ТекущийДокумент,CurrentDocument,ВыбратьДокументы,SelectDocuments," & _
		"ВыбратьПодчиненныеДокументы,SelectChildDocs,ВыбратьПоПоследовательности,SelectBySequence,ВыбратьПоНомеру,SelectByNum," & _
		"СортироватьСтроки,SortLines,Провести,MakeActions,СделатьНеПроведенным,UnPost,СравнитьТА,CompareWithAP"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","
	'Еще добавим общие реквизиты документа :)
	ttext = GetORD()
	ttext = Replace(ttext,vbCrLf,",")
	If Len(ttext)>0 Then
		AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","
	End IF

	'*************************СписокЗначений**********************
	ttextObjekts = "СписокЗначений"
	ttext = "Размерсписка,GetListSize,СортироватьПоПредставлению,SortByPresent,ИзСтрокиСРазделителями,FromSeparatedString,ВстрокуСРазделителями," & _
	"ToSeparatedString,Пометка,Check,УдалитьВсе,RemoveAll,СдвинутьЗначение,MoveValue"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","

	'*************************Справочник**********************
	ttextObjekts = "Справочник"
	ttext = "НайтиПоНаименованию,FindByDescr,НайтиПоРеквизиту,FindByAttribute,НайтиЭлемент,FindItem,ПолучитьЭлемент,GetItem," & _
	"ИспользоватьВладельца,UseOwner,ВключатьПодчиненные,IncludeChildren,ПорядокКодов,OrderByCode," & _
	"ПорядокНаименований,OrderByDescr,ПорядокРеквизита,OrderByAttribute,НоваяГруппа,NewGroup," & _
	"ПрефиксКода,CodePrefix,УстановитьНовыйКод,SetNewCode"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","

	'*************************Таблица**********************
	ttextObjekts = "Таблица"
	ttext = "ВывестиСекцию,PutSection,ПрисоединитьСекцию,AttachSection,НоваяСтраница,NewPage,ВысотаТаблицы,TableHeight," & _
	"ШиринаСекции,SectionWidth,ТолькоПросмотр,ReadOnly,Защита,Protection,Область,Area," & _
	"ОбластьПечати,PrintRange,ПараметрыСтраницы,PageSetup,КоличествоЭкземпляров,NumberOfCopies,ЭкземпляровНаСтранице,CopyesPerPage," & _
	"Напечатать,Print,ЗначениеТекущейЯчейки,ValueOfCurrentCell"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","


	'*************************ТаблицаЗначений**********************
	ttextObjekts = "ТаблицаЗначений"
	ttext = "КоличествоКолонок,ColumnCount,ВставитьКолонку,InsertColumn,УдалитьКолонкуDeleteColumn,УстановитьПараметрыКолонки,SetColumnParameters," & _
	"ПолучитьПараметрыКолонки,GetColumnParameters,СдвинутьСтроку,MoveLine,Заполнить,Fill," & _
	"Свернуть,GroupBy,ВидимостьКолонки,ColumnVisibility,Фиксировать,Fix,ВыводитьПиктограммы,ShowImages"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","

	'*************************ТаблицаЗначений**********************
	ttextObjekts = "Регистр"
	ttext = "Приход,Income,Расход,Outcome,ВыбратьДвижения,SelectActs,ВыбратьДвиженияДокумента,SelectDocActs,ПолучитьДвижение,GetDocAct," & _
	"ВыбратьИтоги,SelectTotals,ПолучитьИтог,GetTotal,ВременныйРасчет,TempCalc,ВыгрузитьИтоги,RetrieveTotals," & _
	"УстановитьЗначениеФильтра,SetFilterValue,ИспользоватьПериод,UsePeriod,СводныйИтог,ConsolidatedTotal,СводныеИтоги,ConsolidatedTotals," & _
	"ИтогиПолучить,TotalsGet,Остаток,Rest,СводныйОстаток,ConsolidatedRest,Остатки,Rests,СводныеОстатки,ConsolidatedRests,ОстаткиПолучить," & _
	"GetRests,ПривязыватьСтроку,LinkLine,ДвижениеПриход,ActIncome,ДвижениеРасход,ActOutcome,ДвижениеПриходВыполнить,DoActIncome," & _
	"ДвижениеРасходВыполнить,DoActOutcome,Движение,Act,ДвижениеВыполнить,DoAct"
	AddToDict UniMetodsAndRekv, ttext, ttextObjekts, ","
	SoobshitType = False
	Loadvk_dict_CreateObject
End Sub

'Контроль модуля на неверные "Форма.<Реквизит>
Sub SyntaxCheckModule()
	Doc = "" : WorkBook = ""
	If Not CheckWindowOnWorkbook(Doc) Then
		Exit Sub
	End IF

	' Проверка валидности выражений Форма.АтрибутФормы
	'Массив методов объекта "Форма". Их надо исключить из проверки...
	ArrOFMethods = Array("Закладки","TabCtrl","Параметр","Parameter","ТолькоПросмотр","ReadOnly","Обновить","Refresh","ИспользоватьЗакладки","TabCtrlState",_
	"ИспользоватьСлой","UseLayer","Заголовок","Caption","ПанельИнструментов","ToolBar","КнопкаПоУмолчанию","DefButton",_
	"ОбработкаВыбораСтроки","ProcessSelectLine","ВыполнитьВыбор","MakeChoice","РежимВыбора","ChoiceMode",_
	"МодальныйРежим","ModalMode","ПолучитьАтрибут","GetAttrib","АктивныйЭлемент","ActiveControl","ТекущаяКолонка","CurrentColumn","Закрыть","Close")

	Patern = "(\s|^|;)+(форма|Form)+\s*\.+[" & cnstRExWORD & "]+"
	'Найдем вхождения в текст "Форма.Реквизит
	ttext = FindInStrEx(Patern, Doc.text)
	ArrFR = Split(ttext,vbCrLf)
	ttextROF = GetTableRecvFromForms(0,FALSE)
	ArrROF = Split(ttextROF,vbCrLf)
	if (UBound(ArrFR)<>-1) And (UBound(ArrROF)<>-1) Then
		For qq=0 To UBound(ArrFR)
			ArrFR(qq) = ReplaceEX(ArrFR(qq), Array(" ","",";","", vbcr,"",vbTab,""))
			if Len(ArrFR(qq))>0 Then
				Arr = Split(ArrFR(qq),".")
				if UBound(Arr)=1 Then
					IsFind = False
					if Not FindInArray(Arr(1),ArrOFMethods) Then
						For qq2=0 To UBound(ArrROF)
							if UCase(Arr(1)) = UCase(ArrROF(qq2)) Then
								IsFind = True
								Exit For
							End IF
						Next
						If Not IsFind Then
							Message "Некоректное выражение: """ & ArrFR(qq)&"""", 2
						End IF
					End IF
				End IF
			End IF
		Next
	End If
End Sub

Function FindInArray(TheWord, TheArray)
	FindInArray = False
	For qq = 0 To UBound(TheArray)
		if Trim(UCase(TheArray(qq))) = Trim(UCase(TheWord)) Then
			FindInArray = True
			Exit For
		End IF
	Next
End Function

'
' Процедура инициализации скрипта
'
Sub InitScript(dummy) ' Фиктивный параметр, чтобы процедура не попадала в макросы

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

End Sub ' InitScript

Init 0