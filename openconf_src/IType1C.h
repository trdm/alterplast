// IType1C.h: interface for the CIType1C class.
#if !defined(AFX_ITYPE1C_H__EBEF0872_4A6D_46C7_B1C5_D207F63050A8__INCLUDED_)
#define AFX_ITYPE1C_H__EBEF0872_4A6D_46C7_B1C5_D207F63050A8__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIType1C : public CDispImpl<IType1C,Type1C>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IType1C)
	END_COM_MAP()
	CIType1C(const CType& type):m_type(type){}

	STDMETHOD(get_Type)(tTypes *pVal);
	STDMETHOD(put_Type)(tTypes newVal);
	STDMETHOD(get_FullName)(BSTR *pVal);
	STDMETHOD(get_Name)(BSTR *pVal);
	STDMETHOD(get_Code)(BSTR *pVal);
	STDMETHOD(get_ID)(long *pVal);
	STDMETHOD(put_ID)(long newVal);
	STDMETHOD(get_Length)(long *pVal);
	STDMETHOD(put_Length)(long newVal);
	STDMETHOD(get_Precision)(long *pVal);
	STDMETHOD(put_Precision)(long newVal);
	STDMETHOD(get_PositiveOnly)(VARIANT_BOOL *pVal);
	STDMETHOD(put_PositiveOnly)(VARIANT_BOOL newVal);
	STDMETHOD(get_NumSeparated)(VARIANT_BOOL *pVal);
	STDMETHOD(put_NumSeparated)(VARIANT_BOOL newVal);
	
	CType m_type;
};

#endif // !defined(AFX_ITYPE1C_H__EBEF0872_4A6D_46C7_B1C5_D207F63050A8__INCLUDED_)
