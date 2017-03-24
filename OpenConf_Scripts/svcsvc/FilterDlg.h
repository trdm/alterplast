#if !defined(AFX_FILTERDLG_H__C5024AAB_FDF2_445C_85D1_6A46A927C3CF__INCLUDED_)
#define AFX_FILTERDLG_H__C5024AAB_FDF2_445C_85D1_6A46A927C3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// FilterDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog
#define FVF_IN_SUBSTR		0x1			// Фильтровать по вхождению
#define FVF_IN_MOUSEPOS		0x2			// Вывести в позиции мыши
#define FVF_IN_CARETPOS		0x4			// Вывести в позиции курсора
#define FVF_IN_CALLERPOS	0x8			// Вывести в указанных координатах
#define FVF_ALLOW_USERVAL	0x10		// Допускать ввод своих значений
#define FVF_USE_CAPTION		0x20		// Использовать заголовок
#define FVF_USE_INIT_STR	0x40		// Использовать параметр заголовока как инициализирующее значение фильтра 
#define FVF_AUTOFILL_FROM_COMBOBOX 0x80 // Попытаться заполнить список из активного комбобокса или листбокса
#define FVF_SORT			0x100		// Сортировка списка
#define FVF_AUTOWIDTH		0x200		// Автоширина окна

class CMyEdit:public CEdit
{
public:
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CMyTree:public CTreeCtrl
{
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

// единица сортировки списка
struct SValue
{
public:
	SValue(CString& val, CString& data, int imgidx) : m_val(val), m_data(data), m_imgidx(imgidx) {};
	friend bool operator < (const SValue &v1, const SValue &v2)
	{
		return v1.m_val.CompareNoCase(v2.m_val) < 0 ? true : false;
	}
	friend bool operator > (const SValue &v1, const SValue &v2)
	{
		return v1.m_val.CompareNoCase(v2.m_val) > 0 ? true : false;
	}

	CString m_val;
	CString m_data;
	int m_imgidx;
};

class CFilterDlg : public CDialog
{
// Construction
public:
	void OnOK();
	void OnCancel();
	void MoveSel(int);
	void ReSize();
	void FillTree();
	static CString FilterValue(CString vals,CString cap,short flags,long x,long y, long w, long h);
	CFilterDlg(CWnd* pParent = NULL);   // standard constructor
	CRect m_rcEdit;
	CRect m_rcTree;
	CString m_result;
	int nSelectedItem;
	CString m_Caption;

	CStringArray m_values; // массив значений
	CStringArray m_AdditionalData; // массив дополнительных значений
	CArray<int, int> m_imgidx; // массив индексов картинок

	CPoint	m_point;
	long	m_width; // ширина окна.
	long	m_height; //[+]metaeditor, высота окна
	bool	m_up;
	short	m_flags;
	long	m_diff;
	bool	m_lock;

	HWND m_ParenthWnd;
	BOOL m_ParentSubClassed;
		
	static CImageList m_ImageList;
	static int m_nDefaultImageIdx;

	static CString GetWndText(HWND WH, bool IsListBox);
	static HWND GetAnyFocus();

	void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	long GetFullAutoWidth();
	long GetTreeAutoWidth();
	static void SetDefaultImageList();
	static CString SetImageList(CString csList);

// Dialog Data
	//{{AFX_DATA(CFilterDlg)
	enum { IDD = IDD_DIALOG2 };
	CMyTree m_tree;
	CMyEdit	m_edit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEdit1();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_FILTERDLG_H__C5024AAB_FDF2_445C_85D1_6A46A927C3CF__INCLUDED_)
