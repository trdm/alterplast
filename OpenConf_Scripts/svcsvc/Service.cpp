// Service.cpp : Implementation of CService
#include "stdafx.h"
#include "Svcsvc.h"
#include "Service.h"
#include <afxdlgs.h>
#include "selectdlg.h"
#include "FilterDlg.h"

STDMETHODIMP CService::AnsiToOEM(BSTR Ansi, BSTR *OEM)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	CString txt=(LPCTSTR)_bstr_t(Ansi);
	txt.AnsiToOem();
	*OEM=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CService::OEMToAnsi(BSTR OEM, BSTR *Ansi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString txt=(LPCTSTR)_bstr_t(OEM);
	txt.OemToAnsi();
	*Ansi=txt.AllocSysString();
	return S_OK;
}

int CALLBACK BrowseCallbackProc(
    HWND hwnd, 
    UINT uMsg, 
    LPARAM lParam, 
    LPARAM lpData
    )
{
	if(uMsg==BFFM_INITIALIZED && lpData)
		SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
	return 0;
}


STDMETHODIMP CService::SelectFolder(BSTR Caption,BSTR StartFolder, long Flags,
									BSTR Root, BSTR *FolderPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(IsBadWritePtr(FolderPath,sizeof(*FolderPath)))
		return E_POINTER;

	CString retPath;
	CString cap=(LPCTSTR)_bstr_t(Caption),
		start=(LPCTSTR)_bstr_t(StartFolder);
	if(cap.IsEmpty())
		cap="Выберите значение";

	char buf[MAX_PATH];
	BROWSEINFO bInfo;
	bInfo.hwndOwner=NULL;
	bInfo.pidlRoot=NULL;
	if(Root)
	{
		IShellFolder* iShell;
		SHGetDesktopFolder(&iShell);
		ULONG num,atr;
		LPITEMIDLIST idl=NULL;
		if(S_OK==iShell->ParseDisplayName(NULL,NULL,Root,&num,&idl,&atr))
			bInfo.pidlRoot=idl;
		iShell->Release();
	}
	bInfo.pszDisplayName=buf;
	bInfo.lpszTitle=cap;
	bInfo.ulFlags=(ULONG)Flags;
	bInfo.iImage=0;
	bInfo.lpfn=BrowseCallbackProc;
	bInfo.lParam=(LPARAM)(LPCTSTR)start;
	LPITEMIDLIST lid=SHBrowseForFolder(&bInfo);
	if(lid)
	{
		SHGetPathFromIDList(lid,retPath.GetBuffer(MAX_PATH));
		retPath.ReleaseBuffer();
	}
	if(bInfo.pidlRoot)
	{
		LPMALLOC pMalloc;
		SHGetMalloc(&pMalloc);
		pMalloc->Free((void*)bInfo.pidlRoot);
		pMalloc->Release();
	}
	*FolderPath=retPath.AllocSysString();
	return S_OK;
}

STDMETHODIMP CService::SelectFile(VARIANT_BOOL ForSave, BSTR FileName,
								  BSTR Filter, VARIANT_BOOL MultiSelect,BSTR *FilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString fName=(LPCTSTR)_bstr_t(FileName),
		filter=(LPCTSTR)_bstr_t(Filter),
		ret;
	if(!filter.IsEmpty())
	{
		if(filter.Right(2)!="||")
			filter+="||";
	}
	CFileDialog dlg(ForSave==VARIANT_FALSE,NULL,fName,MultiSelect?OFN_ALLOWMULTISELECT:0,filter);
	
	if (MultiSelect) //[+]metaeditor
	{
		dlg.m_ofn.nMaxFile = 65535;
		dlg.m_ofn.lpstrFile = new char[65535];
		memset(dlg.m_ofn.lpstrFile,0,65535);
		lstrcpyn(dlg.m_ofn.lpstrFile,fName,fName.GetLength()+1);
	}
	
	if(dlg.DoModal()==IDOK)
	{
		if(MultiSelect)
		{
			for(POSITION pos=dlg.GetStartPosition();pos;)
			{
				CString txt=dlg.GetNextPathName(pos);
				if(!ret.IsEmpty())
					ret+="\r\n";
				ret+=txt;
			}
			delete [] dlg.m_ofn.lpstrFile;
		}
		else
			ret=dlg.GetPathName();
	}
	*FilePath=ret.AllocSysString();
	return S_OK;
}

STDMETHODIMP CService::FileA2O(BSTR Path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString path=(LPCTSTR)_bstr_t(Path);
	try{
		CFile file(path,CFile::modeRead);
		int size=file.GetLength();
		CString txt;
		file.Read(txt.GetBuffer(size),size);
		file.Close();
		txt.ReleaseBuffer();
		txt.AnsiToOem();
		CFile out(path,CFile::modeWrite);
		out.Write((LPCTSTR)txt,size);
		out.Close();
	}
	catch(CFileException* e)
	{
		e->Delete();
	}
	return S_OK;
}

STDMETHODIMP CService::FileO2A(BSTR Path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString path=(LPCTSTR)_bstr_t(Path);
	try{
		CFile file(path,CFile::modeRead);
		int size=file.GetLength();
		CString txt;
		file.Read(txt.GetBuffer(size),size);
		file.Close();
		txt.ReleaseBuffer();
		txt.OemToAnsi();
		CFile out(path,CFile::modeWrite);
		out.Write((LPCTSTR)txt,size);
		out.Close();
	}
	catch(CFileException* e)
	{
		e->Delete();
	}
	return S_OK;
}

STDMETHODIMP CService::SelectValue(BSTR Values, BSTR Caption,
								   VARIANT_BOOL Mark, BSTR *Select)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	CSelectDlg::DoSelect(Caption,Values,Select,Mark?1:0);
	return S_OK;
}

STDMETHODIMP CService::SelectInTree(BSTR Values, BSTR Caption, VARIANT_BOOL Mark,
									VARIANT_BOOL OnlyItems, BSTR *Select)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CSelectDlg::DoSelect(Caption,Values,Select,Mark?4:(OnlyItems==VARIANT_TRUE?2:3));
	return S_OK;
}

static CStringArray strKeys;
static void ProcessMenu(HMENU menu,LPCTSTR& ptr,int& level,int& count)
{
	int newLevel;
	while(*ptr)
	{
		LPCTSTR pStart=ptr;
		DWORD flag=0;
		while(*ptr && *ptr!='\r' && *ptr!='\n')
		{
			if(*ptr=='|')
			{
				if(ptr[1]=='|')
					ptr++;
				else
					break;
			}
			ptr++;
		}
		CString name(pStart,ptr-pStart);
		CString key=name;
		name.Replace("||","|");
		if(*ptr=='|')
		{
			ptr++;
			while(*ptr && *ptr!='\r' && *ptr!='\n' && *ptr!='|')
			{
				if(*ptr=='c' || *ptr=='C')
					flag|=MF_CHECKED;
				else if(*ptr=='d' || *ptr=='D')
					flag|=MF_GRAYED;
				ptr++;
			}
			if(*ptr=='|')
			{
				pStart=++ptr;
				while(*ptr && *ptr!='\r' && *ptr!='\n')
					ptr++;
				key=CString(pStart,ptr-pStart);
			}
		}
		while(*ptr && (*ptr=='\r' || *ptr=='\n'))
			ptr++;
		if(*ptr)
		{
			newLevel=0;
			while(*ptr && *ptr=='\t')
				*ptr++,newLevel++;
		}
		else
			newLevel=level;
		if(newLevel>level)
		{
			HMENU hNewMenu=CreatePopupMenu();
			ProcessMenu(hNewMenu,ptr,newLevel,count);
			AppendMenu(menu,MF_POPUP|MF_STRING,(UINT)hNewMenu,name);
		}
		else
		{
			if(name=="-")
				AppendMenu(menu,MF_SEPARATOR,0,NULL);
			else
			{
				AppendMenu(menu,MF_STRING|flag,++count,name);
				strKeys.Add(key);
			}
		}
		if(newLevel<level)
			break;
	}
	level=newLevel;
}

STDMETHODIMP CService::PopupMenu(BSTR Values, short ShowIn, long X, long Y, BSTR *Select)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString vals=(LPCTSTR)_bstr_t(Values), ret;
	strKeys.RemoveAll();
	CPoint pt;
	vals.TrimLeft();
	vals.TrimRight();
	if(!vals.IsEmpty())
	{
		if(0==ShowIn)
			GetCursorPos(&pt);
		else if(1==ShowIn)
		{
			if(GetCaretPos(&pt))
			{
				HWND hFocus=GetFocus();
				ClientToScreen(hFocus,&pt);
				HDC dc=GetDC(hFocus);
				CSize s;
				GetTextExtentPoint(dc," ",1,&s);
				pt.y+=s.cy;
				ReleaseDC(hFocus,dc);
			}
		}
		else
		{
			pt.x=X;
			pt.y=Y;
		}
		int level=0;
		int count=0;
		LPCTSTR ptr=vals;
		HMENU hMenu=CreatePopupMenu();
		ProcessMenu(hMenu,ptr,level,count);
		HWND hWnd=CreateWindow("EDIT",NULL,WS_OVERLAPPED,0,0,0,0,NULL,NULL,_Module.m_hInst,NULL);
		SetForegroundWindow(hWnd);
		int cmd=(int)TrackPopupMenu(
			hMenu,
			TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON,
			pt.x,pt.y,
			0,
			hWnd,
			NULL);
		if(cmd)
			ret=strKeys[cmd-1];
		DestroyMenu(hMenu);
		DestroyWindow(hWnd);
	}
	strKeys.RemoveAll();
	*Select=ret.AllocSysString();
	return S_OK;
}

STDMETHODIMP CService::FilterValue(BSTR Values, short Flags, BSTR Caption,long X, long Y, long W, long H, BSTR *Result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		if(IsBadWritePtr(Result,sizeof(*Result)))
			return E_POINTER;
		*Result=CFilterDlg::FilterValue(Values,Caption,Flags,X,Y,W,H).AllocSysString();
		return S_OK;
}

STDMETHODIMP CService::SetImageList(BSTR Values, BSTR *Result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		if(IsBadWritePtr(Result,sizeof(*Result)))
			return E_POINTER;
		*Result=CFilterDlg::SetImageList(Values).AllocSysString();
		return S_OK;
}

STDMETHODIMP CService::GetWindowText(long WH, VARIANT_BOOL IsList, BSTR *Result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		if(IsBadWritePtr(Result,sizeof(*Result)))
			return E_POINTER;
		
		HWND h = WH==0 ?  CFilterDlg::GetAnyFocus() : (HWND)WH;
		*Result=CFilterDlg::GetWndText(h,IsList?1:0).AllocSysString();
		return S_OK;
}

STDMETHODIMP CService::SetWindowText(long WH, BSTR Text)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HWND h = WH==0 ? CFilterDlg::GetAnyFocus() : (HWND)WH;
	_bstr_t bstrt = _bstr_t(Text);
	::SendMessageA(h,WM_SETTEXT,0,(long)(char*)bstrt);
	return S_OK;
}

/*
Номер версии определяется автоматически из ресурсов модуля.
При этом int64 (полное представление версии файла) пакуется в десятичное число,
в котором каждый разряд поля версии представлен одним десятичным разрядами

В силу примененной логики преобразования не рекомендуется в описании
ресурса версии применять числа более 9 для каждого поля версии.
*/
STDMETHODIMP CService::get_Version(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long nVer = 0;

	TCHAR szFileName[1024];
	GetModuleFileName(_Module.m_hInst, szFileName, 1024);

	DWORD dwHandle;
	DWORD dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);

    if(dwLen)
    {
        char* pVersion = new char[dwLen];
        if (GetFileVersionInfo(szFileName, NULL, dwLen, pVersion))
		{
	        VS_FIXEDFILEINFO *pInfo;
		    if (VerQueryValue(pVersion, "\\", (void**)&pInfo, (UINT*)&dwLen))
				nVer = LOWORD(pInfo->dwFileVersionLS)
				+ HIWORD(pInfo->dwFileVersionLS) *10
				+ LOWORD(pInfo->dwFileVersionMS) *100
				+ HIWORD(pInfo->dwFileVersionMS) *1000;
		}
        delete[] pVersion;
	}

	*pVal = nVer;
	return S_OK;
}
