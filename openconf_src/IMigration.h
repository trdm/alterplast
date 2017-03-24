// IMigration.h: interface for the CIMigration class.
#if !defined(AFX_IMIGRATION_H__4EFA2781_6D5E_11D8_9FCD_ECE2D2E62408__INCLUDED_)
#define AFX_IMIGRATION_H__4EFA2781_6D5E_11D8_9FCD_ECE2D2E62408__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIMigration : public CDispImpl<IMigration>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMigration)
	END_COM_MAP()

	CIMigration(CMetaDataObjDistr* pDistr):m_pDistr(pDistr){}
	virtual ~CIMigration();

	STDMETHOD(get_RuleOfMigration)(MigrationRules *pVal);
	STDMETHOD(put_RuleOfMigration)(MigrationRules newVal);
	STDMETHOD(get_AutoRegister)(VARIANT_BOOL *pVal);
	STDMETHOD(put_AutoRegister)(VARIANT_BOOL newVal);
	STDMETHOD(get_AdditDBSigns)(BSTR *pVal);
	STDMETHOD(put_AdditDBSigns)(BSTR newVal);
	CMetaDataObjDistr* m_pDistr;
	
};

#endif // !defined(AFX_IMIGRATION_H__4EFA2781_6D5E_11D8_9FCD_ECE2D2E62408__INCLUDED_)
