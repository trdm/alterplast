//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyServerSimple.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CMyServerSimple::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyServerSimple,CValue);

CMyServerSimple::CMyServerSimple()
{
	nType=100;
	m_nPort=PORT_2C;
	m_socket=INVALID_SOCKET;
	m_pClient=0;
}

CMyServerSimple::~CMyServerSimple()
{
	Stop(CValue(),0);
}

void CMyServerSimple::SetClient(CContextData &client)
{
	m_pClient=&client;
	m_socket=client.socket;
	m_nPort=client.nPort;
}

//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CMyServerSimple::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CMyServerSimple::GetLocalAddress,"GetLocalAddress","ПолучитьЛокальныйАдрес",""},
	{CMyServerSimple::Run,"Run","Запустить","Запустить()"},
	{CMyServerSimple::Send,"Send","Отправить","Отправить(Значение) - /отправка данных клиенту/"},
	{CMyServerSimple::Port,"Port","Порт","Порт(Номер=0)"},
	{CMyServerSimple::Stop,"CloseClient","ОтлючитьКлиента","ОтлючитьКлиента() - /Закрыть соединение текущего клиента/"},
};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CMyServerSimple::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CMyServerSimple::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CMyServerSimple::GetString()
{
	return "Сервер";
}
CString CMyServerSimple::GetTypeString(void)const
{
	return "Сервер";
}


//////////////////////////////////////////////////////////////////////
UINT  MTServerThread(LPVOID pParam);
UINT  ClientThread(LPVOID pParam);

UINT  MTServerThread(LPVOID pParam)
{	
	CContextData context=*(CContextData*)pParam;
	//CValue vVal=*client.This;//ValueByRef(context.pThis);//блокируем удаление сервера


	SOCKET server;


	WSADATA wsaData;
	sockaddr_in local;
	int wsaret=WSAStartup(0x101,&wsaData);
	if(wsaret!=0)
	{
		return 0;
	}
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short)context.nPort);
	server=socket(AF_INET,SOCK_STREAM,0);
	if(server==INVALID_SOCKET)
	{
		context.pThis->m_bErr=1;
		//Error(_T("WSAAsyncSelect() error %ld\n"),WSAGetLastError());
		OutToDebug(_T("WSAAsyncSelect() error %ld\n"),WSAGetLastError());
		return 0;
	}
	if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
	{
		context.pThis->m_bErr=1;
		//Error(_T("bind() error %ld\n"),WSAGetLastError());
		OutToDebug(_T("bind() error %ld\n"),WSAGetLastError());
		return 0;
	}
	if(listen(server,10)!=0)
	{
		context.pThis->m_bErr=1;
		//Error(_T("listen() error %ld\n"),WSAGetLastError());
		OutToDebug(_T("listen() error %ld\n"),WSAGetLastError());
		return 0;
	}

	sockaddr_in from;
	int fromlen=sizeof(from);
	CContextData client;

	static int ID=0;
	//static CRITICAL_SECTION m_criticalSection;
	//InitializeCriticalSection(&m_criticalSection);
	while(true)
	{
		//EnterCriticalSection(&m_criticalSection);

		ID++;
		context.ID=ID;

		context.socket=accept(server,(struct sockaddr*)&from,&fromlen);
		AfxBeginThread(ClientThread,(LPVOID)&context);	
		Sleep(10);

		//LeaveCriticalSection(&m_criticalSection);
	}	
	//DeleteCriticalSection(&m_criticalSection);

	closesocket(server);

	return 0;
}

#define HDR_SIZE sizeof(int)
BOOL Receive(SOCKET socket,CMemFile &f)
{	
	f.SeekToBegin();
	char buf[BUF_SIZE];
	while(true)
	{
		int nSize=0;
		int n=recv(socket,buf,BUF_SIZE,0);
		if(n >= (int)HDR_SIZE)
		{
			CopyMemory(&nSize, &buf[0], HDR_SIZE);
			n-=HDR_SIZE;
			if(n>0)
			{
				f.Write(&buf[4],n);
			}
		
			if(nSize>0)
			while(true)
			{
				int nLeftRead=nSize-f.GetLength();
				if(nLeftRead>0)
				{
					n=recv(socket,buf,min(nLeftRead,BUF_SIZE),0);
					if(n==SOCKET_ERROR )			
					{
						return 0;
					}
					f.Write(buf,n);
				}
				else
				{
					return 1;
				}
			}
			return 1;
		}
		Sleep(1);
	}
	return 1;
}
BOOL Send(SOCKET socket,CString buff)
{
	int nMustLength=buff.GetLength();
	//поддержка расширенного формата - с передачей размера пакета
	char *p=new char[nMustLength+HDR_SIZE];
	memcpy(p,&nMustLength,HDR_SIZE);
	memcpy(p+HDR_SIZE,buff.GetBuffer(0),nMustLength);
	buff.ReleaseBuffer();
	
	nMustLength=nMustLength+HDR_SIZE;

	int nSend=send(socket,p,nMustLength,0);
	delete []p;
	if(nSend<nMustLength)
	{
		return 0;
	}
	return 1;
}


void CloseSocket(CContextData &client,CValue &vHostName,CValue &vID)
{
	CMyServerSimple vRunClient;
	vRunClient.SetClient(client);
	try
	{
		if(client.pRunContext->FindMethod("ОбработкаСоединения")>=0)
			client.pRunContext->CallFunction("ОбработкаСоединения",&vRunClient,&(String("Соединение прервано")),&vHostName,&vID);
	}
	catch(...)
	{
		OutToDebug("Ошибка выполнения процедуры ""ОбработкаСоединения""\n");
	}
	closesocket(client.socket);
}



UINT  ClientThread(LPVOID pParam)
{
    CMemFile f(BUF_SIZE);

	CContextData client=*((CContextData*)pParam);
	CMyServerSimple vRunClient;
	vRunClient.SetClient(client);

	CValue vHostName=String(client.pThis->GetHostName(client.socket));
	CValue vID=client.ID;
	try
	{
		if(client.pRunContext->FindMethod("ОбработкаСоединения")>=0)
			client.pRunContext->CallFunction("ОбработкаСоединения",&vRunClient,&(String("Соединение выполнено")),&vHostName,&vID);
	}
	catch(...)
	{
		OutToDebug("Ошибка выполнения процедуры ""ОбработкаСоединения""\n");
		CloseSocket(client,vHostName,vID);
		return 0;
	}

	while(client.bClose==0)
	{
		BOOL bRes=Receive(client.socket,f);
		if(!bRes)
		{
			OutToDebug("Ошибка при получении данных от клиента\n");
			CloseSocket(client,vHostName,vID);
			return 0;
		}

		CString Str;
		int nLength=f.GetLength();
		if(nLength>0)
		{
			f.SeekToBegin();
			f.ReadHuge(Str.GetBuffer(nLength),nLength);
			Str.ReleaseBuffer();
		}
		try
		{
			if(client.pRunContext->FindMethod("ОбработкаДанных")>=0)
			{
				CValue val;
				val.LoadFromString(Str);
				client.pRunContext->CallFunction("ОбработкаДанных",&vRunClient,&val,&vHostName,&vID);
			}
		}
		catch(...)
		{
			OutToDebug("Ошибка выполнения процедуры ""ОбработкаДанных""\n");
			closesocket(client.socket);
			return 0;
		}
	}		

	closesocket(client.socket);
	return 0;
}
CString CMyServerSimple::GetHostName(SOCKET socket)
{
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	
	BOOL bResult = getpeername(socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	
	return bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
}

///////////////////////////////////////////////////////////////////////////////
// GetLocalAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Get TCP address of local computer in dot format ex: "127.0.0.0"
// PARAMETERS:
//	LPTSTR strAddress: pointer to hold address string, must be long enough
//	UINT nSize: maximum size of this buffer
///////////////////////////////////////////////////////////////////////////////
#define HOSTNAME_SIZE	MAX_PATH
bool CMyServerSimple::GetLocalAddress2(LPTSTR strAddress, UINT nSize)
{
	// Get computer local address
	if (strAddress != NULL && nSize > 0)
	{
		char strHost[HOSTNAME_SIZE] = { 0 };

		// get host name, if fail, SetLastError is called
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
		{
			struct hostent* hp;
			hp = gethostbyname(strHost);
			if (hp != NULL && hp->h_addr_list[0] != NULL)
			{
				// Address is four bytes (32-bit)
				if ( hp->h_length < 4)
					return false;

				// Convert address to . format
				strHost[0] = 0;

				// Create Address string
				sprintf(strHost, "%u.%u.%u.%u",
					(UINT)(((PBYTE) hp->h_addr_list[0])[0]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[1]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[2]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[3]));

				// check if user provide enough buffer
				if (strlen(strHost) > nSize)
				{
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					return false;
				}

			// Unicode conversion
#ifdef _UNICODE
				return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strAddress,
					nSize, NULL, NULL ));
#else
				_tcscpy(strAddress, strHost);
				return true;
#endif
			}
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}


CString CMyServerSimple::GetLocalAddress1()
{
	CString csRes;
	char *p=csRes.GetBuffer(HOSTNAME_SIZE);
	GetLocalAddress2(p, HOSTNAME_SIZE);
	csRes.ReleaseBuffer();
	return csRes;
}



//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////
BOOL CMyServerSimple::Run(CValue &rez,CValue**ppV)
{
	//m_vContext=Eval("Контекст");
	m_vRun=GetRunContext();
	//CValue *pVal=new CValue();
	//*pVal=GetRunContext();

	m_bErr=0;
	m_context.nPort=m_nPort;
	m_context.pRunContext=&m_vRun;
	m_context.pThis=this;

	AfxBeginThread(MTServerThread,(LPVOID)&m_context);
	Sleep(100);
	if(m_bErr)
	{
		Error("Ошибка соединения");
	}

	return TRUE;
};


BOOL CMyServerSimple::Send(CValue &rez,CValue**ppV)
{
	CString strText;
	ppV[0]->SaveToString(strText);
	
	if(!::Send(m_socket,strText))
	{
		Error("Ошибка при передаче данных на сервер");
	}


	return TRUE;
};

BOOL CMyServerSimple::Port(CValue &rez,CValue**ppV)
{
	rez=m_nPort;
	if(ppV[0]->nType)
		m_nPort=ppV[0]->GetNumber();

	return TRUE;
};

BOOL CMyServerSimple::Stop(CValue &rez,CValue**ppV)
{
	if(m_pClient)
		m_pClient->bClose=1;
	return TRUE;
};
BOOL CMyServerSimple::GetLocalAddress(CValue &rez,CValue**ppV)
{
	rez=String(GetLocalAddress1());
	return TRUE;
};
