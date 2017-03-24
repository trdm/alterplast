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
    BL_METH_PROC("AssignForm", "УстановитьФорму", 1, MethAssignForm)
    BL_METH_FUNC("CreateControl", "СоздатьЭлементУправления", 2, MethCreateControl)

	// объединяет функционал MethAssignForm и MethCreateControl
    BL_METH_FUNC("CreateControlOnForm", "СоздатьЭлементУправленияНаФорме", 3, funcCreateControlOnForm)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7Form)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7Form, "Form", "Форма", 1, NULL, NULL, -1);

enum V7FormEvents
{
    feOnCreate,
    feLastEvent
};
stCtrlEvents CV7FormEvents[]  =  
{
    {"ПриСоздании", "OnCreate", 0},
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

	return TRUE;
}

BOOL CV7Form::MethCreateControl(CValue& rValue, CValue** ppValues)
{
	if (!m_pDoc)
		RuntimeError("Форма не установлена");

	if (ppValues[0]->GetTypeCode() == STRING_TYPE_1C)
	{
		if (!rValue.CreateObject(ppValues[0]->GetString()))
			RuntimeError("Недопустимое имя класса: '%s'.", ppValues[0]->GetString());
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
		if (!static_cast<CComponentClass*>(pUDC)->GetBaseClass(&pV7Control))
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
				pField = ((CFormCtrlContext*)pCtrlContext)->m_pGetField;
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
		jmpLayoutForm = LayoutForm;
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
		jmpOnCloseDoc = OnCloseDocument;
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
