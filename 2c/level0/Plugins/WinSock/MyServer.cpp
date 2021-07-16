//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyServer.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CMyServer::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyServer,CValue);

CMyServer::CMyServer()
{
	nType=100;
	m_nPort=PORT_2C;
	m_pCurContext=0;
}

CMyServer::~CMyServer()
{
	Stop(CValue(),0);
}


//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CMyServer::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CMyServer::Run,"Run","Запустить","Запустить()"},
	{CMyServer::Send,"Send","Отправить","Отправить(Значение,ФлагТекстДанных=0) - /отправка данных на сервер/"},
	{CMyServer::Port,"Port","Порт","Порт(Номер=0)"},
	{CMyServer::Stop,"Stop","Остановить"},
	{CMyServer::GetLocalAddress,"GetLocalAddress","ПолучитьЛокальныйАдрес"},
	

};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CMyServer::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CMyServer::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CMyServer::GetString()
{
	return "Сервер";
}
CString CMyServer::GetTypeString(void)const
{
	return "Сервер";
}



//////////////////////////////////////////////////////////////////////
void CALLBACK NotifyProc(LPVOID lpParam, CValue vRunContext, ClientContext* pContext, UINT nCode)
{
	CMyServer* pServer= (CMyServer*) lpParam;
	CValue* pRunContext= &vRunContext;
	CIOCPServer *pIOServer = &pServer->m_iocpServer;
	pServer->m_pCurContext=pContext;


	
	CValue vHostName=String(pIOServer->GetHostName(pContext->m_Socket));
	CValue vID=pContext->ID;

	//OutToDebug("nCode=%d\n",nCode);
	switch (nCode)
	{
	case NC_CLIENT_CONNECT:
		if(pRunContext->FindMethod("ОбработкаСоединения")>=0)
			pRunContext->CallFunction("ОбработкаСоединения",pServer,&(String("Соединение выполнено")),&vHostName,&vID);
		break;
	case NC_CLIENT_DISCONNECT:
		if(pRunContext->FindMethod("ОбработкаСоединения")>=0)
			pRunContext->CallFunction("ОбработкаСоединения",pServer,&(String("Соединение прервано")),&vHostName,&vID);
		break;
	case NC_TRANSMIT:
		break;
	case NC_RECEIVE:
		{
			//if(pContext->m_ReadBuffer.IsEndRead())
			if(pRunContext->FindMethod("ОбработкаДанных")>=0)
			{
				CValue val;
				CString Str=pContext->m_ReadBuffer.GetString();
				val.LoadFromString(Str);
				pRunContext->CallFunction("ОбработкаДанных",pServer,&val,&vHostName,&vID);
			}
		}
		break;
	

	}
	pServer->m_pCurContext=0;

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
bool GetLocalAddress(LPTSTR strAddress, UINT nSize)
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


CString GetLocalAddress()
{
	CString csRes;
	char *p=csRes.GetBuffer(HOSTNAME_SIZE);
	GetLocalAddress(p, HOSTNAME_SIZE);
	csRes.ReleaseBuffer();
	return csRes;
}



//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////

//#include<locale>
//locale loc_rus (".1251");

BOOL CMyServer::Run(CValue &rez,CValue**ppV)
{
	//m_vContext=Eval("Контекст");
	CValue vContext=GetRunContext();

	bool bSuccess=m_iocpServer.Initialize(NotifyProc, this, vContext, MAX_CONNECTION, m_nPort);

	if(!bSuccess)
	{
		CString Str;
		Str.Format("Ошибка создания сокета, порт:%d",m_nPort);
		Error(Str);
	}
	return TRUE;
};


BOOL CMyServer::Send(CValue &rez,CValue**ppV)
{
	int nIsTextCommand=ppV[1]->GetNumber();

	CString strText;
	if(nIsTextCommand)
		strText=ppV[0]->GetString();
	else
		ppV[0]->SaveToString(strText);


	m_iocpServer.Send(m_pCurContext, strText);
	return TRUE;
};

BOOL CMyServer::Port(CValue &rez,CValue**ppV)
{
	rez=m_nPort;
	if(ppV[0]->nType)
		m_nPort=ppV[0]->GetNumber();

	return TRUE;
};

BOOL CMyServer::Stop(CValue &rez,CValue**ppV)
{
	m_iocpServer.Stop();//все равно не работает :-(

	return TRUE;
};
BOOL CMyServer::GetLocalAddress(CValue &rez,CValue**ppV)
{
	rez=String(::GetLocalAddress());
	return TRUE;
};
