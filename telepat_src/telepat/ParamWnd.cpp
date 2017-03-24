// ParamWnd.cpp : implementation file
#include "stdafx.h"
#include "ParamWnd.h"
#include "txttrap.h"
#include "telelist.h"
#include "parser.h"
#include "typescont.h"
#include "option.h"
#include "hintwnd.h"
CParamWnd* CParamWnd::m_pParamWnd=NULL;
static CString paramClassName;
static LPCTSTR ParamNames[]={
	"ParamInfo\\quick",
	"ParamInfo\\hintmethod",
	"ParamInfo\\hintparam",
};

void CParamWnd::Init()
{
	Create(CHintWnd::m_ClassName,NULL,WS_POPUP|WS_BORDER,CRect(0,0,0,0),pMainFrame,0);
	m_timer=0;
	HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	m_boldFont.Attach(hFont);
	LOGFONT lf;
	m_boldFont.GetLogFont(&lf);
	lf.lfWeight=600;
	m_boldFont.Detach();
	m_boldFont.CreateFontIndirect(&lf);
	CDC* pDC=GetDC();
	CGdiObject* pOldFont=pDC->SelectStockObject(DEFAULT_GUI_FONT);
	m_LineHeight=pDC->GetTextExtent(" ").cy;
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	COption* pOpt=COption::m_pOption;
	bool* pVals[]={&m_QuickOption,&m_HintMethodOption,&m_HintParamOption};
	for(int i=0;i<3;i++)
	{
		int val=pOpt->GetInt(ParamNames[i]);
		if(!val)
			pOpt->SetInt(ParamNames[i],val=2);
		*pVals[i]=val==2?true:false;
	}
}

bool CParamWnd::GetQuickOption()
{
	return m_QuickOption;
}


void CParamWnd::SetQuickOption(bool val)
{
	if(val!=m_QuickOption)
	{
		m_QuickOption=val;
		COption::m_pOption->SetInt(ParamNames[0],val?2:1);
	}
}

bool CParamWnd::GetHintMethodOption()
{
	return m_HintMethodOption;
}

void CParamWnd::SetHintMethodOption(bool val)
{
	if(val!=m_HintMethodOption)
	{
		m_HintMethodOption=val;
		COption::m_pOption->SetInt(ParamNames[1],val?2:1);
	}
}

bool CParamWnd::GetHintParamOption()
{
	return m_HintParamOption;
}

void CParamWnd::SetHintParamOption(bool val)
{
	if(val!=m_HintParamOption)
	{
		m_HintParamOption=val;
		COption::m_pOption->SetInt(ParamNames[2],val?2:1);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CParamWnd
CParamWnd::CParamWnd()
{
	m_pParamWnd=this;
}

CParamWnd::~CParamWnd()
{
}

void CALLBACK CParamWnd::TimerProc(
	HWND hwnd,     // handle of window for timer messages
	UINT uMsg,     // WM_TIMER message
	UINT idEvent,  // timer identifier
	DWORD dwTime   // current system time
)
{
	CParamWnd* pWnd=GetParamWnd();
	if(!IsWindow(pWnd->m_EditorHWND))
	{
		pWnd->Hide();
		return;
	}
	if(CTxtTrap::GetCurrentView()!=pWnd->m_pEditor)
	{
		pWnd->Hide();
		return;
	}
	HWND hFocus = ::GetFocus();
	if(!(hFocus == pWnd->m_EditorHWND || hFocus == CTeleList::m_pOneList->m_hWnd ||
		::IsChild(pWnd->m_EditorHWND, hFocus)))
	{
		pWnd->Hide();
		return;
	}
}


BEGIN_MESSAGE_MAP(CParamWnd, CWnd)
	//{{AFX_MSG_MAP(CParamWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CParamWnd::Clear()
{
	int s=m_methods.GetSize();
	for(int i=0;i<s;i++)
		delete (SMethodInfo*)m_methods[i];
	m_methods.RemoveAll();
	m_curMethod=m_curParam=0;
	m_name.Empty();
}

void CParamWnd::Show(CTxtTrap* pEditor)
{
	if(!m_methods.GetSize())
	{
		Hide();
		return;
	}
	m_pEditor=pEditor;
	m_EditorHWND=pEditor->m_hWnd;
	if(!m_timer)
		m_timer=SetTimer(1,100,TimerProc);

	CPoint ptCaret=GetCaretPos();
	m_startPoint=m_pEditor->GetCaretPos();

	m_pEditor->ClientToScreen(&ptCaret);
	ptCaret.x-=50;
	if(ptCaret.x<0)
		ptCaret.x=0;
	ShowAt(ptCaret.x,ptCaret.y,true);
}

void CParamWnd::ReShow()
{
	CPoint ptCaret=m_pEditor->GetCaretPos();
	if(ptCaret.y!=m_startPoint.y)
		Show(m_pEditor);
	else
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);
		ShowAt(rcWnd.left,rcWnd.top,false);
	}
}

void CParamWnd::ShowAt(int x, int y,bool addLine)
{
	CalculateSize();
	CRect wrc;
	GetDesktopWindow()->GetWindowRect(wrc);
	if(x+m_rect.right>=wrc.right)
		x=wrc.right-m_rect.right-10;
	if(addLine)
	{
		m_pEditor->CalcFontSize();
		y+=CTxtTrap::m_szFont.y+2;
		if(y+m_rect.bottom>=wrc.bottom)
			y-=CTxtTrap::m_szFont.y+2+m_rect.bottom;
	}
	SetWindowPos(&wndTopMost,x,y,m_rect.Width(),m_rect.Height(),SWP_SHOWWINDOW|SWP_NOACTIVATE);
	Invalidate();
	UpdateWindow();
}

void CParamWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.FillSolidRect(m_rect,CTEntryDrawing::m_ToolTipColor);
	dc.SetTextColor(CTEntryDrawing::m_ToolTipFColor);
	dc.SetBkMode(TRANSPARENT);
	CGdiObject* pOldFont=dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.SetTextAlign(TA_UPDATECP);
	int line=2-m_LineHeight;
	for(int i=0;i<m_h1.GetSize();i++)
	{
		line+=m_LineHeight;
		dc.MoveTo(2,line);
		dc.TextOut(0,0,m_h1[i]);
	}
	dc.SelectObject(&m_boldFont);
	for(i=0;i<m_b.GetSize();i++)
	{
		if(i)
			dc.MoveTo(2,(line+=m_LineHeight));
		dc.TextOut(0,0,m_b[i]);
	}
	dc.SelectStockObject(DEFAULT_GUI_FONT);
	for(i=0;i<m_h2.GetSize();i++)
	{
		if(i)
			dc.MoveTo(2,(line+=m_LineHeight));
		dc.TextOut(0,0,m_h2[i]);
	}
	dc.SelectObject(pOldFont);
}

void CParamWnd::Hide()
{
	ShowWindow(SW_HIDE);
	if(m_timer)
	{
		KillTimer(m_timer);
		m_timer=0;
	}
	Clear();
}

// Разбивает строку текста на несколько строк.
// Возвращает ширину последней строки
static int LinesCount;
static int BreakLine(CString txt,CStringArray& str,HDC hDC,int maxW,int start,int& calcMaxW)
{
	int cr;
	while((cr=txt.Find('\n'))>-1)
	{
		BreakLine(txt.Left(cr),str,hDC,maxW,start,calcMaxW);
		LinesCount++;
		txt=txt.Mid(cr+1);
		start=0;
	}
	int symb;
	int lineW=start;
	CSize size;
	txt.Replace('\t',' ');
	while(txt.GetLength())
	{
		CString line;
		GetTextExtentExPoint(hDC,txt,txt.GetLength(),maxW-start,&symb,NULL,&size);
		if(symb<txt.GetLength())
		{
			LinesCount++;
			line=txt.Left(symb);
			int space=line.ReverseFind(' ');
			if(space>-1)
			{
				line=line.Left(space);
				txt=txt.Mid(space+1);
			}
			else
			{
				if(start)
				{
					str.Add("");
					start=0;
					continue;
				}
				txt=txt.Mid(line.GetLength());
			}
			GetTextExtentPoint32(hDC,line,line.GetLength(),&size);
		}
		else
		{
			line=txt;
			txt="";
		}
		str.Add(line);
		lineW=start+size.cx;
		if(lineW>calcMaxW)
			calcMaxW=lineW;
		start=0;
	}
	return lineW;
}

void CParamWnd::CalculateSize()
{
	m_rect.SetRectEmpty();
	CRect wrc;
	GetDesktopWindow()->GetWindowRect(wrc);
	int maxW=wrc.Width()*2/3;

	CDC *pDC=GetDC();
	SMethodInfo* pInfo=(SMethodInfo*)m_methods[m_curMethod];
	CString h1,h2,bold;
	int c=m_methods.GetSize();
	if(c>1)
		h1.Format("[%i из %i] ",m_curMethod+1,c);
	pInfo->GetText(h1,bold,h2);
	m_h1.RemoveAll();
	m_h2.RemoveAll();
	m_b.RemoveAll();
	int start=0;
	int maxCalcW=0;
	LinesCount=1;
	CGdiObject* pOldFont=pDC->SelectStockObject(DEFAULT_GUI_FONT);
	start=BreakLine(h1,m_h1,pDC->m_hDC,maxW,start,maxCalcW);
	pDC->SelectObject(&m_boldFont);
	start=BreakLine(bold,m_b,pDC->m_hDC,maxW,start,maxCalcW);
	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	BreakLine(h2,m_h2,pDC->m_hDC,maxW,start,maxCalcW);
	m_rect.right=maxCalcW+6;
	m_rect.bottom=m_LineHeight*LinesCount+6;
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

CString CParamWnd::SParam::GetText()
{
	CString head,tail;
	if(!def.IsEmpty())
	{
		head='[';
		if(def[0]!=' ')
			tail=tail+" = "+def;
		tail+=']';
	}
	if(val)
		head+="Знач ";
	return head+name+tail;
}

void CParamWnd::SMethodInfo::AddParam(CParamWnd::SParam *p)
{
	if(!params)
		params=p;
	else
	{
		SParam* ptr=params;
		while(ptr->next)
			ptr=ptr->next;
		ptr->next=p;
	}
}

void CParamWnd::SMethodInfo::GetText(CString &h1, CString &bold, CString &h2)
{
	h1+=name+'(';
	CParamWnd* pWnd=CParamWnd::GetParamWnd();
	int curParam=pWnd->m_curParam;
	SParam *p=params, *pCurParam=NULL;
	for(int i=0;i<curParam;i++)
	{
		if(!p)
			break;
		h1+=p->GetText();
		p=p->next;
		if(p)
			h1+=", ";
	}
	bold.Empty();
	for(;i<=curParam;i++)
	{
		if(!p)
			break;
		bold=p->GetText();
		pCurParam=p;
		p=p->next;
		if(p)
			bold+=", ";
	}
	h2.Empty();
	while(p)
	{
		h2+=p->GetText();
		p=p->next;
		if(p)
			h2+=", ";
	}
	h2+=')';
	if(pWnd->GetHintMethodOption() && !hint.IsEmpty())
		h2=h2+'\n'+hint;
	if(pCurParam && pWnd->GetHintParamOption() && !pCurParam->hint.IsEmpty())
		h2=h2+'\n'+pCurParam->name+": "+pCurParam->hint;
}

bool CParamWnd::Prepeare(CTxtTrap *pEditor)
{
	CParser* pParser=pEditor->GetMyParser();
	if(!pParser)
		return false;
	CMethodParser& mParser=pParser->m_MethParser;
	CString name=mParser.m_ParamsMethod;
	if(!name.CompareNoCase(m_name))
	{
		m_curParam=mParser.m_ParamsCount;
		ReShow();
		return false;
	}
	Clear();
	m_name=name;
	m_curParam=mParser.m_ParamsCount;
	MakeMethods((CProcCont*)pParser->m_Conts[CParser::Methods]);
	if(!pParser->IsGM())
		MakeMethods((CProcCont*)CParser::m_GM->m_Conts[CParser::Methods]);
	MakeStdMethods();
	return true;
}

void CParamWnd::ParamNext()
{
	if(!IsWindowVisible())
		return;
	int c=m_methods.GetSize();
	if(c<2)
		return;
	m_curMethod=(m_curMethod+1)%c;
	ReShow();
}

void CParamWnd::ParamPrev()
{
	if(!IsWindowVisible())
		return;
	int c=m_methods.GetSize();
	if(c<2)
		return;
	m_curMethod=(m_curMethod-1+c)%c;
	ReShow();
}

void CParamWnd::MakeMethods(CProcCont *pCont)
{
	SProcInfo* pInfo=pCont->GetProc(m_name);
	if(!pInfo)
		pInfo=pCont->GetForwards(m_name);
	if(!pInfo)
		return;
	SMethodInfo* pMethInfo=new SMethodInfo;
	pMethInfo->name=pInfo->name;
	pMethInfo->hint.Format("%s, Строка %i",pCont->m_Group[0]->name,pInfo->start.line+1);
	SParamInfo* par=pInfo->params;
	while(par)
	{
		SParam* pParam=new SParam(par->name,"",par->def);
		if(par->array)
			pParam->name+="[]";
		pParam->val=par->val;
		pMethInfo->AddParam(pParam);
		par=par->next;
	}
	m_methods.Add(pMethInfo);
}

void CParamWnd::MakeStdMethods()
{
	CTypesCont* pCont=CTypesCont::m_pOneCont;
	STMethInfo** pMeth=(STMethInfo**)pCont->m_Name2Methods.GetKey(m_name);
	if(!pMeth)
		return;
	while(*pMeth)
	{
		STMethInfo* pMethod=*pMeth;
		SMethodInfo* pInfo=new SMethodInfo(pCont->GetString(pMethod->Descr));
		if(pMethod->ParentType->Alias)
			pInfo->name=pCont->GetString(pMethod->ParentType->Alias);
		if(!pInfo->name.IsEmpty())
			pInfo->name+="::";
		pInfo->name+=pCont->GetString(pMethod->NameRus);
		CString ret=pCont->GetString(pMethod->retDescr);
		if(!ret.IsEmpty())
		{
			if(!pInfo->hint.IsEmpty())
				pInfo->hint+=". ";
			pInfo->hint=pInfo->hint+"Возвращает: "+ret+".";
		}
		for(int i=0;i<pMethod->ParamCnt;i++)
		{
			SParam* pPar=new SParam(pCont->GetString(pMethod->pParams[i].Name),
				pCont->GetString(pMethod->pParams[i].Descr));
			pPar->def=pCont->GetString(pMethod->pParams[i].DefVal);
			if(pPar->def=="_")
				pPar->def=" ";
			pInfo->AddParam(pPar);
		}
		m_methods.Add(pInfo);
		pMeth++;
	}
}

void CParamWnd::QuickActivate(CTxtTrap* pEditor)
{
	if(m_QuickOption)
		pEditor->PostMessage(WM_COMMAND,ID_SHOWPARAMS);
}
