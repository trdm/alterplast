// varcont.cpp
#include "stdafx.h"
#include "parser.h"
#include "tentrydrawing.h"
#include "txttrap.h"

class CVarEntry:public CTeleEntry
{
public:
	SVarInfo* m_pInfo;
	CVarEntry(SVarInfo* pInfo):m_pInfo(pInfo)
	{
		m_Name=pInfo->name;
	}
	void GetHint(CString& buf)
	{
		buf="Перем ";
		buf+=m_Name;
		if(!m_pInfo->array.IsEmpty())
			buf=buf+"["+m_pInfo->array+"]";
		if(m_pInfo->export)
			buf+=" Экспорт";
		buf=buf+"\n"+m_pParent->name;
		CString txt;
		txt.Format(" Строка %i",m_pInfo->name.line+1);
		buf+=txt;
		LPCTSTR ptr=m_pInfo->name.pStart,pComm=NULL;
		int comm=0;
		while(*ptr!='\r' && *ptr)
		{
			switch(comm)
			{
			case 0:
				if(*ptr=='/')
					comm=1;
				break;
			case 1:
				if(*ptr=='/')
					comm=2;
				break;
			case 2:
				if((BYTE)*ptr>32)
				{
					comm=3;
					pComm=ptr;
				}
			}
			ptr++;
		}
		if(pComm && ptr-pComm)
		{
			txt=CString(pComm,ptr-pComm);
			txt.TrimRight();
			buf=buf+'\n'+txt;
		}
	}
	void GetInsert(CString& buf)
	{
		buf=m_Name;
		if(!m_pInfo->array.IsEmpty())
			buf+="[!]";
	}
};

const int	CVarCont::GetGroupCount() const
{
	return 4;
}

const int*	CVarCont::GetImagesNum()  const
{
	// 0 - var, 1 - var[], 2 - var export, 3 - var[] export
	static const img[2][4]={
		{IDI_GMATRPRIV_,IDI_GMATRPRIV_,IDI_GMATREXP_,IDI_GMATREXP_},
		{IDI_CMATRPRIV_,IDI_CMATRPRIV_,IDI_CMATREXP_,IDI_CMATREXP_}
	};
	return m_pParser->IsGM()?img[0]:img[1];
}

LPCTSTR*	CVarCont::GetGroupNames() const
{
	static LPCTSTR names[2][4]={
		{"Глобальный модуль","Глобальный модуль","Глобальный модуль","Глобальный модуль"},
		{"Текущий модуль","Текущий модуль","Текущий модуль","Текущий модуль"}
	};
	return m_pParser->IsGM()?names[0]:names[1];
}

CVarCont::CVarCont(CParser* pParser):CTeleCont(pParser)
{
	InitGroups();
}

SVarInfo* CVarCont::AddVar(SLexem& var)
{
	SVarInfo* pInfo=new SVarInfo;
	pInfo->name=var;
	m_Vars.Add(pInfo);
	m_mVars.InsertKey(var,pInfo);
	return pInfo;
}

void CVarCont::Clear()
{
	CTeleCont::Clear();
	m_mVars.RemoveAll();
	int s=m_Vars.GetSize();
	for(int i=0;i<s;i++)
		delete (SVarInfo*)m_Vars[i];
	m_Vars.RemoveAll();
}

void CVarCont::FillGroup(CPtrArray& arr)
{
	if(!m_builded)
	{
		CTeleCont::Clear();
		int c=GetCount();
		c=m_Vars.GetSize();
		for(int i=0;i<c;i++)
		{
			int gr=0;
			SVarInfo* pInfo=GetVar(i);
			if(!pInfo->array.IsEmpty())
				gr=1;
			if(pInfo->export)
				gr+=2;
			CTeleEntry* pEntry=new CVarEntry(pInfo);
			m_Group[gr]->AddEntry(pEntry);
		}
		m_builded=true;
	}
	CParser* pParser=CTxtTrap::GetParser();
	bool AllowArray=(pParser->m_flags & CParser::NotArrayVar)==0;
	if(m_Group[2]->GetCount())	// Экспортные переменные добавляем всегда
		arr.Add(m_Group[2]);
	if(m_Group[3]->GetCount() && AllowArray)
		arr.Add(m_Group[3]);
	if(m_pParser==pParser)	// Частные переменные добавляем для текущего модуля
	{
		if(m_Group[0]->GetCount())
			arr.Add(m_Group[0]);
		if(m_Group[1]->GetCount() && AllowArray)
			arr.Add(m_Group[1]);
	}
}
