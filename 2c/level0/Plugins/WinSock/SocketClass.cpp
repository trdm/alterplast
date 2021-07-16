// SocketClass.cpp: implementation of the CSocketClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketClass.h"
#include <process.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketClass::CSocketClass()
{
	m_bConnect = FALSE;
	m_bInProgress = FALSE;

	m_hSocket = NULL;
	m_hEvent = NULL;
	m_hThread = NULL;
	InitializeCriticalSection(&m_criticalSection);
	m_hKillEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_nCnt = 0;
}

CSocketClass::~CSocketClass()
{
    Stop();
	closesocket(m_hSocket);	
	WSACloseEvent(m_hEvent);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_criticalSection);
    CloseHandle(m_hKillEvent);
}


bool CSocketClass::Init(const CString& strIPAddr, int nPort, NOTIFYPROC2 pNotifyProc,LPVOID hWnd, CValue vRunContext)
{
	m_pNotifyProc	= pNotifyProc;

	m_nPort = nPort;
	m_strIPAddr = strIPAddr;

	m_hWnd = hWnd;
	m_vRunContext=vRunContext;

	
	m_hThread=
			(HANDLE)_beginthreadex(NULL,									// Security
				 0,											// Stack size - use default
				 SocketThreadProc,     			// Thread fn entry point
				 (void*) this,	      
				 0,											// Init flag
				 &m_dwThreadId);					// Thread address
	


	return TRUE;
}

void CSocketClass::Stop()
{
    ::SetEvent(m_hKillEvent);
    WaitForSingleObject(m_hThread, INFINITE);
}

unsigned CSocketClass::SocketThreadProc(LPVOID lParam)
{

	CSocketClass* pThis = reinterpret_cast<CSocketClass*>(lParam);


	WSANETWORKEVENTS events;

	bool bExit = false;
	while(!bExit)
	{
		//
		// Wait for something to happen
		//
        if (WaitForSingleObject(pThis->m_hKillEvent, 100) == WAIT_OBJECT_0)
            break;


			if (!pThis->Connect())
		{
			Sleep(1000);
			continue;
		}

		DWORD dwRet;
		dwRet = WSAWaitForMultipleEvents(1,
									 &pThis->m_hEvent,
									 FALSE,
									 100,
									 FALSE);

		if (dwRet == WSA_WAIT_TIMEOUT)
			continue;

		//
		// Figure out what happened
		//
		int nRet = WSAEnumNetworkEvents(pThis->m_hSocket,
								 pThis->m_hEvent,
								 &events);
		
		if (nRet == SOCKET_ERROR)
		{
			CString str;
			str.Format("WSAEnumNetworkEvents error %ld\n",WSAGetLastError());
			//::PostMessage(pThis->m_hWnd, WM_STATUS_MSG, 0,(LPARAM) AllocBuffer(str));
			pThis->m_pNotifyProc((LPVOID) pThis->m_hWnd, pThis->m_vRunContext, NC_CLIENT_ERROR,str);

			break;
		}

		//				 //
		// Handle events //
		//				 //

		// ACCEPT
		bExit = pThis->ProcessNetworkMessage(events.lNetworkEvents);

	} // while....

	return 0; // Normal Thread Exit Code...
}

bool CSocketClass::OnRead()
{
	Receive(1);

	return false;
}
CString CSocketClass::Receive(int nSendMessage)
{

	DWORD dwSize = 0;
	int nRet = ioctlsocket(m_hSocket, FIONREAD, &dwSize);

	if (nRet == -1)
		return "";

	if (dwSize == 0)
		return "";


	BYTE* pData = new BYTE[dwSize+1];
	
	int nRead = recv(m_hSocket, (char*) pData, dwSize, 0);
	if (nRead == -1)
		return "";


	m_recvBuff.Write(pData, nRead);
	delete [] pData;

	while (m_recvBuff.GetBufferLen() > (sizeof(int)*1))
	{
		int nSize = 0;
		int nCommand = 0;
		int nPageId = 0;
		CopyMemory(&nSize, m_recvBuff.GetBuffer(), sizeof(int));
		

		if (nSize && m_recvBuff.GetBufferLen() >= nSize)
		{
			// Read off header
			m_recvBuff.Read((PBYTE) &nSize, sizeof(int));

		////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////
			// SO you would process your data here
			// 
			// I'm just going to post message so we can see the data
			//nSize -= sizeof(int);

			int nTrueSize = nSize+1;
			PBYTE pData = new BYTE[nTrueSize];
			
			CString str;

			m_recvBuff.Read(pData,nSize);
			pData[nTrueSize-1] =  NULL;
			str = pData;
			// Clean Up
			delete [] pData;

			if(nSendMessage)
			{
				//::PostMessage(m_hWnd, WM_REC_MSG, 0, (LPARAM) AllocBuffer(str));
				m_pNotifyProc((LPVOID) m_hWnd, m_vRunContext, NC_RECEIVE,str);
			}
			else
			{
				return str;
			}
			m_bReadHeader = TRUE;
		}
		else
			break;
	}


	return "";
}

bool CSocketClass::OnWrite()
{
	return false;
}
bool CSocketClass::OnClose()
{
	m_bConnect = FALSE;
	//::PostMessage(m_hWnd, WM_STATUS_MSG, 0, (LPARAM) AllocBuffer("Client Disconnected"));
	m_pNotifyProc((LPVOID) m_hWnd, m_vRunContext, NC_CLIENT_DISCONNECT,"");
	return false;
}
bool CSocketClass::OnConnect()
{
	m_bConnect = TRUE;
	m_bInProgress = FALSE;
	//::PostMessage(m_hWnd, WM_STATUS_MSG, 0, (LPARAM) AllocBuffer("Client Connected"));
	m_pNotifyProc((LPVOID) m_hWnd, m_vRunContext, NC_CLIENT_CONNECT,"");
	return false;
}

BOOL CSocketClass::Connect()
{
	if  (m_bConnect || m_bInProgress)
		return TRUE;

	m_bReadHeader = TRUE;

	if (m_hEvent)
	{
		WSACloseEvent(m_hEvent);
		m_hEvent = NULL;
	}
	if (m_hSocket)
	{
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}
	

	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_hSocket == INVALID_SOCKET)
	{
		TRACE("Could not create  socket %ld\n",WSAGetLastError());
		return FALSE;
	}


	SOCKADDR_IN		saServer;		

	memset(&saServer,0,sizeof(saServer));

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(m_strIPAddr);
	saServer.sin_port = htons(m_nPort);
	int nRet = connect(m_hSocket,(sockaddr*)&saServer, sizeof(saServer));

	if (nRet == SOCKET_ERROR &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		TRACE("connect() error %ld\n",WSAGetLastError());

		closesocket(m_hSocket);

		return FALSE;
	}
	
	
	m_bInProgress = TRUE;

	m_hEvent = WSACreateEvent();
	if (m_hEvent == WSA_INVALID_EVENT)
	{
		closesocket(m_hSocket);
		return FALSE;
	}

	//
	// Request async notification
	//
	nRet = WSAEventSelect(m_hSocket,
						  m_hEvent,
						  FD_CONNECT|FD_CLOSE|FD_READ);
	
	if (nRet == SOCKET_ERROR)
	{
		closesocket(m_hSocket);

		return FALSE;
	}

	return TRUE;
}



void CSocketClass::SendData(CString strData)
{
	EnterCriticalSection(&m_criticalSection);

//	CString strHdr;
//	CString strPkt;


	int nSize = strData.GetLength();

	m_sendBuff.Write((PBYTE) &nSize, sizeof(nSize));
	m_sendBuff.Write((PBYTE) strData.GetBuffer(nSize), nSize);

	DoAsyncSendBuff();
	LeaveCriticalSection(&m_criticalSection);
}



void CSocketClass::DoAsyncSendBuff()
{
	
	int nSendDataLen = m_sendBuff.GetBufferLen();

	
	m_nBytesSent = 0;
	
	while (m_nBytesSent < nSendDataLen)  
	{
		int nBytes;

		if ((nBytes = send(m_hSocket, (LPCTSTR)m_sendBuff.GetBuffer(), (nSendDataLen - m_nBytesSent),0))
			== SOCKET_ERROR)
		{
			if (GetLastError() == WSAEWOULDBLOCK) 
			{
				TRACE("Blocking\n");
				OutToDebug("Blocking\n");
				Sleep(1);
				break;
			}
			else
			{
				TCHAR szError[256];
				wsprintf(szError, _TEXT("Server Socket failed to send: %s"), GetGetLastStringError());
				OutToDebug("Error: %s\n",szError);
				
				closesocket(m_hSocket);
								
				m_nBytesSent = 0;				
				return;
			}
		}
		else
		{
			m_nBytesSent += nBytes;
			m_sendBuff.Delete(nBytes);
		}
	}
}

