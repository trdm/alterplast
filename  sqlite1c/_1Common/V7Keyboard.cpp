// V7Keyboard.cpp: implementation of the CV7Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#include "V7Keyboard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CV7Keyboard* pV7Keyboard = NULL;
CV7Keyboard* CV7Keyboard::GetV7Keyboard()
{
	if (!pV7Keyboard)
		pV7Keyboard = (CV7Keyboard*)CBLContext::CreateInstance("Keyboard");
	return pV7Keyboard;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CV7Keyboard::CV7Keyboard()
{

}

CV7Keyboard::~CV7Keyboard()
{

}

BL_INIT_CONTEXT(CV7Keyboard);

BOOL CV7Keyboard::GetKeyState(CValue& retVal, CValue** ppParams)
{
	SHORT state = ::GetKeyState(ppParams[0]->GetNumeric());
	retVal = HIBYTE(state) ? 1L : 0L;
	*ppParams[1] = LOBYTE(state) ? 1L : 0L;
	return TRUE;
}
BOOL CV7Keyboard::GetKeyState_GetDefVal(int nParam, CValue* pValue)const
{
	if (nParam == 1)
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
};

BOOL CV7Keyboard::GetKeyNameText(CValue& retVal, CValue** ppParams)
{
	DWORD dw = ppParams[0]->GetNumeric();
	if (ppParams[1]->GetNumeric() == 1)
		dw |= 0x02000000;

	CString str;
	str.GetBufferSetLength(50);
	::GetKeyNameText(dw, str.GetBufferSetLength(50), 50);
	str.ReleaseBuffer();

	retVal = str;
	return TRUE;
}
BOOL CV7Keyboard::GetKeyNameText_GetDefVal(int nParam, CValue* pValue)const
{
	if (nParam == 1)
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
};
