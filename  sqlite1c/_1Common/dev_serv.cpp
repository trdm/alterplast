#include "stdafx.h"
#include "dev_serv.h"

void DoMsgLine(LPCSTR format, MessageMarker marker/*=mmNone*/,...)
{
	va_list arg;
	va_start(arg,marker);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoMessageLine(txt,marker);
}

void GetRTCFull(CRuntimeClass* rtc, CString& Str)
{
    if (rtc)
    {
        Str.Format("%s%s%s(%Xh/%Xh)", Str, "<-", rtc->m_lpszClassName, rtc->m_nObjectSize, rtc->m_nObjectSize - (rtc->m_pfnGetBaseClass() ? rtc->m_pfnGetBaseClass()->m_nObjectSize : 0));
        GetRTCFull(rtc->m_pfnGetBaseClass(), Str);
    }
}

void MessageFullRTC(CObject* obj, CString Indent/*=""*/)
{
        if (obj)
			MessageFullRTC(obj->GetRuntimeClass(), Indent);
}

void MessageFullRTC(CRuntimeClass* rtc, CString Indent/*=""*/)
{
        if (rtc)
        {
                CString Str;
                GetRTCFull(rtc, Str);
                DoMsgLine("%s%s", mmNone, Indent, Str);
        }
}

void TraceProfile(CProfile7* pProf, CString Indent)
{
        if (!pProf)
        {
                CApp7* pApp7 = (CApp7*)AfxGetApp();
                pProf = pApp7->GetProps();
        }

        DoMsgLine("%s%s # %s # %d", mmNone, Indent, pProf->m_strKey, pProf->m_strPath, pProf->GetEntryCount());
        CString str;
        for (int i=0; i<pProf->GetEntryCount(); i++)
        {
                ((CProfile7_*)pProf)->PropToString(i, str);
                const CProfileEntry7* pPE = pProf->GetEntryAt(i);
                DoMsgLine("%s%s[%d] %d %s: %s", mmNone," ",Indent, i, pPE->type, pPE->EntryName, str);
        }

        POSITION pos = pProf->GetFirstProfile();
        while(pos)
                TraceProfile(pProf->GetNextProfile(pos), Indent+"  ");
}

void TraceWindows(CWnd* pWnd, BOOL bWithMsgMap, UINT nMessageFilter, UINT nIDFilter, CString Indent)
{
        CString Str;
        GetRTCFull(pWnd->GetRuntimeClass(), Str);
        DoMsgLine("%s(%X)%s", mmNone, Indent, pWnd->m_hWnd, Str);
		
		if (bWithMsgMap)
			TraceMessageMap(pWnd, nMessageFilter, nIDFilter,Indent);

		if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			CView* pView = (CView*)pWnd;
			Str.Empty();
			GetRTCFull(pView->GetParentFrame()->GetRuntimeClass(), Str);
	        DoMsgLine("%s(%X)(Frame:%s)", mmNone, Indent, pView->GetParentFrame()->m_hWnd, Str);
			Str.Empty();
			GetRTCFull(pView->GetDocument()->GetRuntimeClass(), Str);
	        DoMsgLine("%s(Document:%s)", mmNone, Indent, Str);
			if (bWithMsgMap)
				TraceMessageMap(pView->GetDocument(), nMessageFilter, nIDFilter, Indent);
		};
        for (pWnd = pWnd->GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetWindow(GW_HWNDNEXT))
                TraceWindows(pWnd, bWithMsgMap, nMessageFilter, nIDFilter, Indent+"  ");
}

void TraceMenu(CMenu* Menu, CString Indent)
{
        if (Menu)
        {
                CString str;
                MENUITEMINFO mii;
                for (UINT i=0; i < Menu->GetMenuItemCount(); i++)
                {
                        Menu->GetMenuString(i, str, MF_BYPOSITION);
                        DWORD id = Menu->GetMenuItemID(i);
                        Menu->GetMenuItemInfo(i, &mii, MF_BYPOSITION);
                        DoMsgLine("%s%08X %08X %s", mmNone, Indent, id, mii.fState, str);
                        TraceMenu(Menu->GetSubMenu(i), Indent+"  ");
                }
        }
}

void TraceMessageMap(const AFX_MSGMAP* pMSGMAP, UINT nMessageFilter, UINT nIDFilter, CString Indent)
{
	if (pMSGMAP)
	{
		const AFX_MSGMAP_ENTRY* lpEntries = pMSGMAP->lpEntries;
		while (lpEntries->nMessage)
		{
			if (!nMessageFilter || nMessageFilter == lpEntries->nMessage)
			{
				if (!nIDFilter || nIDFilter>=lpEntries->nID && nIDFilter<=lpEntries->nLastID)
				{
					DoMsgLine("%sMessage:%08X Code:%08X ID:%08X LastID:%08X Sig:%08X", mmNone, Indent,
						lpEntries->nMessage, lpEntries->nCode, lpEntries->nID, lpEntries->nLastID, lpEntries->nSig);
				}
			}
			lpEntries++;
		}
		TraceMessageMap(pMSGMAP->pfnGetBaseMap(), nMessageFilter, nIDFilter, Indent+"  ");
	}
}

void TraceMessageMap(CCmdTarget* pTarget, UINT nMessageFilter, UINT nIDFilter, CString Indent)
{
	class CCmdTarget_ : public CCmdTarget
	{
		friend void TraceMessageMap(CCmdTarget* pTarget, UINT nMessageFilter, UINT nIDFilter, CString Indent);
	};
	if (pTarget)
		TraceMessageMap(((CCmdTarget_*)pTarget)->GetMessageMap(), nMessageFilter, nIDFilter, Indent);
}


void TraceBLContext(CBLContext* pBLC, CString Indent)
{
	MessageFullRTC(pBLC, Indent);
	Indent += "  ";
	DoMsgLine("%sNProps:%d NMethods:%d", mmNone,Indent, pBLC->GetNProps(), pBLC->GetNMethods());
	int i;
	for (i = 0; i < pBLC->GetNProps(); i++)
	{
		DoMsgLine("%s%02d %s%s %s/%s", mmNone,
			Indent+"  ",
			i,
			pBLC->IsPropReadable(i)?"r":"",
			pBLC->IsPropWritable(i)?"w":"",
			pBLC->GetPropName(i,0),
			pBLC->GetPropName(i,1)
			);
	}
	for (i = 0; i < pBLC->GetNMethods(); i++)
	{
		DoMsgLine("%s%02d %s %s/%s(%d)", mmNone,
			Indent+"  ",
			i,
			pBLC->HasRetVal(i)?"=":" ",
			pBLC->GetMethodName(i,0),
			pBLC->GetMethodName(i,1),
			pBLC->GetNParams(i)
			);
	}
}

void TraceGC(CBLContext* pBLC, CString Indent, BOOL Detail)
{
	if (pBLC)
	{
		CRuntimeClass* pRTC = pBLC->GetRuntimeClass();
		if (pRTC == RUNTIME_CLASS(CGroupContext) || !strcmp("CAutoGroupContext", pBLC->GetRuntimeClass()->m_lpszClassName))
		{
			CGroupContext* pGC = (CGroupContext*)pBLC;
			MessageFullRTC(pBLC, Indent);
			Indent += "  ";
			DoMsgLine("%sNContexts:%d NProps:%d NMethods:%d", mmNone,Indent, pGC->GetSize(), pGC->GetNProps(), pGC->GetNMethods());
			for(int i = 0; Detail && i < pGC->GetSize(); i++)
			{
					TraceBLContext(pGC->GetContext(i), Indent+"  ");
			}
		}
	}
}

CBLContext* GetContFromGC(CGroupContext* pGC, CRuntimeClass* pRTC)
{
	if (pGC)
	{
		if (pGC->IsKindOf(RUNTIME_CLASS(CGroupContext)))
		{
			for(int i = 0; i < pGC->GetSize(); i++)
			{
				CBLContext* pCont = pGC->GetContext(i);
				if (pCont->GetRuntimeClass() == pRTC)
				{
					return pCont;
				}
			}
		}
	}
	return NULL;
}

CBLContext* GetContFromGC(CGroupContext* pGC, CString RTCName)
{
	if (pGC)
	{
		if (pGC->IsKindOf(RUNTIME_CLASS(CGroupContext)))
		{
			for(int i = 0; i < pGC->GetSize(); i++)
			{
				CBLContext* pCont = pGC->GetContext(i);
				if (!strcmp(RTCName, pCont->GetRuntimeClass()->m_lpszClassName))
				{
					return pCont;
				}
			}
		}
	}
	return NULL;
}

void FormatMessageAndRuntimeError(DWORD dwMessageId)
{
	LPVOID lpMsgBuf;
	DWORD res = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		dwMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	CString strErr;
	if (res)
		strErr = (LPTSTR)lpMsgBuf;
	else
		strErr.Format("Неизвестная ошибка: 0x%08X", dwMessageId);
	LocalFree(lpMsgBuf);
	RuntimeError(strErr);
}

#pragma comment(lib, "version.lib")

CString GetModuleVersionInfo(HMODULE hModule, CString VerInfoName)
//VerInfoName: FileVersion, PrivateBuild, etc
{
	CString res;

	char* fname = new char[MAX_FNAME_LEN];
	GetModuleFileName(hModule, fname, MAX_FNAME_LEN);

	DWORD size, z;
	size = ::GetFileVersionInfoSize(fname, &z);

	if(size)
	{
		static const char* EngNames[]={
			"FileVersion",
			"PrivateBuild",
		};
		char* pVersion=new char[size];
		GetFileVersionInfo(fname, 0, size, pVersion);

		WORD* translate;
		UINT len;
		VerQueryValue(pVersion, "\\VarFileInfo\\Translation", (void**)&translate, &len);
		CString blockName;
		blockName.Format("\\StringFileInfo\\%04x%04x\\%s",translate[0], translate[1], VerInfoName);

		LPCTSTR valuebuf;
		VerQueryValue(pVersion, (char*)(LPCTSTR)(blockName), (void**)&valuebuf, &len);
		res = valuebuf;

		delete pVersion;
	}
	delete fname;

	return res;
}

CString GetModuleVersionInfo(CString ModuleName, CString VerInfoName)
//VerInfoName: FileVersion, PrivateBuild, etc
{
	return GetModuleVersionInfo(GetModuleHandle(ModuleName), VerInfoName);
}

