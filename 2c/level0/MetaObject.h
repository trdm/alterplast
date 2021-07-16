// Авторские права - VTOOLS.RU (info@vtools.ru)
// MetaObject.h: interface for the CMetaObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METAOBJECT_H__11E8EE01_C3E8_4519_9C86_934111848FCB__INCLUDED_)
#define AFX_METAOBJECT_H__11E8EE01_C3E8_4519_9C86_934111848FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipArchive\ZipArchive.h"	


int DeleteFile(CZipArchive &m_zip,CString csPath);
int DeleteFolder(CZipArchive &m_zip,CString csPath);
void OpenNewFile(CZipArchive &m_zip,CString csPath,CString csComment,CString csFile="");
void WriteInt(CZipArchive &m_zip,int nValue);
void WriteString(CZipArchive &m_zip,CString csStr);
int ReadInt(CZipArchive &m_zip);
CString ReadString(CZipArchive &m_zip);
CString ReadFileToString(CZipArchive &m_zip,CString csKey,CString &csAlias,CString &csComment);
int WriteFileFromString(CString csModule,CZipArchive &m_zip,CString csKey,CString csAlias,CString csComment,BOOL bMastSave=0);
int WriteFileFromStream(void *pBuffer,int nSize,CString csFile,CZipArchive &m_zip,CString csKey,CString csAlias,CString csComment,BOOL bMastSave=0);

#define MODULENAME	CString("module.2c")
#define CONFIGNAME	CString("config.2c")
//#define INFONAME	CString("info")
#define DESCRIPNAME	CString("description.txt")
#define FORMNAME	CString("form data.frm")




class CMetaObject  
{
public:
	CMetaObject();
	virtual ~CMetaObject();

	//Атрибуты:
	int nID;				//внутренний код (номер) объекта
	CString csName;			//имя объекта (идентификатор)
	CString csAlias;		//синоним объекта
	CString csComment;		//комментарий объекта
	bool	bEnable;		//использование объекта в конфигурации
	CString csIdDB;		    //идентификатор БД
	//int		nImage;

//	class CObjectData *pData;		//дополнительные данные по поведению объекта в конфигураторе
//	BOOL	bAutoDeleteData;//дополнительные данные
	CValue vExtVal;


	CString csInfo;			//описание объекта
	CString csFile;			//содержимое файла данных
	CString csForm;			//содержимое формы данных
	CString csAttr;			//описание атрибутов

	CString csParam1;

	BOOL IsDeleted;			//признак удаления
	CString csPrevFileName;	//путь в контейнере при загрузке (для очистки при перезаписи)

	HTREEITEM nTempID;		//связь с визуальными объектами конфигуратора
	class CMetadataTree *pTree;

	int bCanSave;
	int	nTypeObject;
	CString csOldName;		//имя для хранения в спсике

	//Методы:
	BOOL Save(CZipArchive &m_zip,CString csFileName);
	BOOL Load(CZipArchive &m_zip,CString csFileName,CString csObjName);

	void SaveToInfo();
	void LoadFromInfo();

//	CObjectData cData;
	//атрибуты в режиме ObjectInfo
//	CString csParent;	//имя родителя
//	CString csName;		//имя объекта (идентификатор)
//	int nArhivType;		//режим хранения: ObjectInfo,ObjectFile,FormFiles
//	int		nParent;	//родитель
	//атрибуты в режиме ObjectFile и FormFiles
	//CString csTypeFile;	//тип файла
//	BOOL nInSave;

	//внутренние
//	int nStatus;		//статус редактирования (STATUS_EDIT,STATUS_REMOVE)

//	CString csPrevName;	//старое имя объекта - также используется для определения текущего пути объекта
//	CString csObjectPath;//путь в контейнере при загрузке (для очистки при перезаписи)



//public:
/*
	void InitPath(CString csKey);
	BOOL SaveInfo(CZipArchive &m_zip);
	BOOL GetInfo(CZipArchive &m_zip,CString csKey);
	BOOL GetFile(CZipArchive &m_zip,CString csKey);//csKey - полный путь к файлу
	BOOL SaveFile(CZipArchive &m_zip);//csFileName - новое имя файла

	BOOL GetForm(CZipArchive &m_zip,CString csKey);//csKey - путь к файлу
	BOOL SaveForm(CZipArchive &m_zip);
*/
};

#endif // !defined(AFX_METAOBJECT_H__11E8EE01_C3E8_4519_9C86_934111848FCB__INCLUDED_)
