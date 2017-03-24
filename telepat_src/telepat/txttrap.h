// txttrap.h
#ifndef TXTTRAP_H
#define TXTTRAP_H
#include "hackvirtual.h"

class CMyDocTemp:public CMultiDocTemplate
{
public:
	CRuntimeClass* GetDocClass(){return m_pDocClass;}
};
class CParser;
//#define ID_QUICK_AUTOCOMPLETE	22510
// Класс для перехвата событий в окне CTextEditor
// Подменяет адреса обработчиков в messagemap.
class CTxtTrap: public CTextEditor
{
public:
	friend class CTemplateEntry;
	friend class CKWTempl;
	void  OnUpdateCaretPosEx(CCmdUI *);
	void OnUpdateCustomCmd(CCmdUI* pcm);
	void OnCustomCmd(UINT id);
	void InsertTemplate(UINT id);
	bool FillTemplateMenu(CMenu* pMenu);
	void OnQuickAutoComplete();
	bool AutoReplace(BYTE nChar);
	CPoint GetCaretPos();
	static void MergeAccelTable(HACCEL& hAccel);
	static void MergeMenu(HMENU pMenu);
	static CTxtTrap* GetCurrentView();
	static const AFX_MSGMAP* PASCAL _GetBaseWrap();
	static void Init();
	void OnTxtEnumProc();
	void CalcFontSize()
	{
		CDC* pDC=MyGetDC();
		m_szFont=pDC->GetTextExtent(" ");
		ReleaseDC(pDC);
	}
	static CPoint m_szFont;
	static CTxtTrap* m_ParsedView;
	static CTextDocument* m_ParsedDoc;
	static CParser* GetParser();
	CParser* GetMyParser()
	{
		if(((DWORD*)m_pDocument->m_dwRef)>(DWORD*)0xFFFF)
			return (CParser*)m_pDocument->m_dwRef;
		return NULL;
	}

	static CRuntimeClass* m_pTextDocClass;
	static CRuntimeClass* m_pTextEditorClass;
	static CRuntimeClass* m_pWorkBookClass;
	static CDocTemplate*  m_pAssistant;
	static CHackSwap m_hackDestroy;
	static CHackSwap m_hackTemplate;
	int  ReplaceTemplWrap(LPCTSTR str1,LPCTSTR str2);


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTxtTrap)
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(CTxtTrap)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAutocomplete();
	afx_msg void OnShowparams();
	afx_msg void OnShowtype();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCodejump();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnParamNext();
	afx_msg void OnParamPrev();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowtemplate();
	afx_msg void OnMethodListShow();
	afx_msg void OnJumpAnchor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnUpdateButtons(CCmdUI* pCmdUI);
};


#endif
