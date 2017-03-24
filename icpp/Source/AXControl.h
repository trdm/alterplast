//////////////////////////////////////////////////////////////////////
// Класс АктивИксКонтрол
// Разработчик: Рыбальченко Степан aka Steban
// Контакты: e-mail: steban@mail.ru, ICQ: 81707670
//////////////////////////////////////////////////////////////////////
// AXControl.h: interface for the CAXControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXCONTROL_H__D8FD4B9A_02B0_4E5B_8669_103A3172B113__INCLUDED_)
#define AFX_AXCONTROL_H__D8FD4B9A_02B0_4E5B_8669_103A3172B113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycontextbase.h"
#include "V7Control.h"
//#include <..\src\occimpl.h>
#include "CtrlEvents.h"

#include "System/sh_ptr.hpp"

#define IMPLTYPE_MASK (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE|\
   IMPLTYPEFLAG_FRESTRICTED)
#define IMPLTYPE_DEFAULTSOURCE (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE)
#define HOSTING_CONTROL_TYPE 1

class CAXControl;



struct EventInfo
{
	DWORD procid;
	DWORD argNum;
};

typedef CMap<DISPID,DISPID,EventInfo,EventInfo> CDispIdToProcIdMap ;

class CAXCtrlContainer;

class CAXControlWnd : public CWnd
{
	CAXControlWnd(CAXCtrlContainer* pCont);
	friend class CAXCtrlContainer;
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()
	void OnSetFocus(CWnd* pOldWnd);
	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	CAXCtrlContainer* m_pCont;

//protected:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

class CAXCtrlContainer : public CWnd, public IV7ControlContext
{
	friend class CAXControl;
	friend class CAXControlWnd;

	CBLContext* GetControlContext() const;

	CAXCtrlContainer(CAXControl* pParent) : 
		m_EventHandlers(new CDispIdToProcIdMap(10)),
		m_ComponentEventHandlers(new CDispIdToProcIdMap(10))
		,m_iid( IID_NULL ), m_dwEventCookie(0), m_pIConnectionPoint(NULL)
	{
		m_iEventProcessing=1;
		m_pComponentBLModule=NULL;
		m_pDoc=NULL;
		m_pControl=NULL;
		m_bIsTab = FALSE;
		m_pParentObj = pParent;
	}
	~CAXCtrlContainer();

	BOOL m_iEventProcessing;
	CGetDoc7* m_pDoc;
	CBLModule* m_pComponentBLModule;
	CString m_ControlID;
	sh_ptr<CDispIdToProcIdMap> m_EventHandlers;
	sh_ptr<CDispIdToProcIdMap> m_ComponentEventHandlers;
	BOOL m_bIsTab;
	CAXControlWnd* m_pControl;
	CAXControl* m_pParentObj;
	
	void SetupEventHandlers(void);
	void CallEventHandler(EventInfo ei, CValue** lArray, CBLModule7* pModule);
	BOOL OnCtrlCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL CreateCtrlWnd(const CString& strClassName, CValue& RetVal);
	void Reset();
	static CAXCtrlContainer* CreateContainer(CAXControl*, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC);


	DECLARE_MESSAGE_MAP()
	void OnSize(UINT nType, int cx, int cy)
	{
		if(m_pControl)
		{
			CRect rc;
			GetClientRect(rc);
			m_pControl->MoveWindow(rc);
		}
	}
	void OnSetFocus(CWnd* pOldWnd)
	{
		if(m_pControl)
		{
			if(pOldWnd != m_pControl)	// Фокус зашел не из контрола
			{
				int id = GetDlgCtrlID();	// Устанавливаем активный элемент
				m_pDoc->SetActiveCtl(id);
			}
			::SetFocus(m_pControl->m_hWnd); // Устанавливаем фокус через WinAPI, отработает железно
			m_pControl->SetFocus();			// А тут отработает активация через IOleInPlaceObject
		}
	}
	UINT OnGetDlgCode()
	{
		return m_bIsTab ? DLGC_BUTTON : DLGC_STATIC;
	}
	void PostNcDestroy()
	{
		delete this;
	}
	void OnPaint()
	{
		CPaintDC dc(this);
		if(!m_pControl)
		{
			CRect rc;
			GetClientRect(rc);
			CBrush br;
			br.CreateSolidBrush(RGB(255,255,255));
			dc.FillRect(rc,&br);
			dc.MoveTo(0, 0);
			dc.LineTo(rc.right, rc.bottom);
			dc.MoveTo(rc.right, 0);
			dc.LineTo(0, rc.bottom);
			dc.TextOut(0, 0, "Здесь должен быть АктивИкс");
		}
	}

//protected:
	CCtrlEventManager m_EventManager;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	// универсальное подключение к событиям АктивИкс - artbear
	
	IID m_iid;
	IConnectionPointPtr m_pIConnectionPoint;
	DWORD m_dwEventCookie;
	
	void OnEvent( DISPID dispid, DISPPARAMS* pdpParams );
	
	BEGIN_INTERFACE_PART( EventHandler, IDispatch )
		STDMETHOD( GetIDsOfNames )( REFIID iid, LPOLESTR* ppszNames, UINT nNames,
		LCID lcid, DISPID* pDispIDs );
	STDMETHOD( GetTypeInfo )( UINT iTypeInfo, LCID lcid,
		ITypeInfo** ppTypeInfo );
	STDMETHOD( GetTypeInfoCount )( UINT* pnInfoCount );
	STDMETHOD( Invoke )( DISPID dispidMember, REFIID iid, LCID lcid,
		WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
		EXCEPINFO* pExceptionInfo, UINT* piArgError );
	END_INTERFACE_PART( EventHandler )
		
	DECLARE_INTERFACE_MAP()
		
	IUnknown* GetInterfaceHook( const void* pv );

	BOOL EstablishSink(LPUNKNOWN iUnkControl);
	void CloseSink();

	bool IsSinkToEvent() const { return 0 != m_dwEventCookie; };
};

class CAXControl : public CMyContextBase, public CV7Control
{
	DECLARE_DYNCREATE(CAXControl);
public:
	BOOL SetPropEventProcessing(CValue const& Value) {if(m_pCtrlCnt) m_pCtrlCnt->m_iEventProcessing=Value.GetNumeric();return TRUE;};
	BOOL GetPropEventProcessing(CValue& Value) const {if(m_pCtrlCnt) Value=m_pCtrlCnt->m_iEventProcessing; return TRUE;};
	BOOL SetPropObject(CValue const& Value);
	BOOL GetPropObject(CValue& Value) const;
	BOOL methDestroy(CValue** params);
	BOOL methSetFocus(CValue** params);
	BOOL methAssignAttrib(CValue** params);
	BOOL methCreateControl(CValue& RetVal, CValue **params);

	CAXControl();
	virtual ~CAXControl();
    //static BOOL CreateControl(CWnd* pFrame, CGetDoc7* pDoc, CGetField* pGetField, CValue& vControl);
    //CV7Control
    virtual BOOL CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC);
    virtual BOOL _Create(CValue** ppValue);

    DECLARE_MY_CONTEXT()
private:
	friend class CAXCtrlContainer;
	CAXCtrlContainer *m_pCtrlCnt;
};

#endif // !defined(AFX_AXCONTROL_H__D8FD4B9A_02B0_4E5B_8669_103A3172B113__INCLUDED_)
