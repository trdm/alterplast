// metadata.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "dispimpl.h"
#include "iconfig.h"
#include "metadata.h"
#include "metadataobj.h"
#include "savewrapper.h"
#include "imigration.h"
#include "iconstants.h"
#include "IRefers.h"
#include "IMetaEnums.h"

ITypeInfo* CIMetaData::m_pTypeInfo=NULL;

CMetaDataCont* CIMetaData::m_pMetaData=NULL;
CIMetaData* CIMetaData::m_pOneMetaData=NULL;


CIMetaData::CIMetaData()
{
	m_pMetaData=GetMetaData();
	m_pOneMetaData=this;
	m_pTemplateOfMetaData=NULL;
	m_isMDModified=false;
	// Шаблон документа окна конфигурации и документ MetaStream
	int fnd=0;
	for(POSITION pos=pMainApp->GetFirstDocTemplatePosition();pos;)
	{
		CMyDocTemp* pTemp=(CMyDocTemp*)pMainApp->GetNextDocTemplate(pos);
		if(!lstrcmp(pTemp->GetDocClass()->m_lpszClassName,"CMetaDataDoc"))
		{
			m_pTemplateOfMetaData=pTemp;
			fnd++;
		}
		else if(!lstrcmp(pTemp->GetDocClass()->m_lpszClassName,"CMetaDataStream"))
		{
			m_pTemplateMetaStream=pTemp;
			fnd++;
		}
		if(fnd==2)
			break;
	}
}
static void OnOpenMDWindow(CWnd*)
{
	if(CIMetaData::m_pOneMetaData->m_isMDModified)
	{
		CIMetaData::m_pOneMetaData->SetMDModified();
		CIMetaData::m_pOneMetaData->m_isMDModified=false;
	}
}

void CIMetaData::SetMDModified()
{
	POSITION p=m_pTemplateOfMetaData->GetFirstDocPosition();
	if(p)
	{
		CDocument* pDoc=m_pTemplateOfMetaData->GetNextDoc(p);
		if(pDoc)
			pDoc->SetModifiedFlag();
	}
	else
	{
		m_isMDModified=true;
		CConfigSvcImpl::m_pTheService->AddConfigEventHandler(CFG_ON_CREATE_CFG_WND,OnOpenMDWindow);
		POSITION pos=m_pTemplateMetaStream->GetFirstDocPosition();
		if(pos)
		{
			CDocument* pDoc=m_pTemplateMetaStream->GetNextDoc(pos);
			if(pDoc)
				pDoc->SetModifiedFlag();
		}
	}
}
void CIMetaData::SetDocModified()
{
	CConfigCont::GetContainer()->SetModifiedFlag(TRUE);
}
CString CIMetaData::GetFullName(long id)
{
	long i;
	CString txt=m_pMetaData->GetFullName(id,&i,
		m_pMetaData->GetTaskDef()->GetDefaultLanguage());
	return txt;
}


STDMETHODIMP CIMetaData::get_TaskDef(ITaskDef **pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	*pVal=(ITaskDef*)CIMetaDataObj::GetObj(1);
	if(!*pVal)
		return SetError(E_FAIL,"Не удалось получить данные о задаче");
	return S_OK;
}
STDMETHODIMP CIMetaData::get_Modified(short *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	*pVal=CConfigCont::GetContainer()->IsModified()?1:0;
	POSITION pos=m_pTemplateMetaStream->GetFirstDocPosition();
	if(pos)
	{
		CDocument* pDoc=m_pTemplateMetaStream->GetNextDoc(pos);
		if(pDoc)
		{
			if(pDoc->IsModified())
				*pVal=2;
		}
	}
	return S_OK;
}
STDMETHODIMP CIMetaData::FindObject(long id,IMetaDataObj **pObj)
{
	if(IsBadWritePtr(pObj,4))
		return E_POINTER;
	*pObj=CIMetaDataObj::GetObj(id);
	if(!*pObj)
		return SetError(E_FAIL,"Не найден объект метаданных с ID=%i",id);
	return S_OK;
}
STDMETHODIMP CIMetaData::RefreshMDWnd()
{
	short IsModif=0;
	get_Modified(&IsModif);
	if(IsModif<2)
		return S_OK;
	POSITION p=m_pTemplateOfMetaData->GetFirstDocPosition();
	if(p)
	{
		CDocument* pDoc=m_pTemplateOfMetaData->GetNextDoc(p);
		if(pDoc)
		{
			p=pDoc->GetFirstViewPosition();
			if(p)
			{
				CView* pView=pDoc->GetNextView(p);
				if(pView)
				{
					((CTreeView*)pView)->GetTreeCtrl().DeleteAllItems();
					pView->OnInitialUpdate();
				}
			}
			pDoc->SetModifiedFlag();
		}
	}
	return S_OK;
}

STDMETHODIMP CIMetaData::SaveMDToFile(BSTR path,VARIANT_BOOL FireEvent,VARIANT_BOOL *retVal)
{
	if(IsBadWritePtr(retVal,4))
		return E_POINTER;
	CString lpPath(path);
	*retVal=CSaveWrapper::FastSaveMDToFile(lpPath,FireEvent?true:false)?-1:0;
	return S_OK;
}
STDMETHODIMP CIMetaData::Describe(BSTR *pRet)
{
	if(IsBadWritePtr(pRet,4))
		return E_POINTER;
	CString desc;
	m_pMetaData->DescribeMDCont(desc);
	*pRet=desc.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMetaData::get_LongCopyright(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString txt=m_pMetaData->GetLongCopyright();
	*pVal=txt.AllocSysString();
	return S_OK;
}

/*STDMETHODIMP CIMetaData::put_LongCopyright(BSTR newVal)
{
	_bstr_t bVal=newVal;
	CString old=m_pMetaData->GetLongCopyright();
	if(old!=(LPCTSTR)bVal)
	{
		SetMDModified();
		m_pMetaData->SetLongCopyright(bVal);
	}
	return S_OK;
}*/

STDMETHODIMP CIMetaData::get_ShortCopyright(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString txt=m_pMetaData->GetShortCopyright();
	*pVal=txt.AllocSysString();
	return S_OK;
}

/*STDMETHODIMP CIMetaData::put_ShortCopyright(BSTR newVal)
{
	_bstr_t bVal=newVal;
	CString old=m_pMetaData->GetShortCopyright();
	if(old!=(LPCTSTR)bVal)
	{
		SetMDModified();
		m_pMetaData->SetShortCopyright(bVal);
		m_pMetaData->SaveData("c:\\temp\\2.md");
	}
	return S_OK;
}*/

STDMETHODIMP CIMetaData::SaveMMDS(BSTR Path)
{
	_bstr_t bPath=Path;
	m_pMetaData->SaveData(bPath);
	return S_OK;
}

STDMETHODIMP CIMetaData::get_Migration(long id,IMigration **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CMetaDataObjDistr* pDistr=m_pMetaData->GetMetaDataObjDistr(id);
	if(!pDistr)
		return SetError(E_FAIL,"Объект с id=%i не имеет правил миграции");
	int r=pDistr->GetRuler();
	*pVal=new CIMigration(pDistr);
	return S_OK;
}
STDMETHODIMP CIMetaData::get_Constants(IConstants **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=new CIConstants;
	return S_OK;
}

STDMETHODIMP CIMetaData::TestRefers(long ID,VARIANT_BOOL FindAll,IMetaDataObjArray **pRetArr)
{
	if(IsBadWritePtr(pRetArr,sizeof(*pRetArr)))
		return E_POINTER;
	CPtrArray* pArr=new CPtrArray;
	if(FindAll)
		m_pMetaData->TestRefersForOne(ID,(CArray<CMetaDataObj*,CMetaDataObj*>&)*pArr);
	else
		m_pMetaData->TestRefers(ID,(CArray<CMetaDataObj*,CMetaDataObj*>&)*pArr);
	*pRetArr=new CIRefers(pArr);
	return S_OK;
}

STDMETHODIMP CIMetaData::get_Enums(IMetaEnums **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	static CIMetaEnums _MetaEnums;
	*pVal=&_MetaEnums;
	_MetaEnums.AddRef();
	return S_OK;
}
