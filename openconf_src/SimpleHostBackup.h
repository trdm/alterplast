/******************************************************************************
*
*  File: SimpleHostBackup.h
*
*  Author:  Joel Alley
*
*  Date: November 20, 1998
*
*  Description:   This file contains the declaration of the SimpleHostBackup 
*                 class, which fills in the functions of a Smart Host for those
*                 Script Hosts which do not implement IActiveScriptSiteDebug.
*
*  Modifications: 
******************************************************************************/

class SimpleHostBackup : public IActiveScriptSite,
                         public IActiveScriptSiteWindow,
                         public IHostInfoProvider,
                         public IActiveScriptSiteDebug
{
protected:
   long m_refCount;
   IActiveScriptSite* m_pScriptSite;
   IActiveScriptSiteWindow* m_pScriptSiteWindow;
   IHostInfoProvider* m_pHostInfoProvider;
   IProcessDebugManager* m_pProcDebugManager;
   IDebugApplication* m_pDebugApplication;
   DWORD m_dwDebugCookie;
   IDebugDocumentHelper* m_pDebugDocumentHelper;
   ULONG m_characterOffset;

public:
   //Constructor
   SimpleHostBackup() : m_refCount(0), m_pScriptSite(NULL), 
      m_pScriptSiteWindow(NULL), m_pHostInfoProvider(NULL), 
      m_pProcDebugManager(NULL), m_pDebugApplication(NULL),
      m_pDebugDocumentHelper(NULL), m_characterOffset(0) {;}
   //Destructor
   ~SimpleHostBackup() {;}

   BOOL Initialize( IActiveScriptSite* pScriptSite );
   STDMETHODIMP PreParseScriptText( LPCOLESTR pstrCode, IActiveScript* pScript,
      BOOL fScriptlet, DWORD* pdwSourceContextCookie);

   /******* IUnknown *******/
   STDMETHODIMP QueryInterface(REFIID riid, void * * ppvObj);
   STDMETHODIMP_(ULONG) AddRef();
   STDMETHODIMP_(ULONG) Release();

   /******* IActiveScriptSite *******/
   STDMETHODIMP GetLCID( LCID *plcid );
   STDMETHODIMP GetItemInfo( LPCOLESTR pstrName, DWORD dwReturnMask,
      IUnknown **ppunkItem, ITypeInfo **ppTypeInfo);
   STDMETHODIMP GetDocVersionString( BSTR *pbstrVersionString );
   STDMETHODIMP OnScriptTerminate( const VARIANT *pvarResult, 
      const EXCEPINFO *pexcepinfo);
   STDMETHODIMP OnStateChange( SCRIPTSTATE ssScriptState );
   STDMETHODIMP OnScriptError( IActiveScriptError *pase );
   STDMETHODIMP OnEnterScript(void);
   STDMETHODIMP OnLeaveScript(void);

   /******* IActiveScriptSiteWindow *******/
   STDMETHODIMP GetWindow(HWND *phwnd);
   STDMETHODIMP EnableModeless(BOOL fEnable);

  	/******* IHostInfoProvider Methods *******/
	STDMETHODIMP GetHostInfo(hostinfo hostinfoRequest, void * * ppvInfo);

	/******* IActiveScriptSiteDebug *******/
	STDMETHODIMP GetDocumentContextFromPosition(
	   DWORD dwSourceContext, // As provided to ParseScriptText or AddScriptlet
	   ULONG uCharacterOffset, // character offset relative to start of script 
							   // block or scriptlet 
	   ULONG uNumChars, // Number of characters in context 
	   IDebugDocumentContext **ppsc); 
	STDMETHODIMP GetApplication(
	   IDebugApplication **ppda); 
	STDMETHODIMP GetRootApplicationNode(
	   IDebugApplicationNode **ppdanRoot); 
	STDMETHODIMP OnScriptErrorDebug(
	   IActiveScriptErrorDebug *pErrorDebug,
	   BOOL *pfEnterDebugger,
	   BOOL *pfCallOnScriptErrorWhenContinuing);
};