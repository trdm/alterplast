// MetaGroup.cpp: implementation of the CMetaGroup class.
#include "stdafx.h"
#include "MetaGroup.h"
#include "parser.h"
#include "typescont.h"
#include "typegroup.h"
#include "tentrydrawing.h"

CMetaGroup::PFILLFUNC CMetaGroup::m_func[]={
	NULL,
	FillSubList,		//"SubList",
	FillSubconto,		//"SubFolder",
	FillSubconto,		//"SubConto",
	FillDoc,			//"Document",
	FillJournal,		//"Journal",
	FillOperationList,	//"OperationList",
	FillOperation,		//"ProvList",
	FillDoc,			//"Transact",
	FillAccountChart,	//"AccountChart",
	FillAccountChart,	//"AccountChartList",
	NULL,				//"Report",
	NULL,				//"CalcVar",
	FillOperation,		//"Operation",
	FillCalcAlg,		//"CalcAlg",
	FillCJournal,		//"CalcJournal"
};

class CTAtrEntry:public CTeleEntry{
public:
	CMetaDataObj* m_pObj;
	CTAtrEntry(CMetaDataObj* pObj):CTeleEntry(),m_pObj(pObj){m_Name=pObj->GetCode();}
	void GetHint(CString& buf)
	{
		long p;
		buf=pMetaDataCont->GetFullName(m_pObj->GetID(),&p,1);
		buf+='\n';

		CString vals[3];
		long len=0,dec=0;
		vals[0]=m_pObj->GetPresent();
		vals[1]=m_pObj->GetDescr();
		if(m_pObj->IsRefObj())
		{
			CMetaDataRefObj* pRef=(CMetaDataRefObj*)m_pObj;
			vals[2].Format("Список ссылок [%i]",pRef->GetNRefDefs());
		}
		else if(m_pObj->IsTypedObj())
		{
			CType type=((CMetaDataTypedObj*)m_pObj)->GetType();
			vals[2]=type.GetTypeTitle();
			int kind=type.GetTypeID();
			if(kind)
			{
				CMetaDataObj* pObj=pMetaDataCont->FindObject(kind);
				if(pObj)
					vals[2]=vals[2]+'.'+pObj->GetCode();
			}
			len=type.GetLength();
			dec=type.GetPrecision();
		}
		if(!vals[0].IsEmpty())
			buf+=vals[0]+'\n';
		if(!vals[1].IsEmpty())
			buf+=vals[1]+'\n';
		if(!vals[2].IsEmpty())
			buf+=vals[2];
		if(len)
		{
			CString fmt;
			fmt.Format(" Длина %i",len);
			buf+=fmt;
			if(dec)
			{
				fmt.Format(" Точность %i",dec);
				buf+=fmt;
			}
		}
	}
};

const int	CMetaGroup::GetGroupCount() const
{
	return 1;
}

const int*	CMetaGroup::GetImagesNum()  const
{
	static const int img[]={IDI_METAATR_};
	return img;
}

LPCTSTR*	CMetaGroup::GetGroupNames() const
{
	static LPCTSTR nullNames[]={NULL};
	return nullNames;
}

CMetaGroup::CMetaGroup(CParser* pParser):CTeleCont(pParser)
{
	InitGroups();
	m_FillFunc=NULL;
	if(m_pParser->m_ID<2)
		return;
	CString type;
	CConfigCont::DocumentToID(m_pParser->m_pDocument,type);
	m_FillFunc=m_func[CTypeGroup::GetConfTypeNum(type)];
}

void CMetaGroup::FillFromObjArr(CMetaDataObjArray* pArr)
{
	if(pArr)
	{
		int size=pArr->GetNItems();
		for(int i=0;i<size;i++)
		{
			CMetaDataObj* pObj=pArr->GetAt(i);
			if(pObj)
			{
				CTAtrEntry* pEntry=new CTAtrEntry(pObj);
				m_Group[0]->AddEntry(pEntry);
				m_atribs.InsertKey(pEntry->m_Name,pEntry);
			}
		}
	}
}

void CMetaGroup::Clear()
{
	CTeleCont::Clear();
	m_atribs.RemoveAll();
	if(m_FillFunc)
	{
		CMetaDataObj* pObj=pMetaDataCont->FindObject(m_pParser->m_ID);
		if(pObj)
			(this->*m_FillFunc)(pObj);
	}
}

void CMetaGroup::FillGroup(CPtrArray& arr)
{
	if(m_Group[0]->GetCount() && !(m_pParser->m_flags & CParser::NotArrayVar))
		arr.Add(m_Group[0]);
}

bool CMetaGroup::IsAttribExist(SLexem& name)
{
	return m_atribs.GetKey(name)!=NULL;
}

void CMetaGroup::FillSubconto(CMetaDataObj *pObj)
{
	CSbCntTypeDef* pDef=(CSbCntTypeDef*)pObj;
	FillFromObjArr(pDef->GetParams());
}
void CMetaGroup::FillSubList(CMetaDataObj *pObj)
{
	long parent;
	pMetaDataCont->GetFullName(pObj->GetID(),&parent,1);
	if(parent)
	{
		CMetaDataObj* pParent=pMetaDataCont->FindObject(parent);
		if(pParent)
			FillSubconto(pParent);
	}
}

void CMetaGroup::FillDoc(CMetaDataObj *pObj)
{
	CDocDef* pDef=(CDocDef*)pObj;
	FillFromObjArr(pDef->GetHeads());
	FillFromObjArr(pDef->GetTables());
	FillFromObjArr((CMetaDataObjArray*)pMetaDataCont->GetGenJrnlFlds());
}

void CMetaGroup::FillJournal(CMetaDataObj *pObj)
{
	long parent;
	pMetaDataCont->GetFullName(pObj->GetID(),&parent,1);
	if(parent)
	{
		CJournalDef* pDef=(CJournalDef*)pMetaDataCont->FindObject(parent);
		if(pDef)
			FillFromObjArr(pDef->GetFilds());
	}
	FillFromObjArr((CMetaDataObjArray*)pMetaDataCont->GetGenJrnlFlds());
}
void CMetaGroup::FillAccountChart(CMetaDataObj *pObj)
{
	CBuhDef* pDef=pMetaDataCont->GetBuhDef();
	if(pDef)
		FillFromObjArr((CMetaDataObjArray*)pDef->GetAccParamDefs());
}
void CMetaGroup::FillOperation(CMetaDataObj *pObj)
{
	CBuhDef* pDef=pMetaDataCont->GetBuhDef();
	if(pDef)
	{
		FillFromObjArr((CMetaDataObjArray*)pDef->GetOperParamDefs());
		FillFromObjArr((CMetaDataObjArray*)pDef->GetProvParamDefs());
	}
}
void CMetaGroup::FillOperationList(CMetaDataObj *pObj)
{
	FillOperation(NULL);
	CBuhDef* pDef=pMetaDataCont->GetBuhDef();
	if(pDef)
	{
		CJournalDef* pObj=(CJournalDef*)pDef->GetOperJournalDef();
		FillFromObjArr(pObj->GetFilds());
	}
}

void CMetaGroup::FillCJ(CMetaDataObj *pObj)
{
	FillFromObjArr(((CCJDef*)pObj)->GetCJParamDefs());
}

void CMetaGroup::FillCJournal(CMetaDataObj *pObj)
{
	long parent;
	pMetaDataCont->GetFullName(pObj->GetID(),&parent,1);
	if(parent)
	{
		CMetaDataObj* pParent=pMetaDataCont->FindObject(parent);
		if(pParent)
			FillCJ(pParent);
	}
}
void CMetaGroup::FillCalcAlg(CMetaDataObj *pObj)
{
	CMetaDataObjArray* pArr=pMetaDataCont->GetCJDefs();
	if(pArr)
	{
		int n=pArr->GetNItems();
		for(int i=0;i<n;i++)
			FillCJ(pArr->GetAt(i));
	}
}