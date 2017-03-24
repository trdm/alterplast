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
}
