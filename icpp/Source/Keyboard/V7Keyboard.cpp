// V7Keyboard.cpp: implementation of the CV7Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7Keyboard.h"
#include "VKeys.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//**********************************
//–≈јЋ»«ј÷»я —¬ќ…—“¬
//**********************************


//описание глобального массива имен и значений VK
#define ASSOC_VK(parVK) {#parVK, ##parVK}
CV7Keyboard::VirtKeyAssoc CV7Keyboard::vkarr[] =
{
	ASSOC_VK(VK_LBUTTON),
	ASSOC_VK(VK_RBUTTON),
	ASSOC_VK(VK_CANCEL),
	ASSOC_VK(VK_MBUTTON),

	ASSOC_VK(VK_BACK),
	ASSOC_VK(VK_TAB),

	ASSOC_VK(VK_CLEAR),
	ASSOC_VK(VK_RETURN),

	ASSOC_VK(VK_SHIFT),
	ASSOC_VK(VK_CONTROL),
	ASSOC_VK(VK_MENU),
	ASSOC_VK(VK_PAUSE),
	ASSOC_VK(VK_CAPITAL),

	ASSOC_VK(VK_KANA),
	ASSOC_VK(VK_HANGEUL),
	ASSOC_VK(VK_HANGUL),
	ASSOC_VK(VK_JUNJA),
	ASSOC_VK(VK_FINAL),
	ASSOC_VK(VK_HANJA),
	ASSOC_VK(VK_KANJI),

	ASSOC_VK(VK_ESCAPE),

	ASSOC_VK(VK_CONVERT),
	ASSOC_VK(VK_NONCONVERT),
	ASSOC_VK(VK_ACCEPT),
	ASSOC_VK(VK_MODECHANGE),

	ASSOC_VK(VK_SPACE),
	ASSOC_VK(VK_PRIOR),
	ASSOC_VK(VK_NEXT),
	ASSOC_VK(VK_END),
	ASSOC_VK(VK_HOME),
	ASSOC_VK(VK_LEFT),
	ASSOC_VK(VK_UP),
	ASSOC_VK(VK_RIGHT),
	ASSOC_VK(VK_DOWN),
	ASSOC_VK(VK_SELECT),
	ASSOC_VK(VK_PRINT),
	ASSOC_VK(VK_EXECUTE),
	ASSOC_VK(VK_SNAPSHOT),
	ASSOC_VK(VK_INSERT),
	ASSOC_VK(VK_DELETE),
	ASSOC_VK(VK_HELP),

	ASSOC_VK(VK_0),
	ASSOC_VK(VK_1),
	ASSOC_VK(VK_2),
	ASSOC_VK(VK_3),
	ASSOC_VK(VK_4),
	ASSOC_VK(VK_5),
	ASSOC_VK(VK_6),
	ASSOC_VK(VK_7),
	ASSOC_VK(VK_8),
	ASSOC_VK(VK_9),

	ASSOC_VK(VK_A),
	ASSOC_VK(VK_B),
	ASSOC_VK(VK_C),
	ASSOC_VK(VK_D),
	ASSOC_VK(VK_E),
	ASSOC_VK(VK_F),
	ASSOC_VK(VK_G),
	ASSOC_VK(VK_H),
	ASSOC_VK(VK_I),
	ASSOC_VK(VK_J),
	ASSOC_VK(VK_K),
	ASSOC_VK(VK_L),
	ASSOC_VK(VK_M),
	ASSOC_VK(VK_N),
	ASSOC_VK(VK_O),
	ASSOC_VK(VK_P),
	ASSOC_VK(VK_Q),
	ASSOC_VK(VK_R),
	ASSOC_VK(VK_S),
	ASSOC_VK(VK_T),
	ASSOC_VK(VK_U),
	ASSOC_VK(VK_V),
	ASSOC_VK(VK_W),
	ASSOC_VK(VK_X),
	ASSOC_VK(VK_Y),
	ASSOC_VK(VK_Z),

	ASSOC_VK(VK_LWIN),
	ASSOC_VK(VK_RWIN),
	ASSOC_VK(VK_APPS),

	ASSOC_VK(VK_SLEEP),

	ASSOC_VK(VK_NUMPAD0),
	ASSOC_VK(VK_NUMPAD1),
	ASSOC_VK(VK_NUMPAD2),
	ASSOC_VK(VK_NUMPAD3),
	ASSOC_VK(VK_NUMPAD4),
	ASSOC_VK(VK_NUMPAD5),
	ASSOC_VK(VK_NUMPAD6),
	ASSOC_VK(VK_NUMPAD7),
	ASSOC_VK(VK_NUMPAD8),
	ASSOC_VK(VK_NUMPAD9),
	ASSOC_VK(VK_MULTIPLY),
	ASSOC_VK(VK_ADD),
	ASSOC_VK(VK_SEPARATOR),
	ASSOC_VK(VK_SUBTRACT),
	ASSOC_VK(VK_DECIMAL),
	ASSOC_VK(VK_DIVIDE),
	ASSOC_VK(VK_F1),
	ASSOC_VK(VK_F2),
	ASSOC_VK(VK_F3),
	ASSOC_VK(VK_F4),
	ASSOC_VK(VK_F5),
	ASSOC_VK(VK_F6),
	ASSOC_VK(VK_F7),
	ASSOC_VK(VK_F8),
	ASSOC_VK(VK_F9),
	ASSOC_VK(VK_F10),
	ASSOC_VK(VK_F11),
	ASSOC_VK(VK_F12),
	ASSOC_VK(VK_F13),
	ASSOC_VK(VK_F14),
	ASSOC_VK(VK_F15),
	ASSOC_VK(VK_F16),
	ASSOC_VK(VK_F17),
	ASSOC_VK(VK_F18),
	ASSOC_VK(VK_F19),
	ASSOC_VK(VK_F20),
	ASSOC_VK(VK_F21),
	ASSOC_VK(VK_F22),
	ASSOC_VK(VK_F23),
	ASSOC_VK(VK_F24),

	ASSOC_VK(VK_NUMLOCK),
	ASSOC_VK(VK_SCROLL),

	ASSOC_VK(VK_LSHIFT),
	ASSOC_VK(VK_RSHIFT),
	ASSOC_VK(VK_LCONTROL),
	ASSOC_VK(VK_RCONTROL),
	ASSOC_VK(VK_LMENU),
	ASSOC_VK(VK_RMENU),

	ASSOC_VK(VK_PROCESSKEY),

	ASSOC_VK(VK_ATTN),
	ASSOC_VK(VK_CRSEL),
	ASSOC_VK(VK_EXSEL),
	ASSOC_VK(VK_EREOF),
	ASSOC_VK(VK_PLAY),
	ASSOC_VK(VK_ZOOM),
	ASSOC_VK(VK_NONAME),
	ASSOC_VK(VK_PA1),
	ASSOC_VK(VK_OEM_CLEAR),
};

//инициализаци€ карты свойств VK_xxx
CBLMap CV7Keyboard::VKMap;
void CV7Keyboard::InitVKMap() //static
{
	VKMap.RemoveAll();
	int nVK = sizeof vkarr / sizeof VirtKeyAssoc;
	for(int i=0; i<nVK; i++)
	{
		VKMap.InsertKey(vkarr[i].VK_name,i);
	}
	VKMap.Optimize();
}

BEGIN_BL_PROP_MAP(CV7Keyboard)
	//свойств определ€емых через шаблон пока нет
END_BL_PROP_MAP()

//реализаци€ композитного набора свойств
int CV7Keyboard::GetNProps(void)const
{
	return VKMap.GetCount() + CMyContextBase::GetNProps();
}

int  CV7Keyboard::FindProp(char const * Name)const
{
	int res = VKMap.GetKey(Name);
	if (res == -1)
	{
		res = CMyContextBase::FindProp(Name);
		if (res != -1)
			res += VKMap.GetCount();
	}
	return res;
}

char const* CV7Keyboard::GetPropName(int iPropNum, int iAlias)const
{
	if (iPropNum < VKMap.GetCount())
		return vkarr[iPropNum].VK_name;
	else
		return CMyContextBase::GetPropName(iPropNum-VKMap.GetCount(), iAlias);
}

BOOL CV7Keyboard::IsPropReadable(int iPropNum)const
{
	if (iPropNum < VKMap.GetCount())
		return TRUE;
	else
		return CMyContextBase::IsPropReadable(iPropNum-VKMap.GetCount());
}

BOOL CV7Keyboard::IsPropWritable(int iPropNum)const
{
	if (iPropNum < VKMap.GetCount())
		return FALSE;
	else
		return CMyContextBase::IsPropWritable(iPropNum-VKMap.GetCount());
}

BOOL CV7Keyboard::GetPropVal(int iPropNum,CValue &rValue)const
{
	if (iPropNum < VKMap.GetCount())
	{
		rValue = vkarr[iPropNum].VK_val;
		return TRUE;
	}
	else
		return CMyContextBase::GetPropVal(iPropNum-VKMap.GetCount(), rValue);
}

BOOL CV7Keyboard::SetPropVal(int iPropNum,CValue const &vValue)
{
	if (iPropNum < VKMap.GetCount())
		return FALSE;
	else
		return CMyContextBase::SetPropVal(iPropNum-VKMap.GetCount(), vValue);
}

//**********************************
//–≈јЋ»«ј÷»я ћ≈“ќƒќ¬
//**********************************

BEGIN_BL_METH_MAP(CV7Keyboard)
	BL_METH_FUNC_DEF_PARAM("GetKeyState", "ѕолучить—осто€ние лавиши", 2, &GetKeyState, &GetKeyState_GetDefVal)
	BL_METH_FUNC_DEF_PARAM("GetKeyNameText", "ѕолучить»м€ лавиши", 2, &GetKeyNameText, &GetKeyNameText_GetDefVal)
END_BL_METH_MAP()

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
}

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
}

//**********************************
//–еализаци€ прочей инфраструктуры
//**********************************
IMPLEMENT_MY_CONTEXT(CV7Keyboard, "1cpp.Keyboard", "1cpp. лавиатура", 1, &InitVKMap, NULL, -1);

//глобальный экземпл€р класса  лавиатура
CV7Keyboard* pV7Keyboard = NULL;
CV7Keyboard* CV7Keyboard::GetV7Keyboard() //static
{
	if (!pV7Keyboard)
		pV7Keyboard = (CV7Keyboard*)CBLContext::CreateInstance("1cpp.Keyboard");
	return pV7Keyboard;
}
