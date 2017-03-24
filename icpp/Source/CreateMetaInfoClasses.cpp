// CreateMetaInfoClasses.cpp: implementation of the CCreateMetaInfoClasses class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "CreateMetaInfoClasses.h"
#include "ModuleString.h"
#include "Struct.h"
#include "MetaDataOfClasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;
extern CDataBase7 * pDataBase7;


IMPLEMENT_DYNCREATE(CCreateMetaInfoClasses, CBLContext);

class CObjID CCreateMetaInfoClasses::ObjID;
//////////////////////////////////////////////////////////////////////
// Описание методов и функций класса для 1С языка
// {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
//////////////////////////////////////////////////////////////////////
struct CCreateMetaInfoClasses::paramdefs CCreateMetaInfoClasses::defFnNames[] = {
	{"CreateALS","СоздатьALS",0,2},
	/*
	CreateALS(strNameFileALS, strCompany) рус. СоздатьALS(стрИмяФайлаALS, стрИмяКомпании) - метод создает
	для синтаксис-помощника описания классов
	+Параметры:
	- стрИмяФайлаALS: путь имя файла в который будет записана данные структуированные для файла -
	синтаксис помощника 1С
	- стрИмяКомпании: имя компании, владельца файла, данная строка будет корневой в иерархии папок
	синтаксис помощника и может быть любой строкой, например: "Классы 1С++ фирмы OXY"
	*/
	{"TuningALS","НастройкаALS",0,7},
	/*
	TuningALS(strNameSpace, strNameClass, strNameMethod, strNameProperty, strNameNames, strNameParam, strNameReturns)
	*/
	{"SetFilterClasses","УстФильтрПоКлассам",0,1},
	{"SetFilterNamespace","УстФильтрПоПростИмен",0,1},
	{"GetLoadIncludeFiles","ПолучитьЗагрВложФайлы",1,0},
	{"GetDefSymbolOfPreproc","ПолучитьОпрСимволыПрепроц",1,0},
	{"GetAllClassesNames","ПолучитьВсеИменаКлассов",1,0},
	{"GetAllAliasAndPath","ПолучитьПсевдИ_Пути",1,0},
	{"GetNamesMethOfClass","ПолучитьИменаМетодовКласса",1,1},
	{"IsClassExist","КлассСуществует",1,1},
	{"IsMethExist","МетодСуществует",1,2},
	{"GetInfoParamMeth","ПолучитьИнфоПоПар",1,2},
	{"GetVersion","ПолучитьВерсию",1,1},
	{"GetCurSettings","ПолучитьТекУстановки",0,3},
	{"SetCurSettings","УстановитьТекУстановки",0,3},
	{"ClearReposClasses","ОчиститьКлассыРепозитария",0,0},
	{"ThisIsSQL_Ver","ЭтоSQL_Версия",1,0},
	{"ReadDefinesOfClasses","ЗагрузитьОписаниеКлассов",0,2},
	{"GetBaseClasses","ПолучитьИменаБазовыхКлассов",1,1},
	{"TheClassIs","ЯвляетсяОбъектом",1,2},
	{"GetFileVersion","ПолучитьВерсиюФайла",1,2},

	{"ClearClassOptimizationData","ОчиститьДанныеОптимизацииКлассов",0,0},

	{NULL,NULL, 0,0}

};

enum {
	procCreateALS,
	procTuningALS,
	procSetFilterClasses,
	procSetFilterNamespace,
	methGetLoadIncludeFiles,
	methGetDefSymbolOfPreproc,
	methGetAllClassesNames,
	methGetAllAliasAndPath,
	methGetNamesMethOfClass,
	methIsClassExist,
	methIsMethExist,
	methGetInfoParamMeth,
	methGetVersion,
	procGetCurSettings,
	procSetCurSettings,
	procClearReposClasses,
	methThisIsSQL_Ver,
	procReadDefinesOfClasses,
	funcGetBaseClasses,
	funcTheClassIs,
	methGetFileVersion,

	procClearClassOptimizationData,

	lastMethod
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateMetaInfoClasses::CCreateMetaInfoClasses() : pDoc(NULL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strNameSpace    = L"namespace";
	m_strNameClass    = L"class";
	m_strNameMethod   = L"method";
	m_strNameProperty = L"property";
	m_strNameNames    = L"name";
	m_strNameParam    = L"param";
	m_strNameReturns  = L"returns";
}

CCreateMetaInfoClasses::~CCreateMetaInfoClasses()
{
}

void CCreateMetaInfoClasses::PrepareXMLPart(CString &strSource)
{
	int nLen = strSource.GetLength();
	int nState = 0;
	int nCountDel = 0;
	for (int i = 0; i < nLen; i++)
	{
		TCHAR sym = strSource[i];
		if (sym == '/')
			nCountDel++;

		if (sym == '/' && nState == 0) {
			nState = 1; continue;}
		else if (sym == '/' && nState == 1) {
			nState = 2; continue;}
		else if (sym == '/' && nState == 2) {
			nState = 3; continue;}
		else if (sym != '/' && nState == 3) {
			nState = 4; continue;}
		else if (sym == '/' && nState == 3) {
			nState = 5; continue;}

		switch(nState)
		{
		case 2:
		case 5:
			{
				//удалим комментарий
				int nCount = 1;
				int posComment = i-nCountDel;

				while(i != nLen && strSource[i] != '\n')
				{
					i++;
					nCount++;
				}
				i -= nCount;
				strSource.Delete(posComment, nCount);
				nLen = strSource.GetLength();
			}
			break;
		case 4:
			{
				//for(int ind = i - 4; ind <= i-1; ind++)
				//strSource.SetAt(ind, (TCHAR)' ');
				int posComment = i-nCountDel-1;
				strSource.Delete(posComment, nCountDel);
				nLen = strSource.GetLength();
			}
			break;
		}
		nState    = 0;
		nCountDel = 0;
	}
}
void CCreateMetaInfoClasses::ProcessChildNode(MSXML::IXMLDOMElement *node, CString &strDest)
{
	if(node->hasChildNodes())
	{
		MSXML::IXMLDOMNodeListPtr	childs;
		MSXML::IXMLDOMNodePtr childnode;

		node->get_childNodes(&childs);
		long nCnt = childs->Getlength();
		for(int i=0;i<nCnt;++i)
		{
			childs->get_item(i,&childnode);
			ProcessElement((MSXML::IXMLDOMElement *)childnode.GetInterfacePtr(), strDest);
		}
	}
}

void CCreateMetaInfoClasses::ProcessMethod(MSXML::IXMLDOMElement *node, CString &strDest)
{
	MSXML::IXMLDOMNamedNodeMapPtr pNodeMap;
	HRESULT hr = node->get_attributes(&pNodeMap);
	bool bErrState = false;
	if(SUCCEEDED(hr) && pNodeMap != NULL)
	{
		MSXML::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(m_strNameNames.m_str);
		if (pNode != NULL)
		{
			CComBSTR s;
			pNode->get_text(&s);
			strDest +=",{\"Item\",\"AST\",\"";
			strDest += CString(s);
			strDest += "\",\"";
			strDest += CString(s);
			strDest += "\"";

			MSXML::IXMLDOMNodeListPtr	childs;
			int  nCountParams = 0;
			bool bHasRetVal   = false;

			if(node->hasChildNodes())
			{
				node->get_childNodes(&childs);
				for(int i=0;i<childs->Getlength();i++)
				{
					MSXML::IXMLDOMNodePtr childnode;
					CComBSTR name;

					childs->get_item(i,&childnode);
					childnode->get_nodeName(&name);

					if (!wcscmp(name,m_strNameParam))
						nCountParams++;
					else if (!wcscmp(name,m_strNameReturns))
						bHasRetVal = true;
				}
			}
			CString part("(");
			for (int i = 0; nCountParams > i; ++i)
			{
				if (i == 0) part += "<?>";
				if (i > 0 && i != nCountParams) part += ",";
			}
			part += ")";
			strDest += ",\"";
			strDest += CString(s) + part;
			strDest += "\"";
			strDest += ",\"";
			strDest += CString(s) + part;
			strDest += "\"";

			strDest += ",\"";
			// free text

			long lLength = pNodeMap->Getlength();
			for (int ind = 0; ind < lLength; ind++)
			{
				pNode = NULL;
				CComBSTR sText;
				CComBSTR s;
				pNodeMap->get_item(ind, &pNode);
				pNode->get_nodeName(&s);

				if (!wcscmp(s,m_strNameNames))
					continue;

				pNode->get_text(&sText);
				CString res;
				if (SysStringLen(sText)>0)
				{
					strDest += "@";
					res = s; res.TrimLeft(); res.TrimRight();
					strDest += res;
					strDest += "\r\n";
					res = sText; res.TrimLeft(); res.TrimRight(); res.Replace("\r\n","");
					strDest += res;
					strDest += "\r\n";
				}
			}

			if(node->hasChildNodes())
			{
				bool isTitleParamExist = false;
				bool isTitleRetExist   = false;
				for(int i=0;i<childs->Getlength();i++)
				{
					MSXML::IXMLDOMNodePtr childnode;
					CComBSTR name;
					CComBSTR sTextChild;
					childs->get_item(i,&childnode);
					childnode->get_nodeName(&name);
					childnode->get_text(&sTextChild);
					if (!isTitleParamExist&& nCountParams > 0 && !wcscmp(name,m_strNameParam))
					{
						isTitleParamExist = true;
						strDest += "@Параметры\r\n";
					}
					else if (bHasRetVal && !isTitleRetExist && !wcscmp(name,m_strNameReturns))
					{
						isTitleRetExist = true;
						strDest += "@Возвращаемое значение\r\n";
					}
					else if (wcscmp(name,m_strNameParam) && wcscmp(name,m_strNameReturns))
					{
						strDest += "@";
						strDest += CString(name);
						strDest += "\r\n";
					}
					pNodeMap = NULL;
					HRESULT hr = childnode->get_attributes(&pNodeMap);
					if(SUCCEEDED(hr) && pNodeMap != NULL)
					{
						//if (!wcscmp(name,m_strNameParam))
						{
							long lLength = pNodeMap->Getlength();
							if (lLength > 0)
							{
								strDest += (unsigned char)149;
								strDest += " < ";
							}
							for (int ind = 0; ind < lLength; ind++)
							{
								pNode = NULL;
								CComBSTR sText;
								CComBSTR name;
								pNodeMap->get_item(ind, &pNode);
								pNode->get_nodeName(&name);
								pNode->get_text(&sText);
								strDest += CString(name);
								strDest += "=\"\"";
								strDest += CString(sText);
								strDest += "\"\" ";
							}
							if (lLength > 0)
								strDest += ">\r\n";
						}
					}
					CString strTextChild(sTextChild);
					strTextChild.Replace("\"", "\"\"");
					strTextChild.Replace("\t", "");
					RemoveNeedlessBlank(strTextChild); //ISetOfHierarchy::RemoveNeedlessBlank(strTextChild);
					strDest += strTextChild;
					strDest += "\r\n";

				}
			}
			strDest += "\"";

			strDest +="}";
			//pBkEndUI->DoMessageLine(CString(s), mmInformation);
		}
		else
			bErrState = true;
	}
	else
		bErrState = true;

	if (bErrState)
		throw CExeptionUser("В элементе method не найден атрибут name!", node);

}
void CCreateMetaInfoClasses::ProcessAttribut(MSXML::IXMLDOMElement *node, CString &strDest)
{
	MSXML::IXMLDOMNamedNodeMapPtr pNodeMap;
	HRESULT hr = node->get_attributes(&pNodeMap);
	bool bErrState = false;
	if(SUCCEEDED(hr) && pNodeMap != NULL)
	{
		MSXML::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(m_strNameNames.m_str);
		if (pNode != NULL)
		{
			CComBSTR s;
			pNode->get_text(&s);
			strDest +=",{\"Item\",\"AST\",\"";
			strDest += CString(s);
			strDest += "\",\"";
			strDest += CString(s);
			strDest += "\"";
			strDest += ",\"";
			strDest += CString(s);
			strDest += "\"";
			strDest += ",\"";
			strDest += CString(s);
			strDest += "\"";

			strDest += ",\"";
			// free text
			long lLength = pNodeMap->Getlength();
			for (int ind = 0; ind < lLength; ind++)
			{
				pNode = NULL;
				CComBSTR sText;
				CComBSTR s;
				pNodeMap->get_item(ind, &pNode);
				pNode->get_nodeName(&s);

				if (!wcscmp(s,m_strNameNames))
					continue;

				pNode->get_text(&sText);
				CString res;
				if (SysStringLen(sText)>0)
				{
					strDest += "@";
					res = s; res.TrimLeft(); res.TrimRight();
					strDest += res;
					strDest += "\r\n";
					res = sText; res.TrimLeft(); res.TrimRight(); res.Replace("\r\n","");
					strDest += res;
					strDest += "\r\n";
				}
			}
			CComBSTR sText;
			node->get_text(&sText);
			strDest += "@Описание:\r\n";
			CString strText(sText);
			strText.Replace("\"", "\"\"");
			strText.Replace("\t", "");
			RemoveNeedlessBlank(strText); //ISetOfHierarchy::RemoveNeedlessBlank(strText);
			strDest += strText;
			strDest += "\"";
			strDest += "}";

		}
		else
			bErrState = true;
	}
	else
		bErrState = true;

	if (bErrState)
		throw CExeptionUser("В элементе property не найден атрибут name!", node);
}
void CCreateMetaInfoClasses::ProcessClass(MSXML::IXMLDOMElement *node, CString &strDest)
{
	CComBSTR s;
	MSXML::IXMLDOMNamedNodeMapPtr pNodeMap;
	HRESULT hr = node->get_attributes(&pNodeMap);
	bool bErrState = false;
	if(SUCCEEDED(hr) && pNodeMap != NULL)
	{
		MSXML::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(m_strNameNames.m_str);
		if (pNode != NULL)
		{
			pNode->get_text(&s);
			//pBkEndUI->DoMessageLine(CString(s), mmInformation);

			int nSize = m_filterClasses.GetSize();
			bool isFind = nSize > 0 ? false : true;
			CString sNameClass(s);
			for (int i = 0; i < nSize; ++i)
			{
				if (sNameClass == m_filterClasses[i])
				{
					isFind = true;
					break;
				}
			}
			if(!isFind)
				return;

			strDest +=",{\"Folder\",\"AST\",\"";
			strDest += CString(s);
			strDest += "\",\"\"";
			strDest +=",{\"Item\",\"AST\",\"";
			strDest += CString(s);
			strDest += "\",\"";
			strDest += sNameClass;
			strDest += "\"";
			strDest += ",\"";
			strDest += sNameClass;
			strDest += "\"";
			strDest += ",\"\"";
			strDest += ",\"";

			long lLength = pNodeMap->Getlength();
			for (int ind = 0; ind < lLength; ind++)
			{
				pNode = NULL;
				BSTR sText = NULL;
				pNodeMap->get_item(ind, &pNode);
				pNode->get_nodeName(&s);
				pNode->get_text(&sText);
				CString res;
				if (SysStringLen(sText)>0)
				{
					strDest += "@";
					res = s; res.TrimLeft(); res.TrimRight();
					strDest += res;
					strDest += "\r\n";
					res = sText; res.TrimLeft(); res.TrimRight(); res.Replace("\r\n","");
					RemoveNeedlessBlank(res); //ISetOfHierarchy::RemoveNeedlessBlank(res);
					strDest += res;
					strDest += "\r\n";
				}
				SysFreeString(sText);
			}
			strDest += "\"";
			strDest+="}";

			//pBkEndUI->DoMessageLine(CString(s), mmInformation);
		}
		else
			bErrState = true;
	}
	else
		bErrState = true;

	if (bErrState)
		throw CExeptionUser("В элементе class не найден атрибут name!", node);

	ProcessChildNode(node, strDest);

	strDest+="}";
}

void CCreateMetaInfoClasses::ProcessNamespace(MSXML::IXMLDOMElement *node, CString &strDest)
{
	CComBSTR s;
	MSXML::IXMLDOMNamedNodeMapPtr pNodeMap;
	HRESULT hr = node->get_attributes(&pNodeMap);
	bool bErrState = false;
	if(SUCCEEDED(hr) && pNodeMap != NULL)
	{
		MSXML::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(m_strNameNames.m_str);
		if (pNode != NULL)
		{
			pNode->get_text(&s);
			int nSize = m_filterNamespace.GetSize();
			bool isFind = nSize > 0 ? false : true;
			CString sNameNamespace(s);
			for (int i = 0; i < nSize; ++i)
			{
				if (sNameNamespace == m_filterNamespace[i])
				{
					isFind = true;
					break;
				}
			}
			if(!isFind)
				return;


			//pBkEndUI->DoMessageLine(CString(s), mmInformation);
			strDest +=",{\"Folder\",\"AST\",\"";
			strDest += sNameNamespace;
			strDest += "\",\"\"";
		}
		else
			bErrState = true;
	}
	else
		bErrState = true;

	if (bErrState)
		throw CExeptionUser("В элементе namespace не найден атрибут name!", node);

	ProcessChildNode(node, strDest);

	strDest+="}";
}

void CCreateMetaInfoClasses::ProcessElement(MSXML::IXMLDOMElement *node, CString &strDest)
{
	BSTR s = NULL;
	node->get_nodeTypeString(&s);
	if(!wcscmp(s,L"element"))
	{
		SysFreeString(s);
		node->get_nodeName(&s);
		if (!wcscmp(s,m_strNameSpace))
			ProcessNamespace(node, strDest);
		else if (!wcscmp(s,m_strNameClass))
			ProcessClass(node, strDest);
		else if (!wcscmp(s,m_strNameMethod))
			ProcessMethod(node, strDest);
		else if (!wcscmp(s,m_strNameProperty))
			ProcessAttribut(node, strDest);
		else
			ProcessChildNode(node, strDest);

		SysFreeString(s);
	}
}

void CCreateMetaInfoClasses::CreateALS(CString &strPlace, CString &strCompany)
{
	HRESULT hr = CoCreateInstance (MSXML::CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
		MSXML::IID_IXMLDOMDocument, (LPVOID *)&pDoc);
	/*char sym[2] = {0};
	for (int i = 0; i < 255; ++i)
	{
	sym[0] = (char)i;
	CString str;
	str.Format("%d=%s",i,sym);
	pBkEndUI->DoMessageLine(str, mmExclamation);
	}*/

	if (!pDoc)
	{
		CBLModule::RaiseExtRuntimeError("Невозможно воспользоваться методом. Для его работы необходим Microsoft XML Parser",0);
	}

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
	if (pHierarchy)
	{
		CString strALS("{\"Shell\",");
		strALS += "{\"Folder\",\"AST\",\"";
		strALS += strCompany;
		strALS += "\",\"\"";
		CStringArray arr;
		arr.Add(CString(pHierarchy->GetPathBase1C()+"defcls.prm"));
		pHierarchy->LoadListNameOfIncludeFiles(arr);
		for (int i = 0; i < arr.GetSize(); i++)
		{
			CModuleString source;
			if (source.LoadModule(arr.GetAt(i))) // fez TODO - добавить обработку новой возможности по хранению нескольких классов в одном модуле.
			{
				PrepareXMLPart(source);
				pDoc->put_async(false);
				BSTR	bstr = source.AllocSysString();
				hr = pDoc->loadXML(bstr);
				SysFreeString(bstr);

				if(!hr)
				{
					long line, linePos;
					BSTR reason  = NULL;
					BSTR srcText = NULL;
					MSXML::IXMLDOMParseErrorPtr pParsingErr;
					char msg[1000];
					pDoc->get_parseError(&pParsingErr);
					pParsingErr->get_errorCode(&hr);
					if (hr != 0xC00CE556 && hr != 0xC00CE558)
					{
						pParsingErr->get_line(&line);
						pParsingErr->get_linepos(&linePos);
						pParsingErr->get_reason(&reason);

						pParsingErr->get_srcText(&srcText);
						sprintf(msg, "%s(\"%s\")\r\nОшибка: 0x%.8X в линии %d, позиция %d\r\n%s", CString(reason).operator LPCTSTR(), CString(srcText).operator LPCTSTR(), hr, line, linePos, arr.GetAt(i).operator LPCTSTR());
						pBkEndUI->DoMessageLine(msg, mmInformation);
						SysFreeString(srcText);
						SysFreeString(reason);
					}
					continue;
				}
				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				char fname[_MAX_FNAME];
				char ext[_MAX_EXT];
				_splitpath(arr.GetAt(i), drive, dir, fname, ext);
				CString strFilename(drive);
				strFilename += dir;
				strFilename += fname;
				strFilename += ".xml";
				CFile f;
				source.Insert(0, "<?xml version=\"1.0\" encoding=\"windows-1251\" ?>");
				f.Open(strFilename, CFile::modeCreate | CFile::modeWrite);
				f.Write(source, source.GetLength());
				f.Close();

				MSXML::IXMLDOMElementPtr element;
				pDoc->get_documentElement(&element);
				if (element)
				{
					CString strResult;
					try
					{
						ProcessElement(element, strResult);
						strALS += strResult;
					}
					catch(CExeptionUser& err)
					{
						CString error;
						error.Format("При обработке файла:%s\r\nПроизошла ошибка:%s",arr.GetAt(i).operator LPCTSTR(), err.GetErrorText().operator LPCTSTR());
						pBkEndUI->DoMessageLine(error, mmBlackErr);
					}
				}
			}
		}
		strALS += "}}";

		CFile f;
		if(f.Open(strPlace, CFile::modeCreate | CFile::modeWrite))
		{
			f.Write(strALS, strALS.GetLength());
			f.Close();
		}
	}
	pDoc->Release();
}

int  CCreateMetaInfoClasses::CallAsFunc(int iMethNum,CValue & rValue,CValue * *ppValue)
{
	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
	//   if (!pHierarchy)
	//		RuntimeError("Не могу получить экземпляр класса ISetOfHierarchy!");
	const CMetaDataOfClasses& MetaDataOfClasses = ISetOfHierarchy::GetMetaDataOfClasses();

	switch(iMethNum)
	{
	case methThisIsSQL_Ver:
		{
			static bool staticBOOLisSQL = "CODBCDB7" == CString(pDataBase7->GetRuntimeClass()->m_lpszClassName);
			rValue = staticBOOLisSQL ? 1L : 0l;
			break;
		}
	case methGetLoadIncludeFiles:
	case methGetDefSymbolOfPreproc:
	case methGetAllClassesNames:
	case methGetAllAliasAndPath:
	case methGetNamesMethOfClass:
	case funcGetBaseClasses:
		{
			rValue.CreateObject("ValueList");
			CBLContext* pCont = rValue.GetContext();
			if (pCont)
			{
				static int nPosMeth = pCont->FindMethod("AddValue");
				if (nPosMeth != -1)
				{
					CStringArrayEx exArr;
					switch(iMethNum)
					{
					case methGetLoadIncludeFiles:{
						CString strLoadIncludeFiles = pHierarchy->GetLoadIncludeFiles(",");
						exArr.FillSeparateString(strLoadIncludeFiles);
												 }
												 break;
					case methGetDefSymbolOfPreproc:{
						CString strDefSymbolOfPreproc = pHierarchy->GetDefSymbolOfPreproc(",");
						exArr.FillSeparateString(strDefSymbolOfPreproc);
												   }
												   break;
					case methGetAllClassesNames:{
						CString strAllClassesNames = MetaDataOfClasses.GetAllClassesNames(",");
						exArr.FillSeparateString(strAllClassesNames);
												}
												break;
					case methGetAllAliasAndPath:{
						CString strAllAliasAndPath = MetaDataOfClasses.GetAllAliasAndPath(",");
						exArr.FillSeparateString(strAllAliasAndPath);
												}
												break;
					case methGetNamesMethOfClass:
						{
							CString par1 = ppValue[0]->GetString();
							if (par1.IsEmpty())
								RuntimeError("В метод необходимо передавать имя класса");
							if (!MetaDataOfClasses.IsClassExist(par1))
							{
								CString error;
								error.Format("Класс с именем: %s не зарегистрирован в системе!", par1.operator LPCTSTR());
								RuntimeError(error);
							}
							CString strNamesMethOfClass = MetaDataOfClasses.GetNamesMethOfClass(par1,",");
							exArr.FillSeparateString(strNamesMethOfClass);
						}
						break;
					case funcGetBaseClasses:
						{
							CString par1 = ppValue[0]->GetString();
							if (par1.IsEmpty())
								CBLModule::RaiseExtRuntimeError("В метод необходимо передавать имя класса",0);
							if (!MetaDataOfClasses.IsClassExist(par1))
							{
								CString error;
								error.Format("Класс с именем: %s не зарегестрирован в системе!", par1.operator LPCTSTR());
								RuntimeError(error);
							}
							CString strBaseClasses = MetaDataOfClasses.GetBaseClasses(par1);
							exArr.FillSeparateString(strBaseClasses);
						}
						break;
					}
					int nSize = exArr.GetSize();
					for (int i = 0; i < nSize; ++i)
					{
						CString res = exArr[i];
						res.TrimLeft(); res.TrimRight();
						if (!res.IsEmpty())
						{
							CValue val1;
							CValue val2;
							CValue* Par[2];
							Par[0] = &val1;
							Par[1] = &val2;
							if (iMethNum == methGetAllClassesNames || iMethNum == methGetAllAliasAndPath)
							{
								int nPosSymbEql = res.Find('=');
								CString s = res.Left(nPosSymbEql);
								s.TrimRight();
								val1 = s;
								s = res.Right(res.GetLength() - nPosSymbEql-1);
								s.TrimLeft();
								val2 = s;
							}
							else
							{
								val1 = res;
							}
							pCont->CallAsProc(nPosMeth, Par);
						}
					}
				}
			}
		}
		break;

	case funcTheClassIs: // ЯвляетсяОбъектом(НазваниеПроверяемогоТипа, НазваниеТипаНаКоторыйПроверяем
		{
			CString par1 = ppValue[0]->GetString();
			if (par1.IsEmpty())
			{
				//CBLModule::RaiseExtRuntimeError("В метод необходимо передавать имя класса",0);
				rValue = CNumeric(0);
				break;
			}

			CString par2 = ppValue[1]->GetString();
			if (par2.IsEmpty())
			{
				//CBLModule::RaiseExtRuntimeError("В метод необходимо передавать имя класса",0);
				rValue = CNumeric(0);
				break;
			}
			if (!par1.CompareNoCase(par2))
				rValue = CNumeric(1);
			else
				if (MetaDataOfClasses.TheClassIsDerive(par1, par2))
					rValue = CNumeric(1);
				else
					rValue = CNumeric(0);
		}
		break;

	case methIsClassExist:
		{
			CString par1 = ppValue[0]->GetString();
			if (par1.IsEmpty())
				RuntimeError("В метод необходимо передавать имя класса");

			rValue = MetaDataOfClasses.IsClassExist(par1);
		}
		break;
	case methIsMethExist:
		{
			CString par1 = ppValue[0]->GetString();
			if (par1.IsEmpty())
				RuntimeError("В метод необходимо передавать имя класса");

			CString par2 = ppValue[1]->GetString();
			if (par2.IsEmpty())
				RuntimeError("В метод необходимо передавать имя метода поиска");

			rValue = MetaDataOfClasses.IsMethExist(par1, par2);
		}
		break;
	case methGetInfoParamMeth:
		{
			CString par1 = ppValue[0]->GetString();
			if (par1.IsEmpty())
				RuntimeError("В метод необходимо передавать имя класса");

			CString par2 = ppValue[1]->GetString();
			if (par2.IsEmpty())
				RuntimeError("В метод необходимо передавать имя метода поиска");

			if (!MetaDataOfClasses.IsMethExist(par1, par2))
			{
				CString error;
				error.Format("Класс с именем: %s методом %s не зарегестрирован в системе!", par1.operator LPCTSTR(), par2.operator LPCTSTR());
				RuntimeError(error);
			}

			rValue.CreateObject("ValueList");
			CBLContext* pCont = rValue.GetContext();
			if (pCont)
			{
				static int nPosMeth = pCont->FindMethod("AddValue");
				if (nPosMeth != -1)
				{
					CParamDefs::CArrayOfMethodsParamData_PTR arr = MetaDataOfClasses.GetInfoParamMeth(par1, par2);
					int nSize = arr->GetSize();
					for (int i = 0; i < nSize; ++i)
					{
						CMethodsParamData::Ptr pType = arr->GetAt(i);
						CValue ValSt;
						CStruct* pContSt = new CStruct; // TODO проще исправить на смарт-пойнтер CBLPtr или CSafeContextPtr
						ValSt.AssignContext(pContSt);
						if (pContSt == NULL)
							return 0;
						static int nPosMeth2 = pContSt->FindMethod("Insert");
						if (nPosMeth2 == -1)
							return 0;
						CValue vKey;
						CValue vVal;
						CValue vRoutine(1L);
						CValue *parms[3];
						parms[0] = &vKey;
						parms[1] = &vVal;
						parms[2] = &vRoutine;

						vKey = "Имя";
						vVal = pType->GetName();
						pContSt->CallAsProc(nPosMeth2, parms);
						vKey = "Тип";
						vVal = pType->GetType();
						pContSt->CallAsProc(nPosMeth2, parms);
						vKey = "Значение";
						vVal = pType->GetValue();
						pContSt->CallAsProc(nPosMeth2, parms);
						vKey = "Режим";
						vVal = pType->IsByVal() ? "ПоЗначению" : "ПоСылке";
						pContSt->CallAsProc(nPosMeth2, parms);
						CValue val1;
						CValue val2;
						CValue* Par[2];
						Par[0] = &ValSt;
						Par[1] = &val2;
						val2 = pType->GetName();
						pCont->CallAsProc(nPosMeth, Par);
#ifndef _DEBUG
						pContSt->DecrRef(); // TODO проще исправить на смарт-пойнтер CBLPtr или CSafeContextPtr
#endif

					}
				}
			}
		}
		break;
	case methGetVersion:
		{
			CString strVersion = ::GetVersionField("FileVersion");
			strVersion.Replace(',','.');
			strVersion.Remove(' ');
			*ppValue[0] = strVersion;

			UINT nVersion = 0;
			nVersion |= atol(strtok(strVersion.LockBuffer(), "."));
			for (int i = 0; i < 3; i++)
			{
				nVersion <<= 8;
				nVersion |= atol(strtok(NULL, "."));
			}
			rValue = nVersion;
		}
		break;
	case methGetFileVersion:
		{
			CString filePath = ppValue[0]->GetString();
			CString fieldName = ppValue[1]->GetString();
			if (filePath.IsEmpty())
			{
				extern HINSTANCE hDllInstance;
				std::vector<char> vectorOfBuffer(MAX_PATH); //LPTSTR buffer =  new char[MAX_PATH];
				LPTSTR buffer = &vectorOfBuffer[0];
				::GetModuleFileName(hDllInstance, buffer, MAX_PATH);
				filePath = buffer; // путь к ВК
				//delete [] buffer;
			}
			if (fieldName.IsEmpty())
				fieldName = "FileVersion";

			try{
				CString sVersion = ::GetVersionField(filePath, fieldName);
				rValue = sVersion;
			}
			catch(CFileException exception)
			{
				RuntimeError("Не удалось получить доступ к файлу "+filePath);
			}
			catch(...)
			{
				RuntimeError("Неизвестная ошибка при попытке получения версии файла "+filePath);
			}
		}
		break;
	default:
		{

		}
	};
	return TRUE;
}

int  CCreateMetaInfoClasses::CallAsProc(int iMethNum,CValue * * ppValue)
{
	switch(iMethNum)
	{
	case procReadDefinesOfClasses:
		{
			CString strDefFileNameOrDefinitionString   = ppValue[0]->GetString();
			if (strDefFileNameOrDefinitionString.IsEmpty())
				RuntimeError("Укажите имя файла или непосредственно строку с описанием класса!");

			CString DefinitionString;
			if (FileExists(strDefFileNameOrDefinitionString)){
				TRY
				{
					ReadFileToStringWithExceptions(strDefFileNameOrDefinitionString, DefinitionString);
				}
				CATCH(CFileException, pEx)
				{
					RuntimeError("Не удалось открыть файл определения классов:%s", strDefFileNameOrDefinitionString.operator LPCTSTR());
				}
				AND_CATCH(CMemoryException, pEx)
				{
					ShowMsg("Внимание не хватило памяти для выделения буфера в функции ISetOfHierarchy::ISetOfHierarchy()", mmExclamation);
				}
				END_CATCH
			}
			else{
				DefinitionString = strDefFileNameOrDefinitionString;
				// artbear 
				// если второй параметр пустая строка или не передан, то по умолчанию файлы классов ищутся относительно каталога ИБ !
				// иначе файлы классов ищутся относительно КаталогаИБ, а затем относительно указанного каталога или его подкаталогов
				strDefFileNameOrDefinitionString = ppValue[1]->GetString();
				//if (!strDefFileNameOrDefinitionString.GetLength())
				//	RuntimeError("Не удалось открыть файл определения классов:%s", strDefFileNameOrDefinitionString.operator LPCTSTR());

				if (strDefFileNameOrDefinitionString.Right(1).CompareNoCase("\\"))
					strDefFileNameOrDefinitionString += "\\";
			}

			ISetOfHierarchy* pSetOfHierarchy = ISetOfHierarchy::GetHierarchy();
			if (pSetOfHierarchy)
			{
				pSetOfHierarchy->LoadNewClasssesDefine(DefinitionString, strDefFileNameOrDefinitionString);
			}
		}
		break;
	case procCreateALS:
		{
			CString strPlace   = ppValue[0]->GetString();
			if (strPlace.IsEmpty())
				CBLModule::RaiseExtRuntimeError("Укажите имя файла!",0);

			CString strCompany = ppValue[1]->GetString();

			if (strCompany.IsEmpty())
				CBLModule::RaiseExtRuntimeError("Укажите имя компании!",0);

			CreateALS(strPlace, strCompany);
		}
		break;
	case procTuningALS:
		{
			CString str;
			str = ppValue[0]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameSpace    = str;

			str = ppValue[1]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameClass    = ppValue[1]->GetString();

			str = ppValue[2]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameMethod   = str;

			str = ppValue[3]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameProperty = str;

			str = ppValue[4]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameNames    = str;

			str = ppValue[5]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameParam    = str;

			str = ppValue[6]->GetString();
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_strNameReturns  = str;

		}
		break;
	case procSetFilterClasses:
		{

			CString str   = ppValue[0]->GetString();
			str.TrimLeft(); str.TrimRight();
			if (str.IsEmpty())
				m_filterClasses.RemoveAll();
			else
				m_filterClasses.FillSeparateString(str);
		}
		break;
	case procSetFilterNamespace:
		{
			CString str   = ppValue[0]->GetString();
			str.TrimLeft(); str.TrimRight();
			if (str.IsEmpty())
				m_filterNamespace.RemoveAll();
			else
				m_filterNamespace.FillSeparateString(str);
		}
		break;
	case procGetCurSettings:
		*ppValue[0] = pMainAddIn->m_EnableOptimizate;
		*ppValue[1] = pMainAddIn->m_EnabledCheckType;
		*ppValue[2] = pMainAddIn->m_EnableDebugging;
		break;
	case procSetCurSettings:
		{
			// kms fix: [bug 2308]
			int nPar = ppValue[0]->GetNumeric().operator long();
			if (nPar == 1 || nPar == 0)
				pMainAddIn->Property["EnableOptimizate"] = nPar;

			nPar = ppValue[1]->GetNumeric().operator long();
			if (nPar == 1 || nPar == 0)
				pMainAddIn->Property["EnabledCheckType"] = nPar;

			nPar = ppValue[2]->GetNumeric().operator long();
			if (nPar == 1 || nPar == 0)
				pMainAddIn->Property["EnableDebugging"] = nPar;
		}
		break;
	case procClearReposClasses:
		{
			CComponentClass::ClearOptimizationData();
		}
		break;
	case procClearClassOptimizationData:
		{
			CComponentClass::ClearOptimizationData();
		}
		break;
	default:
		{
		}
	};
	return TRUE;
}


int  CCreateMetaInfoClasses::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	return -1;
}

char const *  CCreateMetaInfoClasses::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CCreateMetaInfoClasses::GetNMethods(void)const
{
	return lastMethod;
}

int  CCreateMetaInfoClasses::HasRetVal(int iMethodNum)const
{
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CCreateMetaInfoClasses::GetNParams(int iMethodNum)const
{
	return defFnNames[iMethodNum].NumberOfParams;
}

int  CCreateMetaInfoClasses::GetParamDefValue(int iMethodNum,int iParamNum,CValue * pDefValue)const
{
	switch(iMethodNum)
	{
	case methGetVersion:
		{
			pDefValue->Reset();
			return 1;
		}
		break;
	case methGetFileVersion:
		{
			if (iParamNum == 0 || iParamNum == 1)
			{
				*pDefValue = "";
				return 1;
			}
		}
		break;
	case procReadDefinesOfClasses:
		{
			if (1 == iParamNum)
			{
				*pDefValue = "";
				return 1;
			}
		}
		break;
	default:
		{
		}
	};
	return 0;
}



void  CCreateMetaInfoClasses::DecrRef(void)
{
	CBLContext::DecrRef();
}


char const *  CCreateMetaInfoClasses::GetCode(void)const
{
	return 0;
}

int  CCreateMetaInfoClasses::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CCreateMetaInfoClasses::GetExactValue(CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CCreateMetaInfoClasses::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CCreateMetaInfoClasses::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}




long  CCreateMetaInfoClasses::GetTypeID(void)const
{
	return 100;
}

char const *  CCreateMetaInfoClasses::GetTypeString(void)const
{
	return "MetaInfoClasses";
}

CType   CCreateMetaInfoClasses::GetValueType(void)const
{
	CType tType(100);
	return tType;
}


void  CCreateMetaInfoClasses::IncrRef(void)
{
	CBLContext::IncrRef();
}

//void  CCreateMetaInfoClasses::InitObject(CType const & tType)
//{
//	CBLContext::InitObject(tType);
//}
//
//void  CCreateMetaInfoClasses::InitObject(char const * strName)
//{
//	CBLContext::InitObject(strName);
//}

int  CCreateMetaInfoClasses::IsExactValue(void)const
{
	return 0;
}

int  CCreateMetaInfoClasses::IsOleContext(void)const
{
	return 0;
}

int  CCreateMetaInfoClasses::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CCreateMetaInfoClasses::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CCreateMetaInfoClasses::IsSerializable(void)
{
	return 0;
}

int  CCreateMetaInfoClasses::SaveToString(CString & csStr)
{
	return 0;
}

void  CCreateMetaInfoClasses::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CCreateMetaInfoClasses::GetNProps(void)const
{
	return 0;
}

char const *  CCreateMetaInfoClasses::GetPropName(int A,int B)const
{
	return NULL;
}

int  CCreateMetaInfoClasses::GetPropVal(int iPropNum,CValue & rValue)const
{
	return 0;
}

int  CCreateMetaInfoClasses::SetPropVal(int iPropNum,CValue const & vValue)
{
	return 0;
}

int  CCreateMetaInfoClasses::FindProp(char const * Name)const
{
	return 0;
}
