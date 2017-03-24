// DynaValue.cpp: implementation of the CDynaValue class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "addin.h"
#include "DynaValue.h"
#include "dml.h"

#include "DynaValueList.h"

#undef DEBUG_THIS

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

//#define DEBUG_THIS

#define DEBUG_THIS

#ifdef DEBUG_THIS
	static char dbg[512];
	#define DBG OutputDebugString(dbg)
	#define DBG_DYNAVALUE_CONSTRUCTION
//	#define DBG_DYNAVALUE_REFS
//	#define DBG_DYNAVALUE_METHODS
#endif

//#define DYNADEBUG
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDynaValue, CBLContext);

class CObjID CDynaValue::ObjID;

enum {
methAddProp=0,
methAddPropA,
methSelectProps,
methGetProp,
methGetPropByNum,
methGetNProps,
methSaveToString,
methLoadFromString,
methFindByName,
methGetByName,
methSetByName,
methSetByNum,
methDeleteByNum,
methDeleteByName,

methTotalByProps,
methClear,

methProps2ValueList,
methFindByValue,
methFindBySubValue,

methSaveToFile,
methLoadFromFile,

//internal methods
methIntDVLbatch,

lastMethod
};

struct CDynaValue::paramdefs CDynaValue::defFnNames[] = {
	{{"AddProp","ƒобавить—войство"}, 1, 2},
	{{"AddPropA","ƒобавить—войствој"}, 1, 3},
	{{"SelectProps","¬ыбрать—войства"}, 1, 0},
	{{"GetProp","ѕолучить—войство"}, 1, 2},
	{{"GetPropByNum","ѕолучить—войствоѕоЌомеру"}, 1, 1},
	{{"GetNProps"," оличество—войств"}, 1, 0},
	{{"SaveToString","¬ыгрузить¬—троку"}, 1, 0},
	{{"LoadFromString","«агрузить»з—троки"}, 0, 1},
	{{"FindByName","Ќайтиѕо»мени"}, 1, 1},
	{{"GetByName","ѕолучитьѕо»мени"}, 1, 1},
	{{"SetByName","”становитьѕо»мени"}, 0, 2},
	{{"SetByNum","”становитьѕоЌомеру"}, 0, 2},
	{{"DeleteByNum","”далитьѕоЌомеру"}, 0, 1},
	{{"DeleteByName","”далитьѕо»мени"}, 0, 1},
	{{"TotalByProps","»тогѕо—войствам"}, 1, 0},
	{{"Clear","ќчистить"}, 0, 0},
	{{"PropsToValueList","—войства¬—писок«начений"}, 0, 1},
	{{"FindByValue","Ќайтиѕо«начению"}, 1, 2},
	{{"FindByAtributeValue","Ќайтиѕо«начениюјтрибута"}, 1, 3},
	{{"SaveToFile","¬ыгрузить¬‘айл"}, 1, 1},
	{{"LoadFromFile","«агрузить»з‘айла"}, 1, 1},
	//internal methods
	{{"InternalDVLbatch","InternalDVLbatch"}, 1, 2},
};

//dyMemNode CDynaValue::memNode = {
//	0x033540BD,RUNTIME_CLASS(CDynaValue),classname,"RuntimeContextClass",0,0,0,0,0,0};
CDMFeature* CDynaValue::m_fNode = new CDMFeature(CDYNAVALUE_MAGIC,RUNTIME_CLASS(CDynaValue),CDYNAVALUE_NAME,"RuntimeContextClass");

CDynaValue::CDynaValue(int param):CBLContext(param)
{
#ifdef DBG_DYNAVALUE_CONSTRUCTION
	sprintf(dbg,"CDynaValue()%08X",this);
	DBG;
#endif
	nProps = 0;
	firstProp = 0;
	lastProp = 0;
	flags = 0;
	iflags = 0;
	uMem=0;
	
	m_fNode->IncrInst();
	m_fNode->RegAlloc(sizeof(class CDynaValue));
	m_fNode->IncrRef();
}

void CDynaValue::operator delete(void *p)
{
#ifdef _DEBUG
	_free_dbg(p,_CLIENT_BLOCK);
#else
	free(p);
#endif
	m_fNode->UnRegAlloc(sizeof(class CDynaValue));
}

void CDynaValue::ClearAll()
{
	struct propdefs *pd=firstProp,*next;
	unsigned long sz;

	if(iflags & IFLAG_SOLID)
	{
		sz=0;
		while(pd)
		{
			if(pd->val)
			{
				*pd->val = (int)0;
				sz += sizeof(class CValue);
				m_fNode->Free(pd->val);
			}
			pd->val=0;
			pd = pd->next;
		}
		m_fNode->UnRegAlloc(sz);
		m_fNode->Free(firstProp);
	}
	else
	{
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
	}
	firstProp=0;
	lastProp=0;
	nProps=0;
}

CDynaValue::~CDynaValue()
{
#ifdef DBG_DYNAVALUE_CONSTRUCTION
	sprintf(dbg,"~CDynaValue()%08X",this);
	DBG;
#endif
	ClearAll();
	m_fNode->DecrInst();
	m_fNode->DecrRef();
	uMem=0;
}


struct propdefs* findNpd(struct propdefs *pd, CString name)
{
	while(pd)
	{

		if(stricmp(name,pd->Name)==NULL)
		{

			return pd;
		}
		pd=pd->next;
	}
	return 0;
}

int CDynaValue::AddProp(const CString &name, CValue *defVal,unsigned int flags)
{
	void *mem;
	struct propdefs *npd;
	int sz,fsz,realsz;

	if(iflags & IFLAG_SOLID)
	{
		CBLModule::RaiseExtRuntimeError("DynaValue::AddProp() method invalid for solid model!",0);
		return 0;
	}

	sz=name.GetLength();
	if(sz>0)
	{
		if(findNpd(firstProp,name))
		{
			sprintf(buf,"—войство с именем %s уже существует",name);
			CBLModule::RaiseExtRuntimeError(buf,0);
			return 0;
		}
	}

	fsz = sz + sizeof(struct propdefs) + 1;
	realsz = fsz;
	mem = m_fNode->Alloc(realsz);
	if(mem)
	{
		uMem += realsz;
		npd = (struct propdefs*)mem;
		npd->Name = (char*)((unsigned long)mem + sizeof(struct propdefs));
		npd->next=0;
		npd->flags = flags;
		if(sz)memcpy(npd->Name,name,sz);
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

	npd->val->MakeExactValueFrom(defVal);  
	return 1;
}

void CDynaValue::DeleteProp(int iPropNum)
{
	struct propdefs *pd,*last;
	int i;

	if((iflags & IFLAG_SOLID)==0)
	{
		if(nProps >= iPropNum && iPropNum > -1)
		{
			pd=firstProp;
			if(iPropNum == 0)
			{
				firstProp = pd->next;
			}
			else
			{
				i=iPropNum;
				while(i--)
				{
					last=pd;
					pd=pd->next;
				}
				last->next = pd->next;
				if(iPropNum == (nProps-1))
				{
					lastProp = last;
				}
			}
			if(pd->val)
			{
				*pd->val = (int)0;
				m_fNode->Free(pd->val);
				m_fNode->UnRegAlloc(sizeof(class CValue));
			}
			pd->val=0;
			m_fNode->Free(pd);
			nProps--;
			if(nProps == 0)
			{
				lastProp=0;
				firstProp=0;
			}
		}
    else
    {
      CString strErr;
      strErr.Format("¬ ”далитьѕоЌомеру передан неверный номер свойства (%d)",iPropNum);
      CBLModule::RaiseExtRuntimeError(strErr,0);
    }
	}
}

void CDynaValue::Reset(void)
{

//	sprintf(buf,"CDynaValue(0x%08X)::Reset()",this);
//	pBkEndUI->DoMessageLine(buf,mmNone);

	ClearAll();
	flags=0;
	iflags=0;
	m_fNode->UnRegAlloc(uMem);
	uMem=0;
}

int CDynaValue::SelectProps(void)
{
	if(nProps)
	{
		flags |= FLAG_PROPS_SELECTED;
		selected = 0;
		return 1;
	}
	return 0;
}

int CDynaValue::GetSelectedProp(class CValue *rProp, class CValue *rName)
{
	rName->Reset();
	rProp->Reset();
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

int  CDynaValue::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	int rc=1;
	CString *cstr = new CString();
	int i;

	switch (iMethNum) {
//	iMethNum = m_fNode->m_license->CheckExpiration(iMethNum);

	//Internal methods
	case methIntDVLbatch:
		//pBkEndUI->DoMessageLine("InternalGetPropPtr",mmNone);
		if(ppValue[0]->GetNumeric()==0xFE010001)
		{
			struct propdefs *pd = firstProp;
			CBLContext *cont;
			i = ppValue[1]->GetNumeric();
			while(pd)
			{
				if( cont = pd->val->GetContext())
				{
					if(pd->val->GetTypeString() == m_fNode->m_strName)
					{
						cont->CallAsFunc(i,rValue,0);
					}
				}
				pd=pd->next;
			}
			rValue = (int)firstProp;
		}
		else
		{
			rValue = (int)0;
		}
		break;

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
				rValue = (int)-1;
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
			rc = 0;
		}
		break;

	//public methods
	case methAddProp:
	case methAddPropA:
		i = nProps;
    if(iMethNum==methAddPropA)
      int l = ppValue[2]->GetNumeric();

		rc = AddProp(ppValue[0]->GetString(),ppValue[1],iMethNum==methAddPropA ? ppValue[2]->GetNumeric():0);
		if(rc)
		{
			rc = GetPropVal(nProps-1,rValue);
		}
		break;

	case methSelectProps:
		rValue = SelectProps();
		break;

	case methGetProp:
		rValue = GetSelectedProp(ppValue[0],ppValue[1]);
		break;

	case methGetPropByNum:
		rc = GetPropVal(ppValue[0]->GetNumeric(),rValue);
		break;

	case methGetNProps:
		rValue = nProps;
		break;

	case methSaveToString:
		SaveToString(*cstr);
		rValue = *cstr;
		break;

	case methFindByName:
		rValue = FindProp(ppValue[0]->GetString());
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

	case methFindByValue:
		*ppValue[1] = CNumeric(FindByValue(ppValue[0],&rValue));
		break;

	case methFindBySubValue:
		*ppValue[2] = CNumeric(FindBySubValue(ppValue[0]->GetString(),ppValue[1],&rValue));
		break;

	default:
		rc=0;
	};
	delete cstr;
	return rc;
}

int  CDynaValue::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	int rc=1;
	int i;

	switch (iMethNum) {
//	iMethNum = m_fNode->m_license->CheckExpiration(iMethNum);

	case methAddProp:
	case methAddPropA:
		rc = AddProp(ppValue[0]->GetString(),ppValue[1],iMethNum==methAddPropA ? ppValue[2]->GetNumeric():0);
		break;

	case methSelectProps:
		SelectProps();
		break;

	case methGetProp:
		rc = GetSelectedProp(ppValue[0],ppValue[1]);
		break;

	case methGetPropByNum:
		i = ppValue[0]->GetNumeric();
		*ppValue[2] = GetPropName(i,0);
		rc = GetPropVal(i,*ppValue[1]);
		break;

	case methLoadFromString:
		LoadFromString(ppValue[0]->GetString(),0);
		break;

	case methSetByNum:
		SetPropVal(ppValue[0]->GetNumeric(),*ppValue[1]);
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
	
	case methDeleteByNum:
		DeleteProp(ppValue[0]->GetNumeric());
		break;

	case methDeleteByName:
		{
			int i=0;
			struct propdefs *pd = firstProp;
			const char *name = ppValue[0]->GetString();
			while(pd)
			{
				if(stricmp(name,pd->Name)==NULL)
				{
					DeleteProp(i);
					break;
				}
				pd=pd->next;
				i++;
			}
			break;
		}

	case methClear:
		Reset();
		break;

	case methProps2ValueList:
		{
			int meth;
			CBLContext *vl;			
      vl = ppValue[0]->GetContext();
      bool bCreate = true;
      if (vl)
      {
        CString strClassName(vl->GetRuntimeClass()->m_lpszClassName);
        if(strClassName == "CValueListContext")
          bCreate = false;            
      }
      if (bCreate)
      {
        ppValue[0]->Reset();
        ppValue[0]->CreateObject("ValueList");
        vl = ppValue[0]->GetContext();
      }        
			if(vl && nProps)
			{          
				meth = vl->FindMethod("ƒобавить«начение");
				if(meth != -1)
				{
					struct propdefs *pd = firstProp;
					CValue b,*pa[]={0,&b,NULL};
					b.Reset();
					while(pd)
					{
						pa[0] = pd->val;
            *pa[1] = pd->Name;
						i = vl->CallAsProc(meth,pa);
						pd = pd->next;
					}
				}				
			}

			break;
		}

	default:
		return 0;
	};
	return rc;
}


int  CDynaValue::FindMethod(char const * lpMethodName)const
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

char const *  CDynaValue::GetMethodName(int iMethodNum,int iMethodAlias)const
{

	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].Names[iMethodAlias];
	return 0;
}

int  CDynaValue::GetNMethods(void)const
{
	return lastMethod;
}

int  CDynaValue::HasRetVal(int iMethodNum)const
{
	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].HasReturnValue;
	return 0;
}

int CDynaValue::GetNParams(int iMethodNum)const
{
	if ((iMethodNum>=0) && (iMethodNum<lastMethod))
		return defFnNames[iMethodNum].NumberOfParams;
	return 0;
}

int  CDynaValue::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	return 0;
}



void  CDynaValue::DecrRef(void)
{
#ifdef DBG_DYNAVALUE_REFS
	sprintf(dbg,"CDynaValue::DecrRef()%08X refs %d",this,this->m_RefCount-1);
	DBG;
#endif
	m_fNode->DecrRef();
	CBLContext::DecrRef();
}

char const *  CDynaValue::GetCode(void)const
{
	return 0;
}

int  CDynaValue::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CDynaValue::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CDynaValue::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CDynaValue::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long  CDynaValue::GetTypeID(void)const
{
	return 100;
}

char const *  CDynaValue::GetTypeString(void)const
{
	return m_fNode->m_strName;
}

class CType   CDynaValue::GetValueType(void)const
{
	return CType(100);
}


void  CDynaValue::IncrRef(void)
{
	CBLContext::IncrRef();
	m_fNode->IncrRef();
#ifdef DBG_DYNAVALUE_REFS
	sprintf(dbg,"CDynaValue::IncrRef()%08X refs %d",this,this->m_RefCount);
	DBG;
#endif
}

void  CDynaValue::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CDynaValue::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CDynaValue::IsExactValue(void)const
{
	return 0;
}

int  CDynaValue::IsOleContext(void)const
{
	return 0;
}

int  CDynaValue::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CDynaValue::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CDynaValue::IsSerializable(void)
{
	return 0;
}

int  CDynaValue::SaveToString(class CString & csStr)
{
#ifdef DBG_DV_SAVE
	sprintf(dbg,"CDynaValue::SaveToString()%08X %d refs",this,this->m_RefCount);
	DBG;
#endif

#ifdef _DEBUG
	char tsb[32768];
	CString ts(tsb,32768);
#else
	CString ts;
#endif
	struct propdefs *pd = firstProp;
  char size[9] = {"00000000"};  
	int n,nsz;
  char ext[9]  = {"00000000"};
	const char *type;

	unsigned int npr=0;
	while(pd)
	{
		if((pd->flags & IFLAG_RUNTIME)==0) {npr++;}
		pd = pd->next;
	}
	pd = firstProp;
//if(npr != nProps)OutputDebugString("----- striped props -----");
	csStr = "{#dv2";
	if(iflags)
	{
		sprintf(size,"f%07X",iflags);
		csStr += size;
	}
  
	sprintf(size,"%08X",npr);
  //pBkEndUI->DoMessageLine(size,mmInformation);
	csStr += size;
	if(npr)
	{
		while(pd)
		{
			if((pd->flags & IFLAG_RUNTIME)==0)
			{
				if((iflags & IFLAG_NAMELESS)==0)
				{
					nsz=strlen(pd->Name);
					sprintf(size+6,"%02X",nsz);
					csStr += (size+6);
					if(nsz)csStr += pd->Name;
				}
				type = pd->val->GetTypeString();        
				
        if(n = IsDMSContext(type) || (CSetOfHierarchy::GetHierarchy()->IsClassExist(type)&&pd->val->GetContext()->IsSerializable()))
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
				sprintf(size,"%08X",ts.GetLength());
				csStr += size;
				csStr += ts;
			}
			pd=pd->next;
		}
	}
	csStr += "}";
	//delete ts;
	return 1;
}

int CDynaValue::LoadFromString(const char *str, int inum)
{
	int i,n;
	char *p = (char *)str;	
  char size[10] = {"00000000\0"};  
	char ch;
	char name[256];
	int ext,num;
	unsigned int flags;
	CBLContext *cont;
	CValue val,a,*pa[]={&a,NULL,NULL};
	int ver = 0;

	Reset();
	p++;
	if(*p=='#')
	{
		if(p[1]=='d' && p[2]=='v')
		{
			if(p[3]=='2')
			{
				ver=2;
			}
			else
			{
				ver=-1;
			}
			p+=4;
		}
		else
		{
			ver=-1;
		}
	}

	if(ver == -1 || strlen(p)==0)
	{
		CBLModule::RaiseExtRuntimeError("Invalid string",0);
		return 0;
	}

	if(ver == 2)
	{
		if(p[0]=='f')
		{
			memcpy(size,p+1,8);
			sscanf(size,"%07X",&n);
			iflags = n;
			p += 8;
		}
	}

	memcpy(size,p,8);
	sscanf(size,"%08X",&n);
	p += 8;
	i = n;

	if(iflags & IFLAG_SOLID)
	{
	//saved ValueTable
		CBLModule::RaiseExtRuntimeError("Solid model can't be loaded",0);
		return 0;
	}
	else
	{
		while(i--)
		{
			if(*p=='F')
			{
				memcpy(size+1,p+1,7);
				sscanf(size+1,"%07X",&flags);
				p += 8;
			}
			else
			{
				flags=0;
			}
			size[6] = *p++;
			size[7] = *p++;
			sscanf(size+6,"%02X",&n);
			val.Reset();
			if(n)memcpy(name,p,n);
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
        /*int debug_n = cont->FindMethod("LoadFromString");
        CString strErr;        
        strErr.Format("debug_n = %d", debug_n);        
        pBkEndUI->DoMessageLine(strErr,mmInformation);*/
        num = cont->FindMethod("LoadFromString");
        num = num == -1 ? cont->FindMethod("«агрузить»з—троки") : num;
				if(num != -1)
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
	}
	return 1;
}
void  CDynaValue::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CDynaValue::GetNProps(void)const
{
	return nProps;
}

char const *  CDynaValue::GetPropName(int A,int B)const
{
	struct propdefs *pd;

	if(A<nProps)
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
		pBkEndUI->DoMessageLine("propNum out of range",mmRedErr);
	}
	return NULL;
}

CValue* CDynaValue::FindPropVal(int iPropNum)const
{
	struct propdefs *pd;

	if(iPropNum < nProps && iPropNum > -1)
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
		sprintf(buf,"CDynaValue::FindPropVal(%d) iPropNum is out of range (%d properties at all)",iPropNum,nProps);
		pBkEndUI->DoMessageLine(buf,mmRedErr);
	}
	return NULL;
}

int  CDynaValue::GetPropVal(int iPropNum,class CValue & rValue)const
{
	CValue *val;

	if(val=FindPropVal(iPropNum))
	{
		rValue.MakeExactValueFrom(val);
	}
	else return 0;

	return 1;

}

int  CDynaValue::SetPropVal(int iPropNum,class CValue const & vValue)
{
	CValue *val;

	if(val = FindPropVal(iPropNum))
	{
		val->MakeExactValueFrom(&vValue);
		return 1;
	}
	return 0;
}


int  CDynaValue::FindProp(char const * Name)const
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
	return -1;
}

int CDynaValue::TotalByProps(void)
{
	struct propdefs *pd,*pv,*dpd,*d;
	int methNum,nt,i;
	CBLContext *cont;
	CValue rv;
	long double *totals = 0;

	//pBkEndUI->DoMessageLine("CDynaValue::TotalByProps",mmNone);
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

		pv = firstProp;
		while(pv)
		{
			if(cont=pv->val->GetContext())
			{
				methNum = cont->FindMethod("TotalByProps");
				if(methNum>=0)
				{
//					sprintf(buf,"DV %s",pv->Name);
//					pBkEndUI->DoMessageLine(buf,mmNone);
					if(cont->CallAsFunc(methNum,rv,0))
					{
						if(dpd = (struct propdefs*)((int)rv.GetNumeric()))
						{
//							sprintf(buf,"dpd 0x%08X",dpd);
//							pBkEndUI->DoMessageLine(buf,mmNone);
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
//					sprintf(buf,"#DV %s",pv->Name);
//					pBkEndUI->DoMessageLine(buf,mmNone);
				}
			}
			pv=pv->next;
		}
		
		if(totals)
		{
//pBkEndUI->DoMessageLine("T-DV",mmInformation);
			pd=firstProp;
			i=0;
			while(pd)
			{
				if(pd->flags & PDFLAG_TOTAL)
				{
//					sprintf(buf,"%s %f flags 0x%08X",pd->Name,totals[i],pd->flags);
//					pBkEndUI->DoMessageLine(buf,mmNone);
					*pd->val = CNumeric(totals[i]);
					i++;
				}
				pd=pd->next;
			}
			free(totals);
//pBkEndUI->DoMessageLine("#T-DV",mmInformation);
		}
	
	}
	return 1;
}

int CDynaValue::FindByValue(CValue *val,CValue *rValue)
{
	struct propdefs *pd = firstProp;
	int i=0;

	while(pd)
	{
		if(pd->val->operator==(*val))
		{
			*rValue = *pd->val;
			return i;
		}
		pd = pd->next;
		i++;
	}
	rValue->Reset();
	return -1;
}

int CDynaValue::FindBySubValue(const char *name,CValue *val,CValue *rValue)
{
	struct propdefs *pd = firstProp;
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
	rValue->Reset();
	return -1;
}

int CDynaValue::SaveToFile(CFile *file)
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
	file->Write(&iflags,sizeof(UINT));	//internal flags
	file->Write(&npr,sizeof(int));		//number of props
	file->Write(&i,sizeof(int));		//number of values

	if(npr)
	{
		while(pd)
		{
			if((pd->flags & IFLAG_RUNTIME)==0)
			{
				if((iflags & IFLAG_NAMELESS)==0)
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
				}
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
				{
          CBLContext* pContext = pd->val->GetContext();
          if (pContext != NULL && pContext->GetRuntimeClass()->m_lpszClassName == "CComponentClass")
          { // пробуем сохранить классы 1—++
            CString strName = pContext->GetTypeString();

            ch = 4;
            i = strName.GetLength();
            file->Write(&ch,sizeof(char));
					  file->Write(&i,sizeof(int));
            file->Write(strName.operator LPCTSTR(),i);
            i = 0;
            CString strData;
            if (pContext->IsSerializable() != 0)
            {              
              pContext->SaveToString(strData);
              i = strData.GetLength();
            }
            file->Write(&i,sizeof(int));
            if (i > 0)
            {
               file->Write(strData.operator LPCTSTR(),i);
            }
          }
          else //save 1C object
          {
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

			}
			pd=pd->next;
		}
	}
	return 1;
}

int CDynaValue::LoadFromFile(CFile *file)
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
			file->Read(&iflags,sizeof(int));
			file->Read(&nprops,sizeof(int));
			file->Read(&nvals,sizeof(int));

			while(nprops--)
			{
				char *name = 0;
				val.Reset();
				if((iflags & IFLAG_NAMELESS)==0)
				{
					file->Read(&sh,sizeof(short));
					if(sh)
					{
						name = new char[sh];
						file->Read(name,sh);            
					}
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
				else if(ch == 3)
				{//3 - 1C
					file->Read(&i,sizeof(int));
					char *str = new char[i];
					file->Read(str,i);
					val.LoadFromString(str,0);
          delete[] str;
				}
        else if(ch == 4) // класс —++
        {
          i = 0;
          file->Read(&i,sizeof(int));
          char *str = new char[i+1];
          file->Read(str,i);
          str[i] = '\0';
          //pBkEndUI->DoMessageLine(str, mmExclamation);	
          int res = val.CreateObject(str);
          delete[] str;
          i = 0;
          file->Read(&i,sizeof(int));
          if (i > 0)
          {
            str = new char[i+1];
            file->Read(str,i);
            str[i] = '\0';
            //pBkEndUI->DoMessageLine(str, mmExclamation);	
            CBLContext *cont = val.GetContext();
            if (cont != NULL && res != 0)
            {
              int num = cont->FindMethod("LoadFromString");
              num = num == -1 ? cont->FindMethod("«агрузить»з—троки") : num;
				      if(num != -1)
				      {          
					      CValue val = str;
                CValue *masVal[1] = {&val};
					      cont->CallAsProc(num, masVal);
				      }
            }
            delete[] str;
          }

        }
				this->AddProp(name,&val,0);
        delete[] name;
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
	return -1;
}