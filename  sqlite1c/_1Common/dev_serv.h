#ifndef FORM_COMMON_H
#define FORM_COMMON_H

#pragma once


class CProfile7_ : public CProfile7
{
public:
	void PropToString(int i, CString &s)
	{
		CProfile7::PropToString(i, s);
	};
	void PropFromString(int i, char const * s)
	{
		CProfile7::PropFromString(i, s);
	};
};

class CBLContextExt : public CBLContext
{
public:
	void GetPropValByName(LPCSTR RuName, LPCSTR EnName, CValue &val)
	{
		int nProp;
		if (0 <= (nProp = FindProp(RuName)))
			GetPropVal(nProp, val);
		else if (0 <= (nProp = FindProp(EnName)))
			GetPropVal(nProp, val);
		else
			val.Reset();
	};
};

void DoMsgLine(LPCSTR format, MessageMarker marker = mmNone, ...);

#define MSG_CLASS_SIZE(class_name)\
DoMsgLine("size of %s is %Xh", mmNone, #class_name, sizeof class_name)

void GetRTCFull(CRuntimeClass* rtc, CString& Str);
void MessageFullRTC(CObject* obj, CString Indent="");
void MessageFullRTC(CRuntimeClass* rtc, CString Indent="");

void TraceProfile(CProfile7* pProf=NULL, CString Indent="");
void TraceWindows(CWnd* pWnd, BOOL bWithMsgMap = FALSE, UINT nMessageFilter=0, UINT nIDFilter=0, CString Indent="");
void TraceMenu(CMenu* Menu, CString Indent="");
void TraceBLContext(CBLContext* pBLC, CString Indent="");
void TraceGC(CBLContext* pBLC, CString Indent="", BOOL Detail=TRUE);
void TraceMessageMap(const AFX_MSGMAP* pMSGMAP, UINT nMessageFilter=0, UINT nIDFilter=0, CString Indent="");
void TraceMessageMap(CCmdTarget* pTarget, UINT nMessageFilter=0, UINT nIDFilter=0, CString Indent="");
CBLContext* GetContFromGC(CGroupContext* pGC, CRuntimeClass* pRTC);
CBLContext* GetContFromGC(CGroupContext* pGC, CString RTCName);

void FormatMessageAndRuntimeError(DWORD dwMessageId);

CString GetModuleVersionInfo(HMODULE hModule, CString VerInfoName);
CString GetModuleVersionInfo(CString ModuleName, CString VerInfoName);

#endif
