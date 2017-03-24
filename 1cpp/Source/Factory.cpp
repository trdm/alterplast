
#include "StdAfx.h"
#include "Factory.h"
#include "istrmap.h"
#include "mycontextbase.h"
#include "ComponentClass.h"

BEGIN_BL_METH_MAP(CObjectFactory)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("New", "Новый", 1, funcNewObject)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CObjectFactory)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CObjectFactory, "ObjectFactory", "ФабрикаОбъектов");

enum{nMethodsCount = sizeof(CObjectFactory::m_MethDef)/sizeof(CObjectFactory::m_MethDef[0]) - 1};

class CClassItem : public CCollectionItem
{
public:
    CRuntimeClass* m_pRuntimeClass;
    int m_nCtorParamsCount;
};

CCollection collClasses;

void CObjectFactory::RegisterClass(LPCSTR szClassName, CRuntimeClass* pRuntimeClass, int nCtorParamsCount)
{
    CClassItem* pClassItem = new CClassItem;
    pClassItem->SetName(szClassName);
    pClassItem->m_pRuntimeClass = pRuntimeClass;
    pClassItem->m_nCtorParamsCount = nCtorParamsCount;
    collClasses.Add(pClassItem);
}

CObjectFactory::CObjectFactory()
{
}

int CObjectFactory::GetNMethods(void)const
{
    return nMethodsCount + collClasses.GetCount();
}

int CObjectFactory::FindMethod(char const* szName)const
{
	int iRes = CMyContextBase::FindMethod(szName);
	if (-1 != iRes)
		return iRes;

	return collClasses.GetIndex(szName);
	
}

char const* CObjectFactory::GetMethodName(int nMethIndex, int bAlias)const
{
	if (nMethIndex < nMethodsCount)
	{
		return CMyContextBase::GetMethodName(nMethIndex, bAlias);
	}

    return collClasses[nMethIndex - nMethodsCount]->GetName();
}

int CObjectFactory::GetNParams(int nMethIndex)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::GetNParams(nMethIndex);
	}
    
	return static_cast<CClassItem*>(collClasses[nMethIndex - nMethodsCount])->m_nCtorParamsCount;
}

int CObjectFactory::GetParamDefValue(int nMethIndex, int nParamIndex, class CValue * pValue)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::GetParamDefValue(nMethIndex, nParamIndex, pValue);
	}

    pValue->Reset();
	pValue->type = -1;
    return 1;
}

int CObjectFactory::HasRetVal(int nMethIndex)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::HasRetVal(nMethIndex);
	}
    return 1;
}

int CObjectFactory::CallAsProc(int nMethIndex, class CValue** ppValues)
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::CallAsProc(nMethIndex, ppValues);
	}
    return 0;
}

int CObjectFactory::CallAsFunc(int nMethIndex, class CValue& rValue, class CValue** ppValues)
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::CallAsFunc(nMethIndex, rValue, ppValues);
	}

    CClassItem* pClassItem = static_cast<CClassItem*>(collClasses[nMethIndex - nMethodsCount]);

	// здесь была утечка памяти !
	// раньше создание объекта - +1 IncrRef, rValue.AssignContext - +1 IncrRef
	// в итоге счетчик ссылок равен 2, и объект зависнет в памяти !!
	// сейчас создание объекта - +1 IncrRef, rValue.AssignContext - +1 IncrRef
    //CBLContext* pBLContext = static_cast<CBLContext*>(pClassItem->m_pRuntimeClass->CreateObject());
     CBLPtr<CBLContext> pCont(*pClassItem->m_pRuntimeClass);
 	CBLContext* pBLContext = pCont;
	
    pBLContext->InitObject(pClassItem->GetName());
    
    BOOL bSuccess;

    if (pBLContext->IsKindOf(RUNTIME_CLASS(CComponentClass)))
    {
        int nIndex = pBLContext->FindMethod("_Create");
        if (nIndex != -1)
        {
            int nParamCount = pBLContext->GetNParams(nIndex);
            if (nParamCount == pClassItem->m_nCtorParamsCount)
                bSuccess = pBLContext->CallAsProc(nIndex, ppValues);
            else
                RuntimeError("Не совпадает количество параметров.");
        }
        else
            RuntimeError("Не найден конструктор.");
    }
    else
        //if object is internal, then he must be inherited by CMyContextBase
        bSuccess = static_cast<CMyContextBase*>(pBLContext)->_Create(ppValues);

    if (bSuccess)
        rValue.AssignContext(pBLContext);
    else
    {
        rValue.Reset();
        //pBLContext->DecrRef(); // при использования CBLPtr не нужно
    }

    return bSuccess;
}

BOOL CObjectFactory::funcNewObject(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& valTypeName = *ppValue[0];
	// TODO нужна ли проверка. Без нее создание объектов будет быстрее !
	//ValidateParamNotEmptyString(valTypeName, "Новый : необходимо передать строку с названием типа");

	CString strTypeName = valTypeName.GetString();
	
	CString  strConstructorName;
	if (nParamsCount >= 2)
	{
		// TODO нужна ли проверка. Без нее создание объектов будет быстрее !
		//ValidateParamNotEmptyString(*ppValue[1], "Новый : необходимо передать строку с названием конструктора");
		strConstructorName = ppValue[1]->GetString();

		CComponentClass::SetFlagCreateWithoutConstructor();
	}

	CBLPtr<CBLContext> pCont(strTypeName);
	
	CComponentClass::ClearFlagCreateWithoutConstructor();
	if (!pCont)
		RuntimeError("Фабрика.Новый - Ошибка при создании класса %s", strTypeName);

	if (nParamsCount >= 2)
	{
		// сначала попытаюсь найти метод у объекта-контекста
		// это даст универсальность при работе с другими объектами, не только с классами КОП 1С++

		CValue** ppValueForCallMethod = &ppValue[2];

		if (IS_KINDOF_CComponentClass(pCont))
		{
			const CComponentClass* pClassCont = (CComponentClass*)(CBLContext*)pCont;
			pClassCont->CallUserConstructor(strConstructorName, nParamsCount-2, ppValueForCallMethod);
		}
		else
		{
			int iProc = pCont->FindMethod(strConstructorName);
			if (-1 == iProc)
				RuntimeError("У класса <%s> не существует процедуры-конструктора <%s>", strTypeName, strConstructorName);

			if (pCont->HasRetVal(iProc))
				RuntimeError("У класса <%s> не существует процедуры-конструктора <%s>, но есть такая функция", strTypeName, strConstructorName);


			// TODO здесь есть баг при вызове метода с параметрами по умолчанию - 
			//	например, из-за этого падает тест ТестыФабрики::Тест_КонструкторИндексированнойТаблицы
			int nMethodParams = pCont->GetNParams(iProc);
			int nCallParams = nParamsCount - 2;
			
			// Проверим, что передано точное количество параметров, и заполним значения по умолчанию
			if(nMethodParams < nCallParams)
				RuntimeError("%s::%s - Слишком много фактических параметров при вызове", strTypeName, strConstructorName);
			
			vector<CValue*> ValueParams; //CValuePtrArray ValueParams;
			int nUpperBound = nCallParams < nMethodParams ? nCallParams : nMethodParams;
			CValue** ppValueForCallMethod = ppValue + 2;

			ValueParams.reserve(nMethodParams);
			for(int i=0;i<nUpperBound;i++)
			{
				if((*ppValueForCallMethod)->type == -1)	// Значение по умолчанию
				{
					if(!pCont->GetParamDefValue(iProc, i, *ppValueForCallMethod))
						RuntimeError("%s::%s - Неуказан обязательный параметр № %i", strTypeName, strConstructorName, i);
				}
				ValueParams.push_back(*ppValueForCallMethod++); //ValueParams.Add(*ppValueForCallMethod++);
			}

			vector<CValue> pNewValues;

			if(nUpperBound < nMethodParams)
			{
				pNewValues.resize(nMethodParams - nUpperBound);
				CValue* pNewParams = &pNewValues[0];
				for(int i=nUpperBound; i<nMethodParams; i++)
				{
					if(!pCont->GetParamDefValue(iProc, i, pNewParams))
						RuntimeError("%s::%s - Неуказан обязательный параметр № %i", strTypeName, strConstructorName, i);
					ValueParams.push_back(pNewParams++); //ValueParams.Add(pNewParams++);
				}
			}
			pCont->CallAsProc(iProc, &ValueParams[0]); //pCont->CallAsProc(iProc, ValueParams.GetData());
		}
	}

	RetVal.AssignContext(pCont);

	return TRUE;
}
