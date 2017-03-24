// V7Form.cpp: implementation of the CV7Form class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7Form.h"
#include "../V7Control.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CV7Form
//////////////////////////////////////////////////////////////////////

//BEGIN of BL context Interface

//#define NUM_METH_ASSIGNFORM 0
BEGIN_BL_METH_MAP(CV7Form)
    BL_METH_PROC("AssignForm", "УстановитьФорму", 1, &MethAssignForm)
    BL_METH_FUNC("CreateControl", "СоздатьЭлементУправления", 2, &MethCreateControl)
	BL_METH_FUNC("GetControl", "ПолучитьЭлементУправления", 1, &MethGetControl)

	// объединяет функционал MethAssignForm и MethCreateControl
    BL_METH_FUNC("CreateControlOnForm", "СоздатьЭлементУправленияНаФорме", 3, &funcCreateControlOnForm)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7Form)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7Form, "Form", "Форма", 1, NULL, NULL, -1);

enum V7FormEvents
{
    feOnCreate,
	feAfterWrite,
    feLastEvent
};
stCtrlEvents CV7FormEvents[]  =  
{
    {"ПриСоздании", "OnCreate", 0},
	{"ПослеЗаписи", "AfterWrite", 1},
};

void CV7Form::Release()
{
	if (m_pDoc)
	{
		m_pDoc = NULL;
		m_EventManager.SetCtrlObject(NULL);
		DecrRef(); //обработка привязывания к времени жизни документа
	}
}

BOOL CV7Form::MethAssignForm(CValue** ppValues)
{
	Release(); //обработка привязывания к времени жизни документа

	if (ppValues[0]->GetTypeCode() != AGREGATE_TYPE_1C)
		RuntimeError("Недопустимое значение параметра");

	CBLContext* pCtrlContext = ppValues[0]->GetContext();
	if (strcmp(pCtrlContext->GetRuntimeClass()->m_lpszClassName,"CFormAllCtrlsContext"))
		RuntimeError("Недопустимое значение параметра");
	
	// artbear
	//m_pDoc = (CGetDoc7*)((CFormAllCtrlsContext*)pCtrlContext)->m_GetDoc;
	m_pDoc = ((CFormAllCtrlsContext*)pCtrlContext)->m_GetDoc;

	IncrRef(); //обработка привязывания к времени жизни документа

	//m_EventManager.SetEvents(CV7FormEvents, feLastEvent, "Форма",
	//	(CBLModule7*)(((char*)m_pDoc) + 0x4E0), this, NULL);
	m_EventManager.SetEvents(CV7FormEvents, feLastEvent, "Форма",
		m_pDoc->m_pBLModule, this, NULL);

	if (m_pDoc->IsDocLayouted())
	{
		m_bCreated = TRUE;
	}
	else
	{
		WrapLayoutForm(m_pDoc);
	}

	WrapOnCloseDoc(m_pDoc);
	WrapDoWriteDoc(m_pDoc);
	WrapDoWriteRef(m_pDoc);

	return TRUE;
}

BOOL CV7Form::MethCreateControl(CValue& rValue, CValue** ppValues)
{
	if (!m_pDoc)
		RuntimeError("Форма не установлена");

	if (ppValues[0]->GetTypeCode() == STRING_TYPE_1C)
	{
		if (!rValue.CreateObject(ppValues[0]->GetString()))
			RuntimeError("Недопустимое имя класса: '%s'.", ppValues[0]->GetString().operator LPCTSTR());
	}
	else if (ppValues[0]->GetTypeCode() == AGREGATE_TYPE_1C)
		rValue = *ppValues[0];
	else
		RuntimeError("Недопустимое значение первого параметра.");

	CV7Control* pV7Control = NULL;
	CBLContext* pUDC = NULL;

	if (IS_BLTYPE(rValue, CComponentClass))
	{
		pUDC = rValue.GetContext();
		if (!static_cast<IUserClassContext*>(pUDC)->GetBaseClass(&pV7Control))
			RuntimeError("Не удалось определить класс ЭУ среди родителей класса '%s'.", pUDC->GetTypeString());
	}
	else
	{
		pV7Control = dynamic_cast<CV7Control*>(rValue.GetContext());
		if (!pV7Control)
			RuntimeError("Класс '%s' не является ЭУ.", rValue.GetTypeString());
	}

	if (pV7Control)
	{
		CGetField* pField = NULL;
		switch (ppValues[1]->GetTypeCode())
		{
		case AGREGATE_TYPE_1C:
			{
				CBLContext* pCtrlContext = ppValues[1]->GetContext();
				if (strcmp(pCtrlContext->GetRuntimeClass()->m_lpszClassName,"CFormCtrlContext"))
					RuntimeError("Недопустимое значение второго параметра.");
				pField = static_cast<CFormCtrlContext*>(pCtrlContext)->m_pGetField;
			}
			break;
		case STRING_TYPE_1C:
			pField = m_pDoc->GetField(ppValues[1]->GetString());
			break;
		default:
			RuntimeError("Недопустимое значение второго параметра.");
		}
		if (!pField)
			RuntimeError("Атрибут не найден.");

		if (!m_pDoc->IsDocLayouted())
			RuntimeError("Не создано окно формы.");

		if (!pV7Control->CreateControlWnd(m_pDoc->GetForm(), m_pDoc, pField, pUDC))
			RuntimeError("Неудачная попытка создания ЭУ '%s'.", rValue.GetTypeString());
	}
	else
		RuntimeError("Неизвестная ошибка создания ЭУ");

	return TRUE;
}

BOOL CV7Form::MethGetControl(CValue& rValue, CValue** ppValues)
{
	if (!m_pDoc)
		RuntimeError("Форма не установлена");

	if (!m_pDoc->IsDocLayouted())
		RuntimeError("Не создано окно формы.");

	CGetField* pField = NULL;
	switch (ppValues[0]->GetTypeCode())
	{
	case AGREGATE_TYPE_1C:
		{
			CBLContext* pCtrlContext = ppValues[0]->GetContext();
			if (strcmp(pCtrlContext->GetRuntimeClass()->m_lpszClassName,"CFormCtrlContext"))
				RuntimeError("Неверно указан атрибут формы!");
			pField = static_cast<CFormCtrlContext*>(pCtrlContext)->m_pGetField;
		}
		break;
	case STRING_TYPE_1C:
		pField = m_pDoc->GetField(ppValues[0]->GetString());
		break;
	default:
		RuntimeError("Неверно указан атрибут формы!");
	}
	if (!pField)
		RuntimeError("Атрибут формы не найден!");

	CWnd* pWnd = m_pDoc->GetFieldWnd(pField);
	if (!pWnd)
		RuntimeError("Окно атрибута формы не найдено!");

	IV7ControlContext* pCtx = safe_dynamic_cast<IV7ControlContext*>(pWnd);
	CBLContext* pCont = pCtx ? pCtx->GetControlContext() : NULL;
	pCont ? rValue.AssignContext(pCont): rValue.Reset();

	return TRUE;
}

// объединяет функционал MethAssignForm и MethCreateControl
BOOL CV7Form::funcCreateControlOnForm(CValue& rValue, CValue** ppValues)
{
	MethAssignForm(ppValues);
	MethCreateControl(rValue, &ppValues[1]);

	return TRUE;
}

//END of BL context Interface

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CV7Form::CV7Form()
{
	m_pDoc = NULL;
	m_bCreated = FALSE;
}

CV7Form::~CV7Form()
{
	UnWrapLayoutForm();
	UnWrapOnCloseDoc();
	UnWrapDoWriteDoc();
	UnWrapDoWriteRef();
}


//////////////////////////////////////////////////////////////////////
//wrap CGetDoc7::LayoutForm
//////////////////////////////////////////////////////////////////////
CMap<CV7Form*,CV7Form*,CGetDoc7*,CGetDoc7*> CV7Form::m_AssocMap;
DWORD CV7Form::OldProtect = 0;
BYTE CV7Form::TrapCodeLayoutForm[6];
BYTE* CV7Form::adrLayoutForm;
CV7Form::PFLayoutForm CV7Form::realLayoutForm;
CV7Form::PFLayoutForm CV7Form::jmpLayoutForm;

int CV7Form::LayoutForm(CFormView* pFormView)
{
	SwapCode();
	int res = (this->*realLayoutForm)(pFormView);
	SwapCode();

	CV7Form* pRealThis;
	CGetDoc7* pDoc;
	POSITION pos = m_AssocMap.GetStartPosition();
	while(pos)
	{
		m_AssocMap.GetNextAssoc(pos,pRealThis,pDoc);
		if(pDoc == (CGetDoc7*)this)
		{
			pRealThis->m_bCreated = TRUE;
			if (pRealThis->m_EventManager.IsAppointed(feOnCreate))
			{
				CExecBatchHelper ExecBatchHelper(pDoc, FALSE);
				pRealThis->m_EventManager.Invoke0(feOnCreate);
			}
			pRealThis->UnWrapLayoutForm();
			// оставляем однократное событие
			break;
		}
	}
	return res;
}

void CV7Form::WrapLayoutForm(CGetDoc7* pDoc)
{
	if (m_AssocMap.IsEmpty())
	{
		HINSTANCE h7 = GetModuleHandle("basic.dll");
		adrLayoutForm = (BYTE*)GetProcAddress(h7,"?LayoutForm@CGetDoc7@@UAEHPAVCFormView@@@Z");
		realLayoutForm = *(PFLayoutForm*)&adrLayoutForm;
		for(int i = 0;i<6;i++)
			TrapCodeLayoutForm[i] = adrLayoutForm[i];
		VirtualProtect(adrLayoutForm, 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		jmpLayoutForm = &CV7Form::LayoutForm;
		*adrLayoutForm++ = 0xFF;
		*adrLayoutForm++ = 0x25;
		*(DWORD*)adrLayoutForm = (DWORD)&jmpLayoutForm;
		adrLayoutForm -= 2;
	}
	m_AssocMap[this] = pDoc;
}

void CV7Form::UnWrapLayoutForm()
{
	if(!m_AssocMap.IsEmpty())
	{
		m_AssocMap.RemoveKey(this);
		if(m_AssocMap.IsEmpty())
		{
			for(int i = 0;i<6;i++)
				adrLayoutForm[i] = TrapCodeLayoutForm[i];
			DWORD dwProtect;
			VirtualProtect(adrLayoutForm,6,OldProtect,&dwProtect);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//wrap CGetDoc7::OnCloseDocument
//////////////////////////////////////////////////////////////////////
CMap<CV7Form*,CV7Form*,CGetDoc7*,CGetDoc7*> CV7Form::m_AssocMapOnCloseDoc;
DWORD CV7Form::OldProtectOnCloseDoc = 0;
BYTE CV7Form::TrapCodeOnCloseDoc[6];
BYTE* CV7Form::adrOnCloseDoc;
CV7Form::PFOnCloseDoc CV7Form::realOnCloseDoc;
CV7Form::PFOnCloseDoc CV7Form::jmpOnCloseDoc;

void CV7Form::OnCloseDocument()
{
	SwapCodeOnCloseDoc();
	(this->*realOnCloseDoc)();
	SwapCodeOnCloseDoc();

	CV7Form* pRealThis;
	CGetDoc7* pDoc;
	POSITION pos = m_AssocMapOnCloseDoc.GetStartPosition();
	while(pos)
	{
		m_AssocMapOnCloseDoc.GetNextAssoc(pos,pRealThis,pDoc);
		if(pDoc == (CGetDoc7*)this)
		{
			pRealThis->UnWrapOnCloseDoc();
			pRealThis->Release(); //обработка привязывания к времени жизни документа
		}
	}
}

void CV7Form::WrapOnCloseDoc(CGetDoc7* pDoc)
{
	if (m_AssocMapOnCloseDoc.IsEmpty())
	{
		HINSTANCE h7 = GetModuleHandle("basic.dll");
		adrOnCloseDoc = (BYTE*)GetProcAddress(h7,"?OnCloseDocument@CGetDoc7@@UAEXXZ");
		realOnCloseDoc = *(PFOnCloseDoc*)&adrOnCloseDoc;
		for(int i = 0;i<6;i++)
			TrapCodeOnCloseDoc[i] = adrOnCloseDoc[i];
		VirtualProtect(adrOnCloseDoc, 6, PAGE_EXECUTE_READWRITE, &OldProtectOnCloseDoc);
		jmpOnCloseDoc = &CV7Form::OnCloseDocument;
		*adrOnCloseDoc++ = 0xFF;
		*adrOnCloseDoc++ = 0x25;
		*(DWORD*)adrOnCloseDoc = (DWORD)&jmpOnCloseDoc;
		adrOnCloseDoc -= 2;
	
	}
	m_AssocMapOnCloseDoc[this] = pDoc;
}

void CV7Form::UnWrapOnCloseDoc()
{
	if(!m_AssocMapOnCloseDoc.IsEmpty())
	{
		m_AssocMapOnCloseDoc.RemoveKey(this);
		if(m_AssocMapOnCloseDoc.IsEmpty())
		{
			for(int i = 0;i<6;i++)
				adrOnCloseDoc[i] = TrapCodeOnCloseDoc[i];
			DWORD dwProtect;
			VirtualProtect(adrOnCloseDoc,6,OldProtectOnCloseDoc,&dwProtect);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// wrap DoWriteDoc
//////////////////////////////////////////////////////////////////////////
CMap<CV7Form*, CV7Form*, CGetDoc7*, CGetDoc7*> CV7Form::m_mapDoWriteDoc;
CTrapSwap CV7Form::m_trDoWriteDoc;

void CV7Form::WrapDoWriteDoc(CGetDoc7* pDoc)
{
	if (!m_trDoWriteDoc.IsTrapped())
		m_trDoWriteDoc.SetTrapByName("basic.dll", "?DoWriteDoc@CDocForm@@QAEHHHHPAVCValue@@PAVCString@@PAH@Z", &CV7Form::DoWriteDoc);

	m_mapDoWriteDoc[this] = pDoc;
}

void CV7Form::UnWrapDoWriteDoc()
{
	m_mapDoWriteDoc.RemoveKey(this);
}

int CV7Form::DoWriteDoc(int n1, int n2, int n3, CValue* pVal, CString* pStr, int* n4)
{
	m_trDoWriteDoc.Swap();
	CDocForm* pDocForm = reinterpret_cast<CDocForm*>(this);
	int nRes = pDocForm->DoWriteDoc(n1, n2, n3, pVal, pStr, n4);
	if (!m_mapDoWriteDoc.IsEmpty())
	{
		m_trDoWriteDoc.Swap();

		CV7Form* pThis;
		CGetDoc7* pDoc;
		POSITION pos = m_mapDoWriteDoc.GetStartPosition();
		while(pos)
		{
			m_mapDoWriteDoc.GetNextAssoc(pos, pThis, pDoc);
			if(pDoc == pDocForm)
			{
				if (pThis->m_EventManager.IsAppointed(feAfterWrite))
				{
					CExecBatchHelper ExecBatchHelper(pDoc, FALSE);
					CValue vRes = nRes;
					pThis->m_EventManager.Invoke1(feAfterWrite, vRes);
				}
				// оставляем однократное событие
				break;
			}
		}
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
// wrap DoWriteRef
//////////////////////////////////////////////////////////////////////////
CMap<CV7Form*, CV7Form*, CGetDoc7*, CGetDoc7*> CV7Form::m_mapDoWriteRef;
CTrapSwap CV7Form::m_trDoWriteRef;
CV7Form::PF_DoWriteRef CV7Form::m_pDoWriteRef = NULL;

void CV7Form::WrapDoWriteRef(CGetDoc7* pDoc)
{
	if (!m_trDoWriteRef.IsTrapped())
	{
		CString ver = GetModuleVersionInfo("basic.dll", "FileVersion");
		CString minorver = ver.Right(3);

		void* pAddr = 0;
		switch(atoi(minorver))
		{
		case 27:
		case 26:
			pAddr = (void*)0x2A0D57C0;
			break;
		case 25:
			pAddr = (void*)0x2A0D56A0;
			break;
		case 23:
		case 21:
			pAddr = (void*)0x2A0D5650;
			break;
		case 20:
			pAddr = (void*)0x2A0D5530;
			break;
		case 19:
			pAddr = (void*)0x2A0D5420;
			break;
		case 18:
			pAddr = (void*)0x2A0D5300;
			break;
		};

		union
		{
			PF_DoWriteRef pf;
			void* pAddr;
		} uAddr;

		uAddr.pAddr = pAddr;
		m_pDoWriteRef = uAddr.pf;

		if (m_pDoWriteRef)
			m_trDoWriteRef.SetTrapOnFunc(pAddr, &CV7Form::DoWriteRef);
	}

	m_mapDoWriteRef[this] = pDoc;
}

void CV7Form::UnWrapDoWriteRef()
{
	m_mapDoWriteRef.RemoveKey(this);
}

int CV7Form::DoWriteRef(int n1)
{
	m_trDoWriteRef.Swap();
	CSubcForm* pSubcForm = reinterpret_cast<CSubcForm*>(this);
	int nRes = (pSubcForm->*m_pDoWriteRef)(n1);
	if (!m_mapDoWriteRef.IsEmpty())
	{
		m_trDoWriteRef.Swap();

		CV7Form* pThis;
		CGetDoc7* pDoc;
		POSITION pos = m_mapDoWriteRef.GetStartPosition();
		while(pos)
		{
			m_mapDoWriteRef.GetNextAssoc(pos, pThis, pDoc);
			if(pDoc == pSubcForm)
			{
				if (pThis->m_EventManager.IsAppointed(feAfterWrite))
				{
					CExecBatchHelper ExecBatchHelper(pDoc, FALSE);
					CValue vRes = nRes;
					pThis->m_EventManager.Invoke1(feAfterWrite, vRes);
				}
				// оставляем однократное событие
				break;
			}
		}
	}
	return nRes;
}
