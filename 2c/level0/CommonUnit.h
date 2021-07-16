//��������� ����� - VTOOLS.RU (info@vtools.ru)
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "DragDialog\DragFormView.h"
#include "EditForm\DynControl.h"
#include "1CCtrl.h"
#include "Compiler\ValueForm.h"
#include "GridCtrl_src\GridCtrl.h"
#include "Compiler\CompileModule.h"

#define WM_CANDESTROY	WM_USER+11
#define WM_DEFBUTTON	WM_USER+12
#define WM_REFRESHEXPR	WM_USER+13

typedef CArray<CString,CString>	DefNameList;
typedef CArray<CValue*,CValue*>	DefValueList;


#define NM_DBLCLKGRID NM_DBLCLK
class CFormUnit : public CPARENT
{
DECLARE_DYNCREATE(CFormUnit)
#ifndef CFormUnit
protected:
	CFormUnit();           // protected constructor used by dynamic creation
public:
	enum { IDD = IDD_FORMUNIT };
	virtual void OnInitialUpdate();
#else
public:
	CFormUnit(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_DIALOGUNIT };

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
#endif


// Attributes
public:
	HICON m_hIcon;

	CString csModule;//����� ������
	CString csDescription;//����� ��������
	CString csForm;//����� �����
	CString csPath;
	CString csFormID;
	CValueForm *pFormVal;
	BOOL bModified;
	


	CProcUnit *pRunRunModule;//������-�������� �����


	CValue vContext;
	CValue vParentContext;
	BOOL bHasParent;
	//������ ���������� ��������
	CArray <CDynControl*,CDynControl*> aControlList;
	CMap <UINT,UINT,CDynControl*,CDynControl*> aControlListById;
	CArray <CValue,CValue&> aElements;
	CArray <CGridCtrl*,CGridCtrl*> aGridCtrl;
	int form_dx;
	int form_dy;

	CCompileModule *pModule;
	bool bDeleteModule;
	CProcUnit *pRunModule;
	CProcUnit cRunModule;

	int nModeCloseWindow;//0 - ���������� �����, 1- ��������� �����������, 2 - ��������� �������� ����


	CString csFormTitle;
	int bCanOpen;
	int bCanClose;
	int	bCanResize;


	CWnd *pSetFocus;//��������� ������ (��� ������������ ����������� ����� �� ������� �� ��������)
	CWnd *pDefButton;//������ �� ��������� (Ctrl+Enter)
	int nChoiceMode;//����� �������
	CValue vCurrentValue;//������� ��������
	CValue vParentChoiceContext;//��������, ��������� ������ �����
	CArray <CValue,CValue &> aChildContext; //���������, ������� ������� ������ ����� (��������� ������������ ��� �������� ������ �����)

	int nLineSelect;//���������������������


	int nRunCount;
	CToolTipCtrl	m_ToolTip;  //������������������ �����
// Operations
public:

	int RefreshExpr(WPARAM wParam=0, LPARAM lParam=0);

	CValue MakeChoice(CValue Val);//��������������

	CString DefButton(CString Str);
	CString ActiveControl(CString Str);

	static CMapStringToPtr aOpenForm;

	BOOL m_bUseTabCtrl;
	int nNumberTabCtrl;//����� ������������ ���-��������
	void UseTabCtrl(BOOL bMode);

	void SetFormSize();

	static BOOL FindOpen(CString Str);
	BOOL Load();
	void Init();
	void ClearData();
	BOOL PrepareObjects();
	BOOL CreateForm();
	int Run();
	int IsCanDestroy();


	afx_msg int CloseForm(WPARAM wParam=0, LPARAM lParam=0);
	afx_msg int CanDestroy(WPARAM wParam=0, LPARAM lParam=0);
	afx_msg int CallDefButton(WPARAM a=0, LPARAM b=0);


	void GetNameValueList();
	void MoveChildWindows(CDynControl *pControl,int delta_x,int delta_y);
	void CallFunction(CString csFunction,CValue vParam1,CValue vParam2,CValue vParam3);
	void CallFunction2(CString csFunction,CValue &vParam1,CValue &vParam2,CValue &vParam3);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DefNameList aNameList;
	DefValueList aValueList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormUnit)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

#ifndef CFormUnit
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
#endif

// Implementation
public:
	virtual ~CFormUnit();
	void OnButton(UINT nID,int nMessage);
	void OnButton2(UINT nID,int nMessage);

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


	//����������� �������� 
	//��� �������� �� �������� ���������� � ��������� OnButton

	//������� ��� ������������ ������� ���������
	//#define Message0(x) afx_msg void OnMessage0##x(){OnButton(nID,x);};
	#define Message1(x) afx_msg void OnMessage##x(UINT nID){OnButton(nID,x);};
	#define Message2(x) afx_msg void OnMessage##x(UINT nID,NMHDR* pNMHDR, LRESULT* pResult){OnButton(nID,x);}


	//������� ��� ������������ ����������� ���������
	#define MessageGrid(x)\
	afx_msg void OnMessage##x( WPARAM wParam, LPARAM lParam, LRESULT* pResult)\
	{\
		int _nReturnStatus=nReturnStatus;\
		nReturnStatus=1;\
		OnButton(wParam,x);\
		if(!nReturnStatus)\
			*pResult=-1;\
		nReturnStatus=_nReturnStatus;\
	};
	#define Message3(x) afx_msg void OnMessage##x( WPARAM wParam, LPARAM lParam, LRESULT* pResult){OnButton(wParam,x);*pResult = 0;};


	//����� �������� 


	Message2(TVN_SELCHANGED)//Tree

	Message1(BN_CLICKED)
	Message1(EN_CHANGE)
	Message1(LBN_DBLCLK)
	Message1(CBN_SELENDOK)
	Message1(LBN_SELCHANGE)
//	Message3(WM_DRAWITEM)//Static

	Message3(NM_CLICK)
	Message3(NM_DBLCLK)
	Message3(TVN_SELCHANGED)
	Message3(TVN_ITEMEXPANDED)
	Message3(NM_RCLICK)

	Message3(TCN_SELCHANGE)

	MessageGrid(GVN_BEGINLABELEDIT)//Grid
	MessageGrid(GVN_ENDLABELEDIT)//Grid
	MessageGrid(GVN_SELCHANGING)//Grid
	MessageGrid(GVN_SELCHANGED)//Grid
	MessageGrid(GVN_BEGINDRAG)//Grid

	MessageGrid(NM_DBLCLKGRID)//Grid
//	MessageGrid(WM_KEYDOWN)//Grid


	Message3(NM_CUSTOMDRAW)//Slider,Static
	Message3(NM_RELEASEDCAPTURE)//Slider
//	Message3(TB_THUMBPOSITION)//Slider
//	Message3(TB_ENDTRACK)//Slider


	Message3(WM_ONSELECT)//����� ������ ����������� ��������

	// Generated message map functions
	//{{AFX_MSG(CFormUnit)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
