// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include <atlconv.h>
#include "SocketManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
const UINT EVT_CONSUCCESS = 0x0000;	// Connection established
const UINT EVT_CONFAILURE = 0x0001;	// General failure - Wait Connection failed
const UINT EVT_CONDROP	  = 0x0002;	// Connection dropped
const UINT EVT_ZEROLENGTH = 0x0003;	// Zero length message
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager()
{

}

CSocketManager::~CSocketManager()
{

}




inline LPCTSTR A2CT(LPCSTR lp) { return lp; }

void CSocketManager::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
	OutToDebug("ПолучениеДанных\n");
	if(vContext.FindMethod("ПолучениеДанных")>=0)
		vContext.CallFunction("ПолучениеДанных",pvThis,&(String("ПолучениеДанных")));

	/*
	LPBYTE lpData = lpBuffer;
	SockAddrIn origAddr;
	stMessageProxy msgProxy;
	if (IsSmartAddressing())
	{
		dwCount = __min(sizeof(msgProxy), dwCount);
		memcpy(&msgProxy, lpBuffer, dwCount);
		origAddr = msgProxy.address;
		if (IsServer())
		{
			// broadcast message to all
			msgProxy.address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			WriteComm((const LPBYTE)&msgProxy, dwCount, 0L);
		}
		dwCount -= sizeof(msgProxy.address);
		lpData = msgProxy.byData;
	}

	char *p=m_csBuffer.GetBuffer(dwCount);
	memcpy(&p, A2CT((LPSTR)lpData), dwCount);
	m_csBuffer.ReleaseBuffer();
	*/
	//AfxMessageBox(m_csBuffer);
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
// Send message to parent window to indicate connection status
void CSocketManager::OnEvent(UINT uEvent)
{
	switch( uEvent )
	{
		case EVT_CONSUCCESS:
			OutToDebug("ОбработкаСоединения\n");
			if(vContext.FindMethod("ОбработкаСоединения")>=0)
				vContext.CallFunction("ОбработкаСоединения",pvThis,&(String("Соединение выполнено")));
			//AppendMessage( _T("Connection Established\r\n") );
			break;
		case EVT_CONFAILURE:
			if(vContext.FindMethod("ОбработкаСоединения")>=0)
				vContext.CallFunction("ОбработкаСоединения",pvThis,&(String("Соединение не выполнено")));
			//AppendMessage( _T("Connection Failed\r\n") );
			break;
		case EVT_CONDROP:
			if(vContext.FindMethod("ОбработкаСоединения")>=0)
				vContext.CallFunction("ОбработкаСоединения",pvThis,&(String("Соединение прервано")));
			//AppendMessage( _T("Connection Abandonned\r\n") );
			break;
		case EVT_ZEROLENGTH:
			//AppendMessage( _T("Zero Length Message\r\n") );
			break;
		default:
			TRACE("Unknown Socket event\n");
			break;
	}

	//pParent->PostMessage( WM_UPDATE_CONNECTION, uEvent, (LPARAM) this);

}
