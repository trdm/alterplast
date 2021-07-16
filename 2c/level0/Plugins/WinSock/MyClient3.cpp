//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClient3.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "IOCPServer.h"

static int bWasInitCriticalSection=0;
static CRITICAL_SECTION m_cs1;
static CRITICAL_SECTION m_cs2;
static CRITICAL_SECTION m_cs3;
static CRITICAL_SECTION m_cs4;

CMethods CMyClient3::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyClient3,CValue);

CMyClient3::CMyClient3()
{
	if(!bWasInitCriticalSection)
	{
		InitializeCriticalSection(&m_cs1);
		InitializeCriticalSection(&m_cs2);
		InitializeCriticalSection(&m_cs3);
		InitializeCriticalSection(&m_cs4);
		bWasInitCriticalSection=1;
	}

	nType=100;
	m_nPort=PORT_2C;
	bWasDataReceive=0;
}

CMyClient3::~CMyClient3()
{
	Close(CValue(),0);
}



//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CMyClient3::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CMyClient3::Connect,"Connect","Соединение","Соединение(Адрес)"},
	{CMyClient3::Send,"Send","Отправить","Отправить(Значение,ФлагТекстДанных=0) - /отправка данных на сервер/"},
	{CMyClient3::Receive,"Receive","Получить","Получить(ФлагТекстДанных=0) - /получение данных с сервера/"},
	{CMyClient3::Port,"Port","Порт","Порт(Номер=0)"},
	{CMyClient3::Close,"Close","Отключить",""},
//	{CMyClient3::LoadToFile,"DownLoadToFile","ЗагрузитьВФайл","ЗагрузитьВФайл(ФайлНаСервере,ФайлНаДиске) - /загрузка файла по http протоколу/"},
};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CMyClient3::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CMyClient3::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CMyClient3::GetString()
{
	return "Client";
}
CString CMyClient3::GetTypeString(void)const
{
	return "Client";
}


//////////////////////////////////////////////////////////////////////
/*
int ParseURL(CString m_url, CString &server, CString &filepath, CString &filename)
{
	int nHTTPMode=0;
	int n;
	CString url=m_url;
	CString url2=url.Left(7);
	url2.MakeLower();
	if(url2=="http://")
	{
		url=url.Mid(7);
		nHTTPMode=1;
	}
	n=url.Find('/');
	if(n>0)
	{
		server=url.Left(n);
		filepath=url.Mid(n);
		n=filepath.ReverseFind('/');
		filename=filepath.Right(filepath.GetLength()-(n+1));
	}
	else
	{
		server=url;
		filepath="";
		filename="";
	}
	return nHTTPMode;
}
*/
//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CALLBACK NotifyProc2(LPVOID lpParam, CValue vRunContext, UINT nCode, CString csData)
{
	CMyClient3* pClient= (CMyClient3*) lpParam;
	//CValue* pRunContext= &pClient->m_vContext;
	CValue* pRunContext=&vRunContext;

	CValue val;

	pClient->csData=csData;
	pClient->bWasDataReceive=1; 

	//OutToDebug("nCode=%d\n",nCode);
	switch (nCode)
	{
	case NC_CLIENT_CONNECT:
		{
		    CLock cs(m_cs1, "NC_CLIENT_CONNECT" );
			CValue vData=String("Соединение выполнено");
			if(pRunContext)
			if(pRunContext->FindMethod("ОбработкаСоединения")>=0)
				pRunContext->CallFunction("ОбработкаСоединения",pClient,&vData,&val,&val);
			//TRACE(_T("OK - NC_CLIENT_CONNECT\n"));
		}
		break;
	case NC_CLIENT_DISCONNECT:
		{
		    CLock cs(m_cs2, "NC_CLIENT_DISCONNECT" );
			CValue vData=String("Соединение прервано");
			if(pRunContext)
			if(pRunContext->FindMethod("ОбработкаСоединения")>=0)
				pRunContext->CallFunction("ОбработкаСоединения",pClient,&vData,&val,&val);
			break;
		}
	case NC_CLIENT_ERROR:
		if(pRunContext)
		if(pRunContext->FindMethod("ОбработкаСоединения")>=0)
			pRunContext->CallFunction("ОбработкаСоединения",pClient,&(String(csData)),&val,&val);
		break;
	case NC_RECEIVE:
		{
			return;
		    CLock cs(m_cs3, "NC_RECEIVE" );
			//OutToDebug("csData=%s\n",csData);
			if(pRunContext)
			if(pRunContext->FindMethod("ОбработкаДанных")>=0)
			{
				CValue rez;
				if(csData.Left(1)=='{')
					rez.LoadFromString(csData);
				else
					rez=String(csData);
				pRunContext->CallFunction("ОбработкаДанных",pClient,&rez,&val,&val);
			}
		}
		break;
	

	}
}


BOOL CMyClient3::Connect(CValue &rez,CValue**ppV)
{
	CString strServer=ppV[0]->GetString();

	//DoMessage2C();
	CValue vContext=GetRunContext();
	//m_vContext=Eval("Контекст");
	bool bSuccess=m_socket.Init(strServer, m_nPort, NotifyProc2, this, vContext);

	if(!bSuccess)
	{
		CString Str;
		Str.Format("Ошибка соединения с сервером: %s, порт:%d",strServer,m_nPort);
		Error(Str);
	}

	return TRUE;
};

BOOL CMyClient3::Send(CValue &rez,CValue**ppV)
{
	int nIsTextCommand=ppV[1]->GetNumber();

	CString strText;
	if(nIsTextCommand)
		strText=ppV[0]->GetString();
	else
		ppV[0]->SaveToString(strText);


	bWasDataReceive=0;
	m_socket.SendData(strText);
	return TRUE;
};
BOOL CMyClient3::Receive(CValue &rez,CValue**ppV)
{
	int nIsTextCommand=ppV[0]->GetNumber();

	WaitMessage();
	while(bWasDataReceive!=1)
	{
		SetStatus("Ожидание ответа от сервера");
		Sleep(1);
		WaitMessage();

		MSG msg;
		while( ::PeekMessage( &msg, NULL, FD_READ, FD_READ, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, FD_READ, FD_READ ) )
				break;
			::DispatchMessage( &msg );
		}
		//отрисовка окна
		while( ::PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_PAINT, WM_PAINT ) )
				break;
			::DispatchMessage( &msg );
		}
	}


	bWasDataReceive=0;
	if(nIsTextCommand)
	{
		rez=String(csData);
	}
	else
	{
		//преобразование данных в значение (объект)
		rez.LoadFromString(csData);
	}
	return TRUE;
};

BOOL CMyClient3::LoadToFile(CValue &rez,CValue**ppV)
{

	return TRUE;
};
BOOL CMyClient3::Close(CValue &rez,CValue**ppV)
{
	m_socket.Stop();

	return TRUE;
};


BOOL CMyClient3::Port(CValue &rez,CValue**ppV)
{
	rez=m_nPort;
	if(ppV[0]->nType)
		m_nPort=ppV[0]->GetNumber();

	return TRUE;
};
