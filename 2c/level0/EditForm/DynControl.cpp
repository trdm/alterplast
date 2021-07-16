// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ============	===========	=======	===============================================

#include "stdafx.h"
#include "DynControl.h"
#include "../resource.h"
#include "../1CCtrl.h"
#include "../Edit/amsEdit.h"
#include "../Edit/xrnumericedit.h"
#include "../Resource.h"
#include "../DialogUnit.h"
#include "../Compiler/ValueArray.h"
#include "../ImageButton/ImageButton.h"
#include "../MetadataTree.h"
#include "../IconListBox.h"
#include "../GridCtrl_src/GridCtrl.h"
#include "../MicroForm.h"
#include "../OpenForm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEFAULT_DIALOG_H	22

//���������������� ������ ��������� ��������� ������ ��������
int nTypeValue=2;
int nTypeObject=3;
int nTypeLayers=4;
int nTypeAllStrings=5;
int nTypeVariable=6;
char *aChooseList[]=
{
	"����,�����,���",//0
	"����,�����,�����",//1
	"",//2 ��� ��������
	"",//3 ������
	"",//4 ����
	"",//5 ������������ ������
	"",//6 ����������
};


int GetPosNumber(CString Str,int nList)
{
	CString csValueStr=aChooseList[nList];
	csValueStr+=",";
	int nNumber=0;
	int nIndexPrev=-1;
	int nIndex=csValueStr.Find(",");
	while(nIndex>=0)
	{
		CString csName=csValueStr.Mid(nIndexPrev+1,nIndex-nIndexPrev-1);
		if(csName==Str)
			return nNumber;

		nIndexPrev=nIndex;
		nIndex=csValueStr.Find(",",nIndex+1);
		nNumber++;
	}
	return 0;
}


CImageList *GetImageList()
{
	static bool bPictureForIcon=0;
	static CBitmap BitmapForIcon2;
	static CBitmap BitmapForIcon3;
	static CBitmap BitmapForIcon4;
	static CBitmap BitmapForIcon5;
	static CBitmap BitmapForIcon6;
	static CBitmap BitmapForIcon7;
	static CBitmap BitmapForIcon8;
	static CImageList PictureForIcon;

	if(bPictureForIcon==0)
	{
		BitmapForIcon2.LoadBitmap(IDB_BITMAP2);
		BitmapForIcon3.LoadBitmap(IDB_BITMAP3);
		BitmapForIcon4.LoadBitmap(IDB_BITMAP4);
		BitmapForIcon5.LoadBitmap(IDB_BITMAP5);
		BitmapForIcon6.LoadBitmap(IDB_BITMAP6);
		BitmapForIcon7.LoadBitmap(IDB_BITMAP7);
		BitmapForIcon8.LoadBitmap(IDB_BITMAP8);
		PictureForIcon.Create(MAKEINTRESOURCE(IDB_BITMAP1), 16, 16, RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon2,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon3,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon4,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon5,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon6,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon7,RGB(0,128,128));
		PictureForIcon.Add(&BitmapForIcon8,RGB(0,128,128));
		bPictureForIcon=1;
	}
	return &PictureForIcon;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int nUniqueID=FORM_START_ID;

void CDynControl::ResetID()
{
	nUniqueID=FORM_START_ID;
}

CDynControl::CDynControl(CWnd* pWnd, int nControlType, BOOL bConfigMode,CString csName)
{
	// construct a new control
	//nMaxDataList=0;
	nUniqueID++;
	if(nUniqueID>FORM_FINISH_ID)
		ResetID();


	IsNew=1;

	m_bConfigMode=bConfigMode;
	// font for labels
	LOGFONT logfontDefaultFont;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), 
        &logfontDefaultFont); 
	m_fontDefault.CreateFontIndirect(&logfontDefaultFont);

	// initialise
	m_pParentWnd = pWnd;
	m_nControlType = nControlType; 
	m_nID = nUniqueID;
	nCreate = nUniqueID;

	m_bVisible = TRUE;
	m_bEnable = TRUE;
	
	m_csValue = "";
	m_nTabOrder = 0;

	m_bAutoSizeDown=0;
	m_bAutoSizeRight=0;

	m_pWnd = NULL;
	bWasEdit=0;

	m_csObjName=csName;


	m_x1=0;
	m_y1=0;
	m_x2=50;
	m_y2=DEFAULT_DIALOG_H;
	m_x3=0;
	m_y3=0;

	m_csLayer=DEFAULT_LAYER_NAME;


}

CDynControl::~CDynControl()
{
	if (m_pWnd != NULL)
		delete m_pWnd;

}

void CDynControl::EnableWindow(BOOL bEnable)
{
	// Enable Controls

	// different controls are enabled differently
	// so you may need to enhance this in order to put
	// do your own bits!	
	if (bEnable == FALSE)
	{
		m_pParentWnd->GetDlgItem(m_nID)->EnableWindow(FALSE);
	}

	if (bEnable == TRUE)
	{
		m_pParentWnd->GetDlgItem(m_nID)->EnableWindow(TRUE);
	}
}


void CDynControl::SaveToValue(CValue &ObjectVal)
{
	if(!m_pWnd)
		return;

	ObjectVal.SetAt("�����������",m_nControlType);
	ObjectVal.SetAt("����������",String(m_csObjName));

	ObjectVal.SetAt("nID",m_nID);

	
	ObjectVal.SetAt("�������������",String(m_csName));
	ObjectVal.SetAt("��������",String(m_csDescription));

	ObjectVal.SetAt("���������",String(m_csValue));
	ObjectVal.SetAt("��������� ����",String(m_csVAllign));
	ObjectVal.SetAt("��������� �����",String(m_csHAllign));

	ObjectVal.SetAt("����",m_bAutoSizeDown);
	ObjectVal.SetAt("������",m_bAutoSizeRight);
	ObjectVal.SetAt("����",m_csLayer);
	

	ObjectVal.SetAt("���������",m_bVisible);
	ObjectVal.SetAt("�����������",m_bEnable);
	ObjectVal.SetAt("���������� ��� �����",m_nTabOrder);

	for(int i=0;i<aDataList.GetSize();i++)
	{
		ControlData data=aDataList[i];
		int nType=data.nType;
		if(nType==FUNCTION_DATA||nType==STRING_DATA||nType==LIST_DATA)
			ObjectVal.SetAt(data.csName,String(data.sData));
		else
		if(nType==INT_DATA||nType==COLOR_DATA)
			ObjectVal.SetAt(data.csName,data.nData);
		else
		if(nType==BOOL_DATA)
			ObjectVal.SetAt(data.csName,data.bData?1:0);
		else
		if(nType==DOUBLE_DATA)
			ObjectVal.SetAt(data.csName,data.fData);
	}

	CRect Rect;
	m_pWnd->GetWindowRect(Rect);
	m_pParentWnd->ScreenToClient(Rect);
	

	ObjectVal.SetAt("x1",Rect.left);
	ObjectVal.SetAt("y1",Rect.top);
	ObjectVal.SetAt("x2",Rect.right);
	ObjectVal.SetAt("y2",Rect.bottom);


	if(m_nControlType==ID_DYNBUTTONGRID)
	if(bWasEdit)
	{
		CGridCtrl *pGrid=(CGridCtrl *)m_pWnd;
		for(int nCol=0;nCol<pGrid->GetColumnCount();nCol++)
		{
			CValue Val;
			Val.CreateObject("���������");
			CGridCell* pCell = (CGridCell*)pGrid->GetCell(0, nCol);
			if (pCell)
			{
				Val.SetAt("�������������",String(pGrid->aColumns[nCol].csName));
				Val.SetAt("��������",String(pGrid->aColumns[nCol].csDescription));
				Val.SetAt("���������",String(pCell->TextData()));
				Val.SetAt("�������",String(pGrid->aColumns[nCol].csFormule));
				Val.SetAt("��� ��������",String(pGrid->aColumns[nCol].csType));
				Val.SetAt("�����",pGrid->aColumns[nCol].nLength);
				Val.SetAt("��������",pGrid->aColumns[nCol].nPrec);
				Val.SetAt("���������",pGrid->aColumns[nCol].bVisible);
				Val.SetAt("�����������",pGrid->aColumns[nCol].bEnable);
				Val.SetAt("���������� ��� �����",pGrid->aColumns[nCol].bTab);
				Val.SetAt("������ �������",pGrid->aColumns[nCol].nColumnWidth);
			}
			ObjectVal.SetAt(nCol+1,Val);
		}
		ObjectVal.SetAt("WasEdit",1);
		ObjectVal.SetAt("����� �������",pGrid->GetColumnCount());
	}

}

void CDynControl::AddControl(CValue &ObjectVal)
{
	if(ObjectVal.GetSizeStrArray()==0)
		return;
	m_csObjName=ObjectVal.GetAt("����������");
	m_nControlType=ObjectVal.GetAt("�����������");

	int nID=ObjectVal.GetAt("nID");
	if(nID>=FORM_START_ID && nID<=FORM_FINISH_ID)
	{
		m_nID=nID;
	}

	CRect Rect;
	Rect.left=ObjectVal.GetAt("x1");
	Rect.top=ObjectVal.GetAt("y1");
	Rect.right=ObjectVal.GetAt("x2");
	Rect.bottom=ObjectVal.GetAt("y2");

	m_x1=Rect.left;
	m_y1=Rect.top;
	m_x2=Rect.right;
	m_y2=Rect.bottom;
	m_x3=m_x1;
	m_y3=m_y1;

	m_csName=ObjectVal.GetAt("�������������");
	m_csDescription=ObjectVal.GetAt("��������");
	m_csValue=ObjectVal.GetAt("���������");
	m_csVAllign=ObjectVal.GetAt("��������� ����");
	m_csHAllign=ObjectVal.GetAt("��������� �����");

	m_bAutoSizeDown=ObjectVal.GetAt("����")!=0;
	m_bAutoSizeRight=ObjectVal.GetAt("������")!=0;
	m_csLayer=ObjectVal.GetAt("����");
	if(m_csLayer.IsEmpty())
		m_csLayer=DEFAULT_LAYER_NAME;

	m_bVisible=ObjectVal.GetAt("���������")!=0;
	m_bEnable=ObjectVal.GetAt("�����������")!=0;
	m_nTabOrder=ObjectVal.GetAt("���������� ��� �����")!=0;

	if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
	{
		m_Property=ObjectVal;
	}
	else
	{
		PrepareDataArray();

		for(int i=0;i<aDataList.GetSize();i++)
		{
			int nType=aDataList[i].nType;
			if(nType==FUNCTION_DATA||nType==STRING_DATA||nType==LIST_DATA)
				aDataList[i].sData=ObjectVal.GetAt(aDataList[i].csName);
			else
			if(nType==INT_DATA||nType==BOOL_DATA||nType==COLOR_DATA)
				aDataList[i].nData=ObjectVal.GetAt(aDataList[i].csName);
			else
			if(nType==DOUBLE_DATA)
				aDataList[i].fData=ObjectVal.GetAt(aDataList[i].csName);
		}
	}


	AddControl(Rect);

	if(m_nControlType==ID_DYNBUTTONGRID)
	{
		bWasEdit=ObjectVal.GetAt("WasEdit");
		if(bWasEdit)
		{
			CGridCtrl *pGrid=(CGridCtrl *)m_pWnd;
			int nColumn=ObjectVal.GetAt("����� �������");
			pGrid->SetColumnCount(nColumn);
			for(int nCol=0;nCol<pGrid->GetColumnCount();nCol++)
			{
				CValue Val;
				Val=ObjectVal.GetAt(nCol+1);
				CGridCell* pCell = (CGridCell*)pGrid->GetCell(0, nCol);
				if (pCell)
				if(Val.GetSizeStrArray())
				{
					pGrid->aColumns[nCol].csName=Val.GetAt("�������������");
					pGrid->aColumns[nCol].csDescription=Val.GetAt("��������");
					pCell->TextData()=Val.GetAt("���������");
					pGrid->aColumns[nCol].csTitle=pCell->TextData();
					pGrid->aColumns[nCol].csFormule=Val.GetAt("�������");
					pGrid->aColumns[nCol].csType=Val.GetAt("��� ��������");
					pGrid->aColumns[nCol].nLength=Val.GetAt("�����");
					pGrid->aColumns[nCol].nPrec=Val.GetAt("��������");

					pGrid->aColumns[nCol].bVisible=Val.GetAt("���������")?1:0;
					pGrid->aColumns[nCol].bEnable=Val.GetAt("�����������")?1:0;
					pGrid->aColumns[nCol].bTab=Val.GetAt("���������� ��� �����")?1:0;
					int W=Val.GetAt("������ �������");
					//if(W!=0)
					{
						pGrid->aColumns[nCol].nColumnWidth=W;
						pGrid->m_arColWidths[nCol]=W;
					}
				}
			}
		}
	}

}
void CDynControl::AddShowProperty(CPropertyBar *p,CString csModuleName,CString csVarValues)
{
	CString csIdValues="...";
	if(!csModuleName.IsEmpty())
	if(AfxGetModuleManager())
	{
		//����� ��������������� ���������
		CArray<CString,CString> aVariable;
		CArray<CString,CString> aType;
		CString csVarValues2;
		csVarValues2.Format(",%s,",csVarValues);
		if(AfxGetModuleManager()->GetAttrVariable(csModuleName,aVariable,aType))
		{
			for(int i=0;i<aVariable.GetSize();i++)
			{
				CString csName(aVariable[i]);
				csIdValues=csIdValues+","+csName;

				CString csName2;
				csName2.Format(",%s,",csName);
				if(csVarValues2.Find(csName2)==-1)
					csVarValues=csVarValues+","+csName;

			}
		}
	}


	if(m_nControlType==ID_DYNBUTTONGRID)
	{
		CGridCtrl *pGrid=(CGridCtrl *)m_pWnd;
		CCellRange Selection=pGrid->GetSelectedCellRange();
		if(Selection.Count()>0)
		{
			int nCol=Selection.GetMaxCol();
			if(nCol>=0)
			{
				CGridCell* pCell = (CGridCell*)pGrid->GetCell(0, nCol);
				if (pCell)
				{
					p->AddProperty(csPropertyName,"�������������",pGrid->aColumns[nCol].csName,csIdValues);
					p->AddProperty(csPropertyName,"���������",pCell->TextData());
					p->AddProperty(csPropertyName,"��������",pGrid->aColumns[nCol].csDescription);
					p->AddProperty(csPropertyName,"�������",pGrid->aColumns[nCol].csFormule);
					p->AddProperty(csPropertyName,"��� ��������",pGrid->aColumns[nCol].csType,aChooseList[nTypeValue]);
					p->AddProperty(csPropertyName,"�����",pGrid->aColumns[nCol].nLength);
					p->AddProperty(csPropertyName,"��������",pGrid->aColumns[nCol].nPrec,0,10);
					p->AddProperty(csPropertyName,"���������",pGrid->aColumns[nCol].bVisible);
					p->AddProperty(csPropertyName,"�����������",pGrid->aColumns[nCol].bEnable);
					p->AddProperty(csPropertyName,"���������� ��� �����",pGrid->aColumns[nCol].bTab);
					p->AddProperty(csPropertyName,"������ �������",pGrid->aColumns[nCol].nColumnWidth,0,1000);
					bWasEdit=1;
					return;
				}
			}
		}
	}


	p->AddProperty(csPropertyName,"�������������",m_csName,csIdValues);
	p->AddProperty(csPropertyName,"���������",m_csValue);
	p->AddProperty(csPropertyName,"��������",m_csDescription);
	p->AddProperty(csPropertyName,"���������",m_bVisible);
	p->AddProperty(csPropertyName,"�����������",m_bEnable);

	int i;
	for(i=0;i<aDataList.GetSize();i++)
	{
		if(aDataList[i].csName.IsEmpty())
			continue;
		if(aDataList[i].nType==STRING_DATA)
			p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].sData);
		else
		if(aDataList[i].nType==INT_DATA)
			p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].nData);
		else 
		if(aDataList[i].nType==BOOL_DATA)
			p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].bData);
		else
		if(aDataList[i].nType==COLOR_DATA)
			p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].Color);
		else
		if(aDataList[i].nType==LIST_DATA)
		{
			if(aDataList[i].nData==nTypeVariable)
			{
				p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].sData,csVarValues);
			}
			else
			{
				p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].sData,aChooseList[aDataList[i].nData]);
			}
		}
		else
		if(aDataList[i].nType==DOUBLE_DATA)
			p->AddProperty(csPropertyName,aDataList[i].csName,aDataList[i].fData);
	}
	for(i=0;i<aDataList.GetSize();i++)
	{
		if(aDataList[i].nType==FUNCTION_DATA)
			p->AddProperty("�������",aDataList[i].csName,aDataList[i].sData);
	}

	

	p->AddProperty("��������������","���������� ��� �����",m_nTabOrder);
	p->AddProperty("��������������","����",m_csLayer,aChooseList[nTypeLayers]);


//	p->AddProperty("��������������","��������� ����",m_csVAllign,"����,�����,���");
//	p->AddProperty("��������������","��������� �����",m_csHAllign,"����,�����,�����");
	p->AddProperty("����-���.�������","����",m_bAutoSizeDown);
	p->AddProperty("����-���.�������","������",m_bAutoSizeRight);


	if(m_nControlType==ID_BUTTONFORMOBJECT)
	{
		CString csPatchValues="...";
		CStringArray aList;
		CMetadataTree* pTree=AfxGetMetadataTree();
		if(pTree)
		{
			pTree->GetFormArray(aList);
			for(int i=0;i<aList.GetSize();i++)
			{
				csPatchValues=csPatchValues+","+aList[i];
			}
		}

		p->AddProperty(csPropertyName,"���� � �����",m_csObjName,csPatchValues);
	}

} 

void CDynControl::AddControl()
{
	CPoint p(0,0);
	AddControl(&p);
	if(m_pWnd)
		m_pWnd->MoveWindow(m_x1, m_y1, m_x2, m_y2,1);
}
void CDynControl::AddControl(CRect r)
{
	CPoint p(0,0);
	AddControl(&p);
	if(m_pWnd)
		m_pWnd->MoveWindow(r.left, r.top, r.right-r.left, r.bottom-r.top,1);
}


void CDynControl::AddIntProperty(CString csName,int Data)
{
	ControlData data;
	data.csName=csName;
	data.nData=Data;
	data.nType=INT_DATA;
	aDataList.Add(data);
}
void CDynControl::AddDoubleProperty(CString csName,double Data)
{
	ControlData data;
	data.csName=csName;
	data.fData=Data;
	data.nType=DOUBLE_DATA;
	aDataList.Add(data);
}

void CDynControl::AddBoolProperty(CString csName,bool Data)
{
	ControlData data;
	data.csName=csName;
	data.bData=Data;
	data.nType=BOOL_DATA;
	aDataList.Add(data);
}
void CDynControl::AddColorProperty(CString csName,int Data)
{
	ControlData data;
	data.csName=csName;
	data.Color=Data;
	data.nType=COLOR_DATA;
	aDataList.Add(data);
}
void CDynControl::AddStringProperty(CString csName,int nList,CString sData)
{
	ControlData data;
	data.csName=csName;
	data.sData=sData;
	data.nData=nList;
	data.nType=LIST_DATA;
	aDataList.Add(data);
}
void CDynControl::AddFuncProperty(CString csName,int Message,CString csFunction)
{
	ControlData data;
	data.csName=csName;
	data.sData=csFunction;
	data.nData=Message;
	data.nType=FUNCTION_DATA;
	aDataList.Add(data);
}
void CDynControl::SetFuncProperty(CString csName,CString csFunction)
{
	for(int i=0;i<aDataList.GetSize();i++)
	{
		if(aDataList[i].nType==FUNCTION_DATA)
		if(aDataList[i].csName==csName)
		{
			aDataList[i].sData=csFunction;
			break;
		}
	}
}

void CDynControl::SetBoolProperty(CString csName,bool Data)
{
	for(int i=0;i<aDataList.GetSize();i++)
	{
		if(aDataList[i].nType==BOOL_DATA)
		if(aDataList[i].csName==csName)
		{
			aDataList[i].bData=Data;
			break;
		}
	}
}

BOOL CDynControl::GetFirstFunctionParams(CString &csName,CString &csFunction)
{
	for(int i=0;i<aDataList.GetSize();i++)
	{
		if(aDataList[i].nType==FUNCTION_DATA)
		{
			csName=aDataList[i].csName;
			csFunction=aDataList[i].sData;
			return 1;
		}
	}
	return 0;
}


//todo
void CDynControl::FillDataArray()//���. ��������� ������� (���������� �� �������������)
{
	if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
	if(m_Property.GetSizeStrArray())
	{
		aDataList.RemoveAll();
		CValueArray *pArray=(CValueArray *)m_Property.pRef;
		for(int i=0;i<pArray->GetSizeStrArray();i++)
		{
			CString csName=pArray->GetIdentifierByNumber(i);
			CValue Val=m_Property.GetAt(csName);
			if(Val.GetType()==TYPE_ARRAY)
			{
				if(Val.GetSizeStrArray())
				{
					CValueArray  *pArray=(CValueArray *)Val.pRef;

					CString csType=pArray->GetAt("���").GetString();
					int nPrec=pArray->GetAt("��������").GetNumber();
					CValue Value=pArray->GetAt("��������");
					if(csType=="����")
					{
						//pValue->nType=TYPE_NUMBER;
						AddColorProperty(csName,Value.GetNumber());
					}
					else
					if(csType=="�����")
					{
						//pValue->nType=TYPE_NUMBER;
						if(nPrec==0)
							AddIntProperty(csName,Value.GetNumber());
						else
							AddDoubleProperty(csName,Value.GetNumber());
					}
					else
					if(csType=="�������")
					{
						AddFuncProperty(csName,0,Value.GetString());
					}
					else
					if(csType=="�����������")
					{
						AddStringProperty(csName,nTypeValue,Value.GetString());
					}
					else
					if(csType=="����������")
					{
						AddStringProperty(csName,nTypeVariable,Value.GetString());
					}
					else
					//if(csType=="������")
					{
						AddStringProperty(csName,nTypeAllStrings,Value.GetString());
					}
				}
			}
		}
	}

}

void CDynControl::PrepareDataArray()
{
	if(IsNew==0)
		return;
	IsNew=0;
	csPropertyName="��������";
	if(m_nControlType == ID_DYNBUTTONTREE)//TREE
	{
		csPropertyName="������";
		AddBoolProperty("���� ������",1);
		AddBoolProperty("���� �����",1);
		AddBoolProperty("���� �������� �����",1);
		AddBoolProperty("���� �����������",0);

		AddStringProperty("��� ��������",nTypeValue,"��������������");

		AddFuncProperty("�����",NM_DBLCLK,"");
		AddFuncProperty("���������",TVN_SELCHANGED,"");
		AddFuncProperty("����. �����",TVN_ITEMEXPANDED,"");
		
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if(m_nControlType == ID_DYNBUTTONGRID)//GRID
	{
		csPropertyName="Grid";
		AddIntProperty("����� �����",20);
		AddIntProperty("����� �������",10);
		AddIntProperty("����� ����.���",1);
		AddIntProperty("����� ����.���.",1);
		AddBoolProperty("��������� ��������������",1);

		AddBoolProperty("",1);
		AddBoolProperty("",1);
//		AddBoolProperty("�������� ����.�������",1);//5
//		AddBoolProperty("�������� ����.������",1);
		AddBoolProperty("���������� �������",1);
		AddIntProperty("������ ������",16);
		AddIntProperty("������ ������",130);
		AddBoolProperty("",1);
//		AddBoolProperty("��������� ������",1);//10
		AddBoolProperty("���������� �������",0);
		AddBoolProperty("����������� �����",0);
		AddColorProperty("���� ���� �������",RGB(255,255,255));

		AddStringProperty("��� ��������",nTypeValue,csPropertyName);

		AddFuncProperty("������ ��������������",GVN_BEGINLABELEDIT,"");
		AddFuncProperty("�����.���.������",NM_DBLCLK,"");
		AddFuncProperty("����� ������",NM_CLICK,"");
		AddFuncProperty("������ ������",NM_RCLICK,"");
		AddFuncProperty("��������� ��������������",GVN_ENDLABELEDIT,"");
		AddFuncProperty("������ ���������",GVN_SELCHANGING,"");
		AddFuncProperty("��������� ���������",GVN_SELCHANGED,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
		AddFuncProperty("�������� ����� ������������ ������",9999,"");
	}
	else
	if (m_nControlType == ID_DYNBUTTONEDIT) // EDIT
	{
		csPropertyName="�������� �������";

		AddStringProperty("��� ��������",nTypeValue,"������");
		AddBoolProperty("�������������� �����",1);
		AddIntProperty("�����",10);
		AddIntProperty("��������",0);
		AddBoolProperty("���������������",0);//4
		AddBoolProperty("�������������",0);
		AddBoolProperty("���� ������",0);
		AddBoolProperty("������ ������",0);//7
		AddBoolProperty("��������� ��������������",0);//8

		AddFuncProperty("�����",WM_ONSELECT,"");
		AddFuncProperty("��������������",EN_CHANGE,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if (m_nControlType == ID_DYNBUTTONRADIO) // RADIO
	{
		csPropertyName="�������������";
		AddBoolProperty("������ � ������",0);
		AddStringProperty("��� ��������",nTypeValue,"");

		AddFuncProperty("�����",BN_CLICKED,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if (m_nControlType == ID_DYNBUTTONLISTBOX) // LISTBOX
	{
		csPropertyName="������";
		AddBoolProperty("��������������� �����",0);
		AddStringProperty("��� ��������",nTypeValue,"��������������");

		AddFuncProperty("�����",LBN_DBLCLK,"");
		AddFuncProperty("����������������",LBN_SELCHANGE,"");
		//LBN_SELCANCEL
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if (m_nControlType == ID_DYNBUTTONLABEL) // LABEL
	{
		csPropertyName="�������";

		AddIntProperty("������ ������",8);
		AddBoolProperty("��������",0);
		AddBoolProperty("������",0);
		AddBoolProperty("�������������",0);
		AddColorProperty("����",0);


		AddStringProperty("��������� �����",1,"����");//5
		AddStringProperty("��������� ����",0,"�����");//6

		AddFuncProperty("�������",BN_CLICKED,"");//7
		AddFuncProperty("������� �������",NM_CUSTOMDRAW,"");

		//AddFuncProperty("�������",ANY_MESSAGE,"");
	}
	else
	if (m_nControlType == ID_DYNBUTTONCOMBO) // COMBO
	{
		csPropertyName="���� �� �������";
		AddStringProperty("��� ��������",nTypeValue,"��������������");

		//AddStringProperty("��� ��������",nTypeValue,"");
		AddFuncProperty("�����",CBN_SELENDOK,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if (m_nControlType == ID_DYNBUTTONGROUP) // GROUPBOX
		csPropertyName="����� ������";
	else
	if (m_nControlType == ID_DYNBUTTONCHECK) // CHECK
	{
		csPropertyName="������";
		AddStringProperty("��� ��������",nTypeValue,"");
		AddFuncProperty("�����",BN_CLICKED,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
	}
	else
	if (m_nControlType == ID_DYNBUTTON) // BUTTON
	{
		csPropertyName="������";
		AddBoolProperty("������ �� ���������",0);
		AddStringProperty("��� ��������",nTypeValue,"");
		AddFuncProperty("�������",BN_CLICKED,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
		AddColorProperty("����",0);//4
		AddIntProperty("��������",0);//5
		AddStringProperty("�������� �����",1,"����");//6
		AddStringProperty("�������� ����",0,"����");//7
	}
	else
	if (m_nControlType == ID_DYNBUTTONDATE) // DATE
		csPropertyName="����";
	else
	if (m_nControlType == ID_BUTTONPROGRESS) // CProgressCtrl
	{
		csPropertyName="���������";
		AddIntProperty("���. ��������",0);
		AddIntProperty("����. ��������",100);
		AddBoolProperty("������������ ������������",0);
		AddBoolProperty("����������� ����������",0);
		AddStringProperty("��� ��������",nTypeValue,"");
	}
	else
	if (m_nControlType == ID_BUTTONSLIDER) // CSliderCtrl
	{
		csPropertyName="�������";
		AddIntProperty("���. ��������",0);
		AddIntProperty("����. ��������",100);
		AddBoolProperty("������������ ������������",0);
		AddBoolProperty("������������� �����������",1);
		AddBoolProperty("�������",0);//4
		AddStringProperty("��� ��������",nTypeValue,"");
		AddFuncProperty("��������",NM_CUSTOMDRAW,"");
		AddFuncProperty("������� �������",WM_KEYDOWN,""); 
		AddFuncProperty("��� ���������",NM_RELEASEDCAPTURE,"");
		
	}
	else
	if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
	{
		csPropertyName=m_csObjName;//"����������";
	}
	else
	if (m_nControlType == ID_BUTTON_TAB)//CTabCtrl
	{
		AddFuncProperty("�����",TCN_SELCHANGE,"");//0
		AddBoolProperty("������",0);//1				TCS_BUTTONS
		AddBoolProperty("�������������",0);//2		TCS_MULTILINE
		AddBoolProperty("�����������",0);//3		TCS_VERTICAL
		AddBoolProperty("�����������",0);//4		TCS_BOTTOM + TCS_RIGHT
	}
}

void CDynControl::AddDialog(CWnd *pControl,CPoint* ppoint,int cx,int cy,CString name,int nStyle0,int nStyleE)
{
	int nStyle=WS_CHILD;
	if(nStyle0!=0)
		nStyle=nStyle0;

	if(m_bVisible||m_bConfigMode)
		nStyle = nStyle | WS_VISIBLE;
	if(!m_nTabOrder)
		nStyle = nStyle | WS_TABSTOP;
	

	CRect rectPos;
	rectPos.left = ppoint->x;
	rectPos.top = ppoint->y;
	rectPos.right = rectPos.left + cx; // width and height
	rectPos.bottom = rectPos.top + cy; // of the control

	m_pWnd = pControl;
	m_hWnd = pControl->GetSafeHwnd();
	pControl->CreateEx(nStyleE,name,m_csValue,nStyle,
		   rectPos.left,
		   rectPos.top,
		   rectPos.Width(),
		   rectPos.Height(),
		   m_pParentWnd->m_hWnd,
		   NULL);
	pControl->SetDlgCtrlID(m_nID);
	pControl->SetFont(&m_fontDefault);	
	pControl->EnableWindow(m_bEnable);
}

void CDynControl::AddControl(CPoint* ppoint)
{
	PrepareDataArray();

	if(m_nControlType == ID_DYNBUTTONTREE)//TREE
	{
		CTreeCtrl* pControl=new CTreeCtrl();
		int nStyle= WS_CHILD | TVS_SHOWSELALWAYS | WS_BORDER;
		if(aDataList[0].bData)
			nStyle = nStyle | TVS_HASBUTTONS;
		if(aDataList[1].bData)
			nStyle = nStyle | TVS_HASLINES;
		if(aDataList[2].bData)
			nStyle = nStyle | TVS_LINESATROOT;


		AddDialog(pControl,ppoint,200,100,"SysTreeView32",nStyle,WS_EX_CLIENTEDGE);


		if(aDataList[3].bData)
			pControl->SetImageList(GetImageList(),TVSIL_NORMAL);

		OnUpdate();

	}
	else
	if (m_nControlType == ID_DYNBUTTONGRID) // GRID - MFCGridCtrl
	{
		CGridCtrl* pControl=new CGridCtrl();
		AddDialog(pControl,ppoint,200,100,"MFCGridCtrl",0,WS_EX_CLIENTEDGE);

#ifdef MYDEBUG
		pControl->SetDoubleBuffering(0);
#else
		pControl->SetDoubleBuffering(1);
#endif
		pControl->SetImageList(GetImageList());

        //��������� ��������� �������
        pControl->EnableDragAndDrop(false);
        pControl->SetGridLineColor(0);

        //pControl->SetFixedColumnSelection(aDataList[5].bData);
        //pControl->SetFixedRowSelection(aDataList[6].bData);
        pControl->SetHeaderSort(aDataList[7].bData);
		if(aDataList[8].nData<=0)
			aDataList[8].nData=16;
		pControl->SetDefCellHeight(aDataList[8].nData);
		if(aDataList[9].nData<=0)
			aDataList[9].nData=130;
        pControl->SetDefCellWidth(aDataList[9].nData);
        pControl->AutoSizeColumns(aDataList[11].bData);

		pControl->SetGridBkColor(aDataList[13].Color);
        //pControl->m_NotUseMyExtEdit=TRUE;
        //pControl->SetCompareFunction(CGridCtrl::pfnCellNumericCompare);//��������� �����
		if(m_bConfigMode)
		{
			int nColumn=5;
			pControl->SetEditable(0);
	        pControl->SetHeaderSort(0);
			pControl->SetRowCount(20);
			pControl->SetColumnCount(nColumn);
			pControl->SetFixedRowCount(1);
			pControl->SetFixedColumnCount(0);
			pControl->SetSingleColSelection(1);
			pControl->SetFixedColumnSelection(1);
		}
		else
		{
			pControl->EnableSelection(0);
			pControl->m_bAutoExpandColumnsToFit=TRUE;
	        pControl->SetVirtualMode(aDataList[12].bData);
			pControl->SetEditable(aDataList[4].bData);
			pControl->SetRowCount(aDataList[0].nData);
			pControl->SetColumnCount(aDataList[1].nData);
			pControl->SetFixedRowCount(aDataList[2].nData);
			pControl->SetFixedColumnCount(aDataList[3].nData);
	        //pControl->SetSingleRowSelection(aDataList[10].bData);
			//pControl->SetListMode(aDataList[10].bData);
		}

		OnUpdate();

	}
	else
	if (m_nControlType == ID_DYNBUTTONEDIT) // EDIT
	{
		if(aDataList[0].sData=="����"&&(!m_bConfigMode))
		{
			m_nControlType = ID_DYNBUTTONDATE; // DATE
			if(afxAppRunMode!=CONFIG_MODE)
			if(aDataList[8].bData)
				m_bEnable=0;
		}
		else
		{

			int nStyle = WS_CHILD|ES_AUTOHSCROLL;
			if(aDataList[5].bData)
				nStyle = nStyle | ES_MULTILINE | WS_VSCROLL | ES_WANTRETURN | WS_HSCROLL ;

			if(!m_bConfigMode)
			if(aDataList[6].bData)
				nStyle = nStyle | ES_PASSWORD;
			
			if(aDataList[0].sData=="�����"&&(!m_bConfigMode))
			{
				nStyle = nStyle | ES_RIGHT;

				if(aDataList[7].bData)
				{
					//nStyle = nStyle | ES_NUMBER;

					CCalcEdit *pControl=new CCalcEdit(1);
					pControl->nUseFilter=1;
					AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"EDIT",nStyle,WS_EX_CLIENTEDGE);
					if(!aDataList[1].bData)
						pControl->SetLimitText(aDataList[2].nData);
					if(!m_bConfigMode)
						pControl->SetReadOnly(aDataList[8].bData);
				}
				else
				{
					CXrNumericEdit *pControl=new CXrNumericEdit();
					AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"EDIT",nStyle,WS_EX_CLIENTEDGE);

					pControl->SetCountDigitsDecimals(aDataList[2].nData-aDataList[3].nData);
					pControl->SetCountDigitsAfterDecimals(aDataList[3].nData);
					pControl->SetDisplayDigitsDecimalsFormat(xrDDDFDataTypeOnlyInputs);//xrDDDFDataTypeFull
					pControl->SetDisplayLeadingZero(xrDLZDataTypeFull);//xrDLZDataTypeParcial
					pControl->SetDecimalSymbol('.');
					pControl->SetSeparatorGrouping("'");
					pControl->SetNegativeValue(aDataList[4].bData);
					pControl->SetWindowText("0");
					if(!m_bConfigMode)
						pControl->SetReadOnly(aDataList[8].bData);
				}
			}
			else
			{
				CDropEdit*pControl=new CDropEdit(aDataList[7].bData);
				pControl->SetInvalidCharacters("");
				AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"EDIT",nStyle,WS_EX_CLIENTEDGE);
				if(!aDataList[1].bData)
					pControl->SetLimitText(aDataList[2].nData);
				if(!m_bConfigMode)
					pControl->SetReadOnly(aDataList[8].bData);
			}
		}
	}
	else
	if (m_nControlType == ID_DYNBUTTONLABEL) // LABEL
	{
		int nStyle = WS_CHILD|WS_DISABLED;// | WS_BORDER;

		if(m_bVisible||m_bConfigMode)
			nStyle = nStyle | WS_VISIBLE;


		CXColorStatic *pControl=new CXColorStatic();
		AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"STATIC",WS_CHILD|WS_DISABLED | WS_BORDER);
		CWnd *p=pControl->GetOwner();
		pControl->SetOwner(m_pParentWnd);
		pControl->SetParent(m_pParentWnd);

		if(aDataList[0].nData==0)
			aDataList[0].nData=8;

		
		int nHAllign=GetPosNumber(aDataList[5].sData,aDataList[5].nData);
		int nVAllign=GetPosNumber(aDataList[6].sData,aDataList[6].nData);
		SetStaticParams(*pControl,nHAllign,nVAllign,"MS Sans Serif",aDataList[1].bData,aDataList[2].bData,aDataList[3].bData,aDataList[0].nData,aDataList[4].Color);
		OnUpdate();

	
		if(!aDataList[7].sData.IsEmpty()||!aDataList[8].sData.IsEmpty())
			::SetWindowLong(pControl->m_hWnd, GWL_STYLE, ::GetWindowLong(pControl->m_hWnd, GWL_STYLE) | SS_NOTIFY);

	}
	else
	if (m_nControlType == ID_DYNBUTTONCOMBO) // COMBO
	{
		CComboBox* pControl=new CComboBox();
		AddDialog(pControl,ppoint,90,20*6,"COMBOBOX",WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,0);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_DYNBUTTONLISTBOX) // LISTBOX
	{
		int nStyle = WS_CHILD|LBS_STANDARD|WS_HSCROLL|LBS_NOINTEGRALHEIGHT;
		CIconListBox* pControl=new CIconListBox();
		if(aDataList[0].bData)
		{
			nStyle = nStyle | LBS_OWNERDRAWFIXED |LBS_NOTIFY |LBS_HASSTRINGS;
			pControl->SetImageList(GetImageList());
		}
		
		AddDialog(pControl,ppoint,90,20*5,"LISTBOX",nStyle,WS_EX_CLIENTEDGE );

		OnUpdate();
	}
	else
	if (m_nControlType == ID_DYNBUTTONGROUP) // GROUPBOX
	{
		AddDialog(new CButton(),ppoint,90,DEFAULT_DIALOG_H*5,"BUTTON",WS_CHILD | BS_GROUPBOX &~WS_TABSTOP,0);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_DYNBUTTONCHECK) // CHECK
	{
		AddDialog(new CButton(),ppoint,90,DEFAULT_DIALOG_H,"BUTTON",WS_CHILD | BS_AUTOCHECKBOX,0);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_DYNBUTTON) // BUTTON
	{
		int nStyle = WS_CHILD;
		if(aDataList[0].bData) 
			nStyle=nStyle | BS_DEFPUSHBUTTON;
		nStyle=nStyle | BS_OWNERDRAW;
		AddDialog(new CImageButton(),ppoint,80,22,"BUTTON",nStyle);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_DYNBUTTONRADIO) // RADIO
	{
		int nStyle = WS_CHILD | BS_AUTORADIOBUTTON;
		if (aDataList[0].bData)
			nStyle = nStyle | WS_GROUP;
//		if (m_csValue.IsEmpty())
//			m_csValue = "�������������";
		AddDialog(new CRadio(),ppoint,110,DEFAULT_DIALOG_H,"BUTTON",nStyle,0);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_BUTTONPROGRESS) // CProgressCtrl
	{
		int nStyle = WS_CHILD;
		if (aDataList[2].bData)
			nStyle = nStyle | PBS_VERTICAL;
		if (aDataList[3].bData)
			nStyle = nStyle | PBS_SMOOTH;

		CProgressCtrl* pControl=new CProgressCtrl();
		AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"msctls_progress32",nStyle,0);
		pControl->SetRange32(aDataList[0].nData,aDataList[1].nData);
	}
	else
	if (m_nControlType == ID_BUTTONSLIDER) // CSliderCtrl
	{
		int nStyle = WS_CHILD | TBS_AUTOTICKS;
		if (aDataList[2].bData)
			nStyle = nStyle | TBS_VERT;
		if (aDataList[3].bData)
			nStyle = nStyle | TBS_TOP;

		if (!aDataList[4].bData)
			nStyle = nStyle | TBS_NOTICKS;
		

		CSliderCtrl* pControl=new CSliderCtrl();
		AddDialog(pControl,ppoint,90,DEFAULT_DIALOG_H,"msctls_trackbar32",nStyle,0);
		pControl->SetRange(aDataList[0].nData,aDataList[1].nData);
	}
	else
	if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
	{
		CreateMicroForm();
		m_pWnd->MoveWindow(ppoint->x,ppoint->y, 200,100,1);
		OnUpdate();
	}
	else
	if (m_nControlType == ID_BUTTON_TAB) // Tabl
	{
		CMyTabCtrl* pControl=new CMyTabCtrl();

		int nStyle=WS_CHILD;
		if(aDataList[1].bData)//������
			nStyle |= TCS_BUTTONS;
		if(aDataList[2].bData)//�������������
			nStyle |= TCS_MULTILINE;
		if(aDataList[3].bData)//�����������
			nStyle |= TCS_VERTICAL;
		if(aDataList[4].bData)//�����������
			nStyle |= TCS_BOTTOM;

		AddDialog(pControl,ppoint,200,60,"SysTabControl32",nStyle );

		if(m_bConfigMode)
		{
			TCITEM tcItem;
			tcItem.mask = TCIF_TEXT;
			tcItem.pszText = "���� 1";
			pControl->InsertItem(0,&tcItem);
			tcItem.pszText = "���� 2";
			pControl->InsertItem(1,&tcItem);
			tcItem.pszText = "���� 3";
			pControl->InsertItem(2,&tcItem);
		}


		OnUpdate();
	}



	//��� else, �.�. �������� ����������� CEdit � ����
	if (m_nControlType == ID_DYNBUTTONDATE) // DATE
	{
		int nStyle = WS_CHILD | WS_BORDER ;//| DTS_SHOWNONE;		
		if(m_bVisible||m_bConfigMode)
			nStyle = nStyle | WS_VISIBLE;
		if(!m_nTabOrder)
			nStyle = nStyle | WS_TABSTOP;

		CRect rectPos;
		rectPos.left = ppoint->x;
		rectPos.top = ppoint->y;
		rectPos.right = rectPos.left + 110; // width and height
		rectPos.bottom = rectPos.top + 21; // of the control

		CDateTimeCtrl* pControl = new CDateTimeCtrl;
		
		m_pWnd = pControl;
		m_hWnd = pControl->GetSafeHwnd();
		pControl->CreateEx(0,DATETIMEPICK_CLASS,"DateTime",nStyle,
			   rectPos.left,
			   rectPos.top,
			   rectPos.Width(),
			   rectPos.Height(),
			   m_pParentWnd->m_hWnd,NULL);
		pControl->SetDlgCtrlID(m_nID);
		pControl->SetFont(&m_fontDefault);	
		pControl->EnableWindow(m_bEnable);
	}
}

void CDynControl::CreateMicroForm()
{
	CString csFileName;
	if(m_csObjName.Find(":")==1||m_csObjName.Left(2)=="\\\\")
		csFileName=m_csObjName;

	CValue FormParam;
	FormParam.CreateObject("���������");//String("����������");
	FormParam.SetAt("����������",m_Property);
	CMicroForm* pControl = new CMicroForm();
	pControl->m_bConfigMode=m_bConfigMode;
	pControl->m_csObjName=m_csObjName;
	
	//eventVal - �������� �������� �����
	OpenFormExt(m_csObjName,eventVal,FormParam,csFileName,"",0, 0,0,FormParam, pControl,m_pParentWnd);
	//OpenFormExt(m_csObjName,eventVal,CValue(),csFileName,"",0, 0,0,FormParam, pControl,m_pParentWnd);

	if(m_bVisible||m_bConfigMode)
		pControl->ShowWindow(SW_SHOW);

	m_pWnd = pControl;
	m_hWnd = pControl->GetSafeHwnd();

	pControl->SetDlgCtrlID(m_nID);
	pControl->SetFont(&m_fontDefault);	

	FillDataArray();//���. ��������� ������� (���������� �� �������������)
}
void CDynControl::OnUpdate()
{
	if(m_bConfigMode)
	if(m_pWnd)
	{
		CString csDialogName=m_csName;
		if(csDialogName.IsEmpty())
			csDialogName=csPropertyName;

		csDialogName=CString("<")+csDialogName+">";

		if(m_nControlType == ID_DYNBUTTONTREE)//TREE
		{
			CTreeCtrl* pControl=(CTreeCtrl*)m_pWnd;
			pControl->SetRedraw(0);
			pControl->DeleteAllItems();
			HTREEITEM CurParent=pControl->InsertItem(csDialogName,0);
			pControl->SetItemState(CurParent,TVIS_BOLD,TVIS_BOLD);

			HTREEITEM CurItem=pControl->InsertItem(csDialogName,CurParent);
			pControl->Expand(CurParent,TVE_EXPAND);
			pControl->InsertItem(csDialogName,CurItem);
			pControl->InsertItem(csDialogName,CurItem);
			pControl->Expand(CurItem,TVE_EXPAND);
			CurItem=pControl->InsertItem(csDialogName,CurParent);
			pControl->InsertItem(csDialogName,CurItem);
			pControl->InsertItem(csDialogName,CurItem);
			pControl->Expand(CurItem,TVE_EXPAND);
			pControl->SetRedraw(1);

		}
		if (m_nControlType == ID_DYNBUTTONGRID) // GRID - MFCGridCtrl
		{
			CGridCtrl* pControl=(CGridCtrl*)m_pWnd;
			LOGFONT *pFont;
			pFont=(LOGFONT *)pControl->GetItemFont(1,0);
			pFont->lfWeight = FW_BOLD;
			CGridCellBase *pCell=pControl->GetCell(1,0);
			pControl->SetItemFormat(1,0, DT_NOCLIP | DT_BOTTOM | DT_LEFT);
			pControl->SetItemText(1,0,csDialogName);
			pControl->SetItemFont(1,0,pFont);
			for(int i=1;i<pControl->GetColumnCount();i++)
			{
				pControl->SetItemText(1,i,"");
			}

		}
		else
		if (m_nControlType == ID_DYNBUTTONEDIT) // EDIT
		{
			m_pWnd->SetWindowText(csDialogName);
		}
		else
		if (m_nControlType == ID_DYNBUTTONLISTBOX) // LISTBOX
		{
			CListBox* pControl=(CListBox*)m_pWnd;
			pControl->ResetContent();
			for(int i=0;i<5;i++)
				pControl->AddString(csDialogName);
		}
		else
		if (m_nControlType == ID_DYNBUTTONCOMBO) // COMBOBOX
		{
			CComboBox* pControl=(CComboBox*)m_pWnd;
			pControl->ResetContent();
			pControl->AddString(csDialogName);
			pControl->SetCurSel(0);
		}
		else
		if (m_nControlType == ID_DYNBUTTONLABEL) // LABEL
		{
			CXColorStatic* pControl=(CXColorStatic*)m_pWnd;
			int nHAllign=GetPosNumber(aDataList[5].sData,aDataList[5].nData);
			int nVAllign=GetPosNumber(aDataList[6].sData,aDataList[6].nData);
			SetStaticParams(*pControl,nHAllign,nVAllign,"MS Sans Serif",aDataList[1].bData,aDataList[2].bData,aDataList[3].bData,aDataList[0].nData,aDataList[4].Color);
			if(!m_csValue.IsEmpty())
				csDialogName=m_csValue;
			pControl->SetWindowText(csDialogName);
			pControl->SetPlainBorder(TRUE);
			pControl->SetPlainBorder(FALSE);
		}
		else
		if (m_nControlType == ID_DYNBUTTON &&aDataList[5].nData) // BUTTON + ��������
		{
			m_pWnd->SetWindowText(m_csValue);
			
		}
		else
		{

			if(m_csValue.IsEmpty())
				m_pWnd->SetWindowText(csDialogName);
			else
				m_pWnd->SetWindowText(m_csValue);
		}


		/* ������...
		if (m_nControlType == ID_BUTTON_TAB) // Tabl
		{
			CMyTabCtrl* pControl=(CMyTabCtrl*)m_pWnd;
			int nStyle= WS_CHILD | WS_VISIBLE;
			
			if(aDataList[1].bData)//������
				nStyle |= TCS_BUTTONS;
			if(aDataList[2].bData)//�������������
				nStyle |= TCS_MULTILINE;
			if(aDataList[3].bData)//�����������
				nStyle |= TCS_VERTICAL;
			if(aDataList[4].bData)//�����������
				nStyle |= TCS_BOTTOM;

			::SetWindowLong(pControl->m_hWnd, GWL_STYLE, nStyle); 

			pControl->SetMinTabWidth(-1);
		}
		*/
		if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
		{
			CMicroForm* pControl=(CMicroForm*)m_pWnd;
			if(pControl)
			if(pControl->m_csObjName!=m_csObjName)
			{
				CRect Rect;
				m_pWnd->GetWindowRect(Rect);
				m_pParentWnd->ScreenToClient(Rect);

				pControl->ShowWindow(0);
				m_pWnd=0;
				CreateMicroForm();
				m_pWnd->MoveWindow(Rect.left,Rect.top,Rect.Width(),Rect.Height(),1);
			}
		}
	}


	//���. ���������
	if (m_nControlType == ID_DYNBUTTON) // BUTTON
	{
		CImageButton* pControl=(CImageButton*)m_pWnd;
		pControl->SetTooltipText(m_csDescription,TRUE);
		
		pControl->SetTextColor(aDataList[4].Color);
		int nPicture=aDataList[5].nData;//��������
		if(nPicture)
		{
			int nHAllign=GetPosNumber(aDataList[6].sData,aDataList[6].nData);
			int nVAllign=GetPosNumber(aDataList[7].sData,aDataList[7].nData);
			int MasHoriz[3]={0x0000,0x0010,0x0002};
			int MasVert[3]={0x0000,0x0004,0x0008};

			pControl->SetAlignStyle(CImageButton::TEXT_INCLUDE | MasHoriz[nHAllign] | MasVert[nVAllign]);
		}
		else
		{
			pControl->SetAlignStyle(CImageButton::TEXT_INCLUDE);
		}
		pControl->SetImage(nPicture);
	}
	else
	if (m_nControlType == ID_BUTTONFORMOBJECT) //���������������� ����� - ����������
	{
		m_pWnd->EnableWindow(m_bEnable);
	}
}

