// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// MyDataExchange.cpp : implementation file
//

#include "stdafx.h"
#include "MyDataExchange.h"
#include "Compiler/ValueArray.h"
#include "Resource.h"
#include "OpenForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define MESSAGE_DEBUG
#define NAME_ERROR "<������>"

#include "MessageBar.h"

CString GetModuleText(CString csPath,BOOL &bError);
CString LoadFromFile(CString csFileName);
CRunContext *pRunContext=0;

extern CMessageBar *pMessageWindow;//��� ������ �� �����
extern BOOL afxMessageFromConfig;
extern BOOL afxSimpleErrorMessage;
extern CString afxStrErrorMessage;

void AddDebugPoint(CString csModule,int nLine);
void ClearDebugPoint(CString csModule);
void ClearDebugPoint();
int AfxGetCountRunContext();

BOOL bUseDebug=0;
BOOL bUseDebug2=0;
BOOL bDebugLoop=0;
BOOL bDebugStopLine=0;
BOOL bStopRunProgram=0;
int bDoLoop=0;
int bStopDebug=0;
int nCurrentNumberStopContext=0;
BOOL bConfigGetAttribute=0;


HWND Get2C();
void SetCurrentStack(int nNumber);

typedef CArray <BOOL,BOOL> CMapInt;
//typedef CMap<UINT,UINT,BOOL,BOOL> CMapInt;

//����� ����������� � ��������������
#define MYMESSAGE_WND_TITLE	csIBDir+":MyDataExchange2C"
#define MYMESSAGE_WND_DEBUG	csIBDir+":MyDataExchange2C:Debug"


//��� �����������:
CString csLastDebugListName;
CMapInt *pLastDebugList=0;


CString StrMessage;
//������ ��������� �������
CArray <CString,CString> aAttributeName;
CArray <CString,CString> aAttributeType;
CArray <CString,CString> aAttributeValue;

int GotoLine(CString Str,int nLine,BOOL bRun);
void SendDebugPoint(BOOL bPost);
CString GetVariable(CString Str);
void SendVarStringArray();
void SetStack();

//#include "DDE\SharedMemory.h"


CArray <void*,void*> aPList;
CArray <CString,CString> aList;

CRITICAL_SECTION m_criticalSection2;
BOOL bInitCriticalSection2=0;


class CMustZeroOnExit
{
public:
	CMustZeroOnExit(BOOL &Val)
	{
		m_pVal=&Val;
	};
	~CMustZeroOnExit()
	{
		*m_pVal=0;
	};
	BOOL *m_pVal;
};

void ClearPList()
{
	for(int i=0;i<aPList.GetSize();i++)
	{
		CArray <CString,CString> *pList=(CArray <CString,CString>*)aPList[i];
		if(pList)
			delete pList;
	}
	aPList.RemoveAll();
}
int CompareListInt( const void *arg1, const void *arg2 )//������� ��������� ��������� ������
{
 	int n1=(*(int*)arg1);
	int n2=(*(int*)arg2);
	return n1-n2;
}
void SortList(CArray <int,int&>  &List)
{
	if(List.GetSize())
		qsort(&List[0], List.GetSize(), sizeof(List[0]), CompareListInt);
}




class CDebugList
{
public:
	CMapStringToPtr aModuleList;//������ �������

	void AddPoint(CString csModule,int nLine)
	{
		CMapInt *aDebugList=(CMapInt *)aModuleList[mUpper(csModule)];
		if(!aDebugList)
		{
			aDebugList=new CMapInt();
			aModuleList[mUpper(csModule)]=aDebugList;
		}
		aDebugList->SetAtGrow(nLine,1);

	};
	
	~CDebugList()
	{
		Clear();
	};

	void Clear()
	{
		CMapInt *p;
		POSITION pos;
		CString csKey;
		for( pos = aModuleList.GetStartPosition(); pos != NULL; )
		{
			aModuleList.GetNextAssoc( pos, csKey, (void*&)p );
			if(p)
			{
				delete p;
			}
		}
		aModuleList.RemoveAll();
	};
}aDebugList;

/////////////////////////////////////////////////////////////////////////////
// CMyDataExchange dialog

LRESULT MySendMessage(
  HWND hWnd,      // handle to destination window
  UINT Msg,       // message
  WPARAM wParam,  // first message parameter
  LPARAM lParam,   // second message parameter
  int pPost=0
)
{
	//pPost=0;
	if(pPost)
		return ::PostMessage(hWnd,Msg,wParam,lParam);
	else
		return ::SendMessage(hWnd,Msg,wParam,lParam);
}


CMyDataExchange::CMyDataExchange(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDataExchange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDataExchange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMyDataExchange::~CMyDataExchange()
{
}

void CMyDataExchange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDataExchange)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDataExchange, CDialog)
	//{{AFX_MSG_MAP(CMyDataExchange)
	ON_MESSAGE(WM_MESSAGE_FROM_2C, OnMessageFrom2C)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void LoadStringArray(CArray <CString,CString> *pList=0)
{
	if(!pList)
		pList=&aList;
	aAttributeName.SetSize(pList->GetSize()/3);
	aAttributeType.SetSize(pList->GetSize()/3);
	aAttributeValue.SetSize(pList->GetSize()/3);
	for(int i=0;i<pList->GetSize()/3;i++)
	{
		aAttributeName[i]=pList->GetAt(i*3+0);
		aAttributeType[i]=pList->GetAt(i*3+1);
		aAttributeValue[i]=pList->GetAt(i*3+2);
	}
}
void LoadStringArray(CArray <CString,CString> &aList,const char *buffer)
{
	//��������� ������
	//���-�� ��������� � �������
	//����� ����� ���������
	//��� ���������
	//����� i-�� �������� ���������
	//������ i-�� �������� ���������
	int n=0;
	int nLength;
	memcpy(&nLength,&buffer[n],sizeof(nLength));//���-�� ��������� � �������
	n+=sizeof(nLength);

	aList.SetSize(nLength);

	memcpy(&nLength,&buffer[n],sizeof(nLength));//����� ����� ���������
	n+=sizeof(nLength);

	memcpy(StrMessage.GetBuffer(nLength),&buffer[n],nLength);//��� ���������
	n+=nLength;
	StrMessage.ReleaseBuffer(nLength);

	for(int i=0;i<aList.GetSize();i++)
	{
		memcpy(&nLength,&buffer[n],sizeof(nLength));//����� i-�� �������� ���������
		n+=sizeof(nLength);

		char *p=aList[i].GetBuffer(nLength);
		memcpy(p,&buffer[n],nLength);//������ i-�� �������� ���������
		n+=nLength;
		aList[i].ReleaseBuffer(nLength);
	}
}
BOOL CMyDataExchange::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(pCopyDataStruct->dwData==0)
	{
		StrMessage = (LPCSTR)(pCopyDataStruct->lpData);
		StrMessage.ReleaseBuffer(pCopyDataStruct->cbData);
	}
	else
	if(pCopyDataStruct->dwData==1)//������� ������� �����
	{
		const char *buffer=(LPCSTR)pCopyDataStruct->lpData;
		LoadStringArray(aList,buffer);
	}
	else
	if(pCopyDataStruct->dwData==2)//������� ����. ������� �����
	{
		const char *buffer=(LPCSTR)pCopyDataStruct->lpData;
		int i;

		//������� ���������� ������
		ClearPList();


		int n=0;
		int nSize;
		memcpy(&nSize,&buffer[n],sizeof(nSize));//���������� ��������
		n+=sizeof(nSize);
		aPList.SetSize(nSize);

		for(i=0;i<aPList.GetSize();i++)
		{
			int nLength;
			memcpy(&nLength,&buffer[n],sizeof(nLength));//����� ������� �����
			n+=sizeof(nLength);

			CArray <CString,CString> *aList=new CArray <CString,CString>;

			LoadStringArray(*aList,&buffer[n]);//������ �����
			n+=nLength;

			aPList[i]=aList;
		}
	}



#ifdef MESSAGE_DEBUG
	OutToDebug("Get: %s\n",StrMessage);
#endif

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


int CMyDataExchange::OnMessageFrom2C(WPARAM wParam, LPARAM lParam)
{
	switch(wParam) 
	{
	case MYMESSAGE_START:
		StrMessage="";
		break;
	case MYMESSAGE_DATA:
		StrMessage=StrMessage+char(lParam);
		break;
	case MYMESSAGE_FINISH:
		Message(String(StrMessage));
		break;
	case MYMESSAGE_ACTIVATE:
		GotoLine(StrMessage,lParam,0);
		break;


	case SET_CUR_LINE:
		return GotoLine(StrMessage,lParam,1);
		break;

	case GET_DEBUG_POINT:
		SendDebugPoint(0);
		break;

	case ADD_DEBUG_POINT:
		AddDebugPoint(StrMessage,lParam);
		break;
	case REMOVE_ALL_DEBUG_POINT:
		ClearDebugPoint();
		break;

	case RUN_DEBUG:
		bDebugLoop=0;
		bDoLoop=0;

		break;
	case NEXT_DEBUG:
		bDebugStopLine=1;
		break;

	case NEXT_DEBUG_OVER:
		nCurrentNumberStopContext=AfxGetCountRunContext();
		break;

	case GET_VARIABLE:
		GetVariable(StrMessage);
		break;

	case SET_VARIABLE:
		break;

	case SET_ATTRIBUTE_NAME:
		aAttributeName.Add(StrMessage);
		break;
	
	case SET_ATTRIBUTE_VALUE:
		aAttributeValue.Add(StrMessage);
		break;

	case SET_ATTRIBUTE_TYPE:
		aAttributeType.Add(StrMessage);
		break;
		

	case PAUSE_DEBUG:
		bUseDebug=1;
		bDebugStopLine=1;
		break;
	case STOP_DEBUG:
		bUseDebug=1;
		bStopDebug=1;
		break;

	case STOP_PROGRAM:
		bStopRunProgram=1;
		bDebugLoop=0;

		if(AfxGetMainWnd())
			AfxGetMainWnd()->DestroyWindow();
		exit(0);
		break;

	case SET_STACK_ARRAY:
		break;
	case SET_STRING_ARRAY:
		LoadStringArray();
		break;
	case GET_VAR_STRING_ARRAY:
		SendVarStringArray();//�������� ������ ������� ��������
		break;

	case GET_STACK:
		SetStack();//�������� ������ �������� �����
		break;
	case SET_CURRENT_STACK:
		SetCurrentStack(lParam);
		break;
	}


#ifdef MESSAGE_DEBUG
	switch(wParam)
	{
	case SET_STRING_ARRAY:
	case MYMESSAGE_ACTIVATE:
	case SET_CUR_LINE:
	case ADD_DEBUG_POINT:
	case MYMESSAGE_FINISH:
	case SET_ATTRIBUTE_NAME:
	case SET_ATTRIBUTE_VALUE:
	case SET_ATTRIBUTE_TYPE:
	case GET_VARIABLE:
	if(afxAppRunMode==CONFIG_MODE)
		OutToDebug("Config get     %2.0f: %s\n",wParam,StrMessage);
	else
		OutToDebug("Enterprise get %2.0f: %s\n",wParam,StrMessage);
	}
#endif

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CMyDataExchange message handlers
BOOL CMyDataExchange::OnInitDialog() 
{
	CDialog::OnInitDialog(); 

	if(afxAppRunMode==CONFIG_MODE)
	{
		SetWindowText(MYMESSAGE_WND_TITLE);
	}
	else
	{
		SetWindowText(MYMESSAGE_WND_DEBUG);
		HWND hwnd=::FindWindow(NULL,MYMESSAGE_WND_TITLE);
		if(hwnd)
			MySendMessage(hwnd,WM_MESSAGE_FROM_2C,GET_DEBUG_POINT,0);
	}
	
	return TRUE;
}
 

int SendString(HWND hwnd,CString txt,int nMode=MYMESSAGE_FINISH,int nLine=0,int bPost=0)
{
#ifdef MESSAGE_DEBUG
	OutToDebug("Send %2.0f: %s\n",nMode,txt);
#endif
//*
		COPYDATASTRUCT cpd;
		cpd.dwData = 0;
		cpd.cbData = txt.GetLength();
		cpd.lpData = (void*)txt.GetBuffer(txt.GetLength());
		MySendMessage(hwnd,WM_COPYDATA,
					(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
					(LPARAM)&cpd,0);
		txt.ReleaseBuffer();
//*/


/*
	MySendMessage(hwnd,WM_MESSAGE_FROM_2C,MYMESSAGE_START,NULL,bPost);//�������������
	for(int i=0;i<txt.GetLength();i++)
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,MYMESSAGE_DATA,txt[i],bPost);
//*/

	return MySendMessage(hwnd,WM_MESSAGE_FROM_2C,nMode,nLine,bPost);//��������� ��������

}


//������� ������ ������ ������
int GetSizeStringArray(CString csName,CArray <CString,CString> &aList)
{
	//��������� ������
	//���-�� ��������� � �������
	//����� ����� ���������
	//��� ���������
	//����� i-�� �������� ���������
	//������ i-�� �������� ���������

	int i;
	int nSize;
	nSize=sizeof(int);//��� ���������� ��������� �������
	nSize+=sizeof(int);//��� ����� �����
	nSize+=csName.GetLength();

	for(i=0;i<aList.GetSize();i++)
	{
		nSize+=sizeof(int);//��� ����� ������
		nSize+=aList[i].GetLength();
	}
	return nSize;
}

//�������� ������ � �����
//(������� ��������������� ������� ������)
char *GetBufferStringArray(CString csName,CArray <CString,CString> &aList,int &nSize)
{
	//��������� ������
	//���-�� ��������� � �������
	//����� ����� ���������
	//��� ���������
	//����� i-�� �������� ���������
	//������ i-�� �������� ���������
	int i;
	nSize=GetSizeStringArray(csName,aList);//�������� ������ ������ ������
	char *buffer=new char[nSize];

	//��������� ������ � �����
	int n=0;
	int nLength=aList.GetSize();
	memcpy(&buffer[n],&nLength,sizeof(nLength));//���-�� ��������� � �������
	n+=sizeof(nLength);

	nLength=csName.GetLength();
	memcpy(&buffer[n],&nLength,sizeof(nLength));//����� ����� ���������
	n+=sizeof(nLength);

	memcpy(&buffer[n],csName.GetBuffer(0),nLength);//��� ���������
	n+=nLength;

	for(i=0;i<aList.GetSize();i++)
	{
		nLength=aList[i].GetLength();
		memcpy(&buffer[n],&nLength,sizeof(nLength));//����� i-�� �������� ���������
		n+=sizeof(nLength);

		memcpy(&buffer[n],aList[i].GetBuffer(0),nLength);//������ i-�� �������� ���������
		n+=nLength;
	}
	return buffer;
}

int SendStringArray(HWND hwnd,CString csName,CArray <CString,CString> &aList,int nMode,int bPost=0)
{
#ifdef MESSAGE_DEBUG
	OutToDebug("Send data %2.0f: %s\n",nMode,csName);
#endif

	int nSize;
	char *buffer=GetBufferStringArray(csName,aList,nSize);


	COPYDATASTRUCT cpd;
	cpd.dwData = 1;//������� ������� �����
	cpd.cbData = nSize;
	cpd.lpData = (void*)buffer;
	MySendMessage(hwnd,WM_COPYDATA,
				(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
				(LPARAM)&cpd,0);

	delete []buffer;
	return MySendMessage(hwnd,WM_MESSAGE_FROM_2C,nMode,0,bPost);//��������� ��������
}

int SendStringArray(HWND hwnd,CString csName,CArray <void*,void*> &aPList,int nMode,int bPost=0)
{
#ifdef MESSAGE_DEBUG
	OutToDebug("Send data2 %2.0f: %s\n",nMode,csName);
#endif

	int i;
	int nAllSize=sizeof(int);//��� ���������� ��������
	for(i=0;i<aPList.GetSize();i++)
	{
		CArray <CString,CString> *aList=(CArray <CString,CString> *)aPList[i];
		nAllSize+=GetSizeStringArray(csName,*aList);//�������� ������ ������ ������
		nAllSize+=sizeof(int);//��� ����� ������� �����
	}
	if(nAllSize==0)
		return 0;

	//nAllSize+=10;

	char *bufferAll=new char[nAllSize];
	int n=0;
	int nSize=aPList.GetSize();
	memcpy(&bufferAll[n],&nSize,sizeof(nSize));//���������� ��������
	n+=sizeof(nSize);
	for(i=0;i<aPList.GetSize();i++)
	{
		int nSize;
		CArray <CString,CString> *aList=(CArray <CString,CString> *)aPList[i];
		char *buffer=GetBufferStringArray(csName,*aList,nSize);
		memcpy(&bufferAll[n],&nSize,sizeof(nSize));//����� ������� �����
		n+=sizeof(nSize);
		memcpy(&bufferAll[n],&buffer[0],nSize);//������ �����
		n+=nSize;
		delete []buffer;
	}

		


	COPYDATASTRUCT cpd;
	cpd.dwData = 2;//������� ����. ������� �����
	cpd.cbData = nAllSize;
	cpd.lpData = (void*)bufferAll;
	MySendMessage(hwnd,WM_COPYDATA,
				(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
				(LPARAM)&cpd,0);

	delete []bufferAll;
	return MySendMessage(hwnd,WM_MESSAGE_FROM_2C,nMode,0,bPost);//��������� ��������
}

void SendStringToConfig(CString csMessage)
{
	if(afxAppRunMode!=CONFIG_MODE)
	{
		HWND hwnd=::FindWindow(NULL,MYMESSAGE_WND_TITLE);
		if(hwnd)
			SendString(hwnd,csMessage);
	}
}

				
void ActivateConfig(CString csText,int nErrorLine)
{
	if(afxAppRunMode==CONFIG_MODE)
	{
		GotoLine(csText,nErrorLine,0);
	}
	else
	{
		HWND hwnd=::FindWindow(NULL,MYMESSAGE_WND_TITLE);
		if(hwnd)
		{
			HWND hwndp=::GetParent(hwnd);
			if(hwndp)
			{
				::BringWindowToTop(hwndp);
				::SetFocus(hwndp);
				//::SetForegroundWindow(hwndp); 
			}
			SendString(hwnd,csText,MYMESSAGE_ACTIVATE,nErrorLine);
		}
	}

}

#include "MetadataTree.h"

int GotoLine(CString Str,int nLine,BOOL bRun)
{
	if(afxAppRunMode==CONFIG_MODE)
	{
		CMetadataTree* pMetaTree=AfxGetMetadataTree();
		if(pMetaTree)
			return pMetaTree->EditModule(Str,nLine,bRun);
	}
	return 0;
}

//************������� ���������*************************
//************������� ���������*************************
//************������� ���������*************************

HWND Get2C()
{
	Sleep(10);//������� ����� ����������� ������������ �������� :)   - �� ����� ���� ����� ����� ����������� ����, ������� ������ �� 100% ������ ������...
	if(afxAppRunMode==CONFIG_MODE)
	{
		return ::FindWindow(NULL,MYMESSAGE_WND_DEBUG);
	}
	else
		return ::FindWindow(NULL,MYMESSAGE_WND_TITLE);
}


void AddDebugPoint(CString csModule,int nLine)
{
	aDebugList.AddPoint(csModule,nLine);
	bUseDebug=1;
	bUseDebug2=1;
}

CMapInt *GetDebugList(CString csModule)
{
	if(csModule==csLastDebugListName)
		return pLastDebugList;

	pLastDebugList=0;
	csLastDebugListName=csModule;

	if(aDebugList.aModuleList.GetCount()==0)
		return 0;
	BOOL bRes=aDebugList.aModuleList.Lookup(mUpper(csModule),(void*&)pLastDebugList);
	if(bRes)
	{
		return pLastDebugList;
	}
	return 0;
}
void CheckDebugPoint()
{
	bUseDebug=0;
	pLastDebugList=0;
	csLastDebugListName="";

	//���������� ���������� �����, ���� ���� ���� �� ���� ����� ��������
	if(aDebugList.aModuleList.GetCount()==0)
	{
		return;
	}

	CMapInt *p;
	POSITION pos;
	CString csKey;
	for( pos = aDebugList.aModuleList.GetStartPosition(); pos != NULL; )
	{
		aDebugList.aModuleList.GetNextAssoc( pos, csKey, (void*&)p );
		if(p)
		if(p->GetSize())
		{
			bUseDebug=1;
			break;
		}
	}
}

void ClearDebugPoint()
{
	pLastDebugList=0;
	csLastDebugListName="";
	if(afxAppRunMode!=CONFIG_MODE)
	{
		aDebugList.Clear();
		bUseDebug=0;
	}
}

void ClearDebugPoint(CString csModule)
{
	CMapInt *p=GetDebugList(csModule);
	if(p)
	{
		p->RemoveAll();
		aDebugList.aModuleList[mUpper(csModule)]=0;
	}

	CheckDebugPoint();
	SendDebugPoint(1);
}


void SendDebugPoint(BOOL bPost)
{
	HWND hwnd=Get2C();
	if(hwnd)
	{
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,REMOVE_ALL_DEBUG_POINT,0,bPost);


		CMapInt *pArray;
		POSITION pos;
		CString csKey;
		for( pos = aDebugList.aModuleList.GetStartPosition(); pos != NULL; )
		{
			aDebugList.aModuleList.GetNextAssoc( pos, csKey, (void*&)pArray );
			if(pArray)
			{
				for(int i=0;i<pArray->GetSize();i++)
				{
					if(pArray->GetAt(i))
						SendString(hwnd,csKey,ADD_DEBUG_POINT,i,bPost);
				}
			}
		}
	}
}

void DoDebugLoop(CString csModule,int nLine,CRunContext *pSetRunContext)
{
	pRunContext=pSetRunContext;
	HWND hwnd=Get2C();
	if(!hwnd)//��� �������������
	{
		bUseDebug=0;
		pRunContext=0;
		return;
	}


	HWND hwndp=::GetParent(hwnd);
	int nRes=SendString(hwnd,csModule,SET_CUR_LINE,nLine,1);
	if(nRes==0)
	{
		pRunContext=0;
		return;
	}
	if(hwndp)
	{
		::BringWindowToTop(hwndp);
		::SetFocus(hwndp);
	}
	//Sleep(10);

	bDebugLoop=1;
	bDebugStopLine=0;
	nCurrentNumberStopContext=0;
	while(bDebugLoop)
	{
		MSG msg;
		//����� ������� ��� ���������
		while( ::PeekMessage( &msg, NULL, WM_COPYDATA, WM_COPYDATA, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_COPYDATA, WM_COPYDATA ) )
				break;
			::DispatchMessage( &msg );
		}
		while( ::PeekMessage( &msg, NULL, WM_MESSAGE_FROM_2C, WM_MESSAGE_FROM_2C, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_MESSAGE_FROM_2C, WM_MESSAGE_FROM_2C ) )
				break;
			::DispatchMessage( &msg );
		}

		//���������� ����
		while( ::PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_PAINT, WM_PAINT ) )
				break;
			::DispatchMessage( &msg );
		}

		//��������� ����� � ���� ���������
		if(pMessageWindow)
		while( ::PeekMessage( &msg, pMessageWindow->m_hWnd, 0, 0, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, pMessageWindow->m_hWnd, 0, 0 ) )
				break;
			::DispatchMessage( &msg );
		}



		Sleep(10);
		hwnd=Get2C();
		if(!hwnd)//��� �������������
		{
			ClearDebugPoint();
			pRunContext=0;
			return;
		}
	}
	pRunContext=0;
}
void RunDebug()
{
	HWND hwnd=Get2C();
	if(hwnd)
	{
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,RUN_DEBUG,0,1);
		//::BringWindowToTop(hwnd);
	}
}

void NextDebug(int nRunLine)
{
	HWND hwnd=Get2C();
	if(hwnd)
	MySendMessage(hwnd,WM_MESSAGE_FROM_2C,NEXT_DEBUG,0,1);
}

void NextDebugOver(int)
{
	HWND hwnd=Get2C();
	if(hwnd)
	MySendMessage(hwnd,WM_MESSAGE_FROM_2C,NEXT_DEBUG_OVER,0,1);
}

void RemoveAllLists()
{
	aAttributeName.RemoveAll();
	aAttributeType.RemoveAll();
	aAttributeValue.RemoveAll();
}
void AddAttribute(CString csVariable,CArray <CString,CString> &aList)
{
	CLock cs(m_criticalSection2, "AddAttribute",bInitCriticalSection2);
	CMustZeroOnExit mz(afxMessageFromConfig);

	afxMessageFromConfig=1;

	try
	{


		CString _afxStrErrorMessage=afxStrErrorMessage;
		afxStrErrorMessage="";
		CValue vResult;
		if(pRunContext)
			vResult=CProcUnit::Eval(csVariable,pRunContext);

		aList.Add(csVariable);
		if(afxStrErrorMessage.IsEmpty())
		{
			aList.Add(vResult.GetTypeString());
			StrMessage=vResult.GetString();
		}
		else
		{
			aList.Add(NAME_ERROR);
			StrMessage="";
		}
		aList.Add(vResult.GetString());

		afxStrErrorMessage=_afxStrErrorMessage;


		if(TYPE_ARRAY==vResult.GetType())
		{
			CValueArray *pArray=(CValueArray *)vResult.pRef;
			if(pArray) 
			{
				CStringExt csKey;
				CValue Val;
				CString csName;
				for(int i=0;i<pArray->GetSizeStrArray();i++)
				{
					csName.Format("[\"%s\"]",pArray->GetIdentifierByNumber(i));
					Val=pArray->GetAt(pArray->GetIdentifierByNumber(i));
					aList.Add(csName);
					aList.Add(Val.GetTypeString());
					aList.Add(Val.GetString());
				}


				//�.�. ������ � ��� ������� �������� � ����������������� ������� ������, �� ������� �� �������� � ������ � ���������
				int nCount=pArray->GetSizeArray();
				CArray <int,int&>  List;
				List.SetSize(nCount);
				CString csKey2;
				for(int nKey=1;nKey<=nCount;nKey++)
				{
					csKey2.Format("[%d]",nKey);
					Val=pArray->GetAt(nKey);
					aList.Add(csKey2);
					aList.Add(Val.GetTypeString());
					aList.Add(Val.GetString());
				}
			}
		}
		else
		if(vResult.nType==TYPE_REFFER)
		{
			int nCount=vResult.pRef->GetNAttributes();
			for(int i=0;i<nCount;i++)
			{
				CString csName=vResult.pRef->GetAttributeName(i,1);
				int n=vResult.pRef->FindAttribute(csName);
				if(n>=0)
				{
						afxMessageFromConfig=1;
						CValue vAttribute=vResult.pRef->GetAttribute(n);

						aList.Add(csName);
						aList.Add(vAttribute.GetTypeString());
						aList.Add(vAttribute.GetString());
				}
			}
		}
	}
	catch(...)
	{
		//afxMessageFromConfig=0;
	}

	afxMessageFromConfig=0;
}
void SetAttribute(CString csVariable,HWND hwnd)
{
	CArray <CString,CString> aList;
	AddAttribute(csVariable,aList);
	SendStringArray(hwnd,StrMessage,aList,SET_STRING_ARRAY);
	aList.RemoveAll();
}
//�������� �������� ������� ���������� (aList) � ������������
void SendVarStringArray()
{
	HWND hwnd=Get2C();
	if(hwnd)
	{
		ClearPList();
		aPList.SetSize(min(MAX_LIST,aList.GetSize()));
		for(int i=0;i<aPList.GetSize();i++)
		{
			CArray <CString,CString> *pList=new CArray <CString,CString>;
			aPList[i]=pList;
			if(aList[i]!="<...>")
				AddAttribute(aList[i],*pList);
		}
		SendStringArray(hwnd,"+",aPList,SET_VAR_STRING_ARRAY);
		ClearPList();
	}
}

CString GetVariable(CString csVariable)
{
	HWND hwnd=Get2C();
	if(!bConfigGetAttribute)
	{
		bConfigGetAttribute=1;
		if(afxAppRunMode==CONFIG_MODE)
		{
			if(hwnd)
			{
				RemoveAllLists();
				SendString(hwnd,csVariable,GET_VARIABLE);
				CString Str=StrMessage;
				StrMessage="";
				bConfigGetAttribute=0;
				return Str; 
			}
		}
		else
		{
			if(hwnd)
			{
				try
				{
					SetAttribute(csVariable,hwnd);
					MySendMessage(hwnd,WM_MESSAGE_FROM_2C,SET_VARIABLE,0);
				}
				catch(CTranslateError *)
				{
				}
			}
		}
		bConfigGetAttribute=0;
	}
	return "";
}

void PauseDebug()
{
	HWND hwnd=Get2C();
	if(hwnd)
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,PAUSE_DEBUG,0,1);
}

void StopDebug()
{
	HWND hwnd=Get2C();
	if(hwnd)
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,STOP_DEBUG,0,1);
}

void StopProgram()
{
	HWND hwnd=Get2C();
	if(hwnd)
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,STOP_PROGRAM,0,1);
}

CArray <CString,CString> *GetStack()
{
	aList.RemoveAll();
	StrMessage="";
	HWND hwnd=Get2C();
	if(hwnd)
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,GET_STACK,0,0);
	return &aList;
}

#include <vector>
extern vector <CRunContext *> afxRunContext;           //������ ����������� ����� �������
void SetStack()
{
	HWND hwnd=Get2C();
	if(hwnd)
	{
		CArray <CString,CString> aList;
		for(int i=afxRunContext.size()-1;i>=0;i--)//�������� ����� �����
		{
			CRunContext *pRunContext=afxRunContext[i];
			ASSERT(pRunContext);
			if(!pRunContext)
				return;
			int nCurLine=pRunContext->nCurLine;
			ASSERT(nCurLine>=0);
			if(nCurLine<0)
				return;
			CByteCode *pByteCode=&pRunContext->pCompileContext->pModule->cByteCode;


			if(pByteCode)
			{
				int nCurPos=0;
				CString csModuleName=pByteCode->csModuleName;
				if(nCurLine>=0&&nCurLine<pByteCode->CodeList.GetSize())
				{
					nCurPos=pByteCode->CodeList[nCurLine].nStringNumber;
					csModuleName=pByteCode->CodeList[nCurLine].csModuleName;
					BOOL bError;
					CString csModule=GetModuleText(csModuleName,bError);
					if(!csModule.IsEmpty())
					{
						CString Str=FindErrorCodeLine(csModule,nCurPos,1);
						aList.Add(csModuleName+" "+Str);
					}
				}
			}
		}
		SendStringArray(hwnd,"#Stack",aList,SET_STACK_ARRAY);
	}

}

void SetCurrentStack(int nNumber)
{
	HWND hwnd=Get2C();
	if(!hwnd)
		return;

	if(afxAppRunMode==CONFIG_MODE)
	{
		MySendMessage(hwnd,WM_MESSAGE_FROM_2C,SET_CURRENT_STACK,nNumber);
	}
	else
	{
		if(afxRunContext.size()>nNumber)
			pRunContext=afxRunContext[afxRunContext.size()-nNumber-1];//��������� �������� ����� �����
	}
}

