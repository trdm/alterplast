// V7Keyboard.h: interface for the CV7Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_V7KEYBOARD_H__D18E9BD1_82D8_4323_9699_BCF4C48543D4__INCLUDED_)
#define AFX_V7KEYBOARD_H__D18E9BD1_82D8_4323_9699_BCF4C48543D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../mycontextbase.h"

class CV7Keyboard : public CMyContextBase
{
	DECLARE_DYNCREATE(CV7Keyboard)
public:
	CV7Keyboard(){};
	static CV7Keyboard* GetV7Keyboard();

	DECLARE_MY_CONTEXT()

	BOOL GetKeyState(CValue& retVal, CValue** ppParams);
	BOOL GetKeyState_GetDefVal(int nParam, CValue* pValue)const;
	BOOL GetKeyNameText(CValue& retVal, CValue** ppParams);
	BOOL GetKeyNameText_GetDefVal(int nParam, CValue* pValue)const;
	
	//перекрываем работу со свойствами для реализации композитного набора свойств (BL_PROP_MAP + VKMap)
	virtual int  GetNProps(void)const;
	virtual int  FindProp(char const *)const;
	virtual char const* GetPropName(int,int)const;
	virtual BOOL IsPropReadable(int)const;
	virtual BOOL IsPropWritable(int)const;
	virtual BOOL GetPropVal(int,CValue&)const;
	virtual BOOL SetPropVal(int,CValue const&);

	struct VirtKeyAssoc{
		LPCSTR VK_name;
		DWORD  VK_val;
	};
	static VirtKeyAssoc vkarr[];
	static CBLMap VKMap;
	static void InitVKMap();
};

#endif // !defined(AFX_V7KEYBOARD_H__D18E9BD1_82D8_4323_9699_BCF4C48543D4__INCLUDED_)
