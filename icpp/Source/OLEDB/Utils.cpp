#include "StdAfx.h"
#include "OleDbResult.h"
#include "ResultProxy.h"
#include "../VTExt/VTExtended.h"
#include "../Utils/CorrectMinMax.h"

namespace OleDb {

namespace {

void MakeValueTableStructureFromResult (
	CResultSequentialAccessProxy const &Result,
	CValueTable &Table)
{
	ULONG cCols = Result.GetNumFields ();

	CString strFormat;
	for(ULONG nCol = 0; nCol < cCols; ++nCol) {
		CString const &Name = Result.GetFieldName(nCol);
		CType const &Type = Result.GetFieldType(nCol);

		// Пока выяснил, что если типизируем поле как Перечисление (без указания вида),
		// то стоит создавать колонку нетипизированную (так делает сама 1с).
		// Про другие типы ничего пока не знаю
		//
		int TypeCode = Type.GetTypeCode();
		if((TypeCode == ENUM_TYPE_1C && Type.GetTypeID() == 0) ||
			TypeCode == CALCULATIONKIND_TYPE_1C) {

			Table.AddColumn (Name, CType(0), Name, 0, strFormat, 0);
		}
		else {
			Table.AddColumn (Name, Type, Name, 0, strFormat, 0);
		}
	}
} // MakeValueTableStructureFromResult

void MakeIndexedTableStructureFromResult (
	CResultSequentialAccessProxy const &Result,
	CVTExtended &Table)
{
	ULONG cCols = Result.GetNumFields ();

	CString strFormat;
	for(ULONG nCol = 0; nCol < cCols; ++nCol) {
		CString const &Name = Result.GetFieldName(nCol);
		Table.NewColumn (Name);
	}
} // MakeValueTableStructureFromResult

}

void ResultToValueTable (CResultSequentialAccessProxy const &Result,
	CValueTable &Table, bool Clear)
{
	int cCols = Result.GetNumFields ();

	if(Clear)
	{
		Table.Clear (TRUE);
		MakeValueTableStructureFromResult (Result, Table);
	}
	else
	{
		cCols = std::min (cCols, Table.GetColumnCount());
	}

	CValue Value;
	ULONG nRow = 0;

	while (Result.GetNextRow ())
	{
		Table.NewRow(nRow);

		for (ULONG nCol = 0; nCol < cCols; ++nCol) {
			Result.GetFieldValue(nCol, Value);
			Table.SetValue(Value, nCol, nRow);
		}

		++nRow;
	}
} // ResultToValueTable

void ResultToIndexedTable (CResultSequentialAccessProxy const &Result,
	CVTExtended &Table, bool Clear)
{
	int cCols = Result.GetNumFields ();

	if (Clear) {
		Table.procCleanup (NULL);
		MakeIndexedTableStructureFromResult (Result, Table);
	}
	else {
		cCols = std::min (cCols, Table.GetColumnsCount ());
	}

	CValue Value;

	while (Result.GetNextRow ())
	{
		CVTExtRow *pRow = Table.GetRow (Table.NewRow ());
		ASSERT (pRow);

		for (ULONG nCol = 0; nCol < cCols; ++nCol) {
			Result.GetFieldValue(nCol, Value);
			pRow->SetValue (nCol, Value);
		}
	}
} // ResultToIndexedTable

} // namespace OleDb
