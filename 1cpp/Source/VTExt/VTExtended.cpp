//////////////////////////////////////////////////////////////////////
// Класс CVTExtended, реализующий тип 1С "ИндексированнаяТаблица"
//
// Автор: Диркс Алексей aka ADirks
// Контакты: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////

#include <errno.h>
#include "stdafx.h"
#include "VTExtended.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////
////  CVTExtended  ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CVTExtended)
    BL_METH("NewColumn",     "НоваяКолонка",     1, NULL,    funcNewColumn, NULL)
    BL_METH("RenameColumn",  "ПереименоватьКолонку", 2, procRenameColumn, NULL, NULL)
    BL_METH("RemoveColumn",  "УдалитьКолонку",   1, procRemoveColumn, NULL, NULL)
    BL_METH("ColumnCount",   "КоличествоКолонок",0, NULL,    funcGetColCount,    NULL)
    BL_METH("ColumnName",    "ИмяКолонки",       1, NULL,    funcGetColumnName, NULL)
    BL_METH("ColumnNumber",  "НомерКолонки",     1, NULL,    funcGetColumnNumber, NULL)

    BL_METH("NewRow",        "НоваяСтрока",     0, NULL,    funcNewRow, NULL)
    BL_METH("RemoveRow",     "УдалитьСтроку",   1, procRemoveRow, NULL, NULL)
	BL_METH_PROC_DEF_PARAM("RemoveRows",    "УдалитьСтроки",   1, procRemoveAllRows, defsRemoveAllRows)
    BL_METH("RowCount",      "КоличествоСтрок", 2, NULL,    funcGetRowsCount,    defsGetRowsCount)
	BL_METH_FUNC_DEF_PARAM("RowNumber", "НомерСтроки",   1, funcGetCurrentRow, defsGetCurrentRow)

    BL_METH("Cleanup",       "Очистить",       0, procCleanup, NULL, NULL)

	BL_METH_FUNC_DEF_PARAM("AddIndex", "ДобавитьИндекс",  3, funcAddIndex, defsAddIndex)
    BL_METH("DropIndex",     "УдалитьИндекс",   1, procDropIndex, NULL, NULL)
    BL_METH("IndexCount",    "КоличествоИндексов",0, NULL,    funcGetIndexCount, NULL)
    BL_METH("IndexName",     "ИмяИндекса",       1, NULL,     funcGetIndexName, NULL)
    BL_METH("IndexExpr",     "ВыражениеИндекса", 1, NULL,     funcGetIndexExpr, NULL)
    BL_METH("IndexNumber",   "НомерИндекса",     1, NULL,     funcGetIndexNumber, NULL)

	BL_METH_FUNC_DEF_PARAM("SaveIterator",		"СохранитьИтератор",	1,	funcSaveIterator, defsSaveIterator)
	BL_METH_FUNC_DEF_PARAM("RestoreIterator",	"ВосстановитьИтератор",	2,	funcRestoreIterator, defsRestoreIterator)

    BL_METH("Sort",          "Сортировать",     1, procReindex, NULL, NULL)

	BL_METH_FUNC_DEF_PARAM("IndexIsUnique",  "ИндексУникален",  1, funcIndexIsUnique, defsIndexIsUnique)
	BL_METH_FUNC_DEF_PARAM("UniqueKeyCount", "КоличествоУникальныхКлючей",  1, funcUniqueKeyCount, defsUniqueKeyCount)
	BL_METH_FUNC_DEF_PARAM("KeyValueCount",  "КоличествоЗначенийКлюча",  2, funcKeyValueCount, defsKeyValueCount)

    BL_METH("FindRow",       "НайтиСтроку",          4, NULL, funcFindRow, defsFindRow)
    BL_METH("FindNearestGE", "НайтиБлижайшуюБольше", 3, NULL, funcFindNearestGE, defsFindNearest)
    BL_METH("FindNearestLE", "НайтиБлижайшуюМеньше", 3, NULL, funcFindNearestLE, defsFindNearest)

    BL_METH("SetFilter",   "УстановитьФильтр", 5, procSetFilter,  NULL, defsSetFilter)
    BL_METH("Subset",      "Подмножество",     3, procSetFilter2, NULL, defsSetFilter)
	BL_METH_PROC_DEF_PARAM("DropFilter",  "ВыключитьФильтр",  1, procDropFilter, defsDropFilter)

    BL_METH("SelectRows",  "ВыбратьСтроки",   2, NULL, funcFirstRow, defsNextRow)
    BL_METH("FirstRow",    "ВНачало",         2, NULL, funcFirstRow, defsNextRow)
    BL_METH("LastRow",     "ВКонец",          2, NULL, funcLastRow, defsNextRow)
    BL_METH("GetRow",      "ПолучитьСтроку",  2, NULL, funcNextRow, defsNextRow)
    BL_METH("Next",        "СледующаяСтрока", 2, NULL, funcNextRow, defsNextRow)
    BL_METH("Previous",    "ПредыдущаяСтрока",2, NULL, funcPrevRow, defsNextRow)

	BL_METH_FUNC_DEF_PARAM("Get",	"Получить",    2, funcGetValue, defs_Get_Set_Value)
	BL_METH_FUNC_DEF_PARAM("GetValue",    "ПолучитьЗначение",    2, funcGetValue, defs_Get_Set_Value)
	BL_METH_PROC_DEF_PARAM("Set",         "Установить",         3, procSetValue, defs_Get_Set_Value)
	BL_METH_PROC_DEF_PARAM("SetValue",    "УстановитьЗначение", 3, procSetValue, defs_Get_Set_Value)

    BL_METH("FillRow",     "ЗаполнитьСтроку",   4, procFillRow, NULL, defsFillRow)
	BL_METH_PROC_DEF_PARAM("FillColumn",  "ЗаполнитьКолонку",  5, procFillColumn, defsFillColumn)

	BL_METH_PROC_DEF_PARAM("GroupBy",     "Свернуть",           3, procGroupBy, defsGroupBy)
	BL_METH_FUNC_DEF_PARAM("Sum", "Итог",		2, funcSum, defsSum)
	BL_METH_FUNC_DEF_PARAM("NodeSum",     "ИтогПоУзлу",         2, funcNodeSum, defsNodeSum)
	BL_METH_FUNC_DEF_PARAM("SumByRange",  "ИтогПоДиапазону",    4, funcSumByRange, defsSumByRange)
	BL_METH("Group",       "Группировать",       4, procGroup, NULL, defsGroup)

	BL_METH("Unload",      "Выгрузить",          4, procUnloadTo, NULL, defsUnloadTo)
    BL_METH("Load",        "Загрузить",          3, procLoadFrom, NULL, defsLoadFrom)
    BL_METH("LoadQuery",   "ЗагрузитьЗапрос",    3, procLoadFromQuery, NULL, defsLoadFromQuery)
    BL_METH("Union",       "Объединить",         3, procUnionVT, NULL, defsUnionVT)
    BL_METH("Conjunction", "Пересечение",        4, procConjunctionVT, NULL, defsConjunctionVT)
    BL_METH("Difference",  "Разность",           4, procDifferenceVT, NULL, defsConjunctionVT)
    //BL_METH("Subtract",    "Вычесть",            2, procSubtractVT, NULL, NULL)
    //BL_METH("NotConjunction", "НеПересечение",   2, procNotConjunctionVT, NULL, NULL)
    BL_METH("LeftJoin", "ЛевоеСоединение",        4, procLeftJoin, NULL, defsLeftJoin)
    BL_METH("RightJoin", "ПравоеСоединение",        4, procRightJoin, NULL, defsRightJoin)
    BL_METH("InnerJoin", "ВнутреннееСоединение",        4, procInnerJoin, NULL, defsInnerJoin)
    BL_METH("FullJoin", "ПолноеСоединение",        4, procFullJoin, NULL, defsFullJoin)

    BL_METH("Copy",        "Копия",              2, NULL, funcCopy, defsCopy)

    BL_METH("SaveToFile",  "ВФайл",              4, procSaveToFile, NULL, defsSaveToFile)
    BL_METH("LoadFromFile","ИзФайла",            3, NULL, funcLoadFromFile, defsLoadFromFile)
    
	BL_METH("SaveToString",  "ВСтроку",          2, NULL, funcSaveToString, defsSaveToString)
    BL_METH("LoadFromString","ИзСтроки",         1, procLoadFromString, NULL, NULL)
    
    BL_METH("ChooseLine",  "ВыбратьСтроку",      4, NULL, funcChooseLine, defsChooseLine)
	BL_METH_FUNC_DEF_PARAM("Show",        "Показать",           1, funcShowTable, defsShowTable)

    BL_METH("MemInfo",      "Память",         1, NULL, funcCRTInfo, NULL)
    BL_METH("HeapInfo",     "Куча",           1, NULL, funcHeapInfo, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CVTExtended)
//    BL_PROP("CurrentRow", "НомерСтроки", propGetCurrentRow, propSetCurrentRow)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CVTExtended, "IndexedTable", "ИндексированнаяТаблица", 1, NULL, NULL, -1);

enum VTExt_FixedProp {
	VTExt_FixedProp_CurrentRowEng,
	VTExt_FixedProp_CurrentRowRus,
	VTExt_FixedProp_Last
};
CIStringMapToIndex CVTExtended::FixedProps;

///////////////////////////////////////////////////////////////////////////
CVTExtended::CVTExtended()
{
	if( FixedProps.GetCount() == 0 )
	{
		FixedProps["RowNumber"] = VTExt_FixedProp_CurrentRowEng;
		FixedProps["НомерСтроки"] = VTExt_FixedProp_CurrentRowRus;
	}

	DefaultIndex = new CVTExtIndex("", this); // TODO а где удаление ?? (а удаление в объекте Indexes, см. след. строку)
	Indexes.Add(DefaultIndex);
}

CVTExtended::~CVTExtended()
{
	for( int i = Rows.GetUpperBound(); i >= 0; i-- )
	{
		delete GetRow(i);
	}
}


BOOL CVTExtended::procCleanup(CValue** params)
{
	//Дефолтный индекс должен остаться!
	for( int i = Indexes.GetUpperBound(); i > 0; i-- )
	{
		Indexes.RemoveIndex(i);
	}
	DefaultIndex->DropIndex();

	ColumnNames.RemoveAll();

	for( i = Rows.GetUpperBound(); i >= 0; i-- )
	{
		delete GetRow(i);
	}
	Rows.RemoveAll();

	return TRUE;
}

//===========================================================================
// Exported methods and props
//===========================================================================
int CVTExtended::NewColumn(CString const& ColumnName)
{
	int ColNum;

	//validity checks
	if( ColumnName.IsEmpty() )
	{
		RuntimeError("Не задано имя колонки!");
		return -1;
	}
	if( ColumnNames.Lookup(ColumnName, ColNum) )
	{
		RuntimeError("Колонка %s уже есть!", ColumnName);
		return -1;
	}

	int nColNum = ColumnNames.Add(ColumnName);

	//adding a column into each row
	for( int i = Rows.GetUpperBound(); i >= 0; i-- )
	{
		GetRow(i)->NewColumn();
	}

	//inform all indexes that column added
	Indexes.NewColumn();

	return nColNum;
}

BOOL CVTExtended::funcNewColumn(CValue& RetVal, CValue** params)
{
	CString const& ColName = params[0]->GetString();
	int ColNum = NewColumn(ColName);

	RetVal = ColNum + 1;

	return TRUE;
}

BOOL CVTExtended::procRenameColumn(CValue** params)
{
	int nCol = ColumnNumber(params[0]);
	CheckBounds_Col(nCol);

	CString NewName = params[1]->GetString();
	if( NewName.IsEmpty() )
		RuntimeError("Не задано новое имя колонки");
	if( ColumnNumber(NewName, true) >= 0 )
		RuntimeError("Колонка '%s' уже есть в таблице", NewName);

	CString OldName = ColumnName(nCol);
	ColumnNames.Remove(OldName);
	ColumnNames.Insert(nCol, NewName);

	return TRUE;
}

void CVTExtended::RemoveColumn(int nCol, bool bCheckIndexes)
{
	if( bCheckIndexes ) Indexes.RemoveColumn(nCol);

	for( int i = Rows.GetUpperBound(); i >= 0; i-- )
		GetRow(i)->RemoveColumn(nCol);

	//удалим ключ из хэша
	ColumnNames.Remove(nCol);
}

BOOL CVTExtended::procRemoveColumn(CValue** params)
{
	int nCol = ColumnNumber(params[0]);
	CheckBounds_Col(nCol);
	RemoveColumn(nCol);
	return TRUE;
}

BOOL CVTExtended::funcGetColumnName(CValue& RetVal, CValue** params)
{
	int col_num = ColumnNumber(params[0]);
	CheckBounds_Col(col_num);
	RetVal = ColumnName(col_num);
	return TRUE;
}

BOOL CVTExtended::funcGetColumnNumber(CValue& RetVal, CValue** params)
{
	RetVal = 1L + ColumnNumber(params[0], true);
	return TRUE;
}

int CVTExtended::NewRow(BOOL bIndex)
{
	CVTExtRow* row = new CVTExtRow(ColumnNames.GetCount());
	int row_num = Rows.Add(row);
	if (bIndex)
		Indexes.InsertRow(row, row_num);

	return row_num;
}

void CVTExtended::RemoveRow(int nRow, BOOL bIndex)
{
	if( bIndex )
		Indexes.RemoveRow(nRow);

	delete GetRow(nRow);
	Rows.RemoveAt(nRow);
}

BOOL CVTExtended::funcNewRow(CValue& RetVal, CValue** params)
{
	RetVal = 1L + NewRow();
	//DefaultIndex->SetCurrentRow(row_num);
	return TRUE;
}

BOOL CVTExtended::procRemoveRow(CValue** params)
{
	int nRow = (int)(params[0]->GetNumeric()) - 1;
	CheckBounds_Row(nRow);

	RemoveRow(nRow, true);

	return TRUE;
}

BOOL CVTExtended::defsRemoveAllRows(int nParam, CValue* param) const
{
	if(0 == nParam ){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::procRemoveAllRows(CValue** params)
{
	CVTExtIndex *pIndex = NULL;
	if( params != NULL && !(params[0]->IsEmpty()) ) pIndex = GetIndex(params[0]);
	if( pIndex != NULL && pIndex->pFilter == NULL ) pIndex = NULL;

	if( pIndex == NULL )
	{
		Indexes.RemoveAllRows();
		for( int i = Rows.GetUpperBound(); i >= 0; i-- )
		{
			delete GetRow(i);
		}
		Rows.RemoveAll();
	}
	else
	{
		bool *pDeletedRows = new bool[GetRowsCount()];
		memset((void*)pDeletedRows, 0, sizeof(bool) * GetRowsCount());

		CVTExtIterator *pIterator = pIndex->CreateIterator();
		pIterator->First();
		while( pIterator->Next() ) pDeletedRows[pIterator->CurrentRowIndex()] = true;

		for( int i = GetRowsCount() - 1; i >= 0; i-- )
		{
			if( pDeletedRows[i] )
			{
				delete GetRow(i);
				Rows.RemoveAt(i);
			}
		}
		Indexes.Reindex();
		delete[] pDeletedRows;
	}

	return TRUE;
}

BOOL CVTExtended::defsGetRowsCount(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0:
		*param = "";
		return TRUE;
		break;
	case 1:
		*param = 0L;
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL CVTExtended::funcGetRowsCount(CValue& RetVal, CValue** params)
{
	CVTExtIndex *pIndex = GetIndex(params[0]);
	bool bUnique = CValue2Bool(*params[1]);

	if( pIndex->IsEmpty() )
		RetVal = (long)GetRowsCount();
	else
		RetVal = pIndex->GetRowsCount(bUnique);
	return TRUE;
}

BOOL CVTExtended::defsGetCurrentRow(int nParam, CValue* param) const
{
	if(0 == nParam ){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcGetCurrentRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *pIndex = GetIndex(params[0]);
	RetVal = 1L + pIndex->GetCurrentRow();
	return TRUE;
}

BOOL CVTExtended::funcGetColCount(CValue& RetVal, CValue** params)
{
	RetVal = (long)GetColumnsCount();
	return TRUE;
}

BOOL CVTExtended::defs_Get_Set_Value(int nParam, CValue* param) const
{
	if(0 == nParam ){
		param->Reset();
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcGetValue(CValue& RetVal, CValue** params) 
{
	int nRow;
	if( params[0]->IsEmpty() )
		nRow = DefaultIndex->nCurrentRow;
	else
		nRow = (int)(params[0]->GetNumeric()) - 1;
	int nCol = ColumnNumber(params[1]);
	CheckBounds(nRow, nCol);
	RetVal = GetValue(nRow, nCol); 
	return TRUE;
}

BOOL CVTExtended::procSetValue(CValue** params)
{
	int row_num;
	if( params[0]->IsEmpty() )
		row_num = DefaultIndex->nCurrentRow;
	else
		row_num = (int)(params[0]->GetNumeric()) - 1;
	int col_num = ColumnNumber(params[1]);
	CheckBounds(row_num, col_num);
	SetValue(row_num, col_num, *(params[2])); 
	return TRUE;
}

int	CVTExtended::defsSum(int nParam, CValue* param)const
{
	if (1 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcSum(CValue& RetVal, CValue** params)
{
	int nCol = ColumnNumber(params[0]);
	CVTExtIndex *Index = GetIndex(params[1]);
	Index->Sum(nCol, RetVal);
	return TRUE;
}

int	CVTExtended::defsNodeSum(int nParam, CValue* param)const
{
	if (1 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcNodeSum(CValue& RetVal, CValue** params)
{
	int nCol = ColumnNumber(params[0]);
	CVTExtIndex *Index = GetIndex(params[1]);
	Index->NodeSum(nCol, RetVal);
	return TRUE;
}

int	CVTExtended::defsSumByRange(int nParam, CValue* param)const
{
	if (1 == nParam){
		*param = long(0);
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcSumByRange(CValue& RetVal, CValue** params)
{
	int nCol = ColumnNumber(params[0]);
	CVTExtIndex *Index = GetIndex(params[1]);
	Index->SumByRange(nCol, *params[2], *params[3], RetVal);
	return TRUE;
}

void CVTExtended::propSetCurrentRow(CValue const& Value)
{
	int row = Value.GetNumeric();
	row--;
	CheckBounds_Row(row);
	DefaultIndex->SetCurrentRow(row);
}

int	CVTExtended::defsAddIndex(int nParam, CValue* param)const
{
	if (2 == nParam){
		*param = 0L;
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcAddIndex(CValue& RetVal, CValue** params)
{
	CString IndexName = params[0]->GetString();
	CString IndexExpr = params[1]->GetString();
	bool bUnique = (int)(params[2]->GetNumeric()) == 0 ? false : true;
	RetVal = 1L + Indexes.AddIndex(IndexName, IndexExpr, bUnique, this);
	return TRUE;
}

BOOL CVTExtended::procDropIndex(CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	if( Index == DefaultIndex )
		RuntimeError("Нельзя удалять основной индекс!");
	Indexes.RemoveIndex(Index);
	return TRUE;
}

BOOL CVTExtended::funcGetIndexCount(CValue& RetVal, CValue** params)
{
	RetVal = (long)Indexes.GetCount();
	return TRUE;
}

BOOL CVTExtended::funcGetIndexName(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	RetVal = Index->GetName();
	return TRUE;
}

BOOL CVTExtended::funcGetIndexExpr(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	RetVal = Index->GetIndexExpr();
	return TRUE;
}

BOOL CVTExtended::funcGetIndexNumber(CValue& RetVal, CValue** params)
{
	int nIndex = 0;
	CVTExtIndex* Index = GetIndex(params[0], TRUE);
	if (Index)
		nIndex = Indexes.GetIndex(Index) + 1;

	RetVal = nIndex;
	return TRUE;
}

BOOL CVTExtended::defsSaveIterator(int nParam, CValue* param) const
{
	if (0 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcSaveIterator(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	RetVal = Index->SaveIterator() + 1;
	return TRUE;
}

BOOL CVTExtended::defsRestoreIterator(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		*param = "";
		return TRUE;
		break;
	case 1:
		*param = 0L;
		return TRUE;
		break;
	}

	return FALSE;
}

BOOL CVTExtended::funcRestoreIterator(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	
	int nIteratorIndex;
	switch (params[1]->GetTypeCode())
	{
	case NUMBER_TYPE_1C:
		nIteratorIndex = (LONG)params[1]->GetNumeric() - 1;
		break;
	case UNDEFINE_TYPE_1C:
		nIteratorIndex = -1;
		break;
	default:
		RuntimeError("Некорректный тип параметра 2");
	}

	BOOL bSuccess = Index->RestoreIterator(nIteratorIndex);
	RetVal = bSuccess ? 1L : 0L;
	return TRUE;
}

BOOL CVTExtended::procReindex(CValue** params)
{
	CString IndexExpr = params[0]->GetString();
	DefaultIndex->Reindex(IndexExpr);
	return TRUE;
}

BOOL CVTExtended::defsIndexIsUnique(int nParam, CValue* param) const
{
	if (0 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcIndexIsUnique(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	if( !Index->IsIndexed() )
		RuntimeError("Сортировка по индексу отсутствует!");
	RetVal = Index->Count == Index->UniqueCount ? 1L : 0L;
	return TRUE;
}

BOOL CVTExtended::defsUniqueKeyCount(int nParam, CValue* param) const
{
	if (0 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcUniqueKeyCount(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	if( !Index->IsIndexed() )
		RuntimeError("Сортировка по индексу отсутствует!");
	RetVal = Index->UniqueCount;
	return TRUE;
}

BOOL CVTExtended::defsKeyValueCount(int nParam, CValue* param) const
{
	if(0 == nParam ){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcKeyValueCount(CValue& RetVal, CValue** params)
{
	CVTExtIndex* Index = GetIndex(params[0]);
	if( !Index->IsIndexed() )
		RuntimeError("Сортировка по индексу отсутствует!");
	RetVal = Index->KeyCount(*(params[1]));
	return TRUE;
}

BOOL CVTExtended::defsFindRow(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 2: *param = 0L; return TRUE; break;
	case 3: *param = 0L; return TRUE; break;
	}
	return FALSE;
}

//Параметры:
// 1: имя индекса, или пустое значение (будет использован DefaultIndex). 
// 2: сзКлюч - список сначений, задающий значения ключевых колонок
// 3: 0 - найти первое вхождение, 1 - последнее
// 4: 1 - позиционироваться на найденной строке
//Возвращает: 0 - не найдено, номер строки - если найдено
BOOL CVTExtended::funcFindRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *pIndex = GetIndex(params[0]);
	CValue &What = *(params[1]);
	bool bFindLastRow = CValue2Bool(*params[2]);
	bool bChangePosition = CValue2Bool(*params[3]);
	
	int found = pIndex->Find(What, bFindLastRow, bChangePosition);
	RetVal = 1L + found;

	if( found >= 0 && bChangePosition )
		SetCurrentRow(pIndex);

	return TRUE;
}

BOOL CVTExtended::defsFindNearest(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 2: *param = 0L; return TRUE; break;
	}
	return FALSE;
}

//Параметры те же, что и у НайтиСтроку (funcFindRow), за исключением параметра 3
BOOL CVTExtended::funcFindNearestGE(CValue& RetVal, CValue** params)
{
	CValue &What = *(params[0]);
	CVTExtIndex *Index = GetIndex(params[1]);
	bool bChangePosition = CValue2Bool(*params[2]);
	
	int found = Index->FindNearestGE(What, bChangePosition);
	RetVal = 1L + found;
	if( found >= 0 && bChangePosition )
		SetCurrentRow(Index);

	return TRUE;
}

//Параметры те же, что и у НайтиСтроку (funcFindRow), за исключением параметра 3
BOOL CVTExtended::funcFindNearestLE(CValue& RetVal, CValue** params)
{
	CValue &What = *(params[0]);
	CVTExtIndex *Index = GetIndex(params[1]);
	bool bChangePosition = CValue2Bool(*params[2]);
	
	int found = Index->FindNearestLE(What, bChangePosition);
	RetVal = 1L + found;
	if( found >= 0 && bChangePosition )
		SetCurrentRow(Index);

	return TRUE;
}

BOOL CVTExtended::defsSetFilter(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0: param->SetTypeCode(USHRT_MAX); return TRUE; break;
	case 1: param->SetTypeCode(USHRT_MAX); return TRUE; break;
	case 2: *param = ""; return TRUE; break;
	case 3: *param = 0L; return TRUE; break;
	case 4: *param = 0L; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procSetFilter(CValue** params)
{
	CValue &Min = *(params[0]);
	CValue &Max = *(params[1]);
	CVTExtIndex *Index = GetIndex(params[2]);
	bool bUnique = CValue2Bool(*params[3]);
	bool bInverse = CValue2Bool(*params[4]);
	
	Index->SetFilter(Min, Max, bUnique, bInverse);

	return TRUE;
}

BOOL CVTExtended::defsSetFilter2(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 2: *param = ""; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procSetFilter2(CValue** params)
{
	CValue &SubSet = *(params[0]);
	int nCols = -1;
	if( !params[1]->IsEmpty() ) nCols = params[1]->GetNumeric();
	CVTExtIndex *Index = GetIndex(params[2]);
	
	Index->SetFilter(SubSet, nCols);

	return TRUE;
}

BOOL CVTExtended::defsDropFilter(int nParam, CValue* param) const
{
	if (0 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::procDropFilter(CValue** params)
{
	CVTExtIndex *Index = GetIndex(params[0]);
	Index->DropFilter();
	return TRUE;
}


BOOL CVTExtended::defsNextRow(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0:
		*param = "";
		return TRUE;
		break;
	case 1:
		*param = 0L;
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL CVTExtended::funcFirstRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *Index = GetIndex(params[0]);
	bool bUnique = CValue2Bool(*params[1]);

	if( Index->First(bUnique) )
		RetVal = 1L;
	else
		RetVal = 0L;
	SetCurrentRow(Index);
	return TRUE;
}

BOOL CVTExtended::funcNextRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *Index = GetIndex(params[0]);
	bool bUnique = CValue2Bool(*params[1]);

	if( Index->Next(bUnique) )
		RetVal = 1L;
	else
		RetVal = 0L;
	SetCurrentRow(Index);

	return TRUE;
}

BOOL CVTExtended::funcLastRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *Index = GetIndex(params[0]);
	bool bUnique = CValue2Bool(*params[1]);

	if( Index->Last(bUnique) )
		RetVal = 1L;
	else
		RetVal = 0L;
	SetCurrentRow(Index);

	return TRUE;
}

BOOL CVTExtended::funcPrevRow(CValue& RetVal, CValue** params)
{
	CVTExtIndex *Index = GetIndex(params[0]);
	bool bUnique = CValue2Bool(*params[1]);

	if( Index->Prev(bUnique) )
		RetVal = 1L;
	else
		RetVal = 0L;
	SetCurrentRow(Index);

	return TRUE;
}

BOOL CVTExtended::defsGroupBy(int nParam, CValue* param) const
{
	if (2 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::procGroupBy(CValue** params)
{
	if( params[2]->IsEmpty() )
	{
		CVTExtIndex Index("", params[0]->GetString(), false, this);
		Index.GroupBy(params[1]->GetString());
	}
	else
	{
		CVTExtIndex* pIndex = GetIndex(params[2]);
		pIndex->GroupBy(params[1]->GetString());
	}

	Indexes.Reindex();
	return TRUE;
}


void CVTExtended::FillColumnNumbersMap(TColumnsMap &ColMap, TColumnedTable* pDest, TColumnedTable* pSrc, bool bJoinColumns, CValue* pColumns)
{
	int nSrcColumns = pSrc->GetColumnsCount();
	TColumnsMapPair MapPair;

	BOOL bDone = FALSE;
	if( pColumns )
	{
		switch( pColumns->GetTypeCode() )
		{
		case STRING_TYPE_1C:
			{
				//Строка в формате "к1,к2,..." или "к1, к2:к2_2,..."
				//выражение вида к2:к2_2 значит копировать pSrc.к2 в pDest.к2_2
				CString sIndexExpr = pColumns->GetString();

				if (!sIndexExpr.IsEmpty())
				{
					CStringArrayEx saFields;
					saFields.FillSeparateString(sIndexExpr, ",");
					
					int nFields = saFields.GetSize();
					if( nFields )
					{
						bDone = TRUE;

						ColMap.reserve(nFields);

						for( int i = 0; i < nFields; i++)
						{
							CStringArrayEx saFieldParts;
							saFieldParts.FillSeparateString(saFields[i], ":");
							CString &sSrcField = saFieldParts[0];
							CString &sDestField = saFieldParts.GetSize() > 1 ? saFieldParts[1] : sSrcField;

							int nSrcCol = pSrc->ColumnNumber(sSrcField);
							if( nSrcCol < 0 )
								RuntimeError("Неверное имя колонки таблицы-источника \"%s\"", sSrcField);

							int nDestCol = pDest->ColumnNumber(sDestField);
							if( nDestCol < 0 && bJoinColumns ) 
								nDestCol = pDest->NewColumn(sDestField);
							
							if( nDestCol >= 0 )
							{
								MapPair.nSrcCol = nSrcCol;
								MapPair.nDestCol = nDestCol;
								ColMap.push_back(MapPair);
							}
							else
							{
								RuntimeError("Неверное имя колонки таблицы-приёмника \"%s\"", sDestField);
							}

						}
					}
				}
			}
			break;
		case NUMBER_TYPE_1C:
			{
				int nSrcCol = pColumns->GetNumeric().operator long() - 1;
				if( nSrcCol < 0 || nSrcCol >= pSrc->GetColumnsCount() )
					RuntimeError("Неверный номер колонки исходной таблицы: %i", nSrcCol + 1);

				LPCSTR sField = pSrc->ColumnName(nSrcCol);
				if (!sField)
					RuntimeError("Неверный номер колонки исходной таблицы: %i", nSrcCol + 1);
				
				bDone = TRUE;
				
				int nDestCol = pDest->ColumnNumber(sField);
				if( nDestCol < 0 && bJoinColumns ) 
					nDestCol = pDest->NewColumn(sField);
				
				if( nDestCol >= 0 )
				{
					MapPair.nSrcCol = nSrcCol;
					MapPair.nDestCol = nDestCol;
					ColMap.push_back(MapPair);
				}
			}
			break;
		default:
			;
		}
	}

	if (!bDone)
	{
		int nColumns = pSrc->GetColumnsCount();
		for( int i = 0; i < nColumns; i++ )
		{
			int nDestCol = pDest->ColumnNumber(pSrc->ColumnName(i));
			if( nDestCol < 0 && bJoinColumns ) 
				nDestCol = pDest->NewColumn(pSrc->ColumnName(i));

			if( nDestCol >= 0 )
			{
				MapPair.nSrcCol = i;
				MapPair.nDestCol = nDestCol;
				ColMap.push_back(MapPair);
			}
		}
	}
}


int* CVTExtended::ColumnNumbersMap(CVTExtended* pDest, CVTExtended* pSrc, bool bJoinColumns, CValue* pColumns)
{
	int nColumns = pSrc->GetColumnsCount();
	int *pColNumbersMap = new int[nColumns];

	//Зачистка
	for(int i = 0; i < nColumns; i++)
		pColNumbersMap[i] = -1;
	

	BOOL bDone = FALSE;
	if (pColumns)
	{
		switch (pColumns->GetTypeCode())
		{
		case STRING_TYPE_1C:
			{
				//Строка в формате "к1,к2,..."
				CString sIndexExpr = pColumns->GetString();

				if (!sIndexExpr.IsEmpty())
				{
					CStringArrayEx saFields;
					saFields.FillSeparateString(sIndexExpr, ",");
					
					int nFields = saFields.GetSize();
					if( nFields )
					{
						bDone = TRUE;
						for(i = 0; i < nFields; i++)
						{
							CString &sField = saFields[i];

							int nSrcCol = pSrc->ColumnNumber(sField, false);

							int nDestCol = pDest->ColumnNumber(sField, true);
							if( nDestCol < 0 && bJoinColumns ) 
								nDestCol = pDest->NewColumn(sField);
							
							pColNumbersMap[nSrcCol] = nDestCol;
						}
					}
				}
			}
			break;
		case NUMBER_TYPE_1C:
			{
				int nSrcCol = pColumns->GetNumeric().operator long() - 1;
				pSrc->CheckBounds_Col(nSrcCol);

				LPCSTR sField = pSrc->ColumnName(nSrcCol);
				if (!sField)
					RuntimeError("Неверный номер колонки: %i", nSrcCol + 1);
				
				bDone = TRUE;
				
				int nDestCol = pDest->ColumnNumber(sField, true);
				if( nDestCol < 0 && bJoinColumns ) 
					nDestCol = pDest->NewColumn(sField);
				
				pColNumbersMap[nSrcCol] = nDestCol;
			}
			break;
		default:
			;
		}
	}

	if (!bDone)
	{
		for( int i = 0; i < nColumns; i++ )
		{
			int nDestCol = pDest->ColumnNumber(pSrc->ColumnName(i), true);
			if( nDestCol < 0 && bJoinColumns ) 
				nDestCol = pDest->NewColumn(pSrc->ColumnName(i));
			pColNumbersMap[i] = nDestCol;
		}
	}

	return pColNumbersMap;
}

int* CVTExtended::ColumnNumbersMap(CVTExtended* pDest, CValueTable* pSrc, bool bJoinColumns, CValue* pColumns)
{
	int nColumns = pSrc->GetColumnCount();
	int* pColNumbersMap = new int[nColumns];

	BOOL bDone = FALSE;
	if (pColumns)
	{
		switch (pColumns->GetTypeCode())
		{
		case STRING_TYPE_1C:
			{
				CString sIndexExpr = pColumns->GetString();
				if (!sIndexExpr.IsEmpty())
				{
					CStringArrayEx saFields;
					saFields.FillSeparateString(sIndexExpr, ",");
					
					int nFields = saFields.GetSize();
					if(nFields)
					{
						bDone = TRUE;
						for(int i = 0; i < nColumns; i++)
							pColNumbersMap[i] = -1;
						
						for(i = 0; i < nFields; i++)
						{
							CString& sField = saFields[i];
							int nSrcCol = pSrc->GetColumnIdx(sField);
							if (nSrcCol < 0)
								RuntimeError("Колонка '%s' не найдена!", sField);

							int nDestCol = pDest->ColumnNumber(sField, true);
							if(nDestCol < 0) 
								nDestCol =pDest->NewColumn(sField);
							
							pColNumbersMap[nSrcCol] = nDestCol;
						}
					}
				}
			}
			break;
		case NUMBER_TYPE_1C:
			{
				int nSrcCol = pColumns->GetNumeric().operator long() - 1;
				if (nSrcCol < 0 || nSrcCol >= nColumns)
					RuntimeError("Неверно указан номер колонки!  Номер колонки = %i, всего колонок = %i", nSrcCol + 1, nColumns);

				CString const& sField = pSrc->GetColumnCode(nSrcCol);
				if (sField.IsEmpty())
					RuntimeError("Неверный номер колонки: %i", nSrcCol + 1);
				
				bDone = TRUE;
				for(int i = 0; i < nColumns; i++)
					pColNumbersMap[i] = -1;
				
				int nDestCol = pDest->ColumnNumber(sField, true);
				if(nDestCol < 0) 
					nDestCol = pDest->NewColumn(sField);
				
				pColNumbersMap[nSrcCol] = nDestCol;
			}
			break;
		default:
			;
		}
	}

	if (!bDone)
	{
		for( int i = 0; i < nColumns; i++ )
		{
			int nDestCol = pDest->ColumnNumber(pSrc->GetColumnCode(i), true);
			if( nDestCol < 0 && bJoinColumns ) 
				nDestCol = pDest->NewColumn(pSrc->GetColumnCode(i));
			pColNumbersMap[i] = nDestCol;
		}
	}

	return pColNumbersMap;
}

int* CVTExtended::ColumnNumbersMap(CVTExtended* pDest, CPtrArray* ppValueList)
{
	int nColumns = ppValueList->GetUpperBound() + 1;
	int *pColNumbersMap = new int[nColumns];
	for( int i = 0; i < nColumns; i++ )
	{
		CValueItem* pVLItem = (CValueItem*)(ppValueList->GetAt(i));
		pColNumbersMap[i] = pDest->ColumnNumber(pVLItem->m_text, true);
	}
	return pColNumbersMap;
}

void CVTExtended::CopyRowFromVTExt(CVTExtRow *pDest, CVTExtRow *pSrc, int nColumns, int *pColNumbersMap)
{
	for( int i = 0; i < nColumns; i++ )
	{
		int nCol = pColNumbersMap[i];
		if( nCol >= 0 )
		{
			pDest->SetValue(nCol, pSrc->GetValue(i));
		}
	}
}

void CVTExtended::CopyRowFromVTExt(CVTExtRow *pDest, CVTExtRow *pSrc, TColumnsMap ColNumbersMap)
{
	int nColumns = ColNumbersMap.size();
	for( int i = 0; i < nColumns; i++ )
	{
		pDest->SetValue(ColNumbersMap[i].nDestCol, pSrc->GetValue(ColNumbersMap[i].nSrcCol));
	}
}

void CVTExtended::CopyRowFromVT(CVTExtRow *pDest, CValueTable *pSrc, int nSrcRow, TColumnsMap ColNumbersMap)
{
	int nColumns = ColNumbersMap.size();
	for( int i = 0; i < nColumns; i++ )
	{
		pDest->SetValue(ColNumbersMap[i].nDestCol, pSrc->GetValue(ColNumbersMap[i].nSrcCol, nSrcRow));
	}
}

void CVTExtended::CopyRowFromVL(CVTExtRow *pDest, CPtrArray* ppValueList, int *pColNumbersMap)
{
	for( int i = ppValueList->GetUpperBound(); i >= 0; i-- )
	{
		int nCol = pColNumbersMap[i];
		if( nCol >= 0 )
		{
			CValue* val = (CValue*)(ppValueList->GetAt(i));
			pDest->SetValue(nCol, *val);
		}
	}
}

void CVTExtended::DuplicateRow(int nDestRow, int nSrcRow)
{
	CVTExtRow *pDestRow = GetRow(nDestRow), *pSrcRow = GetRow(nSrcRow); 
	int nColumns = GetColumnsCount();
	for( int nCol = 0; nCol < nColumns; nCol++ )
	{
		pDestRow->SetValue(nCol, pSrcRow->GetValue(nCol));
	}
}

void CVTExtended::LoadFromVTExt(CVTExtended* pSrc, CValue* IndexName, CValue* pColumns, bool bMerge, bool bUnique)
{
	CVTExtIndex *pIndex = pSrc->GetIndex(IndexName);
	if( !bMerge ) procCleanup(NULL);

	TColumnsMap ColNumbersMap;
	FillColumnNumbersMap(ColNumbersMap, &TColumnedTable_VTExt(this), &TColumnedTable_VTExt(pSrc), true, pColumns);

	CVTExtIterator* pIterator = pIndex->CreateIterator(bUnique);
	pIterator->First();
	while( pIterator->Next() )
	{
		int nDestRow = NewRow(FALSE);
		CVTExtRow *pDest = GetRow(nDestRow);
		CopyRowFromVTExt(pDest, pIterator->CurrentRow(), ColNumbersMap);
		Indexes.InsertRow(pDest, nDestRow);
	}

	delete pIterator;
}

void CVTExtended::LoadFromVT(CValueTable* pVT, CValue* pColumns, bool bMerge)
{
	if( !bMerge ) procCleanup(NULL);

	TColumnsMap ColNumbersMap;
	FillColumnNumbersMap(ColNumbersMap, &TColumnedTable_VTExt(this), &TColumnedTable_VT(pVT), true, pColumns);

	int nRows = pVT->GetRowCount();
	for( int nRow = 0; nRow < nRows; nRow++ )
	{
		int nDestRow = NewRow(FALSE);
		CVTExtRow *pDest = GetRow(nDestRow);
		CopyRowFromVT(pDest, pVT, nRow, ColNumbersMap);
		Indexes.InsertRow(pDest, nDestRow);
	}
}

BOOL CVTExtended::defsFillRow(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0:
		*param = 0L;
		return TRUE;
		break;
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

BOOL CVTExtended::procFillRow(CValue** params)
{
	//param 0
	int nDestRow = (int)(params[0]->GetNumeric()) - 1;
	CVTExtRow *pRow = NULL;
	if( nDestRow < 0 )
		pRow = DefaultIndex->pCurrentRow;
	else
	{
		CheckBounds_Row(nDestRow);
		pRow = GetRow(nDestRow);
	}
	if( pRow == NULL )
		RuntimeError("Строка не выбрана!");
	//param1
	CValue const& Value = *(params[1]);
	CVTExtended* pVTExt = CValue2VTExt(Value);
	CValueTable* pVT = CValue2VT(Value);
	CPtrArray* ppValueList = CValue2VL(Value);
	//param 2
	int nSrcRow = (int)(params[2]->GetNumeric()) - 1;
	//param 3
	bool ByColNames = params[3]->GetNumeric() != 0;


	if( ppValueList )
	{
		if( ByColNames )
		{
			int *pColNumMap = ColumnNumbersMap(this, ppValueList);
			CopyRowFromVL(pRow, ppValueList, pColNumMap);
			delete[] pColNumMap;
		}
		else
		{
			for( int i = min(ppValueList->GetUpperBound(), GetColumnsCount()-1); i >= 0; i-- )
			{
				pRow->SetValue(i, *(CValue*)(ppValueList->GetAt(i)));
			}
		}
	}
	else if( pVTExt )
	{
		if( nSrcRow < 0 ) nSrcRow = pVTExt->DefaultIndex->nCurrentRow;
		pVTExt->CheckBounds_Row(nSrcRow);
		if( ByColNames )
		{
			int *pColNumMap = ColumnNumbersMap(this, pVTExt, false);
			CopyRowFromVTExt(pRow, 
				pVTExt->GetRow(nSrcRow), 
				pVTExt->GetColumnsCount(), 
				pColNumMap);
			delete[] pColNumMap;
		}
		else
			*pRow = *(pVTExt->GetRow(nSrcRow));
	}
	else if( pVT )
	{
		if( nSrcRow < 0 ) nSrcRow = pVT->GetIndex(0);
		if( ByColNames )
		{
			int nColumns = pVT->GetColumnCount();
			int *pColNumMap = ColumnNumbersMap(this, pVT, false);
			for( int i = 0; i < nColumns; i++ )
			{
				int nDestCol = pColNumMap[i];
				if( nDestCol >= 0 ) pRow->SetValue(nDestCol, pVT->GetValue(i, nSrcRow));
			}
			delete[] pColNumMap;
		}
		else
		{
			int nColumns = min(GetColumnsCount(), pVT->GetColumnCount());
			for( int nCol = 0; nCol < nColumns; nCol++ )
				pRow->SetValue(nCol, pVT->GetValue(nCol, nSrcRow));
		}
	}
	else
	{
		for( int i = GetColumnsCount()-1; i >= 0; i-- )
		{
			pRow->SetValue(i, Value);
		}
	}

	Indexes.OnChangeRow(DefaultIndex->nCurrentRow);

	return TRUE;
}

BOOL CVTExtended::defsFillColumn(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0:
	case 3:
	case 4:
		*param = "";
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL CVTExtended::procFillColumn(CValue** params)
{
	//param 0
	CVTExtIndex *pIndex = GetIndex(params[0]);
	//param 1
	int nDestCol = ColumnNumber(params[1]);
	//param 2
	CValue const& Value = *(params[2]);
	CVTExtended* pVTExt = CValue2VTExt(Value);
	CValueTable* pVT = CValue2VT(Value);
	CPtrArray* ppValueList = CValue2VL(Value);

	CVTExtIterator* pIterator = pIndex->CreateIterator();

	bool bNeedReindex = Indexes.ColumnIndexed(nDestCol);
	bool *bChangedRows;
	
	if( bNeedReindex )
	{
		bChangedRows = new bool[GetRowsCount()];
		memset((void*)bChangedRows, 0, sizeof(bool) * GetRowsCount());
	}


	if( ppValueList )
	{
		int nValue = 0, nValues = ppValueList->GetSize();
		pIterator->First();
		while( nValue < nValues && pIterator->Next() )
		{
			CValue *pVal = (CValue*)(ppValueList->GetAt(nValue));
			pIterator->CurrentRow()->SetValue(nDestCol, *pVal);
			//if( bNeedReindex ) bChangedRows[pIterator->CurrentRowIndex()] = true;
			nValue++;
		}
	}
	else if( pVT )
	{
		int nSrcCol = GetVTColumnNumber(pVT, params[3], params[1]);
		if( nSrcCol < 0 || nSrcCol >= pVT->GetColumnCount() )
			RuntimeError("Неверно указана колонка в таблице-источнике!");
		int nSrcRow = 0, nSrcRows = pVT->GetRowCount();
		pIterator->First();
		while( nSrcRow < nSrcRows && pIterator->Next() )
		{
			CValue const& Value = pVT->GetValue(nSrcCol, nSrcRow);
			pIterator->CurrentRow()->SetValue(nDestCol, Value);
			//if( bNeedReindex ) bChangedRows[pIterator->CurrentRowIndex()] = true;
			nSrcRow++;
		}
	}
	else if( pVTExt )
	{
		CVTExtIndex *pSrcIndex = pVTExt->GetIndex(params[3]);
		int nSrcCol = GetVTColumnNumber(pVTExt, params[4], params[1]);
		if( nSrcCol < 0 || nSrcCol >= pVTExt->GetColumnsCount() )
			RuntimeError("Неверно указана колонка в таблице-источнике!");

		CVTExtIterator* pSrcIterator = pSrcIndex->CreateIterator();

		pIterator->First();
		pSrcIterator->First();
		while( pIterator->Next() && pSrcIterator->Next() )
		{
			CValue const& Value = pSrcIterator->CurrentRow()->GetValue(nSrcCol);
			pIterator->CurrentRow()->SetValue(nDestCol, Value);
			//if( bNeedReindex ) bChangedRows[pIterator->CurrentRowIndex()] = true;
		}
		delete pSrcIterator;
	}
	else
	{
		pIterator->First();
		while( pIterator->Next() )
		{
			pIterator->CurrentRow()->SetValue(nDestCol, Value);
			if( bNeedReindex ) bChangedRows[pIterator->CurrentRowIndex()] = true;
		}
	}

	delete pIterator;

	if( bNeedReindex )
	{
		//Сначала удаляем все строки из деревьев
		for( int i = GetRowsCount() - 1; i >= 0; i-- )
		{
			if( bChangedRows[i] )
			{
				for( int nIndex = Indexes.GetCount() - 1; nIndex >= 0; nIndex-- )
				{
					if( Indexes[nIndex]->ColumnIndexed(nDestCol) )
					{
						Indexes[nIndex]->RemoveRowFromTree(i);
					}
				}
			}
		}

		//И только потом добавляем их заново.  Иначе фигня будет, потому что строки с уже измененными значениями оказываются не на своих местах в дереве!
		for( i = GetRowsCount() - 1; i >= 0; i-- )
		{
			if( bChangedRows[i] )
			{
				for( int nIndex = Indexes.GetCount() - 1; nIndex >= 0; nIndex-- )
				{
					if( Indexes[nIndex]->ColumnIndexed(nDestCol) )
					{
						Indexes[nIndex]->InsertRow(GetRow(i), i);
					}
				}
			}
		}

		delete[] bChangedRows;
	}

	return TRUE;
}

void CVTExtended::UnloadToVT(CValueTable* pVT, CVTExtIndex* pIndex, CValue* pColumns, bool bUnique)
{
	// kms test
	// save refresh flag and disable refresh
	BOOL bPrevEnableRefresh = pVT->m_bEnableRefresh;
	pVT->m_bEnableRefresh = FALSE;

	pVT->Clear(TRUE);
	CType Type1C(UNDEFINE_TYPE_1C);

	int nColumns;
	int* pColNumbers = NULL;
	if (pColumns)
	{
		switch (pColumns->GetTypeCode())
		{
		case STRING_TYPE_1C:
			{
				CString sIndexExpr = pColumns->GetString();
				if (!sIndexExpr.IsEmpty())
				{
					CStringArrayEx saFields;
					saFields.FillSeparateString(sIndexExpr, ",");
					
					nColumns = saFields.GetSize();
					if(nColumns)
					{
						pColNumbers = new int[nColumns];
						for(int i = 0; i < nColumns; i++)
						{
							CString& sField = saFields[i];
							pColNumbers[i] = ColumnNumber(sField, false);
							pVT->AddColumn(sField, Type1C, sField, 0, "", 0);
						}
					}
				}
			}
			break;
		case NUMBER_TYPE_1C:
			{
				int nSrcCol = pColumns->GetNumeric().operator long() - 1;
				LPCSTR sField = ColumnName(nSrcCol);
				if (!sField)
					RuntimeError("Неверный номер колонки: %i", nSrcCol + 1);
				
				nColumns = 1;
				pColNumbers = new int[1];
				pColNumbers[0] = nSrcCol;
				pVT->AddColumn(sField, Type1C, sField, 0, "", 0);
			}
			break;
		default:
			;
		}
	}
	
	if (!pColNumbers)
	{
		nColumns = GetColumnsCount();
		pColNumbers = new int [nColumns];
		for( int i = 0; i < nColumns; i++ )
		{
			pColNumbers[i] = i;
			LPCSTR sField =ColumnName(i); 
			pVT->AddColumn(sField, Type1C, sField, 0, "", 0);
		}
	}

	CValue** ppValues = new CValue* [nColumns];

	CVTExtIterator* pIterator = pIndex->CreateIterator(bUnique);
	CVTExtRow *pRow;
	pIterator->First();
	while( pIterator->Next() )
	{
		pRow = pIterator->CurrentRow();
		for( int nCol = 0; nCol < nColumns; nCol++ )
			ppValues[nCol] = &pRow->GetValue(pColNumbers[nCol]);

		pVT->AddRow(ppValues, -1);
	}
	delete pIterator;
	delete[] ppValues;
	delete[] pColNumbers;

	// restore refresh flag and refresh
	pVT->m_bEnableRefresh = bPrevEnableRefresh;
	pVT->DeleteRow(pVT->NewRow(-1));
}

void CVTExtended::UnloadToDoc(CDocContext* pDoc, CVTExtIndex* pIndex, bool bUnique)
{
	CValue vVT;
	vVT.CreateObject("ТаблицаЗначений");
	CValueTable* pVT = CValue2VT(vVT);
	UnloadToVT(pVT, pIndex, NULL, bUnique);

	CValue* params[1] = {&vVT};
	::CallAsProc(pDoc, "ЗагрузитьТабличнуюЧасть", params);
}


BOOL CVTExtended::defsUnloadTo(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 1:
		*param = "";
		return TRUE;
		break;
	case 2:
		*param = "";
		return TRUE;
		break;
	case 3:
		*param = 0L;
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL CVTExtended::procUnloadTo(CValue** params)
{
	CValue &Dest = *(params[0]);
	CVTExtended* pVTExt = NULL;
	CValueTable* pVT = NULL;
	CPtrArray* pVL = NULL;
	CDocContext* pDoc;

	bool bUnique = CValue2Bool(*params[3]);

	if( Dest.GetTypeCode() == UNDEFINE_TYPE_1C )
	{
        Dest.CreateObject("ИндексированнаяТаблица");
		pVTExt = CValue2VTExt(Dest);
	}
	else
	{
		pVTExt = CValue2VTExt(Dest);
		pVT = CValue2VT(Dest);
		pVL = CValue2VL(Dest);
	}

	if( pVTExt )
	{
		pVTExt->LoadFromVTExt(this, params[1], params[2], false, bUnique);
	}
	else if( pVT )
	{
		CVTExtIndex *pIndex = GetIndex(params[1]);
		UnloadToVT(pVT, pIndex, params[2], bUnique);
	}
	else if( pVL )
	{
		CVTExtIndex *pIndex = GetIndex(params[1]);
		int nCol = ColumnNumber(params[2]);

		CBLContext* pCont = Dest.GetContext();
		int nRemoveAllMethod = pCont->FindMethod("RemoveAll");
		int nAddMethod = pCont->FindMethod("AddValue");
		CValue vString = "";
		CValue* params[2];
		params[1] = &vString;

		pCont->CallAsProc(nRemoveAllMethod, NULL);

		CVTExtIterator* pIterator = pIndex->CreateIterator(bUnique);
		pIterator->First();
		while( pIterator->Next() )
		{
			params[0] = &(pIterator->CurrentRow()->GetValue(nCol));
			pCont->CallAsProc(nAddMethod, params);
		}
		delete pIterator;
	}
	else if( (pDoc = CValue2Doc(*params[0])) != NULL )
	{
		CVTExtIndex *pIndex = GetIndex(params[1]);
		UnloadToDoc(pDoc, pIndex, bUnique);
	}
	else
	{
		RuntimeError("Требуется переменная с типом ИндексированнаяТаблица, ТаблицаЗначений, СписокЗначений или Документ!");
	}

	return TRUE;
}

void CVTExtended::LoadFromDoc(CDocContext* pDoc, CValue& vColumns)
{
	CValue vVT;
	vVT.CreateObject("ТаблицаЗначений");
	CValue* params[2] = {&vVT, &vColumns};
	CValue RetVal;
	if( ::CallAsFunc(pDoc, "ВыгрузитьТабличнуюЧасть", RetVal, params) )
	{
		CValueTable* pVT = CValue2VT(vVT);
		LoadFromVT(pVT, NULL, false);
	}
}

void CVTExtended::LoadFromQuery(CBLContext* pQuery, CValue** ppParams)
{
	CValue vVT;
	vVT.CreateObject("ТаблицаЗначений");
	//CValue Flag = 0L, Sums = 0L;
	CValue* params[3] = {&vVT, ppParams[0], ppParams[1]};
	CValue RetVal;
	if( ::CallAsFunc(pQuery, "Выгрузить", RetVal, params) )
	{
		CValueTable* pVT = CValue2VT(vVT);
		LoadFromVT(pVT, NULL, false);
	}
}

BOOL CVTExtended::defsLoadFrom(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 1: param->Reset(); return TRUE; break;
	case 2: param->Reset(); return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procLoadFrom(CValue** params)
{
	CVTExtended* pVTExt;
	CValueTable* pVT;
	CDocContext* pDoc;
	CValue* pvDoc;

	if( (pVTExt = CValue2VTExt(*params[0])) != NULL )
	{
		if( pVTExt == this )
			RuntimeError("Нельзя загружать саму в себя!");
		LoadFromVTExt(pVTExt, params[1], params[2], false, false);
	}
	else if( (pVT = CValue2VT(*params[0])) != NULL )
	{
		LoadFromVT(pVT, params[2], false);
	}
	else if( (pDoc = CValue2Doc(*params[0])) != NULL )
	{
		LoadFromDoc(pDoc, *params[1]);
	}
	else if( (pvDoc = CValueCurDoc2Doc(*params[0])) != NULL )
	{
		pDoc = CValue2Doc(*pvDoc);
		LoadFromDoc(pDoc, *params[1]);
		delete pvDoc;
	}
	else
	{
		RuntimeError("Требуется переменная с типом ТаблицаЗначений, ИндексированнаяТаблица или Документ!");
		return FALSE;
	}

	return TRUE;
}

BOOL CVTExtended::defsLoadFromQuery(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 1: *param = 0L; return TRUE; break;
	case 2: *param = 1L; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procLoadFromQuery(CValue** params)
{
	CBLContext* pQuery = CValue2Query(*params[0]);
	if( pQuery != NULL )
	{
		LoadFromQuery(pQuery, params+1);
	}
	else
	{
		RuntimeError("Требуется переменная с типом Запрос!");
		return FALSE;
	}
	return TRUE;
}

BOOL CVTExtended::defsUnionVT(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 1: param->Reset(); return TRUE; break;
	case 2: param->Reset(); return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procUnionVT(CValue** params)
{
	CVTExtended* pVTExt = CValue2VTExt(*params[0]);
	CValueTable* pVT = CValue2VT(*params[0]);

	if( pVTExt == NULL && pVT == NULL )
	{
		RuntimeError("Требуется переменная с типом ТаблицаЗначений или ИндексированнаяТаблица!");
		return FALSE;
	}

	if( pVTExt != NULL )
	{
		if( pVTExt == this )
			RuntimeError("Нельзя загружать саму в себя!");
		LoadFromVTExt(pVTExt, params[1], params[2], true, false);
	}
	else
	{
		LoadFromVT(pVT, params[2], true);
	}

//	Indexes.Reindex();

	return TRUE;
}

void CVTExtended::FillKeyRow(CVTExtRow & KeyRow, CVTExtRow& SrcRow, 
							 CVTExtIndexFields& DestIdxFields, CVTExtIndexFields& SrcIdxFields, 
							 int nKeyFields)
{
	for( int i = 0; i < nKeyFields; i++ )
	{
		CValue &val = SrcRow.GetValue(SrcIdxFields[i].ColumnNumber);
		KeyRow.SetValue(DestIdxFields[i].ColumnNumber, val);
	}
}

BOOL CVTExtended::defsConjunctionVT(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 3: *param = 1L; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::procConjunctionVT(CValue** params)
{
	CVTExtended* pVTExt = CValue2VTExt(*params[0]);

	if( pVTExt == NULL  )
		RuntimeError("Требуется переменная с типом ИндексированнаяТаблица!");
	else if( pVTExt == this )
		RuntimeError("Нельзя загружать саму в себя!");

	//Возьмём индексы, по которым будем сравнивать строки
	CVTExtIndex *pIndexDest = GetIndex(params[1]);
	CVTExtIndex *pIndexSrc = pVTExt->GetIndex(params[2]);
	int nKeyFields = pIndexDest->IndexFields.size();

	if( nKeyFields != pIndexSrc->IndexFields.size() )
		RuntimeError("Количество полей в индексах не совпадает!");

	//Ключ для поиска в таблице Src
	CVTExtRow SrcKeyRow(pVTExt->GetColumnsCount());
	CVTIndexRecord SrcIdxRec(&SrcKeyRow, &(pIndexSrc->IndexFields));


	//Проедемся по строкам снизу вверх, и всё, что не найдётся в Src - удалим
	CVTExtIndexFields &SrcIdxFields = pIndexSrc->IndexFields;
	CVTExtIndexFields &DestIdxFields = pIndexDest->IndexFields;
	for( int nRow = GetRowsCount() - 1; nRow >= 0; nRow-- )
	{
		CVTExtRow *pDestRow = GetRow(nRow);
		FillKeyRow(SrcKeyRow, *pDestRow, SrcIdxFields, DestIdxFields, nKeyFields);
		if( pIndexSrc->Search(SrcIdxRec) == NULL )
		{
			delete pDestRow;
			Rows.RemoveAt(nRow);
		}
	}
	Indexes.Reindex();

	if (bool bAddSrcRows = CValue2Bool(*params[3]))
	{
		//Объединим набор колонок, и построим карту соответствия номеров колонок
		int *pColNumbersMap = ColumnNumbersMap(this, pVTExt, true);
		
		//ключ для поиска в текущей таблице
		CVTExtRow DestKeyRow(GetColumnsCount());
		CVTIndexRecord DestIdxRec(&DestKeyRow, &(pIndexDest->IndexFields));
		
		//Проедемся по строкам таблицы Src, и всё, что найдётся в текущей, добавим
		int nSrcColumns = pVTExt->GetColumnsCount();
		int nSrcRows = pVTExt->GetRowsCount();
		for( nRow = 0; nRow < nSrcRows; nRow++ )
		{
			CVTExtRow *pSrcRow = pVTExt->GetRow(nRow);
			FillKeyRow(DestKeyRow, *pSrcRow, DestIdxFields, SrcIdxFields, nKeyFields);
			if( pIndexDest->Search(DestIdxRec) )
			{
				CVTExtRow *NewRow = new CVTExtRow(GetColumnsCount());
				CopyRowFromVTExt(NewRow, pSrcRow, nSrcColumns, pColNumbersMap);
				Rows.Add(NewRow);
				Indexes.InsertRow(NewRow, GetRowsCount());
			}
		}
		delete[] pColNumbersMap;
	}

	return TRUE;
}

BOOL CVTExtended::procDifferenceVT(CValue** params)
{
	CVTExtended* pVTExt = CValue2VTExt(*params[0]);

	if( pVTExt == NULL  )
		RuntimeError("Требуется переменная с типом ИндексированнаяТаблица!");
	else if( pVTExt == this )
		RuntimeError("Нельзя загружать саму в себя!");

	//Возьмём индексы, по которым будем сравнивать строки
	CVTExtIndex *pIndexDest = GetIndex(params[1]);
	CVTExtIndex *pIndexSrc = pVTExt->GetIndex(params[2]);
	int nKeyFields = pIndexDest->IndexFields.size();

	if( nKeyFields != pIndexSrc->IndexFields.size() )
		RuntimeError("Количество полей в индексах не совпадает!");

	// сохраним число строк таблицы назначения для последующей обработки
	int nDestRows = GetRowsCount();

	CVTExtIndexFields &SrcIdxFields = pIndexSrc->IndexFields;
	CVTExtIndexFields &DestIdxFields = pIndexDest->IndexFields;

	if (bool bAddSrcRows = CValue2Bool(*params[3]))
	{
		//Объединим набор колонок, и построим карту соответствия номеров колонок
		int *pColNumbersMap = ColumnNumbersMap(this, pVTExt, true);

		//ключ для поиска в текущей таблице
		CVTExtRow DestKeyRow(GetColumnsCount());
		CVTIndexRecord DestIdxRec(&DestKeyRow, &(pIndexDest->IndexFields));
		
		//Проедемся по строкам таблицы Src, и всё, что не найдётся в текущей, добавим
		int nSrcColumns = pVTExt->GetColumnsCount();
		int nSrcRows = pVTExt->GetRowsCount();
		for( int nRow = 0; nRow < nSrcRows; nRow++ )
		{
			CVTExtRow *pSrcRow = pVTExt->GetRow(nRow);
			FillKeyRow(DestKeyRow, *pSrcRow, DestIdxFields, SrcIdxFields, nKeyFields);
			if( !pIndexDest->Search(DestIdxRec) )
			{
				CVTExtRow *NewRow = new CVTExtRow(GetColumnsCount());
				CopyRowFromVTExt(NewRow, pSrcRow, nSrcColumns, pColNumbersMap);
				Rows.Add(NewRow);
				Indexes.InsertRow(NewRow, GetRowsCount());
			}
		}
		delete[] pColNumbersMap;
	}
		
	//Ключ для поиска в таблице Src
	CVTExtRow SrcKeyRow(pVTExt->GetColumnsCount());
	CVTIndexRecord SrcIdxRec(&SrcKeyRow, &(pIndexSrc->IndexFields));

	//Проедемся по строкам таблицы назначения снизу вверх, и всё, что найдётся в Src - удалим
	for( int nRow = nDestRows - 1; nRow >= 0; nRow-- )
	{
		CVTExtRow *pDestRow = GetRow(nRow);
		FillKeyRow(SrcKeyRow, *pDestRow, SrcIdxFields, DestIdxFields, nKeyFields);
		if( pIndexSrc->Search(SrcIdxRec) != NULL )
		{
			delete pDestRow;
			Rows.RemoveAt(nRow);
		}
	}
	Indexes.Reindex();

	return TRUE;
}

void _Join_FillFoundRows(unsigned char *FoundRows, CVTExtIndexTreeNode &node)
{
	FoundRows[node.ID] = 1;
	if( node.ArrayEqualIDs )
	{
		vector<int> &IDs = *(node.ArrayEqualIDs);
		for( vector<int>::iterator Iterator = IDs.begin(); Iterator != IDs.end(); Iterator++ )
		{
			FoundRows[*Iterator] = 1;
		}
	}
}

BOOL CVTExtended::Join(CValue** params, TJoinType JoinType)
{
	CVTExtIndexFields LeftFields((CString)(params[0]->GetString()), this);
	int nKeyFields = LeftFields.size();

	CVTExtended* pRightVT = CValue2VTExt(*params[1]);

	if( nKeyFields == 0 )
		RuntimeError("Не заданы колонки соединения для левой таблицы!");
	else if( !pRightVT  )
		RuntimeError("Не задана правая таблица!");

	CVTExtIndex *pRightIndex = pRightVT->GetIndex(params[2]);
	if( !pRightIndex )
		RuntimeError("Не задан индекс для правой таблицы!");

	TColumnsMap ColNumMap;
	FillColumnNumbersMap(ColNumMap, &TColumnedTable_VTExt(this), &TColumnedTable_VTExt(pRightVT), true, params[3]);


	if( nKeyFields != pRightIndex->IndexFields.size() )
		RuntimeError("Количество колонок соединения в левой и правой таблицах не совпадают!");

	CVTExtIndexFields &RightIdxFields = pRightIndex->IndexFields;

	//ключ для поиска в правой таблице
	CVTIndexRecord RightIdxRec(&(pRightIndex->KeyRow), &(pRightIndex->IndexFields));
	
	unsigned char *FoundRowsR = NULL;
	if( JoinType == joinFull || JoinType == joinRight )
	{
		FoundRowsR = new unsigned char[pRightVT->GetRowsCount()];
		memset(FoundRowsR, 0, pRightVT->GetRowsCount() * sizeof(unsigned char));
	}

	int nRows = GetRowsCount(), nRightCols = pRightVT->GetColumnsCount();
	for( int nRow = 0; nRow < nRows; nRow++ )
	{
		CVTExtRow *pRow = GetRow(nRow);
		FillKeyRow(pRightIndex->KeyRow, *pRow, pRightIndex->IndexFields, LeftFields, nKeyFields);
		
		CVTExtIndexTreeNode *pRightNode = pRightIndex->Search(RightIdxRec);
		if( pRightNode )
		{
			CVTExtRow *pRightRow = pRightVT->GetRow(pRightNode->ID);
			if( FoundRowsR ) _Join_FillFoundRows(FoundRowsR, *pRightNode);

			CopyRowFromVTExt(pRow, pRightRow, ColNumMap);

			//Если в правой таблице несколько строк с таким ключом, то надо наштамповать дублей в левой таблице
			if( pRightNode->Count > 1)
			{
				int nSize = pRightNode->Count - 1;
				for( int i = 0; i < nSize; i++ )
				{
					int nNewRow = NewRow(FALSE);
					DuplicateRow(nNewRow, nRow);

					pRightRow = pRightVT->GetRow(pRightNode->ArrayEqualIDs->operator[](i));
					CopyRowFromVTExt(GetRow(nNewRow), pRightRow, ColNumMap);
				}
			}
		}
		else
		{
			if( JoinType == joinInner )
			{
				RemoveRow(nRow, false);
				nRow--;
				nRows--;
			}
			else if( JoinType == joinRight )
			{
				RemoveRow(nRow, FALSE);
				nRow--;
				nRows--;
			}
		}
	}

	if( JoinType == joinFull || JoinType == joinRight )
	{
		int nRightRows = pRightVT->GetRowsCount();
		for( int nRightRow = 0; nRightRow < nRightRows; nRightRow++ )
		{
			if( FoundRowsR[nRightRow] == 0 )
			{
				int nLeftRow = NewRow(FALSE);
				CVTExtRow *pRightRow = pRightVT->GetRow(nRightRow);
				CopyRowFromVTExt(GetRow(nLeftRow), pRightRow, ColNumMap);
			}
		}
	}

		
	if( FoundRowsR ) delete[] FoundRowsR;
	Indexes.Reindex();

	return TRUE;
}

BOOL CVTExtended::defsInnerJoin(int nParam, CValue* param) const
{
	return FALSE;
}


BOOL CVTExtended::procInnerJoin(CValue** params)
{
	return Join(params, joinInner);
}

BOOL CVTExtended::defsLeftJoin(int nParam, CValue* param) const
{
	return FALSE;
}

BOOL CVTExtended::procLeftJoin(CValue** params)
{
	return Join(params, joinLeft);
}


BOOL CVTExtended::defsRightJoin(int nParam, CValue* param) const
{
	return FALSE;
}

BOOL CVTExtended::procRightJoin(CValue** params)
{
	return Join(params, joinRight);
}

BOOL CVTExtended::defsFullJoin(int nParam, CValue* param) const
{
	return FALSE;
}

BOOL CVTExtended::procFullJoin(CValue** params)
{
	return Join(params, joinFull);
}


void CVTExtended::LoadStructure(CVTExtended* pVT)
{
	int nCols = pVT->GetColumnsCount();
	for( int nCol = 0; nCol < nCols; nCol++ )
	{
		NewColumn(pVT->ColumnName(nCol));
		Indexes.NewColumn();
	}
}

BOOL CVTExtended::defsCopy(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0: *param = 0L; return TRUE; break;
	case 1: *param = 1L; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::funcCopy(CValue& RetVal, CValue** params)
{
	RetVal.CreateObject("IndexedTable");
	CVTExtended* pVT = (CVTExtended*)(RetVal.GetContext());
	
	pVT->LoadStructure(this);

	bool bCopyData = params[1]->GetNumeric() != 0L;
	if( bCopyData )
	{
		for( int nRow = 0; nRow < GetRowsCount(); nRow++ )
		{
			pVT->NewRow();
			CVTExtRow *pDestRow = pVT->GetRow(nRow);
			CVTExtRow *pSrcRow = GetRow(nRow);
			*pDestRow = *pSrcRow;
		}
	}

	bool bCopyIndexes = params[0]->GetNumeric() != 0L;
	if( bCopyIndexes )
	{
		*(pVT->DefaultIndex) = *DefaultIndex;
		int nIndexCount = Indexes.GetUpperBound();
		for( int nIndex = 1; nIndex <= nIndexCount; nIndex++ )
		{
			CVTExtIndex* pIndex = new CVTExtIndex(Indexes[nIndex]->GetName(), pVT);
			*pIndex = *(Indexes[nIndex]);
			pVT->Indexes.AddIndex(pIndex);
		}
	}

	if( bCopyData ) pVT->Indexes.Reindex();

	return TRUE;
}


void QuoteString(CString& str)
{
	str.Replace("\r", "\\r");
	str.Replace("\n", "\\n");
}

void UnQuoteString(CString& str)
{
	str.Replace("\\r", "\r");
	str.Replace("\\n", "\n");
}

void CVTExtended::SaveToStorage(CSerialStorage& Storage, CVTExtIndex *pIndex, bool InnerFormat)
{
	int nCol;
	CString str;

	if( pIndex == NULL ) pIndex = DefaultIndex;

	int nRows = pIndex->IsEmpty() ? GetRowsCount() : pIndex->Count;

	Storage.AddString("{IndexedTable:\n%i, %i, %i\n", InnerFormat ? 1 : 2, GetColumnsCount(), nRows);

	for( nCol = 0; nCol < GetColumnsCount(); nCol++ )
		Storage.AddString("%s\n", ColumnName(nCol));

	CVTExtIterator* pIterator = pIndex->CreateIterator();

	pIterator->First();
	while( pIterator->Next() )
	{
		CVTExtRow *pRow = pIterator->CurrentRow();

		Storage.AddString("{\n");
		for( nCol = 0; nCol < GetColumnsCount(); nCol++ )
		{
			CValue& val = pRow->GetValue(nCol);
			CVTExtended *pVT = CValue2VTExt(val);

			if( pVT )
				pVT->SaveToStorage(Storage, NULL, InnerFormat);
			else
			{
				if( InnerFormat )
				{
					if( val.GetTypeCode() == AGREGATE_TYPE_1C )
						//val.GetContext()->SaveToString(str);
						val.SaveToString(str);
					else
						val.SaveToString(str);

					QuoteString(str);

					Storage.AddString("%s\n", str);
				}
				else
				{
					char t;
					switch( val.GetTypeCode() )
					{
					case 1:  t = 'N'; break;
					case 2:  t = 'S'; break;
					case 3:  t = 'D'; break;
					default: t = 'S'; break;
					}

					CString strVal;
					switch( val.GetTypeCode() )
					{
					case 3:
						strVal = val.GetDate().Format(df_dd_MM_yyyy, NULL);
						break;
					default:
						strVal = val.Format();
						break;
					}

					QuoteString(strVal);
					Storage.AddString("%c:%s\n", t, strVal);
				}
			}
		}
		Storage.AddString("}\n");
	}
	Storage.AddString("}\n");

	delete pIterator;
}

void CVTExtended::SaveToStorage_CSV(CSerialStorage& Storage, CVTExtIndex *pIndex, bool InnerFormat, bool bWithColNames)
{
	int nCol;
	int nRows = pIndex->IsEmpty() ? GetRowsCount() : pIndex->Count;
	int nCols = GetColumnsCount();
	char *semicolon = "";

	if( bWithColNames )
	{
		for( nCol = 0; nCol < nCols; nCol++ )
		{
			Storage.AddString("%s%s", semicolon, ColumnName(nCol));
			semicolon = ";";
		}
		Storage.AddString("\n");
	}

	CVTExtIterator* pIterator = pIndex->CreateIterator();

	CString str;
	pIterator->First();
	while( pIterator->Next() )
	{
		CVTExtRow *pRow = pIterator->CurrentRow();

		semicolon = "";
		for( nCol = 0; nCol < nCols; nCol++ )
		{
			CValue& val = pRow->GetValue(nCol);
			if( InnerFormat )
			{
				if( val.GetTypeCode() == AGREGATE_TYPE_1C )
					//val.GetContext()->SaveToString(str);
					val.SaveToString(str);
				else
					val.SaveToString(str);
			}
			else
			{
				str = val.Format();
			}

			QuoteString(str);

			char *quote = "";
			if( str.Find(';') >= 0 )
			{
				str.Replace("\"", "\"\"");
				quote = "\"";
			}

			Storage.AddString("%s%s%s%s", semicolon, quote, str, quote);

			semicolon = ";";
		}
		Storage.AddString("\n");
	}

	delete pIterator;
}

BOOL CVTExtended::defsSaveToFile(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		break;
	case 1:
		*param = 1L;
		return TRUE; 
		break;
	case 2:
		*param = "";
		return TRUE; 
		break;
	case 3:
		*param = 1L;
		return TRUE; 
		break;
	}
	return FALSE;
}

BOOL CVTExtended::procSaveToFile(CValue** params)
{
	CString FileName = params[0]->GetString();
	int format = params[1]->GetNumeric();
	CVTExtIndex *pIndex = GetIndex(params[2]);
	bool bWithColNames = params[3]->GetNumeric() != 0;

	FILE *f = fopen(FileName, "wt");
	if( f == NULL )
		RuntimeError("Невозможно открыть файл %s! %s", FileName, strerror(errno));
	CSerialStorage Storage(f);

	switch( format )
	{
	case 1:
		SaveToStorage(Storage, pIndex, true);
		break;
	case 2:
		SaveToStorage(Storage, pIndex, false);
		break;
	case 3:
		SaveToStorage_CSV(Storage, pIndex, true, bWithColNames);
		break;
	case 4:
		SaveToStorage_CSV(Storage, pIndex, false, bWithColNames);
		break;
	}

	fclose(f);

	return TRUE;
}

bool ReadDateFromString(char* str, int& dd, int& mm, int& yy)
{
	char *ptr;

	if( (ptr = strchr(str, '.')) == NULL ) return false;
	*ptr = '\0';
	dd = atoi(str);

	str = ptr + 1;
	if( (ptr = strchr(str, '.')) == NULL ) return false;
	*ptr = '\0';
	mm = atoi(str);

	str = ptr + 1;
	yy = atoi(str);
	if( yy < 100 ) yy += 2000;

	return true;
}

bool CVTExtended::LoadFromStorage2(CSerialStorage& Storage)
{
	char *str;
	int nCol, nRow;
	CVTExtRow *pRow;
	enum {read_table, read_row} state = read_table;

	procCleanup(NULL);

	int nFormat = 0, nCols = 0, nRows = 0;
	str = Storage.ReadString();
	sscanf(str, "%i, %i, %i", &nFormat, &nCols, &nRows);
	bool bInnerFormat = nFormat == 1;

	for( nCol = 0; nCol < nCols; nCol++ )
	{
		str = Storage.ReadString();
		NewColumn(str);
	}

	while( (str = Storage.ReadString()) != NULL )
	{
		switch( state )
		{
		case read_table:
			if( strcmp(str, "{") == 0 )
			{
				nRow = NewRow();
				pRow = GetRow(nRow);
				nCol = 0;
				state = read_row;
			}
			else if( strcmp(str, "}") == 0 )
			{
				return true;
			}
			break;
		case read_row:
			if( strcmp(str, "}") == 0 )
			{
				//if( (nCol + 1) != nCols ) return false;
				state = read_table;
			}
			else 
			{
				if( nCol >= nCols ) return false;

				if( strncmp(str, "{IndexedTable:", 14) == 0 )
				{
					pRow->GetValue(nCol).CreateObject("IndexedTable");
					CVTExtended *pVT = CValue2VTExt(pRow->GetValue(nCol));

					if( !pVT->LoadFromStorage2(Storage) ) return false;
				}
				else
				{
					CString Str(str);
					UnQuoteString(Str);
					str = (char*)(LPCSTR)Str;

					CValue &val = pRow->GetValue(nCol);
					if( bInnerFormat )
						val.LoadFromString(str, 0);
					else
					{
						char t;
						sscanf(str, "%c:", &t);
						switch( t )
						{
						case 'N':
							{
								CNumeric n;
								n.FromString(str + 2, NULL);
								val = n;
							}
							break;
						case 'S':
							val = str + 2;
							break;
						case 'D':
							{
								int dd, mm, yy;
								if( !ReadDateFromString(str+2, dd, mm, yy) )
									RuntimeError("Ошибка чтения даты '%s'", str+2);
								if( dd == 0 || mm == 0 || yy == 0 )
								{
									CDate d(0,0,0);
									d.m_DateNum = 0;
									val = d;
								}
								else
								{
									CDate d(yy, mm, dd);
									val = d;
								}
							}
							break;
						default:
							val = str + 2;
							break;
						}
					}
				}
				nCol++;
			}
			break;
		}
	}

	return true;
}

bool CVTExtended::LoadFromStorage(CSerialStorage& Storage)
{
	char *str = Storage.ReadString();

	if( str == NULL ) return false;

	if( strncmp(str, "{IndexedTable:", 14) != 0 ) return false;
	
	return LoadFromStorage2(Storage);
}

bool parse_string(char*& ptr, CString& Token)
{
	if( ptr == NULL || *ptr == '\0' ) return false;

	char *token = NULL;
	if( *ptr == '"' )
	{
		token = ptr + 1;
		char *pos = strchr(ptr+1, '"');
		while( pos != NULL && *(pos+1) == '"' ) pos = strchr(pos+2, '"');
		if( pos )
		{
			*pos = '\0';
			ptr = pos + 1;
		}
		else
			ptr = NULL; //строка кончилась

		if( ptr != NULL && *ptr == ';' ) ptr++;

		Token = token;
		Token.Replace("\"\"", "\"");
	}
	else
	{
		token = ptr;
		ptr = strchr(ptr, ';');
		if( ptr )
		{
			*ptr = '\0';
			ptr++;
		}
		else
		{
			ptr = NULL;
		}
		Token = token;
	}

	return true;
}

bool CVTExtended::LoadFromStorage_CSV(CSerialStorage& Storage, bool bInnerFormat, bool bWithColNames)
{
	char *ptr;
	CString Token;
	int nCols = 0, nCol, nRow;
	CVTExtRow *pRow;

	procCleanup(NULL);

	if( bWithColNames )
	{
		ptr = Storage.ReadString();
		while( parse_string(ptr, Token) )
		{
			NewColumn(Token);
			nCols++;
		}
	}

	while( (ptr = Storage.ReadString()) != NULL )
	{
		nRow = NewRow();
		pRow = GetRow(nRow);
		nCol = 0;

		//парсим строку, учитывая тот факт, что строки могут заключаться в кавычки
		while( parse_string(ptr, Token) )
		{
			UnQuoteString(Token);

			if( nCol >= nCols )
			{
				CString ColName;
				ColName.Format("__column__%i", nCol+1);
				NewColumn(ColName);
				nCols++;
			}
			if( bInnerFormat )
				pRow->GetValue(nCol).LoadFromString(Token, 0);
			else
				pRow->GetValue(nCol) = Token;

			nCol++;
		}
	}
	
	return true;
}

BOOL CVTExtended::defsLoadFromFile(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		break;
	case 1:
		*param = 1L;
		return TRUE; 
		break;
	case 2:
		*param = 1L;
		return TRUE; 
		break;
	}
	return FALSE;
}

BOOL CVTExtended::funcLoadFromFile(CValue& RetVal, CValue** params)
{
	CString FileName = params[0]->GetString();
	int format = params[1]->GetNumeric();
	bool bWithColNames = params[2]->GetNumeric() != 0L;

	FILE *f = fopen(FileName, "rt");
	if( f == NULL )
		RuntimeError("Невозможно открыть файл %s! %s", FileName, strerror(errno));
	CSerialStorage Storage(f);

	switch( format )
	{
	case 1:
	case 2:
		RetVal = LoadFromStorage(Storage) ? 1 : 0;
		break;
	case 3:
		RetVal = LoadFromStorage_CSV(Storage, true, bWithColNames) ? 1 : 0;
		break;
	case 4:
		RetVal = LoadFromStorage_CSV(Storage, false, bWithColNames) ? 1 : 0;
		break;
	}

	fclose(f);

	return TRUE;
}

BOOL CVTExtended::defsSaveToString(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0:
		*param = 1L;
		return TRUE; 
		break;
	case 1:
		*param = "";
		return TRUE; 
		break;
	}
	return FALSE;
}

BOOL CVTExtended::funcSaveToString(CValue& RetVal, CValue** params)
{
	bool bInnerFormat = params[0]->GetNumeric() != 0L;
	CVTExtIndex *pIndex = GetIndex(params[1]);
	CSerialStorage Storage;
	SaveToStorage(Storage, pIndex, bInnerFormat);
	RetVal = Storage.GetResultString();
	return TRUE;
}

BOOL CVTExtended::procLoadFromString(CValue** params)
{
	CSerialStorage Storage(params[0]->GetString());
	LoadFromStorage(Storage);
	return TRUE;
}


BOOL CVTExtended::defsChooseLine(int nParam, CValue* param) const
{
	switch( nParam )
	{
	case 0: *param = 0L; return TRUE; break;
	case 1: *param = ""; return TRUE; break;
	case 2: *param = 0L; return TRUE; break;
	case 3: *param = ""; return TRUE; break;
	}
	return FALSE;
}

BOOL CVTExtended::funcChooseLine(CValue& RetVal, CValue** params)
{
	CValue VT;
	VT.CreateObject("ValueTable");

	CValueTable* pVT = CValue2VT(VT);
	CVTExtIndex* pIndex = GetIndex(params[3]);

	UnloadToVT(pVT, pIndex, NULL, false);

	CValue* pParams[3] = {params[0], params[1], params[2]};
	::CallAsFunc(VT.GetContext(), "ChooseLine", RetVal, pParams);

	return TRUE;
}

BOOL CVTExtended::defsShowTable(int nParam, CValue* param) const
{
	if(0 == nParam ){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

BOOL CVTExtended::funcShowTable(CValue& RetVal, CValue** params)
{
	CValue VT;
	VT.CreateObject("ValueTable");

	CValueTable* pVT = CValue2VT(VT);
	CVTExtIndex* pIndex = GetIndex(params[0]);

	UnloadToVT(pVT, pIndex, NULL, false);

	CValue p1, p2, p3, *pParams[3] = {&p1, &p2, &p3};
	::CallAsFunc(VT.GetContext(), "ChooseLine", RetVal, pParams);

	return TRUE;
}

//===========================================================================
// Inner methods
//===========================================================================
void CVTExtended::CheckBounds_Row(int row) const
{
	if( row < 0 )
		RuntimeError("Номер строки не может быть меньше 1!");
	if( row > Rows.GetUpperBound() )
		RuntimeError("Неверно указан номер строки!  Номер строки = %i, всего строк = %i", row+1, Rows.GetUpperBound()+1);
}
void CVTExtended::CheckBounds_Col(int col) const
{
	if( col < 0 )
		RuntimeError("Номер колонки не может быть меньше 1!");
	else if( col >= ColumnNames.GetCount() )
		RuntimeError("Неверно указан номер колонки!  Номер колонки = %i, всего колонок = %i", col+1, ColumnNames.GetCount());
}
void CVTExtended::CheckBounds(int row, int col) const
{
	CheckBounds_Row(row);
	CheckBounds_Col(col);
}

CVTExtIndex* CVTExtended::GetIndex(CValue* IndexName, BOOL bQuiet)
{
	if( IndexName->IsEmpty() )
		return DefaultIndex;
	else
		return Indexes.GetAt(IndexName, bQuiet);
}

void CVTExtended::SetCurrentRow(CVTExtIndex* Index)
{
	if( Index != DefaultIndex ) DefaultIndex->SetCurrentRow(Index->GetCurrentRow());
}

CValue& CVTExtended::GetValue(int row, int col)
{
	CheckBounds(row, col);
	return GetRow(row)->GetValue(col);
}

void CVTExtended::SetValue(int row_num, int col_num, CValue const& val)
{
	CVTExtRow *row = GetRow(row_num);
	row->SetValue(col_num, val);
	Indexes.OnChangeValue(row_num, col_num);
}


LPCSTR CVTExtended::ColumnName(int ColNum) const
{
	POSITION pos = ColumnNames.GetStartPosition();
	LPCSTR ColName;
	int i;
	while( pos )
	{
		ColumnNames.GetNextAssoc(pos, ColName, i);
		if( i == ColNum ) return ColName;
	}
	return NULL;
}

int CVTExtended::ColumnNumber(CString const& ColName, bool bQuiet)
{
	int nCol;
	if( !ColumnNames.Lookup(ColName, nCol) )
	{
		if( !bQuiet ) RuntimeError("Колонка '%s' не найдена!", (LPCSTR)ColName);
		return -1;
	}
	return nCol;
}

int CVTExtended::ColumnNumber(CValue* ColNum, bool bQuiet)
{
	int col_num;
	switch( ColNum->GetTypeCode() )
	{
	case 1:
		col_num = (int)(ColNum->GetNumeric()) - 1;
		CheckBounds_Col(col_num);
		break;
	case 2:
		col_num = ColumnNumber(ColNum->GetString(), bQuiet);
		break;
	default:
		col_num = ColumnNumber(ColNum->Format(), bQuiet);
	}
	return col_num;
}


//==================================================================================
// Методы для работы с динамическими свойствами
//==================================================================================
/*
int CVTExtended::CallAsFunc(int nMethod, CValue& ReturnValue, CValue** Parameters)
{
	try {
		return CMyContextBase::CallAsFunc(nMethod, ReturnValue, Parameters);
	} catch(CMemoryException err)
	{
		RuntimeError("Непонятная ошибка в методе ИндексированнаяТаблица::%s", m_MethDef[nMethod].Names[1]);
		return FALSE;
	}
}

int CVTExtended::CallAsProc(int nMethod, CValue** Parameters)
{
	try {
		return CMyContextBase::CallAsProc(nMethod, Parameters);
	} catch(CMemoryException err)
	{
		RuntimeError("Непонятная ошибка в методе ИндексированнаяТаблица::%s", m_MethDef[nMethod].Names[1]);
		return FALSE;
	}
}
*/

int CVTExtended::GetNProps(void)const
{
	return ColumnNames.GetCount() + VTExt_FixedProp_Last;
}

int CVTExtended::FindProp(char const * Name)const
{
	int i;
	if( FixedProps.Lookup(Name, i) )
	{
		return i;
	}
	if( ColumnNames.Lookup(Name, i) )
	{
		return i + VTExt_FixedProp_Last;
	}
	return -1;
}

char const *  CVTExtended::GetPropName(int A, int B)const
{
	if( A <= ColumnNames.GetCount() ) return ColumnName(A);
	return NULL;
}

int CVTExtended::GetPropVal(int iPropNum, class CValue & lValue) const
{
	switch( iPropNum )
	{
	case VTExt_FixedProp_CurrentRowRus:
	case VTExt_FixedProp_CurrentRowEng:
		propGetCurrentRow(lValue);
		break;
	default:
		DefaultIndex->GetCurrentValue(iPropNum - VTExt_FixedProp_Last, lValue);
	}
	return 1;
}

int CVTExtended::SetPropVal(int iPropNum, class CValue const & vValue)
{
	switch( iPropNum )
	{
	case VTExt_FixedProp_CurrentRowRus:
	case VTExt_FixedProp_CurrentRowEng:
		propSetCurrentRow(vValue);
		break;
	default:
		DefaultIndex->SetCurrentValue(iPropNum - VTExt_FixedProp_Last, vValue);
		Indexes.OnChangeValue(DefaultIndex->GetCurrentRow(), iPropNum - VTExt_FixedProp_Last);
	}
	return 1;
}

int CVTExtended::IsPropReadable(int iPropNum)const
{
	return 1;
}

int CVTExtended::IsPropWritable(int iPropNum)const
{
	return 1;
}

int CVTExtended::SaveToString(CString &str)
{
	CSerialStorage Storage("");

	CVTExtIndex *pIndex = DefaultIndex;
	if( !(pIndex->IsEmpty()) ) pIndex = new CVTExtIndex("", this);

	SaveToStorage(Storage, pIndex, true);

	if( pIndex != DefaultIndex ) delete pIndex;

	str = Storage.GetResultString();

	return TRUE;
}


//======================================================================================
//======================================================================================
//======================================================================================
CDocContext* CValue2Doc(CValue const& Value)
{
	int nTypeCode = Value.GetTypeCode();
    CBLContext* pContext = Value.GetContext();
	if( pContext == NULL ) return NULL;

	if( nTypeCode == DOCUMENT_TYPE_1C )
	{
		return (CDocContext*)pContext;
	}

	if( nTypeCode != AGREGATE_TYPE_1C ) 
		return NULL;

	if( strcmp(pContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") != 0 )
	{
		if( strcmp(pContext->GetRuntimeClass()->m_lpszClassName, "CGroupContext") == 0 )
		{
			CValue vRetVal;
			if( CallAsFunc(pContext, "ТекущийДокумент", vRetVal, NULL) )
				return (CDocContext*)pContext;
			else
				return NULL;
		}
		else
		{
			return NULL;
		}
	}

	CValue vBaseClass;
	if( static_cast<CComponentClass*>(pContext)->GetBaseClass("Документ", vBaseClass) )
	{
		return (CDocContext*)(vBaseClass.GetContext());
	}
	else
	{
		CValue vKind;
		if( !CallAsFunc(pContext, "Вид", vKind, NULL) ) return NULL;
		CString sType = "Документ.";
		sType += vKind.GetString();
		CValue vType(sType);
		if( static_cast<CComponentClass*>(pContext)->GetBaseClass(vType, vBaseClass) )
		{
			return (CDocContext*)(vBaseClass.GetContext());
		}
	}

	return NULL;
}

CValue* CValueCurDoc2Doc(CValue const& Value)
{
	int nTypeCode = Value.GetTypeCode();
    CBLContext* pContext = Value.GetContext();

	if( nTypeCode == DOCUMENT_TYPE_1C )
	{
		if( pContext == NULL )
		{
			CValue* vDoc = new CValue;
			vDoc->CreateObject("Документ");
			pContext = vDoc->GetContext();
			CValue* params[1] = {(CValue*)&Value};
			CValue RetVal;
			if( !::CallAsFunc(pContext, "НайтиДокумент", RetVal, params) )
				return NULL;
			if( RetVal.GetNumeric() == 0 )
				return NULL;
			return vDoc;
		}
	}

	return NULL;
}


CBLContext* CValue2Query(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) 
		return NULL;

    CBLContext* pContext = Value.GetContext();
	if( pContext == NULL ) return NULL;

	if( strcmp(pContext->GetRuntimeClass()->m_lpszClassName, "CRGcontext") != 0 )
		return NULL;

	return pContext;
}

bool CValue2Bool(CValue const& val)
{
	if( val.GetTypeCode() != NUMBER_TYPE_1C ) return false;
	return (int)(val.GetNumeric()) == 0 ? false : true;
}

bool CallAsFunc(CBLContext* pContext, char const* sFuncName, CValue& RetVal, CValue** ppParams)
{
	int nMethod = pContext->FindMethod(sFuncName);
	if (-1 == nMethod)
		RuntimeError("::CallAsFunc - у контекста <%s> не найден метод <%s>", pContext->GetTypeString(), sFuncName);

	int res = pContext->CallAsFunc(nMethod, RetVal, ppParams);
	//Msg("%s: %i =  %i", sFuncName, nMethod, res);
	return res != 0;
}

bool CallAsProc(CBLContext* pContext, char const* sProcName, CValue** ppParams)
{
	int nMethod = pContext->FindMethod(sProcName);
	if (-1 == nMethod)
		RuntimeError("::CallAsProc - у контекста <%s> не найден метод <%s>", pContext->GetTypeString(), sProcName);

	int res = pContext->CallAsProc(nMethod, ppParams);
	//Msg("%s: %i =  %i", sProcName, nMethod, res);
	return res != 0;
}

bool GetPropVal(CBLContext* pContext, char const* sPropName, CValue& RetVal)
{
	int nProp = pContext->FindProp(sPropName);
	if (-1 == nProp)
		RuntimeError("::GetPropVal - у контекста <%s> не найдено свойство <%s>", pContext->GetTypeString(), sPropName);

	int res = pContext->GetPropVal(nProp, RetVal);
	return res != 0;
}

int GetVTColumnNumber(CVTExtended* pVT, CValue const* pVal, CValue const* pDefVal)
{
	CValue* pRealVal = (CValue*)pVal;

	if( pVal->IsEmpty() ) pRealVal = (CValue*)pDefVal;
	if( pRealVal->IsEmpty() ) return -1;

	if( pRealVal->GetTypeCode() == NUMBER_TYPE_1C )
		return (int)(pRealVal->GetNumeric()) - 1;
	else
	{
		try 
		{
			return pVT->ColumnNumber(pRealVal);
		}
		catch(...)
		{
			return -1;
		}
	}
}

int GetVTColumnNumber(CValueTable* pVT, CValue const* pVal, CValue const* pDefVal)
{
	CValue* pRealVal = (CValue*)pVal;

	if( pVal->IsEmpty() ) pRealVal = (CValue*)pDefVal;
	if( pRealVal->IsEmpty() ) return -1;

	if( pRealVal->GetTypeCode() == NUMBER_TYPE_1C )
		return (int)(pRealVal->GetNumeric()) - 1;
	else
		return pVT->GetColumnIdx(pRealVal->GetString());
}



//______________________________________________________________________________
//
// Функции для контроля за памятью
//______________________________________________________________________________
//
//HEAP WALKER
//______________________________________________________________________________
long crtwalk(bool bShowMsg)
{
	_HEAPINFO hinfo;
	int heapstatus;
	char *buf;
	DWORD dwUsed = 0;
	DWORD dwFree = 0;
	int nUsed = 0, nFree = 0;
	
	hinfo._pentry = NULL;
	while( (heapstatus = _heapwalk( &hinfo )) == _HEAPOK )
	{
		if  ( hinfo._useflag == _USEDENTRY )
		{
			dwUsed += hinfo._size;
			nUsed++;
		}
		else
		{
			dwFree += hinfo._size;
			nFree++;
		}
	}
	switch( heapstatus )
	{
	case _HEAPEMPTY:
		buf = "OK - empty heap";
		break;
	case _HEAPEND:
		buf = "OK";
		break;
	case _HEAPBADPTR:
		buf = "ERROR - bad pointer to heap";
		break;
	case _HEAPBADBEGIN:
		buf = "ERROR - bad start of heap";
		break;
	case _HEAPBADNODE:
		buf = "ERROR - bad node in heap";
		break;
	}

	if( bShowMsg )
	{
		Msg("Heap( ----- CRT ----- ): Total: %u ( %u block(s) ); Used: %u ( %u block(s) ); Free: %u; Status: %s;",
			dwUsed + dwFree, nUsed + nFree, dwUsed, nUsed, dwFree, buf);
	}
	
	return dwUsed + dwFree;
}

long heapwalk(HANDLE hHeap, bool bShowMsg)
{
	PROCESS_HEAP_ENTRY heapEntry;
	DWORD dwTotal = 0;
	DWORD dwOverhead = 0;
	DWORD dwCommitted = 0;
	DWORD dwUnCommitted = 0;
	int nEntries = 0;

	heapEntry.lpData = NULL;
	heapEntry.wFlags = PROCESS_HEAP_REGION;

	while ( HeapWalk( hHeap, &heapEntry ) )
	{
		if ( heapEntry.wFlags & PROCESS_HEAP_REGION )
		{
			dwCommitted += heapEntry.Region.dwCommittedSize;
			dwUnCommitted += heapEntry.Region.dwUnCommittedSize;
		}
		if ( heapEntry.wFlags & PROCESS_HEAP_ENTRY_BUSY )
		{
			dwTotal += heapEntry.cbData;
			dwOverhead += heapEntry.cbOverhead;
			nEntries++;
		}
	}
	if( bShowMsg )
		Msg("Total bytes in use: %u ( %u entries ); Overhead: %u; Committed: %u; UnCommitted: %u",
			dwTotal, nEntries, dwOverhead, dwCommitted, dwUnCommitted);

	return dwTotal;
}

BOOL CVTExtended::funcHeapInfo(CValue& RetVal, CValue **params)
{
	bool bShowMsg = CValue2Bool(*params[0]);

	RetVal = 0L;
	HANDLE hHeap = CVTExtHeap::Get();
	if( hHeap )
	{
		RetVal = heapwalk(hHeap, bShowMsg);
	}

	return TRUE;
}

BOOL CVTExtended::funcCRTInfo(CValue& RetVal, CValue **params)
{
	bool bShowMsg = CValue2Bool(*params[0]);
	RetVal = 0L;
	RetVal = crtwalk(bShowMsg);
	return TRUE;
}
