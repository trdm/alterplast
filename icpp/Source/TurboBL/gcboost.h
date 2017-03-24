// gcboost.h
#ifndef GCBOOST_H
#define GCBOOST_H
#include "../blmap.h"
#include "../System/BatchGuard.h"

class CGCBoost:public CBLContext
{
private:
	typedef int		(CGCBoost::*PF_I_IV)(int,CValue&);			// GetPropVal,SetPropVal
	typedef int		(CGCBoost::*PF_I_I) (int);					// IsPropReadable,IsPropWritable,GetNParams,HasRetVal
	typedef int		(CGCBoost::*PF_I_IIV)(int,int,CValue *);	// GetParamDefValue
	typedef int		(CGCBoost::*PF_CAP)(int,CValue **);			// CallAsProc
	typedef int		(CGCBoost::*PF_CAF)(int,CValue&,CValue**);	// CallAsFunc
	typedef char const*(CGCBoost::*PF_C_II)(int,int);		// GetPropName,GetMethodName

	struct SMethInfo
	{
		int HasRetVal;
		int NumberOfParam;
	};

	class BatchGuard: CExecBatchHelper
	{
	public:
		BatchGuard(CGCBoost* pCont):
			CExecBatchHelper(pCont->GetDoc7(), FALSE, pCont->GetNeedBatch())
		{}
	};

	struct SGCData{
		// TODO: масштабируемая реализация
		enum {
			nCallMethodsAsEvents = 0,
			nAddContext,
			nSpecialMeth //= 2
		};

		SGCData(CBLModule7* pMod,CBLContext* pCont,BOOL optimize)
		{
			m_nMethInCtx		= pCont->GetNMethods();
			int nMethInModule	= pMod->GetNProcs()-1;
			m_nMethAll			= m_nMethInCtx + nSpecialMeth + nMethInModule;
			m_MethInfo			= m_nMethAll ? new SMethInfo[m_nMethAll] : NULL;

			int nCounter=0;
			for(int j=0; j<m_nMethInCtx; j++)
			{
				AddMethod(nCounter, pCont->GetMethodName(j, 0), pCont->GetMethodName(j, 1),
					pCont->GetNParams(j), pCont->HasRetVal(j));
			}
			AddMethod(nCounter, "__ВызыватьМетодыКакСобытия", "__CallMethodsAsEvents", 1, 1);
			AddMethod(nCounter, "__ДобавитьКонтекст", "__AddContext", 1, 0);
			for(int j=1; j<=nMethInModule; j++)
			{
				CBLProcInfo info;
				pMod->GetProcInfo(j, info);
				AddMethod(nCounter, info.GetName(), NULL, info.GetNParams(), info.HasRetVal());
			}
			m_MethMap.Optimize();

			// Строим карту свойств
			m_nPropInCtx = pCont->GetNProps();
			int i;
			for(i=0; i<m_nPropInCtx; i++)
			{
				m_PropMap.InsertKey(pCont->GetPropName(i, 0), i);
				m_PropMap.InsertKey(pCont->GetPropName(i, 1), i);
			}
			int nModuleProps = pMod->GetNStaticVars();
			m_nPropAll = m_nPropInCtx + nModuleProps;
			for(int j=0; j<nModuleProps; j++)
			{
				CBLVarInfo info;
				pMod->GetStaticVarDescr(j, info);
				m_PropMap.InsertKey(info.GetName(), i);
				i++;
			}
			m_PropMap.Optimize();
			m_Destroy=!optimize;
			if(optimize)
				m_DataMap[pMod->GetFullName()]=this;
		}

		~SGCData(){delete[] m_MethInfo;}

		void Remove()
		{
			if(m_Destroy)
				delete this;
		}

		void AddMethod(int& MethodCounter, PCSTR pcszNameRus, PCSTR pcszNameEng, int NumberOfParam, int HasRetVal)
		{
			m_MethMap.InsertKey(pcszNameRus, MethodCounter);
			if (pcszNameEng)
				m_MethMap.InsertKey(pcszNameEng, MethodCounter);
			m_MethInfo[MethodCounter].NumberOfParam=NumberOfParam;
			m_MethInfo[MethodCounter].HasRetVal=HasRetVal;
			MethodCounter++;
		}

		static SGCData* GetData(CBLModule7* pMod,CBLContext* pCont,BOOL optimize)
		{
			SGCData* pRet=NULL;
			if(optimize)
				m_DataMap.Lookup(pMod->GetFullName(), (void*&)pRet);
			if(!pRet)
				pRet=new SGCData(pMod,pCont,optimize);
			return pRet;
		}

		static SGCData* GetDataByModule(CBLModule7* pMod)
		{
			return static_cast<SGCData*>(m_DataMap[pMod->GetFullName()]);
		}

		static SGCData* Create(CBLModule7* pMod,CBLContext* pCont,BOOL optimize)
		{
			return new SGCData(pMod,pCont,optimize);
		}

		int GetNProps()
		{
			return m_nPropAll;
		}

		int	FindProp(char const*name)const
		{
			return m_PropMap.GetKey(name);
		}

		char const * GetPropName(const CGCBoost* pCont, DWORD dwPropNum, DWORD dwAlias)const
		{
			if(dwPropNum < m_nPropInCtx)
				return (((CGCBoost*)pCont)->*(*(PF_C_II*)&m_pRealVTable[20]))(dwPropNum, dwAlias);

			if(dwAlias) return NULL;

			CBLVarInfo info;
			pCont->GetModule()->GetStaticVarDescr(dwPropNum - m_nPropInCtx, info);
			return info.GetName();
		}

		int GetPropVal(const CGCBoost* pCont, DWORD dwPropNum, CValue &val)
		{
			if(dwPropNum < m_nPropInCtx)
				return (((CGCBoost*)pCont)->*(*(PF_I_IV*)&m_pRealVTable[21]))(dwPropNum, val);

			return pCont->GetModule()->GetStaticVarValue(dwPropNum-m_nPropInCtx, val, 0);
		}

		int SetPropVal(CGCBoost* pCont,DWORD dwPropNum, CValue const &val)
		{
			if(dwPropNum < m_nPropInCtx)
				return (pCont->*(*(PF_I_IV*)&m_pRealVTable[22]))(dwPropNum, const_cast<CValue&>(val));

			return pCont->GetModule()->SetStaticVarValue(dwPropNum-m_nPropInCtx,val,0);
		}

		int IsPropReadable(const CGCBoost* pCont,DWORD dwPropNum)
		{
			if(dwPropNum < m_nPropInCtx)
				return (((CGCBoost*)pCont)->*(*(PF_I_I*)&m_pRealVTable[23]))(dwPropNum);

			return 1;
		}

		int IsPropWritable(const CGCBoost* pCont,DWORD dwPropNum)
		{
			if(dwPropNum < m_nPropInCtx)
				return (((CGCBoost*)pCont)->*(*(PF_I_I*)&m_pRealVTable[24]))(dwPropNum);

			return 1;
		}

		int FindMethod(char const *name)
		{
			return m_MethMap.GetKey(name);
		}

		int GetNMethods()
		{
			return m_nMethAll;
		}

		int GetNParams(int iMethNum)
		{
			return m_MethInfo[iMethNum].NumberOfParam;
		}

		int HasRetVal(int iMethNum)
		{
			return m_MethInfo[iMethNum].HasRetVal;
		}

		// TODO: масштабируемая реализация
		bool IsSpecialMethod(int iMethNum) const
		{
			return (iMethNum >= m_nMethInCtx) && (iMethNum < m_nMethInCtx + nSpecialMeth) ;
		}

		int SpecialMethodNumber(int iMethNum) const
		{
			assert(IsSpecialMethod(iMethNum));
			return iMethNum - m_nMethInCtx;
		}

		bool IsContextMethod(int iMethNum) const
		{
			return (iMethNum < m_nMethInCtx);
		}

		int ModuleMethodNumber(int iMethNum) const
		{
			assert(!IsContextMethod(iMethNum) && !IsSpecialMethod(iMethNum));
			return iMethNum - (nSpecialMeth + m_nMethInCtx) + 1;
		}

		// TODO: поддержка специальных методов
		char const * GetMethodName(const CGCBoost* pCont, int iMethNum, DWORD dwAlias)const
		{
			if (IsSpecialMethod(iMethNum)) return NULL;

			if (IsContextMethod(iMethNum))
				return (((CGCBoost*)pCont)->*(*(PF_C_II*)&m_pRealVTable[27]))(iMethNum, dwAlias);

			if(dwAlias) return NULL;

			CBLProcInfo info;
			pCont->GetModule()->GetProcInfo(ModuleMethodNumber(iMethNum), info);
			return info.GetName();
		}

		int GetParamDefValue(const CGCBoost* pCont, int iMethNum, int iParamNum, CValue *pVal)
		{
			if (IsContextMethod(iMethNum))
				return (((CGCBoost*)pCont)->*(*(PF_I_IIV*)&m_pRealVTable[29]))(iMethNum, iParamNum, pVal);
			if (IsSpecialMethod(iMethNum))
			{
				if (nCallMethodsAsEvents == SpecialMethodNumber(iMethNum))
				if (0 == iParamNum){
					pVal = 0L;
					return TRUE;
				}
					return FALSE;
			}

			CCompiledModule* pCompiledModule = pCont->GetModule()->GetInternalData()->pCompiledModule;
			CBLModuleProcVarDescr* pVar = pCompiledModule->ProcArray[ModuleMethodNumber(iMethNum)]->VarList[iParamNum];
			if(pVar->Type2 >= 0)
			{
				if(pVal)
					*pVal = pCompiledModule->ConstArray[pVar->Type2]->val;
				return TRUE;
			}
			return FALSE;
		}

		int CallAsProc(CGCBoost* pCont,int iMethNum,CValue **ppParam)
		{
			if (IsSpecialMethod(iMethNum))
			{
				if (nAddContext == SpecialMethodNumber(iMethNum))
				{
				CBLContext* pNewCont = ppParam[0]->GetContext();
				if (pNewCont)
				{
					CGroupContext* pGroupCont = pCont->GetDoc7()->GetGroupContext();
					for (int i = 0; i < pGroupCont->GetSize(); ++i)
					{
						CBLContext const* pNext = pGroupCont->GetContext(i);
						if (pNext == pNewCont)
							RuntimeError("Контекст уже добавлен!");
					}

					pGroupCont->AddItem(pNewCont);
					pCont->GetInfo()->m_AddedContexts.push_back(CBLPtr<CBLContext>(pNewCont));
					pGroupCont->Accept();
					return TRUE;
				}
				else
					RuntimeError("Неверный параметр (ожидается контекст)");
					return FALSE;
			}
				return FALSE;
			}

			if (IsContextMethod(iMethNum))
				return (pCont->*(*(PF_CAP*)&m_pRealVTable[31]))(iMethNum, ppParam);

			BatchGuard bg(pCont);
			return pCont->GetModule()->CallAsProc(ModuleMethodNumber(iMethNum), m_MethInfo[iMethNum].NumberOfParam, ppParam);
		}

		int CallAsFunc(CGCBoost* pCont, int iMethNum, CValue& retVal, CValue **ppParam)
		{
			if (IsSpecialMethod(iMethNum))
			{
				if (nCallMethodsAsEvents == SpecialMethodNumber(iMethNum)) {
				retVal = pCont->GetNeedBatch();

				CValue *pNumericValue = ppParam[0];
				if (UNDEFINE_TYPE_1C == pNumericValue->GetTypeCode())
					return FALSE;

				pCont->SetNeedBatch(long(pNumericValue->GetNumeric()) ? TRUE : FALSE);
				return TRUE;
			}
				return FALSE;
			}

			if(IsContextMethod(iMethNum))
				return (pCont->*(*(PF_CAF*)&m_pRealVTable[32]))(iMethNum, retVal, ppParam);

			// если передавать в следующий вызов CallAsFunc непосредственно retVal, возникает ошибка вложенного вызова -
			// юнит-тест "TurboBLТесты::ТестВызовВложенногоМетодаКонтекста"
			CValue locVal;
			BatchGuard bg(pCont);
			int ret=pCont->GetModule()->CallAsFunc(ModuleMethodNumber(iMethNum), locVal, m_MethInfo[iMethNum].NumberOfParam, ppParam);
			retVal=locVal;
			return ret;
		}

		BOOL	m_Destroy;
		CBLMap	m_MethMap;
		int		m_nMethInCtx;
		int		m_nMethAll;
		CBLMap	m_PropMap;
		DWORD	m_nPropInCtx;
		DWORD	m_nPropAll;
		SMethInfo* m_MethInfo;
		static CMapStringToPtr m_DataMap;
	};
	struct SGCInfo
	{
		SGCInfo(CBLModule* pMod, SGCData* pData, CGetDoc7* pDoc=NULL):
			m_pMod(pMod), m_pData(pData), m_pDoc(pDoc), m_bNeedBatch(TRUE) {}
		~SGCInfo(){m_pData->Remove();}
		CBLModule*	m_pMod;				// Ук-ль на модуль
		CGetDoc7*	m_pDoc;
		SGCData*	m_pData;
		BOOL		m_bNeedBatch;
		std::vector<CBLPtr<CBLContext> > m_AddedContexts;
	};
public:
	friend struct SGCData;
	friend class BatchGuard;

	virtual  ~CGCBoost(void);												//1
	virtual int				GetNProps(void)const;							//18 // artbear
	virtual int				FindProp(char const *)const;					//19
	virtual char const *	GetPropName(int,int)const;						//20 // artbear
	virtual int				GetPropVal(int,CValue &)const;			//21
	virtual int				SetPropVal(int,CValue const &);			//22
	virtual int				IsPropReadable(int)const;						//23
	virtual int				IsPropWritable(int)const;						//24
	virtual int				GetNMethods(void)const;							//25 // artbear
	virtual int				FindMethod(char const *)const;					//26
	virtual char const *	GetMethodName(int,int)const;					//27 // artbear
	virtual int				GetNParams(int)const;							//28
	virtual int				GetParamDefValue(int,int,CValue *)const;	//29
	virtual int				HasRetVal(int)const;							//30
	virtual int				CallAsProc(int,CValue * *); 				//31
	virtual int				CallAsFunc(int,CValue &,CValue * *);//32

	static void Init();
	static void Done();

	// обновление данных контекста будет использоваться, например, при
	// динамическом добавлении атрибутов на форму
	static void RefreshGroupContextData(CGroupContext* pGroupContext);

private:
	static BOOL Boost(CBLModule7 *pMod,BOOL optimize);
	int AssignAndCompile(CBLModule7*);
	int WrapCompile();
	void GroupDestruct();
	int Accept();
	void HashMethods(int);
	void HashProps(int);
	int FindMethod1(LPCSTR) const;
	int FindProp1(LPCSTR) const;

	// обозначу тот факт, что первое обращение к компилированному модулю и соответственно, к контексту
	// здесь уже можно использовать этот контекст и обращаться к нему
	static BOOL FirstBoost(CBLModule7 *pMod,BOOL optimize);

private:
	SGCInfo*	GetInfo()	const	{return (SGCInfo*)m_FlagAutoDestroy;}
	CBLModule*	GetModule()	const	{return GetInfo()->m_pMod;}
	SGCData*	GetData()	const	{return GetInfo()->m_pData;}
	CGetDoc7*	GetDoc7()	const	{return GetInfo()->m_pDoc;}
	BOOL		GetNeedBatch()	const	{return GetInfo()->m_bNeedBatch;}
	void		SetNeedBatch(BOOL bNeedBatch)	{GetInfo()->m_bNeedBatch = bNeedBatch;}

	static DWORD* m_pMyVTable;
	static DWORD* m_pRealVTable;

public:
	static CBLModule* GetModuleByContext(const CGroupContext* pCont);
};

#endif