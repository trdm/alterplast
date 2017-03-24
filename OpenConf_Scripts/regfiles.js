/***************************************************************************
 * За основу взят код скрипта regwscs.js из дистрибутива плагина SHPCE
 * для Far (http://scrhostplugin.sf.net), с некоторыми правками -- a13x
 **************************************************************************/

/*
 *	Регистрирует все системные библиотеки, используемые скриптами для ОпенКонфа.
 *
 *	Использование.
 *
 *		Запустите скрипт в папке, в которой расположены системные фaйлы.
 *
 *		Командная строка для запуска регистрации всех системных файлов:
 *
 *				cscript //nologo regfiles.js /I
 *
 *		Командная строка для отмены регистрации файлов:
 *
 *				cscript //nologo regfiles.js /U
 *
 *		Последним параметром можно указывать /S, в этом случае скрипт
 *		отработает в "тихом" режиме, т.е. не будет ничего выводить на консоль.
 *		Для вывода сообщений о регистрации в файл regfiles.log следует использовать
 *		дополнительный параметр /L. Если при этом использовать также параметр /S,
 *		то вывод сообщений будет производиться *только* в файл regfiles.log.
 *
 *		Можно также воспользоваться пакетными файлами regall.bat и unregall.bat
 *		соответственно для регистрации и отмены регистрации системных файлов.
 *		Как вы можете догадаться, они как раз и выполняют запуск скрипта описанными
 *		выше способами.
 *
 *		Об ошибках сообщайте по электропочте:
 *					kuntashov at yandex dot ru
 *					kuntashov at gmail	dot com
 *		Просьба добавлять в начало темы (сабжа) слово "OC_Install:" (без кавычек).
 */

//*************************************************************************

// !!! Порядок следования имен файлов важен:
// сначала *.dll, потом *.wsc. Порядок среди wsc-файлов тоже важен,
// поскольку одни файлы могут использовать другие.

var files = new Array(
"SelectValue.dll",
"svcsvc.dll",
"dynwrap.dll",
"WshExtra.dll",
"macrosenum.dll",
"SelectDialog.dll",
"ArtWin.dll",
"CommonServices.wsc",
"Collections.wsc",
"Registry.wsc",
"1S.StatusIB.wsc",
"SyntaxAnalysis.wsc",
"OpenConf.RegistryIniFile.wsc",
"tlbinf32.dll",
"Templates.wsc"
);

//*************************************************************************

try {

	var WshShell = new ActiveXObject("WScript.Shell")
	var fso = new ActiveXObject("Scripting.FileSystemObject");

	// Скрипт должен лежать в одной директории вместе с файлами, которые необходимо
	// зарегистрировать, и на случай, если он будет запускаться из другой рабочей
	// директории, то принудительно исправим значение текущей директории на ту,
	// в которой лежит сам скрипт
	WshShell.CurrentDirectory = fso.GetParentFolderName(WScript.ScriptFullName)

	var silent	= false;
	var unr		= false;
	var logfile	= null;

	// Если запускаемся не под управлением CScript.exe, то принудительно
	// подавляем вывод сообщений пользователю, дабы избавить его от назойливых
	// MessageBox'ов при каждом вызове Echo() и вместо этого обязательно пишем
	// все сообщения в лог (regfiles.log)

	var forseSilent = (/wscript.exe$/i).test(WScript.FullName);

	if (!WScript.Arguments.Count()) {
		WScript.Echo(usage());
		WScript.Quit(1);
	}

	unr	= WScript.Arguments.Named.Exists("U");

	if (WScript.Arguments.Named.Exists("L") || forseSilent) {
		logfile=fso.CreateTextFile("regfiles.log", true);
	}

	if (WScript.Arguments.Named.Exists("S")) {
		silent		= true;
		forseSilent = false;
	}

	if (!(unr||WScript.Arguments.Named.Exists("I"))) {
		WScript.Echo(usage());
		WScript.Quit(1);
	}

	var res = unr ? unregisterAll() : registerAll();

	var str = "";
	if (logfile) {
		logfile.Close();
		str = "\nПодробности регистрации в файле regfiles.log";
	}

	if (res) {
		if (!silent||forseSilent)
			WScript.Echo("Регистрация выполнена успешно!" + str);
		WScript.Quit(0); // OK
	}

	if (!silent||forseSilent)
		WScript.Echo("В процессе регистрации обнаружены ошибки!" + str);
	WScript.Quit(1); // not OK

}
catch(e) {
	if (!silent||forseSilent)
		WScript.Echo("В процессе регистрации обнаружены ошибки!\n"
					+ e.description);
	WScript.Quit(1); // not OK
}

//**************************************************************************

function usage() {
	return "Usage: cscript //nologo regfiles.js [/I|/U] [/S] [/L]\n"
		 + "  /I - register *.DLL and *.WSC files\n"
		 + "  /U - unregister *.DLL and *.WSC files\n"
		 + "  /S - keep silent (do not write any progress output to stdin)\n"
		 + "  /L - output messages to the regfiles.log\n";
}
function registerAll() {
	var isGood = false;
	if(testPreconditions())
    	isGood=runAll(files)
	return isGood;
}
function runAll(files, par){
    var isGood=true;
    for(var i in files) {
    	isGood=isGood&&runreg(i, files[i], par);
	}
    return isGood;
}
function unregisterAll(){
    return runAll(files.reverse(), "/U");
}
function runreg(i, file, par){
    var fullPath=WshShell.CurrentDirectory+"\\"+file;
	var cl = (file.match(/\.dll$/)) ? regdll(fullPath, par) : regwsc(fullPath, par);
	if (par&&(par.toUpperCase()=="/U")&&(file.toUpperCase()=="DYNWRAP.DLL")) {
		// XXX ДинаВрап часто не может нормально деинсталлиться :-(,
		// пока игнорируем такое поведение
		msg("Skipped "+"["+(new Number(i)+1)+"/"+files.length+"]:"+cl);
		return true;
	}
    msg("Running "+"["+(new Number(i)+1)+"/"+files.length+"]:"+cl);
    var errcode=WshShell.Run(cl,1,true);
	msg(((errcode==0)?"OK":"Failed")+" ( Error code = "+errcode+" )");
    return (errcode==0);
} //runreg
function regwsc(fullPath, params){
	return "regsvr32 /s "+(params?params+" ":"")+'scrobj.dll /n /i:"'+fullPath+'"';
}
function regdll(fullPath, params){
	return "regsvr32 /s " + (params?params+" ":"") + '"' + fullPath + '"';
}
function testPreconditions(){
	return testClass("MSScriptControl.ScriptControl", "Please install MS Script control");
} //testPreconditions
function testClass(progID, msg){
	try{
		new ActiveXObject(progID)
		return true;
	}catch(e){
		msg("Failed to create "+progID+". "+msg);
		return false;
	}
} //testClass
function msg(str){
	if (!silent) WScript.Echo(str);
	if (logfile) logfile.WriteLine(str);
}
