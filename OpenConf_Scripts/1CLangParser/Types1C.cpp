
#include <afxdisp.h>
#include "Types1C.h"
#include "CommonFunctions.h"

//√лобальна€ таблица типов
CIStringMapToPtr* CTypes1C::pTypes = NULL;
CIStringMapToString* CTypes1C::pSynonyms = NULL;
//—четчик созданных экземпл€ров
int CTypes1C::InstanceCounter = 0;



CTypes1C::CTypes1C()
{
	InstanceCounter++;
	if( pTypes == NULL )
	{
		pTypes = new CIStringMapToPtr;
		pSynonyms = new CIStringMapToString;
	}
}

CTypes1C::~CTypes1C()
{
	InstanceCounter--;
	if( InstanceCounter == 0 )
	{
		RemoveAll();
		delete pTypes;
		delete pSynonyms;
	}
}

void CTypes1C::RemoveAll()
{
	CString TypeName;
	void* type;
	POSITION pos = pTypes->GetStartPosition();
	while( pos != NULL )
	{
		pTypes->GetNextAssoc(pos, TypeName, type);
		delete ((CType1C*)type);
	}
	pTypes->RemoveAll();
	pSynonyms->RemoveAll();
}

void CTypes1C::AddType(const char* TypeName, const char* Synonym, CType1C* Type)
{
	int len = strlen(TypeName);
	if( len == 0 )
		return;

	if( TypeName[len-1] == '.' )
		((char*)TypeName)[len-1] = '\0';

	len = strlen(Synonym);
	if( len > 0 && Synonym[len-1] == '.' )
		((char*)Synonym)[len-1] = '\0';

	pTypes->SetAt(TypeName, Type);
	pSynonyms->SetAt(Synonym, TypeName);
}

void CTypes1C::RemoveType(const char* TypeName)
{
	CType1C* pType = GetType(TypeName);

	pTypes->RemoveKey(TypeName);
	pSynonyms->RemoveKey(TypeName);

	if( pType )
		delete pType;
}

CType1C* CTypes1C::GetType(const char* TypeName)
{
	void* type;

	int len = strlen(TypeName);
	if( len == 0 )
		return NULL;

	if( TypeName[len-1] == '.' )
		((char*)TypeName)[len-1] = '\0';

	if( !pTypes->Lookup(TypeName, type) )
	{
		CString TypeSynonym;
		if( !pSynonyms->Lookup(TypeName, TypeSynonym) )
			return NULL;
		if( !pTypes->Lookup(TypeSynonym, type) )
			return NULL;
	}

	return (CType1C*)type;
}

CString CTypes1C::GetSubType(const char* TypeName, const char* PropName)
{
	CType1C* Type = GetType(TypeName);
	if( Type == NULL )
		return "";
	return Type->GetPropType(PropName);
}

//==============================================================================
CType1C::CType1C(CString aTypeName)
{
	BaseTypes.SetSize(4);
	nBaseTypesCount = 0;
	nIteratedBaseType = 0;

	TypeName = aTypeName;
	IteratorPos = NULL;
}

void CType1C::AddBaseType(CType1C* pBaseType)
{
	BaseTypes.SetAt(nBaseTypesCount, pBaseType);
	nBaseTypesCount++;
}

void CType1C::AddProperty(CString Name, CString Synonym, CString Type)
{
	Properties.SetAt(Name, Type);
	Synonyms.SetAt(Synonym, Name);
	Msg("Add prop %s : %s", Name, Type);
}

bool CType1C::PropExists(const char* PropName)
{
	CString Type;
	if( Properties.Lookup(PropName, Type) )
		return true;

	if( nBaseTypesCount )
	{
		for( int i = 0; i < nBaseTypesCount; i++ )
			if( ((CType1C*)(BaseTypes[i]))->PropExists(PropName) ) return true;
	}

	return false;
}

CString CType1C::GetPropType(const char* PropName)
{
	CString Type;
	bool Found = false;

	if( !Properties.Lookup(PropName, Type) )
	{
		CString Synonym;
		if( Synonyms.Lookup(PropName, Synonym) )
			Found = Properties.Lookup(Synonym, Type);
	}

	if( !Found && nBaseTypesCount )
	{
		for( int i = 0; i < nBaseTypesCount; i++ )
		{
			Type = ((CType1C*)(BaseTypes[i]))->GetPropType(PropName);
			if( !Type.IsEmpty() ) break;
		}
	}

	return Type;
}

void CType1C::StartPropsIteration()
{
	if( nBaseTypesCount )
	{
		nIteratedBaseType = 0;
		((CType1C*)(BaseTypes[0]))->StartPropsIteration();
	}
	IteratorPos = Properties.GetStartPosition();
}

bool CType1C::NextProp(CString& PropName)
{
	if( nBaseTypesCount )
	{
		CType1C* pBaseType = (CType1C*)(BaseTypes[nIteratedBaseType]);
		if( pBaseType->NextProp(PropName) ) return true;

		while( nIteratedBaseType < (nBaseTypesCount-1))
		{
			nIteratedBaseType++;
			pBaseType = (CType1C*)(BaseTypes[nIteratedBaseType]);
			if( pBaseType->NextProp(PropName) ) return true;
		}
	}

	if( IteratorPos == NULL )
		return false;

	CString PropType;
	Properties.GetNextAssoc(IteratorPos, PropName, PropType);
	return true;
}


//==============================================================================
/*
CMetaDataType1C::CMetaDataType1C(CString& strType, CString& strKind, IConfigurator* pConfigurator)
:CType1C(strType+"."+strKind, NULL)
{
	pMetaDataObj = NULL;

	IMetaData* pMetaData = NULL;
	pConfigurator->get_MetaData(&pMetaData);
	ITaskDef* pTaskDef = NULL;
	pMetaData->get_TaskDef(&pTaskDef);
	IMetaDataObjChilds* pChilds = NULL;
	pTaskDef->get_Childs(&pChilds);

	long nChilds;
	pChilds->get_Count(&nChilds);
	COleVariant idxChild;
	VARIANT_BOOL bRus = true;
	BSTR bstrChildName;
	for( long i = 0; i < nChilds; i++ )
	{
		idxChild = i;
		pChilds->get_Name(idxChild, bRus, &bstrChildName);
		if( strType == bstrChildName )
		{
			IMetaDataObjArray* pTypeChildsArray;
			pChilds->get_item(idxChild, &pTypeChildsArray);
			pTypeChildsArray->get_Count(&nChilds);
			for( i = 0; i < nChilds; i++ )
			{
				idxChild = i;
				IMetaDataObj* pMDObj;
				pTypeChildsArray->get_item(idxChild, &pMDObj);
				BSTR bstrKindName;
				pMDObj->get_Name(&bstrKindName);
				if( strKind == bstrKindName )
				{
					pMetaDataObj = pMDObj;
					debug("type %S.%S", bstrChildName, bstrKindName);
				}
			}
			break;
		}
	}
	//.Childs(("—правочник"));
}

bool CMetaDataType1C::PropExists(const char* PropName)
{
	//pMetaDataObj->;
	return false;
}

CString CMetaDataType1C::GetPropType(const char* PropName)
{
	return "";
}

void CMetaDataType1C::StartPropsIteration()
{
}

bool CMetaDataType1C::NextProp(CString& PropName)
{
	return false;
}

*/


//==============================================================================
CTypeExemplarModifyingMethodList::~CTypeExemplarModifyingMethodList()
{
	CString MethodName;
	void* pMethod;
	POSITION pos = GetStartPosition();
	while( pos != NULL )
	{
		GetNextAssoc(pos, MethodName, pMethod);
		delete ((CTypeExemplarModifyingMethod*)pMethod);
	}
}

CTypeExemplarModifyingMethod* 
CTypeExemplarModifyingMethodList::Add(const char* MethodName, int nParam_PropName, int nParam_PropType)
{
	CTypeExemplarModifyingMethod* pMethod;

	if( !Lookup(MethodName, (void*&)pMethod) )
	{
		pMethod = new CTypeExemplarModifyingMethod;
		pMethod->Name = MethodName;
		SetAt(MethodName, (void*)pMethod);
	}

	pMethod->nParam_PropName = nParam_PropName;
	pMethod->nParam_PropType = nParam_PropType;


	return pMethod;
}

CTypeExemplarModifyingMethod* CTypeExemplarModifyingMethodList::GetMethod(const char* MethodName)
{
	void* pMethod;
	if( Lookup(MethodName, pMethod) )
		return (CTypeExemplarModifyingMethod*)pMethod;
	return NULL;
}


//==============================================================================
void CVariable::AddProperty(const char* PropName, const char* PropType)
{
	if( PropType != NULL && *PropType != '\0' ) //type is not empty
	{
		Properties.SetAt(PropName, PropType);
	}
	else
	{
		CString csPropType;
		if( !Properties.Lookup(PropName, csPropType) )
			Properties.SetAt(PropName, PropType);
	}
}

CString CVariable::GetPropType(const char* PropName)
{
	CString PropType;
	Properties.Lookup(PropName, PropType);
	return PropType;
}

void CVariable::StartPropsIteration()
{
	IteratorPos = Properties.GetStartPosition();
}

bool CVariable::NextProp(CString& PropName)
{
	if( IteratorPos == NULL )
		return false;

	CString PropType;
	Properties.GetNextAssoc(IteratorPos, PropName, PropType);
	return true;
}

//==============================================================================
CVarList::~CVarList()
{
	CString VarName;
	void* pVar;
	POSITION pos = GetStartPosition();
	while( pos != NULL )
	{
		GetNextAssoc(pos, VarName, pVar);
		delete ((CVariable*)pVar);
	}
}

void CVarList::Add(const char* VarName, CVariable* Var)
{
	SetAt(VarName, (void*)Var);
}

CVariable* CVarList::Add(const char* VarName, const char* VarType)
{
	void* ptr;
	CVariable* pVar;
	if( Lookup(VarName, ptr) )
	{
		pVar = (CVariable*)ptr;
	}
	else
	{
		pVar = new CVariable(VarType);
		Add(VarName, pVar);
	}

	if( pVar && VarType != NULL && *VarType != '\0' )
		pVar->Type = VarType;

	return pVar;
}

CVariable* CVarList::GetVar(const char* VarName)
{
	void* pVar;
	if( Lookup(VarName, pVar) )
		return (CVariable*)pVar;
	return NULL;
}

