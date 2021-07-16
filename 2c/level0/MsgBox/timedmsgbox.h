#ifndef __TIMES_MESSAGE_BOX_H__
#define __TIMES_MESSAGE_BOX_H__

//
// copyrights and infos 
// see timedmsgbox.cpp
//

/////////////////////////////////////////////////////////////////////////
//
// CDlgTimedMessageBox
//
/////////////////////////////////////////////////////////////////////////

// some ideas taken from TimedMessageBox, see Q181934

// 
// calling:
// -     CDlgTimedMessageBox::TimedMessageBox()
// -  or create a class-instance and call .ShowMessageBox

class CDlgTimedMessageBox
{
public:
	CDlgTimedMessageBox(UINT flags, LPCTSTR ptszMessage, LPCTSTR ptszTitle, 
							DWORD dwTimeout, UINT dDefaultReturn,
							LPCTSTR ptszMessageTimer=NULL, HWND hwndParent=NULL);
	virtual ~CDlgTimedMessageBox();
	
	UINT		ShowMessageBox(BOOL *pbStoppedByUser=NULL);
	void		GetWindowHandles(void);
	void		LocalTimerProc(void);

	// to call the messagebox within one line !
	static UINT	TimedMessageBox(UINT flags, LPCTSTR ptszMessage, LPCTSTR ptszTitle, 
							DWORD dwTimeout, UINT dDefaultReturn,
							LPCTSTR ptszMessageTimer=NULL, HWND hwndParent=NULL, BOOL *pbStoppedByUser=NULL);

protected:
	// the map is needed in the GlobalTimerProc 
	// to find the corresponding class for a timer-id
	static		CMapPtrToPtr		m_mapTimerIdToClassMe;
	// used to get thread-save
	static		CCriticalSection	m_sectMap;
	static void CALLBACK GlobalTimerProc(HWND hwnd, UINT uiMsg, UINT_PTR idEvent, DWORD dwTime);

	HWND		m_hParent,
				m_hMsgBox,
				m_hStaticText,
				m_hDefaultButton;
	CString		m_Message,
				m_MessageTimer,
				m_Title,
				m_CurrentMessage;
	UINT		m_flags;
	DWORD		m_dwTimeout,
				m_dwStarted;
	UINT		m_idTimer;
	BOOL		m_bRunning;
	BOOL		m_bStoppedByTimer;
	UINT		m_DefaultReturn;
};


#endif
