// sp_oledbparam.h: interface for the COLEDBParam class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SP_OLEDBPARAM_H__61DECD3D_C6C7_4FBE_B72E_6245E6C2939D__INCLUDED_)
#define AFX_SP_OLEDBPARAM_H__61DECD3D_C6C7_4FBE_B72E_6245E6C2939D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//_____________________________________________________________________________
//
class COLEDBParam  
{
public:
	COLEDBParam();
	COLEDBParam(const COLEDBParam&);
	virtual ~COLEDBParam();

	operator COLEDBParam*() const;
	COLEDBParam& operator=(const COLEDBParam&);

public:
	ULONG m_Ordinal;
	DWORD m_ParamIO;
	WORD m_Type;
	BYTE m_Length;
	BYTE m_Precision;
	CValue m_Value;
	long m_MemSize; // размер параметра в байтах
};

#endif // !defined(AFX_SP_OLEDBPARAM_H__61DECD3D_C6C7_4FBE_B72E_6245E6C2939D__INCLUDED_)