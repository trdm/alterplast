#include "stdafx.h"
#include "DialogUnit.h"
#include "FormUnit.h"

#include "OpenForm.h"
#include "MicroForm.h"


CRITICAL_SECTION m_criticalSection;
BOOL bInitCriticalSection=0;



/////////////////////////////////////////////////////////////////////////////
CMetadataTree *afxMetaTree;//������� ������ ����������
CMetaObject *afxMetaObj;	//������� ������ ����������


/////////////////////////////////////////////////////////////////////////////
CDocument* OpenFormMDI(CString csClassName,int bVisible,CMetadataTree *pMetaTree,CMetaObject *pMetaObj)
{
	CLock cs(m_criticalSection, "OpenFormMDI",bInitCriticalSection);

	afxMetaTree=pMetaTree;
	afxMetaObj=pMetaObj;
	CDocument* pDoc=CEnterpriseApp::m_pSystemManager->OpenForm(csClassName,bVisible);
	afxMetaTree=0;
	afxMetaObj=0;
	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
//�������� ������������ ����� 2�
//��� ��������� ���������� ���� ���������� ����������!!!
/////////////////////////////////////////////////////////////////////////////
CValue afxFormParam;//�������� ��� �����
CValue afxFormContext;//�������� �������� �����
CString afxFormPath;//���� ������� �����
CString afxFormID;//���������� �� ����������� �����
CValue afxChoiceContext;//��������, �� �������� ������ ������
CValue afxCurrentValue;//������� �������� (��� ������ �����)
int afxChoiceMode=0;


////////////////////////////////////////////////////////////////////////////
//nMode:
#define NORMAL_MODE		0
#define MODAL_MODE		1
#define EDITABLE_MODE	2
#define MICROFORM_MODE	3// (or pControl!=0)
int OpenFormExt2(CString csFormName,CValue &vContext, CValue Param,CString csFileName,CString csFormID,int nMode,  CValue vChoiceContext,int nChoiceMode,CValue vCurrentValue, CMicroForm* pControl,CWnd *pParent)
{

	afxFormPath="";
	afxCurrentValue=vCurrentValue;
	if(!pControl)
		afxChoiceContext=vChoiceContext;
	afxChoiceMode=nChoiceMode;
	
	if(csFormID.IsEmpty() && (!csFileName.IsEmpty()))
		csFormID=csFileName;

	if(!pControl)
	{
		if(!csFormID.IsEmpty())
		{
			if(CDialogUnit::FindOpen(csFormID))
			{
				vContext=afxFormContext;
				return 1;
			}
			if(CFormUnit::FindOpen(csFormID))
			{
				vContext=afxFormContext;
				return 1;
			}
		}
		
	}
	try
	{
		int Ret=AfxGetModuleManager()->FindForm(csFormName,csFileName,afxMetaObj); 
		if(!Ret)
		{
			if(!pControl)
				return 0;
		}
	}
	catch(...)
	{
		afxMetaObj=new CMetaObject();

	}
	afxFormParam=Param;
	afxFormID=csFormID;

	if(pControl)//����������
	{
		try
		{
			pControl->Create((LPCTSTR )IDD_FORMUNIT,pParent);
		}
		catch(...)
		{
		}
		vContext=afxFormContext;
		return 1;
	}


	if(MODAL_MODE==nMode || START_MODE==afxAppRunMode)
	{
		CDialogUnit dlg;
		dlg.DoModal();
		vContext=afxFormParam;
	}
	else
	if(NORMAL_MODE==nMode && CONFIG_MODE==afxAppRunMode)
	{
		OpenFormMDI("����������������",1,0,afxMetaObj);
		vContext=afxFormContext;
	}
	else
	if(EDITABLE_MODE==nMode || ENTERPRISE_MODE==afxAppRunMode)
	{
		OpenFormMDI("�����",1,0,afxMetaObj);
		vContext=afxFormContext;
	}
	
	return 1;
}

//������� ������ �������� ����� � ����������� ��. ���������� � �����
int OpenFormExt(CString csFormName,CValue &vContext, CValue Param,CString csFileName,CString csFormID,int nMode,  CValue vChoiceContext,int nChoiceMode,CValue vCurrentValue, CMicroForm* pControl,CWnd *pParent)
{
	CLock cs(m_criticalSection, "OpenFormExt",bInitCriticalSection);

	//���������� ����. ����������
	CValue _afxFormParam=afxFormParam;
	CValue _afxFormContext=afxFormContext;
	CString _afxFormPath=afxFormPath;
	CString _afxFormID=afxFormID;
	CValue _afxChoiceContext=afxChoiceContext;
	int _afxChoiceMode=afxChoiceMode;
	CMetaObject *_afxMetaObj=afxMetaObj;
	CValue _afxCurrentValue=afxCurrentValue;

	//����� ����������� ���������
	int nReturn=OpenFormExt2(csFormName,vContext,Param,csFileName,csFormID,nMode,vChoiceContext,nChoiceMode,vCurrentValue,pControl,pParent);

	//��������������� ����. ����������
	afxFormParam=_afxFormParam;
	afxFormContext=_afxFormContext;
	afxFormPath=_afxFormPath;
	afxFormID=_afxFormID;
	afxChoiceContext=_afxChoiceContext;
	afxChoiceMode=_afxChoiceMode;
	afxMetaObj=_afxMetaObj;
	afxCurrentValue=_afxCurrentValue;
	return nReturn;
}

