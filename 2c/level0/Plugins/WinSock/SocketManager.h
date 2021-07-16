// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
#define AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"
#include "SocketComm.h"

#define WM_UPDATE_CONNECTION	WM_APP+0x1234

struct stMessageProxy
{
  SockAddrIn address;
  BYTE byData[BUFFER_SIZE];
};

class CSocketManager : public CSocketComm  
{
public:
	CSocketManager();
	virtual ~CSocketManager();
public:
	CString m_csBuffer;


	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
	virtual void OnEvent(UINT uEvent);

	void SetContext(CValue *pvd,CValue v){pvThis=pvd,vContext=v;};



protected:
	//обратный вызов
	CValue vContext;
	CValue *pvThis;
};

#endif // !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
