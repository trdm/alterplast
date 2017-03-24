//////////////////////////////////////////////////////////////////////
// Класс ИндексированнаяТаблица
//
// Автор: Диркс Алексей aka ADirks
// Контакты: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////


#pragma once

#include "../mycontextbase.h"
#include "RBTree.h"
#include "../StringArrayEx.h"
#include "SerialStorage.h"
#include "../V7SerializeToString.h"
#include <deque>
#include "CVCompare.h"

#include "../SQLITE1C/resultloader.h"

class CVTIndexRecord;
class CVTExtended;


//typedef CIStringMap<int, int> CIStringMapInt;
typedef CRBTree<CVTIndexRecord, CVTIndexRecord&>  CVTExtIndexTree;
typedef CBinaryTreeNode<CVTIndexRecord> CVTExtIndexTreeNode;


CDocContext* CValue2Doc(CValue const& Value);
CValue* CValueCurDoc2Doc(CValue const& Value);
CBLContext* CValue2Query(CValue const& Value);
bool CValue2Bool(CValue const& val);

int GetVTColumnNumber(CValueTable* pVT, CValue const* pVal, CValue const* pDefVal);
int GetVTColumnNumber(CVTExtended* pVT, CValue const* pVal, CValue const* pDefVal);


class CVTExtHeap
{
private:
	static HANDLE hHeap;

public:
	CVTExtHeap();
	~CVTExtHeap();

	static HANDLE Get() {return hHeap;}
};


///////////////////////////////////////////////////////////////////////////
//////  CVTExtRow - строка таблицы ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class CVTExtRow
{
private:
	CValue* pValues;
	int nSize;
	int nMemSize;

	CValue* AllocMem(int nSize);
	CValue* ReAllocMem(CValue* pCurrentV, int nCurrentSize, int nNewSize);
	void FreeMem(CValue* pValues, int nSize);

public:
	CVTExtRow(int numValues);
	~CVTExtRow();

	int NewColumn();
	void RemoveColumn(int col_num);

	int GetSize() { return nSize; }


	CVTExtRow& operator= (CVTExtRow& SrcRow);
	CValue& GetValue(int i) { return pValues[i]; }
	void SetValue(int i, CValue const& val) { pValues[i] = val; }

#ifdef _NDEBUG
	CString Format();
#endif
};

///////////////////////////////////////////////////////////////////////////
//////  Классы, реализующие функционал индекса  ///////////////////////////
///////////////////////////////////////////////////////////////////////////

// Колонки индекса
class CVTExtIndexFields : public vector<TVTIndexDescrRecord>
{
private:
	CString m_IndexExpr;
public:
	CVTExtIndexFields() {};
	CVTExtIndexFields(CString& IndexExpr, CVTExtended* pVT);
	CVTExtIndexFields& operator= (CVTExtIndexFields const& src);

	bool Build(CString const& IndexExpr, CVTExtended* pVT);
	CString& GetIndexExpr() { return m_IndexExpr; };
};

// 
class CVTExtGroupingRecord
{
public:
	CString IndexName;
	CVTExtIndexFields IndexFields;

public:
	int BySubcontoGroups();
};

class CVTExtGroupings : public CArray<CVTExtGroupingRecord, CVTExtGroupingRecord&>
{
public:
	CVTExtGroupings(CString& Groupings, CVTExtended* pVT);
};

//Структура, использующаяся для сравнения строк таблицы
class CVTIndexRecord
{
public:
	class CVTExtRow *Row;
	CVTExtIndexFields *IndexFields;

public:
	CVTIndexRecord() {}
	CVTIndexRecord(CVTExtRow *aRow, CVTExtIndexFields *aIndexFields) { Row = aRow; IndexFields = aIndexFields; }
	~CVTIndexRecord() {}
	CVTIndexRecord& operator= (CVTIndexRecord const& rec) { Row = rec.Row; IndexFields = rec.IndexFields; return *this; }

	int compare(CVTIndexRecord& RightRecord);
};

///////////////////////////////////////////////////////////////////////////
class CVTExtFilter
{
public:
	CVTExtRow Row_min, Row_max;
	CVTIndexRecord Rec_min, Rec_max;
	CVTIndexRecord compare_rec;
	bool bOnlyUnique;
	bool bInverse;

	CVTExtFilter(int ColumnsCount, CVTExtIndexFields *pIndexFields);
	CVTExtFilter(int ColumnsCount, CVTExtIndexFields *pIndexFields, CValue const& Min, CValue const& Max);
	void Set(CValue const& Min, CValue const& Max);

	void FillMinRow();
	void FillMaxRow();


	int CompareRow(CVTExtRow* pRow);
	bool CheckRow(CVTExtRow* pRow);

	CVTExtFilter& operator= (CVTExtFilter& SrcFilter);

	void NewColumn() { Row_min.NewColumn(); Row_max.NewColumn(); }
	void RemoveColumn(int col_num) { Row_min.RemoveColumn(col_num); Row_max.RemoveColumn(col_num); }
};

///////////////////////////////////////////////////////////////////////////
class CVTExtIndex : public CVTExtIndexTree, public CCollectionItem
{
	friend class CVTExtended;
	friend class CVTExtIterator;
	friend class CVTExtProvider;

private:
	CVTExtended* pVT;
	CVTExtIndexFields IndexFields;

	CVTExtRow KeyRow;
	CVTExtFilter*   pFilter;
	CVTExtIterator* pIterator;

	struct stIteratorState
	{
		stIteratorState(CVTExtFilter* pF, CVTExtIterator* pI, int nVersion) : m_pFilter(pF), m_pIterator(pI), m_nVersion(nVersion) {};
		CVTExtFilter*   m_pFilter;
		CVTExtIterator* m_pIterator;
		int m_nVersion;
	};

	CTypedPtrArray<class CPtrArray, stIteratorState*> m_IteratorStack;

	int nCurrentRow;
	CVTExtRow* pCurrentRow;

	int Version; //increments after any changes in tree

public:
	CVTExtIndex(LPCSTR Name, CString const& IndexExpression, bool bUnique, CVTExtended* vt);
	CVTExtIndex(LPCSTR Name, CVTExtIndexFields const& IndexFields, bool bUnique, CVTExtended* vt);
	CVTExtIndex(LPCSTR Name, CVTExtended* vt);
	~CVTExtIndex();
	CVTExtIndex& operator= (CVTExtIndex& SrcIndex);

	void Reindex();
	void Reindex(CString& IndexExpression);
	void Reindex(CVTExtIndexFields& TheIndexFields);
	void DropIndex();
	CVTExtIterator* CreateIterator(bool bUnique = false); //создаёт итератор в соответствии с текущими настройками
	bool IsIndexed() { return IndexFields.size() > 0; }

	void InsertRow(CVTExtRow* pRow, int nRow);
	void RemoveRow(int nCol);
	void RemoveRowFromTree(int nRow);
	void RemoveAllRows();
	void OnChangeRow(int nCol);
	void OnChangeValue(int nRow, int nCol);

	int GetVersion() { return Version; }

	bool ColumnIndexed(int nCol);
	void NewColumn();
	void RemoveColumn(int nCol);
	CString& GetIndexExpr() { return IndexFields.GetIndexExpr(); };

	CVTExtIndexTreeNode* Find(CValueItemList* pKeyValues);
	int Find(CValue const& What, bool FindLast, bool ChangePosition);
	int FindNearestGE(CValue const& What, bool ChangePosition);
	int FindNearestLE(CValue const& What, bool ChangePosition);

	int KeyCount(CValue const& What);

	void SetFilter(CValue const& Min, CValue const& Max, bool bUnique, bool bInverse);
	void SetFilter(CValue const& SubSet, int nCols);
	void DropFilter();
	int GetRowsCount(bool bUnique); //количество строк по фильтру

	bool First(bool bUnique = false);
	bool Last(bool bUnique = false);
	bool Next(bool bUnique = false);
	bool Prev(bool bUnique = false);

	int  GetCurrentRow() { return nCurrentRow; }
	void SetCurrentRow(int nRow);
	void SetCurrentValue(int nCol, CValue const& NewVal);
	void GetCurrentValue(int nCol, CValue& RetVal);

	void GroupBy(CString Columns);
	void Sum(int nCol, CValue& RetVal);
	void NodeSum(int nCol, CValue& RetVal);
	void SumByFilter(int nCol, CVTExtFilter& Filter, CValue& RetVal);
	void SumByRange(int nCol, CValue& min, CValue& max, CValue& RetVal);

	int SaveIterator();
	BOOL RestoreIterator(int nIndex);

private:
	void ResetPosition();
	int PositioningOnNode(CVTExtIndexTreeNode* node, bool FindLastRow, bool ChangePosition);
	void RemoveRowFromTree(int nRow, int delta);

	void SumByIndexIterator(int nCol, CValue& RetVal);
	void SumAllRows(int nCol, CValue& RetVal);
};

// Iterators
class CVTExtIterator
{
protected:
	CVTExtIndex* pIndex;
	class CVTExtended* pVT;
	
	int nCurrentRow;
	CVTExtRow *pCurrentRow;
	CVTExtIndexTreeNode* pCurrentNode;
	int IndexInNode;
	bool SelStarted;

	void PASCAL ResetPosition();
	void PASCAL SetCurrentRow();

	// unique iterator flag
	bool m_bUnique;
public:
	CVTExtIterator(CVTExtIndex* pIndex, bool bUnique);
	virtual ~CVTExtIterator() {}

	virtual bool IsValid() = 0;
	virtual bool First() = 0;
	virtual bool Last() = 0;
	virtual bool NextNode() = 0;
	virtual bool PrevNode() = 0;
	virtual bool Next() = 0;
	virtual bool Prev() = 0;
	bool IsDone() { return nCurrentRow >= 0; }

	//принудительная установка итератора. Предназначена для методов типа Find()
	void SetPosition(CVTExtIndexTreeNode* pNode, int nIndexInNode, CVTExtRow* pRow, int nRow);

	CVTExtIndexTreeNode* CurrentNode() { return pCurrentNode; }

	int CurrentRowIndex()   { return nCurrentRow; }
	CVTExtRow* CurrentRow() { return pCurrentRow; }
};

class CVTExtIteratorSimple : public CVTExtIterator
{
public:
	CVTExtIteratorSimple(CVTExtended* pVT);

	virtual bool IsValid() {return true;}
	virtual bool First();
	virtual bool Last();
	virtual bool NextNode() {return Next(); }
	virtual bool PrevNode() {return Prev(); }
	virtual bool Next();
	virtual bool Prev();
};

class CVTExtIteratorIndex : public CVTExtIterator
{
	friend class CVTExtIteratorFilter;
private:
	int ValidIndexVersion;
	void Validate() { ValidIndexVersion = pIndex->GetVersion(); }

public:
	CVTExtIteratorIndex(CVTExtIndex* pIndex, bool bUnique);

	virtual bool IsValid();
	virtual bool First();
	virtual bool Last();
	virtual bool NextNode();
	virtual bool PrevNode();
	virtual bool Next();
	virtual bool Prev();

protected:
	void GoToNode_Backward();
};

class CVTExtIteratorFilter : public CVTExtIteratorIndex
{
private:
	CVTExtFilter* pFilter;

	void PASCAL SecondDiapasonBegin();
	void PASCAL FirstDiapasonEnd();

public:
	CVTExtIteratorFilter(CVTExtIndex* pIndex, CVTExtFilter* pFilter, bool bUnique);

	virtual bool First();
	virtual bool Last();
	virtual bool NextNode();
	virtual bool PrevNode();
	virtual bool Next();
	virtual bool Prev();
};

///////////////////////////////////////////////////////////////////////////
/////  Коллекция индексов  ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class CVTExtIndexCollection: public CCollection
{
public:
	~CVTExtIndexCollection() { RemoveAll(); }

	CVTExtIndex* operator [](int nIndex) const;
	CVTExtIndex* operator [](LPCSTR szName) const;
	CVTExtIndex* operator [](CValue* Index) const;
	CVTExtIndex* GetAt(CValue* Index, BOOL bQuiet) const;

	int AddIndex(CVTExtIndex* pIndex);
	int AddIndex(CString const& IndexName, CString& IndexExpr, bool bUnique, CVTExtended* pVT);
	int AddIndex(CString const& IndexName, CVTExtIndexFields const& IndexFields, bool bUnique, CVTExtended* pVT);
	void RemoveIndex(int nIndex);
	void RemoveIndex(CVTExtIndex* Index);
	void RemoveAll();

	void Reindex();

	bool ColumnIndexed(int col_num);
	void NewColumn();
	void RemoveColumn(int col_num);

	void InsertRow(CVTExtRow* row, int row_num);
	void RemoveRow(int row_num);
	void RemoveAllRows();
	void OnChangeValue(int row_num, int col_num);
	void OnChangeRow(int row_num);
};

///////////////////////////////////////////////////////////////////////////
//////  CVTExtended - самый главный класс /////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class CVTExtended : public CMyContextBase, public IV7SerializeToString, public ISQLiteResultLoader//, public IResultLoader
{
	DECLARE_DYNCREATE(CVTExtended);

	friend class CVTExtIndex;
	friend class CVTExtProvider;

private:
	CPtrArray Rows;
	static CIStringMapToIndex FixedProps;
	CIStringMapToIndex ColumnNames;
	
	CVTExtIndexCollection Indexes;
	CVTExtIndex* DefaultIndex;

	//=== Вспомогательные типы для копирования данных из одного объекта в другой ===
	class TColumnsMapPair
	{
	public:
		int nSrcCol;
		int nDestCol;
	};
	typedef vector<TColumnsMapPair> TColumnsMap;
	//Интерфейс к таблице с колонками, позволяющий этими колонками манипулировать
	class TColumnedTable
	{
	public:
		virtual int GetColumnsCount() = 0;
		virtual int ColumnNumber(CString const& ColName) = 0;
		virtual LPCSTR ColumnName(int nCol) = 0;
		virtual int NewColumn(CString const& ColumnName) = 0;
	};
	class TColumnedTable_VTExt : public TColumnedTable
	{
	private:
		CVTExtended *pVTExt;
	public:
		TColumnedTable_VTExt(CVTExtended *_pVTExt) {pVTExt = _pVTExt;};
		int GetColumnsCount() {return pVTExt->GetColumnsCount();};
		int ColumnNumber(CString const& ColName) {return pVTExt->ColumnNumber(ColName, true);};
		LPCSTR ColumnName(int nCol) {return pVTExt->ColumnName(nCol);};
		int NewColumn(CString const& ColumnName) {return pVTExt->NewColumn(ColumnName);};
	};
	class TColumnedTable_VT : public TColumnedTable
	{
	private:
		CValueTable *pVT;
	public:
		TColumnedTable_VT(CValueTable *_pVT) {pVT = _pVT;};
		int GetColumnsCount() {return pVT->GetColumnCount();};
		int ColumnNumber(CString const& ColName) {return pVT->GetColumnIdx(ColName);};
		LPCSTR ColumnName(int nCol) {
			CString const& sColName = pVT->GetColumnCode(nCol);
			if( sColName.IsEmpty() )
				return NULL;
			else
				return sColName;
		};
		int NewColumn(CString const& ColumnName) {return pVT->AddColumn(ColumnName, UNDEFINE_TYPE_1C, ColumnName, 0, "", 0);};
	};
	//===============================================================================

	enum TJoinType {joinLeft, joinRight, joinInner, joinFull};

private:
	void SetRow(int i, CVTExtRow* aRow) { Rows[i] = aRow; }

	void CheckBounds_Row(int row) const;
	void CheckBounds_Col(int col) const;
	void CheckBounds(int row, int col) const;

	CVTExtIndex* GetIndex(CValue* IndexName, BOOL bQuiet = FALSE);

	static void FillColumnNumbersMap(TColumnsMap &ColMap, TColumnedTable* pDest, TColumnedTable* pSrc, bool bJoinColumns, CValue* pColumns = NULL);
	static int* ColumnNumbersMap(CVTExtended* pDest, CVTExtended* pSrc, bool bJoinColumns, CValue* pColumns = NULL);
	static int* ColumnNumbersMap(CVTExtended* pDest, CValueTable* pSrc, bool bJoinColumns, CValue* pColumns = NULL);
	static int* ColumnNumbersMap(CVTExtended* pDest, CValueItemList* ppValueList);
	static void CopyRowFromVTExt(CVTExtRow *pDest, CVTExtRow *pSrc, int nColumns, int *pColNumbersMap);
	static void CopyRowFromVTExt(CVTExtRow *pDest, CVTExtRow *pSrc, TColumnsMap ColNumbersMap);
	static void CVTExtended::CopyRowFromVT(CVTExtRow *pDest, CValueTable *pSrc, int nSrcRow, TColumnsMap ColNumbersMap);
	static void CopyRowFromVL(CVTExtRow *pDest, CValueItemList* ppValueList, int *pColNumbersMap);
	void DuplicateRow(int nDestRow, int nSrcRow);

	void LoadStructure(CVTExtended* pVT);
	static void FillKeyRow(CVTExtRow & KeyRow, CVTExtRow& SrcRow, 
					CVTExtIndexFields& DestIdxFields, CVTExtIndexFields& SrcIdxFields, 
					int nKeyFields);

	void CalcSums(CVTExtIndexTreeNode* node, CVTExtRow* pSumRow, int* arrSumCols, int nSumCols);
	void DeleteUnneededRows(bool* deleted, int nRowsRest, bool bDelete);
	void Group(CVTExtGroupings& Groupings, int* arrSumCols, int nSumCols, bool bSaveLastLevel, bool bRemoveColumns, int nGrouping);
	bool AddToAllParentFolders(CVTExtended* pVTGroups,
							   int nRow, CVTExtRow* pRow,
							   int nRefCol, int nRefParentCol, int nParentCol,
							   int nChildrenCol,
							   int* arrSumCols, int nSumCols,
							   bool bIsGroup,
							   bool* deleted);
	void CalcGroupLevels(CVTExtended* pVTGroups, int nParentCol, int nLevelCol);
	void SumsBySubcontoFolders(CVTExtGroupings& Groupings, int* arrSumCols, int nSumCols, int nGrouping);

	BOOL Join(CValue** params, TJoinType JoinType);

	void PartiallyReindex(std::deque<bool>& bChangedRows, int nChangedColumn);


public:
	CVTExtended();
	~CVTExtended();

	BOOL procCleanup(CValue** params);

	int NewColumn(CString const& ColumnName);
	BOOL funcNewColumn(CValue& RetVal, CValue** params);
	void RemoveColumn(int nCol, bool bCheckIndexes = true);
	BOOL procRenameColumn(CValue** params);
	BOOL procRemoveColumn(CValue** params);
	LPCSTR ColumnName(int ColNum) const;
	BOOL funcGetColumnName(CValue& RetVal, CValue** params);
	BOOL funcGetColumnNumber(CValue& RetVal, CValue** params);
	int ColumnNumber(LPCSTR ColName, bool bQuiet = false) const;
	int ColumnNumber(CValue* ColNum, bool bQuiet = false) const;
	int GetColumnsCount() const { return ColumnNames.GetCount(); };
	BOOL funcGetColCount(CValue& RetVal, CValue** params);

	//operations with rows
	int NewRow(BOOL bReindex = TRUE);
	CVTExtRow* GetRow(int i) { return (CVTExtRow*)(Rows[i]); }
	void RemoveRow(int nRow, BOOL bIndex);
	BOOL funcNewRow(CValue& RetVal, CValue** params);
	BOOL procRemoveRow(CValue** params);
	BOOL procRemoveAllRows(CValue** params);
	BOOL defsRemoveAllRows(int nParam, CValue* param) const;
	int GetRowsCount() { return Rows.GetUpperBound() + 1; }

	BOOL defsGetRowsCount(int nParam, CValue* param) const;
	BOOL funcGetRowsCount(CValue& RetVal, CValue** params);
	void propGetCurrentRow(CValue& RetVal) const { RetVal = 1L + DefaultIndex->GetCurrentRow(); }
	void propSetCurrentRow(CValue const& Value);
	void SetCurrentRow(CVTExtIndex* Index);
	BOOL funcGetCurrentRow(CValue& RetVal, CValue** params);
	BOOL defsGetCurrentRow(int nParam, CValue* param) const;

	//values access
	BOOL funcGetValue(CValue& RetVal, CValue** params); 
	int	defs_Get_Set_Value(int nParam, CValue* param)const;
	BOOL procSetValue(CValue** params);
	BOOL funcSum(CValue& RetVal, CValue** params);
	int	defsSum(int nParam, CValue* param)const;
	BOOL funcNodeSum(CValue& RetVal, CValue** params);
	int	defsNodeSum(int nParam, CValue* param)const;
	BOOL funcSumByRange(CValue& RetVal, CValue** params);
	int	defsSumByRange(int nParam, CValue* param)const;

	CValue& GetValue(int row, int col);
	void SetValue(int row, int col, CValue const& val);
	CValue& CurRow_GetValue(int col) const;
	void CurRow_SetValue(int col, CValue const& val);

	//operations with index
	BOOL funcAddIndex(CValue& RetVal, CValue** params);
	int	defsAddIndex(int nParam, CValue* param)const;
	BOOL procDropIndex(CValue** params);
	BOOL funcGetIndexCount(CValue& RetVal, CValue** params);
	BOOL funcGetIndexName(CValue& RetVal, CValue** params);
	BOOL funcGetIndexExpr(CValue& RetVal, CValue** params);
	BOOL funcGetIndexNumber(CValue& RetVal, CValue** params);

	BOOL procReindex(CValue** params);

	BOOL funcIndexIsUnique(CValue& RetVal, CValue** params);
	BOOL defsIndexIsUnique(int nParam, CValue* param) const;
	BOOL funcUniqueKeyCount(CValue& RetVal, CValue** params);
	BOOL defsUniqueKeyCount(int nParam, CValue* param) const;
	BOOL funcKeyValueCount(CValue& RetVal, CValue** params);
	BOOL defsKeyValueCount(int nParam, CValue* param) const;

	BOOL defsFindRow(int nParam, CValue* param) const;
	BOOL funcFindRow(CValue& RetVal, CValue** params);
	BOOL defsFindNearest(int nParam, CValue* param) const;
	BOOL funcFindNearestGE(CValue& RetVal, CValue** params);
	BOOL funcFindNearestLE(CValue& RetVal, CValue** params);
	
	BOOL defsSetFilter(int nParam, CValue* param) const;
	BOOL procSetFilter(CValue** params);
	BOOL defsSetFilter2(int nParam, CValue* param) const;
	BOOL procSetFilter2(CValue** params);
	BOOL procDropFilter(CValue** params);
	BOOL defsDropFilter(int nParam, CValue* param) const;

	BOOL defsNextRow(int nParam, CValue* param) const;
	BOOL funcFirstRow(CValue& RetVal, CValue** params);
	BOOL funcNextRow(CValue& RetVal, CValue** params);
	BOOL funcLastRow(CValue& RetVal, CValue** params);
	BOOL funcPrevRow(CValue& RetVal, CValue** params);

	BOOL procGroupBy(CValue** params);
	BOOL defsGroupBy(int nParam, CValue* param) const;

	BOOL procFillRow(CValue** params);
	BOOL defsFillRow(int nParam, CValue* param) const;
	BOOL procFillColumn(CValue** params);
	BOOL defsFillColumn(int nParam, CValue* param) const;

	BOOL procFillColumnByAlgorithm(CValue** params);
	BOOL defsFillColumnByAlgorithm(int nParam, CValue* param) const;

	void LoadFromVTExt(CVTExtended* pVTExt, CValue* IndexName, CValue* pColumns, bool bMerge, bool bUnique);
	void LoadFromVT(CValueTable* pVT, CValue* pColumns, bool bMerge);
	void UnloadToVT(CValueTable* pVT, CVTExtIndex* pIndex, CValue* pColumns, bool bUnique);
	void UnloadToDoc(CDocContext* pDoc, CVTExtIndex* pIndex, bool bUnique);
	void LoadFromDoc(CDocContext* pDoc, CValue& vColumns);
	void LoadFromQuery(CBLContext* pQuery, CValue** ppParams);

	BOOL defsUnloadTo(int nParam, CValue* param) const;
	BOOL procUnloadTo(CValue** params);
	BOOL defsLoadFrom(int nParam, CValue* param) const;
	BOOL procLoadFrom(CValue** params);
	BOOL defsLoadFromQuery(int nParam, CValue* param) const;
	BOOL procLoadFromQuery(CValue** params);
	BOOL defsUnionVT(int nParam, CValue* param) const;
	BOOL procUnionVT(CValue** params);

	BOOL defsUnloadRowTo(int nParam, CValue* param) const;
	BOOL procUnloadRowTo(CValue** params);
	void UnloadRowTo(CValue const& vDest, CValue const& vRow, CValue const& vColumns, CValue const& vUnloadNames);

	BOOL defsConjunctionVT(int nParam, CValue* param) const;
	BOOL procConjunctionVT(CValue** params);
	BOOL procDifferenceVT(CValue** params);

	//SQL-like joins
	BOOL defsInnerJoin(int nParam, CValue* param) const;
	BOOL procInnerJoin(CValue** params);
	BOOL defsLeftJoin(int nParam, CValue* param) const;
	BOOL procLeftJoin(CValue** params);
	BOOL defsRightJoin(int nParam, CValue* param) const;
	BOOL procRightJoin(CValue** params);
	BOOL defsFullJoin(int nParam, CValue* param) const;
	BOOL procFullJoin(CValue** params);

	//Saving/loading
	BOOL defsCopy(int nParam, CValue* param) const;
	BOOL funcCopy(CValue& RetVal, CValue** params);

	BOOL defsSaveToFile(int nParam, CValue* param) const;
	BOOL procSaveToFile(CValue** params);
	void SaveToStorage(CSerialStorage& Storage, CVTExtIndex *pIndex, bool InnerFormat);
	void SaveToStorage_CSV(CSerialStorage& Storage, CVTExtIndex *pIndex, bool InnerFormat, bool bWithColNames);

	BOOL defsLoadFromFile(int nParam, CValue* param) const;
	BOOL funcLoadFromFile(CValue& RetVal, CValue** params);
	bool LoadFromStorage(CSerialStorage& Storage);
	bool LoadFromStorage2(CSerialStorage& Storage);
	bool LoadFromStorage_CSV(CSerialStorage& Storage, bool bInnerFormat, bool bWithColNames);

	BOOL defsSaveToString(int nParam, CValue* param) const;
	BOOL funcSaveToString(CValue& RetVal, CValue** params);
	BOOL procLoadFromString(CValue** params);
	BOOL LoadFromString(LPCSTR szSource);

	//
	BOOL defsGroup(int nParam, CValue* param) const;
	BOOL procGroup(CValue** params);

	BOOL defsChooseLine(int nParam, CValue* param) const;
	BOOL funcChooseLine(CValue& RetVal, CValue** params);
	BOOL funcShowTable(CValue& RetVal, CValue** params);
	BOOL defsShowTable(int nParam, CValue* param) const;

	BOOL funcCRTInfo(CValue& RetVal, CValue **params);
	BOOL funcHeapInfo(CValue& RetVal, CValue **params);

	BOOL funcSaveIterator(CValue& RetVal, CValue **params);
	BOOL defsSaveIterator(int nParam, CValue* param) const;
	BOOL funcRestoreIterator(CValue& RetVal, CValue **params);
	BOOL defsRestoreIterator(int nParam, CValue* param) const;
public:
	// Методы для работы с динамическими свойствами
	//virtual int CallAsFunc(int nMethod, CValue& ReturnValue, CValue** Parameters);
	//virtual int CallAsProc(int nMethod, CValue** Parameters);
	virtual int				GetNProps(void)const;
	virtual int				FindProp(char const* szName)const;
	virtual char const* 	GetPropName(int nPropIndex, int nAlias)const;
	virtual int				GetPropVal(int nPropIndex, CValue& Value) const;
	virtual int				SetPropVal(int nPropIndex, CValue const& Value);
	virtual int				IsPropReadable(int nPropIndex)const;
	virtual int				IsPropWritable(int nPropIndex)const;
	virtual int             IsSerializable(void) { return TRUE; }
	virtual int				SaveToString(CString &str);

public:
	// интерфейс ISQLiteResultLoader

	// Подготовка приемника к приему результатов
	// Должен возвращать количество колонок, которое готов принять приемник.
	virtual DWORD init(DWORD colCountInResult, sqlite3* pBase, CValue* pClear);
	// Добавление колонки в результат запроса
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type);
	// Добавление строки в результат запроса.
	// Параметр - массив указателей на CValue, количеством равный количеству
	// добавленных колонок
	virtual void addValues(CValue** ppValues);
	// Присвоение возвращаемого значения
	virtual void assignRetValue(CValue& retVal);

private:
	DWORD m_colCount;

	// end of ISQLiteResultLoader

protected:
    virtual void Release() {DecrRef();};

	void GetFullColumnsList(bool bNeedColumnNames, std::pair<std::vector<int>, CStringArrayEx>* pPair) const;
	void GetColumnsListFromValue(const CValue& vColumns, bool bNeedColumnNames, std::pair<std::vector<int>, CStringArrayEx>* pPair) const;

	void UnloadToResultLoader(IResultLoader* pLoader, CVTExtIndex* pIndex, const CValue& vColumns, bool bUnique);

	DECLARE_MY_CONTEXT()
};
