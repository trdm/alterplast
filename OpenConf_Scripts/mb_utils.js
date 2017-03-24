$NAME Metabuilder-Утилиты
//$Id: mb_utils.js,v 1.7 2005/09/07 10:23:43 alest Exp $
//
//Автор Антипин Алексей aka alest (j_alesant@mail.ru)
//Соавторы: 
//  Артур Аюханов aka artbear
//  a13x
//
//распространяется на условиях GPL

//удаляет в концах строк табы и пробелы
//построчная замена с выводом количества исправлений
function cleanEOL(doc) {
	var re = /\s+$/;
	var lines = doc.Text.split("\r\n");
	var corrections = 0;
	var bChangeSel = 0;
	for (var i = 0; i < lines.length; i++) {
		var oldLength = lines[i].length;
		var selStartCol = doc.SelStartCol;
		var selEndCol = doc.SelEndCol;
		lines[i] = lines[i].replace(re, "");
		if (lines[i].length !=  oldLength) {
			doc.range(i, 0, i, oldLength) = lines[i];
			if ((i == doc.SelStartLine) && (selStartCol > lines[i].length)) {
				selStartCol = lines[i].length;
				bChangeSel = 1;
	}
			if ((i == doc.SelEndLine) && (selEndCol > lines[i].length)) {
				selEndCol = lines[i].length;
				bChangeSel = 1;
			}
			//Message("Строка #" + (i+1));
			corrections++;
		}
	}
	if (bChangeSel) {
		doc.MoveCaret(doc.SelStartLine, selStartCol, doc.SelEndLine, selEndCol);
	}
	if (corrections)
		Message("Удаление пробелов и табуляции в конце строк: " + corrections);
}

//вешаем на Ctrl-S
//для модулей перед записью удаляет в концах строк табы и пробелы
function save() {
	var a;
	if (a = Windows.ActiveWnd) {
		if (a = a.Document) {
			if (a == docWorkBook && a.ActivePage == 1) {
				a = a.Page(1);
			}
			if (a == docText) {
				cleanEOL(a);
			}
		}
	    SendCommand(cmdSave);
	}
}

//перезагружает текст файла (просто изменяет текст в открытом окне)
//для случаев, когда файл меняется другим приложением
function reloadFile(doc) {
	if (CommonScripts.FSO.FileExists(doc.Path)) {
		var stream = CommonScripts.FSO.OpenTextFile(doc.Path, 1, false);
		var text = stream.ReadAll();
		stream.Close();
		doc.Text = text;
		doc.Save();
	}
}

//выполняет перезагрузку текста для активного окна
function reloadActiveWnd() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		reloadFile(doc);
	}
}

//выполняет перезагрузку для всех открытых текстовых окон
//после cvs update особенно полезно
function reloadAllFiles() {
    var wnd = Windows.FirstWnd;
    while (wnd) {
        var doc = wnd.Document;
		if (doc == docText) {
			reloadFile(doc);
		}
        wnd = Windows.NextWnd(wnd);
	}
}

function closeActiveWnd() {
	if (Windows.ActiveWnd) {
		Windows.ActiveWnd.Close();
	}
}

//загрузить ГлобальныйМодуль.txt  в мд с помощью glob2md.bat
function glob2md() {
	var wsh = new ActiveXObject("WScript.Shell");
	wsh.CurrentDirectory = IBDir;
	var CmdLine = "glob2md.bat";
	var oExec = wsh.Exec(CmdLine);
	var outputLines = oExec.StdOut.ReadAll().split("\r\n");
	Message("Glob2md " + outputLines[outputLines.length-2]);
}

function start1C() {
	glob2md();
	Run1CApp(rmEnterprise);
}

//копировать в буфер путь открытого в активном окне файла
function copyFilePath() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		CommonScripts.CopyToClipboard(doc.Path);
	}
}

//получить команду для запуска исполняемого файла редактора из реестра
//если соответствующий параметр в реестре не задан, то предлагается
//выбрать нужный редактор (выбор сохраняется в реестре)
function getEditorCmd(defaultEditor) {
	var editor = defaultEditor;
	var ocReg = CommonScripts.Registry;
	if (!ocReg) {
		Message(CommonScripts.GetLastError(), mRedErr);
		return defaultEditor;
	}
	try {
		var rk = ocReg.ScriptRootKey(SelfScript.Name);
		editor = ocReg.Param(rk, "Editor");
		if (!editor) {
			editor = CommonScripts.SelectFileForRead("", "Исполняемые файлы (*.exe)|*.exe");		
			if (editor) {
				ocReg.Param(rk, "Editor") = editor;
			}
		}
	}
	catch (e) {
	}
	return editor ? editor : defaultEditor;
}

//открыть файл активного окна в другом редакторе
function openFileWithEditor() {
	var editor = getEditorCmd("notepad.exe");
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		CommonScripts.RunCommand(editor, doc.Path, 0);
	}
}

//открывает файл в инструкции ЗагрузитьИзФайла(LoadFromFile) и ГлобальныйМодуль.txt (из окна конфигурации и из глоб. модуля)
function openIncludeFile() {
	var a = Windows.ActiveWnd;
	if (!a) {
		SendCommand(cmdOpenConfigWnd);
	}
	else if (a && a.Caption.indexOf("Конфигурация") != -1) {
	    if (!Documents.Open(IBDir + "Modules\\ModuleText\\ГлобальныйМодуль.txt")) {
	    	Documents("ГлобальныйМодуль").Open();
	    }
	}
	else {
		var doc = CommonScripts.GetTextDoc(0);
		if (doc) {
			if (doc.Kind == "ModuleText") {
			    var d = Documents.Open(IBDir + "Modules\\ModuleText\\ГлобальныйМодуль.txt");
				d.MoveCaret(doc.SelStartLine, doc.SelStartCol, doc.SelEndLine, doc.SelEndCol);
			}
			else {
				var firstLine = doc.range(0, "\r\n");
				if ( (firstLine.toUpperCase().indexOf("#ЗагрузитьИзФайла".toUpperCase())  != -1)
				|| (firstLine.toUpperCase().indexOf("#LoadFromFile".toUpperCase()) != -1) ) {
					firstLine = firstLine.replace(/^\s*(\S.*\S)\s*$/, "$1");
					var path = firstLine.substring(18);
					var d = Documents.Open(IBDir + path);
				    if (!d) {
						d = Windows.ActiveWnd.Document;
						if (d == docWorkBook) {
							if (d.ActivePage != 1)
								d.ActivePage = 1;
				    	}
				    }
				}
				else {
					var d = Windows.ActiveWnd.Document;
					if (d == docWorkBook) {
						if (d.ActivePage != 1)
							d.ActivePage = 1;
			    	}
				}
			}
		}
	}
}

//комментировать текст 
//в отличие от типового ставит комментарий, 
//даже если ничего не выделено или выделен текст на одной строке
function commentSelection() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		var sCommentSymbol = "//";
		//если ничего не выделено или выделен текст на одной строке
		if (doc.SelStartLine == doc.SelEndLine) {
			var str = doc.Range(doc.SelStartLine);
			var pos = 0;	//позиция символов комментария
			var selection = {startCol: doc.SelStartCol, endCol: doc.SelEndCol};
			if (str) {
				var matches = str.match(/^(\s*)(\S.+)/);
	            if (matches.length) {
					doc.Range(doc.SelStartLine) = matches[1] + sCommentSymbol + matches[2];
					pos = matches[1].length;
        		}
        	}
			else {//если пуста, комментарий ставлю в начале
				doc.Range(doc.SelStartLine) = sCommentSymbol + doc.Range(doc.SelStartLine);
			}
			if (pos <= doc.SelStartCol) {
				selection.startCol += sCommentSymbol.length;
			}
			if (pos <= doc.SelEndCol) {
				selection.endCol += sCommentSymbol.length;
			}
			doc.MoveCaret(doc.SelStartLine, selection.startCol, doc.SelStartLine, selection.endCol);
		}
       	else {
			doc.CommentSel();
		}
	}
}
         
//раскомментировать текст 
//в отличие от типового ставит комментарий, 
//даже если ничего не выделено или выделен текст на одной строке
function unCommentSelection() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		var sCommentSymbol = "//";
		//если ничего не выделено или выделен текст на одной строке
		if (doc.SelStartLine == doc.SelEndLine) {
			var selection = {startCol: doc.SelStartCol, endCol: doc.SelEndCol};
			var str = doc.Range(doc.SelStartLine);
			var pos = str.indexOf(sCommentSymbol); 	//позиция символов комментария
			if (pos != -1) {
				doc.Range(doc.SelStartLine) = str.substring(0, pos) + str.substring(pos+sCommentSymbol.length);

				if (pos < selection.startCol) {
					selection.startCol -= (sCommentSymbol.length - Math.max(pos + sCommentSymbol.length - selection.startCol, 0));
				}
				if (pos < selection.endCol) {
					selection.endCol -= (sCommentSymbol.length - Math.max(pos + sCommentSymbol.length - selection.endCol, 0));
				}
				doc.MoveCaret(doc.SelStartLine, selection.startCol, doc.SelStartLine, selection.endCol);
			}
	    }
		else {
			doc.UnCommentSel();
		}
	}
}

var PositionInModule;
//получить следующий номер строки, соответствующую регэкспу
function getNextLineMatch(lines, re, curPosition) {
	for (var i = curPosition; i < lines.length; i++) {
		var str = lines[i].toUpperCase();
		if (str.search(re) != -1) {
			return i;
		}
	}
	return -1;
}

//получить предыдующий номер строки, соответствующую регэкспу
function getPrevLineMatch(lines, re, curPosition) {
	for (var i = curPosition; i >= 0; i--) {
		var str = lines[i].toUpperCase();
		if (str.search(re) != -1) {
			return i;
		}
	}
	return -1;
}

function getFunctionStart(text, curPosition) {
	return getPrevLineMatch(text.split("\r\n"), /^\s*ФУНКЦИЯ|^\s*ПРОЦЕДУРА/, curPosition);
}

function getFunctionEnd(text, curPosition) {
	return getNextLineMatch(text.split("\r\n"), /^\s*КОНЕЦФУНКЦИИ|^\s*КОНЕЦПРОЦЕДУРЫ/, curPosition);
}

function gotoFunctionStart() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		PositionInModule =  CommonScripts.GetDocumentPosition(doc);
		var i = getFunctionStart(doc.Text, doc.SelStartLine);
		if (i >= 0) {
			doc.MoveCaret(i, 0);
		}
	}
}

function gotoFunctionEnd() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		PositionInModule =  CommonScripts.GetDocumentPosition(doc);
		var i = getFunctionEnd(doc.Text, doc.SelStartLine);
		if (i >= 0) {
			doc.MoveCaret(i, 0);
		}
	}
}

function gotoSavedPosition() {
	PositionInModule.Restore();
}

function selectCurrentFunction() {
	var doc = CommonScripts.GetTextDocIfOpened(0);
	if (doc) {
		doc.MoveCaret(getFunctionStart(doc.text, doc.selStartLine), 0, getFunctionEnd(doc.Text, doc.selStartLine)+1, 0);
	}
}

function init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
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

init(0);
