//////////////////////////////////////////////////////////////////////
//
// Разработчик: Алексей Фёдоров aka АЛьФ, mailto: alf@dorex.ru
//
//////////////////////////////////////////////////////////////////////
//
// WrapAboutDlg.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "HyperLink.h"
#include "..\resource.h"

#include "WrapAboutDlg.h"

#ifdef FORMEX_VK_DEFINED
	#include "artbear\SafeContext.h"
	#include "artbear\MyAboutWindowManager.h"

#else
	#include "MyAboutWindowManager.h"

	extern HINSTANCE hDllInstance;
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CWndTabBase;

class CMyAboutDlg
{
public:
	typedef std::vector<IOwnAboutWindowManager*> IOwnAboutWindowManagerVector;

	CMyAboutDlg(CAboutDlg* pAboutDlg, IOwnAboutWindowManagerVector& OwnPagesVector);
	~CMyAboutDlg();

	CTabCtrl* GetTabControl() const { return m_pTabCtrl; };
	CWnd* GetSystemPage() const { return m_pSystemPage; };

	CWnd* GetOwnPage(int index) const { return GetOwnAboutWindowManager(index)->GetPage(); };

	void ShowNeedPage(bool bShowWindow);

private:
	void CreateAllWindows( CAboutDlg* pAboutDlg );
	CWnd* CreateSystemPage( CAboutDlg* pDlg, const CRect& PageRect );

	CWndTabBase* m_pPageBase;
	CImageList* m_pImageList;

	CTabCtrl* m_pTabCtrl;
	CWnd* m_pSystemPage;

	IOwnAboutWindowManager* GetOwnAboutWindowManager(int index) const { return m_OwnPagesVector[index]; };
	IOwnAboutWindowManagerVector& m_OwnPagesVector;

	typedef IOwnAboutWindowManagerVector::iterator ITER;
};

class CWrapAboutDlgImpl
{
public:
	static void Init();
	static bool AddMyAboutWindowManager(IOwnAboutWindowManager* pMyAboutWindowManager);
	static IOwnAboutWindowManager* GetOwnAboutWindowManager();

private:
	static void ReplaceVirtualTableForCAboutDlgByOwnVirtualTable( CAboutDlg* pDlg );
	static const int iOnInitDialogVirtualMethodNumber;
	static const int iDestroyWindowVirtualMethodNumber;
	static const int iCAboutDlgVirtualMethodsCount;
	static const int iCAboutDlgVirtualMethodSize;

	static DWORD* m_pMyVTable;
	static DWORD* m_pRealVTable;

	virtual int  OnInitDialog(void);
	virtual void DestrAboutDlg(void);

	CWrapAboutDlgImpl() {};
	~CWrapAboutDlgImpl();

	static boost::shared_ptr<CMyAboutDlg> m_pWrapperForAboutDlg;
	static CMyAboutDlg::IOwnAboutWindowManagerVector m_OwnPagesVector;

};

class CWndTabBase : public CWnd
{
	CMyAboutDlg& m_owner;

public:
	CWndTabBase(CMyAboutDlg& owner, CAboutDlg* pAboutDlg, const CRect& ClientRect) : m_owner(owner)
	{
		Create("STATIC", "", WS_VISIBLE|WS_CHILDWINDOW, ClientRect, pAboutDlg, 0x000b);
	};

	//{{AFX_VIRTUAL(CWndTabBase)
	public:
	void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

	virtual ~CWndTabBase()
	{
	};

protected:
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CWndTabBase, CWnd)
	//{{AFX_MSG_MAP(CWndTabBase)
	ON_NOTIFY(TCN_SELCHANGE, 0x1100, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, 0x1100, OnSelchangingTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWrapAboutDlg::Init()
{
	CSafeContextPtrDef pCont;
	try
	{
#ifdef FORMEX_VK_DEFINED
		LPCSTR szClassName = "Перехватчик";
#else
		LPCSTR szClassName = "Система";
#endif
		pCont.Create(szClassName);
	}
	catch(...)
	{
	}

	if (!pCont)
		CWrapAboutDlgImpl::Init();
	else{
		CBLContext* pCont2 = pCont;
		IAddOwnAboutWindowManager *pAddOwnAboutWindowManagerContext = dynamic_cast<IAddOwnAboutWindowManager *>(pCont2);
		if (NULL == pAddOwnAboutWindowManagerContext)
			CWrapAboutDlgImpl::Init();
		else
			pAddOwnAboutWindowManagerContext->Add(CWrapAboutDlgImpl::GetOwnAboutWindowManager());
	}
}

bool CWrapAboutDlg::AddMyAboutWindowManager(IOwnAboutWindowManager* pMyAboutWindowManager)
{
	return CWrapAboutDlgImpl::AddMyAboutWindowManager(pMyAboutWindowManager);
}

/////////////////////////////////////////////////////////////////////////////
// CWrapAboutDlgImpl dialog

DWORD* CWrapAboutDlgImpl::m_pMyVTable=NULL;
DWORD* CWrapAboutDlgImpl::m_pRealVTable=NULL;

const int CWrapAboutDlgImpl::iOnInitDialogVirtualMethodNumber = 0x31;
const int CWrapAboutDlgImpl::iDestroyWindowVirtualMethodNumber = 0x18;
const int CWrapAboutDlgImpl::iCAboutDlgVirtualMethodsCount = 0x36;
const int CWrapAboutDlgImpl::iCAboutDlgVirtualMethodSize = 4 * CWrapAboutDlgImpl::iCAboutDlgVirtualMethodsCount;

boost::shared_ptr<CMyAboutDlg> CWrapAboutDlgImpl::m_pWrapperForAboutDlg;

CMyAboutDlg::IOwnAboutWindowManagerVector CWrapAboutDlgImpl::m_OwnPagesVector;

typedef int (CAboutDlg::*PF_ONINITDIALOG)(void);
typedef BOOL (CWnd::*PF_DestroyWindow)();

template <typename TFunctionType>
class CVirtualTableWrapperGuard
{
	// TODO сделать красивый/нормальный способ, без использования union
	union PtrConvAboutDlg{
		void* pV;
		TFunctionType pFunctionPointer;
	} prtconvAboutDlg;

	DWORD* m_pMyVTable;
	DWORD* m_pRealVTable;
	DWORD m_Adr;
	int m_iVirtualMethodNumber;
public:
	CVirtualTableWrapperGuard(DWORD* pMyVTable, DWORD* pRealVTable, int iVirtualMethodNumber) :
		m_pMyVTable(pMyVTable), m_pRealVTable(pRealVTable), m_iVirtualMethodNumber(iVirtualMethodNumber)
	{
		m_Adr = m_pRealVTable[iVirtualMethodNumber];
		m_pRealVTable[iVirtualMethodNumber] = m_pMyVTable[iVirtualMethodNumber];

		prtconvAboutDlg.pV = reinterpret_cast<void*>(m_pRealVTable[iVirtualMethodNumber]);
	}

	TFunctionType GetOrigMethod() const { return prtconvAboutDlg.pFunctionPointer; };

	~CVirtualTableWrapperGuard()
	{
		m_pRealVTable[m_iVirtualMethodNumber] = m_Adr;
	}
};

void CWndTabBase::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_owner.ShowNeedPage(true);
	*pResult = 0;
}

void CWndTabBase::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_owner.ShowNeedPage(false);
	*pResult = 0;
}

CMyAboutDlg::CMyAboutDlg(CAboutDlg* pAboutDlg, CMyAboutDlg::IOwnAboutWindowManagerVector& OwnPagesVector) :
	m_pPageBase(NULL), m_pImageList(NULL), m_pTabCtrl(NULL), m_pSystemPage(NULL),
	m_OwnPagesVector(OwnPagesVector)
{
	CreateAllWindows(pAboutDlg);
}

CMyAboutDlg::~CMyAboutDlg()
{
	delete m_pImageList;

	delete m_pTabCtrl;
	delete m_pSystemPage;

	delete m_pPageBase;

	for (ITER iter = m_OwnPagesVector.begin(); iter != m_OwnPagesVector.end(); iter++)
	{
		IOwnAboutWindowManager* pOwnAboutWindowManager = *iter;
		pOwnAboutWindowManager->DestroyOwnPage();
	}
}

void CMyAboutDlg::CreateAllWindows( CAboutDlg* pAboutDlg )
{
	const int bHorizontalOrientation = false;  // artbear

	CRect rect1;
	pAboutDlg->GetWindowRect(&rect1);
	//rect1.InflateRect(6,6);
	rect1.bottom += 6;

	if (bHorizontalOrientation)
		rect1.bottom += 25;

	rect1.right += 31;
	pAboutDlg->MoveWindow(&rect1);

	CRect ClientRect;
	pAboutDlg->GetClientRect(&ClientRect);

	m_pPageBase = new CWndTabBase(*this, pAboutDlg, ClientRect);

	CRect PageRect = ClientRect;
	PageRect.DeflateRect(2,2);
	//PageRect.bottom -= 25;

	if (!bHorizontalOrientation)
 	 	PageRect.left += 30;
	else
		PageRect.top += 25;

	//CRect rectCh;

	m_pSystemPage = CreateSystemPage(pAboutDlg, PageRect);

	UINT nID = 0x3000;
	for (ITER iter = m_OwnPagesVector.begin(); iter != m_OwnPagesVector.end(); iter++)
	{
		IOwnAboutWindowManager* pOwnAboutWindowManager = *iter;
		pOwnAboutWindowManager->CreateOwnPage(m_pPageBase, PageRect, nID++);
	}

	m_pTabCtrl = new CTabCtrl;

	const int iTabStyle = !bHorizontalOrientation ? TCS_VERTICAL : 0;
	if(m_pTabCtrl->Create(WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|TCS_FOCUSNEVER|iTabStyle, //TCS_BOTTOM,
		ClientRect, m_pPageBase,0x1100))
	{
		m_pImageList = new CImageList;
		m_pImageList->Create(16, 16, ILC_MASK, 2, 0);

		HBITMAP hBitmap;
		hBitmap = ::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_ABOUTTAB));
		m_pImageList->Add(CBitmap::FromHandle(hBitmap), RGB(0x00,0x80,0x80));
		::DeleteObject(hBitmap);

		for (ITER iter = m_OwnPagesVector.begin(); iter != m_OwnPagesVector.end(); iter++)
		{
			IOwnAboutWindowManager* pOwnAboutWindowManager = *iter;
			HBITMAP hAddBitmap = pOwnAboutWindowManager->GetBitmapHandle();
			if (hAddBitmap){
				m_pImageList->Add(CBitmap::FromHandle(hAddBitmap), RGB(0x00,0x80,0x80));
				::DeleteObject(hAddBitmap);
			}
		}

		m_pTabCtrl->SetFont(pAboutDlg->GetFont());
		m_pTabCtrl->SetImageList(m_pImageList);
		m_pTabCtrl->InsertItem(0,"1С:Предприятие",0);

// 		//m_pTabCtrl->InsertItem(1,"FormEx",1);
		for (int iCounter = 0; iCounter < m_OwnPagesVector.size(); iCounter++)
		{
			IOwnAboutWindowManager* pOwnAboutWindowManager = GetOwnAboutWindowManager(iCounter);

			const int iImageListCounter = iCounter + 1;
			m_pTabCtrl->InsertItem(iImageListCounter, pOwnAboutWindowManager->GetTabName(), iImageListCounter);
		}

	}
}


CWnd* CMyAboutDlg::CreateSystemPage( CAboutDlg* pDlg, const CRect& PageRect )
{
	CWnd* pNewSystemPage = new CWnd;
	if(pNewSystemPage->Create("STATIC","",WS_VISIBLE|WS_CHILDWINDOW,PageRect, m_pPageBase,0x2000))
	{
		CWnd* pWnd = pDlg->GetNextWindow(GW_CHILD);
		while(pWnd)
		{
			if(pWnd->GetSafeHwnd() == m_pPageBase->GetSafeHwnd())
			{
				pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
				if(pWnd->GetSafeHwnd() == m_pPageBase->GetSafeHwnd())
					pWnd = NULL;
			}
			if(pWnd)
			{
				CWnd* pNextWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
				pWnd->SetParent(pNewSystemPage);
				pWnd = pNextWnd;
			}
		}
		pWnd = pNewSystemPage->GetDlgItem(0x0001);
		if(pWnd)
		{
			pWnd->SetParent(pDlg);
			pWnd->ShowWindow(SW_HIDE); //OLE_COLOR
		}
	}
	return pNewSystemPage;
}

void CMyAboutDlg::ShowNeedPage(bool bShowWindow)
{
	int iTab = GetTabControl()->GetCurSel();
	CWnd* pWnd = iTab == 0? GetSystemPage(): GetOwnPage(--iTab);
	if(pWnd)
		pWnd->ShowWindow(bShowWindow ? SW_SHOW : SW_HIDE);
}

int  CWrapAboutDlgImpl::OnInitDialog(void)
{
	CAboutDlg* This = reinterpret_cast<CAboutDlg*>(this);

	int iRet = 0;
	{
		CVirtualTableWrapperGuard<PF_ONINITDIALOG> wrapper(m_pMyVTable, m_pRealVTable, iOnInitDialogVirtualMethodNumber);

		iRet = (This->*wrapper.GetOrigMethod())();//iRet = (This->*prtconvAboutDlg.pOnInitDialog)();//pubOnInitDialog();
	}

	m_pWrapperForAboutDlg = boost::shared_ptr<CMyAboutDlg>(new CMyAboutDlg(This, m_OwnPagesVector));

	return iRet;
}

void CWrapAboutDlgImpl::DestrAboutDlg(void)
{
	{
		CVirtualTableWrapperGuard<PF_DestroyWindow> wrapper(m_pMyVTable, m_pRealVTable, iDestroyWindowVirtualMethodNumber);

		CAboutDlg* This = reinterpret_cast<CAboutDlg*>(this);
		This->DestroyWindow();
	}

	m_pWrapperForAboutDlg.reset();

}

CWrapAboutDlgImpl::~CWrapAboutDlgImpl()
{
}

void CWrapAboutDlgImpl::Init()
{
	CAboutDlg AboutDlg(0);
	ReplaceVirtualTableForCAboutDlgByOwnVirtualTable(&AboutDlg);

	IOwnAboutWindowManager* pMyAboutWindowManager = GetOwnAboutWindowManager(); // CMyAboutWindowManager
	AddMyAboutWindowManager(pMyAboutWindowManager);

#ifdef ABOUT_WINDOW_EXAMPLE

	CMyAboutWindowManagerExample* pMyAboutWindowManager2 = new CMyAboutWindowManagerExample("FormEx_2");
	AddMyAboutWindowManager(pMyAboutWindowManager2);

	CMyAboutWindowManagerExample* pMyAboutWindowManager3 = new CMyAboutWindowManagerExample("FormEx_3");
	AddMyAboutWindowManager(pMyAboutWindowManager3);

	CMyAboutWindowManagerExample* pMyAboutWindowManager4 = new CMyAboutWindowManagerExample("FormEx_4");
	AddMyAboutWindowManager(pMyAboutWindowManager4);

#endif // #ifdef ABOUT_WINDOW_EXAMPLE
}

void CWrapAboutDlgImpl::ReplaceVirtualTableForCAboutDlgByOwnVirtualTable( CAboutDlg* pDlg )
{
	if(!m_pMyVTable)
	{
		m_pRealVTable = *(DWORD**)pDlg;
		CWrapAboutDlgImpl* pWrap = new CWrapAboutDlgImpl;
		DWORD* pMyVTable=*(DWORD**)pWrap;
		delete pWrap;

		m_pMyVTable=new DWORD[iCAboutDlgVirtualMethodsCount];

		memcpy(m_pMyVTable,m_pRealVTable,iCAboutDlgVirtualMethodSize);
		DWORD old;
		VirtualProtect(m_pRealVTable,iCAboutDlgVirtualMethodSize,PAGE_EXECUTE_READWRITE,&old);
		m_pRealVTable[iOnInitDialogVirtualMethodNumber]=pMyVTable[0];
		m_pRealVTable[iDestroyWindowVirtualMethodNumber]=pMyVTable[1];
	}
}

bool CWrapAboutDlgImpl::AddMyAboutWindowManager( IOwnAboutWindowManager* pMyAboutWindowManager )
{
	//assert(m_OwnPagesVector.empty());
	m_OwnPagesVector.push_back(pMyAboutWindowManager);

	return true;
}

IOwnAboutWindowManager* CWrapAboutDlgImpl::GetOwnAboutWindowManager()
{
	IOwnAboutWindowManager* pMyAboutWindowManager = new CMyAboutWindowManager();
	return pMyAboutWindowManager;
}

typedef CWnd* (CColumn::*PM_EDIT)(class CRect & ,class CBrowse *);
DWORD* CWrapColumn::m_pMyVTable=NULL;
DWORD* CWrapColumn::m_pRealVTable=NULL;

CWnd * CWrapColumn::Edit(class CRect & a_Rect,class CBrowse * a_pBrowse)
{
	CColumn* This = (CColumn*)this;
	//DWORD Adr = m_pRealVTable[0x19];
	//m_pRealVTable[0x31] = m_pMyVTable[0x19];
	PM_EDIT real = NULL;
	memcpy(&real,&m_pMyVTable[0x19],4);
	CWnd * wndRet = (This->*real)(a_Rect,a_pBrowse);
	//m_pRealVTable[0x31] = Adr;

	pBkEndUI->DoMessageLine("CWrapColumn::Edit", mmNone);

	return wndRet;
}

void CWrapColumn::Wrap(CColumn* pColumn)
{
	if(!m_pMyVTable)
	{
		m_pRealVTable = *(DWORD**)pColumn;
		CWrapColumn* pWrap = new CWrapColumn;
		DWORD* pMyVTable=*(DWORD**)pWrap;
		delete pWrap;

		m_pMyVTable=new DWORD[0x1C];

		memcpy(m_pMyVTable,m_pRealVTable,0x70);
		DWORD old;
		VirtualProtect(m_pRealVTable,0x70,PAGE_EXECUTE_READWRITE,&old);
		m_pRealVTable[0x19]=pMyVTable[0];
	}
}
