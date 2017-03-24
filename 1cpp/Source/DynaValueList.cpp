// DynaValueList.cpp: implementation of the CDynaValueList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "addin.h"
#include "DynaValueList.h"
#include "dml.h"

#undef DEBUG_THIS

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

//#define DEBUG_THIS

#ifdef DEBUG_THIS
	static char dbg[512];
	#define DBG OutputDebugString(dbg)
//	#define DBG_DYNAVALUELIST_CONSTRUCTION
//	#define DBG_DVL_REFS
//	#define DBG_DYNAVALUELIST_METHODS
//	#define DBG_DVL_SAVE
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static char* unnamed="(unnamed)";

IMPLEMENT_DYNCREATE(CDynaValueList, CBLContext);

class CObjID CDynaValueList::ObjID;

enum {
methAddProp=0,
methAddPropA,
methAddValue,
methAddDynaValue,
methAddDynaValueAc,
methSaveToString,
methLoadFromString,
methSelectProps,
methGetProp,
methSelectValues,
methGetValue,
methGetPropByNum,
methGetValueByNum,
methGetNProps,
methGetNValues,
methFindByName,
methFindPropByName,
methFindValueByName,
methGetByName,
methSetByName,
methSetByNum,
methGetByNum,
methBuildHash,
methFindByHash,
methClearHash,
methTotalByProps,
methDeleteValueByName,
methDeleteValueByNum,
methDeleteValues,
methClear,
methFindByValue,
methGetNameByNum,

methSaveToFile,
methLoadFromFile,

//internal methods
methDynaDump,

lastMethod
};

struct CDynaValueList::paramdefs CDynaValueList::defFnNames[] = {
	{{"AddProp","ƒобавить—войство"}, 1, 2},
	{{"AddPropA","ƒобавить—войствој"}, 1, 3},
	{{"AddValue","ƒобавить«начение"}, 1, 3},
	{{"AddDynaValue","ƒобавитьƒинамическое«начение"}, 1, 1},
	{{"AddDynaValueAc","ƒобавитьƒинамическое«начениејк"}, 1, 1},
	{{"SaveToString","¬ыгрузить¬—троку"}, 1, 0},
	{{"LoadFromString","«агрузить»з—троки"}, 0, 1},
	{{"SelectProps","¬ыбрать—войства"}, 1, 0},
	{{"GetProp","ѕолучить—войство"}, 1, 2},
	{{"SelectValues","¬ыбрать«начени€"}, 1, 0},
	{{"GetValue","ѕолучить«начение"}, 1, 2},
	{{"GetPropByNum","ѕолучить—войствоѕоЌомеру"}, 1, 1},
	{{"GetValueByNum","ѕолучить«начениеѕоЌомеру"}, 1, 1},
	{{"GetNProps"," оличество—войств"}, 1, 0},
	{{"GetNValues"," оличество«начений"}, 1, 0},
	{{"FindByName","Ќайтиѕо»мени"}, 1, 1},
	{{"FindPropByName","Ќайти—войствоѕо»мени"}, 1, 1},
	{{"FindValueByName","Ќайти«начениеѕо»мени"}, 1, 1},
	{{"GetByName","ѕолучитьѕо»мени"}, 1, 1},
	{{"SetByName","”становитьѕо»мени"}, 0, 2},
	{{"SetByNum","”становитьѕоЌомеру"}, 0, 2},
	{{"GetByNum","ѕолучитьѕоЌомеру"}, 1, 1},

	{{"BuildHash","ѕостроить’эш"}, 1, 1},
	{{"FindByHash","Ќайти’эш"}, 1, 2},
	{{"ClearHash","ќчистить’эш"}, 0, 0},

	{{"TotalByProps","»тогѕо—войствам"}, 1, 0},
	{{"DeleteValueByName","”далить«начениеѕо»мени"}, 0, 1},
	{{"DeleteValueByNum","”далить«начениеѕоЌомеру"}, 0, 1},
	{{"DeleteValues","”далить«начени€"}, 0, 0},
	{{"Clear","ќчистить"}, 0, 0},
	{{"FindByValue","Ќайтиѕо«начению"}, 1, 3},
	{{"GetNameByNum","ѕолучить»м€ѕоЌомеру"}, 1, 1},

	{{"SaveToFile","¬ыгрузить¬‘айл"}, 1, 1},
	{{"LoadFromFile","«агрузить»з‘айла"}, 1, 1},
	//internal methods
	{{"DynaDump","DynaDump"}, 0, 0},

};

CDMFeature* CDynaValueList::m_fNode = new CDMFeature(CDYNAVALUELIST_MAGIC,RUNTIME_CLASS(CDynaValueList),CDYNAVALUELIST_NAME,"RuntimeContextClass");

CDynaValueList::CDynaValueList(int param):CBLContext(param)
{
#ifdef DBG_DYNAVALUELIST_CONSTRUCTION
	sprintf(dbg,"CDynaValueList()%08X",this);
	DBG;
#endif
	nProps=0;
	firstProp=0;
	lastProp=0;
	selected=0;

	nVals=0;
	firstVal=0;
	lastVal=0;
	selectedVal=0;
	uMem=0;
	hash=0;

	m_fNode->IncrInst();
	m_fNode->RegAlloc(sizeof(class CDynaValueList));
	m_fNode->IncrRef();
}

void CDynaValueList::operator delete(void *p)
{
#ifdef _DEBUG
	_free_dbg(p,_CLIENT_BLOCK);
#else
	free(p);
#endif
	m_fNode->UnRegAlloc(sizeof(class CDynaValueList));
}

CDynaValueList::~CDynaValueList()
{
#ifdef DBG_DYNAVALUELIST_CONSTRUCTION
	sprintf(dbg,"~CDynaValueList()%08X",this);
	DBG;
#endif

	DeleteProps();
	DeleteValues();
}


int CDynaValueList::BuildHash(const char *name)
{
	struct propdefs *pd = firstVal;
	CBLContext *cont;
	int i;
	long id,*ph;
	CValue val;

	if(hash) ClearHash();
	if(nVals && pd)
	{
		if(hash = (long*)m_fNode->Alloc(nVals*sizeof(long)))
		{
//sprintf(buf,"---- hash table of CDynaValueList (%d entries)",nVals);
//pBkEndUI->DoMessageLine(buf,mmNone);
			ph = hash;
			while(pd)
			{
				id = 0;
				if(cont = pd->val->GetContext())
				{
					i = cont->FindProp(name);
					if(i >= 0)
					{
						if(cont->GetPropVal(i,val))
						{
							id = val.GetObjID().ObjID;
						}
					}
				}
				*ph++ = id;
				pd = pd->next;
//sprintf(buf,"%08X",id);
//pBkEndUI->DoMessageLine(buf,mmNone);
			}
			return 1;
		}
	}
	return 0;
}

int CDynaValueList::FindByHash(CValue *fValue, CValue *rValue)
{
	long *ph = hash;
	int i = nVals;
	long id;
	struct propdefs *pd = firstVal;

	if(i && ph)
	{
		id = fValue->GetObjID().ObjID;
		while(i--)
		{
			if(id == *ph)
			{
				rValue->MakeExactValueFrom(pd->val);
//sprintf(buf,"FindByHash(%08X)%08X",id,*ph);
//pBkEndUI->DoMessageLine(buf,mmNone);
				return 1;
			}
			pd = pd->next;
			ph++;
		}
	}
	return 0;
}

void CDynaValueList::ClearHash(void)
{
	if(hash)
	{
		m_fNode->Free(hash);
		m_fNode->UnRegAlloc(nVals*sizeof(long));
		hash=0;
	}
}

void CDynaValueList::DeleteValues()
{
	struct propdefs *pd = firstVal, *next;

	ClearHash();

	while(pd)
	{
		if(pd->val)
		{
			*pd->val = (int)0;
			m_fNode->Free(pd->val);
			m_fNode->UnRegAlloc(sizeof(class CValue));
		}
		pd->val=0;
		next=pd->next;
		m_fNode->Free(pd);
		pd=next;
	}
	firstVal = 0;
	lastVal = 0;
	nVals = 0;
	m_fNode->UnRegAlloc(uMem);
	uMem=0;
}

void CDynaValueList::DeleteProps()
{
	struct propdefs *pd = firstProp, *next;

	while(pd)
	{
		if(pd->val)
		{
			*pd->val = (int)0;
			m_fNode->Free(pd->val);
			m_fNode->UnRegAlloc(sizeof(class CValue));
		}
		pd->val=0;
		next=pd->next;
		m_fNode->Free(pd);
		pd=next;
	}
	firstProp=0;
	lastProp=0;
	nProps=0;
	flags=0;
}

void CDynaValueList::Reset(void)
{
	DeleteProps();
	DeleteValues();
}

int CDynaValueList::AddProp(const CString &name, CValue *defVal, unsigned int flags)
{
	void *mem;
	struct propdefs *npd;
	int sz,fsz,realsz;

	sz=name.GetLength();
	if(sz>0)
	{
		npd = findNpd(firstProp,name);
		if(npd==0)
		{
			fsz = sz + sizeof(struct propdefs) + 1;
//			realsz = fsz>256?fsz:256;
			realsz = fsz;
			mem = m_fNode->Alloc(realsz);
			if(mem)
			{
				uMem += realsz;
				npd = (struct propdefs*)mem;
				npd->Name = (char*)((unsigned long)mem + sizeof(struct propdefs));
				npd->next=0;
				npd->flags = flags;
				memcpy(npd->Name,name,sz);
				((byte*)mem)[fsz-1]=0;
				npd->val = new CValue();
				m_fNode->RegAlloc(sizeof(class CValue));
				if(lastProp==0)
				{
					lastProp=npd;
					firstProp=npd;
				}
				else
				{
					lastProp->next = npd;
					lastProp=npd;
				}
				nProps++;
			}
			else
			{
				CBLModule::RaiseExtRuntimeError("out of memory",0);
				return 0;
			}
		}
		else
		{
			sprintf(buf,"—войство с именем %s уже существует",name);
			CBLModule::RaiseExtRuntimeError(buf,0);
			return 0;
		}
		npd->val->MakeExactValueFrom(defVal);
		return 1;
	}
	return 0;
}

int CDynaValueList::AddValue(const CString &name, const CString &valname, CValue *defVal, int prop_flags)
{
	void *mem;
	struct propdefs *pd,*pv;
	int sz,realsz;
	CBLContext *cont;
	CValue a,b,c;
	CValue *pa[]={&a,0,NULL,NULL};
	int meth1,meth2;
	const char *type;

	ClearHash();

	sz=name.GetLength();
	if(sz>0)
	{
		if(pv = findNpd(firstVal,name))
		{
			if(prop_flags & DYNAVALUE_ACCUMULATE)
			{
				lastUsedVal = pv->val;
				return 1;
			}
			else
			{
				sprintf(buf,"«начение с именем '%s' уже существует",name);
				CBLModule::RaiseExtRuntimeError(buf,0);
				return 0;
			}
		}
		realsz = sz + sizeof(struct propdefs) + 1;
	}
	else
	{
		realsz = sizeof(struct propdefs) + 1;
	}

	mem = m_fNode->Alloc(realsz);
	if(mem)
	{
		uMem += realsz;
		pv = (struct propdefs*)mem;
		if(sz)
		{
			pv->Name = (char*)((unsigned long)mem + sizeof(struct propdefs));
			memcpy(pv->Name,name,sz);
			((byte*)mem)[sz+sizeof(struct propdefs)]=0;
		}
		else
		{
			pv->Name = unnamed;
		}
		pv->next=0;
		pv->val = new CValue();
		m_fNode->RegAlloc(sizeof(class CValue));
		pv->val->CreateObject("DynaValue");
		cont = pv->val->GetContext();
		if(prop_flags & DYNAVALUE_PROPOGATE)
		{
			pd=firstProp;
			while(pd)
			{
				type = pd->val->GetTypeString();
				if(IsDMSContext(type))
				{
					b.Reset();
					b.CreateObject(type);
					meth1 = b.GetContext()->FindMethod("LoadFromString");
					meth2 = pd->val->GetContext()->FindMethod("SaveToString");
					pa[1]=NULL;
					
					pd->val->GetContext()->CallAsFunc(meth2,a,pa);
					b.GetContext()->CallAsProc(meth1,pa);
					a = pd->Name;
					pa[1]=&b;
					cont->CallAsProc(0,pa);//AddProp
					if(type == m_fNode->m_strName)
					{
						//pBkEndUI->DoMessageLine("Cloning DynaValueList",mmNone);
						((CDynaValueList*)b.GetContext())->Clone(pd);
					}
				}
				else
				{
					pa[1]=pd->val;
					pa[2]=&c;
					c=(int)(pd->flags&(pd->flags>>1));
					pa[3]=NULL;
					a=pd->Name;
					cont->CallAsProc(1,pa);//AddPropA
				}
				pd=pd->next;
			}
		}
		else if(valname.GetLength())
		{
			a=valname;
			pa[1]=defVal;
			cont->CallAsProc(0,pa);
		}
		if(lastVal==0)
		{
			lastVal=pv;
			firstVal=pv;
		}
		else
		{
			lastVal->next = pv;
			lastVal=pv;
		}
		nVals++;
		lastUsedVal = pv->val;
		return 1;
	}
	else
	{
		CBLModule::RaiseExtRuntimeError("out of memory",0);
	}
	return 0;
}

int CDynaValueList::DeleteValueByName(const CString &name)
{
	struct propdefs *pd = firstVal, **lpd=&firstVal;
	int len = strlen(name)+1;

	while(pd)
	{
		if(strncmp(name,pd->Name,len)==0)
		{
			*lpd = pd->next;
			if(pd->val)
			{
				*pd->val = (int)0;
				m_fNode->Free(pd->val);
				m_fNode->UnRegAlloc(sizeof(class CValue));
			}
			pd->val=0;
			m_fNode->Free(pd);
			nVals--;
			if(pd == lastVal)
			{
				if(pd = firstVal)
				{while(pd->next) pd=pd->next;}
				lastVal = pd;
			}
			return 1;
		}
		lpd=&pd->next;
		pd = pd->next;
	}
	return 0;
}

int CDynaValueList::DeleteValueByNum(int num)
{
	struct propdefs *pd = firstVal, **lpd=&firstVal;
	if(num < nVals)
	{
		while(num--)
		{
			lpd=&pd->next;
			pd = pd->next;
		}
		if(pd)
		{
			*lpd = pd->next;
			if(pd->val)
			{
				*pd->val = (int)0;
				m_fNode->Free(pd->val);
				m_fNode->UnRegAlloc(sizeof(class CValue));
			}
			pd->val=0;
			m_fNode->Free(pd);
			nVals--;
			if(pd == lastVal)
			{
				if(pd = firstVal)
				{while(pd->next) pd=pd->next;}
				lastVal = pd;
			}
			return 1;
		}
	}
	return 0;
}

void CDynaValueList::Clone(struct propdefs *pdef)
{
	CDynaValueList *org;
	struct propdefs *opd,*pd;

	org = (CDynaValueList*)(pdef->val->GetContext());
	if(org->nProps)
	{
		opd=org->firstProp;
		pd = firstProp;
		while(opd)
		{
			if(pd)
			{
				pd->flags = opd->flags;
				pd = pd->next;
			}
			opd=opd->next;
		}
	}

}

int CDynaValueList::SelectProps(void)
{
	if(nProps)
	{
		flags |= FLAG_PROPS_SELECTED;
		selected = 0;
		return 1;
	}
	return 0;
}

int CDynaValueList::GetSelectedProp(class CValue *rProp, class CValue *rName)
{
	rName->Reset();
	rProp->Reset();
//sprintf(buf,"GetSelectedProp %d 0x%08X",selected,flags);
//pBkEndUI->DoMessageLine(buf,mmInformation);
	if(flags & FLAG_PROPS_SELECTED)
	{
		if(nProps == 0)
		{
			return 0;
		}
		GetPropVal(selected,*rProp);
		*rName = GetPropName(selected,0);
		selected++;
		if(selected >= nProps)
		{
			flags ^= FLAG_PROPS_SELECTED;
		}
		return 1;
	}
	return 0;
}

int CDynaValueList::SelectValues(void)
{
	if(nVals)
	{
		flags |= FLAG_VALS_SELECTED;
		selectedVal = 0;
		return 1;
	}
	return 0;
}

int CDynaValueList::GetSelectedValue(class CValue *rProp, class CValue *rName)
{
	rName->Reset();
	rProp->Reset();
	if(flags & FLAG_VALS_SELECTED)
	{
		if(nVals == 0)
		{
			return 0;
		}
		GetPropVal(selectedVal + nProps, *rProp);
		*rName = GetPropName(selectedVal + nProps,0);
		selectedVal++;
		if(selectedVal >= nVals)
		{
			flags ^= FLAG_VALS_SELECTED;
		}
		return 1;
	}
	return 0;
}

int  CDynaValueList::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	int i;
	int rc = 1;
	CString *cstr = new CString();

	switch (iMethNum) {
//	iMethNum = m_fNode->m_license->CheckExpiration(iMethNum);

	case methSaveToFile:
		{
			CFile file;
			CFileException e;
			if(file.Open(ppValue[0]->GetString(),CFile::modeWrite | CFile::modeCreate,&e))
			{
				SaveToFile(&file);
				file.Close();
				rValue = (int)1;
			}
			else
			{
				e.ReportError();
				rValue = (int)0;
			}
			break;
		}

	case methLoadFromFile:
		{
			CFile file;
			CFileException e;
			if(file.Open(ppValue[0]->GetString(),CFile::modeRead,&e))
			{
				int rc = LoadFromFile(&file);
				file.Close();
				rValue = rc;
			}
			else
			{
				e.ReportError();
				rValue = (int)0;
			}
			break;
		}

	case methTotalByProps:
		if(TotalByProps())
		{
			rValue = (int)firstProp;
		}
		else
		{
			rValue = (int)0;
			rc=0;
		}
		break;

	case methBuildHash:
		rc = BuildHash((const char*)ppValue[0]->GetString());
		break;

	case methFindByHash:
		rValue = FindByHash(ppValue[0],ppValue[1]);
		break;

	case methAddProp:
	case methAddPropA:
		rc = AddProp(ppValue[0]->GetString(),ppValue[1],iMethNum==methAddPropA ? ppValue[2]->GetNumeric():0);
		if(rc)
		{
			rc = GetPropVal(nProps-1,rValue);
		}
		break;

	case methAddValue:
		rc = AddValue(ppValue[0]->GetString(),ppValue[1]->GetString(),ppValue[2],0);
		if(rc)
		{
			rValue.MakeExactValueFrom(lastUsedVal);
		}
		break;

	case methAddDynaValue:
	case methAddDynaValueAc:
		rc = AddValue(ppValue[0]->GetString(),"",NULL,DYNAVALUE_PROPOGATE|(iMethNum==methAddDynaValueAc?DYNAVALUE_ACCUMULATE:0));
		if(rc)
		{
			rValue.MakeExactValueFrom(lastUsedVal);
		}
		break;

	case methSaveToString:
		SaveToString(*cstr);
		rValue = *cstr;
		break;

	case methSelectProps:
		rValue = SelectProps();
		break;

	case methGetProp:
		rValue = GetSelectedProp(ppValue[0],ppValue[1]);
		break;

	case methSelectValues:
		rValue = SelectValues();
		break;

	case methGetValue:
		rValue = GetSelectedValue(ppValue[0],ppValue[1]);
		break;

	case methGetPropByNum:
		rc = GetPropVal(ppValue[0]->GetNumeric(),rValue);
		break;

	case methGetValueByNum:
		rc = GetPropVal(int(ppValue[0]->GetNumeric()) + nProps,rValue);
		break;
	case methGetNameByNum:
		{
			int i=(int)ppValue[0]->GetNumeric();
			if(i >= 0 && i<nVals)
			{
				struct propdefs *pd = firstVal;
				while(i--){pd=pd->next;}
				rValue = pd->Name;
			}
			else
			{
				rValue="";
			}
			break;
		}

	case methGetNProps:
		rValue = nProps;
		break;

	case methGetNValues:
		rValue = nVals;
		break;

	case methFindByName:
		rValue = FindProp(ppValue[0]->GetString());
		break;

	case methFindPropByName:
		rValue = FindPropEx(ppValue[0]->GetString(),0);
		break;

	case methFindValueByName:
		rValue = FindPropEx(ppValue[0]->GetString(),1);
		break;

	case methGetByName:
		i = FindProp(ppValue[0]->GetString());
		if(i >= 0)
		{
			GetPropVal(i,rValue);
		}
		else
		{
			rValue.Reset();
			rc=0;
		}
		break;

	case methGetByNum:
		rc = GetPropVal(ppValue[0]->GetNumeric(),rValue);
		break;

	case methFindByValue:
		*ppValue[2] = CNumeric(FindByValue(ppValue[0]->GetString(),ppValue[1],&rValue));
		break;

	default:
		rc=0;
	};
	delete cstr;
	return rc;
}

int  CDynaValueList::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	int rc=1;
	int i;

	switch (iMethNum) {
//	iMethNum = m_fNode->m_license->CheckExpiration(iMethNum);

	case methLoadFromString:
		LoadFromString(ppValue[0]->GetString(),0);
		break;

	case methSelectProps:
		SelectProps();
		break;

	case methSelectValues:
		SelectValues();
		break;

	case methSetByName:
		i = FindProp(ppValue[0]->GetString());
		if(i >= 0)
		{
			SetPropVal(i,*ppValue[1]);
		}
		else
		{
			rc=0;
		}
		break;

	case methSetByNum:
		SetPropVal(ppValue[0]->GetNumeric(),*ppValue[1]);
		break;

	case methClearHash:
		ClearHash();
		break;

	case methDeleteValueByName:
		rc = DeleteValueByName(ppValue[0]->GetString());
		break;

	case methDeleteValueByNum:
		rc = DeleteValueByNum(ppValue[0]->GetNumeric());
		break;

	case methDeleteValues:
		DeleteValues();
		break;

	case methClear:
		Reset();
		break;

	case methDynaDump:
		{
			DumpContent("DynaDump");
			break;
		}
	default:
		return 0;
	};
	return rc;
}


int  CDynaValueList::FindMethod(char const * lpMethodName)const
{
	int i;
	
	for (i = 0;i<lastMethod;i++){
		if (!_stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!_stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	return -1;
}

char const *  CDynaValueList::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].Names[iMethodAlias];
	return 0;
}

int  CDynaValueList::GetNMethods(void)const
{
	return lastMethod;
}

int  CDynaValueList::HasRetVal(int iMethodNum)const
{
	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].HasReturnValue;
	return 0;
}

int CDynaValueList::GetNParams(int iMethodNum)const
{
	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].NumberOfParams;
	return 0;
}

int  CDynaValueList::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	return 0;
}



void  CDynaValueList::DecrRef(void)
{
#ifdef DBG_DVL_REFS
	sprintf(dbg,"CDynaValueList::DecrRef()%08X refs %d",this,this->m_RefCount-1);
	DBG;
#endif
	m_fNode->DecrRef();
	CBLContext::DecrRef();
}



char const *  CDynaValueList::GetCode(void)const
{
	return 0;
}

int  CDynaValueList::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CDynaValueList::GetExactValue(class CValue & vParam)
{

	CBLContext::GetExactValue(vParam);
}

class CObjID   CDynaValueList::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CDynaValueList::GetInternalData(void)
{

	return CBLContext::GetInternalData();

}



long  CDynaValueList::GetTypeID(void)const
{
	return 100;
}

char const *  CDynaValueList::GetTypeString(void)const
{
	return m_fNode->m_strName;
}

class CType   CDynaValueList::GetValueType(void)const
{
	return CType(100);
}


void  CDynaValueList::IncrRef(void)
{
	CBLContext::IncrRef();
	m_fNode->IncrRef();
#ifdef DBG_DVL_REFS
	sprintf(dbg,"CDynaValueList::IncrRef()%08X refs %d",this,this->m_RefCount);
	DBG;
#endif
}

void  CDynaValueList::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CDynaValueList::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CDynaValueList::IsExactValue(void)const
{
	return 0;
}

int  CDynaValueList::IsOleContext(void)const
{
	return 0;
}

int  CDynaValueList::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CDynaValueList::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CDynaValueList::IsSerializable(void)
{
	return 0;
}

int  CDynaValueList::SaveToString(class CString & csStr)
{
#ifdef DBG_DVL_SAVE
	sprintf(dbg,"CDynaValueList::SaveToString()%08X refs %d",this,this->m_RefCount);
	DBG;
#endif

	struct propdefs *pd = firstProp;
#ifdef _DEBUG
	char tsb[32768];
	CString ts(tsb,32768);
#else
	CString ts;
#endif
	char *size = "00000000";
	int n;
	char *ext  = "00000000";
	const char *type;


//pBkEndUI->DoMessageLine("CDynaValue::SaveToString",mmNone);

	csStr = "{";
	sprintf(size,"%08X",nProps);
	csStr += size;
	if(nProps)
	{
		while(pd)
		{
			if(pd->flags)
			{
				sprintf(ext,"F%07X",pd->flags&0x0FFFFFFF);
				csStr += ext;
			}
			sprintf(size+6,"%02X",strlen(pd->Name));
			csStr += (size+6);
			csStr += pd->Name;
			type = pd->val->GetTypeString();
			if(n = IsDMSContext(type))
			{
				sprintf(ext,"8%07X",strlen(type));
				csStr += (ext);
				csStr += type;
				pd->val->GetContext()->SaveToString(ts);
			}
			else
			{
				pd->val->SaveToString(ts);
			}
			pd=pd->next;
			sprintf(size,"%08X",ts.GetLength());
			csStr += size;
			csStr += ts;
		}
	}
	sprintf(size,"%08X",nVals);
	csStr += size;
	if(nVals)
	{
		pd = firstVal;
		while(pd)
		{
			sprintf(size+6,"%02X",strlen(pd->Name));
			csStr += (size+6);
			csStr += pd->Name;
			type = pd->val->GetTypeString();
			if(n = IsDMSContext(type))
			{
				sprintf(ext,"8%07X",strlen(type));
				csStr += (ext);
				csStr += type;
				pd->val->GetContext()->SaveToString(ts);
			}
			else
			{
				pd->val->SaveToString(ts);
			}
			pd=pd->next;
			sprintf(size,"%08X",ts.GetLength());
			csStr += size;
			csStr += ts;
		}
	}
	csStr += "}";

#ifdef DBG_DVL_SAVE
	sprintf(dbg," string closed %d bytes lenght, refs %d",csStr.GetLength(),this->m_RefCount);
	DBG;
#endif

	//delete ts;

#ifdef DBG_DVL_SAVE
	sprintf(dbg," SaveToString() finished");
	DBG;
#endif

	return 1;
}

int CDynaValueList::LoadFromString(const char *str, int inum)
{
	int i,n,sz,fsz,realsz;
	char *p = (char *)str;
	char *size = "00000000\0";
	char ch;
	char name[256];
	int ext,num;
	CBLContext *cont;
	CValue val,a,*pa[]={&a,NULL,NULL};
	struct propdefs *pv;
	void *mem;
	unsigned int flags;

//sprintf(buf,"DynaValueList::LoadFromString this 0x%08X",this);
//pBkEndUI->DoMessageLine(buf,mmNone);
	Reset();
	p++;
	memcpy(size,p,8);
	sscanf(size,"%08X",&n);
	p += 8;
	i = n;
	while(i--)
	{
		if(*p=='F')
		{
			memcpy(size+1,p+1,7);
			sscanf(size+1,"%07X",&flags);
			p += 8;
//sprintf(buf,"DVL flag 0x%08X",flags);
//pBkEndUI->DoMessageLine(buf,mmNone);
		}
		else
		{
			flags=0;
		}
		size[6] = *p++;
		size[7] = *p++;
		sscanf(size+6,"%02X",&n);
		val.Reset();
		memcpy(name,p,n);
		name[n]=0;
		p += n;
		memcpy(size,p,8);
		sscanf(size,"%08X",&n);
		ext = n & 0x80000000;
		n &= 0x7FFFFFFF;
		p += 8;
		ch = p[n];
		p[n]=0;
		if(ext)
		{
			val.CreateObject(p);
			p[n] = ch;
			p += n;
			memcpy(size,p,8);
			sscanf(size,"%08X",&n);
			p += 8;
			ch = p[n];
			p[n]=0;
			cont = val.GetContext();
			if(num = cont->FindMethod("LoadFromString"))
			{
				a = p;
				cont->CallAsProc(num,pa);
			}
		}
		else
		{
			val.LoadFromString(p,0);
		}
		p[n] = ch;
		p += n;
		AddProp(name,&val,flags);
	}
//Values
	memcpy(size,p,8);
	sscanf(size,"%08X",&n);
	p += 8;
	i = n;
//sprintf(buf,"found %d Values",n);
//pBkEndUI->DoMessageLine(buf,mmExclamation);
	while(i--)
	{
		size[6] = *p++;
		size[7] = *p++;
		sscanf(size+6,"%02X",&n);
		val.Reset();
		memcpy(name,p,n);
		name[n]=0;
		p += n;
		memcpy(size,p,8);
		sscanf(size,"%08X",&n);
		ext = n & 0x80000000;
		n &= 0x7FFFFFFF;
		p += 8;
		ch = p[n];
		p[n]=0;
		if(ext)
		{
			sz = strlen(name);
			fsz = sz + sizeof(struct propdefs) + 1;
			realsz = fsz>256?fsz:256;
			mem = m_fNode->Alloc(realsz);
			if(mem)
			{
				uMem += realsz;
				pv = (struct propdefs*)mem;
				pv->Name = (char*)((unsigned long)mem + sizeof(struct propdefs));
				pv->next=0;
				memcpy(pv->Name,name,sz);
				((byte*)mem)[fsz-1]=0;
				pv->val = new CValue();
				m_fNode->RegAlloc(sizeof(class CValue));

				pv->val->CreateObject(p);
				p[n] = ch;
				p += n;
				memcpy(size,p,8);
				sscanf(size,"%08X",&n);
				p += 8;
				ch = p[n];
				p[n]=0;
				cont = pv->val->GetContext();
				if(num = cont->FindMethod("LoadFromString"))
				{
					a = p;
					cont->CallAsProc(num,pa);
				}
				if(lastVal==0)
				{
					lastVal=pv;
					firstVal=pv;
				}
				else
				{
					lastVal->next = pv;
					lastVal=pv;
				}
				nVals++;
			}
			else
			{
				CBLModule::RaiseExtRuntimeError("out of memory",0);
				return 0;
			}
		}
		else
		{
			//val.LoadFromString(p,0);
		}
		p[n] = ch;
		p += n;
	}

	return 1;
}

void  CDynaValueList::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CDynaValueList::GetNProps(void)const
{
	return nProps+nVals;
}

char const *  CDynaValueList::GetPropName(int A,int B)const
{
	struct propdefs *pd;

	if((nProps) && (A<nProps))
	{
		pd=firstProp;
		while(A--)
		{
			pd=pd->next;
		}
		return pd->Name;
	}
	else
	{
		A -= nProps;
		if(A<nVals)
		{
			pd=firstVal;
			while(A--)
			{
				pd=pd->next;
			}
			return pd->Name;
		}
	}
	return NULL;
}

CValue* CDynaValueList::FindPropVal(int iPropNum)const
{
	struct propdefs *pd;

	if(iPropNum < nProps)
	{
		//defValue
		pd=firstProp;
		while(iPropNum--)
		{
			if(pd==0) return 0;
			pd=pd->next;
		}
		return pd->val;
	}
	else
	{
		pBkEndUI->DoMessageLine("CDynaValueList::FindPropVal() iPropNum is out of range",mmRedErr);
	}
	return NULL;
}

CValue* CDynaValueList::FindDynaVal(int iPropNum)const
{
	struct propdefs *pd;

	if(iPropNum < nVals)
	{
		//defValue
		pd=firstVal;
		while(iPropNum--)
		{
			if(pd==0) return 0;
			pd=pd->next;
		}
		return pd->val;
	}
	else
	{
		sprintf(buf,"DynaValueList::FindDynaVal(%d) номер значени€ за пределами допустимого (max %d)",iPropNum,nVals-1);
		pBkEndUI->DoMessageLine(buf,mmRedErr);
	}
	return NULL;
}

int  CDynaValueList::GetPropVal(int iPropNum,class CValue & rValue)const
{
	CValue a[2];
	CValue *pa[]={&a[0],&a[1],NULL};
	CValue *val;

	rValue.Reset();
	if(iPropNum<nProps)
	{
		if(val=FindPropVal(iPropNum))
		{
			rValue.MakeExactValueFrom(val);
		}
		else return 0;
	}
	else
	{
		if(val=FindDynaVal(iPropNum-nProps))
		{
			rValue.MakeExactValueFrom(val);
		}
		else return 0;
	}
	return 1;

}

int  CDynaValueList::SetPropVal(int iPropNum,class CValue const & vValue)
{
	CValue *val;
	const char *type;

	if(iPropNum<nProps)
	{
		if(val=FindPropVal(iPropNum))
		{
			val->MakeExactValueFrom(&vValue);
		}
		else return 0;
	}
	else
	{
		iPropNum -= nProps;
		if(val=FindDynaVal(iPropNum))
		{
			type = vValue.GetTypeString();
			if(strcmp(CDynaValue::m_fNode->m_strName,type))
			{
				sprintf(buf,"ѕопытка присвоить динамическому значению в списке значение типа %s",type);
				pBkEndUI->DoMessageLine(buf,mmRedErr);
				return 0;
			}
			ClearHash();
			val->MakeExactValueFrom(&vValue);
		}
		else return 0;
	}
	return 1;
}

int  CDynaValueList::FindProp(char const * Name)
{
	long i = 0;
	struct propdefs *pd = firstProp;

	while(pd)
	{
		if(stricmp(Name,pd->Name)==NULL)
		{
			return i;
		}
		i++;
		pd=pd->next;
	}
	pd = firstVal;
	while(pd)
	{
		if(stricmp(Name,pd->Name)==NULL)
		{
			return i;
		}
		i++;
		pd=pd->next;
	}

	return -1;
}

int  CDynaValueList::FindPropEx(char const * Name, int mode)
{
	//mode: 0-search in Props; 1-search in Values
	long i = 0;
	struct propdefs *pd = (mode?firstVal:firstProp);

	while(pd)
	{
		if(stricmp(Name,pd->Name)==NULL)
		{
			return i;
		}
		i++;
		pd=pd->next;
	}

	return -1;
}

int CDynaValueList::TotalByProps(void)
{
	struct propdefs *pd,*pv,*dpd,*d;
	int methNum,nt,i;
	CBLContext *cont;
	CValue rv;
	long double *totals = 0;
	
//	pBkEndUI->DoMessageLine("CDynaValueList::TotalByProps",mmNone);
	if(nProps)
	{
		pd=firstProp;
		nt=0;
		while(pd)
		{
			if(pd->flags & PDFLAG_TOTAL)
			{
				nt++;
			}
			pd = pd->next;
		}
		if(nt)
		{
			if(totals = (long double *)malloc(sizeof(long double)*nt))
			{
				for(i=0;i<nt;i++)
				{
					totals[i]=0.0;
				}
			}
		}
	}
	
	if(nVals)
	{
		pv=firstVal;
		while(pv)
		{
			if(cont=pv->val->GetContext())
			{
				methNum = cont->FindMethod("TotalByProps");
				if(methNum>=0)
				{
//					sprintf(buf,"DVL %s",pv->Name);
//					pBkEndUI->DoMessageLine(buf,mmNone);
					if(cont->CallAsFunc(methNum,rv,0))
					{
						if(dpd = (struct propdefs*)((int)rv.GetNumeric()))
						{
							pd=firstProp;
							i=0;
							while(pd)
							{
								if(pd->flags & PDFLAG_TOTAL)
								{
									d=dpd;
									while(d)
									{
										if(strcmp(pd->Name,d->Name)==0)
										{
											if(d->val && totals)
											{
												totals[i] += d->val->GetNumeric().GetDouble();
											}
											break;
										}
										d=d->next;
									}
									i++;
								}
								pd=pd->next;
							}
						}
					}
//					sprintf(buf,"#DVL %s",pv->Name);
//					pBkEndUI->DoMessageLine(buf,mmNone);
				}
			}
			pv=pv->next;
		}
	}//nVals

	if(totals)
	{
//pBkEndUI->DoMessageLine("T-DVL",mmInformation);
		pd=firstProp;
		i=0;
		while(pd)
		{
			if(pd->flags & PDFLAG_TOTAL)
			{
				//sprintf(buf,"%s %f",pd->Name,totals[i]);
				//pBkEndUI->DoMessageLine(buf,mmNone);
				*pd->val = CNumeric(totals[i]);
				i++;
			}
			pd=pd->next;
		}
		//pBkEndUI->DoMessageLine("End of DynaValueList Totals",mmNone);
		free(totals);
//pBkEndUI->DoMessageLine("#T-DVL",mmInformation);
	}

	return 1;
}

int CDynaValueList::FindByValue(const char *name,CValue *val,CValue *rValue)
{
	struct propdefs *pd = firstVal;
	CBLContext *cont;
	int i=0;
	int n;

	while(pd)
	{
		if(cont = pd->val->GetContext())
		{
			if((n = cont->FindProp(name))!=-1)
			{
				if(cont->GetPropVal(n,*rValue))
				{
					if(rValue->operator==(*val))
					{
						*rValue = *pd->val;
						return i;
					}
				}
			}
		}
		pd = pd->next;
		i++;
	}
	*rValue = CNumeric(0);
	return -1;
}

void CDynaValueList::DumpContent(const char *msg)
{
	char b[1024];
	sprintf(b,"CDynaValueList::Dump(%s) %d props %d values",msg,nProps,nVals);OutputDebugString(b);
	int n=0;
	propdefs *pd = firstVal;
	while(pd)
	{
		sprintf(b,"val#%d id<%s> &%08X",n,pd->Name,pd);OutputDebugString(b);
		pd=pd->next;
		n++;
	}
	OutputDebugString("--- end of dump ---");
}

int CDynaValueList::SaveToFile(CFile *file)
{
	int		npr = 0;
	int		i = 0;
	short	sh;
	char	ch;
	const char *type;
	struct propdefs *pd = firstProp;

	while(pd)
	{
		if((pd->flags & IFLAG_RUNTIME)==0) {npr++;}
		pd = pd->next;
	}
	pd = firstProp;

	file->Write("DVB1",4);				//file ID
	file->Write(&i,sizeof(UINT));	//internal flags
	file->Write(&npr,sizeof(int));		//number of props
	file->Write(&nVals,sizeof(int));		//number of values

	if(npr)
	{
		while(pd)
		{
			if((pd->flags & IFLAG_RUNTIME)==0)
			{
				sh = strlen(pd->Name) + 1;
				file->Write(&sh,sizeof(short));
				sh--;
				if(sh)
				{
					file->Write(pd->Name,sh);
				}
				ch = 0;
				file->Write(&ch,sizeof(char));

				type = pd->val->GetTypeString();
				if(strcmp(type,"DynaValueList")==0)
				{//save dvl
					ch = 1;
					file->Write(&ch,sizeof(char));
					CDynaValueList *dv = (CDynaValueList*)pd->val->GetContext();
					dv->SaveToFile(file);
				}
				else if(strcmp(type,"DynaValue")==0)
				{//save dv
					ch = 2;
					file->Write(&ch,sizeof(char));
					CDynaValue *dv = (CDynaValue*)pd->val->GetContext();
					dv->SaveToFile(file);
				}
				else
				{//save 1C object
					ch = 3;
					CString str;
					pd->val->SaveToString(str);
					i = str.GetLength() + 1;
					file->Write(&ch,sizeof(char));
					file->Write(&i,sizeof(int));
					i--;
					if(i)
					{
						file->Write(str.operator LPCTSTR(),i);
						ch = 0;
						file->Write(&ch,sizeof(char));
					}
				}

			}
			pd=pd->next;
		}
	}

	if(nVals)
	{
		pd = firstVal;
		while(pd)
		{
			sh = strlen(pd->Name) + 1;
			file->Write(&sh,sizeof(short));
			sh--;
			if(sh)
			{
				file->Write(pd->Name,sh);
			}
			ch = 0;
			file->Write(&ch,sizeof(char));

			type = pd->val->GetTypeString();
			if(strcmp(type,"DynaValueList")==0)
			{//save dvl
				ch = 1;
				file->Write(&ch,sizeof(char));
				CDynaValueList *dv = (CDynaValueList*)pd->val->GetContext();
				dv->SaveToFile(file);
			}
			else if(strcmp(type,"DynaValue")==0)
			{//save dv
				ch = 2;
				file->Write(&ch,sizeof(char));
				CDynaValue *dv = (CDynaValue*)pd->val->GetContext();
				dv->SaveToFile(file);
			}
			else
			{//save 1C object
				ch = 3;
				CString str;
				pd->val->SaveToString(str);
				i = str.GetLength() + 1;
				file->Write(&ch,sizeof(char));
				file->Write(&i,sizeof(int));
				i--;
				if(i)
				{
					file->Write(str.operator LPCTSTR(),i);
					ch = 0;
					file->Write(&ch,sizeof(char));
				}
			}
			pd = pd->next;
		}
	}

	return 1;
}

int CDynaValueList::LoadFromFile(CFile *file)
{
	int		nprops = 0;
	int		nvals;
	int		i = 0;
	short	sh;
	char	ch;
	UINT	id;
	CValue	val;

	Reset();
	try
	{
		file->Read(&id,4);
		if(id == (('1'<<24)|('B'<<16)|('V'<<8)|'D'))
		{
			file->Read(&i,sizeof(int));
			file->Read(&nprops,sizeof(int));
			file->Read(&nvals,sizeof(int));

			while(nprops--)
			{
				char *name = 0;
				val.Reset();
				file->Read(&sh,sizeof(short));
				if(sh)
				{
					name = new char[sh];
					file->Read(name,sh);
				}
				file->Read(&ch,sizeof(char));
				if(ch == 1)
				{//dvl
					val.CreateObject("DynaValueList");
					CDynaValueList *dv = (CDynaValueList*)val.GetContext();
					dv->LoadFromFile(file);
				}
				else if(ch == 2)
				{//dv
					val.CreateObject("DynaValue");
					CDynaValue *dv = (CDynaValue*)val.GetContext();
					dv->LoadFromFile(file);
				}
				else
				{//3 - 1C
					file->Read(&i,sizeof(int));
					char *str = new char[i];
					file->Read(str,i);
					val.LoadFromString(str,0);
				}
				this->AddProp(name,&val,0);
			}

			while(nvals--)
			{
				char *mem;
				file->Read(&sh,sizeof(short));
				mem = (char*)malloc(sizeof(struct propdefs)+sh);
				struct propdefs *pv = (struct propdefs*)mem;
				pv->Name = (char*)mem + sizeof(struct propdefs);
				pv->flags = 0;
				pv->val = new CValue();

				if(sh)
				{
					file->Read(pv->Name,sh);
				}

				file->Read(&ch,sizeof(char));
				if(ch == 1)
				{//dvl
					pv->val->CreateObject("DynaValueList");
					CDynaValueList *dv = (CDynaValueList*)val.GetContext();
					dv->LoadFromFile(file);
				}
				else if(ch == 2)
				{//dv
					pv->val->CreateObject("DynaValue");
					CDynaValue *dv = (CDynaValue*)val.GetContext();
					dv->LoadFromFile(file);
				}
				else
				{//3 - 1C
					file->Read(&i,sizeof(int));
					char *str = new char[i];
					file->Read(str,i);
					pv->val->LoadFromString(str,0);
				}
				//добавление value
				if(lastVal==0)
				{
					lastVal=pv;
					firstVal=pv;
				}
				else
				{
					lastVal->next = pv;
					lastVal=pv;
				}
				nVals++;
				lastUsedVal = pv->val;
			}
			
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch(CFileException *e)
	{
		e->ReportError();
		e->Delete();
		return -1;
	}
	return 0;
}