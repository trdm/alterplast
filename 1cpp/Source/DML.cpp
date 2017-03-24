// DML.cpp: implementation of licensing system
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "dml.h"
#include "_protected_string_table.h"

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
//	#define DBG_LICENSE_NODE
//	#define DBG_LICENSE_LIST
//	#define DBG_MODULEKEY_NODE
//	#define DBG_MODULEKEY_LIST
//	#define DBG_CRYPTKEY_NODE
//	#define DBG_CRYPTKEY_LIST
//	#define DBG_FEATURE_NODE
//	#define DBG_FEATURE_NODE_EX
	#define DBG_FEATURE_LIST
//	#define DBG_EXTMODULE_NODE
//	#define DBG_EXTMODULE_LIST
#endif

CDMLicense::CDMLicense(char *name,char *user, ULONG start, ULONG expire) : CXTNode(m_strName)
{
#ifdef DBG_LICENSE_NODE
	sprintf(dbg,"CDMLicense(%s,%s,%08X,%08X)&%08X",name,user,start,expire,this);
	DBG;
#endif

	int l = 0;
	if(name)
	{
		l = strlen(name);
		if(l > 255) {l=255;}
		memcpy(m_strName,name,l);
	}
	m_strName[l]=0;

	l = 0;
	if(user)
	{
		l = strlen(user);
		if(l > 255) {l=255;}
		memcpy(m_strUser,user,l);
	}
	m_strUser[l]=0;

	m_dateStart = start;
	m_dateExpire = expire;

}

CDMLicense::~CDMLicense()
{
#ifdef DBG_LICENSE_NODE
	sprintf(dbg,"~CDMLicense &%08X name <%s>",this,m_strName);
	DBG;
#endif
}

int CDMLicense::CheckExpiration(int i)
{
	SYSTEMTIME st;
	unsigned int check;

	GetSystemTime(&st);
	check = st.wDay + (st.wMonth<<5) + ((st.wYear-2000)<<9);
	if(m_dateExpire > check)
	{
		return i;
	}
	if(m_dateExpire)
	{
		m_dateExpire=0;
		sprintf(buf,pst[PST_EXPIRATION_MB],m_strName,m_strUser);
		MessageBox(NULL,buf,pst[PST_EXPIRATION_CAPTION],MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
	}
	return -1;

}
////////////////////////////////////////////////////////////////
// CDMLicenseList
CDMLicenseList::CDMLicenseList()
{
#ifdef DBG_LICENSE_LIST
	sprintf(dbg,"CDMLicenseList()&%08X",this);
	DBG;
#endif
}

CDMLicenseList::~CDMLicenseList()
{
#ifdef DBG_LICENSE_LIST
	sprintf(dbg,"~CDMLicenseList &%08X",this);
	DBG;
#endif
	CDMLicense *node;
	while(node = (CDMLicense*)RemoveTail())
	{
		delete node;
	}
}

////////////////////////////////////////////////////////////////
// Module key node
////////////////////////////////////////////////////////////////
CDMModuleKey::CDMModuleKey(class CDMLicense *license, char *name, unsigned char *key) : CXTNode(m_strName)
{
#ifdef DBG_MODULEKEY_NODE
	sprintf(dbg,"CDMModuleKey(%s,%s)&%08X",license->m_Name,name,this);
	DBG;
#endif

	int l = 0;
	if(name)
	{
		l = strlen(name);
		if(l > 31) {l=31;}
		memcpy(m_strName,name,l);
	}
	m_strName[l]=0;
	m_license = license;
	if(key)
	{
		memcpy(m_key,key,32);
	}

}

CDMModuleKey::~CDMModuleKey()
{
#ifdef DBG_MODULEKEY_NODE
	sprintf(dbg,"~CDMModuleKey &%08X name <%s>",this,m_strName);
	DBG;
#endif
}

////////////////////////////////////////////////////////////////
// Module key list
////////////////////////////////////////////////////////////////
CDMModuleKeyList::CDMModuleKeyList()
{
#ifdef DBG_MODULEKEY_LIST
	sprintf(dbg,"CDMModuleKeyList()&%08X",this);
	DBG;
#endif
}

CDMModuleKeyList::~CDMModuleKeyList()
{
#ifdef DBG_MODULEKEY_LIST
	sprintf(dbg,"~CDMModuleKeyList &%08X",this);
	DBG;
#endif
	CDMModuleKey *node;
	while(node = (CDMModuleKey*)RemoveTail())
	{
		delete node;
	}
}


////////////////////////////////////////////////////////////////
// Crypt key node
////////////////////////////////////////////////////////////////
CDMCryptKey::CDMCryptKey(class CDMLicense *license, char *name, unsigned char *key) : CXTNode(m_strName)
{
#ifdef DBG_CRYPTKEY_NODE
	sprintf(dbg,"CDMCryptKey(%s,%s)&%08X",license->m_Name,name,this);
	DBG;
#endif

	int l = 0;
	if(name)
	{
		l = strlen(name);
		if(l > 31) {l=31;}
		memcpy(m_strName,name,l);
	}
	m_strName[l]=0;

	if(key)
	{
		memcpy(m_key,key,8);
	}

}

CDMCryptKey::~CDMCryptKey()
{
#ifdef DBG_CRYPTKEY_NODE
	sprintf(dbg,"~CDMCryptKey &%08X name <%s>",this,m_strName);
	DBG;
#endif
}

////////////////////////////////////////////////////////////////
// Crypt key list
////////////////////////////////////////////////////////////////
CDMCryptKeyList::CDMCryptKeyList()
{
#ifdef DBG_CRYPTKEY_LIST
	sprintf(dbg,"CDMCryptKeyList()&%08X",this);
	DBG;
#endif
}

CDMCryptKeyList::~CDMCryptKeyList()
{
#ifdef DBG_CRYPTKEY_LIST
	sprintf(dbg,"~CDMCryptKeyList &%08X",this);
	DBG;
#endif
	CDMCryptKey *node;
	while(node = (CDMCryptKey*)RemoveTail())
	{
		delete node;
	}
}

////////////////////////////////////////////////////////////////
// Feature node
////////////////////////////////////////////////////////////////
CDMFeature::CDMFeature(UINT id,  CRuntimeClass *rclass, char *name, const char *desc) : CXTNode(m_strName)
{
#ifdef DBG_FEATURE_NODE
	sprintf(dbg,"CDMFeature(id %08X, name <%s>, desc <%s>)&%08X",id,name,desc,this);
	DBG;
#endif

	int l = 0;
	if(name)
	{
		l = strlen(name);
		if(l > 31) {l=31;}
		memcpy(m_strName,name,l);
	}
	m_strName[l]=0;

	m_desc = desc;
	m_rclass = rclass;
	m_id = id;
	m_license = 0;
	m_state = 0;

	m_usedMem = 0;
	m_nInst = 0;
	m_maxMem = 0;
	m_maxInst = 0;
	m_nRefs = 0;

	cFeatureList.AddTail(this);
	if(id == 0 && rclass)
	{
#ifdef DBG_FEATURE_NODE
	sprintf(dbg,"regisring class %s",name);
	DBG;
#endif
		CBLContext::RegisterContextClass(rclass, name, CType(100));
		m_state = 1;
	}
}

CDMFeature::~CDMFeature()
{
#ifdef DBG_FEATURE_NODE
	sprintf(dbg,"~CDMFeature &%08X name <%s>",this,m_strName);
	DBG;
#endif
}

void CDMFeature::IncrInst()
{
	m_nInst++;
	if(m_nInst > m_maxInst) { m_maxInst = m_nInst;}
}

void CDMFeature::DecrInst()
{
	m_nInst--;
}

void CDMFeature::IncrRef()
{
	m_nRefs++;
}

void CDMFeature::DecrRef()
{
#ifdef DBG_FEATURE_NODE_EX
	sprintf(dbg,"* object DecrRef() %08X",this);
	DBG;
#endif
	m_nRefs--;
}

void CDMFeature::RegAlloc(UINT size)
{
	m_usedMem += size;
	if(m_usedMem > m_maxMem) {m_maxMem = m_usedMem;}
}

void CDMFeature::UnRegAlloc(UINT size)
{
	m_usedMem -= size;
}

void* CDMFeature::Alloc(UINT size)
{
	void *mem = malloc(size);
	if(mem) {RegAlloc(size);}
	return mem;
}

void CDMFeature::Free(void *mem)
{
	free(mem);
}

////////////////////////////////////////////////////////////////
// Feature list
////////////////////////////////////////////////////////////////
CDMFeatureList::CDMFeatureList()
{
#ifdef DBG_FEATURE_LIST
	sprintf(dbg,"CDMFeatureList()&%08X",this);
	DBG;
#endif
}

CDMFeatureList::~CDMFeatureList()
{
#ifdef DBG_FEATURE_LIST
	sprintf(dbg,"~CDMFeatureList &%08X",this);
	DBG;
#endif
	CDMFeature *node;
	while(node = (CDMFeature*)RemoveTail())
	{
		delete node;
	}
}

void CDMFeatureList::Enable(UINT id, char *name, CDMLicense *pLic)
{
#ifdef DBG_FEATURE_LIST
	sprintf(dbg,"CDMFeatureList::Enable(%08X,%s)",id,name);
	DBG;
#endif
	CDMFeature *node = (CDMFeature*)GetHead();
	while(node)
	{
		if(node->m_id == id)
		{
			if(node->m_state == 0 && node->m_rclass)
			{
				strcpy(node->m_strName,name);
				CBLContext::RegisterContextClass(node->m_rclass,node->m_strName,CType(100));
				node->m_state = 1;
				node->m_license = pLic;
#ifdef DBG_FEAUTRE_LIST
	sprintf(dbg," feature registred");
	DBG;
#endif
				return;
			}
		}
		node = (CDMFeature*)node->GetNext();
	}
}

void CDMFeatureList::UnregAll()
{
#ifdef DBG_FEATURE_LIST
	sprintf(dbg,"CDMFeatureList::UnregAll()");
	DBG;
#endif
	CDMFeature *node = (CDMFeature*)GetHead();
	while(node)
	{
		if((node->m_state & 1) && node->m_rclass)
		{
#ifdef DBG_FEATURE_LIST
	sprintf(dbg," unregisting feature %s",node->m_strName);
	DBG;
#endif
			CBLContext::UnRegisterContextClass(node->m_rclass);
			node->m_state = 0;
		}
		node = (CDMFeature*)node->GetNext();
	}
}

////////////////////////////////////////////////////////////////
// ExtModule node
////////////////////////////////////////////////////////////////
CDMExtModule::CDMExtModule(class CDMModuleKey *key, char *name, int size) : CXTNode(m_strName)
{
#ifdef DBG_EXTMODULE_NODE
	sprintf(dbg,"CDMExtModule(%s,%d bytes)&%08X",name,size,this);
	DBG;
#endif

	int l = 0;
	if(name)
	{
		l = strlen(name);
		if(l > 31) {l=31;}
		memcpy(m_strName,name,l);
	}
	m_strName[l]=0;

	m_key = key;
	m_status = 0;
	if(size)
	{
		if(m_text = (char*)malloc(size))
		{m_size = size;}
		else
		{m_size = 0;}
	}
	else
	{
		m_text = 0;
		m_size = 0;
	}
}

CDMExtModule::~CDMExtModule()
{
#ifdef DBG_EXTMODULE_NODE
	sprintf(dbg,"~CDMExtModule &%08X name <%s>",this,m_strName);
	DBG;
#endif
	if(m_text)
	{
		free(m_text);
		m_text = 0;
		m_size = 0;
	}
}

////////////////////////////////////////////////////////////////
// ExtModule list
////////////////////////////////////////////////////////////////
CDMExtModuleList::CDMExtModuleList()
{
#ifdef DBG_EXTMODULE_LIST
	sprintf(dbg,"CDMExtModuleList()&%08X",this);
	DBG;
#endif
}

CDMExtModuleList::~CDMExtModuleList()
{
#ifdef DBG_EXTMODULE_LIST
	sprintf(dbg,"~CDMExtModuleList &%08X",this);
	DBG;
#endif
	CDMExtModule *node;
	while(node = (CDMExtModule*)RemoveTail())
	{
		delete node;
	}
}


CDMLicenseList		cLicenseList;
CDMModuleKeyList	cModuleKeyList;
CDMCryptKeyList		cCryptKeyList;
CDMFeatureList		cFeatureList;
CDMExtModuleList	cExtModuleList;