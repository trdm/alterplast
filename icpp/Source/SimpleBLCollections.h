#ifndef SIMPLE_BL_COLLECTIONS_H_INCLUDED
#define SIMPLE_BL_COLLECTIONS_H_INCLUDED

#include "SimpleVector.h"
#include "AssocVector.h"
#include "VTExt/CVCompare.h"

//////////////////////////////////////////////////////////////////////////
// CBLSimpleVector
//////////////////////////////////////////////////////////////////////////

typedef CSC_CMN::CValuePtr CValuePtr;

typedef CSimpleCollection<
	CSimpleVector<CSC_CMN::CValuePtr, CValuePtr const&>,
	scf_Modify | scf_Insert | scf_Move> BASE_SV;

class CBLSimpleVector : public BASE_SV
{
	SIMPLE_DECLARE(CBLSimpleVector, BASE_SV)

public:
	I_PDEF(defsFALSE);
	I_PDEF(defsSort);

	I_PROC(procReserve);
	I_PROC(procResize);
	I_PROC(procSort);
};

//////////////////////////////////////////////////////////////////////////
// CBLAssocVector
//////////////////////////////////////////////////////////////////////////

typedef CAssocCollection<CAssocVector<LPCSTR, LPCSTR, CValue, CValue const&>,
scf_Modify | scf_Insert | scf_Move> BASE_AV;

class CBLAssocVector : public BASE_AV
{
	SIMPLE_DECLARE(CBLAssocVector, BASE_AV)

public:
	I_PDEF(defsFALSE);

	I_PROC(procReserve);
	I_PROC(procResize);
	I_PROC(procSort);

public:
	// CBLContext interface
	int GetNProps() const;
	int FindProp(LPCSTR szName) const;
	LPCSTR GetPropName(int nIndex, int nLanguageIdx) const;
	BOOL GetPropVal(int nIndex, CValue& RetValue) const;
	BOOL SetPropVal(int nIndex, CValue const& Value);
	BOOL IsPropReadable(int nIndex) const;
	int	IsPropWritable(int nIndex) const;
};

//////////////////////////////////////////////////////////////////////////
// CInitPtr
//////////////////////////////////////////////////////////////////////////

template<typename T>
class CInitPtr
{
public:
	void operator ()(T& ptr) { ptr.reset(new T::value_type); }
};

#endif

//////////////////////////////////////////////////////////////////////////
// CSimpleComparator
//////////////////////////////////////////////////////////////////////////

template <typename ITEMREF, typename VALREF>
class CSimpleComparator
{
};

template <>
class CSimpleComparator<CValuePtr const&, CValue const&>
{
public:
	CSimpleComparator(LPCSTR szOrder);
	BOOL operator()(CValuePtr const& lhs, CValuePtr const& rhs);
private:
	TVTIndexDescrRecord m_rec;
};

//////////////////////////////////////////////////////////////////////////
// CAssocComparator
//////////////////////////////////////////////////////////////////////////

template <typename ITEMREF, typename KEYREF, typename VALREF>
class CAssocComparator
{
};

template <typename ITEMREF>
class CAssocComparator<ITEMREF, LPCSTR, CValue const&>
{
public:
	enum ColumnNumbers { cnKey, cnValue };
	CAssocComparator(LPCSTR szOrder);
	LPCSTR GetLastError();
	BOOL operator()(ITEMREF lhs, ITEMREF rhs);
private:
	typedef std::vector<TVTIndexDescrRecord> V_REC;
	V_REC m_VRec;
};
