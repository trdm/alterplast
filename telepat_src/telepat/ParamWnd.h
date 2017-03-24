#if !defined(AFX_PARAMWND_H__BB7F25F5_38E7_448C_BD5C_92F68A277BD3__INCLUDED_)
#define AFX_PARAMWND_H__BB7F25F5_38E7_448C_BD5C_92F68A277BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamWnd window
class CTxtTrap;
class CProcCont;

class CParamWnd : public CWnd
{
// Construction
public:
	struct SParam
	{
		SParam():next(NULL),val(false){}
		SParam(CString n,CString h="",CString d=""):val(false),name(n),def(d),hint(h),next(NULL)
		{
			def.Replace("\r\n","\n");
		}
		~SParam(){if(next) delete next;}
		CString GetText();

		CString name;
		CString def;
		CString hint;
		bool val;
		SParam* next;
	};

	struct SMethodInfo
	{
		SMethodInfo():params(NULL),count(0){}
		SMethodInfo(CString h):hint(h),params(NULL),count(0){}
		~SMethodInfo(){if(params)delete params;}
		void GetText(CString& h1,CString& bold, CString& h2);
		void AddParam(SParam* p);

		CString name;
		CString hint;
		int		count;
		SParam* params;
	};

	CParamWnd();
	CString		m_name;			// Имя метода
	CPtrArray	m_methods;		// Массив методов
	int			m_curMethod;	// Текущий метод
	int			m_curParam;		// Номер текущего параметра
	CRect		m_rect;
	DWORD		m_timer;
	int			m_LineHeight;
	
	CFont		m_boldFont;
	CStringArray	m_h1;
	CStringArray	m_b;
	CStringArray	m_h2;

	bool	GetQuickOption();
	void	SetQuickOption(bool);
	bool	GetHintMethodOption();
	void	SetHintMethodOption(bool);
	bool	GetHintParamOption();
	void	SetHintParamOption(bool);
	bool		m_QuickOption;
	bool		m_HintMethodOption;
	bool		m_HintParamOption;

	static CParamWnd* m_pParamWnd;
	static CParamWnd* GetParamWnd(){return m_pParamWnd;}
	static void CALLBACK TimerProc(HWND,UINT,UINT,DWORD);

	void Init();
	void Clear();
	CTxtTrap*	m_pEditor;
	HWND		m_EditorHWND;
	CPoint		m_startPoint;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void QuickActivate(CTxtTrap* pEditor);
	void MakeStdMethods();
	void MakeMethods(CProcCont* pCont);
	void ShowAt(int x,int y,bool addLine);
	void ReShow();
	void ParamPrev();
	void ParamNext();
	bool Prepeare(CTxtTrap* pEditor);
	void CalculateSize();
	void Hide();
	void Show(CTxtTrap* pEditor);
	virtual ~CParamWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CParamWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMWND_H__BB7F25F5_38E7_448C_BD5C_92F68A277BD3__INCLUDED_)
