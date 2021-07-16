// MetaObject.cpp: implementation of the CMetaObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "MetaObject.h"
#include "MetaData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Compiler\\OutToDebug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMetaObject::CMetaObject()
{
	nID=0;
	pTree=0;
	IsDeleted=0;
	nTempID=0;
	nTypeObject=0;
	bCanSave=1;
	bEnable=1;
	vExtVal.CreateObject("Структура");
}

CMetaObject::~CMetaObject()
{
}

void CMetaObject::SaveToInfo()
{
	if(!vExtVal.GetSizeStrArray())
		vExtVal.CreateObject("Структура");
	//сохранение доп. информации
	vExtVal.SetAt("Name",String(csName));
	vExtVal.SetAt("Alias",String(csAlias));
	vExtVal.SetAt("Comment",String(csComment));
	vExtVal.SetAt("Enable",bEnable);
	vExtVal.SetAt("Param1",String(csParam1));
	vExtVal.SetAt("NameDB",String(csIdDB));

	vExtVal.SaveToString(csInfo);

}
void CMetaObject::LoadFromInfo()
{
	//восстановление доп. информации
	vExtVal.LoadFromString(csInfo);
	if(vExtVal.GetSizeStrArray())
	{
//		csName=vExtVal.GetAt("Name").GetString();
//		csAlias=vExtVal.GetAt("Alias").GetString();
//		csComment=vExtVal.GetAt("Comment").GetString();
		bEnable=vExtVal.GetAt("Enable")?1:0;
		csParam1=vExtVal.GetAt("Param1").GetString();
		csIdDB=vExtVal.GetAt("NameDB").GetString();
	}
	else
	{
		vExtVal.CreateObject("Структура");
	}
}


BOOL CMetaObject::Save(CZipArchive &m_zip,CString csFileName)
{
	if(!bCanSave&&!IsDeleted)
		return 0;
	if(IsDeleted==0 && csFileName.IsEmpty())
		return 0;

	//удаление старой копии
	if(!csPrevFileName.IsEmpty())
	{
		{
			DeleteFile(m_zip,csPrevFileName+".frm");
			DeleteFile(m_zip,csPrevFileName);
			DeleteFile(m_zip,csPrevFileName+".2c");
			DeleteFile(m_zip,csPrevFileName+".atr");
			DeleteFile(m_zip,csPrevFileName+".inf");
		}
	}
	if(IsDeleted||csFileName.IsEmpty())
	{
		DeleteFolder(m_zip,csPrevFileName);
		return TRUE;
	}

	int bMastSave=0;

	SaveToInfo();

	WriteFileFromString(csForm,m_zip,csFileName+".frm",csAlias,csComment);
	WriteFileFromString(csFile,m_zip,csFileName,csAlias,csComment);
	WriteFileFromString(csAttr,m_zip,csFileName+".atr",csAlias,csComment);
	WriteFileFromString(csInfo,m_zip,csFileName+".inf",csAlias,csComment);
	return TRUE;
}

BOOL CMetaObject::Load(CZipArchive &m_zip,CString csFileName,CString csObjName)
{
	csName=csObjName;
	csOldName=csObjName;
	csAlias="";
	csComment="";
	csIdDB="";
	csForm=ReadFileToString(m_zip,csFileName+".frm",csAlias,csComment);
	csFile=ReadFileToString(m_zip,csFileName,csAlias,csComment);
	int n=csFile.GetLength();
	if(csFile.IsEmpty())//поддержка старого формата
		csFile=ReadFileToString(m_zip,csFileName+".2c",csAlias,csComment);
	csAttr=ReadFileToString(m_zip,csFileName+".atr",csAlias,csComment);
	csInfo=ReadFileToString(m_zip,csFileName+".inf",csAlias,csComment);

	LoadFromInfo();

	csPrevFileName=csFileName;
	return TRUE;
}


//РАБОТА С АРХИВОМ
int DeleteFile(CZipArchive &m_zip,CString csPath)
{
	int nRes=m_zip.FindFile(csPath);
	if(nRes>=0)
	{
		m_zip.DeleteFile(nRes);
		return 1;
	}
	return 0;
}

int DeleteFolder(CZipArchive &m_zip,CString csPath)
{
	int nDelCount=0;
	if(!csPath.IsEmpty())
	{
		int nLength=csPath.GetLength()+1;
		for (WORD i = 0; i < m_zip.GetNoEntries(); i++)
		{
			CZipFileHeader fhInfo;
			m_zip.GetFileInfo(fhInfo, i);
			CString Str=fhInfo.GetFileName();
			if(csPath+"\\"==Str.Left(nLength)||csPath+"."==Str.Left(nLength))
			{
				m_zip.DeleteFile(i);
				i--;
				nDelCount++;
			}
		}
	}
	return (nDelCount>0);
}

void OpenNewFile(CZipArchive &m_zip,CString csPath,CString csComment,CString csFile)
{
	DeleteFile(m_zip,csPath);
	CZipFileHeader header;
	header.SetFileName(csPath);
	header.SetComment(csComment);
	if(csFile.IsEmpty())
		bool bRes=m_zip.OpenNewFile(header,Z_BEST_SPEED);
	else
		bool bRes=m_zip.OpenNewFile(header,Z_BEST_SPEED,csFile);
}

void WriteInt(CZipArchive &m_zip,int nValue)
{
	m_zip.WriteNewFile(&nValue,sizeof(nValue));
}

void WriteString(CZipArchive &m_zip,CString csStr)
{
	WriteInt(m_zip,csStr.GetLength());
	m_zip.WriteNewFile(csStr.GetBuffer(0),csStr.GetLength());
	//csStr.ReleaseBuffer();
}

int ReadInt(CZipArchive &m_zip)
{
	int nValue;
	m_zip.ReadFile(&nValue,sizeof(nValue));
	return nValue;
}

CString ReadString(CZipArchive &m_zip)
{
	CString csStr;
	int nSize=ReadInt(m_zip);
	m_zip.ReadFile(csStr.GetBuffer(nSize),nSize);
	csStr.ReleaseBuffer(nSize);
	return csStr;
}

CString ReadFileToString(CZipArchive &m_zip,CString csKey,CString &csAlias,CString &csComment)
{
	CString csModule;
	if(!csKey.IsEmpty())
	{
		int nRes=m_zip.FindFile(csKey);
		if(nRes>=0)
		{
			bool bRes=m_zip.OpenFile(nRes);
			CZipFileHeader fhInfo;
			m_zip.GetFileInfo(fhInfo, nRes);
			CString csDescription=fhInfo.GetComment();
			int nIndex=csDescription.Find("\n");
			csAlias=csDescription.Left(nIndex);
			csComment=csDescription.Mid(nIndex+1);
			DWORD nRes=m_zip.ReadFile(csModule.GetBuffer(fhInfo.m_uUncomprSize),fhInfo.m_uUncomprSize);
			csModule.ReleaseBuffer(nRes);
			m_zip.CloseFile();
		}
	}
	return csModule;
}

int WriteFileFromString(CString csModule,CZipArchive &m_zip,CString csKey,CString csAlias,CString csComment,BOOL bMastSave)
{
	if(csKey.IsEmpty())
		return 0;
	if(bMastSave||(!csModule.IsEmpty()))
	{
		OpenNewFile(m_zip,csKey,csAlias+"\n"+csComment,"");
		m_zip.WriteNewFile(csModule.GetBuffer(0),csModule.GetLength());
		csModule.ReleaseBuffer();
		m_zip.CloseNewFile();
		return 1;
	}
	return 0;
}

int WriteFileFromStream(void *pBuffer,int nSize,CString csFile,CZipArchive &m_zip,CString csKey,CString csAlias,CString csComment,BOOL bMastSave)
{
	if(csKey.IsEmpty())
		return 0;
	if(bMastSave||(!sizeof(pBuffer)))
	{
		OpenNewFile(m_zip,csKey,csAlias+"\n"+csComment,csFile);
		m_zip.WriteNewFile(pBuffer, nSize);
		delete []pBuffer;
		m_zip.CloseNewFile();
		return 1;
	}
	return 0;
}
