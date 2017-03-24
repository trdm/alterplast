// Silenser.cpp: implementation of the CSilenser class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "iconfig.h"
#include "Silenser.h"

CSilenser::PAfxMessageBoxT CSilenser::m_pAfxMsgBoxT;
CSilenser::PDoModal	 CSilenser::m_pFDlgDoModal;
CSilenser::PDoModal  CSilenser::m_pDlgDoModal;

static DWORD jmpMsgBoxT;
static DWORD jmpDlgDoModal;
static DWORD jmpFDlgDoModal;
static BYTE ambTbuf[6]="\xFF\x25";
static BYTE dlgdmd[6] ="\xFF\x25";
static BYTE fdlgdmd[6]="\xFF\x25";

static CSilenser::PDoModal pInitDlg;

static void SwapCode(BYTE* pRead,BYTE* pWrite)
{
	char tmp;
	for(int i=0;i<6;i++)
		tmp=*pRead,*pRead++=*pWrite,*pWrite++=tmp;
}
int AFXAPI CSilenser::AfxMessageBoxT(LPCTSTR text,UINT style,UINT help)
{
	int ret=CIConfigEvents::m_ConfigEvents.FireMsgBox(text,style);
	if(!ret)
	{
		SwapCode(ambTbuf,(BYTE*)m_pAfxMsgBoxT);
		ret=AfxMessageBox(text,style,help);
		SwapCode(ambTbuf,(BYTE*)m_pAfxMsgBoxT);
	}
	return ret;
}

int CSilenser::DoModalFileDialog()
{
	CString fName=m_ofn.lpstrFile;
	LPCTSTR lpFilter=m_ofn.lpstrFilter;
	CString strFilter;
	int count=0;
	if(lpFilter)
	{
		while(*lpFilter)
		{
			do
				count++;
			while(*lpFilter++);
		}
		LPSTR pWrite=strFilter.GetBuffer(count+1);
		lpFilter=m_ofn.lpstrFilter;
		while(*lpFilter)
		{
			do
				*pWrite++=*lpFilter++;
			while(*lpFilter);
			*pWrite++='|';
			*lpFilter++;
		}
		*pWrite++='|';
		strFilter.ReleaseBuffer(count);
	}
	int ret=CIConfigEvents::m_ConfigEvents.FireFileDialog(
		!m_bOpenFileDialog,m_ofn.lpstrTitle,strFilter,fName);
	if(!ret)
	{
		//if(!(m_ofn.Flags & OFN_EXPLORER))
		//	m_ofn.Flags&=~OFN_ENABLEHOOK;
		//m_ofn.Flags=/*OFN_ENABLESIZING|*/OFN_EXPLORER|OFN_HIDEREADONLY;
		SwapCode(fdlgdmd,*(BYTE**)&m_pFDlgDoModal);
		ret=DoModal();
		SwapCode(fdlgdmd,*(BYTE**)&m_pFDlgDoModal);
		//m_ofn.Flags=OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY;
		//m_ofn.lpfnHook=NULL;
		//GetOpenFileName(&m_ofn);
	}
	else 
	{
		if(ret==IDOK)
		{
			strcpy(m_ofn.lpstrFile,fName);
			m_ofn.lpstrCustomFilter=NULL;
			m_ofn.nFilterIndex=0;
			m_ofn.nFileExtension=fName.ReverseFind('.');
			m_ofn.nFileOffset=fName.ReverseFind('\\');
		}
		else
			ret=IDCANCEL;
	}
	return ret;
}

int CSilenser::DoModalDialog()
{
	PDoModal* pVtable=*(PDoModal**)this;
	pVtable+=0xC4/4;
	// Получим разрешение на запись
	DWORD old;
	VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
	// Запомним адрес обработчикаа
	pInitDlg=*pVtable;
	// Впишем свой адрес
	*pVtable=OnInitDlgTrap;
	// Вызовем нормальный DoModal
	SwapCode(dlgdmd,*(BYTE**)&m_pDlgDoModal);
	int ret=(this->*m_pDlgDoModal)();
	SwapCode(dlgdmd,*(BYTE**)&m_pDlgDoModal);
	return ret;
}
int CSilenser::OnInitDlgTrap()
{
	// Сюда мы попадаем в момент обработки диалогом WM_INITDIALOG
	// Дадим ему спокойно отработать инициализацию
	PDoModal* pVtable=*(PDoModal**)this;
	pVtable+=0xC4/4;
	*pVtable=pInitDlg;
	BOOL ret=(this->*pInitDlg)();
	int end=CIConfigEvents::m_ConfigEvents.FireDoModal(this);
	if(end)
	{
		SendMessage(WM_COMMAND,end);
		if(strcmp(GetRuntimeClass()->m_lpszClassName,"COlePropertyPageSite"))
			EndDialog(end);
	}
	return ret;
}

void CSilenser::Init()
{
	m_pAfxMsgBoxT=AfxMessageBox;		// Получаем адес шлюза
	char* ptr=(char*)m_pAfxMsgBoxT;		
	ptr+=2;								// Пропускаем 0xFF25
	m_pAfxMsgBoxT=**(PAfxMessageBoxT**)ptr;	// А вот теперь реальный адрес в mfc.dll
	jmpMsgBoxT=(DWORD)AfxMessageBoxT;
	DWORD old;
	VirtualProtect(m_pAfxMsgBoxT,6,PAGE_EXECUTE_READWRITE,&old);
	ptr=(char*)ambTbuf+2;
	*(DWORD**)ptr=&jmpMsgBoxT;

	// Теперь займемся диалогом
	CDialog dlg;
	DWORD* pVtable=*(DWORD**)&dlg;
	pVtable+=0xC0/4;
	m_pDlgDoModal=*(PDoModal*)pVtable;
	*(PDoModal*)(&jmpDlgDoModal)=DoModalDialog;
	VirtualProtect((void*)*pVtable,6,PAGE_EXECUTE_READWRITE,&old);
	ptr=(char*)dlgdmd+2;
	*(DWORD**)ptr=&jmpDlgDoModal;

	// И файловым диалогом
	CFileDialog fdlg(0);
	pVtable=*(DWORD**)&fdlg;
	pVtable+=0xC0/4;
	m_pFDlgDoModal=*(PDoModal*)pVtable;
	*(PDoModal*)(&jmpFDlgDoModal)=DoModalFileDialog;
	VirtualProtect((void*)*pVtable,6,PAGE_EXECUTE_READWRITE,&old);
	ptr=(char*)fdlgdmd+2;
	*(DWORD**)ptr=&jmpFDlgDoModal;

	SetSilense(true);
}


void CSilenser::SetSilense(bool bSilence)
{
	if(IsSilence()==bSilence)
		return;
	SwapCode(ambTbuf,(BYTE*)m_pAfxMsgBoxT);
	SwapCode(dlgdmd,*(BYTE**)&m_pDlgDoModal);
	SwapCode(fdlgdmd,*(BYTE**)&m_pFDlgDoModal);
}

bool CSilenser::IsSilence()
{
	return ambTbuf[0]!=0xFF;
}

void CSilenser::Done()
{
	SetSilense(false);
}
