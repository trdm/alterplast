// Parser.cpp: implementation of the CParser class.

#include "stdafx.h"
#include "telepat.h"
#include "Parser.h"
#include "tentrydrawing.h"
#include "txttrap.h"
#include "typegroup.h"
#include "option.h"
#include "MetaGroup.h"
#include "tform.h"
#include "telelist.h"
#include "paramwnd.h"
#include "stringparser.h"
#include "TemplateInfo.h"
#include "plugin.h"

CKeyWords	CParser::m_kwords;
CParser*	CParser::m_GM=NULL;

CParser::CParser(CTextDocument* pDoc)
{
	m_dwRef=0;
	m_pDocument=pDoc;
	m_pMetaMethods	=NULL;
	m_pForm=NULL;
	m_LastParsedFrom=m_LastParsedTo=-1;
	if(pDoc)
	{
		CString type;
		m_ID=CConfigCont::DocumentToID(m_pDocument,type);
		if(m_ID>1)
		{
			m_pMetaMethods=CTypeGroup::GetTypeGroup(CTypeGroup::ConfType2TType(type));
		}
		POSITION pos=m_pDocument->GetFirstViewPosition();
		if(pos)
		{
			CView* pView=m_pDocument->GetNextView(pos);
			if(pView)
			{
				CFrameWnd* pFrame=pView->GetParentFrame();
				if(!strcmp("CWorkBookFrame",pFrame->GetRuntimeClass()->m_lpszClassName))
				{
					if(m_ID<1)
						m_pMetaMethods=CTypeGroup::GetTypeGroup("ExtReport");
					CWorkBookDoc* pWB=(CWorkBookDoc*)pFrame->GetActiveDocument();
					m_pForm=new CTForm(this,pWB);
				}
			}
		}
	}
	else
	{
		m_GM=this;
		m_dwRef=0xFFFF;
	}

	m_Conts[Methods]	= new CProcCont(this);
	m_Conts[ModuleVars]	= new CVarCont(this);
	m_Conts[LocalVars]	= new CLocalVarCont(this);
	m_Conts[MetaAttribs]= new CMetaGroup(this);

	m_MethParser.m_pParser=this;
	m_MethParser.m_lexer=&m_lexParser;
	m_NeedQuick=true;
	m_state=0;
}

CParser::~CParser()
{
	for(int i=0;i<typeLast;i++)
		delete m_Conts[i];
	if(m_pForm)
		delete m_pForm;
	if(m_pDocument)
		m_pDocument->m_dwRef=1;
}

void CParser::FillGroup(CPtrArray& arr,int line,int col)
{
	// Сбросим все флаги и допустимые категории объектов
	m_kwords.m_allowedWords=0;
	m_flags=0;
	for(int i=0;i<typeLast;i++)
		m_AllowedConts[i]=0;
	int iStartMethodLine=GetMethodText(line,col);
	if(iStartMethodLine!=m_LastParsedFrom)
		ParseModul(line,col);
	m_LastParsedFrom=iStartMethodLine;
	ParseMethod(line,col);
	if(m_curLexem.type==_date_1C)
		return;

	for(i=0;i<typeLast;i++)
	{
		if(m_AllowedConts[i])
			m_Conts[i]->FillGroup(arr);
	}

	if(m_AllowedConts[Methods])
	{
		CTypeGroup* pTypeGroup=CTypeGroup::GetTypeGroup("Global");
		pTypeGroup->FillGroup(arr);
		if(m_pMetaMethods)
			m_pMetaMethods->FillGroup(arr);
	}
	if(!IsGM())
	{
		if(m_AllowedConts[Methods])
			m_GM->m_Conts[Methods]->FillGroup(arr);
		if(m_AllowedConts[ModuleVars])
		{
			m_GM->m_Conts[ModuleVars]->FillGroup(arr);
			if(m_pForm)
				m_pForm->FillGroup(arr);
		}
	}
	CParser::m_kwords.FillGroup(arr);
	CTemplateInfo::m_TemplateInfo.FillGroup(arr);
}

void CParser::ParseModul(int line,int col)
{
	CTextDocument* pDoc=GetDocument();
	if(pDoc)
	{
		if(line>-1)
		{
			CPoint ptStart(0,0),ptEnd(col,line);
			pDoc->SetSel(ptStart,ptEnd);
			pDoc->GetSelectedText(m_text);
			int len=pDoc->GetLineLength(line);
			if(line!=pDoc->GetLineCount()-1)
				len-=2;
			if(len<col)
			{
				CString space;
				while(len++<col)
					space+=' ';
				m_text+=space;
			}
		}
		else
			pDoc->GetText(m_text);
	}
	m_lexParser.SetSource(m_text,m_text.GetLength());
	m_state=1;
	m_MethParser.Parse(CMethodParser::ParseModul);
	m_LastParsedTo=line;
}

void CParser::ParseMethod(int line,int col)
{
	m_lexParser.SetSource(m_textMethod,m_textMethod.GetLength(),NULL,
		m_LastParsedFrom);
	if(!IsGM() && (!m_GM->m_state || m_GM->m_LastParsedTo>-1))
		m_GM->ParseModul();
	m_MethParser.Parse(CMethodParser::ParseMethod);
}

void CParser::ParseParams(CTxtTrap *pEditor)
{
	if(!CTeleList::m_pOneList->IsWindowVisible())
	{
		CPoint pt=pEditor->GetCaretPos();
		int iStartMethodLine=GetMethodText(pt.y,pt.x);
		if(iStartMethodLine!=m_LastParsedFrom)
			ParseModul(pt.y,pt.x);
		if(!IsGM() && (!m_GM->m_state || m_GM->m_LastParsedTo>-1))
			m_GM->ParseModul();
		m_LastParsedFrom=iStartMethodLine;
	}
	m_lexParser.SetSource(m_textMethod,m_textMethod.GetLength(),NULL,
		m_LastParsedFrom);
	m_MethParser.Parse(CMethodParser::ParseParam);
	if(m_MethParser.m_ParamsMethod.pStart)
	{
		if(CParamWnd::GetParamWnd()->Prepeare(pEditor))
			CParamWnd::GetParamWnd()->Show(pEditor);
	}
	else
		CParamWnd::GetParamWnd()->Hide();

}

#pragma optimize ("agt",on)
int CParser::GetMethodText(int line,int col)
{
	m_textMethod.Empty();
	CTextDocument* pDoc=GetDocument();
	if(!pDoc)
		return -1;
	static CLexer lexer;
	int tLen=0;
	int len;

	CString tLine;
	CStringArray strarr;
	SLexem lexem;
	CPoint ptStart(0,line),ptEnd(col,line);
	pDoc->SetSel(ptStart,ptEnd);
	pDoc->GetSelectedText(tLine);
	int space=col-tLine.GetLength();

	register LPCTSTR pStartLine;
	LPCTSTR pStartBlock;
	LPCTSTR pEndLine;

	while(line>0 || ptEnd.x>0)
	{
		ptStart.y	= line-100;
		if(ptStart.y<0)
			ptStart.y=0;
		pDoc->SetSel(ptStart,ptEnd);
		pDoc->GetSelectedText(tLine);
		len=tLine.GetLength();
		pStartBlock=tLine,pStartLine=pStartBlock+len-1,pEndLine;
		while(*pStartLine)
		{
			while(*pStartLine)
			{
				if(*(BYTE*)pStartLine>32)
					break;
				if(*pStartLine=='\r')
					line--;
				pStartLine--;
			}
			if(!*pStartLine)
				break;

			pEndLine=pStartLine--;
			while(*(BYTE*)pStartLine>31 || *pStartLine==9)
				pStartLine--;
			lexer.SetSource(pStartLine+1,pEndLine-pStartLine);
			do{
				lexer.NextLexem(lexem);
				if(CMethodParser::m_Lexem2TokenFast[lexem.type]>0)
				{
					len-=++pStartLine-pStartBlock;
					strarr.Add(CString(pStartLine,len));
					tLen+=len;
					goto fnd;
				}
			}while(lexer.m_pCurPos<pEndLine);
		}
		strarr.Add(tLine);
		tLen+=len;
		ptEnd.y		= line;
		ptEnd.x=0;
	}
fnd:
	LPTSTR pWrite=m_textMethod.GetBuffer(tLen+space);
	int s=strarr.GetSize();
	while(s>0)
	{
		tLine=strarr[--s];
		len=tLine.GetLength();
		memcpy(pWrite,(LPCTSTR)tLine,len);
		pWrite+=len;
	}
	for(int i=0;i<space;i++)
		*pWrite++=' ';
	*pWrite=0;
	m_textMethod.ReleaseBuffer(tLen+space);
	return line;
}

CTextDocument* CParser::GetDocument()
{
	if(!m_pDocument)
	{
		CString path;
		CModuleCont* pCont=NULL;
		CConfigCont::IDToTextModule(1,"ModuleText",path,&pCont,0);
		if(pCont)
			return (CTextDocument*)pCont->GetTextDocument();
	}
	return m_pDocument;
}

void CParser::QuickParse(CTxtTrap *pEditor,BYTE curChar)
{
	/*
	Варианты
	В комментариях не работаем
	В строках - проверка на мета-имена
	проверка на автозамены

	( ) ,	- обновить окно параметров
	.		- проверить на метод/атрибут объекта
	символ	- проверить на автоактивацию
	*/
	int acnum=COption::m_pOption->GetACompleteSymbols();
	CPoint pt=pEditor->GetCaretPos();
	CTextDocument* pDoc=pEditor->GetDocument();
	SLexem lexem;
	if(curChar=='\r' && pt.y>0)
	{
		if(AnalizeLine(lexem,pDoc,pt.y-1,pDoc->GetLineLength(pt.y-1)-2)==-2)
		{
			CString line;
			pDoc->GetLine(pt.y,line);
			pt.x=0;
			LPCTSTR ptr=line;
			while(*ptr)
			{
				if(*ptr==' ' || *ptr=='\t')
					pt.x++;
				else
					break;
				ptr++;
			}
			pDoc->InsertStr("|",pt,1);
			pt.x++;
			pEditor->MoveCaret(pt,pt,1);
		}
		m_NeedQuick=true;
		return;
	}
	if(curChar==27 && CParamWnd::GetParamWnd()->IsWindowVisible())
	{
		CParamWnd::GetParamWnd()->Hide();
		return;
	}
	int mode=AnalizeLine(lexem,pDoc,pt.y,pt.x);
	if(mode>=0)
	{
		if(pEditor->AutoReplace(curChar))
			return;
	}
	CTxtTrap::m_ParsedView=pEditor;
	if(mode<=0 && mode>-2 && curChar!='.')
	{
		m_NeedQuick=true;
		if(*lexem.pStart=='(' || *lexem.pStart==')' || *lexem.pStart==',')
			CParamWnd::GetParamWnd()->QuickActivate(pEditor);
		return;
	}
	if(m_NeedQuick)
	{
		if(!acnum && curChar!='.')
			return;
		if(curChar=='.' || (lexem.len>=acnum && CBLMap::m_LowerTable[curChar]!=' '))
			pEditor->PostMessage(WM_COMMAND,ID_QUICK_AUTOCOMPLETE);
	}
	else
	{
		if(curChar=='.')
			pEditor->PostMessage(WM_COMMAND,ID_QUICK_AUTOCOMPLETE);
		else if(CBLMap::m_LowerTable[curChar]==' ')
			m_NeedQuick=true;
	}
}
/*
				Быстрый			Обычный
Коментарий		-				+
Строка			+				+
идентификатор	+				+
( ) ,			параметры		+
*/
void CParser::AutoComplete(CTxtTrap *pEditor)
{
	if(CTeleList::m_pOneList->IsWindowVisible())
		return;
	CTextDocument* pDoc=pEditor->GetDocument();
	if(pDoc->IsReadOnly() || !pDoc->IsModule())
		return;
	CTxtTrap::m_ParsedView=pEditor;
	CTxtTrap::m_ParsedDoc=pDoc;
	CPoint pt=pEditor->GetCaretPos();
	SLexem lexem;
	int mode=AnalizeLine(lexem,pDoc,pt.y,pt.x);
	if(mode==-2)
	{
		ParseString(lexem);
		return;
	}
	CPtrArray arr;
	FillGroup(arr,pt.y,pt.x);

	CStringArray strarr;
	if(m_MethParser.m_IsMember)
	{
		CPlugin::m_pPlugin->FireMemberList(pt.y, pt.x-m_curLexem.len-1, strarr);
	}
	int s=arr.GetSize(),memb=strarr.GetSize();
	if(s || memb)
	{
		static CTeleGroup membGr;
		membGr.m_Image=IDI_STRINGS_;
		membGr.ClearEntries();
		for(int i=0;i<memb;i++)
		{
			CTeleEntry* pEntry=new CTeleEntry(strarr[i]);
			membGr.AddEntry(pEntry);
		}
		if(memb)
		{
			arr.Add(&membGr);
			s++;
		}
		CTeleList::m_pOneList->ResetContent();
		for(i=0;i<s;i++)
			CTeleList::m_pOneList->AddTGroup((CTeleGroup*)arr[i]);

		CTeleList::m_pOneList->SetBufer(m_curLexem);
		CTeleList::m_pOneList->FilterList(m_curLexem);
		if(CTeleList::m_pOneList->GetCount())
		{
			CTeleList::m_pOneList->Prepare(pEditor);
			CTeleList::m_pOneList->ShowList();
		}
	}
	m_NeedQuick=false;
}

int CParser::AnalizeLine(SLexem& lexem,CTextDocument* pDoc,int lineNum,int colNum)
{
	// Возвращает -1 в комментарии, -2 в строке, 0 в числах датах
	static CString line;
	CPoint ptStart(0,lineNum),ptEnd(colNum,lineNum);
	pDoc->SetSel(ptStart,ptEnd);
	pDoc->GetSelectedText(line);
	int len=line.GetLength();
	if(len<colNum)
	{
		while(len++<colNum)
			line+=' ';
	}
	m_lexParser.SetSource(line,line.GetLength());
	lexem.type=0;
	while(1)
	{
		m_lexParser.NextLexem(lexem);
		if(m_lexParser.m_pCurPos==m_lexParser.m_pEndPos)
			break;
	}
	if(lexem.type<=STATE_NAME || lexem.type==STATE_EOF)
		return lexem.type;
	if(lexem.type==STATE_REMARK)
	{
		lexem.pStart+=lexem.len-1;
		lexem.len=0;
		while(CBLMap::m_LowerTable[(BYTE)*lexem.pStart]!=' ')
			lexem.pStart--,lexem.len++;
		lexem.pStart++;
		return -1;
	}
	if(lexem.type==STATE_QUOT1)
	{
		lexem.pStart+=lexem.len-1;
		lexem.len=1;
		while(CBLMap::m_LowerTable[(BYTE)*lexem.pStart]!=' ' || *lexem.pStart=='.')
			lexem.pStart--,lexem.len++;
		lexem.pStart++,lexem.len--;
		return -2;
	}
	return 0;
}

void CParser::ParseString(CString str)
{
	CStringParser sp;
	sp.ParseString(str,	CTxtTrap::m_ParsedView);
}

void GetGMMethods(CStringArray& arr)
{
	arr.RemoveAll();
	if(CParser::m_GM)
	{
		CProcCont* pProc = (CProcCont*)CParser::m_GM->m_Conts[CParser::Methods];
		int s = pProc->GetCount();
		arr.SetSize(s);
		for(int i = 0; i < s ; i++)
			arr[i] = pProc->GetProc(i)->name;
	}
}

typedef void(*PGETGMM)(CStringArray&);
HINSTANCE hInst = GetModuleHandle("telepat.dll");
if(hInst)
{
	PGETGMM func = (PGETGMM)GetProcAddress(hInst, "GetGMMeths");
	if(func)
	{
		CStringArray gmMeths;
		func(gmMeths);
	}
}