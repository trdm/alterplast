// InfoWrcSpace.cpp: implementation of the CInfoWrcSpace class.

#include "stdafx.h"

#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "InfoWrcSpace.h"
#include ".\1cpp\preprocessor.h"
#include ".\1cpp\MetaDataOfClasses.h"
#include "deffileinfo.h"
#include "rootprop.h"
#include "mytextdoc.h"
#include "syntax.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// class COpenDoc:public CDocument
// {
// public:
// 	void Open(CString title)
// 	{
// 		m_strTitle=title;	// Пока так, на скорую руку
// 		CFrameWnd* pFrame=m_pDocTemplate->CreateNewFrame(this,NULL);
// 		m_pDocTemplate->InitialUpdateFrame(pFrame,this,TRUE);
// 	}
// };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoWrcSpace::CInfoWrcSpace():
	CInfo("",TYPE_WRCSPACE_FOLDER)
{
	CDefFileInfo::RemoveAllDefFiles();

	// artbear перенес сюда, иначе при анализе defcls@md не учитывался путь к базе
	CMetaDataOfClasses *pMeta=CMetaDataOfClasses::m_oneMetadata;
	pMeta->pathBase1C = pSvc->IBDir();
	assert(!pMeta->pathBase1C.IsEmpty());
	//end

	CString strDefClsName=pSvc->IBDir()+"defcls.prm";

	CTaskDef* pDef=GetMetaData()->GetTaskDef();

	m_DisplayName=pDef->GetPresent();
	if(m_DisplayName.IsEmpty())
		m_DisplayName=pDef->GetCode();

	CFileStatus st;
	CModuleString strDefclsModuleText;

	if(!CFile::GetStatus(strDefClsName, st))
	{
		
		strDefClsName = "defcls@MD";
		if (!strDefclsModuleText.LoadModule(strDefClsName))
		{
			m_DisplayName+=" (defcls.prm не найден)";    
			return;
		}
	}
	else
		ReadDefclsTextToModuleString(strDefClsName, strDefclsModuleText);

	ReadAllAliasesFrom_OxyIniFile();

	AnalyzeAllClassesInfo(strDefclsModuleText, strDefClsName);
}

CInfoWrcSpace::~CInfoWrcSpace()
{
}

HTREEITEM CInfoWrcSpace::InsertInTree(HTREEITEM parent)
{
	HTREEITEM item = CInfo::InsertInTree(parent);

	CDefFileInfo::CreateAndInsertInTreeForEachDefFile(item);

	return item;
}

static const int mtRefresh		= 1;
static const int mtSaveAll		= 2;
static const int mtCompileAll	= 3;
static const int mtNextError	= 4;
static const int mtPrevError	= 5;

int  CInfoWrcSpace::GetContextMenu(CStringList& lst, DWORDPtrList& idflags)
{
	lst.AddTail("Обновить");
	idflags.AddTail(mtRefresh);
	
	bool mod = CDocumentInterceptor::HaveUnsavedDocuments();
	
	UINT flag = mod ? 0 : MF_GRAYED;
	lst.AddTail("Сохранить все\tCtrl+S");
	idflags.AddTail(mtSaveAll | (flag<<16));

	AddSeparator(lst, idflags);

	lst.AddTail("Проверить все");
	idflags.AddTail(mtCompileAll);

	flag = CSyntax::GetTotalError() ? 0:MF_GRAYED<<16;

	lst.AddTail("К следующей ошибке\tF4");
	idflags.AddTail(mtNextError | flag);
	
	lst.AddTail("К предыдущей ошибке\tShift+F4");
	idflags.AddTail(mtPrevError | flag);
	
	return 0;
}

void CInfoWrcSpace::OnContextMenu(UINT cmd)
{
	if(cmd == mtRefresh)
		CCPPView::GetView()->Refresh();
	else if(cmd == mtSaveAll)
		CCPPView::GetView()->OnSave();
	else if(cmd == mtCompileAll)
		CSyntax::CompileAll();
	else if(cmd == mtNextError || cmd == mtPrevError)
		CSyntax::OpenNextError(cmd == mtPrevError);
}

void CInfoWrcSpace::GetPropertyPages(CPropertyPagePtr& pPropPage)
{
	pPropPage.reset(new CRootProp);
}

void CInfoWrcSpace::ReadAllAliasesFrom_OxyIniFile()
{
	CMetaDataOfClasses *pMeta = CMetaDataOfClasses::m_oneMetadata;

	CFile file;
	CString strFileNameIni = pSvc->BinDir() + "OXY.ini";    
	if (!file.Open(strFileNameIni, CFile::modeRead))
		return;

	DWORD nLength = file.GetLength();
	file.Close();
	
	std::vector<char> BufVector(nLength+2);
	char* pBuf = &BufVector[0];
	
	int nSize = 0;
	nSize = GetPrivateProfileSection("alias_path", pBuf, nLength+2, strFileNameIni);
	if (nSize > 0)
	{
		CString str;
		for (int i = 0; i < nSize; i++)
		{
			
			if (pBuf[i] == '\0')
			{         
				int nPosEq = str.Find("=");
				if (-1 != nPosEq)
				{
					CString strAlias       = str.Left(nPosEq);
					CString strPathOfAlias = str.Right(str.GetLength() - nPosEq - 1);
					strAlias.TrimLeft(); strAlias.TrimRight();
					strPathOfAlias.TrimLeft(); strPathOfAlias.TrimRight();
					if (strPathOfAlias.GetAt(strPathOfAlias.GetLength()-1) == '\\')              
						pMeta->m_AliasOfPaths[strAlias] = strPathOfAlias.Left(strPathOfAlias.GetLength()-1);              
					else
						pMeta->m_AliasOfPaths[strAlias] = strPathOfAlias;
					
					//pBkEndUI->DoMessageLine(strAlies, mmBlackErr);	
					//pBkEndUI->DoMessageLine(strPathOfAlies, mmBlackErr);	
				}
				str.Empty();
				if (pBuf[i+1] == '\0')
					break; // конец секции
				continue;
			}
			str += pBuf[i];        
		}
	}          
}

void CInfoWrcSpace::ReadDefclsTextToModuleString( const CString& strDefClsName, CModuleString &strDefclsModuleText )
{
	{  
		TRY{     
			CFile file(strDefClsName,CFile::modeRead);
			
			/* // artbear этот функционал запрещен
			// artbear автоматом включаю строку //#include defcls@md если ее нет
			bool bHaveDeflcsInMD = false;
			{
			CString nameRep = "defcls"; // имя обработки в конфе
			CMetaDataCont *pMD = GetMetaData();
			
			  CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
			  if (pObj) // есть такая обработка
			  bHaveDeflcsInMD = true;
			  }
			  // end
			*/
			
			DWORD dwLength = file.GetLength();
			LPTSTR buff = strDefclsModuleText.GetBuffer(dwLength);
			file.Read(buff,dwLength);
			strDefclsModuleText.ReleaseBuffer(dwLength);
			file.Close();
			
			/* // artbear этот функционал запрещен
			// artbear автоматом включаю строку //#include defcls@md если ее нет
			if (bHaveDeflcsInMD)
			{
			CString strTemp = strDefclsModuleText;
			strTemp.MakeLower();
			if (strTemp.Find("defcls@md") == -1)
			strDefclsModuleText = strDefclsModuleText + "\r\n//#include defcls@md\r\n";
			}
			// end
			*/
		}
		CATCH(CFileException, pE)
		{
			pBkEndUI->DoMessageLine("Не удалось открыть файл определения классов", mmBlackErr);
			pBkEndUI->DoMessageLine(strDefClsName,mmBlackErr);
		}
		AND_CATCH(CMemoryException,pE)
		{
			pBkEndUI->DoMessageLine("Внимание не хватило памяти для выделения буфера в функции CSetOfHierarchy::CSetOfHierarchy()", mmExclamation);
		}
		END_CATCH
	}
}

void CInfoWrcSpace::AnalyzeAllClassesInfo( CModuleString& strDefclsModuleText, const CString& strDefClsName )
{
	CMetaDataOfClasses *pMeta = CMetaDataOfClasses::m_oneMetadata;

	CRangesOfDescr Ranges;
	CPreprocessor preproc(strDefclsModuleText,pMeta->m_SymbolOfPreprocessor,pMeta->m_LoadedIncludeFiles,strDefClsName,&Ranges);
	
	try{
		preproc.Start();
		pMeta->ParsingString(strDefclsModuleText,Ranges);
	}
	catch(CPreprocessor::ExeptionPreprocessor &ex)
	{
		CString strErr;        
		strErr.Format("В модуле файла: %s",strDefClsName);
		pBkEndUI->DoMessageLine(strErr,mmBlackErr);
		strErr = ex.GetErrorString();
		pBkEndUI->DoMessageLine(strErr, mmBlackErr);	
		strErr.Empty();
		if (!ex.GetCodeString().IsEmpty())
		{
			strErr.Format("(%d) %s",ex.GetNumString(),ex.GetCodeString());
			pBkEndUI->DoMessageLine(strErr, mmBlackErr);
		}
	}
	
	CDefFileInfo::CreateListOfClassesNameForEachDefFile();
}
