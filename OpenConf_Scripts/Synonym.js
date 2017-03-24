$NAME Синоним из идентификатора

/* ===========================================================================
Скрипт: Synonym.js
Версия: $Revision: 1.2 $
Автор: Phoenix, a13x

Описание:
	Установка синонима на основании Идентификатора.
	Формирование синонима происходит по технологии 1Сv8.0
	В идентификаторе прописная буква заменятся на пробел + сточная буква.

Макросы:
	СформироватьСиноним(), GetSinonimByName()
		Создает и устанавливает синоним реквизита на основе
		его идентификатора. Макрос можно использовать в диалогах
		"Свойства реквизита" и "Свойства объекта" (справочника, документа 
		и т.п.), в том числе и в окне свойств объекта, открываемом с помощью
		двойного клика на объекте метаданных в дереве конфигуратора.
			
=========================================================================== */

var GW_HWNDNEXT	= 0x0002;
var GW_CHILD	= 0x0005;

var CoreObj = {

	api	: null, 
	svc	: null,
	
	hIdent		: null, // хранит найденный хэндл editbox'а идентификатора
	hSynonym	: null, // хранит найденный хэндл editbox'а синонима

	// инициализация
	init: function ()
	{
		this.svc = new ActiveXObject("Svcsvc.Service");		
		this.api = new ActiveXObject("DynamicWrapper");

		this.api.Register("USER32.DLL", "GetForegroundWindow", "f=s", "r=l");
		this.api.Register("USER32.DLL", "GetWindow",  "I=ll", "f=s", "r=l")
	},

	// генерация синонима на основе идентификатора
	genSynonym: function (id) 
	{
		if (!id) return "";

		var UpChars = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮQWERTYUIOPASDFGHJKLZXCVBNM";
		var lResult = lCutLit = id.substring(0,1);

		for (var i = 1; i < id.length; i++) {
			lCutLit = id.substring(i, i+1);
			if (UpChars.indexOf(lCutLit) != -1) {
				lResult += " " + lCutLit.toLowerCase();
			}
			else {
				lResult += lCutLit;
			}
		}

		return lResult;
	},

	/* рекурсивный поиск хэндлов полей с идентификатором и синонимом
		среди дочерних элементов окна с хэндлом hParent */
	findOurHandlers: function (hParent) 
	{
		// если нужные хэндлы найдены - прекращаем поиск
		if (this.hIdent && this.hSynonym) return;

		// обходим все дочерние контролы окна с хэндлом hParent
		for (var hChild = this.api.GetWindow(hParent, GW_CHILD); hChild; hChild = this.api.GetWindow(hChild, GW_HWNDNEXT))
		{
			if (this.hIdent && this.hSynonym) return;

			var title = this.svc.GetWindowText(hChild, false);
			if ("&Идентификатор:" == title) { 
				/* нашли лейбу с подписью к editbox'у с идентификатором,
				   значит, следующий контрол - сам editbox, запомним его */
				this.hIdent = hChild = this.api.GetWindow(hChild, GW_HWNDNEXT);
				continue;
			}			
			if ("&Синоним:" == title) { 
				/* нашли лейбу с подписью к editbox'у с cинонимом,
				   значит, следующий контрол - сам editbox, запомним его */
				this.hSynonym = hChild = this.api.GetWindow(hChild, GW_HWNDNEXT);
				continue;
			}

			// рекурсивно обойдем дочерние элементы
			this.findOurHandlers(hChild);
		}						 
	},

	// генерирует и устанавливает синоним на основе идентификатора
	getSynonymById: function ()
	{
		this.hIdent		= null;
		this.hSynonym	= null;

		// получаем активное окно
		var hWnd = this.api.GetForegroundWindow();
		if (!hWnd) return;

		// поиск хэндлов эдит-боксов идентификатора и синонима 
		// среди всех контролов активного окна
		this.findOurHandlers(hWnd);

		// если хэндлы найдены, генерим синоним на основе идентификатора
		if (this.hIdent && this.hSynonym) {
			var synonym = this.genSynonym(this.svc.GetWindowText(this.hIdent, false));
			if (synonym) {				
				this.svc.SetWindowText(this.hSynonym, synonym);
			}
		}		
	}
}

// макросы

function СформироватьСиноним() { CoreObj.getSynonymById() }

// сохранен, чтобы не надо было переназначать макросы, 
// назначенные при использовании предыдущей версии
function GetSinonimByName() { CoreObj.getSynonymById() }

CoreObj.init();

