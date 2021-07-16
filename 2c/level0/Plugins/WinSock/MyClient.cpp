//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClient.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CMyClient::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMyClient,CValue);

CMyClient::CMyClient()
{
	nType=100;
	nWasConnect=0;
	conn=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(conn==INVALID_SOCKET)
		Error("INVALID_SOCKET");
}

CMyClient::~CMyClient()
{
	Close(CValue(),0);
}


//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CMyClient::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CMyClient::Connect,"Connect","Соединение","Соединение(Адрес,Порт)"},
	{CMyClient::Send,"Send","Отправить","Отправить(Значение,ФлагТекстДанных=0) - /отправка данных на сервер/"},
	{CMyClient::Receive,"Receive","Получить","Получить(ФлагТекстДанных=0) - /получение данных с сервера/"},
	{CMyClient::LoadToFile,"DownLoadToFile","ЗагрузитьВФайл","ЗагрузитьВФайл(ФайлНаСервере,ФайлНаДиске) - /загрузка файла по http протоколу/"},
	
	{CMyClient::Close,"Close","Отключение",""},
};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CMyClient::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CMyClient::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CMyClient::GetString()
{
	return "Client";
}
CString CMyClient::GetTypeString(void)const
{
	return "Client";
}


//////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////

BOOL CMyClient::Connect(CValue &rez,CValue**ppV)
{
	CString m_url=ppV[0]->GetString();

	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	CString servername; 
	CString filepath;
	CString filename;	
	int nHTTPMode=ParseURL(m_url,servername,filepath,filename);

	int nPort=80;
	if(!nHTTPMode)
		nPort=19760;

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
BOOL CMyClient::Send(CValue &rez,CValue**ppV)
{
	int nIsTextCommand=ppV[1]->GetNumber();

	if(!nWasConnect)
		Error("Не было соединения с сервером");

	//отправка команды или значения переменной
	CString buff;
	if(nIsTextCommand)
	{
		buff=ppV[0]->GetString();
	}
	else
	{
		ppV[0]->SaveToString(buff);
	}

	int nMustLength=buff.GetLength();
	int nSend=send(conn,buff.GetBuffer(0),nMustLength,0);
	if(nSend<nMustLength)
	{
		Error("Ошибка при передаче данных на сервер");
	}


	return TRUE;
};

BOOL CMyClient::Receive(CValue &rez,CValue**ppV)
{
	int nIsTextCommand=ppV[0]->GetNumber();

	if(!nWasConnect)
		Error("Не было соединения с сервером");

    CMemFile f(BUF_SIZE);
	//получение данных с сервера
	char buff[BUF_SIZE];
	int y;
	while(y=recv(conn,buff,BUF_SIZE,0))
	{
		if(y<0)
			break;
		f.Write(buff,y);
	}
	CString Str;
	int nLength=f.GetLength();
	if(nLength>0)
	{
		f.SeekToBegin();
		f.ReadHuge(Str.GetBuffer(nLength),nLength);
		Str.ReleaseBuffer();
	}
	if(nIsTextCommand)
	{
		rez=String(Str);
	}
	else
	{
		//преобразование данных в значение (объект)
		rez.LoadFromString(Str);
	}
	return TRUE; 
};
BOOL CMyClient::LoadToFile(CValue &rez,CValue**ppV)
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
BOOL CMyClient::Close(CValue &rez,CValue**ppV)
{
	if(nWasConnect)
	{
		closesocket(conn);
	}
	nWasConnect=0;
	return TRUE;
};
