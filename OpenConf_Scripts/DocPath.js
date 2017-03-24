/*===========================================================================
Скрипт:  DocPath.js
Версия:  $Revision: 1.4 $
Автор:   Александр Кунташов
E-mail:  kuntashov at [yandex.ru|gmail.com]
ICQ UIN: 338758861

Описание: 
    Макросы для показа/копирования в буфер обмена полного пути 
    открытого документа (внешнего отчета, мокселя или текстового файла).

	Также скрипт решает следующую проблему: при открытии внешнего отчёта 
	его полный путь выводится в строке заголовка окна. Однако, после первого 
	сохранения путь урезается до имени файла, что не всегда удобно. 
	Данный скрипт перехватывает событие сохранения внешнего отчета и
	выводит полный путь в строке статуса или заголовке окна (настраивается, см. 
	ниже, комментарий НАСТРОЙКА).
	Автор - Phoenix.

	Макросы:
	
    	ShowDocPath	- выводит путь текущего документа в окно сообщений
    	CopyDocPath	- копирует путь текущего документа в буфер обмена 
    	OpenParentDir - открывает каталог, в котором находится текущий 
						документ в эксплорере
    	CopyParentDir - копирует каталог, в котором находится текущий
						документ в буфер обмена

===========================================================================*/

var mInStatusBar	= 0; // показывать путь в строке статуса
var mInCaption		= 1; // показывать путь в заголовке окна

// НАСТРОЙКА (можно изменить на одну из констант выше):
// где показывать путь внешнего отчета - в заголовке или в строке статуса
// по умолчанию - в заголовке
var mShowPathIn		= mInCaption; 

// ===========================================================================

var CoreObj = {
		
	version : "$Revision: 1.4 $".replace(/\s|\$/ig, "").replace(/^Revision:/, ""),

	ocs : null,

	init : function () 
	{
		try {
			this.ocs = new ActiveXObject("OpenConf.CommonServices");
			this.ocs.SetConfig(Configurator);
		}
		catch (e) {
			Message("Не могу создать объект OpenConf.CommonServices", mRedErr);
			Message(e.description, mRedErr);
			Message("Скрипт " + SelfScript.Name + " не загружен", mInformation);
			Scripts.UnLoad(SelfScript.Name); 		
		}	
	},
	
	show : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				Message(a.Path, mInformation);
			}
		}
	},

	copy : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				this.ocs.CopyToClipboard(a.Path);
			}
		}
	},

	openDir : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				var p = this.ocs.FSO.GetParentFolderName(a.Path);				
				this.ocs.WSH.Run("%SYSTEMROOT%\\explorer.exe " + p);
			}
		}
	},

	copyDir : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				var p = this.ocs.FSO.GetParentFolderName(a.Path);
				this.ocs.CopyToClipboard(p);
			}
		}
	},

	getFileName : function ()
	{
    	if (!(Windows.ActiveWnd == null)) {
        	var WndName = Windows.ActiveWnd.Document.Name;
        	
    	    if ((WndName.substring(0, 14) == "CWorkBookDoc::") || (WndName.substring(0, 14) == "CBodyTextDoc::")) {
            	return Windows.ActiveWnd.Document.path;
        	}
    	    else {
        	    return WndName;
    	    }
	    }
	}

}

function ShowDocPath() { CoreObj.show() }
function CopyDocPath() { CoreObj.copy() }

function OpenParentDir() { CoreObj.openDir() }
function CopyParentDir() { CoreObj.copyDir() }

function Configurator::OnFileSaved(Doc)
{
    if (mShowPathIn == mInStatusBar)
        Status(CoreObj.getFileName());
    else
        Windows.ActiveWnd.Caption = CoreObj.getFileName();
}	

CoreObj.init()

