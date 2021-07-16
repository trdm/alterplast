#if !defined(AFX_IMAGEBUTTON_H__198CE0D4_D86C_43A8_9F37_B5372FBEF041__INCLUDED_)
#define AFX_IMAGEBUTTON_H__198CE0D4_D86C_43A8_9F37_B5372FBEF041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ImageButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageButton Class  
// Uður HAMZADAYI  2002
//
// Bitmap  Button Ctrl

class CImageButton : public CButton
{ 
// Construction
public:
	CImageButton();

	enum {
	   TEXT_INCLUDE      = 0x0001, 
	   IMAGE_RIGHT	     = 0x0002,
	   IMAGE_VCENTER     = 0x0004,
	   IMAGE_BOTTOM      = 0x0008,
	   IMAGE_HCENTER     = 0x0010,
	   FOCUS_TEXTONLY    = 0x0020,
    };

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextColor( COLORREF color );
	BOOL SetAlignStyle( DWORD dwStyle );
	void SetImage(int n){nImage=n;ReCalculateSettings();};
//	BOOL SetButtonImage( UINT uiImageID , COLORREF  clrMask );
//	BOOL SetButtonImage( HBITMAP hBitmap  , COLORREF clrMask );
//	BOOL DoImage(COLORREF clrMask );
//	void Clear();
	void SetTooltipText(CString & sText, BOOL bActivate);
	BOOL PreTranslateMessage(MSG* pMsg);
	
	virtual ~CImageButton();

	// Generated message map functions
protected:


	void ReCalculateSettings();
	void InitToolTip();
//	void OnMouseMove(UINT nFlags, CPoint point);

	int nImage;

	COLORREF    m_clrMask;
	CBitmap     m_bitmapImage;
	BITMAP      m_bitmap;
	HBITMAP	    m_hbmpDisabled;
	CImageList	m_ImageList;
	BOOL        m_bLoaded;
	CToolTipCtrl	m_cToolTip;

	DWORD       m_dwAlign;   //View Style 


	CRect       m_RectImage;
	CRect       m_RectText;
	CRect       m_RectTextFocus;

	COLORREF    m_clrText;       //Text Color
	
	//{{AFX_MSG(CImageButton)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEBUTTON_H__198CE0D4_D86C_43A8_9F37_B5372FBEF041__INCLUDED_)
