// CodeJump.cpp: implementation of the CCodeJump class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "telecont.h"
#include "parser.h"
#include "CodeJump.h"
#include "txttrap.h"
#include "cjstack.h"


CCodeJump* CCodeJump::m_OneCodeJump;
CMapStringToPtr CCodeJump::m_TypesOfModuls;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const char* ModName[]={
	"ModuleText",
	"Transact",
	"CalcAlg",
	"UserHelp"
};

CCodeJump::CCodeJump()
{
	m_CurPos=0;
	m_TopPos=-1;
	for(int i=0;i<4;i++)
		m_TypesOfModuls[ModName[i]]=NULL;
}

CCodeJump::~CCodeJump()
{
}

void CCodeJump::Jump(CTxtTrap *pEditor)
{
	CString word;
	pEditor->GetCurrentWord(word);
	if(word.IsEmpty())
		return;
	CPoint StartPoint=pEditor->GetCaretPos();
	CParser* pMod=pEditor->GetMyParser();
	CTxtTrap::m_ParsedDoc=pEditor->GetDocument();
	
	int line=-1;
	while(1)
	{
		pMod->ParseModul();
		SProcInfo* pInfo=((CProcCont*)pMod->m_Conts[CParser::Methods])->GetProc(word);
		if(pInfo)
			line=pInfo->start.line;
		else
		{
			SVarInfo* pInfo=((CVarCont*)pMod->m_Conts[CParser::ModuleVars])->GetVar(word);
			if(pInfo)
				line=pInfo->name.line;
			/*else
			{
				SLocalVarInfo* pInfo=((CLocalVarCont*)pMod->m_Conts[CParser::LocalVars])->GetVar(word);
				if(pInfo)
					line=pInfo->name.line;
			}*/
		}
		if(line<0)
		{
			if(pMod->IsGM())
				break;
			else
				pMod=CParser::m_GM;
		}
		else
			break;
	};
	if(line<0)
		return;

	SaveCurrentPos(pEditor);
	
	m_TopPos=m_CurPos++;

	CTextDocument* pDoc=CTxtTrap::m_ParsedDoc;
	if(pMod!=pEditor->GetMyParser())
	{
		pDoc=NULL;
		CString path;
		CModuleCont* pCont=NULL;
		CConfigCont::IDToTextModule(1,"ModuleText",path,&pCont,FALSE);
		if(pCont)
		{
			pCont->ShowDocument("ModuleText");
			pDoc=(CTextDocument*)pCont->GetTextDocument();
		}
	}
	if(pDoc)
		JumpToLine(pDoc,line);
	CTxtTrap::m_ParsedDoc=NULL;
}

CCodeJump* CCodeJump::GetCodeJump()
{
	if(!m_OneCodeJump)
		m_OneCodeJump=new CCodeJump;
	return m_OneCodeJump;
}

void CCodeJump::Back()
{
	if(m_CurPos<1)
		return;
	SaveCurrentPos(NULL);
	Jump(--m_CurPos);
}

void CCodeJump::Forward()
{
	if(m_CurPos>=m_TopPos)
		return;
	SaveCurrentPos(NULL);
	Jump(++m_CurPos);
}

void CCodeJump::ShowStack()
{
	CCJStack::ShowStack();
}

void CCodeJump::Jump(int pos)
{
	if(pos<0 || pos>m_TopPos)
		return;
	SBackState* back=(SBackState*)m_stack[pos];

	CWnd* pWnd=CWnd::FromHandlePermanent(back->hWnd);
	CDocument *pDoc=NULL;
	if(pWnd)
		pDoc=((CFrameWnd*)pWnd)->GetActiveDocument();
	else
	{
		if(back->modulID>=0)
		{
			void* val;
			if(m_TypesOfModuls.Lookup(back->data,val))
			{
				CString path;
				CModuleCont* pCont=NULL;
				CConfigCont::IDToTextModule(back->modulID,back->data,path,&pCont,FALSE);
				if(pCont)
				{
					pCont->ShowDocument(back->data);
					pDoc=(CTextDocument*)pCont->GetTextDocument();
				}
			}
			else
			{
				CTypedCont* pCont=NULL;
				CConfigCont::IDToPath(back->modulID,back->data,back->data,&pCont,FALSE);
				if(pCont)
				{
					pCont->ShowDocument(back->modulID);
					pDoc=pCont->GetWorkBook();
				}
			}
		}
		else
		{
			if(!back->data.IsEmpty())
				pDoc=pConfSvc->OpenFile(back->data);
		}
	}
	if(pDoc)
	{
		if(!strcmp("CWorkBookDoc",pDoc->GetRuntimeClass()->m_lpszClassName))
		{
			((CWorkBookDoc*)pDoc)->SwitchToPage(1);
			pDoc=((CWorkBookDoc*)pDoc)->GetTextDocument();
		}
		if(pDoc)
			Jump((CTextDocument*)pDoc,back->point,back->point);
	}
	else
	{
		int size=m_stack.GetSize();
		for(int i=pos;i<size-1;i++)
			m_stack[i]=m_stack[i+1];
		m_stack.RemoveAt(size-1);
		m_TopPos--;
		Jump(pos);
	}
}

void CCodeJump::Jump(CTextDocument *pDoc, CPoint pt1, CPoint pt2)
{
	if(pDoc)
	{
		POSITION pos=pDoc->GetFirstViewPosition();
		if(pos)
		{
			CTextEditor* pEditor=(CTextEditor*)pDoc->GetNextView(pos);
			if(pEditor)
			{
				CFrameWnd* pFrame=pEditor->GetParentFrame();
				if(pFrame)
				{
					pFrame->ActivateFrame();
					pEditor=(CTextEditor*)pFrame->GetActiveView();
					if(pEditor)
					{
						pEditor->SetFocus();
						pEditor->MoveCaret(pt1,pt2,TRUE);
					}
				}
			}
		}
	}
}

void CCodeJump::JumpFromTo(CTxtTrap* pEditor,CTextDocument* pDoc,int line)
{
	SaveCurrentPos(pEditor);
	m_TopPos=m_CurPos++;
	JumpToLine(pDoc,line);
}

void CCodeJump::SaveCurrentPos(CTxtTrap *pEditor)
{
	if(!pEditor)
		pEditor=CTxtTrap::GetCurrentView();
	if(!pEditor)
		return;
	SBackState* back;
	int size=m_stack.GetSize();
	if(m_CurPos<size)
		back=(SBackState*)m_stack[m_CurPos];
	else
	{
		back=new SBackState;
		m_stack.Add(back);
	}
	if(m_CurPos>m_TopPos)
		m_TopPos=m_CurPos;
	CTextDocument* pDoc=pEditor->GetDocument();
	back->point=pEditor->GetCaretPos();
	back->word.Empty();
	for(int i=back->point.y-2;i<back->point.y+3;i++)
	{
		if(i<0 || i>=pDoc->GetLineCount())
			continue;
		CString line;
		pDoc->GetLine(i,line);
		line.TrimRight("\r\n\t ");
		int len=line.GetLength();
		CString newLine;
		for(int k=0,j=0;k<len;k++)
		{
			if(i==back->point.y && k==back->point.x)
			{
				newLine+='¦';
				j++;
			}
			char symb=line[k];
			if(symb=='\t')
			{
				for(int c=j%4;c<4;c++)
				{
					newLine+=' ';
					j++;
				}
			}
			else
			{
				newLine+=symb;
				j++;
			}
		}
		if(i==back->point.y && len<=back->point.x)
			newLine+='¦';
		if(!back->word.IsEmpty())
			back->word+='\n';
		back->word+=newLine;
	}
	back->word.TrimRight("\r\n");
	back->modulID=CConfigCont::DocumentToID(pDoc,back->data);
	if(back->modulID<0)
	{
		back->data=pDoc->GetPathName();
		back->title=pEditor->GetParentFrame()->GetActiveDocument()->GetTitle();
	}
	else
		back->title=GetModuleFullName(back->modulID,back->data,1,pMetaDataCont);
	back->hWnd=pEditor->GetParentFrame()->m_hWnd;
}

void CCodeJump::JumpToLine(CTextDocument *pDoc, int line)
{
	CPoint pt1,pt2;
	pt1.y=pt2.y=line;
	pt1.x=0;
	pt2.x=pDoc->GetLineLength(line);
	if(line!=pDoc->GetLineCount()-1)
		pt2.x-=2;
	Jump(pDoc,pt1,pt2);
}
