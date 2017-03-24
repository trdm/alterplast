// MetaWork.cpp: implementation of the CMetaWork class.
#include "stdafx.h"
#include "MetaWork.h"

typedef bool (*PFINDFUNC)(CString,CPtrArray&);
static const DWORD Type2Funcs[]=
{
	0xFFFFFFFF,
};


static const PFINDFUNC CheckFunc[]=
{
	NULL,
};

CMetaWork::CMetaWork()
{
}

CMetaWork::~CMetaWork()
{
}

CTypeWrap* CMetaWork::GetTypesFromProps(CString prop, CTypeWrap* pCurrentType)
{
	// Build id index from existing types
	return NULL;
}
