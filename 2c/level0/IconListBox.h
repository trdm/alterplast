#if !defined(AFX_ICONLISTBOX_H__DA46242F_5FCA_11D5_BBEE_0050BAD90EEA__INCLUDED_)
#define AFX_ICONLISTBOX_H__DA46242F_5FCA_11D5_BBEE_0050BAD90EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconListBox window

class CIconListBox : public CListBox
{
// Construction
public:
	CIconListBox();

// Attributes
public:

// Attributes
protected:
	CImageList* m_pImageList;

// Operations
public:
	int AddString(LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem, int iImg);
	int InsertString(int iIndex, LPCTSTR lpszItem);
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg);
	void SetItemImage(int iIndex, int iImg);
	inline void SetImageList(CImageList* pImgList = NULL)
	{ m_pImageList = pImgList; }
	inline CImageList* GetImageList()
	{ return m_pImageList; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONLISTBOX_H__DA46242F_5FCA_11D5_BBEE_0050BAD90EEA__INCLUDED_)
