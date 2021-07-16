// SampleDoc.cpp : implementation of the CModuleDoc class
//

#include "stdafx.h"
#include "resource.h"
//#include "Sample.h"

#include "ModuleDoc.h"
//#include "FormObject.h"
#include "Compiler/ProcUnit.h"
#include "EditForm/FormEditor.h"
#include "ModuleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MetadataTree.h"
#include "MetedataDocument.h"
#include "ChildFrm.h"


/////////////////////////////////////////////////////////////////////////////
// CModuleDoc

IMPLEMENT_DYNCREATE(CModuleDoc, CMetaDocument)

BEGIN_MESSAGE_MAP(CModuleDoc, CMetaDocument)
	//{{AFX_MSG_MAP(CModuleDoc)
	ON_COMMAND(ID_READ_ONLY, OnReadOnly)
	ON_UPDATE_COMMAND_UI(ID_READ_ONLY, OnUpdateReadOnly)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ENTERPRISE, OnUpdateButtonEnterprise)
	ON_COMMAND(ID_BUTTON_COMPILE, OnButtonCompile)
	ON_COMMAND(ID_BUTTON_RUN, OnButtonRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc construction/destruction

#pragma warning(disable:4355)
CModuleDoc::CModuleDoc() : m_xTextBuffer(this)
{
	// TODO: add one-time construction code here

	//	Initialize LOGFONT structure
	memset(&m_lf, 0, sizeof(m_lf));
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfHeight=-14;
	//m_lf.lfHeight= -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	m_lf.lfCharSet = DEFAULT_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(m_lf.lfFaceName, "Courier");


	m_xTextBuffer.pParentDoc=this;
	nCanDefualtType=0;

	SetTree(afxMetaTree);
	SetObj(afxMetaObj);
}

CModuleDoc::~CModuleDoc()
{
}

void CModuleDoc::SetModifiedFlag( BOOL bModified  )
{
	CMetaDocument::SetModifiedFlag(bModified);
	UpdateAllViews(0);
}


BOOL CModuleDoc::OnNewDocument()
{
	if (!CMetaDocument::OnNewDocument())
		return FALSE;
	m_xTextBuffer.InitNew();
	return TRUE;
}


void CModuleDoc::SetDefaultText()
{
	CString StrDefualt=GetDefaultText(nCanDefualtType);
	if(!StrDefualt.IsEmpty())
		SetText(StrDefualt);
} 
 

CString CModuleDoc::GetDefaultText(int nTypeText)
{
	CString StrDefualt;
	if(nTypeText==DEFAULT_FORM_MODULE)
	{
StrDefualt="\
///////////////////////////////////////////////////////////////////////\n\
//Предопределенная процедура - вызывается при первом открытии формы\n\
Процедура ПриОткрытии()\n\
	//глВосстановитьЗначенияФормы(Контекст);\n\
КонецПроцедуры\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Предопределенная процедура - вызывается при закрытии формы\n\
Процедура ПриЗакрытии()\n\
//	Если СостояниеКлавиши(27) Тогда\n\
//		СтатусВозврата(0);\n\
//	КонецЕсли;\n\
//	глЗапомнитьЗначенияФормы(Контекст);\n\
КонецПроцедуры\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Предопределенная процедура - вызывается при повторном открытии формы\n\
Процедура ПриПовторномОткрытии()\n\
КонецПроцедуры\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Предопределенная процедура - вызывается при начале выбора значения\n\
Процедура ПриНачалеВыбораЗначения(Идентификатор,ФлагСтандартнойОбработки)\n\
КонецПроцедуры\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Предопределенная процедура - вызывается при окончании выбора значения\n\
Процедура ОбработкаВыбораЗначения(Значение,Идентификатор,ФлагСтандартнойОбработки)\n\
КонецПроцедуры\n\
\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Процедура обработки нажатия кнопки Закрыть - выполняет закрытие формы\n\
Процедура ЗакрытьФорму()\n\
	Форма.Закрыть();\n\
КонецПроцедуры\n\
\n\
///////////////////////////////////////////////////////////////////////\n\
//Процедура обработки нажатия кнопки Выполнить\n\
Процедура Выполнить()\n\
\n\
КонецПроцедуры\n";

	}
	else
	if(nTypeText==DEFAULT_OBJ_RUN_MODULE)
	{
StrDefualt="\
//******************************************************************************************\n\
//Атрибуты объекта доступные через точку (переменные с признаком Экспорт)\n\
//******************************************************************************************\n\
//...\n\
\n\
//******************************************************************************************\n\
//Внутренние переменные\n\
//******************************************************************************************\n\
//...\n\
\n\
//******************************************************************************************\n\
//Предопределенные процедуры и функции объекта\n\
//******************************************************************************************\n\
\n\
//___________________________________________________________________________________________\n\
//Динамическое получение атрибутов\n\
Функция ПолучитьАтрибут(Имя) Экспорт\n\
\n\
КонецФункции\n\
\n\
//___________________________________________________________________________________________\n\
//Динамическое задание атрибутов\n\
Процедура УстановитьАтрибут(Имя,Значение) Экспорт\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
Функция ПолучитьИмяАтрибута(Номер) Экспорт\n\
\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
Функция ПолучитьКоличествоАтрибутов() Экспорт\n\
\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
//Установка внутреннего идентификатора (номера в БД) объекта \n\
//(или условное позиционирование объекта)\n\
Процедура УстановитьИдентификаторОбъекта(НовИдентификатор)Экспорт\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
//Получение внутреннего идентификатора объекта \n\
Функция ПолучитьИдентификаторОбъекта() Экспорт\n\
\n\
КонецФункции\n\
\n\
""//___________________________________________________________________________________________\n\
//Строковое представление объекта\n\
Функция ПолучитьПредставлениеОбъекта() Экспорт\n\
\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
//Признак не пустого значения\n\
Функция Выбран()Экспорт\n\
	Возврат 1;\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
//Строковое значение типа объекта\n\
Функция ПолучитьТипОбъекта()Экспорт\n\
	 //Возврат \"Справочник\";\n\
КонецФункции\n\
\n\
//___________________________________________________________________________________________\n\
//Конструктор (вызывается при инициализации объекта)\n\
//ВидОбъекта - вид объекта, задающийся строкой следуемой за символом \".\"\n\
//Параметр - произвольный параметр (для визуальных элементов - объект базового класса)\n\
Процедура Конструктор(ВидОбъекта,Параметр)Экспорт\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
//Деструктор (вызывается при удалении объекта)\n\
//Процедура Деструктор()Экспорт\n\
//\n\
//КонецПроцедуры\n\
\n\
\n\
\n\
\n\
""//___________________________________________________________________________________________\n\
//Обрабочик событий визуального объекта\n\
//Возвращает:\n\
//0 - событие не обработано (передается в форму для дальнейшей обработки)\n\
//1 - событие обработано (далее в форму не передается)\n\
//ИмяСобытия - имя события как оно задано в окне свойств конфигуратора\n\
Функция ОбработкаСобытия(ИмяСобытия) Экспорт\n\
\n\
	Возврат 0;\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
//Открытие стандартной формы редактирования объекта\n\
Функция ОткрытьФормуОбъекта(Парам1,Парам2) Экспорт\n\
	Возврат 0;//0 - форма отсутствует\n\
КонецФункции\n\
//___________________________________________________________________________________________\n\
//Вызывается при открытии любой подчиненной формы\n\
Процедура ОбработкаОткрытияФормы(Конт) Экспорт\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
//Вызывается при закрытии любой подчиненной формы\n\
Процедура ОбработкаЗакрытияФормы(Конт,НомерВызова) Экспорт\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
//Вызывается при попытке присвоить значение диалоговому элементу или запрещенной для записи переменной\n\
Процедура УстановитьЗначение(Значение) Экспорт\n\
\n\
КонецПроцедуры\n\
\n"\
"\
//___________________________________________________________________________________________\n\
//Возвращает текущий вид объекта (вызывается при выгрузке значения в строку)\n\
Функция Вид() Экспорт\n\
\n\
	Возврат \"\";\n\
КонецФункции\n\
//******************************************************************************************\n\
//Методы объекта доступные через точку (процедуры и функции с признаком Экспорт)\n\
//******************************************************************************************\n\
//Процедура Метод1() Экспорт\n\
//\n\
//КонецПроцедуры\n\
\n\
\n\
//******************************************************************************************\n\
//Внутренние процедуры и функции\n\
//******************************************************************************************\n\
\n\
";
	}
	else
	if(nTypeText==DEFAULT_OBJ_COMMON_MODULE)
	{
StrDefualt="\
//******************************************************************************************\n\
//Предопределенные процедуры и функции в режиме Предприятия\n\
//******************************************************************************************\n\
Процедура ПриНачалеРаботыСистемы()\n\
\n\
КонецПроцедуры\n\
\n\
Процедура ПриЗавершенииРаботыСистемы()\n\
\n\
КонецПроцедуры \n\
\n\
\n\
//******************************************************************************************\n\
//Предопределенные процедуры и функции в режиме Конфигуратора\n\
//******************************************************************************************\n\
Процедура ПриНачалеРаботыКонфигуратора()\n\
\n\
КонецПроцедуры \n\
\n\
\n\
Процедура ПриЗавершенииРаботыКонфигуратора()\n\
\n\
КонецПроцедуры \n\
\n\
\n\
";
	}
	else
	if(nTypeText==DEFAULT_OBJ_CONF_MODULE)
	{
StrDefualt="\
//******************************************************************************************\n\
//Предопределенные процедуры и функции в режиме Конфигуратора\n\
//******************************************************************************************\n\
Процедура ПриНачалеРаботыКонфигуратора()\n\
\n\
КонецПроцедуры \n\
\n\
Процедура ПриЗавершенииРаботыКонфигуратора()\n\
\n\
КонецПроцедуры \n\
\n\
//___________________________________________________________________________________________\n\
//Процедура вызывается при открытии дерева метаданных\n\
//Здесь долен быть прописан код по заполнению элементов дерева объекта\n\
//Дерево - объект типа ДеревоЗначений\n\
//ПапкаМетаданных - элемент дерева, который является родителем для данного объекта\n\
//Дерево:ДеревоЗначений\n\
Процедура ПриЗагрузкеКонфигурации(Дерево,ПапкаМетаданных)\n\
\n\
КонецПроцедуры\n\
\n\
//___________________________________________________________________________________________\n\
//Процедура вызывается при записи метаданных (сохранение метаданных производится автоматически)\n\
//Здесь должен быть прописан код по сохранению дополнительных параметров\n\
//или по выполнении реструктризации данных в случае такой необходимости\n\
//Дерево:ДеревоЗначений\n\
Процедура ПриСохраненииКонфигурации(Дерево)\n\
\n\
КонецПроцедуры\n\
\n\
";
	}
	return StrDefualt;
}




/////////////////////////////////////////////////////////////////////////////
// CModuleDoc serialization

void CModuleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc diagnostics

#ifdef _DEBUG
void CModuleDoc::AssertValid() const
{
	CMetaDocument::AssertValid();
}

void CModuleDoc::Dump(CDumpContext& dc) const
{
	CMetaDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc commands

void CModuleDoc::DeleteContents() 
{
	CMetaDocument::DeleteContents();

	m_xTextBuffer.FreeAll();
}

CFormEditor *CModuleDoc::GetForm() 
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		CRuntimeClass *pClass=pView->GetRuntimeClass();
		CString csClassName=pClass->m_lpszClassName;
		if(csClassName=="CFormEditor")//Чтение содержимого формы
		{
			return (CFormEditor *)pView;
		}
	}
	return 0;
}
CMyCodeView *CModuleDoc::GetModule() 
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		CRuntimeClass *pClass=pView->GetRuntimeClass();
		CString csClassName=pClass->m_lpszClassName;
		if(csClassName=="CMyCodeView"||csClassName=="CModuleView")
		{
			return (CMyCodeView *)pView;
		}
	}
	return 0;
}

BOOL CModuleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CMetaDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CString Str=lpszPathName;

	if(!GetTree())
	if(GetViewCount()>1)//это режим внешней обработки
	{
		CZipArchive m_zip;
		try
		{
			m_zip.Open(Str, CZipArchive::openReadOnly);
		}
		catch(...)
		{
			AfxMessageBox(CString("Ошибка открытия файла ")+Str);
			return FALSE;
		}


		if(!GetObj())
			SetObj(new CMetaObject());
		bDeleteObj=1;


	//	GetObj()->GetForm(m_zip,FORMNAME);
		
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			CRuntimeClass *pClass=pView->GetRuntimeClass();
			CString csClassName=pClass->m_lpszClassName;
			if(csClassName=="CMyCodeView")//Чтение текста модуля
			{
				CString csAlias;
				CString csComment;
				GetObj()->csFile=ReadFileToString(m_zip,MODULENAME,csAlias,csComment);
			}
			else
			if(csClassName=="CMyDescriptionView")//Чтение текста описания
			{
				CString csAlias;
				CString csComment;
				GetObj()->csComment=ReadFileToString(m_zip,DESCRIPNAME,csAlias,csComment);
				//AfxMessageBox(GetObj()->csComment);

			}
			else
			if(csClassName=="CFormEditor")//Чтение содержимого формы
			{
				CString csAlias;
				CString csComment;
				GetObj()->csForm=ReadFileToString(m_zip,FORMNAME,csAlias,csComment);
			}
		}

		m_zip.Close();	
		return TRUE;
	}


	return m_xTextBuffer.LoadFromFile(lpszPathName);
}
void CModuleDoc::GetAllData(CString &csModule,CString &csForm,CString &csDescription)
{
	csModule=GetText();
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		CRuntimeClass *pClass=pView->GetRuntimeClass();
		CString csClassName=pClass->m_lpszClassName;
		if(csClassName=="CMyDescriptionView")//текст описания
		{
			((CMyDescriptionView*)pView)->GetWindowText(csDescription);
			//SetModifiedFlag(0);
		}
		else
		if(csClassName=="CFormEditor")//Запись содержимого формы
		{
			((CFormEditor*)pView)->GetWindowText(csForm);
		}
	}
}

BOOL CModuleDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString Str=lpszPathName;

	if(!GetTree())
	if(GetViewCount()>1)//это режим внешней обработки
	{
		CZipArchive m_zip;
		m_zip.Open(Str, CZipArchive::create,0);//Новый архив
		m_zip.EnableFindFast(1);

		CString csModule;
		CString csForm;
		CString csDescription;
		GetAllData(csModule,csForm,csDescription);

		WriteFileFromString(csModule,m_zip,MODULENAME,"","");
		WriteFileFromString(csForm,m_zip,FORMNAME,"","");
		WriteFileFromString(csDescription,m_zip,DESCRIPNAME,"","");

		m_zip.Close();	
		SetModifiedFlag(0);
		return TRUE;
	}

	if((!GetTree())||(!Str.IsEmpty()))
	{
		if(mUpper(Str.Right(3))=="HTM"||mUpper(Str.Right(4))=="HTML")
		{
			GetModule()->SaveAsHtml(lpszPathName);
		}
		else
			m_xTextBuffer.SaveToFile(lpszPathName);
	}
	return TRUE;
}

void CModuleDoc::OnReadOnly() 
{
	if (! m_xTextBuffer.GetReadOnly())
	{
		m_xTextBuffer.SetReadOnly(TRUE);
		//AfxMessageBox("Document now read-only!");
	}
	else
		m_xTextBuffer.SetReadOnly(FALSE);
}

void CModuleDoc::OnUpdateReadOnly(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_xTextBuffer.GetReadOnly());
}


char *LoadFromFileBin(CString sFileName,int &size);

BOOL CModuleDoc::SaveModified() 
{
	if(GetTree())
	{
		if(GetViewCount()>1)//это режим формы в составе конфигурацмм
			GetAllData(GetObj()->csFile,GetObj()->csForm,GetObj()->csComment);
		else
			GetObj()->csFile=GetText();//это режим модуля в составе конфигурацмм
		return TRUE;
	}
	else//это самостоятельная форма или модуль
	{
		//if(CRichEditDoc::IsModified())
		return CDocument::SaveModified();
	}
	return 1;
}

void WritoToFile(CString csCFile,CString Str);

void CModuleDoc::SetText(CString csStr)
{
/*	
	csStr.Replace("\n\r","\n");
	//csStr.Replace("\n","\n\r");
	m_xTextBuffer.InitNew();
	int nEndLine;
	int nEndChar;
	m_xTextBuffer.InsertText(0, 0, 0, csStr, nEndLine, nEndChar, CE_ACTION_UNKNOWN);
//*/
//*

	CString csTemp;
	GetTempPath(1024,csTemp.GetBuffer(1024));
	csTemp.ReleaseBuffer();
	csTemp+="temp.2c";

	WritoToFile(csTemp,csStr);
	m_xTextBuffer.FreeAll();
	m_xTextBuffer.LoadFromFile(csTemp);
	DeleteFile(csTemp);
//*/
}
CString CModuleDoc::GetText()
{
	CString csRes;
	if(m_xTextBuffer.GetLineCount()>0)
		m_xTextBuffer.GetText(0,0,m_xTextBuffer.GetLineCount()-1,m_xTextBuffer.GetLineLength(m_xTextBuffer.GetLineCount()-1) , csRes);
	return csRes;
}

CString CModuleDoc::GetModulePath()
{
	CString csName;
	if(GetObj())
		csName=GetObj()->csPrevFileName;
	if(csName.IsEmpty())
		csName=GetPathName();
	return csName;
}

void CModuleDoc::Run(bool bRun) 
{
	CString Str=GetText();

	CCompileModule cModule;
	CProcUnit Run;
	try
	{
		if(!bRun)
			ClearMessageWindow();
		if(afxAppRunMode==ENTERPRISE_MODE)
		{
			CCompileModule *p;
			CCompileModule *pParent=m_RunModuleManager.GetParent("",p,p);
			CProcUnit *pRunParent=m_RunModuleManager.GetRunParent(0);
			cModule.SetParent(pParent);

			CString csName=GetModulePath();

			cModule.csModuleName=csName;
			cModule.Compile(Str);
			if(bRun)
			{
				Run.SetParent(pRunParent);
				Run.Exec(cModule.cByteCode);
			}
			else
			{
				Message(String("Синтаксических ошибок не обнаружено"));
			}
			return;
		}

		CCompileModule *pParent=0;

		CModuleManager* pOld=AfxGetModuleManager();
		CModuleManager *pNewModuleManager=new CModuleManager();
		delete pNewModuleManager->pMetadata;
		pNewModuleManager->pMetadata=pOld->pMetadata;
 
		if(AfxGetMetadataTree())
		{
			AfxGetMetadataTree()->GetAllData();
			pNewModuleManager->Load(0);//здесь компилируются общие модули конфигурации
		}

		cModule.csModuleName=GetModulePath();

		CCompileModule *pStopParent;
		CCompileModule *pContinueParent;
		pParent=pNewModuleManager->GetParent(cModule.csModuleName,pStopParent,pContinueParent);
		cModule.SetParent(pParent,pStopParent,pContinueParent);

		CFormEditor *pForm=GetForm();
		if(pForm)//это режим формы - добавляем внешние переменные
		{
			for(int i=0;i<pForm->m_pDialog->aControlList.GetSize();i++)
			{
				CDynControl *pControl=pForm->m_pDialog->aControlList[i];
				if(!pControl->m_csName.IsEmpty())
					cModule.AddVariable(pControl->m_csName,0);//объявление диалоговых элементов как внешних переменных
			}
			cModule.AddVariable(FORM_NAME,0);//объявление переменной для доступа к форме
		}

		pNewModuleManager->SetAttrVariable(&cModule);

		cModule.Compile(Str);
		pNewModuleManager->pMetadata=0;
		delete pNewModuleManager;
		Message(String("Синтаксических ошибок не обнаружено"));
	}
	catch(CTranslateError *)
	{
		DoError();
		return;
	};
	
}


void CModuleDoc::OnButtonRun() 
{
	if(afxAppRunMode==ENTERPRISE_MODE)
	{
		Run(1);
	}
	else
	{
		CMyCodeView *pModule=GetModule();
		if(pModule)
		{
			pModule->OnRunDebug();
		}

	}
}


void CModuleDoc::OnUpdateButtonEnterprise(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTree()!=0);
}

void CModuleDoc::OnButtonCompile() 
{
	Run(0);
}

int CModuleDoc::GetViewCount() 
{
	int nCount=0;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		nCount++;
	}
	return nCount;
}

BOOL CModuleDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		CEnterpriseApp *p=(CEnterpriseApp*)AfxGetApp();
		if (!p->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName,1);

	return TRUE;        // success
}

