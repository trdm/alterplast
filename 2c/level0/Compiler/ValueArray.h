// ValueArray.h: interface for the CValueArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEARRAY_H__69AB6707_0AEA_48E1_8B37_7BEA1DD1621A__INCLUDED_)
#define AFX_VALUEARRAY_H__69AB6707_0AEA_48E1_8B37_7BEA1DD1621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "StringExt.h"

class CStringToValue : public CMap <CStringExt ,CStringExt ,CValue ,CValue&> 
{
public:
	CValue *GetPtrAt(CStringExt key,BOOL &bFind)
	{
		ASSERT_VALID(this);

		UINT nHash;
		CAssoc* pAssoc;
		if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
		{
			bFind=0;
			if (m_pHashTable == NULL)
				InitHashTable(m_nHashTableSize);

			// it doesn't exist, add a new Association
			pAssoc = NewAssoc();
			pAssoc->nHashValue = nHash;
			pAssoc->key = key;
			// 'pAssoc->value' is a constructed object, nothing more

			// put into hash table
			pAssoc->pNext = m_pHashTable[nHash];
			m_pHashTable[nHash] = pAssoc;
		}
		else
		{
			bFind=1;
		}
		return &pAssoc->value;  // return new reference
	}
};
typedef CArray <CString,CString> DefStringKey;
//Поддержка массивов
#define MAX_HASHTABLE	100000
class CValueArray : public CValue  
{
	DECLARE_DYNCREATE(CValueArray);
public:
	CValueArray();
	~CValueArray();
public:
	CArray <CValue,CValue&>	aValueArray;
	CStringToValue	*pStringValueList;
private:
	DefStringKey	*pStringKey;
	
	inline void CheckStrArray()
	{
		if(!pStringValueList)
			pStringValueList=new CStringToValue();
		if(!pStringKey)
			pStringKey=new DefStringKey();
	};

public:
	CValue& GetAt(int nKey);
	void SetAt(int nKey,CValue Val);
	int GetType(void)const;

	int GetSizeArray(void)const;
	void SetSizeArray(int nSize);
	int GetSizeStrArray(void)const;
	void SetSizeStrArray(int nSize);

	CValue& GetAt(CString Key);
	void SetAt(CString Key,CValue Val);

	void SaveToString(CString &Str);
	void LoadFromString(CString &Str);

	CString GetTypeString(void)const;
	CString GetString(void)const;
private:
	int LoadArray(CString &Str,int nIndex0,int nMode);//1-int,2-string array
	void CheckIndex(int nIndex);



	//Атрибут -> Строковый ключ
	//работа с массивом как с агрегатным объектом
public:
	static CMethods Methods;
	virtual void SetAttribute(int iName,CValue &Val);//установка атрибута
	virtual CValue GetAttribute(int iName);//значение атрибута
	virtual int  FindAttribute(CString csName);

	virtual CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	virtual CValue Method(int iName,CValue **aParams);//вызов метода


//Расширенные методы
	void Sort(CValue cvSort=CValue(),int nDirect=0);
	void SortId(CValue cvSort=CValue(),int nDirect=0);
	void Load(CValue &vSource,int nMode=0,int nClear=1);//nMode - количество вложенных загрузок массивов
	void RemoveAll();
	CValue Find(CValue &vFind,CValue vExtValue=CValue());
	CString GetIdentifierByNumber(int nIndex);
	int FindNumber(CString Key);

	BOOL FindId(CString Key);
	void RemoveKey(CString Key);

};

#endif // !defined(AFX_VALUEARRAY_H__69AB6707_0AEA_48E1_8B37_7BEA1DD1621A__INCLUDED_)
