#include "stdafx.h"
#include "WrapperForSyntaxCheck.h"

#include "MyModule.h"
#include "Syntax.h"

const unsigned int WM_SETMESSAGESTRING = 0x0362;	// wParam = nIDS (or 0),
													// lParam = lpszOther (or NULL)

const unsigned int WM_COMMAND_SYNTAXCHECK = 33297; // "Синтаксический контроль исходного текста модуля\nСинтаксический контроль"

//const unsigned int iResId_String_SyntaxErrors = 29172; // 	// "При проверке модуля обнаружены синтаксические ошибки!" или
const unsigned int iResId_String_SyntaxCheckOnlyExtFilesOrReportInMD = 5914; // 	// "Проверка синтаксиса выполняется только для модулей, входящих в конфигурацию, и модулей внешних отчетов!"

//const unsigned int iResId_String_FullSyntaxMDCheck = 33884; // "Полный синтаксический контроль для всех модулей\nПолный синтаксический контроль"

// artbear TODO если кто-то будет переделывать работу по синтакс-проверке, 
// прошу учесть и протестировать все варианты
//
// В связи со своеобразным поведением 1С при синтакс-проверке пришлось применить непростую схему
// В обработчиках событий ( хуки на WH_CallWndProc и CWinApp::PreTranslateMessage)
// событие синтакс-проверки не останавливается, а пропускается дальше
// т.к. к сожалению событие, перехваченное через хук WH_CallWndProc, нельзя остановить
// поэтому чтобы было меньше путаницы, оба обработчика пропускают события дальше
//
//	фактически проверка классов происходит на втором этапе
// Это сделано для того, чтобы после синтакс-проверки сообщения об ошибках не удалялись
// если работать на первом проходе, сообщения будут удалены
// Для остальных документов 1С поведение не изменилось
//	Также были приложены усилия, чтобы не было дурацких, повторяющихся сообщений (хук CMainMessageHandler::WriteMessage)
//
//  Тестовая таблица
// класс внешний Текст      8
// 	проверяем глоб. 8
// 	проверяем класс 8
// класс внешний ерт        8
// 	проверяем глоб. 8
// 	проверяем класс 8
// 	пропускаю класс 7
// класс внутренний         8
// 	проверяем глоб. 8
// 	проверяем класс 8
// 	пропускаю класс 7
//                       
// обработка внешняя    7
// 	пропускаю
// 
// обработка внутренняя     7
// 	пропускаю
// 	
// глобальный               7
// 	??
// 	пропускаю
// 	
// обычный Текст  ??          ?
// 	пропускаю
// 
// ПЛЮС обязательно учесть, что можно синтакс-проверку вызвать через контекстное меню Виз1С++ 
// с двумя режимами проверки (с Глоб.Модулем и без него)
// 

bool CWrapperForSyntaxCheck::bInsideCompileHandler = false;

CTrap<CWrapperForSyntaxCheck::TFunc_CBLModule7_Compile> CWrapperForSyntaxCheck::trCompile;

int CWrapperForSyntaxCheck::CallOrigCBLModuleCompile(CBLModule7* pMod)
{
	CTrapGuard<TFunc_CBLModule7_Compile> trap(trCompile);
	return pMod->Compile();
}

int CWrapperForSyntaxCheck::Wrap_CBLModule_Compile() //int CBLModule7::Compile()
{
	if (bInsideCompileHandler)
		return true;

	bInsideCompileHandler = true;

	CTrapGuard<TFunc_CBLModule7_Compile> trap(trCompile);

	CBLModule7* pMod = reinterpret_cast<CBLModule7*>(this);
	return CSyntax::ModuleCompile(pMod, bInsideCompileHandler);
}

HHOOK CWrapperForSyntaxCheck::WH_CallWndProc_HookHandle = 0;

// для перехвата нажатия кнопки "Синтакс-проверки" на панели "Текстовый редактор"
LRESULT __stdcall CWrapperForSyntaxCheck::WH_CallWndProc_Hook(int code, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	CWPSTRUCT* pMsg = (CWPSTRUCT* )lParam;
	if(pMsg->message==WM_COMMAND && WM_COMMAND_SYNTAXCHECK == pMsg->wParam) // 0x0111 273 // синтакс-контроль
	{
		//LogErr("CallWndProc_Hook WM_COMMAND wParam %d, code %d", pMsg->wParam, code);
		// MessageBox("CallWndProc_Hook WM_COMMAND wParam %d, code %d", pMsg->wParam, code);

		// TODO нужно обеспечить единообразное поведение (хуки на WH_CallWndProc и CWinApp::PreTranslateMessage)
		CSyntax::SyntaxCheck();
	}

	return CallNextHookEx(WH_CallWndProc_HookHandle, code, wParam, lParam); // по рекомендации МСДН
}

CWrapperForSyntaxCheck::PPRETRANSLATE CWrapperForSyntaxCheck::m_pPreTranslate;

// для отработки клавиши F4 - переход к очередной ошибке
//		TODO если ошибки нет, проверять модуль с нуля
//
// для перехвата вызова "Синтакс-проверки" через макрос Опенконфа
//
BOOL CWrapperForSyntaxCheck::PreTranslateMessage(MSG* pMsg)
{
	UINT message = pMsg->message;
	WPARAM wParam = pMsg->wParam;

	if ( WM_COMMAND_SYNTAXCHECK == wParam && 
		(message == WM_SETMESSAGESTRING || message == WM_COMMAND)
		)
	{
		//LogErr("pMsg->message %d", message);

		// TODO нужно обеспечить единообразное поведение (хуки на WH_CallWndProc и CWinApp::PreTranslateMessage)
		CSyntax::SyntaxCheck();
	}
	else
		if(message == WM_KEYDOWN)
		{
			if(LOWORD(wParam) == VK_F4)
			{
				SHORT stCtrl = GetKeyState(VK_CONTROL);
				if(!(stCtrl & 0x10000))
				{
					stCtrl=GetKeyState(VK_SHIFT);
					CSyntax::OpenNextError((stCtrl & 0x10000)!=0);
					return TRUE;
				}
			}
			/*if(LOWORD(wParam) == 'A')
			{
				SHORT stCtrl = GetKeyState(VK_CONTROL);
				if(stCtrl & 0x10000)
				{
					AfxGetMainWnd()->SendMessage(WM_COMMAND,33188,0);
					return TRUE;
				}
			}
			*/
		}

	return (((CWnd*)this)->*m_pPreTranslate)(pMsg);
}

CTrap<CWrapperForSyntaxCheck::TFunc_CMainMessageHandler_WriteMessage> CWrapperForSyntaxCheck::TrWrap_CMainMessageHandler_WriteMessage;

// перехват вывода в окно сообщений
void CWrapperForSyntaxCheck::Wrap_CMainMessageHandler_WriteMessage(char const * szString, enum MessageMarker marker, int i1)
{
	CTrapGuard<TFunc_CMainMessageHandler_WriteMessage> trap(TrWrap_CMainMessageHandler_WriteMessage);
	//LogErr("Перехватили вызов :<%s>, marker = <%d>, i1 = <%d>", szString, (int)marker, i1);

	if (!ResString(iResId_String_SyntaxCheckOnlyExtFilesOrReportInMD).CompareNoCase(szString))
		return;

	CMainMessageHandler* pMainMessageHandler = (CMainMessageHandler*) this;

	pMainMessageHandler->WriteMessage(szString, marker, i1);
}

void CWrapperForSyntaxCheck::SetHooks()
{
	trCompile.SetTrapOnFunc((TFunc_CBLModule7_Compile)&CBLModule::Compile, &Wrap_CBLModule_Compile);

	CWinApp* pApp = pMainApp;
	WH_CallWndProc_HookHandle = SetWindowsHookEx(WH_CALLWNDPROC, WH_CallWndProc_Hook, pApp->m_hInstance, pApp->m_nThreadID);

	DWORD* pVtable=*(DWORD**)pMainApp;
	pVtable+=0x60/4;
	DWORD old;
	VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
	PPRETRANSLATE pPre=(PPRETRANSLATE)&PreTranslateMessage;
	//DWORD* pNew=(DWORD*)&pPre,*pOld=(DWORD*)&m_pPreTranslate;
	DWORD* pNew=(DWORD*)&pPre;
	DWORD *pOld=(DWORD*)&m_pPreTranslate;
	*pOld=*pVtable;
	*pVtable=*pNew;

	TrWrap_CMainMessageHandler_WriteMessage.SetTrapOnFunc((TFunc_CMainMessageHandler_WriteMessage)&CMainMessageHandler::WriteMessage, &Wrap_CMainMessageHandler_WriteMessage);
	
}

void CWrapperForSyntaxCheck::DestroyHooks()
{
	
	trCompile.Swap();

	UnhookWindowsHookEx(WH_CallWndProc_HookHandle);

	TrWrap_CMainMessageHandler_WriteMessage.Swap();
	
}
