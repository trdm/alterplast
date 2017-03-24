// DML.h licensing support

#if !defined(DML_H)
#define DML_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xt_list.h"

/////////////////////////////////////////////////////////////////////
// License
/////////////////////////////////////////////////////////////////////
class CDMLicense : public CXTNode
{
public:
	CDMLicense(char *name, char *user, ULONG start, ULONG expire);
	~CDMLicense();

	char			m_strName[256];
	char			m_strUser[256];
	ULONG			m_dateStart;
	ULONG			m_dateExpire;

	int				CheckExpiration(int i);
};


class CDMLicenseList : public CXTList
{
public:
	CDMLicenseList();
	~CDMLicenseList();

};

/////////////////////////////////////////////////////////////////////
// Module key
/////////////////////////////////////////////////////////////////////
class CDMModuleKey : public CXTNode
{
public:
	CDMModuleKey(class CDMLicense *license, char *name, unsigned char *key);
	~CDMModuleKey();

	char			m_strName[32];
	unsigned char	m_key[32];
	CDMLicense*		m_license;
};

class CDMModuleKeyList : public CXTList
{
public:
	CDMModuleKeyList();
	~CDMModuleKeyList();

};


/////////////////////////////////////////////////////////////////////
// Crypt key
/////////////////////////////////////////////////////////////////////
class CDMCryptKey : public CXTNode
{
public:
	CDMCryptKey(class CDMLicense *license, char *name, unsigned char *key);
	~CDMCryptKey();

	char			m_strName[32];
	unsigned char	m_key[8];
};

class CDMCryptKeyList : public CXTList
{
public:
	CDMCryptKeyList();
	~CDMCryptKeyList();

};

/////////////////////////////////////////////////////////////////////
// Feature
/////////////////////////////////////////////////////////////////////
class CDMFeature : public CXTNode
{
public:
	CDMFeature(UINT id,  CRuntimeClass *rclass, char *name, const char *desc);
	~CDMFeature();

	char					m_strName[32];
	const char*				m_desc;
	struct CRuntimeClass*	m_rclass;
	class CDMLicense*		m_license;
	UINT					m_id;
	UINT					m_state;

	int						m_usedMem;// total allocations made by class
	int						m_nInst;//currenly active instances of class
	int						m_maxMem;//maximum allocated memory
	int						m_maxInst;//max simultaniously active objects
	int						m_nRefs;//active references to class objects

	void					IncrInst();
	void					DecrInst();
	void					IncrRef();
	void					DecrRef();
	void					RegAlloc(UINT size);
	void					UnRegAlloc(UINT size);
	void*					Alloc(UINT size);
	void					Free(void*);
};

class CDMFeatureList : public CXTList
{
public:
	CDMFeatureList();
	~CDMFeatureList();

	void	Enable(UINT id, char *name, CDMLicense *pLic);
	void	UnregAll();
};

/////////////////////////////////////////////////////////////////////
// ExtModule
/////////////////////////////////////////////////////////////////////
class CDMExtModule : public CXTNode
{
public:
	CDMExtModule(class CDMModuleKey *key, char *name, int size);
	~CDMExtModule();

	char			m_strName[32];
	int				m_size;
	CDMModuleKey*	m_key;
	char*			m_text;
	UINT			m_status;
};

class CDMExtModuleList : public CXTList
{
public:
	CDMExtModuleList();
	~CDMExtModuleList();

};


extern CDMLicenseList	cLicenseList;
extern CDMModuleKeyList	cModuleKeyList;
extern CDMCryptKeyList	cCryptKeyList;
extern CDMFeatureList	cFeatureList;
extern CDMExtModuleList	cExtModuleList;

#endif //!defined(DML_H)