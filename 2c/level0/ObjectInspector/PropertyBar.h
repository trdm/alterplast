#if !defined(AFX_PropertyBar_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_)
#define AFX_PropertyBar_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyBar.h : header file
//
//#include "secwb.h"
//#include "Sbartool.h"
#include "ObjectInspector.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar dialog
class CPropertyBar *AfxGetProperty();

#define IDC_OI			WM_USER+3
#define IDC_DEFBUTTON	WM_USER+4

#define PropertyBarParent CExtWRB< CObjectInspector >
class CPropertyBar : public PropertyBarParent//COXSizeControlBar//SECControlBar//COXSizeControlBar
{
// Construction
public:
	CPropertyBar(CWnd* pParent = NULL);   // standard constructor

	CMapStringToPtr aParentList;
	CDocument *pDocUpdate;
	LPARAM pDocData;//элемент редактируемого дерева или элемент диалога ...
	LPARAM pDocType;//тип объекта (дополнительный параметр)

	CObjectInspector::CProperty *pCommonParent;


	void AddProperty(CString csParentName,CString csValueName,COLORREF &Value);
	void AddProperty(CString csParentName,CString csValueName,int &Value, int nMin = INT_MIN, int nMax = INT_MAX);
	void AddProperty(CString csParentName,CString csValueName,double &Value);
	void AddProperty(CString csParentName,CString csValueName,bool &Value);
	//void AddProperty(CString csParentName,CString csValueName,CString &Value);
	void AddProperty(CString csParentName,CString csValueName,CString &Value,CString csValueStr="");
	void AddProperty(CString csParentName,CString csValueName,byte &Value,CString csValueStr);
	void ReleaseDoc(CDocument *pDoc);
	void SetDoc(CDocument *pSetDocUpdate,LPARAM pSetDocData,LPARAM pSetDocType,BOOL bReadOnly=0);
	LPARAM GetDocData(){return pDocData;};
	LPARAM GetDocType(){return pDocType;};

	CObjectInspector::CProperty *GetParentProp(CString csParentName);
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void PostClear();
	afx_msg void Clear();

// Dialog Data
	//{{AFX_DATA(CPropertyBar)
	enum { IDD = ID_PROPERTYBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	//CObjectInspector	m_Property;


// Overrides
	//BOOL Create(CWnd * pParentWnd, const UINT nID=ID_LISTBOXBAR);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyBar)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
// Implementation
public:

//	virtual void OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags);
	// Generated message map functions
	//{{AFX_MSG(CPropertyBar)
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PropertyBar_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_)
