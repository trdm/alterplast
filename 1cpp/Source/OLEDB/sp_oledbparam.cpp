// sp_oledbparam.cpp: implementation of the COLEDBParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sp_oledbparam.h"

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
COLEDBParam::COLEDBParam()
{
	m_Ordinal	 = 0;
	m_ParamIO	 = 0;
	m_Type		 = 0;
	m_Length	 = 0;
	m_Precision	 = 0;
	m_Value.Reset();

	//pBkEndUI->DoMessageLine("COLEDBParam::COLEDBParam()", mmNone);
}

//_____________________________________________________________________________
//
COLEDBParam::COLEDBParam(const COLEDBParam& Param)
{
	m_Ordinal	 = Param.m_Ordinal;
	m_ParamIO	 = Param.m_ParamIO;
	m_Type		 = Param.m_Type;
	m_Length	 = Param.m_Length;
	m_Precision	 = Param.m_Precision;
	m_Value		 = Param.m_Value;

	//pBkEndUI->DoMessageLine("COLEDBParam::COLEDBParam()", mmNone);
}

//_____________________________________________________________________________
//
COLEDBParam::~COLEDBParam()
{
	m_Value.Reset();

	//pBkEndUI->DoMessageLine("COLEDBParam::~COLEDBParam()", mmNone);
}

//_____________________________________________________________________________
//
COLEDBParam::operator COLEDBParam*() const
{
	return (COLEDBParam*)this;
}

//_____________________________________________________________________________
//
COLEDBParam& COLEDBParam::operator=(const COLEDBParam& Param)
{
	if (this != &Param)
	{
		m_Ordinal	 = Param.m_Ordinal;
		m_ParamIO	 = Param.m_ParamIO;
		m_Type		 = Param.m_Type;
		m_Length	 = Param.m_Length;
		m_Precision	 = Param.m_Precision;
		m_Value		 = Param.m_Value;
	}
	
	return *this;
}
