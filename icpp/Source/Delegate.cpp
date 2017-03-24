//	Delegate.cpp: implementation of the CDelegate class.
//
//	Based on Deb initial release
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "Delegate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CDelegate) 
	BL_METH_PROC_DEF_PARAM("DefineType",		"ОпределитьТип",	3,		&procDefineType, &defsDefineType)
	BL_METH_FUNC_DEF_PARAM("AddMethod",		"Добавить",	3,	&funcAdd, &defsAdd)
    BL_METH("RemMethod",		"Удалить",			2,		NULL,			&funcRemove,			NULL)
    BL_METH("RemMethodByKey",	"УдалитьПоКлючу",	1,		NULL,			&funcRemoveByKey,	NULL)
    BL_METH("Invoke",			"Вызвать",			0,		NULL,			&funcInvoke,			NULL)
    BL_METH("InvokeVL",			"ВызватьСЗ",		1,		NULL,			&funcInvokeVL,		NULL)
    BL_METH("InvokeByKey",		"ВызватьПоКлючу",	1,		NULL,			&funcInvokeByKey,	NULL)
    BL_METH("InvokeVLByKey",	"ВызватьСЗПоКлючу",	2,		NULL,			&funcInvokeVLByKey,	NULL)
    BL_METH("Erase",			"Очистить",			0,		&procErase,		NULL,				NULL)
    BL_METH("IsEmpty",			"Пустой",			0,		NULL,			&funcIsEmpty,		NULL)

    BL_METH_FUNC("GetHandlersTable",	"ПолучитьТаблицуОбработчиков",	0,		&funcGetHandlersTable)
    BL_METH_FUNC("GetHandlersTableByKey",	"ПолучитьТаблицуОбработчиковПоКлючу",	1,		&funcGetHandlersTableByKey)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CDelegate)
//    BL_PROP("propEng", "propRus", propGet, propSet)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CDelegate, "Delegate", "Делегат", 1, NULL, NULL, -1);
//_____________________________________________________________________________
//
LPCSTR CDelegate::m_voidKey = "";
LPCSTR CDelegate::m_defaultKey = "__default";
//_____________________________________________________________________________
//
CDelegate::CDelegate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ResetType();

	int * pn = aInvokeMethods;
	*pn++ = FindMethod("Invoke");
	*pn++ = FindMethod("InvokeByKey");
}
//_____________________________________________________________________________
//
CDelegate::~CDelegate()
{
	Erase(false);
}
//_____________________________________________________________________________
//
// checks
//_____________________________________________________________________________
//
void CDelegate::CheckType() const
{
	if (m_nParamCount == -1)
		RuntimeError("Перед использованием делегата необходимо установить его тип методом Делегат::ОпределитьТип()");
}

void CDelegate::CheckMethName(CString& sMethName, LPCSTR sFuncName, LPCSTR sParamNum)
{
	if (sMethName.IsEmpty())      
		RuntimeError("%s параметр метода Делегат::%s необходимо передавать название метода!", sFuncName, sParamNum);
}

void CDelegate::CheckContext(CBLContext *pCont, LPCSTR sFuncName, LPCSTR sParamNum)
{
	if (!pCont)
		RuntimeError("%s параметр метода Делегат::%s необходимо передавать агрегатные объекты 1С (воспользуйтесь: CreateObject())!", sFuncName, sParamNum);
}

int CDelegate::CheckFindMethod(CBLContext *pCont, CString& sMethName)
{
	int nMeth = pCont->FindMethod(sMethName);
	if (nMeth == -1)
		RuntimeError("В классе '%s' не найден метод '%s'", pCont->GetTypeString(), sMethName.operator LPCTSTR());

	return nMeth;
}
//_____________________________________________________________________________
//
// service
//_____________________________________________________________________________
//
LPCSTR CDelegate::ResolveKey(LPCSTR key) const
{
	if (!key)
		key = m_voidKey;

	return key;
}

LPCSTR CDelegate::ResolveKey(CValue* pcvKey) const
{
	LPCSTR key = NULL;

	if ( !pcvKey->IsEmpty() )
		key = pcvKey->GetString().operator LPCTSTR();
	else
		key = ResolveKey(key);

	return key;
}

CBLContext* CDelegate::GetContext(const CValue *pvCont) const
{
	return pvCont->GetContext();
}

CValue** CDelegate::GetVLParams(CValue const& val) const
{
	CheckType();
	
	CValueItemList* pa = CValue2VL(val);
	if (!pa)
		RuntimeError("Неверный параметр 2 (ожидается список значений)");
	
	if (pa->GetSize() < m_nParamCount)
		RuntimeError("Недостаточное число параметров передано в списке значений");
	
	CValue** vl_params = reinterpret_cast<CValue**>(pa->GetData());
	return vl_params;
}
//_____________________________________________________________________________
//
// frontend определение типа
//_____________________________________________________________________________
//
BOOL CDelegate::procDefineType(CValue **params)
{
	Erase(false);
	DefineType(params[0]->GetNumeric(), params[1]->GetNumeric(), params[2]->GetNumeric());

	return TRUE;
}

int	 CDelegate::defsDefineType(int nParam, CValue* param)const
{
	if(nParam == 2){
		*param = long(0);
		return TRUE;
	}
	
	return FALSE;
}

//_____________________________________________________________________________
//
// backend определение типа
//_____________________________________________________________________________
//
void CDelegate::DefineType(long nParamCount, long nIsFunc, long nHonorBreakRequests)
{
	m_nParamCount	= nParamCount;
	m_nIsFunc		= nIsFunc ? 1 : 0;
	m_bHonorBreakRequests = nHonorBreakRequests ? true : false;
}
//_____________________________________________________________________________
//
void CDelegate::ResetType()
{
	DefineType(-1, 0, 0);
}
//_____________________________________________________________________________
//
// frontend добавление метода с ключом
//_____________________________________________________________________________
//
BOOL CDelegate::funcAdd(CValue &RetVal, CValue **params)
{
	LPCSTR key = ResolveKey(params[2]);

	RetVal = Add(params, key);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend добавление метода по ключу
// параметры: [0]: объект, [1]; имя метода; key: ключ
//_____________________________________________________________________________
//
int	CDelegate::defsAdd(int nParam, CValue* param)const
{
	if(nParam == 2){
		*param = "";
		return TRUE;
	}
	
	return FALSE;
}

long CDelegate::Add(CValue **params, LPCSTR key)
{
	CheckType();

	CBLContext *pCont = GetContext(params[0]);
	CheckContext(pCont, "Добавить()", "В первый");

	CString sMethName = params[1]->GetString();
	CheckMethName(sMethName, "Добавить()", "Во второй");

	int nMeth = CheckFindMethod(pCont, sMethName);

	if (pCont->HasRetVal(nMeth) != m_nIsFunc)
		RuntimeError("В классе '%s' метод '%s' объявлен как %s",
		pCont->GetTypeString(), sMethName.operator LPCTSTR(), m_nIsFunc == 1 ? "процедура, необходима функция" : "функция, необходима процедура");

	if (pCont->GetNParams(nMeth) != m_nParamCount)
	{
		LPSTR sParam;
		int nParamCount = m_nParamCount % 10;
		if (nParamCount == 0 || nParamCount > 4)
			sParam = "параметров";
		else if (nParamCount == 1)
			sParam = "параметр";   
		else
			sParam = "параметра";   
		
		RuntimeError("В классе '%s' метод '%s' должен принимать %d %s!", pCont->GetTypeString(), sMethName.operator LPCTSTR(), m_nParamCount, sParam);
	}

	CValue *pvCont = new CValue(*params[0]);
	InfoEventHandlers info(pCont, nMeth, pvCont);
/*
	// ГрупповойКонтекст - особый случай
	LPCSTR szTypeString = pCont->GetTypeString();
	if ( !m_map.Compare(szTypeString, "ГрупповойКонтекст") || !m_map.Compare(szTypeString, "GroupContext"))
		m_pBLGroupContext = pCont;
*/
	key = ResolveKey(key);

	LIST *hl;
	if ( !m_map.Lookup(key, hl) || !hl)
	{
		hl = new LIST;
		m_map[key] = hl;
	}
	
	hl->insert(hl->end(), info);
	return 1;
}
//_____________________________________________________________________________
//
// frontend удаление метода объекта по всем ключам
// параметры: [0]: объект, [1]: имя метода
//_____________________________________________________________________________
//
BOOL CDelegate::funcRemove(CValue &RetVal, CValue **params)
{
	CheckType();

	CBLContext *pCont = params[0]->GetContext();
	CheckContext(pCont, "Удалить()", "В первый");

	CString sMethName = params[1]->GetString();
	CheckMethName(sMethName, "Удалить()", "Во второй");

	int nMeth = CheckFindMethod(pCont, sMethName);

	LPCSTR key;
	LIST *hl;
	long bFound = 0;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		ITER iter;
		while ((iter = find(hl->begin(), hl->end(), InfoEventHandlers(pCont, nMeth, 0L))) != hl->end())
		{
			bFound = 1;

			hl->erase(iter);
		}
		if (hl->empty())
		{
			delete hl;
			m_map.RemoveKey(key);
		}
	}
	RetVal = bFound;
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend удаление всех методов по ключу
// параметры: [0]: ключ
//_____________________________________________________________________________
//

BOOL CDelegate::funcRemoveByKey(CValue &RetVal, CValue **params)
{
	CheckType();

	LPCSTR key = ResolveKey(params[0]);
	LIST *hl;
	long bFound = 0;

	if (m_map.Lookup(key, hl) && hl)
	{
		bFound = EmptyList(hl);

		m_map.RemoveKey(key);
	}
	RetVal = bFound;
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend вызов всех методов без учета ключа
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvoke(CValue &RetVal, CValue **params)
{
	Invoke(RetVal, params, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend вызов всех методов без учета ключа, параметры в СЗ
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeVL(CValue &RetVal, CValue **params)
{
	CValue** vl_params = GetVLParams(*params[0]);
	Invoke(RetVal, vl_params, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend вызов всех методов без учета ключа
//_____________________________________________________________________________
//
void CDelegate::Invoke(CValue &RetVal, CValue **params, bool bHonorBreakRequests)
{
	CheckType();

	LPCSTR key = ResolveKey(static_cast<LPCSTR>(NULL));
 	LIST *hl;

	RetVal = 1L;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		if (!CallFromList(hl, RetVal, params, bHonorBreakRequests))
			return;
	}
}
//_____________________________________________________________________________
//
// frontend вызов всех методов по ключу
// параметры: [0]: ключ, [...] параметры делегата
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeByKey(CValue &RetVal, CValue **params)
{
	CheckType();

	LPCSTR key = ResolveKey(params[0]);

	InvokeByKey(RetVal, params + 1, key, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend вызов всех методов по ключу
// параметры: [0]: ключ, [1] параметры в СЗ
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeVLByKey(CValue &RetVal, CValue **params)
{
	CheckType();
	
	LPCSTR key = ResolveKey(params[0]);

	CValue** vl_params = GetVLParams(*params[1]);
	InvokeByKey(RetVal, vl_params, key, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend вызов всех методов по ключу
// параметры: [...]: параметры делегата
//_____________________________________________________________________________
//
void CDelegate::InvokeByKey(CValue &RetVal, CValue **params, LPCSTR key, bool bHonorBreakRequests)
{
 	LIST *hl;
	
	RetVal = 1L;
	
	if (m_map.Lookup(key, hl) && hl)
	{
		if (!CallFromList(hl, RetVal, params, bHonorBreakRequests))
			return;
	}
	else if (key != m_defaultKey)
	{
		InvokeByKey(RetVal, params, m_defaultKey, bHonorBreakRequests);
	}
}

bool CDelegate::CallFromList(LIST *hl, CValue &RetVal, CValue **params, bool bHonorBreakRequests)
{
	if (!hl)
		return false;

	CValue vZero = CValue(0L);

	CONST_ITER iter;
	for (iter = hl->begin(); iter != hl->end(); iter++ )
	{
		//CBLContext *pBLCont = iter->pBLCont;
		CBLContext *pBLCont = GetContext(iter->pvCont.get());
		if (pBLCont)
		{
			int nMeth = iter->nNumMethOfCall;
			
			if (m_nIsFunc)
			{
				pBLCont->CallAsFunc(nMeth, RetVal, params);
				if (bHonorBreakRequests)
					if (RetVal == vZero)                  
						return false;
			}
			else
				pBLCont->CallAsProc(nMeth, params);
		}
	}
	return true;
}
//_____________________________________________________________________________
//
// frontend очистка
//_____________________________________________________________________________
//
BOOL CDelegate::procErase(CValue **params)
{
	Erase(true);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend очистка
//_____________________________________________________________________________
//
void CDelegate::Erase(bool bResetType)
{
	LPCSTR key;
	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		EmptyList(hl);
	}
	m_map.RemoveAll();

	if (bResetType)
		ResetType();
}

int CDelegate::EmptyList(LIST *hl)
{
	int bFound = 0;
 	REVERSE_ITER iter(hl->rbegin());  // artbear - не нужно
 	for (; iter != hl->rend(); iter++ )
	{
		bFound = 1;
		iter->pvCont = sh_ptr<CValue>(NULL); // чтобы соблюсти порядок создания-удаления
	}
	delete hl;

	return bFound;
}
//_____________________________________________________________________________
//
// frontend проверка
//_____________________________________________________________________________
//
BOOL CDelegate::funcIsEmpty(CValue &RetVal, CValue **params)
{
	LPCSTR key;
	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	long bEmpty = 1;

	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);
		if ( hl && !hl->empty() )
		{
			bEmpty = 0;
			break;
		}
	}

	RetVal = bEmpty;
	return TRUE;
}
//_____________________________________________________________________________
//
// получить число параметров делегата
//_____________________________________________________________________________
//
int CDelegate::GetDefinedParamCount() const
{
	return m_nParamCount;
}
//_____________________________________________________________________________
//
// context overload
//_____________________________________________________________________________
//
int CDelegate::GetNParams(int iMethodNum) const
{
	_S_MyContextBaseInfo* pInfo=GetBaseInfo();
	if(((DWORD)iMethodNum)>=pInfo->methCount)
		return 0;

	// учет методов переменной длины
	int nParamCount = 0;
	for (int i = 0; i < nInvokeMethods; ++i)
	{
		if (iMethodNum == aInvokeMethods[i])
		{
			nParamCount = m_nParamCount;
			break;
		}
	}

	return pInfo->pMethodDef[iMethodNum].NumberOfParam + nParamCount;
}
//_____________________________________________________________________________
//

BOOL CDelegate::funcGetHandlersTable(CValue& RetVal, CValue **params)
{
	GetHandlersTable(RetVal);
	return TRUE;
}

void CDelegate::GetHandlersTable(CValue& RetVal) const
{
	sh_array<CValue> pv;
	sh_array<CValue*> ppv;
	CValueTable* pVT = PrepareHandlersTable(RetVal, pv, ppv);

	LPCSTR key = ResolveKey(static_cast<LPCSTR>(NULL));
 	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		SaveToHandlersTable(hl, pVT, pv, ppv);
	}
}

// параметры: [0]: ключ
BOOL CDelegate::funcGetHandlersTableByKey(CValue& RetVal, CValue **params)
{
	LPCSTR key = ResolveKey(params[0]);

	GetHandlersTableByKey(RetVal, key);

	return TRUE;
}

void CDelegate::GetHandlersTableByKey(CValue &RetVal, LPCSTR key) const
{
	sh_array<CValue> pv;
	sh_array<CValue*> ppv;
	CValueTable* pVT = PrepareHandlersTable(RetVal, pv, ppv);

	LIST *hl;
	
	if (m_map.Lookup(key, hl) && hl)
	{
		SaveToHandlersTable(hl, pVT, pv, ppv);
	}
	else if (key != m_defaultKey)
	{
		RetVal.Reset();
		GetHandlersTableByKey(RetVal, m_defaultKey);
	}
}

CValueTable* CDelegate::PrepareHandlersTable(CValue& RetVal, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const
{
	RetVal.CreateObject("ТаблицаЗначений");
	CValueTable* pVT = static_cast<CValueTableContextData*>(RetVal.GetContext()->GetInternalData())->GetValueTable();
	if(!pVT)
		RuntimeError("Не удалось получить таблицу значений");

	CString str;
	CType ctStr(STRING_TYPE_1C);
	CType ctAGREGATE(AGREGATE_TYPE_1C);

	pVT->Clear(TRUE);
	pVT->AddColumn(str = "Объект",				ctAGREGATE, str, 0, "", 0);
	pVT->AddColumn(str = "НаименованиеКласса",	ctStr,		str, 0, "", 0);
	pVT->AddColumn(str = "НаименованиеМетода",	ctStr,		str, 0, "", 0);

  	pv = sh_array<CValue>(new CValue [3]);
  	ppv = sh_array<CValue*>(new CValue* [3]);

	for (int i = 0; i < 3; ++i)
		ppv[i] = &pv[i];

	return pVT;
}

void CDelegate::SaveToHandlersTable(LIST *hl, CValueTable* pVT, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const
{
	if (!hl)
		return;

	CONST_ITER iter;
	for (iter = hl->begin(); iter != hl->end(); iter++ )
	{
		CBLContext *pCont = GetContext(iter->pvCont.get());
		if (pCont)
		{
			pv[0] = *iter->pvCont;
			pv[1] = pCont->GetTypeString();
			pv[2] = pCont->GetMethodName(iter->nNumMethOfCall, 1);

			pVT->AddRow(ppv.get(), -1);
		}
	}
}
