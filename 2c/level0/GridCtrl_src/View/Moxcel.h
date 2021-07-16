// Moxcel.h: interface for the CMoxcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOXCEL_H__9B6E7324_0F0D_479C_9784_F89F58D96C8C__INCLUDED_)
#define AFX_MOXCEL_H__9B6E7324_0F0D_479C_9784_F89F58D96C8C__INCLUDED_
#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MOXEL_HEADER CString("MOXCEL")
#define MOXEL_HEADER_LENGTH MOXEL_HEADER.GetLength()

#define MY_MOXEL_HEADER CString("MYMXL2")
#define MY_MOXEL_HEADER_LENGTH MY_MOXEL_HEADER.GetLength()

//Серилизация строк
void write(CFile &f,CString &Str);
void read(CFile &f,CString &Str);

class CCell
{
public:
	int nColumn;		//номер колонок
	int nRow;

	byte nMode;//режим (0-текст,1-выражение,2-шаблон...)
	byte nType;		//тип данных ячейки U(неопределенный),N(число),S(строка),B(справчоник),O(документ),T(счет),K(вид субконто)
	byte nLen;		//длина
	byte nPrec;		//точность
	bool bProtect;	//флаг защиты ячейки
	byte nControl;	//Контроль (0-Авто,1-обрезать,2-забивать...)
	byte nHPosition;	//Положение по горизонтали (0-лево,1-право,2-по ширине,3-центр) старшая половина слова - количество столбцов, по которому выравнивается ячейка
	byte nVPosition;	//Положение по вертикали (0-верх,1-низ,2-центр)
	//Рамка:
	COLORREF nRamkaColor;//цвет рамки
	byte nRamkaL;//слева
	byte nRamkaU;//сверху
	byte nRamkaR;//справа
	byte nRamkaD;//снизу
	//значения - стиль:
	//1-точки1
	//2-тонкая линия
	//3-толстая линия
	//4-очень толстая линия
	//5-двойная линия
	//6-пунктир1
	//7-пунктир2
	//8-точки2
	//9-толстый пунктир

	COLORREF nBackgroundColor;//цвет фона (0-55 из палитры)
	byte nUzor;//Узор (0-15)
	COLORREF nUzorColor;//цвет узора (0-55 из палитры)


	__int16 nFontNumber;//номер шрифта из списка
	bool nBold;//жирность
	bool nItalic;//наклонность
	bool nUnderLine;//подчеркивание
	COLORREF nFontColor;//цвет шрифта
	int nFontHeight;//размер шрифта

	__int16 nCoveredRow;
	__int16 nCoveredCol;

	CString csText;		//текст ячейки
	CString csFormula;	//расшифровка

	CString csFormat;
	CString csMaska;

	CCell()
	{
		nMode=0;
		nColumn=0;
		nRow=0;
		nType=0;
		nLen=0;
		nPrec=0;
		bProtect=1;
		nControl=0;
		nHPosition=0;
		nVPosition=0;

		nRamkaColor=0;
		nRamkaL=0;
		nRamkaU=0;
		nRamkaR=0;
		nRamkaD=0;
		nBackgroundColor=0xFFFFFF;
		nUzor=0;
		nUzorColor=0;

		nFontNumber=0;
		nBold=0;
		nItalic=0;
		nUnderLine=0;
		nFontColor=0;
		nFontHeight=0;
		nCoveredRow=0;
		nCoveredCol=0;
	};

	//серилизация
	void save(CFile &f);
	void load(CFile &f);
};

class CMoxcel;

class CRow
{
public:
	CRow();
	virtual ~CRow();
public:
	int nColumnCount;//число заполненных колонок
	CCell *aCell;
	int nRowHeight;//высота строки
public:
	//загрузка 1С формата
	void load(char *buf,int &nPos);


	//серилизация
	void save(CFile &sf);
	void load(CFile &sf);
};

class CSection
{
public:
	CString csName;
	int nRange1;
	int nRange2;
};

class CSectionArray:public CArray <CSection,CSection&>
{
public:
	void save(CFile &f);
	void load(CFile &f);
};

//typedef CArray <CSection,CSection&> CSectionArray;

class CMoxcel  
{
public:
	CMoxcel();
	virtual ~CMoxcel();
	void Clear();
public:
	int nAllColumnCount;//всего колонок в таблице
	int nAllRowCount;//всего строк в таблице

	int nRowCount;//число заполненных строк
	int *aRowNumber;//номера строк
	CRow *aRow;//строки

	int nWidthCount;//размер массива задания ширины колонок
	int *aWidthNumber;//номера колонок у которых задается ширина
	int *aWidth;//ширина колонок

	int nFontCount;//количество шрифтов
	int *aFontNumber;//номера шрифтов
	CString *aFont;//шрифты

	int nMergeCells;//количество объединенных ячеек
	int *aMergeCells;//массив объединенных ячеек

	//секции:
	CSectionArray HorizSection;
	CSectionArray VertSection;

public:
	//загрузка 1С формата
	BOOL Load(CString csBuffer);
	BOOL Save(CString &csBuffer);


	//серилизация
	void save(CFile &sf);
	void load(CFile &sf);

protected:
	void Load(char *buf);
};


//доп функции по работе с 1С форматом
//UINT GetPallete(int nIndex);
int GetRamka(int nIndex);
int GetUzor(int nIndex);

#endif // !defined(AFX_MOXCEL_H__9B6E7324_0F0D_479C_9784_F89F58D96C8C__INCLUDED_)
