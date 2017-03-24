$NAME Многострочный текст

/*===========================================================================
Скрипт:  MultiString.js
Версия:  $Revision: 1.1 $
Автор:   Александр Кунташов
E-mail:  kuntashov at [yandex.ru|gmail.com]
ICQ UIN: 338758861
Описание: 
	Макросы для вставки из буфера/копирования в буфер многострочных 
	строковых констант из модулей 1С с добавлением/удалением символов признака
	новой строки (|) и заменой одинарных двойных кавычек (") на парную ("") 
	и обратно.
===========================================================================*/

function ВставитьИзБуфераОбмена() {

	var doc	= CommonScripts.GetTextDocIfOpened();	
	var str = CommonScripts.GetFromClipboard();
	
	if (doc && str) {
		var m, ind = "";
		/* для остальных строк многострочного текста используем 
			отступ строки, в которой находится курсор */
		if (m=doc.Range(doc.SelStartLine).match(/^(\s+)/)) {
			ind = m[1];
		}
		doc.Range(
			doc.SelStartLine, doc.SelStartCol, 
			doc.SelEndLine, doc.SelEndCol) = str.replace(/\n/g, "\n"+ind+"\|").replace(/\"/g,'""');
	}
}

function КопироватьВБуферОбмена() {

	var doc	= CommonScripts.GetTextDocIfOpened();	

	if (doc) {
		var str = doc.Range(doc.SelStartLine, doc.SelStartCol, doc.SelEndLine, doc.SelEndCol);
		if (str) {
			CommonScripts.CopyToClipboard(str.replace(/^\s*\|/gm, "").replace(/\"\"/g, '"'));
		}
	}	
}

/*
 * процедура инициализации скрипта
 */
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

