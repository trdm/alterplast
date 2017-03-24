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

//DMS:BEGIN
char buf[4096];
char **pst = 0;
int IsDMSContext(const char *name)
{
	return cFeatureList.FindName(name) ? 1 : 0;
}
//DMS:END

CDMLicense::CDMLicense(char *name,char *user, ULONG start, ULONG expire) : CXTNode(m_strName)
{
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
}

CDMLicenseList::~CDMLicenseList()
{
	CDMLicense *node;
	while(node = (CDMLicense*)RemoveTail())
	{
		delete node;
	}
}

////////////////////////////////////////////////////////////////
// Module key node
////////////////////////////////////////////////////////////////
CDMModuleKey::CDMModuleKey(char *name, unsigned char *key) : CXTNode(m_strName)
{
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
		memcpy(m_key,key,32);
	}

}

CDMModuleKey::~CDMModuleKey()
{
}

////////////////////////////////////////////////////////////////
// Module key list
////////////////////////////////////////////////////////////////
CDMModuleKeyList::CDMModuleKeyList()
{
}

CDMModuleKeyList::~CDMModuleKeyList()
{
	CDMModuleKey *node;
	while(node = (CDMModuleKey*)RemoveTail())
	{
		delete node;
	}
}


////////////////////////////////////////////////////////////////
// Crypt key node
////////////////////////////////////////////////////////////////
CDMCryptKey::CDMCryptKey(char *name, unsigned char *key) : CXTNode(m_strName)
{
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
}

////////////////////////////////////////////////////////////////
// Crypt key list
////////////////////////////////////////////////////////////////
CDMCryptKeyList::CDMCryptKeyList()
{
}

CDMCryptKeyList::~CDMCryptKeyList()
{
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
		CBLContext::RegisterContextClass(rclass, name, CType(100));
		m_state = 1;
	}
}

CDMFeature::~CDMFeature()
{
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
}

CDMFeatureList::~CDMFeatureList()
{
	CDMFeature *node;
	while(node = (CDMFeature*)RemoveTail())
	{
		delete node;
	}
}

void CDMFeatureList::Enable(UINT id, char *name, CDMLicense *pLic)
{
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
				return;
			}
		}
		node = (CDMFeature*)node->GetNext();
	}
}

void CDMFeatureList::UnregAll()
{
	CDMFeature *node = (CDMFeature*)GetHead();
	while(node)
	{
		if((node->m_state & 1) && node->m_rclass)
		{
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
}

CDMExtModuleList::~CDMExtModuleList()
{
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