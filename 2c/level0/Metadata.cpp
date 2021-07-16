//Интегрированная платформа автоматизации предприятия
//Авторские права - VTOOLS.RU (info@vtools.ru)

// Metadata.cpp: implementation of the CMetadata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "Metadata.h"
#include "MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#include "FormObjectDoc.h"


/*
class CListItem
{
public:


};
*/

extern CMapStringToPtr	aHashKeywordList;//список ключевых слов для подсветки
extern CArray <CString,CString> aGlVariables;


CString GetGetLastStringError();


int CompareList( const void *arg1, const void *arg2 )//функция сравнения элементов списка
{
 	CString Str1=(*(CString*)arg1);
	CString Str2=(*(CString*)arg2);
	return _stricmp(Str1,Str2);
}
void SortList(CArray <CString,CString&>  &List)
{
	if(List.GetSize())
		qsort(&List[0], List.GetSize(), sizeof(List[0]), CompareList);
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMetadata::CMetadata()
{
	nZipState=CLOSE_FILE;
	bIsLibrary=0;
}

CMetadata::~CMetadata()
{
	Clear();
	OpenZip(CLOSE_FILE);
}

void CMetadata::Clear()
{
	ListModuleName.RemoveAll();
	ListFormName.RemoveAll();
	ListMaketName.RemoveAll();
	ListObjectName.RemoveAll();

	ListModuleAdd.RemoveAll();
	ListFormAdd.RemoveAll();
	ListMaketAdd.RemoveAll();
	ListObjectAdd.RemoveAll();

	ListModuleAdd[INITMODULENAME]=(void*)1;
	ListModuleAdd[mUpper(INITMODULENAME)]=(void*)1;



//	ListMetadataName.RemoveAll();

	CMetaObject *p;
	POSITION pos;
	CString csKey;
	if(!bIsLibrary)
	for( pos = ListObjects.GetStartPosition(); pos != NULL; )
	{
		ListObjects.GetNextAssoc( pos, csKey, (void*&)p );
		if(p)
			delete p;
	}
	ListObjects.RemoveAll();

	ObjectDescription *p2;
	if(!bIsLibrary)
	for( pos = OList.GetStartPosition(); pos != NULL; )
	{
		OList.GetNextAssoc( pos, csKey, (void*&)p2 );
		if(p2)
			delete p2;
	}
	OList.RemoveAll();
	

	//ListDelObjects.RemoveAll();

	vMeta.Reset();


	for(int i=0;i<aLibList.GetSize();i++)
		delete aLibList[i];

	aLibList.RemoveAll();

}

void CMetadata::AddToMeta(CString Str)
{
	CValue *pCurPos=&vMeta;

	int nPoint=Str.ReverseFind('.');
	CString csType;
	if(nPoint>0)
	{
		csType=mUpper(Str.Mid(nPoint));
		Str=Str.Left(nPoint);
	}

	CString csName;
	int nIndex1=0;
	int nIndex2=Str.Find("\\",nIndex1);
	while(nIndex2>0)
	{
		csName=Str.Mid(nIndex1,nIndex2-nIndex1);

		if(pCurPos->GetType()!=TYPE_ARRAY)
			pCurPos->CreateObject("Структура");
		pCurPos=&pCurPos->GetAt(csName);
		

		nIndex1=nIndex2+1;
		nIndex2=Str.Find("\\",nIndex1);
	}
	csName=Str.Mid(nIndex1);
	if(pCurPos->GetType()!=TYPE_ARRAY)
		pCurPos->CreateObject("Структура");

	if(csType!=".ATR")//&&csType!=".INF") 
	{
		if(pCurPos->GetAt(csName).GetType()!=TYPE_ARRAY)
			pCurPos->SetAt(csName,CValue());
	}
	else
	if(csType==".ATR") 
	{
		CMetaObject *pObj=GetMetaObject(Str,csName,1);
		if(pObj)
		{
			CValue Val;
			Val.LoadFromString(pObj->csAttr);
			Val.SetAt(String("#"),1);//признак атрибута
			pCurPos->SetAt(csName,Val);

			Val.SetAt("Идентификатор",String(csName));
			Val.SetAt("Синоним",String(pObj->csAlias));
			Val.SetAt("Комментарий",String(pObj->csComment));
		}
	}
}

void CMetadata::RemoveMetaObject(CMetaObject *pObj)
{
	pObj->IsDeleted=1;
}
/*
void CMetadata::RenameMetaObject(CMetaObject *pObj,CString csNewName)
{
	pObj->IsDeleted=1;
}
*/


//поиск метаобъекта во всех библиотеках
CMetaObject *CMetadata::GetMetaObject(CString csFileName,CString csObjName,int bReadOnly) 
{
	csFileName.Replace("/","\\");
	CMetaObject *pObject=GetMetaObject2(csFileName,csObjName,1);
	if(pObject && bReadOnly==0)
	{
		pObject->bCanSave=1;
	}

	if(!bIsLibrary)
	if(!pObject)
	{
		for(int i=aLibList.GetSize()-1;i>=0;i--)
		{
			pObject=aLibList[i]->GetMetaObject2(csFileName,csObjName,1);
			if(pObject)
				break;
		}
	}

	if(!pObject && bReadOnly==0)
	{
		pObject=GetMetaObject2(csFileName,csObjName,0);
	}

	return pObject;//GetMetaObject2(csFileName,csObjName,bReadOnly);
}
//получение метаобъекта (если такой метаобъект еще не загружен из файла, то он загружается)
CMetaObject *CMetadata::GetMetaObject2(CString csFileName,CString csObjName,int bReadOnly) 
{
	if(!OpenZip(OPEN_LOAD))
		return 0;


	if(afxAppRunMode==ENTERPRISE_MODE || bIsLibrary)
		bReadOnly=1;

	CMetaObject *pObject=(CMetaObject *)ListObjects[mUpper(csFileName)];
	if(pObject)
	{ 
		if(!csObjName.IsEmpty()&&pObject->csOldName!=pObject->csName)
		{
			//было изменено имя - переписываем указатель
			ListObjects[mUpper(pObject->csName)]=pObject;
			pObject=0;
		}
		else
		{
			if(pObject->IsDeleted)
			{//объект был удален - значит больше его не грузим
				pObject=new CMetaObject();
				ListObjects[mUpper(csFileName)]=pObject;
			}
			if(!bReadOnly)//если объекта берется для изменения, то разрешаем его последующую запись
				pObject->bCanSave=1;
			return pObject;
		}
	}

	if(bReadOnly)//проверяем физическое наличие в файле метаданных
	{
		int n1=m_zip.FindFile(csFileName);
		if(n1<0)
			n1=m_zip.FindFile(csFileName+".inf");
		if(n1<0)
			n1=m_zip.FindFile(csFileName+".2c");
		if(n1<0)
		{
			return 0;
		}
	}

	pObject=new CMetaObject();
	if(bReadOnly)//объект еще не загружался + режим взятия только для чтения - помечаем его как незаписываемый (для уменьшения трафика)
		pObject->bCanSave=0;
	ListObjects[mUpper(csFileName)]=pObject;
	pObject->Load(m_zip,csFileName,csObjName);

//	OpenZip(CLOSE_FILE); //временное закрытие

	return pObject;
}

//Заполнение списка метаданных
BOOL CMetadata::Load(CString csFileName)
{
	m_csConfigPath=csFileName;

	Clear();

	if(!bIsLibrary)
		LoadSystemLibrary();

	if(!OpenZip(OPEN_LOAD))
		return 0;


	int nEntries=m_zip.GetNoEntries();
	for (WORD i = 0; i < nEntries; i++)
	{
		CZipFileHeader fhInfo;
		m_zip.GetFileInfo(fhInfo, i);
		CString Str=fhInfo.GetFileName()+"\\";

		if(!m_zip.IsFileDirectory(i))
			AddToMeta(fhInfo.GetFileName());

		int nPoint=Str.ReverseFind('.');
		CString csExt;
		if(nPoint>0)
		{
			csExt=mUpper(Str.Mid(nPoint));
			csExt.TrimRight('\\');
		}
		if(csExt==".INF")
			Str=Str.Left(nPoint)+"\\";

		int nIndex1=Str.Find("\\");
		if(nIndex1>0)
		{
			int nIndex2=Str.Find("\\",nIndex1+1);
			if(nIndex2>0)
			{
				CString Str2=Str.Left(nIndex2);
				int n=Str2.ReverseFind('\\');
				CString csName=Str2.Mid(n+1);
				CString csType=mUpper(Str2.Left(n));
				if(csType==mUpper(OBJECTSNAME))
				{
					csName.TrimRight();
					if(csName.IsEmpty())
						continue;

					if(!ListObjectAdd[mUpper(csName)])
					{
						ListObjectAdd[mUpper(csName)]=(void*)1;
						ListObjectName.Add(csName);
					}
					//Объекты\Документ\Модули\Модуль среды исполнения.2c
					int nIndex3=Str.Find("\\",nIndex2+1);
					int nIndex4=Str.ReverseFind('.');
					int nPoint=nIndex4;
					if(nIndex4<=0)
						nIndex4=Str.Find("\\",nIndex3+1);
					if(nIndex3>0&&nIndex4>0)
					{
						CString csGroup=mUpper(Str.Mid(nIndex2+1,nIndex3-nIndex2-1));
						CString csModule=Str.Mid(nIndex3+1,nIndex4-nIndex3-1);

						csModule.TrimRight();
						if(csModule.IsEmpty())
							continue;

						ObjectDescription *pObject;
						pObject=(ObjectDescription *)OList[mUpper(csName)];
						if(!pObject)
							pObject=new ObjectDescription();

						pObject->bReadOnly=bIsLibrary;

						OList[mUpper(csName)]=pObject;
						if(csGroup==mUpper(OBJFORM))
						{
							if(csExt==".FRM")
								pObject->Add(pObject->aForms,pObject->ListFormAdd,csModule);
								//pObject->aForms.Add(csModule);
						}
						else
						if(csGroup==mUpper(OBJMODULE))
						{
							//if(csExt==".2C"&&mUpper(csModule)!=mUpper(CONFIGMODULENAME)&&mUpper(csModule)!=mUpper(ENTERPRISEMODULENAME))
							if(csExt!=".INF")
							if(mUpper(csModule)!=mUpper(CONFIGMODULENAME)&&mUpper(csModule)!=mUpper(ENTERPRISEMODULENAME))
								pObject->Add(pObject->aModule,pObject->ListModuleAdd,csModule);
								//pObject->aModule.Add(csModule);
						}
						else
						if(csGroup==mUpper(OBJMAKET))
						{
							pObject->Add(pObject->aMakets,pObject->ListMaketAdd,csModule);
							//pObject->aMakets.Add(csModule);
						}
						else
						if(csGroup==mUpper(OBJCONST))
						{
							if(pObject->Add(pObject->aConst,pObject->ListConstAdd,csModule))
							if(!aHashKeywordList[mUpper(csModule)])
							{
								aGlVariables.Add(csModule);
								aHashKeywordList[mUpper(csModule)]=(void*)1;
							}
/*
							if(!mUpper(csModule).IsEmpty())
							if(!aHashKeywordList[mUpper(csModule)])
							{
								aGlVariables.Add(csModule);
								aHashKeywordList[mUpper(csModule)]=(void*)1;
							}
							pObject->aConst.Add(csModule);*/
						}
					}
				}
/*				else
				if(csType==mUpper(METADATANAME))
				{
					if(csExt==".ATR")
					{
						ObjectDescription *pObject;
						pObject=(ObjectDescription *)OList[mUpper(csName)];
						if(!pObject)
							pObject=new ObjectDescription();
						OList[mUpper(csName)]=pObject;
						pObject->aAttrList.Add(csModule);
					}
				}
*/				else
				if(csType==mUpper(MODULESNAME))
				{
					CString csFileType=mUpper(Str2.Right(3));
					if(csFileType!=".INF")
					{
						if(csFileType==".2C")
							csName=csName.Left(csName.GetLength()-3);
						csName.TrimRight();
						if(csName.IsEmpty())
							continue;
						if(!ListModuleAdd[mUpper(csName)])
						{
							ListModuleAdd[mUpper(csName)]=(void*)1;
							ListModuleName.Add(csName);
						}
					}
				}
				else
				if(csType==mUpper(FORMSNAME))
				{
					CString csFileType=mUpper(Str2.Right(4));
					if(csFileType==".FRM")
					{
						csName=csName.Left(csName.GetLength()-4);
						csName.TrimRight();
						if(csName.IsEmpty())
							continue;
						if(!ListFormAdd[mUpper(csName)])
						{
							ListFormAdd[mUpper(csName)]=(void*)1;
							ListFormName.Add(csName);
						}
					}
				}
				else
				if(csType==mUpper(MAKETSNAME))
				{
					csName.TrimRight();
					if(csName.IsEmpty())
						continue;
					if(!ListMaketAdd[mUpper(csName)])
					{
						ListMaketAdd[mUpper(csName)]=(void*)1;
						ListMaketName.Add(csName);
					}
				}
			}
		}
	}

	SortList(ListFormName);
	SortList(ListMaketName);
	SortList(ListModuleName);
	SortList(ListObjectName);
//	SortList(ListMetadataName);

	OpenZip(CLOSE_FILE); //временное закрытие
	return TRUE;
}

BOOL CMetadata::Save()
{
	if(!OpenZip(OPEN_SAVE))
		return 0;

	CMetaObject *p;
	POSITION pos;
	CString csKey;


	//сначала обрабатываем удаляемые элементы
	for( pos = ListObjects.GetStartPosition(); pos != NULL; )
	{
		ListObjects.GetNextAssoc( pos, csKey, (void*&)p );
		if(p)
			if(p->IsDeleted)
				p->Save
				(m_zip,"");
	}

	for( pos = ListObjects.GetStartPosition(); pos != NULL; )
	{
		ListObjects.GetNextAssoc( pos, csKey, (void*&)p );
		if(p)
		if(!p->IsDeleted)
		if(p->pTree)
		if(p->nTempID)
			p->Save(m_zip,p->pTree->GetPath(p->nTempID));
	}
	OpenZip(CLOSE_FILE); //временное закрытие
	return TRUE;
}


BOOL CMetadata::OpenZip(int nSetState)
{
	if(nZipState!=nSetState)
	{
		if(nZipState!=CLOSE_FILE)
		{
			m_zip.Close();
		}

		//открываем
		if(nSetState!=CLOSE_FILE)
		{
			//CString m_csConfigPath=csIBDir+"\\"+METADATA_FILENAME;

			if(m_csConfigPath.IsEmpty())
			{
				//AfxMessageBox("Не задан файл метаданных!");
				Message("Не задан файл метаданных!");
				return 0;
			}

			if(!FileExist(m_csConfigPath))
			{
				m_zip.Open(m_csConfigPath, CZipArchive::create,0);//Новый архив
				m_zip.Close();	
			}
Start:
			try
			{
				if(nSetState==OPEN_LOAD)
					m_zip.Open(m_csConfigPath, CZipArchive::openReadOnly);
				else//SAVE
					m_zip.Open(m_csConfigPath, CZipArchive::open);
			}
			catch(...)
			{
				HWND hWnd=0;
				if(AfxGetMainWnd())
					hWnd=AfxGetMainWnd()->m_hWnd;
				int nRet=MessageBox(hWnd,CString("Ошибка открытия файла ")+m_csConfigPath+"\nПовторить?","Внимание",MB_OKCANCEL|MB_ICONQUESTION);
				if(nRet!=1)
					return FALSE;
				goto Start;
			}

			m_zip.EnableFindFast(1);

		}
		nZipState=nSetState;
	}
	return TRUE;
}

CString CMetadata::ReadFileToString(CString csKey,CString &csAlias,CString &csComment)
{
	if(!OpenZip(OPEN_LOAD))
		return "";
	return ::ReadFileToString(m_zip,csKey,csAlias,csComment);
}
int CMetadata::WriteFileFromString(CString csModule,CString csKey,CString csAlias,CString csComment)
{
	if(!OpenZip(OPEN_SAVE))
		return 0;
	return ::WriteFileFromString(csModule,m_zip,csKey,csAlias,csComment,1);
}
int CMetadata::WriteFileFromStream(void *pBuffer,int nSize,CString csFile,CString csKey,CString csAlias,CString csComment)
{
	if(!OpenZip(OPEN_SAVE))
		return 0;
	return ::WriteFileFromStream(pBuffer,nSize,csFile,m_zip,csKey,csAlias,csComment,1);
}

int CMetadata::DeleteData(CString csPath)
{
	if(!OpenZip(OPEN_SAVE))
		return 0;

	if(m_zip.FindFile(csPath))
		return ::DeleteFile(m_zip,csPath);
	else
		return ::DeleteFolder(m_zip,csPath);
}

void CObjectData::SaveData(CValue &Val)
{
	Val.CreateObject("Структура");
	Val.SetAt("Тип",String(sType));
	if(nLength)
		Val.SetAt("Длина",nLength);
	if(nPrec)
		Val.SetAt("Точность",nPrec);

	if(nChildImage)
		Val.SetAt("ChildImage",nChildImage);
	if(bEdit)
		Val.SetAt("Edit",1);
	if(bGroup)
		Val.SetAt("Group",1);
	if(nType)
		Val.SetAt("Type",nType);
}
void CObjectData::LoadData(CValue &Val)
{
	if(Val.GetSizeStrArray())
	{
		sType=Val.GetAt("Тип").GetString();
		nLength=Val.GetAt("Длина");
		nPrec=Val.GetAt("Точность");
/*
		nChildImage=Val.GetAt("ChildImage");
		bEdit=Val.GetAt("Edit");
		bGroup=Val.GetAt("Group");
		nType=Val.GetAt("Type");
*/
	}
}

#define EscapePress()	GetAsyncKeyState(VK_ESCAPE)==-32768

void CMetadata::AllFind(CString csFind,int nCase,int nWorld) 
{
	if(csFind.IsEmpty())
		return;
	csFind.TrimRight();
	csFind.TrimLeft();
	if(!nCase)
		csFind.MakeUpper();

	int nCount=0;
	if(OpenZip(OPEN_LOAD))
	{
		int nEntries=m_zip.GetNoEntries();
		CZipFileHeader fhInfo;
		for (WORD i = 0; i < nEntries; i++)
		{
			if(EscapePress())
			{
				Message("Поиск прерван пользователем");
				return;
			}
			if(!m_zip.IsFileDirectory(i))
			{
				m_zip.GetFileInfo(fhInfo, i);
				CString csFileName=fhInfo.GetFileName();

				CMetaObject *pObj=GetMetaObject(csFileName,"",1);
				if(pObj)
				{
					CString csFile=pObj->csFile;
					if(!nCase)
						csFile.MakeUpper();

					int nPos=csFile.Find(csFind); 
					if(nPos>=0)
					{
						CString csText=FindErrorCodeLine(pObj->csFile,nPos,3);


						CString StrMessage;
						StrMessage.Format("%s %s",csFileName,csText);
						Message(StrMessage);
						nCount++;
						nPos=csFile.Find(csFind,nPos+1);
					}
				}
			}
		}
	}
	CString Str;
	Str.Format("Всего найдено %d",nCount);
	Message(Str);
}



/////////////////////////////////////////////////////////////////////////////
void AddUniqueList(CMapStringToPtr &HashList, CArray <CString,CString&>  &ListDest, CArray <CString,CString&>  &ListSource)
{
	for(int i=0;i<ListSource.GetSize();i++)
	{
		CString csName=ListSource[i];
		if(!HashList[mUpper(csName)])
		{
			HashList[mUpper(csName)]=(void*)1;
			ListDest.Add(csName);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//загрузка системной библиотеки объектов
void CMetadata::LoadSystemLibrary()
{
	CString StartPath=csCurrentExeDir;
	StartPath.TrimRight('\\');
	StartPath+="\\SYSTEM\\";
	
	
	SetCurrentDirectory(csIBDir);

	WIN32_FIND_DATA fdata;
	HANDLE hfind=FindFirstFile(StartPath+"*.md2",&fdata);
	if(NULL==hfind || INVALID_HANDLE_VALUE==hfind)
		return;

	CArray <CString,CString&> aLibName;
	do
	{
		CString csName=StartPath + fdata.cFileName;
		if(mUpper(csName)!=mUpper(m_csConfigPath))
			aLibName.Add(csName);
	}
	while(FindNextFile(hfind,&fdata));
	FindClose(hfind);


//	m_zip.Open(m_csConfigPath, CZipArchive::create,0);//Новый архив
//	m_zip.Close();	

	//сортировка
	SortList(aLibName);
	for(int i=0;i<aLibName.GetSize();i++)
	{
		CString csName=mUpper(aLibName[i]);
		if(csName.Right(4)==".MD2")
		//AfxMessageBox(aLibName[i]);
			AddLibrary(aLibName[i]);
	}

}

void CMetadata::AddLibrary(CString csFileName)
{
	CMetadata *pLib=new CMetadata();
	pLib->bIsLibrary=1;

	pLib->vMeta=vMeta;
	if(pLib->Load(csFileName))
	{
		vMeta=pLib->vMeta;
		AddUniqueList(ListModuleAdd, ListModuleName,pLib->ListModuleName);
		AddUniqueList(ListFormAdd, ListFormName,pLib->ListFormName);
		AddUniqueList(ListMaketAdd, ListMaketName,pLib->ListMaketName);
		AddUniqueList(ListObjectAdd, ListObjectName,pLib->ListObjectName);


		//описание объектов
		POSITION pos;
		CString csKey;
		ObjectDescription *p2;
		for( pos = pLib->OList.GetStartPosition(); pos != NULL; )
		{
			pLib->OList.GetNextAssoc( pos, csKey, (void*&)p2 );
			if(p2)
			{
				OList[mUpper(csKey)]=p2;
			}
		}

		aLibList.Add(pLib);
	}
	else
	{
		delete pLib;
	}
}


BOOL ObjectDescription::Add(CArray <CString,CString&> &aListName,CMapStringToPtr &aListFind, CString csName0)
{
	CString csNAME=mUpper(csName0);
	if(!csNAME.IsEmpty())
	if(!aListFind[csNAME])
	{
		aListName.Add(csName0);
		aListFind[csNAME]=(void*)1;
		return TRUE;
	}
	return FALSE;
}
