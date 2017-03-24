
#include "StdAfx.h"
#include "ValueCollection.h"

CParamDefs CValueCollection::m_ParamDefs;

enum
{
    //read only methods
    methCount,
    methGet,
    methIndexOf,
    //write methods
    methSet,
    LastMethod
};

CValueCollection::CValueCollection(BOOL bIsWitable /*= FALSE*/): m_bIsWitable(bIsWitable)
{
    if (m_ParamDefs.Empty())
    {
        m_ParamDefs.SetPoolSize(LastMethod, 0);
        m_ParamDefs.AddParam("Count", "Количество", methCount, 1, 0);
        m_ParamDefs.AddParam("Get", "Получить", methGet, 1, 1);
        m_ParamDefs.AddParam("IndexOf", "Индекс", methIndexOf, 1, 1);
        m_ParamDefs.AddParam("Set", "Установить", methSet, 0, 2);
    }
}

CValueCollection::~CValueCollection()
{
}

int	CValueCollection::IsPropReadable(int nPropIndex)const
{
    return TRUE;
}

int	CValueCollection::IsPropWritable(int nPropIndex)const
{
    return m_bIsWitable;
}

int	CValueCollection::GetNMethods(void)const
{
    return m_ParamDefs.Size();
}

int	CValueCollection::FindMethod(char const* szName)const
{
    int nMethID = m_ParamDefs.GetIndexByName(szName);
    if (!m_bIsWitable && nMethID >= methSet)
        return -1;
    else
        return nMethID;
}

char const* CValueCollection::GetMethodName(int nMethIndex, int nAlias)const
{
    return m_ParamDefs[nMethIndex].Names[nAlias];
}

int	CValueCollection::GetNParams(int nMethIndex)const
{
    return m_ParamDefs[nMethIndex].NumberOfParams;
}

int	CValueCollection::GetParamDefValue(int nMethIndex, int nParamIndex, CValue* pDefValue)const
{
    return FALSE;
}

int	CValueCollection::HasRetVal(int nMethIndex)const
{
    return m_ParamDefs[nMethIndex].HasReturnValue;
}

int	CValueCollection::CallAsProc(int nMethIndex, CValue** ppValue)
{
    int nType = ppValue[0]->GetTypeCode();
    if (nType == NUMBER_TYPE_1C)
    {
        int nIndex = ppValue[0]->GetNumeric();
        if (nIndex >= 0 && nIndex < GetNProps())
            return SetPropVal(nIndex, *ppValue[1]);
    }
    else if (nType == STRING_TYPE_1C)
    {
        int nIndex = FindProp(ppValue[0]->GetString());
        if (nIndex != -1)
            return SetPropVal(nIndex, *ppValue[1]);
    }
    CBLModule::RaiseExtRuntimeError("Недопустимое значение первого параметра.", 0);
    return FALSE;
}

int	CValueCollection::CallAsFunc(int nMethIndex, CValue& RetValue, CValue** ppValue)
{
    switch (nMethIndex)
    {
    case methCount:
        RetValue = GetNProps();
        break;
    case methGet:
        {
            int nType = ppValue[0]->GetTypeCode();
            if (nType == NUMBER_TYPE_1C)
            {
                int nIndex = ppValue[0]->GetNumeric();
                if (nIndex >= 0 && nIndex < GetNProps())
                    return GetPropVal(nIndex, RetValue);
            }
            else if (nType == STRING_TYPE_1C)
            {
                int nIndex = FindProp(ppValue[0]->GetString());
                if (nIndex != -1)
                    return GetPropVal(nIndex, RetValue);
            }
            CBLModule::RaiseExtRuntimeError("Недопустимое значение первого параметра.", 0);
            return FALSE;
        }
        break;
    case methIndexOf:
        RetValue = FindProp(ppValue[0]->GetString());
        break;
    }
    return TRUE;
}
