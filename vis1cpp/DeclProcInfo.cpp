// DeclProcInfo.cpp: implementation of the CDeclProcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "DeclProcInfo.h"
#include "classinfo.h"
#include "moduleparser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeclProcInfo::CDeclProcInfo(const CString& proc,CMetaDataOfClasses::ARR_TYPE_NAME* pArr)
	:CInfo(proc,TYPE_DECLARED_PROC)
{
	m_ProcName=proc;
	if(!pArr)
		return;
	m_DisplayName="";
	int size=pArr->GetSize();
	int i=0;
	NameBindOfType* pName;
	CString type;
	if(size)
	{
		pName=pArr->GetAt(0);
		if(pName->name=="#return")
		{
			type=pName->type;
			//m_DisplayName=pName->type+" ";
			m_HasRetVal=false;
			i=1;
		}
		else
			m_HasRetVal=true;
	}
	else
		m_HasRetVal=false;

	m_DisplayName+=m_ProcName+"(";
	for(;i<pArr->GetSize();i++)
	{
		pName=pArr->GetAt(i);
		if(pName->bIsVal)
			m_DisplayName+="Знач ";
		m_DisplayName+=pName->type+" "+pName->name;
		if(!pName->value.IsEmpty())
			m_DisplayName=m_DisplayName+"="+pName->value;
		m_DisplayName+=", ";
	}
	m_DisplayName.TrimRight(", ");
	m_DisplayName+=")";
	if(!type.IsEmpty())
		m_DisplayName+=" : "+type;
}

CDeclProcInfo::~CDeclProcInfo()
{

}

// HTREEITEM CDeclProcInfo::InsertInTree(HTREEITEM parent)
// {
// 	return CInfo::InsertInTree(parent);
// }

void CDeclProcInfo::OnSelect()
{
// 	CClassInfo* pInfo=(CClassInfo*)GetParentData(GetCurItem());
// 	pInfo->OpenKeys(m_ProcName);
	OpenKeysForClassInfoFromCurItem(m_ProcName);
}
