#include "StdAfx.h"
#include "SimpleBLCollections.h"
#include "StringArrayEx.h"

//////////////////////////////////////////////////////////////////////////
// CBLSimpleVector
//////////////////////////////////////////////////////////////////////////

SIMPLE_IMPLEMENT(CBLSimpleVector, CSimpleContext, "Vector", "Вектор")

void CBLSimpleVector::InitDef()
{
	AddDef("Reserve",	"Резервировать",	1, &CBLSimpleVector::procReserve,	&CBLSimpleVector::defsFALSE);
	AddDef("Resize",	"ИзменитьРазмер",	1, &CBLSimpleVector::procResize,	&CBLSimpleVector::defsFALSE);
	AddDef("Sort",		"Сортировать",		1, &CBLSimpleVector::procSort,		&CBLSimpleVector::defsSort);
}

BOOL CBLSimpleVector::defsFALSE(int nIndex, CValue* pValue) const
{
	return FALSE;
}

BOOL CBLSimpleVector::procReserve(CValue** ppValues)
{
	reserve(ppValues[0]->GetNumeric());
	return TRUE;
}

BOOL CBLSimpleVector::procResize(CValue** ppValues)
{
	size_t nNewSize = ppValues[0]->GetNumeric();
	size_t nCurSize = size();
	resize(nNewSize);
	if (nNewSize > nCurSize)
	{
		iterator End = end();
		std::for_each(begin() + nCurSize, End, CInitPtr<ITEM>());
	}
	return TRUE;
}

BOOL CBLSimpleVector::defsSort(int nIndex, CValue* pValue) const
{
	*pValue = "";
	return TRUE;
}

BOOL CBLSimpleVector::procSort(CValue** ppValues)
{
	CSimpleComparator<ITEMREF, CValue const&> comp(ppValues[0]->GetString());
	std::sort(begin(), end(), comp);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CBLAssocVector
//////////////////////////////////////////////////////////////////////////

SIMPLE_IMPLEMENT(CBLAssocVector, CSimpleContext, "AssociativeVector", "АссоциативныйВектор")

void CBLAssocVector::InitDef()
{
	AddDef("Reserve",	"Резервировать",	1, &CBLAssocVector::procReserve,	&CBLAssocVector::defsFALSE);
	AddDef("Resize",	"ИзменитьРазмер",	1, &CBLAssocVector::procResize,		&CBLAssocVector::defsFALSE);
	AddDef("Sort",		"Сортировать",		1, &CBLAssocVector::procSort,		&CBLAssocVector::defsFALSE);
}

BOOL CBLAssocVector::defsFALSE(int nIndex, CValue* pValue) const
{
	return FALSE;
}

BOOL CBLAssocVector::procReserve(CValue** ppValues)
{
	reserve(ppValues[0]->GetNumeric());
	return TRUE;
}

BOOL CBLAssocVector::procResize(CValue** ppValues)
{
	size_t nNewSize = ppValues[0]->GetNumeric();
	size_t nCurSize = size();
	resize(nNewSize);
	if (nNewSize > nCurSize)
	{
		iterator iEnd = end();
		std::for_each(begin() + nCurSize, iEnd, CInitPtr<ITEM>());
	}
	return TRUE;
}

// CBLContext interface
int CBLAssocVector::GetNProps() const
{
	return GetAssocSize();
}

int CBLAssocVector::FindProp(LPCSTR szName) const
{
	//	return GetIndexOf(szName, TRUE);
	// 	linear to const complexity hack
	ASSOC* pAssoc = GetAssocAt(szName, TRUE);
	return pAssoc ? reinterpret_cast<int>(pAssoc) : -1;
}

LPCSTR CBLAssocVector::GetPropName(int nIndex, int nLanguageIdx) const
{
	return GetAssocAt(nIndex, TRUE)->GetKey();
}

BOOL CBLAssocVector::GetPropVal(int nIndex, CValue& RetValue) const
{
	//	return f_GetAt(nIndex, RetValue);
	// 	linear to const complexity hack
	CSC_CMN::AssignItem(RetValue, reinterpret_cast<ASSOC*>(nIndex)->GetVal());
	return TRUE;
}

BOOL CBLAssocVector::SetPropVal(int nIndex, CValue const& Value)
{
	// 	return f_SetAt(nIndex, Value);
	// 	linear to const complexity hack
	reinterpret_cast<ASSOC*>(nIndex)->SetVal(CSC_CMN::GetItem<VAL>(Value));
	return TRUE;
}

BOOL CBLAssocVector::IsPropReadable(int nIndex) const
{
	return TRUE;
}

int	CBLAssocVector::IsPropWritable(int nIndex) const
{
	return TRUE;
}

BOOL CBLAssocVector::procSort(CValue** ppValues)
{
	ValidateParamString(*ppValues[0], "Неверный тип выражения сортировки (ожидается строка)!");
	CAssocComparator<ITEMREF, KEYREF, VALREF> comp(ppValues[0]->GetString());
	LPCSTR szErr = comp.GetLastError();
	if (szErr)
		RuntimeError(szErr);
	std::sort(begin(), end(), comp);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CSimpleComparator
//////////////////////////////////////////////////////////////////////////

CSimpleComparator<CValuePtr const&, CValue const&>::
CSimpleComparator(LPCSTR szDescr)
{
	m_rec.ParseMod(szDescr);
}

BOOL CSimpleComparator<CValuePtr const&, CValue const&>::
operator()(CValuePtr const& lhs, CValuePtr const& rhs)
{
	int nRes = CVCompare::CValue_compare(*lhs, *rhs, m_rec);
	if (nRes)
		return m_rec.Direction > 0 ? nRes < 0 : nRes > 0;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// CAssocComparator
//////////////////////////////////////////////////////////////////////////

template <typename ITEMREF>
CAssocComparator<ITEMREF, LPCSTR, CValue const&>::
CAssocComparator(LPCSTR szDescr)
{
	CStringArrayEx csArr(szDescr);
	TVTIndexDescrRecord	rec;
	for (int i = 0; i < csArr.GetSize(); ++i)
	{
		CString& csDescr = csArr[i];
		rec.ParseMod(csDescr);
		rec.TrimMod(csDescr);
		if (!FastCompareNoCase(csDescr, "Ключ") || !FastCompareNoCase(csDescr, "Key"))
			rec.ColumnNumber = cnKey;
		else if (FastCompareNoCase(csDescr, "Значение") || FastCompareNoCase(csDescr, "Value"))
			rec.ColumnNumber = cnValue;
		else
			continue;
		m_VRec.push_back(rec);
	}
}

template <typename ITEMREF>
LPCSTR CAssocComparator<ITEMREF, LPCSTR, CValue const&>::
GetLastError()
{
	LPCSTR szErr = NULL;
	if (!m_VRec.size())
		szErr = "Не заданы параметры сортировки!";
	return szErr;
}

template <typename ITEMREF>
BOOL CAssocComparator<ITEMREF, LPCSTR, CValue const&>::
operator()(ITEMREF lhs, ITEMREF rhs)
{
	V_REC::iterator iEnd = m_VRec.end();
	for (V_REC::iterator i = m_VRec.begin(); i != iEnd; ++i)
	{
		int nRes;
		switch(i->ColumnNumber)
		{
		case cnKey:
			nRes = CVCompare::safe_string_compare(lhs->GetKey(), rhs->GetKey(), *i);
			break;
		case cnValue:
			nRes = CVCompare::CValue_compare(lhs->GetVal(), rhs->GetVal(), *i);
			break;
		}
		if (nRes)
			return i->Direction > 0 ? nRes < 0 : nRes > 0;
	}
	return FALSE;
}
