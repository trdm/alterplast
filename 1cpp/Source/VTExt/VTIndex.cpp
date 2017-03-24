//////////////////////////////////////////////////////////////////////
// Классы и методы для работы с индексами для CVTExtended
//
// Автор: Диркс Алексей aka ADirks
// Контакты: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTExtended.h"
#include "../StringArrayEx.h"
#include "../System/compare.h"

/////////////////////////////////////////////////////////////
////  CVTIndexRecord  ///////////////////////////////////////
/////////////////////////////////////////////////////////////
int numeric_compare(int a, int b)
{
	return a - b;
}

int cv_numeric_compare(CValue& left, CValue& right)
{
	return left.GetNumeric().Compare(right.GetNumeric());
}

int string_compare(LPCSTR left, LPCSTR right, TVTIndexDescrRecord const& IdxRec)
{
	if( IdxRec.NoCaseStringCompare )
	{
		if( IdxRec.TrimStrings )
			return FastCompareNoCase.CompareTrimSpc(left, right);
		else
			return FastCompareNoCase.Compare(left, right);
	}
	else if ( IdxRec.TrimStrings )
		return FastCompare.CompareTrimSpc(left, right);
	else
		return FastCompare.Compare(left, right);
}

int cv_string_compare(CValue& left, CValue& right, TVTIndexDescrRecord const& IdxRec)
{
	return string_compare((LPCSTR)CString(left.Format()), (LPCSTR)CString(right.Format()), IdxRec);
}

int date_compare(CDate const& d1, CDate const& d2)
{
	int comp_res = numeric_compare(d1.GetYear(), d2.GetYear());
	if( comp_res ) return comp_res;
	comp_res = numeric_compare(d1.GetMonth(), d2.GetMonth());
	if( comp_res ) return comp_res;
	comp_res = numeric_compare(d1.GetMonthDay(), d2.GetMonthDay());
	return comp_res;
}

int time_compare(CEventTime const& t1, CEventTime const& t2)
{
	if( t1 < t2 ) return -1;
	else if( t1 > t2 ) return 1;
	return 0;
}

inline DWORD gettypeid(CValue const &cv)
{
	return cv.m_mdid ? cv.m_mdid : cv.ValTypeID;
}

int inner_compare(CValue &left, CValue &right)
{
	int diff;
	if( !(diff = gettypeid(left) - gettypeid(right)) )
	{
		if( !(diff = left.m_ObjID.GetlObjID() - right.m_ObjID.GetlObjID()) )
			diff = FastCompare.Compare(left.m_ObjID.DBSign.Sign, right.m_ObjID.DBSign.Sign);
	}
	return diff;
}


int CValue_compare(CValue &left, CValue &right, TVTIndexDescrRecord const& IdxRec)
{
	int nTypeCodeLeft = left.GetTypeCode();
	int nTypeCodeRight = right.GetTypeCode();
	if( nTypeCodeLeft == nTypeCodeRight )
	{
		switch( nTypeCodeLeft )
		{
		case UNDEFINE_TYPE_1C:
			return 0;
			break;
		case NUMBER_TYPE_1C:
			return cv_numeric_compare(left, right);
			break;
		case STRING_TYPE_1C:
			return string_compare(left.m_String.operator LPCSTR(), right.m_String.operator LPCSTR(), IdxRec);
			break;
		case DATE_TYPE_1C:
			{
				CDate &d1 = left.GetDate(), &d2 = right.GetDate();
				return date_compare(d1, d2);
			}
			break;
		case REFERENCE_TYPE_1C:
			{
				if( IdxRec.CompareType == CmpByInnerRepr )
					return inner_compare(left, right);
				
				// учтено время жизни указателей
				int comp_res = cv_string_compare(left, right, IdxRec);
				if( comp_res == 0 )
					comp_res = inner_compare(left, right);

				return comp_res;
			}
			break;
		case DOCUMENT_TYPE_1C:
			if( IdxRec.CompareType == CmpByInnerRepr )
			{
				return inner_compare(left, right);
			}
			else
			{
				CDate d1(0,0,0), d2(0,0,0);
				CEventTime t1, t2;
				GetDateTimeFromValue(left, d1, t1);
				GetDateTimeFromValue(right, d2, t2);
				int comp_res = date_compare(d1, d2);
				if( comp_res == 0 ) comp_res = time_compare(t1, t2);
				return comp_res;
			}
			break;

		case AGREGATE_TYPE_1C:
			{
				if( IdxRec.CompareType == CmpByInnerRepr )
					return left.m_Context - right.m_Context;

				int comp_res = cv_string_compare(left, right, IdxRec);
				if( comp_res == 0 )
					comp_res = left.m_Context - right.m_Context;

				return comp_res;
			}
			break;

		default:
			if( IdxRec.CompareType == CmpByInnerRepr )
				return inner_compare(left, right);
			else
				return cv_string_compare(left, right, IdxRec);
		}
	}
	else
	{
		return numeric_compare(nTypeCodeLeft, nTypeCodeRight);
	}
}

int CVTIndexRecord::compare(CVTIndexRecord& RightRecord)
{
	int sz = IndexFields->size();
	for( int i = 0; i < sz; i++ )
	{
		int comp_res;
		TVTIndexDescrRecord &IdxRec = IndexFields->operator[](i);

		CValue &left = Row->GetValue(IdxRec.ColumnNumber);
		CValue &right = RightRecord.Row->GetValue(IdxRec.ColumnNumber);

/*
		try{
			comp_res = CValue_compare(left, right, IdxRec);
			
		} catch(...) 
		{
			RuntimeError("Ошибка памяти при сравнении строк в индексе. rows: %p vs %p; values: %p vs %p", 
				Row, RightRecord.Row, &left, &right);
		}
		
		if( comp_res != 0 ) return comp_res * IdxRec.Direction;
*/
		comp_res = CValue_compare(left, right, IdxRec);
		if (comp_res)
		{
			if (IdxRec.Direction < 0)
				comp_res =-comp_res;

			return comp_res;
		}
	}

	return 0;
}


/////////////////////////////////////////////////////////////
////  CVTExtIndexFields  ////////////////////////////////////
/////////////////////////////////////////////////////////////
CVTExtIndexFields::CVTExtIndexFields(CString& IndexExpr, CVTExtended* pVT)
{
	Build(IndexExpr, pVT);
}

CVTExtIndexFields& CVTExtIndexFields::operator= (CVTExtIndexFields const& src)
{
	vector<TVTIndexDescrRecord>::operator=(src);
	m_IndexExpr = src.m_IndexExpr;
	return *this;
}

bool CVTExtIndexFields::Build(CString const& IndexExpr, CVTExtended* pVT)
{
	m_IndexExpr = IndexExpr;

	clear();

	CStringArrayEx Fields;
	Fields.FillSeparateString(IndexExpr, ",");
	int nFields = Fields.GetSize();

	if( nFields == 0 )
	{
		return true; //позволим иметь "пустой" индекс, т.е. строки идут в порядке создания
	}

	resize(nFields);

	for( int i = 0; i < nFields; i++ )
	{
		CString& ColName = Fields[i];
		int direction = 1;
		TCompareType CompareType = CmpByStringRepr;
		bool TrimStrings = false, NoCaseStringCompare = false;
		
		operator[](i).CompareType = ColName.Find('*') < 0 ? CmpByStringRepr : CmpByInnerRepr;
		operator[](i).Direction = ColName.Find('-') < 0 ? 1 : -1;
		operator[](i).TrimStrings = ColName.Find('#') >= 0;
		operator[](i).NoCaseStringCompare = ColName.Find('^') >= 0;
		operator[](i).SumsBySubcontoGroups = ColName.Find('&') >= 0;
		
		ColName.Replace("-", "");
		ColName.Replace("*", "");
		ColName.Replace("#", "");
		ColName.Replace("^", "");
		ColName.Replace("&", "");

		if( pVT )
		{
			int col_num = pVT->ColumnNumber(ColName, false);
			if( col_num < 0 )
				RuntimeError("Колонка '%s' не существует!", ColName);

			operator[](i).ColumnNumber = col_num;
		}
	}

	return true;
}
/////////////////////////////////////////////////////////////
////  CVTExtIndex  //////////////////////////////////////////
/////////////////////////////////////////////////////////////
CVTExtIndex::CVTExtIndex(LPCSTR Name, CString const& IndexExpression, bool bUnique, CVTExtended* vt)
:KeyRow(vt->GetColumnsCount())
{
	SetName(Name);
	pVT = vt;
	pFilter = NULL;
	pIterator = NULL;

	IndexFields.Build(IndexExpression, pVT);
	NoRepeat = bUnique;
	Reindex();
}

CVTExtIndex::CVTExtIndex(LPCSTR Name, CVTExtIndexFields const& TheIndexFields, bool bUnique, CVTExtended* vt)
:KeyRow(vt->GetColumnsCount())
{
	SetName(Name);
	pVT = vt;
	pFilter = NULL;
	pIterator = NULL;

	IndexFields = TheIndexFields;
	NoRepeat = bUnique;
	Reindex();
}

CVTExtIndex::CVTExtIndex(LPCSTR Name, CVTExtended* vt)
:KeyRow(vt->GetColumnsCount())
{
	SetName(Name);
	pVT = vt;
	pFilter = NULL;
	pIterator = NULL;
	ResetPosition();
}

CVTExtIndex::~CVTExtIndex()
{
	if( pFilter   ) delete pFilter;
	if( pIterator ) delete pIterator;

	for(int i = 0; i < m_IteratorStack.GetSize(); i++ )
	{
		stIteratorState* pState = m_IteratorStack.GetAt(i);
		if (pState)
		{
			if (pState->m_pFilter)
				delete pState->m_pFilter;

			if (pState->m_pIterator)
				delete pState->m_pIterator;

			delete pState;
		}
	}
}

void CVTExtIndex::ResetPosition()
{
	nCurrentRow = -1;
	pCurrentRow = NULL;
}

CVTExtIndex& CVTExtIndex::operator= (CVTExtIndex& SrcIndex)
{
	NoRepeat = SrcIndex.NoRepeat;
	IndexFields = SrcIndex.IndexFields;
	Reindex();
	if( SrcIndex.pFilter )
	{
		if( pFilter == NULL )
			pFilter = new CVTExtFilter(KeyRow.GetSize(), &IndexFields);
		*pFilter = *(SrcIndex.pFilter);
	}
	return *this;
}

void CVTExtIndex::Reindex()
{
	RemoveAll();
	if( !IsIndexed() ) return;
	
	CVTIndexRecord idx_rec;
	idx_rec.IndexFields = &IndexFields;

	int num_rows = pVT->GetRowsCount();
	for( int i = 0; i < num_rows; i++ )
	{
		CVTExtRow *row = pVT->GetRow(i);
		idx_rec.Row = row;
		Insert(idx_rec, i);
	}

	ResetPosition();
}

void CVTExtIndex::Reindex(CString& IndexExpr)
{
	DropFilter();
	IndexFields.Build(IndexExpr, pVT);
	Reindex();
}

void CVTExtIndex::Reindex(CVTExtIndexFields& TheIndexFields)
{
	DropFilter();
	IndexFields = TheIndexFields;
	Reindex();
}

void CVTExtIndex::DropIndex()
{
	DropFilter();
	IndexFields.clear();
	Reindex();
}

bool CVTExtIndex::ColumnIndexed(int col_num)
{
	for( int i = IndexFields.size() - 1; i >= 0; i-- )
	{
		if( IndexFields[i].ColumnNumber == col_num ) return true;
	}
	return false;
}

void CVTExtIndex::RemoveRowFromTree(int row_num, int delta)
{
	if( !IsIndexed() ) return;

	CVTExtIndexTreeNode *node_to_delete = NULL;
	CVTExtIndexTreeNode *node = Min(Root);
	while( node )
	{
		if( node->ArrayEqualIDs && !node->ArrayEqualIDs->empty() )
		{
			vector<int> &Vector = *(node->ArrayEqualIDs);
			bool vector_erased = false;

			if( node->ID == row_num )
			{
				//Ключ ссылается на удаляемую строку, поэтому в качестве ключа нужно взять какую-то другую строку.
				//Возьмём первую строку из списка дублей.
				node->ID = Vector.back();
				node->Key.Row = pVT->GetRow(node->ID);

				//Vector.erase(Vector.begin());
				Vector.pop_back();
				vector_erased = true;
				node->Count--;
				Count--;
			}
			if( node->ID > row_num )
			{
				node->ID += delta;
			}

			vector<int>::iterator Iterator = Vector.begin();
			while( Iterator != Vector.end() )
			{
				if( *Iterator == row_num )
				{
					Iterator = Vector.erase(Iterator); //Удаляемая строка оказалась в списке дублей. Уберём её оттуда.
					vector_erased = true;
					node->Count--;
					Count--;
				}
				else
				{
					if( *Iterator > row_num )
					{
						*Iterator += delta;
					}
					Iterator++;
				}
			}


			if( vector_erased )
			{
				if( Vector.empty() )
				{
					delete node->ArrayEqualIDs;
					node->ArrayEqualIDs = NULL;
					UniqueCount++;
				}
				if( delta == 0 ) break; //если строка удаляется только из индекса, то дальше перебирать узлы смысла нет
			}
		}
		else if( node->ID == row_num )
		{
			node_to_delete = node;
			if( delta == 0 ) break; //если строка удаляется только из индекса, то дальше перебирать узлы смысла нет
		}
		else if( node->ID > row_num )
		{
			node->ID += delta;
		}
		node = Successor(node);
	}

	if( node_to_delete ) Delete(node_to_delete);
}

void CVTExtIndex::RemoveRowFromTree(int nRow)
{
	RemoveRowFromTree(nRow, 0);
}

void CVTExtIndex::RemoveRow(int nRow)
{
	//Если выборка открыта, и удаляемая строка является текущей, то нужно перепозиционироваться
	if( pIterator )
	{
		if( nRow == pIterator->CurrentRowIndex() )
		{
			Next();
		}
	}
	else
	{
		//если выборки нет - надо обнулить текущую позицию
		if( nRow == nCurrentRow )
		{
			ResetPosition();
		}
		else if( nCurrentRow > nRow )
		{
			nCurrentRow--;
		}
	}

	RemoveRowFromTree(nRow, -1);
}

void CVTExtIndex::RemoveAllRows()
{
	RemoveAll();
	ResetPosition();

	if( pFilter   ) delete pFilter;
	pFilter = NULL;
	if( pIterator ) delete pIterator;
	pIterator = NULL;
}

void CVTExtIndex::InsertRow(CVTExtRow* pRow, int nRow)
{
	nCurrentRow = nRow;
	pCurrentRow = pRow;

	if( IndexFields.size() == 0 ) return;

	CVTIndexRecord idx_rec;
	idx_rec.IndexFields = &IndexFields;
	idx_rec.Row = pRow;
	Insert(idx_rec, nRow);
}

void CVTExtIndex::OnChangeRow(int nRow)
{
	if( IsEmpty() ) return;

	RemoveRowFromTree(nRow, 0);
	InsertRow(pVT->GetRow(nRow), nRow);
}

void CVTExtIndex::OnChangeValue(int nRow, int nCol)
{
	if( IsEmpty() ) return;
	if( !ColumnIndexed(nCol) ) return;

	RemoveRowFromTree(nRow, 0);
	InsertRow(pVT->GetRow(nRow), nRow);
}

void CVTExtIndex::NewColumn()
{
	KeyRow.NewColumn();

	if( pFilter )
		pFilter->NewColumn();
}

void CVTExtIndex::RemoveColumn(int nCol)
{
	for( int i = IndexFields.size() - 1; i >= 0; i-- )
		if( IndexFields[i].ColumnNumber > nCol ) IndexFields[i].ColumnNumber--;

	KeyRow.RemoveColumn(nCol);

	if( pFilter )
		pFilter->RemoveColumn(nCol);
}

void FillSearchKey(CPtrArray* ppValues, CVTExtIndexFields *pIndexFields, CVTExtRow& KeyRow, CVTIndexRecord& IdxRec)
{
	if( ppValues->GetSize() < pIndexFields->size())
		RuntimeError("В ключе недостаточно значений!  Должно быть %i, а передано %i", pIndexFields->size(), ppValues->GetSize());

	IdxRec.IndexFields = pIndexFields;
	IdxRec.Row = &KeyRow;
	for( int i = pIndexFields->size() - 1; i >= 0; i-- )
	{
		int col_num = pIndexFields->operator[](i).ColumnNumber;
		CValue* val = (CValue*)(ppValues->GetAt(i));
		KeyRow.SetValue(col_num, *val);
	}
}

void FillSearchKey(CValue const& What, CVTExtIndexFields *pIndexFields, CVTExtRow& KeyRow, CVTIndexRecord& IdxRec)
{
	CPtrArray* ppVLArray = CValue2VL(What);
	if( ppVLArray == NULL && pIndexFields->size() > 1 )
		RuntimeError("В качестве ключа может использоваться только список значений!");

	if( ppVLArray == NULL && pIndexFields->size() == 1 )
	{
		IdxRec.IndexFields = pIndexFields;
		IdxRec.Row = &KeyRow;
		int col_num = pIndexFields->operator[](0).ColumnNumber;
		KeyRow.SetValue(col_num, What);
	}
	else
	{
		FillSearchKey(ppVLArray, pIndexFields, KeyRow, IdxRec);
	}
}

int CVTExtIndex::PositioningOnNode(CVTExtIndexTreeNode* node, bool FindLastRow, bool ChangePosition)
{
	if( node )
	{
		int FoundRow, FoundIndexInNode = 0;

		if( FindLastRow && node->ArrayEqualIDs )
		{
			FoundIndexInNode = node->ArrayEqualIDs->size();
			FoundRow = node->ArrayEqualIDs->operator[](FoundIndexInNode - 1);
		}
		else
		{
			FoundRow = node->ID;
		}

		if( ChangePosition )
		{
			nCurrentRow = FoundRow;
			pCurrentRow = pVT->GetRow(nCurrentRow);
			if( pIterator )
			{
				pIterator->SetPosition(node, FoundIndexInNode, pVT->GetRow(FoundRow), FoundRow);
			}
			else
			{
				pIterator = CreateIterator();
				pIterator->SetPosition(node, FoundIndexInNode, pVT->GetRow(FoundRow), FoundRow);
			}
		}

		return FoundRow;
	}
	return -1;
}

int CVTExtIndex::Find(CValue const& What, bool FindLastRow, bool ChangePosition)
{
	if( !IsIndexed() ) RuntimeError("Таблица не проиндексирована!");

	CVTIndexRecord IdxRec;
	FillSearchKey(What, &IndexFields, KeyRow, IdxRec);

	CVTExtIndexTreeNode* node = Search(IdxRec);
	return PositioningOnNode(node, FindLastRow, ChangePosition);
}

CVTExtIndexTreeNode* CVTExtIndex::Find(CPtrArray* pKeyValues)
{
	if( !IsIndexed() ) RuntimeError("Таблица не проиндексирована!");

	CVTIndexRecord IdxRec;
	FillSearchKey(pKeyValues, &IndexFields, KeyRow, IdxRec);
	return Search(IdxRec);
}

int CVTExtIndex::FindNearestGE(CValue const& What, bool ChangePosition)
{
	if( !IsIndexed() ) RuntimeError("Таблица не проиндексирована!");

	CVTIndexRecord IdxRec;
	FillSearchKey(What, &IndexFields, KeyRow, IdxRec);

	CVTExtIndexTreeNode* node = SearchNearestGE(IdxRec);
	return PositioningOnNode(node, false, ChangePosition);
}

int CVTExtIndex::FindNearestLE(CValue const& What, bool ChangePosition)
{
	if( !IsIndexed() ) RuntimeError("Таблица не проиндексирована!");

	CVTIndexRecord IdxRec;
	FillSearchKey(What, &IndexFields, KeyRow, IdxRec);

	CVTExtIndexTreeNode* node = SearchNearestLE(IdxRec);
	return PositioningOnNode(node, true, ChangePosition);
}

int CVTExtIndex::KeyCount(CValue const& What)
{
	if( !IsIndexed() ) RuntimeError("Таблица не проиндексирована!");

	CVTIndexRecord IdxRec;
	FillSearchKey(What, &IndexFields, KeyRow, IdxRec);
	CVTExtIndexTreeNode *node = Search(IdxRec);
	if( node )
		return node->Count;
	else
		return 0;
}

CVTExtFilter::CVTExtFilter(int ColumnsCount, CVTExtIndexFields *pIndexFields)
:Row_min(ColumnsCount), Row_max(ColumnsCount)
{
	Rec_min.IndexFields = pIndexFields;
	Rec_max.IndexFields = pIndexFields;
	compare_rec.IndexFields = pIndexFields;
	Rec_min.Row = &Row_min;
	Rec_max.Row = &Row_max;
	bOnlyUnique = false;
	bInverse = false;
}

CVTExtFilter::CVTExtFilter(int ColumnsCount, CVTExtIndexFields *pIndexFields, CValue const& Min, CValue const& Max)
:Row_min(ColumnsCount), Row_max(ColumnsCount)
{
	Rec_min.IndexFields = pIndexFields;
	Rec_max.IndexFields = pIndexFields;
	compare_rec.IndexFields = pIndexFields;
	Set(Min, Max);
	bOnlyUnique = false;
	bInverse = false;
}


void CVTExtFilter::Set(CValue const& Min, CValue const& Max)
{
	if( Min.GetTypeCode() == USHRT_MAX )
	{
		FillMinRow();
		FillSearchKey(Max, Rec_max.IndexFields, Row_max, Rec_max);
	}
	else if( Max.GetTypeCode() == USHRT_MAX )
	{
		FillSearchKey(Min, Rec_min.IndexFields, Row_min, Rec_min);
		FillMaxRow();
	}
	else
	{
		FillSearchKey(Min, Rec_min.IndexFields, Row_min, Rec_min);
		FillSearchKey(Max, Rec_max.IndexFields, Row_max, Rec_max);
	}
}

void CVTExtFilter::FillMinRow()
{
	Rec_min.Row = &Row_min;

	//Установим код типа так, чтобы записи гарантированно были мин
	CVTExtIndexFields& IndexFields = *(Rec_min.IndexFields);
	int sz = IndexFields.size();
	for( int i = 0; i < sz; i++ )
	{
		int nCol = IndexFields[i].ColumnNumber;
		if( IndexFields[i].Direction >= 0 )
		{
			Row_min.GetValue(nCol).type = 0;
		}
		else
		{
			Row_min.GetValue(nCol).type = USHRT_MAX;
		}
	}
}

void CVTExtFilter::FillMaxRow()
{
	Rec_max.Row = &Row_max;

	//Установим код типа так, чтобы записи гарантированно были макс.
	CVTExtIndexFields& IndexFields = *(Rec_max.IndexFields);
	int sz = IndexFields.size();
	for( int i = 0; i < sz; i++ )
	{
		int nCol = IndexFields[i].ColumnNumber;
		if( IndexFields[i].Direction >= 0 )
		{
			Row_max.GetValue(nCol).type = USHRT_MAX;
		}
		else
		{
			Row_max.GetValue(nCol).type = 0;
		}
	}
}


bool CVTExtFilter::CheckRow(CVTExtRow* pRow)
{
	int res = CompareRow(pRow);
	if( bInverse ) return res != 0;
	return res == 0;
}

int CVTExtFilter::CompareRow(CVTExtRow* pRow)
{
	compare_rec.Row = pRow;
	if( compare_rec.compare(Rec_min) < 0 ) return -1;
	if( compare_rec.compare(Rec_max) > 0 ) return 1;
	return 0;
}

CVTExtFilter& CVTExtFilter::operator= (CVTExtFilter& SrcFilter)
{
	Row_min = SrcFilter.Row_min;
	Row_max = SrcFilter.Row_max;
	compare_rec = SrcFilter.compare_rec;
	bOnlyUnique = SrcFilter.bOnlyUnique;
	bInverse = SrcFilter.bInverse;
	return *this;
}

void CVTExtIndex::SetFilter(CValue const& Min, CValue const& Max, bool bUnique, bool bInverse)
{
	if( !IsIndexed() )
		RuntimeError("Перед установкой фильтра таблица должна быть проиндексирована!");

	if( pFilter )
		pFilter->Set(Min, Max);
	else
		pFilter = new CVTExtFilter(pVT->GetColumnsCount(), &IndexFields, Min, Max);

	pFilter->bOnlyUnique = bUnique;
	pFilter->bInverse = bInverse;
}

void CVTExtIndex::SetFilter(CValue const& SubSet, int nCols)
{
	if( !IsIndexed() )
		RuntimeError("Перед установкой фильтра таблица должна быть проиндексирована!");

	CPtrArray* ppVLArray = CValue2VL(SubSet);
	if( ppVLArray == NULL )
		RuntimeError("В качестве ключа может использоваться только список значений!");

	int nMaxIndex;
	if( nCols >= 0 )
		nMaxIndex = nCols - 1;
	else
		nMaxIndex = min(ppVLArray->GetUpperBound(), IndexFields.size() - 1);
	if( nMaxIndex > ppVLArray->GetUpperBound() )
		RuntimeError("В ключе недостаточно значений!");
	if( nMaxIndex >= IndexFields.size() )
		RuntimeError("В ключе значений больше, чем в индексе!");

	if( pFilter == NULL )
		pFilter = new CVTExtFilter(pVT->GetColumnsCount(), &IndexFields);

	//Установим код типа так, чтобы записи гарантированно были мин и макс.
	int sz = IndexFields.size();
	for( int i = 0; i < sz; i++ )
	{
		int nCol = IndexFields[i].ColumnNumber;
		if( IndexFields[i].Direction >= 0 )
		{
			pFilter->Row_min.GetValue(nCol).type = 0;
			pFilter->Row_max.GetValue(nCol).type = USHRT_MAX;
		}
		else
		{
			pFilter->Row_min.GetValue(nCol).type = USHRT_MAX;
			pFilter->Row_max.GetValue(nCol).type = 0;
		}
	}

	//Заполним первые колонки индекса нормальными значениями
	for( i = 0; i <= nMaxIndex; i++ )
	{
		int col_num = IndexFields[i].ColumnNumber;
		CValue* val = (CValue*)(ppVLArray->GetAt(i));
		pFilter->Row_min.SetValue(col_num, *val);
		pFilter->Row_max.SetValue(col_num, *val);
	}
}

void CVTExtIndex::DropFilter()
{
	if( pFilter )
	{
		delete pFilter;
		pFilter = NULL;
	}
}

int CVTExtIndex::GetRowsCount(bool bUnique)
{
	if( !pFilter && !bUnique) return Count;

	bUnique = bUnique || pFilter->bOnlyUnique;
	int nRows = 0;
	CVTExtIterator* pIterator = CreateIterator();
	pIterator->First();
	while( pIterator->NextNode() )
	{
		if( bUnique )
			nRows++;
		else
			nRows += pIterator->CurrentNode()->Count;
	}
	delete pIterator;

	return nRows;
}

//-----------------------------------------------------------------------------
CVTExtIterator* CVTExtIndex::CreateIterator(bool bUnique)
{
	if( IsIndexed() )
	{
		if( pFilter == NULL )
			return new CVTExtIteratorIndex(this, bUnique);
		else
			return new CVTExtIteratorFilter(this, pFilter, bUnique);
	}
	else
	{
		return new CVTExtIteratorSimple(pVT);
	}
}

bool CVTExtIndex::First(bool bUnique)
{
	if( pIterator ) delete pIterator;

	pIterator = CreateIterator(bUnique);

	bool res = pIterator->First();
	nCurrentRow = pIterator->CurrentRowIndex();
	pCurrentRow = pIterator->CurrentRow();
	return res;
}

bool CVTExtIndex::Next(bool bUnique)
{
	if( !pIterator ) return false;
	bool res;
	if( IsIndexed() )
	{
		if( bUnique )
			res = pIterator->NextNode();
		else
			res = pIterator->Next();
	}
	else
		res = pIterator->Next();
	nCurrentRow = pIterator->CurrentRowIndex();
	pCurrentRow = pIterator->CurrentRow();
	return res;
}

bool CVTExtIndex::Last(bool bUnique)
{
	if( pIterator ) delete pIterator;

	pIterator = CreateIterator(bUnique);

	bool res = pIterator->Last();
	nCurrentRow = pIterator->CurrentRowIndex();
	pCurrentRow = pIterator->CurrentRow();
	return res;
}

bool CVTExtIndex::Prev(bool bUnique)
{
	if( !pIterator ) return false;
	bool res;
	if( IsIndexed() )
	{
		if( bUnique )
			res = pIterator->PrevNode();
		else
			res = pIterator->Prev();
	}
	else
		res = pIterator->Prev();
	nCurrentRow = pIterator->CurrentRowIndex();
	pCurrentRow = pIterator->CurrentRow();
	return res;
}

//-----------------------------------------------------------------
void CVTExtIndex::SetCurrentRow(int nRow)
{
	if( nRow < 0 )
	{
		ResetPosition();
		return;
	}

	nCurrentRow = nRow;
	pCurrentRow = pVT->GetRow(nRow);

	if( IsIndexed() )
	{
		CVTExtIndexTreeNode *node = Min(Root);
		CVTExtIndexTreeNode *found_node = NULL;
		while( node )
		{
			if( node->ID == nCurrentRow )
			{
				found_node = node;
				break;
			}
			else if( node->ArrayEqualIDs && !node->ArrayEqualIDs->empty() )
			{
				vector<int> &Vector = *(node->ArrayEqualIDs);
				for( int i = Vector.size() - 1; i >= 0; i-- )
				{
					if( Vector[i] == nCurrentRow )
					{
						found_node = node;
						break;
					}
				}
				if( found_node ) break;
			}
			node = Successor(node);
		}
	}
}

void CVTExtIndex::SetCurrentValue(int nCol, CValue const& NewVal)
{
	if( pCurrentRow != NULL )
	{
		pCurrentRow->SetValue(nCol, NewVal);
	}
	else
	{
		RuntimeError("Нет текущей строки!");
	}
}

void CVTExtIndex::GetCurrentValue(int nCol, CValue& RetVal)
{
	if( pCurrentRow != NULL )
		RetVal = pCurrentRow->GetValue(nCol);
	else
		RuntimeError("Нет текущей строки!");
}

//----------------------------------------------------------------------------
void CVTExtIndex::GroupBy(CString Columns)
{
	CVTExtIndexFields SumFields;
	if( !SumFields.Build(Columns, pVT) ) return;

	int nColumns = SumFields.size(), i;
	CVTExtRow *pRow, *pSumRow;
	bool *DeletedRows = new bool[pVT->GetRowsCount()];
	memset((void*)DeletedRows, 0, sizeof(bool) * pVT->GetRowsCount());

	CNumeric** pSums = new CNumeric*[nColumns];

	CVTExtIterator* pIterator = CreateIterator();
	pIterator->First();
	while( pIterator->NextNode() )
	{
		CVTExtIndexTreeNode* pNode = pIterator->CurrentNode();
		if( pNode->ArrayEqualIDs )
		{
			pSumRow = pVT->GetRow(pNode->ID);
			for( i = 0; i < nColumns; i++ )
			{
				CValue& val = pSumRow->GetValue(SumFields[i].ColumnNumber);
				if( val.GetTypeCode() != NUMBER_TYPE_1C )
					val = 0L;
				pSums[i] = &(val.m_Number);
			}

			for( int nRow = pNode->ArrayEqualIDs->size() - 1; nRow >= 0; nRow-- )
			{
				int nRealRowNum = pNode->ArrayEqualIDs->operator[](nRow);
				pRow = pVT->GetRow(nRealRowNum);
				for( i = 0; i < nColumns; i++ )
				{
					int nCol = SumFields[i].ColumnNumber;
					CNumeric& val = *(pSums[i]);
					CValue& add = pRow->GetValue(nCol);
					if( add.GetTypeCode() == NUMBER_TYPE_1C )
						val = val + add.m_Number;
				}
				DeletedRows[nRealRowNum] = true;
			}

			delete pNode->ArrayEqualIDs;
			pNode->ArrayEqualIDs = NULL;
		}
	}

	for( i = pVT->GetRowsCount() - 1; i >= 0; i-- )
	{
		if( DeletedRows[i] )
		{
			delete pVT->GetRow(i);
			pVT->Rows.RemoveAt(i);
		}
	}

	delete pIterator;
	delete[] DeletedRows;
	delete[] pSums;
}

void CVTExtIndex::Sum(int col_num, CValue& RetVal)
{
	if( IsIndexed() )
	{
		if( pFilter )
			SumByFilter(col_num, *pFilter, RetVal);
		else if( NoRepeat )
			SumByIndexIterator(col_num, RetVal);
		else
			SumAllRows(col_num, RetVal);
	}
	else
		SumAllRows(col_num, RetVal);

/*
	if( !IsIndexed() || pFilter == NULL )
	{
		RetVal = 0L;
		CNumeric& sum = (CNumeric&)(RetVal.GetNumeric());
		sum = 0L;
		for( int i = pVT->GetRowsCount() - 1; i >= 0; i-- )
			sum = sum + pVT->GetRow(i)->GetValue(col_num).GetNumeric();
		return;
	}

	//Sum by filter
	SumByFilter(col_num, *pFilter, RetVal);
	*/
}

void CVTExtIndex::NodeSum(int nCol, CValue& RetVal)
{
	RetVal = 0L;
	CNumeric& sum = (CNumeric&)(RetVal.GetNumeric());
	sum = 0L;
	CVTExtIndexTreeNode* pNode;
	if( pIterator && (pNode = pIterator->CurrentNode()) )
	{
		sum = sum + pVT->GetRow(pNode->ID)->GetValue(nCol).GetNumeric();
		if( pNode->ArrayEqualIDs )
		{
			for( int i = pNode->ArrayEqualIDs->size() - 1; i >= 0; i-- )
			{
				sum = sum + pVT->GetRow(pNode->ArrayEqualIDs->operator[](i))->GetValue(nCol).GetNumeric();
			}
		}
	}
}

void CVTExtIndex::SumByFilter(int nCol, CVTExtFilter& Filter, CValue& RetVal)
{
	RetVal = 0L;
	CNumeric& sum = (CNumeric&)(RetVal.GetNumeric());
	sum = 0L;

	bool bUnique = Filter.bOnlyUnique;
	CVTExtIteratorFilter* pIter = new CVTExtIteratorFilter(this, &Filter, false);

	pIter->First();
	while( pIter->NextNode() )
	{
		CVTExtIndexTreeNode *pNode = pIter->CurrentNode();
		sum = sum + pVT->GetRow(pNode->ID)->GetValue(nCol).GetNumeric();
		if( !bUnique && pNode->ArrayEqualIDs )
		{
			for( int i = pNode->ArrayEqualIDs->size() - 1; i >= 0; i-- )
				sum = sum + pVT->GetRow(pNode->ArrayEqualIDs->operator[](i))->GetValue(nCol).GetNumeric();
		}
	}
	delete pIter;
}

void CVTExtIndex::SumByIndexIterator(int nCol, CValue& RetVal)
{
	RetVal = 0L;
	CNumeric& sum = (CNumeric&)(RetVal.GetNumeric());
	sum = 0L;

	CVTExtIteratorIndex* pIter = new CVTExtIteratorIndex(this, false);

	pIter->First();
	while( pIter->NextNode() )
	{
		CVTExtIndexTreeNode *pNode = pIter->CurrentNode();
		sum = sum + pVT->GetRow(pNode->ID)->GetValue(nCol).GetNumeric();
		if( pNode->ArrayEqualIDs )
		{
			for( int i = pNode->ArrayEqualIDs->size() - 1; i >= 0; i-- )
				sum = sum + pVT->GetRow(pNode->ArrayEqualIDs->operator[](i))->GetValue(nCol).GetNumeric();
		}
	}
	delete pIter;
}

void CVTExtIndex::SumAllRows(int nCol, CValue& RetVal)
{
	RetVal = 0L;
	CNumeric& sum = (CNumeric&)(RetVal.GetNumeric());
	sum = 0L;
	for( int i = pVT->GetRowsCount() - 1; i >= 0; i-- )
		sum = sum + pVT->GetRow(i)->GetValue(nCol).GetNumeric();
	return;
}

void CVTExtIndex::SumByRange(int col_num, CValue& min, CValue& max, CValue& RetVal)
{
	CVTExtFilter Filter(pVT->GetColumnsCount(), &IndexFields, min, max);
	SumByFilter(col_num, Filter, RetVal);
}

int CVTExtIndex::SaveIterator()
{
	int nIndex = -1;
	if (pIterator)
	{
		stIteratorState* pState = new stIteratorState(pFilter, pIterator);
		nIndex = m_IteratorStack.Add(pState);
		pFilter = NULL;
		pIterator = NULL;
	}

	return nIndex;
}

BOOL CVTExtIndex::RestoreIterator(int nIndex)
{
	BOOL bSuccess = FALSE;
	int nUpperBound = m_IteratorStack.GetUpperBound();
	if (nIndex < 0)
		nIndex = nUpperBound;

	if (nIndex >= 0 && nIndex <= nUpperBound)
	{
		stIteratorState* pState = m_IteratorStack.GetAt(nIndex);
		if (pState)
		{
			if (pFilter)
				delete pFilter;
			
			if (pIterator)
				delete pIterator;
			
			pFilter = pState->m_pFilter;
			pIterator = pState->m_pIterator;
			nCurrentRow = pIterator->CurrentRowIndex();
			pCurrentRow = pIterator->CurrentRow();
			
			delete pState;
			bSuccess = TRUE;
			
			if (nIndex == nUpperBound)
				m_IteratorStack.RemoveAt(nIndex);
			else
				m_IteratorStack.SetAt(nIndex, NULL);
		}
	}
	return bSuccess;
}

///////////////////////////////////////////////////////////////////////////
////  CVTExtIndexCollection  //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
CVTExtIndex* CVTExtIndexCollection::operator [](int nIndex) const
{
	return (CVTExtIndex*)(CCollection::operator [](nIndex));
}

CVTExtIndex* CVTExtIndexCollection::operator [](LPCSTR szName) const
{
	return (CVTExtIndex*)(CCollection::operator [](szName));
}

CVTExtIndex* CVTExtIndexCollection::operator [](CValue* Index) const
{
	return GetAt(Index, FALSE);
}

CVTExtIndex* CVTExtIndexCollection::GetAt(CValue* Index, BOOL bQuiet) const
{
	CVTExtIndex* pVTIndex = NULL;
	switch( Index->GetTypeCode() )
	{
	case NUMBER_TYPE_1C:
		{
			int nIndex = (int)(Index->GetNumeric());
			pVTIndex = operator [](nIndex - 1);
			if( !bQuiet && pVTIndex == NULL )
				RuntimeError("Индекс с номером %i не найден !", nIndex);
		}
		break;
	case STRING_TYPE_1C:
		{
			LPCSTR szIndex = Index->GetString();
			pVTIndex = operator [](szIndex);
			if( !bQuiet && pVTIndex == NULL )
				RuntimeError("Индекс с именем %s не найден !", szIndex);
		}
		break;
	default:
		RuntimeError("Номер индекса может быть либо числом либо строкой!  (%i)", Index->GetTypeCode());
	}

	return pVTIndex;
}

int CVTExtIndexCollection::AddIndex(CVTExtIndex* pIndex)
{
	Add(pIndex);
	return GetCount() - 1;
}

int CVTExtIndexCollection::AddIndex(CString const& IndexName, CVTExtIndexFields const& IndexFields, bool bUnique, CVTExtended* pVT)
{
	if( IndexName.IsEmpty() )
		RuntimeError("Не указано имя индекса!");
	if( GetItem(IndexName) != NULL )
		RuntimeError("Индекс %s уже есть!", IndexName);

	CVTExtIndex* pIndex = new CVTExtIndex(IndexName, IndexFields, bUnique, pVT);
	return AddIndex(pIndex);
}

int CVTExtIndexCollection::AddIndex(CString const& IndexName, CString& IndexExpr, bool bUnique, CVTExtended* pVT)
{
	if( IndexName.IsEmpty() )
		RuntimeError("Не указано имя индекса!");
	if( GetItem(IndexName) != NULL )
		RuntimeError("Индекс %s уже есть!", IndexName);

	CVTExtIndex* pIndex = new CVTExtIndex(IndexName, IndexExpr, bUnique, pVT);
	return AddIndex(pIndex);
}


void CVTExtIndexCollection::RemoveIndex(int nIndex)
{
	CVTExtIndex* pIndex = operator[](nIndex);
	Remove(nIndex);
	delete pIndex;
}

void CVTExtIndexCollection::RemoveIndex(CVTExtIndex* pIndex)
{
	int nIndex = GetIndex(pIndex);
	Remove(nIndex);
	delete pIndex;
}

void CVTExtIndexCollection::RemoveAll()
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		RemoveIndex(i);
	}
}

void CVTExtIndexCollection::Reindex()
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->Reindex();
	}
}

bool CVTExtIndexCollection::ColumnIndexed(int col_num)
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		if( operator[](i)->ColumnIndexed(col_num) ) return true;
	}
	return false;
}

void CVTExtIndexCollection::NewColumn()
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->NewColumn();
	}
}

void CVTExtIndexCollection::RemoveColumn(int col_num)
{
	if( ColumnIndexed(col_num) )
		RuntimeError("Нельзя удалять колонку, которая учавствует в индексах!");

	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->RemoveColumn(col_num);
	}
}


void CVTExtIndexCollection::InsertRow(CVTExtRow* row, int row_num)
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->InsertRow(row, row_num);
	}
}

void CVTExtIndexCollection::RemoveRow(int row_num)
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->RemoveRow(row_num);
	}
}

void CVTExtIndexCollection::RemoveAllRows()
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->RemoveAllRows();
	}
}

void CVTExtIndexCollection::OnChangeValue(int row_num, int col_num)
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->OnChangeValue(row_num, col_num);
	}
}

void CVTExtIndexCollection::OnChangeRow(int row_num)
{
	for( int i = GetUpperBound(); i >= 0; i-- )
	{
		operator[](i)->OnChangeRow(row_num);
	}
}
