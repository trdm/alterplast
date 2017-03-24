// AXControl.cpp: implementation of the CAXControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxpriv2.h"
#include "AXControl.h"

enum enAXCtrlEvents
{
	axeSelection,
	axeLastEvent
};
stCtrlEvents AXCtrlEvents[] = 
{
	// подчеркивание нужно для единообразия с событиями АктивИкс
	{"_Выбор", "_Selection", 0},
};

BEGIN_INTERFACE_MAP( CAXCtrlContainer, CWnd )
	INTERFACE_PART( CAXCtrlContainer, IID_IDispatch, EventHandler )
END_INTERFACE_MAP()

// Создание контрола в контейнере
BOOL CAXCtrlContainer::CreateCtrlWnd(const CString& strClassName, CValue& RetVal)
{
	CRect rect;
	GetClientRect(rect);
	
	CAXControlWnd* pNewWnd = new CAXControlWnd(this);
	if(pNewWnd->CreateControl((LPCTSTR )strClassName,NULL, WS_VISIBLE|WS_CHILD|(m_bIsTab ? WS_TABSTOP : 0),rect, this, 0, NULL,FALSE,NULL))
	{
		delete m_pControl;
		m_pControl = pNewWnd;
		// Чтобы отрабатывал перехват WM_SETFOCUS, надо сабклассировать контрол
		*m_pControl->GetSuperWndProcAddr() = (WNDPROC)::SetWindowLong(m_pControl->m_hWnd, GWL_WNDPROC, (LONG)AfxGetAfxWndProc());

		LPUNKNOWN iUnkControl=m_pControl->GetControlUnknown();
		DispatchToValue(iUnkControl, &RetVal);
		SetupEventHandlers();

		EstablishSink(iUnkControl);

		return TRUE;
	}else{
		delete pNewWnd;
		RuntimeError("Создание элемента управления %s не удалось",strClassName.operator LPCTSTR());
		return FALSE;
	}	
}

CAXCtrlContainer::~CAXCtrlContainer()
{
	CloseSink();

	delete m_pControl;
	//delete m_EventHandlers;
	//delete m_ComponentEventHandlers;
	if(m_pParentObj)
		m_pParentObj->m_pCtrlCnt=NULL;
}

void CAXCtrlContainer::Reset()
{
	delete m_pControl;
	m_pComponentBLModule=NULL;
	m_pDoc=NULL;
	m_pControl=NULL;
	m_pParentObj=NULL;
}

/*
STDMETHOD(OnControlInfoChanged)();
STDMETHOD(LockInPlaceActive)(BOOL fLock);
STDMETHOD(GetExtendedControl)(LPDISPATCH* ppDisp);
STDMETHOD(TransformCoords)(POINTL* lpptlHimetric,
	POINTF* lpptfContainer, DWORD flags);
STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, DWORD grfModifiers);
STDMETHOD(OnFocus)(BOOL fGotFocus);
STDMETHOD(ShowPropertyFrame)();
STDMETHODIMP CAXCtrlContainer::XOleCtrlSite::OnFocus(BOOL bFocus)
{
	METHOD_PROLOGUE_EX_(CAXCtrlContainer, OleCtrlSite)
	return pThis->m_pCtrlSite->m_xOleControlSite.OnFocus(bFocus);
	//return S_OK;
}
*/


// Создание окна-контейнера на месте атрибута формы
// При этом если атрибут - текст, то считается, что этот контрол не должен получать фокус
// (не обходится табом) - различные картинки, тулбары и тп.
// А если создаем на месте кнопки, то обходится табом.
CAXCtrlContainer* CAXCtrlContainer::CreateContainer(CAXControl* pCtrlObj, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC)
{
	int attrType;
    if(!pGetField || !((attrType=pGetField->GetCtrlInfo()->m_CtrlType)==1 || attrType==3))
    {
		RuntimeError("Неверный тип атрибута (используйте текст или кнопку)");
        RuntimeError("Недопустимый атрибут формы.");
        return NULL;
    }

	CAXCtrlContainer* pCont = new CAXCtrlContainer(pCtrlObj);
	pCont->m_pDoc=pDoc;
	CControlID* pControlID = pGetField->GetCtrlInfo();
	pCont->m_ControlID = pControlID->GetCodeName();
	
	if(pUDC)
		pCont->m_pComponentBLModule = static_cast<IUserClassContext*>(pUDC)->GetModule();

	CWnd* pOldWnd = pDoc->GetFieldWnd(pGetField);
	CWnd* pParent = pOldWnd->GetParent();
	
	CRect rect;
	pOldWnd->GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	int id = pOldWnd->GetDlgCtrlID();
	delete pOldWnd;

	pCont->m_bIsTab = (attrType != 1);

	DWORD dwStyle = WS_CHILD | (pCont->m_bIsTab ? WS_TABSTOP : 0);
	if (pGetField->m_Visible)
		dwStyle |= WS_VISIBLE;
	if (pGetField->GetReadOnly())
		dwStyle |= WS_DISABLED;
	
	pCont->Create(AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW), 0),
		NULL, dwStyle, rect, pParent, id);
	return pCont;
}

extern CBkEndUI * pBkEndUI;

BEGIN_BL_METH_MAP(CAXControl)
    BL_METH("AssignAttrib", "УстановитьАтрибут", 2, &methAssignAttrib, NULL, NULL)
    BL_METH("CreateControl", "СоздатьЭУ", 1, NULL, &methCreateControl, NULL)
    BL_METH("SetFocus", "УстановитьФокус", 0, &methSetFocus, NULL, NULL)
    BL_METH("Destroy", "Уничтожить", 0, &methDestroy, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CAXControl)
    BL_PROP("Object", "Объект", &GetPropObject, NULL)
    BL_PROP("EventProcessing", "ОбработкаСобытий", &GetPropEventProcessing, &SetPropEventProcessing)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CAXControl, "ActiveX", "АктивИкс", 1, NULL, NULL, 3);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAXControl::CAXControl()
{
	m_pCtrlCnt=NULL;
}

CAXControl::~CAXControl()
{
	if(m_pCtrlCnt)
		m_pCtrlCnt->Reset();
}

BOOL CAXControl::methAssignAttrib(CValue **params)
{
	CBLContext* pBLCont = params[0]->GetContext();
	if (pBLCont)
	{
		if(!strcmp(pBLCont->GetRuntimeClass()->m_lpszClassName,"CFormAllCtrlsContext"))
		{
			CFormAllCtrlsContext *p_Form = (CFormAllCtrlsContext*)pBLCont;
		
			CGetDoc7* pDoc = (CGetDoc7*)p_Form->m_GetDoc;

				CGetFieldsArray* pGetFieldsArray = &(pDoc->m_GetFieldsArray);
				
				if(pDoc->m_CtrlArray.GetSize() <= 0)
					RuntimeError("Метод <АктивИкс::УстановитьАтрибут> можно использовать только после физического открытия окна формы");

				int cnt = pGetFieldsArray->GetSize();
				int i;
				for(i = 0; i<cnt; i++)
				{
					if(!strcmp(params[1]->m_String, pGetFieldsArray->GetAt(i)->GetCtrlInfo()->GetCodeName()))
						break;
				}
				if(i==cnt)
				{
					RuntimeError("Неверный идентификатор атрибута");
					return FALSE;
				}
				m_pCtrlCnt = CAXCtrlContainer::CreateContainer(this, pDoc, pGetFieldsArray->GetAt(i), NULL);

				m_pCtrlCnt->m_EventManager.SetEvents(AXCtrlEvents, axeLastEvent, m_pCtrlCnt->m_ControlID, pDoc->m_pBLModule, this, NULL);
		}else{
			RuntimeError("Неверно указана форма");
		}
	}else{
		RuntimeError("Не указана форма");
	}
	return TRUE;
}

BOOL CAXControl::methCreateControl(CValue &RetVal, CValue **params)
{
	if(!m_pCtrlCnt)
	{
		RuntimeError("Не задан родительский элемент управления");
		return FALSE;
	}
	CString className=params[0]->GetString();
	className.TrimRight();
	return m_pCtrlCnt->CreateCtrlWnd(className, RetVal);
}

BOOL CAXControl::methDestroy(CValue **params)
{
	if(m_pCtrlCnt)
	{
		m_pCtrlCnt->CloseSink();
		m_pCtrlCnt->DestroyWindow();
	}
	return TRUE;
}

BOOL CAXControl::methSetFocus(CValue **params)
{
    if(m_pCtrlCnt) SetFocus(m_pCtrlCnt->m_pControl->m_hWnd);
	return TRUE;
}

BOOL CAXControl::SetPropObject(CValue const& Value)
{
	if(IS_BLTYPE(Value,CStdOleBLContext)){
/*		CStdOleBLContext *cont=((CStdOleBLContext*)(Value.GetContext()));
		IOleObject *iOleObj;
		cont->m_IUnknown->QueryInterface(IID_IOleObject,(void**)&iOleObj);
		COleControlSite *pSite = afxOccManager->CreateSite(pControlWnd->m_pCtrlCont);
		pSite->m_pWndCtrl = m_pCtrlWnd;
		//pSite->m_hWnd= m_pCtrlWnd->m_hWnd;
		m_pCtrlWnd->m_pCtrlSite = pSite;
		_AFX_THREAD_STATE* pState = AfxGetThreadState();
		if (!pState->m_bNeedTerm && !AfxOleInit())
			return FALSE;
		pSite->m_pObject = iOleObj;
		pSite->QuickActivate();
		iOleObj->SetClientSite(&(pSite->m_xOleClientSite));
		HWND hWnd = NULL;
		cont->m_IUnknown->QueryInterface(IID_IOleInPlaceObject,
				(LPVOID*)&(pSite->m_pInPlaceObject));
		if (SUCCEEDED(pSite->m_pInPlaceObject->GetWindow(&hWnd)))
		{
			if (pSite->m_hWnd != hWnd)
			{
				pSite->m_hWnd = hWnd;
				if (pSite->m_pWndCtrl != NULL)
					pSite->m_pWndCtrl->Attach(pSite->m_hWnd);
			}
		}
		pControlWnd->m_pCtrlCont->m_siteMap.SetAt(pSite->m_hWnd, pSite);
		/**/
	}
	return TRUE;
}

BOOL CAXControl::GetPropObject(CValue &Value) const
{
	if(!m_pCtrlCnt) return TRUE;
	if(m_pCtrlCnt->m_pControl)
	{
		LPUNKNOWN iUnkControl = m_pCtrlCnt->m_pControl->GetControlUnknown();
		DispatchToValue(iUnkControl, &Value);
	}
	return TRUE;
}
/*
BOOL CAXControl::CreateControl(CWnd* pFrame, CGetDoc7* pDoc, CGetField* pGetField, CValue& vControl)
{
    if (pGetField == NULL || pGetField->GetCtrlInfo()->m_CtrlType != HOSTING_CONTROL_TYPE)
    {
        RuntimeError("Недопустимый атрибут формы.");
        return FALSE;
    }
    CAXControl* pAXControl = NULL;
    CBLContext* pControl = NULL;
	BOOL bIsComponentClass = FALSE;

    if (IS_BLTYPE(vControl,CComponentClass))
    {
        CBLContext* pAXControlCtx;
        if (static_cast<CComponentClass*>(vControl.GetContext())->GetBaseClass(RUNTIME_CLASS(CAXControl), &pAXControlCtx))
        {
            pAXControl = static_cast<CAXControl*>(pAXControlCtx);
			bIsComponentClass = TRUE;
        }
    }
	else if(IS_BLTYPE(vControl,CAXControl))
	{
		pAXControl=static_cast<CAXControl*>(vControl.GetContext());
	}
    else if (vControl.GetTypeCode() == UNDEFINE_TYPE_1C)
    {
        pAXControl = new CAXControl;
        vControl.AssignContext(pAXControl);
        pAXControl->DecrRef();
    }
    if (!pAXControl)
    {
        RuntimeError("Недопустимый объект ЭУ.", 0);
        return FALSE;
    }
    CControlID* pControlID = pGetField->GetCtrlInfo();
    pAXControl->m_pParentWnd = (CAXControlWnd*)pDoc->GetFieldWnd(pGetField);
	pAXControl->m_pCtrlWnd = new CAXControlWnd;
	if(bIsComponentClass)
		pAXControl->m_pCtrlWnd->m_pComponentBLModule=((CComponentClass*)(vControl.GetContext()))->GetModule();
	pAXControl->m_pCtrlWnd->m_pDoc=pDoc;
	pAXControl->m_pCtrlWnd->m_ControlID = pControlID->GetCodeName();
    return TRUE;
}
*/
BOOL CAXControl::CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC)
{
	m_pCtrlCnt = CAXCtrlContainer::CreateContainer(this, pDoc, pGetField, pUDC);
	m_pCtrlCnt->m_EventManager.SetEvents(AXCtrlEvents, axeLastEvent, m_pCtrlCnt->m_ControlID, pDoc->m_pBLModule, this, pUDC);
    return TRUE;
}

BOOL CAXControl::_Create(CValue** ppValue)
{
	methAssignAttrib(ppValue);
	CValue RetVal;
	methCreateControl(RetVal,&ppValue[2]);
	return TRUE;
}

CAXControlWnd::CAXControlWnd(CAXCtrlContainer* pCont):m_pCont(pCont)
{
}

CBLContext* CAXCtrlContainer::GetControlContext() const
{
	CBLContext* pCont = NULL; 
	if (m_pComponentBLModule)
		pCont = m_pComponentBLModule->pIntInfo->pGeneralContext;

	if (!pCont)
		pCont = m_pParentObj; 

	return pCont;
}

void CAXCtrlContainer::SetupEventHandlers(){
	if(!m_pControl)
		return;

	IProvideClassInfoPtr pPCI;	
	HRESULT hResult = m_pControl->GetControlUnknown()->QueryInterface( IID_IProvideClassInfo,
		(void**)&pPCI );

	if( FAILED( hResult ) )
		return;

	ITypeInfoPtr pClassInfo;
	hResult = pPCI->GetClassInfo(&pClassInfo);
	if( FAILED( hResult ) )
		return;

	TYPEATTR *pTypeAttr;
	hResult = pClassInfo->GetTypeAttr(&pTypeAttr);
	if( FAILED( hResult ) )
		return;

	for(UINT iType = 0; (iType < pTypeAttr->cImplTypes) ; iType++ )
	{
		int iFlags;
		if(S_OK == pClassInfo->GetImplTypeFlags( iType, &iFlags ) )
		{
			if((iFlags & IMPLTYPE_MASK) == IMPLTYPE_DEFAULTSOURCE )
			{
				ITypeInfoPtr pTypeInfo;
				HREFTYPE hRefType;
				if(S_OK == pClassInfo->GetRefTypeOfImplType( iType, &hRefType ))
				{
					if(S_OK == pClassInfo->GetRefTypeInfo(hRefType, &pTypeInfo) && pTypeInfo!=NULL)
					{
						int iMethod;
						UINT nNames;
						TYPEATTR* pta;
						if(S_OK == pTypeInfo->GetTypeAttr( &pta ))
						{
							m_iid = pta->guid;

							for( iMethod = 0; iMethod < pta->cFuncs; iMethod++ )
							{
								FUNCDESC* pfd;
								if(S_OK == pTypeInfo->GetFuncDesc( iMethod, &pfd ))
								{
									if( !(pfd->wFuncFlags&FUNCFLAG_FRESTRICTED) &&
										(pfd->funckind == FUNC_DISPATCH) )
									{
										CComBSTR bstrNames=NULL;
										if(S_OK == pTypeInfo->GetNames( pfd->memid, &bstrNames, 1,&nNames ))
										{
											CString funcName(bstrNames);
											//SysFreeString(bstrNames);
											CBLModule7 *pMod = m_pDoc->m_pBLModule;
											if(pMod)
											{
												CString nameProc(m_ControlID+"_"+funcName);
												int r = pMod->FindProc(nameProc, 0);
												if(r<0)
													r = pMod->FindFunc(nameProc);
												if(r>=0)
												{
													EventInfo ei={r,pfd->cParams};
													m_EventHandlers->SetAt(pfd->memid,ei);
												}
											}
											if(m_pComponentBLModule)
											{
												int r = m_pComponentBLModule->FindProc(funcName,0);
												if(r<0)
													r = m_pComponentBLModule->FindFunc(funcName);
												if(r>=0)
												{
													EventInfo ei={r,pfd->cParams};
													m_ComponentEventHandlers->SetAt(pfd->memid,ei);
												}
											}
										}
									}
									pTypeInfo->ReleaseFuncDesc(pfd);
								}
							}
							pTypeInfo->ReleaseTypeAttr(pta);
						}
					}
				}
				break;
			}
		}
	}
	pClassInfo->ReleaseTypeAttr(pTypeAttr);
}

void CAXCtrlContainer::CallEventHandler(EventInfo ei, CValue** lArray, CBLModule7* pModule)
{
	if(ei.procid>0)
	{
		CBLProcInfo cblProcInfo;
		if(pModule->GetProcInfo(ei.procid,cblProcInfo))
		{
			int newArgNum = cblProcInfo.GetNParams();
			if(newArgNum > ei.argNum) newArgNum = ei.argNum;
			int t = cblProcInfo.GetType();

			CGetCtrl* pCurCtrl=m_pDoc->GetActiveCtl();
			if(pCurCtrl) pCurCtrl->DeActivate();
			m_pDoc->OnStartBatch();
			if(t == 1)
				pModule->CallAsProc(ei.procid,newArgNum,lArray);
			else
			{
				CValue arValue;
				pModule->CallAsFunc(ei.procid,arValue,newArgNum,lArray);
			}
			m_pDoc->OnEndBatch(1);
		}
	}

}
/**/
BOOL CAXControlWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return m_pCont->OnCtrlCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//BOOL CAXControlWnd::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO попытка правильного решения бага 3473 с формулой на кнопке
//	// после такого кода АктивИкс перестает реагировать на нажатие Ентер-а
//	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
//		return TRUE;
//
//	return CWnd::PreTranslateMessage(pMsg);
//};

BEGIN_MESSAGE_MAP(CAXCtrlContainer, CWnd)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAXControlWnd, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

void CAXControlWnd::OnSetFocus(CWnd* pOldWnd)
{
	if(pOldWnd!=m_pCont)
	{
		int id = m_pCont->GetDlgCtrlID();
		m_pCont->m_pDoc->SetActiveCtl(id);
	}
	else
		Default();
}

int CAXControlWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	LRESULT lRet = Default();
	if(lRet==MA_ACTIVATE)
	{
		int id = m_pCont->GetDlgCtrlID();
		m_pCont->m_pDoc->SetActiveCtl(id);
	}
	return lRet;
}

BOOL CAXCtrlContainer::OnCtrlCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(m_iEventProcessing){
		if(nCode == CN_EVENT){
			AFX_EVENT* pEvent=(AFX_EVENT*)pExtra;
			
			if(pEvent->m_eventKind == AFX_EVENT::event){

				if (!IsSinkToEvent())
					OnEvent( pEvent->m_dispid, pEvent->m_pDispParams );

			}else if(pEvent->m_eventKind == AFX_EVENT::propChanged){
				//Msg("%s_propChanged(dispid=%i)", m_ControlID, pEvent->m_dispid);

			}else if(pEvent->m_eventKind == AFX_EVENT::propDSCNotify){
				//Msg("%s_DSCNotify(DSCSTATE=%i,DSCREASON=%i)", m_ControlID, pEvent->m_nDSCState, pEvent->m_nDSCReason);
			}
		}
		return FALSE;
	}else{
		return FALSE;
	}
}

BOOL CAXCtrlContainer::PreTranslateMessage(MSG* pMsg)
{
	if ( (pMsg->message != WM_KEYDOWN || pMsg->wParam != VK_RETURN) && pMsg->message != WM_LBUTTONDBLCLK )
		return CWnd::PreTranslateMessage(pMsg);
		
	if (m_EventManager.IsAppointed(axeSelection))
	{
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		m_EventManager.Invoke0(axeSelection);
	}
	
	if (WM_LBUTTONDBLCLK == pMsg->message ) // двойное нажатие работает как обычно
		return FALSE;

	LPUNKNOWN iUnkControl = m_pControl->GetControlUnknown();
	if (iUnkControl)
	{
		CComPtr<IOleInPlaceActiveObject> iOleInPlaceActiveObject;
		const HRESULT res = iUnkControl->QueryInterface(IID_IOleInPlaceActiveObject, reinterpret_cast<void**>(&iOleInPlaceActiveObject));
		if (res == S_OK)
		{
			if (iOleInPlaceActiveObject->TranslateAccelerator(pMsg) == S_FALSE)
				::SendMessage(pMsg->hwnd,pMsg->message,pMsg->wParam,pMsg->lParam);
		}
	}
	return TRUE;
};

///////////////////////////////////////////////////////////////////////////////
// CAXCtrlContainer::XEventHandler
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CAXCtrlContainer::XEventHandler::AddRef()
{
	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )

	return( E_NOTIMPL );
	return( pThis->ExternalAddRef() );
}

STDMETHODIMP_( ULONG ) CAXCtrlContainer::XEventHandler::Release()
{
	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )

	return( E_NOTIMPL );
	return( pThis->ExternalRelease() );
}

STDMETHODIMP CAXCtrlContainer::XEventHandler::QueryInterface( REFIID iid,
															 LPVOID* ppvObj)
{
	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )

	if( IsEqualIID( iid, IID_IDispatch ) )
	{
		// QI'ing for IID_IDispatch will get you the default dispinterface on an
		// object.  In the case of the control site, that would be the ambient
		// properties dispinterface.  Since the control's event sink connection
		// point is exposed not as IID_IDispatch, but rather as the IID of the
		// control's event dispinterface,
	}

	HRESULT hr = pThis->ExternalQueryInterface( &iid, ppvObj );
	return hr;
}

STDMETHODIMP CAXCtrlContainer::XEventHandler::GetIDsOfNames( REFIID iid,
															LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs )
{
	(void)iid;
	(void)ppszNames;
	(void)nNames;
	(void)lcid;
	(void)pDispIDs;

	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )
		ASSERT_VALID( pThis );

	return( E_NOTIMPL );
}

STDMETHODIMP CAXCtrlContainer::XEventHandler::GetTypeInfo( UINT iTypeInfo,
														  LCID lcid, ITypeInfo** ppTypeInfo )
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )
		ASSERT_VALID( pThis );

	return( E_NOTIMPL );
}

STDMETHODIMP CAXCtrlContainer::XEventHandler::GetTypeInfoCount(
	UINT* pnInfoCount )
{
	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )
		ASSERT_VALID( pThis );

	*pnInfoCount = 0;

	return( S_OK );
}

STDMETHODIMP CAXCtrlContainer::XEventHandler::Invoke( DISPID dispidMember,
													 REFIID iid, LCID lcid, USHORT wFlags, DISPPARAMS* pdpParams,
													 VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgErr )
{
	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pvarResult;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE( CAXCtrlContainer, EventHandler )
		ASSERT_VALID( pThis );

	pThis->OnEvent( dispidMember, pdpParams );  // Call the event handler, who
	// also cleans up.

	return( S_OK );
}

HRESULT TraceSafeArray(SAFEARRAY * psa)
{
	// Считываем данные из возвращенного массива.
	USES_CONVERSION;
	LONG iLBound, iUBound;
	HRESULT hr = SafeArrayGetLBound(psa, 1, &iLBound);
	if(FAILED(hr))
		return hr;
	hr = SafeArrayGetUBound(psa, 1, &iUBound);
	if(FAILED(hr))
		return hr;

	//StudentStruct * pStudentStruct = NULL;
	//hr = SafeArrayAccessData(psa, (void**)&pStudentStruct);
	//if(FAILED(hr))
	//	return hr;
	void * p = NULL;
	hr = SafeArrayAccessData(psa, (void**)&p);
	if(FAILED(hr))
		return hr;

	//for(int i = 0; i <= iUBound - iLBound; i++)
	//	ATLTRACE("%s\n", OLE2T(pStudentStruct[i].name));

	hr = SafeArrayUnaccessData(psa);
	if(FAILED(hr))
		return hr;
	return S_OK;
}

void CAXCtrlContainer::OnEvent( DISPID dispid, DISPPARAMS* pdpParams )
{
	if(m_iEventProcessing){
		EventInfo ei;
		BOOL foundModuleHandler=m_EventHandlers->Lookup(dispid,ei);
		BOOL foundComponentHandler=FALSE;
		if(m_pComponentBLModule){
			foundComponentHandler=m_ComponentEventHandlers->Lookup(dispid,ei);
		}
		if(foundModuleHandler || foundComponentHandler){
			CValue p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
			CValue *lArray[10]={&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10};
			UINT ui;
			((CBLMyEx*)NULL)->DispParamToValueEx(lArray, pdpParams,&ui);
			if(foundComponentHandler){
				CallEventHandler(ei,lArray,(CBLModule7*)m_pComponentBLModule);
			}
			if(foundModuleHandler){
				CallEventHandler(ei,lArray,&(m_pDoc->m_cBLModule7));
			}
			for(int i=0;i<pdpParams->cArgs;i++){
				VARIANTARG v=pdpParams->rgvarg[i];
				if(v.vt & VT_BYREF){
					CValue RetValue=*(lArray[pdpParams->cArgs-i-1]);
					VARTYPE ordType = v.vt ^ VT_BYREF;
					switch (RetValue.type)
					{
					case 1:
						switch (ordType)
						{
						case VT_UI1: *(v.pbVal)		=RetValue.m_Number; break;
						case VT_UI2: *(v.puiVal)	=RetValue.m_Number; break;
						case VT_UI4: *(v.pulVal)	=RetValue.m_Number; break;
						case VT_I1: *(v.pcVal)		=RetValue.m_Number; break;
						case VT_I2: *(v.piVal)		=RetValue.m_Number; break;
						case VT_I4: *(v.plVal)		=RetValue.m_Number; break;
						case VT_INT: *(v.pintVal)	=RetValue.m_Number; break;
						case VT_UINT: *(v.puintVal)	=RetValue.m_Number; break;
						case VT_BOOL: *(v.piVal)	=RetValue.m_Number; break;
						case VT_R4: *(v.pfltVal)	=RetValue.m_Number; break;
						default:
							((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal); break;
						}
						break;
					case 2:
						switch (ordType)
						{
						case VT_BSTR:
							*(v.pbstrVal) = RetValue.m_String.AllocSysString(); break;
						}
						break;
					case 3:
						((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal);
						break;
					default:
						if((ordType !=VT_DISPATCH) && (ordType !=VT_UNKNOWN))
							((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal);
					}
				}
			}/**/
		}
	}

}

IUnknown* CAXCtrlContainer::GetInterfaceHook( const void* pv )
{
	const IID* piid;

	// #ifdef _DEBUG
	// 	// The standard MFC implementation of QueryInterface doesn't switch module
	// 	// states, since most objects just use static data to implement QI.  Since
	// 	// we're calling GetDocument(), though, we wind up calling ASSERT_VALID on
	// 	// a bunch of MFC objects, and that requires the correct module state.
	// 	// Because ASSERT_VALID doesn't do anything in release builds, we only need
	// 	// to switch module states in debug builds.
	AFX_MANAGE_STATE( m_pModuleState );
	// #endif  // _DEBUG

	piid = (const IID*)pv;

	if( *piid == m_iid )
	{
		return( &m_xEventHandler );
	}

	return( NULL );
}

BOOL CAXCtrlContainer::EstablishSink( LPUNKNOWN iUnkControl )
{
	assert(NULL != iUnkControl);

	CloseSink();

	{
		// TODO artbear - без следующей вроде бы ненужной проверки не работают некоторые события - например, соответствующий юнит-тест
		// но, возможно, тест составлен неверно. Пока размышляю. С другой стороны, от этого кода ошибки не будет точно :)
		IQuickActivatePtr pQuickActivate;

		HRESULT hResult = iUnkControl->QueryInterface( IID_IQuickActivate,
			(void**)&pQuickActivate );
		if( FAILED( hResult ) )
			return( FALSE );
	}

	IConnectionPointContainerPtr pConnPtContainer;

	HRESULT hr = iUnkControl->QueryInterface(IID_IConnectionPointContainer, (void**)&pConnPtContainer);
	if ( FAILED(hr) )
		return FALSE;

	hr = pConnPtContainer->FindConnectionPoint(m_iid, &m_pIConnectionPoint);
	if ( FAILED(hr) )
		return FALSE;

	assert(m_pIConnectionPoint != NULL);
	assert(!IsSinkToEvent());

	if( !IsSinkToEvent() ){
		hr = m_pIConnectionPoint->Advise(&m_xEventHandler, &m_dwEventCookie);
		if( CONNECT_E_CANNOTCONNECT == hr)
			return FALSE;
		if( CONNECT_E_ADVISELIMIT == hr)
			return FALSE;

		assert(SUCCEEDED(hr));
		assert(IsSinkToEvent());
	}

	//iUnkControl->Release();
	return TRUE;
}

void CAXCtrlContainer::CloseSink()
{
	if (!m_pControl || !m_pControl->GetControlUnknown()) {
		m_dwEventCookie = 0;
		m_pIConnectionPoint = NULL;
		return;
	}

	if (IsSinkToEvent())
	{
		assert(m_pIConnectionPoint != NULL);

		HRESULT hr = m_pIConnectionPoint->Unadvise(m_dwEventCookie);
		assert(SUCCEEDED(hr));
		m_dwEventCookie = 0;
		m_pIConnectionPoint = NULL;
	}
}