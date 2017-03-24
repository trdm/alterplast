
#include "stdafx.h"
#include "GUID.h"

extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

/******************************************************************************************/
// CGUID class implementation
/******************************************************************************************/

enum {
    procFromString,
    funcToString,
    funcIsNil,
    procSetNil,
    funcIsEqual,
    procNew,
    lastDBGUIDMethod
};

class CObjID CGUID::ObjID;
class CParamDefs CGUID::defFnNames;

IMPLEMENT_DYNCREATE(CGUID, CBLContext);

CGUID::CGUID()
{
  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastDBGUIDMethod, 0);        
    defFnNames.AddParam("FromString", "ИзСтроки", procFromString, 0, 1);
    defFnNames.AddParam("ToString", "ВСтроку", funcToString, 1, 0);
    defFnNames.AddParam("IsNil", "Пустой", funcIsNil, 1, 0);
    defFnNames.AddParam("SetNil", "УстПустой", procSetNil, 0, 0);
    defFnNames.AddParam("IsEqual", "Равны", funcIsEqual, 1, 1);
    defFnNames.AddParam("New", "Новый", procNew, 0, 0);
  }

  ::UuidCreateNil(&m_guid);
}

CGUID::~CGUID()
{
}

// CBLContext implemetnation

int  CGUID::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
    int ret = 0;
	switch(iMethNum)
  {
    case funcToString:
      {
          rValue = GetCode();
          ret = 1;
      }
      break;
    case funcIsNil:
      {
          RPC_STATUS status;
          if (::UuidIsNil(&m_guid, &status))
              rValue = 1;
          else
              rValue = 0L;
          ret = 1;
      }
      break;
    case funcIsEqual:
      {
          GUID guid;
          GUID* pguid = &guid;
          if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
          {
              if (!ConvertFormStr(ppValue[0]->GetString(), &guid))
                 break;
          }
          else if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
          {
              if (strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName, "CGUID"))
              {
                  CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра.", 0);
                  break;
              }
              pguid = &static_cast<CGUID*>(ppValue[0]->GetContext())->m_guid;
          }
          RPC_STATUS status;
          if (::UuidEqual(&m_guid, pguid, &status))
              rValue = 1;
          else
              rValue = 0L;
          ret = 1;
      }
      break;
    default:
      {
      }
  };
  return ret;
}

int CGUID::ConvertFormStr(const char* ptr, GUID* guid)
{
    if (::UuidFromString((unsigned char*)ptr, guid) != RPC_S_OK)
    {
        CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра.", 0);
        return 0;
    }
    m_StringView.Empty();
   return 1;
}

int  CGUID::CallAsProc(int iMethNum,class CValue * * ppValue)
{
    int ret = 0;
	switch(iMethNum)
  {
    case procFromString:
      {
          CString s;
          s = ppValue[0]->GetString();
          if (!ConvertFormStr(s.GetBuffer(32+4), &m_guid))
          {
              ::UuidCreateNil(&m_guid);
              break;
          }
          ret = 1;
      }
      break;
    case procSetNil:
      {
          ::UuidCreateNil(&m_guid);
          m_StringView.Empty();
          ret = 1;
      }
      break;
    case procNew:
      {
          ::UuidCreate(&m_guid);
          m_StringView.Empty();
          ret = 1;
      }
      break;
    default:
      {
      }
  };
  return ret;
}


int  CGUID::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastDBGUIDMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}	
	return -1;
}

char const *  CGUID::GetMethodName(int iMethodNum,int iMethodAlias)const
{
  return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CGUID::GetNMethods(void)const
{
	return lastDBGUIDMethod;
}

int  CGUID::HasRetVal(int iMethodNum)const
{
  return defFnNames[iMethodNum].HasReturnValue ;
}

int CGUID::GetNParams(int iMethodNum)const
{
  return defFnNames[iMethodNum].NumberOfParams;
}

int  CGUID::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
    /*
	switch(iMethodNum)
  {
    case 0:
      {       
      }
      break;
    default:
      {
      }
  };
  */
  return 0;
}



void  CGUID::DecrRef(void)
{
	CBLContext::DecrRef();
}

char const *  CGUID::GetCode(void)const
{
    if (m_StringView.GetLength())
        return m_StringView;

    unsigned char* str;
    ::UuidToString((GUID*)&m_guid, &str);
    m_StringView = str;
    ::RpcStringFree(&str);
	return m_StringView;
}

int  CGUID::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CGUID::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CGUID::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CGUID::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long  CGUID::GetTypeID(void)const
{
	return 100;
}

char const *  CGUID::GetTypeString(void)const
{
	return "GUID";
}

class CType   CGUID::GetValueType(void)const
{
	CType tType(100);	
	return tType;
}


void  CGUID::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CGUID::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CGUID::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CGUID::IsExactValue(void)const
{
	return 0;
}

int  CGUID::IsOleContext(void)const
{
	return 0;
}

int  CGUID::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CGUID::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CGUID::IsSerializable(void)
{
	return 0;
}

int  CGUID::SaveToString(class CString & csStr)
{	
	return 0;
}

void  CGUID::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CGUID::GetNProps(void)const
{
	return 0;
}

char const *  CGUID::GetPropName(int A,int B)const
{	
	return NULL;
}

int  CGUID::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return 0;
}

int  CGUID::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return 0;
}

int  CGUID::FindProp(char const * Name)const
{
	return 0;
}

void CGUID::GetBinaryString(CString &str) const
{
    char* pStr = str.GetBufferSetLength(sizeof(GUID) * 2 + 2);
    *pStr++ = '0';
    *pStr++ = 'x';
    
    BYTE* pSour = (BYTE*)&m_guid;

    for (int i = 0; i < sizeof(GUID); i++)
    {
        BYTE b;
        b = pSour[i] >> 4;

        if (b < 10) b += '0'; else b += 'A' - 10;
        *pStr++ = b;

        b = pSour[i] & 0x0F;

        if (b < 10) b += '0'; else b += 'A' - 10;
        *pStr++ = b;
        
    }
}

void CGUID::CreateNew(void)
{
	::UuidCreate(&m_guid);
	m_StringView.Empty();
}
