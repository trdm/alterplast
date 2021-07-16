// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueList.cpp: implementation of the CValueList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Dialog.h"
#include "ValueList.h"
#include "ChooseValue.h"
#include "ValueComboBox.h"
#include "ValueListBox.h"
#include "../Menu/MenuListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRect	afxCurrentRect(0,0,0,0);//текущий координаты диалогового окна, который инициирует выборку из списка значений (CButton,CEdit)
CWnd	*afxCurrentWnd=0;

CMethods CValueList::Methods;
int nListDirectSort=1;//направление сортировки для функции сравнения
int CompareListByPresent( const void *arg1, const void *arg2 )//функция сравнения элементов списка
{
 	CString Str1=(*(CElementList*)arg1).Present;
	CString Str2=(*(CElementList*)arg2).Present;
	return nListDirectSort*_stricmp(Str1,Str2);
}
int CompareListByValue( const void *arg1, const void *arg2 )
{
	CValue Val1=(*(CElementList*)arg1).Value;
	CValue Val2=(*(CElementList*)arg2).Value;
	if(Val1.nType>Val2.nType)
		return 1;
	else 
	{
		if(Val1.nType==Val2.nType)
		{
			BOOL Res=(Val1>Val2);
			if(Res)
				return nListDirectSort;
			Res=(Val1 == Val2);
			if(Res)
				return 0;
			else
				return -nListDirectSort;
		}
		else
			return -1;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueList,CValueControl);
CValueList::CValueList()
{
	bValidFind=FALSE;
	nType=100;
}

CValueList::~CValueList()
{

}

//******************************
//Работа как агрегатного объекта
//******************************
void CValueList::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"AddValue","ДобавитьЗначение"},
		{"InsertValue","ВставитьЗначение"},
		{"GetListSize","РазмерСписка"},
		{"SetValue","УстановитьЗначение"},
		{"Set","Установить"},
		{"Sort","Сортировать"},
		{"SortByPresent","СортироватьПоПредставлению"},
		{"Belong","Принадлежит"},//50}%
		{"FindValue","НайтиЗначение"},
		{"GetValue","ПолучитьЗначение"},
		{"Get","Получить"},
		{"FromSeparatedString","ИзСтрокиСРазделителями"},
		{"ToSeparatedString","ВСтрокуСРазделителями"},
		{"ChooseValue","ВыбратьЗначение"},//99}%
		{"CheckValue","ОтметитьЗначения"},
		{"Check","Пометка"},
		{"CurSel","ТекущаяСтрока"},
		{"RemoveValue","УдалитьЗначение"},
		{"RemoveAll","УдалитьВсе"},
		{"MoveValue","СдвинутьЗначение"},
		{"Unload","Выгрузить"},
		{"SetSize","УстановитьРазмер"},
		{"SetRedraw","УстановитьПрорисовку"},
		
		{"Visible","Видимость"},
		{"Enable","Доступность"},
		{"Activate","Активизировать"},
		
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

#define Param1	(*aParams[0])
#define Param2	(*aParams[1])
#define Param3	(*aParams[2])
#define Param4	(*aParams[3])
#define Param5	(*aParams[4])

//если это пременная диалога, то возвращаем только значение
CValue UnrefValues(CValue &Val)
{
	if(Val.nType==TYPE_REFFER)
	{
		int nType=Val.GetType();
		if(nType==TYPE_NUMBER)
			return CValue(Val.GetNumber());
		if(nType==TYPE_STRING)
			return CValue(Val.GetString());
		if(nType==TYPE_DATE)
			return CValue(Val.GetDate());
		//иначе - ссылка...
	}
	else
	if(Val.bReadOnly)
	{
		CValue Val2;
		Val2.SetValue(Val);
		return Val2;
	}
	return Val;
}

CValue CValueList::Method(int iName,CValue **aParams)
{
	switch(iName)
	{
		case enAddValue: 
			{
				if(Param2.nType==TYPE_EMPTY)
		 			AddValue(UnrefValues(Param1));
				else
			 		AddValue(UnrefValues(Param1),Param2);
				break;
			}
		case enInsertValue:
			{
		 		InsertValue(Param1,UnrefValues(Param2),Param3,Param4);
				break;
			}
		
		case enGetListSize:
			{
		 		return GetListSize();
			}
		case enSetValue:
			{
		 		SetValue(Param1,UnrefValues(Param2),Param3,Param4);
				break;
			}
		case enSet:
			{
		 		return Set(Param1,UnrefValues(Param2));
			}
		case enSort:
			{
				if(Param1.nType==TYPE_EMPTY)
		 			Sort();
				else
		 			Sort(Param1);
				break;
			}
		case enSortByPresent:
			{
				if(Param1.nType==TYPE_EMPTY)
			 		SortByPresent();
				else
			 		SortByPresent(Param1);
				break;
			}
		case enBelong:
			{
		 		return Belong(Param1);
			}
		case enFindValue:
			{
				CValue Ret;
				Ret=FindValue(Param1);
		 		return Ret;
			}
		case enGetValue:
			{
				CString Str;
				CValue Res=GetItemValue(Param1,Str);
				Param2=String(Str);
				return Res;
			}
		case enGet:
			{
				return Get(Param1);
			}
		case enToSeparatedString:
			{
		 		return String(ToSeparatedString());
			}
		case enFromSeparatedString:
			{
		 		FromSeparatedString(Param1);
				break;
			}
		case enChooseValue:
			{
				CValue Val=Param1;
				int nIndex=0;
		 		int nRes=ChooseValue(Val,Param2,nIndex,Param5);
				Param1=Val;
				Param3=nIndex;
				return nRes;
			}
		case enCheckValue:
			{
				CValue Val=Param1;
				int nIndex=0;
		 		int nRes=CheckValue(Val,Param2,nIndex);
				Param1=Val;
				Param3=nIndex;
				return nRes;
			}
		case enCheck:
			{
				if(Param2.nType==TYPE_EMPTY)
					return Check(Param1);
				else
					return Check(Param1,Param2);
			}
		case enRemoveValue:
			{
				RemoveValue(Param1,Param2);
				break;
			}
		case enRemoveAll:
			{
				RemoveAll();
				ASSERT(GetListSize()==0);
				break;
			}
		case enMoveValue:
			{
				MoveValue(Param1,Param2);
				break;
			}
		case enUnload:
			{
				//CValue List=Param1;
				
				if(Param3.nType==TYPE_EMPTY)
				{
					if(Param2.nType==TYPE_EMPTY)
						Unload(Param1);
					else
						Unload(Param1,Param2.GetNumber());
				}
				else
					Unload(Param1,Param2.GetNumber(),Param3.GetNumber());
				//Param1=List;
				break;
			}

		case enSetSize:
			{
				SetSize(Param1);
				break;
			}

		case enCurSel:
			{
				if(Param2.nType)
					return UseLayer(Param1.GetString(),Param2.GetNumber());
				else
				if(Param1.nType==TYPE_EMPTY)
					return CurSel();
				else
					return CurSel(Param1);
			}


		case enVisible:
			{
				if(!pWnd)
					return 0;
				CValue Ret=Visible();
				if(aParams[0]->nType!=TYPE_EMPTY)
					Visible(aParams[0]->GetNumber());
				return Ret;
			}
		case enEnable:
			{
				if(!pWnd)
					return 0;
				CValue Ret=Enable();
				if(aParams[0]->nType!=TYPE_EMPTY)
					Enable(aParams[0]->GetNumber());
				return Ret;
			}
		case enFocus:
			{
				if(!pWnd)
					return 0;
				((CWnd*)pWnd)->SetFocus();
			}


	}
	return CValue();
}




//******************************
//Обычные методы
//******************************
void CValueList::AddValue(CValue Val,CString Str)
{
	//if(Str=="")
	//	Str="";

	CElementList data;
	data.Value=Val;
	data.Present=Str;

	aValue.Add(data);
	if(bValidFind)
	{
		int i=aValue.GetSize()-1;
		PrepareFind(i, data);
	}
}

int CValueList::GetListSize()const
{
	return aValue.GetSize();
}

void CValueList::InsertValue(int nIndex, CValue Val, CString Str, int nCount)
{
	nIndex--;
	if(nCount<=0)
		nCount=1;
//	if(nIndex<1||nIndex>GetListSize())
//		SetError("Индекс не входит в границы списка значений!");
	if(nIndex>=GetListSize())
		nIndex=GetListSize()-1;
	if(nIndex<0)
		nIndex=0;

	CElementList data;
	data.Value=Val.GetValue();//GetValue() - дабы добавление элементов было по значению, а не по ссылке (актуально для диалоговых элементов)
	data.Present=Str;

	aValue.InsertAt(nIndex,data,nCount);
	bValidFind=FALSE;
}

void CValueList::SetValue(int nIndex, CValue Val, CString Str, int nCount)//установка значения по индексу
{
	nIndex--;
	if(nCount<=0)
		nCount=1;
	if(nIndex<0||nIndex>=GetListSize())
		SetError("Индекс не входит в границы списка значений!");

	CElementList data;
	data.Value=Val.GetValue();
	data.Present=Str;
	for(int i=nIndex;i<nIndex+nCount;i++)
	{
		if(i>aValue.GetSize())
			break;
		aValue.SetAt(i,data);
	}
	bValidFind=FALSE;
}

int CValueList::Set(CString Str,CValue Val)//поиск + установка значения
{
	if(!bValidFind)
		PrepareFind();
	int n=(int)treePresents[Str];
	if(n)
	{
		int nIndex=n-1;
		CElementList data;
		data.Value=Val.GetValue();
		data.Present=Str;
		aValue.SetAt(nIndex,data);
		PrepareFind(nIndex, data);
		return nIndex+1;
	}
	else
	{
		AddValue(Val,Str);
		return GetListSize();
	}
}

void CValueList::PrepareFind(int nIndex, CElementList data)
{
	CString Str;
	Str.Format("%d-%s",data.Value.GetType(),data.Value.GetString());

	treeValues[Str]=(void*)(nIndex+1);
	treePresents[data.Present]=(void*)(nIndex+1);
}
void CValueList::PrepareFind(void)
{
	//заново формируем деревья быстрого поиска
	treePresents.RemoveAll();
	treeValues.RemoveAll();
	for(int i=0;i<aValue.GetSize();i++)
		PrepareFind(i, aValue[i]);
	bValidFind=TRUE;
}

void CValueList::RemoveAll(void)
{
	aValue.RemoveAll();
	treePresents.RemoveAll();
	treeValues.RemoveAll();
	bValidFind=TRUE;
}
void CValueList::Sort(int nDirect)
{
	if(nDirect<=0)
		nListDirectSort=1;
	else
		nListDirectSort=-1;
	if(aValue.GetSize()>0)
		qsort(&aValue[0], aValue.GetSize(), sizeof(aValue[0]), CompareListByValue);
	bValidFind=FALSE;
}
void CValueList::SortByPresent(int nDirect)
{
	if(nDirect<=0)
		nListDirectSort=1;
	else
		nListDirectSort=-1;
	if(aValue.GetSize()>0)
		qsort(&aValue[0], aValue.GetSize(), sizeof(aValue[0]), CompareListByPresent);
	bValidFind=FALSE;
}

int CValueList::Belong(CValue Val)
{
	CString Str;
	Str.Format("%d-%s",Val.GetType(),Val.GetString());

	if(!bValidFind)
		PrepareFind();
	if(treeValues[Str])
		return 1;
	return 0;

//todo: доделать проверку вложенного вхождения в другие агр. объекты
/*
	for(int i=0;i<aValue.GetSize();i++)
		if(aValue[i].Value==Val)
			return 1;
*/

}

int CValueList::FindValue(CValue Val)
{
	CString Str;
	Str.Format("%d-%s",Val.GetType(),Val.GetString());

	if(!bValidFind)
		PrepareFind();
	int n=(int)treeValues[Str];
	return n;
}
CValue CValueList::GetItemValue(int nIndex,CString &Str)
{
	nIndex--;
	if(nIndex<0||nIndex>=GetListSize())
		SetError("Индекс не входит в границы списка значений!");
	if(aValue[nIndex].Present=="")
		Str="";
	else
		Str=aValue[nIndex].Present;
	return aValue[nIndex].Value;
}
CValue CValueList::GetItemValue(int nIndex)
{
	CString Str;
	return GetItemValue(nIndex,Str);
}

CValue CValueList::Get(CString Str)//поиск + выборка значения
{
	if(!bValidFind)
		PrepareFind();
	if(int n=(int)treePresents[Str])
	{
		int nIndex=n-1;
		return aValue[nIndex].Value;
	}
	return CValue();
}

CString CValueList::ToSeparatedString()
{
	CString Str;
	for(int i=0;i<aValue.GetSize();i++)
	{
		if(i>0)
			Str+=",";
		if(aValue[i].Value.GetType()==TYPE_NUMBER)
		{
			Str+=aValue[i].Value;
		}
		else
		{
			Str+="\"";
			CString temp=aValue[i].Value;
			temp.Replace("\"","\"\"");
			Str+=temp;
			Str+="\"";
		}

	}
	return Str;
}

void CValueList::FromSeparatedString(CString Str)
{
	RemoveAll();
	for(int i=0;i<Str.GetLength();i++)
	{
		int nStart=i;
		CElementList data;
		
		if(Str[nStart]!='\"')
		{//число
			int n=Str.Find(",",i+1);
			if(n<0)
			{
				data.Value=atof(Str.Mid(i));
				i=Str.GetLength();//конец цикла
			}
			else
			{
				data.Value=atof(Str.Mid(i,n-i));
				i=n;
			}
		}
		else
		{//строка
			CString csAdd;	
			while(true)
			{
				int n=Str.Find("\"",i+1);//ищем окончание строки
				if(n>=0&&n+1<Str.GetLength())
				{
					if(Str[n+1]=='\"')//но двойные кавычки
						i=n+1;//пропускаем
					else
					{
						csAdd=Str.Mid(nStart+1,n-nStart-1);
						i=n+1;
						break;
					}
				}
				else
				{
					csAdd=Str.Mid(nStart+1,Str.GetLength()-nStart-2);
					i=Str.GetLength();//конец цикла
					break;
				}
			}

			csAdd.Replace("\"\"","\"");
			data.Value=String(csAdd);
		}
		AddValue(data.Value);
	}

	bValidFind=FALSE;
}

int CValueList::ChooseValue(CValue &Val,CString Title,int &nIndex,int nMode)
{
	if(1==nMode)
	{
		HMENU hmenu=CreatePopupMenu(); 

		for(int i=0;i<aValue.GetSize();i++)
		{
			if(aValue[i].Present.IsEmpty())
				AppendMenu (hmenu, MF_STRING|MF_ENABLED+MF_CHECKED*aValue[i].bCheck,i+MENU_START_ID,CString(String(aValue[i].Value)));
			else
				if(aValue[i].Present=="-")
					AppendMenu (hmenu, MF_SEPARATOR,0,0);
				else
					AppendMenu (hmenu, MF_STRING|MF_ENABLED+MF_CHECKED*aValue[i].bCheck,i+MENU_START_ID,aValue[i].Present);//MF_CHECKED
		}
		
		HWND hParent=AfxGetMainWnd()->m_hWnd;
		//определяем текущие координаты мышки
		POINT P;
		GetCursorPos(&P);

		short int num=TrackPopupMenu(hmenu, 
			TPM_LEFTALIGN | TPM_RIGHTBUTTON|TPM_RETURNCMD,
			P.x,P.y, 0, hParent, NULL); 
		DestroyMenu(hmenu); 

		if(num>0)
		{
			UINT nDelta=MENU_START_ID-1;
			nIndex=(UINT)num-nDelta;
			if(nIndex>=1&&nIndex<=aValue.GetSize())
				Val=aValue[nIndex-1].Value;
			return 1;
		}
		else
			return 0;
	}
	if(2==nMode)
	{
		CWnd *pItem;
		CWnd *pParent=AfxGetMainWnd();

		if(afxCurrentWnd)
		{
			pItem=afxCurrentWnd;
			pItem->GetClientRect(afxCurrentRect);
			pItem->ClientToScreen(afxCurrentRect);
		}
		else
		{
			pItem=pParent;
		}


		ASSERT(pParent);
		if(!pParent)
			return 0;

		//определяем текущие координаты курсора
		if(afxCurrentRect.Width()==0)
		{
			//GetCursorPos(&P);
		}
		else
		{
		}


		CRect Rect;
		Rect.left=afxCurrentRect.left;
		Rect.right=afxCurrentRect.right;
		Rect.top=afxCurrentRect.bottom;
		Rect.bottom=Rect.top+150;
		pParent->ScreenToClient(Rect);

		BOOL bWasSel=0;
		CMenuListBox m_listbox;
		m_listbox.Create(LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_TABSTOP | WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER , Rect, pParent,100);
		m_listbox.SetFont(pItem->GetFont());
		for(int i=0;i<aValue.GetSize();i++)
		{
			if(aValue[i].Present.IsEmpty())
				m_listbox.AddString(CString(String(aValue[i].Value)));
			else
				if(aValue[i].Present=="-")
					m_listbox.AddString("");
				else
					m_listbox.AddString(aValue[i].Present);
			if(Val.GetString()==aValue[i].Value.GetString())
			{
				m_listbox.SetCurSel(i);
				bWasSel=1;
			}
		}

		if(aValue.GetSize()>0)
		{
			int nH=(aValue.GetSize()+1)*m_listbox.GetItemHeight(0);
			if(nH<150)
			{
				Rect.bottom=Rect.top+nH;
				m_listbox.MoveWindow(&Rect);
			}

				
			if(!bWasSel)
					m_listbox.SetCurSel(0);
		}





		afxCurrentWnd=0;
		int num=m_listbox.ChooseValue()+1;

		if(num>0)
		{
			nIndex=num;
			if(nIndex>=1&&nIndex<=aValue.GetSize())
				Val=aValue[nIndex-1].Value;
			return 1;
		}
		else
			return 0;

	}


	CChooseValue dlg;
	dlg.List.RemoveAll();
	for(int i=0;i<aValue.GetSize();i++)
	{
		if(aValue[i].Present.IsEmpty())
			dlg.List.Add(CString(String(aValue[i].Value)));
		else
			dlg.List.Add(aValue[i].Present);
		if(nMode<0)
		{
			BOOL bCheck=aValue[i].bCheck;
			dlg.ListCheck.Add(bCheck);
		}
	}

	dlg.nStart=FindValue(Val);
	dlg.csTitle=Title;
	dlg.nMode=nMode;//0-выбор значения, -1 - пометка значений

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		if(nMode<0)
			for(int i=0;i<aValue.GetSize();i++)
			{
				BOOL bCheck=dlg.ListCheck[i];
				Check(i+1,bCheck);
			}


		nIndex=dlg.nRes+1;
		if(nIndex>=1&&nIndex<=aValue.GetSize())
			Val=aValue[nIndex-1].Value;
		return 1;

	}
	return 0;
}
int CValueList::ChooseValue(CValue &Val,CString Title)
{
	int nIndex;
	return ChooseValue(Val,Title,nIndex);
}

int CValueList::CheckValue(CValue &Val,CString Title,int &nIndex)
{
	return ChooseValue(Val,Title,nIndex,-1);
}

int CValueList::Check(int nIndex,int bValue)
{
	if(nIndex<1||nIndex>GetListSize())
		SetError("Индекс не входит в границы списка значений!");
	int nRes=aValue[nIndex-1].bCheck;
	if(bValue!=-1)
		aValue[nIndex-1].bCheck=bValue;
	return nRes;
}


void CValueList::RemoveValue(int nIndex, int nCount)
{
	if(nCount<=0)
		nCount=1;
	if(nIndex<1||nIndex>GetListSize())
		SetError("Индекс не входит в границы списка значений!");
	aValue.RemoveAt(nIndex-1,nCount);
	bValidFind=FALSE;
}

void CValueList::MoveValue(int nCount,int nIndex)
{
	if(nCount==0)
		return;
	nIndex--;
	int ndelta=0;
	if(nCount<0)
		ndelta=1;
	int nNewIndex=nIndex+nCount+1-ndelta;
	if(nIndex+ndelta==nNewIndex+1)
		return;

	if(nIndex<0||nIndex>=GetListSize())
		SetError("Индекс не входит в границы списка значений!");
	if(nNewIndex<0)
		nNewIndex=0;
	if(nNewIndex>=GetListSize())
		nNewIndex=GetListSize();

	CElementList data=aValue[nIndex];
	aValue.InsertAt(nNewIndex,data,1);
	aValue.RemoveAt(nIndex+ndelta,1);
	bValidFind=FALSE;
}
void CValueList::Unload(CValue &List,int nStart,int nEnd)
{
	if(nStart==-1)
		nStart=1;
	if(nEnd==-1)
		nEnd=GetListSize();

	if(List.nType!=TYPE_REFFER||List.pRef==0)
	{
		List.CreateObject("СписокЗначений");
		//SetError("Значение не представляет объект CValue!");
	}

	CValue *pList=&List;
/*
	if(!List.pRef->IsKindOf(RUNTIME_CLASS(CValueList)))
	{
		if(!List.pRef->pRef)
			List.pRef->CreateObject("СписокЗначений");
		else
		if(!List.pRef->pRef->IsKindOf(RUNTIME_CLASS(CValueList)))
			List.pRef->CreateObject("СписокЗначений");
	}
	pList=List.pRef;

*/
	if(GetListSize()!=0)
	{
		if(nStart<1||nStart>GetListSize())
			SetError("Индекс 1 выходит за границы списка значений!");
		if(nEnd<1||(nEnd+nStart-1)>GetListSize())
			SetError("Индекс 2 выходит за границы списка значений!");
	}



	int nIndex=0;//текущее выделенное значение от 0 до размера списка -1

	ASSERT(pList);
/*
	if(pList->IsKindOf( RUNTIME_CLASS( CValueComboBox ) ) ||
		pList->IsKindOf( RUNTIME_CLASS( CValueListBox ) ))
		{
			nIndex=pList->Method("CurSel",0)-1;
		}
*/
	pList->Method("RemoveAll",0);

	CValue vSize=pList->Method("GetListSize",0)+nEnd;
	CValue **RefSize=new CValue*[3];
	RefSize[0]=&vSize;
	pList->Method("SetSize",RefSize);

	for(int i=nStart;i<nEnd+nStart;i++)
	{
		CValue Val1=aValue[i-1].Value;
		CValue Val2=String(aValue[i-1].Present);
		RefSize[0]=&Val1;
		RefSize[1]=&Val2;
		pList->Method("AddValue",RefSize);
	}

	if(pList)
	if(pList->IsKindOf( RUNTIME_CLASS( CValueComboBox ) ) ||
		pList->IsKindOf( RUNTIME_CLASS( CValueListBox ) ))
	{
		if(nIndex<0)
			nIndex=0;
		int nCount=pList->Method("GetListSize",0);
		if(nCount)
		{
			nIndex=min(nIndex,nCount-1);

			CValue Val1=nIndex;
			RefSize[0]=&Val1;
			//pList->Method("AddValue",RefSize);
			pList->Method("CurSel",RefSize);

		}
	}

	delete []RefSize;

}

void CValueList::SetSize(int nSize)
{
	aValue.SetSize(aValue.GetSize(),nSize);
}
int CValueList::UseLayer(CString csName,int nMode)
{
	return 0;
}

int CValueList::CurSel(int nCur)
{
	return 0;
}


void CValueList::SaveToString(CString &Str)
{
	CString csData;

	for(int i=0;i<aValue.GetSize();i++)
	{
		CString csElement;
		CString csValue;
		CString csPresent=aValue[i].Present;
		aValue[i].Value.SaveToString(csValue);
		ReplaceBySave(csPresent);

		csElement.Format("{%d,%s,%s}",aValue[i].bCheck,csValue,csPresent);
		csData+=csElement;
	}
	Str.Format("{СписокЗначений/%d/%s}",aValue.GetSize(),csData);

}
int GetCloseStr(CString &Str,int nFirst);

void CValueList::LoadFromString(CString &Str)
{
	RemoveAll();
	int nCurIndex;
	int nIndex1=Str.Find("/");
	if(nIndex1>0)
	{
		int nIndex2=Str.Find("/",nIndex1+1);
		nCurIndex=nIndex2+1;
		int nCount=atoi(Str.Mid(nIndex1+1,nIndex2-nIndex1-1));
		if(nCount>0)
		{
			CValue v;
			CString csPresent;
			nCurIndex=nIndex2+1;//позиция текущего загружаемого элемента списка
			for(int i=0;i<nCount;i++)
			{
				int nIndex3=Str.Find(",",nCurIndex+1);
				int bCheck=atoi(Str.Mid(nCurIndex+1,nIndex3-nCurIndex-2));
				nCurIndex=nIndex3+1;

				//ищем парную закрывающуюся скобку
				int nLast=GetCloseStr(Str,nCurIndex);
				v.LoadFromString(Str.Mid(nCurIndex,nLast-nCurIndex+1));
				nCurIndex=nLast+2;

				int nIndex4=Str.Find("}",nCurIndex);
				csPresent=Str.Mid(nCurIndex,nIndex4-nCurIndex);
				ReplaceByLoad(csPresent);

				AddValue(v,csPresent);


				nCurIndex=nIndex4+1;
			}
		}
	}
}
