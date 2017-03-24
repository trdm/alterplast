// proccont.cpp
#include "stdafx.h"
#include "parser.h"
#include "tentrydrawing.h"
#include "txttrap.h"
#include "paramwnd.h"
#include "telelist.h"

void GetComments(CString& txt,SLexem& startLexem)
{
	txt.Empty();
	const BYTE* pLine=(const BYTE*)startLexem.pStart;
	LPCTSTR pEndLine;
	int currentLine=startLexem.line;
	int lineCount=0;
	while(currentLine>0)
	{
		currentLine--;
		while(*pLine>31 || *pLine==9)
			pLine--;
		pEndLine=(LPCTSTR)pLine+1;
		pLine-=2;
		while(*pLine>31 || *pLine==9)
			pLine--;
		pLine++;

		bool Found=false;
		LPCTSTR ptr=(LPCTSTR)pLine;
		int state=0;
		LPCTSTR pStart=NULL;
		while(*ptr!='\r')
		{
			char symb=*ptr;
			switch(state)
			{
			case 0:
				if(symb=='/')
					state=1;
				break;
			case 1:
				if(symb=='/')
				{
					Found=true;
					state=2;
				}
				else
					state=0;
				break;
			case 2:
				if(!(symb=='/' || symb=='-' || symb=='*' || symb=='_' || symb==' '))
				{
					state=3;
					pStart=ptr;
				}
			}
			ptr++;
		}
		if(pStart)
		{
			txt.Insert(0,CString(pStart,pEndLine-pStart));
			lineCount++;
		}
		else if(!Found)
			break;
	}
	txt.Replace("\r\n","\n");
	txt.Replace("\t","  ");
	if(lineCount>5)
	{
		lineCount=5;
		LPCTSTR ptr=txt;
		while(lineCount)
		{
			while(*ptr++!='\n');
			lineCount--;
		}
		txt=txt.Left(ptr-(LPCTSTR)txt);
		txt+="....";
	}
}

const int	CProcCont::GetGroupCount() const
{
	return 4;
}

const int*	CProcCont::GetImagesNum()  const
{
	// 0 - proc
	// 1 - func
	// 2 - proc export
	// 3 - func export
	static const img[2][4]={
		{IDI_GMMETPRIV_,IDI_GMMETPRIV_,IDI_GMMETEXP_,IDI_GMMETEXP_},
		{IDI_CMMETPRIV_,IDI_CMMETPRIV_,IDI_CMMETEXP_,IDI_CMMETEXP_}
	};
	return m_pParser->IsGM()?img[0]:img[1];
}

LPCTSTR*	CProcCont::GetGroupNames() const
{
	static LPCTSTR names[2][4]={
		{"Глобальный модуль","Глобальный модуль","Глобальный модуль","Глобальный модуль"},
		{"Текущий модуль","Текущий модуль","Текущий модуль","Текущий модуль"}
	};
	return m_pParser->IsGM()?names[0]:names[1];
}

void CProcCont::Clear()
{
	for(int i=0;i<m_Proc.GetSize();i++)
		delete (SProcInfo*)m_Proc[i];
	for(i=0;i<m_Forwards.GetSize();i++)
		delete (SProcInfo*)m_Forwards[i];
	m_Proc.RemoveAll();
	m_Forwards.RemoveAll();
	m_mNames.RemoveAll();
	CTeleCont::Clear();
}


void CProcCont::AddProc(SProcInfo* pInfo,bool forward)
{
	if(forward)
		m_Forwards.Add(pInfo);
	else
	{
		m_Proc.Add(pInfo);
		m_mNames.InsertKey(pInfo->name,pInfo);
	}
}

void CProcCont::FillGroup(CPtrArray& arr)
{
	CParser* pParser=CTxtTrap::GetParser();
	if(!m_builded)
	{
		for(int i=0;i<4;i++)
			m_Group[i]->ClearEntries();
		int c=GetCount();
		for(i=0;i<c;i++)
		{
			int gr=0;
			SProcInfo* pInfo=GetProc(i);
			if(pInfo->func)
				gr=1;
			if(pInfo->export)
				gr+=2;
			CTeleEntry* pEntry=new CMethodEntry(pInfo,false);
			m_Group[gr]->AddEntry(pEntry);
		}
		c=m_Forwards.GetSize();
		for(i=0;i<c;i++)
		{
			int gr=0;
			SProcInfo* pInfo=(SProcInfo*)m_Forwards[i];
			if(!GetProc(pInfo->name))
			{
				if(pInfo->func)
					gr=1;
				if(pInfo->export)
					gr+=2;
				CTeleEntry* pEntry=new CMethodEntry(pInfo,true);
				m_Group[gr]->AddEntry(pEntry);
			}
		}
		m_builded=true;
	}
	// 0 - proc
	// 1 - func
	// 2 - proc export
	// 3 - func export
	if(m_Group[3]->GetCount())	// Экспортные функции добавляем всегда
		arr.Add(m_Group[3]);
	bool InExpr=(pParser->m_flags & CParser::InExpression)!=0;
	if(!InExpr && m_Group[2]->GetCount())	// Экспортные процедуры добавляем если сейчас не в выражении
		arr.Add(m_Group[2]);

	if(pParser==m_pParser)	// Заполняется сисок методов для текущего модуля
	{
		if(m_Group[1]->GetCount())	// Частные функции добавляем всегда
			arr.Add(m_Group[1]);
		if(!InExpr && m_Group[0]->GetCount())	// Частные процедуры добавляем если сейчас не в выражении
			arr.Add(m_Group[0]);
	}
}

void CMethodEntry::GetHint(CString& buf)
{
	if(m_pInfo->func)
		buf="Функция ";
	else
		buf="Процедура ";
	buf+=m_Name+"(";
	SParamInfo* p=m_pInfo->params;
	while(p)
	{
		if(p->val)
			buf+="Знач ";
		buf+=p->name;
		if(p->array)
			buf+="[]";
		else if(!p->def.IsEmpty())
			buf=buf+"="+p->def;
		if(p->next)
			buf+=", ";
		p=p->next;
	}
	buf=buf+")";
	if(m_pInfo->export)
		buf+=" Экспорт";
	if(m_forward)
		buf+=" Далее";
	buf=buf+"\n"+m_pParent->name;
	CString txt;
	txt.Format(", Строка %i",m_pInfo->start.line+1);
	buf+=txt;
	GetComments(txt,m_pInfo->start);
	if(!txt.IsEmpty())
		buf=buf+'\n'+txt;
}

void CMethodEntry::GetInsert(CString& buf)
{
	buf=m_Name+"(";
	if(m_pInfo->params)
		buf+="!";
	buf+=")";
	if(!(CTxtTrap::GetParser()->m_flags & CParser::InExpression))
		buf+=";";
}

int CProcCont::FillListBox(CWnd* list,int sort, int curLine,CPtrArray& arr,bool listbox)
{
	CTeleCont::Clear();
	int c=GetCount();
	int f=0;
	UINT GetDataMsg=LB_GETITEMDATA;
	UINT AddStrMsg=LB_ADDSTRING;
	UINT InsertStrMsg=LB_INSERTSTRING;
	UINT SetDataMsg=LB_SETITEMDATA;
	if(!listbox)
	{
		AddStrMsg=CB_ADDSTRING;
		InsertStrMsg=CB_INSERTSTRING;
		SetDataMsg=CB_SETITEMDATA;
		GetDataMsg=CB_GETITEMDATA;
	}
	for(int i=0;i<c;i++)
	{
		int gr=0;
		SProcInfo* pInfo=GetProc(i);
		if(pInfo->func)
			gr=1;
		if(pInfo->export)
			gr+=2;
		CTeleEntry* pEntry=new CMethodEntry(pInfo,false);
		m_Group[gr]->AddEntry(pEntry);
		int idx;
		if(sort)
			idx=list->SendMessage(InsertStrMsg, i,(LPARAM)(LPCTSTR)pEntry->m_Name);
		else
			idx=list->SendMessage(AddStrMsg,0,(LPARAM)(LPCTSTR)pEntry->m_Name);
		list->SendMessage(SetDataMsg,idx,(LPARAM)pEntry);
		arr.Add(pEntry);
		if(pInfo->func)
			f++;
	}
	int aLine=-1,aIdx=0;
	for(i=0;i<c;i++)
	{
		CMethodEntry* pEntry=(CMethodEntry*)list->SendMessage(GetDataMsg,i);
		if(pEntry->m_pInfo->start.line<=curLine && pEntry->m_pInfo->start.line>aLine)
		{
			aLine=pEntry->m_pInfo->start.line;
			aIdx=i;
		}
	}
	list->SendMessage(listbox?LB_SETCURSEL:CB_SETCURSEL,aIdx);
	list->SendMessage(listbox?LB_SETTOPINDEX:CB_SETTOPINDEX,aIdx);
	return f;
}

SProcInfo* CProcCont::GetForwards(CString name)
{
	int c=m_Forwards.GetSize();
	for(int i=0;i<c;i++)
	{
		SProcInfo* pInfo=(SProcInfo*)m_Forwards[i];
		if(!pInfo->name.CompareNoCase(name))
			return pInfo;
	}
	return NULL;
}

bool CMethodEntry::MakeInsert()
{
	CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
	CTeleList::m_pOneList->Insert(this);
	if(m_pInfo->params)
		CParamWnd::GetParamWnd()->QuickActivate(pEditor);
	return true;
}
