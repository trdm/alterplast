// index_selector.h
#pragma once

#include "vtab_info.h"

struct op_node
{
	op_node(TestOp _op, usage_ptr pU, op_node* n)
		: CompareOp(_op), pUsage(pU), next(n){}

	TestOp CompareOp;
	usage_ptr pUsage;
	op_node* next;
};

struct idx_field_node 
{
	idx_field_node(DWORD p, idx_field_node* n) : posInIdx(p), compares(NULL), next(n){}
	DWORD posInIdx;		// -1 - full index
	op_node* compares;
	idx_field_node* next;
};

struct idx_node
{
	idx_node(DWORD i, idx_node* n, const CVtabInfo& t)
		: indexNum(i), fields(NULL), orderByLen(0), shiftInOrderBy(0), order(nooNa), next(n)
	{
		fullLenInIdx = t.phisInfo().index(indexNum)->fieldsCount();
	}
	DWORD indexNum;
	idx_field_node* fields;
	int orderByLen;
	int shiftInOrderBy;
	NeedOrderOutput order;
	int usedLenOfIndex;
	int fullLenInIdx;
	idx_node* next;
};

struct index_selector
{
	index_selector() : root(NULL){}
	~index_selector()
	{
		idx_node* pIdx = root;
		while(pIdx)
		{
			idx_field_node* pFld = pIdx->fields;
			while(pFld)
			{
				op_node* pOp = pFld->compares;
				while(pOp)
				{
					op_node* next = pOp->next;
					delete pOp;
					pOp = next;
				}
				idx_field_node* next = pFld->next;
				delete pFld;
				pFld = next;
			}
			idx_node* next = pIdx->next;
			delete pIdx;
			pIdx = next;
		}
	}

	const idx_node* bestIndex(sqlite3_index_info* pIdx, const CVtabInfo& table)
	{
		constraint_ptr pCtr = pIdx->aConstraint;
		usage_ptr pUsg = pIdx->aConstraintUsage;

		// Наполним инфой об индексах
		for(DWORD i = 0; i < pIdx->nConstraint ; i++, pCtr++, pUsg++)
		{
			if(!isConstraint(pCtr))
				continue;
			
			const one_field& fld = table.field(pCtr->iColumn);
			TestOp op = sqlite2TestOp(pCtr->op);
			
			if(fld.isRecNo())
			{
				if(toEqual == op)
					addConstaraint(-1, 0, toEqual, pUsg, table);
			}
			else if(fld.isField())
			{
				if(fld.canUseFieldInIdx(op))
				{
					for(phisical_info::fld2idxIt it = table.phisInfo().fld2idx(fld.pos()) ; it ; ++it)
						addConstaraint(it.idxNum(), it.fldNum(), op, pUsg, table);
				}
			}
			else if(fld.isIndex()) // index
			{
				addConstaraint(fld.pos(), -1, op, pUsg, table);
			}
		}
		// Проверим на order by
		processOrderBy(pIdx->aOrderBy, pIdx->nOrderBy, table);
		return bestOfTheBest();
	}

	static TestOp sqlite2TestOp(DWORD op)
	{
		if(op == SQLITE_INDEX_CONSTRAINT_EQ)
			return toEqual;
		else if(op == SQLITE_INDEX_CONSTRAINT_LT)
			return toLess;
		else if(op == SQLITE_INDEX_CONSTRAINT_LE)
			return toLessEq;
		else if(op == SQLITE_INDEX_CONSTRAINT_GT)
			return toGrat;
		else if(op == SQLITE_INDEX_CONSTRAINT_GE)
			return toGratEq;
		return (TestOp) -1;
	}

private:
	
	idx_node* root;

	idx_node* find_idx(DWORD idxNum, const CVtabInfo& table, BOOL bCreate)
	{
		idx_node** ppNode = &root;
		while(*ppNode)
		{
			if((*ppNode)->indexNum == idxNum)
				return *ppNode;
			if((*ppNode)->indexNum > idxNum)
				break;
			ppNode = &(*ppNode)->next;
		}
		return bCreate ? (*ppNode = new idx_node(idxNum, *ppNode, table)) : NULL;
	}
	idx_field_node* find_pos_in_idx(idx_node* pIdx, DWORD posInIndex)
	{
		idx_field_node** ppNode = &pIdx->fields;
		while(*ppNode)
		{
			DWORD p = (*ppNode)->posInIdx;
			if(p == posInIndex)
				return *ppNode;
			else if(p > posInIndex)
				break;
			ppNode = &(*ppNode)->next;
		}
		return *ppNode = new idx_field_node(posInIndex, *ppNode);
	}
	
	void addConstaraint(DWORD idxNum, DWORD posInIdx, TestOp op, usage_ptr pUsage, const CVtabInfo& table)
	{
		idx_field_node* pNode = find_pos_in_idx(find_idx(idxNum, table, TRUE), posInIdx);
		op_node** ppNode = &pNode->compares;
		while(*ppNode)
		{
			if((*ppNode)->CompareOp >= op)
				break;
			ppNode = &(*ppNode)->next;
		}
		*ppNode = new op_node(op, pUsage, *ppNode);
	}

	BOOL addOrderFieldInIdx(DWORD idxNum, DWORD posInIdx, DWORD posInOrderBy, NeedOrderOutput order, const CVtabInfo& table)
	{
		if(idx_node* pIndexNode = find_idx(idxNum, table, posInOrderBy == 0))
		{
			if(posInOrderBy == posInIdx - pIndexNode->shiftInOrderBy)
			{
				if(pIndexNode->orderByLen == posInIdx)
				{
					pIndexNode->orderByLen++;
					pIndexNode->order = order;
					return TRUE;
				}
			}
			else if(posInOrderBy == 0 && posInIdx < pIndexNode->shiftInOrderBy)
			{
				pIndexNode->shiftInOrderBy = posInIdx;
				pIndexNode->orderByLen = posInIdx + 1;
				pIndexNode->order = order;
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL addOrderFieldToIndexes(DWORD fieldNumInTable, DWORD posInOrderBy, NeedOrderOutput order, const CVtabInfo& table)
	{
		BOOL ret = FALSE;
		for(phisical_info::fld2idxIt it = table.phisInfo().fld2idx(fieldNumInTable) ; it ; ++it)
		{
			if(addOrderFieldInIdx(it.idxNum(), it.fldNum(), posInOrderBy, order, table))
				ret = TRUE;
		}
		return ret;
	}
	
	void processOrderBy(orderby_ptr pOrderBy, int countOrderBy, const CVtabInfo& table)
	{
		if(!countOrderBy)
			return;
		// Для каждого индекса определим, сколько входящих в него полей
		// ограничены сравнением '=', тк допустим, при условиях
		// parentid= and isfolder= order by descr,
		// при использовании индекса parentid_isfolder_descr он попадет в сортировку
		for(idx_node* pIndexNode = root; pIndexNode ; pIndexNode = pIndexNode->next)
		{
			DWORD pos = 0;
			for(idx_field_node* pFieldNode = pIndexNode->fields; pFieldNode ; pFieldNode = pFieldNode->next)
			{
				if(pFieldNode->posInIdx == -1)
					continue;
				if(pFieldNode->posInIdx > pos)
					break;
				if(pFieldNode->compares->CompareOp == toEqual)
				{
					pIndexNode->orderByLen++;
					pIndexNode->shiftInOrderBy++;
					pos++;
				}
				else
					break;
			}
		}
		
		NeedOrderOutput order = pOrderBy->desc ? nooDesc : nooAsc;
		for(DWORD posInOrderBy = 0; countOrderBy--; )
		{
			if((order == nooAsc && pOrderBy->desc) || (order == nooDesc && !pOrderBy->desc))
				return;	// Индексы 1С не могут обеспечить такую сортировку.
			const one_field& field = table.field(pOrderBy->iColumn);
			BOOL bAdded = FALSE;
			if(field.isField())
			{
				bAdded = addOrderFieldToIndexes(field.pos(), posInOrderBy, countOrderBy ? nooNa : order, table);
				posInOrderBy++;
			}
			else if(field.isIndex())
			{
				const index_info* pIndexInfo = table.phisInfo().index(field.pos());
				const idx_field_info* pIdxField = pIndexInfo->fields();
				for(DWORD c = pIndexInfo->fieldsCount(); c--; pIdxField++)
				{
					if(addOrderFieldToIndexes(pIdxField->numInTable(), posInOrderBy, countOrderBy ? nooNa : order, table))
						bAdded = TRUE;
					posInOrderBy++;
				}
			}
			if(!bAdded)
				return;
			pOrderBy++;
		}
	}

	DWORD indexScores(idx_node* idx)
	{
		DWORD scores = 0;	// Набранные баллы.
		if(idx->indexNum == -1)	// recNo
		{
			for(idx_field_node* pField = idx->fields; pField ; pField = pField->next)
				scores += 32;
			return scores;
		}
		// За попадание в сортировку добавим баллов
		if(idx->order != nooNa)
			scores += idx->orderByLen;
		else
			idx->orderByLen = 0;
		int posInIdx = 0, bFull = 0;
		for(idx_field_node* pField = idx->fields; pField ; pField = pField->next)
		{
			if(pField->posInIdx == -1)	// Полный индекс
			{
				for(op_node* pOp = pField->compares; pOp; pOp = pOp->next)
				{
					scores += idx->fullLenInIdx;
					if(pOp->CompareOp == toEqual)
						scores += idx->fullLenInIdx;
				}
				bFull = TRUE;
			}
			else
			{
				if(pField->posInIdx > posInIdx)	// поле не попадает в индекс
					break;
				
				BOOL bAllowNext = FALSE;

				for(op_node* pOp = pField->compares; pOp; pOp = pOp->next)
				{
					scores++;
					if(pOp->CompareOp == toEqual)
					{
						bAllowNext = TRUE;
						scores++;
					}
				}
				posInIdx++;
				if(!bAllowNext)
					break;
			}
		}
		idx->usedLenOfIndex = bFull ? idx->fullLenInIdx : posInIdx;
		return scores;
	}
	
	idx_node* bestOfTheBest()
	{
		DWORD maxScores = 0;
		idx_node* pBest = NULL;

		for(idx_node* idx = root; idx ; idx = idx->next)
		{
			DWORD scores = indexScores(idx);
			if(maxScores < scores)
			{
				pBest = idx;
				maxScores = scores;
			}
		}
		return pBest;
	}
};

