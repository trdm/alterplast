// V7LogFilter.cpp: implementation of the CV7LogFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7LogFilter.h"
#include "V7LogProvider.h"
#include "..\\SQL\\MetaDataWork.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CV7FilterItem class
//////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CV7FilterItem)
	BL_METH("SetTypeAndValue",   "”становить“ип»«начение",   2,   NULL,   SetTypeAndValue,   DefaultSetTypeAndValue)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7FilterItem)
	BL_PROP("Type",    "“ип",        GetPropType,    NULL)
	BL_PROP("Value",   "«начение",   GetPropValue,   NULL)
	BL_PROP("Name",    "»м€",        GetPropName,    NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7FilterItem, "FilterItem", "Ёлементќтбора", 0, NULL, NULL, -1);

CV7FilterItem::CV7FilterItem():m_nType(LOG_DP_FILTER_CLEAR)
{
}


CV7FilterItem::~CV7FilterItem()
{
}


BOOL CV7FilterItem::GetPropType(CValue& Value) const
{
	Value=m_nType;
	return TRUE;
}


BOOL CV7FilterItem::GetPropValue(CValue& Value) const
{
	if (m_nType)
		Value=m_Value;
	else
		Value.Reset();
	return TRUE;
}


BOOL CV7FilterItem::GetPropName(CValue& Value) const
{
	Value=GetName();
	return TRUE;
}


BOOL CV7FilterItem::SetTypeAndValue(CValue& Result, CValue** ppValue)
{
	bool bResult;
	if (bResult = ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
	{
		CString csVal;
		switch (ppValue[0]->GetNumeric())
		{
		case LOG_DP_FILTER_USER:
			bResult = ppValue[1]->GetTypeCode() == STRING_TYPE_1C;
			break;
		case LOG_DP_FILTER_MODE:
			if (bResult = ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
			{
				csVal=ppValue[1]->GetString();
				if (bResult = csVal.GetLength() == 1)
				{
					csVal.MakeUpper();
					bResult = CString(_T("CDME")).Find(csVal[0]) != -1;
				}
			}
			break;
		case LOG_DP_FILTER_EVENTTYPE:
			if (bResult = ppValue[1]->GetTypeCode() == AGREGATE_TYPE_1C)
			{
				CBLContext* pCont=ppValue[1]->GetContext();
				bResult = pCont->GetRuntimeClass() == RUNTIME_CLASS(CV7LogEventType);
			}
			else
				bResult = ppValue[1]->GetTypeCode() == STRING_TYPE_1C;
			break;
		case LOG_DP_FILTER_EVENT:
			if (bResult = ppValue[1]->GetTypeCode() == AGREGATE_TYPE_1C)
			{
				CBLContext* pCont=ppValue[1]->GetContext();
				bResult = pCont->GetRuntimeClass() == RUNTIME_CLASS(CV7LogEvent);
			}
			else
				bResult = ppValue[1]->GetTypeCode() == STRING_TYPE_1C;
			break;
		case LOG_DP_FILTER_CATEGORY:
			if (bResult = ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
			{
				int nCat=ppValue[1]->GetNumeric();
				bResult = (nCat >=0) && (nCat<=5);
			}
			break;
		case LOG_DP_FILTER_COMMENT:
		case LOG_DP_FILTER_PRESENT:
			bResult = ppValue[1]->GetTypeCode() == STRING_TYPE_1C;
			break;
		case LOG_DP_FILTER_OBJECT:
			bResult = GetStringPresent(ppValue[1],TRUE,csVal) == TRUE;
			break;
		case LOG_DP_FILTER_OBJECTKIND:
			{
				CType t(0);
				if (bResult = CMetaDataWork::SetTypeAndKind(t,ppValue[1]->GetString()))
					bResult = GetStringPresent(&CValue(t),FALSE,csVal) == TRUE;
			}
			break;
		default:
			bResult=false;
			break;
		}
	}
	if (bResult)
	{
		m_nType=ppValue[0]->GetNumeric();
		m_Value=*ppValue[1];
	}
	Result=bResult;
	return TRUE;
}


BOOL CV7FilterItem::DefaultSetTypeAndValue(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		*param=CNumeric(LOG_DP_FILTER_CLEAR);
		return TRUE;
		break;
	case 1:
		*param=CValue();
		return TRUE;
		break;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// class CLogFilterItemsCollection
//////////////////////////////////////////////////////////////////////////
CLogFilterItemsCollection::CLogFilterItemsCollection()
{
}


CLogFilterItemsCollection::~CLogFilterItemsCollection()
{
	Clear();
}


int CLogFilterItemsCollection::GetSize() const
{
	return GetCount();
}


CV7FilterItem* CLogFilterItemsCollection::Add(LPCSTR szName)
{
	CV7FilterItem* pEvent = this->GetAt(szName);
	if (pEvent == NULL)
	{
		pEvent = new CV7FilterItem();
		pEvent->SetName(szName);
		CCollection::Add(pEvent);
	}
	return pEvent;
}


CV7FilterItem* CLogFilterItemsCollection::Insert(int nIndex, LPCSTR szName)
{
	return NULL;
}


CV7FilterItem* CLogFilterItemsCollection::GetAt(int nIndex) const
{
	CCollectionItem* pCollectionItem = GetItem(nIndex);
	if (pCollectionItem)
		return static_cast<CV7FilterItem*>(pCollectionItem);
	else
		return NULL;
}


CV7FilterItem* CLogFilterItemsCollection::GetAt(LPCSTR szName) const
{
	CCollectionItem* pCollectionItem = GetItem(szName);
	if (pCollectionItem)
		return static_cast<CV7FilterItem*>(pCollectionItem);
	else
		return NULL;
}


int CLogFilterItemsCollection::IndexOf(LPCSTR szName) const
{
	return GetIndex(szName);
}


int CLogFilterItemsCollection::IndexOf(CV7FilterItem* pLogEvent) const
{
	return GetIndex(pLogEvent);
}


BOOL CLogFilterItemsCollection::Remove(int nIndex)
{
	if (nIndex < 0 || nIndex > GetUpperBound())
		return FALSE;
	CCollection::Remove(nIndex);
	return TRUE;
}


BOOL CLogFilterItemsCollection::Move(int nIndex, int nOffset)
{
	return FALSE;
}


void CLogFilterItemsCollection::RemoveAll()
{
	Clear();
}


void CLogFilterItemsCollection::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
	CV7FilterItem* pEvent=static_cast<CV7FilterItem*>(pItem);
	pEvent->DecrRef();
}


//////////////////////////////////////////////////////////////////////
// CV7LogFilter class
//////////////////////////////////////////////////////////////////////
int CV7LogFilter::m_nMethodSet=0;
const TCHAR* CV7LogFilter::m_Set[][2]={ _T("Set"), _T("”становить") };


IMPLEMENT_DYNAMIC(CV7LogFilter,CBLContext);

void CV7LogFilter::IncrRef()
{
	m_pLogProvider->IncrRef();
}


void CV7LogFilter::DecrRef()
{
	m_pLogProvider->DecrRef();
}


void CV7LogFilter::SetProvider(CV7LogProvider* pLogProvider)
{
	m_pLogProvider=pLogProvider;
}


void CV7LogFilter::Reset(const bool& bClearCollection)
{
	m_dwFilter=0;
	m_bInvalidFilter=FALSE;

	m_strMode.Empty();
	m_strCategory.Empty();
	m_strComment.Empty();
	m_strPresent.Empty();

	m_mapUser.RemoveAll();
	m_mapEventType.RemoveAll();
	m_mapEvent.RemoveAll();
	m_mapObject.RemoveAll();
	m_mapObjectKind.RemoveAll();

	if (bClearCollection)
		Clear();
}


void CV7LogFilter::GetModeAndCategory(const CIStringMapToIndex* const pMap, const bool bMode, CString& csCheck) const
{
	
	POSITION Pos=pMap->GetStartPosition();
	while (Pos!=NULL)
	{
		CString csKey;
		int nVal;
		pMap->GetNextAssoc(Pos,csKey,nVal);
		csKey.Empty();
		if (bMode)
		{
			nVal=LOG_DP_1C_Events[nVal].Modes;
			if (nVal & LOG_DP_RUNMODE_CONFIG)
				csKey+=_T('C');
			if (nVal & LOG_DP_RUNMODE_DEBUG)
				csKey+=_T('D');
			if (nVal & LOG_DP_RUNMODE_MONITOR)
				csKey+=_T('M');
			if (nVal & LOG_DP_RUNMODE_ENTERPRISE)
				csKey+=_T('E');
		}
		else
		{
			nVal=LOG_DP_1C_Events[nVal].Categories;
			if (nVal & LOG_DP_CATEGORY_SESSION)
				csKey+=_T('0');
			if (nVal & LOG_DP_CATEGORY_ADMIN)
				csKey+=_T('1');
			if (nVal & LOG_DP_CATEGORY_DATACHANGE)
				csKey+=_T('2');
			if (nVal & LOG_DP_CATEGORY_INFORMATION)
				csKey+=_T('3');
			if (nVal & LOG_DP_CATEGORY_WARNING)
				csKey+=_T('4');
			if (nVal & LOG_DP_CATEGORY_ERROR)
				csKey+=_T('5');
		}
		if (nVal=csKey.GetLength())
		{
			for (int i=0; i<nVal; i++)
			{
				TCHAR chSymb=csKey[i];
				if (csCheck.Find(chSymb) == -1)
					csCheck+=chSymb;
			}
		}
	}
}


void CV7LogFilter::AddFilterItem(unsigned char& nFilterType,
								 int& nCount,
								 CValue& Value)
{
	CString csName;
	csName.Format(LOG_DP_1C_LogFields[_ROW_FIELD_INDEX_(nFilterType)][GetMetaData()->GetTaskDef()->GetDefaultLanguage()]+CString(_T("_%u")),++nCount);
	if (CV7FilterItem* pItem=Add(csName))
	{
		CValue vT=nFilterType;
		CValue vR;
		CValue* Param[]={&vT,&Value};
		pItem->SetTypeAndValue(vR,Param);
	}
}


bool CV7LogFilter::CheckFilter()
{
	bool bResult=true;
	if (bResult&&(m_strMode.GetLength()))
	{
		CString csCheck;
		csCheck.Empty();
		GetModeAndCategory(&m_mapEventType,true,csCheck);
		GetModeAndCategory(&m_mapEvent,true,csCheck);
		GetModeAndCategory(&m_mapObject,true,csCheck);
		GetModeAndCategory(&m_mapObjectKind,true,csCheck);
		if (int nLen=csCheck.GetLength())
		{
			int i=0;
			while (i<nLen)
			{
				if (bResult = m_strMode.Find(csCheck[i]) != -1)
					break;
				else
					i++;
			}
		}
		if (bResult&&(m_strPresent.GetLength()))
			bResult = m_strMode.Find(_T('E')) != -1;
	}
	if (bResult&&(m_strCategory.GetLength()))
	{
		CString csCheck;
		csCheck.Empty();
		GetModeAndCategory(&m_mapEventType,false,csCheck);
		GetModeAndCategory(&m_mapEvent,false,csCheck);
		GetModeAndCategory(&m_mapObject,false,csCheck);
		GetModeAndCategory(&m_mapObjectKind,false,csCheck);
		if (int nLen=csCheck.GetLength())
		{
			int i=0;
			while (i<nLen)
			{
				if (bResult = m_strCategory.Find(csCheck[i]) != -1)
					break;
				else
					i++;
			}
		}
	}
	return bResult;
}


bool CV7LogFilter::SetFilter(const bool& bUseBuffer)
{
	bool bResult;

	if (bResult=CheckFilter())
	{
		m_dwFilter=0;
		Clear();

		for (UCHAR nFilterType=LOG_DP_FILTER_USER; nFilterType<=LOG_DP_FILTER_OBJECTKIND; nFilterType++)
		{
			int nCount=0, nSize=0, nIndex;
			CString csKey;

			switch (nFilterType)
			{
			case LOG_DP_FILTER_USER:
				if (nSize=m_mapUser.GetCount())
				{
					POSITION Pos=m_mapUser.GetStartPosition();
					while (Pos)
					{
						CObject* pObj;
						m_mapUser.GetNextAssoc(Pos,csKey,pObj);
						AddFilterItem(nFilterType,nCount,CValue(csKey));
					}
				}
				break;
			case LOG_DP_FILTER_MODE:
				if (nSize=m_strMode.GetLength())
				{
					while (nCount<nSize)
					{
						csKey=m_strMode[nCount];
						AddFilterItem(nFilterType,nCount,CValue(csKey));
					}
				}
				break;
			case LOG_DP_FILTER_CATEGORY:
				if (nSize=m_strCategory.GetLength())
				{
					while (nCount<nSize)
						AddFilterItem(nFilterType,nCount,CValue(m_strCategory[nCount]-48));
				}
				break;
			case LOG_DP_FILTER_EVENTTYPE:
				if (nSize=m_mapEventType.GetCount())
				{
					POSITION Pos=m_mapEventType.GetStartPosition();
					while (Pos)
					{
						m_mapEventType.GetNextAssoc(Pos,csKey,nIndex);
						if (CV7LogEventType* pEventType=m_pLogProvider->GetEventTypeByName(csKey))
						{
							CValue v;
							v.AssignContext(pEventType);
							AddFilterItem(nFilterType,nCount,v);
						}
						else // введено по-старому, нет в коллекции типов событий
							AddFilterItem(nFilterType,nCount,CValue(csKey));
					}
				}
				break;
			case LOG_DP_FILTER_EVENT:
				if (nSize=m_mapEvent.GetCount())
				{
					POSITION Pos=m_mapEvent.GetStartPosition();
					while (Pos)
					{
						m_mapEvent.GetNextAssoc(Pos,csKey,nIndex);
						if (CV7LogEvent* pEvent=m_pLogProvider->GetEventByName(csKey))
						{
							CValue v;
							v.AssignContext(pEvent);
							AddFilterItem(nFilterType,nCount,v);
						}
						else // нет в коллекции событий
							AddFilterItem(nFilterType,nCount,CValue(csKey));
					}
				}
				break;
			case LOG_DP_FILTER_COMMENT:
				if (nSize=m_strComment.GetLength())
					AddFilterItem(nFilterType,nCount,CValue(m_strComment));
				break;
			case LOG_DP_FILTER_OBJECT:
				if (nSize=m_mapObject.GetCount())
				{
					POSITION Pos=m_mapObject.GetStartPosition();
					while (Pos)
					{
						m_mapObject.GetNextAssoc(Pos,csKey,nIndex);
						CValue v;
						LogValueFromString(v,csKey,1);
						AddFilterItem(nFilterType,nCount,v);
					}
				}
				break;
			case LOG_DP_FILTER_OBJECTKIND:
				if (nSize=m_mapObjectKind.GetCount())
				{
					POSITION Pos=m_mapObjectKind.GetStartPosition();
					while (Pos)
					{
						m_mapObjectKind.GetNextAssoc(Pos,csKey,nIndex);
						nIndex=csKey.GetLength();
						if (csKey[0]==_T('E'))
						{
							if (nIndex==1)
							{
								csKey=GetMetaData()->GetTaskDef()->GetDefaultLanguage() ? _T("ѕеречисление") : _T("Enum");
								nIndex=0;
							}
						}
						if (nIndex)
						{
							if (nIndex>1)
								csKey+=_T("/0");
							CValue v;
							LogValueFromString(v,csKey,1);
							csKey=v.GetTypeString();

							if (nIndex>1)
							{
								CMetaDataObj* pObj=NULL;
								switch (v.GetTypeCode()) 
								{
								case ENUM_TYPE_1C:
									pObj=pMetaDataCont->GetEnumDefs()->GetItem(v.m_mdid);
									break;
								case REFERENCE_TYPE_1C:
									pObj=pMetaDataCont->GetSTypeDefs()->GetItem(v.m_mdid);
									break;
								case DOCUMENT_TYPE_1C:
									pObj=pMetaDataCont->GetDocDefs()->GetItem(v.m_mdid);
									break;
								case CALENDAR_TYPE_1C:
									pObj=pMetaDataCont->GetCalendarTypeDefs()->GetItem(v.m_mdid);
									break;
								case CALCULATIONKIND_TYPE_1C:
									pObj=pMetaDataCont->GetAlgorithmDefs()->GetItem(v.m_mdid);
									break;
								case ACCOUNT_TYPE_1C:
									{
										CBuhDef* pBuhDef=pMetaDataCont->GetBuhDef();
										if (pBuhDef)
											pObj=pBuhDef->GetPlanDef(v.m_mdid);
									}
									break;
								default:
									break;
								}
								if (pObj)
								{
									csKey+=_T('.');
									csKey+=pObj->m_Code;
								}
							}
						}
						AddFilterItem(nFilterType,nCount,CValue(csKey));
					}
				}
				break;
			case LOG_DP_FILTER_PRESENT:
				if (nSize=m_strPresent.GetLength())
					AddFilterItem(nFilterType,nCount,CValue(m_strPresent));
				break;
			}

			if (nSize)
				m_dwFilter=m_dwFilter|(1<<(nFilterType-1));
		}

		m_bInvalidFilter=FALSE;

		if (m_dwFilter&&bUseBuffer)
			m_dwFilter=m_dwFilter|(1<<(LOG_DP_FILTER_USING_BUFFER-1));
	}
	else
		m_bInvalidFilter=TRUE;

	return bResult;
}


void CV7LogFilter::ChangeFilter(const unsigned char nFilterType,
								const int& nIndex,
								CString& csKey,
								const bool& bAddFilter)
{
	switch (nFilterType)
	{
	case LOG_DP_FILTER_USER:
		if (bAddFilter)
			m_mapUser.SetAt(csKey,NULL);
		else
			m_mapUser.RemoveKey(csKey);
		break;
	case LOG_DP_FILTER_MODE:
		m_strMode=GetStringFromTmpl(m_strMode, csKey, _T("CMDE"), bAddFilter);
		break;
	case LOG_DP_FILTER_EVENTTYPE:
		if (bAddFilter)
		{
			m_mapEventType.SetAt(csKey,nIndex);
			if (nIndex >= LOG_DP_FOR_BACKWARD_COMPATIBILITY)
			{
				int i=nIndex+1;
				while (LOG_DP_1C_Events[i].Parent)
				{
					if (GetResourceString(csKey,LOG_DP_1C_Events[i].ResourceID,erpIdentifier))
						m_mapEvent.RemoveKey(csKey);
					i++;
				}
			}
		}
		else
			m_mapEventType.RemoveKey(csKey);
		break;
	case LOG_DP_FILTER_EVENT:
		if (bAddFilter)
		{
			m_mapEvent.SetAt(csKey,nIndex);
			if (nIndex >= LOG_DP_FOR_BACKWARD_COMPATIBILITY)
			{
				if (GetResourceString(csKey,LOG_DP_1C_Events[nIndex].Parent,erpIdentifier))
					m_mapEventType.RemoveKey(csKey);
			}
		}
		else
			m_mapEvent.RemoveKey(csKey);
		break;
	case LOG_DP_FILTER_CATEGORY:
		m_strCategory=GetStringFromTmpl(m_strCategory, csKey, _T("012345"), bAddFilter);
		break;
	case LOG_DP_FILTER_COMMENT:
		csKey.MakeLower();
		m_strComment=csKey;
		break;
	case LOG_DP_FILTER_OBJECT:
		if (bAddFilter)
			m_mapObject.SetAt(csKey,nIndex);
		else
			m_mapObject.RemoveKey(csKey);
		break;
	case LOG_DP_FILTER_PRESENT:
		csKey.MakeLower();
		m_strPresent=csKey;
		break;
	case LOG_DP_FILTER_OBJECTKIND:
		{
			CType t(0);
			if (CMetaDataWork::SetTypeAndKind(t,csKey))
			{
				if (GetStringPresent(&CValue(t),FALSE,csKey))
				{
					if (bAddFilter)
						m_mapObjectKind.SetAt(csKey,nIndex);
					else
						m_mapObjectKind.RemoveKey(csKey);
				}
			}
		}
		break;
	}
}


bool CV7LogFilter::SetFilterEx(const bool& bUseBuffer)
{
	Reset(false);

	for (int i=0; i<GetSize(); i++)
	{
		if (CV7FilterItem* pFilterItem=GetAt(i))
		{
			CValue v;

			pFilterItem->GetPropType(v);
			unsigned char nFilterType=v.GetNumeric();
			int nIndex=nFilterType-LOG_DP_FILTER_USER;

			pFilterItem->GetPropValue(v);

			CString csKey;
			csKey.Empty();

			switch (nFilterType)
			{
			case LOG_DP_FILTER_USER:
			case LOG_DP_FILTER_MODE:
			case LOG_DP_FILTER_CATEGORY:
			case LOG_DP_FILTER_COMMENT:
			case LOG_DP_FILTER_PRESENT:
				csKey=v.GetString();
				break;
			case LOG_DP_FILTER_EVENTTYPE:
				if (CV7LogEventType* pCont=static_cast<CV7LogEventType*>(v.GetContext()))
				{
					nIndex=pCont->GetIndex();
					pCont->GetPropEventID(v);
					csKey=v.GetString();
				}
				else
				{
					nIndex=LOG_DP_EVENT_TYPE_USER_INDEX_BC;
					csKey=v.GetString();
				}
				break;
			case LOG_DP_FILTER_EVENT:
				if (CV7LogEvent* pCont=static_cast<CV7LogEvent*>(v.GetContext()))
				{
					nIndex=pCont->GetIndex();
					pCont->GetPropEventID(v);
					csKey=v.GetString();
				}
				else
				{
					nIndex=LOG_DP_EVENT_USER_MSG_INDEX_BC;
					csKey=v.GetString();
				}
				break;
			case LOG_DP_FILTER_OBJECT:
				if (!GetStringPresent(&v,TRUE,csKey))
					csKey.Empty();
				break;
			case LOG_DP_FILTER_OBJECTKIND:
				{
					CType t(0);
					if (CMetaDataWork::SetTypeAndKind(t,v.GetString()))
					{
						if (!GetStringPresent(&CValue(t),FALSE,csKey))
							csKey.Empty();
					}
				}
				break;
			}
			if (!csKey.IsEmpty())
			{
				ChangeFilter(nFilterType,nIndex,csKey,true);
				m_dwFilter=m_dwFilter|(1<<(nFilterType-1));
			}
		}
	}
	bool bResult;
	
	if (bResult=this->CheckFilter())
	{
		m_bInvalidFilter=FALSE;

		if (m_dwFilter&&bUseBuffer)
			m_dwFilter=m_dwFilter|(1<<(LOG_DP_FILTER_USING_BUFFER-1));

		m_pLogProvider->ApplyFilter();
	}
	else
		m_bInvalidFilter=TRUE;

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
// вспомогательные функции
//////////////////////////////////////////////////////////////////////////
CString GetStringFromTmpl(CString csFindIn, CString& csParam, const CString& csTemplate, const bool& bAddFilter)
{
	csParam.MakeUpper();
	csParam=csParam.SpanIncluding(csTemplate);
	if (int nLen=csParam.GetLength())
	{
		if (!bAddFilter && csFindIn.IsEmpty())
			csFindIn=csTemplate;
		for (int i=0; i<nLen; i++)
		{
			TCHAR chSymb=csParam[i];
			if (bAddFilter)
			{
				if (csFindIn.Find(chSymb) == -1)
					csFindIn+=chSymb;
			}
			else
				csFindIn.Remove(chSymb);
		}
		if (bAddFilter && csFindIn.GetLength()==csTemplate.GetLength())
			csFindIn.Empty();
	}
	return csFindIn;
}


BOOL GetStringPresent(const CValue* const val, const BOOL bFull, CString& csResult)
{
	TCHAR chType;
	switch (val->GetTypeCode())
	{
	case ENUM_TYPE_1C:
		chType=_T('E');
		break;
	case REFERENCE_TYPE_1C:
		chType=_T('B');
		break;
	case DOCUMENT_TYPE_1C:
		chType=_T('O');
		break;
	case CALENDAR_TYPE_1C:
		chType=_T('C');
		break;
	case CALCULATIONKIND_TYPE_1C:
		chType=_T('A');
		break;
	case ACCOUNT_TYPE_1C:
		chType=_T('T');
		break;
	case SUBCONTOKIND_TYPE_1C:
		chType=_T('K');
		break;
	case CHART_OF_ACC_TYPE_1C:
		chType=_T('P');
		break;
	default:
		chType=0;
	}

	csResult.Empty();

	if (chType)
	{
		csResult=chType;
		char buf[20];
		if (long nKind=val->GetTypeID())
			csResult+="/"+CString(ltoa(nKind,buf,10));
		else
		{
			if (bFull)
				csResult+="/0";
		}
		if (bFull)
		{
			CString strSign=val->m_ObjID.GetDBSign();
			if (strSign.Compare("   "))
				csResult+="/("+strSign+")";
			else
				csResult+="/";
			long nID=val->m_ObjID.GetlObjID();
			csResult+=CString(ltoa(nID,buf,10));
		}
	}
	return !csResult.IsEmpty();
}
