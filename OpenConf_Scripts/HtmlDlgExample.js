// ========================================================================
// 2005-11-07
// a13x <kuntashov at yandex dot ru>
// Пример скрипта с html-диалогом
// ========================================================================

// обработчики событий контролов html-диалога для удобства
// реализуем в виде одного js-объекта
var HtmlDlgEventProxy =
{
	Button1OnClick : function (ThisForm) {
		var name = "Мир";
		if (!ThisForm.CheckBox1.checked) {
//			name = CommonScripts.InputBox("Ведите имя", "Html Dialog Example", "");
			name = CommonScripts.InputBox("Введите имя");
		}
		Message("Привет, " + name + "!");
	},

	Button2OnClick : function (ThisForm) {
		Message(ThisForm.Edit1.value);
	},

	Button3OnClick : function (ThisForm)
	{
		// здесь будем генерить элементы второго листбокса

		// очищаем список
		ThisForm.ComboBox1.options.length=0;

		// добавляем 10 новых элементов
		for (var i=0; i<10; i++) {
			var opt = HtmlWindow.Document.createElement("OPTION");
			opt.value = i;
			opt.text = "Значение " + i;
			ThisForm.ComboBox1.options.add(opt);
		}

	},

	OnOpen : function ()
	{
		Message("При открытии");
	},

	OnClose : function () {
		// перезагружаемся, чтобы очистить пространство имен скрипта от переменной HtmlWindow
		// и оставить пользователю возможность открыть диалог без глюков еще раз :-)
		Scripts.Reload(SelfScript.Name);
	},

	ListBox1OnChange : function (ThisForm) {
		Message("Выбран элемент: " + ThisForm.ListBox1.options[ThisForm.ListBox1.selectedIndex].value
				+ " (" + ThisForm.ListBox1.options[ThisForm.ListBox1.selectedIndex].innerText + ")");
	},

	ComboBox1OnChange : function (ThisForm) {
		Message("Выбран элемент: " + ThisForm.ComboBox1.options[ThisForm.ComboBox1.selectedIndex].value);
	},

	RadioButton1OnClick: function (ThisForm, SelectedIndex) {
		Message("Выбран радиобатон № " + ThisForm.RadioButton1[SelectedIndex].value);
	}
}

// вызывается при открытии окна, здесь мы должны передать в html-диалог
// наш объект, в котором реализованы обработчики событий html-элементов
function onDocumentComplete(d, u)
{
	try {
		// привязываемся к событиям контролов html-документа
		HtmlWindow.Document.Script.SetEventProxy(HtmlDlgEventProxy);
		Windows.ActiveWnd.Maximized = true;
	}
	catch (e) {
		Message("Ошибка инициализации окна HTML-диалога", mRedErr);
		Message(e.description, mRedErr);
	}
}

// макрос для открытия html-диалога нашего скрипта
function OpenHTMLDialog()
{
	var wnd = OpenOleForm("Shell.Explorer", "HTML Dialog Example");
	SelfScript.AddNamedItem("HtmlWindow", wnd, false);
    eval('function HtmlWindow::DocumentComplete(d,u){ return onDocumentComplete(d, u) }');
	HtmlWindow.Navigate2(BinDir+"config\\data\\HtmlDlgExample.htm");
}

function CreateObjectOrDie(progid, id)
{
	try {
		var obj = new ActiveXObject(progid);
		if (id) {
			SelfScript.AddNamedItem(id, obj, false);
		}
		return obj;
	}
	catch (e) {
		Message("Не могу создать объект " + progid, mRedErr);
		Message(e.description, mRedErr);
		Message("Скрипт " + SelfScript.Name + " не загружен", mInformation);
		Scripts.UnLoad(SelfScript.Name);
	}
}

function Init(_)
{
	CreateObjectOrDie("OpenConf.CommonServices", "CommonScripts");
	//OpenHTMLDialog();
}

Init();
