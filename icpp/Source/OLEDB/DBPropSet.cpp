// DBPropSet.cpp: implementation of the CDBPropSet class
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "DBPropSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________
//
CDBPropSet::CDBPropSet()
{
	rgProperties    = NULL;
	cProperties     = 0;
}

//_____________________________________________________________________________
//
CDBPropSet::CDBPropSet(const GUID& guid)
{
	rgProperties    = NULL;
	cProperties     = 0;
	guidPropertySet = guid;
}

//_____________________________________________________________________________
//
CDBPropSet::CDBPropSet(const CDBPropSet& propset)
{
	InternalCopy(propset);
}

//_____________________________________________________________________________
//
CDBPropSet::~CDBPropSet()
{
	for (ULONG i = 0; i < cProperties; i++)
		VariantClear(&rgProperties[i].vValue);
	
	CoTaskMemFree(rgProperties);
}

//_____________________________________________________________________________
//
CDBPropSet& CDBPropSet::operator=(CDBPropSet& propset)
{
	this->~CDBPropSet();
	InternalCopy(propset);
	
	return *this;
}

//_____________________________________________________________________________
//
void CDBPropSet::SetGUID(const GUID& guid)
{
	guidPropertySet = guid;
}

//_____________________________________________________________________________
//
void CDBPropSet::InternalCopy(const CDBPropSet& propset)
{
	cProperties     = propset.cProperties;
	guidPropertySet = propset.guidPropertySet;
	rgProperties    = (DBPROP*)CoTaskMemAlloc(cProperties * sizeof(DBPROP));

	if (rgProperties != NULL)
	{
		for (ULONG i = 0; i < cProperties; i++)
		{
			rgProperties[i].dwPropertyID = propset.rgProperties[i].dwPropertyID;
			rgProperties[i].dwOptions    = DBPROPOPTIONS_REQUIRED;
			rgProperties[i].colid        = DB_NULLID;
			rgProperties[i].vValue.vt    = VT_EMPTY;
			VariantCopy(&rgProperties[i].vValue, &propset.rgProperties[i].vValue);
		}
	}
	else
	{
		cProperties = 0;
	}
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, LPCSTR szValue)
{
	USES_CONVERSION;
	
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID   = dwPropertyID;
	rgProperties[cProperties].vValue.vt      = VT_BSTR;
	rgProperties[cProperties].vValue.bstrVal = SysAllocString(A2COLE(szValue));
	if (rgProperties[cProperties].vValue.bstrVal == NULL)
		return false;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, bool bValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID   = dwPropertyID;
	rgProperties[cProperties].vValue.vt      = VT_BOOL;
#pragma warning(disable: 4310)
	rgProperties[cProperties].vValue.boolVal = (bValue) ? VARIANT_TRUE : VARIANT_FALSE;
#pragma warning(default: 4310)
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, BYTE bValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_UI1;
	rgProperties[cProperties].vValue.bVal   = bValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, short nValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_I2;
	rgProperties[cProperties].vValue.iVal   = nValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, long nValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_I4;
	rgProperties[cProperties].vValue.lVal   = nValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, float fltValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_R4;
	rgProperties[cProperties].vValue.fltVal = fltValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, double dblValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_R8;
	rgProperties[cProperties].vValue.dblVal = dblValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, CY cyValue)
{
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID  = dwPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_CY;
	rgProperties[cProperties].vValue.cyVal  = cyValue;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::AddProperty(DWORD dwPropertyID, LPCWSTR szValue)
{
	USES_CONVERSION;
	
	if (!Add())
		return false;
	
	rgProperties[cProperties].dwPropertyID   = dwPropertyID;
	rgProperties[cProperties].vValue.vt      = VT_BSTR;
	rgProperties[cProperties].vValue.bstrVal = SysAllocString(W2COLE(szValue));
	
	if (rgProperties[cProperties].vValue.bstrVal == NULL)
		return false;
	cProperties++;
	
	return true;
}

//_____________________________________________________________________________
//
bool CDBPropSet::Add()
{
	rgProperties = (DBPROP*)CoTaskMemRealloc(rgProperties, (cProperties + 1) * sizeof(DBPROP));
	if (rgProperties != NULL)
	{
		rgProperties[cProperties].dwOptions = DBPROPOPTIONS_REQUIRED;
		rgProperties[cProperties].colid     = DB_NULLID;
		rgProperties[cProperties].vValue.vt = VT_EMPTY;
		
		return true;
	}
	else
		return false;
}