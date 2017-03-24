// MySQL_Query.cpp: implementation of the CMySQL_Query class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "MySQL_Query.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {
  procTest,
  procTest2,
  procTest3,
  procTest4,
  procTest5,
  procTest6,
  procTest7,
	lastMethod
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CMySQL_Query, CBLContext);

class CObjID CMySQL_Query::ObjID;
class CParamDefs CMySQL_Query::defFnNames;
//class CDatabase CMySQL_Query::m_db;
//class CODBCDatabase CMySQL_Query::m_db;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySQL_Query::CMySQL_Query() //: m_rs(&m_db)
{
  //AFX_MANAGE_STATE(AfxGetStaticModuleState());	
  CString strClassName(pDataBase7->GetRuntimeClass()->m_lpszClassName);
  if ("CODBCDB7" != strClassName)
    CBLModule::RaiseExtRuntimeError("ќбъект SQL_Query можно создавать и использовать только в SQL версии 1—:ѕредпри€тие!",0);  
  //DebugBreak();  

  if (!m_db.IsConnected())
    m_db.Attach(*(SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c));

  m_rs.AttachRecordset(&m_db);

  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastMethod + 1, 0);
    defFnNames.AddParam("Test", "Test", procTest, 0, 0);
    defFnNames.AddParam("Test2", "Test2", procTest2, 0, 0);
    defFnNames.AddParam("Test3", "Test3", procTest3, 0, 0);
    defFnNames.AddParam("Test4", "Test4", procTest4, 0, 0);
    defFnNames.AddParam("Test5", "Test5", procTest5, 0, 0);
    defFnNames.AddParam("Test6", "Test6", procTest6, 0, 1);
    defFnNames.AddParam("Test7", "Test7", procTest7, 0, 1);
    
  }
#ifdef _DEBUG
  //pBkEndUI->DoMessageLine("јктивизаци€ класса CMySQL_Query!", mmExclamation);	
#endif
}

CMySQL_Query::~CMySQL_Query()
{
  //m_db.m_hdbc = SQL_NULL_HDBC;
  m_rs.Close(TRUE);
#ifdef _DEBUG
  //pBkEndUI->DoMessageLine("ƒеактивизаци€ класса CMySQL_Query!", mmExclamation);	
#endif
}

int  CMySQL_Query::CallAsFunc(int iMethNum,CValue & rValue,CValue * *ppValue)
{
	return 1;
}

int  CMySQL_Query::CallAsProc(int iMethNum,CValue * * ppValue)
{
  switch(iMethNum)
  {
    case procTest7:
    {
      {

      }
      break;
    }
    case procTest:
      {       
        /*try
        {
          m_rs.Open("SELECT ROW_ID, ID, CODE, DESCR, ISMARK, VERSTAMP FROM SC18 (NOLOCK)", CRecordset::dynaset);
          for( ; ! m_rs.IsEOF(); m_rs.MoveNext() )
          {            
            pBkEndUI->DoMessageLine(m_rs("DESCR").AsString(), mmInformation);	
            /*if (m_rs.CanUpdate())
            {
              //m_rs.Delete();
              m_rs.Edit();	                
	            //m_rs.Field("ROW_ID") = 100;  // Implicit conversion
              //m_rs.Field("ID") = CString("10");  // Implicit conversion
              m_rs("DESCR") = CString("»зменил название");  // Implicit conversion
              //m_rs.Field("ISMARK") = 0;  // Implicit conversion
              //m_rs.Field("VERSTAMP") = 200;  // Implicit conversion
	            m_rs.Update();

            }
          }
          /*m_rs.AddNew();	        
	        m_rs.Field("ROW_ID") = 100;  // Implicit conversion
          m_rs.Field("ID") = CString("10");  // Implicit conversion
          m_rs.Field("DESCR") = CString("»зменил название");  // Implicit conversion
          m_rs.Field("ISMARK") = 0;  // Implicit conversion
          m_rs.Field("VERSTAMP") = 200;  // Implicit conversion
	        m_rs.Update();*/
          /*}

        }
        catch(CDBException* e)
        {
          pBkEndUI->DoMessageLine(e->m_strError, mmBlackErr);	          
          pBkEndUI->DoMessageLine(e->m_strStateNativeOrigin, mmBlackErr);
          e->Delete();          
        }*/        
        pBkEndUI->DoMessageLine("Test1:", mmInformation);
        // ќдноразовый запрос        
        m_rs.Execute("EXEC sp_dboption 'TestSQL', 'single user', 'FALSE'");        
        
        // «апрос с выборкой
        if (m_rs.Open("SELECT DESCR, ROW_ID, ID, CODE, ISMARK, VERSTAMP FROM SC12 (NOLOCK)"))
        {          
          for( ; ! m_rs.IsEof(); m_rs.MoveNext() )
          {
            // покажем значение колонки ROW_ID
            pBkEndUI->DoMessageLine((CString("ROW_ID = "))+m_rs["ROW_ID"].AsString(), mmInformation);
            // покажем значение колонки ID
            pBkEndUI->DoMessageLine((CString("ID = "))+m_rs["ID"].AsString(), mmInformation);
            // покажем значение колонки DESCR
            pBkEndUI->DoMessageLine((CString("DESCR = "))+m_rs["DESCR"].AsString(), mmInformation);
            pBkEndUI->DoMessageLine("==================================", mmInformation);
          }
        }
        else
        {
          pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);          
        }
        m_rs.Close();
        break;        
      }
    case procTest2:
      {
        pBkEndUI->DoMessageLine("Test2:", mmInformation);
        // ѕараметризированный запрос
        CType t(1);
        m_rs.AddParam("", SQL_PARAM_INPUT, SQL_C_BIT, t, 0,0);        
        (CDBField&)m_rs.GetParam(0) = false;
                     
        if (m_rs.Prepare("SELECT ROW_ID, ID, CODE, DESCR, ISMARK, VERSTAMP FROM SC12 (NOLOCK) WHERE ISMARK = ?"))
        {
          //pBkEndUI->DoMessageLine("Prepare is done!", mmInformation);
          //if (m_rs.Open("SELECT ROW_ID, ID, CODE, DESCR, ISMARK, VERSTAMP FROM SC12 (NOLOCK) WHERE ISMARK = ?", true))
          if (m_rs.Open(NULL, false, true)) 
          {
            pBkEndUI->DoMessageLine("Execute is done!", mmInformation);            
            for( ; ! m_rs.IsEof(); m_rs.MoveNext() )
            {                          
              pBkEndUI->DoMessageLine((CString("DESCR = "))+m_rs["DESCR"].AsString(), mmInformation);
              pBkEndUI->DoMessageLine("==================================", mmInformation);
            }
          }
          else
            pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);
        }
        m_rs.Close();
      }
      break;
    case procTest3:
      {
        // параметризированный запрос с автоматическим создание параметров
        pBkEndUI->DoMessageLine("Test3:", mmInformation);
        // подготовим к выполнению параметризированный запрос
        if (m_rs.Prepare("SELECT ROW_ID, DESCR, ID, CODE, ISMARK, VERSTAMP FROM SC12 (NOLOCK) WHERE ISMARK = ? AND CODE = ?"))
        {
          pBkEndUI->DoMessageLine("Prepare is done!", mmInformation);
          // построим параметры дл€ запроса
          if (m_rs.BuildParams())
          {
            // установим значение параметра ISMARK = ?
            (CDBField&)m_rs.GetParam(0) = true;
            // установим значение параметра CODE = ?
            (CDBField&)m_rs.GetParam(1) = "    1";
            // выполн€ем и открываем ранее подготовленну выборку
            if (m_rs.Open(NULL, false, true)) 
            {
              pBkEndUI->DoMessageLine("Execute is done!", mmInformation);            
              // если в выборке есть строки выведем их на экран
              for( ; ! m_rs.IsEof(); m_rs.MoveNext() )
              {                          
                pBkEndUI->DoMessageLine((CString("DESCR = "))+m_rs["DESCR"].AsString(), mmInformation);
                pBkEndUI->DoMessageLine("==================================", mmInformation);
              }
            }
          }
          else
          {
            pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);
          }
        }
        else
        {
          pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);          
        }
        m_rs.Close();
      }
      break;
    case procTest4:
      {
        // параметризированный запрос (вызов хранимой процедуры) с автоматическим создание параметров
        pBkEndUI->DoMessageLine("Test4:", mmInformation);
        // подготовим к выполнению параметризированный запрос
        if (m_rs.Prepare("{call _1sp_SC12_ByID(?)}"))
        {
          pBkEndUI->DoMessageLine("Prepare is done!", mmInformation);
          // построим параметры дл€ запроса
          if (m_rs.BuildParams())
          {
            // установим значение параметра ISMARK = ?
            (CDBField&)m_rs.GetParam(0) = "    1";
            
            // выполн€ем вызов процедуры
            m_rs.Execute(NULL, true);            
          }
          else
          {
            pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);
          }
        }
        else
        {
          pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);          
        }
        m_rs.Close();
      }
      break;
    case procTest5:
      {
        pBkEndUI->DoMessageLine("Test5:", mmInformation);
        m_rs.SetRowsetSize(5);
        if (m_rs.Open("SELECT DESCR, ROW_ID, ID, CODE, ISMARK, VERSTAMP FROM SC12 (NOLOCK)", true))
        {                   
          for( ; ! m_rs.IsEof(); m_rs.MoveNext() )
          {
            for (SQLUINTEGER i = 0; i < m_rs.GetRowFetched() ; i++)          
            {            
              // получим строку из массива данных блочного курсора (Block cursors)
              BYTE* Rowset = m_rs.GetRowset() + m_rs.GetRowSize()*i;
            
              // получим и покажем значение колонки DESCR
              if (m_rs.GetRowStatus(0) == SQL_ROW_SUCCESS || m_rs.GetRowStatus(0) ==  SQL_ROW_SUCCESS_WITH_INFO)
              {
                if (SQL_NULL_DATA == m_rs.GetFields()[1].m_nLength)
                  pBkEndUI->DoMessageLine("DESCR = NULL", mmRedErr);
                else
                {
                  CString DESCR("DESCR = ");
                  DESCR += &Rowset[m_rs.GetOffset(0)]; //DESCR            
                  pBkEndUI->DoMessageLine(DESCR, mmInformation);            
                }
              }
              if (m_rs.GetRowStatus(1) == SQL_ROW_SUCCESS || m_rs.GetRowStatus(1) ==  SQL_ROW_SUCCESS_WITH_INFO)
              {
              
              
                // получим и покажем значение колонки ROW_ID
                CString ROW_ID;
                long row_id = (long)Rowset[m_rs.GetOffset(1)];
                ROW_ID.Format("ROW_ID = %d", row_id);
                pBkEndUI->DoMessageLine(ROW_ID, mmInformation);
              }
              if (m_rs.GetRowStatus(2) == SQL_ROW_SUCCESS || m_rs.GetRowStatus(2) ==  SQL_ROW_SUCCESS_WITH_INFO)
              {
                // получим и покажем значение колонки ID
                CString ID("ID = ");
                ID += &Rowset[m_rs.GetOffset(2)]; //ID
                pBkEndUI->DoMessageLine(ID, mmInformation);
              }
            
              if (m_rs.GetRowStatus(3) == SQL_ROW_SUCCESS || m_rs.GetRowStatus(3) ==  SQL_ROW_SUCCESS_WITH_INFO)
              {
                // получим и покажем значение колонки CODE
                CString CODE("CODE = ");
                CODE += &Rowset[m_rs.GetOffset(3)]; //CODE
                pBkEndUI->DoMessageLine(CODE, mmInformation);
                pBkEndUI->DoMessageLine("==================================", mmInformation);            
              }
            }
          }
        }
        else
        {
          pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);          
        }
        m_rs.Close();
      }
      break;
    case procTest6:
      {
        if (m_rs.Prepare("INSERT INTO MyTable (NAME, AGE, SALARY, DATEOFBIRTH) VALUES(?, ?, ?, ?)"))
        //if (m_rs.Prepare("INSERT INTO MyTable (NAME, AGE) VALUES(?, ?)"))
        //if (m_rs.Prepare("INSERT INTO MyTable (NAME) VALUES(?)"))
        {
          m_rs.SetRowsetSizeForParams(5);          
          m_rs.BuildParams();                    
          m_rs.ResizeRowsetSizeForParams(10);
          for(int i = 1; i <= 10; ++i)
          {            
            const char* pName = "ƒимон";
            m_rs.SetParamOfRow(i, 1, strlen(pName), (void*)pName);
            int nAge = i + 20;
            m_rs.SetParamOfRow(i, 2, sizeof(nAge), &nAge);
            double nSalary = 1000*i;
            m_rs.SetParamOfRow(i, 3, sizeof(nSalary), &nSalary);
            TIMESTAMP_STRUCT pdate;
            memset(&pdate, 0, sizeof(pdate));
            pdate.year  = 1990;
            pdate.month = 12;
            pdate.day   = 1;            
            m_rs.SetParamOfRow(i, 4, sizeof(pdate), &pdate);

          }          
          if (!m_rs.Execute(NULL, TRUE))
          {
            pBkEndUI->DoMessageLine(m_rs.GetLastError(), mmBlackErr);
          }
          m_rs.Close();
        }        
      }
      break;
    default:
      {
      }
  };
	return 1;
}


int  CMySQL_Query::FindMethod(char const * lpMethodName) const
{	
	return defFnNames.GetIndexByName(lpMethodName);
}

char const *  CMySQL_Query::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CMySQL_Query::GetNMethods(void)const
{
	return defFnNames.Size();
}

int  CMySQL_Query::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CMySQL_Query::GetNParams(int iMethodNum)const
{	
	return defFnNames[iMethodNum].NumberOfParams;
}

int  CMySQL_Query::GetParamDefValue(int iMethodNum,int iParamNum,CValue * pDefValue)const
{	
	return 0;
}



void  CMySQL_Query::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  CMySQL_Query::GetCode(void)const
{
	return 0;
}

int  CMySQL_Query::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CMySQL_Query::GetExactValue(CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CMySQL_Query::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CMySQL_Query::GetInternalData(void)
{

	return CBLContext::GetInternalData();

}




long  CMySQL_Query::GetTypeID(void)const
{
	return 100;
}

char const *  CMySQL_Query::GetTypeString(void)const
{
	return "MySQL_Query";
}

CType   CMySQL_Query::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  CMySQL_Query::IncrRef(void)
{
	CBLContext::IncrRef();
}

//void  CMySQL_Query::InitObject(CType const & tType)
//{
//	CBLContext::InitObject(tType);
//}
//
//void  CMySQL_Query::InitObject(char const * strName)
//{
//	CBLContext::InitObject(strName);
//}

int  CMySQL_Query::IsExactValue(void)const
{
	return 0;
}

int  CMySQL_Query::IsOleContext(void)const
{
	return 0;
}

int  CMySQL_Query::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CMySQL_Query::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CMySQL_Query::IsSerializable(void)
{
	return 0;
}

int  CMySQL_Query::SaveToString(CString & csStr)
{
	csStr = "";
	return 1;
}

void  CMySQL_Query::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CMySQL_Query::GetNProps(void)const
{
	return 0;
}

char const *  CMySQL_Query::GetPropName(int A,int B)const
{
	return NULL;
}

int  CMySQL_Query::GetPropVal(int iPropNum,CValue & rValue)const
{
	return -1;
}

int  CMySQL_Query::SetPropVal(int iPropNum,CValue const & vValue)
{
	return -1;
}

int  CMySQL_Query::FindProp(char const * Name)const
{
	return -1;
}
