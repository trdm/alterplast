#if !defined(_CAST_CONTEXT_H__INCLUDED_)
	#define _CAST_CONTEXT_H__INCLUDED_

// Автор исходной версии - Саша Орефков by orefkov
// http://www.1cpp.ru/forum/YaBB.pl?num=1210164626/4#4

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Заголовок интерфейса для обхода древовидной структуры объектов.
// Если ваш класс, реализующий CBLContext, имеет иерархию, он должен реализовать и этот интерфейс.
struct IContextTree
{
	// Получить количество базовых объектов.
	virtual DWORD GetBaseObjCount() = 0;
	// Получить имя базового объекта по индексу.
	virtual CString GetBaseObjName(DWORD idx) = 0;
	// Получить базовый объект по индексу.
	virtual CBLContext* GetBaseObj(DWORD idx) = 0;
	// Для upcast'а
	virtual CBLContext* GetDerivedObj() = 0;
};

// Набор различных методов приведения контекстов
struct CastContext
{
	// Общая реализация обхода дерева с проверкой узлов передаваемым функтором
	template<typename Op>
	static CBLContext* DoCast(CBLContext* pCont, Op& op)
	{
		if(op.CheckContext(pCont))
			return pCont;
		IContextTree* pTree;
		try{
			if(!(pTree = dynamic_cast<IContextTree*>(pCont)))
				return NULL;
		}catch(...){ return NULL; }

		for(DWORD idx = 0, nBases = pTree->GetBaseObjCount(); idx < nBases ; idx++)
		{
			CBLContext* pBaseObj = pTree->GetBaseObj(idx);
			if(op.CheckBase(pTree, idx, pBaseObj))
				return pBaseObj;
			if(pBaseObj = DoCast(pBaseObj, op))
				return pBaseObj;
		}
		return NULL;
	}
	// Пустой функтор
	struct by_empty 
	{
		BOOL CheckContext(CBLContext* pCont) {return FALSE; }
		BOOL CheckBase(IContextTree* pTree, DWORD idx, CBLContext* pCont) { return FALSE; }
	};
	// Функтор для поиска контекста по CRunTimeClass
	struct by_rtc : by_empty
	{
		const CRuntimeClass* pRTC;
		by_rtc(const CRuntimeClass* p) : pRTC(p){}
		BOOL CheckContext(CBLContext* pCont) {return pCont->GetRuntimeClass()->IsDerivedFrom(pRTC); }
	};
	// Функтор для поиска контекста по названию класса в CRunTimeClass
	struct by_rtc_name : by_empty
	{
		const CString& strName;
		by_rtc_name(const CString& s) : strName(s){}
		BOOL CheckContext(CBLContext* pCont) {return 0 == strName.CompareNoCase(pCont->GetRuntimeClass()->m_lpszClassName); }
	};
	// Функтор для поиска контекста по названию базового класса
	// внимание: поиск осуществляется сразу с базовых классов, сам класс на имя не проверяется
	struct by_base_name : by_empty
	{
		const CString& strName;
		by_base_name(const CString& s) : strName(s){}
		BOOL CheckBase(IContextTree* pTree, DWORD idx, CBLContext* pCont)
		{ return 0 == strName.CompareNoCase(pTree->GetBaseObjName(idx)); }
	};
	// Функтор для поиска контекста, реализующего интерфейс, доступный через динамиккаст
	template<typename T>
	struct by_dynamic_cast : by_empty
	{
		T*& pResult;
		by_dynamic_cast(T*& p) : pResult(p){}
		BOOL CheckContext(CBLContext* pCont)
		{
			try{
				pResult = dynamic_cast<T*>(pCont);
			}catch(...){ return FALSE; }
			return pResult != NULL;
		}
	};

	// Собственно, методы приведения

	// Upcast - получить конечный класс.
	static CBLContext* Up(CBLContext* pCont)
	{
		IContextTree* pTree;
		for(;;)
		{
			try{
				if(!(pTree = dynamic_cast<IContextTree*>(pCont)))
					break;
			}catch(...){ break; }
			CBLContext* pDerived = pTree->GetDerivedObj();
			if(!pDerived)
				break;
			pCont = pDerived;
		}
		return pCont;
	}

	// Приведение по CRunTimeClass'у
	static CBLContext* ByRTC(CBLContext* pCont, const CRuntimeClass* pRTC)
	{
		by_rtc temp_for_disable_warning_with_num_504(pRTC);
		return DoCast(pCont, temp_for_disable_warning_with_num_504);
	}
	// Приведение по названию в CRunTimeClass'е
	static CBLContext* ByRTCName(CBLContext* pCont, const CString& strName)
	{
		by_rtc_name temp_for_disable_warning_with_num_504(strName);
		return DoCast(pCont, temp_for_disable_warning_with_num_504);
	}
	// Приведение по имени базового класса
	static CBLContext* ByBaseName(CBLContext* pCont, const CString& strName)
	{
		by_base_name temp_for_disable_warning_with_num_504(strName);
		return DoCast(pCont, temp_for_disable_warning_with_num_504);
	}
	// Приведение через dynamic_cast к заданному типу
	template<typename T>
	static CBLContext* Dynamic(CBLContext* pCont, T*& pResult)
	{
		by_dynamic_cast<T> temp_for_disable_warning_with_num_504(pResult);
		return DoCast(pCont, temp_for_disable_warning_with_num_504);
	}
};


#endif // _CAST_CONTEXT_H__INCLUDED_
