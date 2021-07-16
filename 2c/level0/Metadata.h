// Авторские права - VTOOLS.RU (info@vtools.ru)
// Metadata.h: interface for the CMetadata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATA_H__866C7CF9_A3C2_4D0B_A113_4FB02338C89E__INCLUDED_)
#define AFX_METADATA_H__866C7CF9_A3C2_4D0B_A113_4FB02338C89E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Compiler/Definition.h"
#include "MetaObject.h"
#include "ZipArchive\ZipArchive.h"	

enum
{
	OBJ_TEXT=0,//Текст
	OBJ_ATTR,//Атрибут
	OBJ_MODULE,//Модуль
	OBJ_FORM,//Форма
	OBJ_MAKET,//Макет
	OBJ_RUN,//Исполняемая функция
	OBJ_CLASS,
	OBJ_DOC,//Документацмя
};
class CObjectData
{
public:
	CObjectData()
	{
		bEdit=0;
		bGroup=0;
		nType=0;
		nChildImage=0;
		pRun=0;
		nLength=0;
		nPrec=0;
	};
	BOOL bEdit;//Признак разрешения редактирования (удаление и изменение имени)
	BOOL bGroup;//Признак что это группа
	int  nType;//тип элемента

	int  nChildImage;//картинка для подч. элементов группы
	CProcUnit	*pRun;//модуль, в котором вызывается исполняемая функция
	CString csFunction;//имя функции, если это исполняемая команда
	CValue vData;//дополнит. информация передаваемя в качестве параметра при вызове функции

	CString  sType;//строковое значение типа (если не задано, то произвольный тип)
	int		 nLength;
	int		 nPrec;


	//Методы:
	void SaveData(CValue &Val);
	void LoadData(CValue &Val);
};

enum
{
	CLOSE_FILE,
	OPEN_LOAD,
	OPEN_SAVE,

};//nZipState


class ObjectDescription
{
public:
	ObjectDescription()
	{
		bReadOnly=0;
	}
	CArray <CString,CString&> aModule;
	CArray <CString,CString&> aForms;
	CArray <CString,CString&> aMakets;
	CArray <CString,CString&> aConst;

	CArray <CString,CString&> aAttrList;


	CMapStringToPtr ListModuleAdd;
	CMapStringToPtr ListFormAdd;
	CMapStringToPtr ListMaketAdd;
	CMapStringToPtr ListConstAdd;

	BOOL bReadOnly;


	BOOL Add(CArray <CString,CString&> &aListName,CMapStringToPtr &aListFind, CString csName);
};


class CMetadata  
{
public:
	CMetadata();
	virtual ~CMetadata();


	//АТРИБУТЫ:
	CMapStringToPtr ListObjects;//список открытых объектов конфигурации
	//CMap <int,int,CString,CString&> ListObjectID;//список соответствий идентификаторов и имен объектов
	CArray <CString,CString&> ListModuleName;//список имен общих модулей
	CArray <CString,CString&> ListFormName;//список имен общих форм
	CArray <CString,CString&> ListMaketName;//список имен общих макетов
	CArray <CString,CString&> ListObjectName;//список имен объектов
	CMapStringToPtr OList;
	//CValue ObjectsArray;//многомерный массив - список объектов с данными
	CMapStringToPtr ListModuleAdd;
	CMapStringToPtr ListFormAdd;
	CMapStringToPtr ListMaketAdd;
	CMapStringToPtr ListObjectAdd;
//	CArray <CString,CString&> ListMetadataName;//список метаданных

	CValue vMeta;

private:
	CZipArchive m_zip;
	CString m_csConfigPath;
	int nZipState;


	//поддержка библиотек объектов
	BOOL bIsLibrary;
	CArray <CMetadata *,CMetadata *> aLibList;

public:
	//МЕТОДЫ:
	void AllFind(CString csFind,int nCase,int nWorld);

	void AddToMeta(CString Str);
	void RenameMetaObject(CMetaObject *pObj,CString csNewName);
	void RemoveMetaObject(CMetaObject *pObj);
	//получение метаобъекта (если такой метаобъект еще не загружен из файла, то он загружается)
	CMetaObject *GetMetaObject(CString csFileName,CString csObjName,int bReadOnly=0);
	CMetaObject *GetMetaObject2(CString csFileName,CString csObjName,int bReadOnly);

	CString ReadFileToString(CString csKey,CString &csAlias,CString &csComment);
	int WriteFileFromString(CString csModule,CString csKey,CString csAlias,CString csComment);
	int WriteFileFromStream(void *pBuffer,int nSize,CString csFile,CString csKey,CString csAlias,CString csComment);
	int DeleteData(CString csPath);
	
	void Clear();
	BOOL Load(CString csFileName);//Заполнение списка метаданных
	BOOL Save();//сохранение в файл открытых на редактирование объектов
	BOOL OpenZip(int nCurState);

	//поддержка библиотек объектов
	void AddLibrary(CString csFileName);
	void LoadSystemLibrary();

private:

};

#endif // !defined(AFX_METADATA_H__866C7CF9_A3C2_4D0B_A113_4FB02338C89E__INCLUDED_)
