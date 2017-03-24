// V7Form.h: interface for the CV7Form class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_V7FORM_H__C6DE91C3_8EC2_4DA4_9515_2CC68DB485D6__INCLUDED_)
#define AFX_V7FORM_H__C6DE91C3_8EC2_4DA4_9515_2CC68DB485D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../mycontextbase.h"
#include "../CtrlEvents.h"
#include "../MethodsWrapper.h"

class CV7Form : public CMyContextBase
{
    DECLARE_DYNCREATE(CV7Form);
public:
	CV7Form();
	virtual ~CV7Form();
	BOOL MethAssignForm(CValue** ppValues);
	BOOL MethCreateControl(CValue& rValue, CValue** ppValues);
	BOOL MethGetControl(CValue& rValue, CValue** ppValues);

	void Release();

	// объединяет функционал MethAssignForm и MethCreateControl
	BOOL funcCreateControlOnForm(CValue& rValue, CValue** ppValues);

public:
	inline CGetDoc7* GetDoc(){return m_pDoc;};

private:
	BOOL m_bCreated;
	CGetDoc7* m_pDoc;
    CCtrlEventManager m_EventManager;

	//wrap DoWriteDoc
	static CMap<CV7Form*, CV7Form*, CGetDoc7*, CGetDoc7*> m_mapDoWriteDoc;
	static CTrapSwap m_trDoWriteDoc;
	int DoWriteDoc(int n1, int n2, int n3, CValue* pVal, CString* pStr, int* n4);
	void WrapDoWriteDoc(CGetDoc7* pDoc);
	void UnWrapDoWriteDoc();

	//wrap DoWriteRef
	typedef int (CGetDoc7::*PF_DoWriteRef)(int);
	static PF_DoWriteRef m_pDoWriteRef;
	static CMap<CV7Form*, CV7Form*, CGetDoc7*, CGetDoc7*> m_mapDoWriteRef;
	static CTrapSwap m_trDoWriteRef;
	int DoWriteRef(int n1);
	void WrapDoWriteRef(CGetDoc7* pDoc);
	void UnWrapDoWriteRef();

//todo:
//По перехватам требуется рефакторинг
//Будут идеи - перепишу

//wrap CGetDoc7::LayoutForm
	typedef int (CV7Form::*PFLayoutForm)(CFormView*);
	int LayoutForm(CFormView*);
	void WrapLayoutForm(CGetDoc7*);
	void UnWrapLayoutForm();
	static CMap<CV7Form*,CV7Form*,CGetDoc7*,CGetDoc7*> m_AssocMap;
	static DWORD OldProtect;
	static BYTE TrapCodeLayoutForm[6];
	static BYTE* adrLayoutForm;
	static PFLayoutForm realLayoutForm;
	static PFLayoutForm jmpLayoutForm;
	static inline void SwapCode()
	{
		BYTE *read,*write = TrapCodeLayoutForm,symb;
		read = *(BYTE**)&realLayoutForm;
		for(int i = 0;i<6;i++)
			symb = *read,*read++ = *write,*write++ = symb;
	}

//wrap CGetDoc7::OnCloseDocument
	typedef void (CV7Form::*PFOnCloseDoc)();
	void OnCloseDocument();
	void WrapOnCloseDoc(CGetDoc7*);
	void UnWrapOnCloseDoc();
	static CMap<CV7Form*,CV7Form*,CGetDoc7*,CGetDoc7*> m_AssocMapOnCloseDoc;
	static DWORD OldProtectOnCloseDoc;
	static BYTE TrapCodeOnCloseDoc[6];
	static BYTE* adrOnCloseDoc;
	static PFOnCloseDoc realOnCloseDoc;
	static PFOnCloseDoc jmpOnCloseDoc;
	static inline void SwapCodeOnCloseDoc()
	{
		BYTE *read,*write = TrapCodeOnCloseDoc,symb;
		read = *(BYTE**)&realOnCloseDoc;
		for(int i = 0;i<6;i++)
			symb = *read,*read++ = *write,*write++ = symb;
	}

public:
    DECLARE_MY_CONTEXT()
};

#endif // !defined(AFX_V7FORM_H__C6DE91C3_8EC2_4DA4_9515_2CC68DB485D6__INCLUDED_)
