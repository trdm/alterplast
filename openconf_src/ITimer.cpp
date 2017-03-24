// ITimer.cpp: implementation of the CITimer class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ITimer.h"
#include "IConfig.h"

ITypeInfo* CITimer::m_pTypeInfo=NULL;
CITimer* CITimer::m_pTimer=NULL;

CITimer::CITimer()
{
	m_pTimer=this;
}

CITimer::~CITimer()
{
	KillAll();
}

VOID CALLBACK CITimer::TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	CIConfigEvents::m_ConfigEvents.FireTimer(idEvent);
	if(m_pTimer->m_OneTimers[(void*)idEvent])
	{
		::KillTimer(NULL,idEvent);
		m_pTimer->m_OneTimers.RemoveKey((void*)idEvent);
	}
}

STDMETHODIMP CITimer::get_Time(long Num,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(Num<0 || Num>=m_timerIDs.GetCount())
		return SetError(E_FAIL,"Неправильный номер таймера");
	void *key,*time;
	POSITION pos=m_timerIDs.GetStartPosition();
	for(int i=-1;i<Num;i++)
		m_timerIDs.GetNextAssoc(pos,key,time);
	*pVal=(long)time;
	return S_OK;
}

STDMETHODIMP CITimer::get_ID(long Num,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(Num<0 || Num>=m_timerIDs.GetCount())
		return SetError(E_FAIL,"Неправильный номер таймера");
	void *key,*time;
	POSITION pos=m_timerIDs.GetStartPosition();
	for(int i=-1;i<Num;i++)
		m_timerIDs.GetNextAssoc(pos,key,time);
	*pVal=(long)key;
	return S_OK;

}

STDMETHODIMP CITimer::SetTimer(long Interval,VARIANT_BOOL OneTimeOnly,long *TimerID)
{
	if(IsBadWritePtr(TimerID,sizeof(*TimerID)))
		return E_POINTER;
	UINT id=::SetTimer(NULL,0,Interval,TimerProc);
	m_timerIDs[(void*)id]=(void*)Interval;
	if(OneTimeOnly)
		m_OneTimers[(void*)id]=(void*)id;
	*TimerID=(long)id;
	return S_OK;
}

STDMETHODIMP CITimer::KillTimer(long TimerID)
{
	if(m_timerIDs[(void*)TimerID])
	{
		m_timerIDs.RemoveKey((void*)TimerID);
		m_OneTimers.RemoveKey((void*)TimerID);
		::KillTimer(NULL,TimerID);
	}
	return S_OK;
}

STDMETHODIMP CITimer::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_timerIDs.GetCount();
	return S_OK;
}

STDMETHODIMP CITimer::KillAll()
{
	UINT id;
	void* interval;
	for(POSITION pos=m_timerIDs.GetStartPosition();pos;)
	{
		m_timerIDs.GetNextAssoc(pos,(void*&)id,interval);
		::KillTimer(NULL,id);
	}
	m_timerIDs.RemoveAll();
	m_OneTimers.RemoveAll();
	return S_OK;
}
