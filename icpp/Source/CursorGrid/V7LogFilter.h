// V7LogFilter.h: interface for the CV7LogFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_V7LOGFILTER_H__F502C2D7_65FE_4D7B_840B_0629E9B95F34__INCLUDED_)
#define AFX_V7LOGFILTER_H__F502C2D7_65FE_4D7B_840B_0629E9B95F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "V7LogEvent.h"
#include "..\mycontextbase.h"


class CV7LogProvider;


class CV7FilterItem : public CMyContextBase, public CCollectionItem
{
    DECLARE_DYNCREATE(CV7FilterItem);
public:
	CV7FilterItem();
	virtual ~CV7FilterItem();

	BOOL GetPropType(CValue& Value) const;
	BOOL GetPropValue(CValue& Value) const;
	BOOL GetPropName(CValue& Value) const;

	BOOL SetTypeAndValue(CValue& Result, CValue** ppValue);
	BOOL DefaultSetTypeAndValue(int nParam, CValue* param) const;

	DECLARE_MY_CONTEXT();
private:
	unsigned char m_nType;
	CValue m_Value;
};


class CLogFilterItemsCollection : public CCollection
{
public:
	CLogFilterItemsCollection();
	~CLogFilterItemsCollection();

    int GetSize() const;
    CV7FilterItem* Add(LPCSTR szName);
    CV7FilterItem* Insert(int nIndex, LPCSTR szName);
	CV7FilterItem* GetAt(int nIndex) const;
    CV7FilterItem* GetAt(LPCSTR szName) const;
    int IndexOf(LPCSTR szName) const;
    int IndexOf(CV7FilterItem* pLogEvent) const;
    BOOL Remove(int nIndex);
    BOOL Move(int nIndex, int nOffset);
    void RemoveAll();
protected:
    virtual void OnRemove(CCollectionItem* pItem, int nIndexWas);
};


class CV7LogFilter : public CBLCollection<CLogFilterItemsCollection, CV7FilterItem, RUNTIME_CLASS(CV7FilterItem), BLC_MODIFED>
{
	friend class CV7LogProvider;
	DECLARE_DYNAMIC(CV7LogFilter);
public:
	CV7LogFilter():m_pLogProvider(NULL)
	{
		if (m_ParamDefs.GetIndexByName(m_Set[0][1]) == -1)
		{
			m_nMethodSet=m_ParamDefs.Size();
			m_ParamDefs.AddParam(m_Set[0][0],m_Set[0][1],m_nMethodSet,1,1);
		}
	}
	virtual int CallAsFunc(int nMethIndex, CValue& RetValue, CValue** ppValue)
	{
		if (nMethIndex == m_nMethodSet)
		{
			RetValue=CNumeric(0);
			if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
			{
				switch (ppValue[0]->GetNumeric())
				{
				case 0:
					RetValue=CNumeric(SetFilterEx(false));
					break;
				case 1:
					RetValue=CNumeric(SetFilterEx(true));
					break;
				}
			}
		}
		else
			CBLCollection<CLogFilterItemsCollection,CV7FilterItem,RUNTIME_CLASS(CV7FilterItem),BLC_MODIFED>::CallAsFunc(nMethIndex,RetValue,ppValue);
		return 1;
	}

	virtual void IncrRef();
	virtual void DecrRef();
	void SetProvider(CV7LogProvider* m_pLogProvider);

	virtual LPCSTR GetTypeString(void) const
	{
		if (GetMetaData()->GetTaskDef()->GetDefaultLanguage() == 0)
			return _T("Filter");
		else
			return _T("Отбор");
	}

	void Reset(const bool& bClearCollection);
	bool SetFilter(const bool& bUseBuffer);
	inline bool UseBuffer() const { return m_dwFilter&(1<<(LOG_DP_FILTER_USING_BUFFER-1)) ? true : false; }
	void CV7LogFilter::ChangeFilter(
		const unsigned char nFilterType,
		const int& nIndex,
		CString& csKey,
		const bool& bAddFilter);

private:
	CV7LogProvider* m_pLogProvider;

	void GetModeAndCategory(const CIStringMapToIndex* const pMap, const bool bMode, CString& csCheck) const;

	void AddFilterItem(unsigned char& nFilterType, int& nCount, CValue& Value);

	bool SetFilterEx(const bool& bUseBuffer);

	bool CheckFilter();

	// для фильта
	DWORD m_dwFilter;
	BOOL m_bInvalidFilter;

	CString m_strMode;
	CString m_strCategory;
	CString m_strComment;
	CString m_strPresent;

	CIStringMapToIndex m_mapEventType;
	CIStringMapToIndex m_mapEvent;
	CIStringMapToIndex m_mapObject;
	CIStringMapToIndex m_mapObjectKind;
	CMapStringToOb m_mapUser;

	static int m_nMethodSet;
	const static TCHAR* m_Set[][2];
};


// --------------------------------------------------------------------------------------------------------------
CString GetStringFromTmpl(CString csFindIn, CString& csParam, const CString& csTemplate, const bool& bAddFilter);
BOOL GetStringPresent(const CValue* const val, const BOOL bFull, CString& csResult);


#endif // !defined(AFX_V7LOGFILTER_H__F502C2D7_65FE_4D7B_840B_0629E9B95F34__INCLUDED_)
