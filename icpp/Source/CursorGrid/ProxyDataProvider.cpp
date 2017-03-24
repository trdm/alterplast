// DataProvider.cpp: implementation of the CProxyDataProvider class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProxyDataProvider.h"
#include "../V7Control.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CProxyDataRow
//////////////////////////////////////////////////////////////////////
char* ProxyMethodsNames[pmLast] =
{
	//DataRow
	"GetFieldCount",
	"GetFieldIndexOf",
	"GetFieldName",
	"GetFieldType",
	"FormatField",
	"GetRowIndex",
	"GetValue",
	//Provider
	"QueryRows",
	"Fetch",
	"RefreshRow",
	"CompareRows",
	"GetLastError",
	"GetRowCount",
	"QueryField",
	"ReleaseField",
	"GetQuickSearchType",
	"QuickSearch",
	"OnAttach",
	"OnDetach",
	"BuildRowByValue"
};

#ifdef _DEBUG_PURE
int iProxyDataRowCount = 0;
#endif

CProxyDataRow::CProxyDataRow(CValue &Val, CBLContext* pProv, int* ProxyMethNums)
{
#ifdef _DEBUG_PURE
	iProxyDataRowCount++;
#endif
	m_DataRowVal = Val;
	m_pProv = pProv;
	m_ProxyMethNums = ProxyMethNums;
}

CProxyDataRow::~CProxyDataRow()
{
#ifdef _DEBUG_PURE
	iProxyDataRowCount--;
#endif
}

//CDataRow
int CProxyDataRow::GetFieldCount()
{
	int n = m_ProxyMethNums[pmGetFieldCount];
	if(n>=0)
	{
		CValue ret(0L);
		m_pProv->CallAsFunc(n, ret, NULL);
		return ret.GetNumeric();
	}
    return 0;
}

int CProxyDataRow::GetFieldIndexOf(LPCSTR szFieldName)
{
	int n = m_ProxyMethNums[pmGetFieldIndexOf];
	if(n>=0)
	{
		CValue ret(-1);
		CValue valFieldName(szFieldName);
		CValue* pParams[] = {&m_DataRowVal, &valFieldName};
		m_pProv->CallAsFunc(n, ret, pParams);
		return ret.GetNumeric();
	}
    return -1;
}

LPCSTR CProxyDataRow::GetFieldName(int nIndex)
{
	int n = m_ProxyMethNums[pmGetFieldName];
	if(n>=0)
	{
		CValue ret("");
		CValue valFieldIndex(nIndex);
		CValue* pParams[] = {&m_DataRowVal, &valFieldIndex};
		m_pProv->CallAsFunc(n, ret, pParams);
		return ret.GetString();
	}
    return "";
}

DataType CProxyDataRow::GetFieldType(int nIndex)
{
	int n = m_ProxyMethNums[pmGetFieldType];
	if(n>=0)
	{
		CValue ret(0L);
		CValue valFieldIndex(nIndex);
		CValue* pParams[] = {&m_DataRowVal, &valFieldIndex};
		m_pProv->CallAsFunc(n, ret, pParams);
		return CProxyDataProvider::NumericToDataType(ret.GetNumeric());
	}
	return dtUndefined;
};

void CProxyDataRow::FormatField(int nIndex, CString& strValue)
{
	int n = m_ProxyMethNums[pmFormatField];
	if(n>=0)
	{
		CValue ret("");
		CValue valIndex(nIndex);
		CValue* pParams[] = {&m_DataRowVal, &valIndex};
		m_pProv->CallAsFunc(n, ret, pParams);
		strValue = ret.Format();
	}
	else
	{
		CValue ret;
		n = m_ProxyMethNums[pmGetValue];
		if(n>=0)
		{
			CValue valFieldIndex(nIndex);
			CValue valDataRow(m_DataRowVal);
			CValue* pParams[] = {&valDataRow, &valFieldIndex};
			m_pProv->CallAsFunc(n, ret, pParams);
		}
		FormatValue(ret,strValue);
	}

}

UINT CProxyDataRow::GetRowIndex()
{
	int n = m_ProxyMethNums[pmGetRowIndex];
	if(n>=0)
	{
		CValue ret(0L);
		CValue* pParams[] = {&m_DataRowVal};
		m_pProv->CallAsFunc(n, ret, pParams);
		return ret.GetNumeric();
	}
	return 0;
};


//CV7DataRow

const CValue& CProxyDataRow::GetValue(int nIndex) const
{
	m_val.Reset();
	//CValue ret;
	int n = m_ProxyMethNums[pmGetValue];
	if(n>=0)
	{
		CValue valFieldIndex(nIndex);
		CValue valDataRow(m_DataRowVal);
		CValue* pParams[] = {&valDataRow, &valFieldIndex};
		m_pProv->CallAsFunc(n, m_val, pParams);
	}
	return m_val;
}


//////////////////////////////////////////////////////////////////////
// CProxyDataProvider
//////////////////////////////////////////////////////////////////////
CProxyDataProvider::CProxyDataProvider()
{
#ifdef _DEBUG_PURE
	int iProxyDataRowCount = 0;
#endif
	UpdateProxeMethNums();
}

CProxyDataProvider::~CProxyDataProvider()
{
#ifdef _DEBUG_PURE
	Msg("ProxyDataRowCount: %d",mmNone,iProxyDataRowCount);
#endif
}

void CProxyDataProvider::UpdateProxeMethNums()
{
	for (int i=0; i<pmLast; i++)
		m_ProxyMethNums[i] = (m_pProv)?m_pProv->FindMethod(ProxyMethodsNames[i]):-1;
}

DataType CProxyDataProvider::NumericToDataType(const CNumeric &num)
{
	switch(num.operator long())
	{
	case 0:
		return dtUndefined;
	case 1:
		return dtText;
	case 2:
		return dtNumeric;
	case 3:
		return dtDate;
	case 4:
		return dtFalse;
	case 5:
		return dtTrue;
	default:
		return dtUndefined;
	}
}
long CProxyDataProvider::DataTypeToLong(DataType dt)
{
	switch(dt)
	{
	case dtUndefined:
		return 0;
	case dtText:
		return 1;
	case dtNumeric:
		return 2;
	case dtDate:
		return 3;
	case dtFalse:
		return 4;
	case dtTrue:
		return 5;
	default:
	    return 0;
	}
}

//BL_INIT_CONTEXT(CProxyDataProvider);
BEGIN_BL_METH_MAP(CProxyDataProvider)
	BL_METH_PROC("SetProvider", "УстПоставщика", 1, &SetProvider)
END_BL_METH_MAP()

BEGIN_EMPTY_BL_PROP_MAP(CProxyDataProvider)

IMPLEMENT_MY_CREATE_CONTEXT(CProxyDataProvider, "ProxyDataProvider", "ПроксиПоставщикДанных");

BOOL CProxyDataProvider::SetProvider(CValue** ppValues)
{
	m_pProv = ppValues[0]->GetContext();
	UpdateProxeMethNums();

	if (!m_pProv)
		RuntimeError("Плохой тип фактического параметра");

	int n = m_pProv->FindMethod("GetBaseClass");
	if (n > -1)
	{
		CValue ret;
		CValue BaseName("BaseDataProvider");
		CValue* pParams[] = {&BaseName};
		m_pProv->CallAsFunc(n, ret, pParams);
		CBLContext* pBaseCont = ret.GetContext();
		if (pBaseCont)
		{
			CBaseDataProvider* pBDP = static_cast<CBaseDataProvider*>(pBaseCont);
			pBDP->SetProxy(this);
		}
	}
	return TRUE;
}


//CDataProvider
void CProxyDataProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
	int n = m_ProxyMethNums[pmQueryRows];
	if(n>=0)
	{
		CValue DataRowVal;
		if (pRowFrom)
			DataRowVal = static_cast<CProxyDataRow*>(pRowFrom)->m_DataRowVal;
		CValue valRowsCount(nRowsCount);
		CValue valPageSizeHint(nPageSizeHint);
		CValue* pParams[] = {&DataRowVal, &valRowsCount, &valPageSizeHint};
		m_pProv->CallAsProc(n, pParams);
	}
}

CDataRow* CProxyDataProvider::Fetch()
{
	int n = m_ProxyMethNums[pmFetch];
	if(n>=0)
	{
		CValue ret;
		m_pProv->CallAsFunc(n,ret,NULL);
		if (ret.IsEmpty())
			return NULL;
		return new CProxyDataRow(ret, m_pProv, m_ProxyMethNums);
	}
    return NULL;
}

BOOL CProxyDataProvider::RefreshRow(CDataRow* pRowFrom)
{
	int n = m_ProxyMethNums[pmRefreshRow];
	if(n>=0)
	{
		CValue ret(0L);
		CValue DataRowVal;
		if (pRowFrom)
			DataRowVal = static_cast<CProxyDataRow*>(pRowFrom)->m_DataRowVal;
		CValue* pParams[] = {&DataRowVal};
		m_pProv->CallAsFunc(n, ret, pParams);
		return ret.GetNumeric();
	}
    return FALSE;
}

BOOL CProxyDataProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
	int n = m_ProxyMethNums[pmCompareRows];
	if(n>=0)
	{
		CValue ret(0L);
		CValue DataRowVal1;
		if (pRow1)
			DataRowVal1= static_cast<CProxyDataRow*>(pRow1)->m_DataRowVal;
		CValue DataRowVal2;
		if (pRow2)
			DataRowVal2= static_cast<CProxyDataRow*>(pRow2)->m_DataRowVal;

		CValue* pParams[] = {&DataRowVal1, &DataRowVal2};
		m_pProv->CallAsFunc(n, ret, pParams);
		return ret.GetNumeric();
	}
    return FALSE;
}

LPCSTR CProxyDataProvider::GetLastError()
{
	int n = m_ProxyMethNums[pmGetLastError];
	if(n>=0)
	{
		CValue ret("");
		m_pProv->CallAsFunc(n, ret, NULL);
		return ret.GetString();
	}
	return NULL;
}

BOOL CProxyDataProvider::GetRowCount(UINT* pCount)
{
	int n = m_ProxyMethNums[pmGetRowCount];
	if(n>=0)
	{
		CValue ret(0L);
		CValue valCount;
		CValue* pParams[] = {&valCount};
		if (m_pProv->CallAsFunc(n, ret, pParams))
			*pCount = valCount.GetNumeric();
		return ret.GetNumeric();
	}
    *pCount = 0;
    return FALSE;
}

void CProxyDataProvider::QueryField(LPCSTR szFieldName)
{
	int n = m_ProxyMethNums[pmQueryField];
	if(n>=0)
	{
		CValue valFieldName(szFieldName);
		CValue* pParams[] = {&valFieldName};
		m_pProv->CallAsProc(n, pParams);
	}
};

void CProxyDataProvider::ReleaseField(LPCSTR szFieldName)
{
	int n = m_ProxyMethNums[pmReleaseField];
	if(n>=0)
	{
		CValue valFieldName(szFieldName);
		CValue* pParams[] = {&valFieldName};
		m_pProv->CallAsProc(n, pParams);
	}
};

void CProxyDataProvider::OnFieldsChanged()
{
	CDataProvider::OnFieldsChanged();
};

DataType CProxyDataProvider::GetQuickSearchType(LPCSTR szFieldName)
{
	int n = m_ProxyMethNums[pmGetQuickSearchType];
	if(n>=0)
	{
		CValue ret(0L);
		CValue valFieldName(szFieldName);
		CValue* pParams[] = {&valFieldName};
		m_pProv->CallAsFunc(n, ret, pParams);
		return NumericToDataType(ret.GetNumeric());
	}
	return dtUndefined;
}

void CProxyDataProvider::QuickSearch(stQuickSearchPattern& QSPattern)
{
	int n = m_ProxyMethNums[pmQuickSearch];
	if(n>=0)
	{
		CValue valFieldName(QSPattern.szFieldName);
		CValue valDataType(DataTypeToLong(QSPattern.dataType));
		CValue valTextData(*QSPattern.pTextData);
		CValue* pParams[] = {&valFieldName, &valDataType, &valTextData};
		if (m_pProv->CallAsProc(n, pParams))
			*QSPattern.pTextData = valTextData.GetString();
	}
}

void CProxyDataProvider::OnAttach()
{
	ResetData();

	int n = m_ProxyMethNums[pmOnAttach];
	if(n>=0)
		m_pProv->CallAsProc(n, NULL);
}

void CProxyDataProvider::OnDetach()
{
	int n = m_ProxyMethNums[pmOnDetach];
	if(n>=0)
		m_pProv->CallAsProc(n, NULL);

};



//CV7DataProvider
CV7DataRow* CProxyDataProvider::GetV7DataRow(CDataRow* pDataRow)  const
{
    return static_cast<CProxyDataRow*>(pDataRow);
}

void CProxyDataProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
	if (pDataRow)
		value = static_cast<CProxyDataRow*>(pDataRow)->m_DataRowVal;
}

CDataRow* CProxyDataProvider::BuildRowByValue(const CValue& value)
{
	int n = m_ProxyMethNums[pmBuildRowByValue];
	if(n>=0)
	{
		CValue ret;
		CValue valVal(value);
		CValue* pParams[] = {&valVal};
		m_pProv->CallAsFunc(n, ret, pParams);
		if (ret.IsEmpty())
			return NULL;
		return new CProxyDataRow(ret, m_pProv, m_ProxyMethNums);
	}
    return NULL;
}

void CProxyDataProvider::InvokeSetCurrentRow(CValue& val)
{
	CProxyDataRow* pRow = new CProxyDataRow(val, m_pProv, m_ProxyMethNums);
	SetCurrentRow(pRow);
	delete pRow;
}

void CProxyDataProvider::InvokeResetData(CValue& val)
{
	CProxyDataRow* pRow = new CProxyDataRow(val, m_pProv, m_ProxyMethNums);
	ResetData(pRow);
	delete pRow;
}


//////////////////////////////////////////////////////////////////////
// CBaseDataProvider
//////////////////////////////////////////////////////////////////////

CBaseDataProvider::CBaseDataProvider():m_pProxy(NULL)
{

}

CBaseDataProvider::~CBaseDataProvider()
{

}

BEGIN_BL_METH_MAP(CBaseDataProvider)
    BL_METH("GetControl", "GetControl", 0, NULL, &MethGetControl, NULL)
	BL_METH("SetControl", "SetControl", 1, &MethSetControl, NULL, NULL)
	BL_METH("ResetData", "ResetData", 1, &MethResetData, NULL, NULL)
	BL_METH("SetCurrentRow", "SetCurrentRow", 1, &MethSetCurrentRow, NULL, NULL)
	BL_METH("OnFieldsChanged", "OnFieldsChanged", 1, &MethOnFieldsChanged, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CBaseDataProvider)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CBaseDataProvider, "BaseDataProvider", "BaseDataProvider", 1, NULL, NULL, -1);

BOOL CBaseDataProvider::MethGetControl(CValue& Value,CValue** ppValue)
{
	Value.Reset();
	
	if (m_pProxy)
	{
		if (m_pProxy->GetControl())
		{
			CWnd* pWnd = CWnd::FromHandle(m_pProxy->GetControl());
			IV7ControlContext* pCtx = dynamic_cast<IV7ControlContext*>(pWnd);
			CBLContext* pCont = pCtx ? pCtx->GetControlContext() : NULL;
			pCont ? Value.AssignContext(pCont): Value.Reset();
		}
	}
	
	return TRUE;
}

BOOL CBaseDataProvider::MethSetControl(CValue** ppValue)
{
	return TRUE;
}

BOOL CBaseDataProvider::MethResetData(CValue** ppValues)
{
	m_pProxy->InvokeResetData(*ppValues[0]);
	return TRUE;
}

BOOL CBaseDataProvider::MethSetCurrentRow(CValue** ppValues)
{
	m_pProxy->InvokeSetCurrentRow(*ppValues[0]);
	return TRUE;
}

BOOL CBaseDataProvider::MethOnFieldsChanged(CValue** ppValues)
{
	m_pProxy->OnFieldsChanged();
	return TRUE;
}
