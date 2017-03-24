// MethComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "telepat.h"
#include "MethComboBox.h"
#include "EnumProcDlg.h"
#include "txttrap.h"
#include "tentrydrawing.h"
#include "parser.h"
#include "codejump.h"
#include "option.h"
/////////////////////////////////////////////////////////////////////////////
// CMethComboBox
CHackSwap		hackSetBtn;
CMapPtrToWord CMethComboBox::m_boxes;
int CMethComboBox::m_lastMethListLine=-1;
CTxtTrap* CMethComboBox::m_pEditor=NULL;


CMethComboBox::CMethComboBox():m_hList(NULL),m_line(-1)
{
}

CMethComboBox::~CMethComboBox()
{
	m_boxes.RemoveKey(this);
}

void CMyToolBarMgr::Init()
{
	hackSetBtn.Hack("Frame.dll","?SetButtonMap@SECToolBarManager@@QAEXPBUSECBtnMapEntry@@@Z",(AFX_PMSG)&SetBtnMapEx);
}

BEGIN_MESSAGE_MAP(CMethComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMethComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMethComboBox message handlers

void CMethComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight=CTEntryDrawing::m_EntryHeight;
}

void CMethComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		int sel=GetCurSel();
		if(sel!=m_lastSel)
		{
			m_hint->Hide();
			m_lastSel=sel;
		}
	}
	CTEntryDrawing::Draw(lpDrawItemStruct);
}

void CMethComboBox::OnDropdown() 
{
	CTxtTrap* pEditor=CTxtTrap::GetCurrentView();
	CString txt;
	m_pToolBar->GetWindowText(txt);
	if(!pEditor)
		return;
	CRect rc;
	GetWindowRect(rc);
	GetParent()->ScreenToClient(rc);
	rc.bottom=270;
	MoveWindow(rc);
	m_enabled=true;
	int sort,filter;
	ResetContent();
	m_array.RemoveAll();
	CEnumProcDlg::FillMethodList(m_array,pEditor,this,sort,filter,false);
	m_hint=new CHintWnd(this);
	m_hint->m_list=false;
	m_hint->Hide();
	m_lastSel=-1;
}

void CMethComboBox::OnSelendok() 
{
	int pos=GetCurSel();
	if(pos>=0)
	{
		CTxtTrap* pEditor=CTxtTrap::GetCurrentView();
		if(pEditor)
		{
			SProcInfo* pInfo=((CMethodEntry*)GetItemData(pos))->m_pInfo;
			int line=pInfo->start.line;
			CCodeJump::GetCodeJump()->JumpFromTo(pEditor,pEditor->GetDocument(),line);
		}
	}
}

void CMethComboBox::OnCloseup() 
{
	if(m_hint)
	{
		m_hint->Hide(false);
		delete m_hint;
		m_hint=NULL;
	}
	ResetContent();
	m_array.RemoveAll();
	SetWindowText(NULL);
	CTxtTrap* pEditor=CTxtTrap::GetCurrentView();
	if(pEditor)
	{
		pEditor->SetFocus();
		m_lastMethListLine=-1;
	}
	if(m_hList)
		m_hList=NULL;
}

void CMethComboBox::OnEditchange() 
{
	static bool lock=false;
	if(lock)
		return;
	lock=true;
	if(m_hint)
		m_hint->Hide();
	SendMessage(WM_SETREDRAW,0);

	CString txt;
	GetWindowText(txt);
	CString old=txt;
	txt.MakeLower();

	int cnt=GetCount();
	while(cnt-->0)
		DeleteString(0);

	CStringArray arr;
	SplitStrToArray(txt,arr,' ');
	int strCount=arr.GetSize();
	int sort =COption::m_pOption->GetSortMethods();

	int s=m_array.GetSize();
	for(int i=0,count=0;i<s;i++)
	{
		CTeleEntry* pEntry=(CTeleEntry*)m_array[i];
		CString name=pEntry->m_Name;
		name.MakeLower();
		bool add=true;
		for(int k=0;k<strCount;k++)
		{
			if(name.Find(arr[k])<0)
			{
				add=false;
				break;
			}
		}
		if(add)
		{
			int idx;
			if(sort)
				idx=InsertString(count++,pEntry->m_Name);
			else
				idx=AddString(pEntry->m_Name);
			SetItemDataPtr(idx,pEntry);
		}
	}
	cnt=GetCount();
	if(m_hList)
	{
		CRect rc;
		::GetWindowRect(m_hList,&rc);
		int height=cnt*CTEntryDrawing::m_EntryHeight+4;
		if(height>250)
			height=250/CTEntryDrawing::m_EntryHeight * CTEntryDrawing::m_EntryHeight+4;
		rc.bottom=rc.top+height;
		::MoveWindow(m_hList,rc.left,rc.top,rc.Width(),rc.Height(),TRUE);
	}
	m_lastSel=-1;
	SendMessage(WM_SETREDRAW,1);
	if(old.GetLength()==1)
	{
		SetWindowText(old);
		SetEditSel(1,1);
	}
	lock=false;
}

HBRUSH CMethComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_LISTBOX)
		m_hList=pWnd->m_hWnd;
	else if(nCtlColor==CTLCOLOR_EDIT)
		pDC->SetTextColor(m_enabled?0:0x808080);
	return hbr;
}

void CMethComboBox::OnSelchange() 
{
	m_lastSel=GetCurSel();
	if(m_hint)
		m_hint->Hide();
}



BOOL CMethComboBox::CreateWnd(CWnd* pParentWnd, DWORD dwStyle, CRect& rect, int nID)
{
	if(!(dwStyle & (CBS_DROPDOWNLIST | CBS_DROPDOWN)))
		dwStyle |= CBS_DROPDOWN;

	// Create the combo box.
	BOOL ret=CComboBox::Create(dwStyle, rect, pParentWnd, nID);
	AdjustSize();
	SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT));
	SetDroppedWidth(300);
	m_boxes[this]=1;
	m_pEdit=new SECComboBtnEdit;
	m_pEdit->m_pCombo=this;
	m_pEdit->SubclassWindow(::GetWindow(m_hWnd,GW_CHILD));
	return ret;
}

void CMethComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!LButtonDown(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnLButtonDown(nFlags, point);
}

void CMethComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!LButtonUp(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnLButtonUp(nFlags, point);
}

void CMethComboBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!LButtonDblClk(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnLButtonDblClk(nFlags, point);
}

void CMethComboBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!RButtonDown(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnRButtonDown(nFlags, point);
}

void CMethComboBox::OnRButtonUp(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!RButtonUp(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnRButtonUp(nFlags, point);
}

void CMethComboBox::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// Give the SECWndBtn a first crack
	if(!RButtonDblClk(nFlags, point))
		// It doesn't want it so I can handle
		CComboBox::OnRButtonDblClk(nFlags, point);
}

void CMethComboBox::OnPaint()
{
	PrePaint();
	CComboBox::OnPaint();
	PostPaint();
}

BOOL CMethComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Give the SECWndBtn a first crack
	BOOL bRes = SECWndBtn::SetCursor();
	if(!bRes)
		// It doesn't want it so I can handle
		bRes = CComboBox::OnSetCursor(pWnd, nHitTest, message);

	return bRes;
}

static const SECBtnMapEntry btnMap[]={
	{&CMethComboBox::classMyBtn,ID_METHOD_LIST,0,ID_METHOD_LIST,
		WS_VSCROLL|CBS_DROPDOWN|CBS_OWNERDRAWFIXED|CBS_HASSTRINGS|CBS_SORT|CBS_AUTOHSCROLL,
		MAKELONG(200,80),150,0},
};


SECButtonClass CMethComboBox::classMyBtn={&CMethComboBox::CreateButton};

void CMyToolBarMgr::SetBtnMapEx(SECBtnMapEntry *pMap)
{
	int s=0;
	SECBtnMapEntry* pMapPtr=pMap;
	while(pMapPtr++->m_pBtnClass)
		s++;
	SECBtnMapEntry* pNewMap=new SECBtnMapEntry[s+1+sizeof(btnMap)/sizeof(btnMap[0])];
	memcpy(pNewMap,btnMap,sizeof(btnMap));
	memcpy(pNewMap+sizeof(btnMap)/sizeof(btnMap[0]),pMap,(s+1)*sizeof(btnMap[0]));

	hackSetBtn.Swap();
	SetButtonMap(pNewMap);
	hackSetBtn.Swap();
}

void CMethComboBox::OnDestroy() 
{
	m_pEdit->Detach();
	delete m_pEdit;
	CComboBox::OnDestroy();
}


///////////////////////////////////////////////////////////////////////////////
//  SECComboBtnEdit - used for subclassing the edit control of a combobox
//
BEGIN_MESSAGE_MAP(SECComboBtnEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

SECComboBtnEdit::SECComboBtnEdit() {
	m_pCombo=NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  SECComboBtnEdit - message handlers
//
void SECComboBtnEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->LButtonDown(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnLButtonDown(nFlags, point);
}

void SECComboBtnEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->LButtonUp(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnLButtonUp(nFlags, point);
}

void SECComboBtnEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->LButtonDblClk(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnLButtonDblClk(nFlags, point);
}

void SECComboBtnEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->RButtonDown(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnRButtonDown(nFlags, point);
}

void SECComboBtnEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->RButtonUp(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnRButtonUp(nFlags, point);
}

void SECComboBtnEdit::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;
	if(m_pCombo)
		// Give the SECWndBtn first crack at this.
		bHandled = m_pCombo->RButtonDblClk(nFlags, point);

	if(!bHandled)
		// Not yet handled, so I can handle it.
		CEdit::OnRButtonDblClk(nFlags, point);
}

BOOL SECComboBtnEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Give the SECWndBtn first crack at this.
	BOOL bRes = m_pCombo->SetCursor();
	if(!bRes)
		// It hasn't handled, so I can handle
		bRes = CEdit::OnSetCursor(pWnd, nHitTest, message);

	return bRes;
}

void CMethComboBox::UpdateList(CTxtTrap *pEditor)
{
	CString word;
	CParser* pParser=pEditor->GetMyParser();
	if(pParser)
	{
		CPoint ptCurPos=pEditor->GetCaretPos();
		if(ptCurPos.y==m_lastMethListLine && m_pEditor==pEditor)
			return;
		m_lastMethListLine=ptCurPos.y;
		m_pEditor=pEditor;
		int x,line=ptCurPos.y,count=0;
		while(word.IsEmpty() && line>0)
		{
			x=pEditor->GetDocument()->GetLineLength(line);
			if(line<pEditor->GetDocument()->GetLineCount()-1)
				x-=2;
			line=pParser->GetMethodText(line,x)-1;
			pParser->m_MethParser.Parse(CMethodParser::ParseMetHeader);
			word=pParser->m_MethParser.m_curMetName;
			count++;
		}
		CString txt;
		bool enabled=count==1;
		for(POSITION pos=m_boxes.GetStartPosition();pos;)
		{
			CMethComboBox* pBox;
			WORD w;
			m_boxes.GetNextAssoc(pos,(void*&)pBox,w);
			pBox->GetWindowText(txt);
			if(txt!=word)
			{
				pBox->SetWindowText(word);
				pBox->m_line=word.IsEmpty()?-1:line+1;
			}
			if(pBox->m_enabled!=enabled)
			{
				pBox->m_enabled=enabled;
				pBox->CComboBox::Invalidate();
				pBox->UpdateWindow();
			}
		}
	}
}

void SECComboBtnEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	if(!m_pCombo->GetDroppedState() && pOldWnd->m_hWnd!=m_pCombo->m_hWnd)
		m_pCombo->ShowDropDown();
}
