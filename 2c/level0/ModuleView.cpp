// SampleView.cpp : implementation of the CModuleView class
//

#include "stdafx.h"
#include "Resource.h"

#include "ModuleDoc.h"
#include "ModuleView.h"
#include "MetaObject.h"
#include "MetadataTree.h"
#include "MainFrm.h"
#include "LineInput.h"
#include "FunctionList.h"
#include "DebugMessageBar.h"
#include "StackBar.h"


#include "ModuleDoc.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CMapStringToPtr	aHelpDescription;//описание ключевых слов и системных функций
extern 	int AfxCallInitProc(CString csName,CValue Param,CValue &vDeleteText,CValue &vInsertText);
extern CDebugMessageBar *pDebugMessageBar;
extern CStackBar *pDebugStackBar;
int nRunLine=-1;


/////////////////////////////////////////////////////////////////////////////
// CModuleView

IMPLEMENT_DYNCREATE(CModuleView, CCrystalEditView)

BEGIN_MESSAGE_MAP(CModuleView, CAutocomplectionCtrl)
	//{{AFX_MSG_MAP(CModuleView)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTONCOMMENT, OnButtoncomment)
	ON_COMMAND(ID_BUTTONDELCOMMENT, OnButtondelcomment)
	ON_COMMAND(ID_BUTTONPROC, OnButtonproc)
	ON_COMMAND(ID_BUTTONGOTOLINE, OnButtongotoline)
//	ON_UPDATE_COMMAND_UI(ID_BUTTONCOMMENT, OnUpdateFormatBlock)
	ON_COMMAND(ID_HELP_SEARCH_COMBO, OnHelpSearchCombo)
	ON_COMMAND(ID_EDIT_DEBUG_POINT, OnEditDebugPoint)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEBUG_POINT, OnUpdateEditDebugPoint)
	ON_COMMAND(ID_NEXT_DEBUG, OnNextDebug)
	ON_WM_MOUSEMOVE()
	ON_WM_SYSKEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_NEXT_DEBUG_OVER, OnNextDebugOver)
	ON_COMMAND(ID_RUN_DEBUG, OnRunDebug)
	ON_COMMAND(ID_STOP_DEBUG, OnStopDebug)
	ON_UPDATE_COMMAND_UI(ID_STOP_DEBUG, OnUpdateStopDebug)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	ON_WM_KILLFOCUS()
//	ON_UPDATE_COMMAND_UI(ID_BUTTONDELCOMMENT, OnUpdateFormatBlock)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BLOCK, OnUpdateFormatBlock)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CCrystalEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CCrystalEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CCrystalEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleView construction/destruction

CModuleView::CModuleView()
{
	pointShowTitle.x=0;
	pointShowTitle.y=0;
	bSingleModule=1;
	nGotoLine=0;
}

CModuleView::~CModuleView()
{

}

BOOL CModuleView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CAutocomplectionCtrl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CModuleView diagnostics

#ifdef _DEBUG
void CModuleView::AssertValid() const
{
	CAutocomplectionCtrl::AssertValid();
}

void CModuleView::Dump(CDumpContext& dc) const
{
	CAutocomplectionCtrl::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModuleView message handlers

CCrystalTextBuffer *CModuleView::LocateTextBuffer()
{
	return &GetDocument()->m_xTextBuffer;
}


void CModuleView::OnInitialUpdate() 
{
	SetFont(GetDocument()->m_lf);

	CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->GetObj();
	CMetadataTree *m_MetaTree=((CMetaDocument*)GetDocument())->GetTree();
	CString csObjTypeName=((CMetaDocument*)GetDocument())->GetObjName();
	if(m_MetaTree)
	{
		CString csTitleName;
		csTitleName=m_MetaTree->GetPath(m_MetaObj->nTempID);
		csTitleName=csTitleName.Mid(csTitleName.Find("\\")+1);
		csTitleName.Replace("\\"," : ");
		csTitleName.TrimLeft();
		csTitleName.TrimRight();
		GetDocument()->SetTitle(csTitleName);
		
	}
	if(m_MetaObj)
	{
		GetDocument()->SetText(m_MetaObj->csFile);
		if(!m_MetaObj->bCanSave)
		{
			GetDocument()->m_xTextBuffer.SetReadOnly(TRUE);
		}
	}
	


	if(GetDocument()->GetViewCount()>1)
		bSingleModule=0;


	((CModuleDoc*)GetDocument())->SetDefaultText();

	if(bSingleModule)
		((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_SAMPLETYPE );

	CAutocomplectionCtrl::OnInitialUpdate();

}



void CModuleView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if(AfxGetProperty())
		AfxGetProperty()->EndEdit();

	CAutocomplectionCtrl::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CModuleView::OnButtoncomment() 
{
	CPoint pStartSel,pEndSel;
	GetSelection(pStartSel,pEndSel);
	int nStartLine,nEndLine;
	nStartLine=pStartSel.y;
	nEndLine=pEndSel.y;
	if(pEndSel.x>0)
		nEndLine++;
	if(nEndLine>GetLineCount())
		nEndLine=GetLineCount();

	if(nStartLine==nEndLine)
	{
		nEndLine++;
	}

	m_pTextBuffer->BeginUndoGroup();
	int x,y;
	for(int nLC=nStartLine;nLC<nEndLine;nLC++)
	{
		m_pTextBuffer->InsertText(this, nLC, 0,"//",y,x, CE_ACTION_TYPING);
	}
	m_pTextBuffer->FlushUndoGroup(this);
}

inline int IsSpace3(int c)
{
	if(c<0)
		c=-c;
	if(c==32||c==9)
		return 1;
	else
		return 0;
}
void CModuleView::OnButtondelcomment() 
{
	CPoint pStartSel,pEndSel;
	GetSelection(pStartSel,pEndSel);
	int nStartLine,nEndLine;
	nStartLine=pStartSel.y;
	nEndLine=pEndSel.y;
	if(pEndSel.x>0)
		nEndLine++;
	if(nEndLine>GetLineCount())
		nEndLine=GetLineCount();

	if(nStartLine==nEndLine)
	{
		nEndLine++;
	}

	m_pTextBuffer->BeginUndoGroup();
	for(int nLC=nStartLine;nLC<nEndLine;nLC++)
	{
		CString Str = GetLineChars(nLC);
		int nLength = GetLineLength(nLC);
		for(int i=0;i<nLength;i++)
			if(!IsSpace3(Str.GetAt(i)))
			{
				if(Str.GetAt(i)=='/')
				if(i+1<nLength)
				if(Str.GetAt(i+1)=='/')
					m_pTextBuffer->DeleteText(this, nLC, i, nLC, i+2, CE_ACTION_TYPING);
				break;
			}
	}
	m_pTextBuffer->FlushUndoGroup(this);
}

int CModuleView::GotoProc(CString csObj,CString csName)
{
	csName.TrimLeft();
	csName.TrimRight();
	if(csName.IsEmpty())
		return 0;

	GetContextProc();

	int i=aElements.Find(csName);
	if(i>=0)
		{
			GotoLine(aElements[i].nLine);
			return 1;
		}
/*
	CString csUpperName=csName;
	csUpperName.MakeUpper();
	for(int i=0;i<aElements.GetSize();i++)
	{
		int n=aElements.Find();
		CString csCurName=aElements[i].sName;
		csCurName.MakeUpper();
		csCurName.TrimLeft();
		csCurName.TrimRight();
		if(csCurName==csUpperName)
		{
			GotoLine(aListLine[i]);
			return 1;
		}
	}
*/
	//создаем новую процедуру
	//ищем строку, в которую можно поместить процедуру
	int nInsert=0;
	for(int nLC=GetLineCount()-1;nLC>=0;nLC--)
	{
		CString Str = GetLineChars(nLC);
		int nLength = GetLineLength(nLC);
		Str.GetBuffer(nLength);
		Str.ReleaseBuffer(nLength);
		Str.TrimLeft();
		Str.TrimRight();
		Str.MakeUpper();
		Str.Replace("\t"," ");
		if(	Str.Left(6)=="ПЕРЕМ "||
			Str.Left(14)=="КОНЕЦПРОЦЕДУРЫ"||
			Str.Left(12)=="КОНЕЦФУНКЦИИ"||
			Str.Left(4)=="VAR "||
			Str.Left(12)=="ENDPROCEDURE"||
			Str.Left(11)=="ENDFUNCTION"
			)
		{
			nInsert=nLC+1;
			break;
		}
	}

CString csProc;
csProc.Format("\
\r\n\
///////////////////////////////////////////////////////////////////////\r\n\
//Обработка события объекта %s\r\n\
Процедура %s()\r\n\
\r\n\
КонецПроцедуры\r\n\
",csObj,csName);


	int x,y=0;
	if(nInsert>=m_pTextBuffer->GetLineCount())
	{
		//вставляем в конец
		nInsert=m_pTextBuffer->GetLineCount()-1;
		y=m_pTextBuffer->GetLineLength(nInsert);
	}
	m_pTextBuffer->InsertText(this, nInsert, y,csProc,y,x, CE_ACTION_TYPING);

	GotoLine(nInsert+3);
	return 0;
}

int CModuleView::GetContextProc()//CArray <CString,CString> &aListName,CArray <CString,CString> &aListDescription,CArray <UINT,UINT> &aListImage,CArray <UINT,UINT> &aListLine)//BOOL bGetVariable)
{
	int nCurLine=0;
	CPoint point=GetCursorPos();

	for(int nLC=0;nLC<GetLineCount();nLC++)
	{
		int nImage=0;
		CString Str = GetLineChars(nLC);
		int nLength = GetLineLength(nLC);
		Str.GetBuffer(nLength);
		Str.ReleaseBuffer(nLength);
		Str.TrimLeft();
		Str.TrimRight();
		while(Str.Replace("  "," "));
		nLength=Str.GetLength();
		CString Str0=Str;
		Str.MakeUpper();
		int nIndex=-1;
		nIndex=Str.Find("ФУНКЦИЯ");
		if(nIndex>=0)
		{
			CString csLeft=Str.Left(nIndex);
			
			if(!csLeft.IsEmpty())
				continue;
			nIndex=nIndex+7;
			nImage=351;
		}
		if(nIndex<0)
		{
			nIndex=Str.Find("ПРОЦЕДУРА");
			if(nIndex>=0)
			{
				CString csLeft=Str.Left(nIndex);
				
				if(!csLeft.IsEmpty())
					continue;
				nIndex=nIndex+9;
				nImage=352;
			}
		}
		if(nIndex<0)
		{
			nIndex=Str.Find("FUNCTION");
			if(nIndex>=0)
			{
				CString csLeft=Str.Left(nIndex);
				
				if(!csLeft.IsEmpty())
					continue;
				nIndex=nIndex+8;
				nImage=351;
			}
		}

		if(nIndex<0)
		{
			nIndex=Str.Find("PROCEDURE");
			if(nIndex>=0)
			{
				CString csLeft=Str.Left(nIndex);
				
				if(!csLeft.IsEmpty())
					continue;
				nIndex=nIndex+9;
				nImage=352;
			}
		}
		if(nIndex>=0)
		{
			int nIndex2=Str.Find("(",nIndex);
			if(nIndex2<0)
				nIndex2=nLength;

			int nIndex3;
			nIndex3=Str.Find(" ",nIndex+1);
			if(nIndex3>0)
				nIndex2=min(nIndex2,nIndex3);

			nIndex3=Str.Find("\t",nIndex+1);
			if(nIndex3>0)
				nIndex2=min(nIndex2,nIndex3);

			nIndex3=Str.Find(",",nIndex+1);
			if(nIndex3>0)
				nIndex2=min(nIndex2,nIndex3);

			nIndex3=Str.Find(";",nIndex+1);
			if(nIndex3>0)
				nIndex2=min(nIndex2,nIndex3);

			CString csName=Str0.Mid(nIndex,nIndex2-nIndex);
			CString csShortDescription=Str0.Mid(nIndex);
			int nRes=csShortDescription.ReverseFind('/');
			if(nRes>0)
			{
				if(csShortDescription.GetAt(nRes-1)=='/')//итак - это комментарий
				{
					csShortDescription=csShortDescription.Mid(nRes+1);
				}
			}
			else
			{
				nRes=csShortDescription.ReverseFind(')');
				csShortDescription=csShortDescription.Left(nRes+1);
			}

			csName.TrimLeft();
			csName.TrimRight();

			CModuleElementInfo data;
			data.csName=csName;
			data.csDescription=csShortDescription;
			data.nImage=nImage;
			data.nLine=nLC;

			aElements.Add(data);
			if(point.y>=nLC)//текущая позиция
				nCurLine=aElements.GetSize()-1;
		}
	}
	return nCurLine;
}
void CModuleView::OnButtonproc() 
{
	aElements.RemoveAll();
	int nCurLine=GetContextProc();//aListName,aListDescription,aListImage,aListLine);

	CFunctionList dlg;
	dlg.nLine=nCurLine;
	for(int i=0;i<aElements.GetSize();i++)
	{
		dlg.aListName.Add(aElements[i].csName);
		dlg.aListImage.Add(aElements[i].nImage);

	}

	int nRes=dlg.DoModal();
	if(nRes==IDOK)
	if(dlg.nLine>=0&&dlg.nLine<aElements.GetSize())
	{
		GotoLine(aElements[dlg.nLine].nLine);
	}
}

void CModuleView::GotoLine(UINT nLine,BOOL bSetRunLine) 
{
	if(nLine>=GetLineCount())
		nLine=GetLineCount()-1;
	if(nLine<0)
		return;

	CPoint ptCursorPos,ptCursorPos2;
	ptCursorPos.x=0;
	ptCursorPos.y=nLine; 
	ptCursorPos2.x=GetLineLength(nLine);
	ptCursorPos2.y=nLine;

	if(bSetRunLine)
	{
		DelCurLine();
		nRunLine=nLine;
		m_pTextBuffer->SetLineFlag(nRunLine, LF_EXECUTION,1, FALSE);

		if(pDebugMessageBar)
			pDebugMessageBar->Refresh();
		if(pDebugStackBar)
			pDebugStackBar->Refresh();

	}
	else
	{
		SetSelection(ptCursorPos, ptCursorPos2);
	}

	SetCursorPos(ptCursorPos);
	SetAnchor(ptCursorPos);

	ptCursorPos.y-=10;
	EnsureVisible(ptCursorPos);
	ptCursorPos.y+=20;
	EnsureVisible(ptCursorPos);

}
void CModuleView::OnButtongotoline() 
{
	CLineInput dlg;
	int nRes=dlg.DoModal();
	if(nRes==IDOK)
	{
		GotoLine(dlg.nLine-1);
	}

}

void CModuleView::OnUpdateFormatBlock(CCmdUI* pCmdUI) 
{
	CPoint pStartSel,pEndSel;
	GetSelection(pStartSel,pEndSel);
	if(abs(pStartSel.y-pEndSel.y)>1)
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);

}

void CModuleView::OnHelpSearchCombo() 
{
	// TODO: Add your command handler code here
	
}

void CModuleView::SaveAsHtml(CString lpszPathName)
{
	CFile cf;
	cf.Open(lpszPathName,CFile::modeCreate | CFile::modeWrite);


	CString Str;
	Str="<HTML><HEAD><META CHARSET=\"windows-1251\"></META></HEAD><BODY>\r\n";
	cf.Write(Str,Str.GetLength());

	for(int nLC=0;nLC<GetLineCount();nLC++)
	{
		int nLength = GetLineLength(nLC);
		CString Str;

		TEXTBLOCK *pBuf = new TEXTBLOCK[sizeof(TEXTBLOCK) * nLength * 3];
		int nBlocks = 0;
		ParseLine(0, nLC, pBuf, nBlocks);

		LPCTSTR pszChars = GetLineChars(nLC);
		if (nBlocks > 0)
		{
			for (int I = 0; I < nBlocks ; I ++)
			{


				CString line;
				int nCount;
				if(I<nBlocks - 1)
					nCount=pBuf[I + 1].m_nCharPos - pBuf[I].m_nCharPos;
				else
					nCount=nLength - pBuf[I].m_nCharPos;

				ExpandChars(pszChars, pBuf[I].m_nCharPos, nCount, line);

				CString Str2=line;
				Str2.TrimRight();
				line.Replace(" ","&nbsp;");
				if(!Str2.IsEmpty())
				{

					UINT nColor=GetColor(pBuf[I].m_nColorIndex);
					UINT nColor1=nColor%256;
					UINT nColor2=(nColor%(256*256)/256)%256;
					UINT nColor3=(nColor/(256*256))%256;

					CString csFont;
					csFont.Format("<FONT color=#%02x%02x%02x>",nColor1,nColor2,nColor3);




					Str+=csFont;
					Str+=line;
					Str+="</FONT>";
				}
				else
				{
					Str+=line;
				}
			}
			if(pBuf)
				delete []pBuf;
		}
		Str+="<BR>\r\n";
		cf.Write(Str.GetBuffer(0),Str.GetLength());
		Str.ReleaseBuffer();
	}
	Str="</BODY></HTML>\r\n";
	cf.Write(Str,Str.GetLength());
	cf.Close();
}



HWND Get2C();
void ClearDebugPoint(CString csModule);
void AddDebugPoint(CString csModule,int nLine);
void SendDebugPoint(BOOL bPost);
void NextDebug(int);
void NextDebugOver(int);
void RunDebug();
CString GetVariable(CString Str1);
void StopDebug();

extern BOOL bDebugStopLine;
extern int nCurrentNumberStopContext;

void CModuleView::OnUpdateEditDebugPoint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(afxAppRunMode==CONFIG_MODE);
}

CString CModuleView::GetName() 
{
	CString csName;
	CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->GetObj();
	if(m_MetaObj)
		csName=m_MetaObj->csPrevFileName;
	if(csName.IsEmpty())
		csName=GetDocument()->GetPathName();
	return csName;
}
void CModuleView::OnEditDebugPoint() 
{
	if (m_pTextBuffer != NULL)
	{
		CPoint ptCursorPos=GetCursorPos();
		DWORD dwFlags = GetLineFlags(ptCursorPos.y);
		DWORD dwMask  = LF_BREAKPOINT;
		m_pTextBuffer->SetLineFlag(ptCursorPos.y, dwMask, (dwFlags & dwMask) == 0, FALSE);
		OnToggleBookmark();

		//Обновляем список точек останова
		CString csName=GetName();
		ClearDebugPoint(csName);
		for(int nLC=0;nLC<GetLineCount();nLC++)
		{
			dwFlags = GetLineFlags(nLC);
			if(dwFlags&dwMask)
			{
				AddDebugPoint(csName,nLC);
			}
		}
		SendDebugPoint(1);
	}
}


void CModuleView::DelCurLine()
{
	if(nRunLine>-1)
	{
		if(nRunLine<GetLineCount())
		{
			m_pTextBuffer->SetLineFlag(nRunLine, LF_EXECUTION,0, FALSE);
			nRunLine=-1;
		}
	}
}

void CModuleView::RunDebug()
{
	if(nRunLine>-1)
		DelCurLine();
	else
	if(!Get2C())
	{
		SetCurrentDirectory(csCurrentExeDir);
		ShellExecute(NULL,NULL,__argv[0],CString("/enterprise")+" /d"+csIBDir,NULL,SW_HIDE);
	}
	::RunDebug();
}

void CModuleView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CModuleView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CModuleView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CModuleView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnRButtonDown(nFlags, point);
}

void CModuleView::OnMouseMove(UINT nFlags, CPoint point) 
{
	static CPoint prevPointMove(0,0);
	if(!m_listbox.m_hWnd)//только если нет Intelli-sense
	if(prevPointMove!=point)
	{
		m_TitleTip.Hide();
		pointShowTitle=point;
		prevPointMove=point;
		SetTimer(MOUSE_SHOW_TIMER_ID,500,0);
	}

	CAutocomplectionCtrl::OnMouseMove(nFlags, point);
}

void CModuleView::OnLButtonDown(UINT nFlags, CPoint point0) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnLButtonDown(nFlags, point0);
}

BOOL CModuleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    m_TitleTip.Hide();  // hide any titletips
	return CAutocomplectionCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CModuleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    m_TitleTip.Hide();  // hide any titletips
	CAutocomplectionCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if(nChar==VK_F1)
	{
		CPoint ptCursorPos = GetCursorPos();
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		if(ptCursorPos.y>=0&&ptCursorPos.y<GetLineCount())
		{
			CString Str=GetCurWord(ptCursorPos,0);
			if(!Str.IsEmpty())
			{
				CValue	vDeleteText;
				CValue	vInsertText;
				int nRet=AfxCallInitProc("ПриКонтекстнойПомощи",String(Str),vDeleteText,vInsertText);
			}
		}

	}
}

void CModuleView::OnNextDebug() 
{
	if(nRunLine>-1)
	{
		NextDebug(nRunLine);
		DelCurLine();
		RunDebug();
	}
}
void CModuleView::OnNextDebugOver() 
{
	if(nRunLine>-1)
	{
		NextDebugOver(nRunLine);
		DelCurLine();
		RunDebug();
	}
}

void CModuleView::OnRunDebug() 
{
	if(nRunLine>-1)
	{
		DelCurLine();
		bDebugStopLine=0;
		nCurrentNumberStopContext=0;
		RunDebug();
		::BringWindowToTop(Get2C());
	}
}


void CModuleView::OnUpdateStopDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(Get2C()!=0&&nRunLine>-1);
}
void CModuleView::OnStopDebug() 
{
	if(nRunLine>-1)
	{
		StopDebug();
		RunDebug();
	}
}

void CModuleView::OnDestroy() 
{
	ClearDebugPoint(GetName());

	if(bSingleModule)
		((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
	CAutocomplectionCtrl::OnDestroy();
}

void CModuleView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CAutocomplectionCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CModuleView::OnTimer(UINT nIDEvent) 
{
	if(MOUSE_SHOW_TIMER_ID==nIDEvent||LINE_SHOW_TIMER_ID==nIDEvent)
	{
		KillTimer(nIDEvent);
		m_TitleTip.Hide();

		CString csCurWord;
		if(!m_listbox.m_hWnd)//только если нет Intelli-sense
		{
			CPoint ptText;
			if(MOUSE_SHOW_TIMER_ID==nIDEvent)//определяем текущую позицию по положению курсора мышки
			{
				CPoint pointNewPos;
				::GetCursorPos(&pointNewPos);
				ScreenToClient(&pointNewPos);
				if(pointNewPos!=pointShowTitle)
					return;
				ptText=ClientToText(pointShowTitle); 
			}
			else//текущую позицию определяем по текущему курсору каретки ввода
			{
				ptText=GetCursorPos();
				pointShowTitle=TextToClient(ptText);
				ptText.x--;
			}
			if(nRunLine>-1)//вывод значения переменной
			{
				csCurWord=GetCurWord(ptText,1);//получаем слово с учетом точки
			}
			else
			{
				csCurWord=GetCurWord(ptText,0);

				if(csCurWord.IsEmpty())
					return;

				BOOL bRes=0;
				ptText.x--;//с учетом точки
				if(ptText.x>0)
				{
					
					bRes=LoadIntelliList(ptText,0,1);
					if(bRes)//это объект - загружаем методы текущего объекта
					{
						csCurWord=aElements.GetDesription(csCurWord);
						//csCurWord=(char*)aListDescription[mUpper(csCurWord)];
					}
				}

				if(bRes==0)//это функция
				{
					LoadContextData(1);
					void *p;
					if(aHelpDescription.Lookup(mUpper(csCurWord),p))
						csCurWord=(char*)aHelpDescription[mUpper(csCurWord)];//системная функция
					else
						csCurWord=aElements.GetDesription(csCurWord);//локальный объект (переменная, функция...)
						//csCurWord=(char*)aListDescription[mUpper(csCurWord)];
				}
			}
		}
		else
		{
			//void *p;
			if(aElements.Find(csCurTitleName)>=0)
				//csCurWord=(char*)aListDescription[mUpper(csCurTitleName)];
				csCurWord=aElements.GetDesription(csCurTitleName);
			else
				csCurWord=(char*)aHelpDescription[mUpper(csCurTitleName)];
		}

		if(csCurWord.IsEmpty())
			return;

		CPoint point=pointShowTitle;
		CRect Rect;
		point.y+=20;
		Rect.SetRect(point,point);
		Rect.bottom+=18;
		Rect.right+=100;
		LOGFONT LogFont;
		GetFont()->GetLogFont(&LogFont);
		

		m_TitleTip.SetParentWnd(this);
		if(nRunLine>-1)//программа остановлена - вывод значения переменной
		{
			CString Str=GetVariable(csCurWord);
			if(!Str.IsEmpty())
				m_TitleTip.Show(Rect,Str,0,Rect,&LogFont);
		}
		else//вывод типа переменной или описание функции, метода...
		{
			m_TitleTip.Show(Rect,csCurWord,0,Rect,&LogFont);
		}
		return;
	}
	
	CAutocomplectionCtrl::OnTimer(nIDEvent);
}

int GotoLine(CString Str,int nLine,BOOL bRun);

void CModuleView::OnEditGoto() 
{
	if(nGotoLine>=0)
	{
	    m_TitleTip.Hide();  // hide any titletips
		if(csGotoModuleName.IsEmpty())
			GotoLine(nGotoLine);
		else
			::GotoLine(csGotoModuleName,nGotoLine+1,0);
	}
}
void CModuleView::AddGotoInfo(CMenu* pPopup)
{
	nGotoLine=-1;
	csGotoModuleName="";
	CPoint ptText=GetCursorPos();
	CString csCurWord=GetCurWord(ptText,0);
	if(!csCurWord.IsEmpty())
	{
		BOOL bRes=0;
		ptText.x--;//с учетом точки
		if(ptText.x>0)
			bRes=LoadIntelliList(ptText,0,1);
		if(bRes==0)//это функция
			LoadContextData(1); 

		nGotoLine=aElements.GetLine(csCurWord);
		csGotoModuleName=aElements.GetModuleName(csCurWord);


		if(nGotoLine>=0)
		{
			pPopup->RemoveMenu(ID_EDIT_GOTO,MF_BYCOMMAND|MF_ENABLED );
			pPopup->AppendMenu (MF_BYCOMMAND|MF_ENABLED,ID_EDIT_GOTO,CString ("Перейти в ")+csCurWord);
		}
	}
}

void CModuleView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	CMenu* pPopup;
	VERIFY(menu.LoadMenu(IDR_ALL_EDIT_MENU));
	pPopup = menu.GetSubMenu(0);
	pPopup->EnableMenuItem(ID_EDIT_CUT,MF_BYCOMMAND|MF_ENABLED );
	pPopup->EnableMenuItem(ID_EDIT_COPY,MF_BYCOMMAND|MF_ENABLED );
	pPopup->EnableMenuItem(ID_EDIT_PASTE,MF_BYCOMMAND|MF_ENABLED );


	//Goto
	AddGotoInfo(pPopup);

	CWnd* pWndPopupOwner = this;//GetParent();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);

}

void CModuleView::OnKillFocus(CWnd* pNewWnd) 
{
	CAutocomplectionCtrl::OnKillFocus(pNewWnd);
	
	m_TitleTip.Hide();
	
}




CString GetIDName(CString csStr)
{
	CString csResult;
	csResult=mUpper(csStr.GetAt(0));
	int nLastSpace=-1;
	for(int i=1;i<csStr.GetLength();i++)
	{
		int nChar=csStr.GetAt(i);
		if(nChar==' ')//пробел
		{
			nLastSpace=i;
			continue;
		}
		if(nLastSpace+1==i)//первый сивол после пробела
			csResult+=mUpper(nChar);
		else
			csResult+=nChar;
	}
	return csResult;
}
CString GetNormalProcName(CString Str,CString csEventName,CString csIdName)
{
	if(Str.IsEmpty())
	{
		Str=csEventName+" "+csIdName;
		Str.Replace("."," ");
		Str=GetIDName(Str);
	}
	int nIndex=Str.Find("(");
	if(nIndex>0)
		Str=Str.Left(nIndex);
	Str.Replace("."," ");
	Str.Replace(" ","");
	return Str;
}
int GotoModuleProc(CString csIdName,CString Str)
{
	if(!Str.IsEmpty())
	if(AfxGetProperty())
	if(AfxGetProperty()->pDocUpdate)
	if(AfxGetProperty()->pDocUpdate->IsKindOf(RUNTIME_CLASS(CModuleDoc)))
	{
		CModuleDoc *pDoc=(CModuleDoc*)AfxGetProperty()->pDocUpdate;
		CMyCodeView *pModule=pDoc->GetModule();

		pModule->GotoProc(csIdName,Str);

		CChildFrame *pFrame=(CChildFrame *)pModule->GetParentFrame();
		if(pFrame->GetRuntimeClass())
			if(CString(pFrame->GetRuntimeClass()->m_lpszClassName)=="CChildFrame")
			{
				pFrame->m_TabViewContainer.SetActivePageIndex(1);
				return 1;
			}
	}
	return 0;
}



