#ifndef OLE_DB_UTILS_H
#define OLE_DB_UTILS_H

#pragma once

class CVTExtended;

namespace OleDb {

class CResultSequentialAccess;

void ResultToValueTable (CResultSequentialAccessProxy const &Result,
	CValueTable &Table, bool Clear);

void ResultToIndexedTable (CResultSequentialAccessProxy const &Result,
	CVTExtended &Table, bool Clear);

} // namespace OleDb

#endif // OLE_DB_UTILS_H
