// configsvcimpl.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "resource.h"
#include "ocdoc.h"
#include "scriptmaneger.h"
#include "formcnt.h"
#include "pluginmanager.h"
#include "metadata.h"
#include "initializer.h"
#include "silenser.h"
#include "IdleHandler.h"
#include "tlibversion.h"

CBkEndUI *pBkEndUI;
#include "zlibeng.h"
#include "ocmenu.h"
#include "sectoolbarmgr.h"
#include "PanelManager.h"


CString PrintRTC(CRuntimeClass* pClass)
{
	CString txt;
	while(pClass)
	{
		txt=txt+pClass->m_lpszClassName+"->";
		pClass=(*pClass->m_pfnGetBaseClass)();
	}
	return txt;
}
CString PrintRTC(CObject* pObj)
{
	if(!pObj)
		return "";
	return PrintRTC(pObj->GetRuntimeClass());
}

PFONCREATE CFakeWnd::m_pRealFunc=NULL;
PFONCREATE CFakeWnd::m_pCreateMain;
HWND CFakeWnd::m_hTabWnd=NULL;


// Вызывается при создании окна Конфигурация
int CFakeWnd::MyCreate(LPCREATESTRUCT lp)
{
	int ret=(this->*m_pRealFunc)(lp);
	m_hTabWnd=::GetWindow(m_hWnd,GW_CHILD);
	CPtrList* pList=CConfigSvcImpl::m_pTheService->GetHandlersList(CFG_ON_CREATE_CFG_WND);
	POSITION pos=pList->GetHeadPosition();
	while(pos)
		(*(HANDLER_ON_CREATE_WND)pList->GetNext(pos))(this);
	CIConfigEvents::m_ConfigEvents.FireOpenConfigWnd();
	return ret;
}

// Перехват создания основного окна
HHOOK hHook;
LRESULT CALLBACK CBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	LRESULT ret=CallNextHookEx(hHook,nCode,wParam,lParam);
	if(nCode==HCBT_CREATEWND)
	{
		COCMenu::Init((HWND)wParam);
		UnhookWindowsHookEx(hHook);
	}
	return ret;
}
// Установка перехватов
void CFakeWnd::Init()
{
	// Назначение - установка перехвата OnCreate для окна конфигурации
	POSITION pos=pMainApp->GetFirstDocTemplatePosition();
	CMyDocTemp *pTemp;
	DWORD old;
	while(pos)
	{
		pTemp=(CMyDocTemp*)pMainApp->GetNextDocTemplate(pos);
		if(pTemp->GetFrame()=="CTabMDFrame")
			break;
	}
	CFakeWnd *pWnd=(CFakeWnd*)pTemp->GetFrameClass()->CreateObject();
	AFX_MSGMAP* pMap=pWnd->GetMap();
	AFX_MSGMAP_ENTRY* pEntry=(AFX_MSGMAP_ENTRY*)pMap->lpEntries;
	while(pEntry->nMessage && pEntry->nMessage!=WM_CREATE)
		pEntry++;
	m_pRealFunc=(PFONCREATE)pEntry->pfn;
	VirtualProtect(pEntry,24,PAGE_EXECUTE_READWRITE,&old);
	pEntry->pfn=(AFX_PMSG)&CFakeWnd::MyCreate;
	delete pWnd;
	// Установка перехвата создания основного окна
	hHook=::SetWindowsHookEx(WH_CBT,CBTProc,NULL,::GetCurrentThreadId());
}

CConfigSvcImpl* CConfigSvcImpl::m_pTheService;

CConfigSvcImpl::CConfigSvcImpl()
{
	m_pTheService=this;
}

CConfigSvcImpl::~CConfigSvcImpl()
{
	CObject* pObj;
	WORD key;
	for(POSITION pos=m_HandlersMaps.GetStartPosition();pos;)
	{
		m_HandlersMaps.GetNextAssoc(pos,key,pObj);
		delete pObj;
	}
}

CString CConfigSvcImpl::IBDir()
{
	return m_IBPath;
}

CString CConfigSvcImpl::BinDir()
{
	return m_BinPath;
}

CDocument* CConfigSvcImpl::OpenFile(CString path)
{
	CString ext=path.Right(4);
	ext.MakeLower();
	if(ext==".txt")
		return pMainApp->OpenDocumentFile(path);
	else if(ext==".ert" || ext==".mxl")
	{
		CIdleHandler* pApp=(CIdleHandler*)pMainApp;
		if(!pApp->GetRFL())
			return NULL;
		pApp->AddToRecentFileList(path);
		CMDIFrameWnd* pMain=(CMDIFrameWnd*)AfxGetMainWnd();
		pMain->SendMessage(WM_COMMAND,ID_FILE_MRU_FILE1,0);
		CDocument* pDoc=NULL;
		CMDIChildWnd* pChild=pMain->MDIGetActive();
		if(pChild)
		{
			pDoc=pChild->GetActiveDocument();
			if(!lstrcmp(pDoc->GetRuntimeClass()->m_lpszClassName,
				ext==".ert"?"CWorkBookDoc":"CSheetDoc"))
			{
				if(pDoc->GetPathName()!=path)
					pDoc=NULL;
			}
			else
				pDoc=NULL;
		}
		return pDoc;
	}
	else
	{
		int npos = path.Find("@MD");
		if(npos != -1)
		{
			CString nameRep = path.Left(npos);
			CMetaDataCont *pMD = GetMetaData();        
			CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
			if(!pObj)
				return NULL;
			long id=pObj->GetID();
			CTypedCont* pCont=NULL;
			CString text;
			CConfigCont::IDToPath(id,"CalcVar",text,&pCont,FALSE);
			if(pCont)
			{
				pCont->ShowDocument(id);
				CDocument* pOpenDoc=pCont->GetWorkBook();
				if(pOpenDoc)
				{
					POSITION pos=pOpenDoc->GetFirstViewPosition();
					if(pos)
					{
						CView* pView=pOpenDoc->GetNextView(pos);
						if(pView)
						{
							CFrameWnd* pFrame=pView->GetParentFrame();
							pFrame->ActivateFrame();
							pView->SetFocus();
						}
					}
				}
				return pOpenDoc;
			}
			else
				return NULL;
		}
		else
		{
			CBodyTextDoc* pNew=(CBodyTextDoc*)m_pTextDoc->GetDocClass()->CreateObject();
			pNew->SetPathName(path,0);
			path=pNew->GetPathName();
			delete pNew;
			pNew=NULL;
			CDocument *pDoc;
			for(POSITION pos=m_pTextDoc->GetFirstDocPosition();pos;)
			{
				pDoc=m_pTextDoc->GetNextDoc(pos);
				if(pDoc->GetPathName()==path)
				{
					pNew=(CBodyTextDoc*)pDoc;
					break;
				}
			}
			if(!pNew)
			{
				pNew=(CBodyTextDoc*)m_pTextDoc->GetDocClass()->CreateObject();
				CFrameWnd* pFrame;
				if(!(pFrame=m_pTextDoc->CreateNewFrame(pNew,NULL)))
				{
					delete pNew;
					return NULL;
				}
				if(!pNew->OnOpenDocument(path))
				{
					pFrame->DestroyWindow();
					return NULL;
				}
				pNew->SetPathName(path,TRUE);
				m_pTextDoc->InitialUpdateFrame(pFrame,pNew);
				m_pTextDoc->AddDocument(pNew);
			}
			else
			{
				POSITION pos=pNew->GetFirstViewPosition();
				CView* pView=pNew->GetNextView(pos);
				CWnd* pFrame=pView->GetParentFrame();
				CMDIFrameWnd* pMain=(CMDIFrameWnd*)AfxGetMainWnd();
				pMain->MDIActivate(pFrame);
			}
			return pNew;
		}
	}
	return NULL;
}
/*
struct tabchanged
{
	static DWORD dwVTable[128];
	
	template<typename T> static DWORD ClassFuncPtr2Dword(T ptr){return *reinterpret_cast<DWORD*>(&ptr);}

	void  ActivateTab(CWnd* pWnd, int i)
	{
		SECTabWndBase* ptr = reinterpret_cast<SECTabWndBase*>(this);
		CString text;
		text.Format("Активируется стр %i", i);
		AfxMessageBox(text);
		ptr->SECTabWndBase::ActivateTab(pWnd, i);
	}

	static void Init(CWnd* pPage)
	{
		while(!pPage->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(SECTabWndBase)))
			pPage = pPage->GetParent();

		DWORD* pVtable = *reinterpret_cast<DWORD**>(pPage);
		if(pVtable != dwVTable)
		{
			memcpy(dwVTable, pVtable, sizeof(dwVTable));
			dwVTable[65]=ClassFuncPtr2Dword(&ActivateTab);
			*reinterpret_cast<DWORD**>(pPage) = dwVTable;
		}
	}
};
DWORD tabchanged::dwVTable[128];
*/

bool CConfigSvcImpl::AddConfigTab(CString name,HICON hIcon,
			CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView)
{
	if(!CFakeWnd::m_hTabWnd)
		return false;
	if(!IsWindow(CFakeWnd::m_hTabWnd))
	{
		CFakeWnd::m_hTabWnd=NULL;
		return false;
	}
	if(!pRTCDoc)
		return false;
	if(!pRTCView)
		return false;
	SECTabWndBase* pWnd=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
	if(!pWnd)
		return false;

	//tabchanged::Init(pWnd);

	CCreateContext ctx;
	ctx.m_pCurrentDoc=(CDocument*)pRTCDoc->CreateObject();
	if(!ctx.m_pCurrentDoc->OnNewDocument())
	{
		delete ctx.m_pCurrentDoc;
		return false;
	}
	ctx.m_pCurrentFrame=NULL;
	ctx.m_pLastView=NULL;
	ctx.m_pNewDocTemplate=NULL;
	ctx.m_pNewViewClass=pRTCView;

	if(!pWnd->SECTabWndBase::AddTab(pRTCView,name,&ctx,0))
		return false;
	if(hIcon)
		pWnd->SECTabWndBase::SetTabIcon(pWnd->GetTabCount()-1,hIcon);
	return true;
}
void CConfigSvcImpl::AddConfigEventHandler(WORD wEventType,void* pHandler)
{
	CPtrList *pList=GetHandlersList(wEventType);
	POSITION pos=pList->Find(pHandler);
	if(!pos)
		pList->AddTail(pHandler);
}

CPtrList* CConfigSvcImpl::GetHandlersList(WORD wType)
{
	CPtrList *pList;
	if(!m_HandlersMaps.Lookup(wType,(CObject*&)pList))
	{
		pList=new CPtrList;
		m_HandlersMaps[wType]=pList;
	}
	return pList;
}

void CConfigSvcImpl::InitTextDoc()
{
	CMyDocTemp* pTemp;
	for(POSITION pos=pMainApp->GetFirstDocTemplatePosition();pos;)
	{
		pTemp=(CMyDocTemp*)pMainApp->GetNextDocTemplate(pos);
		if(!strcmp("CBodyTextDoc",pTemp->GetDocClass()->m_lpszClassName))
		{
			CConfigSvcImpl::m_pTheService->m_pTextDoc=pTemp;
			break;
		}
	}
}

bool CConfigSvcImpl::LoadModule(CString strFileName,CString& src)
{
	if(strFileName.IsEmpty())
		return true;
	if (strFileName.Find(".ert") != -1) // Значит файл отчета необходимо достать оттуда модуль
	{
		IStoragePtr pStgRoot;
		IStorage* pStorage = NULL;
		HRESULT hr;
		USES_CONVERSION;
		hr = ::StgOpenStorage(
			T2COLE(strFileName),
			NULL,
			STGM_READ | STGM_SHARE_DENY_WRITE,
			NULL,
			0,
			&pStorage);

		if (FAILED(hr))
		{
			CString StrError;
			StrError = "Не удалось прочитать файл: ";
			StrError += strFileName;
			pBkEndUI->DoMessageLine(StrError, mmBlackErr);	
			return false;
		}              
		pStgRoot.Attach(pStorage, false);

		IStreamPtr pStream;
		IStream *pIStr = NULL;
		hr = pStgRoot->OpenStream(T2COLE("MD Programm text"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
		if(FAILED(hr)) 
		{
			pBkEndUI->DoMessageLine("failed IStorage::OpenStream", mmBlackErr);	            
			return false;        
		}
		pStream.Attach(pIStr, false);    

		IStreamPtr pStreamOut;
		pIStr = NULL;
		if(S_OK == CreateStreamOnHGlobal(NULL, TRUE, &pIStr))
		{
			pStreamOut.Attach(pIStr, false);
			try{
			IzlibEnginePtr iLibEngine;
			iLibEngine.CreateInstance(L"V75.zlibEngine");
			hr = iLibEngine->pkDecompress(pStream, pStreamOut);
			if(FAILED(hr))
			{
				pBkEndUI->DoMessageLine("error iLibEngine->pkDecompress", mmBlackErr);          
				return false;
			}          
			else
			{
				HGLOBAL hGlobal=NULL;
				LPVOID lpMemStream = NULL;
				STATSTG St;
				pStreamOut->Stat(&St, STATFLAG_NONAME);
				ULONG cbStreamSizeOut = St.cbSize.LowPart;
				GetHGlobalFromStream(pStreamOut, &hGlobal);
				if(hGlobal)
				{
					lpMemStream = GlobalLock(hGlobal);                  
					LPTSTR lpStr = src.GetBufferSetLength(cbStreamSizeOut);
					memcpy(&lpStr[0], lpMemStream, cbStreamSizeOut);            
					GlobalUnlock(hGlobal);
					GlobalFree(hGlobal);
				}
				else
				{
					pBkEndUI->DoMessageLine("hGlobal == NULL", mmBlackErr);	                            
					return false;
				}
			}
			}catch (_com_error&)
			{
				pBkEndUI->DoMessageLine("catch (_com_error& er) ", mmBlackErr);
			}                 
		}
		else
		{
			pBkEndUI->DoMessageLine("error in CreateStreamOnHGlobal", mmBlackErr);    
			return false; 
		}
		return true;    
	}
	else
	{
		CFile ComponentFile;
		if (ComponentFile.Open(strFileName, CFile::modeRead))
		{
			DWORD dwLength = ComponentFile.GetLength();
			LPTSTR buff = src.GetBuffer(dwLength);
			ComponentFile.Read(buff, dwLength);
			src.ReleaseBuffer(dwLength);
			ComponentFile.Close();
			return true;
		}
		else
		{
			CString StrError;
			StrError = "Не удалось прочитать файл: ";
			StrError += strFileName;
			pBkEndUI->DoMessageLine(StrError, mmBlackErr);	
			return false;
		}
	}
}

void ExtractMacrosFromDispatch(IDispatch* pDisp,CStringArray& names,CDWordArray& dispIDs)
{
	ITypeInfoPtr ti=NULL;
	HRESULT hr;
	hr=pDisp->GetTypeInfo(0,0,&ti);
	if(ti!=NULL)
	{
		TYPEATTR* ta=NULL;
		ti->GetTypeAttr(&ta);
		if(ta)
		{
			for(int i=0;i<ta->cFuncs;i++)
			{
				FUNCDESC* pfd;
				ti->GetFuncDesc(i,&pfd);
				if(!pfd->cParams && pfd->invkind==DISPATCH_METHOD)
				{
					BSTR bstr;
					UINT n;
					ti->GetNames(pfd->memid,&bstr,1,&n);
					CString macros=bstr;
					if(macros.Find("_")==-1 && macros!="AddRef" && macros!="Release")
					{
						names.Add(macros);
						dispIDs.Add(pfd->memid);
					}
					SysFreeString(bstr);
				}
				ti->ReleaseFuncDesc(pfd);
			}
		}
		ti->ReleaseTypeAttr(ta);
	}
}

bool CConfigSvcImpl::InitConfigSvc()
{
	CProfile7* pProf=pMainApp->GetProps();
	// IBDir
	m_IBPath=pProf->GetStringProp(appIBDir);
	// BinDir
	m_BinPath=pProf->GetStringProp(appBinDir);

	AddConfigEventHandler(CFG_ON_CREATE_CFG_WND,COCDoc::OnCreateConfigWindow);
	// Загрузка библиотеки типов
	HRESULT hr=LoadRegTypeLib(LIBID_OpenConf,TLIB_MAJOR,TLIB_MINOR,0,&m_pTypeLib);
	if(S_OK!=hr)
	{
		// Не удалось загрузить из реестра. Попробуем загрузить из файла
		_bstr_t bstrPath(m_BinPath+"config.tlb");
		hr=LoadTypeLib(bstrPath,&m_pTypeLib);
		if(S_OK!=hr)
		{
			AfxMessageBox("Не удалось загрузить библиотеку типов config.tlb\n"
				"Проверьте ее наличие в калоге bin.");
			return false;
		}
		// Теперь проверим версию загруженного файла
		TLIBATTR* pAttr=NULL;
		if(S_OK!=m_pTypeLib->GetLibAttr(&pAttr) || !pAttr)
			return false;
		bool succes=(pAttr->guid==LIBID_OpenConf && pAttr->wMajorVerNum==TLIB_MAJOR && pAttr->wMinorVerNum==TLIB_MINOR);
		m_pTypeLib->ReleaseTLibAttr(pAttr);
		if(!succes)
		{
			AfxMessageBox("Неправильная версия config.tlb");
			m_pTypeLib=NULL;
			return false;
		}
		// Все загрузилось. Зарегим в реестре. Возвращаемое значение не проверяем, ибонах.
		RegisterTypeLib(m_pTypeLib,bstrPath,NULL);
	}
	m_Configurator=new CIConfig;
	return true;
}

void CConfigSvcImpl::LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count)
{
	SECToolBarMgr::LoadToolBar(hInst,nID,lpszName,count);
}

void CConfigSvcImpl::AddCmdTarget(CCmdTarget* pTarget)
{
	COCMenu::AddCmdTarget(pTarget);
}

void CConfigSvcImpl::RemoveCmdTarget(CCmdTarget* pTarget)
{
	COCMenu::RemoveCmdTarget(pTarget);
}

bool CConfigSvcImpl::RegisterPanelClass(LPCTSTR lpszName,CRuntimeClass *pWndClass,HICON hIcon)
{
	void* fnd;
	if(CPanelManager::m_rtcPanels.Lookup(lpszName,fnd))
		return false;
	STabInfo* pTab=new STabInfo;
	pTab->pWndClass=pWndClass;
	pTab->hIcon=hIcon;
	CPanelManager::m_rtcPanels[lpszName]=pTab;
	return true;
}
