// V7LogEvent.cpp: implementation of the CV7LogEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7LogEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// class CV7LogEvent
//////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CV7LogEvent)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7LogEvent)
	BL_PROP("Index",         "Индекс",          &GetPropIndex,        NULL)
	BL_PROP("Identifier",    "Идентификатор",   &GetPropEventID,      NULL)
	BL_PROP("Description",   "Описание",        &GetPropEventDescr,   NULL)
	BL_PROP("IsSystem",      "Системное",       &GetPropIsSystem,     NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7LogEvent, "LogEvent", "СобытиеЖурналаРегистрации", 0, NULL, NULL, -1);

CV7LogEvent::CV7LogEvent():m_nIndex(-1)
{
}


CV7LogEvent::~CV7LogEvent()
{
}


BOOL CV7LogEvent::GetPropIndex(CValue& Value) const
{
	if (m_nIndex != -1)
		Value=LOG_DP_1C_Events[m_nIndex].Index;
	else
		Value=-1;
	return TRUE;
}


BOOL CV7LogEvent::GetPropEventID(CValue& Value) const
{
	Value=GetName();
	return TRUE;
}


BOOL CV7LogEvent::GetPropEventDescr(CValue& Value) const
{
	if (IsSystemEventType())
	{
		CString csDescr;
		if (GetResourceString(csDescr,LOG_DP_1C_Events[m_nIndex].ResourceID,erpDescription))
			Value=csDescr;
	}
	else
		Value=GetName();
	return TRUE;
}


BOOL CV7LogEvent::GetPropIsSystem(CValue& Value) const
{
	Value = IsSystemEventType();
	return TRUE;
}


bool CV7LogEvent::IsSystemEventType() const
{
	return m_nIndex >= LOG_DP_FOR_BACKWARD_COMPATIBILITY;
}


//////////////////////////////////////////////////////////////////////////
// class CLogEventsCollection
//////////////////////////////////////////////////////////////////////////
CLogEventsCollection::CLogEventsCollection()
{
}


CLogEventsCollection::~CLogEventsCollection()
{
	Clear();
}


int CLogEventsCollection::GetSize() const
{
	return GetCount();
}


CV7LogEvent* CLogEventsCollection::Add(LPCSTR szName)
{
	CV7LogEvent* pEvent = new CV7LogEvent();
	pEvent->SetName(szName);
	CCollection::Add(pEvent);
	return pEvent;
}


CV7LogEvent* CLogEventsCollection::Insert(int nIndex, LPCSTR szName)
{
	return NULL;
}


CV7LogEvent* CLogEventsCollection::GetAt(int nIndex) const
{
	CCollectionItem* pCollectionItem = GetItem(nIndex);
	if (pCollectionItem)
		return static_cast<CV7LogEvent*>(pCollectionItem);
	else
		return NULL;
}


CV7LogEvent* CLogEventsCollection::GetAt(LPCSTR szName) const
{
	CCollectionItem* pCollectionItem = GetItem(szName);
	if (pCollectionItem)
		return static_cast<CV7LogEvent*>(pCollectionItem);
	else
		return NULL;
}


int CLogEventsCollection::IndexOf(LPCSTR szName) const
{
	return GetIndex(szName);
}


int CLogEventsCollection::IndexOf(CV7LogEvent* pLogEvent) const
{
	return GetIndex(pLogEvent);
}


BOOL CLogEventsCollection::Remove(int nIndex)
{
	if (nIndex < 0 || nIndex > GetUpperBound())
		return FALSE;
	CCollection::Remove(nIndex);
	return TRUE;
}


BOOL CLogEventsCollection::Move(int nIndex, int nOffset)
{
	return FALSE;
}


void CLogEventsCollection::RemoveAll()
{
	Clear();
}


void CLogEventsCollection::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
	CV7LogEvent* pEvent=static_cast<CV7LogEvent*>(pItem);
	pEvent->DecrRef();
}

//////////////////////////////////////////////////////////////////////////
// class CV7LogEventsCollection
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CV7LogEventsCollection,CBLContext);


//////////////////////////////////////////////////////////////////////////
// class CV7LogEventType
//////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CV7LogEventType)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7LogEventType)
	BL_PROP("Index",         "Индекс",          &GetPropIndex,        NULL)
	BL_PROP("Identifier",    "Идентификатор",   &GetPropEventID,      NULL)
	BL_PROP("Description",   "Описание",        &GetPropEventDescr,   NULL)
	BL_PROP("IsSystem",      "Системный",       &GetPropIsSystem,     NULL)
	BL_PROP("Events",        "События",         &GetPropEvents,       NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7LogEventType, "LogEventType", "ТипСобытияЖурналаРегистрации", 0, NULL, NULL, -1);

CV7LogEventType::CV7LogEventType():m_nIndex(-1),m_pEvents(NULL)
{
	m_pEvents.Create();
}


CV7LogEventType::~CV7LogEventType()
{
}


BOOL CV7LogEventType::GetPropIndex(CValue& Value) const
{
	if (m_nIndex != -1)
		Value=LOG_DP_1C_Events[m_nIndex].Index;
	else
		Value=-1;
	return TRUE;
}


BOOL CV7LogEventType::GetPropEventID(CValue& Value) const
{
	Value=GetName();
	return TRUE;
}


BOOL CV7LogEventType::GetPropEventDescr(CValue& Value) const
{
	if (IsSystemEventType())
	{
		CString csDescr;
		if (GetResourceString(csDescr,LOG_DP_1C_Events[m_nIndex].ResourceID,erpDescription))
			Value=csDescr;
	}
	else
		Value=GetName();
	return TRUE;
}


BOOL CV7LogEventType::GetPropEvents(CValue& Value) const
{
	if (m_pEvents)
		Value.AssignContext(static_cast<CV7LogEventsCollection*>(m_pEvents));
	else
		Value.Reset();
	return TRUE;
}


BOOL CV7LogEventType::GetPropIsSystem(CValue& Value) const
{
	Value = IsSystemEventType();
	return TRUE;
}


bool CV7LogEventType::IsSystemEventType() const
{
	return m_nIndex >= LOG_DP_FOR_BACKWARD_COMPATIBILITY;
}


void CV7LogEventType::SetIndex(int nIndex)
{
	m_nIndex=nIndex;
	if (IsSystemEventType() && m_pEvents.operator bool())
	{
		int nResourceID=LOG_DP_1C_Events[nIndex].ResourceID;
		for (nIndex=LOG_DP_FOR_BACKWARD_COMPATIBILITY; nIndex<LOG_DP_EVENTS_COUNT; nIndex++)
		{
			if (LOG_DP_1C_Events[nIndex].Parent == nResourceID)
			{
				CString csID;
				if (GetResourceString(csID,LOG_DP_1C_Events[nIndex].ResourceID,erpIdentifier))
				{
					if (CV7LogEvent* pEvent=m_pEvents->Add(csID))
						pEvent->SetIndex(nIndex);
				}
			}
		}
	}
}


void CV7LogEventType::RemoveUserMessages()
{
	if (CString(LOG_DP_EVENT_TYPE_USER).CompareNoCase(GetName()) == 0)
	{
		while (m_pEvents->GetCount() != 1)
			m_pEvents->Remove(m_pEvents->GetUpperBound());
	}
}


void CV7LogEventType::AddUserEvent(LPCSTR szName, const int& nIndex)
{
	CV7LogEvent* pEvent=m_pEvents->Add(szName);
	if (nIndex != -1)
		pEvent->SetIndex(nIndex);
}


CV7LogEvent* CV7LogEventType::FindEvent(LPCSTR szName) const
{
	if (m_pEvents.operator bool() && (szName != NULL))
		return m_pEvents->GetAt(szName);
	else
		return NULL;
}



//////////////////////////////////////////////////////////////////////////
// class CLogEventTypessCollection
//////////////////////////////////////////////////////////////////////////
CLogEventTypesCollection::CLogEventTypesCollection()
{
}


CLogEventTypesCollection::~CLogEventTypesCollection()
{
	Clear();
}


int CLogEventTypesCollection::GetSize() const
{
	return GetCount();
}


CV7LogEventType* CLogEventTypesCollection::Add(LPCSTR szName)
{
	CV7LogEventType* pEvent = new CV7LogEventType();
	pEvent->SetName(szName);
	CCollection::Add(pEvent);
	return pEvent;
}


CV7LogEventType* CLogEventTypesCollection::Insert(int nIndex, LPCSTR szName)
{
	return NULL;
}


CV7LogEventType* CLogEventTypesCollection::GetAt(int nIndex) const
{
	CCollectionItem* pCollectionItem = GetItem(nIndex);
	if (pCollectionItem)
		return static_cast<CV7LogEventType*>(pCollectionItem);
	else
		return NULL;
}


CV7LogEventType* CLogEventTypesCollection::GetAt(LPCSTR szName) const
{
	CCollectionItem* pCollectionItem = GetItem(szName);
	if (pCollectionItem)
		return static_cast<CV7LogEventType*>(pCollectionItem);
	else
		return NULL;
}


int CLogEventTypesCollection::IndexOf(LPCSTR szName) const
{
	return GetIndex(szName);
}


int CLogEventTypesCollection::IndexOf(CV7LogEventType* pEvent) const
{
	return GetIndex(pEvent);
}


BOOL CLogEventTypesCollection::Remove(int nIndex)
{
	if (nIndex < 0 || nIndex > GetUpperBound())
		return FALSE;
	CCollection::Remove(nIndex);
	return TRUE;
}


BOOL CLogEventTypesCollection::Move(int nIndex, int nOffset)
{
	return FALSE;
}


void CLogEventTypesCollection::RemoveAll()
{
	Clear();
}


void CLogEventTypesCollection::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
	CV7LogEventType* pEvent=static_cast<CV7LogEventType*>(pItem);
	pEvent->DecrRef();
}

//////////////////////////////////////////////////////////////////////////
// class CV7LogEventTypesCollection
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CV7LogEventTypesCollection,CBLContext);


CV7LogEventTypesCollection::CV7LogEventTypesCollection()
{
	for (int nIndex=LOG_DP_FOR_BACKWARD_COMPATIBILITY; nIndex<LOG_DP_EVENTS_COUNT; nIndex++)
	{
		if (LOG_DP_1C_Events[nIndex].Parent == 0)
		{
			CString csID;
			if (GetResourceString(csID,LOG_DP_1C_Events[nIndex].ResourceID,erpIdentifier))
			{
				if (CV7LogEventType* pEventType=Add(csID))
					pEventType->SetIndex(nIndex);
			}
		}
	}
	m_nSystemEventsCount=GetCount();
}


void CV7LogEventTypesCollection::AddUserEvents(CString csEventsSource)
{
	while (GetCount() != m_nSystemEventsCount)
		Remove(GetUpperBound());

	if (CV7LogEventType* pEventType=GetAt(LOG_DP_EVENT_TYPE_USER))
		pEventType->RemoveUserMessages();

	if (!csEventsSource.IsEmpty())
	{
		int nIndex=csEventsSource.ReverseFind(_T('\\'));
		csEventsSource = csEventsSource.Left(nIndex+1)+_T("1cv7evt.txt");

		CFile* pEvFile = new CFile();
		if (pEvFile->Open(csEventsSource, CFile::modeNoTruncate|CFile::modeRead|CFile::shareDenyNone))
		{
			csEventsSource.Empty();
			if (DWORD qwSize=pEvFile->GetLength())
			{
				qwSize=pEvFile->ReadHuge(csEventsSource.GetBuffer(qwSize),qwSize);
				csEventsSource.ReleaseBuffer(qwSize);
			}
			pEvFile->Close();
		}
		else
			csEventsSource.Empty();
		delete pEvFile;


		CStringList EventTypesList;
		CStringList EventsList;

		CString csEvent;

		enum SearchStatus {EventType,Event} Search=EventType;

		while (!csEventsSource.IsEmpty())
		{
			CString csSeparator;
			switch (Search)
			{
			case EventType:
				csSeparator=_T(';');
				break;
			case Event:
				csSeparator=_T("\r\n");
				break;
			}
			if ((csEventsSource[0] == _T('\x1')) || (csEventsSource[0] == _T('"')))
				csSeparator=_T('"')+csSeparator;
			nIndex=csEventsSource.Find(csSeparator,0);

			CString csStr=csEventsSource.Left(nIndex);
			csEventsSource=csEventsSource.Mid(nIndex+csSeparator.GetLength());

			csStr.TrimLeft();
			csStr.TrimRight();
			csStr.Remove(_T('"'));
			csStr.Replace(_T('\x1'),_T('"'));

			switch (Search)
			{
			case EventType:
				csEvent=csStr;
				if (EventTypesList.Find(csEvent) == NULL)
					EventTypesList.AddTail(csEvent);
				Search=Event;
				break;
			case Event:
				csEvent+=_T(';')+csStr;
				if (EventsList.Find(csEvent) == NULL)
					EventsList.AddTail(csEvent);
				Search=EventType;
				break;
			}
		}
		for (POSITION CurrEventType=EventTypesList.GetHeadPosition(); CurrEventType!=NULL; )
		{
			CString csEventType=EventTypesList.GetNext(CurrEventType);
			CV7LogEventType* pEventType=GetAt(csEventType);
			if (pEventType == NULL)
			{
				pEventType=Add(csEventType);
				pEventType->SetIndex(LOG_DP_EVENT_TYPE_USER_INDEX_BC);
			}
			if (pEventType)
			{
				for (POSITION CurrEvent=EventsList.GetHeadPosition(); CurrEvent!=NULL; )
				{
					csEvent=EventsList.GetNext(CurrEvent);
					if (csEventType.Compare(csEvent.Left(csEventType.GetLength())) == 0)
					{
						csEvent=csEvent.Mid(csEventType.GetLength()+1);
						pEventType->AddUserEvent(csEvent, csEvent.CompareNoCase(LOG_DP_EVENT_USER_MSG) == 0 ? LOG_DP_EVENT_USER_MSG_INDEX : LOG_DP_EVENT_USER_MSG_INDEX_BC);
					}
				}
			}
		}
		/*
		Msg(_T("ТИПЫ СОБЫТИЙ:"));
		for (POSITION Pos=EventTypesList.GetHeadPosition(); Pos!=NULL; )
			Msg((LPSTR)EventTypesList.GetNext(Pos).operator LPCTSTR());
		Msg(_T("\r\nСОБЫТИЯ:"));
		for (Pos=EventsList.GetHeadPosition(); Pos!=NULL; )
			Msg((LPSTR)EventsList.GetNext(Pos).operator LPCTSTR());
		*/
	}
}

//////////////////////////////////////////////////////////////////////////
// прочее
//////////////////////////////////////////////////////////////////////////
BOOL GetResourceString(CString& csResource, const int& nResourceID, const LOG_DP_EventResourcePart erpPart)
{
	csResource.Empty();

	if (nResourceID)
	{
		if (HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nResourceID), RT_STRING))
		{
			int nSize = 500;
			LPTSTR pBuf = csResource.GetBuffer(nSize);
			nSize=LoadString(hInst,nResourceID,pBuf,nSize);
			csResource.ReleaseBuffer(nSize);

			if (erpPart != erpCompletely)
			{
				nSize=csResource.Find(_T(','));
				if (nSize == -1)
					return FALSE;
				if (erpPart == erpIdentifier)
					csResource=csResource.Left(nSize);
				else
				{
					csResource=csResource.Mid(nSize+1);
					nSize=csResource.Find(_T(','));
					if (nSize != -1)
						csResource=csResource.Left(nSize);
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}
