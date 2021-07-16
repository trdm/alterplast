//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClientSimple.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BOOL Send(SOCKET socket,CString buff);
BOOL Receive(SOCKET socket,CMemFile &f);

CMethods CMyClientSimple::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyClientSimple,CValue);

CMyClientSimple::CMyClientSimple()
{
	nType=100;
	nWasConnect=0;
	conn=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(conn==INVALID_SOCKET)
		Error("INVALID_SOCKET");

	m_nPort=PORT_2C;

}

CMyClientSimple::~CMyClientSimple()
{
	Close(CValue(),0);
}


//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CMyClientSimple::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CMyClientSimple::Close,"Close","Отключение",""},
	{CMyClientSimple::Connect,"Connect","Соединение","Соединение(Адрес,Порт)"},
	{CMyClientSimple::Send,"Send","Отправить","Отправить(Значение) - /отправка данных на сервер/"},
	{CMyClientSimple::Receive,"Receive","Получить","Получить() - /получение данных с сервера/"},
	{CMyClientSimple::Port,"Port","Порт","Порт(Номер=0)"},

//	{CMyClientSimple::LoadToFile,"DownLoadHTTP","ЗагрузитьHTTP","ЗагрузитьHTTP(ФайлНаСервере,ФайлНаДиске) - /загрузка файла по http протоколу/"},
	
};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CMyClientSimple::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CMyClientSimple::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CMyClientSimple::GetString()
{
	return "Client";
}
CString CMyClientSimple::GetTypeString(void)const
{
	return "Client";
}


//////////////////////////////////////////////////////////////////////
int CMyClientSimple::ParseURL(CString m_url, CString &server, CString &filepath, CString &filename)
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

//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////

BOOL CMyClientSimple::Connect(CValue &rez,CValue**ppV)
{
	CString m_url=ppV[0]->GetString();

	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	CString servername; 
	CString filepath;
	CString filename;	
	int nHTTPMode=ParseURL(m_url,servername,filepath,filename);

	int nPort=m_nPort;
	if(nHTTPMode)
		nPort=80;

	if(ppV[1]->nType)
		nPort=ppV[1]->GetNumber();



	SetStatus("Initialized WinSock");



	if(inet_addr(servername)==INADDR_NONE)
	{
		hp=gethostbyname(servername);
	}
	else
	{
		addr=inet_addr(servername);
		hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
	}
	nWasConnect=0;
	if(hp==NULL)
	{
		closesocket(conn);
		Error("Ошибка разбора адреса");
		return FALSE;
	}
	SetStatus("hostname/ipaddress resolved");
	server.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
	server.sin_family=AF_INET;
	server.sin_port=htons(nPort);
	if(connect(conn,(struct sockaddr*)&server,sizeof(server)))
	{
		//SetStatus("Ошибка соединения");
		closesocket(conn);
		Error("Ошибка соединения");
		return FALSE;	
	}
	SetStatus(CString("Connected to server: ")+servername+" - OK");
	nWasConnect=1;

	return TRUE;
}; 

#define HDR_SIZE sizeof(int)

BOOL CMyClientSimple::Send(CValue &rez,CValue**ppV)
{
	if(!nWasConnect)
		Error("Не было соединения с сервером");

	//отправка команды или значения переменной
	CString buff;
	ppV[0]->SaveToString(buff);

	if(!::Send(conn,buff))
	{
		Error("Ошибка при передаче данных на сервер");
	}

	return TRUE;
};

BOOL CMyClientSimple::Receive(CValue &rez,CValue**ppV)
{
	if(!nWasConnect)
		Error("Не было соединения с сервером");


	BOOL bRes=::Receive(conn,f);
	if(!bRes)
	{
		Error("Ошибка при получении данных от клиента\n");
		return FALSE;
	}

	CString Str;
	int nLength=f.GetLength();
	if(nLength>0)
	{
		f.SeekToBegin();
		f.ReadHuge(Str.GetBuffer(nLength),nLength);
		Str.ReleaseBuffer();
	}


	//преобразование данных в значение (объект)
	rez.LoadFromString(Str);
	return TRUE;
};

BOOL CMyClientSimple::LoadToFile(CValue &rez,CValue**ppV)
{
	CString filedownload=ppV[0]->GetString();
	CString fsave=ppV[1]->GetString();

	CString url=filedownload;
	if(url.Left(7)=="http://")
	{
		if(!Connect(rez,ppV))
			return FALSE;

		CString servername; 
		CString filename;	
		ParseURL(url,servername,filedownload,filename);
	}

	if(!nWasConnect)
		Error("Не было соединения с сервером");


	char buff[512];
	CFile f;
	int y;

	sprintf(buff,"GET %s\r\n\r\n",filedownload);
	send(conn,buff,strlen(buff),0);
	SetStatus(CString("Sending command :- GET")+filedownload+" to server");

	f.Open(fsave,CFile::modeCreate | CFile::modeWrite);
	SetStatus("starting to receive file");
	while(y=recv(conn,buff,512,0))
	{
		if(y<0)
			break;
		f.Write(buff,y);
	}
	f.Close();
	SetStatus(CString("File downloaded and saved: ")+fsave);

	return TRUE;
};
BOOL CMyClientSimple::Close(CValue &rez,CValue**ppV)
{
	if(nWasConnect)
	{
		closesocket(conn);
	}
	nWasConnect=0;
	return TRUE;
};

BOOL CMyClientSimple::Port(CValue &rez,CValue**ppV)
{
	rez=m_nPort;
	if(ppV[0]->nType)
		m_nPort=ppV[0]->GetNumber();

	return TRUE;
};
