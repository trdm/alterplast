
#if !defined(__COM_Object_H__)
#define __COM_Object_H__

#include "mycontextbase.h"
#include "V7Control.h"
#include "CtrlEvents.h"
#include "System/sh_ptr.hpp"

#define IMPLTYPE_MASK (IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE | IMPLTYPEFLAG_FRESTRICTED)
#define IMPLTYPE_DEFAULTSOURCE (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE)


struct EventInfo
{
	DWORD procid;
	DWORD argNum;
};

typedef CMap<DISPID,DISPID,EventInfo,EventInfo> CDispIdToProcIdMap ;

class CCOM_ObjectContainer : public CCmdTarget
{
public:
	CString m_ClassName;
	CString m_EventPrefix; //префикс дл€ обработчиков событий
	CBLModule* m_pHostModule; //модуль - обработчик событий
	IUnknownPtr m_pUnknown; //контейнер дл€ COM-объекта
	sh_ptr<CDispIdToProcIdMap> m_EventHandlers;

	CCOM_ObjectContainer() :
		m_EventHandlers(new CDispIdToProcIdMap(10)),
		m_iid( IID_NULL ),
		m_dwEventCookie(0), m_pIConnectionPoint(NULL)
	{
		m_pHostModule = NULL;
		m_pUnknown = NULL;
	};

	~CCOM_ObjectContainer();

	BOOL CreateObject(CString strClassName, CBLContext* pBLCont, CString strEventPrefix);
	void SetupEventHandlers();
	void CallEventHandler(EventInfo ei, CValue** lArray, CBLModule7* pModule);


	//=============================================================
	// универсальное подключение к событи€м јктив»кс - artbear
	//=============================================================
	IID m_iid;
	IConnectionPointPtr m_pIConnectionPoint;
	DWORD m_dwEventCookie;
	
	void OnEvent( DISPID dispid, DISPPARAMS* pdpParams );
	
	BEGIN_INTERFACE_PART( EventHandler, IDispatch )
		STDMETHOD( GetIDsOfNames )( REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs );
		STDMETHOD( GetTypeInfo )( UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo );
		STDMETHOD( GetTypeInfoCount )( UINT* pnInfoCount );
		STDMETHOD( Invoke )( DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgError );
	END_INTERFACE_PART( EventHandler )
		
	DECLARE_INTERFACE_MAP()
		
	IUnknown* GetInterfaceHook( const void* pv );

	BOOL EstablishSink();
	void CloseSink();

	bool IsSinkToEvent() const { return 0 != m_dwEventCookie; };
};



class CCOM_Object : public CMyContextBase
{
	DECLARE_DYNCREATE(CCOM_Object);
public:
	BOOL GetPropObject(CValue& Value) const;
	BOOL methCreateObject(CValue& RetVal, CValue **params);
	

	//CAXControl();
	virtual ~CCOM_Object();

    DECLARE_MY_CONTEXT()

	CCOM_ObjectContainer Container;

};


#endif
