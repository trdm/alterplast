// IOCPServer.h: interface for the CIOCPServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPSERVER_H__75B80E90_FD25_4FFB_B273_0090AA43BBDF__INCLUDED_)
#define AFX_IOCPSERVER_H__75B80E90_FD25_4FFB_B273_0090AA43BBDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "Buffer2.h"
#include "CpuUsage.h"


#include <process.h>

#include <afxtempl.h>


class CLock
{
public:
	CLock(CRITICAL_SECTION& cs, const CString& strFunc)
	{
		m_strFunc = strFunc;
		m_pcs = &cs;
		Lock();
	}
	~CLock()
	{
		Unlock();

	}
	
	void Unlock()
	{
		LeaveCriticalSection(m_pcs);
		TRACE(_T("LC %d %s\n") , GetCurrentThreadId() , m_strFunc);
	}

	void Lock()
	{
		TRACE(_T("EC %d %s\n") , GetCurrentThreadId(), m_strFunc);
		EnterCriticalSection(m_pcs);
	}


protected:
	CRITICAL_SECTION*	m_pcs;
	CString				m_strFunc;

};

enum IOType 
{
	IOInitialize,
	IORead,
	IOWrite,
	IOIdle
};


class OVERLAPPEDPLUS 
{
public:
	OVERLAPPED			m_ol;
	IOType				m_ioType;

	OVERLAPPEDPLUS(IOType ioType) {
		ZeroMemory(this, sizeof(OVERLAPPEDPLUS));
		m_ioType = ioType;
	}
};

struct ClientContext
{
/*	ClientContext()
	{
		ClientContextID++;
		ID=ClientContextID;
	};*/

	int					ID;

    SOCKET				m_Socket;

	// Store buffers
	CBuffer				m_ReadBuffer;
	CBuffer				m_WriteBuffer;

	// Input Elements for Winsock
	WSABUF				m_wsaInBuffer;
	BYTE				m_byInBuffer[BUF_SIZE];

	// Output elements for Winsock
	WSABUF				m_wsaOutBuffer;
	HANDLE				m_hWriteComplete;

	ClientContext*		m_pWriteContext;
	ClientContext*		m_pReadContext;

};

template<>
inline UINT AFXAPI HashKey(CString & strGuid)
{
  return HashKey( (LPCTSTR) strGuid);         
}

#include "Mapper.h"

typedef void (CALLBACK* NOTIFYPROC)(LPVOID, CValue vRunContext, ClientContext*, UINT nCode);

typedef CMap<CString, CString&, ClientContext*, ClientContext* > ContextList;
typedef CList<ClientContext*, ClientContext* > FreeContextList;

class CMainFrame;

class CIOCPServer
{
public:
	void DisconnectAll();
	CIOCPServer();
	virtual ~CIOCPServer();
	void SetEchoMode();
	bool GetEchoMode();

	NOTIFYPROC					m_pNotifyProc;
	LPVOID						m_pFrame;
	CValue						m_vRunContext;
	
	bool Initialize(NOTIFYPROC pNotifyProc, LPVOID pFrame,  CValue vRunContext, int nConnections, int nPort);

	static unsigned __stdcall ListenThreadProc(LPVOID lpVoid);
	static unsigned __stdcall  ThreadPoolFunc (LPVOID WorkContext);
	
	static CRITICAL_SECTION	m_cs;

	void Send(const CString& strClient, CString strData);
	void Send(ClientContext* pContext, CString strData);
	void Shutdown();
	void ResetConnection(ClientContext* pContext);
	
	LONG					m_nCurrentThreads;
	LONG					m_nBusyThreads;


	void Stop();
	CString GetHostName(SOCKET socket);
protected:
	void InitializeClientRead(ClientContext* pContext);
	BOOL AssociateSocketWithCompletionPort(SOCKET device, HANDLE hCompletionPort, DWORD dwCompletionKey);
	void RemoveStaleClient(ClientContext* pContext, BOOL bGraceful);
	ClientContext* FindClient(const CString& strLogonID);
	void MoveToFreePool(CString& strKey);
	ClientContext*  AllocateContext();

	LONG				m_nWorkerCnt;


	bool				m_bInit;
	bool				m_bDisconnectAll;

	void CloseCompletionPort();
	void OnAccept();
	bool InitializeIOCP(void);

	ContextList				m_listContexts;
	FreeContextList			m_listFreePool;
	WSAEVENT				m_hEvent;
	SOCKET					m_socListen;    
    HANDLE					m_hKillEvent;
	HANDLE					m_hThread;
	HANDLE					m_hCompletionPort;
	bool					m_bTimeToKill;
	CCpuUsage				m_cpu;
	bool					m_bEcho;


	// Thread Pool Tunables
	LONG					m_nThreadPoolMin;
	LONG					m_nThreadPoolMax;
	LONG					m_nCPULoThreshold;
	LONG					m_nCPUHiThreshold;


	void CreateStream(ClientContext* pContext);

	BEGIN_IO_MSG_MAP()
		IO_MESSAGE_HANDLER(IORead, OnClientReading)
		IO_MESSAGE_HANDLER(IOWrite, OnClientWriting)
		IO_MESSAGE_HANDLER(IOInitialize, OnClientInitializing)
	END_IO_MSG_MAP()

	bool OnClientInitializing	(ClientContext* pContext, DWORD dwSize = 0);
	bool OnClientReading		(ClientContext* pContext, DWORD dwSize = 0);
	bool OnClientWriting		(ClientContext* pContext, DWORD dwSize = 0);

};

#endif // !defined(AFX_IOCPSERVER_H__75B80E90_FD25_4FFB_B273_0090AA43BBDF__INCLUDED_)
