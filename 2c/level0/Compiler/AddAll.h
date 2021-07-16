//��������� ���������� �������� ��� �����-�����

#define SMALL_TRANSLATE
#include <afxtempl.h>
#include <afxmt.h>
#include <math.h>

 

#include "Compiler\Functions.h"

//��������� ������ ������
CString FindErrorCodeLine(CString &Buffer,int nCurPos,int nOnlyCodeLine);
class CByteCode *AfxGetCurrentByteCode();
void DoError();

//������� ����������� ������
extern class CProcUnit *afxCurrentRunModule;
CString mUpper(CString );


#include "Compiler\Value.h"//������� ����� ���� ����������
#include "Compiler\ProcUnit.h"
#include "Compiler\ValueArray.h"

//����� ����������� ������
void Error(int nErr,...);
void Error(LPCTSTR s,...);

//���������� ������
void RunModule(CString Str);
