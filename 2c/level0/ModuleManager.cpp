// ModuleManager.cpp: implementation of the CModuleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "ModuleManager.h"
#include "MetadataTree.h"
#include "FormUnit.h"
#include "DialogUnit.h"
#include "Compiler/ValueArray.h" 
#include "Compiler/ValueObject.h"
#include "OpenForm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int RegisterEvent(CString csEventName);
CString LoadFromFile(CString csFileName);
void SortList(CArray <CString,CString&>  &List);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int afxEventMessage;
CProcUnit *afxpRunInit=0;
int AfxCallInitProc(CString csName,CValue Param,CValue &vDeleteText,CValue &vInsertText)
{
	if(afxpRunInit)
	{
		try
		{
			afxEventMessage=1;
			CValue Ret=afxpRunInit->CallFunction(csName,Param,vDeleteText,vInsertText);
			afxEventMessage=0;
			return Ret;
		}
		catch(CTranslateError *)
		{
			DoError();
		};
	}
	return 0; 
}

CModuleManager* AfxGetModuleManager()
{
	return &m_RunModuleManager;
	//return afx_ModuleManager;
};

CModuleManager::CModuleManager()
{
	pTree=0;
	pCommonParent=0;
	pCommonRunParent=0;
	pMetadata=new CMetadata();
}

CModuleManager::~CModuleManager()
{
	if(pMetadata)
		delete pMetadata;
	pMetadata=0;
	Clear();
}

BOOL CModuleManager::IsCanDestroy()
{
	nReturnStatus=1;
	//все общие модули
	for(int nMode=3;nMode>=1;nMode--)//запуск по приоритетам
	for(int k=0;k<aRunModules.GetSize();k++)
	if(nMode==aRunModules[k]->nCommonModule)
	{
		if(afxAppRunMode==ENTERPRISE_MODE)
			aRunModules[k]->CallFunction("ПриЗавершенииРаботыСистемы");
		else
		if(afxAppRunMode==CONFIG_MODE)
			aRunModules[k]->CallFunction("ПриЗавершенииРаботыКонфигуратора");

		if(nReturnStatus==0)//если хотя бы один модуль не разрешил, то не выходим из программы
			return 0;
	}
	return 1;
}

void CModuleManager::ClearCompileList(CMapStringToPtr &List)
{
	CCompileModule *p;
	POSITION pos;
	CString csKey;
	for( pos = List.GetStartPosition(); pos != NULL; )
	{
		List.GetNextAssoc( pos, csKey, (void*&)p );
		if(p)
			delete p;
	}
	List.RemoveAll();
}

void CModuleManager::Clear()
{
	//Удаляем все второстепенные запущенные общие модули
	CProcUnit *p;
	POSITION pos;
	CString csKey;
	for( pos = ListRunModules.GetStartPosition(); pos != NULL; )
	{
		ListRunModules.GetNextAssoc( pos, csKey, (void*&)p );
		for(int k=0;k<aRunModules.GetSize();k++)
		{
			CProcUnit *p2=aRunModules[k];
			if(p==p2)//это общий модуль - его удалим потом
			{
				p=0;
				break;
			}
		}

		if(p)
			delete p;
	}

	//Удаляем общие модули объектов и конфигурации в обратном порядке запуска
	for(int k=aRunModules.GetSize()-1;k>=0;k--)
	//for(int k=0;k<aRunModules.GetSize();k++)
	{
		try
		{
			delete aRunModules[k];
		}
		catch(...)
		{
			CProcUnit *p2=aRunModules[k];
		};
	}

	ClearCompileList(ListModules);//Удаляем все скомилированные модули
	ListRunModules.RemoveAll();
	aRunModules.RemoveAll();
	pCommonRunParent=0;
}

//извлечение модуля конфигурации (если еще не скомилирован - то производится компиляция)
CCompileModule *CModuleManager::GetCompileModule(CString csModuleName,CCompileModule *pSetParent,CCompileModule *pSetStopParent,BOOL bRetZero)
{
	//Message(CValue(csModuleName));
	CCompileModule *pModule=(CCompileModule *)ListModules[mUpper(csModuleName)];
	if(pModule)//ранее был скомпилирован
		return pModule;

	//создание и компилирование
	CMetaObject *pObject=pMetadata->GetMetaObject(csModuleName,"",1);
	CString csCompile;
	if(pObject)
		csCompile=pObject->csFile;
	else
		if(bRetZero)
			return 0;

	pModule=new CCompileModule();
	ListModules[mUpper(csModuleName)]=pModule;

	CCompileModule *pContinueParent=0;
	if(!pSetParent)
	{
		pSetParent=GetParent(csModuleName,pSetStopParent,pContinueParent);
	}
	
	pModule->SetParent(pSetParent,pSetStopParent,pContinueParent);

	pModule->csModuleName=csModuleName;
	SetAttrVariable(pModule);//добавляем переменные - атрибуты


/*	//если это общий модуль объекта
	if(csModuleName.Left(OBJECTSNAME.GetLength())==OBJECTSNAME)
	if(mUpper(csModuleName).Find(mUpper("\\"+OBJMODULE+"\\"))>0)
	if(csModuleName.Right(ENTERPRISEMODULENAME.GetLength())!=ENTERPRISEMODULENAME)
	{
		pModule->cContext.bStaticVariable=1;
	}*/

	try
	{
		pModule->Compile(csCompile);
	}
	catch(CTranslateError *err)
	{
		ListModules[mUpper(csModuleName)]=0;
		throw (err); 
	};

	return pModule;
}

//загрузка и инициализация конфигурации (компиляция общих модулей и макрообъектов конфигурации)
void CModuleManager::Load(BOOL bRunFunction)
{
	int i;
	Clear();
/*
	if(!csFileName.IsEmpty())//иначе - перезагрузка модулей из контейнера (при условии что первая загрузка уже была)
	{
		pMetadata->Load(csFileName);
	}
*/
	pCommonRunParent=0;
	pCommonParent=0;
	CCompileModule *pCurParent=0;
	CProcUnit *pRunCurParent=0;


	//Модуль инициализации системы
	try
	{
		pCurParent=GetCompileModule(MODULESNAME+"\\"+INITMODULENAME,0);
		ASSERT(pCurParent);
		pCurParent->nCommonModule=1;//тип общего модуля и приоритет запуска
		if(bRunFunction)
		{
			pRunCurParent=GetRunModule(MODULESNAME+"\\"+INITMODULENAME,0,0);
			afxpRunInit=pRunCurParent;
			aRunModules.Add(afxpRunInit);
		}

		//компиляция и проецирование в памяти общих модулей конфигурации
		//(код модуля, не входящий в функции и процедуры игнорируется)
		for(i=0;i<pMetadata->ListModuleName.GetSize();i++)
		{
			CString csName(pMetadata->ListModuleName[i]);

			pCurParent=GetCompileModule(MODULESNAME+"\\"+csName,pCurParent);
			ASSERT(pCurParent);
			pCurParent->nCommonModule=3;//приоритет запуска
			if(bRunFunction)
			{
				pRunCurParent=GetRunModule(MODULESNAME+"\\"+csName,0,pRunCurParent);
				aRunModules.Add(pRunCurParent);
			}
		}




		pCommonParent=pCurParent;
		pCommonRunParent=pRunCurParent;
		CCompileModule *pStopParent=0;//общие модули конфигурации для объектов не доступны
		//компиляция и проецирование общих модулей объектов
		for(i=0;i<pMetadata->ListObjectName.GetSize();i++)
		{
			CString csName(pMetadata->ListObjectName[i]);

			//регистрация макрообъектов(на класс - менеджер макрообъектов: CValueObject)
			if(afxAppRunMode==ENTERPRISE_MODE)
				RegisterObject(RUNTIME_CLASS(CValueObject),csName);

			//Общие модули объектов
			ObjectDescription *pObject;
			pObject=(ObjectDescription *)pMetadata->OList[mUpper(csName)];
			CCompileModule *ObjParent=pCurParent;
			CProcUnit *ObjRunParent=pRunCurParent;
			if(pObject)
			{
				SortList(pObject->aModule);

				for(int m=0;m<pObject->aModule.GetSize();m++)
				{
					CString csPath(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+pObject->aModule[m]);
					ObjParent=GetCompileModule(csPath,0,0);
					ASSERT(ObjParent);
					ObjParent->nCommonModule=2;//приоритет запуска
					if(bRunFunction)
					{
						ObjRunParent=GetRunModule(csPath,0,0);
						if(ObjRunParent)//запуск предопределенной процедуры общих модулей объектов
						{
							aRunModules.Add(ObjRunParent);
							ObjRunParent->CallFunction("ИнициализацияМодуля");
						}
					}
				}
			}

			if(afxAppRunMode==CONFIG_MODE)
			{
				ObjParent=GetCompileModule(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+CONFIGMODULENAME,ObjParent,0);
				ASSERT(ObjParent);
				ObjParent->nCommonModule=2;//приоритет запуска
				if(bRunFunction)
				{
					//компилируем и проецируем в памяти модуль среды конфигурирования

					ObjRunParent=GetRunModule(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+CONFIGMODULENAME,0,ObjRunParent);

					aRunModules.Add(ObjRunParent);
				}
			}
		}

		//объекты скомилированы полностью - теперь можно запускать модули
		if(bRunFunction)
		{
			//все общие модули
			RegisterEvent("ПриОткрытииФормы");
			/*for(int nMode=1;nMode<=3;nMode++)//запуск по приоритетам
			for(int k=0;k<aRunModules.GetSize();k++)
			if(nMode==aRunModules[k]->nCommonModule)
			{
				CProcUnit *P=aRunModules[k];
				P->CallFunction("ИнициализацияМодуля");
			}*/
			for(int nMode=1;nMode<=3;nMode++)//запуск по приоритетам
			for(int k=0;k<aRunModules.GetSize();k++)
			if(nMode==aRunModules[k]->nCommonModule)
			{
				CProcUnit *P=aRunModules[k];
				if(afxAppRunMode==ENTERPRISE_MODE)
					P->CallFunction("ПриНачалеРаботыСистемы");
				else
				if(afxAppRunMode==CONFIG_MODE)
					P->CallFunction("ПриНачалеРаботыКонфигуратора");
			}
		}
	}
	catch(CTranslateError *p)
	{
		if(bRunFunction==0)
		{
			throw(p);
		}
		DoError();
	};



	if(!pTree)
	if(bRunFunction)
	{
		pTree=OpenFormMDI("Метаданные",1);
		if(pTree)
			pTree->SetTitle(CONFIG_NAME);
	}

	pMetadata->OpenZip(CLOSE_FILE); //временное закрытие
}

static int nObjectLength1=OBJECTSNAME.GetLength();
static int nObjectLength2=METADATANAME.GetLength();
static CString csObjName1=mUpper(OBJECTSNAME);
static CString csObjName2=mUpper(METADATANAME);
CString CModuleManager::GetObjectName(CString csModulePath0,int &nType)
{
	nType=TYPE_NONE;
	csModulePath0.TrimLeft();
	csModulePath0.TrimRight();
	CString csModulePath(csModulePath0);
	csModulePath.MakeUpper();
	if(csModulePath.Left(nObjectLength1+1)==csObjName1+"\\")
		nType=TYPE_OBJECT_CONTEXT;
	else
	if(csModulePath.Left(nObjectLength2+1)==csObjName2+"\\")
		nType=TYPE_METADATA_CONTEXT;

	if(nType)
	{
		int nIndex1=csModulePath.Find('\\');
		int nIndex2=csModulePath.Find('\\',nIndex1+1);
		if(nIndex2>0)
			return csModulePath0.Mid(nIndex1+1,nIndex2-nIndex1-1);
		

	}
	return "";
}
CProcUnit *CModuleManager::GetNewRunModule(CString csModuleName,BOOL bRunModule,CProcUnit *pSetRunParent)
{
	CCompileModule *pModule=GetCompileModule(csModuleName);
	if(!pModule)
		return 0;
	CProcUnit *pRun=new CProcUnit();
	ASSERT(pRun);
	pRun->nCommonModule=pModule->nCommonModule;
	if(!pSetRunParent)
		pSetRunParent=GetRunParent(pModule);
	
//	if(pSetRunParent)
//	if(pSetRunParent->nCommonModule==2)
//		pRun->nAutoDeleteParent=1;

	pRun->SetParent(pSetRunParent);

	//OutToDebug("%s\n",csModuleName);
	pRun->Exec(pModule->cByteCode,bRunModule);
	return pRun;
}
CProcUnit *CModuleManager::GetRunModule(CString csModuleName,BOOL bRunModule,CProcUnit *pSetRunParent)
{
	CProcUnit *pRun=(CProcUnit *)ListRunModules[mUpper(csModuleName)];
	if(pRun)
	{
		return pRun;
	}
	pRun=GetNewRunModule(csModuleName,bRunModule,pSetRunParent);
	ListRunModules[mUpper(csModuleName)]=pRun;
	return pRun;
}


BOOL CModuleManager::FindForm(CString csFormName0,CString csFileName,CMetaObject* &pObject)
{
	return FindObject(csFormName0,csFileName,pObject,OBJFORM,FORMSNAME);
}
CString CModuleManager::GetTable(CString csFileName)
{
	CMetaObject* pObject=0;
	BOOL bRes=FindObject(csFileName,"",pObject,OBJMAKET,MAKETSNAME);

	if(!bRes)
	{
		Error(CString("Ошибка задания имени таблицы: \"")+csFileName+"\"");
	}
	if(!afxFormPath.IsEmpty())
	{
		CString Ret=LoadFromFile(afxFormPath);
		afxFormPath="";
		return Ret;
	}
	ASSERT(pObject);
	return pObject->csFile;
}
CString CModuleManager::GetModule(CString csFileName)
{
	CMetaObject* pObject=0;
	BOOL bRes=FindObject(csFileName,"",pObject,OBJMODULE,MODULESNAME);
	if(!bRes)
	{
		Error(CString("Ошибка задания имени модуля: \"")+csFileName+"\"");
	}
	if(!afxFormPath.IsEmpty())
	{
		return LoadFromFile(afxFormPath);
	}
	ASSERT(pObject);
	return pObject->csFile;
}

BOOL CModuleManager::FindObject(CString csFormName0,CString csFileName,CMetaObject* &pObject,CString csObjGroup,CString csCommonObjGroup)
{
	pObject=0;
	csFormName0.Replace("/","\\");
	if(mUpper(csFormName0.Left(3))=="MD:")//явно заданный полный путь
	{
		if(csFormName0.Mid(3,1)=="\\")
			csFormName0=csFormName0.Mid(4);
		else
			csFormName0=csFormName0.Mid(3);

		pObject=pMetadata->GetMetaObject(csFormName0,"",1);
	}
	else 
	{
		if(!csFormName0.IsEmpty())
		{
			if(csFormName0.Mid(1,1)==":")//локальный диск
				csFileName=csFormName0;
			if(csFormName0.Mid(0,2)=="\\\\")//сетевой путь
				csFileName=csFormName0;
		}
		
		afxFormPath="";
		if(!csFileName.IsEmpty())//явное задание файла
		{
			if(!FileExist(csFileName))
				Error(CString("Не найден файл "+csFileName));
			afxFormPath=csFileName;
			return 1;
		}


		CString csFormName(mUpper(csFormName0));

		//ищем в локальном контексте объекта
		CByteCode *pByteCode=AfxGetCurrentByteCode();
		if(pByteCode)
		if(!pByteCode->csModuleName.IsEmpty())
		{
			CString csObjectPath(pByteCode->csModuleName);
			csObjectPath.TrimRight();
			csObjectPath.TrimRight('\\');

			pObject=pMetadata->GetMetaObject(csObjectPath+"\\"+csObjGroup+"\\"+csFormName,csFormName,1);

			int nIndex=csObjectPath.ReverseFind('\\');
			csObjectPath=csObjectPath.Left(nIndex);
			if(!pObject)
				pObject=pMetadata->GetMetaObject(csObjectPath+"\\"+csObjGroup+"\\"+csFormName,csFormName,1);
			if(!pObject)
				pObject=pMetadata->GetMetaObject(csObjectPath+"\\"+csFormName,csFormName,1);

			nIndex=csObjectPath.ReverseFind('\\');
			csObjectPath=csObjectPath.Left(nIndex);

			if(!pObject)
				pObject=pMetadata->GetMetaObject(csObjectPath+"\\"+csObjGroup+"\\"+csFormName,csFormName,1);
			if(!pObject)
				pObject=pMetadata->GetMetaObject(csObjectPath+"\\"+csFormName,csFormName,1);
		}
		
		//ищем в общих формах
		if(!pObject)
			pObject=pMetadata->GetMetaObject(csCommonObjGroup+"\\"+csFormName,csFormName,1);
/*		for(i=0;i<pMetadata->ListFormName.GetSize();i++)
		{
			CString csName=pMetadata->ListFormName[i];
			if(mUpper(csName)==csFormName)
			{
				pObject=pMetadata->GetMetaObject(csCommonObjGroup+"\\"+csFormName,csFormName,1);
				return 1;
			}
		}
*/
		if(!pObject)//полный путь
			pObject=pMetadata->GetMetaObject(csFormName0,"",1);
		if(!pObject)//неявное задание файла
		{
			if(!FileExist(csFormName0))
				Error(CString("Не найден файл "+csFormName0));
			afxFormPath=csFormName0;
			return 1;
		}


	}
 
	if(pObject)
	{
		return 1;
	}
	else
	{
		Message(String(CString("Объект \"")+csFormName0+"\" не найден"));
		return 0;
	}

}


//Получить родителя для синтаксического контроля
CCompileModule *CModuleManager::GetParent(CString csPath,CCompileModule *&pStopParent,CCompileModule *&pContinueParent,BOOL bEnterpriseModule)
{
	if(csPath.IsEmpty())
		return pCommonParent;

	static int nLevel=0;
	//Message(CValue(nLevel));
	nLevel++;
	if(nLevel>MAX_OBJECTS_LEVEL)
	{
		Message(csPath);
	}
	if(nLevel>2*MAX_OBJECTS_LEVEL)
		Error("Рекурсивный вызов модулей (#1)");

	pStopParent=0;
	pContinueParent=0;
	int nType;
	CString csName(GetObjectName(csPath,nType));
	if(!csName.IsEmpty())
	{
		if(nType==TYPE_METADATA_CONTEXT)
		{
			CCompileModule *pParent=GetCompileModule(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+ENTERPRISEMODULENAME);
			nLevel--;
			return pParent;
		}
		else
		if(nType==TYPE_OBJECT_CONTEXT)
		{
			//pStopParent=pCommonParent; 
			ObjectDescription *pObject;
			pObject=(ObjectDescription *)pMetadata->OList[mUpper(csName)];
			if(pObject)
			{
				CString csParentName;
				for(int m=0;m<pObject->aModule.GetSize();m++)
				{
					CString csCurPath(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+pObject->aModule[m]);
					if(mUpper(csPath)==mUpper(csCurPath))
						break;

					csParentName=pObject->aModule[m];
				}
				if(!csParentName.IsEmpty())
				{
					CCompileModule *pParent=GetCompileModule(OBJECTSNAME+"\\"+csName+"\\"+OBJMODULE+"\\"+csParentName);
					nLevel--;
					return pParent;
				}
				else
				{//это старший модуль либо объект не имеет общих модулей

					CMetaObject *pObject=pMetadata->GetMetaObject(OBJECTSNAME+"\\"+csName,"",1);
					ASSERT(pObject);
					//проверка на наличие родителя
					CString csParent;
					if(pObject)
						csParent=pObject->csParam1;
					if(!csParent.IsEmpty())
					{
						//переподчиняем его модулю объекта-родителя
						CCompileModule *pParent;
						if(bEnterpriseModule)
						{
							pParent=GetCompileModule(OBJECTSNAME+"\\"+csParent+"\\"+OBJMODULE+"\\"+ENTERPRISEMODULENAME);
						}
						else
						{
							pParent=GetCompileModule(OBJECTSNAME+"\\"+csParent+"\\"+OBJMODULE+"\\"+CONFIGMODULENAME);
						}
						ASSERT(pParent);
						if(!pParent)
						{
							Error(CString("Не найден объект ")+csParent+" (ссылка из объекта "+csName+")");
						}
						nLevel--;
						return pParent;
					}

					//родитель - последний глобальный общий модуль конфигурации
					nLevel--;
					return pCommonParent;
				}
			}
		}
		ASSERT(FALSE);
	}
	nLevel--;
	return pCommonParent;
}

//параметры определяем по модулю компиляции
CProcUnit *CModuleManager::GetRunParent(CCompileModule *pChildCompile)
{
	//ASSERT(pChildCompile); 
	if(!pChildCompile)
		return pCommonRunParent;
	else
	if(pChildCompile->pParent==pCommonParent)
		return pCommonRunParent;
	else
	if(pChildCompile->pParent==0)
		return 0;  
	else
	{
		if(pChildCompile->pParent->nCommonModule)
		{
			CProcUnit *pParent=GetRunModule(pChildCompile->pParent->csModuleName);
			if(pParent->nCommonModule==2)
				pParent->SetParent(GetRunParent(pChildCompile->pParent));
			return pParent;
		}
		else
		{
			CProcUnit *pRunRunModule=afxCurrentRunModule;

			if(pRunRunModule&&mUpper(pChildCompile->csModuleName.Left(METADATANAME.GetLength()+1))==mUpper(METADATANAME+"\\"))
			{
				CValueObject *pRef=(CValueObject *)pRunRunModule->cCurContext.pRefLocVars[0];
				if(pRef->GetRuntimeClass()==&CValueObject::classCValueObject)
				{
					return pRef->pRun;
				}
			}

			CProcUnit *pParent=GetNewRunModule(pChildCompile->pParent->csModuleName);
			return pParent;
		}

	}
}


//Возвращает ссылку на массив атрибутов заданных в метаданных для текущего модуля
BOOL CModuleManager::GetAttrVariable(CString csModuleName,CArray<CString,CString> &aVariable,CArray<CString,CString> &aTypeVariable)
{
	int nType;
	CString csName=GetObjectName(csModuleName,nType);
	if(pMetadata)
	if(!csName.IsEmpty())
	{
		if(nType==TYPE_METADATA_CONTEXT)
		{
			CArray<CString,CString> aNameList;
			CValue *pCurPos=&pMetadata->vMeta;

			CString csPath(csModuleName);
			int nIndex=csPath.ReverseFind('\\');
			CString Str(csPath.Left(nIndex));
			Str.TrimRight('\\');//нормализация строки
			Str+="\\";

			CString csName;
			int nIndex1=0;
			int nIndex2=Str.Find("\\",nIndex1);
			if(nIndex2<=0)
				return 0;//неправильный путь
			while(nIndex2>0)
			{
				if(pCurPos->GetType()!=TYPE_ARRAY)
					return 0;//нет атрибутов для заданного пути
				csName=Str.Mid(nIndex1,nIndex2-nIndex1);
				pCurPos=&pCurPos->GetAt(csName);

				nIndex1=nIndex2+1;
				nIndex2=Str.Find("\\",nIndex1);
			}
			//разбор массива 2С
			GetAttrVariableFromMetaArray((CValueArray *)pCurPos->pRef,aVariable,aTypeVariable);
			return 1;

		}
	}
	return 0;
}

//объявления атрибутов метаданных как внешних переменных модуля
void CModuleManager::SetAttrVariable(CCompileModule *pModule)
{
	CArray<CString,CString> aVariable;
	CArray<CString,CString> aType;
	if(pModule)
	if(GetAttrVariable(pModule->csModuleName,aVariable,aType))
	{
		for(int i=0;i<aVariable.GetSize();i++)
		{
			CString csName(aVariable[i]);
			csName.MakeUpper();
			if(!pModule->cContext.FindVariable(csName))
				pModule->AddVariable(csName,0);//если еще нет такой переменной (внешней), то создаем псевдо-внешнюю переменную (т.к. вначале д.б. именно внешние)
		}
	}
}
//преобразование иерархического массива метаданных в плоский список имен атрибутов
void CModuleManager::GetAttrVariableFromMetaArray(CValueArray *pArray,CArray<CString,CString> &aVariable,CArray<CString,CString> &aTypeVariable)
{
	CString csName;
	if(pArray)
	for(int i=0;i<pArray->GetSizeStrArray();i++)
	{
		csName=pArray->GetIdentifierByNumber(i);
		CValue Val=pArray->GetAt(csName);
		if(Val.GetSizeStrArray()>0)
		{
			if(Val.GetAt(String("#"))==1)
			{
				aVariable.Add(csName);
				aTypeVariable.Add(Val.GetAt("Тип"));
			}
			else
			if(Val.GetSizeStrArray()>0)
			{
				GetAttrVariableFromMetaArray((CValueArray *)Val.pRef,aVariable,aTypeVariable);
			}
		}
	}
}

