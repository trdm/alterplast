#include "stdafx.h"
#include "Math.h"

#include <math.h>
#include <float.h>
//_____________________________________________________________________________
//
// CMath Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CMath) 
	BL_METH_FUNC_DEF_PARAM("Long", "„исло",			2,	&funcLong, &defsLong)
    BL_METH("Double",			"Double",			1,	NULL,		&funcDouble,		NULL)

    BL_METH("ABS",				"ABS",				1,	NULL,		&funcABS,		NULL)
    BL_METH("ACOS",				"ACOS",				1,	NULL,		&funcACOS,		NULL)
    BL_METH("ASIN",				"ASIN",				1,	NULL,		&funcASIN,		NULL)
    BL_METH("ATAN",				"ATAN",				1,	NULL,		&funcATAN,		NULL)
    BL_METH("ATAN2",			"ATAN2",			2,	NULL,		&funcATAN2,		NULL)
    BL_METH("CEIL",				"CEIL",				1,	NULL,		&funcCEIL,		NULL)
    BL_METH("COPYSIGN",			"COPYSIGN",			2,	NULL,		&funcCOPYSIGN,	NULL)
    BL_METH("COS",				"COS",				1,	NULL,		&funcCOS,		NULL)
    BL_METH("COSH",				"COSH",				1,	NULL,		&funcCOSH,		NULL)
    BL_METH("EXP",				"EXP",				1,	NULL,		&funcEXP,		NULL)
//    BL_METH("FABS",				"FABS",				1,	NULL,		funcFABS,		NULL)
    BL_METH("FLOOR",			"FLOOR",			1,	NULL,		&funcFLOOR,		NULL)
    BL_METH("FMOD",				"FMOD",				2,	NULL,		&funcFMOD,		NULL)
    BL_METH("FREXP",			"FREXP",			2,	NULL,		&funcFREXP,		NULL)
    BL_METH("HYPOT",			"HYPOT",			2,	NULL,		&funcHYPOT,		NULL)
//    BL_METH("LABS",				"LABS",				1,	NULL,		funcLABS,		NULL)
    BL_METH("LDEXP",			"LDEXP",			2,	NULL,		&funcLDEXP,		NULL)
    BL_METH("LDIV",				"LDIV",				3,	NULL,		&funcLDIV,		NULL)
    BL_METH("LOG",				"LOG",				1,	NULL,		&funcLOG,		NULL)
    BL_METH("LOG10",			"LOG10",			1,	NULL,		&funcLOG10,		NULL)
    BL_METH("MODF",				"MODF",				2,	NULL,		&funcMODF,		NULL)
    BL_METH("PI",				"PI",				0,	NULL,		&funcPI,			NULL)
    BL_METH("POW",				"POW",				2,	NULL,		&funcPOW,		NULL)
    BL_METH("RAND",				"RAND",				0,	NULL,		&funcRAND,		NULL)
    BL_METH("RAND1",			"RAND1",			0,	NULL,		&funcRAND1,		NULL)
    BL_METH("ROTL",				"ROTL",				2,	NULL,		&funcROTL,		NULL)
    BL_METH("ROTR",				"ROTR",				2,	NULL,		&funcROTR,		NULL)
//    BL_METH("SCALB",			"SCALB",			2,	NULL,		funcSCALB,		NULL)
    BL_METH("SHL",				"SHL",				2,	NULL,		&funcSHL,		NULL)
    BL_METH("SHR",				"SHR",				2,	NULL,		&funcSHR,		NULL)
    BL_METH("SIN",				"SIN",				1,	NULL,		&funcSIN,		NULL)
    BL_METH("SINH",				"SINH",				1,	NULL,		&funcSINH,		NULL)
    BL_METH("SQRT",				"SQRT",				1,	NULL,		&funcSQRT,		NULL)
    BL_METH("SRAND",			"SRAND",			1,	&procSRAND,	NULL,			NULL)
    BL_METH("TAN",				"TAN",				1,	NULL,		&funcTAN,		NULL)
    BL_METH("TANH",				"TANH",				1,	NULL,		&funcTANH,		NULL)

    BL_METH("BITAND",			"BITAND",			2,	NULL,		&funcBITAND,		NULL)
    BL_METH("BITOR",			"BITOR",			2,	NULL,		&funcBITOR,		NULL)
    BL_METH("COMPL",			"COMPL",			1,	NULL,		&funcCOMPL,		NULL)
    BL_METH("XOR",				"XOR",				2,	NULL,		&funcXOR,		NULL)
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CMath)
//    BL_PROP("propEng", "propRus", propGet, propSet)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CMath, "Math", "ћатематика", 1, NULL, NULL, -1);
//______________________________________________________________________________
//
long getlong(CValue *cv, int radix = 0)
{
	int nType = cv->GetTypeCode();
	long retval;
	LPCSTR buf;
	LPSTR endptr;

	switch(nType)
	{
	case NUMBER_TYPE_1C:
		retval = cv->GetNumeric().operator long();
		break;

	case STRING_TYPE_1C:
		buf = cv->GetString().operator LPCSTR();
		retval = strtol(buf, &endptr, radix);
		break;

	default:
		retval = 0;
	}

	return retval;
}
//______________________________________________________________________________
//
double getdouble(CValue *cv)
{
	int nType = cv->GetTypeCode();
	double retval;
	LPCSTR buf;
	LPSTR endptr;

	switch(nType)
	{
	case NUMBER_TYPE_1C:
		retval = cv->GetNumeric().GetDouble();
		break;

	case STRING_TYPE_1C:
		buf = cv->GetString().operator LPCSTR();
		retval = strtod(buf, &endptr);
		break;

	default:
		retval = 0;
	}

	return retval;
}
//______________________________________________________________________________
//
// параметры: 1: число или строка 2: база дл€ строки (число)
//______________________________________________________________________________
//
int	CMath::defsLong(int nParam, CValue* param)const
{
	if (1 == nParam){
		*param = long(0);
		return TRUE;
	}
	return FALSE;
}

BOOL CMath::funcLong(CValue& RetVal, CValue **params)
{
	RetVal = getlong(params[0], params[1]->GetNumeric());
	return TRUE;
}
//______________________________________________________________________________
//
// параметры: 1: число или строка
//______________________________________________________________________________
//
BOOL CMath::funcDouble(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(getdouble(params[0]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcABS(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(fabs(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcACOS(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(acos(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcASIN(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(asin(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcATAN(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(atan(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcATAN2(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(atan2(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcCEIL(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(ceil(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcCOPYSIGN(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(_copysign(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcCOS(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(cos(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcCOSH(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(cosh(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcEXP(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(exp(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcFABS(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(fabs(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcFLOOR(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(floor(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcFMOD(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(fmod(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcFREXP(CValue& RetVal, CValue **params)
{
	int exp;

	RetVal = CNumeric(frexp(getdouble(params[0]), &exp));
	*params[1] = static_cast<long>(exp);
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcHYPOT(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(_hypot(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcLABS(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(labs(getlong(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcLDEXP(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(ldexp(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcLDIV(CValue& RetVal, CValue **params)
{
	long nDivider = getlong(params[1]);
	if (!nDivider)
		RuntimeErrorRes(1611);
	_ldiv_t a = ldiv(getlong(params[0]), nDivider);

	RetVal = CNumeric(a.quot);
	*params[2] = static_cast<long>(a.rem);
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcLOG(CValue& RetVal, CValue **params)
{
	double d = log(getdouble(params[0]));
	if (!_finite(d))
		RuntimeError("LOG: Evaluated to INF / NaN");

	RetVal = CNumeric(d);

	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcLOG10(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(log10(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcMODF(CValue& RetVal, CValue **params)
{
	double b;

	RetVal = CNumeric(modf(getdouble(params[0]), &b));
	*params[1] = CNumeric(b);
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcPOW(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(pow(getdouble(params[0]), getdouble(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcPI(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(3.141592653589793238462643383279502884197169399375);
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcRAND(CValue& RetVal, CValue **params)
{
	RetVal = static_cast<long>(rand());
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcRAND1(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(static_cast<long double>(rand()) / static_cast<long double>(RAND_MAX));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcROTL(CValue& RetVal, CValue **params)
{
	RetVal = static_cast<long>(_rotl(getlong(params[0]), getlong(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcROTR(CValue& RetVal, CValue **params)
{
	RetVal = static_cast<long>(_rotr(getlong(params[0]), getlong(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSCALB(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(_scalb(getdouble(params[0]), getlong(params[1])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSHL(CValue& RetVal, CValue **params)
{
	RetVal = static_cast<long>(getlong(params[0]) << getlong(params[1]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSHR(CValue& RetVal, CValue **params)
{
	RetVal = static_cast<long>(getlong(params[0]) >> getlong(params[1]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSIN(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(sin(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSINH(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(sinh(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcSQRT(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(sqrt(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::procSRAND(CValue **params)
{
	srand(getlong(params[0]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcTAN(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(tan(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcTANH(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(tanh(getdouble(params[0])));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcBITAND(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(getlong(params[0]) & getlong(params[1]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcBITOR(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(getlong(params[0]) | getlong(params[1]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcCOMPL(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(~getlong(params[0]));
	return TRUE;
}
//______________________________________________________________________________
//
BOOL CMath::funcXOR(CValue& RetVal, CValue **params)
{
	RetVal = CNumeric(getlong(params[0]) ^ getlong(params[1]));
	return TRUE;
}
//______________________________________________________________________________
//

