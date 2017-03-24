//////////////////////////////////////////////////////////////////////
// BLCollection.h: interface for the CBLCollection template class.
// Copyright (c) 2005-2008. All Rights Reserved.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

/*
The ItemType class must be public delivered from CBLContext class and must
have this interface:
class ItemType
{
public:
    LPCSTR GetName() const;
}

The CollType class must have this interface:
class CollType
{
public:
    int GetSize() const;
    ItemType* Add(LPCSTR szName);
    ItemType* Insert(int nIndex, LPCSTR szName);
    ItemType* GetAt(int nIndex) const;
    ItemType* GetAt(LPCSTR szName) const;
    int IndexOf(LPCSTR szName) const;
    int IndexOf(ItemType* pItemType) const;
    BOOL Remove(int nIndex);
    BOOL Move(int nIndex, int nOffset);
    void RemoveAll();
}
*/

#pragma once

#define BLC_READONLY    0x00000000
#define BLC_MODIFED     0x00000001
#define BLC_INSERT      0x00000002
#define BLC_MOVE        0x00000004

template <class CollType, class ItemType, CRuntimeClass* pItemRuntimeClass, DWORD dwFlags = BLC_READONLY>
class CBLCollection : public CollType, public CBLContext
{
public:
    enum
    {
        methCount,
        methGet,
        methIndexOf,
        methAdd,
        methRemove,
        methClear,
        methInsert,
        methMove,
        LastMethod
    };

    CBLCollection()
    {
        if (m_ParamDefs.Empty())
        {
            m_ParamDefs.SetPoolSize(LastMethod, 0);
            m_ParamDefs.AddParam("Count", "Количество", methCount, 1, 0);
            m_ParamDefs.AddParam("Get", "Получить", methGet, 1, 2);
            m_ParamDefs.AddParam("IndexOf", "Индекс", methIndexOf, 1, 1);
            if (dwFlags & BLC_MODIFED)
            {
                m_ParamDefs.AddParam("Add", "Добавить", methAdd, 1, 1);
                m_ParamDefs.AddParam("Remove", "Удалить", methRemove, 0, 1);
                m_ParamDefs.AddParam("Clear", "Очистить", methClear, 0, 0);
                if (dwFlags & BLC_INSERT)
                    m_ParamDefs.AddParam("Insert", "Вставить", methInsert, 1, 2);
                if (dwFlags & BLC_MOVE)
                    m_ParamDefs.AddParam("Move", "Сдвинуть", methMove, 0, 2);
            }
        }
    };
    virtual ~CBLCollection()
    {};

	virtual void InitItem(ItemType*, CValue**) {};
	virtual int CountAddParams()const {return 0;};
	virtual BOOL GetDefValAddParams(int nParamIndex, CValue* pDefValue)const {return FALSE;};

    //CBLContext
    //virtual void			InitObject(char const* szName);
    virtual char const* 	GetTypeString(void)const
    {
        if (dwFlags == BLC_READONLY)
            return "ФиксированнаяКоллекция";
        else
            return "Коллекция";
    };
    
	virtual int				GetNProps(void)const
    {
        return GetSize();
    };
	virtual int				FindProp(char const* szName)const
    {
        return IndexOf(szName);
    };
	virtual char const* 	GetPropName(int nPropIndex, int nAlias)const
    {
        return GetAt(nPropIndex)->GetName();
    };
	virtual int				GetPropVal(int nPropIndex, class CValue& Value)const
    {
        Value.AssignContext(static_cast<ItemType*>(GetAt(nPropIndex)));
        return 1;
    };
	//virtual int				SetPropVal(int nPropIndex, class CValue const& Value);
	virtual int				IsPropReadable(int nPropIndex)const
    {
        return TRUE;
    };
	virtual int				IsPropWritable(int nPropIndex)const
    {
        return FALSE;
    };
    
	virtual int				GetNMethods(void)const
    {
        return m_ParamDefs.Size();
    };
	virtual int				FindMethod(char const* szName)const
    {
        return m_ParamDefs.GetIndexByName(szName);
    };
	virtual char const* 	GetMethodName(int nMethIndex, int nAlias)const
    {
        return m_ParamDefs[nMethIndex].Names[nAlias];
    };
	virtual int				GetNParams(int nMethIndex)const
    {
		switch (nMethIndex)
		{
		case methAdd:
		case methInsert:
			return m_ParamDefs[nMethIndex].NumberOfParams + CountAddParams();
		default:
			return m_ParamDefs[nMethIndex].NumberOfParams;
		}
    };
	virtual int				GetParamDefValue(int nMethIndex, int nParamIndex, class CValue* pDefValue)const
    {
		switch (nMethIndex)
		{
		case methAdd:
		case methInsert:
			return GetDefValAddParams(nParamIndex, pDefValue);
			break;
		case methGet:
			if (nParamIndex == 1)
			{
				pDefValue->Reset();
				return 1;
			}
			break;
		}
		return 0;
    };
	virtual int				HasRetVal(int nMethIndex)const
    {
        return m_ParamDefs[nMethIndex].HasReturnValue;
    };
	virtual int				CallAsProc(int nMethIndex, class CValue** ppValue)
    {
        if (dwFlags & BLC_MODIFED)
        {
            switch (nMethIndex)
            {
            case methRemove:
                if (!Remove(ppValue[0]->GetNumeric()))
                {
                    CBLModule::RaiseExtRuntimeError("Недопустимый индекс.", 0);
                    return 0;
                }
                break;
            case methClear:
                RemoveAll();
                break;
            case methMove:
                if (dwFlags & BLC_MOVE)
                {
                    if (!Move(ppValue[0]->GetNumeric(), ppValue[1]->GetNumeric()))
                    {
                        CBLModule::RaiseExtRuntimeError("Недопустимые параметры.", 0);
                        return 0;
                    }
                }
                break;
            }
            return 1;
        }
        else
            return 0;
    };

	virtual int				CallAsFunc(int nMethIndex, class CValue& RetValue, class CValue** ppValue)
    {
        switch (nMethIndex)
        {
        case methCount:
            RetValue = GetSize();
            break;
        case methAdd:
            if (dwFlags & BLC_MODIFED)
			{
				ItemType* pItemType = static_cast<ItemType*>(Add(ppValue[0]->GetString()));
				InitItem(pItemType,&(ppValue[1]));
                RetValue.AssignContext(pItemType);
			}
            break;
        case methInsert:
            if (dwFlags & BLC_INSERT)
            {
                ItemType* pItemType = static_cast<ItemType*>(Insert(ppValue[0]->GetNumeric(), ppValue[1]->GetString()));
                if (!pItemType)
                {
                    CBLModule::RaiseExtRuntimeError("Недопустимый тип первого параметра.", 0);
                    return 0;
                }
				InitItem(pItemType,&(ppValue[2]));
                RetValue.AssignContext(pItemType);
            }
            break;
        case methGet:
          {
            int nType = ppValue[0]->GetTypeCode();
            ItemType* pItem;
            if (nType == NUMBER_TYPE_1C)
            {
                pItem = static_cast<ItemType*>(GetAt(ppValue[0]->GetNumeric().operator long()));
                if (pItem)
                    *ppValue[1] = pItem->GetName();
            }
            else if (nType == STRING_TYPE_1C)
            {
                pItem = static_cast<ItemType*>(GetAt(ppValue[0]->GetString()));
                *ppValue[1] = IndexOf(ppValue[0]->GetString());
            }
            else
            {
                CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра.", 0);
                return 0;
            }
            if (pItem == NULL)
            {
				RetValue.Reset();
                //CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра.", 0);
                //return 0;
            }
			else
				RetValue.AssignContext(pItem);
          }
            break;
        case methIndexOf:
			/*
            if (ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C)
            {
                CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра.", 0);
                return 0;
            }
            ItemType* pItem = static_cast<ItemType*>(ppValue[0]->GetContext());
            if (pItem->GetRuntimeClass() != pItemRuntimeClass)
            {
                CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра.", 0);
                return 0;
            }
            RetValue = IndexOf(pItem);
            break;
			*/
            if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
            {
				ItemType* pItem = static_cast<ItemType*>(ppValue[0]->GetContext());
				if (pItem->GetRuntimeClass() != pItemRuntimeClass)
				{
					CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра.", 0);
					return 0;
				}
				RetValue = IndexOf(pItem);
			}
            else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
			{
				RetValue = IndexOf(ppValue[0]->GetString());
			}
			else
            {
                CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра.", 0);
                return 0;
            }
            break;
        }
        return 1;
    };

    static CParamDefs m_ParamDefs;
};

template <class CollType, class ItemType, CRuntimeClass* pItemRuntimeClass, BOOL bIsModifed>
CParamDefs CBLCollection<CollType, ItemType, pItemRuntimeClass, bIsModifed>::m_ParamDefs;
