// RMsgWindow.cpp: implementation of the CRMsgWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RMsgWindow.h"

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMsgWindow) 
    RBL_METH(Message,		"Сообщить",			3,	NULL)
//.<destination>Отображает окно сообщений на экране.
//.<syntax>Сообщить(пТекстСообщения, пЗаголовокОкна, пВыравниваниеСообщения);
//.<param>
	//.<@name>пТекстСообщения
	//.<@destination>Собственно текст сообщения.
	//.<@optional>1
	//.<@type>Строка
	//.<@default>Пустая строка
	//.<@values>
//.<param>
	//.<@name>пЗаголовокОкна
	//.<@destination>Текст в заголовке окна сообщений.
	//.<@optional>1
	//.<@type>Строка
	//.<@default>"Сообщение"
	//.<@values>
//.<param>
	//.<@name>пВыравниваниеСообщения
	//.<@destination>Вариант выравнивания текста сообщения.
	//.<@optional>1
	//.<@type>Число
	//.<@default>2 - по центру
	//.<@values>1 - по левой, 2 - по центру, 3 - по правой границе окна
//.<descr>Отображает окно сообщения на экране. Если окно до вызова метода было скрыто, оно появляется на экране. Если окно уже на экране, то меняется текст сообщения, заголовок окна, выравнивание в соответствии с переданными параметрами.
//.<sample>лОкно=СоздатьОбъект("RMsgWindow");
//.<sample>лОкно.Сообщить("Привет!", "Приветственное сообщение", 1);
    RBL_METH(Close,			"Закрыть",			0,	NULL)
//.<destination>Закрывает окно сообщений.
//.<syntax>Закрыть();
//.<descr>Закрывает окно сообщений, но не уничтожает его. При повторном отображении окна на экране будут использованы ранее сделанные установки положения окна, выравнивания, текстов сообщения и заголовка.
    RBL_METH(Move,			"Переместить",		4,	NULL)
//.<destination>Перемещает окно сообщений в указанную позицию и устанавливает указанный размер.
//.<syntax>Переместить(пЛево, пВерх, пШирина, пВысота);
//.<return>
//.<param>
	//.<@name>пЛево
	//.<@destination>Координата левого верхего угла окна сообщений по горизонтали.
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>0 - размер экрана по ширине
//.<param>
	//.<@name>пВерх
	//.<@destination>Координата левого верхего угла окна сообщений по вертикали.
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>0 - размер экрана по высоте
//.<param>
	//.<@name>пШирина
	//.<@destination>Ширина окна сообщений.
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>0 - ...
//.<param>
	//.<@name>пВысота
	//.<@destination>Высота окна сообщений.
	//.<@optional>0
	//.<@type>Число
	//.<@default>
	//.<@values>0 - ...
//.<descr>Начало координат находится в левом верхнем углу. Координаты задаются в пикселях. По умолчанию окно сообщений отображается в середине экрана.
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMsgWindow)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CRMsgWindow, "RMsgWindow", "RMsgWindow", 1, NULL, NULL, -1);
//.<destination> Объект типа RMsgWindow позволяет отображать на экране одно или несколько окон сообщений и произвольно манипулировать ими. Информация в окне сообщений отображается подобно строке состояния основного окна 1С:Предприятия с возможностью выровнять текст по центру, левому или правому краям окна. Окно отображается на экране до тех пор пока не будет вызван метод Закрыть(), намеренно уничтожен экземпляр типа 'RMsgWindow' или переменная выйдет за пределы видимости и окно будет уничтожено автоматически. Поэтому если вы хотите быть уверены, что окно закроется автоматически после окончания работы модуля, даже если выполнение модуля будет прервано ошибкой или пользователем клавишей 'Esc', используйте локальные переменные процедур для определения экземпляров объектов типа 'RMsgWindow'.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMsgWindow::CRMsgWindow()
{
	msg = new CDlgMsg;
	msg->Create(AfxGetApp()->GetMainWnd()->GetSafeHwnd());
	msg->CenterWindow(CWnd::GetDesktopWindow()->GetSafeHwnd());
	hwndText = GetDlgItem(msg->m_hWnd, IDC_Text);
}

CRMsgWindow::~CRMsgWindow()
{
	msg->DestroyWindow();
	delete msg;
}

BOOL CRMsgWindow::funcMessage(CValue& RetVal, CValue **params)
{
	if(!msg->IsWindowVisible()) msg->ShowWindow(SW_SHOW);
	if(!params[1]->GetString().IsEmpty()) SetWindowText(msg->m_hWnd, params[1]->GetString());
	if((params[2]->GetNumeric()>0) && (params[2]->GetNumeric()<4)) 
	{
		::SetWindowLong(hwndText, GWL_STYLE, (::GetWindowLong(hwndText, GWL_STYLE)&~SS_CENTER&~SS_LEFT&~SS_RIGHT)|long(params[2]->GetNumeric())-1);
		::SetWindowPos(hwndText, 0, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
	};
	SetWindowText(hwndText, params[0]->GetString());
	msg->m_message = params[0]->GetString();
	
	return TRUE;
}

BOOL CRMsgWindow::funcClose(CValue& RetVal, CValue **params)
{
	msg->ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL CRMsgWindow::funcMove(CValue& RetVal, CValue **params)
{
	RECT r;
	msg->GetWindowRect(&r);
	r.left	= params[0]->GetNumeric();
	r.top		= params[1]->GetNumeric();
	r.right	= r.left + params[2]->GetNumeric();
	r.bottom	= r.top + params[3]->GetNumeric();
	msg->MoveWindow(&r);
	
	return TRUE;
}

