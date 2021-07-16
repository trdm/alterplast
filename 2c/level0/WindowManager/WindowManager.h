#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#ifndef __AFXTEMPL_H__
#pragma message("Include <afxtempl.h> in your stdafx.h to avoid this message")
#include <afxtempl.h>
#endif

#include "..\\resource.h"       // main symbols

////////////////////////////////////////////////////////////////////////////////
// Forward declaration

class CViewManager;
// 1. TODO: Please change the CMainFrame to your main frame
#define  CMyMainFrameClass CMainFrame
class CMyMainFrameClass;

///////////////////////////////////////////////////////////
// CDocumentList declaration

class CDocumentList : public CList <CDocument*, CDocument*>
{
public:
	// Constuction
	CDocumentList();  // To automatically enumerates the documents in a given application
	// Destruction
	~CDocumentList(); // To empty and destroy the list when out of scope
	
	// Operations
	CDocument* GetNextDocument();  // Gets next document object
	
	// Prevent any copying and assignment of CDocumentList objects
private:
	CDocumentList(const CDocumentList&);
	const CDocumentList& operator=(const CDocumentList&);
	
	// Implementation
private:
	// Keep the current position in the document list
	POSITION m_CurPosInDocList;
};


/////////////////////////////////////////////////////////////////////////////
// CWindowDlg dialog

class CWindowDlg : public CDialog
{
	// Construction
public:
	CWindowDlg(CMDIFrameWnd* pMDIFrame, CWnd* pParentWnd);
	CWindowDlg(CMDIFrameWnd* pMDIFrame);   
	
	// Dialog Data
	//{{AFX_DATA(CWindowDlg)
	enum { IDD = IDD_WINDOW_MANAGE };
	CListBox	m_wndList;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	void  MDIMessage(UINT uMsg, WPARAM flag);
	void  FillWindowList(void);
	void  SelActive(void);
	void  UpdateButtons(void);
	
	CMDIFrameWnd* m_pMDIFrame;
	
	// Generated message map functions
	//{{AFX_MSG(CWindowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelChange();
	afx_msg void OnSave();
	afx_msg void OnActivate();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnUpdateUI(CCmdUI * pCmdUI);
	afx_msg void OnTileHorz();
	afx_msg void OnMinimize();
	afx_msg void OnTileVert();
	afx_msg void OnCascade();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMDIClient window

class CMDIClient : public CWnd
{
// Construction
public:
	CMDIClient();
	
	enum DispType 
	{
		DISPTILE,
		DISPCENTER,
		DISPSTRETCH 
	};
	
// Attributes
public:
	// Set background color
	void SetBkColor(COLORREF crBkValue);
	COLORREF GetBkColor() const;
	// Set Logo Text color
	void SetLogoColor(COLORREF crValue);
	COLORREF GetLogoColor() const;

	BOOL GetBkBitmapState() const;

	// Set the logo text font...
	void SetLogoFont(CFont* pLogoFont);
	void SetLogoFont(int nLogoWeight = FW_SEMIBOLD, BOOL bLogoItalic = FALSE,   
		BOOL bLogoUnderline = FALSE);

	CFont* GetLogoFont();
	
	// Load background bitmap from given file
	BOOL SetBitmap(LPCTSTR lpszFileName, UINT uFlags = LR_LOADMAP3DCOLORS);
	// Load background bitmap from resource.
	BOOL SetBitmap(UINT nBmpID, COLORMAP* pClrMap = NULL, int nCount = 0);
	// Load background bitmap from resource.
	BOOL SetDefBitmap(UINT nBmpID, COLORMAP* pClrMap = NULL, int nCount = 0);
	
	// Set desired display mode (tile, center, or stretch)
	void SetDispType(DispType enuDispType);
	DispType GetDispType() const;
	
	// Return the current image size.
	const CSize& GetImageSize() const;
	
	// Return the filename of the bitmap
	const CString& GetFileName() const;

	// For access to the private view manager member
//	void SetViewManager(CViewManager* pViewManager)  { m_pViewManager = pViewManager; }
	CViewManager* GetViewManager() const  { return m_pViewManager; }
	
protected:
	CString       m_strWindows;
	HMENU	      m_hMenuWindow;
	CMyMainFrameClass*   m_pMDIFrame;
	
	// Full Screen mode support
	CRect         m_rcMainFrame;
	BOOL          m_bMaxChild;
	CToolBar*     m_pwndFullScrnToolBar;
	
private:
	void FullScreenOff();
	void FullScreenOn();
	CSize         m_sizeClient;
	CViewManager* m_pViewManager;
	BOOL          m_bFullScreen;        // TRUE for "On" mode, FALSE for "Off"
	BOOL          m_bFirstTime;
	BOOL          m_bBkBitmap;
	
	void HideControlBars(void);
	void ShowControlBars(void);

	struct FSControlBars
	{
		BOOL         bWasVisible;
		CControlBar* pControlBar;
	};

	CArray<FSControlBars, FSControlBars&>* m_pwndControlBars;

// Operations
public:
	void ManageWindows(CMDIFrameWnd* pMDIFrame);
	void ManageWindows(CMDIFrameWnd* pMDIFrame, CWnd* pParentWnd);
	
    BOOL SubclassMDIClient(CMDIFrameWnd* pMDIFrameWnd, 
		CViewManager* pViewManager = NULL, UINT uID = ID_VIEW_VIEWTAB);
	BOOL IsFullScreen() { return m_bFullScreen; }
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIClient)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
// Implementation
public:
	void Defaults(BOOL bBkBitmap = TRUE, BOOL bDelBkBitmap = TRUE);
	void RestoreMainFrameState(UINT nCmdShow);
	void SaveMainFrameState();
	virtual ~CMDIClient();
	
protected:
    COLORREF m_crBkColor;          // Background color
    COLORREF m_crLogoColor;        // Right logo text color
    CBitmap  m_bkBitmap;           // background bitmap
    CBitmap  m_bkDefBitmap;        // Default background bitmap
    CBrush   m_bkBrush;            // Brush used for background painting
    CString  m_strFileName;        // Filename of any bitmap loaded from a file
    CSize    m_sizImage;           // Bitmap image size
    CSize    m_sizDefImage;        // Default Bitmap image size
    DispType m_enuDispType;        // Current display type
    CFont    m_fontLogo;           // Font for drawing the logo text
	
// Generated message map functions
protected:
	void PaintLogo(CDC* pDC);

	//{{AFX_MSG(CMDIClient)
	afx_msg LRESULT OnRefreshMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMenu(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowManage();
	afx_msg void OnWindowNext();
	afx_msg void OnWindowPrevious();
	afx_msg void OnWindowSaveAll();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnWindowClose();
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnTabView();
	afx_msg void OnUpdateTabView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowManage(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnMDICreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);	

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMDIClient inlines

inline COLORREF CMDIClient::GetBkColor() const
{
	return m_crBkColor;
}

inline COLORREF CMDIClient::GetLogoColor() const
{
	return m_crLogoColor;
}

inline CMDIClient::DispType CMDIClient::GetDispType() const
{
	return m_enuDispType;
}

inline const CString& CMDIClient::GetFileName() const
{
	return m_strFileName;
}

inline const CSize& CMDIClient::GetImageSize() const
{   
	return m_sizImage;
}

inline CFont* CMDIClient::GetLogoFont()
{   
	return &m_fontLogo;
}

inline BOOL CMDIClient::GetBkBitmapState() const
{   
	return m_bBkBitmap;
}

#endif
