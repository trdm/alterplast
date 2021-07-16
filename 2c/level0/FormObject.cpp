//Авторские права - VTOOLS.RU (info@vtools.ru)
// FormObject.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormObject.h"
#include "ModuleView.h"
#include "MetadataTree.h"
#include "MetedataDocument.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "ModuleView.h"
#include "ModuleDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CFormObject

IMPLEMENT_DYNCREATE(CFormObject, CDragFormView)

CFormObject::CFormObject()
	: CDragFormView(CFormObject::IDD)
{
	//{{AFX_DATA_INIT(CFormObject)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDD_METAOBJECT);
}

CFormObject::~CFormObject()
{

}

void CFormObject::DoDataExchange(CDataExchange* pDX)
{
	CDragFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormObject)
	DDX_Control(pDX, IDCLOSEFORM, m_Close);
	DDX_Control(pDX, IDC_OK, m_OK);
	DDX_Control(pDX, IDC_USE, m_Use);
	DDX_Control(pDX, IDC_OBJECTNAME2, m_Alias);
	DDX_Control(pDX, IDC_PARENT, m_Parent);
	DDX_Control(pDX, IDC_OBJECTNAME, m_Name);
	DDX_Control(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormObject, CDragFormView)
	//{{AFX_MSG_MAP(CFormObject)
	ON_EN_CHANGE(IDC_OBJECTNAME, OnChangeObjectname)
	ON_BN_CLICKED(IDCLOSEFORM, OnCloseform)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDMODULE, OnModule)
	ON_EN_CHANGE(IDC_OBJECTNAME2, OnChangeObjectname)
	ON_EN_CHANGE(IDC_COMMENT, OnChangeObjectname)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_BN_CLICKED(IDC_USE, OnUse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormObject diagnostics

#ifdef _DEBUG
void CFormObject::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormObject::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormObject message handlers
#define m_MetaObj ((CMetaDocument*)GetDocument())->GetObj()
#define m_MetaTree ((CMetaDocument*)GetDocument())->GetTree()

void CFormObject::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit(FALSE);

	SetIcon(m_hIcon, TRUE);			// Set big icon

	if(m_MetaTree)
		OnUpdate();

	m_Use.EnableWindow(0);

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_FORMOBJECT_TMPL );

}
void CFormObject::OnDestroy() 
{
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
}


void CFormObject::OnChangeObjectname() 
{
	m_Use.EnableWindow(1);
}



void CFormObject::UpdateData()
{
	m_Name.GetWindowText(m_MetaObj->csName);
	m_Alias.GetWindowText(m_MetaObj->csAlias);
	m_Comment.GetWindowText(m_MetaObj->csComment);

	if(m_MetaTree)
		m_MetaTree->GetTreeCtrl().SetItemText(m_MetaObj->nTempID,m_MetaObj->csName);


}
void CFormObject::OnModule() 
{
	if(m_MetaTree)
		m_MetaTree->OnModuleOpen();
}


void CFormObject::OnFileSave() 
{
	OnUse();
	if(m_MetaTree)
		m_MetaTree->GetDocument()->OnFileSave();
}

//Загрузка данных формы
void CFormObject::OnUpdate()
{
	if(m_MetaObj)
	{
		GetDocument()->SetTitle(m_MetaObj->csName);
		m_Name.SetWindowText(m_MetaObj->csName);
		m_Comment.SetWindowText(m_MetaObj->csComment);
		m_Alias.SetWindowText(m_MetaObj->csAlias);
	}
}


void CFormObject::OnUse() 
{
	if(m_MetaTree)
	{
		if(m_Use.IsWindowEnabled())
		{
			UpdateData();
			m_MetaTree->GetDocument()->SetModifiedFlag(1);
		}
	}
}

void CFormObject::OnCloseform() 
{
	//OnUse();
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CFormObject::OnOk() 
{
	OnUse();
	OnCloseform();
}
