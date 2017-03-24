/******************************************************************************
*
*  File: SimpleHostBackup.cpp
*
*  Author:  Joel Alley
*
*  Date: November 20, 1998
*
*  Description:   This file contains the definition of the SimpleHostBackup 
*                 class, which fills in the functions of a Smart Host for those
*                 Script Hosts which do not implement IActiveScriptSiteDebug.
*
*  Modifications: 
******************************************************************************/
#define SIMPLEHOSTBACKUP
#include "debugDepends.h"

/******************************************************************************
*  Initialize -- This method intializes the SimpleHostBackup with the 
*  IActiveScriptSite reference it will contain to provide Active Debugging 
*  support.
*  Parameters: pScriptSite -- IActiveScriptSite pointer to be contained by this
*                             object.
*  Returns: none
******************************************************************************/
BOOL SimpleHostBackup::Initialize( IActiveScriptSite* pScriptSite )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::Initialize\n");
   HRESULT hr = S_OK;

   //cache the pScriptSite pointer
   m_pScriptSite = pScriptSite;
   pScriptSite->AddRef();
   
   //Interrogate the script host for the interfaces we're prepared to contain.
   //We don't really care about the HRESULT's here.
   
   //IActiveScriptSiteWindow
   hr = pScriptSite->QueryInterface( IID_IActiveScriptSiteWindow, 
      (void**)&m_pScriptSiteWindow );
   
   //IHostInfoProvider
   hr = pScriptSite->QueryInterface( IID_IHostInfoProvider, 
      (void**)&m_pHostInfoProvider );

   //Now we need to set up for Smart Debugging.  First, we need a Process Debug 
   //Manager.
   hr = CoCreateInstance(CLSID_ProcessDebugManager, NULL, 
      CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, 
      IID_IProcessDebugManager, (void **)&m_pProcDebugManager);

   //If that failed, the facilities aren't available for smart debugging
   if (FAILED(hr)){
      HOSTBACKUPTRACE("No Process Debug Manager available!\n");
      return false;
   }

   //Next, create a debugger application to associate with the script we're 
   //running.  An "application" is the smallest unit that can be debugged.
   hr = m_pProcDebugManager->CreateApplication(&m_pDebugApplication);
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("Failed to create debug application.\n");
      return false;
   }

   //Set the name that will appear in the debugger heirarchy for this app.
   hr = m_pDebugApplication->SetName(L"DebugScript script");
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("IDebugApplication::SetName failed.\n");
      return false;
   }

   //Add the debugger application to the debug heirarchy of the debug manager
   //so it will appear in the debug window.
   hr = m_pProcDebugManager->AddApplication(m_pDebugApplication, 
      &m_dwDebugCookie);
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("Failed to Add application.\n");
      return false;
   }

   //Create a new DebugDocumentHelper to use with the script added by the 
   //simple host.
   hr = m_pProcDebugManager->CreateDebugDocumentHelper(NULL, 
      &m_pDebugDocumentHelper );
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("Failed to create debug document helper.\n");
      return false;
   }
   
   //Initialize the DebugDocumentHelper so it's ready to handle the script.
   hr = m_pDebugDocumentHelper->Init(m_pDebugApplication, L"DebugScript", 
      L"DebugScript document", TEXT_DOC_ATTR_READONLY);
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("Failed to initialize m_pDebugDocumentHelper.\n");
      return false;
   }
   
   //Attach our DebugDocumentHelper to the debugger application.  The NULL
   //parameter indicates that the DebugDocumentHelper should be at the root
   //of the debug tree.
   hr = m_pDebugDocumentHelper->Attach(NULL);
   if (FAILED(hr))
   {
      HOSTBACKUPTRACE("Failed to attach m_pDebugDocumentHelper.\n");
      return false;
   }
   
   //Everything succeeded, so return true
   return true;
}

/******************************************************************************
*  PreParseScriptText -- This method preprocesses the script text added to the
*  script engine to provide Active Debugging support.
*  Parameters: pstrCode -- the script text to enter into the debugger
*              pScript -- IActiveScript reference of the engine responsible for 
*                         this script block.
*              fScriptlet -- Boolean value indicating whether or not the script 
*                            block is a scriplet
*              pdwSourceContextCookie -- the source context provided by the 
*                                        script debugger.
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::PreParseScriptText( LPCOLESTR pstrCode, 
                                                  IActiveScript* pScript,
                                                  BOOL fScriptlet, 
                                                  DWORD* pdwSourceContextCookie )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::PreParseScriptText\n");

   HRESULT hr = S_OK;

   //What is the length of this script block
   ULONG scriptLength = wcslen( pstrCode );

   //Add the script text to the Debug Document Helper
   hr = m_pDebugDocumentHelper->AddUnicodeText( pstrCode );
   
   if (FAILED(hr)){
      //This is bad, but it's not really a fatal error
      HOSTBACKUPTRACE("Failed to add script text to debug document helper.\n");
   }

   //Define the script block
   hr = m_pDebugDocumentHelper->DefineScriptBlock( m_characterOffset, 
      scriptLength, pScript, fScriptlet, pdwSourceContextCookie );

   if (FAILED(hr)){
      //Once again, this is bad, but not really fatal
      HOSTBACKUPTRACE("Failed to define script block!\n");
   }

   //We get a chance here to define script attributes, so use CScanner to get
   //the text attributes.  Create an array of SOURCE_TEXT_ATTR's to fill first.
   SOURCE_TEXT_ATTR *attrArray = new SOURCE_TEXT_ATTR[ scriptLength ];
   memset( attrArray, 0, scriptLength );

   //Scan the text and get the attributes back
   CScanner* pScanner = new CScanner();
   pScanner->ScanScript( pstrCode, scriptLength, attrArray );

   //Set the attributes of the script block
   hr = m_pDebugDocumentHelper->SetTextAttributes( m_characterOffset, 
      scriptLength, attrArray );

   if (FAILED(hr)){
      //Still not a fatal error
      HOSTBACKUPTRACE("Failed to set text attributes!\n");
   }

   //Add the length of this script block to the current character offset
   m_characterOffset += scriptLength;

   return S_OK;
}

/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either directly or 
*   indirectly, the IUnknown interface.
******************************************************************************/

/******************************************************************************
*   QueryInterface -- Determines if this component supports the requested 
*   interface, places a pointer to that interface in ppvObj if it's available,
*   and returns S_OK.  If not, sets ppvObj to NULL and returns E_NOINTERFACE.
******************************************************************************/
STDMETHODIMP SimpleHostBackup::QueryInterface(REFIID riid, void ** ppvObj)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::QueryInterface->");
   
   if (riid == IID_IUnknown){
      HOSTBACKUPTRACE("IUnknown -- Supported");
      *ppvObj = static_cast<IActiveScriptSite*>(this);
   }
   
   else if (riid == IID_IActiveScriptSite){
      HOSTBACKUPTRACE("IActiveScriptSite -- Supported");
      *ppvObj = static_cast<IActiveScriptSite*>(this);
   }
   
   else if (riid == IID_IActiveScriptSiteWindow){
      HOSTBACKUPTRACE("IActiveScriptSiteWindow");
      
      //Make sure the true host supports the interface
      if ( m_pScriptSiteWindow != NULL){
         HOSTBACKUPTRACE(" -- Supported\n" );
         *ppvObj = static_cast<IActiveScriptSiteWindow*>(this);
      }

      //If not, then don't mimic the interface.
      else{
         HOSTBACKUPTRACE(" -- Not Supported\n");
         *ppvObj = NULL;
         return E_NOINTERFACE;
      }
   }
   
   else if (riid == IID_IHostInfoProvider){
      HOSTBACKUPTRACE("IHostInfoProvider");
      
      //Make sure the true host supports the interface
      if (m_pHostInfoProvider != NULL){
         HOSTBACKUPTRACE(" -- Supported\n");
         *ppvObj = static_cast<IHostInfoProvider*>(this);
      }

      //If not, then don't mimic the interface.
      else{
         HOSTBACKUPTRACE(" -- Not Supported\n");
         *ppvObj = NULL;
         return E_NOINTERFACE;
      }
   }
   
   else if (riid == IID_IActiveScriptSiteDebug){
      HOSTBACKUPTRACE("IActiveScriptSiteDebug -- Supported");
      *ppvObj = static_cast<IActiveScriptSiteDebug*>(this);
   }
   
   else{
      HOSTBACKUPTRACE("Unsupported Interface\n");
      *ppvObj = NULL;
      return E_NOINTERFACE;
   }
   
   static_cast<IUnknown*>(*ppvObj)->AddRef();
   return S_OK;
}

/******************************************************************************
*   AddRef() -- In order to allow an object to delete itself when it is no 
*   longer needed, it is necessary to maintain a count of all references to 
*   this object.  When a new reference is created, this function increments
*   the count.
******************************************************************************/
STDMETHODIMP_(ULONG) SimpleHostBackup::AddRef()
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::AddRef\n");
   
   return ++m_refCount;
}

/******************************************************************************
*   Release() -- When a reference to this object is removed, this function 
*   decrements the reference count.  If the reference count is 0, then this 
*   function deletes this object and returns 0;
******************************************************************************/
STDMETHODIMP_(ULONG) SimpleHostBackup::Release()
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::Release\n");
   
   if (--m_refCount == 0)
   {
      delete this;
      return 0;
   }
   return m_refCount;
}

/******************************************************************************
*  IActiveScriptSite interface -- This interface is the root of ActiveX Script
*  Hosting.  All ActiveX Script Hosts must implement it.  We implement it here
*  because we are containing the actual script host to provide debugging 
*  support.
******************************************************************************/

/******************************************************************************
*  GetLCID -- Retrieves the locale identifier associated with the host's user 
*  interface. The scripting engine uses the identifier to ensure that error 
*  strings and other user-interface elements generated by the engine appear in 
*  the appropriate language. 
*  Parameters: plcid -- address of variable for language identifier
*  Returns: S_OK
*           E_NOTIMPL
*           E_POINTER
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetLCID( LCID *plcid )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetLCID\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->GetLCID( plcid );
}

/******************************************************************************
*  GetItemInfo -- Allows the scripting engine to obtain information about an 
*  item added with the IActiveScript::AddNamedItem method
*  Parameters: pstrName -- address of item name
*              dwReturnMask -- bit mask for information retrieval
*              ppunkItem -- address of pointer to item's IUnknown
*              ppTypeInfo -- address of pointer to item's ITypeInfo
*  Returns: S_OK
*           E_INVALIDARG
*           E_POINTER
*           TYPE_E_ELEMENTNOTFOUND
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetItemInfo( LPCOLESTR pstrName, DWORD dwReturnMask,
                                           IUnknown **ppunkItem, ITypeInfo **ppTypeInfo)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetItemInfo\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->GetItemInfo( pstrName, dwReturnMask, ppunkItem, 
      ppTypeInfo );
}

/******************************************************************************
*  GetDocVersionString -- Retrieves a host-defined string that uniquely 
*  identifies the current document version. If the related document has changed
*  outside the scope of ActiveX Scripting (as in the case of an HTML page being 
*  edited with NotePad), the scripting engine can save this along with its 
*  persisted state, forcing a recompile the next time the script is loaded. 
*
*  Parameters: pbstrVersionString -- address of document version string
*
*  Returns: S_OK
*           E_NOTIMPL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetDocVersionString( BSTR *pbstrVersionString )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetDocVersionString\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->GetDocVersionString( pbstrVersionString );
}

/******************************************************************************
*  OnScriptTerminate -- Informs the host that the script has completed 
*  execution
*  Parameters: pvarResult -- address of script results
*              pexcepinfo -- address of structure with exception information
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnScriptTerminate( const VARIANT *pvarResult, 
                                                 const EXCEPINFO *pexcepinfo)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnScriptTerminate\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->OnScriptTerminate( pvarResult, pexcepinfo );
}

/******************************************************************************
*  OnStateChange -- Informs the host that the scripting engine has changed 
*  states
*  Parameters: ssScriptState -- new state of engine
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnStateChange( SCRIPTSTATE ssScriptState )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnStateChange\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->OnStateChange( ssScriptState );
}

/******************************************************************************
*  OnScriptError -- Informs the host that an execution error occurred while the 
*  engine was running the script
*  Parameters: pase -- address of error interface
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnScriptError( IActiveScriptError *pase )
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnScriptError\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->OnScriptError( pase );
}

/******************************************************************************
*  OnEnterScript -- Informs the host that the scripting engine has begun 
*  executing the script code
*  Parameters: none
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnEnterScript(void)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnEnterScript\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->OnEnterScript();
}

/******************************************************************************
*  OnLeaveScript -- Informs the host that the scripting engine has returned 
*  from executing script code
*  Parameters: none
*  Returns: S_OK
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnLeaveScript(void)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnLeaveScript\n");
   
   //Pass on the call to the true host.
   return m_pScriptSite->OnLeaveScript();
}

/******************************************************************************
*  IActiveScriptSiteWindow -- This interface is implemented by hosts that 
*  support a user interface on the same object as IActiveScriptSite. Hosts that 
*  do not support a user interface, such as servers, would not implement the 
*  IActiveScriptSiteWindow interface. The scripting engine accesses this 
*  interface by calling QueryInterface from IActiveScriptSite.  It is 
*  implemented here because we are containing the actual script host to provide
*  debugging support.
******************************************************************************/

/******************************************************************************
*  GetWindow -- Retrieves the handle to a window that can act as the owner of a 
*  pop-up window that the scripting engine must display
*  Parameters: phwnd -- address of variable for window handle
*  Returns: S_OK
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetWindow(HWND *phwnd)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetWindow\n");
   
   //Pass on the call to the true host.
   return m_pScriptSiteWindow->GetWindow( phwnd );
}

/******************************************************************************
*  EnableModeless -- Causes the host to enable or disable its main window as 
*  well as any modeless dialog boxes
*  Parameters: fEnable -- enable flag
*  Returns: S_OK
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::EnableModeless(BOOL fEnable)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::EnableModeless\n");
   
   //Pass on the call to the true host.
   return m_pScriptSiteWindow->EnableModeless( fEnable );
}

/******************************************************************************
*  IHostInfoProvider -- This interface allows the host to inform the script
*  engine that it needs to ask the host for the LCID to use in various
*  circumstances.
******************************************************************************/

/******************************************************************************
*  GetHostInfo -- Takes a enumerated value specifying which type of information
*  the script engine is requesting from the host.  It then passes a 
*  CoTaskMemAlloc'd block containing the LCID through ppvInfo.
*  Parameters: hostinfo -- enumerated value indicating what LCID the engine 
*                          requested.
*              ppvInfo -- CoTaskMemAlloc'd block containing the LCID
*  Returns: S_OK
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetHostInfo(hostinfo hostinfoRequest, 
                                           void **ppvInfo)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetHostInfo\n");
   
   //Pass on the call to the true host.
   return m_pHostInfoProvider->GetHostInfo( hostinfoRequest, ppvInfo );
}

/******************************************************************************
*  IActiveScriptSiteDebug interface -- This interface is implemented by smart 
*  hosts and is QI-able from IActiveScriptSite. It provides the means by which 
*  a smart host takes over document management and participates in debugging.
******************************************************************************/

/******************************************************************************
*  GetDocumentContextFromPosition -- Used by the language engine to delegate 
*  IDebugCodeContext::GetSourceContext and get the IDebugDocumentContext
*  associated with a position.
*  Parameters: dwSourceContext -- Host defined cookie which was passed to 
*                                 IActiveScriptParse::ParseScriptText or 
*                                 IActiveScriptParse::AddScriptlet when the 
*                                 text the engine is interested in was added.
*              uCharacterOffset -- Offset from the beginning of the script 
*                                  block
*              uNumChars -- Length of the script block
*              ppsc -- Address of the IDebugDocumentContext reference to be
*                      returned by this method
*  Returns: S_OK
*           E_INVALIDARG
*           E_POINTER
*           E_FALI
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetDocumentContextFromPosition(
                                                  DWORD dwSourceContext,
                                                  ULONG uCharacterOffset,
                                                  ULONG uNumChars,
                                                  IDebugDocumentContext **ppsc)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetDocumentContextFromPosition\n");

   HRESULT hr = E_FAIL;
   
   //Use the IDebugDocumentHelper for this SimpleHostBackup to delegate this
   //call
   if (m_pDebugDocumentHelper != NULL){
      ULONG ulStartPos;

      //Find out where this source context starts.
      hr = m_pDebugDocumentHelper->GetScriptBlockInfo( dwSourceContext, NULL, 
         &ulStartPos, NULL );

      //If that succeeded, then create an IDebugDocumentContext
      if (SUCCEEDED(hr)){
         hr = m_pDebugDocumentHelper->CreateDebugDocumentContext( 
            ulStartPos + uCharacterOffset, uNumChars, ppsc );
      }
   }

   return hr;
}

/******************************************************************************
*  GetApplication -- Returns the debug application object associated with this 
*  script site. Provides a means for a smart host to define what application 
*  object each script belongs to. Script engines should attempt to call this 
*  method to get their containing application and resort to 
*  IProcessDebugManager::GetDefaultApplication if this fails. 
*
*  Parameters: ppda -- Address of the IDebugApplication pointer which this 
*                      method should fill.
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetApplication(IDebugApplication **ppda)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetApplication\n");

   HRESULT hr = E_FAIL;

   //if we were successful in creating an IDebugApplication in Initialize,
   //then return it here.
   if (m_pDebugApplication != NULL){
      hr = m_pDebugApplication->QueryInterface( IID_IDebugApplication, 
         (void**)ppda );
   }

   return hr;
}

/******************************************************************************
*  GetRootApplicationNode -- Gets the application node under which script 
*  documents should be added.  This method can return NULL if script documents 
*  should be top-level.
*  Parameters: ppdanRoot -- Address of the IDebugApplicationNode pointer which
*                           this method should fill
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::GetRootApplicationNode(
                                          IDebugApplicationNode **ppdanRoot)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::GetRootApplicationNode\n");
   
   HRESULT hr = E_FAIL;

   //If we were successful in creating an IDebugApplication in Initialize,
   //then call GetRootNode on it here.
   if (m_pDebugApplication != NULL){
      hr = m_pDebugApplication->GetRootNode( ppdanRoot );
   }

   return hr;
}

/******************************************************************************
*  OnScriptErrorDebug -- Allows a smart host to control the handling of runtime 
*  errors.
*  Parameters: pErrorDebug -- the runtime error that occurred 
*              pfEnterDebugger -- whether to pass the error to the debugger to 
*                                 do JIT debugging
*              pfCallOnScriptErrorWhenContinuing -- whether to call 
*                                 IActiveScriptSite::OnScriptError() when the 
*                                 user decides to continue without debugging. 
*  Returns: S_OK
*           E_POINTER
*           E_FAIL
******************************************************************************/
STDMETHODIMP SimpleHostBackup::OnScriptErrorDebug(
                                      IActiveScriptErrorDebug *pErrorDebug,
                                      BOOL *pfEnterDebugger,
                                      BOOL *pfCallOnScriptErrorWhenContinuing)
{
   //tracing purposes only
   HOSTBACKUPTRACE("SimpleHostBackup::OnScriptErrorDebug\n");

   //Request JIT debugging
   *pfEnterDebugger = true;

   //If JIT debugging is declined, call IActiveScriptSite::OnScriptError
   *pfCallOnScriptErrorWhenContinuing = true;

   return S_OK;
}
