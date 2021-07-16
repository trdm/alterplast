// SocketClass.h: interface for the CSocketClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETCLASS_H__1246136D_3FCD_4E8A_A4B8_BEAEADE7F3A6__INCLUDED_)
#define AFX_SOCKETCLASS_H__1246136D_3FCD_4E8A_A4B8_BEAEADE7F3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define WM_STATUS_MSG	WM_APP + 0x1001
#define WM_REC_MSG		WM_APP + 0x1002

#include "Mapping.h"
#include "Buffer2.h"

typedef void (CALLBACK* NOTIFYPROC2)(LPVOID, CValue vRunContext, UINT nCode, CString csData);

class CSocketClass : public CNetworkMessageMap 
{
public:
	CSocketClass();
	virtual ~CSocketClass();

	bool Init(const CString& strIPAddr, int nPort, NOTIFYPROC2 pNotifyProc, LPVOID hWnd, CValue vRunContext);
	void SendData(CString strData);
    void  Stop();
	BOOL Connect();

	BOOL				m_bInProgress;
	BOOL				m_bConnect;
	WSAEVENT			m_hEvent;
	DWORD				m_dwThread;
    HANDLE              m_hKillEvent;

	HANDLE				m_hThread;
	UINT				m_dwThreadId;

	CRITICAL_SECTION	m_criticalSection;
	SOCKET				m_hSocket;
	UINT				m_nPort;
	CString				m_strIPAddr;
	BOOL				m_bReadHeader;
	int					m_nBlockSize;
	int					m_nTotalRead;
	CString				m_strBlock;

	LPVOID				m_hWnd;
	UINT				m_nCnt;
	

	static unsigned __stdcall SocketThreadProc(LPVOID lParam);

//////////////////////////////////////////////////
// Message Map Begin
//////////////////////////////////////////////////
	BEGIN_NETWORK_MSG_MAP(CSocketClass)
		NETWORK_MESSAGE_HANDLER(FD_READ,  OnRead)
		NETWORK_MESSAGE_HANDLER(FD_CLOSE, OnClose)
		NETWORK_MESSAGE_HANDLER(FD_CONNECT, OnConnect)
		NETWORK_MESSAGE_HANDLER(FD_CLOSE, OnClose)
	END_NETWORK_MSG_MAP()

	net_msg virtual bool OnRead();
	net_msg virtual bool OnWrite();
	net_msg virtual bool OnConnect();
	net_msg virtual bool OnClose();

	void CSocketClass::DoAsyncSendBuff();

	CString Receive(int nSendMessage=0);

//////////////////////////////////////////////////

	CBuffer	m_sendBuff;
	CBuffer	m_recvBuff;

	INT m_nSendDataLen;
	INT	m_nBytesSent;

	NOTIFYPROC2		m_pNotifyProc;
	CValue			m_vRunContext;

};

#endif // !defined(AFX_SOCKETCLASS_H__1246136D_3FCD_4E8A_A4B8_BEAEADE7F3A6__INCLUDED_)
