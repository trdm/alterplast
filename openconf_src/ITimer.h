// ITimer.h: interface for the CITimer class.
#if !defined(AFX_ITIMER_H__56F5B92B_8012_4798_A96E_777A2F793CE0__INCLUDED_)
#define AFX_ITIMER_H__56F5B92B_8012_4798_A96E_777A2F793CE0__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CITimer : public CDispImpl<ICfgTimer, CfgTimer>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ICfgTimer)
	END_COM_MAP()

	CITimer();
	virtual ~CITimer();
	STDMETHOD(get_Time)(long Num,long *pVal);
	STDMETHOD(SetTimer)(long Interval,VARIANT_BOOL OneTimeOnly,long *TimerID);
	STDMETHOD(KillTimer)(long TimerID);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(KillAll)();
	STDMETHOD(get_ID)(long Num,long *pVal);

	
	static CITimer* m_pTimer;
	static void Done(){if(m_pTimer)	delete m_pTimer;}
	static VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);

	CMapPtrToPtr m_timerIDs;
	CMapPtrToPtr m_OneTimers;

};

#endif // !defined(AFX_ITIMER_H__56F5B92B_8012_4798_A96E_777A2F793CE0__INCLUDED_)
