// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueObject.cpp: implementation of the CValueObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueObject,CValueControl);

CValueObject::CValueObject()
{
	nType=100;
	bReadOnly=1;
	pRun=0;
	nMethodGetAttr=-1;
	nMethodSetAttr=-1;
	bHasParent=0;
}
 
CValueObject::~CValueObject()
{
	if(pRun) 
	{
		pRun->CallFunction("����������");
		delete pRun;
	}
}

void CValueObject::SetObjectType(CString StrTypeVid)
{
	//������ ��� ������ �������
	csObjectName=StrTypeVid;

	//��� ������� 
	int nIndex=csObjectName.Find(".");
	if(nIndex>=0)
	{
		//��� �������
		csObjectType=csObjectName.Left(nIndex);
		csObjectKind=csObjectName.Mid(nIndex+1);
	}
	else
	{
		csObjectType=csObjectName;
		csObjectKind="";
	}

	csObjectType.TrimRight();
	csObjectType.TrimLeft();
}

//�������� (�������������) ������ ������������ ������ � ������
void CValueObject::Init(CString StrTypeVid,CValue Param)
{
	ASSERT(AfxGetModuleManager());

	SetObjectType(StrTypeVid);
	if(AfxGetModuleManager())
		pRun=AfxGetModuleManager()->GetNewRunModule(OBJECTSNAME+"\\"+csObjectType+"\\"+OBJMODULE+"\\"+ENTERPRISEMODULENAME);
	if(!pRun)
	{
		CString csStr;
		csStr.Format("������ �������� ������������ %s (������ �������� ������)",csObjectName);
		Error(csStr.GetBuffer(0));
	}

	InitObject(pRun,Param);
}

void CValueObject::InitObject(CProcUnit *pSetRun,CValue Param)
{
	static int nLevel=0;
	nLevel++;
	if(nLevel>=MAX_OBJECTS_LEVEL)
		Error(CString("����������� ����� ������� ")+csObjectName);

	bHasParent=0;

	pRun=pSetRun;
	pRun->pCurrentObj=this;
	//�������� = ������ �� ������
	//pRun->cCurContext.cRefLocVars[0]=this;//!
	pRun->cCurContext.pRefLocVars[0]=this;

	//���� ��������
	for(int i=0;i<pRun->GetParentCount();i++)
	{
		CProcUnit *pCurRun=pRun->GetParent(i);
		ASSERT(pCurRun->pByteCode);
		if(pCurRun->pByteCode->csModuleName.Right(ENTERPRISEMODULENAME.GetLength())==ENTERPRISEMODULENAME)
		{
			int nType;
			CString csName=AfxGetModuleManager()->GetObjectName(pCurRun->pByteCode->csModuleName,nType);

			//������� �������� ������� �� ���� ��� ���������������� � ������ ������
			CValueObject *p=new CValueObject();
			p->SetObjectType(csName);
			p->InitObject(pCurRun,Param);

			pRun->cCurContext.pRefLocVars[1]=p;
			vParent=ValueByRef(p);
			break;
		}
	}

	bHasParent=vParent.GetType();
	SetParentContext();

	ASSERT(pRun);
	pRun->CallFunction("�����������",String(csObjectKind),Param);
	nLevel--;
}

//________________________________________________________________________
//����������� �������� �������� ���� ���������
void CValueObject::SetParentContext()//��� ������������ ����� ������� ��������
{
	if(bHasParent)
	if(vParent.pRef)
	{
		if(vParent.pRef->GetRuntimeClass()==&classCValueObject)
		{
			CValueObject *pParent=(CValueObject*)vParent.pRef;
			//�������� = ������ �� ������
			//pParent->pRun->cCurContext.cRefLocVars[0]=this;//!
			pParent->pRun->cCurContext.pRefLocVars[0]=this;
			pParent->SetParentContext();
		}
	}
}

#define DELTA_COUNT	1000000//������������ ���������� �������/����� ������
//________________________________________________________________________
CValue CValueObject::Method(int iName,CValue **aParams)
{
#ifdef _DEBUG2
	if(pRun)
	{
		static int nGetName=0;
		if(!nGetName)
		{
			nGetName=1;
			csName=pRun->CallFunction("����������������������������").GetString();
			nGetName=0;
		}
	}
#endif

	int nFound=1;
	if(iName>=DELTA_COUNT)
	{
		nFound=0;
		iName=iName-DELTA_COUNT;
	}

	if(nFound)
	{
		if(iName>=0)
			return pRun->CallFunction(iName,aParams);
	}
	if(bHasParent)
		return vParent.Method(iName,aParams);

	return CValue();
}

//________________________________________________________________________
int  CValueObject::FindMethod(CString csName)
{
	int n=(int)pRun->FindExportFunction(csName);
	if(n>=0)
	{
		return n;
	}
	if(bHasParent)
	{
		n=vParent.FindMethod(csName);
		if(n>=0)
			return DELTA_COUNT+n;
	}
	return -1;
}

//________________________________________________________________________
CString csTempAttrName;
int  CValueObject::FindAttribute(CString csName)
{
	csTempAttrName=mUpper(csName);
	return 0;
}
//________________________________________________________________________
CValue CValueObject::GetAttribute(int )//��������� �������� ��������
{
	if(!pRun)
		return CValue();
	//������� ����� � ������ ���� ���������� ���������� (� ����� ���������� ��������� ������ ���������)
	int nVar=pRun->FindAttribute(csTempAttrName);
	if(nVar>=0)
		return pRun->GetAttribute(nVar);

	if(nMethodGetAttr==-1)
		nMethodGetAttr=(int)pRun->FindFunction("���������������");
	if(nMethodGetAttr==-1)
	{
		if(bHasParent)
		{
			nVar=vParent.FindAttribute(csTempAttrName);
			if(nVar>=0)
				return vParent.GetAttribute(nVar);
		}

		CString csStr;
		csStr.Format("���� (%s) ����������� ������� �� ����������",csTempAttrName);
		Error(csStr.GetBuffer(0));
	}

	CValue Attr;
	Attr.SetString(csTempAttrName);
	return pRun->CallFunction(nMethodGetAttr,Attr);
}
void CValueObject::SetAttribute(int ,CValue &Val)//��������� ��������
{
	if(!pRun)
		return;
	//������� ����� � ������ ���� ���������� ���������� (� ����� ���������� ��������� ������ ���������)
	int nVar=pRun->FindAttribute(mUpper(csTempAttrName));
	if(nVar>=0)
	{
		pRun->SetAttribute(nVar,Val);
		return;
	}

	if(nMethodSetAttr==-1)
		nMethodSetAttr=(int)pRun->FindFunction("�����������������");
	if(nMethodSetAttr==-1)
	{
		if(bHasParent)
		{
			nVar=vParent.FindAttribute(csTempAttrName);
			if(nVar>=0)
			{
				vParent.SetAttribute(nVar,Val);
				return;
			}
		}

		CString csStr;
		csStr.Format("���� (%s) ����������� ������� �� ����������",csTempAttrName);
		Error(csStr.GetBuffer(0));
	}

	CValue Attr;
	Attr.SetString(csTempAttrName);
	pRun->CallFunction(nMethodSetAttr,Attr,Val);
}
int  CValueObject::GetNAttributes(void)
{
	if(!pRun)
		return 0;
	int nExport=pRun->pByteCode->ExportVarList.GetCount();
	int nInner=pRun->CallFunction("���������������������������");

	int nParent=0;
	if(bHasParent)
		nParent=vParent.GetNAttributes();

	return nExport+nInner+nParent;
}
CString CValueObject::GetAttributeName(int nNumber,int nAlias)
{
	if(!pRun)
		return "";
	int nExport=pRun->pByteCode->ExportVarList.GetCount();

	if(nNumber<nExport)
	{
		void* p;
		POSITION pos;
		CString Key;
		int nCount=0;
		for( pos = pRun->pByteCode->ExportVarList.GetStartPosition(); pos != NULL; )
		{
			pRun->pByteCode->ExportVarList.GetNextAssoc( pos, Key, p );
			if(nCount==nNumber)
			{
				csTempAttrName=Key;
				return Key;
			}
			nCount++;
		}

	}
	nNumber-=nExport;

	int nInner=pRun->CallFunction("���������������������������");
	if(nNumber<nInner)
	{
		csTempAttrName=pRun->CallFunction("�������������������",CValue(nNumber+1)).GetString();
		return csTempAttrName;
	}
	nNumber-=nInner;

	if(bHasParent)
		return  vParent.GetAttributeName(nNumber,nAlias);

	csTempAttrName="";

	return csTempAttrName;
}


void CValueObject::SaveToString(CString &Str)
{
	CString csTypeVid=csObjectName;
	int nIndex=csTypeVid.Find(".");
	if(nIndex==-1)
	{
		CString csVid=CallFunction("���").GetString();
		if(!csVid.IsEmpty())
			csTypeVid.Format("%s.%s",csObjectName,csVid);
	}

	Str.Format("{%s/%s}",csTypeVid,CallFunction("����������������������������").GetString());
}
void CValueObject::LoadFromString(CString &Str)
{ 
	int nIndex1=Str.Find("/");
	CValue cVal=String(Str.Mid(nIndex1+1));
	cVal.sData.TrimRight('}');
	CallFunction("������������������������������",cVal);
}
CString CValueObject::GetString(void)const
{
	if(((CValueObject*)this)->FindMethod("����������������������������")<0)
		return GetTypeString();
	return ((CValueObject*)this)->CallFunction("����������������������������").GetString();
}
CString CValueObject::GetTypeString(void)const
{
	CString Str=((CValueObject*)this)->CallFunction("������������������").GetString();
	if(Str.IsEmpty())
		return csObjectType;
	else
		return Str;
}


void CValueObject::SetValue(CValue &Val)
{
	int  nSetValue=FindMethod("������������������");
	if(nSetValue>=0)
		CallFunction("������������������",Val);
	if(pWnd)
		((CWnd *)pWnd)->SetWindowText(GetString());
}


#define MAX_VAR_PARAMS	10
CValue ParamMas2[MAX_VAR_PARAMS];
CValue **GetArray2()
{
	CValue **ppParams2=new CValue*[MAX_VAR_PARAMS];
	for(int i=0;i<MAX_VAR_PARAMS;i++)
		ppParams2[i]=&ParamMas2[i];
	return ppParams2;
}

CValue CValueObject::CallFunction(CString csName)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValueObject::CallFunction(CString csName,CValue &vParam1)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	ppParams[0]=&vParam1;
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValueObject::CallFunction(CString csName,CValue &vParam1,CValue &vParam2)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2; 
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}

BOOL CValueObject::IsEmpty(void)
{
	if(CallFunction("������").GetNumber())
		return 0;
	else
		return 1;
}
