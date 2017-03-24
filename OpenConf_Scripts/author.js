$NAME Авторские комментарии
/*===========================================================================
Copyright (c) 2004-2005 Alexander Kuntashov
=============================================================================
Скрипт:  author.js ("Авторские комментарии")
Версия:  $Revision: 1.9 $
Автор:   Александр Кунташов
E-mail:  kuntashov at yandex dot ru
ICQ:	 338758861
Зависимости: 
	CommonServices.wsc Registry.wsc
Описание: 
	Разметка кода по признакам модифицированности с вставкой авторского
комментария (подписи автора). Возможна гибкая настройка подписи с 
использованием специальных переменных. Подпись (сигнатура) автора формируется 
из имени автора (возможно автоматическое использование имени текущего 
пользователя 1С или пользователя операционной системы), названия организации 
и текущей даты. Все части подписи опциональны; дата форматируется в соответствии
с заданным шаблоном. Настройка скрипта производится в диалговом окне.

===========================================================================*/

/*===========================================================================
								МАКРОСЫ
===========================================================================*/


//@Макрос: МаркерДобавлен ()
//	Маркирует выделенный блок или текущую строку признаком "Добавлен"
function МаркерДобавлен()
{
	setMarker(1);
}

//@Макрос: МаркерИзменен ()
//	Копирует выделенный блок или текущую строку как комментарий
//	и маркирует блок/строку признаком "Изменен"
function МаркерИзменен()
{
	setMarker(2);
}

//@Макрос: МаркерУдален ()
//	Комментирует выделенный блок/строку с одновременной установкой
//	маркера "Удален"
function МаркерУдален()
{
	setMarker(3);
}

//@Макрос: Настройка ()
//	Вызывает окно настройки параметров скрипта
function Настройка()
{
	OpenSettingsWindow();
}

//@Макрос: ВставитьТекст ()
//	Вставляет текст из буфера обмена с добавлением
//	соответствующих маркеров
function ВставитьТекст()
{
	PasteTextFromClipboard();
}

//@Макрос: СохранитьДобавивДатуВремяИзменения ()
//	Добавляет в начало активного документа текущую дату и время
//	и изменения и сохраняет его
function СохранитьДобавивДатуВремяИзменения()
{
	AddTimeStampAndSave();
}

/*===================================================================================
							Вместо тех. дока
=====================================================================================
	Для управления настройками используется скриптлет "Registry.wsc"

	Настройки хранятся в реестре в 
		HKCU\Software\1C\1Cv7\7.7\OpenConf\Scripts\<$NAME этого cкрипта> 
				
	Используются следующие параметры (все - строковые):	
		authorName - имя/никнейм автора, в строке распознаются следующие спецсимволы,
					 для которых выполняется автоподстановка:
		    		 %1CUser% - текущий пользователь 1С
					 %OSUser% - текущий пользователь (операционной) системы
				
		companyName - название организации
				
		dateFormat - формат даты
		
		markerAdded 	- открывающий маркер добавленного блока
		markerChanged	- открывающий маркер измененного блока
		markerDeleted	- открывающий маркет удаленного блока
		markerEndBlock	- закрывающий маркет блока
		
		oneliner - если задан, то используется в качестве маркера для 
				   однострочного комментария, если не задан, то в качестве маркера 
				   однострочного комментария будет использоваться соответствующий
				   открывающий маркер блока				  
				   
		signature - собственно сигнатура, которая подставляется после маркера,
					распознаются следующие спецсимволы:
					%Author% - заменяется на значение параметра authorName
					%Company% - заменяется на значение параметра companyName
					%Date% - заменяется на текущую дату, отформатированную в 
							 соответствии с dateFormat

		splitter - разделитель кода при замене

		doNotIndent - не использовать отсуп при изменении/замене - как в "Замене кода" Артура Аюханова
					  (по умолчанию используется отступ такой же, как в первой строке блока)

		doNotSignAtEnd - не добавлять сигнатуру в закрывающий маркер блока

		doNotCopyOldCode - не копировать старый код перед изменением

		addModDateTimeAtEnd - добавлять время последнего изменения в конец файла
							 (по умолчанию добавляется в начало файла)
		
===================================================================================*/

var __AuthorJSSettingsWindowCaption = "Авторские комментарии :: Настройка";
var __AuthorJSSettingsWindowPath; // путь до HTML-файла окна настроек

function indent(line)
{
    var m = line.match(/^(\s*)/);
    if (m) {
        if (m[0] !== line) {
            return m[1];
        }
    }
    return '';
}

function setMarker(markerType, newCode)
{
	var s = null, doc = null;
	if (!(doc = CommonScripts.GetTextDocIfOpened())) {
		return;
	}
	with (new Settings(CommonScripts.Registry, SelfScript.Name)) {
		Load();
		s = GetData();
	}

	/* 
	 *  Инициализация переменных подстановки 
	 */
	
	// маркер
	var AC_Marker = "";
	switch (markerType) {
		case 1:
			AC_Marker = s.markerAdded;
			break;
		case 2:
			AC_Marker = s.markerChanged;
			break;
		case 3:
			AC_Marker = s.markerDeleted;
			break;
		default:
			return; // не должно быть в принципе
	}
	
	// разделитель старого и нового кода при замене
	var AC_Splitter = s.splitter; 

	// Дата и время
	var AC_Date = getCurrentDate(s.dateFormat);
	var AC_Time = getCurrentTime(); // перемення %Time% осталась для совместимости

	// Автор
	var AC_Author = s.authorName.replace(/%1CUser%/i, get1CUser()).replace(/%OSUser%/i, getOSUser());

	// Сигнатура
	var AC_Sign = s.signature.replace(/%Author%/i, AC_Author)
				.replace(/%Company%/i, s.companyName)
				.replace(/%Date%/i, AC_Date)
				.replace(/%Time%/i, AC_Time);
	
	if (((doc.SelStartLine == doc.SelEndLine) && ((markerType != 2) || s.doNotCopyOldCode) && (!newCode))) {
		// Установка однострочного маркера
		
		var line = doc.Range(doc.SelStartLine).replace(/\s*$/i, "");
		if (s.oneliner !== null) { // если для однострочника задан свой маркер, то его и используем
			AC_Marker = s.oneliner;
		}
		doc.Range(doc.SelStartLine) = (markerType == 3?"//":"") + line + " //" + AC_Marker + AC_Sign;	
	}
	else {
		// Разметка блока
	
		var selEndLine	= doc.SelEndLine - (doc.SelEndCol?0:1);		
		var block		= doc.Range(doc.SelStartLine, 0, selEndLine, doc.LineLen(selEndLine));
	
		var lines	= block.split(/\r\n/);
		var ind		= s.doNotIndent?"":indent(lines[0]);									
		
		// открывающий маркер блока
		block = "\r\n" + ind + "//" + AC_Marker + AC_Sign + "\r\n";
		
		// удаление или замена кода - комментируем блок
		if (markerType != 1) {	
			if ((markerType != 2) || !s.doNotCopyOldCode) {
				block += commentLines(lines, ind);		
			}
		}

		// замена кода - добавляем разделитель старого и нового кода
		if (markerType == 2) {
			// при копировании одной строки разделителя не ставим (XXX может сделать опционально?)
			if (AC_Splitter && !s.doNotCopyOldCode && (doc.SelStartLine != doc.SelEndLine)) {
				block += ind + "//" + AC_Splitter + "\r\n";
			}	
		}
			
		// добавленный или скопированный код
		if (markerType != 3) {					
			block += (newCode?newCode:lines.join("\r\n")) + "\r\n";
		}
			
		// закрывающий маркер блока
		block += ind + "//" + s.markerEndBlock + (s.doNotSignAtEnd?"":AC_Sign) + "\r\n";
					
		// вставляем сгенерированный код в документ		
		doc.Range(doc.SelStartLine, 0, selEndLine, doc.LineLen(selEndLine)) = block;		

		// устанавливаем курсор внутри блока, если код скопирован, иначе - сразу после него
		doc.MoveCaret(doc.SelStartLine + lines.length + 3, 0);
	}
}

function PasteTextFromClipboard(_)
{
	var doc = null, clipboard = CommonScripts.GetFromClipboard();

	if (!clipboard) {
		return;
	}
	
	if (!(doc = CommonScripts.GetTextDocIfOpened())) {
		return;
	}
	
	var marker = ((doc.SelStartLine != doc.SelEndLine)||(doc.SelStartCol != doc.SelEndCol))?2:1;
		
	setMarker(marker, clipboard);
}

function AddTimeStampAndSave(_)
{
	var s = null, doc = null;
	if (!(doc = CommonScripts.GetTextDocIfOpened())) {
		return;
	}
	with (new Settings(CommonScripts.Registry, SelfScript.Name)) {
		Load();
		s = GetData();
	}
	
	var lineNo = s.addModDateTimeAtEnd ? doc.LineCount : 0;
	var newTimeStamp = "//@ " + getCurrentDate(s.dateFormat);
	
	var line = doc.Range(lineNo);
	
	if (!line.match(/^\/\/@/)) {
		if (s.addModDateTimeAtEnd) {
			newTimeStamp = doc.Range(lineNo) + "\r\n" + newTimeStamp;
		}
		else {
			newTimeStamp += "\r\n" + doc.Range(lineNo);
		}
	}
	
	doc.Range(lineNo) = newTimeStamp;
	Windows.ActiveWnd.Document.Save();
}

function commentLines(lines, ind)
{
	var ret = "";
	for (var i=0; i<lines.length; i++) {
		ret += ind + "//" + lines[i] + "\r\n";
	}
	return ret;
}

// посвящается Шарлин Спитери из TEXAS :-)
function ZeroZero(num)
{
	return (num>9)?num:('0'+num);
}

function getCurrentDate(format)
{
    with (new Date) {        	
		return format.replace(/yyyy/, getYear())
		.replace(/yy/, (new String(getYear())).substr(2,2))
		.replace(/dd/, ZeroZero(getDate()))
		.replace(/mm/, ZeroZero(getMonth()+1))
		.replace(/HH/, ZeroZero(getHours()))
		.replace(/MM/, ZeroZero(getMinutes()))
		.replace(/SS/, ZeroZero(getSeconds()))
	}
}

function getCurrentTime(_)
{
	with (new Date) {
		return getHours() + ':' + (getMinutes() + 1) + '.' + getSeconds();
	}
}

function get1CUser(_)
{
	return Configurator.AppProps(appUserName);
}

function getOSUser(_)
{
	return CommonScripts.WSH.ExpandEnvironmentStrings("%USERNAME%");
}

function Settings(OCReg, sname)
{		
//private:
	var sname = sname;
	var settings = {
	// настройки по умолчанию
		authorName		: "MyName",
		companyName		: "MyCompany",
		dateFormat		: "yyyy-mm-dd HH:SS:MM",
		markerAdded		: "+",
		markerChanged	: "*",
		markerDeleted	: "-",
		markerEndBlock	: "/",
		oneliner		: null,
		signature		: "%Author%@%Company%, %Date%",
		splitter		: " -------- заменено на:",
		doNotIndent		: null,
		doNotSignAtEnd	: null,
		doNotCopyOldCode: null,
		addModDateTimeAtEnd: null
	}
	
//public:	
	this.Load = function ()
	{
		var val = null, srk = OCReg.ScriptRootKey(sname);
		
		for (var a in settings) {
			if ((val = OCReg.Param(srk, a)) !== null) {
				settings[a] = val;
			}
		}		
	}
	this.Fill = function (f)
	{
		var ix = 2;
		for (var a in settings) {			
			if (settings[a] !== null) {
				f[a].value = settings[a];
			}
		}	
		f.oneliner.disabled = f.rbOneliner[0].checked = (settings.oneliner === null);
		f.rbOneliner[1].checked = !f.rbOneliner[0].checked;
		if (settings.authorName.match(new RegExp("^%1CUser%$", "i"))) {
			ix = 0;
		}
		if (settings.authorName.match(new RegExp("^%OSUser%$", "i"))) {
			ix = 1;
		}
		f.rbAuthorType[ix].checked = true;
		f.authorName.disabled = (ix !== 2);

		f.doNotCopyOldCode_		.checked = settings["doNotCopyOldCode"];
		f.doNotSignAtEnd_		.checked = settings["doNotSignAtEnd"];
		f.doNotIndent_			.checked = settings["doNotIndent"];
		f.addModDateTimeAtEnd_	.checked = settings["addModDateTimeAtEnd"];

	}
	this.Read = function (f) 
	{
		for (var a in settings) {
			settings[a] = f[a].value;
		}
		if (f.rbOneliner[0].checked) {
			settings.oneliner = null;
		}
	}	
	this.Save = function ()
	{
		var srk = OCReg.ScriptRootKey(sname);
		for (var a in settings) {
			if (settings[a] !== null) {
				OCReg.Param(srk, a) = settings[a];
			}
		}	
		if (settings.oneliner === null) {
			OCReg.DeleteParam(srk, "oneliner");
		}
	}
	this.GetData = function ()
	{
		return settings;
	}
}

function initForm(f) 
{
	with (new Settings(CommonScripts.Registry, SelfScript.Name)) {
		Load();
		Fill(f);
	}
}

function saveSettings(f)
{
	with (new Settings(CommonScripts.Registry, SelfScript.Name)) {
		Read(f);
		Save();
	}
	closeWindow();
}

function closeWindow(_)
{
	Windows.ActiveWnd.Close();
	with (CommonScripts.FSO) {
		if (FileExists(__AuthorJSSettingsWindowPath)) {
			DeleteFile(__AuthorJSSettingsWindowPath, true);
		}
	}
	reloadSelf();
}

function reloadSelf(_)
{
	Scripts.Reload(SelfScript.Name);
}

function handleDCEvent(_)
{
	with (CommonScripts) {	
		if (Registry == null) {
			Error(GetLastError() + "\n" + "Недоступны функции для работы с реестром!");
			return;
		}
	}			
	try {		
		var h = {init:initForm, ok:saveSettings, cancel:closeWindow, free:reloadSelf};
		SettingsWindow.Document.Script.SetHandlers(h);		
	}
	catch (e) {
		Message("Ошибка инициализации окна настроек", mRedErr);
		Message(e.description, mRedErr);
	}
}

function OpenSettingsWindow(_)
{
	var f,wnd = null;
	if (wnd = CommonScripts.FindWindow(__AuthorJSSettingsWindowCaption)) {
		Windows.ActiveWnd = wnd;
	} 
	else {
		with (CommonScripts.FSO) {					
			__AuthorJSSettingsWindowPath = BuildPath(GetSpecialFolder(2), GetTempName());
			f = CreateTextFile(__AuthorJSSettingsWindowPath, true);
			f.Write(getHtml());
			f.Close();			
		}		
		wnd = OpenOleForm("Shell.Explorer", __AuthorJSSettingsWindowCaption);
		SelfScript.AddNamedItem("SettingsWindow", wnd, false);
    	eval('function SettingsWindow::DocumentComplete(d,u){ return handleDCEvent()}');
		SettingsWindow.Navigate2(__AuthorJSSettingsWindowPath);
	}	
}

function Init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{
	try {
		var ocs = new ActiveXObject("OpenConf.CommonServices");
		ocs.SetConfig(Configurator);
		SelfScript.AddNamedItem("CommonScripts", ocs, false);
	}
	catch (e) {
		Message("Не могу создать объект OpenConf.CommonServices", mRedErr);
		Message(e.description, mRedErr);
		Message("Скрипт " + SelfScript.Name + " не загружен", mInformation);
		Scripts.UnLoad(SelfScript.Name); 		
	}	
}

Init();

/* сгенерировано автоматически с помощью html2scr.js */
function getHtml(_)
{
	return ''
	+"<html>\r\n"
	+"	<head>\r\n"
	+"	<title>Настройка скрипта &quot;Авторские комментарии&quot;</title>\r\n"
	+"<style type=\"text/css\">\r\n"
	+"body {\r\n"
	+"	font-family			: sans-serif;\r\n"
	+"	font-size			: small;\r\n"
	+"	background-color	: #CCCCCC;\r\n"
	+"}\r\n"
	+"</style>\r\n"
	+"<script language=\"JScript\">\r\n"
	+"var handlers\r\n"
	+"function SetHandlers(h) { handlers = h; handlers.init(settings) }\r\n"
	+"function setCheckboxValue(c) { settings[c.name.replace(/_$/,'')].value = c.checked?\"1\":\"\"; }\r\n"
	+"</script>\r\n"
	+"	\r\n"
	+"</head>\r\n"
	+"<body onUnload=\"handlers.free()\" >\r\n"
	+"<form name=\"settings\">\r\n"
	+"<center>\r\n"
	+"	<table width=\"750 pt\" border=\"1 pt\">\r\n"
	+"		<tr><td>\r\n"
	+"\r\n"
	+"<table width=\"100%\" height=\"100%\">\r\n"
	+"	<tr><td><b>Параметры сигнатуры</b></td></tr>\r\n"
	+"	<tr align=\"top\"><td>\r\n"
	+"	<table width=\"100%\">\r\n"
	+"		<tr><td colspan=\"2\">Автор:</td></tr>\r\n"
	+"		<tr><td>&nbsp;</td><td><input type=\"radio\" name=\"rbAuthorType\" value=\"1\" onClick=\"authorName.value='%1CUser%';authorName.disabled=true\"/>Имя пользователя 1С</td></tr>\r\n"
	+"		<tr><td>&nbsp;</td><td><input type=\"radio\" name=\"rbAuthorType\" value=\"2\" onClick=\"authorName.value='%OSUser%';authorName.disabled=true\"/>Имя пользователя Windows</td></tr>\r\n"
	+"		<tr><td>&nbsp;</td><td><input type=\"radio\" name=\"rbAuthorType\" value=\"3\" onClick=\"authorName.disabled=false\" checked/>Строкой:&nbsp;<input name=\"authorName\" type=\"text\" value=\"\"/></td></tr>\r\n"
	+"	</table>\r\n"
	+"	</td></tr>\r\n"
	+"	<tr><td>\r\n"
	+"	<table>\r\n"
	+"		<tr><td>Организация:&nbsp;</td><td><input name=\"companyName\" type=\"text\" value=\"\"/></td></tr>\r\n"
	+"	</table>\r\n"
	+"	</td></tr>\r\n"
	+"	<tr><td>\r\n"
	+"	<table>\r\n"
	+"		<tr><td>Формат даты:&nbsp;</td><td><input name=\"dateFormat\" type=\"text\" value=\"yyyy/mm/dd\"/></td></tr>		\r\n"
	+"	</table>\r\n"
	+"	<table width=\"100%\">\r\n"
	+"		<tr><td>Формат сигнатуры:&nbsp;</td></tr>		\r\n"
	+"		<tr><td align=\"center\"><input name=\"signature\" type=\"text\" value=\"%Author%@%Company%, %Date%\" size=\"50\"/></td></tr>\r\n"
	+"	</table>	\r\n"
	+"	</td></tr>\r\n"
	+"	</table>			\r\n"
	+"			</td>\r\n"
	+"			<td width=\"50%\" align=\"top\">\r\n"
	+"	\r\n"
	+"<table width=\"100%\" >\r\n"
	+"	<tr><td><b>Формат комментариев</b></td></tr>\r\n"
	+"	<tr><td>\r\n"
	+"	<table width=\"100%\">\r\n"
	+"		<tr><td colspan=\"2\">Открывающие блок маркеры:</td></tr>\r\n"
	+"		<tr><td>&nbsp;&nbsp;&nbsp;Фрагмент добавлен:</td><td><input name=\"markerAdded\" type=\"text\" value=\"+\"/></td></tr>\r\n"
	+"		<tr><td>&nbsp;&nbsp;&nbsp;Фрагмент изменен:</td><td><input name=\"markerChanged\" type=\"text\" value=\"*\"/></td></tr>\r\n"
	+"		<tr><td>&nbsp;&nbsp;&nbsp;Фрагмент удален:</td><td><input name=\"markerDeleted\" type=\"text\" value=\"-\"/></td></tr>\r\n"
	+"	</table>\r\n"
	+"	</td></tr>\r\n"
	+"	<tr><td>\r\n"
	+"	<table>\r\n"
	+"		<tr><td>Закрывающий маркер блока:&nbsp;</td><td><input name=\"markerEndBlock\" type=\"text\" value=\"/\"/></td></tr>\r\n"
	+"	</table>\r\n"
	+"	</td></tr>\r\n"
	+"	<tr><td>\r\n"
	+"	<table width=\"100%\">\r\n"
	+"		<tr><td colspan=\"2\">Однострочный маркер:</td></tr>\r\n"
	+"		<tr><td>&nbsp;</td><td><input name=\"rbOneliner\" type=\"radio\" value=\"1\" onClick=\"oneliner.disabled=true\" checked/>Использовать маркер блока</td></tr>\r\n"
	+"		<tr><td>&nbsp;</td><td><input name=\"rbOneliner\" type=\"radio\" value=\"2\" onClick=\"oneliner.disabled=false\"/>Другой:&nbsp;<input name=\"oneliner\" type=\"text\" value=\"\"/></td></tr>\r\n"
	+"	</table>\r\n"
	+"	</td></tr>\r\n"
	+"	<tr><td>Разделитель кода при замене:</td></tr>\r\n"
	+"	<tr><td align=\"center\"><input name=\"splitter\" type=\"text\" value=\" -------------\" size=\"40\"/></td></tr>\r\n"
	+"</table>		\r\n"
	+"				\r\n"
	+"			</td>\r\n"
	+"		</tr>\r\n"
	+"	</table><br/>\r\n"
	+"	<input name=\"doNotCopyOldCode\"	type=\"hidden\" value=\"\" />\r\n"
	+"	<input name=\"doNotSignAtEnd\"	type=\"hidden\" value=\"\" />\r\n"
	+"	<input name=\"doNotIndent\"		type=\"hidden\" value=\"\" />\r\n"
	+"	<input name=\"addModDateTimeAtEnd\" type=\"hidden\" value=\"\" />\r\n"
	+"	<table width=\"750 pt\">\r\n"
	+"		<tr><td><b>Дополнительные настройки</b></td></tr>\r\n"
	+"		<tr><td><input name=\"doNotCopyOldCode_\" type=\"checkbox\" onClick=\"splitter.disabled=this.checked;setCheckboxValue(this);\"/>Не оставлять копию кода при замене/изменении </td></tr>\r\n"
	+"		<tr><td><input name=\"doNotSignAtEnd_\" type=\"checkbox\" onClick=\"setCheckboxValue(this);\"/>Не добавлять сигнатуру в закрывающий маркер блока</td></tr>\r\n"
	+"		<tr><td><input name=\"doNotIndent_\" type=\"checkbox\" onClick=\"setCheckboxValue(this);\"/>Не использовать отступ (по умолчанию отступ как у первой строки в блоке)</td></tr>\r\n"
	+"		<tr><td><input name=\"addModDateTimeAtEnd_\" type=\"checkbox\" onClick=\"setCheckboxValue(this);\"/>Добавлять дату и время изменения в конец файла</td></tr>\r\n"
	+"	</table>	\r\n"
	+"	<br/>\r\n"
	+"	<input type=\"button\" name=\"btOk\" value=\"Сохранить настройки\" onClick=\"handlers.ok(settings)\" />&nbsp;\r\n"
	+"	<input type=\"button\" name=\"btCancel\" value=\"Отмена\" onClick=\"handlers.cancel()\" />	\r\n"
	+"</form>	\r\n"
	+"</center>\r\n"
	+"\r\n"
	+"<a href=\"http://1c.proclub.ru/modules/mydownloads/personal.php?cid=1004&lid=4915\" target=\"_blank\" title=\"Домашняя страница скрипта на Проклабе\">Посетить домашнюю страницу скрипта на Проклабе</a><br/>\r\n"
	+"<a href=\"mailto:kuntashov@yandex.ru?subject=OC_AuthorJS:\" title=\"Александр Кунташов kuntashov@yandex.ru\">Написать письмо автору скрипта</a></br>\r\n"
	+"<a href=\"http://openconf.itland.ru\" target=\"_blank\" title=\"Домашняя страница проекта &quot;Открытый Конфигуратор&quot;\">Проект &quot;Открытый Конфигуратор&quot;</a><br/>\r\n"
	+"</body>\r\n"
	+"</html>\r\n"
}

