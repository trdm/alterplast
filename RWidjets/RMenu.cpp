// RMenu.cpp: implementation of the CRMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RMenu.h"
#include "MenuItem.h"

/////////////////////////////////////////////////////////////////////////////
extern CMenuItem theMenu;
extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;
extern CMapStringToPtr theMapDefMenuItem;
extern stDefMenuItems theDefMenuItems[];

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMenu) 
    RBL_METH(Add,			"Добавить",				3,	DefaultValue2)
//.<destination>Добавляет пункт в динамическое меню.
//.<syntax>Добавить(пТип, пИднт, пЗаголовок);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пТип
	//.<@destination>Тип добавляемого пункта меню
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>1 - подменю, 2 - разделитель
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор объекта RMenuItem
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>пЗаголовок
	//.<@destination>Строка, отображаемая в меню. Параметр можно опустить, если добавляется разделитель.
	//.<@optional>1
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<descr>Идентификтор должен быть уникальным на текущем уровне меню. 
//.<descr>Для обновления динамического меню следует выполнить метод Обновить. Это сделано для минимизации вычислений при массовом добавлении пунктов меню.
    RBL_METH(AddMenu,		"ДобавитьМеню",			2,	NoDefaultValue)
//.<destination>Добавляет подменю
//.<syntax>ДобавитьМеню(пИднт, пЗаголовок);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор объекта RMenuItem
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>пЗаголовок
	//.<@destination>Строка, отображаемая в меню
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<descr>Идентификтор должен быть уникальным на текущем уровне меню. 
//.<descr>Для обновления динамического меню следует выполнить метод Обновить. Это сделано для минимизации вычислений при массовом добавлении пунктов меню.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>лМеню1.ДобавитьКоманду("Кмнд1", "Первая команта", "Сообщить(""Скрипт Первая команда"");");
//.<sample>глМеню.Обновить();

    RBL_METH(AddSeparator,	"ДобавитьРазделитель",	1,	NoDefaultValue)
//.<destination>Добавляет разделитель меню
//.<syntax>ДобавитьРазделитель(пИднт);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор объекта RMenuItem
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<descr>Идентификтор должен быть уникальным на текущем уровне меню. 
//.<descr>Для обновления динамического меню следует выполнить метод Обновить. Это сделано для минимизации вычислений при массовом добавлении пунктов меню.
    RBL_METH(Delete,		"Удалить",				1,	NoDefaultValue)
//.<destination>Удаляет пункт меню
//.<syntax>Удалить(пИднт);
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор объекта RMenuItem
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<descr>Удаляет пункт меню с идентификатором пИднт из динамического меню. Удаление отображается на главное меню программы сразу. Если удаляется подменю, то удаляются и все подчиненные элементы меню.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>глМеню.Удалить("Кмнд1");
    RBL_METH(DeleteAll,		"УдалитьВсе",			0,	NULL)
//.<destination>Очищает динамическое меню.
//.<syntax>УдалитьВсе();
//.<return>
//.<descr>Удаляет все пункты димнамического меню в главном меню программы вместе со всеми подчиненными. Удаление отображается на главное меню программы сразу.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>глМеню.Удалитьвсе();
    RBL_METH(Get,			"Получить",				1,	NoDefaultValue)
//.<destination>Получить объект RMenuItem по идентификатору или по порядковому номеру
//.<syntax>Получить(пИднтOrNum);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пИднтOrNum
	//.<@destination>Идентификатор объекта RMenuItem или порядковый номер.
	//.<@optional>0
	//.<@type>Строка, Число
	//.<@default>
	//.<@values>Порядковый номер от 1 до Количество()
//.<descr>Возвращает созданный ранее объект, соответствующий пункту динамического меню.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>глМеню.Обновить();
//.<sample>глМеню=0;
//.<sample>
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.Получить("Кмнд1");
//.<sample>лМеню1.ДобавитьКоманду("Кмнд1", "Первая команта", "Сообщить(""Скрипт Первая команда"");");
//.<sample>глМеню.Обновить();
//.<sample>
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>Для А=1 По глМеню.Количество() Цикл
//.<sample>	лМеню=глМеню.Получить(А);
//.<sample>КонецЦикла;
    RBL_METH(GetSystem,		"ПолучитьСистемное",	1,	NoDefaultValue)
//.<destination>Получить системное меню Файл, Операции, Сервис, Помощь.
//.<syntax>ПолучитьСистемное(пИднт);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор системного меню.
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>Файл, Операции, Сервис, Помощь
//.<descr>Возвращает объект, соответствующий системному пункту основного меню программы. По-умолчанию возвращается объект, не содержащий пункты меню. Т.е. если получите системное меню Файл, то в нем не будет пунктов Открыть, Сохранить и т.д. Когда вы добавляете свои пункты, они появляются наряду с системными. Это примерно то же самое, когда вы в конфигураторе добавляете свои пункты к системному меню. 
    RBL_METH(DeleteSystem,		"УдалитьСистемное",	1,	NoDefaultValue)
//.<destination>Удалить системное меню Файл, Операции, Сервис, Помощь или пункт главного меню, определенный в конфигураторе.
//.<syntax>УдалитьСистемное(пИднт);
//.<return>Число
//.<param>
	//.<@name>пИднт
	//.<@destination>Идентификатор системного меню.
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>Файл, Операции, Сервис, Помощь
//.<descr>
    RBL_METH(Count,			"Количество",			0,	NULL)
//.<destination>Возвращает количество пунктов меню.
//.<syntax>Количество();
//.<return>Число
//.<descr>Возвращает количество пунктов динамического меню в главном меню программы.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>Для А=1 По глМеню.Количество() Цикл
//.<sample>	лМеню=глМеню.Получить(А);
//.<sample>КонецЦикла;
    RBL_METH(RedrawMenu,	"Обновить",	0,	NULL)
//.<destination>Обновляет главное меню программы.
//.<syntax>Обновить();
//.<return>
//.<descr>Обновляет главное меню программы после добавлений, удалений или каких-то других изменений пунктов динамического меню.
//.<sample>
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>глМеню.Обновить();
    RBL_METH(SendMessage,		"ПослатьКоманду",		1,	NoDefaultValue)
//.<destination>Послать комаду программе.
//.<syntax>ПослатьКоманду(пКоманда);
//.<param>
	//.<@name>пКоманда
	//.<@destination>Числовой идентификатор команды.
	//.<@optional>0
	//.<@type>Число
//.<descr>Посылает системе одну из команд, которые посылаются системе при выборе пунктов меню, нажатии кнопок панелей инструментов, нажатии горячих клавиш, например, Ctrl+N.
//.<sample>// Глобальный модуль
//.<sample>Процедура ПриВыбореСпр()
//.<sample>	лМеню=СоздатьОбъект("Меню");
//.<sample>	лМеню.ПослатьКоманду(33726);
//.<sample>КонецПроцедуры
//.<sample>//
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>мнСпр=глМеню.ДобавитьМеню("Спр", "Справочники");
//.<sample>мнСпр.ДобавитьКоманду("Спр1", "Справочник...", "ПриВыбореСпр()");
//.<sample>мнСпр.ДобавитьКоманду("Спр2", "Клиенты", "ОткрытьФорму(""Справочник.Клиенты"")");
//.<sample>глМеню.Обновить();
    RBL_METH(LoadInterface,		"ЗагрузитьИнтерфейс",		1,	NoDefaultValue)
//.<destination>Загружает стандартный интерфейс из конфигурации.
//.<syntax>ЗагрузитьИнтерфейс(пНазвание);
//.<param>
	//.<@name>пНазвание
	//.<@destination>Название интерфейса, как оно определено в конфигурации.
	//.<@optional>0
	//.<@type>Строка
//.<descr>Загружает меню из интерфейса, определенного в конфигурации. При загрузке полностью повторяется структура меню интерфейса. 
//.<descr>Внимание! Загруженное таким образом меню будет работать с рядом ограничений. Правильно работают все пункты меню, действие которых можно выполнить из стандартного меню Операции, и пункты меню, которые ссылаются на конкретные формы справочников, документов и т.д. Остальные пункты могут работать не правильно или не работать совсем.
//.<sample>мнМеню=СоздатьОбъект("Меню");
//.<sample>мнМеню.УдалитьВсе();
//.<sample>мнМеню.ЗагрузитьИнтерфейс("Администратор");
//.<sample>мнМеню.Обновить();
    RBL_METH(GetList,		"ПолучитьСписокИнтерфейсов",	0,	NULL)
//.<destination>Возвращает список интерфейсов.
//.<syntax>ПолучитьСписокИнтерфейсов();
//.<return>СписокЗначений
//.<descr>Возвращает объект типа список значений с наименованиями интерфейсов.

/**/RBL_METH(Test,	"Тест",	1,	NULL)
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMenu)
END_BL_PROP_MAP()

////EVENT(OnGetCommand, "ПриПолученииКоманды")
//.<destination>Событие возникает при выборе пункта меню, нажатии кнопки панели инструментов, нажатии горячих клавиш.
//.<syntax>Процедура ПриПолученииКоманды(пКоманда, пЗаблокироватьКоманду)
//.<syntax>КонецПроцедуры
//.<param>
	//.<@name>пКоманда
	//.<@destination>Идентификатор команды
	//.<@optional>1
	//.<@type>Число
//.<param>
	//.<@name>пЗаблокироватьКоманду
	//.<@destination>Если в теле процедуры присвоить этому параметру занчение 1, то стандартная обработка команды системой будет отменена.
	//.<@optional>1
	//.<@type>Число
	//.<@values>1, 0
//.<descr>Процедура может быть размещена в глобальном или любом локальном модуле. При возникновении события процедура запускается в активном локальном контексте, если он есть, и всегда в глобальном. 
//.<descr>Процедура позволяет перехватывать выбор пунктов меню, нажатие горячих клавиш, нажатие кнопок панелей инструментов в главном окне программы и окнах документов, журналов, отчетов. Вы можете отменить выполнение системой стандартных действий присвоив параметру пЗаблокироватьКоманду значение 1.
//.<sample>Процедура ПриПолученииКоманды(пКоманда, пЗаблокироватьКоманду)
//.<sample>	Сообщить("Получена команда "+пКоманда);
//.<sample>	
//.<sample>	Если пКоманда=32805 Тогда
//.<sample>		Предупреждение("Нельзя блокировать сессию!");
//.<sample>		пЗаблокироватьКоманду=1;
//.<sample>	КонецЕсли;
//.<sample>КонецПроцедуры

IMPLEMENT_MY_CONTEXT(CRMenu, "RMenu", "Меню", 1, NULL, NULL, -1);
//.<destination>Объект типа RMenu предназначен для добавления динамического меню к основному меню программы. Объект является интерфейсом для управления меню, но не собственно меню. Если создать одновременно несколько экзепляров RMenu, то они все будут управлять одним и тем же динамическим меню. Удаление экзепляра RMenu не влечет за собой удаление динамического меню из основного меню программы.
//.<destination>Каждый пункт динамического меню является объектом типа RMenuItem. Каждый пункт динамического меню в главном меню является объектом RMenuItem типа подменю. Каждый RMenuItem в свою очередь может содежать несколько подчиненных объектов RMenuItem, которые могут быть пунктом меню, подменю, разделителем. Таким образом, возможно создание иерархического меню. 
//.<destination>Каждый RMenuItem типа пункт меню ассоциируется со скриптом на внутреннем языке 1С, который выполняется при выборе этого пункта.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMenu::CRMenu()
{
	m_pMenuItem = &theMenu;
}

CRMenu::~CRMenu()
{
}

BOOL CRMenu::funcAdd(CValue& RetVal, CValue **params)
{
	enum ItemType itType = (enum ItemType)(int)(params[0]->GetNumeric().GetDouble()-1);

	switch(itType){
	case itSeparator:
		{
			funcAddSeparator(RetVal, params);
			break;
		}
	case itMenu:
		{
			funcAddMenu(RetVal, params);
			break;
		}
	default:
		ERR1(IDS_ERR_WRONGTYPE2, 0)
	};

	return TRUE;
}

BOOL CRMenu::funcAddMenu(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())
	
	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetMenu(params[1]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenu::funcAddSeparator(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())
	
	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetSeparator();

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenu::funcGetSystem(CValue& RetVal, CValue **params)
{
	CString str = params[0]->GetString();
	str.MakeUpper();
	
	stDefMenuItems* pDefMenuItem = (stDefMenuItems*)theMapDefMenuItem[str];

	if(pDefMenuItem)
		RetVal.AssignContext(pDefMenuItem->pMenuItem->GetContext());
	else
		ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());

	return TRUE;
}

BOOL CRMenu::funcDeleteSystem(CValue& RetVal, CValue **params)
{
	CString str = params[0]->GetString();
	str.MakeUpper();

	CAppFrame* pApp = (CAppFrame*)AfxGetApp();
	CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
	CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
	int nSubMenu = 0;

	stDefMenuItems* pDefMenuItem = (stDefMenuItems*)theMapDefMenuItem[str];

	if(pDefMenuItem)
	{
		nSubMenu = pV7Menu1->FindSubMenu(pDefMenuItem->Name);
		if(nSubMenu>=0)
			pV7Menu1->DeleteMenu(nSubMenu, MF_BYPOSITION);
		
		nSubMenu = pV7Menu2->FindSubMenu(pDefMenuItem->Name);
		if(nSubMenu>=0)
			pV7Menu2->DeleteMenu(nSubMenu, MF_BYPOSITION);
	}
	else
	{
		nSubMenu = pV7Menu1->FindSubMenu(params[0]->GetString());
		if(nSubMenu>=0)
		{
			pV7Menu1->DeleteMenu(nSubMenu, MF_BYPOSITION);

			nSubMenu = pV7Menu2->FindSubMenu(params[0]->GetString());
			if(nSubMenu>=0)
				pV7Menu2->DeleteMenu(nSubMenu, MF_BYPOSITION);
		}
		else
			ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());
	}

	return TRUE;
}

BOOL CRMenu::funcGet(CValue& RetVal, CValue **params)
{
	if(params[0]->GetTypeCode() == 1)
	{
		int index = params[0]->GetNumeric();
		if(index<=0 || index>m_pMenuItem->mListSubMenu.GetCount()) ERR1(IDS_WRONGINDEX, index);
		
		POSITION pos = m_pMenuItem->mListSubMenu.FindIndex(index-1);
		CMenuItem* pMenu = m_pMenuItem->mListSubMenu.GetAt(pos);
		RetVal.AssignContext(pMenu->GetContext());
	}
	else
	{
		if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());
		
		CMenuItem* pMenu = m_pMenuItem->Get(params[0]->GetString());
		RetVal.AssignContext(pMenu->GetContext());
	}
	return TRUE;
}

BOOL CRMenu::funcDelete(CValue& RetVal, CValue **params)
{
	if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString())

	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->RemoveAt(params[0]->GetString());
	m_pMenuItem->Build();
	RetVal = 1;

	return TRUE;
}

BOOL CRMenu::funcDeleteAll(CValue& RetVal, CValue **params)
{
	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->ClearList();

	for(int i=0; i<4; i++)
		theDefMenuItems[i].pMenuItem->ClearList();

	RetVal = 1;
	
	return TRUE;
}

BOOL CRMenu::funcRedrawMenu(CValue& RetVal, CValue **params)
{
	try{
	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->Build();

	CMDIChildWnd* wnd = GetMainFrame()->MDIGetActive();
	if(wnd) 
	{
		wnd->ActivateFrame(SW_HIDE);
		wnd->ActivateFrame(SW_SHOW);
	};

	AfxGetApp()->GetMainWnd()->DrawMenuBar();

	RetVal = 1;
	}catch(...){MSG("ERRRORRR funcRedrawMenu")}

	return TRUE;
}

BOOL CRMenu::funcSendMessage(CValue& RetVal, CValue **params)
{
	AfxGetApp()->GetMainWnd()->PostMessage(WM_COMMAND, params[0]->GetNumeric(), 0);
	return TRUE;
}

BOOL CRMenu::funcGetList(CValue& RetVal, CValue **params)
{
	CBLContext* pValueList = CBLContext::CreateInstance("СписокЗначений");
	int iNumMethod = pValueList->FindMethod("AddValue");
	

	CString strMDFileName = static_cast<CApp7*>(AfxGetApp())->GetProps()->GetStringProp(0x08);

	IStoragePtr pStgRoot, pStgUserDef, pStrInterfaces, pStrTheInterface;

	IStorage* pStorage = NULL;
	HRESULT hr;
	USES_CONVERSION;

	hr = ::StgOpenStorage(
		T2COLE(strMDFileName),
		NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStgRoot.Attach(pStorage, false);

	hr = pStgRoot->OpenStorage(T2COLE("UserDef"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStgUserDef.Attach(pStorage, false);

	hr = pStgUserDef->OpenStorage(T2COLE("Page.1"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStrInterfaces.Attach(pStorage, false);

	IStreamPtr pStream;
	IStream *pIStr = NULL;

	hr = pStrInterfaces->OpenStream(T2COLE("Container.Contents"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStream.Attach(pIStr, false);    

	STATSTG statstg;
	hr = pStream->Stat(&statstg, STATFLAG_NONAME);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	ULONG cbStreamSize = statstg.cbSize.LowPart;
	CString strCont;
	ULONG cbRead;
	hr = pStream->Read(strCont.GetBufferSetLength(cbStreamSize), cbStreamSize, &cbRead);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	strCont.ReleaseBuffer();

	int i = 0;
	CString strName;
	CItemList lst;
	// Ищем интерфейс по имени
	lst.LoadFromString(strCont, &i);
	for(int j = 1; j < lst.GetNItems(); j++)
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);

		CValue* pValue = new CValue(pItem2->GetFunctor());
		pValueList->CallAsProc(iNumMethod, &pValue);
	}

	RetVal.AssignContext(pValueList);
	
	return TRUE;
}

BOOL CRMenu::funcTest(CValue& RetVal, CValue **params)
{

	//CGetDoc7* pDoc7 = CGetDlg::GetTopmostDocunent();
	//MSG1("%x", pDoc7);
	

	CWinApp* pApp = AfxGetApp();
	CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
	CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
/*

	CMenu* pMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	while(pMenu->GetMenuItemCount()>0)
		pMenu->DeleteMenu(0, MF_BYPOSITION);
	
	CMenu7::CopySMenu(pV7Menu1, pMenu, 0);
	AfxGetApp()->GetMainWnd()->DrawMenuBar();
*/

	DUMPMENU(pV7Menu1)
	DUMPMENU(pV7Menu2)

	RetVal = 1;
	
	return FALSE;
}

BOOL CRMenu::funcCount(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->mListSubMenu.GetCount();
	return TRUE;
}


BOOL CRMenu::NoDefaultValue(int nParam, CValue* param) const
{
	//*param = -1;
	//*param = CDate(0,0,0);
	return FALSE;
}

BOOL CRMenu::DefaultValue2(int nParam, CValue* param) const
{
	if(nParam>1)
	{
		*param = "";
		return TRUE;
	}
	else 
		return FALSE;
}

void CRMenu::LoadMenuFromMD(IStream* pStream, CMenuItem* pParentMenuItem, CMap<WORD, WORD, LoadedItemData, LoadedItemData>& mapCmdID2Command, CString strID="")
{
	HRESULT hr;
	ULONG cbRead;
	DWORD wdCntMenuItems;
	DWORD wdCommandID;
	BYTE btTextSize;
	CString strText;
	CString strNewID;
	CString strCode1C;
	CMenuItem* pMenuItem;

	hr = pStream->Read(&wdCntMenuItems, 4, &cbRead);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл");

	for(int i=0; i<wdCntMenuItems; i++)
	{
		hr = pStream->Read(&wdCommandID, 4, &cbRead);
		if(FAILED(hr)) ERR("Не удалось прочитать MD файл");
		
		if(wdCommandID) // Если 0, то это разделитель 
		{
			hr = pStream->Read(&btTextSize, 1, &cbRead);
			if(FAILED(hr)) ERR("Не удалось прочитать MD файл");
			
			hr = pStream->Read(strText.GetBufferSetLength(btTextSize), btTextSize, &cbRead);
			if(FAILED(hr)) ERR("Не удалось прочитать MD файл");
			
			strText.ReleaseBuffer();
			//MSG4("%2d %X %s %s", i+1, wdCommandID, strText, mapCmdID2Command[wdCommandID]);
			
			if(wdCommandID == 0xFFFFFFFF)
			{
				if(!strText.CompareNoCase("__1CFixName_File__"))
				{
					pMenuItem = theDefMenuItems[0].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[0].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Oper__"))
				{
					pMenuItem = theDefMenuItems[1].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[1].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Service__"))
				{
					pMenuItem = theDefMenuItems[2].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[2].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Help__"))
				{
					pMenuItem = theDefMenuItems[3].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[3].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else
				{
					strNewID.Format("%s\\%d", strID, i);
					pMenuItem = pParentMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetMenu(strText);

					LoadMenuFromMD(pStream, pMenuItem, mapCmdID2Command, strNewID);
				}
			}
			else
			{
				LoadedItemData lddItemData;
				if(mapCmdID2Command.Lookup(wdCommandID, lddItemData))
				{
					strNewID.Format("%s\\%d", strID, i);
					pMenuItem = pParentMenuItem->AddMenuItem(strNewID);

					pMenuItem->SetCommand(strText, lddItemData.Code);
					pMenuItem->m_strTips = lddItemData.Tips;
				}
			}
		}
		else
		{
			strNewID.Format("%s\\%d", strID, i);
			pMenuItem = pParentMenuItem->AddMenuItem(strNewID);
			pMenuItem->SetSeparator();
		}
	}
}

void CRMenu::LoadSubInterface(CString strSubIntName, CString& strTheInterfaceStrName, CItemList& lst, IStoragePtr pStrInterfaces, CList<CString, CString>& lstExclCommands)
{
	HRESULT hr;
	IStorage* pStorage = NULL;
	IStream *pIStr = NULL;
	ULONG cbRead;
	USES_CONVERSION;
	
	// Ищем подчиненный интерфейс по имени
	for(int j = lst.FindGenericItem("SubUsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("SubUsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!strSubIntName.CompareNoCase(pItem2->GetFunctor()))
			strTheInterfaceStrName=pItem1->GetFunctor();
	}
	
	if(strTheInterfaceStrName.IsEmpty())
		ERR1("Интерфейс %s не найден.", strSubIntName);
	
	// Подчиненный интерфейс найден
	IStoragePtr pStrTheInterface;
	IStreamPtr pStreamCmd;
	
	hr = pStrInterfaces->OpenStorage(T2COLE((LPCTSTR)strTheInterfaceStrName), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	pStrTheInterface.Attach(pStorage, false);
	
	hr = pStrTheInterface->OpenStream(T2COLE("Commands"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	pStreamCmd.Attach(pIStr, false);    
	
	DWORD dwCntCmd;
	BYTE bt[9];
	CString strParentIntName;
	
	// Получаем имя родительского интерфейса
	hr = pStreamCmd->Read(&bt, 5, &cbRead);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	hr = pStreamCmd->Read(strParentIntName.GetBufferSetLength(bt[4]), bt[4], &cbRead);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	strParentIntName.ReleaseBuffer();

	//MSG2("%s ??%s", strParentIntName, strTheInterfaceStrName);
	BOOL bMainInterfaceIsFound = FALSE;

	for(j = lst.FindGenericItem("UsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("UsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!strParentIntName.CompareNoCase(pItem2->GetFunctor()))
		{
			strTheInterfaceStrName=pItem1->GetFunctor();
			bMainInterfaceIsFound = TRUE;
			//MSG2("%s->%s", strParentIntName, strTheInterfaceStrName);
		}
	}

	if(!bMainInterfaceIsFound)
		LoadSubInterface(strParentIntName, strTheInterfaceStrName, lst, pStrInterfaces, lstExclCommands);
	
	hr = pStreamCmd->Read(&bt, 9, &cbRead);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	// Количество записей
	hr = pStreamCmd->Read(&dwCntCmd, 4, &cbRead);
	if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
	
	for(int i=0; i<dwCntCmd; i++)
	{
		hr = pStreamCmd->Read(&bt, 1, &cbRead);
		if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
		CString strExclCmd;
		hr = pStreamCmd->Read(strExclCmd.GetBufferSetLength(bt[0]+1), bt[0]+1, &cbRead);
		if(FAILED(hr)) ERR("Не удалось прочитать MD файл.");
		strExclCmd.ReleaseBuffer();
		
		strExclCmd.TrimLeft();
		strExclCmd.TrimRight();
		
		lstExclCommands.AddTail(strExclCmd);
		//MSG1("lstExclCommands.AddTail %s", strExclCmd);
	}
}

BOOL CRMenu::funcLoadInterface(CValue& RetVal, CValue **params)
{
	CString strMDFileName = static_cast<CApp7*>(AfxGetApp())->GetProps()->GetStringProp(0x08);

	IStoragePtr pStgRoot, pStgUserDef, pStrInterfaces, pStrTheInterface;

	IStorage* pStorage = NULL;
	HRESULT hr;
	USES_CONVERSION;

	hr = ::StgOpenStorage(
		T2COLE(strMDFileName),
		NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStgRoot.Attach(pStorage, false);

	hr = pStgRoot->OpenStorage(T2COLE("UserDef"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStgUserDef.Attach(pStorage, false);

	hr = pStgUserDef->OpenStorage(T2COLE("Page.1"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStrInterfaces.Attach(pStorage, false);

	IStreamPtr pStream;
	IStream *pIStr = NULL;

	hr = pStrInterfaces->OpenStream(T2COLE("Container.Contents"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStream.Attach(pIStr, false);    

	STATSTG statstg;
	hr = pStream->Stat(&statstg, STATFLAG_NONAME);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	ULONG cbStreamSize = statstg.cbSize.LowPart;
	CString strCont;
	ULONG cbRead;
	hr = pStream->Read(strCont.GetBufferSetLength(cbStreamSize), cbStreamSize, &cbRead);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	strCont.ReleaseBuffer();

	CList<CString, CString> lstExclCommands;
	CString strTheInterfaceStrName = "";
	int i = 0;
	CItemList lst;
	// Ищем интерфейс по имени
	lst.LoadFromString(strCont, &i);
	for(int j = lst.FindGenericItem("UsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("UsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!params[0]->GetString().CompareNoCase(pItem2->GetFunctor()))
			strTheInterfaceStrName=pItem1->GetFunctor();
	}

	if(strTheInterfaceStrName.IsEmpty())
		LoadSubInterface(params[0]->GetString(), strTheInterfaceStrName, lst, pStrInterfaces, lstExclCommands);
	

	hr = pStrInterfaces->OpenStorage(T2COLE((LPCTSTR)strTheInterfaceStrName), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("pStrInterfaces Не удалось прочитать файл: %s", strMDFileName);
	pStrTheInterface.Attach(pStorage, false);


	IStreamPtr pStreamCmd;
	hr = pStrTheInterface->OpenStream(T2COLE("Commands"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStreamCmd.Attach(pIStr, false);    

	DWORD dwCntCmd;
	LARGE_INTEGER offset;

	offset.QuadPart = 13;
	hr = pStreamCmd->Seek(offset, STREAM_SEEK_SET, NULL);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	hr = pStreamCmd->Read(&dwCntCmd, 4, &cbRead);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);


	CMap<WORD, WORD, CString, CString> mapCmdID2Command;
	CMap<WORD, WORD, LoadedItemData, LoadedItemData> mapCmdID2Command2;
	WORD wCmdID;
	BYTE btSize[4];
	CString strCmd, strFormName, strTips;
	LoadedItemData lddItemData;
	int iSize;

	for(i=0; i<dwCntCmd; i++)
	{
		// Command ID
		hr = pStreamCmd->Read(&wCmdID, 2, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		// Размер пояснения
		// Читаем первые три байта
		hr = pStreamCmd->Read(&btSize, 3, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);

		if(btSize[2]!=0x00)
		{
			iSize = btSize[2]; // тогда третий байт есть размер поля
		}
		else
		{
			hr = pStreamCmd->Read(&btSize, 1, &cbRead);
			if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);

			if(btSize[0]==0xFF)
			{
				hr = pStreamCmd->Read(&btSize, 2, &cbRead);
				if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
				iSize = 256*btSize[1]+btSize[0];
			}
			else
				iSize = btSize[0];
		}

		// читаем пояснение
		hr = pStreamCmd->Read(strTips.GetBufferSetLength(iSize), iSize, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		strTips.ReleaseBuffer();
		lddItemData.Tips = strTips;

		// размер второй части пояснения
		hr = pStreamCmd->Read(&btSize, 1, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		iSize = btSize[0];

		if(iSize)
		{
			offset.QuadPart = iSize;
			hr = pStreamCmd->Seek(offset, STREAM_SEEK_CUR, NULL);
			if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		}

		// читаем размер след поля
		hr = pStreamCmd->Read(&btSize, 3, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);

		if(btSize[2]!=0x00)
		{
			iSize = btSize[2]; // тогда третий байт есть размер поля
		}
		else
		{
			hr = pStreamCmd->Read(&btSize, 1, &cbRead);
			if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);

			if(btSize[0]==0xFF)
			{
				hr = pStreamCmd->Read(&btSize, 2, &cbRead);
				if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
				iSize = 256*btSize[1]+btSize[0];
			}
			else
				iSize = btSize[0];
		}

		// читаем команду
		hr = pStreamCmd->Read(strCmd.GetBufferSetLength(iSize), iSize, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		strCmd.ReleaseBuffer();

		//MSG4("%3d %X %x %s", i, wCmdID, iSize, strCmd);

		// размер второй части команды
		hr = pStreamCmd->Read(&btSize, 1, &cbRead);
		if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
		iSize = btSize[0];

		strFormName.Empty();
		if(iSize)
		{
			hr = pStreamCmd->Read(strFormName.GetBufferSetLength(iSize), iSize, &cbRead);
			if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
			strFormName.ReleaseBuffer();
		}
			
		//MSG4("%3d %X %s :: %s", i, wCmdID, strCmd, strFormName);

		// Интерпретация 
		CString strWord[3];
		for(int j=0; j<3; j++) strWord[j].Empty();
		int lastpos = 0, pos = 0;
		for(j=0; j<3; j++)
		{
			pos = strCmd.Find(".", lastpos);

			if(pos < 0)
			{
				strWord[j] = strCmd.Mid(lastpos);
				j = 77;
			}
			else
			{
				strWord[j] = strCmd.Mid(lastpos, pos - lastpos);
				lastpos = pos+1;
			}
		}

		CString strCode1C;
		if(!strWord[0].CompareNoCase("Константа"))
		{
			strCode1C="#33727";
		}
		else if(!strWord[0].CompareNoCase("Справочник"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33726";
			else
				strCode1C.Format("ОткрытьФорму(\"Справочник.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("Документ"))
		{
			if(!strWord[1].CompareNoCase("Ввести"))
				strCode1C="Предупреждение(\"Операция не поддерживается.\")";
			else
				strCode1C.Format("ОткрытьФорму(\"Документ.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("Журнал"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33725";
			else if(!strWord[1].CompareNoCase("Открыть полный"))
				strCode1C="#33725";
			else if(!strWord[1].CompareNoCase("Открыть прочие"))
				strCode1C="#33725";
			else
				strCode1C.Format("ОткрытьФорму(\"Журнал.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("Отчет"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33856";
			else if(!strWord[1].CompareNoCase("ОткрытьВнешний"))
				strCode1C="#57601";
			else
				strCode1C.Format("ОткрытьФорму(\"Отчет.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("Обработка"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33859";
			else
				strCode1C.Format("ОткрытьФорму(\"Обработка.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("ЖурналРасчетов"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("Открыть по объекту"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("Сменить период"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("Открыть по документу"))
				strCode1C="#33857";
			else
				strCode1C.Format("ОткрытьФорму(\"ЖурналРасчетов.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("Календарь"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33862";
			else if(!strWord[1].CompareNoCase("Праздники"))
				strCode1C="#33752";
			else
				strCode1C="#33862";
		}
		else if(!strWord[0].CompareNoCase("ПланСчетов"))
		{
			if(!strWord[1].CompareNoCase("Открыть"))
				strCode1C="#33860";
			else
				strCode1C.Format("ОткрытьФорму(\"ПланСчетов.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("Операция"))
		{
			if(!strWord[1].CompareNoCase("ОткрытьЖурнал"))
				strCode1C="#33861";
			else if(!strWord[1].CompareNoCase("Ввести"))
				strCode1C="#33861";
			else if(!strWord[1].CompareNoCase("ВвестиТиповуюОперацию"))
				strCode1C="#33786";
			else if(!strWord[1].CompareNoCase("ТиповыеОперации"))
				strCode1C="#33786";
		}
		else if(!strWord[0].CompareNoCase("Проводка"))
		{
			if(!strWord[1].CompareNoCase("ОткрытьЖурнал"))
				strCode1C="#33788";
			else if(!strWord[1].CompareNoCase("КорректныеПроводки"))
				strCode1C="#33764";
		}
		else if(!strWord[0].CompareNoCase("Задача"))
		{
			if(!strWord[1].CompareNoCase("УправлениеОперативнымиИтогами"))
				strCode1C="#33665";
			else if(!strWord[1].CompareNoCase("УправлениеБухгалтерскимиИтогами"))
				strCode1C="#33761";
			else if(!strWord[1].CompareNoCase("УдалениеПомеченныхОбъектов"))
				strCode1C="#32897";
			else if(!strWord[1].CompareNoCase("ПоискСсылокНаОбъекты"))
				strCode1C="#32899";
			else if(!strWord[1].CompareNoCase("ПроведениеДокументов"))
				strCode1C="#32924";
			else if(!strWord[1].CompareNoCase("Выполнить"))
				strCode1C=strFormName;
		}

		lddItemData.Code = strCode1C;

		
		if(lstExclCommands.Find(strCmd) == NULL)
		{
			//MSG2("+ %X %s count=%d", wCmdID, strCmd, lstExclCommands.GetCount());
			//mapCmdID2Command.SetAt(wCmdID, strCode1C);
			mapCmdID2Command2.SetAt(wCmdID, lddItemData);
		}
	}

	IStreamPtr pStreamInt;
	hr = pStrTheInterface->OpenStream(T2COLE("Page.1"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);
	pStreamInt.Attach(pIStr, false);    

	DWORD wdHeader;
	hr = pStreamInt->Read(&wdHeader, sizeof(wdHeader), &cbRead);
	if(FAILED(hr)) ERR1("Не удалось прочитать файл: %s", strMDFileName);

	LoadMenuFromMD(pStreamInt, &theMenu, mapCmdID2Command2);
	
	return true;    
}

