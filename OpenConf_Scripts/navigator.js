$NAME Навигатор

/*===========================================================================
Copyright (c) 2004 Alexander Kuntashov
=============================================================================
Скрипт:  navigator.js ("Навигатор")
Версия:  1.2
Автор:   Александр Кунташов
E-mail:  kuntashov at yandex dot ru
ICQ UIN: 338758861
Описание: 
	Скрипт "Навигатор" для OpenConf предназначен для быстрого доступа к объектам
матаданных конфигурации или объектам файловой системы, задаваемых в тексте модуля
полным (абсолютным) или относительным (только для путей файловой системы) путем. 
===========================================================================*/

/* сообщение об ошибке */
function error(str)
{
	with (new ActiveXObject("WScript.Shell"))
		Popup(str, 0, SelfScript.Name, 0|48);
}

/* возвращает объект TextDoc, если текущим открытым документом является
	текстовый документ, в противном случае - null */
function getTextDoc(_)
{
	var win, doc;

	if (!(win = Windows.ActiveWnd))
		return null;

	if ((doc = win.Document) == docWorkBook)
		doc = doc.Page(1);

	return (doc == docText)?doc:null;
}

/* интерпретируя строку под курсором как путь, открывает его
	если openDialog - true, то для внутренних объектов конфигурации и
	внешних обработок/отчетов при открытии активизирует страницу "Диалог",
	в противном случае - страницу "Модуль" */
function openPath(openDialog)
{
	var doc, cl, re, path, m, kind, dot;
	
	if(!(doc = getTextDoc()))
		return;

	cl = doc.Range(doc.SelStartLine);   

	re = /[.#\wА-Я\\:\/]/i;

	path = "";

	if (doc.SelStartCol != doc.SelEndCol)
		path = cl.substring(doc.SelStartCol, doc.SelEndCol);
	else {
		for (var pos=doc.SelStartCol; 
				(pos>0) && cl.charAt(pos).match(re);)          
			path = cl.charAt(pos--) + path;       
	
		for (pos=doc.SelStartCol+1; 
				(pos < cl.length) && cl.charAt(pos).match(re);)        
			path += cl.charAt(pos++);   
	}

	try {
		if (path.match(/\\|\//)) {
			openFileSystemPath(path, openDialog);
		}
		else {
			if (!openMetaDataPath(path, openDialog)) {
				error('Не могу открыть объект: ' + path);
			}
	   }
	} 
	catch (e) {
		error(e.description);
	}  
}

/* открывает внешний файл/папку */
function openFileSystemPath(p, openDialog)
{
	var path;
		   
	path = p.match(/^\w:/)?p:CommonScripts.ResolvePath(IBDir, p);            

	with(new ActiveXObject("Scripting.FileSystemObject")) {               
		if (FileExists(path)) {
			openFile(path, openDialog);                            
		} 
		else if (FolderExists(path)) {
			openFolder(path);                                    
		} 
		else
			error('Файл "' + path + '" не существует!');
	}  
}

/* открывает внешний файл */
function openFile(path, openDialog)
{
	var ext;
	   
	with (new ActiveXObject("Scripting.FileSystemObject")) 
		ext  = GetExtensionName(path).toLowerCase();       
	 
	switch (ext) {
		case 'txt': case 'ert': case 'mxl':
		case '1s':  case '1c':  case '1с':
			/* открываем файл в конфигураторе */
			openFileInside(path, openDialog);
		break;
		/* по умолчанию пытаемся открыть файл 
		   ассоциированной с файлами этого типа программой */
		default:
			openFileOutside(path, ext);
	}
}

/* открывает внешний файл в конфигураторе */
function openFileInside(path, openDialog)
{
	with (Documents.Open(path)) 
		if ((Type == docWorkBook)&&(!openDialog))             
			ActivePage = 1;                                 
}

/* открывает внешний файл с помощью программы, ассоциированной
	в системе с соответствующим типом файлов */
function openFileOutside(path, ext)
{
	var key, cmdTmpl, cmd;
	with (new ActiveXObject("WScript.Shell")) {
		try {            
			key     = RegRead("HKCR\\." + ext + "\\");                    
			cmdTmpl = RegRead("HKCR\\" + key + "\\shell\\open\\command\\");            

			cmd = cmdTmpl.replace(/%1/, path);
			if (cmd == cmdTmpl)
				cmd = cmdTmpl + ' ' + path;                        

			Run(cmd);
		} 
		catch(e) {
			//error(e.description);
			error("Для открытия файла этого типа " + 
				"не сопоставлено никакой программы!");
		}        
	}
}

/* открывает папку в Проводнике */             
function openFolder(path)
{
	with (new ActiveXObject("WScript.Shell"))
		Run("%SYSTEMROOT%\\explorer.exe " + path);
}

/* проверяет существование объекта конфигурации 
   с типом type и видом name */
function findMDO(type, name)
{
	var obj, mdObjs = MetaData.TaskDef.Childs;    
	for (var i = 0; i < mdObjs(type).Count; i++) {
		if (mdObjs(type)(i).Name.toUpperCase() == name.toUpperCase()) {
			return mdObjs(type)(i);
		}
	}    
	return  null;
}

/* спрашивает пользователя, какую форму 
   справочника следует открыть */
function askRefForm(name)
{    
	var ref, forms = new Array();
	
	if (!(ref = findMDO("Справочник", name))) {         
		return null;
	}
		
	for (i = 0; i < ref.Childs("ФормаСписка").Count; i++) {
		forms[i] = "ФормаСписка." + ref.Childs("ФормаСписка")(i).Name;
	}

	with (new ActiveXObject("Svcsvc.Service"))
		return SelectValue("Форма\r\n" + "Форма группы\r\n" 
				+ ((forms.length>0)?forms.join("\r\n"):""), 
				'Формы - Справочник.' + name);
}

/* спрашивает пользователя, какую форму списка 
   журнала документов следует открыть */
function askJournalForm(name)
{        
	var jrn, forms = new Array();
	
	if (!(jrn = findMDO("Журнал", name))) {         
		return null;
	}
	   
	for (i = 0; i < jrn.Childs("ФормаСписка").Count; i++) {
		forms[i] = jrn.Childs("ФормаСписка")(i).Name;
	}

	if (forms.length == 0) {
		return null;
	}
	if (forms.length == 1) { 
		// форма только одна, спрашивать не надо
		return forms[0];
	}
	
	with (new ActiveXObject("Svcsvc.Service"))
		return SelectValue(forms.join("\r\n"), 'Формы списка - Журнал' + name);
}

/* спрашивает пользователя, открывать для 
   документа модуль формы или модуль проведения */
function askDocModule(name) 
{
	var a = new Array("Модуль документа", "Форма.Модуль"); 

	with (new ActiveXObject("Svcsvc.Service"))
		return SelectValue(a.join("\r\n"), "Модули - Документ." + name);
}

/* открывает форму/модуль для объекта метаданных */
function openMetaDataPath(p, openDialog)
{
	var m, dot, path, tmp;

	/* для повышения читабельности нижеследующего кода */    
	dot = new Function('s', 'return "." + s;');

	switch ((m = p.split('.')).length) {
		case 0: 
			return false;
		case 1:
			if (m[0].toUpperCase() == 'ОПЕРАЦИЯ')
				break;            
			if (findMDO("Справочник", m[0])) {
				m = new Array("Справочник", m[0]);
			}
			else if (findMDO("Документ", m[0])) {
				m = new Array("Документ", m[0]);
			}
			else if (findMDO("Журнал", m[0])) {
				m = new Array("Журнал", m[0]);
			}
			break; 
		default:
			// do nothing :-)
	}
		
	switch (m[0].toUpperCase()) {                
		/*---------------------------------------------------------- 
			(Элемент|Справочник).Вид / Справочник.Вид.ИмяФормыСписка  
		----------------------------------------------------------*/
		case 'СПРАВОЧНИК': case 'ЭЛЕМЕНТ':                                                       
			if (!(tmp = (m.length == 2)?askRefForm(m[1]):("ФормаСписка." + m[2]))) {
				// пользователь отменил открытие модуля/диалога формы
				return true; 
			}
			path = 'Справочник' + dot(m[1]) + dot(tmp);             
		break;
		/*---------------------------------------------------------- 
			Документ.ВидДокумента
		----------------------------------------------------------*/
		case "ДОКУМЕНТ": 
		//	tmp = "Форма.Диалог";
		//	if (!openDialog) {
		//		if (!(tmp = askDocModule(m[1]))) {               
		//			// пользователь отменил открытие модуля/диалога формы
		//			return true;
		//		}               
		//	}
			tmp = ((getTextDoc().Name.toUpperCase().search('МОДУЛЬ ДОКУМЕНТА') !== -1)?(openDialog?'Форма.Диалог':'Форма.Модуль'):'МОДУЛЬ ДОКУМЕНТА')
			path = m[0] + dot(m[1]) + dot(tmp);
		break;        
		/*---------------------------------------------------------- 
			(Отчет|Обработка).Идентификатор
		----------------------------------------------------------*/
		case "ОТЧЕТ": case "ОБРАБОТКА":
			path = m[0] + dot(m[1]) + dot("Форма");
		break;
		/*---------------------------------------------------------- 
			Журнал.ИдЖурнала.ИдФормыЖурнала
		----------------------------------------------------------*/
		case "ЖУРНАЛ":
			if (m[1].toUpperCase() == "ПОДЧИНЕННЫЕ") {
				// этот путь мы открыть не можем пока точно, 
				// но и молчать не будем, чтобы не вводить никого
				// в заблуждение
				return false; 
			}
			if (!(tmp = (m.length == 2)?askJournalForm(m[1]):m[2])) {
				// пользователь отменил открытие модуля/диалога формы
				return true;
			}                                     
			path = 'Журнал' + dot(m[1]) + dot('Форма') + dot(tmp);
		break;
		/*---------------------------------------------------------- 
			ЖурналРасчетов.ВидЖурналаРасчетов(.ИмяФормыСписка)*
		----------------------------------------------------------*/
		case "ЖУРНАЛРАСЧЕТОВ":
			path = 'ЖурналРасчетов' + dot(m[1]) + dot('Форма') + 
				dot((m.length == 3)?m[2]:'ФормаСписка') + dot('Форма');
		break;
		/*---------------------------------------------------------- 
			Счет.ИмяПланаСчетов
		----------------------------------------------------------*/
		case "СЧЕТ":
			path = 'ПланСчетов' + dot('Форма');
		break;
		/*---------------------------------------------------------- 
			ПланСчетов.ИдПланаСчетов.ИдФормыСписка
		----------------------------------------------------------*/
		case "ПЛАНСЧЕТОВ":
		  path = 'ПланСчетов' + dot('ФормаСписка') + dot(m[2]) + dot('Форма');
		break;  
		/*---------------------------------------------------------- 
			Операция
		----------------------------------------------------------*/
		case "ОПЕРАЦИЯ":           
			path = 'Операция' + dot('Форма');
		break;
		/*---------------------------------------------------------- 
			ЖурналОпераций.ИдФормыЖурналаОпераций
		----------------------------------------------------------*/
		case "ЖУРНАЛОПЕРАЦИЙ":
			path = 'Операция.ФормаСписка' + dot(m[1]) + dot('Форма');
		break;
		/*---------------------------------------------------------- 
			ЖурналПроводок.ИдФормыЖурналаПроводок
		----------------------------------------------------------*/
		case "ЖУРНАЛПРОВОДОК":
			path = 'Проводка' + dot('ФормаСписка') + dot(m[1]) + dot('Форма');            
		break;
		/*---------------------------------------------------------- 
			История.Константа.ИдКонстанты, 
			История.Справочник.ВидСправочника.ИдПериодичРеквизита
			История.Счет
		----------------------------------------------------------*/
		case "ИСТОРИЯ":
			/* пока не знаю (не искал), 
				как можно открыть формы истории в конфигураторе */
			error("Извините, пока не реализовано. Путь: " + m.join('.'));
		//break;        
		/*---------------------------------------------------------- 
			Все остальные конструкции игнорируем
		----------------------------------------------------------*/
		default:        
			return;
	}

	if (m[0].toUpperCase() !== "ДОКУМЕНТ") {
		path += dot(openDialog?'Диалог':'Модуль');
	}
 
	Documents(path).Open();           
	return true;
}

/* Для текущго открытого модуля объекта (модуля одной из 
	форм объекта или модуля проведения документа, в случае,
	когда рассматриваемый объект является документом, предлагает 
	пользователю выбрать из списка и открыть любой другой доступный 
	для этого объекта модуль или форму */
function openAnotherModuleOrFormForCurrentObject(openDialog)
{
	var doc, m;       

	if (!(doc = Windows.ActiveWnd))
		return null;

	doc = doc.Document;
	
	m = doc.Name.split('.');

	switch (m[0].toUpperCase()) {
		case 'ДОКУМЕНТ':
		case 'СПРАВОЧНИК':
		case 'ЖУРНАЛ':
			openMetaDataPath(m[0] + '.' + m[1], openDialog);    
			break;
		default:
			; // do nothing
	}
}

//===========================================================================
//                          user interface :-)
//===========================================================================

/* макросы для установки шорткатов */

// Ctrl + Shift + M[odule]
function ОткрытьМодульДляОбъектаПодКурсором()
{
	openPath();
}

// Ctrl + Shift + F[orm]
function ОткрытьФормуДляОбъектаПодКурсором()
{
	openPath(true);
}

// Alt + Shift + M[odule]
function ОткрытьДругойМодульДляТекущегоОбъекта()
{
	openAnotherModuleOrFormForCurrentObject();
}

// Alt + Shift + F[orm]
function ОткрытьДругуюФормуДляТекущегоОбъекта()
{
	openAnotherModuleOrFormForCurrentObject(true);
}

/* добавляем соответствующие пункты в контекстное меню (см. подпункт в 
	пункте "Шаблоны" - пока только так) при помощи Телепата */
function TelepatGetMenu(_)
{
	var menu = "Открыть модуль...| |__NavigatorJSОткрытьМодуль\r\n" + 
			   "Открыть форму... | |__NavigatorJSОткрытьДиалог\r\n";        
	return menu;
}

/* обработчик выбора одного из пунктов нашего меню */
function TelepatOnCustomMenu(cmd)
{
	if (cmd == '__NavigatorJSОткрытьДиалог')
		openPath(true);
	if (cmd == '__NavigatorJSОткрытьМодуль')
		openPath(false);
}

//===========================================================================

/* так обходится особенность работы JScript'а с 
	подключением обработчиков событий объектов, добавляемых
	в пространство имен скрипта run-time -- a13x */ 
function addTelepatEventHandler(handler)
{
	eval('function Telepat::' + handler + '{ return Telepat' + handler + '}');
}

/*
 * процедура инициализации скрипта
 */
function Init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{
	try {
		var t, ocs = new ActiveXObject("OpenConf.CommonServices");
		ocs.SetConfig(Configurator);
		SelfScript.AddNamedItem("CommonScripts", ocs, false);

		/* Добавляем объект Телепат в пространство имен скрипта */
		
		ocs.ClearError();
		ocs.SetQuietMode(true);		
		ocs.AddPluginToScript(SelfScript, "Телепат", "Telepat", t);		
   		ocs.SetQuietMode(false);				
		
		if (ocs.GetLastError()) {	
			var warnUser = true, OCReg = ocs.Registry;
			if (OCReg !== null) {
				var srk = OCReg.ScriptRootKey(SelfScript.Name);				
				if (OCReg.Param(srk, "DoNotWarnOnStartUp") == "true") {
					warnUser = false;
				}
				else {
					// при следующем запуске конфигуратора сообщение не должно появиться
					OCReg.Param(srk, "DoNotWarnOnStartUp") = "true";
				}
			}	
			if (warnUser) {
				Message(ocs.GetLastError(), mRedErr);			
				Message("Возможно, у Вас не установлен плагин Телепат 2");
				Message("Вызов макросов скрипта Навигатор из контекстного меню будет недоступен!");
			}
		}
		else {
			/* подключаем методы нужные нам обработчики событий Телепата */
			
			//Вызов меню шаблонов
			addTelepatEventHandler("GetMenu()");

			//Выбор пункта меню шаблонов
			addTelepatEventHandler("OnCustomMenu(cmd)");
		}	

	}
	catch (e) {
		Message("Не могу создать объект OpenConf.CommonServices", mRedErr);
		Message(e.description, mRedErr);
		Message("Скрипт " + SelfScript.Name + " не загружен", mInformation);
		Scripts.UnLoad(SelfScript.Name); 		
	}	
}

//===========================================================================
		 
/* Поехали... */
Init();                                                     
