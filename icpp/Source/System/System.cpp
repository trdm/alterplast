//////////////////////////////////////////////////////////////////////
//
//	System.cpp
//	Implementation of CSystem class
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "System.h"
#include "../VTExt/VTExtended.h"
#include "compare.h"

#include "Informer.h"

#include "DllHooker.h"
#include "..\TurboBL\gcboost.h"

#include "../PicService.h"

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

//#define __CSystem_ENABLE_MSG
//_____________________________________________________________________________
//
// CSystem Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CSystem)

	BL_METH("MemInfo",			"Память",					2, NULL,		&funcMemInfo,			&defsMemInfo)
	BL_METH("Priority",			"Приоритет",				1, NULL,		&funcPriority,			NULL)
	BL_METH("Purify",			"Purify",					1, NULL,		&funcPurify,				NULL)
	BL_METH("Test",				"Тест",						2, NULL,		&funcTest,				NULL)

	BL_METH("EnableNewLine",	"РазрешитьДобавлениеСтрок",	1, NULL,		&funcEnableNewLine,		NULL)

    BL_METH("StartBatch",		"StartBatch",				1, NULL,		&funcStartBatch,			NULL)
    //BL_METH("EndBatch",			"EndBatch",					2, NULL,		funcEndBatch,			NULL)
    BL_METH_FUNC_DEF_PARAM("EndBatch", "EndBatch",	2, &funcEndBatch, &defsEndBatch)


	BL_METH("EnableActivation",	"РазрешитьАктивацию",		0, NULL,		&funcEnableActivation,	NULL)
	BL_METH("Activate",			"Активизировать",			1, NULL,		&funcActivate,			NULL)

	BL_METH("CommandLine",		"КоманднаяСтрока",		0, NULL,			&funcCommandLine,		NULL)
	BL_METH("GetAppPath",		"ПутьПриложения1С",		0, NULL,			&funcGetAppPath,		NULL)

	BL_METH_FUNC_DEF_PARAM("CloseForm",		"ЗакрытьФорму",		2, &funcCloseForm, &defsCloseForm)
	BL_METH_PROC_DEF_PARAM("RegisterAddinsInHKCU", "РегистрироватьКомпонентыВ_HKCU", 1,
			&procRegisterAddinsInHKCU, &defsRegisterAddinsInHKCU)

	//BL_METH_FUNC("ResString",		"ResString",		1, funcResString)
	BL_METH_PROC("Sleep",		"Уснуть",		1, &procSleep)
	BL_METH_PROC("Beep",		"Сигнал",		2, &procBeep)
	BL_METH_PROC("MessageBeep",	"СигналСообщения", 1, &procMessageBeep)

	BL_METH_PROC("RefreshGCData",		"ОбновитьДанныеГК",		1, &RefreshGroupContextData)

	BL_METH_FUNC_DEF_PARAM("GetOlePicture", "ПолучитьOLEКартинку", 3, &funcGetOlePic, &defsGetOlePic)
	BL_METH_PROC_DEF_PARAM("SavePic", "СохранитьКартинку", 5, &procSavePic, &defsSavePic)

	BL_METH_FUNC("Serialize",		"Сериализовать",		1, &funcSerialize)

END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CSystem)
//    BL_PROP("propEng", "propRus", propGet, propSet)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CSystem, "System", "Система", 1, NULL, NULL, -1);
//______________________________________________________________________________
//
// static members
//______________________________________________________________________________
//
DWORD CSystem::dwPriorityClasses[] =
{
	IDLE_PRIORITY_CLASS
,	BELOW_NORMAL_PRIORITY_CLASS
,	NORMAL_PRIORITY_CLASS
,	ABOVE_NORMAL_PRIORITY_CLASS
,	HIGH_PRIORITY_CLASS
,	REALTIME_PRIORITY_CLASS
};
//______________________________________________________________________________
//
// static members для механизма активации
//______________________________________________________________________________
//
CDllMethodWrapper<CSystem::PF_CGetDoc7_OnEnter> CSystem::w_CGetDoc7_OnEnter;
CDllMethodWrapper<CSystem::PF_CDocForm_OnEnter> CSystem::w_CDocForm_OnEnter;
CDllMethodWrapper<CSystem::PF_CEditGet_Activate> CSystem::w_CEditGet_Activate;

CString CSystem::m_strActivationRequested;
BOOL CSystem::m_bActivationRequested;

CMapPtrToPtr CSystem::m_BatchGuard_map;
//______________________________________________________________________________
//
LONG CSystem::crtwalk(bool bShowMsg)
{
	_HEAPINFO hinfo;
	int heapstatus;
	char *buf;
	DWORD dwUsed = 0;
	DWORD dwFree = 0;
	int nUsed = 0, nFree = 0;

	hinfo._pentry = NULL;
	while( (heapstatus = _heapwalk( &hinfo )) == _HEAPOK )
	{
		if  ( hinfo._useflag == _USEDENTRY )
		{
			dwUsed += hinfo._size;
			nUsed++;
		}
		else
		{
			dwFree += hinfo._size;
			nFree++;
		}
	}
	switch( heapstatus )
	{
	case _HEAPEMPTY:
		buf = "OK - empty heap";
		break;
	case _HEAPEND:
		buf = "OK";
		break;
	case _HEAPBADPTR:
		buf = "ERROR - bad pointer to heap";
		break;
	case _HEAPBADBEGIN:
		buf = "ERROR - bad start of heap";
		break;
	case _HEAPBADNODE:
		buf = "ERROR - bad node in heap";
		break;
	}

	if( bShowMsg )
	{
		Msg("Heap( ----- CRT ----- ): Total: %u ( %u block(s) ); Used: %u ( %u block(s) ); Free: %u; Status: %s;",
			dwUsed + dwFree, nUsed + nFree, dwUsed, nUsed, dwFree, buf);
	}

	return dwUsed + dwFree;
}
//______________________________________________________________________________
//
LONG CSystem::heapwalk(HANDLE hHeap, bool bShowMsg)
{
	PROCESS_HEAP_ENTRY heapEntry;
	LONG nTotal = 0;
	DWORD dwOverhead = 0;
	DWORD dwCommitted = 0;
	DWORD dwUnCommitted = 0;
	int nEntries = 0;

	if (hHeap)
	{
		heapEntry.lpData = NULL;
		heapEntry.wFlags = PROCESS_HEAP_REGION;

		while ( HeapWalk( hHeap, &heapEntry ) )
		{
			if ( heapEntry.wFlags & PROCESS_HEAP_REGION )
			{
				dwCommitted += heapEntry.Region.dwCommittedSize;
				dwUnCommitted += heapEntry.Region.dwUnCommittedSize;
			}
			if ( heapEntry.wFlags & PROCESS_HEAP_ENTRY_BUSY )
			{
				nTotal += heapEntry.cbData;
				dwOverhead += heapEntry.cbOverhead;
				nEntries++;
			}
		}
		if( bShowMsg )
			Msg( "Heap( 0x%08x ): Used: %u ( %u block(s) ); Overhead: %u; Committed: %u; UnCommitted: %u",
			hHeap, nTotal, nEntries, dwOverhead, dwCommitted, dwUnCommitted );
	}

	return nTotal;
}
//______________________________________________________________________________
//
BOOL CSystem::defsMemInfo(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 1: *param = -1L; return TRUE; break;
	}
	return FALSE;
}

//______________________________________________________________________________
//
// funcMemInfo(флПоказыватьСообщения, битЗатребованнаяИнформация)
// флПоказыватьСообщения == [1 / 0]
// битЗатребованнаяИнформация == [бит 0: CRT Heap; бит 1: VTExtended Heap]
//______________________________________________________________________________
//
BOOL CSystem::funcMemInfo(CValue& RetVal, CValue **params)
{
	bool bShowMsg = ( (int)params[0]->GetNumeric() != 0 );
	int nRequestedInfo = (int)params[1]->GetNumeric();
	LONG nTotal = 0;
	HANDLE hHeap;

	// CRT
	if (nRequestedInfo & 1)
	{
		HMODULE hMod = GetModuleHandle("msvcrt.dll");
		FARPROC _get_heap_handle_explicit = GetProcAddress(hMod, "_get_heap_handle");
		if (_get_heap_handle_explicit)
		{
			// via heapwalk
//			hHeap = reinterpret_cast<HANDLE>(_get_heap_handle());
			hHeap = reinterpret_cast<HANDLE>(_get_heap_handle_explicit());
			nTotal += heapwalk(hHeap, bShowMsg);
		}
		else
		{
			// via crtwalk
			nTotal += crtwalk(bShowMsg);
		}
	}
	// CVTExtHeap
	if (nRequestedInfo & 2)
	{
		hHeap = CVTExtHeap::Get();
		nTotal += heapwalk(hHeap, bShowMsg);
	}

	RetVal = nTotal;
	return TRUE;
}
//______________________________________________________________________________
//
// параметры:
//	0: число от 0 до 4: установить приоритет; не число - получить приоритет
//______________________________________________________________________________
//
BOOL CSystem::funcPriority(CValue& RetVal, CValue **params)
{

	RetVal = -1;

	if (params[0]->GetTypeCode() == NUMBER_TYPE_1C)
	{
		// установка класса приоритета
		UINT nPriority = (UINT)params[0]->GetNumeric().operator long();
		if (nPriority < nPriorityClassCnt)
		{
			if (SetPriorityClass(GetCurrentProcess(), dwPriorityClasses[nPriority]))
				RetVal = 1L;
		} else
		{
			RuntimeError("Неверный приоритет!");
		}
	}
	else
	{
		// получение класса приоритета
		DWORD dwPriority = GetPriorityClass(GetCurrentProcess());
		if (dwPriority)
			for (int i = 0; i < nPriorityClassCnt; ++i)
				if (dwPriorityClasses[i] == dwPriority)
				{
					RetVal = static_cast<long>(i);
					break;
				}
	}

	return TRUE;
}
//______________________________________________________________________________
//
BOOL CSystem::funcPurify(CValue& RetVal, CValue **params)
{
#ifdef _DEBUG_PURE

	int param = (int) params[0]->GetNumeric();
	switch (param)
	{
	case 1:
		PurifyNewLeaks();
		break;
	case 2:
		PurifyNewInuse();
		break;
	case 3:
		PurifyAllLeaks();
		break;
	case 4:
		PurifyAllInuse();
		break;
	default:
		Msg( "Incorrect param" );
	}

#endif // _DEBUG_PURE

	RetVal =0L;
	return TRUE;
}
//______________________________________________________________________________
//
__declspec(naked) __inline int IsNotPowerOf2(int num)
{
	_asm
	{
		mov EAX, [ESP][4];
		test EAX, EAX;
		jz l_ret;
l_shr:
		shr EAX, 1;
		jnc l_shr;
l_ret:
		ret;
	}
}

//проверка отладки классов 1С++
//______________________________________________________________________________
//
CTrapSwap trOnNextLine;
CTrapSwap trOnEnterProc;

class CTrapHandlers
{
public:
	int OnNextLine();
	void OnEnterProc(int nParam);
};
//______________________________________________________________________________
//
int CTrapHandlers::OnNextLine()
{
	CBLModule7* pTHIS = reinterpret_cast<CBLModule7*>(this);

	trOnNextLine.Swap();
	int nRet = pTHIS->OnNextLine();
	trOnNextLine.Swap();

	CBLModule7 const* pMod = (CBLModule7 *)pTHIS->GetCurrentModule();
	int nLineNum = pTHIS->GetExecutedLineNum();

	CBLModuleInternals* pModuleInts = pTHIS->GetInternalData();
	CString const* pStr = pModuleInts->strExecutingProc;

//	if (!pStr)
//		pStr = &CString("no procedure avail");

	pStr = &pMod->m_strModulePath;
	CString str = pMod->GetFullName();
	pStr = &str;

	Msg("OnNextLine, pTHIS: %x,  pMod: %x, Proc: %s, LineNum: %i", pTHIS, pMod, pStr->operator LPCTSTR(), nLineNum);

// 	pTHIS->m_bIsProcessBroken = TRUE;
// 	return 0;

	return nRet;
}

void CTrapHandlers::OnEnterProc(int nParam)
{
	CBLModule7* pTHIS = reinterpret_cast<CBLModule7*>(this);

	trOnEnterProc.Swap();
	pTHIS->OnEnterProc(nParam);
	trOnEnterProc.Swap();

	Msg("OnEnterProc: %i", nParam);
}

//______________________________________________________________________________
//
BOOL CSystem::funcTest(CValue& RetVal, CValue **params)
{
	return TRUE;
}

//______________________________________________________________________________
//
// разрешение добавления строк для контекста документа
// (необходимо для обратных вызовов функций формы документа, модифицирующих мнч)

// параметры:
//	0: контекст документа

// возврат:
//	1: добавление строк разрешено
//	0: контекст документа не найден
//______________________________________________________________________________
//
BOOL CSystem::funcEnableNewLine(CValue& RetVal, CValue **params)
{
	int nCount = 0;

	CGroupContext *pGroupCont = (CGroupContext *)params[0]->GetContext();
	if(pGroupCont)
	{
		CBLContext *pCont;
		for(int i = 0; i < pGroupCont->GetSize(); ++i)
		{
			pCont = pGroupCont->GetContext(i);

			LPCSTR pName = pCont->GetRuntimeClass()->m_lpszClassName;

#if defined __CSystem_ENABLE_MSG
			Msg("%s", pName);
#endif

			if(!strcmp(pName, "CDocContext"))
			{
				static_cast<CDocContext*>(pCont)->SetEnableNewLine(1);
				++nCount;
			}
		}
	}

	RetVal = (nCount) ? 1L : 0L;
	return TRUE;
}
//______________________________________________________________________________
//
CGetDoc7* CSystem::sub_GetDoc7(CValue *cv)
{
	CGetDoc7* pDoc = NULL;

	CGroupContext *pGroupCont = (CGroupContext *)cv->GetContext();
	if(pGroupCont)
	{
		CBLContext *pCont;
		for(int i = 0; i < pGroupCont->GetSize(); ++i)
		{
			pCont = pGroupCont->GetContext(i);
			LPCSTR pName = pCont->GetRuntimeClass()->m_lpszClassName;

#if defined __CSystem_ENABLE_MSG
			Msg("%s", pName);
#endif

			if(!FastCompare.Compare(pName, "CFormContext"))
			{
				CFormContext* pFormCont = static_cast<CFormContext*>(pCont);
				pDoc = pFormCont->m_pDocFC;
				break;
			}
			else if (!FastCompare.Compare(pName, "CFormContextForDoc"))
			{
				CFormContextForDoc* pFormContForDoc = static_cast<CFormContextForDoc*>(pCont);
				pDoc = pFormContForDoc->m_pDocForm;
				break;
			}
		}
	}
	return pDoc;
}

int CSystem::sub_LookupBatch(CGetDoc7* pDoc)
{
	int nRefCount = 0;
	m_BatchGuard_map.Lookup(pDoc, (void *&)nRefCount);

	return nRefCount;
}
//______________________________________________________________________________
//
// TODO: очистка ключа при разрушении CGetDoc7
//
// params[0] - контекст
//
BOOL CSystem::funcStartBatch(CValue& RetVal, CValue **params)
{
	int nRefCount = -1;
	CGetDoc7* pDoc = sub_GetDoc7(params[0]);

	if (pDoc)
	{
		nRefCount = sub_LookupBatch(pDoc);
		if (nRefCount)
			++nRefCount;

		else
		{
			if(!pDoc->IsBatchStarted())
			{
				pDoc->OnStartBatch();
				++nRefCount;
			}
		}

		if (nRefCount)
			m_BatchGuard_map.SetAt(pDoc, (void*)nRefCount);
	}

	RetVal = (long)nRefCount;
	return TRUE;
}
//______________________________________________________________________________
//
// TODO: очистка ключа при разрушении CGetDoc7
//
// params[0] - контекст
// params[1] - флаг обновления формы
//

BOOL CSystem::defsEndBatch(int nParam, CValue* param) const
{
	if (1 == nParam){
 		*param = 0L;
		return TRUE;
	}
	return FALSE;
}

BOOL CSystem::funcEndBatch(CValue& RetVal, CValue **params)
{
	int nRefCount = -1;
	CGetDoc7* pDoc = sub_GetDoc7(params[0]);

	if (pDoc)
	{
		nRefCount = sub_LookupBatch(pDoc);
		if (nRefCount)
		{
			if (--nRefCount)
				m_BatchGuard_map.SetAt(pDoc, (void*)nRefCount);

			else
			{
				BOOL bRefresh = (BOOL)(params[1]->GetNumeric().operator long());
				if (bRefresh)
				{
					if (!pDoc->IsDocLayouted())
						bRefresh = FALSE;
				}
				pDoc->OnEndBatch(bRefresh);
				m_BatchGuard_map.RemoveKey(pDoc);
			}
		}
	}

	RetVal = (long)nRefCount;
	return TRUE;
}
//______________________________________________________________________________
//
// установка запрошенного к активации элемента диалога
//______________________________________________________________________________
//
BOOL CSystem::funcActivate(CValue& RetVal, CValue **params)
{
	// запомним значение
	m_strActivationRequested = params[0]->GetString();
	m_bActivationRequested = FALSE;

	// проверка параметров
	CBLModule * pMod = CBLModule::GetExecutedModule();
	CGroupContext* pGroupCont = (CGroupContext *)CBLModuleWrapper::GetContextFromModule(pMod);
	if (pGroupCont)
	{
		CBLContext *pCont;
		for(int i = 0; i < pGroupCont->GetSize(); ++i)
		{
			pCont = pGroupCont->GetContext(i);
			CGetDoc7 *pDoc7;

			LPCSTR pName = pCont->GetRuntimeClass()->m_lpszClassName;

#if defined __CSystem_ENABLE_MSG
			Msg("%s", pName);
#endif
			// отчеты и справочники
			if(!strcmp(pName, "CFormContext"))
			{
				CFormContext *pFormCont = static_cast<CFormContext*>(pCont);
				pDoc7 = pFormCont->m_pDocFC;
			}
			// документы
			else if (!strcmp(pName, "CFormContextForDoc"))
			{
				CFormContextForDoc *pFormContForDoc = static_cast<CFormContextForDoc*>(pCont);
				pDoc7 = pFormContForDoc->m_pDocForm;
			}
			else
				continue;

#if defined __CSystem_ENABLE_MSG
			pName = pDoc7->GetRuntimeClass()->m_lpszClassName;
			Msg("pDoc7 class: %s", pName);
#endif

			CGetFieldsArray *pGetFieldsArray = &pDoc7->m_GetFieldsArray;

			LPCSTR lpsz = m_strActivationRequested.operator LPCSTR();

			int num;
			for(num = 0; num < pGetFieldsArray->GetSize(); num++)
			{
				CGetField *pGetField = pGetFieldsArray->GetAt(num);
				LPCSTR pCodeName = pGetField->GetCtrlInfo()->GetCodeName();
				if(!stricmp(lpsz, pCodeName))
				{
					m_bActivationRequested = TRUE;
					break;
				}
			}
			if (!m_bActivationRequested)
				RuntimeError("Неверное имя атрибута: %s", m_strActivationRequested.operator LPCTSTR());
		}
	}

	RetVal = 1L;
	return TRUE;
}
//______________________________________________________________________________
//
// установить перехваты для механизмов активации
//
// возврат:
//	1: активация установлена
//	0: активация уже установлена ранее
//______________________________________________________________________________
//
BOOL CSystem::funcEnableActivation(CValue& RetVal, CValue **params)
{
	if (w_CGetDoc7_OnEnter.IsWrapped() ||
		w_CDocForm_OnEnter.IsWrapped() ||
		w_CEditGet_Activate.IsWrapped())
		RetVal = 0L;

	else
	{
		// установка перехватов
		HINSTANCE hm;

		hm = GetModuleHandle("basic.dll");
		w_CGetDoc7_OnEnter.DoWrap(hm, "?OnEnter@CGetDoc7@@IAEXXZ", (PF_CGetDoc7_OnEnter)&CSystem::Wrap_CGetDoc7_OnEnter);
		w_CDocForm_OnEnter.DoWrap(hm, "?OnEnter@CDocForm@@IAEXXZ", (PF_CDocForm_OnEnter)&CSystem::Wrap_CDocForm_OnEnter);

		hm = GetModuleHandle("br32.dll");
		w_CEditGet_Activate.DoWrap(hm, "?Activate@CEditGet@@UAEHXZ", (PF_CEditGet_Activate)&CSystem::Wrap_CEditGet_Activate);

		RetVal = 1L;
	}
	return TRUE;
}
//______________________________________________________________________________
//
// часть механизма активации из формул элементов диалога
//______________________________________________________________________________
//
int CSystem::Wrap_CEditGet_Activate()
{
	int ret;

	// запрещаем только в блоке OnEnter в случае, если запрошена особая активация
	// иначе работает штатный механизм
	if (m_bActivationRequested &&
		(w_CGetDoc7_OnEnter.IsActive() || w_CDocForm_OnEnter.IsActive()))
	{

#if defined __CSystem_ENABLE_MSG
		Msg("CEditGet::Activate: Skip");
#endif

		ret = 1;
	}
	else
	{

#if defined __CSystem_ENABLE_MSG
		Msg("CEditGet::Activate");
#endif

		w_CEditGet_Activate.RestoreOrigAddress();
		ret = (((CEditGet *)this->*w_CEditGet_Activate.GetOrigMethod()))();
		w_CEditGet_Activate.RestoreWrapAddress();
	}

	return ret;
}
//______________________________________________________________________________
//
// часть механизма активации из формул элементов диалога
//______________________________________________________________________________
//
void CSystem::Wrap_CGetDoc7_OnEnter()
{

#if defined __CSystem_ENABLE_MSG
	 Msg("CGetDoc7::OnEnter enter");
#endif

	// начнем с нуля
	m_bActivationRequested = FALSE;

	w_CGetDoc7_OnEnter.RestoreOrigAddress();
	(((CGetDoc7 *)this->*w_CGetDoc7_OnEnter.GetOrigMethod()))();
	w_CGetDoc7_OnEnter.RestoreWrapAddress();

#if defined __CSystem_ENABLE_MSG
	Msg("CGetDoc7::OnEnter leave");
#endif

	if (m_bActivationRequested)
	{
		CGetDoc7 *pDoc7 = (CGetDoc7 *)this;
		pDoc7->TrySetFocusOnCtrl(m_strActivationRequested, 1);
	}
}
//______________________________________________________________________________
//
// часть механизма активации из формул элементов диалога
//______________________________________________________________________________
//
void CSystem::Wrap_CDocForm_OnEnter()
{

#if defined __CSystem_ENABLE_MSG
	Msg("CDocForm::OnEnter enter");
#endif

	// начнем с нуля
	m_bActivationRequested = FALSE;

	w_CDocForm_OnEnter.RestoreOrigAddress();
	(((CDocForm *)this->*w_CDocForm_OnEnter.GetOrigMethod()))();
	w_CDocForm_OnEnter.RestoreWrapAddress();

#if defined __CSystem_ENABLE_MSG
	Msg("CDocForm::OnEnter leave");
#endif

	if (m_bActivationRequested)
	{
		CGetDoc7 *pDoc7 = (CGetDoc7 *)this;
		pDoc7->TrySetFocusOnCtrl(m_strActivationRequested, 1);
	}
}

// получить командную строку 1С (взято из FormEx)
BOOL  CSystem::funcCommandLine(CValue& RetVal, CValue **params)
{
// 	CApp7* pApp7 = (CApp7*) AfxGetApp();
// 	RetVal = pApp7->m_lpCmdLine;
//
// 			/*
// 			CApp7* pApp7 = (CApp7*) AfxGetApp();
// 			CProfile7* pProfile7 = pApp7->GetProps();
//
// 			CString Prop0 = pProfile7->GetStringProp(0x00); // каталог, в который установлен движок
// 			CString Prop1 = pProfile7->GetStringProp(0x01); // каталог базы
// 			CString Prop2 = pProfile7->GetStringProp(0x02); // каталог пользователя
// 			CString Prop3 = pProfile7->GetStringProp(0x03); // временный каталог
// 			CString Prop4 = pProfile7->GetStringProp(0x04); // полный путь к файлу 1Cv7.CFG (с именем файла)
// 			CString Prop5 = pProfile7->GetStringProp(0x05); // полный путь к файлу 1Cv7.DD (с именем файла)
// 			CString Prop6 = pProfile7->GetStringProp(0x06); // полный путь к файлу 1Cv7.DDS (с именем файла)
// 			CString Prop7 = pProfile7->GetStringProp(0x07); // полный путь к файлу 1Cv7.DBA (с именем файла)
// 			CString Prop8 = pProfile7->GetStringProp(0x08); // полный путь к файлу 1Cv7.MD (с именем файла)
// 			CString Prop9 = pProfile7->GetStringProp(0x09); // полный путь к файлу 1Cv7.LCK (с именем файла)
// 			CString PropA = pProfile7->GetStringProp(0x0A); // ???
// 			CString PropB = pProfile7->GetStringProp(0x0B); // ???
// 			CString PropC = pProfile7->GetStringProp(0x0C); // имя текущей ИБ в списке баз
// 			CString PropD = pProfile7->GetStringProp(0x0D); // имя текущего пользователя
// 			CString PropE = pProfile7->GetStringProp(0x0E); // полное имя текущего пользователя
// 			CString PropF = pProfile7->GetStringProp(0xF); // ???
// 			CString Prop10 = pProfile7->GetStringProp(0x10); // название набора прав текущего пользователя
// 			CString Prop11 = pProfile7->GetStringProp(0x11); // название интерфейса текущего пользователя
// 			CString Prop12 = pProfile7->GetStringProp(0x12); // путь в реестре к установкам текущего пользователя (относительно HKEY_CURRENT_USER\Software\1C\1Cv7\7.7)
// 			*/
// /*
//
// 		LPCTSTR m_pszAppName = pApp7->m_pszAppName;  // human readable name
// 									//  (from constructor or AFX_IDS_APP_TITLE)
// 		LPCTSTR m_pszExeName = pApp7->m_pszExeName;       // executable name (no spaces)
// 		LPCTSTR m_pszHelpFilePath = pApp7->m_pszHelpFilePath;  // default based on module path
// 		LPCTSTR m_pszProfileName = pApp7->m_pszProfileName;   // default based on app name
//
// 		m_pszAppName <1С:Предприятие>
// 		m_pszExeName <1CV7>
// 		m_pszHelpFilePath <C:\Program Files\1Cv77\Bin25\BIN\1CV7.HLP>
// 		m_pszProfileName <1Cv7\7.7\Testing 1C++\V7\Default>
// 		sAppPath <C:\Program Files\1Cv77\Bin25\BIN\1CV7.exe>
// */
//

	CBLPtr<CInformer> pInformer("");
	pInformer->funcCommandLine(RetVal, params);
 	return true;
}

BOOL  CSystem::funcGetAppPath(CValue& RetVal, CValue **params)
{
// 	CApp7* pApp7 = (CApp7*) AfxGetApp();
// 	CProfile7* pProfile7 = pApp7->GetProps();
// 	CString sAppPath = pProfile7->GetStringProp(0x00) + pApp7->m_pszExeName + ".exe";
//
// 	RetVal = sAppPath;

	CBLPtr<CInformer> pInformer("");
	pInformer->funcGetAppPath(RetVal, params);
 	return true;
}

void CSystem::ValidateParamGroupContext(const CValue& GroupContValue) const
{
	CBLContext* pGroupCont = GroupContValue.GetContext();

	if (!IS_KINDOF_CGroupContext(pGroupCont))
		RuntimeError(CString("Класс <")+GetTypeString()+"> - должен быть передан групповой контекст");
}

void CSystem::ValidateParamNumeric(const CValue& NumericValue) const
{
	if (1 != NumericValue.GetTypeCode())
		RuntimeError(CString("Класс <")+GetTypeString()+"> - второй параметр должен быть числом");
}

// немедленное закрытие формы, но с отработкой всех положенных событий
// использован код АльФ-а из ВК FormEx
//
BOOL CSystem::funcCloseForm(CValue& RetVal, CValue **ppValue)
{
	RetVal = 0L;

	CValue& GroupContValue = *ppValue[0];
	ValidateParamGroupContext(GroupContValue);

	CValue& NumericValue = *ppValue[1];
	ValidateParamNumeric(NumericValue );

	CBLContext* pGroupCont = GroupContValue.GetContext();

	CGetDoc7* pDoc = GetDoc7(pGroupCont);
	if(pDoc)
	{
		if (!NumericValue.GetNumeric().operator long())
			// TODO возможно, первый параметр должен быть другим числом,
			// но у меня ни на тестах, ни в реальной базе не падает
			pDoc->SetWantClose(0, 1); // не задавать вопросов при закрытии несохраненных документов/элементов справочников

		// нужно самостоятельно обнулить все ссылки на КонтекстФормы.Форма (1С этого почему-то не делает)
		// иначе если сохранить его в отдельной переменной до закрытия,
		// а потом пытаться использовать после физического закрытия - будет баг
		// также 1С будет падать, если эту переменную передать
		// в какой-нибудь метод любой ВК, которая принимает форму
		// например, АктивИкс.УстановитьАтрибут или Форма.УстановитьФорму

		CFormAllCtrlsContext* pForm = GetFormContext(pGroupCont);
		if (!pForm)
			return TRUE;

		CValuePtrArray& CValueArray = pForm->m_Array;
		for(int i = 0; i < CValueArray.GetSize(); ++i)
		{
			CValue* pValue = CValueArray.GetAt(i);
			pValue->Reset();
		}

		// иначе не сработает событие ФормЕкс ПослеЗакрытия :(
		CHookGroupContext::GetHooker()->IfWrapEnableThenRestoreWrapAddressForEvents();

		// мой вариант artbear
		pDoc->GetFrame()->SendMessage(WM_CLOSE, 0, 0);

		RetVal = 1L;
	}

	return TRUE;
}

BOOL CSystem::defsCloseForm(int nParam, CValue* param) const
{
	if (1 == nParam){
 		*param = 0L;
		return TRUE;
	}

	return FALSE;
}

BOOL CSystem::procSleep(CValue **ppValue)
{
	CValue& NumericValue = *ppValue[0];
	ValidateParamNumeric(NumericValue);

	Sleep(NumericValue.GetNumeric());

	return TRUE;
}

BOOL CSystem::procBeep(CValue **ppValue)
{
	DWORD nFreq = ppValue[0]->GetNumeric().operator long();
	DWORD nDuration = ppValue[1]->GetNumeric().operator long();
	Beep(nFreq, nDuration);

	return TRUE;
}

BOOL CSystem::procMessageBeep(CValue **ppValue)
{
// #define MB_ICONHAND                 0x00000010L
// #define MB_ICONQUESTION             0x00000020L
// #define MB_ICONEXCLAMATION          0x00000030L
// #define MB_ICONASTERISK             0x00000040L
	DWORD uType = ppValue[0]->GetNumeric().operator long();
	MessageBeep(uType);

	return TRUE;
}

// //получить строку-ресурса по номеру
// BOOL CSystem::funcResString(CValue& RetVal, CValue **ppValue)
// {
// 	RetVal = 0L;
//
// 	CValue& NumericValue = *ppValue[0];
// 	ValidateParamNumeric(NumericValue );
//
// 	int iResNumber = NumericValue.GetNumeric().operator long();
// 	RetVal = ResString(iResNumber);
//
// 	return TRUE;
//}

/*______________________________________________________________________________

 параметр:
	1: включить регистрацию внешних компонент в HKCR
	0: отключить регистрацию внешних компонент в HKCR
______________________________________________________________________________*/

BOOL CSystem::procRegisterAddinsInHKCU(CValue **ppValue)
{
	RedirectHKCR(ppValue[0]->GetNumeric());
	return TRUE;
}

BOOL CSystem::defsRegisterAddinsInHKCU(int nParam, CValue* param) const
{
	if (0 == nParam){
 		*param = 1L;
		return TRUE;
	}
	return FALSE;
}

BOOL CSystem::RefreshGroupContextData(CValue **ppValue)
{
	CValue& ValueGroupContext = *ppValue[0];
	ValidateParamGroupContext(ValueGroupContext);

	CGCBoost::RefreshGroupContextData(static_cast<CGroupContext*>(ValueGroupContext.GetContext()));
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL CSystem::funcGetOlePic(CValue& RetVal, CValue **params)
{
	enum CPicService::eForm nFrom = CPicService::Error;
	
	if(AGREGATE_TYPE_1C == params[0]->GetTypeCode() 
		&& !strcmp(params[0]->GetContext()->GetRuntimeClass()->m_lpszClassName, "CPictureContext"))
		nFrom = CPicService::Picture;
	else if(STRING_TYPE_1C == params[0]->GetTypeCode())
		nFrom = CPicService::Gallery;
	else
	{
		CBLModule::RaiseExtRuntimeError("Недопустимое значение третьего параметра", mmRedErr);
		return FALSE;
	}
	
	CValue* pPicValue = params[0];
	
	// Индекс картинки
	//
	int nPicIndx = params[1]->GetNumeric();
	nPicIndx = nPicIndx < -1 ? -1 : nPicIndx;
	
	// Ширина картинки
	//
	int nPicWdt = params[2]->GetNumeric();
	nPicWdt = nPicWdt < 1 ? 16 : nPicWdt; // ширина картинки не должна быть меньше 1 пиксела
	
	CPicService PicServ;
	if(!PicServ.SelectPictureHolder7(nFrom, pPicValue))
	{
		if(CPicService::Gallery == nFrom)
			CBLModule::RaiseExtRuntimeError("Не удалось получить из галереи картинку по идентификатору", mmRedErr);
		else
			CBLModule::RaiseExtRuntimeError("Не удалось получить картинку", mmRedErr);
		return FALSE;
	}
	
	// Получаем оригинальный размер картинки
	//
	CSize size = PicServ.GetOriginalPictureSize();
	
	CRect rect;
	if(nPicIndx > 0)
		rect.CopyRect(CRect(CPoint(-nPicIndx * nPicWdt, 0), size));
	else
		rect.CopyRect(CRect(CPoint(0, 0), size));
	
	if((nPicIndx + 1) > (size.cx / nPicWdt))
	{
		CBLModule::RaiseExtRuntimeError("Выход за диапазон допустимых значений", mmRedErr);
		return FALSE;
	}
	
	CSize bmpSize;
	bmpSize.cy = size.cy;
	if(nPicIndx >= 0)
		bmpSize.cx = nPicWdt;
	else
		bmpSize.cx = size.cx;
	
	// Получаем картинку
	//
	if(!PicServ.RenderPicture(nPicIndx, bmpSize, rect))
	{
		CBLModule::RaiseExtRuntimeError("Не удалось отрисовать OLE-картинку", mmRedErr);
		return FALSE;
	}
	
	IPicture* pIPicFrame = NULL;
	if(!PicServ.GetRenderedPicture(static_cast<IPicture**>(&pIPicFrame)))
	{
		CBLModule::RaiseExtRuntimeError("Не удалось получить OLE-картинку", mmRedErr);
		return FALSE;
	}
	
	if(pIPicFrame)
		DispatchToValue(pIPicFrame, &RetVal);

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL CSystem::defsGetOlePic(int nParam, CValue* param) const
{
	BOOL bRet = FALSE;

	switch(nParam)
	{
	case 0: // картинка
		param->SetTypeCode(100);
		bRet = TRUE;
		break;
	case 1: // индекс картинки
		param->SetTypeCode(1);
		*param = -1L;
		bRet = TRUE;
		break;
	case 2: // размер картинки
		param->SetTypeCode(1);
		*param = 16L;
		bRet = TRUE;
		break;
	};

	return bRet;
}

//_____________________________________________________________________________
//
BOOL CSystem::procSavePic(CValue **ppValue)
{
	enum CPicService::eForm nFrom = CPicService::Error;
	
	if(AGREGATE_TYPE_1C == ppValue[2]->GetTypeCode() 
		&& !strcmp(ppValue[2]->GetContext()->GetRuntimeClass()->m_lpszClassName, "CPictureContext"))
		nFrom = CPicService::Picture;
	else if(STRING_TYPE_1C == ppValue[2]->GetTypeCode())
		nFrom = CPicService::Gallery;
	else
	{
		CBLModule::RaiseExtRuntimeError("Недопустимое значение третьего параметра", mmRedErr);
		return FALSE;
	}
	
	CValue* pPicValue = ppValue[2];
	
	// Индекс картинки
	//
	int nPicIndx = ppValue[3]->GetNumeric();
	nPicIndx = nPicIndx < -1 ? -1 : nPicIndx;
	
	// Ширина картинки
	//
	int nPicWdt = ppValue[4]->GetNumeric();
	nPicWdt = nPicWdt < 1 ? 16 : nPicWdt; // ширина картинки не должна быть меньше 1 пиксела
	
	CPicService PicServ;
	if(!PicServ.SelectPictureHolder7(nFrom, pPicValue))
	{
		if(CPicService::Gallery == nFrom)
			CBLModule::RaiseExtRuntimeError("Не удалось получить из галереи картинку по идентификатору", mmRedErr);
		else
			CBLModule::RaiseExtRuntimeError("Не удалось получить картинку", mmRedErr);
		return FALSE;
	}
	
	// Получаем оригинальный размер картинки
	//
	CSize size = PicServ.GetOriginalPictureSize();
	
	CRect rect;
	if(nPicIndx > 0)
		rect.CopyRect(CRect(CPoint(-nPicIndx * nPicWdt, 0), size));
	else
		rect.CopyRect(CRect(CPoint(0, 0), size));
	
	if((nPicIndx + 1) > (size.cx / nPicWdt))
	{
		CBLModule::RaiseExtRuntimeError("Выход за диапазон допустимых значений", mmRedErr);
		return FALSE;
	}
	
	CSize bmpSize;
	bmpSize.cy = size.cy;
	if(nPicIndx >= 0)
		bmpSize.cx = nPicWdt;
	else
		bmpSize.cx = size.cx;
	
	// Получаем картинку
	//
	if(!PicServ.RenderPicture(nPicIndx, bmpSize, rect))
	{
		CBLModule::RaiseExtRuntimeError("Не удалось отрисовать OLE-картинку", mmRedErr);
		return FALSE;
	}
	
	// Каталог
	//
	CString strPath(ppValue[0]->GetString());
	if(!strPath.IsEmpty())
	{
		DWORD dwFileAttrs = GetFileAttributes(strPath);
		if(INVALID_FILE_ATTRIBUTES == dwFileAttrs)
		{
			CBLModule::RaiseExtRuntimeError("Указанный каталог не существует", mmRedErr);
			return FALSE;
		}
	}
	
	// Имя файла
	//
	CString strFileName(ppValue[1]->GetString());
	
	CString strFileNameLoc, strPathLoc;
	
	if(!strFileName.IsEmpty())
		strFileNameLoc = strFileName;
	
	if(!strPath.IsEmpty())
	{
		strPathLoc = strPath;
		if(strPathLoc.Right(1) != "\\")
			strPathLoc += "\\";
	}

	CString strErr;
	
	if(!PicServ.SaveRenderedPictureAsFile(strPathLoc, strFileNameLoc, strErr))
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}
	
	if(strPath.IsEmpty())
	{
		strPath		 = strPathLoc;
		
		ppValue[0]->SetTypeCode(2);
		*ppValue[0]	 = strPath;
	}
	
	if(strFileName.IsEmpty())
	{
		strFileName = strFileNameLoc;
		
		ppValue[1]->SetTypeCode(2);
		*ppValue[1] = strFileName;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL CSystem::defsSavePic(int nParam, CValue* param) const
{
	BOOL bRet = FALSE;
	
	switch(nParam)
	{
	case 0: // каталог
		param->SetTypeCode(2);
		// не нужно значение по умолчанию
		break;
	case 1: // имя файла
		param->SetTypeCode(2);
		// не нужно значение по умолчанию
		break;
	case 2: // картинка
		param->SetTypeCode(100);
		bRet = TRUE;
		break;
	case 3: // индекс картинки
		param->SetTypeCode(1);
		*param	 = -1L;
		bRet = TRUE;
		break;
	case 4: // размер картинки
		param->SetTypeCode(1);
		*param	 = 16L;
		bRet = TRUE;
		break;
	};
	
	return bRet;
}

class CSerialize
{
public:
	// важно - сериализованное представление верно только на время жизни объекта этого класса
	CString Serialize(CValue& Value);
private:
	IDispatchPtr m_IDispatchPtr;
};

BOOL CSystem::funcSerialize(CValue& RetVal, CValue **ppValue)
{
	if (!m_Serialize)
		m_Serialize = boost::shared_ptr<CSerialize>(new CSerialize);

	RetVal = m_Serialize->Serialize(ppValue[0][0]);
	return TRUE;
}

// важно - сериализованное представление верно только на время жизни объекта этого класса
CString CSerialize::Serialize(CValue& Value)
{
	CComVariant param;
	//static_cast<CBLMyEx*>(NULL)->ValueToVariantEx(Value,&param);
	CBLMyEx::ValueToVariantEx(Value,&param);

	if(param.vt != VT_DISPATCH)
		return "";

	m_IDispatchPtr = param.pdispVal;
	param.pdispVal->Release();

	IMonikerPtr moniker;
	if FAILED(CreateObjrefMoniker(m_IDispatchPtr,&moniker))
		RuntimeError("Система::Сериализовать - неудача");

	LPOLESTR pszDisplayName = NULL;
	if FAILED(moniker->GetDisplayName(NULL, NULL, &pszDisplayName))
		RuntimeError("Система::Сериализовать - неудача");

	CString strRet(pszDisplayName);

	CoTaskMemFree(pszDisplayName);

	return strRet;
}