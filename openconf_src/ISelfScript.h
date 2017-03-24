// ISelfScript.h: interface for the CISelfScript class.
#if !defined(AFX_ISELFSCRIPT_H__9231B24C_A98D_4A01_A29B_DA370B7D0B0C__INCLUDED_)
#define AFX_ISELFSCRIPT_H__9231B24C_A98D_4A01_A29B_DA370B7D0B0C__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"
class CScripting;

class CISelfScript : public CDispImpl<ISelfScript, SelfScript>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ISelfScript)
	END_COM_MAP()
	void ResetAdded();
	CISelfScript();
	virtual ~CISelfScript();
	STDMETHOD(get_Name)(BSTR *pVal);
	STDMETHOD(AddNamedItem)(BSTR Name,IDispatch *Object,VARIANT_BOOL AsGlobal);

	CScripting* m_pScript;
	CMapStringToPtr m_Objects;
};

#endif // !defined(AFX_ISELFSCRIPT_H__9231B24C_A98D_4A01_A29B_DA370B7D0B0C__INCLUDED_)
