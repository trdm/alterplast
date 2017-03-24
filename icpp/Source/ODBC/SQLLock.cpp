
#include "stdafx.h"
#include "SQLLock.h"

extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

/******************************************************************************************/
// CSQLLock class implementation
/******************************************************************************************/

enum {
    funcNewID,
    procSetPrefix,
    procSetParent,
    procSetOwner,
    procSetDateNum,
    funcNewCode,
    funcNewDocNumber,
    procSetObjectType,
    procSetTimeout,
    lastLockMethod
};

class CObjID CSQLLock::ObjID;
class CParamDefs CSQLLock::defFnNames;

IMPLEMENT_DYNCREATE(CSQLLock, CBLContext)

CSQLLock::CSQLLock(): m_DateNum(0,0,0)
{
  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastLockMethod, 0);        
    defFnNames.AddParam("GetNewID", "НовыйИД", funcNewID, 1, 0);
    defFnNames.AddParam("SetPrefix", "УстановитьПрефикс", procSetPrefix, 0, 1);
    defFnNames.AddParam("SetParent", "УстановитьРодителя", procSetParent, 0, 1);
    defFnNames.AddParam("SetOwner", "УстановитьВладельца", procSetOwner, 0, 1);
    defFnNames.AddParam("SetDateNum", "УстановитьДатуНомера", procSetDateNum, 0, 1);
    defFnNames.AddParam("GetNewCode", "НовыйКод", funcNewCode, 1, 0);
    defFnNames.AddParam("GetNewNumber", "НовыйНомер", funcNewDocNumber, 1, 0);
    defFnNames.AddParam("SetObjectType", "УстановитьТипОбъекта", procSetObjectType, 0, 2);
    defFnNames.AddParam("SetTimeout", "УстановитьТаймаут", procSetTimeout, 0, 1);
  }

  m_Type = 0;
  m_Timeout = DEFAULT_LOCK_TIMEOUT;

  CBLContext* pBLContext = CBLContext::GetLoadedContext(1);
  CValue val;
  CValue v1;
  CValue v2;
  CValue* arPar[] = {&v1, &v2};
  pBLContext->CallAsFunc(pBLContext->FindMethod("РабочаяДата"), val, arPar);
  m_DateNum = val.GetDate();
}

CSQLLock::~CSQLLock()
{
}

int CSQLLock::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
    int ret = 1;
    switch(iMethNum)
    {
    case funcNewID:
      {
        if (m_Type == 0)
        {
            CBLModule::RaiseExtRuntimeError("Не установлен объект.", 0);
            return 0;
        }
        CString Ret;

        char* Buf = Ret.GetBufferSetLength(9);
        if (m_ObjID.IsEmpty())
        {
            PrepRS();

            CString Query;
            if (m_Type == REFERENCE_TYPE_1C)
                Query.Format("set nocount on select max(id) from %s (updlock)", m_pSbCntTypeDef->GetTableName());
            else
                Query = "set nocount on select max(iddoc) from _1sjourn (updlock)";

            BOOL Res;
            Res = CODBCRecordset::Open(Query);
    
            if (!Res)
            {
                if (!ProcessError())
                    return 0;
            }
            else
            {
                memcpy(Buf, CODBCRecordset::operator [](0).AsString().operator LPCTSTR(), 6);
                Buf[6] = 0;
                m_ObjID.SetlObjID(strtol(Buf, NULL, 36));
            };
            CODBCRecordset::Close();
        };

        m_ObjID.SetlObjID(m_ObjID.GetlObjID() + 1);
        memcpy(Buf, GetObjIdString(m_ObjID).operator LPCSTR() + 1, 9);
        Buf[9] = 0;
        Ret.ReleaseBuffer();
        rValue = Ret;
      }
      break;
    case funcNewCode:
      {
        if (m_Type != REFERENCE_TYPE_1C)
        {
            CBLModule::RaiseExtRuntimeError("К данной блокировке нельзя применять этот метод.", 0);
            return 0;
        }

        if (m_LastCode == 0)
        {
            PrepRS();

            CString Query;
            Query.Format("set nocount on select max(code) from %s (updlock)\n", m_pSbCntTypeDef->GetTableName());

            bool fWhere = false;
            long ParentID = m_pSbCntTypeDef->GetParentID();
            if (ParentID && m_pSbCntTypeDef->GetCodeUnique() != REF_CODE_UNIQUE_ALL)
            {
                NextCond(Query, fWhere);
                Query += "parentext = ";
                Query += GetObjIdString(m_OwnerID);
                Query += '\n';
                fWhere = true;
            };
            
            if (m_pSbCntTypeDef->GetCodeUnique() == REF_CODE_UNIQUE_PAR && m_pSbCntTypeDef->GetLevelsLimit() > 1)
            {
                NextCond(Query, fWhere);
                Query += "parentid = ";
                Query += GetObjIdString(m_ParentID);
            };

            int PrefixLen = m_Prefix.GetLength();
            if (PrefixLen >= m_pSbCntTypeDef->GetCodeLen())
            {
                CBLModule::RaiseExtRuntimeError("Слишком длинный префикс.", 0);
                return 0;
            };

            if (m_pSbCntTypeDef->GetCodeEditType() == EDIT_TYPE_STR && PrefixLen)
            {
                NextCond(Query, fWhere);
                CString s;
                s.Format("left(code, %u) = '%s'", PrefixLen, m_Prefix.operator LPCTSTR());
                Query += s;
            };

            BOOL Res;
            Res = CODBCRecordset::Open(Query);
        
            if (!Res)
            {
                if (!ProcessError())
                    return 0;
            }
            else
            {
                CString s = CODBCRecordset::operator [](0).AsString();
                const char* pStr = s;

                if (PrefixLen && m_pSbCntTypeDef->GetCodeEditType() == EDIT_TYPE_STR)
                    pStr += PrefixLen;

                m_LastCode.FromString(pStr, NULL);
            };
            CODBCRecordset::Close();
        };

        m_LastCode = m_LastCode + CNumeric(int(1));
        rValue = FormatCode(m_pSbCntTypeDef->GetCodeEditType(), m_pSbCntTypeDef->GetCodeLen());
      }
      break;
    case funcNewDocNumber:
      {
        if (m_Type != DOCUMENT_TYPE_1C)
        {
            CBLModule::RaiseExtRuntimeError("К данной блокировке нельзя применять этот метод.", 0);
            return 0;
        }
        if (m_pDocDef == NULL)
        {
            CBLModule::RaiseExtRuntimeError("Не установлен вид документа.", 0);
            return 0;
        }
        
        if (m_LastCode == 0)
        {
            PrepRS();

            CString Query;
            Query = "set nocount on select max(docno) from _1sjourn (updlock) where dnprefix = '";
            
            CString DNPrefix;
            long Numerator = m_pDocDef->GetID();
            if (m_pDocDef->GetDocNumDefID())
                Numerator = m_pDocDef->GetDocNumDefID();
            
            CString s;
            switch (m_pDocDef->GetNumUnique())
            {
            case DOC_NUM_PER_ALL:
                break;
            case DOC_NUM_PER_YEAR:
                s.Format("%04u", m_DateNum.GetYear());
                break;
            case DOC_NUM_PER_QUART:
                s.Format("%04u%u", m_DateNum.GetYear(), (m_DateNum.GetMonth() / 4) + 1);
                break;
            case DOC_NUM_PER_MONTH:
                s.Format("%04u%02u", m_DateNum.GetYear(), m_DateNum.GetMonth());
                break;
            case DOC_NUM_PER_DAY:
                s.Format("%04u%02u%02u", m_DateNum.GetYear(), m_DateNum.GetMonth(), m_DateNum.GetMonthDay());
                break;
            };
            DNPrefix.Format("%10u%-8s", Numerator, s.operator LPCTSTR());

            Query += DNPrefix;
            Query += '\'';

            int PrefixLen = m_Prefix.GetLength();
            if (PrefixLen >= m_pDocDef->GetNumLen())
            {
                CBLModule::RaiseExtRuntimeError("Слишком длинный префикс.", 0);
                return 0;
            };

            if (m_pDocDef->GetNumEditType() == EDIT_TYPE_STR && PrefixLen)
            {
                s.Format(" and left(docno, %u) = '%s'", PrefixLen, m_Prefix.operator LPCTSTR());
                Query += s;
            };

            BOOL Res;
            Res = CODBCRecordset::Open(Query);
        
            if (!Res)
            {
                if (!ProcessError())
                    return 0;
            }
            else
            {
                CString s = CODBCRecordset::operator [](0).AsString();
                const char* pStr = s;

                if (PrefixLen && m_pDocDef->GetNumEditType() == EDIT_TYPE_STR)
                    pStr += PrefixLen;

                m_LastCode.FromString(pStr, NULL);
            };
            CODBCRecordset::Close();
        };

        m_LastCode = m_LastCode + CNumeric(int(1));
        rValue = FormatCode(m_pDocDef->GetNumEditType(), m_pDocDef->GetNumLen());
      }
      break;
    default:
      {
        CString err;
        err.Format("Нет обработчика метода под номером: %d)", iMethNum);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }      
    }
  return ret;
}

int CSQLLock::CallAsProc(int iMethNum,class CValue * * ppValue)
{
    int ret = 1;
    switch(iMethNum)
    {
    case procSetPrefix:
      {
        m_Prefix = ppValue[0]->GetString();
        m_LastCode = 0;
      }
      break;
    case procSetParent:
        m_ParentID = ppValue[0]->GetObjID();
        m_LastCode = 0;
      break;
    case procSetOwner:
        m_OwnerID = ppValue[0]->GetObjID();
        m_LastCode = 0;
      break;
    case procSetDateNum:
        m_DateNum = ppValue[0]->GetDate();
        m_LastCode = 0;
      break;
    case procSetObjectType:
      {
        m_ObjID.Empty();
        m_LastCode = 0;
        if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
        {
            int type = ppValue[0]->GetNumeric();
            if (type != REFERENCE_TYPE_1C && type != DOCUMENT_TYPE_1C)
            {
                CBLModule::RaiseExtRuntimeError("Не вырный тип объекта.", 0);
                return 0;
            }
            m_Type = type;
        }
        else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
        {
            CString sType(ppValue[0]->GetString());
            if (ppValue[0]->GetString().CompareNoCase("Справочник") == 0 || ppValue[0]->GetString().CompareNoCase("Reference") == 0)
                m_Type = REFERENCE_TYPE_1C;
            else if (ppValue[0]->GetString().CompareNoCase("Документ") == 0 || ppValue[0]->GetString().CompareNoCase("Document") == 0)
                m_Type = DOCUMENT_TYPE_1C;
            else
            {
                CBLModule::RaiseExtRuntimeError("Не верный тип объекта.", 0);
                return 0;
            }
        }

        if (m_Type == REFERENCE_TYPE_1C)
        {
            m_pSbCntTypeDef = pMetaDataCont->GetSTypeDef(ppValue[1]->GetString());
            if (m_pSbCntTypeDef == NULL)
            {
                CBLModule::RaiseExtRuntimeError("Не верный вид справочника.", 0);
                return 0;
            }
        }
        else
        {
            m_pDocDef = pMetaDataCont->GetDocDef(ppValue[1]->GetString());
            if (m_pDocDef == NULL)
            {
                CBLModule::RaiseExtRuntimeError("Не верный вид документа.", 0);
                return 0;
            }
        }
      }
      break;
    case procSetTimeout:
        m_Timeout = ppValue[0]->GetNumeric();
      break;
    default:
      {
        CString err;
        err.Format("Нет обработчика метода под номером: %d)", iMethNum);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }      
    }
  return ret;
}

int CSQLLock::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
    return 0;
}

int CSQLLock::FindMethod(char const * lpMethodName) const
{	
	return defFnNames.GetIndexByName(lpMethodName);
}

char const * CSQLLock::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int CSQLLock::GetNMethods(void)const
{
	return defFnNames.Size();
}

int CSQLLock::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CSQLLock::GetNParams(int iMethodNum)const
{	
    return defFnNames[iMethodNum].NumberOfParams;
}

void CSQLLock::DecrRef(void)
{
	CBLContext::DecrRef();
}

char const * CSQLLock::GetCode(void)const
{
	return 0;
}

int CSQLLock::GetDestroyUnRefd(void)const
{
	return 1;
}

void CSQLLock::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID CSQLLock::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData * CSQLLock::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long CSQLLock::GetTypeID(void)const
{
	return 100;
}

char const * CSQLLock::GetTypeString(void)const
{
	return "SQLLock";
}

class CType CSQLLock::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}

void CSQLLock::IncrRef(void)
{
	CBLContext::IncrRef();
}

//void CSQLLock::InitObject(class CType const & tType)
//{
//	CBLContext::InitObject(tType);
//}
//
//void CSQLLock::InitObject(char const * strName)
//{
//	CBLContext::InitObject(strName);
//}

int CSQLLock::IsExactValue(void)const
{
	return 0;
}

int CSQLLock::IsOleContext(void)const
{  
    return 0;
}

int CSQLLock::IsPropReadable(int iPropNum)const
{
	return 0;
}

int CSQLLock::IsPropWritable(int iPropNum)const
{
	return 0;
}

int CSQLLock::IsSerializable(void)
{
	return 0;
}

int CSQLLock::SaveToString(class CString & csStr)
{
	csStr = "";
	return 1;
}

void CSQLLock::SelectByID(class CObjID cID,long lNum)
{
    CBLContext::SelectByID(cID,lNum);
}

int CSQLLock::GetNProps(void)const
{
    return 0;
}

char const * CSQLLock::GetPropName(int A,int B)const
{
    return NULL;
}

int CSQLLock::GetPropVal(int iPropNum,class CValue & rValue)const
{
    return -1;
}

int CSQLLock::SetPropVal(int iPropNum,class CValue const & vValue)
{
    return -1;
}

int CSQLLock::FindProp(char const * Name)const
{
    return -1;
}

CString CSQLLock::FormatCode(int EditType, int Len)
{
    CString Ret;

    char Buf[max(MAX_CODE_LEN, MAX_DOC_NUM_LEN) + 1];

    m_LastCode.Convert(Buf, Len, 0);

    if (EditType == EDIT_TYPE_NUM)
    {
        Ret.Format("%*s", Len, Buf);
    }
    else
    {
        int PrefixLen = m_Prefix.GetLength();
        if (PrefixLen)
            Ret.Format("%s%0*s", m_Prefix.operator LPCTSTR(), Len - PrefixLen, Buf);
        else
            Ret.Format("%0*s", Len, Buf);
    }    
    return Ret;
}

void CSQLLock::NextCond(CString & str, bool &fCond)
{
    if (fCond)
        str += " and ";
    else
    {
        str += "where ";
        fCond = true;
    }
}

void CSQLLock::PrepRS()
{
    if (!IsConnected())
    {
        Attach(*(SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c));
        AttachRecordset(this);
        SetQueryTimeout(m_Timeout);
    };
}

int CSQLLock::ProcessError()
{
    const char* Err = CODBCRecordset::GetLastError();
    if (strlen(Err) > 0)
    {
        if (IsQueryTimeoutExpired())
            Err = "LockTimeoutExpired";
        CBLModule::RaiseExtRuntimeError(Err, 0);
        return 0;
    }
    return 1;
}
