/*
Функция создает окно WebBrowser'а в Конфигураторе с заданным заголовком,
загружает в него страницу с указанного адреса, и начинает отлавливать событие
загрузки страницы.
*/
function ShowHtml(url,title)
{
	h=OpenOleForm("Shell.Explorer", title)
	/*
	Почему-то VBScript не привязывается к событиям WebBrowser'а,
	поэтому используется JScript. Изящное решение, как в JScript
	привязать обработку событий к динамически создаваемому объекту,
	предложил Александр Кунташов, за что ему спасибо.
	*/
	SelfScript.AddNamedItem("Html",h,false)
    eval('function Html::DocumentComplete(d,u){ return HtmlDC(0)}');
	Html.Navigate2(url)
}

/*
Обработчик события окончания загрузки страницы.
Предполагается, что в скриптах на этой странице есть метод SetConf,
через который в страницу передается объект Configurator.
*/
function HtmlDC(_)
{
	// Передадим в страницу Configurator
	try{
	Html.Document.Script.SetConf(Configurator)
	}catch(e)
	{
		Message("Не удалось передать Configurator в страницу")
	}
	// Перезагрузим скрипт, чтобы он был готов к загрузке другой страницы
	Scripts.Reload(SelfScript.Name)
}

function ShowTestPage()
{
	ShowHtml(BinDir+"test.htm","Test page")
}

