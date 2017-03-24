// typegroup.cpp
#include "stdafx.h"
#include "typegroup.h"
#include "option.h"
#include "parser.h"
#include "txttrap.h"
#include "tentrydrawing.h"
#include "typescont.h"
#include "resource.h"
#include "telelist.h"
#include "paramwnd.h"

CBLMap CTypeGroup::m_mTypeGroups;
CBLMap CTypeGroup::m_mMetaTypes;

static LPCTSTR TTypes[]={
	"",
	"SubList",
	"SubConto",
	"SubConto",
	"DocForm",
	"DocumentJournal",
	"OperationList",
	"ProvList",
	"Transact",
	"AccountForm",
	"AccountList",
	"Report",
	"Report",
	"OperationForm",
	"CalcAlg",
	"CalcJournalList",
};

void CTypeGroup::Init()
{
	static LPCTSTR MetaTypes[]={
		"SubList",
		"SubFolder",
		"SubConto",
		"Document",
		"Journal",
		"OperationList",
		"ProvList",
		"Transact",
		"AccountChart",
		"AccountChartList",
		"Report",
		"CalcVar",
		"Operation",
		"CalcAlg",
		"CalcJournal",
	};
	for(int i=0;i<sizeof(MetaTypes)/sizeof(MetaTypes[0]);i++)
		m_mMetaTypes.InsertKey(MetaTypes[i],(void*)(i+1));
}

LPCTSTR CTypeGroup::ConfType2TType(LPCTSTR typeName)
{
	return TTypes[(int)m_mMetaTypes.GetKey(typeName)];
}

int CTypeGroup::GetConfTypeNum(LPCTSTR typeName)
{
	return (int)m_mMetaTypes.GetKey(typeName);
}


CTypeGroup* CTypeGroup::GetTypeGroup(LPCTSTR type)
{
	if(!type || !*type)
		return NULL;
	CTypeGroup* pGroup=(CTypeGroup*)m_mTypeGroups.GetKey(type);
	if(!pGroup)
		pGroup=new CTypeGroup(type);
	return pGroup;
}

class CKeyMetEntry:public CTeleEntry
{
public:
	bool MakeInsert();
	STMethInfo* pInfo;
	CKeyMetEntry(STMethInfo* p,LPCTSTR name):CTeleEntry(name),pInfo(p){}
	void GetHint(CString& buf)
	{
		buf.Empty();

		CTypesCont* pCont=CTypesCont::m_pOneCont;
		STTypeInfo* parent=pInfo->ParentType;
		if(parent && parent->Alias)
		{
			buf=pCont->GetString(parent->Alias);
			buf+="::";
		}
		buf+=pCont->GetString(pInfo->NameRus);
		buf+="(";
		for(int i=0;i<pInfo->ParamCnt;i++)
		{
			buf=buf+pCont->GetString(pInfo->pParams[i].Name);
			if(i<pInfo->ParamCnt-1)
				buf+=", ";
		}
		buf+=")";
		if(pInfo->Descr)
			buf=buf+"\n"+pCont->GetString(pInfo->Descr);
		if(pInfo->retDescr)
			buf=buf+"\nВозвращает:\n"+pCont->GetString(pInfo->retDescr);
	}
	void GetInsert(CString& buf)
	{
		buf=m_Name+'(';
		if(pInfo->ParamCnt)
			buf+="!";
		buf+=")";
		if(!(CTxtTrap::GetParser()->m_flags & CParser::InExpression))
			buf+=";";
	}
};

class CKeyAtrEntry:public CTeleEntry
{
public:
	STAttrInfo* pInfo;
	CKeyAtrEntry(STAttrInfo* p,LPCTSTR name):CTeleEntry(name),pInfo(p){}
	void GetHint(CString& buf)
	{
		buf.Empty();
		CTypesCont* pCont=CTypesCont::m_pOneCont;
		if(pInfo->ParentType && pInfo->ParentType->Alias)
		{
			buf=pCont->GetString(pInfo->ParentType->Alias);
			buf+="::";
		}
		buf+=pCont->GetString(pInfo->NameRus);
		if(pInfo->Descr)
			buf=buf+'\n'+pCont->GetString(pInfo->Descr);
	}
};


static void FillTGroupFromType(CTypeGroup* pGroup,STTypeInfo* pInfo)
{
	CTypesCont* pCont=CTypesCont::m_pOneCont;
	// Заполним атрибуты
	for(int i=0;i<pInfo->AttrCnt;i++)
	{
		STAttrInfo* pAtr=pInfo->pAttribs+i;
		int gr=(pAtr->Flags?3:1);
		CTeleEntry* pEntry=new CKeyAtrEntry(pAtr,pCont->GetString(pAtr->NameRus));
		pGroup->m_Group[gr]->AddEntry(pEntry);
		pGroup->m_mAttribs.InsertKey(pEntry->m_Name,pAtr);

		LPCTSTR eng=pCont->GetString(pAtr->NameEng);
		if(eng)
		{
			pEntry=new CKeyAtrEntry(pAtr,eng);
			pGroup->m_Group[gr-1]->AddEntry(pEntry);
			pGroup->m_mAttribs.InsertKey(pEntry->m_Name,pAtr);
		}
	}
	// Заполним методы
	for(i=0;i<pInfo->MethCnt;i++)
	{
		STMethInfo* pMet=pInfo->pMethods+i;
		if(pMet->flags & 4)
			break;	// handler
		int gr=pMet->flags==0?5:(pMet->flags==1?7:9);
		CTeleEntry* pEntry=new CKeyMetEntry(pMet,pCont->GetString(pMet->NameRus));
		pGroup->m_Group[gr]->AddEntry(pEntry);
		LPCTSTR eng=pCont->GetString(pMet->NameEng);
		if(eng)
		{
			pEntry=new CKeyMetEntry(pMet,eng);
			pGroup->m_Group[gr-1]->AddEntry(pEntry);
		}
	}
	if(pInfo->BaseCnt==1)
		FillTGroupFromType(pGroup,pInfo->pBases);
	else
	{
		for(i=0;i<pInfo->BaseCnt;i++)
			FillTGroupFromType(pGroup,((STTypeInfo**)pInfo->pBases)[i]);
	}
}

const int	CTypeGroup::GetGroupCount() const
{
	return 10;
}

const int*	CTypeGroup::GetImagesNum()  const
{
	static const int img[10]={IDI_KEYATR_,IDI_KEYATR_,IDI_KEYATR_,IDI_KEYATR_,
		IDI_KEYFUNC_,IDI_KEYFUNC_,IDI_KEYFUNC_,IDI_KEYFUNC_,IDI_KEYFUNC_,IDI_KEYFUNC_};
	return img;
}

LPCTSTR*	CTypeGroup::GetGroupNames() const
{
	static LPCTSTR nullNames[10];
	return nullNames;
}


CTypeGroup::CTypeGroup(LPCTSTR type):CTeleCont(NULL)
{
	InitGroups();
	CTypesCont* pCont=CTypesCont::m_pOneCont;
	STTypeInfo* pInfo=pCont->GetTypeInfo(type);
	if(pInfo)
	{
		m_mTypeGroups.InsertKey(type,this);
		FillTGroupFromType(this,pInfo);
	}
}

void CTypeGroup::FillGroup(CPtrArray& arr)
{
	CParser* pParser=CTxtTrap::GetParser();
	if(pParser->m_flags & CParser::NotArrayVar)
		return;
	int allowed[5]={1,1,1,1,1};
	// 0 - Atr, 1 - atr in expr, 2 - proc, 3 - func, 4 - func in expr
	if(pParser->m_flags & CParser::InExpression)
		allowed[2]=0;
	else
		allowed[1]=allowed[4]=0;
	int lang=COption::m_pOption->GetLanguage();
	for(int i=1;i<10;i+=2)
	{
		if(allowed[i/2])
		{
			if(lang & 2)
			{
				if(m_Group[i]->GetCount())
					arr.Add(m_Group[i]);
			}
			if(lang & 1)
			{
				if(m_Group[i-1]->GetCount())
					arr.Add(m_Group[i-1]);
			}
		}
	}
}

bool CTypeGroup::IsAttribExist(SLexem& name)
{
	return m_mAttribs.GetKey(name)!=NULL;
}

bool CKeyMetEntry::MakeInsert()
{
	CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
	CTeleList::m_pOneList->Insert(this);
	if(pInfo->ParamCnt)
		CParamWnd::GetParamWnd()->QuickActivate(pEditor);
	return true;
}

void CTypeGroup::ReFillAll()
{
	CString key;
	for(POSITION pos=m_mTypeGroups.GetStart();pos;)
	{
		CTypeGroup* pGroup=(CTypeGroup*)m_mTypeGroups.GetNext(pos,key);
		pGroup->Clear();
		STTypeInfo* pInfo=CTypesCont::m_pOneCont->GetTypeInfo(key);
		if(pInfo)
			FillTGroupFromType(pGroup,pInfo);
	}
}
