
#pragma once

#include "istrmap.h"

class CPropDefs
{
public:
    CPropDefs():m_pPropDefs(NULL)
    {};
    ~CPropDefs()
    {if (m_pPropDefs) delete[] m_pPropDefs;};

    struct stPropDef
    {
        LPCSTR arNames[2];
        bool bIsReadable;
        bool bIsWritable;
    };

    BOOL IsEmpty()
    {return m_pPropDefs == NULL;};
    void SetSize(int nSize)
    {
        m_pPropDefs = new stPropDef[nSize];
        m_PropMap.InitHashTable(nSize * 1.2);
    };
    void AddProp(LPCSTR szEngAlias, LPCSTR szRusAlias, int nPropIndex, bool bIsReadable, bool bIsWritable)
    {
        stPropDef& pPropDef = m_pPropDefs[nPropIndex];
        pPropDef.arNames[0] = szEngAlias;
        pPropDef.arNames[1] = szRusAlias;
        pPropDef.bIsReadable = bIsReadable;
        pPropDef.bIsWritable = bIsWritable;
        m_PropMap[szEngAlias] = nPropIndex;
        m_PropMap[szRusAlias] = nPropIndex;
    };
    const stPropDef& operator [](int nPropIndex)
    {
        return m_pPropDefs[nPropIndex];
    };
    int FindProp(LPCSTR szPropName)
    {
        int nIndex;
        if (!m_PropMap.Lookup(szPropName, nIndex))
            nIndex = -1;
        return nIndex;
    };
private:
    stPropDef* m_pPropDefs;
    CIStringMap<int, int> m_PropMap;
};
