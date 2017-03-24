// txttrap.cpp
#include "StdAfx.h"
#include "txttrap.h"
#include "resource.h"
#include "parser.h"
#include "telelist.h"
#include "telepanel.h"
#include "typescont.h"
#include "option.h"
#include "typegroup.h"
#include "EnumProcDlg.h"
#include "codejump.h"
#include "paramwnd.h"
#include "stringparser.h"
#include "plugin.h"
#include "templateinfo.h"
#include "MethComboBox.h"

CRuntimeClass* CTxtTrap::m_pTextDocClass;
CRuntimeClass* CTxtTrap::m_pTextEditorClass;
CRuntimeClass* CTxtTrap::m_pWorkBookClass;
CDocTemplate*  CTxtTrap::m_pAssistant;
CPoint CTxtTrap::m_szFont;
CTxtTrap* CTxtTrap::m_ParsedView=NULL;
CTextDocument* CTxtTrap::m_ParsedDoc=NULL;
CHackSwap CTxtTrap::m_hackDestroy;
CHackSwap CTxtTrap::m_hackTemplate;
static CHackSwap hackUpdateCaretPos;

CParser* CTxtTrap::GetParser()
{
	if(!m_ParsedDoc)
		return CParser::m_GM;
	return (CParser*)m_ParsedDoc->m_dwRef;
}
static LPCTSTR docClasses="CMDDescriptionDoc,CWBModuleDoc,CBodyTextDoc,CWorkBookDoc";

int CTxtTrap::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTextEditor::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(!strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
		return 0;
	CParser* pParser;
	CString type;
	long id=CConfigCont::DocumentToID(m_pDocument,type);
	if(id==1)
		pParser=CParser::m_GM;
	else
	{
		pParser=GetMyParser();
		if(!pParser)
			pParser=new CParser((CTextDocument*)m_pDocument);
	}
	m_pDocument->m_dwRef=(DWORD)pParser;
	pParser->IncrRef();
	return 0;
}

void CTxtTrap::OnDestroy() 
{
	if(strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
	{
		CParser* pParser=GetMyParser();
		if(pParser)
			pParser->DecrRef();
	}
	m_hackDestroy.Swap();
	CTextEditor::OnDestroy();
	m_hackDestroy.Swap();
}

const AFX_MSGMAP* PASCAL CTxtTrap::_GetBaseWrap()
{
	return &CTxtTrap::messageMap;
}

void CTxtTrap::OnTxtEnumProc()
{
	if(COption::m_pOption->GetUseEnumProc() ||
		!strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
		CTextEditor::OnTxtEnumproc();
	else
		CEnumProcDlg::ShowEnumProc(this);
}

void CTxtTrap::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CTextEditor::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTxtTrap::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTextEditor::OnChar(nChar, nRepCnt, nFlags);
	CParser* pParser=GetMyParser();
	if(!pParser)
		return;
	pParser->QuickParse(this,nChar);
	/*
	if(pParser->m_NeedQuick)
	{
		if(nChar>32 && (CBLMap::m_LowerTable[(BYTE)nChar]!=' ' || nChar=='.'))
			PostMessage(WM_COMMAND,ID_AUTOCOMPLETE);
	}
	else
	{
		if(nChar=='.')
			PostMessage(WM_COMMAND,ID_AUTOCOMPLETE);
		else if(CBLMap::m_LowerTable[(BYTE)nChar]==' ')
			pParser->m_NeedQuick=true;
	}
	*/
}

void CTxtTrap::Init()
{
	pBkEndUI->DoStatusLine("Телепат: инициализация...");
	new COption;
	CTEntryDrawing::Init();
	new CParser(NULL);
	CLexer::Init();
	CMethodParser::Init();
	new CTypesCont;
	CTypeGroup::Init();
	CStringParser::Init();
	new CParamWnd;
/* 
	Перехват msgmap основного окна.
	Для этого возьмем карту основного окна,
	свою карту, и переставим в них lpEntries.
	Также pfnGetBaseMap из карты основного окна
	перепишем в свою карту, а в основном окне
	вместо нее пропишем адрес функции, возращающую
	нашу карту.
	struct AFX_MSGMAP
	{
		const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)();
		const AFX_MSGMAP_ENTRY* lpEntries;
	};
*/
	HINSTANCE hTxt=GetModuleHandle("txtedt.dll");
	m_hackDestroy.Hack("txtedt.dll","?OnDestroy@CTextEditor@@IAEXXZ",(AFX_PMSG)OnDestroy);
	m_hackTemplate.Hack("txtedt.dll","?ReplaceTempl@CTextDocument@@QAEHPBD0@Z",(AFX_PMSG)ReplaceTemplWrap);
	hackUpdateCaretPos.Hack("txtedt.dll","?OnUpdateCaretPos@CTextEditor@@IAEXPAVCCmdUI@@@Z",(AFX_PMSG)OnUpdateCaretPosEx);
	AFX_MSGMAP* pFrameMap=(AFX_MSGMAP*)GetProcAddress(hTxt,"?messageMap@CTextEditor@@1UAFX_MSGMAP@@B");
	AFX_MSGMAP* pMyMap=(AFX_MSGMAP*)&messageMap;
	DWORD old;
	VirtualProtect(pFrameMap,sizeof(*pFrameMap),PAGE_EXECUTE_READWRITE,&old);
	VirtualProtect(pMyMap,sizeof(*pMyMap),PAGE_EXECUTE_READWRITE,&old);
	AFX_MSGMAP_ENTRY* pTemp=(AFX_MSGMAP_ENTRY*)pFrameMap->lpEntries;
	pFrameMap->lpEntries=pMyMap->lpEntries;
	pMyMap->lpEntries=pTemp;
	pMyMap->pfnGetBaseMap=pFrameMap->pfnGetBaseMap;
	pFrameMap->pfnGetBaseMap=_GetBaseWrap;

	m_pTextDocClass=(CRuntimeClass*)GetProcAddress(hTxt,"?classCTextDocument@CTextDocument@@2UCRuntimeClass@@B");
	m_pTextEditorClass=(CRuntimeClass*)GetProcAddress(hTxt,"?classCTextEditor@CTextEditor@@2UCRuntimeClass@@B");
	hTxt=GetModuleHandle("seven.dll");
	m_pWorkBookClass=(CRuntimeClass*)GetProcAddress(hTxt,"?classCWorkBookDoc@CWorkBookDoc@@2UCRuntimeClass@@B");

	for(POSITION pos=pMainApp->GetFirstDocTemplatePosition();pos;)
	{
		CMyDocTemp* pTemp=(CMyDocTemp*)pMainApp->GetNextDocTemplate(pos);
		CRuntimeClass* pRtc=pTemp->GetDocClass();
		if(strstr("CAssistantDoc",pRtc->m_lpszClassName))
			m_pAssistant=pTemp;
		if(pRtc->IsDerivedFrom(m_pTextDocClass) || pRtc->IsDerivedFrom(m_pWorkBookClass))
		{
			if(strstr(docClasses,pRtc->m_lpszClassName))
			{
				MergeMenu(GetSubMenu(pTemp->m_hMenuShared,0));
				MergeAccelTable(pTemp->m_hAccelTable);
			}
		}
	}
	pBkEndUI->DoStatusLine("");
}

BEGIN_MESSAGE_MAP(CTxtTrap,CTextEditor)
	//{{AFX_MSG_MAP(CTxtTrap)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_AUTOCOMPLETE, OnAutocomplete)
	ON_COMMAND(ID_SHOWPARAMS, OnShowparams)
	ON_COMMAND(ID_SHOWTYPE, OnShowtype)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CODEJUMP, OnCodejump)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PARAM_NEXT, OnParamNext)
	ON_COMMAND(ID_PARAM_PREV, OnParamPrev)
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOVE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_COMMAND(ID_SHOWTEMPLATE, OnShowtemplate)
	ON_COMMAND(ID_METHOD_LIST_SHOW, OnMethodListShow)
	ON_COMMAND(ID_QUICK_AUTOCOMPLETE,OnQuickAutoComplete)
	ON_COMMAND(ID_JUMP_ANCHOR, OnJumpAnchor)
	//}}AFX_MSG_MAP
	ON_COMMAND(33298,OnTxtEnumProc)
	ON_COMMAND_RANGE(1,500,InsertTemplate)
	ON_COMMAND_RANGE(501,1000,OnCustomCmd)
	ON_UPDATE_COMMAND_UI_RANGE(501,1000,OnUpdateCustomCmd)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AUTOCOMPLETE,ID_CODEJUMP, OnUpdateButtons)
END_MESSAGE_MAP()

CTxtTrap* CTxtTrap::GetCurrentView()
{
	CMDIChildWnd* pChild=pMainFrame->MDIGetActive();
	if(pChild)
	{
		CView* pView=pChild->GetActiveView();
		if(pView->IsKindOf(m_pTextEditorClass))
			return (CTxtTrap*)pView;
		CDocument* pDoc=pView->GetDocument();
		if(pDoc && pDoc->IsKindOf(m_pWorkBookClass))
		{
			CDocument* pActive=((CWorkBookDoc*)pDoc)->GetActiveDocument();
			if(pActive && pActive->IsKindOf(m_pTextDocClass))
			{
				POSITION pos=pActive->GetFirstViewPosition();
				if(pos)
				{
					pView=pActive->GetNextView(pos);
					if(pView)
					{
						CFrameWnd* pParent=pView->GetParentFrame();
						if(pParent)
						{
							pView=pParent->GetActiveView();
							if(pView && pView->IsKindOf(m_pTextEditorClass))
								return (CTxtTrap*)pView;
						}
					}
				}
			}
		}
	}
	return NULL;
}

void CTxtTrap::OnAutocomplete() 
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		CTeleList::m_pOneList->Hide();
	CParser* lex=GetMyParser();
	if(lex)
	{
		lex->m_LastParsedFrom=-1;
		CParser::m_GM->m_LastParsedFrom=-1;
		CParser::m_GM->m_state=0;
		lex->AutoComplete(this);
	}
}

void CTxtTrap::OnShowparams() 
{
	CParser* pParser=GetMyParser();
	if(pParser)
		pParser->ParseParams(this);
}

void CTxtTrap::OnShowtype() 
{
	AfxMessageBox("ToDo: ShowType");
}

void CTxtTrap::OnCodejump() 
{
	if(!strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
		return;
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CCodeJump::GetCodeJump()->Jump(this);
}

void CTxtTrap::OnUpdateButtons(CCmdUI* pCmdUI) 
{
	bool enable=false;
	if(strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
	{
		CTextDocument* pDoc=(CTextDocument*)m_pDocument;
		if(pDoc->IsModule())
		{
			if(!(pCmdUI->m_nID==ID_AUTOCOMPLETE && pDoc->IsReadOnly()))
				enable=true;
		}

	}
	pCmdUI->Enable(enable);
}

static void MakeOwnerDraw(HMENU hMenu)
{
	int cnt=GetMenuItemCount(hMenu);
	MENUITEMINFO info;
	info.cbSize=sizeof(info);
	info.fMask=MIIM_ID|MIIM_TYPE|MIIM_DATA|MIIM_SUBMENU;
	for(int i=0;i<cnt;i++)
	{
		char buf[100];
		info.cch=100;
		info.dwTypeData=buf;
		GetMenuItemInfo(hMenu,i,TRUE,&info);
		if(info.wID)
		{
			CMenu7::AddTempString(info.wID,buf,1);
			info.fType=MFT_OWNERDRAW;
			info.dwItemData=info.wID;
			SetMenuItemInfo(hMenu,i,TRUE,&info);
		}
	}
}

void CTxtTrap::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(strstr(docClasses,m_pDocument->GetRuntimeClass()->m_lpszClassName))
	{
		CMenu menu;
		menu.LoadMenu(380);
		CMenu* pPopup=menu.GetSubMenu(0);
		FillTemplateMenu(pPopup->GetSubMenu(4));
		MergeMenu(pPopup->m_hMenu);
		MakeOwnerDraw(pPopup->m_hMenu);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,pMainFrame);
		DestroyMenu(menu);
	}
	else
		CTextEditor::OnContextMenu(pWnd,point);
}

void CTxtTrap::MergeMenu(HMENU pMenu)
{
	HMENU hMenu=LoadMenu(hMyInst,(LPCTSTR)IDR_MENU1);
	int count=GetMenuItemCount(hMenu);
	int cnt=GetMenuItemCount(pMenu);
	char buf[200];
	MENUITEMINFO info;
	info.cbSize=sizeof(info);
	info.fMask=MIIM_DATA|MIIM_ID|MIIM_TYPE|MIIM_SUBMENU;
	for(UINT i=0;i<count;i++)
	{
		info.dwTypeData=buf;
		info.cch=200;
		*buf=0;
		GetMenuItemInfo(hMenu,i,TRUE,&info);
		InsertMenuItem(pMenu,cnt++,TRUE,&info);
	}
}

void CTxtTrap::MergeAccelTable(HACCEL& hAccel)
{
	HACCEL hMyTable=LoadAccelerators(hMyInst,(LPCTSTR)IDR_ACCELERATOR1);
	int count=CopyAcceleratorTable(hMyTable,NULL,0);
	int cnt=CopyAcceleratorTable(hAccel,NULL,0);
	ACCEL* pAcc=new ACCEL[count+cnt];
	CopyAcceleratorTable(hMyTable,pAcc,count);
	DestroyAcceleratorTable(hMyTable);
	CopyAcceleratorTable(hAccel,pAcc+count,cnt);
	DestroyAcceleratorTable(hAccel);
	hAccel=CreateAcceleratorTable(pAcc,count+cnt);
	delete [] pAcc;
}

CPoint CTxtTrap::GetCaretPos()
{
	return m_caretInEndOfSel?m_ptNewEndSel:m_ptNewStartSel;
}

bool CTxtTrap::AutoReplace(BYTE nChar)
{
	CPoint ptStart=GetCaretPos(),ptEnd=ptStart,ptCaret=ptStart;
	CTextDocument* pDoc=(CTextDocument*)m_pDocument;
	CString replText;
	CString line;
	pDoc->GetLine(ptStart.y,line);

	if(nChar=='?')
	{
		ptCaret.x++;
		replText="(,,)";
	}
	else if(nChar=='=' || nChar=='+' || nChar=='-')
	{
		ptStart.x-=2;
		if(ptStart.x<1)
			return false;
		char bef=line.GetAt(ptStart.x);
		if(((bef=='+' || bef=='-' || bef=='*' || bef=='/' || bef=='%') && nChar=='=')||
			((nChar=='+' || nChar=='-') && nChar==bef))
		{
			LPCTSTR wht="";
			LPCTSTR start=line,ptr=start+ptStart.x-1;
			while(ptr>=start && (*ptr==' ' || *ptr=='\t'))
			{
				wht=" ";
				ptr--;
			}
			if(ptr<start)
				return false;
			LPCTSTR last=ptr;
			while(ptr>=start && (*ptr=='.' || CBLMap::m_LowerTable[(BYTE)*ptr]!=' '))
				ptr--;
			if(ptr==last)
				return false;
			ptr++;
			CString txt(ptr,last-ptr+1);
			replText.Format(nChar=='='?"=%s%s%s%c%s":"=%s%s%s%c%s1;",wht,txt,wht,bef,wht);
			ptCaret.x=ptStart.x+replText.GetLength();
		}
	}
	if(!replText.IsEmpty())
	{
		pDoc->SetSel(ptStart,ptEnd);
		pDoc->Replace(replText);
		MoveCaret(ptCaret,ptCaret,1);
		return true;
	}
	return false;
}

void CTxtTrap::OnQuickAutoComplete()
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CParser* lex=GetMyParser();
	if(lex)
		lex->AutoComplete(this);
}

void CTxtTrap::OnParamNext() 
{
	CParamWnd::GetParamWnd()->ParamNext();
}

void CTxtTrap::OnParamPrev() 
{
	CParamWnd::GetParamWnd()->ParamPrev();
}

void CTxtTrap::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTextEditor::OnKeyUp(nChar, nRepCnt, nFlags);
	CParser* pParser=GetMyParser();
	if(!pParser)
		return;
	if(CParamWnd::GetParamWnd()->IsWindowVisible())
		pParser->ParseParams(this);
}

static void HideParamWnd()
{
	CParamWnd* pWnd=CParamWnd::GetParamWnd();
	if(pWnd && pWnd->IsWindowVisible())
		pWnd->Hide();
}

void CTxtTrap::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTextEditor::OnLButtonDown(nFlags, point);
	HideParamWnd();
}

void CTxtTrap::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTextEditor::OnLButtonDblClk(nFlags, point);
	HideParamWnd();
}
void CTxtTrap::OnMove(int x, int y) 
{
	CTextEditor::OnMove(x, y);
	HideParamWnd();
}

void CTxtTrap::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	HideParamWnd();
	CTextEditor::OnRButtonDblClk(nFlags, point);
}

void CTxtTrap::OnRButtonDown(UINT nFlags, CPoint point) 
{
	HideParamWnd();
	CTextEditor::OnRButtonDown(nFlags, point);
}

void CTxtTrap::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	HideParamWnd();
	CTextEditor::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTxtTrap::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	HideParamWnd();
	CTextEditor::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CTxtTrap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	HideParamWnd();
	return CTextEditor::OnMouseWheel(nFlags, zDelta, pt);
}

void CTxtTrap::OnSize(UINT nType, int cx, int cy) 
{
	HideParamWnd();
	CTextEditor::OnSize(nType, cx, cy);
}

static bool TempEnable=false;
int CTxtTrap::ReplaceTemplWrap(LPCTSTR str1,LPCTSTR str2)
{
	CTextDocument* pDoc=(CTextDocument*) this;
	CPoint pt1,pt2;
	pDoc->GetSel(pt1,pt2);
	int disable;
	if((disable=COption::m_pOption->GetDisableTemplate()) && !TempEnable)
	{
		CTxtTrap* pEditor=CTxtTrap::GetCurrentView();
		if(pEditor)
		{
			CParser* pParser=pEditor->GetMyParser();
			if(pParser)
			{
				SLexem lexem;
				CPoint ptCaret=pEditor->GetCaretPos();
				int mode=pParser->AnalizeLine(lexem,pDoc,ptCaret.y,ptCaret.x);
				if((mode==-1 && (disable & 1)) || (mode==-2 && (disable & 2)))
					return 0;
			}
		}
	}
	TempEnable=false;
	CPoint ptCaret=pt1;
	CString tText=str1;
	CString tName=str2;
	tName.Replace("&&","~~~~~");
	tName.Replace("&","");
	tName.Replace("~~~~~","&");
	int end=tName.Find('¦');
	if(end>-1)
		tName=tName.Left(end);
	if(CPlugin::m_pPlugin)
	{
		if(!CPlugin::m_pPlugin->FireOnTemplate(tName,tText))
			return 0;
	}
	pDoc->SetSel(pt1,pt2);
	m_hackTemplate.Swap();
	int ret=pDoc->ReplaceTempl(tText,tName);
	m_hackTemplate.Swap();
	return ret;
}

static void ProcessMenu(HMENU menu,LPCTSTR& ptr,int& level,int& count)
{
	if(count==1000)
		return;
	int newLevel;
	while(*ptr)
	{
		LPCTSTR pStart=ptr;
		DWORD flag=0;
		while(*ptr && *ptr!='\r' && *ptr!='\n')
		{
			if(*ptr=='|')
			{
				if(ptr[1]=='|')
					ptr++;
				else
					break;
			}
			ptr++;
		}
		CString name(pStart,ptr-pStart);
		CString key;
		name.Replace("||","|");
		if(*ptr=='|')
		{
			ptr++;
			while(*ptr && *ptr!='\r' && *ptr!='\n' && *ptr!='|')
			{
				if(*ptr=='c' || *ptr=='C')
					flag|=MF_CHECKED;
				else if(*ptr=='d' || *ptr=='D')
					flag|=MF_GRAYED;
				ptr++;
			}
			if(*ptr=='|')
			{
				pStart=++ptr;
				while(*ptr && *ptr!='\r' && *ptr!='\n')
					ptr++;
				key=CString(pStart,ptr-pStart);
			}
		}
		while(*ptr && (*ptr=='\r' || *ptr=='\n'))
			ptr++;
		if(*ptr)
		{
			newLevel=0;
			while(*ptr && *ptr=='\t')
				*ptr++,newLevel++;
		}
		else
			newLevel=level;
		if(newLevel>level)
		{
			HMENU hNewMenu=CreatePopupMenu();
			ProcessMenu(hNewMenu,ptr,newLevel,count);
			AppendMenu(menu,MF_POPUP|MF_STRING,(UINT)hNewMenu,name);
		}
		else
		{
			if(name=="-")
				AppendMenu(menu,MF_SEPARATOR,0,NULL);
			else
			{
				AppendMenu(menu,MF_STRING|flag,++count,name);
				if(key.IsEmpty())
					key=name;
				CTemplateInfo::m_TemplateInfo.recNames.Add(key);
				CTemplateInfo::m_TemplateInfo.id2temp[(void*)count]=(void*)flag;
			}
		}
		if(newLevel<level)
			break;
	}
	level=newLevel;
}

bool CTxtTrap::FillTemplateMenu(CMenu *pMenu)
{
	UINT add=0;
	CString addName;
	if(pMenu->GetMenuItemCount())
	{
		add=pMenu->GetMenuItemID(0);
		pMenu->GetMenuString(0,addName,MF_BYPOSITION);
		pMenu->RemoveMenu(0,MF_BYPOSITION);
	}
	if(CTemplateInfo::m_TemplateInfo.ReadTemplate())
		CTemplateInfo::m_TemplateInfo.FillMenu(pMenu->m_hMenu);
	else
		AddTemplateMenu(pMenu);

	if(CPlugin::m_pPlugin)
	{
		CString customMenu=CPlugin::m_pPlugin->GetCustomMenu();
		if(!customMenu.IsEmpty())
		{
			CTemplateInfo::m_TemplateInfo.recNames.RemoveAll();
			if(pMenu->GetMenuItemCount())
				pMenu->AppendMenu(MF_SEPARATOR);
			LPCTSTR ptr=customMenu;
			int level=0;
			int count=500;
			ProcessMenu(pMenu->m_hMenu,ptr,level,count);
		}
	}
	if(add)
	{
		if(pMenu->GetMenuItemCount())
			pMenu->AppendMenu(MF_SEPARATOR);
		pMenu->AppendMenu(MF_STRING,add,addName);
	}
	return true;
}

void CTxtTrap::OnShowtemplate() 
{
	CMenu menu;
	menu.CreatePopupMenu();
	FillTemplateMenu(&menu);
	CPoint pt;
	::GetCaretPos(&pt);
	ClientToScreen(&pt);
	CalcFontSize();
	pt.y+=m_szFont.y+2;
	TrackPopupMenu(menu,TPM_LEFTALIGN|TPM_LEFTBUTTON,pt.x,pt.y,0,m_hWnd,NULL);
}

void CTxtTrap::InsertTemplate(UINT id)
{
	TemplateItem* pItem=CTemplateInfo::m_TemplateInfo.GetFromID(id);
	if(!pItem)
		return;
	CString txt;
	GetSelText(txt);
	CPoint pt1,pt2;
	GetDocument()->GetSel(pt1,pt2);
	TempEnable=true;
	ReplaceTemplate(pItem->text,pItem->name,pt1,pt2,TRUE);
	TempEnable=false;
	CTemplateInfo::m_TemplateInfo.AddRecent(pItem);
}

void CTxtTrap::OnCustomCmd(UINT id)
{
	id-=501;
	if(id>=0 && id<CTemplateInfo::m_TemplateInfo.recNames.GetSize())
	{
		CString cmd=CTemplateInfo::m_TemplateInfo.recNames[id];
		if(CPlugin::m_pPlugin)
			CPlugin::m_pPlugin->FireOnMenu(cmd);
	}
	CTemplateInfo::m_TemplateInfo.recNames.RemoveAll();
}

void CTxtTrap::OnUpdateCustomCmd(CCmdUI *pcm)
{
	DWORD flag=(DWORD)CTemplateInfo::m_TemplateInfo.id2temp[(void*)pcm->m_nID];
	if(flag & MF_GRAYED)
		pcm->Enable(FALSE);
}

void CTxtTrap::OnMethodListShow() 
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CParser* pParser=GetMyParser();
	if(pParser)
	{
		for(POSITION pos=CMethComboBox::m_boxes.GetStartPosition();pos;)
		{
			CMethComboBox* pBox;
			WORD w;
			CMethComboBox::m_boxes.GetNextAssoc(pos,(void*&)pBox,w);
			if(pBox->IsWindowVisible())
			{
				if(!pBox->GetDroppedState())
				{
					pBox->SetFocus();
					pBox->ShowDropDown();
				}
				break;
			}
		}
	}
}

void CTxtTrap::OnJumpAnchor() 
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CParser* pParser=GetMyParser();
	if(pParser)
	{
		CString txt;
		GetDocument()->GetText(txt);
		CLexer& lex=pParser->m_lexParser;
		lex.SetSource(txt,txt.GetLength());
		SLexem lexem;
		CStringArray anchors;
		CDWordArray	lines;
		do{
			lex.NextLexem(lexem);
			if(lexem.type==STATE_REMARK)
			{
				if(lexem.IsLexemBegin("//%% ",false))
				{
					CString anchor(lexem.pStart+5,lexem.len-5);
					anchor.TrimLeft();
					anchor.TrimRight();
					if(!anchor.IsEmpty())
					{
						anchors.Add(anchor);
						lines.Add(lexem.line);
					}
				}
			}
		}while(lex.m_pCurPos<lex.m_pEndPos);
		int size=anchors.GetSize();
		if(!size)
		{
			AfxMessageBox("Закладки не установлены. Используйте конструкцию \n//%% Имя закладки\nдля их установки");
			return;
		}
		CMenu menu;
		menu.CreatePopupMenu();
		for(int i=0;i<size;i++)
			menu.InsertMenu(i,MF_BYPOSITION,lines[i]+1,anchors[i]);
		CRect rc;
		/*
		pParser->m_pToolBar->GetItemRect(3,rc);
		pParser->m_pToolBar->ClientToScreen(rc);
		int cmd=menu.TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom+1,this);
		if(cmd)
			CCodeJump::GetCodeJump()->JumpFromTo(this,(CTextDocument*)m_pDocument,--cmd);
		*/
	}
}

void CTxtTrap::OnUpdateCaretPosEx(CCmdUI *pCmdUI)
{
	hackUpdateCaretPos.Swap();
	OnUpdateCaretPos(pCmdUI);
	hackUpdateCaretPos.Swap();
	if(!CTeleList::m_pOneList->IsWindowVisible() && IsWindowVisible())
	{
		HWND hFocus = ::GetFocus();
		if(m_hWnd == hFocus || ::IsChild(m_hWnd, hFocus))
			CMethComboBox::UpdateList(this);
	}
}
