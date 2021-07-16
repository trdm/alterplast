//поддержка агрегатных объектов для макро-языка

#define SMALL_TRANSLATE
#include <afxtempl.h>
#include <afxmt.h>
#include <math.h>

 

#include "Compiler\Functions.h"

//поддержка вывода ошибок
CString FindErrorCodeLine(CString &Buffer,int nCurPos,int nOnlyCodeLine);
class CByteCode *AfxGetCurrentByteCode();
void DoError();

//текущий исполняемый модуль
extern class CProcUnit *afxCurrentRunModule;
CString mUpper(CString );


#include "Compiler\Value.h"//Базовый класс всех переменных
#include "Compiler\ProcUnit.h"
#include "Compiler\ValueArray.h"

//вывод стандартных ошибок
void Error(int nErr,...);
void Error(LPCTSTR s,...);

//выполнение модуля
void RunModule(CString Str);
