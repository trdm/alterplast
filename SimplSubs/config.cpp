// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IConfigurator properties

/////////////////////////////////////////////////////////////////////////////
// IConfigurator operations

CString IConfigurator::GetIBName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IConfigurator::SetIBName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IConfigurator::GetIBDir()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IConfigurator::GetBinDir()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IConfigurator::GetCommandLine()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IConfigurator::Message(LPCTSTR msgLine, long marker)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 msgLine, marker);
}

void IConfigurator::Status(LPCTSTR text)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 text);
}

LPDISPATCH IConfigurator::OpenOLEForm(LPCTSTR progID, LPCTSTR Title)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		progID, Title);
	return result;
}

long IConfigurator::GetVersion()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IConfigurator::GetMetaData()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IConfigurator::SendCommand(long cmd)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 cmd);
}

BOOL IConfigurator::IsConfigWndOpen()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IConfigurator::GetPlugins()
{
	LPDISPATCH result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IConfigurator::GetScripts()
{
	LPDISPATCH result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IConfigurator::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IConfigurator::Quit(BOOL PromptSave)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PromptSave);
}

void IConfigurator::LogMessageWrite(long EventCategory, LPCTSTR Message, LPCTSTR strEventType, LPCTSTR Event)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 EventCategory, Message, strEventType, Event);
}

void IConfigurator::Run1CApp(long mode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

LPDISPATCH IConfigurator::GetDocuments()
{
	LPDISPATCH result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IConfigurator::GetCfgTimer()
{
	LPDISPATCH result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IConfigurator::GetAppProps(long idx)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		idx);
	return result;
}
