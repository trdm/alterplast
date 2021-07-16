// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueMetadata.cpp: implementation of the CValueMetadata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueMetaFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "../ModuleManager.h"
CString GetGetLastStringError();


CMethods CValueMetadata::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueMetadata,CValue);
CValueMetadata::CValueMetadata()
{
	nType=100;
	bWasOpenZip=0;
}

CValueMetadata::~CValueMetadata()
{
	if(bWasOpenZip)
		Close();
	else
	if(AfxGetModuleManager())
		AfxGetModuleManager()->pMetadata->OpenZip(CLOSE_FILE); 

}

//******************************
//Работа как агрегатного объекта
//******************************
enum
{
	enOpen=0,
	enSaveData,
	enSaveData3,
	enLoadData,
	enLoadData3,
	enSaveData2,
	enLoadData2,
	enSaveText,
	enLoadText,
	enDeleteData,
	enClose,
	enGetCountEntries,
	enGetName,
};
void CValueMetadata::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"Open","Открыть","Открыть(ИмяФайла,ФлагТолькоЧтение) - открытие внешнего файла метаданных"},
		{"SaveData","ЗаписатьДанные","ЗаписатьДанные(Путь,Объект)"},
		{"SaveDataFromFile","ЗагрузитьДанные","ЗагрузитьДанные(Путь,ИмяФайла,Синоним,Комментарий)"},
		{"LoadData","ПрочитатьДанные","ПрочитатьДанные(Путь,Объект)"},
		{"LoadDataToFile","ВыгрузитьДанные","ВыгрузитьДанные(Путь,ИмяФайла,Синоним,Комментарий)"},
		{"SaveObject","ЗаписатьОбъект","ЗаписатьОбъект(Путь,Объект)"},
		{"LoadObject","ПрочитатьОбъект","ПрочитатьОбъект(Путь,Объект)"},
		{"SaveText","ЗаписатьТекст","ЗаписатьТекст(Путь,Текст)"},
		{"LoadText","ПрочитатьТекст","ПрочитатьТекст(Путь,Текст)"},
		{"DeleteData","УдалитьДанные","УдалитьДанные(Путь)"},
		{"Close","Закрыть","Закрыть() - закрытие внешнего файла метаданных"},
		{"GetCountEntries","КоличествоЭлементов","КоличествоЭлементов() - количество элементов во внешнем файле метаданных"},
		{"GetName","ПолучитьИмя","ПолучитьИмя(Номер) - получитья имя элемента"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

CString GetFullMetaPath(CString csPathObject,CString Str)
{
	Str.Replace("/","\\");

	if(mUpper(Str.Left(3))=="MD:")
	{
		if(Str.Mid(3,1)=="\\")
			return Str.Mid(4);
		else
			return Str.Mid(3);
	}
	else
	{
		return METADATANAME+"\\"+csPathObject+Str;
	}
}

CValue CValueMetadata::Method(int iName,CValue **p)
{
	CMetadata *pMeta=0;
	if(bWasOpenZip==0 && iName!=enOpen && iName!=enClose && AfxGetModuleManager()==NULL)
	{
		Error("Конфигурация не инициализирована");
	}
	if(AfxGetModuleManager())
		pMeta=AfxGetModuleManager()->pMetadata; 


	CValue Ret;
	switch(iName)
	{
		case enOpen:
			{ 
				if(bWasOpenZip)
					Close();

				CString csFileName=p[0]->GetString();
				BOOL bReadOnly=p[1]->GetNumber();

				if(!bReadOnly)
				if(!FileExist(csFileName))
				{
					try 
					{
						m_zip.Open(csFileName, CZipArchive::create,0);//Новый архив
						m_zip.Close();	
					}
					catch(...)
					{
						Error("Ошибка при попытке создания нового файла: %s",csFileName);
						return 0;
					}

				}

				if(m_zip.FileExists(csFileName))
				{
					try 
					{
						if(bReadOnly)
							m_zip.Open(csFileName, CZipArchive::openReadOnly,0);
						else
							m_zip.Open(csFileName, CZipArchive::open,0);
					}
					catch(...)
					{
						Error("Ошибка открытия файла: %s",csFileName);
						return 0;
					}
				}
				else
					Error("Не найден файл: %s",csFileName);

				m_zip.EnableFindFast(1);
				bWasOpenZip=1;
				return 1;
			}

		case enSaveData3:
			{   
				CString csPath=GetFullMetaPath(csPathObject,p[0]->GetString());
				CString csFile=p[1]->GetString();
				int nSize=0;
				char *buf=LoadFromFileBin(csFile,nSize);
				if(buf)
				{
					if(bWasOpenZip)
					{
						return ::WriteFileFromStream(buf,nSize,csFile,m_zip,p[0]->GetString(),p[2]->GetString(),p[3]->GetString(),1);
					}
					else
					{
						return pMeta->WriteFileFromStream(buf,nSize,csFile,csPath,p[2]->GetString(),p[3]->GetString());
					}
				}
			}
		case enSaveData:
		case enSaveData2:
		case enSaveText:
			{
				CString csData;
				CString csPath=GetFullMetaPath(csPathObject,p[0]->GetString());
				if(enSaveText==iName)
				{
					csData=p[1]->GetString();
				}
				else
				{
					p[1]->SaveToString(csData);
				}
				if(bWasOpenZip)
				{
					return ::WriteFileFromString(csData,m_zip,p[0]->GetString(),p[2]->GetString(),p[3]->GetString(),1);
				}
				else
				{
					return pMeta->WriteFileFromString(csData,csPath,p[2]->GetString(),p[3]->GetString());
				}
			}
		case enLoadData3:
			{
				CString csAlias,csComment;
				CString csPath=GetFullMetaPath(csPathObject,p[0]->GetString());
				CString csFile=p[1]->GetString();
				
				if(bWasOpenZip)
				{
					if(!csPath.IsEmpty())
					{
						int nRes=m_zip.FindFile(csPath);
						if(nRes>=0)
						{
							bool bRes=m_zip.OpenFile(nRes);
							CZipFileHeader fhInfo;
							m_zip.GetFileInfo(fhInfo, nRes);
							CString csDescription=fhInfo.GetComment();
							int nIndex=csDescription.Find("\n");
							csAlias=csDescription.Left(nIndex);
							csComment=csDescription.Mid(nIndex+1);
							int nSize=fhInfo.m_uUncomprSize;
							char* pBuffer=new char [nSize+1];
							pBuffer[nSize]=0;
							DWORD nRez=m_zip.ReadFile(pBuffer,nSize);
							if(FILE *sFile=fopen(csFile,"w+b"))
							{
						
								fwrite(pBuffer,sizeof(char),nSize, sFile);
								fclose(sFile);
							}
							delete []pBuffer;
							m_zip.CloseFile();
						}
					}
				}
				Ret=1;
				p[2]->SetString(csAlias);
				p[3]->SetString(csComment);

				break;
			}
		case enLoadData:
		case enLoadData2:
		case enLoadText:
			{
				CString csAlias,csComment;
				CString csPath=GetFullMetaPath(csPathObject,p[0]->GetString());
				CString csData;
				if(bWasOpenZip)
				{
					csData=::ReadFileToString(m_zip,p[0]->GetString(),csAlias,csComment);
				}
				else
				{
					csData=pMeta->ReadFileToString(csPath,csAlias,csComment);
				}
				if(enLoadText==iName)
				{
					Ret=String(csData);
				}
				else
				{
					Ret.LoadFromString(csData);
				}
				*p[1]=Ret;
				p[2]->SetString(csAlias);
				p[3]->SetString(csComment);

				break;
			}
		case enDeleteData:
			{
				CString csPath=METADATANAME+"\\"+csPathObject+p[0]->GetString();
				if(bWasOpenZip)
				{
					if(m_zip.FindFile(p[0]->GetString()))
						return ::DeleteFile(m_zip,p[0]->GetString());
					else
						return ::DeleteFolder(m_zip,p[0]->GetString());
				}
				else
				{
					return pMeta->DeleteData(csPath);
				}
			}
		case enClose:
			{
				Close();
				break;
			}
		case enGetCountEntries:
			{
				if(!bWasOpenZip)
					Error("Внешний файл метаданных не открыт");

				return m_zip.GetNoEntries();
			}
		case enGetName:
			{
				if(!bWasOpenZip)
					Error("Внешний файл метаданных не открыт");

				int n=p[0]->GetNumber();
				if(n<1 || n>m_zip.GetNoEntries())
					Error("Значение индекса выходит за границу допустимых значений");

				CZipFileHeader fhInfo;
				m_zip.GetFileInfo(fhInfo, n-1);
				return String(fhInfo.GetFileName());
			}

	}
	return Ret;
}

void CValueMetadata::Close()
{
	if(bWasOpenZip)
	{
		m_zip.Close();
		bWasOpenZip=0;
	}
}