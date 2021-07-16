// ��������� ����� - VTOOLS.RU (info@vtools.ru)
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
	OBJ_TEXT=0,//�����
	OBJ_ATTR,//�������
	OBJ_MODULE,//������
	OBJ_FORM,//�����
	OBJ_MAKET,//�����
	OBJ_RUN,//����������� �������
	OBJ_CLASS,
	OBJ_DOC,//������������
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
	BOOL bEdit;//������� ���������� �������������� (�������� � ��������� �����)
	BOOL bGroup;//������� ��� ��� ������
	int  nType;//��� ��������

	int  nChildImage;//�������� ��� ����. ��������� ������
	CProcUnit	*pRun;//������, � ������� ���������� ����������� �������
	CString csFunction;//��� �������, ���� ��� ����������� �������
	CValue vData;//��������. ���������� ����������� � �������� ��������� ��� ������ �������

	CString  sType;//��������� �������� ���� (���� �� ������, �� ������������ ���)
	int		 nLength;
	int		 nPrec;


	//������:
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


	//��������:
	CMapStringToPtr ListObjects;//������ �������� �������� ������������
	//CMap <int,int,CString,CString&> ListObjectID;//������ ������������ ��������������� � ���� ��������
	CArray <CString,CString&> ListModuleName;//������ ���� ����� �������
	CArray <CString,CString&> ListFormName;//������ ���� ����� ����
	CArray <CString,CString&> ListMaketName;//������ ���� ����� �������
	CArray <CString,CString&> ListObjectName;//������ ���� ��������
	CMapStringToPtr OList;
	//CValue ObjectsArray;//����������� ������ - ������ �������� � �������
	CMapStringToPtr ListModuleAdd;
	CMapStringToPtr ListFormAdd;
	CMapStringToPtr ListMaketAdd;
	CMapStringToPtr ListObjectAdd;
//	CArray <CString,CString&> ListMetadataName;//������ ����������

	CValue vMeta;

private:
	CZipArchive m_zip;
	CString m_csConfigPath;
	int nZipState;


	//��������� ��������� ��������
	BOOL bIsLibrary;
	CArray <CMetadata *,CMetadata *> aLibList;

public:
	//������:
	void AllFind(CString csFind,int nCase,int nWorld);

	void AddToMeta(CString Str);
	void RenameMetaObject(CMetaObject *pObj,CString csNewName);
	void RemoveMetaObject(CMetaObject *pObj);
	//��������� ����������� (���� ����� ���������� ��� �� �������� �� �����, �� �� �����������)
	CMetaObject *GetMetaObject(CString csFileName,CString csObjName,int bReadOnly=0);
	CMetaObject *GetMetaObject2(CString csFileName,CString csObjName,int bReadOnly);

	CString ReadFileToString(CString csKey,CString &csAlias,CString &csComment);
	int WriteFileFromString(CString csModule,CString csKey,CString csAlias,CString csComment);
	int WriteFileFromStream(void *pBuffer,int nSize,CString csFile,CString csKey,CString csAlias,CString csComment);
	int DeleteData(CString csPath);
	
	void Clear();
	BOOL Load(CString csFileName);//���������� ������ ����������
	BOOL Save();//���������� � ���� �������� �� �������������� ��������
	BOOL OpenZip(int nCurState);

	//��������� ��������� ��������
	void AddLibrary(CString csFileName);
	void LoadSystemLibrary();

private:

};

#endif // !defined(AFX_METADATA_H__866C7CF9_A3C2_4D0B_A113_4FB02338C89E__INCLUDED_)
