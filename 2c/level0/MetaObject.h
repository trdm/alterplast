// ��������� ����� - VTOOLS.RU (info@vtools.ru)
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

	//��������:
	int nID;				//���������� ��� (�����) �������
	CString csName;			//��� ������� (�������������)
	CString csAlias;		//������� �������
	CString csComment;		//����������� �������
	bool	bEnable;		//������������� ������� � ������������
	CString csIdDB;		    //������������� ��
	//int		nImage;

//	class CObjectData *pData;		//�������������� ������ �� ��������� ������� � �������������
//	BOOL	bAutoDeleteData;//�������������� ������
	CValue vExtVal;


	CString csInfo;			//�������� �������
	CString csFile;			//���������� ����� ������
	CString csForm;			//���������� ����� ������
	CString csAttr;			//�������� ���������

	CString csParam1;

	BOOL IsDeleted;			//������� ��������
	CString csPrevFileName;	//���� � ���������� ��� �������� (��� ������� ��� ����������)

	HTREEITEM nTempID;		//����� � ����������� ��������� �������������
	class CMetadataTree *pTree;

	int bCanSave;
	int	nTypeObject;
	CString csOldName;		//��� ��� �������� � ������

	//������:
	BOOL Save(CZipArchive &m_zip,CString csFileName);
	BOOL Load(CZipArchive &m_zip,CString csFileName,CString csObjName);

	void SaveToInfo();
	void LoadFromInfo();

//	CObjectData cData;
	//�������� � ������ ObjectInfo
//	CString csParent;	//��� ��������
//	CString csName;		//��� ������� (�������������)
//	int nArhivType;		//����� ��������: ObjectInfo,ObjectFile,FormFiles
//	int		nParent;	//��������
	//�������� � ������ ObjectFile � FormFiles
	//CString csTypeFile;	//��� �����
//	BOOL nInSave;

	//����������
//	int nStatus;		//������ �������������� (STATUS_EDIT,STATUS_REMOVE)

//	CString csPrevName;	//������ ��� ������� - ����� ������������ ��� ����������� �������� ���� �������
//	CString csObjectPath;//���� � ���������� ��� �������� (��� ������� ��� ����������)



//public:
/*
	void InitPath(CString csKey);
	BOOL SaveInfo(CZipArchive &m_zip);
	BOOL GetInfo(CZipArchive &m_zip,CString csKey);
	BOOL GetFile(CZipArchive &m_zip,CString csKey);//csKey - ������ ���� � �����
	BOOL SaveFile(CZipArchive &m_zip);//csFileName - ����� ��� �����

	BOOL GetForm(CZipArchive &m_zip,CString csKey);//csKey - ���� � �����
	BOOL SaveForm(CZipArchive &m_zip);
*/
};

#endif // !defined(AFX_METAOBJECT_H__11E8EE01_C3E8_4519_9C86_934111848FCB__INCLUDED_)
