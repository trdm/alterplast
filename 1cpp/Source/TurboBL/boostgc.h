// boostgc.h
#ifndef BOOSTGC_H
#define BOOSTGC_H
#include "../mycontextbase.h"

class CBoostGC : public CMyContextBase
{
DECLARE_DYNCREATE(CBoostGC);
public:
	CBoostGC();
	virtual ~CBoostGC();
	int DoBoost(CValue**);
	DECLARE_MY_CONTEXT()
};

#endif