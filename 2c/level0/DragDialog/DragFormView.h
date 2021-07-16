// DragFormView.h: interface for the CDragFormView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGFORMVIEW_H__0BCE13A1_462A_4A1E_9E0E_BAA3D0829D3C__INCLUDED_)
#define AFX_DRAGFORMVIEW_H__0BCE13A1_462A_4A1E_9E0E_BAA3D0829D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WS_EX_LAYERED    
#define WS_EX_LAYERED			0x00080000
#endif
#ifndef ULW_ALPHA    
#define ULW_ALPHA				0x00000002    
#endif
#ifndef NOFADE    
#define NOFADE					255
#endif


class CDragFormView : public CFormView  
{
	DECLARE_DYNCREATE(CDragFormView)

public:
	CDragFormView(UINT nIDD=0);
	virtual ~CDragFormView();


	void SetDragButtons(BOOL bDragLeft, BOOL bDragRight);
	void KeepDialogInScreen(BOOL bKeepDialogInScreen);
	BOOL ActivateDrag();
	BOOL DeactivateDrag();
	void UpdateScreenMetrics();
	void GetDialogBackToScreen();
	BOOL SetDragCursor(UINT nID);
	void UseDragCursor(BOOL bUseDragCursor);
	BOOL SetFadeWhileDragging(int nFade);
protected:
	void FadeDialog(BOOL bFade);
	void DrawDragCursor();
	CWnd * m_pCapture;
	int m_nFadeWhileDragging;
	int m_nOldMouseX;
	int m_nOldMouseY;
	int m_nScreenX;
	int m_nScreenY;
	BOOL m_bDragLeft;
	BOOL m_bDragRight;
	BOOL m_bKeepDialogInScreen;
	BOOL m_bUseDragCursor;
	BOOL m_bDrag;
	HCURSOR	m_hCursor;
	//{{AFX_VIRTUAL(CDragDrop)
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CDragDrop)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// SetLayeredWindowAttributes function prototype
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
(
	HWND hwnd,	
	COLORREF crKey,	
	BYTE bAlpha,	
	DWORD dwFlags
);

#endif // !defined(AFX_DRAGFORMVIEW_H__0BCE13A1_462A_4A1E_9E0E_BAA3D0829D3C__INCLUDED_)
