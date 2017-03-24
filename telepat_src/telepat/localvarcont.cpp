// localvarcont.cpp
#include "stdafx.h"
#include "parser.h"
#include "tentrydrawing.h"
#include "txttrap.h"

void CLocalVarCont::Clear()
{
	int s=m_Vars.GetSize();
	for(int i=0;i<s;i++)
		delete (SLocalVarInfo*)m_Vars[i];
	m_Vars.RemoveAll();
	m_mVars.RemoveAll();
	CTeleCont::Clear();
}

static CString GetVarLine(SLexem& lexem)
{
	LPCTSTR ptr=lexem.pStart,pStart=ptr;
	while(*pStart && *pStart!=';' && *pStart!='\n')
		pStart--;
	pStart++;
	while(*ptr && *ptr!='\r')
		ptr++;
	CString txt=CString(pStart,ptr-pStart);
	txt.TrimLeft();
	txt.TrimRight();
	txt.Replace("\t","  ");
	return txt;
}

class CMetVarEntry:public CTeleEntry
{
public:
	SLocalVarInfo* m_pInfo;
	CMetVarEntry(SLocalVarInfo* pInfo):m_pInfo(pInfo)
	{
		m_Name=pInfo->name;
	}
	void GetHint(CString& buf)
	{
		buf=m_Name;
		if(!m_pInfo->array.IsEmpty())
			buf=buf+"["+m_pInfo->array+"]";
		buf+='\n';
		if(m_pInfo->type)
		{
			buf+="Локальная переменная";
			buf=buf+'\n'+GetVarLine(m_pInfo->name);
			if(m_pInfo->name.pStart!=m_pInfo->lastDef.pStart)
				buf=buf+"\n...\n"+GetVarLine(m_pInfo->lastDef);
		}
		else
			buf+="Параметр";
	}
	void GetInsert(CString& buf)
	{
		buf=m_Name;
		if(!m_pInfo->array.IsEmpty())
			buf+="[!]";
	}
};

const int	CLocalVarCont::GetGroupCount() const
{
	return 2;
}

const int*	CLocalVarCont::GetImagesNum()  const
{
	static const int img[]={IDI_LOCALVAR_,IDI_LOCALVAR_};
	return img;
}

LPCTSTR*	CLocalVarCont::GetGroupNames() const
{
	static LPCTSTR names[2]={NULL,NULL};
	return names;
}

CLocalVarCont::CLocalVarCont(CParser* pParser):CTeleCont(pParser)
{
	InitGroups();
}

SLocalVarInfo* CLocalVarCont::AddVar(SLexem& name)
{
	SLocalVarInfo* pVar=new SLocalVarInfo;
	pVar->name=name;
	m_Vars.Add(pVar);
	m_mVars.InsertKey(name,pVar);
	return pVar;
}

void CLocalVarCont::FillGroup(CPtrArray& arr)
{
	if(!m_builded)
	{
		CTeleCont::Clear();
		int s=GetCount();
		for(int i=0;i<s;i++)
		{
			SLocalVarInfo* pInfo=GetVar(i);
			CTeleEntry* pEntry=new CMetVarEntry(GetVar(i));
			m_Group[pInfo->array.IsEmpty()?0:1]->AddEntry(pEntry);
		}
	}
	if(m_Group[0]->GetCount())
		arr.Add(m_Group[0]);
	if(m_Group[1]->GetCount() && !(m_pParser->m_flags & CParser::NotArrayVar))
		arr.Add(m_Group[1]);
}