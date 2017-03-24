#ifndef BATCHGUARD_H
#define BATCHGUARD_H

// классы для работы с блоком OnStartBatch/OnEndBatch

class CExecBatchHelper
{
public:
	CExecBatchHelper(CGetDoc7* pDoc, BOOL bWithRefresh, BOOL bNeedBatch=TRUE):
	  m_pDoc(pDoc),m_bWithRefresh(bWithRefresh), m_bNeedBatch(bNeedBatch)
	  {
		  if(!m_pDoc) return;
		  if(!m_bNeedBatch) return;
			  
		  m_bNeedBatch = !m_pDoc->IsBatchStarted();
		  if (m_bNeedBatch)
			  m_pDoc->OnStartBatch();
	  };
	~CExecBatchHelper()
	{
		if (!m_pDoc) return;
		if (!m_bNeedBatch) return;
		m_pDoc->OnEndBatch(m_bWithRefresh);
	};
protected:
	CGetDoc7* m_pDoc;
	BOOL m_bNeedBatch;
	BOOL m_bWithRefresh;
};

/*
// class CTurboBLBatchGuard : public CBatchGuard
// {
// public:
// 	CTurboBLBatchGuard(CGetDoc7* pd): CBatchGuard(pd, FALSE) // TODO возможно и true будет работать
// 	{};
// };
class CTurboBLBatchGuard
{
protected:
	BOOL bNeedEndBatch;
	CGetDoc7* m_pDoc;
public:
	CTurboBLBatchGuard(CGetDoc7* pd):m_pDoc(pd), bNeedEndBatch(FALSE)
	{
		if(m_pDoc)
		{
			bNeedEndBatch=!m_pDoc->IsBatchStarted();
			m_pDoc->OnStartBatch();
		}
	}
	
	~CTurboBLBatchGuard(){
		if(m_pDoc && bNeedEndBatch) 
			m_pDoc->OnEndBatch(FALSE);
	}
};
*/
#endif