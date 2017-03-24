// TeleList.cpp : implementation file
//

#include "stdafx.h"
#include "TeleList.h"
#include "Telepanel.h"
#include "tooltip.h"
#include "tentrydrawing.h"
#include "txttrap.h"
#include "telegroup.h"
#include "blmap.h"
#include "parser.h"
#include "plugin.h"


CTeleList* CTeleList::m_pOneList=NULL;

CTeleList::CTeleList()
{
	if(!m_pOneList)
	{
		m_pOneList=this;
		CTelePanel* parent=new CTelePanel;
		parent->GetClientRect(m_ClientRect);
		Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_VSCROLL|LBS_SORT|
			LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOTIFY,
			m_ClientRect,parent,100);
	}
	m_pShowText=new CToolTip(this);
	m_pHint=new CHintWnd(this);
	m_pHint->Hide();

	m_pEditor=NULL;
	m_CaretPos.x=-1;
}

void CALLBACK CTeleList::CheckView(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	if(m_pOneList && CTxtTrap::GetCurrentView()!=m_pOneList->m_pEditor)
		m_pOneList->Hide();
}
 
bool CTeleList::Prepare(CTxtTrap* pEditor)
{
	m_pEditor=pEditor;
	CPoint pt;
	m_CaretPoint=pEditor->GetCaretPos();
	::GetCaretPos(&m_CaretPos);

	m_UpDown=false;

	pt=m_CaretPos;
	pEditor->ClientToScreen(&pt);
	pEditor->CalcFontSize();

	int Height=12*CTEntryDrawing::m_EntryHeight;
	CRect wrc;
	m_ParentRect=CRect(0,0,200,Height);
	GetDesktopWindow()->GetWindowRect(wrc);

	::AdjustWindowRect(&m_ParentRect,WS_POPUP|WS_CLIPSIBLINGS|WS_DLGFRAME,FALSE);
	pt.x-=m_ParentRect.left;
	pt.y+=CTxtTrap::m_szFont.y+2-m_ParentRect.top;
	m_ParentRect.OffsetRect(pt);

	if(m_ParentRect.right>wrc.right)
	{
		int width=m_ParentRect.right-m_ParentRect.left;
		m_ParentRect.left-=width;
		m_ParentRect.right-=width;
	}
	if(m_ParentRect.bottom>wrc.bottom)
	{
		int h=m_ParentRect.bottom-m_ParentRect.top+CTxtTrap::m_szFont.y+4;
		m_ParentRect.bottom-=h;
		m_ParentRect.top-=h;
		m_UpDown=true;
	}
	SetCurSel(0);
	SetFocus();
	::CreateCaret(pEditor->m_hWnd,NULL,1,CTxtTrap::m_szFont.y);
	::ShowCaret(pEditor->m_hWnd);
	m_timer=SetTimer(1,150,&CTeleList::CheckView);
	return true;
}

CTeleList::~CTeleList()
{
	m_pShowText->DestroyWindow();
	m_pHint->DestroyWindow();
	if(m_pOneList==this)
		m_pOneList=NULL;
}

BEGIN_MESSAGE_MAP(CTeleList, CListBox)
	//{{AFX_MSG_MAP(CTeleList)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCMOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeleList message handlers

void CTeleList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_pOneList==this)
	{
		if(!m_pEditor || CTxtTrap::GetCurrentView()!=m_pEditor || nChar==27)
		{
			Hide();
			return;
		}
		else if(nChar==13)
		{
			MakeSelect();
			return;
		}
		else if(nChar==VK_LEFT || nChar==VK_RIGHT)
		{
			if(GetKeyState(VK_CONTROL))
			{
				CWnd* pWnd=m_pEditor;
				_AFX_THREAD_STATE* state=AfxGetThreadState();
				WPARAM w=state->m_lastSentMsg.wParam;
				LPARAM l=state->m_lastSentMsg.lParam;
				Hide();
				pWnd->SendMessage(WM_KEYDOWN,w,l);
				return;
			}
			int z=nChar==VK_LEFT?-1:1;
			m_curPosInBuf+=z;
			m_CaretPoint.x+=z;
			if(m_CaretPoint.x<0)
				m_CaretPoint.x=0;
			else
				m_CaretPos.x+=z*CTxtTrap::m_szFont.x;
			m_pEditor->MoveCaret(m_CaretPoint,m_CaretPoint,0);
			::SetCaretPos(m_CaretPos.x,m_CaretPos.y);
			if(m_curPosInBuf<0 || m_curPosInBuf>m_buf.GetLength())
				Hide();
			else
			{
				CString txt=m_buf.Left(m_curPosInBuf);
				FilterList(txt);
				if(GetCount())
					ShowList();
				else
					Hide();
			}
			return;
		}
		else if(nChar==VK_DELETE)
		{
			if(m_curPosInBuf==m_buf.GetLength())
			{
				Hide();
				return;
			}
			m_buf.Delete(m_curPosInBuf);
		  	CTextDocument* pDoc=m_pEditor->GetDocument();
			CPoint pt=m_CaretPoint;
			pt.x++;
			pDoc->DeleteSymb(1,pt);
		}
		else if(nChar==VK_UP && !GetCurSel())
		{
			int last=GetCount()-1;
			SetCurSel(last);
			SetTopIndex(last);
			OnSelchange();
			return;
		}
		else if(nChar==VK_DOWN && GetCurSel()==GetCount()-1)
		{
			SetCurSel(0);
			SetTopIndex(0);
			OnSelchange();
			return;
		}
	}
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTeleList::OnKillFocus(CWnd* pNewWnd) 
{
	if(pNewWnd!=m_pShowText && m_pOneList==this)
		Hide();
}

void CTeleList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_pOneList!=this)
	{
		CListBox::OnChar(nChar,nRepCnt,nFlags);
		return;
	}
	if(!m_pEditor || CTxtTrap::GetCurrentView()!=m_pEditor )
	{
		Hide();
		return;
	}
  	CTextDocument* pDoc=m_pEditor->GetDocument();
	bool hide=false,send=false;
	if(nChar==8)
	{
		if(m_curPosInBuf)
		{
			pDoc->DeleteSymb(1,m_CaretPoint);
			m_CaretPoint.x--;
			m_CaretPos.x-=CTxtTrap::m_szFont.x;
			m_curPosInBuf--;
			m_buf.Delete(m_curPosInBuf);
		}
		else
			hide=send=true;
	}
	else if(nChar==127)
	{
		int s=m_buf.GetLength();
		if(s)
		{
			CPoint pt=m_CaretPoint;
			pt.x-=s;
			pDoc->SetSel(pt,m_CaretPoint);
			pDoc->Replace("");
			m_CaretPoint.x-=s;
			m_CaretPos.x-=CTxtTrap::m_szFont.x*s;
		}
		hide=true;
	}
	else
	{
		if(nChar=='\t')
		{
			MakeSelect();
			return;
		}
		if(nChar<32)
			return;
		if(CBLMap::m_LowerTable[(BYTE)nChar]==' ' || nChar=='.')
			hide=send=true;
		else
		{
			m_buf.Insert(m_curPosInBuf++,nChar);
			char buf[2]=" ";
			buf[0]=nChar;
			pDoc->InsertStr(buf,m_CaretPoint,0);
			m_CaretPoint.x++;
			m_CaretPos.x+=CTxtTrap::m_szFont.x;
		}
	}
	m_pEditor->MoveCaret(m_CaretPoint,m_CaretPoint,0);
	::SetCaretPos(m_CaretPos.x,m_CaretPos.y);
	if(hide)
	{
		CTxtTrap* pEditor=m_pEditor;
		Hide();
		if(send)
			pEditor->OnChar(nChar,nRepCnt,nFlags);
	}
	else
	{
		CString txt=m_buf.Left(m_curPosInBuf);
		FilterList(txt);
		if(GetCount())
			ShowList();
		else
			Hide();
	}
}

void CTeleList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight=CTEntryDrawing::m_EntryHeight;
}

void CTeleList::DrawItem(LPDRAWITEMSTRUCT pds) 
{
	CTEntryDrawing::Draw(pds);
}

void CTeleList::OnSelchange() 
{
	GetCursorPos(&m_LastMousePoint);
	ScreenToClient(&m_LastMousePoint);
	m_pShowText->Show(GetCurSel());
	m_pHint->Hide();
}

void CTeleList::MakeSelect(UINT nChar)
{
	int sel=GetCurSel();
	if(sel==-1)
	{
		Hide();
		return;
	}
	CTeleEntry* pEntry=(CTeleEntry*)GetItemDataPtr(sel);
	m_pEditor->GetMyParser()->m_NeedQuick=true;
	if(!pEntry->MakeInsert())
		Insert(pEntry);
	int hot=(int)m_mHotString.GetKey(pEntry->m_Name);
	if(hot)
		m_HotString.RemoveAt(hot-1);
	m_HotString.InsertAt(0,pEntry->m_Name);
	int size=m_HotString.GetSize();
	if(size>100)
		m_HotString.RemoveAt(--size);
	m_mHotString.RemoveAll();
	for(int i=0;i<size;i++)
		m_mHotString.InsertKey(m_HotString[i],(void*)(i+1));
}

void CTeleList::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);
	OnSelchange();
}

void CTeleList::OnMouseMove(UINT nFlags, CPoint point) 
{
	ShowToolTips(point);
	CListBox::OnMouseMove(nFlags, point);
}

BOOL CTeleList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	m_pShowText->Hide();
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}

void CTeleList::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_pShowText->Hide();
	//ShowToolTips(point);
	CListBox::OnNcMouseMove(nHitTest, point);
}

void CTeleList::ShowToolTips(CPoint pt)
{
	if(pt==m_LastMousePoint)
		return;
	m_LastMousePoint=pt;
	bool IsToolTipVisible=m_pShowText->IsWindowVisible()==TRUE;
	bool inrect=false;
	if(m_ClientRect.PtInRect(pt))
		inrect=true;
	if(!inrect)
	{
		if(IsToolTipVisible)	// Проверим, не вышли ли мы за пределы листа
		{
			CRect tr;
			m_pShowText->GetWindowRect(tr);
			ScreenToClient(tr);
			if(tr.PtInRect(pt))
				return;
			m_pShowText->Hide();
			return;
		}
	}
	// Далее, смотрим, не изменилось ли положение мыша на новую строку
	int idx=pt.y/CTEntryDrawing::m_EntryHeight+GetTopIndex();
	if(idx!=m_pShowText->m_Idx)
	{
		m_pShowText->Show(idx);
		return;
	}
}

void CTeleList::ShowList()
{
	int Height=GetCount();
	if(Height>12)
		Height=12;
	else if(!Height)
		Height=1;
	Height=Height-m_ParentRect.Height()/CTEntryDrawing::m_EntryHeight;
	if(Height)
	{
		Height*=CTEntryDrawing::m_EntryHeight;
		if(m_UpDown)
			m_ParentRect.top-=Height;
		else
			m_ParentRect.bottom+=Height;
	}
	CWnd* pParent=GetParent();
	pParent->SetWindowPos(&wndTopMost,m_ParentRect.left,m_ParentRect.top,
		m_ParentRect.right-m_ParentRect.left,m_ParentRect.bottom-m_ParentRect.top,
		SWP_NOACTIVATE|SWP_SHOWWINDOW);
	pParent->GetClientRect(m_ClientRect);
	MoveWindow(m_ClientRect);
	ShowWindow(SW_SHOW);
	OnSelchange();
}

void CTeleList::Hide()
{
	KillTimer(m_timer);
	m_pHint->Hide(false);
	m_pShowText->Hide();
	GetParent()->ShowWindow(SW_HIDE);
	m_CaretPos.x=m_CaretPos.y=-1;
	m_buf="";
	m_curPosInBuf=0;
	m_pEditor=NULL;
	CTxtTrap::m_ParsedDoc=NULL;
	CTxtTrap::m_ParsedView=NULL;
}

void CTeleList::OnDestroy() 
{
	CListBox::OnDestroy();
}

void CTeleList::AddTGroup(CTeleGroup *pGroup)
{
	m_pArrayOfGroup.Add(pGroup);
}

void CTeleList::AddTEntry(CTeleEntry *pEntry)
{
	m_pArrayOfEntries.Add(pEntry);
}

void CTeleList::ResetContent()
{
	m_pArrayOfEntries.RemoveAll();
	m_pArrayOfGroup.RemoveAll();
	CListBox::ResetContent();
}

void CTeleList::FilterList(CString str)
{
	str.MakeLower();
	LPCTSTR val=str.IsEmpty()?NULL:(LPCTSTR)str;
	SendMessage(WM_SETREDRAW,0);
	CListBox::ResetContent();
	int cnt=m_pArrayOfGroup.GetSize();
	for(int i=0;i<cnt;i++)
	{
		CTeleGroup* pGroup=(CTeleGroup*)m_pArrayOfGroup[i];
		for(int s=0;s<pGroup->GetCount();s++)
		{
			bool add=true;
			CTeleEntry* pEntry=pGroup->GetEntry(s);
			if(val)
			{
				for(LPCTSTR p1=pEntry->m_Name,p2=val;
					*p2 && *p1 && 
					CBLMap::m_LowerTable[(BYTE)*p1]==CBLMap::m_LowerTable[(BYTE)*p2];
					p1++,p2++);
				if(*p2)
					add=false;
			}
			if(add)
			{
				CString text=pEntry->m_Name;
				if(m_pOneList==this)
				{
					int hot=(int)m_mHotString.GetKey(text);
					if(hot)
					{
						CString t;
						t.Format("!%03i",hot);
						text=t+text;
					}
				}
				int pos=AddString(text);
				SetItemDataPtr(pos,pEntry);
			}
		}
	}
	SendMessage(WM_SETREDRAW,1);
	SetCurSel(0);
	SetTopIndex(0);
	m_pShowText->Hide();
}

CPoint CTeleList::Insert(CTeleEntry *pEntry)
{
	CTextDocument* pDoc=m_pEditor->GetDocument();
	CPoint ptStart=m_CaretPoint,ptEnd=ptStart;
	ptStart.x-=m_curPosInBuf;
	ptEnd.x=ptStart.x+m_buf.GetLength();
	CString line,insert;
	pEntry->GetInsert(insert);
	if(CPlugin::m_pPlugin)
		CPlugin::m_pPlugin->FireInsert(pEntry->m_pParent->m_Image,pEntry->m_Name,insert);
	CPoint insPos=ptStart;
	for(LPCTSTR ptr=insert;*ptr;ptr++)
	{
		if(*ptr=='!')
			break;
		if(*ptr=='\r')
		{
			insPos.y++;
			insPos.x=0;
			ptr++;
		}
		else
			insPos.x++;
	}
	insert.Replace("!","");
	// Выделим конечную часть вставляемого идентификатора
	CString insTail=insert.Mid(m_curPosInBuf);
	for(int i=0;i<insTail.GetLength();i++)
	{
		if(CBLMap::m_LowerTable[(BYTE)insTail[i]]==' ')
		{
			insTail=insTail.Left(i);
			break;
		}
	}
	// Получим строку и удалим перевод каретки
	pDoc->GetLine(ptStart.y,line);
	int ll=line.GetLength();
	if(ll>1 && line.Right(2)=="\r\n")
		line=line.Left(ll-=2);
	CString tail=line.Mid(ptEnd.x);
	for(i=0;i<tail.GetLength();i++)
	{
		if(CBLMap::m_LowerTable[(BYTE)tail[i]]==' ')
		{
			tail=tail.Left(i);
			break;
		}
	}
	bool extend=false;
	if(!tail.IsEmpty() && tail.GetLength()==insTail.GetLength())
	{
		for(i=0;i<tail.GetLength();i++)
		{
			if(CBLMap::m_LowerTable[(BYTE)tail[i]]!=CBLMap::m_LowerTable[(BYTE)insTail[i]])
				break;
		}
		if(i==tail.GetLength())
			extend=true;
	}
	if(extend)
		ptEnd.x+=tail.GetLength();
	pDoc->SetSel(ptStart,ptEnd);
	pDoc->Replace(insert);
	m_pEditor->MoveCaret(insPos,insPos,TRUE);
	Hide();
	return insPos;
}

void CTeleList::SetBufer(CString txt)
{
	m_buf=txt;
	m_curPosInBuf=txt.GetLength();
}
