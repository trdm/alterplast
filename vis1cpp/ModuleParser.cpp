// ModuleParser.cpp: implementation of the CModuleParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "ModuleParser.h"
#include "cppview.h"
#include ".\1cpp\Modulestring.h"
#include "txtget.h"
//#include "mymodule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MAP_Str_Dword CModuleParser::m_Tokens;
const char CModuleParser::m_delim[]=" \t\r\n(),;=";
CDocTemplate* CModuleParser::m_pDocTxt;
CDocTemplate* CModuleParser::m_pDocErt;

void CModuleParser::InitMaps()
{
	m_Tokens["ПЕРЕМ"]		=1;
	m_Tokens["VAR"]			=1;
	m_Tokens["ПРОЦЕДУРА"]	=2;
	m_Tokens["ФУНКЦИЯ"]		=2;
	m_Tokens["PROCEDURE"]	=2;
	m_Tokens["FUNCTION"]	=2;
	m_Tokens["EXPORT"]		=3;
	m_Tokens["ЭКСПОРТ"]		=3;
	m_Tokens["ДАЛЕЕ"]		=4;
	m_Tokens["FORWARD"]		=4;

	CWinApp* pApp = pMainApp;
	CMyDocTemp* pTemp;
	POSITION pos=pApp->GetFirstDocTemplatePosition();
	while(pos)
	{
		pTemp=(CMyDocTemp*)pApp->GetNextDocTemplate(pos);
		if(!strcmp("CBodyTextDoc",pTemp->GetDocClass()->m_lpszClassName))
			m_pDocTxt=pTemp;
		if(!strcmp("CWorkBookDoc",pTemp->GetDocClass()->m_lpszClassName))
			m_pDocErt=pTemp;
	}
	
}
CString CModuleParser::GetNextToken(const char*& pos)
{
	if(!*pos)
		return "";
	CString token="";
	while(token.IsEmpty())
	{
		if(*pos=='\n')
			m_curLine++;
		for(pos++;*pos;pos++)
		{
			if(*pos=='\"')
			{
				for(pos++;*pos && *pos!='\"';pos++)
				{
					if(*pos=='\n')
						m_curLine++;
				}
				if(!*pos)
					return token;
				continue;
			}
			else if(*pos=='/' && pos[1]=='/')
			{
				for(pos+=2;*pos && *pos!='\n';pos++);
				if(!*pos)
					return token;
				m_curLine++;
				continue;
			}
			if(strchr(m_delim,*pos))
				break;
			token+=*pos;
		}
		if(!*pos)
			return token;
	}
	return token;
}

void CModuleParser::ParseSrc(const CString& src)
{
	if(src.IsEmpty())
		return;

	CString token,name;
	int type;
	LPCTSTR pos=src;
	DWORD state=0,line=0;
	bool FoundVal=false;

	m_keys.clear();
	m_curLine=0;
	Str2Dword_IT it;
	while(!(token=GetNextToken(pos)).IsEmpty())
	{
		it=m_Tokens.find(token);
		type=it==m_Tokens.end()?-1:it->second;
		if(!state)
		{
			if(type==-1)
				continue;
			if(type>2)
				continue;
			if(type==1 && FoundVal)
				continue;
			state=type;
		}
		else
		{
			if(state==1)
			{
				while(*pos && (*pos==' '||*pos=='\t'))
					pos++;
				if(*pos==',' || *pos==';')
				{
					//m_keys[token]=KEY_VAL|m_curLine;
					CKeyAndLinePair keyAndLine(kt2KEY_VAL, m_curLine);
					m_keys[token] = keyAndLine;

					if(*pos!=',')
						state=0;
					continue;
				}
				name=token;
				state=11;
				line=m_curLine;
				pos--;
				continue;
			}
			if(state==11)
			{
				if(type==3)
				{
					//m_keys[name]=KEY_VAL|KEY_EXPORT|line;
					CKeyAndLinePair keyAndLine(kt2KEY_VAL_EXPORT, line);
					m_keys[name] = keyAndLine;

					while(*pos && (*pos==' '||*pos=='\t'))
						pos++;
					if(*pos==',')
						state=1;
					else
					{
						state=0;
						pos--;
					}
				}
				else
					state=0;
			}
			if(state==2)
			{
				FoundVal=true;
				name=token;
				line=m_curLine;
				while(*pos && *pos!=')')
				{
					if(*pos=='\n')
						m_curLine++;
					pos++;
				}
				state=22;
				continue;
			}
			if(state==22)
			{
				state=0;
				KeyType2 key = kt2KEY_PROC;//DWORD mod=0;
				if(type==4)
					continue;
				else if(type==3)
					key = kt2KEY_PROC_EXPORT; // mod=KEY_EXPORT;
				//m_keys[name]=KEY_PROC|mod|line;
				CKeyAndLinePair keyAndLine(key, line);
				m_keys[name] = keyAndLine;
			}
		}
	}
}

int CModuleParser::InnerFindDef(const CString& src,const CString& Class)
{
	m_curLine=0;
	CString token,search;
	search=Class;
	search.MakeUpper();
	char state=0;
	const char *pos=src;
	//DWORD type;
	while(!(token=GetNextToken(pos)).IsEmpty())
	{
		token.MakeUpper();
		if(state)
		{
			if(token==search)
				return m_curLine;
			else
				state=0;
		}
		else
		{
			if(token=="CLASS" || token=="КЛАСС")
				state=1;
		}
	}
	return -1;
}

CBodyTextView* CModuleParser::InnerFindDef(CDocument* pDoc, const CString& Class, int& line)
{
	if(!pDoc)
		return NULL;
	line=-1;
	CBodyTextView* pEditor=CCPPView::FindTextView(pDoc);
	if(!pEditor)
		return NULL;
	CBodyTextDoc* pTxt=pEditor->GetDocument();
	if(!pTxt)
		return NULL;
	CString txt;
	pTxt->GetText(txt);
	txt.Insert(0," ");
	line = InnerFindDef(txt,Class);
	return pEditor;
}

void CModuleParser::ParseClassModule(const CString& ClassName)
{
	ParseSrc(GetSrc(ClassName));
}

CString CModuleParser::GetSrc(const CString& ClassName)
{
	CDocTemplate* pTemp;
	if(m_path.Find(".ert")!=-1)
		pTemp=m_pDocErt;
	else
		pTemp=m_pDocTxt;

	CDocument* pDoc=NULL;
	for(POSITION pos=pTemp->GetFirstDocPosition();pos;pDoc=NULL)
	{
		pDoc=pTemp->GetNextDoc(pos);
		if(!pDoc->GetPathName().CollateNoCase(m_path))
			break;
	}

	CBodyTextDoc* pTxtDoc=NULL;
	if(pDoc)
	{
		if(CString("CWorkBookDoc")==pDoc->GetRuntimeClass()->m_lpszClassName)
			pTxtDoc=(CBodyTextDoc*)((CWorkBookDoc*)pDoc)->GetTextDocument();
		else if(CString("CBodyTextDoc")==pDoc->GetRuntimeClass()->m_lpszClassName)
			pTxtDoc=(CBodyTextDoc*)pDoc;
		if(!pTxtDoc)
		{
			CBodyTextView *pView=CCPPView::FindTextView(pDoc);
			if(!pView)
			{
				Log("Не удалось получить текст модуля "+pDoc->GetTitle());
				return "";
			}
			pTxtDoc=(CBodyTextDoc*)pView->GetDocument();
		}
		if(!pTxtDoc)
		{
			Log("Не удалось получить текст модуля "+pDoc->GetTitle());
			return "";
		}
		
		CString srcText;
		pTxtDoc->GetText(srcText);
		
		if (ClassName.IsEmpty())
			return srcText;

		CModuleString mod(srcText);
		mod.CropModuleToClass(ClassName);

		//m_IsParse=false;
		return GetSrcFromModuleString(mod);
	}
	
	CModuleString mod;
	if (m_path.IsEmpty())
		return "";

	if(!mod.LoadModule(m_path, ClassName)) // читаем текст модуля класса
	{
		Log("!!Не удалось получить текст модуля: "+m_path);
		return "";
	}
	
	//m_IsParse=true;
	return GetSrcFromModuleString(mod);
}

CString CModuleParser::GetSrcFromModuleString(const CModuleString& ModuleString)
{
	// довольно некрасивая конструкция.
	// mod.getOffsetLines() реально что-то возвращает только после
	// mod.LoadModule(m_path, ClassName).
	
	m_OffsetLines = ModuleString.getOffsetLines();
	
	CString srcText =' ';
	srcText += ModuleString;
	
	return srcText;
}

void CModuleParser::Init( const CString& strPath )
{
	m_path = strPath;
	m_path.MakeLower();
}

CString CModuleParser::GetSourceText()
{
	return GetSrc();	
}

//bool CModuleParser::FindKey(const CString& strName, DWORD* pResult) const
bool CModuleParser::FindKey(const CString& strName, CKeyAndLinePair* pResult) const
{
	Str2KeyAndLine_IT itproc = GetKeysMap().find(strName);
	if(itproc == GetKeysMap().end())
		return false;
	
	*pResult = itproc->second;
	return true;
}

CBodyTextView* CModuleParser::FindDef( CDocument* pDoc, const CString& Class, int& line )
{
	CModuleParser mod;
	CBodyTextView* pEditor = mod.InnerFindDef(pDoc, Class, line);
	return pEditor;
}

//long CModuleParser::Compile(CClassInfo* pInfo,CBLModule7 *pGlobModule)
//{
// 	if(m_path.IsEmpty())
// 		return 0;
// 	CString src;
// 	bool newGM=false;
// 	if(!pGlobModule)
// 	{
// 		CTxtGet gm;
// 		src=gm.GetGMText();
// 		pGlobModule=new CMyModule(src);
// 		((CMyModule*)pGlobModule)->m_silent=true;
// 		newGM=true;
// 		CString msg=pBkEndUI->GetStatusLine();
// 		pBkEndUI->DoStatusLine("Компиляция ГМ...");
// 		BOOL success=pGlobModule->Compile();
// 		pBkEndUI->DoStatusLine(msg);
// 		if(!success)
// 		{
// 			if(AfxMessageBox("ГМ содержит ошибки. Продолжить?",MB_YESNO|MB_ICONEXCLAMATION)==IDNO)
// 			{
// 				delete pGlobModule;
// 				return false;
// 			}
// 		}
// 	}
// 	CMyModule* pMod=new CMyModule(GetSrc(true));
// 	pMod->m_pInfo=pInfo;
// 	pMod->AssignFriendModule(pGlobModule);
// 	pMod->Compile();
// 	long ret=pMod->m_count;
// 	if(newGM)
// 		delete pGlobModule;
// 	delete pMod;
// 	return ret;
// 	
// 	return 0;
// }
