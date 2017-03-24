// Scripting.cpp: implementation of the CScripting class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "Scripting.h"
#include "Scriptmaneger.h"
#include "initguid.h"
#include "iconfig.h"
#include "OCView.h"
#include "IdleHandler.h"
#include "ItemScript.h"
#include "iselfscript.h"
#include "tlibversion.h"

CScripting* CScripting::m_pActiveScript=NULL;
CMapStringToString CScripting::m_mapEngineNames;

CScripting::CScripting()
{
	m_dwRef=1;
	m_pEngine=NULL;
	m_pDebugDocumentHelper=NULL;
	m_EnterCount=0;
	m_DelayUnReload=0;
	m_pViewItem=NULL;
	m_pSelf=new CISelfScript;
	m_pSelf->m_pScript=this;
}

CScripting::~CScripting()
{
	if(m_pEngine!=NULL)
	{
		m_pEngine->SetScriptState(SCRIPTSTATE_CLOSED);
		m_pEngine=NULL;
	}
	if(m_pDebugDocumentHelper)
	{
		m_pDebugDocumentHelper->Detach();
		m_pDebugDocumentHelper->Release();
		m_pDebugDocumentHelper=NULL;
	}
	m_pSelf->Release();
}

STDMETHODIMP CScripting::OnStateChange(SCRIPTSTATE ssScriptState)
{
	return S_OK;
}

static void ScriptIdleProc()
{
	CPtrArray& DelayUnReloadScripts=CScriptManager::GetScriptManager()->m_Delayed;
	long count=DelayUnReloadScripts.GetSize();
	for(int i=0;i<count;i++)
	{
		CScripting* pScript=(CScripting*)DelayUnReloadScripts[i];
		if(pScript)
		{
			if(pScript->m_DelayUnReload==1)
			{
				pScript->Unload();
				pScript->Release();
			}
			else if(pScript->m_DelayUnReload==2)
				pScript->ReloadScript();
		}
	}
	DelayUnReloadScripts.RemoveAll();
	CIdleHandler::RemoveIdleHandler(ScriptIdleProc);
}
STDMETHODIMP CScripting::OnLeaveScript(void)
{
	m_pActiveScript=NULL;
	if(!--m_EnterCount)
	{
		if(m_DelayUnReload)
			CIdleHandler::SetIdleHandler(ScriptIdleProc);
	}
	return S_OK;
}

int CScripting::ReportError(IActiveScriptError *pse,bool bDebug)
{
	EXCEPINFO ei;
	BSTR      bstrLine = NULL;
	HRESULT   hr;
	DWORD ctx;
	ULONG lineNum;
	LONG posNum;
	hr=pse->GetSourceLineText(&bstrLine);
	pse->GetSourcePosition(&ctx,&lineNum,&posNum);
	CString line(bstrLine);
	::SysFreeString(bstrLine);
	pse->GetExceptionInfo(&ei);
	CString source(ei.bstrSource);
	CString descr(ei.bstrDescription);
	CString msg;
	msg.Format("Ошибка выполнения скрипта.\n"
		"Скрипт:   %s\n"
		"Файл:     %s\n"
		"Строка:   %i\n"
		"Позиция:  %i\n"
		"Код:      %i\n"
		"Источник: %s\n"
		"Описание: %s",
		m_ScriptName,m_FileName,lineNum,posNum,ei.wCode,source,descr);
	if(!line.IsEmpty())
		msg=msg+"\nИсходный код: "+line;
	UINT mbType=MB_OK;
	if(bDebug)
	{
		msg+="\nЗапустить отладчик скриптов?";
		mbType=MB_YESNO;
	}
	return AfxMessageBox(msg,mbType);
}

STDMETHODIMP CScripting::OnScriptError(IActiveScriptError *pse)
{
	ReportError(pse);
	return S_OK;
}

bool CScripting::LoadScript(const CString file)
{
	CString FileText;
	// Open file
	try{
		CFile f(file,CFile::modeRead|CFile::shareDenyWrite);
		int len=f.GetLength();
		f.Read(FileText.GetBufferSetLength(len),len);
		FileText.ReleaseBuffer();
	}catch(CFileException* pEx)
	{
		char buf[1024];
		pEx->GetErrorMessage(buf,1024);
		pEx->Delete();
		DoMsgLine("Ошибка загрузки скрипта\r\n%s\r\n%s  (%i)",mmRedErr,file,buf,pEx->m_lOsError);
		return false;
	}
	m_FileName=file;
	// Find Name of Scripting Engine
	LPCTSTR pStartText=FileText;
	ExtractEngineAndName(pStartText,m_EngineName,m_ScriptName);

	if(m_EngineName.IsEmpty())
	{
		DoMsgLine("Скрипт %s\r\nНе удалось получить имя скриптового движка.",mmRedErr,m_FileName);
		return false;
	}
	if(m_ScriptName.IsEmpty())
	{
		DoMsgLine("Скрипт %s\r\nНе удалось получить имя скрипта",mmRedErr,m_FileName);
		return false;
	}
	// Create Engine
	if(!CreateScript(pStartText))
		return false;
	return true;
}

bool CScripting::Unload()
{
	if(m_pEngine!=NULL)
	{
		try{
			if(m_EnterCount)
			{
				m_pEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
				m_DelayUnReload=1;
				return false;
			}
			m_pEngine->SetScriptState(SCRIPTSTATE_CLOSED);		
			m_pEngine=NULL;
		}
		catch(_com_error err)
		{
			DoMsgLine(err.ErrorMessage());
		}
		catch(...)
		{
			DoMsgLine("Error Unload Script");
		}
	}
	return true;
}

STDMETHODIMP CScripting::GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,
		IUnknown**  ppunkItemOut,ITypeInfo** pptinfoOut)
{
	return CScriptManager::GetScriptManager()->GetItemInfo(this,pstrName,dwReturnMask,
		ppunkItemOut,pptinfoOut);
}

/*
STDMETHODIMP CScripting::GetDocVersionString(BSTR *pbstrVersion)
{
	return E_NOTIMPL;   // UNDONE: Implement this method
}

STDMETHODIMP CScripting::OnScriptTerminate
	(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo)
{
	return S_OK;
}

  STDMETHODIMP CScripting::OnStateChange(SCRIPTSTATE ssScriptState)
{
  // Don't care about notification
	return S_OK;
}

STDMETHODIMP CScripting::OnEnterScript(void)
{
	return S_OK;
}
STDMETHODIMP CScripting::OnLeaveScript(void)
{
	return S_OK;
}
*/
bool CScripting::CreateScript(LPCTSTR text)
{
	//tracing purposes only
	HRESULT hr = S_OK;
	//Now we need to set up for Smart Debugging.  First, we need a Process Debug 
	//Manager.
	bool IsDebugInit=false;
	CScriptManager* pSM=CScriptManager::GetScriptManager();
	if(pSM->m_pProcDebugManager)
	{
		//Create a new DebugDocumentHelper to use with the script added by the 
		//simple host.
		if(m_pDebugDocumentHelper)
		{
			m_pDebugDocumentHelper->Detach();
			m_pDebugDocumentHelper->Release();
			m_pDebugDocumentHelper=NULL;
		}
		hr=pSM->m_pProcDebugManager->CreateDebugDocumentHelper(NULL,
			&m_pDebugDocumentHelper);
		if(hr==S_OK)
		{
			//Initialize the DebugDocumentHelper so it's ready to handle the script.
			hr=m_pDebugDocumentHelper->Init(pSM->m_pDebugApplication,
				_bstr_t(m_ScriptName),_bstr_t(m_FileName),TEXT_DOC_ATTR_READONLY);
			if(hr==S_OK)
			{
				//Attach our DebugDocumentHelper to the debugger application.  The NULL
				//parameter indicates that the DebugDocumentHelper should be at the root
				//of the debug tree.
				IsDebugInit=(m_pDebugDocumentHelper->Attach(NULL)==S_OK);
			}
		}
	}
	if(!IsDebugInit)
	{
		if(m_pDebugDocumentHelper)
		{
			m_pDebugDocumentHelper->Detach();
			m_pDebugDocumentHelper->Release();
			m_pDebugDocumentHelper=NULL;
		}
	}

	try{
		hr=m_pEngine.CreateInstance(m_EngineName,NULL,CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER);
		if(hr)
		{
			DoMsgLine("Скрипт %s\r\nНе удалось создать экземпляр скриптового движка %s\r\n%s",
				mmRedErr,m_FileName,m_EngineName,GetErrorDescription(hr));
			return false;
		}
		// Script Engine must support IActiveScriptParse for us to use it
		IActiveScriptParsePtr pParse=m_pEngine;
		if(pParse==NULL)
		{
			DoMsgLine("Скрипт %s\r\nДвижок скрипта %s не поддерживает загрузку текста.",mmRedErr,m_FileName,m_EngineName);
			return false;
		}
		bool IsInit=true;
		hr=pParse->InitNew();
		if(hr)
			IsInit=false;
		if(IsInit)
		{
			hr=m_pEngine->SetScriptSite(this);
			if(hr)
				IsInit=false;
		}
		//_bstr_t bstrName=m_ScriptName;
		//hr=m_pEngine->AddNamedItem(bstrName,SCRIPTITEM_ISVISIBLE|SCRIPTITEM_CODEONLY|SCRIPTITEM_GLOBALMEMBERS);
		if(IsInit)
		{
			hr=m_pEngine->AddTypeLib(LIBID_OpenConf,TLIB_MAJOR,TLIB_MINOR,0);
			if(hr)
				IsInit=false;
		}
		if(IsInit)
		{
			_bstr_t bstrSrc=text;
			hr=pParse->ParseScriptText(bstrSrc,NULL,NULL,NULL,0,0,SCRIPTTEXT_ISVISIBLE,NULL,NULL);
			if(hr)
				IsInit=false;
		}
		if(!IsInit)
		{
			DoMsgLine("Скрипт %s\r\nНе удалось инициализировать движок скрипта.",mmRedErr,m_FileName);
			return false;
		}
	}catch(_com_error err)
	{
		MessageBox(NULL,err.ErrorMessage(),"OpenConf",MB_ICONERROR);
		return false;
	}
	return true;
}

bool CScripting::ExtractEngineAndName(LPCTSTR& text,CString& EngineName,CString& ScriptName)
{
	LPCTSTR pStartText=text;
	LPCTSTR pEngPos=strstr(pStartText,"$ENGINE");
	if(pEngPos==text)
	{
		pEngPos=strstr(text,"\r");
		EngineName=CString(text+8,pEngPos-text-8);
		text=pEngPos+2;
	}
	if(EngineName.IsEmpty())
	{
		int lastdec=m_FileName.ReverseFind('.');
		if(lastdec!=-1)
		{
			CString ext=m_FileName.Mid(lastdec);
			ext.MakeLower();
			if(!m_mapEngineNames.Lookup(ext,m_EngineName))
			{
				HKEY hKey;
				if(RegOpenKeyEx(HKEY_CLASSES_ROOT,ext,0,KEY_READ,&hKey)==ERROR_SUCCESS)
				{
					char type[MAX_PATH];
					DWORD size=MAX_PATH;
					if(RegQueryValueEx(hKey,NULL,0,NULL,(BYTE*)type,&size)==ERROR_SUCCESS)
					{
						strcat(type,"\\ScriptEngine");
						HKEY hSubKey;
						if(RegOpenKeyEx(HKEY_CLASSES_ROOT,type,0,KEY_READ,&hSubKey)==ERROR_SUCCESS)
						{
							size=MAX_PATH;
							if(RegQueryValueEx(hSubKey,NULL,0,NULL,(BYTE*)type,&size)==ERROR_SUCCESS)
								EngineName=type;
							RegCloseKey(hSubKey);
						}
					}
					RegCloseKey(hKey);
				}
				if(!EngineName.IsEmpty())
					m_mapEngineNames[ext]=EngineName;
			}
			else
				EngineName=m_mapEngineNames[ext];
		}
		if(EngineName.IsEmpty())
		{
			DoMsgLine("Скрипт %s\r\nНе удалось получить имя скриптового движка.",mmRedErr,m_FileName);
			return false;
		}
	}
	// Find Script Name
	LPCTSTR pNameScript=strstr(text,"$NAME");
	if(pNameScript==text)
	{
		pNameScript=strstr(text,"\r");
		ScriptName=CString(text+6,pNameScript-text-6);
		text=pNameScript+2;
	}
	else
	{
		LPCTSTR pDecPos=NULL;
		int lastdec=m_FileName.ReverseFind('.');
		if(lastdec!=-1)
			pDecPos=LPCTSTR(m_FileName)+lastdec-1;
		if(!pDecPos)
			pDecPos=(LPCTSTR)m_FileName+m_FileName.GetLength()-1;
		while(*pDecPos!='\\')
			ScriptName.Insert(0,*pDecPos--);
	}
	return true;
}

LPCTSTR CScripting::GetCurrentState()
{
	static const char* states[]={
		"Не инициализирован",
		"Стартует",
		"Работает",
		"Отсоединен",
		"Закрыт",
		"Инициализирован"
	};
	SCRIPTSTATE st;
	m_pEngine->GetScriptState(&st);
	return states[st];
}

bool CScripting::ReloadScript()
{
	if(m_EnterCount)
	{
		m_DelayUnReload=2;
		CScriptManager::GetScriptManager()->AddDelayed(this);
		return false;
	}
	DoStsLine("Загрузка скрипта %s ...",m_FileName);
	CString FileText;
	// Open file
	try{
		CFile f(m_FileName,CFile::modeRead);
		int len=f.GetLength();
		f.Read(FileText.GetBufferSetLength(len),len);
		FileText.ReleaseBuffer();
	}catch(CFileException* pEx)
	{
		char buf[1024];
		pEx->GetErrorMessage(buf,1024);
		pEx->Delete();
		DoMsgLine("Ошибка загрузки файла\r\n%s\r\n%s  (%i)",mmRedErr,m_FileName,buf,pEx->m_lOsError);
		return false;
	}
	Unload();		// Прерываем текущий скрипт
	m_pSelf->ResetAdded();
	CString name,engine;
	LPCTSTR pRead=FileText;
	ExtractEngineAndName(pRead,engine,name);	// При перезагрузке имя скрипта и движок не изменяются
	bool ret=CreateScript(pRead);
	if(ret)
		CScriptManager::GetScriptManager()->ConnectScript(this);
	DoStsLine("");
	m_DelayUnReload=0;
	if(m_pViewItem)
		m_pViewItem->FillMacros();
	return ret;
}



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
STDMETHODIMP CScripting::GetDocumentContextFromPosition(
		DWORD dwSourceContext,
		ULONG uCharacterOffset,
		ULONG uNumChars,
		IDebugDocumentContext **ppsc)
{
	HRESULT hr = E_FAIL;
	//Use the IDebugDocumentHelper for this CScripting to delegate this
	//call
	if(m_pDebugDocumentHelper)
	{
		ULONG ulStartPos;
		//Find out where this source context starts.
		hr = m_pDebugDocumentHelper->GetScriptBlockInfo(dwSourceContext,NULL, 
			&ulStartPos,NULL);
		//If that succeeded, then create an IDebugDocumentContext
		if(SUCCEEDED(hr))
		{
			hr=m_pDebugDocumentHelper->CreateDebugDocumentContext(
				ulStartPos+uCharacterOffset,uNumChars,ppsc);
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
STDMETHODIMP CScripting::GetApplication(IDebugApplication **ppda)
{
	HRESULT hr=E_FAIL;
	//if we were successful in creating an IDebugApplication in Initialize,
	//then return it here.
	if(CScriptManager::GetScriptManager()->m_pDebugApplication)
	{
		hr=CScriptManager::GetScriptManager()->m_pDebugApplication->QueryInterface(
			IID_IDebugApplication,(void**)ppda);
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
STDMETHODIMP CScripting::GetRootApplicationNode(
                                          IDebugApplicationNode **ppdanRoot)
{
	HRESULT hr=E_FAIL;
	//If we were successful in creating an IDebugApplication in Initialize,
	//then call GetRootNode on it here.
	if(m_pDebugDocumentHelper)
	{
		hr=m_pDebugDocumentHelper->GetDebugApplicationNode(ppdanRoot);
		//hr=m_pDebugApplication->GetRootNode(ppdanRoot);
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
STDMETHODIMP CScripting::OnScriptErrorDebug(
                                      IActiveScriptErrorDebug *pErrorDebug,
                                      BOOL *pfEnterDebugger,
                                      BOOL *pfCallOnScriptErrorWhenContinuing)
{
	*pfCallOnScriptErrorWhenContinuing = true;
	*pfEnterDebugger = true;
	IActiveScriptError* pError=NULL;
	HRESULT hr=pErrorDebug->QueryInterface(IID_IActiveScriptError,(void**)&pError);
	if(hr==S_OK && pError)
	{
		*pfEnterDebugger=ReportError(pError,true)==IDYES;
		pError->Release();
	}
	return S_OK;
}

