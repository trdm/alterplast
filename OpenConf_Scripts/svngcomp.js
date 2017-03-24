$NAME svn + gcomp
//$Id: svngcomp.js,v 1.2 2007/04/14 20:02:39 alest Exp $
//
//Автор Антипин Алексей aka alest (j_alesant@mail.ru)
//
//распространяется на условиях GPL

// декомпилирует всю конфу, если нет активного окна, или запуск из окна конфигурации
//	или текущий объект
// путь к gcomp указать свой
// исправить, если нужно папку, в которую разбирать мд
function decompile() {
	var gcompPath = 'cmd /U /C D:\\work\\1c\\gcomp.exe';
	var srcMDDir = 'SRC\\md';
	var _gcomp_ini  = ' --no-profiles --no-empty-mxl --no-empty-folders --no-broken-links '
	var params = ' -d -F "' + CommonScripts.FSO.buildPath(CommonScripts.IBDir(), '1cv7.md')
		+ '" -D "' + CommonScripts.FSO.buildPath(CommonScripts.IBDir(), srcMDDir) + '"'
		+ _gcomp_ini;

	cmdPath = gcompPath;
	
	var a = Windows.ActiveWnd;
/*	var doc = a.Document;
	CommonScripts.Echo('ID=' + doc.ID);
	CommonScripts.Echo('Kind=' + doc.Kind);
	CommonScripts.Echo('Path=' + doc.Path);
	CommonScripts.Echo('Name=' + doc.Name);
	CommonScripts.Echo('Type=' + doc.Type);
	CommonScripts.Echo('***************************************************');
*/
	if (!a || (a && a.Caption.indexOf('Конфигурация') != -1)) {
		CommonScripts.RunCommand(gcompPath, params, 0);
	}
	else {
		var doc = a.Document;
		if (doc) {
		
			var fStr = '';
			if (doc.Type == 0) {
				//Name=CMDDocDoc::Документ АктСписания
				fStr = doc.Name;
				if (fStr.indexOf('CMDDocDoc') != -1) {
					var re = /(.+)(Документ)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				//Name=CMDSubDoc::Справочник ДенежныеДокументы
				else if (fStr.indexOf('CMDSubDoc') != -1) {
					var re = /(.+)(Справочник)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				//Name=CMDJournalDoc::Журнал Отклонения
				else if (fStr.indexOf('CMDJournalDoc') != -1) {
					var re = /(.+)(Журнал)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				
			}
			else if (doc.Kind == 'ModuleText') {
				fStr = 'ГлобальныйМодуль';
			}
			else if (doc.Kind == 'Transact') {
				//Name=Документ.АктВРреализСчет.Модуль Документа
				//Документ\АктВРреализСчет\МодульПроведения
				var re = /(Документ)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2\\МодульПроведения");
			}
			else if ((doc.Kind == 'Document') && (doc == docWorkBook)) {
				//Name=Документ.АктВРреализСчет.Форма
				//Документ\АктВРреализСчет
				var re = /(Документ)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
				CommonScripts.Echo(fStr);
			}
			else if (((doc.Kind == 'Subconto') || (doc.Kind == 'SubFolder') || (doc.Kind == 'SubList'))
			&& (doc == docWorkBook)) {
				//Name=Справочник.ДоговораПростогоТоварищества.Форма
				//Документ\АктВРреализСчет
				var re = /(Справочник)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
			}
			else if (doc.Kind == 'Journal') {
				//Name=Журнал.Страхование.Форма.ФормаСписка
				//Журнал\Страхованние
				var re = /(Журнал)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
			}
			else if ((doc.ID == -1) && (doc.Type == 1)) {
				//Name=CWorkBookDoc::Внешний отчет(обработка) - D:\work\1c\Clients\Pinskdrev\Офис\dev\ExtForms\Тов2.ert
				//Path=D:\work\1c\Clients\Pinskdrev\Офис\dev\ExtForms\Тов2.ert
				//в скрипт комбинация \" попадает неверно- парсер питоновый считает, что это ескейп последовательность
				cmdPath = 'cmd /U /C decompile_ert.py';
				params = '-b "' + CommonScripts.FSO.GetAbsolutePathName(CommonScripts.IBDir()) + '" -f "' + doc.Path + '"';
			}
			
			CommonScripts.RunCommand(cmdPath, params + (fStr ? (' --filter ' + fStr) : ''), 0);
		}
	}
}


function init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{
	try {
		var ocs = new ActiveXObject('OpenConf.CommonServices');
		ocs.SetConfig(Configurator);
		SelfScript.AddNamedItem('CommonScripts', ocs, false);
	}
	catch (e) {
		Message('Не могу создать объект OpenConf.CommonServices', mRedErr);
		Message(e.description, mRedErr);
		Message('Скрипт ' + SelfScript.Name + ' не загружен', mInformation);
		Scripts.UnLoad(SelfScript.Name);
	}
}

init(0);
