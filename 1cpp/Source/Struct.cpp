// Struct.cpp: implementation of the CStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "Struct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CBkEndUI * pBkEndUI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CStruct, CBLContext);

class CObjID CStruct::ObjID;

enum {
	procInsert,
		funcCount,
		procClear,
		funcProperty,
		procRemove,
		funcGet,
		funcExistProp,
		lastMethod
};

//////////////////////////////////////////////////////////////////////
// Описание методов и функций класса для 1С языка
// {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
//////////////////////////////////////////////////////////////////////
struct CStruct::paramdefs CStruct::defFnNames[] = {
	{"Insert","Вставить",0,3},
	{"Count","Количество",1,0},
	{"Clear","Очистить",0,0},
	{"Property","Свойство",1,2},
	{"Remove","Удалить",0,1},
	{"Get","Получить",1, 2},
	{"ExistProp","ЕстьСвойство",1, 1},
	{NULL,NULL,0,0}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStruct::CStruct(int iPar):CBLContext(iPar), m_nCurIndex(0), m_mapProperty(TRUE)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	  
}

CStruct::~CStruct()
{    
	Clear();
}

CStruct::InnerNode* CStruct::AddNewNode(LPCSTR szKey)
{
	InnerNode *pInnerNode = NULL;
	if (!m_mapProperty.Lookup(szKey, (void*&)pInnerNode))
	{
		pInnerNode = new InnerNode(m_nCurIndex++, szKey);
		
		m_mapProperty[pInnerNode->szName] = pInnerNode;
		m_Array.Add(pInnerNode);
	}
	return pInnerNode;
}

void CStruct::RemoveNode(LPCSTR szKey)
{
	InnerNode *pInNode = NULL;
	if (m_mapProperty.Lookup(szKey, (void*&)pInNode))
	{
		int nUpperBound = m_Array.GetSize();
		--m_nCurIndex;
		int nIndex = pInNode->nIndex;
		for (int ind = pInNode->nIndex; ind < nUpperBound; ind++)
			--m_Array[ind]->nIndex;          
		
		m_Array.RemoveAt(nIndex);
		m_mapProperty.RemoveKey(szKey);
		delete pInNode; // TODO код дублирован 2 раза
	}
	else // исключение в случае неверного ключа
		RuntimeError("Структура::Удалить: Подобного ключа не существует");
}

void CStruct::Clear()
{    
	LPCSTR key;
	InnerNode *pInNode = NULL;
	for(POSITION pos = m_mapProperty.GetStartPosition(); pos != NULL; )
	{
		m_mapProperty.GetNextAssoc( pos, key, (void*&)pInNode);
		delete pInNode;// TODO неявная работа с памятью, проще исправить на смарт-пойнтер
	}
	m_mapProperty.RemoveAll();
	m_Array.RemoveAll();
	m_nCurIndex = 0;
}

int  CStruct::CallAsFunc(int iMethNum,class CValue & rValue,class CValue** ppValue)
{
	switch(iMethNum)
	{
    case funcCount:
		{
			rValue = CNumeric(m_Array.GetSize());
		}
		break;
    case funcProperty:
		{
			// допустимы любые явные и непустые приведенные строки
			CString strKey = ppValue[0]->GetString();
			if ((ppValue[0]->GetTypeCode() != STRING_TYPE_1C) && strKey.IsEmpty())
				RuntimeError("Необходимо в методе Свойство() определить строковый ключ!");
			
			LPCSTR szKey = (LPCSTR)strKey;
			
			InnerNode *pInNode = NULL;
			if (m_mapProperty.Lookup(szKey, (void*&)pInNode))
			{
				if (pInNode->bIsPtrVal)
				{
					ppValue[1] = pInNode->ptr_value;
				}
				else
					*ppValue[1] = pInNode->value;

				rValue = CNumeric(1);
			}
			else
			{
				ppValue[1]->Reset();          
				rValue = CNumeric(0);
			}
		}
		break;
    case funcGet:
		{
			int nInd = ppValue[0]->GetNumeric();        
			int nUpperBound = m_Array.GetSize();
			if (nInd > nUpperBound || nInd < 1)
				CBLModule::RaiseExtRuntimeError("Неверно определен индекс для метода Получить!", 0);
			
			InnerNode *pInNode = m_Array[nInd-1];
			if (pInNode->bIsPtrVal)
				rValue = *pInNode->ptr_value;
			else
				rValue = pInNode->value;
			
			*ppValue[1] = pInNode->szName;
		}
		break;
    case funcExistProp:
		{
			rValue = FindProp(ppValue[0]->GetString()) + 1;
		}
		break;
    default:
		{
			
		}
	}  
	return 1;
}

int  CStruct::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	switch(iMethNum)
	{
    case procInsert:
		{
			// допустимы любые явные и непустые приведенные строки
			CString strKey = ppValue[0]->GetString();
			if ((ppValue[0]->GetTypeCode() != STRING_TYPE_1C) && strKey.IsEmpty())
				RuntimeError("Необходимо в методе Свойство() определить строковый ключ!");
			
			InnerNode *pInnerNode = AddNewNode(strKey);

			int bWhat = ppValue[2]->GetNumeric().operator long();
			if      (bWhat == 1)
			{
				pInnerNode->value = *ppValue[1];
				pInnerNode->bIsPtrVal = false;
			}
			else if (bWhat == 0)
			{
				pInnerNode->ptr_value = ppValue[1];          
				pInnerNode->bIsPtrVal = true;
			}          
			else
				RuntimeError("В методе Вставить() третий параметр может принимать значения 1 или 0!", 0);        

		}
		break;
    case procClear:
		{
			Clear();
		}
		break;
    case procRemove:
		{
			// допустимы любые явные и непустые приведенные строки
			CString strKey = ppValue[0]->GetString();
			if ((ppValue[0]->GetTypeCode() != STRING_TYPE_1C) && strKey.IsEmpty())
				RuntimeError("Необходимо в методе Удалить() определить строковый ключ!");

			RemoveNode(strKey);
		}
		break;
    default:
		{
			
		}
	}  
	return 1;
}


int  CStruct::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	return -1;
}

char const *  CStruct::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CStruct::GetNMethods(void)const
{
	return lastMethod;
}

int  CStruct::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CStruct::GetNParams(int iMethodNum)const
{
	return defFnNames[iMethodNum].NumberOfParams;
}

int  CStruct::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	switch(iMethodNum)
	{
    case procInsert:
		{
			if (iParamNum == 1)
			{
				pDefValue->Reset();
				return TRUE;
			}
			else if (iParamNum == 2)
			{
				*pDefValue = 1;
				return TRUE;
			}
		}
		break;
    case funcProperty:
		{
			if (iParamNum == 1)
			{
				pDefValue->Reset();
				return TRUE;
			}
		}
		break;
    case funcGet:
		{
			if (iParamNum == 1)        
				return TRUE;        
		}
		break;
    default:
		return TRUE;
	}
	return TRUE;
}

char const *  CStruct::GetCode(void)const
{
	return 0;
}

int  CStruct::GetDestroyUnRefd(void)const
{
	return 1;
}

class CObjID   CStruct::GetID(void)const
{
	return ObjID;
}

long  CStruct::GetTypeID(void)const
{
	return 100;
}

char const *  CStruct::GetTypeString(void)const
{
	return "Структура";
}

class CType   CStruct::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}

int  CStruct::IsExactValue(void)const
{
	return 0;
}

int  CStruct::IsOleContext(void)const
{
	return 0;
}

int  CStruct::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CStruct::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CStruct::IsSerializable(void)
{
	return 0;
}

int  CStruct::SaveToString(class CString & csStr)
{
	csStr = "DJK";
	return 1;
}

int  CStruct::GetNProps(void)const
{
	return m_mapProperty.GetCount();
}

char const *  CStruct::GetPropName(int A,int B)const
{
	return m_Array[A]->szName;
}

int  CStruct::GetPropVal(int iPropNum,class CValue & rValue)const
{
	InnerNode *pInNode = m_Array[iPropNum];
	if (pInNode)
	{
		if (pInNode->bIsPtrVal)    
			rValue = *pInNode->ptr_value;    
		else      
			rValue = pInNode->value;    
		
	}
	else
		return -1;
	return 1;
}

int  CStruct::SetPropVal(int iPropNum,class CValue const & vValue)
{
	InnerNode *pInNode = m_Array[iPropNum];
	if (pInNode)
		if (pInNode->bIsPtrVal)   
			*pInNode->ptr_value = vValue;
		else
			pInNode->value = vValue;
		
		else
			return -1;
		return 1;  
}

int  CStruct::FindProp(char const * Name)const
{
	LPCSTR szKey = Name;  
	InnerNode *pInNode = NULL;
	if (m_mapProperty.Lookup(szKey, (void*&)pInNode))
	{        
		return pInNode->nIndex;
	} 
	else
		return -1;    	
}