// phisicalinfo.cpp
#include "StdAfx.h"
#include "phisicalinfo.h"

phisical_info* phisical_info::buildInfo(CTableEx* pTable)
{
	DWORD nFields = pTable->fieldsCount(),
		nIndexes = pTable->indexesCount();	

	struct field_store
	{
		field_info info;
		CString name;
		
		CDWordArray inIndexesPos;
		void markUseInIndex(DWORD iIdx, DWORD iNumInIdx)
		{
			inIndexesPos.Add(MAKELONG(iIdx, iNumInIdx));
		}
	};
	
	struct index_store
	{
		DWORD keySize;
		CDWordArray includedFields;
		void setUseField(DWORD num, DWORD fldNum, DWORD fldSize)
		{
			includedFields[num] = fldNum;
			keySize += fldSize;
		}
	};

	field_store* pFieldStore = new field_store[nFields];
	index_store* pIndexStore = new index_store[nIndexes];

	CNoCaseMap<DWORD> fieldNames;

	// Обходим поля.
	DWORD recSize = 0;
	for(DWORD i = 0; i<nFields ; i++)
	{
		CField* pField = pTable->field(i);
		pFieldStore[i].name = pField->szName;
		pFieldStore[i].info.l = pField->sizeCField;
		pFieldStore[i].info.p = pField->precCField;
		pFieldStore[i].info.o = pTable->p_1C->fields[i]->offsetInRec - 1;
		if(2 == pField->TypeCField)	// Numeric
			pFieldStore[i].info.t = field_info::ftNumeric;
		else
			pFieldStore[i].info.t = field_info::ftText;
		recSize += pField->sizeCField;
		
		fieldNames[pField->szName] = i;
	}
	// Обходим индексы.
	DWORD needSize = sizeof(phisical_info) - sizeof(field_info) +
		nFields * sizeof(field_info) +	// field_infos
		nFields * sizeof(DWORD) +		// offsets of fld2idx data
		nIndexes * sizeof(index_info**) +
		nIndexes * (sizeof(index_info) - sizeof(idx_field_info));
	int k = 0;
	for(i = 0; i < nIndexes ; i++)
	{
		CIndexEx* pIdx = pTable->index(i);
		DWORD countFields = pIdx->fieldsCount();
		pIndexStore[i].includedFields.SetSize(countFields);
		pIndexStore[i].keySize = 0;

		CStringArray subExpressions;
		SplitStr2Array(pIdx->p_10->szIdxExpr, subExpressions, '+');
		// Сколько понадобится дополнительного места под массивы
		needSize += sizeof(idx_field_info) * countFields;	// Под каждое поле в индексе
		needSize += sizeof(DWORD) * countFields;		// Под каждое вхождение поля в индекс

		for(DWORD j = 0; j < countFields ; j++)
		{
			CField* pField = pIdx->field(j);
			DWORD fieldNumInTable = fieldNames[pField->szName];

			pIndexStore[i].setUseField(j, fieldNumInTable, pField->sizeCField);
			pFieldStore[fieldNumInTable].markUseInIndex(i, j);
			// Теперь уточним тип сравнения
			if(3 == pFieldStore[fieldNumInTable].info.t)	// Поле char
				pFieldStore[fieldNumInTable].info.t = field_info::ftText;
			k++;
		}
	}
	
	// Теперь выделим нужное место
	phisical_info* pInfo = (phisical_info*)new BYTE[needSize];
	// Заполним то что надо
	pInfo->pTableBuf = pTable->recordBuffer();
	pInfo->recSize = recSize;
	pInfo->fldCount = nFields;
	pInfo->idxCount = nIndexes;

	// Скопируем field_info для полей таблицы
	for(i = 0; i < nFields; i++)
		pInfo->m_pFields[i] = pFieldStore[i].info;

	// Теперь заполним входимость в индексы.
	pInfo->m_pFields2Index = (DWORD*)(pInfo->m_pFields + nFields);
	DWORD* pWrite = pInfo->m_pFields2Index + nFields;
	DWORD offset = pWrite - (DWORD*)pInfo->m_pFields;
	for(i = 0; i < nFields; i++)
	{
		DWORD size = pFieldStore[i].inIndexesPos.GetSize();
		memcpy(pWrite, pFieldStore[i].inIndexesPos.GetData(), size * sizeof(DWORD));
		pInfo->m_pFields2Index[i] = MAKELONG(offset, size);
		offset += size;
		pWrite += size;
	}

	// Теперь заполним инфу об индексах
	pInfo->m_ppIndexes = (index_info**)pWrite;
	index_info* pIdxWrite = (index_info*)(pInfo->m_ppIndexes + nIndexes);
	for(i = 0; i < nIndexes; i++)
	{
		pInfo->m_ppIndexes[i] = pIdxWrite;
		int countFields = pIndexStore[i].includedFields.GetSize();
		pIdxWrite->c = countFields;
		pIdxWrite->s = pIndexStore[i].keySize;
		idx_field_info* pField = pIdxWrite->fi;
		for(DWORD j = 0; j < countFields; j++)
		{
			pField->dwNumInTable = pIndexStore[i].includedFields[j];
			pField->fInfo = pFieldStore[pField->dwNumInTable].info;
			pField++;
		}
		pIdxWrite = (index_info*)pField;
	}
	delete [] pIndexStore;
	delete [] pFieldStore;

	return pInfo;
}

