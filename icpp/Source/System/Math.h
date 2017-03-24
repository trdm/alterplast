//////////////////////////////////////////////////////////////////////
//	Math.h
//	Interface for CMath class
//	Refactoring of rainbow project class
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../mycontextbase.h"

class CMath : public CMyContextBase
{
public:
	DECLARE_DYNCREATE(CMath);

	BOOL funcLong(CValue& RetVal, CValue **params);
	int	 defsLong(int nParam, CValue* param)const;
	BOOL funcDouble(CValue& RetVal, CValue **params);

	BOOL funcABS(CValue& RetVal, CValue **params);
	BOOL funcACOS(CValue& RetVal, CValue **params);
	BOOL funcASIN(CValue& RetVal, CValue **params);
	BOOL funcATAN(CValue& RetVal, CValue **params);
	BOOL funcATAN2(CValue& RetVal, CValue **params);
	BOOL funcCEIL(CValue& RetVal, CValue **params);
	BOOL funcCOPYSIGN(CValue& RetVal, CValue **params);
	BOOL funcCOS(CValue& RetVal, CValue **params);
	BOOL funcCOSH(CValue& RetVal, CValue **params);
	BOOL funcEXP(CValue& RetVal, CValue **params);
	BOOL funcFABS(CValue& RetVal, CValue **params);
	BOOL funcFLOOR(CValue& RetVal, CValue **params);
	BOOL funcFMOD(CValue& RetVal, CValue **params);
	BOOL funcFREXP(CValue& RetVal, CValue **params);
	BOOL funcHYPOT(CValue& RetVal, CValue **params);
	BOOL funcLABS(CValue& RetVal, CValue **params);
	BOOL funcLDEXP(CValue& RetVal, CValue **params);
	BOOL funcLDIV(CValue& RetVal, CValue **params);
	BOOL funcLOG(CValue& RetVal, CValue **params);
	BOOL funcLOG10(CValue& RetVal, CValue **params);
	BOOL funcMODF(CValue& RetVal, CValue **params);
	BOOL funcPI(CValue& RetVal, CValue **params);
	BOOL funcPOW(CValue& RetVal, CValue **params);
	BOOL funcRAND(CValue& RetVal, CValue **params);
	BOOL funcRAND1(CValue& RetVal, CValue **params);
	BOOL funcROTL(CValue& RetVal, CValue **params);
	BOOL funcROTR(CValue& RetVal, CValue **params);
	BOOL funcSCALB(CValue& RetVal, CValue **params);
	BOOL funcSHL(CValue& RetVal, CValue **params);
	BOOL funcSHR(CValue& RetVal, CValue **params);
	BOOL funcSIN(CValue& RetVal, CValue **params);
	BOOL funcSINH(CValue& RetVal, CValue **params);
	BOOL funcSQRT(CValue& RetVal, CValue **params);
	BOOL procSRAND(CValue **params);
	BOOL funcTAN(CValue& RetVal, CValue **params);
	BOOL funcTANH(CValue& RetVal, CValue **params);

	BOOL funcBITAND(CValue& RetVal, CValue **params);
	BOOL funcBITOR(CValue& RetVal, CValue **params);
	BOOL funcCOMPL(CValue& RetVal, CValue **params);
	BOOL funcXOR(CValue& RetVal, CValue **params);

	//BOOL defs(int nParam, CValue* param) const;
	
	DECLARE_MY_CONTEXT();
};
