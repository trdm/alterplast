// OXY_Value.cpp: implementation of the COXY_Value class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "OXY_Value.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
typedef char const * (CValue::*PF_Format)(void) const;
static BYTE* adrNativeFormat;
static PF_Format NewFormat;
static BYTE NativeFuncBody[6];

#define COPY6(FromPtr, ToPtr) \
    BYTE* fp = (BYTE*)FromPtr; \
    BYTE* tp = (BYTE*)ToPtr; \
    for (int i = 0; i < 6; i++) *tp++ = *fp++;

#define SetWrapJmp(NativeFuncAdr, NewFuncAdr) \
    BYTE* p = NativeFuncAdr; \
    *p++ = 0xFF; \
    *p++ = 0x25; \
	*(DWORD*)p = (DWORD)&NewFuncAdr;

void COXY_Value::Wrap()
{
	HINSTANCE hBkend = GetModuleHandle("bkend.dll");
    DWORD oldProtect;

    adrNativeFormat = (BYTE*)GetProcAddress(hBkend, "?Format@CValue@@QBEPBDXZ");
	VirtualProtect(adrNativeFormat, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
    COPY6(adrNativeFormat, NativeFuncBody);
	NewFormat = (PF_Format)WrapedFormat;
    SetWrapJmp(adrNativeFormat, NewFormat);
}

char const * COXY_Value::WrapedFormat() const
{
    char const * Ret = NULL;
    
    if (Type == 100)
    {
        CBLContext* pCont = GetContext();
        if (pCont)
        {
            if (!strcmp(pCont->GetRuntimeClass()->m_lpszClassName, "CComponentClass"))
            {
                Ret = static_cast<CComponentClass*>(pCont)->GetObjectStringView();
            }
        }
    };

    if (!Ret)
    {
        COPY6(NativeFuncBody, adrNativeFormat);
        Ret = Format();
        SetWrapJmp(adrNativeFormat, NewFormat);
    };

    return Ret;
}
*/