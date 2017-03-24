// vis1cpp.h
#ifndef VIS1CPP_MY_H
	#define VIS1CPP_MY_H

#include "config.h"
#include "configsvcimpl.h"
#include ".\1cpp\metadataofclasses.h"
#include "cppview.h"



struct CompNoCase{
	bool operator()(const CString& s1,const CString& s2)const{return s1.CollateNoCase(s2)<0;}
};
typedef std::map<CString,CString,CompNoCase> MAP_S_S;
typedef MAP_S_S::iterator SS_IT;

typedef std::map<CString,void*,CompNoCase> MAP_S_V;
typedef MAP_S_V::iterator SV_IT;

typedef std::map<CString,DWORD,CompNoCase> MAP_Str_Dword;
typedef MAP_Str_Dword::const_iterator Str2Dword_IT;

void Log(const char* format,MessageMarker mark=mmBlackErr,bool MsgBox=0,...);

extern CConfigService* pSvc;
extern HINSTANCE hMyInst;

// TODO artbear - грабли при использовании OLE/COM плагина:
// если где-то есть вызов CSomeDialog::DoModal или CMenu::LoadMenu
// вобщем загрузка ресурсов из своей длл, предварительно нужно подменять дескриптор ресурсов
// иначе будет использоваться 1crcrus.dll
//
// HINSTANCE hPrev = AfxGetResourceHandle();
// AfxSetResourceHandle(_Module.m_hInstResource);
// 	CSomeDialog::DoModal();
// AfxSetResourceHandle(hPrev);
//
class CMain1C_ResourceHandler_Guard
{
public:
	CMain1C_ResourceHandler_Guard() : m_h_1C_Handle ( AfxGetResourceHandle() )
	{
		AfxSetResourceHandle(_Module.m_hInstResource);
	}
	~CMain1C_ResourceHandler_Guard()
	{
		AfxSetResourceHandle(m_h_1C_Handle);
	}
private:
	HINSTANCE m_h_1C_Handle;
};

#endif