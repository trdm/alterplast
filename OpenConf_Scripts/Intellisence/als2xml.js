/* ============================================================================ *\
Скрипт:  als2xml.js
Версия:  $Revision: 1.8 $
Автор:   Александр Кунташов aka a13x
E-mail:  <kuntashov@yandex.ru>
ICQ UIN: 338758861
Описание: 
	Скрипт для генерации на основе als-файлов синтаксис-помощника 1С:Предприятия
xml-файлов описания типов для Телепата.
\* ============================================================================ */

var OpenConf = false; // true - доступна мы в окружении OpenConf'а, иначе - WSH

var wsh = new ActiveXObject("WScript.Shell");
var fso = new ActiveXObject("Scripting.FileSystemObject");

/* ============================================================================ *\
   								"ОБЫЧНЫЙ" СКРИПТ
\* ============================================================================ */

// true - "тихий" режим (не выводится никаких сообщений), false - обычный режим
var Silent = false;

function usage(_)
{
	return ( 
'Преобразователь als-файлов в xml в формате утилиты xml2tls.exe Александра Орефкова\n\
\n\
Использование:\n\
cscript.exe //nologo als2xml.js /ALS:ИмяALSФайла [опциональные параметры]\n\
\n\
Параметры:\n\
/ALS:<Путь и имя ALS-файла> - обязательный\n\
	Путь/имя исходного als-файла\n\
\n\
/XML[:<Путь и имя XML-файла>] - необязательный\n\
	Путь и имя генерируемого xml-файла. По умолчанию xml-файл создается \n\
	с тем же именем, что и исходный als (и в том же каталоге), но с\n\
	расширением "xml".\n\
\n\
/TLS[:<Путь и имя TLS-файла>] - необязательный\n\
	Автоматически вызвать утилиту xml2tls.exe для генерации\n\
	на основе полученного xml-файла tls-файла для Телепата.\n\
	xml2tls должна находиться в одном каталоге со скриптом, либо\n\
	в одном из каталогов, прописанных в переменной окружения PATH.\n\
	Если имя и путь файла не заданы, то файл сохраняется в каталоге\n\
	с исходным als-файлом с тем же именем, но с расширением "tls".\n\
\n\
/INTS:<Каталог назначения> - необязательный\n\
	Сгенерировать ints-файлы для скрипта Intellisence.vbs, сохраняя\n\
	их в указанном каталоге. Имена файлов будут совпадать с именами\n\
	типов. Также будет создан файл СоздатьОбъект.ints, в который будут\n\
	записаны имена типов в том же формате, что используется в одноименном\n\
	служебном файле скрипта Intellisence.vbs.\n\
	ВНИМАНИЕ. Не рекомендуется в качестве каталога назначения использовать\n\
	каталог <КаталогИБ>\\config\\Intell, т.к. файлы генерируются на основе\n\
	данных в als-файлах и названия типов в них не всегда соответствуют\n\
	progid-у объекта.\n\
\n\
/NOXML - необязательный\n\
	Не сохранять xml-файл. Имеет смысл использовать совместно с\n\
	опцией /TLS. Если не задавать эту опцию, то вместе с tls-файлом\n\
	будет сохранен и сгенерированный xml-файл.\n\
\n\
/JS[:<Имя и путь к js-файлу>] - необязательный\n\
	Сохранить промежуточный результат парсинга ALS-файла (для отладки).\n\
	Если имя и путь файла не заданы, то файл сохраняется в каталоге\n\
	с исходным als-файлом с тем же именем, но с расширением "js".\n\
\n\
/PREFIX[:<Префикс>] - необязательный\n\
	Использовать префикс <Префикс> для идентификаторов типов,\n\
	извлекаемых из обрабатываемого als-файла. Если ключ задан без\n\
	параметра <Префикс>, то в качестве префикса используется имя\n\
	исходного als-файла (без расшинения).\n\
\n\
/INDENT[:<КоличествоОтступов>] - необязательный\n\
	Форматировать генерируемый XML-код отступами (по иерархии,\n\
	для отступов используются пробелы). Необязательный числовой\n\
	параметр <КоличествоОтступов> задает грубину отступа от начала\n\
	строки, по умолчанию используется значение 2.\n\
\n\
/KEYWORDS:<ИмяФайла> - необязательный\n\
	Сохранить все имена методов и атрибутов в файл <ИмяФайла>. Если задан,\n\
	то все остальные опции по сохранению результата парсинга (/XML, /INTS \n\
	и т.п.) будут проигнорированы.\n\
\n\
/SILENT - необязательный\n\
	Не выводить никаких сообщений.');
}

function xml2tls(xmlFile, tlsFile) 
{
	var xml2tls_exe = "";
	
	if (OpenConf)
		xml2tls_exe= fso.BuildPath(BinDir, "config\\system\\xml2tls\\xml2tls.exe");
	
	var cmd = 'xml2tls.exe "' + xmlFile + '" "' + tlsFile + '"';
	
	if (fso.FileExists(xml2tls_exe)) {
		cmd = cmd.replace(/^xml2tls\.exe/, '"' + xml2tls_exe + '"')
	}
	
	try {
		return (0 == wsh.Run(cmd, 7, true));
	}
	catch (e) {
		msg("Ошибка запуска xml2tls: возможно, утилита не прописана в PATH.");
	}
}

function makePath(alsFile, ext)
{
	return fso.BuildPath(wsh.CurrentDirectory, fso.GetBaseName(alsFile) + '.' + ext);
}

function Run(_) 
{
	var prefix="", indent="";
	var Args = WScript.Arguments.Named;
	
	if (!Args.Count || Args.Exists("?") || Args.Exists("HELP") || Args.Exists("H")) {
		msg(usage());
		exit(1);
	}
	
	if (!Args.Exists("ALS")) {
		msg(usage());
		exit(1);
	}
		
	var alsFile, xmlFile, tlsFile, jsFile, kwFile;
	var intsFolder = null;

	var noXML = Args.Exists("NOXML");
	Silent = Args.Exists("SILENT");

	alsFile = Args.Item('ALS');
	if (!fso.FileExists(alsFile)) {
		msg("Указанный als-файл не существует!");
		exit(1);
	}

	if (Args.Exists("PREFIX")) {
		prefix = Args.Item("PREFIX");
		if (!prefix) {
			prefix = fso.GetBaseName(alsFile);
		}
		// имена файлов могут быть с пробелами
		prefix = makeValidId(prefix);
	}	

	if (Args.Exists("INTS")) {
		intsFolder = Args.Item("INTS");
		if (!fso.FolderExists(intsFolder)) {
			msg("Указанный каталог для генерации ints-файлов не существует!");
			exit(1);
		}
	}
	
	if (Args.Exists("KEYWORDS")) {
		kwFile = Args.Item("KEYWORDS");
	}

	// XXX если не задан /XML и задан /NOXML, то xml-файл создавать в temp'е (?)

	if (Args.Exists("XML") && !Args.Item("XML")) {
		xmlFile = Args.Item("XML");
	}
	else {
		xmlFile = makePath(alsFile, "xml");
	}
	
	if (Args.Exists("TLS")) {
		if (Args.Item("TLS")) {
			tlsFile = Args.Item("TLS");
		}
		else {
			tlsFile = makePath(alsFile, "tls");
		}
	}
	
	if (Args.Exists("JS")) {
		if (Args.Item("JS")) {			
			jsFile = Args.Item("JS");
		}
		else {
			jsFile = makePath(alsFile, "js");
		}
	}		

	if (Args.Exists("INDENT")) {
		indentSize = Args.Item("INDENT");
		if (!indentSize) {
			indentSize = 2;
		}
		for ( ; indent.length<indentSize; indent+=" ")
			; // Кто-то считает это дурным тоном, но мы не из таких :-) -- a13x
	}	

	msg("Парсинг исходного als-файла: " + alsFile);
	var tree = parseAls(alsFile, jsFile);

	if (tree) {			

		msg("Подготовка извлеченных данных...");
		var types = processShell(tree);
		
		if (kwFile) {
			saveKeywords(types, kwFile);
			msg("Ключевые слова извлечены в файл: " + kwFile);
			exit(0);
		}

		if (!noXML||tlsFile) {
			msg("Герерация XML-файла...");
			saveAsXML(types, xmlFile, prefix, indent);
		}
	
		var ret = false;
		if (tlsFile) {
			msg("Компиляция XML-файла...");
			ret = xml2tls(xmlFile, tlsFile);
		}

		if (noXML && fso.FileExists(xmlFile)) {
			msg("Удаление временных файлов...");
			fso.DeleteFile(xmlFile, true);
		}

		if (intsFolder) {
			msg("Генерация ints-файлов...");
			saveAsInts(types, intsFolder, prefix);
		}

		msg("---------------------------------------");
		
		if (!noXML)			msg("XML: " + xmlFile);
		if (tlsFile && ret)	msg("TLS: "	+ tlsFile);
		if (jsFile)			msg("JS:  " + jsFile);		

	}
	else {
		exit(1);
	}
}

/* ============================================================================ *\
   								СКРИПТ ДЛЯ OPENCONF
\* ============================================================================ */

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

function КонвертироватьALS() 
{
	var alsFile = CommonScripts.SelectFileForRead(BinDir + "*.als", "Файлы синтаксис-помощника (*.als)|*.als|Все файлы|*");
		
	if (alsFile) {		
		var xmlFile = fso.BuildPath(fso.GetSpecialFolder(2), fso.GetTempName() + ".xml");
		
		msg("Конвертируется als-файл: " + alsFile);
		var tree = parseAls(alsFile);

		if (tree) {			

			var tlsFile = alsFile.replace(/als$/, "tls");
			
			msg("Подготовка извлеченных данных...");
			var types = processShell(tree);
			
			msg("Герерация XML-файла...");
			saveAsXML(types, xmlFile);
	
			var ret = false;
			msg("Компиляция XML-файла...");
			ret = xml2tls(xmlFile, tlsFile);			
			
			msg("Удаление временных файлов...");
			fso.DeleteFile(xmlFile, true);

			if (ret)
				msg("Готово!");
			else
				msg("Файл не сгенерирован", mRedErr);
		}
	}
}

/* ============================================================================ *\

Если кто-то задумает разбираться с кодом, то это описание может помочь, хотя 
основная его часть была написана до написания скрипта как попытка представить
себе, как это все должно работать, и лишь после получения первой работоспособной
версии преобразована к некому подобию техдока. -- a13x

ALS-файл представляет собой текстовый файл следующего формата: 

{"Shell",
 {"Folder","AST","ИмяОбъекта","ObjectName",
    {"Item","AST","ИмяСвойства","PropertyName",
		"ИмяСвойства","PropertyName","Многострочный текст описания свойства"},
    {"Item","AST","ИмяМетода","MethodName",
		"ИмяМетода()","MethodName()","Многострочный текст описания метода"},
	...
 },
 ...
}

Элементы типа Folder могут иметь другие вложенные элементы типа Folder (не 
ограничено по количеству уровней вложенности).

Алгоритм преобразования следующий:

ШАГ 1. Преобразование als-файла в js-массив. 
--------------------------------------------

Для преобразования используем технику, предложенную Федором Езеевым в скрипте
mms.view.py, входящим в поставку утилиты GComp (http://1c.alterplast.ru),
а именно: пользуемся тем, что структура als-файла напоминает по синтаксису
статическое задание массива в js (в оригинале - Питоне), стоит только заменить 
фигурные скобки ("{" и "}") на квадратные ("[" и "]" соответственно) и решить 
проблему с многострочным текстом и спецсимволами.

Такое преобразование реализовано в функции parseAls().

ШАГ 2. Подготовка полученной структуры и содержащихся в ней данных.
-------------------------------------------------------------------

На данном этапе мы решаем следующие проблемы:

	1. als-файл может описывать структуры произвольной вложенности, в то время
		как в xml-файле отношение иерархии задано жестко и ограничено двумя уровнями:

			 Тип (объект)
			 |	
			 +---Методы и/или свойства

	2. существенная часть информации о методах и свойствах типа (объекта) представлена
		в виде plain-text, впрочем определенным образом размеченного, но, к сожалению,
		формат этой разметки не строгий и с этим надо как-то бороться

В тех случаях, когда описание методов и свойств в фолдерах сгруппированы в разделы (фолдеры)
с именами "Методы" и "Атрибуты" соответственно, очевидно, имеет смысл содержимое таких
фолдеров не выделять в отдельный тип, а "развернуть" иерархию, т.е. добавить вложенные 
элементы к элементам родительского фолдера.

Преобразование выполняем по следующей схеме:

1. для каждого фолдера шелла:
2. запоминаем фолдер в списке типов
3. перебираем вложенные элементы фолдера:
	3.1 обычные айтемы сразу сохраняем в зависимости от типа айтема либо в список
	свойств, либо в список методов типа, соответствующего обрабатываемому фолдеру
	3.2. если вложенный элемент - фолдер, то
		3.2.1 если имя фолдера "Атрибуты" или имя фолдера "Методы", то сохраняем
		все элементы (а они заведомо только типа "Item") в список свойств или методов 
		объекта соответственно
		3.2.2 иначе считаем, что это новый объект и применяем рекурсивно к этому фолдеру 
		настоящий алгоритм, начиная с пункта 2.

ШАГ 3. Полученную обработанную структуру сохраняем в XML.
---------------------------------------------------------

Здесь все достаточно просто, ибо мы хорошо потрудились на предыдущих этапах :-).
   
\* ============================================================================ */

/* ============================================================================ *\
                    ФУНКЦИИ ПРЕОБРАЗОВАНИЯ ALS-ФАЙЛА В JS-МАССИВ
\* ============================================================================ */

/* 
 * Спасибо Федору Езееву aka fez за оригинальную идею, уже давно 
 * подсмотренную мной в mms.view.py -- a13x
 */
function parseAls(alsFile, jsFile) 
{	
	var tree = null;

	var out = jsFile ? fso.OpenTextFile(jsFile, 2, true) : null;
	var inp = fso.OpenTextFile(alsFile, 1);
	
	var s = inp.ReadAll();

/*

0. Экранируем слэши

1. Меняем обрамляющие двойные кавычки на одинарные: 
	
	1.1 Одинарные кавычки внутри строковых параметров заменяем на обратную кавычку ("`")
	
	1.2 Пограничные двойные кавычки меняем на одинарные (о квадратных скобках позаботиться не может,
	ибо они встречаются в некоторых алсах внутри строковых параметров)

		{"Shell, {"Folder и {"Item меняем на соответственно на {'Shell, {'Folder и {'Item

		"} меняем на '}

		",[ меняем на ',}
	
		"," меняем на ','
	
2. Фигурные скобки меняем на квадратные 

3. Многострочные комментарии

	3.1 Удаляем переводы строк вне строковых параметров:

		},<РазделительСтрок>  меняем на },

	(аналогичные случаи с пограничными кавычками автоматически были обработаны на этапе 1.2)

	3.2 В конце каждой строки ставим обратный слеш "\" (ктому же еще сохраняем признак перевода строки!)

4. Наводим красоту 

	4.1 заменяем обратную одинарную кавычку на заэкранированную одинарную
	
	4.2 сдвоенные двойные кавычки меняем на одну двойную кавычку

*/

	s = s.replace(/\\/g, '\\\\');
	
	s = s.replace(/\'/g, '`');
	
	s = s.replace(/\{\s*\"(Shell|Folder|Item)/g,	"{'$1");
	s = s.replace(/\"\s*}\s*(,|})/g,		"'}$1");

	/* Моя головная боль:
	 *	{""Апельсин"", ""Яблоко"", ""Мандарин"", ""Ананас""}
	 */
	
	// пытаемся решить проблему с апельсинами
	s = s.replace(/\{\s*""([^\"])/g,					'{``$1');
	s = s.replace(/([^\"])\"\"\s*,\s*\"\"([^\"])/g,	'$1``,``$2');
	
	s = s.replace(/([^\"])\"\"}/g, "$1``}");
	
	s = s.replace(/\"\s*,\s*\{/g,	"',{");
	s = s.replace(/\"\s*,\s*\"/g,	"','");
	
	s = s.replace(/}\s*(,{0,1})\s+/g, "}$1");

	s = s.replace(/\{/g, '[');
	s = s.replace(/\}/g, ']');

	// боремся с последствиями синтаксических ошибок в als-файлах,
	// с которыми можем справиться (ими грешат Automation.als и ms_whs.als)
	s = s.replace(/\"\s*\]\s*\[\'/g, "'],['");
		
	// разбираемся с многострочными текстовыми параметрами
	s = s.replace(/(\r\n|\r|\n)/g, "\\r\\n\\\r\n") 
		
	// возвращаем одинарные и двойные кавычки на место
	s = s.replace(/``/g, '"');
	s = s.replace(/`/g, "\\'");
	s = s.replace(/\"\"/g, '"');	

	
	// Пытаемся скомпилировать полученный код
	try { 
		eval ("tree = " + s)
	}
	catch (e) { 
		tree = null;
		msg(e.description) 		
	}
	
	if (out) {
		out.WriteLine(s);			
		out.Close();
	}
	
	inp.Close();
	
	return tree;
}

/* ============================================================================ *\
                    ФУНКЦИИ ПОДГОТОВКИ ДАННЫХ К СОХРАНЕНИЮ В XML
\* ============================================================================ */

/* processShell(alsShell)
 * 	Обрабатывает последовательно все вложенные фолдеры шелла.
 *	Возвращает подготовленные к сохранению в XML данные.
 */
function processShell(alsShell) 
{
	var types = {}; // здесь храним подготовленную информацию о типах
	if (alsShell[0] == 'Shell') {
		for (var i=1; i<alsShell.length; i++) {
			if (typeof(alsShell[i])=='object') {
				processFolder(alsShell[i], types);
			}
		}	
	}
	return types;
}

/* processFolder(folder, types)
 *	Выполняет рекурсивную обработку фолдера.
 *	В types добавляется полученная в процессе обработки информация о типах.
 */
function processFolder(folder, types) 
{
	var typeName = folder[2];

	if (!types[typeName]) {
		types[typeName] = { 
			name	: makeValidId(folder[2]),
			alias	: makeValidId(folder[3]),
			attribs	: [], // массив атрибутов
			methods	: []  // массив методов
		}
	}

	// Получаем массив вложенных элементов фолдера
	var items = folder.slice(4);

	/* Обходим вложенные элементы фолдера:
	 *	1. айтемы сохраняем в список методов или свойств обрабатываемого фолдера
	 *	2. элементы вложенных фолдеров с именами "Атрибуты" или "Методы" добавляем
	 *		к свойствам или методам обрабатываемого родительского фолдера
	 *	3. остальные фолдеры обрабатываем рекурсивно 
	 */

	for (var i=0; i<items.length; i++) {
		if (typeof(items[i]) == 'object') {
			if (items[i][0] == 'Item') { //[1]			
				processItem(types[typeName], items[i]);
			}
			else /*if items[i][0] == 'Folder'*/ {
				var re_attr_or_meth = /^(?:Атрибуты|Методы|Свойства)$/;
				if (re_attr_or_meth.test(items[i][2])) {
					//[2] "разворачиваем" второй уровень фолдера
					var items2 = items[i].slice(4);				
					for (var j=0; j<items2.length; j++) {
						if (typeof(items2[j])=='object') {
							if (items2[j][0] == 'Folder') { // для случая Атрибуты/Дебет/Атрибуты и ему подобных
								processFolder(items2[j], types);					
							}
							else {
								processItem(types[typeName], items2[j]);
							}
						}
					}
				} 
				else { // [3] рекурсивно обрабатываем вложенные фолдеры
					processFolder(items[i], types);
				}
			}
		}
	}	
}

/* processItem(type, item)
 *	Обработка элемента фолдера - айтема.
 *	В type добавляется новый метод или свойство - в зависимости 
 *	от содержимого обрабатываемого айтема.
 */
function processItem(type, item) 
{
	if (typeof(item)=='object') {	
		if (isValidId(item[2])) { // это правильный идентификатор?			
			if (item[4] && item[4].match(/^[А-я\w]+\(/)) { // это метод
				addMethod(type.methods, item);
			}
			else { // это свойство
				addAttrib(type.attribs, item);
			}
		}
		else {
			// и вновь боремся с криво написанными als-ами (ms_whs.als в частности)
			var m = item[2].match(/^([А-я\_\w][А-я\_\w\d]+)(\(.*)/); 
			if (m) {
				item[2] = m[1];
				item[4] = item[2];
				addMethod(type.methods, item);
			}
		}
	}
}

var re_bad_ids = /(?:Введение|Пример|Назначение|Описание|readme)/;
var re_valid_id = /^[А-я\_\w][А-я\_\w\d]*$/;

function isValidId(id) 
{
	if (re_valid_id.test(id)) {
		return !re_bad_ids.test(id);
	}
	return false;
}

/* addMethod(methods, item)
 *	Добавляет информацию о методе к списку методов типа.
 */
function addMethod(methods, item) 
{	
	var data = parseDescription(item[6]);
	
	var descrStr = data['НАЗНАЧЕНИЕ'];
	if (!descrStr) {
		descrStr = data['SUMMARY']?data['SUMMARY']:data['ОПИСАНИЕ'];
	}

	var retStr = data['ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ']?data['ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ']:data['ВОЗВРАЩАЕТ'];
		
	var method = {
		name 			: makeValidId(item[2]),
		alias			: makeValidId(item[3]),
		descr			: descrStr,
		params			: data['ПАРАМЕТРЫ'],
		retTypeDescr 	: retStr?retStr.replace(/<.+?\>/,''):retStr,
		retType 		: retrieveType(retStr)
	}
	
	methods[methods.length] = method;
}

/* addAttrib(attribs, item)
 *	Добавляет информацию о свойстве к списку свойств типа.
 */
function addAttrib(attribs, item) 
{
	var data = parseDescription(item[6]);
	
	var descrStr = data['НАЗНАЧЕНИЕ'];
	if (!descrStr) {
		descrStr = data['SUMMARY']?data['SUMMARY']:data['ОПИСАНИЕ'];
	}
	
	var attrib = {
		name 	: makeValidId(item[2]),
		alias	: makeValidId(item[3]),
		descr	: descrStr
	}
	attribs[attribs.length] = attrib;
}

// разделы (секции) описания
var re_sect = /@{0,1}(Назначение|Параметры|Синтаксис|Возвращаемое значение|Возвращает|Пример|Описание|Замечание|[Ss]ummary|sig):*\s*/g;

/* parseDescription(source)
 *	Извлекает информацию из текстового описания айтема.
 */
function parseDescription(source) 
{
	var data = {};

	/* Приходится учитывать следующее:
	 * 	Разделы описания могут начинаться с символа "@", а могут и не начинаться.
	 *	После имени раздела может идти двоеточие (":"), а может и не идти.
	 *	А вот имена разделов точно располагаются в новой строке (ура) - и это нам поможет.
	 */
	
	// Делим весь текст описания на блоки - по границам разделов
	var arr = source.replace(re_sect, "<#!#>$1\r\n").split(/<#!#>/);
	
	// Последовательно обрабатываем каждый раздел
	for (var i=0; i<arr.length; i++) {
		var lines = arr[i].split(/\r\n/);
		var id	= trim(lines[0]).toUpperCase();
		switch (id) {
			// раздел параметров обрабатываем отдельно, извлекая инфу о параметрах
			case 'ПАРАМЕТРЫ': 
				data[id] = parseParamDescr(lines.slice(1));						
				break;
		default:
			// точку в конце текста удалим (Телепат в подсказке точку добавляет сам)
			data[id] = lines.slice(1).join(' ').replace(/\.$/,'');
		}
	}

	return data;
}

/* parseParamDescr(lines)
 *	Извлекает информацию о параметрах на основе строк описания параметров.
 *	Возвращает массив объектов, хранящих информацию об имени типе параметра
 *	и текстовое описание параметра.
 */
function parseParamDescr(lines) 
{
	var m, params = [];
	for (var j=0; j<lines.length; j++) {	
		/* Строка описания параметра имеет следующий формат:
		 * 	<ИмяПараметра> - (ТипПараметра) Описание параметра 
		 * Тип параметра может отсутствовать.
		 */		
		if (m = lines[j].match(/^<([А-я\w]+)>\s*-\s*(\(.+?\))*\s*(.+)/)) {	
			var name = m[1];	
			var type = m[2].replace(/(\)|\()/g,'').replace(/(\\|\/)/,',');
			var descr = m[3].replace(/\.$/,'');
			if (isEmpty(type)) {
				// Если тип не задан в круглых скобках в начале описания, то
				// пытаемся извлечь имя типа из текста описания.
				type = retrieveType(descr);
			}
			params[params.length] = {
				name : name,
				type : type,
				descr: descr
			}
		}
	}
	return params;
}

/* retrieveType(str)
 *	Пытается извлечь информацию о типе из строки (условно) произвольного формата.
 */
function retrieveType(str)
{
	var m;
	if (!isEmpty(str)) {
		/* Имя типа явно задано в скобках, например:
		 *	(Число) 
		 */
		if (m = str.match(/^\s*\((.+?)\)/)) {
			return m[1];
		}
		/* В начале строки описания может быть написано "Число" или "Числовой",
		 * "Строка" или "Строковый"
		 */
		if (m = str.match(/^\s*([Чч]исл|[Сc]трок)/)) {
			switch (m[1].toUpperCase()) {
				case 'ЧИСЛ'	: return 'Число';
				case 'СТРОК': return 'Строка';
			}
		}
		/* Информация о типе указана в виде
		 * "Тип - <ИмяТипа>" или "Тип: <ИмяТипа>", например:
		 * 		Тип - OLE-объкт.
		 */
		if (m = str.match(/Тип\s*[:\-]\s*(.+?)\s*[,;\.]/)) {
			return m[1]
		}
		/* Вероятно, используется для разметки комментариев в 1C++
			< type="Число" >
		*/
		if (m = str.match(/<\s*type=\"(.+?)\"/)) {
			return m[1].replace(/\|/g, ',');;
		}
	}
}

// Массив 
var folder_name_patterns = [
	/Базовый объект - (узел)/,
	/.+?\(объект ([А-я\w]+)\)/, // Например, "Доступ к данным по протоколу HTTP (объект V7HttpReader)"
	/Объект\s+([А-я\w]+)/,		// Напрмер,  "Объект СервисРасшФормы"
	/.+?\((\w+)\)/				// Например, "Дозвон (RasDial)"
]

/* makeValidId(str)
 *	Извлекает из строки заголовка фолдера имя типа, используя перечисленные в
 *	массиве шаблоны. Если не одному шаблону строка заголовка фолдера не удовлетворяет,
 *	то заголовок преобразуется в валидный идентификатор путем удаления пробелов и
 *	и спецсимволов из строки.
 *	Возвращает - правильный идентификатор типа.
 *	TODO "Это идентификатор" -> "ЭтоИдентификатор" (сейчас получается "Этоидентификатор")
 */
function makeValidId(str)
{
	var m;
	for (var i=0; i<folder_name_patterns.length; i++) {
		if (m = str.match(folder_name_patterns[i])) {
			return m[1];
		}
	}
	return str.replace(/\s+/g, '_').replace(/[^А-я\w\d]/g, "");
}
	
/* ============================================================================ *\
                    ФУНКЦИИ СОХРАНЕНИЯ ДАННЫХ В ФОРМАТЕ XML
\* ============================================================================ */

/* saveAsXML(types, xmlFile)
 *	Сохраняет извлеченные из als-файла данные в формате xml.
 *	types - извлеченные данные о типах, которые требуется сохранить
 *	xmlFile - имя xml-файла, в который сохраняются данные
 */
function saveAsXML(types, xmlFile, prefix, indent) 
{
	var xmlDoc = new ActiveXObject('MSXML2.DOMDocument');

	xmlDoc.async = false;
	xmlDoc.resolveExternals = false;

	// заголовок xml-файла
	var pi = xmlDoc.createProcessingInstruction('xml', 'version="1.0" encoding="windows-1251"');
	xmlDoc.appendChild(pi);

	// создаем и добавляем в документ корневой элемент - тэг <Types>
	var xmlTypes = xmlDoc.createElement('Types');
	xmlDoc.appendChild(xmlTypes);

	// последовательно сохраняем все типы
	for (var typeName in types) {
		var type = types[typeName];
		// если есть хотя бы один метод или свойство - то сохраняем
		if (type.methods.length || type.attribs.length) {
			//<type>
			var xmlType = saveTypeAsXML(type, xmlDoc, prefix);
			xmlTypes.appendChild(xmlType);
			//</type>
		}
	}

	if (indent) {
		indentXML(xmlDoc.documentElement, "\n", indent);
	}
	xmlDoc.save(xmlFile);
}

/* saveTypeAsXML(type, xmlDoc)
 *	Сохраняет очередной тип type в xml-документ.
 *	Возвращает xml-узел со структурой, описывающий сохраняемый тип.
 */
function saveTypeAsXML(type, xmlDoc, prefix) 
{
	var xmlType = xmlDoc.createElement('type');

	// основные атрибуты - имя и представление типа для пользователя
	xmlType.setAttribute('name'	,(prefix ? prefix : "") + type.name);
	xmlType.setAttribute('alias',(prefix ? prefix : "") + (isEmpty(type.alias) ? type.name : type.alias));

	// сохраняем свойства (атрибуты) типа
	if (type.attribs.length) {
		// <attribs>
		var xmlAttribs = xmlDoc.createElement('attribs');
		for (var i=0; i<type.attribs.length; i++) {
			//<attrib>
			var xmlAttrib = saveAttribAsXML(type.attribs[i], xmlDoc);
			xmlAttribs.appendChild(xmlAttrib);
			//</attrib>
		}
		xmlType.appendChild(xmlAttribs);
		//</attribs>
	}

	if (type.methods.length) {
		/* TODO отделять обработчики событий (handlers) от методов (methods),
		 * например, используя паттерн /^При/. XXX Вообще это надо делать еще на 
		 * этапе подготовки данных.
		 */
		//<methods>
		var xmlMethods = xmlDoc.createElement('methods');
		for (var i=0; i<type.methods.length; i++) {
			//<method>
			var xmlMethod = saveMethodAsXML(type.methods[i], xmlDoc);
			xmlMethods.appendChild(xmlMethod);
			//</method>
		}
		xmlType.appendChild(xmlMethods);
		//</methods>
	}

	return xmlType;
}

/* saveAttribAsXML(attrib, xmlDoc)
 *	Сохраняет информацию о свойстве (атрибуте) типа в xml.
 *	Возвращает xml-узел с сохраненными данными.
 */
function saveAttribAsXML(attrib, xmlDoc) 
{
	var xmlAttrib = xmlDoc.createElement('attrib');
	xmlAttrib.setAttribute("name", attrib.name);
	if (!isEmpty(attrib.alias)) {
		xmlAttrib.setAttribute("alias", attrib.alias)
	}
	if (!isEmpty(attrib.type)) {
		xmlAttrib.setAttribute("type", attrib.type);
	}
	if (!isEmpty(attrib.descr)) {
		// текстовое описание атрибута
		var xmlDescr = xmlDoc.createTextNode(attrib.descr);
		xmlAttrib.appendChild(xmlDescr);
	}
	return xmlAttrib;
}

/* saveMethodAsXML(method, xmlDoc)
 *	Сохраняет информацию о методе типа в xml.
 *	Возвращает xml-узел с сохраненными данными.
 */
function saveMethodAsXML(method, xmlDoc) 
{
	var xmlMethod = xmlDoc.createElement('method');
	xmlMethod.setAttribute('name', method.name);
	if (!isEmpty(method.alias)) {
		xmlMethod.setAttribute('alias', method.alias);
	}
	if (!isEmpty(method.descr)) {
		var xmlDescr = xmlDoc.createTextNode(method.descr);
		xmlMethod.appendChild(xmlDescr);
	}
	// если есть параметры - сохраняем их
	if (method.params && method.params.length) {
		//<params>
		var xmlParams = xmlDoc.createElement('params');
		for (var i=0; i<method.params.length; i++) {
			//<param>
			var xmlParam = saveMethParamAsXML(method.params[i], xmlDoc);
			xmlParams.appendChild(xmlParam);
			//</param>
		}
		xmlMethod.appendChild(xmlParams);
		//</params>
	}
	// Информация о возвращаемом значении
	if (!isEmpty(method.retTypeDescr)) {
		//<return>
		var xmlReturn = xmlDoc.createElement('return');		
		var xmlDescr = xmlDoc.createTextNode(method.retTypeDescr);
		if (!isEmpty(method.retType)) {
			xmlReturn.setAttribute('type', method.retType);
		}
		xmlReturn.appendChild(xmlDescr); // текстовое описание
		xmlMethod.appendChild(xmlReturn);
		//</return>
	}
	return xmlMethod;
}

/* saveMethParamAsXML(param, xmlDoc)
 *	Сохраняет информацию о параметре метода в xml.
 *	Возвращает xml-узел с сохраненными данными.
 */
function saveMethParamAsXML(param, xmlDoc) 
{
	var xmlParam = xmlDoc.createElement('param');
	xmlParam.setAttribute('name', param.name);
	if (!isEmpty(param.type)) {
		xmlParam.setAttribute('type', param.type);
	}
	if (param.descr.match(/Необязательный параметр[\.,;]*|\([Нн]еобязательный\)/)) {
		// XXX как нибудь извлечь информацию о значении по умолчанию?
		xmlParam.setAttribute("default", "");
	}
	// текстовое описание параметра
	if (!isEmpty(param.descr)) {
		// уберем лишнюю инфу из описания
		var descr = param.descr.replace(/[Нн]еобязательный параметр[\.,;]*|\([Нн]еобязательный\)/g, '');
		var xmlDescr = xmlDoc.createTextNode(descr);
		xmlParam.appendChild(xmlDescr);
	}
	return xmlParam;
}

/* indentXML(node, curIndent, levelIndent)
 *	Выполняет форматирование XML-кода отступами.
 *	Автор: Anton Lapounov
 *	Источник: http://www.raleigh.ru/wiki/faq:save_indent
 */
function indentXML(node, curIndent, levelIndent)
{
	// Если у узла нет содержимого, ничего не делать
	if (!node.hasChildNodes) return;

	// Если содержимое включает текстовые узлы, ничего не делать
	for (var child = node.firstChild; child != null; child = child.nextSibling) {
		if ((child.nodeType == 3)  /* PCDATA */
		||  (child.nodeType == 4)) /* CDATA */
			break;
	}
 
	if (child != null) return;
 
	// Вставить текстовые узлы с отступами между дочерними узлами и обработать их рекурсивно
	var newIndent = curIndent + levelIndent;
	var textNode = node.ownerDocument.createNode(3, "", "");
	textNode.text = newIndent;

	for (child = node.firstChild; child != null; child = child.nextSibling) {
		node.insertBefore(textNode.cloneNode(false), child);
		indentXML(child, newIndent, levelIndent);
	}
 
	textNode.text = curIndent;
	node.appendChild(textNode);
}
/* ============================================================================ *\
                    ФУНКЦИИ СОХРАНЕНИЯ ДАННЫХ В INTS-ФАЙЛАХ
\* ============================================================================ */

function saveAsInts(types, intsFolder, prefix) 
{	
	var crObjFile = fso.OpenTextFile(fso.BuildPath(intsFolder, "СоздатьОбъект.ints"), 8, true);
	for (var typeName in types) {
		var type = types[typeName];
		if (type.attribs.length||type.methods.length) {
			var validTypeName	= (prefix ? prefix+"." : "") + makeValidId(typeName);			
			var intsFileName	= fso.BuildPath(intsFolder, validTypeName) + ".ints";
			crObjFile.WriteLine("0000 " + validTypeName);
			msg(intsFileName + " ...");			
			var intsFile = fso.OpenTextFile(intsFileName, 2, true);
			saveTypeAsInts(types[typeName], intsFile);
			intsFile.Close();
		}
	}
	crObjFile.Close();
}

function saveTypeAsInts(type, intsFile) 
{
	// атрибуты...
	var attribs = type.attribs;
	if (attribs.length) {
		for (var i=0; i<attribs.length; i++) {
			intsFile.WriteLine("0000 " + attribs[i].name);
		}
	}
	// методы...
	var methods = type.methods;	
	if (methods.length) {
		for (var i=0; i<methods.length; i++) {
			var f = (methods[i].params && methods[i].params.length)?"f":"";
			intsFile.WriteLine("0000 " + methods[i].name + "(" + f + ")");
		}
	}
}

/* ============================================================================ *\
              СОХРАНЕНИЕ ИМЕН МЕТОДОВ И АТРИБУТОВ В ФАЙЛ КЛЮЧЕВЫХ СЛОВ
\* ============================================================================ */

function saveKeywords(types, fileName)
{
	var file = fso.OpenTextFile(fileName, 2, true);
	for (var typeName in types) {
		_saveKeywords(file, types[typeName].attribs);
		_saveKeywords(file, types[typeName].methods);
	}
	file.Close();
}

function _saveKeywords(f, a)
{
	if (a && a.length) {
		for (var i=0; i<a.length; i++) {
			f.WriteLine(a[i].name);
		}
	}
}

/* ============================================================================ *\
                          ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
\* ============================================================================ */

function trim(str) 
{
	if (!str) return "";
	return str.replace(/^\s+/,'').replace(/\s+$/,'');
}

function isEmpty(s) 
{
	return (!s || /^\s*$/.test(s));
}

function msg(str, marker)
{
	if (!Silent) {
		if (OpenConf) {
			Message(str, marker?marker:mNone);
		}
		else {
			WScript.Echo(str)
		}
	}
}

function exit(errCode)
{
	WScript.Quit(errCode);
}

function inConfigurator(_)
{
	try {
		var version = Configurator.Version;
		return true;
	} catch(e) {
		return false;
	}
}

/* ============================================================================ *\
                               ОСНОВНАЯ ПРОГРАММА	
\* ============================================================================ */

OpenConf = inConfigurator();

if (OpenConf) 
	Init() 
else {
	Run();
	exit(0);
}

