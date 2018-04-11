// RMenuItem.cpp: implementation of the CRMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RMenuItem.h"
#include "MenuItem.h"

/////////////////////////////////////////////////////////////////////////////
extern CMenuItem theMenu;
extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMenuItem) 
    RBL_METH(Add,			"Добавить",				4,	DefaultValue2)
//.<destination>Добавляет пункт в динамическое меню.
//.<syntax>Добавить(пТип, пИднт, пЗаголовок, пСкрипт);
//.<return>Объект типа RMenuItem
//.<param>
	//.<@name>пТип
	//.<@destination>Тип добавляемого пункта меню
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>1 - подменю, 2 - разделитель, 3 - команда
//.<param>
	//.<@name>пИднт	//.<@destination>Идентификатор объекта RMenuItem
	//.<@optional>0
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>пЗаголовок
	//.<@destination>Строка, отображаемая в меню
	//.<@optional>1
	//.<@type>Строка
	//.<@default>
	//.<@values>Параметр можно опустить, если добавляет разделитель.
//.<param>
	//.<@name>пСкрипт
	//.<@destination>Текст скрипта, выполняемого при выборе пункта меню
	//.<@optional>1
	//.<@type>Строка
	//.<@default>
	//.<@values>Параметр можно опустить, если добавляется разделитель или подменю.
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
    RBL_METH(AddCommand,	"ДобавитьКоманду",		3,	DefaultValue2)
//.<destination>Добавляет команду
//.<syntax>ДобавитьКоманду(пИднт, пЗаголовок, пСкрипт);
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
//.<param>
	//.<@name>пСкрипт
	//.<@destination>Текст скрипта, выполняемого при выборе пункта меню
	//.<@optional>1
	//.<@type>Строка
	//.<@default>
	//.<@values>
//.<descr>Метод добавляет пункт меню, при выборе которого выполняется указанный скрипт. Скрипт выполняется в локальном контексте текущего активного окна или в глобальном контексте.
//.<descr>Идентификтор должен быть уникальным на текущем уровне меню. 
//.<descr>Для обновления динамического меню следует выполнить метод Обновить. Это сделано для минимизации вычислений при массовом добавлении пунктов меню.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>лМеню1.ДобавитьКоманду("Кмнд1", "Первая команта", "Сообщить(""Скрипт Первая команда"");");
//.<sample>лМеню1.ДобавитьКоманду("Кмнд2", "Вторая команта", "глКоманда2()");
//.<sample>глМеню.Обновить();
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
//.<descr>Удаляет пункт меню с идентификатором пИднт из динамического меню. Если удаляется подменю, то удаляются и все подчиненные элементы меню.
//.<descr>Для обновления динамического меню следует выполнить метод Обновить.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.ДобавитьМеню("Кмнд1", "&МЕНЮ1");
//.<sample>глМеню.Удалить("Кмнд1");
    RBL_METH(DeleteAll,		"УдалитьВсе",			0,	NULL)
//.<destination>Очищает динамическое меню.
//.<syntax>УдалитьВсе();
//.<return>
//.<descr>Удаляет все пункты на текущем уровне димнамического меню месте со всеми подчиненными. 
//.<descr>Для обновления динамического меню следует выполнить метод Обновить.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.Получить("Кмнд1");
//.<sample>лМеню1.Удалитьвсе();
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
//.<sample>лМеню1=глМеню.Получить("Кмнд1");
//.<sample>Для А=1 По лМеню1.Количество() Цикл
//.<sample>	лМеню=лМеню1.Получить(А);
//.<sample>КонецЦикла;
    RBL_METH(Count,			"Количество",			0,	NULL)
//.<destination>Возвращает количество пунктов меню.
//.<syntax>Количество();
//.<return>Число
//.<descr>Возвращает количество пунктов динамического меню на текущем уровне.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лМеню1=глМеню.Получить("Кмнд1");
//.<sample>Для А=1 По лМеню1.Количество() Цикл
//.<sample>	лМеню=лМеню1.Получить(А);
//.<sample>КонецЦикла;
    RBL_METH(ReadOnly,			"Доступность",			1,	DefaultValueBase)
//.<destination>Установить доступность
//.<syntax>Доступность(пЗначение);
//.<return>Число
//.<param>
	//.<@name>пЗначение
	//.<@destination>Новое значение доступности
	//.<@optional>1
	//.<@type>Число
	//.<@default>
	//.<@values>1 - доступен, 0 - недоступен
//.<descr>Устанавливает доступность пункта меню для выбора. Недоступный пункт становится серым. Возвращает значение доступности до выполнения метода. Не работает для пунктов главного меню программы.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лПодменю=глМеню.Получить("Кмнд1");
//.<sample>лПункт.Получить("Пункт1");
//.<sample>лПункт.Доступность(0);
//.<sample>глМеню.Обновить();
    RBL_METH(Check,				"Пометка",				1,	DefaultValueBase)
//.<destination>Установить пометку
//.<syntax>Пометка(пЗначение);
//.<return>Число
//.<param>
	//.<@name>пЗначение
	//.<@destination>Новое значение
	//.<@optional>1
	//.<@type>Число
	//.<@default>
	//.<@values>1 - пометка установлена, 0 - пометка снята
//.<descr>Устанавливает пометку слева от пункта меню. Возвращает значение пометки до выполнения метода. Не работает для пунктов главного меню программы.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>лПодменю=глМеню.Получить("Кмнд1");
//.<sample>лПункт.Получить("Пункт1");
//.<sample>лПункт.Пометка(1);
//.<sample>глМеню.Обновить();
    RBL_METH(SetPicture,		"УстановитьКартинку",	3,	DefaultValue1)
//.<destination>Установить картину для пункта меню
//.<syntax>УстановитьКартинку(пКартинка, пНомер, пШирина);
//.<return>Число
//.<param>
	//.<@name>пКартинка
	//.<@destination>Картинка (иконка) для пункта меню
	//.<@optional>0
	//.<@type>Число или Стандартный для 1С объект Картинка 
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>пНомер
	//.<@destination>Индекс изображения в длинной картинке
	//.<@optional>1
	//.<@type>Число
	//.<@default>0
	//.<@values>
//.<param>
	//.<@name>пШирина
	//.<@destination>Ширина картинки
	//.<@optional>1
	//.<@type>Число
	//.<@default>Если не задана, ширина берется равной высоте
	//.<@values>
//.<descr>Устанавливает картинку для пункта меню.
//.<descr>Если первым параметром передано число, то из файла ресурсов 1CRCRUS.DLL извлекается катринка (ресурс типа Bitmap) соотвествтующая номеру ресурса.
//.<descr>По-умолчанию ширина картинки принимается равной высоте. Если картинка длинная и сдержит несколько изображений, то в параметре пНомер указываем его порядковый номер.
//.<sample>глМеню=СоздатьОбъект("Меню");
//.<sample>мнЖурн=глМеню.ДобавитьМеню("Журн", "Журналы");
//.<sample>лКоманда1=мнЖурн.ДобавитьКоманду("К1", "Команда 1", "Предупреждение(""Команда 1"")");
//.<sample>// Добавляем картинку из библиотеки картинок
//.<sample>лКарт=СоздатьОбъект("Картинка");
//.<sample>лКарт.Загрузить("НаборПиктограмм");
//.<sample>лКоманда1.УстановитьКартинку(лКарт, 3, 16);
//.<sample>// Добавляем картинку из файла
//.<sample>лКарт.Загрузить(КаталогПрограммы()+"Pic.bmp");
//.<sample>лКоманда1.УстановитьКартинку(лКарт, 3, 16);
//.<sample>// Добавляем картинку с идентификатором 1037 из файла ресурсов
//.<sample>лКоманда1.УстановитьКартинку(1037, 3, 16);
//.<sample>глМеню.Обновить();
    RBL_METH(GetCommandID,			"ПолучитьИднтКоманды",			0,	NULL)
//.<destination>Получить CommandID пункта меню.
//.<syntax>ПолучитьИднтКоманды();
//.<return>Число
//.<descr>Возвращает CommandID - внутренний идентификатор команды, выполняемой при выборе пункта меню. Для подменю и разделителей возвращает 0. Этот же идентификатор передается в процедуру ПриПолученииКоманды первым параметром при выборе пункта меню.
//.<descr>CommandID присваивается пунктам меню при выполнении метода Обновить. После каждого выполнения метода Обновить CommandID одного и того же пункта может быть другим. Поэтому не рекомендуется заранее сохранять CommandID пункта меню, а потом его сравнивать с чем-то.
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMenuItem)
	BL_PROP("ID",		"Иднт",		getID,		NULL)
//.<destination>Идентификатор объекта
//.<write>0
//.<return>Строка
//.<descr>Возвращает уникальный идентификатор объекта, присвоенный при создании пункта меню.
	BL_PROP("Type",		"Тип",		getType,		NULL)
//.<destination>Тип пункта меню
//.<write>0
//.<return>Число
//.<descr>Возвращает тип пункта меню. 1 - подменю, 2 - разделитель, 3 - команда
	RBL_PROP(Text,		"Текст")
//.<destination>Заголовок пункта меню
//.<return>Строка
//.<descr>Отображаемый заголовок пункта меню.
	RBL_PROP(Script,	"Команда")
//.<destination>Выполняемый скрипт
//.<return>Строка
//.<descr>Скрипт на внутреннем языке 1С, выполняемый при выборе пункта меню. 
//.<descr>Также это может быть текст вида #xxxx, где xxxx - числовой идентификатор команды, посылаемой системе.
	RBL_PROP(Tips,		"Подсказка")
//.<destination>Подсказка пункта меню
//.<return>Строка
//.<descr>Текст, который будет выдаваться в строке состояния главного окна программы при указании мышью на пункт меню. Как правило, строка должна содержать краткое описание действий, которые будут выполнены после выбора этого пункта меню.
	//RBL_PROP(QHTML,		"Представление")
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CRMenuItem, "RMenuItem", "ЭлементМеню", 0, NULL, NULL, -1); 
//.<destination>Объект типа RMenuItem позволяет управлять конкретным пунктом динамического меню.
//.<destination>Может быть создан методами Добавить, ДобавитьМеню, ДобавитьРазделитель, ДобавитьКоманду объектов RMenu и RMenuItem. Не может быть создан функцией СоздатьОбъект.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMenuItem::CRMenuItem()
{

}

CRMenuItem::~CRMenuItem()
{

}

BOOL CRMenuItem::getID(CValue& Value) const
{
    Value = m_pMenuItem->m_strID;
    return TRUE;
}

BOOL CRMenuItem::getType(CValue& Value) const
{
    Value = m_pMenuItem->itType + 1;
    return TRUE;
}

BOOL CRMenuItem::getText(CValue& Value) const
{
    Value = m_pMenuItem->m_strCaption;
    return TRUE;
}

BOOL CRMenuItem::setText(CValue const& Value)
{
	if(m_pMenuItem->itType == itMenu || m_pMenuItem->itType == itCommand)
	{
		if(m_pMenuItem->mParentMenuItem->mIsMainMenu == TRUE)
			m_pMenuItem->mParentMenuItem->ClearMainMenu();

		m_pMenuItem->m_strCaption = Value.GetString();
	};
    return TRUE;
}

BOOL CRMenuItem::getScript(CValue& Value) const
{
    Value = m_pMenuItem->m_strCode1C;
    return TRUE;
}

BOOL CRMenuItem::setScript(CValue const& Value)
{
	if(m_pMenuItem->itType == itCommand)
	    m_pMenuItem->m_strCode1C = Value.GetString();
    return TRUE;
}

BOOL CRMenuItem::getTips(CValue& Value) const
{
    Value = m_pMenuItem->m_strTips;
    return TRUE;
}

BOOL CRMenuItem::setTips(CValue const& Value)
{
	m_pMenuItem->m_strTips = Value.GetString();
    return TRUE;
}

BOOL CRMenuItem::funcAdd(CValue& RetVal, CValue **params)
{
	enum ItemType itType = (enum ItemType)(int)(params[0]->GetNumeric().GetDouble()-1);

	switch(itType){
	case itCommand:
		{
			funcAddCommand(RetVal, params);
			break;
		}
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
		ERR1(IDS_ERR_WRONGTYPE1, 0)
	};

	return TRUE;
}

BOOL CRMenuItem::funcAddMenu(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetMenu(params[1]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcAddCommand(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetCommand(params[1]->GetString(), params[2]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcAddSeparator(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetSeparator();

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcDelete(CValue& RetVal, CValue **params)
{
	if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString())
	
	m_pMenuItem->RemoveAt(params[0]->GetString());
	RetVal = 1;

	return TRUE;
}

BOOL CRMenuItem::funcDeleteAll(CValue& RetVal, CValue **params)
{
	m_pMenuItem->ClearList();
	RetVal = 1;

	return TRUE;
}

BOOL CRMenuItem::funcGet(CValue& RetVal, CValue **params)
{
	if(params[0]->GetTypeCode() == 1)
	{
		int index = params[0]->GetNumeric();
		if(index<=0 || index>m_pMenuItem->mListSubMenu.GetCount()) ERR1(IDS_WRONGINDEX, index);
		
		CMenuItem* pMenu = m_pMenuItem->mListSubMenu.GetAt(m_pMenuItem->mListSubMenu.FindIndex(index-1));
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

BOOL CRMenuItem::funcCount(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->mListSubMenu.GetCount();
	return TRUE;
}

BOOL CRMenuItem::funcGetCommandID(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_nID;
	return TRUE;
}

BOOL CRMenuItem::funcReadOnly(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_bEnabled;
	
	if(m_pMenuItem->itType != itCommand) return FALSE;

	int NewVal = params[0]->GetNumeric();
	if(NewVal == 0 || NewVal == 1)
		m_pMenuItem->m_bEnabled = NewVal;

	return TRUE;
}

BOOL CRMenuItem::funcCheck(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_bCheck;
	
	if(m_pMenuItem->itType != itCommand) return FALSE;

	int NewVal = params[0]->GetNumeric();
	if(NewVal == 0 || NewVal == 1)
		m_pMenuItem->m_bCheck = NewVal;

	return TRUE;
}

BOOL CRMenuItem::funcSetPicture(CValue& RetVal, CValue **params)
{
	CBitmap* pBmp = NULL;
	CBitmap bmp;
	CSize size;
	BOOL bSuccess = FALSE;

	if(m_pMenuItem->m_pPictureHolder7)
		delete m_pMenuItem->m_pPictureHolder7;

    if (params[0]->GetTypeCode() == AGREGATE_TYPE_1C)
	{
		if(params[0]->GetContext())
			if(strcmp(params[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext") == 0)
				{
					CPictureHolder7& PictureHdr = static_cast<CPictureContext*>(params[0]->GetContext())->m_PictureHolder7;

					m_pMenuItem->m_pPictureHolder7 = new CPictureHolder7(PictureHdr);

					OLE_HANDLE handle;
					m_pMenuItem->m_pPictureHolder7->GetPicture()->get_Handle(&handle);
					pBmp = CBitmap::FromHandle(*(HBITMAP*)&handle);

					size = m_pMenuItem->m_pPictureHolder7->GetSize();
					bSuccess = TRUE;
				}
	}
	else
	{
		m_pMenuItem->m_pPictureHolder7 = new CPictureHolder7();
		
		if(bmp.LoadBitmap((UINT)params[0]->GetNumeric()))
		{
			pBmp = &bmp;
			BITMAP bt;
			bmp.GetBitmap(&bt);
			size = CSize(bt.bmWidth, bt.bmHeight);
			bSuccess = TRUE;
		}
		else
			ERR1(IDS_ERR_NORES, (UINT)params[0]->GetNumeric());
	}
		
	if(bSuccess)
	{
		if((int)params[2]->GetNumeric() > 0)
			size.cx = params[2]->GetNumeric();
		else 
			size.cx = size.cy;

		CImageList RowsImages;
		RowsImages.Create(size.cx, size.cy, ILC_MASK, 1, 10);
		RowsImages.Add(pBmp, RGB(0x00,0x80,0x80)); // RGB(0xC0,0xC0,0xC0)
		
		HICON icon;
		if(params[1]->GetNumeric()>0 && params[1]->GetNumeric()<=RowsImages.GetImageCount())
			icon = RowsImages.ExtractIcon((int)params[1]->GetNumeric() - 1);
		else
			icon = RowsImages.ExtractIcon(0);
		
		m_pMenuItem->m_pPictureHolder7->CreateFromIcon(icon);

		RetVal = 1;
		return TRUE;
	}


	ERR(IDS_ERR_NOPICTURE);
	RetVal = 0L;
	return TRUE;
}



BOOL CRMenuItem::NoDefaultValue(int nParam, CValue* param) const
{
	//*param = -1;
	//*param = CDate(0,0,0);
	return FALSE;
}

BOOL CRMenuItem::DefaultValue2(int nParam, CValue* param) const
{
	if(nParam>1)
	{
		*param = "";
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CRMenuItem::DefaultValue1(int nParam, CValue* param) const
{
	if(nParam>0)
	{
		*param = -1;
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CRMenuItem::DefaultValueBase(int nParam, CValue* param) const
{
	*param = -1;
	return TRUE;
}

