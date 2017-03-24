//////////////////////////////////////////////////////////////////////
// Класс CVTExtended, реализующий тип 1С "ИндексированнаяТаблица"
//
// Автор: Диркс Алексей aka ADirks
// Контакты: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTExtended.h"


static CString sChildrenColumn("тзПотомки");
static CString sIsGroupColumn("__ЭтоГруппа__");
static CString sLevelColumn("__Уровень__");
static CValue vEmpty, vZero(0L), vOne(1L);


int CVTExtGroupingRecord::BySubcontoGroups()
{
	int sz = IndexFields.size();
	for( int i = 0; i < sz; i++ )
		if( IndexFields[i].SumsBySubcontoGroups ) return IndexFields[i].ColumnNumber;
	return -1;
}

CVTExtGroupings::CVTExtGroupings(CString& Groupings, CVTExtended* pVT)
{
	CStringArrayEx arrGroupings;
	arrGroupings.FillSeparateString(Groupings, ";");

	int sz = arrGroupings.GetSize();
	SetSize(sz);
	for( int i = 0; i < sz; i++ )
	{
		CStringArrayEx IndexParts;
		IndexParts.FillSeparateString(arrGroupings[i], ":");
		if( IndexParts.GetSize() != 2 )
			RuntimeError("Неверно задана группировка %i: '%s'", i+1, arrGroupings[i].operator LPCTSTR());

		operator[](i).IndexName = IndexParts[0];
		operator[](i).IndexFields.Build(IndexParts[1], pVT);
	}
}

//Строки, которые были перемещены в таблицы-потомки, удаляем из массива Rows
void CVTExtended::DeleteUnneededRows(bool* deleted, int nRowsRest, bool bDelete)
{
	int nRows = GetRowsCount();
	for( int nDestRow = 0, nRow = 0; nRow < nRows; nRow++ )
	{
		if( deleted[nRow] )
		{
			if( bDelete ) delete GetRow(nRow); //убираем строку из массива
		}
		else
		{
			Rows[nDestRow++] = Rows[nRow]; //переносим оставшиеся строки в начало
		}
	}
	Rows.SetSize(nRowsRest); //теперь хвост массива можно просто обрезать
	delete[] deleted;
}

void CVTExtended::CalcSums(CVTExtIndexTreeNode* node, CVTExtRow* pSumRow, int* arrSumCols, int nSumCols)
{
	for( int i = node->ArrayEqualIDs->size() - 1; i >= 0; i-- )
	{
		int nRow = node->ArrayEqualIDs->operator[](i);
		CVTExtRow* pRow = GetRow(nRow);
		for( int nSumCol = 0; nSumCol < nSumCols; nSumCol++ )
		{
			CValue& val = pSumRow->GetValue(arrSumCols[nSumCol]);
			CValue& add = pRow->GetValue(arrSumCols[nSumCol]);
			val = val.GetNumeric() + add.GetNumeric();
		}
	}
}

void CVTExtended::Group(CVTExtGroupings& Groupings, int* arrSumCols, int nSumCols, bool bSaveLastLevel, bool bRemoveColumns, int nGrouping)
{
	bool bIsLastLevel = (Groupings.GetUpperBound()) <= nGrouping;

	int nRows = GetRowsCount();
	int nColumns = GetColumnsCount();
	int nChildrenCol = -1;
	if( !bIsLastLevel || bSaveLastLevel )
		nChildrenCol = NewColumn(sChildrenColumn);

	CVTExtGroupingRecord const& Grp = Groupings[nGrouping];
	int nIndex = Indexes.AddIndex(Grp.IndexName, Grp.IndexFields, false, this);
	CVTExtIndex *pIndex = Indexes[nIndex];

	bool *deleted = new bool[nRows];
	memset((void*)deleted, 0, sizeof(bool) * nRows);

	CVTExtIndexTreeNode *node = pIndex->Min(pIndex->Root);
	while( node )
	{
		int nRow = node->ID;
		CVTExtRow *pRow = GetRow(nRow);
		CVTExtended *pChildVT = NULL;
		if( nChildrenCol >= 0 )
		{
			CValue& vVT = pRow->GetValue(nChildrenCol);
			vVT.CreateObject("IndexedTable");
			pChildVT = CValue2VTExt(vVT);

			for( int nColName = 0; nColName < nColumns; nColName++ )
			{
				pChildVT->NewColumn(ColumnName(nColName));
			}

			pChildVT->Rows.SetSize(node->Count);

			//Первую строку нужно скопировать
			pChildVT->Rows.SetAt(0, new CVTExtRow(nColumns)); 
			*(pChildVT->GetRow(0)) = *pRow;
		}

		if( node->ArrayEqualIDs )
		{
			int nEquals = node->ArrayEqualIDs->size();
			
			//посчитаем суммы в нужных колонках
			CalcSums(node, pRow, arrSumCols, nSumCols);

			//пометим строки как удалённые
			for( int i = 0; i < nEquals; i++ )
			{
				deleted[ node->ArrayEqualIDs->operator[](i) ] = true;
			}

			if( nChildrenCol >= 0 )
			{
				//Все остальные строки просто перенесём в новую таблицу
				int nChildRow = 1;
				for( int i = 0; i < nEquals; i++ )
				{
					int nRow = node->ArrayEqualIDs->operator[](i);
					CVTExtRow* pRow = GetRow(nRow);
					pChildVT->Rows[nChildRow++] = pRow;
				}
			}
		}

		//Сгруппируем таблицу потомков
		if( pChildVT && !bIsLastLevel )
			pChildVT->Group(Groupings, arrSumCols, nSumCols, bSaveLastLevel, bRemoveColumns, nGrouping + 1);

		node = pIndex->Successor(node);
	}

	DeleteUnneededRows(deleted, pIndex->NodesCount, nChildrenCol < 0);
	Indexes.Reindex();
	DefaultIndex->ResetPosition();
}


bool CVTExtended::AddToAllParentFolders(CVTExtended* pVTGroups,
										int nRow, CVTExtRow* pRow,
										int nRefCol, int nRefParentCol, int nParentCol,
										int nChildrenCol,
										int* arrSumCols, int nSumCols,
										bool bIsGroup,
										bool* deleted
										)
{
	CValue &Val = pRow->GetValue(nRefCol);
	if( Val.IsEmpty() ) return false;

	CValue Parent;
	CBLContext* pContext;
	int nValType = Val.GetTypeCode();

	if( nRefParentCol >= 0 )
	{
		if( nValType != REFERENCE_TYPE_1C && nValType != DATE_TYPE_1C ) return false; //не справочник и не дата - как родителя получать не знаем
		Parent = pRow->GetValue(nRefParentCol);
	}
	else
	{
		if( nValType == REFERENCE_TYPE_1C )
		{
			Val.LinkContext(FALSE);
			pContext = Val.GetContext();
			::GetPropVal(pContext, "Родитель", Parent);
			Val.UnlinkContext();
		}
		else if( nValType == DATE_TYPE_1C )
		{
			if( bIsGroup ) return false;
			Parent = Val.GetDate().BegOfMonth();
		}
		else
		{
			return false; //не справочник и не дата - как родителя получать не знаем
		}
	}

	CVTExtIndex* pGrpIndex = pVTGroups->DefaultIndex;
	int nIsGroupCol = ColumnNumber(sIsGroupColumn);
	int nIteration = 0;
	while( !Parent.IsEmpty() )
	{
		bool bParentRowFound;
		int nGrpRow = pGrpIndex->Find(Parent, false, false);
		CVTExtRow* pGrpRow;
		CVTExtended* pVTChildren;
		
		//закэшируем родителя
		if( nIteration > 0 || nRefParentCol < 0 ) pRow->SetValue(nParentCol, Parent);

		if( nGrpRow < 0 )
		{
			bParentRowFound = false;

			//Строку с группой не нашли - надо её создать
			pGrpRow = new CVTExtRow(pVTGroups->GetColumnsCount());
			*pGrpRow = *pRow;
			pGrpRow->SetValue(nRefCol, Parent); //вместо элемента впишем родителя
			pGrpRow->SetValue(nParentCol, vEmpty); //родителя зачистим
			pGrpRow->SetValue(nIsGroupCol, vOne); //запишем флаг группы

			//создадим таблицу для потомков (элементов).
			CValue& VTChildren = pGrpRow->GetValue(nChildrenCol);
			VTChildren.CreateObject("IndexedTable");
			pVTChildren = CValue2VTExt(VTChildren);
			pVTChildren->LoadStructure(this);

			//обнулим суммы в группе
			for( int i = 0; i < nSumCols; i++ )
				pGrpRow->GetValue(arrSumCols[i]) = 0L;

			nGrpRow = pVTGroups->Rows.Add(pGrpRow);
			pGrpIndex->InsertRow(pGrpRow, nGrpRow);
		}
		else
		{
			if( nIteration > 0 ) break; //группы копировать не надо, и подниматься выше 
										//тоже смысла нет

			bParentRowFound = true;

			//Получаем таблицу потомков
			pGrpRow = pVTGroups->GetRow(nGrpRow);
			pVTChildren = CValue2VTExt(pGrpRow->GetValue(nChildrenCol));
		}

		if( nIteration == 0 )
		{
			//Переносим строку в таблицу потомков
			pVTChildren->Rows.Add(pRow);
			deleted[nRow] = true;

			//посчитаем суммы по группе
			for( int i = 0; i < nSumCols; i++ )
			{
				int nCol = arrSumCols[i];
				CNumeric& dest = (CNumeric&)(pGrpRow->GetValue(nCol).GetNumeric());
				dest = dest + pRow->GetValue(nCol).GetNumeric();
			}
		}

		nIteration++;

		//дата пока имеет только один вышестоящий уровень
		if( nValType == DATE_TYPE_1C ) break;

		//если родитель уже есть в таблице, то и все вышестоящие - тоже есть. Выше подниматься не надо
		if( bParentRowFound ) break;
		//если переданная строка уже является группой, то вышестоящие родители уже заполнены, и выше подниматься не надо
		if( bIsGroup ) break;

		Parent.LinkContext(FALSE);
		pContext = Parent.GetContext();
		CValue NewParent;
		::GetPropVal(pContext, "Родитель", NewParent);
		Parent.UnlinkContext();
		Parent = NewParent;

		pRow = pGrpRow;
	}

	return nIteration > 0;
}

void CVTExtended::CalcGroupLevels(CVTExtended* pVTGroups, int nParentCol, int nLevelCol)
{
	CVTExtIndex* pGrpIndex = pVTGroups->DefaultIndex;
	int nRows = pVTGroups->GetRowsCount();

	bool bChanges = false;
	do
	{
		bChanges = false;
		for( int nRow = 0; nRow < nRows; nRow++ )
		{
			CVTExtRow* pRow = pVTGroups->GetRow(nRow);
			if( pRow->GetValue(nLevelCol).GetTypeCode() == NUMBER_TYPE_1C ) continue; //Уровень уже посчитан

			CValue& Parent = pRow->GetValue(nParentCol);
			if( Parent.IsEmpty() )
			{
				pRow->SetValue(nLevelCol, vOne);
				bChanges = true;
			}
			else
			{
				int nParentRow = pGrpIndex->Find(Parent, false, false);
				CVTExtRow* pParentRow = pVTGroups->GetRow(nParentRow);
				CValue& ParentLevel = pParentRow->GetValue(nLevelCol);
				if( ParentLevel.GetTypeCode() == NUMBER_TYPE_1C )
				{
					pRow->SetValue(nLevelCol, ParentLevel.GetNumeric() + vOne.GetNumeric());
					bChanges = true;
				}
			}
		}
	} while( bChanges );
}


void CVTExtended::SumsBySubcontoFolders(CVTExtGroupings & Groupings, int* arrSumCols, int nSumCols, int nGrouping)
{
	bool bIsLastLevel = nGrouping >= (Groupings.GetUpperBound());
	int nRows = GetRowsCount();
	int nColumns = GetColumnsCount();
	int nChildrenCol = ColumnNumber(sChildrenColumn, true);
	CVTExtGroupingRecord& Grp = Groupings.ElementAt(nGrouping);
	int nRefCol = Grp.BySubcontoGroups();

	if( !bIsLastLevel )
	{
		//Сначала посчитаем итоги в нижележащих уровнях
		for( int nRow = 0; nRow < nRows; nRow++ )
		{
			CVTExtRow* pRow = GetRow(nRow);
			CValue& VT = pRow->GetValue(nChildrenCol);
			CVTExtended* pVT = CValue2VTExt(VT);
			pVT->SumsBySubcontoFolders(Groupings, arrSumCols, nSumCols, nGrouping+1);
		}
	}

	if( nRefCol < 0 ) return; //На этом уровне не нужны итоги по группам


	NewColumn(sIsGroupColumn); //колонка для флага группы - она останется в результате

	//имя колонки с родителем
	CString sRefParentCol = ColumnName(nRefCol);
	sRefParentCol += "_Родитель";
	int nRefParentCol = ColumnNumber(sRefParentCol, true);
	//если в исходной таблице нет колонки с родителем, то в служебную таблицу нужно её добавить
	int nParentCol = nRefParentCol;
	if( nParentCol < 0 ) nParentCol = NewColumn(sRefParentCol);
	//служебная колонка для уровня группы
	int nLevelCol = NewColumn(sLevelColumn);

	//если в исходной таблице нет колонки тзПотомки, то нужно её добавить - там будут потомки группы
	if( nChildrenCol < 0 )
		nChildrenCol = NewColumn(sChildrenColumn);

	// Соорудим временную таблицу для групп
	CValue vVTGroups;
	vVTGroups.CreateObject("IndexedTable");
	CVTExtended* pVTGroups = CValue2VTExt(vVTGroups);
	pVTGroups->LoadStructure(this);
	
	// Проиндексируем по колонке группы
	CString sIndexExpr(ColumnName(nRefCol));
	sIndexExpr += "*"; //сортировать будем по внутреннему представлению
	CVTExtIndex* pGrpIndex = pVTGroups->DefaultIndex;
	pGrpIndex->Reindex( sIndexExpr );

	bool *deleted = NULL; //флаги удаления строк
	int nRowsRest = nRows;
	deleted = new bool[nRows];
	memset((void*)deleted, 0, sizeof(bool) * nRows);

	//Скопируем строки в служебную таблицу групп. Строка копируется во все вышестоящие группы
	for( int nRow = 0; nRow < nRows; nRow++ )
	{
		CVTExtRow* pRow = GetRow(nRow);
		if( AddToAllParentFolders(pVTGroups, nRow, pRow, 
			nRefCol, nRefParentCol, nParentCol,
			nChildrenCol, 
			arrSumCols, nSumCols, 
			false,
			deleted) )
		{
			nRowsRest--;
		}
	}

	//Уберём лишние строки
	DeleteUnneededRows(deleted, nRowsRest, false);

	//Проставим уровни для групп
	CalcGroupLevels(pVTGroups, nParentCol, nLevelCol);

	//CValue RetVal, p1, p2, p3, p4, *params[4] = {&p1, &p2, &p3, &p4};
	//::CallAsFunc(pVTGroups, "Show", RetVal, params);
	//::CallAsFunc(this, "Show", RetVal, params);

	//Свернём таблицу групп, и то, что осталось на первом уровне, поместим обратно в текущую таблицу
	CVTExtIndex* pLevelIndex = new CVTExtIndex("Level", sLevelColumn, false, pVTGroups);
	nRowsRest = pVTGroups->GetRowsCount();
	deleted = new bool[nRowsRest];
	memset((void*)deleted, 0, sizeof(bool) * nRowsRest);

	//сворачиваем
	pLevelIndex->Last();
	while( pLevelIndex->Prev() )
	{
		CVTExtRow* pRow = pLevelIndex->pCurrentRow;
		if( AddToAllParentFolders(pVTGroups,
				pLevelIndex->nCurrentRow, pRow, 
				nRefCol, nParentCol, nParentCol,
				nChildrenCol,
				arrSumCols, nSumCols, 
				true,
				deleted) )
		{
			nRowsRest--;
		}
	}
	delete pLevelIndex;
	pVTGroups->DeleteUnneededRows(deleted, nRowsRest, false);

	//переносим
	for( int nRow = 0; nRow < nRowsRest; nRow++ )
	{
		Rows.Add(pVTGroups->GetRow(nRow));
	}
	pVTGroups->Indexes.RemoveAll();
	pVTGroups->Rows.RemoveAll();

	Indexes.Reindex();
}

BOOL CVTExtended::defsGroup(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 2:
		*param = 0L;
		return TRUE;
		break;
	case 3:
		*param = 0L;
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL CVTExtended::procGroup(CValue** params)
{
	CString sGroupings = params[0]->GetString();
	CString sSumCols = params[1]->GetString();
	bool bSaveLastLevel = params[2]->GetNumeric() != 0;
	bool bRemoveColumns = params[3]->GetNumeric() != 0;

	CVTExtGroupings Groupings(sGroupings, this);
	int nGroups = Groupings.GetSize();
	if( nGroups == 0 )
		RuntimeError("Нет ни одной группировки!");

	CStringArrayEx arrSumColNames;
	arrSumColNames.FillSeparateString(sSumCols, ",");
	int* arrSumCols = new int[arrSumColNames.GetSize()];
	for( int nSumCol = arrSumColNames.GetUpperBound(); nSumCol >= 0; nSumCol-- )
	{
		int nCol = ColumnNumber(arrSumColNames[nSumCol]);
		arrSumCols[nSumCol] = nCol;
	}

	Group(Groupings, arrSumCols, arrSumColNames.GetSize(), bSaveLastLevel, bRemoveColumns, 0);

	bool bNeedGroupByFolders = false;
	for( int i = 0; i < nGroups; i++ )
	{
		if( Groupings[i].BySubcontoGroups() >= 0 )
		{
			bNeedGroupByFolders = true;
			break;
		}
	}
	if( bNeedGroupByFolders )
		SumsBySubcontoFolders(Groupings, arrSumCols, arrSumColNames.GetSize(), 0);

	delete[] arrSumCols;

	return TRUE;
}
