// 1CPP_ODBCDatabase.cpp: implementation of the C1CPP_ODBCDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "1CPP_ODBCDatabase.h"
#include "..\\SQL\\MetaDataWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {  
  procClose,
  funcDriverConnect,
  funcGetConnectionTimeout,
  funcGetLastError,
  funcIsConnected,
  funcOpen,
  procSetConnectionTimeout,
  procSetLoginTimeout,
  procSetReadOnly,
  procAttach1C,
  procAttach1C_Syn,
  funcConfigDataSource,
  funcGetDriverVer,
  funcGetODBCDriverVer,
  procReconnectNative,
  procAttachIB,
	lastMethod
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;
extern CIBSet NativeIBSet;

class CObjID C1CPP_ODBCDatabase::ObjID;
class CParamDefs C1CPP_ODBCDatabase::defFnNames;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(C1CPP_ODBCDatabase, CBLContext)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C1CPP_ODBCDatabase::C1CPP_ODBCDatabase() : CBLContext() , is1CSQL_Attach(false)
{
  //AFX_MANAGE_STATE(AfxGetStaticModuleState());	
  static BOOL isSQL = "CODBCDB7" == CString(pDataBase7->GetRuntimeClass()->m_lpszClassName);
  if (isSQL)
  {
    Attach(*(SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c));
    is1CSQL_Attach = true;
  }
    //CBLModule::RaiseExtRuntimeError("Объект SQL_Query можно создавать и использовать только в SQL версии 1С:Предприятие!",0);  
  
  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastMethod + 1, 0);
    defFnNames.AddParam("Close", "Закрыть", procClose, 0, 0);    
    defFnNames.AddParam("DriverConnect", "Соединение", funcDriverConnect, 1, 1); 
    defFnNames.AddParam("GetConnectionTimeout", "ПолучитьЗначениеТаймаута", funcGetConnectionTimeout, 1, 0);     
    defFnNames.AddParam("GetLastError", "ПолучитьОписаниеОшибки", funcGetLastError, 1, 0);
    defFnNames.AddParam("IsConnected", "ЕстьСоединение", funcIsConnected, 1, 0);    
    // 1 - DSN 2 - User 3 - Pass
    defFnNames.AddParam("Open", "Открыть", funcOpen, 1, 3);      
    defFnNames.AddParam("SetConnectionTimeout", "УстЗначениеТаймаута", procSetConnectionTimeout, 0, 1);    
    defFnNames.AddParam("SetLoginTimeout", "УстЗначениеТаймаутаЛогина", procSetLoginTimeout, 0, 1);    
    defFnNames.AddParam("SetReadOnly", "УстТолькоЧтение", procSetReadOnly, 0, 1);    
    defFnNames.AddParam("Attach1C", "Присоеденить1С", procAttach1C, 0, 0);
    defFnNames.AddParam("Attach1C", "Присоединить1С", procAttach1C_Syn, 0, 0);
    defFnNames.AddParam("ConfigDataSource", "КонфигурироватьИсточник", funcConfigDataSource, 1, 3);    
    defFnNames.AddParam("GetDriverVer", "ПолучитьВерсиюДрайвера", funcGetDriverVer, 1, 0);
    defFnNames.AddParam("GetODBCVer", "ПолучитьВерсиюODBC", funcGetODBCDriverVer, 1, 0);
    defFnNames.AddParam("ReconnectNative", "ReconnectNative", procReconnectNative, 0, 1);
    defFnNames.AddParam("AttachIB", "ПрисоединитьИБ", procAttachIB, 0, 4);
  }
#ifdef _DEBUG
  //pBkEndUI->DoMessageLine("Активизация класса C1CPP_ODBCDatabase!", mmExclamation);	
#endif
}

C1CPP_ODBCDatabase::~C1CPP_ODBCDatabase()
{    
#ifdef _DEBUG
  pBkEndUI->DoMessageLine("Деактивизация класса C1CPP_ODBCDatabase!", mmExclamation);	
#endif
}

int  C1CPP_ODBCDatabase::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{  
  int ret = 0;
  switch (iMethNum)
  {
  case funcGetDriverVer:
    {
        rValue = GetDriverVer();
    }
    break;
  case funcGetODBCDriverVer:
    {
        rValue = GetDriverODBCVer();
    }
    break;
  case funcConfigDataSource:
    {
      WORD fRequestR = ppValue[0]->GetNumeric();
      WORD fRequest = 0;
      const CString& strDriver = ppValue[1]->GetString();
      CString strAttributes = ppValue[2]->GetString();
      switch (fRequestR)
      {
        case 1:
          fRequest = ODBC_ADD_DSN;
          break;
        case 2:
          fRequest = ODBC_CONFIG_DSN;
          break;
        case 3:
          fRequest = ODBC_REMOVE_DSN;
          break;
        case 4:
          fRequest = ODBC_ADD_SYS_DSN;
          break;
        case 5:
          fRequest = ODBC_CONFIG_SYS_DSN;
          break;
        case 6:
          fRequest = ODBC_REMOVE_SYS_DSN;
          break;
        default:
          CBLModule::RaiseExtRuntimeError("Передано неверное значение первого параметра!", 0);
      }
      strAttributes.Replace(';', '\0');
      rValue = CNumeric(ConfigDataSource(fRequest, strDriver, strAttributes));
    }
  case funcDriverConnect:
    { 
      if (is1CSQL_Attach)
      {
        m_bIsAttached = false;
        m_hDbc = NULL;
        is1CSQL_Attach = false;
      }
      rValue = DriverConnect(ppValue[0]->GetString());
    }
    break;
  case funcGetConnectionTimeout:
    {
      rValue = GetConnectionTimeout();
    }
    break;
  case funcGetLastError:
    {
      rValue = GetLastError();
    }
    break;
  case funcIsConnected:
    {
      rValue = IsConnected();
    }
    break;
  case funcOpen:
    {      
      if (is1CSQL_Attach)
      {
        m_bIsAttached = false;
        m_hDbc = NULL;
        is1CSQL_Attach = false;
      }
      rValue = Open(ppValue[0]->GetString(), (CHAR*)(LPCSTR)ppValue[1]->GetString(), (CHAR*)(LPCSTR)ppValue[2]->GetString());
    }
    break;
  default:
    {
      CString err;
      err.Format("Нет обработчика метода под номером: %d)", iMethNum);
      CBLModule::RaiseExtRuntimeError(err, 0);
    }
  }
	return 1;
}

CString FormatODBCError(SQLHDBC hdbc, LPCSTR szPrefix)
{
	SQLCHAR state[5];
	SQLINTEGER native;
	CString sMsg;
	SQLCHAR* pchMsg = (SQLCHAR*)sMsg.GetBufferSetLength(256);
	SQLSMALLINT nLen;
    CString sLastError;

	SQLGetDiagRec(
		SQL_HANDLE_DBC,
		hdbc,
		1,
		state,
		&native,
		pchMsg,
		256,
		&nLen);
	sLastError.Format("%s : State %5s, native %u, message %s", szPrefix, state, native, pchMsg);
    return sLastError;
}

void AliveWait(UINT uTimeout)
{
    UINT nTimerID = SetTimer(NULL, 0, uTimeout, NULL);
    BOOL bBreak = FALSE;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) && !bBreak)
    {
        switch (msg.message)
        {
        case WM_TIMER:
            bBreak = TRUE;
            break;
        case WM_NCPAINT:
        case WM_PAINT:
            DispatchMessage(&msg);
            break;
        }
    }
    KillTimer(NULL, nTimerID);
}

class CSpyGame
{
public:
	virtual void HashString(CString &strPwd); // хэширует любую строку
	virtual void UnknownFunc(CString &str);  // получает некое дополнение до "10000"
};

BOOL VerifyUserIB(LPCSTR szPath, LPCSTR szUserName, LPCSTR szPassword, DWORD &UserdefChecksum)
{
	BOOL bSuccess = FALSE;
	CString strFileName;
	strFileName.Format("%s%s", szPath, "usrdef\\users.usr");

	//CUsersDoc<-CContainer<-CDocument<-CCmdTarget<-CObject
	//CUserItem<-CUsrDBaseItemDoc<-CDocument<-CCmdTarget<-CObject
	
	CContainer *pUsersDoc = (CContainer*)CContainer::CreateObject();
	pUsersDoc->SetReadOnlyMode(1);
	pUsersDoc->OnOpenDocument(strFileName);
	CContainerPage *pContPage = NULL;
	for(int i=0; i<pUsersDoc->m_objArray.GetSize(); i++)
	{
		pContPage = (CContainerPage*)pUsersDoc->m_objArray.GetAt(i);
		if(!lstrcmpi(szUserName, pContPage->m_str2)) //m_str2 - имя пользователя из Container.Content users.usr
		{
			if (pUsersDoc->ReadPage(i))
			{
				CDocument *pUserItem = pContPage->m_pDoc; //документ прочитанный из страницы i 
				LPCSTR szUserPwd = *(const char**)(((char*)pUserItem) + 0x70); //хэшированный пароль из users.usr

				//typedef CString (*HashString_)(char const *);
				//CString strPwd = ((HashString_)0x22090BF0)(szPassword);
				CString strPwd(szPassword);
				strPwd.MakeUpper();
				CSpyGame *pSG = (CSpyGame*)GetMainProfileName();
				pSG->HashString(strPwd);

				if(szUserPwd == strPwd)
					bSuccess = TRUE;
			}
			break;
		}
	}
	pUsersDoc->OnCloseDocument();

	CDataBase7::SetUserdefFileName(strFileName);//подставляем свой файл для получения КС
	if (!CDataBase7::GetUserdefChecksum(UserdefChecksum, 0))
		bSuccess = FALSE;

	strFileName.Format("%s%s", ((CApp7*)AfxGetApp())->GetProps()->GetStringProp(1).operator LPCTSTR(), "usrdef\\users.usr");
	CDataBase7::SetUserdefFileName(strFileName);//возвращаем на место нативный путь

	return bSuccess;
}

int  C1CPP_ODBCDatabase::CallAsProc(int iMethNum,class CValue * * ppValue)
{
  int ret = 0;
  switch(iMethNum)
  {
    case procClose:
      {         
        if (is1CSQL_Attach)
        {
          m_bIsAttached = false;
          m_hDbc = NULL;
          is1CSQL_Attach = false;
        }
        Close();
        ret = 1;
      } 
      break;
    case procSetConnectionTimeout:
      {
        SetConnectionTimeout(ppValue[0]->GetNumeric().operator long());
      }
      break;
    case procSetLoginTimeout:
      {
        SetLoginTimeout(ppValue[0]->GetNumeric().operator long());
      }
      break;      
    case procSetReadOnly:
      {
        SetReadOnly(ppValue[0]->GetNumeric().operator long());
      }
      break;
    case procAttach1C:
    case procAttach1C_Syn:
      {
        CMetaDataWork::UnloadMD(m_pMetaDataCont);
        SetIBSet(&NativeIBSet);
        Attach(*(SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c));
        is1CSQL_Attach = true;
      }
      break;
    case procReconnectNative:
      {
        if (!is1CSQL_Attach)
        {
            CBLModule::RaiseExtRuntimeError("Этот метод можно вызывать только на родном подключении.", 0);
            return 0;
        };

        SQLHDBC* phdbc = (SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c);
        SQLHSTMT* phstmt1 = (SQLHSTMT*)(long(pDataBase7->CanBeCommited())+0x50);
        SQLHSTMT* phstmt2 = (SQLHSTMT*)(long(pDataBase7->CanBeCommited())+0x54);

        SQLRETURN ret;
        CString sLastError;

        CODBCConnectParams cp;
        CApp7* pApp = (CApp7*)AfxGetApp();
        cp.FromFile(pApp->GetProps()->GetStringProp(0x7));
        
		CString& ConnectionString = ppValue[0]->GetString();
		ConnectionString.Replace("%server%",cp.GetServerName());
		ConnectionString.Replace("%uid%",cp.GetUID());
		ConnectionString.Replace("%pwd%",cp.GetPassword());
		ConnectionString.Replace("%db%",cp.GetDBName());

		//CString ConnectionString;
        //ConnectionString.Format("Driver={SQL Server};Server=%s;App=1CV7;UID=%s;PWD=%s;",
        //    cp.GetServerName(), cp.GetUID(), cp.GetPassword());

        //Done internal data
        pDataBase7->UndoUIDCtrlSet();
        CUsersSet::Undo();
        //GetSubcService()->Done();
        GetDocService()->Done();
        //GetAccService()->Done();
        //GetOperService()->Done();
        //GetTypOperService()->Done();
        GetDocStreamService()->Done();
        //GetTotalService()->Done();
        //UndoCrossDocSet();
        //

        BOOL bRes;
        CODBCRecordset rs(this);
        bRes = rs.Open("dbcc activecursors (@@spid)");
        if (bRes)
        {
            CString s;
            for (int nNum = 1; !rs.IsEof(); rs.MoveNext(), nNum++)
            {
                s.Format("#%d (%d) : %s", nNum, rs["Cursor Id"].AsInt(), rs["Stmt"].AsString().operator LPCTSTR());
                pBkEndUI->DoMessageLine(s, mmNone);
            }
            rs.Close();

            CUsersSet::Init(pDataBase7);
            pDataBase7->InitUIDCtrlSet();
            pDataBase7->GetMaxObjID(0);
            //GetDocStreamService()->Init();

            CBLModule::RaiseExtRuntimeError("Обнаружены открытые курсоры. Реконнект не выполнен.", 0);
            return 0;
        }
        rs.Close();
        
        ret = SQLDisconnect(*phdbc);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            CString s = FormatODBCError(*phdbc, "Ошибка SQLDisconnect");
            AfxMessageBox(s);
            CBLModule::RaiseExtRuntimeError(s, 0);
            return 0;
        };

        ret = SQLDriverConnect(*phdbc, NULL, (SQLCHAR*)ConnectionString.operator LPCTSTR(),
            SQL_NTS, NULL, 0, NULL, sqlNoPrompt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            CString s = FormatODBCError(*phdbc, "Ошибка SQLDriverConnect");
            AfxMessageBox(s);
            CBLModule::RaiseExtRuntimeError(s, 0);
            return 0;
        };

        ret = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt1);
        ret = SQLAllocHandle(SQL_HANDLE_STMT, *phdbc, phstmt2);

        ConnectionString = "use ";
        ConnectionString += cp.GetDBName();
        
        CString str;
        for (int nTryCount = 1; ; nTryCount++)
        {
            ret = SQLExecDirect(*phstmt1, (SQLCHAR*)ConnectionString.operator LPCTSTR(), SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
                break;
            
            SQLCHAR state[5];
            SQLINTEGER native;
            CString sMsg;
            SQLCHAR* pchMsg = (SQLCHAR*)sMsg.GetBufferSetLength(256);
            SQLSMALLINT nLen;
            
            SQLGetDiagRec(
                SQL_HANDLE_STMT,
                *phstmt1,
                1,
                state,
                &native,
                pchMsg,
                256,
                &nLen);

            if (native != 924)
            {
                str.Format("%s : State %5s, native %u, message %s", "Ошибка подключения к ИБ", state, native, pchMsg);
                AfxMessageBox(str);
                CBLModule::RaiseExtRuntimeError(str, 0);
                return 0;
            }

            str.Format("Ошибка подключения к ИБ (%d)", nTryCount);
            pBkEndUI->DoStatusLine(str);
            
            AliveWait(1000);
        }

        if (CUsersSet::Init(pDataBase7) == 0)
        {
            CString s = "Ошибка инициализации.";
            AfxMessageBox(s);
            CBLModule::RaiseExtRuntimeError(s, 0);
            return 0;
        };
        pDataBase7->InitUIDCtrlSet();
        pDataBase7->GetMaxObjID(0);
        //GetSubcService()->Init(0L);
        //GetDocService()->Init();
        //GetAccService()->Init(0);
        //GetOperService()->InitOper();GetOperService()->InitProv();
        //GetTypOperService()->Init();
        //GetDocStreamService()->Init();
      }
      break;
    case procAttachIB:
        {
			#define DEFAULT_DBF_CS "DRIVER={Microsoft Visual FoxPro Driver};Deleted=No;Null=Yes;Collate=RUSSIAN;Exclusive=No;SourceType=DBF;SourceDB=%s;"
			#define DEFAULT_SQL_CS "Driver={SQL Server};Server=%s;Database=%s;Uid=%s;Pwd=%s;"

			if (ppValue[0]->GetTypeCode() != STRING_TYPE_1C ||
				ppValue[1]->GetTypeCode() != STRING_TYPE_1C ||
				ppValue[2]->GetTypeCode() != STRING_TYPE_1C ||
				ppValue[3]->GetTypeCode() != STRING_TYPE_1C)
			{
                RuntimeErrorRes(4712); //Неверный тип параметра
			}

            CString strPath(ppValue[0]->GetString());
            CString strConnString(ppValue[3]->GetString());
            CString strFileName;

            strFileName.Format("%s%s", strPath.operator LPCTSTR(), "1cv7.dd");
            CFileStatus Status;
            int DataSourceType = -1;
            if (CFile::GetStatus(strFileName, Status)) //dbf
            {
				if (strConnString.IsEmpty())
					strConnString.Format(DEFAULT_DBF_CS,strPath.operator LPCTSTR());
                DataSourceType = DATA_SOURCE_TYPE_DBF;
            }
            else //sql
            {
				if (strConnString.IsEmpty())
				{
					DWORD UserdefChecksum;
					if (!VerifyUserIB(strPath, ppValue[1]->GetString(), ppValue[2]->GetString(), UserdefChecksum))
						RuntimeErrorRes(28759); //Введенный пароль неверен

					strFileName.Format("%s%s", strPath.operator LPCTSTR(), "1cv7.dba");
					CODBCConnectParams ConnParams;
					ConnParams.FromFile(strFileName);
					if (ConnParams.GetUserdefChecksum() != UserdefChecksum)
						ConnParams.Reset();
					
					if (ConnParams.IsEmpty())
						RuntimeErrorRes(29618); //Не указаны параметры соединения с базой данных SQL!

					strConnString.Format(DEFAULT_SQL_CS,
						ConnParams.GetServerName(),
						ConnParams.GetDBName(),
						ConnParams.GetUID(),
						ConnParams.GetPassword());
				}
                DataSourceType = DATA_SOURCE_TYPE_SQL;
            }

            if (is1CSQL_Attach)
            {
                m_bIsAttached = false;
                m_hDbc = NULL;
                is1CSQL_Attach = false;
            }
            if(!DriverConnect(strConnString))
				RuntimeError("Ошибка соединения: %s", GetLastError());

            if (DataSourceType == DATA_SOURCE_TYPE_DBF)
                SetIBSet(new CIBSetDBF(this));
            else
                SetIBSet(new CIBSetSQL(this));
            
            strFileName.Format("%s%s", strPath.operator LPCTSTR(), "1cv7.md");
            if (!CMetaDataWork::LoadMD(strFileName, m_pMetaDataCont))
                RuntimeErrorRes(28771); //Ошибка загрузки метаданных.
        }
        break;
    default:
      {
        CString err;
        err.Format("Нет обработчика метода под номером: %d)", iMethNum);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }
  };
	return ret;
}

int C1CPP_ODBCDatabase::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	BOOL bRes=FALSE;
	switch(iMethodNum)
	{
	case procSetReadOnly:
		if (iParamNum==0)
		{
			bRes=TRUE;
			*pDefValue=CNumeric(1);
		}
		break;
	case procReconnectNative:
		if (iParamNum==0)
		{
			bRes = TRUE;
			*pDefValue = "Driver={SQL Server};Server=%server%;App=1CV7;UID=%uid%;PWD=%pwd%;";
		}
		break;
	case procAttachIB:
		switch(iParamNum)
		{
		case 1:
			*pDefValue=((CApp7*)AfxGetApp())->GetProps()->GetStringProp(13);
			break;
		case 2:
			*pDefValue=((CApp7*)AfxGetApp())->GetProps()->GetStringProp(15);
			break;
		default:
			*pDefValue="";
		}
		bRes=TRUE;
		break;
	}
	return bRes;
}

int  C1CPP_ODBCDatabase::FindMethod(char const * lpMethodName) const
{	
	return defFnNames.GetIndexByName(lpMethodName);
}

char const *  C1CPP_ODBCDatabase::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  C1CPP_ODBCDatabase::GetNMethods(void)const
{
	return defFnNames.Size();
}

int  C1CPP_ODBCDatabase::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int C1CPP_ODBCDatabase::GetNParams(int iMethodNum)const
{	
	return defFnNames[iMethodNum].NumberOfParams;
}

void  C1CPP_ODBCDatabase::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  C1CPP_ODBCDatabase::GetCode(void)const
{
	return 0;
}

int  C1CPP_ODBCDatabase::GetDestroyUnRefd(void)const
{
	return 1;
}

void  C1CPP_ODBCDatabase::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   C1CPP_ODBCDatabase::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  C1CPP_ODBCDatabase::GetInternalData(void)
{

	return CBLContext::GetInternalData();

}




long  C1CPP_ODBCDatabase::GetTypeID(void)const
{
	return 100;
}

char const *  C1CPP_ODBCDatabase::GetTypeString(void)const
{
	return "ODBCDatabase";
}

class CType   C1CPP_ODBCDatabase::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  C1CPP_ODBCDatabase::IncrRef(void)
{
	CBLContext::IncrRef();
}

//void  C1CPP_ODBCDatabase::InitObject(class CType const & tType)
//{
//	CBLContext::InitObject(tType);
//}
//
//void  C1CPP_ODBCDatabase::InitObject(char const * strName)
//{
//	CBLContext::InitObject(strName);
//}

int  C1CPP_ODBCDatabase::IsExactValue(void)const
{
	return 0;
}

int  C1CPP_ODBCDatabase::IsOleContext(void)const
{
	return 0;
}

int  C1CPP_ODBCDatabase::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  C1CPP_ODBCDatabase::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  C1CPP_ODBCDatabase::IsSerializable(void)
{
	return 0;
}

int  C1CPP_ODBCDatabase::SaveToString(class CString & csStr)
{
	csStr = "";
	return 1;
}

void  C1CPP_ODBCDatabase::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  C1CPP_ODBCDatabase::GetNProps(void)const
{
	return 0;
}

char const *  C1CPP_ODBCDatabase::GetPropName(int A,int B)const
{
	return NULL;
}

int  C1CPP_ODBCDatabase::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return -1;
}

int  C1CPP_ODBCDatabase::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return -1;
}

int  C1CPP_ODBCDatabase::FindProp(char const * Name)const
{
	return -1;
}
