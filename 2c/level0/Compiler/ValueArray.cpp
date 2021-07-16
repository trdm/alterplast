// ValueArray.cpp: implementation of the CValueArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueArray,CValue);
CMethods CValueArray::Methods;

#define MIN_SIZE 4

CValueArray::CValueArray()
{
	aValueArray.SetSize(0,MIN_SIZE);
	pStringValueList=0;
	pStringKey=0;
//	if(int(this)==0x019CDAA0)
//		int tt=1;
}
CValueArray::~CValueArray()
{
	RemoveAll();
}
//работа с массивом как с агрегатным объектом
//перечисление строковых ключей
enum
{
	enFindId=0,
	enIdentifierByNumber,
	enSort,
	enSortId,
	enRemoveAll,
	enLoad,
	enFind,
	enCountArray,
	enCountStruct,
	enRemove,
	enValueByNumber,
	enFindNumberID,
};
void CValueArray::PrepareNames(void)
{
	SEngRus aMethods[]=
	{
		{"FindId","НайтиИдентификатор","НайтиИдентификатор(СтрИдентификатор) - возвращает 1, если найден и 0, если не найден"},
		{"IdentifierByNumber","ИдентификаторПоНомеру","ИдентификаторПоНомеру(НомПоПорядку) - возвращает строковый идентификатор"},
		{"Sort","Сортировать","Сортировать(ИдентификаторИлиНомерПодчМассива=\"\",НаправлениеСортировки)"},
		{"SortId","СортироватьИД","СортироватьИД(ИдентификаторИлиНомерПодчМассива=\"\",НаправлениеСортировки)"},
		{"RemoveAll","УдалитьВсе","УдалитьВсе()"},
		{"Load","Загрузить","Загрузить(МассивИсточник,КолВложенныхМассивовЗагрузки=0,ФлОчистки=1)"},
		{"FindValue","НайтиЗначение","НайтиЗначение(Значение,ИдентфифкаторИлиИндексПодчМассива=\"\")"},
		{"SizeArray","РазмерМассива","РазмерМассива()"},
		{"SizeStruct","РазмерСтруктуры","РазмерСтруктуры()"},
		{"Remove","Удалить","Удалить(СтрИдентификатор) - удаление элемента структуры"},
		{"ValueByNumber","ЗначениеПоНомеру","ЗначениеПоНомеру(НомерПоПорядку) - получить значение структуры по номеру записи"},
		{"FindNumberID","НайтиНомерИдентификатора","НайтиНомерИдентификатора(СтрИдентификатор)"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM,aMethods,nCountM);
}

CString csLastKey;
void CValueArray::SetAttribute(int iName,CValue &Val)
{
	SetAt(csLastKey,Val);
}
CValue CValueArray::GetAttribute(int iName)
{
	return GetAt(csLastKey);
}
int CValueArray::FindAttribute(CString csName)
{
	csLastKey=csName;
	return 1;
}

CValue CValueArray::Method(int iName,CValue **p)
{
	switch(iName)
	{
		case enFindId:
			{
				return FindId(p[0]->GetString());
			}
		case enCountArray:
			{
				int nSize=GetSizeArray();
				if(p[0]->nType!=TYPE_EMPTY)
					SetSizeArray(p[0]->GetNumber());
				return nSize;
			}
		case enCountStruct:
			{
				int nSize=GetSizeStrArray();
				if(p[0]->nType!=TYPE_EMPTY)
					SetSizeStrArray(p[0]->GetNumber());
				return nSize;
			}
		case enIdentifierByNumber:
			{
				int nSize=GetSizeStrArray();

				int nIndex=p[0]->GetNumber();
				nIndex--;
				if(nIndex<0||nIndex>=nSize)
					Error("Индекс не входит в границы допустимых значений");

				return String(GetIdentifierByNumber(nIndex));
				break;
			}
		case enValueByNumber:
			{
				int nSize=GetSizeStrArray();

				int nIndex=p[0]->GetNumber();
				nIndex--;
				if(nIndex<0||nIndex>=nSize)
					Error("Индекс не входит в границы допустимых значений");

				return GetAt(GetIdentifierByNumber(nIndex));
			}

		case enSort:
			{
				Sort(*p[0],p[1]->GetNumber());
				break;
			}
		case enSortId:
			{
				SortId(*p[0],p[1]->GetNumber());
				break;
			}
		case enRemoveAll:
			{
				RemoveAll();
				break;
			}
		case enLoad:
			{
				int nClear=p[2]->GetNumber();
				if(!p[2]->nType)//передано пустое значение
					nClear=1;

				Load(*p[0],p[1]->GetNumber(),nClear);
				break;
			}
		case enFind:
			{
				return Find(*p[0],*p[1]);
			}
		case enRemove:
			{
				RemoveKey(p[0]->GetString());
				break;
			}
		case enFindNumberID:
			{
				return FindNumber(p[0]->GetString())+1;
			}
	}
	return CValue();
}

int GetCloseStr(CString &Str,int nFirst)
{
	int nOpenCount=1;
	while(nOpenCount>0)
	{
		int n1=Str.Find("{",nFirst+1);
		int n2=Str.Find("}",nFirst+1);
		if(n2==-1)//ошибочное число скобок
			break;
		if(n1<n2&&n1>=0)
		{
			nFirst=n1;
			nOpenCount++;
		}
		else
		{
			nFirst=n2;
			nOpenCount--;
		}
	}
	return nFirst;
}

void CValueArray::SaveToString(CString &Str)
{
	int nCount=0;
	CString StrAll;
	CString StrVal;
	CString StrElement;
	CValue v;
	//выгрузка числового массива

	for(int Key=1;Key<=GetSizeArray();Key++)
	{
		nCount++;
		v=GetAt(Key);
		v.SaveToString(StrVal);
		StrElement.Format("{%d,%s}",Key,StrVal);
		StrAll+=StrElement;
	}

	int nCount2=0;
	CString StrAll2;
	//выгрузка строкового массива
	if(GetSizeStrArray())
	for(int i=0;i<pStringKey->GetSize();i++)
	{
		nCount2++;
		CString Key=pStringKey->GetAt(i);
		BOOL bFind;
		CValue *pVal=pStringValueList->GetPtrAt(Key,bFind);

		pVal->SaveToString(StrVal);
		ReplaceBySave(Key);
		StrElement.Format("{%s,%s}",Key,StrVal);
		StrAll2+=StrElement;
	}
/*	
	POSITION pos;
	if(GetSizeStrArray())
	for( pos = pStringValueList->GetStartPosition(); pos != NULL; )
	{
		CStringExt KeyExt;
		nCount2++;
		pStringValueList->GetNextAssoc( pos, KeyExt, v );
		CString Key(KeyExt);
		v.SaveToString(StrVal);
		ReplaceBySave(Key);
		StrElement.Format("{%s,%s}",Key,StrVal);
		StrAll2+=StrElement;
	}*/

	Str.Format("{5/%d/%s/%d/%s}",nCount,StrAll,nCount2,StrAll2);
};
void CValueArray::LoadFromString(CString &Str)
{
	int nIndex=LoadArray(Str,0,1);
	CString Str2=Str.Mid(nIndex);
	LoadArray(Str,nIndex,2);
}
int CValueArray::LoadArray(CString &Str,int nIndex0,int nMode)//1-int,2-string array
{
	int nCurIndex=nIndex0;
	int nIndex1=Str.Find("/",nIndex0);
	if(nIndex1>0)
	{
		int nIndex2=Str.Find("/",nIndex1+1);
		nCurIndex=nIndex2+1;
		int nCount=atoi(Str.Mid(nIndex1+1,nIndex2-nIndex1-1));
		if(nCount>0)
		{
			if(nMode==1)
			{
				SetSizeArray(nCount);
			}
			else
			if(nMode==2)
			{
				CheckStrArray();
				pStringValueList->InitHashTable(nCount);
			}
			CString StrVal;
			CString StrElement;
			CValue v;
			nCurIndex=nIndex2+1;//позиция текущего загружаемого элемента массива
			for(int i=0;i<nCount;i++)
			{
				//{Array/2/{2,{1/22}}{1,{1/11}}/2/{Тест2,{2/Привет2}}{Тест,{2/Привет}}}
				int nComma=Str.Find(",",nCurIndex);
				if(nComma<nCurIndex)
					break;//ошибка
				//загружаем ключ
				CString csKey=Str.Mid(nCurIndex+1,nComma-nCurIndex-1);
				int nKey=atoi(csKey);
				//загружаем значение массива (агрегатный объект)
				
				int nFirst=Str.Find("{",nComma);

				//ищем парную закрывающуюся скобку
				int nLast=GetCloseStr(Str,nFirst);


				v.LoadFromString(Str.Mid(nFirst,nLast-nFirst+1));
				if(nMode==1)
				{
					if(nKey>0)
						SetAt(nKey,v);
				}
				else
				if(nMode==2)
				{
					ReplaceByLoad(csKey);
					SetAt(csKey,v);
				}
				nCurIndex=nLast+2;
			}
		}
	}
	return nCurIndex;
}

int nListDirectSort2=1;//направление сортировки для функции сравнения

class CElementSort
{
public:
	CValue Value;	//значение
	CValue vSort;	//поле сортировки
};

int CompareListValue( const void *arg1, const void *arg2 )
{
   BOOL Res=(*(CElementSort*)arg1).vSort > (*(CElementSort*)arg2).vSort;
   if(Res)
	   return nListDirectSort2;
   Res=(*(CElementSort*)arg1).vSort == (*(CElementSort*)arg2).vSort;
   if(Res)
	   return 0;
   else
	   return -nListDirectSort2;
}

//сортировка численно-индексной части массива
void CValueArray::Sort(CValue cvSort, int nDirect)
{
	if(nDirect<=0)
		nListDirectSort2=1;
	else
		nListDirectSort2=-1;

	//выгружаем в список
	CArray <CElementSort,CElementSort&> aList;//список значений и представлений

	CValue Val;
	for(int nKey=1;nKey<=GetSizeArray();nKey++)
	{
		Val=GetAt(nKey);
		CElementSort data;
		data.Value=Val;
		data.vSort=Val;
		aList.Add(data);
	}
	if(aList.GetSize()>0)
		qsort(&aList[0], aList.GetSize(), sizeof(aList[0]), CompareListValue);

	aValueArray.RemoveAll();
	for(int i=0;i<aList.GetSize();i++)
	{
		SetAt(i+1,aList[i].Value);
	}
	aList.RemoveAll();
	CString csIdString;
	CValueArray aValues;
	CValueArray aIds;
	//Теперь то же самое для строковых индексов
	for(int nKey1=1;nKey1<=GetSizeStrArray();nKey1++)
	{
		csIdString=GetIdentifierByNumber(nKey1-1);
		Val=GetAt(csIdString);
		aValues.SetAt(CString(nKey1),Val);
		aIds.SetAt(CString(nKey1),CValue(csIdString));
		CElementSort data;
		data.Value=Val;
		data.vSort=Val;
		aList.Add(data);
	}
	if(aList.GetSize()>0)
		qsort(&aList[0], aList.GetSize(), sizeof(aList[0]), CompareListValue);
	//Удалить все строковые индексы
	if(pStringValueList)
	{
		pStringValueList->RemoveAll();
		delete pStringValueList;
		pStringValueList=0;
	}
	if(pStringKey)
	{
		pStringKey->RemoveAll();
		delete pStringKey;
		pStringKey=0;
	}
	//Занести из отсортированного массива
	CString csFind;
	for(int j=0;j<aList.GetSize();j++)
	{
		csFind=aValues.Find(aList[j].Value);
		SetAt(CString(aIds.GetAt(csFind)),aValues.GetAt(csFind));
		aValues.RemoveKey(csFind);
		aIds.RemoveKey(csFind);
	}
	aList.RemoveAll();
}

//сортировка строковых индексов
void CValueArray::SortId(CValue cvSort,int nDirect)
{
	if(nDirect<=0)
		nListDirectSort2=1;
	else
		nListDirectSort2=-1;

	//выгружаем в список
	CArray <CElementSort,CElementSort&> aList;//список значений и представлений

	CValue Val;
	CString csIdString;
	CValueArray aValues;
	CValueArray aIds;
	for(int nKey1=1;nKey1<=GetSizeStrArray();nKey1++)
	{
		csIdString=GetIdentifierByNumber(nKey1-1);
		Val=CValue(csIdString);
		aValues.SetAt(CString(nKey1),GetAt(csIdString));
		aIds.SetAt(CString(nKey1),CValue(csIdString));
		CElementSort data;
		data.Value=Val;
		data.vSort=Val;
		aList.Add(data);
	}
	if(aList.GetSize()>0)
		qsort(&aList[0], aList.GetSize(), sizeof(aList[0]), CompareListValue);
	//Удалить все строковые индексы
	if(pStringValueList)
	{
		pStringValueList->RemoveAll();
		delete pStringValueList;
		pStringValueList=0;
	}
	if(pStringKey)
	{
		pStringKey->RemoveAll();
		delete pStringKey;
		pStringKey=0;
	}
	//Занести из отсортированного массива
	CString csFind;
	for(int j=0;j<aList.GetSize();j++)
	{
		csFind=aIds.Find(aList[j].Value);
		SetAt(CString(aIds.GetAt(csFind)),aValues.GetAt(csFind));
		aValues.RemoveKey(csFind);
		aIds.RemoveKey(csFind);
	}
	aList.RemoveAll();
}

void CValueArray::Load(CValue &vSource,int nMode,int nClear)//nMode - количество вложенных загрузок массивов
{
	if(nMode<0)
		return;

	if(nClear)
		RemoveAll();

	if(vSource.GetType()!=TYPE_ARRAY)
		SetError("Объект-источник не является массивом");

	CValueArray *pSource=(CValueArray *)vSource.pRef;
	POSITION pos;
	CValue Val;

	//загружаем индексно-численный массив
	int nDelta=GetSizeArray();
	int nStart=1;
	int nFinish=pSource->GetSizeArray();
	SetSizeArray(nDelta+nFinish);
	for(int nKey=nStart;nKey<=nFinish;nKey++)
	{
		Val=pSource->GetAt(nKey);
		if(nMode>0)
		{
			if(TYPE_ARRAY==Val.GetType())
			{
				CValue cNewVal;
				cNewVal.CreateObject(Val.GetTypeString());
				CValueArray *pNewVal=(CValueArray *)cNewVal.pRef;
				pNewVal->Load(Val,nMode-1);
				SetAt(nKey+nDelta,cNewVal);
				continue;
			}
		}
		SetAt(nKey+nDelta,Val);
	}

	CStringExt csKey;
	//загружаем индексно-строковый массив
	int nCount=pSource->GetSizeStrArray();
	if(nCount)
	{
		CValue Val;
		CheckStrArray();
		if(nClear)
		{
			pStringValueList->InitHashTable(nCount);
			pStringKey->SetSize(0,nCount/3);
		}
		for( pos = pSource->pStringValueList->GetStartPosition(); pos != NULL; )
		{
			pSource->pStringValueList->GetNextAssoc( pos, csKey, Val);
			if(nMode>0)
			{
				if(TYPE_ARRAY==Val.GetType())
				{
					CValue cNewVal;
					cNewVal.CreateObject(Val.GetTypeString());
					CValueArray *pNewVal=(CValueArray *)cNewVal.pRef;
					pNewVal->Load(Val,nMode-1);
					SetAt(csKey,cNewVal);
					continue;
				}
			}

			SetAt(csKey,Val);
		}
	}
}
void CValueArray::RemoveAll()
{
	aValueArray.RemoveAll();

	if(pStringValueList)
	{
		pStringValueList->RemoveAll();
		delete pStringValueList;
		pStringValueList=0;
	}
	if(pStringKey)
	{
		pStringKey->RemoveAll();
		delete pStringKey;
		pStringKey=0;
	}

//	aStringValueList.RemoveAll();
//	aStringKey.RemoveAll();
}

CValue CValueArray::Find(CValue &vFind,CValue vExtValue)
{
	int nMode=vExtValue.nType;
	CString csName=vExtValue.GetString();
	int nName=vExtValue.GetNumber();
	if(nMode==TYPE_STRING&&csName.IsEmpty())
		nMode=0;


	POSITION pos;
	CValue Val;
	//ищем в числах
	for(int nKey=1;nKey<=GetSizeArray();nKey++)
	{
		Val=GetAt(nKey);

		if(nMode==TYPE_STRING)
			Val=Val.GetAt(csName);
		else
		if(nMode==TYPE_NUMBER)
			Val=Val.GetAt(nName);

		if(Val==vFind)
			return nKey;
	}
	//ищем в строках
	CStringExt csKey;
	if(GetSizeStrArray())
	for( pos = pStringValueList->GetStartPosition(); pos != NULL; )
	{
		pStringValueList->GetNextAssoc( pos, csKey, Val);
		
		if(nMode==TYPE_STRING)
			Val=Val.GetAt(csName);
		else
		if(nMode==TYPE_NUMBER)
			Val=Val.GetAt(nName);

		if(Val==vFind)
			return String(csKey);
	}
	return CValue();
}




int CValueArray::GetType(void)const
{
	return TYPE_ARRAY;
};
int CValueArray::GetSizeArray(void)const
{
	return aValueArray.GetSize();
};
void CValueArray::SetSizeArray(int nSize)
{
	if(nSize>0)
	{
		aValueArray.SetSize(nSize);
	}
};
int CValueArray::GetSizeStrArray(void)const
{
	if(pStringValueList)
		return pStringValueList->GetCount();
	else
		return 0;
};
void CValueArray::SetSizeStrArray(int nSize)
{
	CheckStrArray();
	if(nSize>=0)
	{
		pStringValueList->InitHashTable(nSize);
		pStringKey->SetSize(pStringKey->GetSize(),nSize/3);
	}
};

void CValueArray::CheckIndex(int nIndex)//индекс массива должен начинаться с 1
{
	if(nIndex<1)
		Error("Индекс выходит за границы массива");

	if(nIndex>aValueArray.GetSize())//автоувеличение
	{
		int nGrowBy=nIndex/4;
		if(nGrowBy<4)
			nGrowBy=4;

		aValueArray.SetSize(nIndex,nGrowBy);
		int n1=aValueArray.GetSize();
	}
}
void CValueArray::SetAt(int nIndex,CValue Val)//индекс массива должен начинаться с 1
{
	CheckIndex(nIndex);

	if(Val.nType==TYPE_REFFER)
		aValueArray[nIndex-1]=Val.GetValue();
	else
		aValueArray[nIndex-1]=Val;
};
void CValueArray::SetAt(CString Key,CValue Val)
{
	CheckStrArray();

	BOOL bFind;
	CValue *pVal=pStringValueList->GetPtrAt(Key,bFind);
	if(!bFind)
		pStringKey->Add(Key);
	if(Val.nType==TYPE_REFFER)
		*pVal=Val.GetValue();
	else
		*pVal=Val;
};

CValue& CValueArray::GetAt(int nIndex)//индекс массива должен начинаться с 1
{
	CheckIndex(nIndex);

	return aValueArray[nIndex-1];
};

CValue& CValueArray::GetAt(CString Key)
{
	CheckStrArray();

	BOOL bFind;
	CValue *pVal=pStringValueList->GetPtrAt(Key,bFind);
	if(!bFind)
		pStringKey->Add(Key);
	return *pVal;
};


CString CValueArray::GetTypeString(void)const
{
	return "Массив";
};
CString CValueArray::GetString(void)const
{
	return "Массив";
};

CString CValueArray::GetIdentifierByNumber(int nIndex)
{
	CheckStrArray();

	return (*pStringKey)[nIndex];
};

BOOL CValueArray::FindId(CString Key)
{
	CheckStrArray();

	CValue Temp;
	return pStringValueList->Lookup(Key,Temp);
}
int CValueArray::FindNumber(CString Key)
{
	CheckStrArray();

	Key=mUpper(Key);
	for(int i=0;i<pStringKey->GetSize();i++)
	if(mUpper(pStringKey->GetAt(i))==Key)
	{
		return i;
	}
	return -1;
}


void CValueArray::RemoveKey(CString Key)
{
	CheckStrArray();
	CValue Temp;
	if(pStringValueList->Lookup(Key,Temp))
	{
		int n=FindNumber(Key);
		if(n>=0)
		{
			pStringKey->RemoveAt(n);
		}
		pStringValueList->RemoveKey(Key);
	}
}

