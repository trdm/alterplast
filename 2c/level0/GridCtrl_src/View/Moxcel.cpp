// Moxcel.cpp: implementation of the CMoxcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Moxcel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT GetPallete(int nIndex);


//****************************************************
//�����������
void CSectionArray::save(CFile &f)
{
	int nLength=GetSize();
	f.Write(&nLength,sizeof(int));
	for(int i=0;i<nLength;i++)
	{
		CSection &c=GetAt(i);
		write(f,c.csName);
		f.Write(&c.nRange1,sizeof(int));
		f.Write(&c.nRange2,sizeof(int));
	}
}
void CSectionArray::load(CFile &f)
{
	int nLength=0;
	f.Read(&nLength,sizeof(int));
	SetSize(nLength);
	for(int i=0;i<nLength;i++)
	{
		CSection c;
		read(f,c.csName);
		f.Read(&c.nRange1,sizeof(int));
		f.Read(&c.nRange2,sizeof(int));
		SetAt(i,c);
	}
}

//****************************************************
//CCell
//����������� �����
void write(CFile &f,CString &Str)
{
	int nLength=Str.GetLength();
	f.Write(&nLength,sizeof(int));
	f.Write(Str.GetBuffer(0),nLength);
}
void read(CFile &f,CString &Str)
{
	int nLength=0;
	f.Read(&nLength,sizeof(int));
	if(nLength>0)
	{
		f.Read(Str.GetBuffer(nLength),nLength);
		Str.ReleaseBuffer(nLength);
	}
	else
	{
		Str.Empty();
	}
}


//����������� �����
#define HEADER_LENGTH sizeof(CCell)-4*sizeof(CString)

void CCell::save(CFile &f)
{
	f.Write(this,HEADER_LENGTH);
	write(f,csText);
	write(f,csFormula);
	write(f,csFormat);
	write(f,csMaska);
}
void CCell::load(CFile &f)
{
	f.Read(this,HEADER_LENGTH);
	read(f,csText);
	read(f,csFormula);
	read(f,csFormat);
	read(f,csMaska);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************************************
CRow::CRow()
{
	nColumnCount=0;
	aCell=NULL;
	nRowHeight=0;
}
CRow::~CRow()
{
	if(aCell)
		delete []aCell;
}

//������������� �������
void GetText(char *buf,CString &Str,int &nPos)
{
	Str="";
	int nLength=0;
	if(((unsigned char)buf[nPos])==0xFF)
	{
		nPos++;
		memcpy(&nLength,&buf[nPos],2);
		nPos+=2;
	}
	else
	{
		nLength=(unsigned char)buf[nPos];
		nPos++;
	}

	if(nLength<0||nLength>0xFFFF)
	{
		AfxMessageBox("������ �������� ����� ������!");
		throw;
	}

	char *p=Str.GetBuffer(nLength+1);
	p[nLength]=0;
	memcpy(p,&buf[nPos],nLength);
	Str.ReleaseBuffer();
	nPos+=nLength;
}

int LoadNumberArray(char *buf,int* &pArray,int &nPos,int nMult=1)
{
	int nArrayCount=0;
	memcpy(&nArrayCount,&buf[nPos],2);
	if(nArrayCount>0)
	{
		if(nArrayCount>1000000)
		{
			AfxMessageBox("������ �������� ������� �������!");
			throw;
		}
		nArrayCount=nArrayCount*nMult;
		pArray=new int[nArrayCount];
		ZeroMemory(pArray,nArrayCount*sizeof(int));
		//��������� ������ ������� �������
		for(int i=0;i<nArrayCount;i++)
		{
			nPos+=2;
			memcpy(&pArray[i],&buf[nPos],2);
			pArray[i]++;
			nPos+=2;
		}
	}
	nPos=nPos+2;
	return nArrayCount;
}

//�������� ����� mxl-�������
void CRow::load(char *buf,int &nPos)
{
	int *aColumnNumber=NULL;
	nColumnCount=LoadNumberArray(buf,aColumnNumber,nPos);
	nPos+=2;
	if(nColumnCount>0)
	{
		aCell=new CCell[nColumnCount];

		//��������� ������ �������
		for(int i=0;i<nColumnCount;i++)
		{
			aCell[i].nColumn=aColumnNumber[i];

			//byte pArray[100];
			//memcpy(pArray,&buf[nPos],sizeof(pArray));

			byte mode=(unsigned char)buf[nPos+2];
			byte flag=(unsigned char)buf[nPos+3];

 			BOOL bExtend=mode&(1<<5);//����. ������
			BOOL bFormula=flag&(1<<6);//����������� ��� �������
			BOOL bText=flag&(1<<7);//�����

			aCell[i].nMode=(unsigned char)buf[nPos+27];

			//�����:
			aCell[i].nRamkaColor=(unsigned char)buf[nPos+23];//���� (0-55 �� �������)
			aCell[i].nRamkaColor=GetPallete(aCell[i].nRamkaColor);

			aCell[i].nRamkaL=(unsigned char)buf[nPos+18];//�����
			aCell[i].nRamkaU=(unsigned char)buf[nPos+19];//������
			aCell[i].nRamkaR=(unsigned char)buf[nPos+20];//������
			aCell[i].nRamkaD=(unsigned char)buf[nPos+21];//�����

			//����:
			int nBackgroundColor=(unsigned char)buf[nPos+25];//���� (0-55 �� �������)
			if(nBackgroundColor)
				aCell[i].nBackgroundColor=GetPallete(nBackgroundColor);

			aCell[i].nUzor=(unsigned char)buf[nPos+17];//���� (0-15)
			aCell[i].nUzorColor=(unsigned char)buf[nPos+22];//���� ����� (0-55 �� �������)
			aCell[i].nUzorColor=GetPallete(aCell[i].nUzorColor);

			aCell[i].nHPosition=(unsigned char)buf[nPos+15];//��������� �� ����������� (0-����,2-�����,4-�� ������,6-�����) ������� �������� ����� - ���������� ��������, �� �������� ������������� ������
			aCell[i].nHPosition=(aCell[i].nHPosition&31)/2;
			aCell[i].nVPosition=(unsigned char)buf[nPos+16];//��������� �� ��������� (0-����,8-���,0x18-�����)
			if(8==aCell[i].nVPosition)
				aCell[i].nVPosition=1;//���
			else
			if(0x18==aCell[i].nVPosition)
				aCell[i].nVPosition=2;//�����

			aCell[i].nControl=(unsigned char)buf[nPos+26];	//�������� (0-����,1-��������,2-��������...)
			aCell[i].bProtect=(unsigned char)buf[nPos+28];	//���� ������
			aCell[i].bProtect=!aCell[i].bProtect;

			aCell[i].nFontNumber=(unsigned char)buf[nPos+8];//����� ������ �� ������ �������
			int nBold=(unsigned char)buf[nPos+12];//��������
			//4-�� ����
			//7-����
			if(nBold>4)
				aCell[i].nBold=1;
			else
				aCell[i].nBold=0;
			aCell[i].nItalic=(unsigned char)buf[nPos+13];//�����������
			aCell[i].nUnderLine=(unsigned char)buf[nPos+14];//�������������
			aCell[i].nFontColor=(unsigned char)buf[nPos+24];//���� ������
			aCell[i].nFontColor=GetPallete(aCell[i].nFontColor);
			short nFontH=0;//������ ������
			memcpy(&nFontH,&buf[nPos+10],2);
			nFontH=-nFontH/4;
			aCell[i].nFontHeight=nFontH;

			nPos+=0x20-2;
			if(bText)//�����
				GetText(buf,aCell[i].csText,nPos);
			if(bFormula)//������� (�����������)
				GetText(buf,aCell[i].csFormula,nPos);

			aCell[i].nType=0;
			if(bExtend)
			{
				aCell[i].nType=buf[nPos+6];//U(��������������),N(�����),S(������),B(����������),O(��������),T(����),K(��� ��������)
				aCell[i].nLen=(unsigned char)buf[nPos+7];
				aCell[i].nPrec=(unsigned char)buf[nPos+11];

				//����� ������ �����
				int nLength=0;
				memcpy(&nLength,&buf[nPos],2);
				nPos+=2;

				int nPos2=nPos;//����������� ����������������
				nPos2+=0x10+3;
				GetText(buf,aCell[i].csFormat,nPos2);//������
				nPos2+=6;
				GetText(buf,aCell[i].csMaska,nPos2);//�����

				nPos+=nLength;
			}

		}
		delete []aColumnNumber;
	}
}


void CRow::save(CFile &sf)
{
	sf.Write(&nRowHeight	,sizeof(int));
	sf.Write(&nColumnCount	,sizeof(int));
	for(int i=0;i<nColumnCount;i++)
	{
		//aCell[i].nBackgroundColor=0xFFFFFF;
		aCell[i].save(sf);
	}
}

void CRow::load(CFile &sf)
{
	sf.Read(&nRowHeight		,sizeof(int));
	sf.Read(&nColumnCount	,sizeof(int));
	if(nColumnCount)
	{
		aCell=new CCell[nColumnCount];
		for(int i=0;i<nColumnCount;i++)
		{
			aCell[i].load(sf);
		}
	}
}


void LoadSection(CSectionArray &Section,char *buf,int &nPos)
{
	int nCount=0;
	memcpy(&nCount,&buf[nPos],2);
	nPos+=2;
	for(int i=0;i<nCount;i++)
	{
		CSection data;
		data.nRange1=0;
		data.nRange2=0;

		memcpy(&data.nRange1,&buf[nPos],4);
		data.nRange1++;
		nPos+=4;
		memcpy(&data.nRange2,&buf[nPos],4);
		data.nRange2++;
		nPos+=4;
		nPos+=4;
		GetText(buf,data.csName,nPos);
		Section.Add(data);
	}
}


//****************************************************
CMoxcel::CMoxcel()
{
	nAllColumnCount=0;
	nAllRowCount=0;

	nRowCount=0;
	aRowNumber=NULL;
	aRow=NULL;
	aWidth=NULL;
	aWidthNumber=NULL;
	nWidthCount=0;

	nFontCount=0;
	aFontNumber=NULL;
	aFont=NULL;

	nMergeCells=0;
	aMergeCells=NULL;
}
void CMoxcel::Clear()
{
	if(aRowNumber)
		delete []aRowNumber;
	if(aWidthNumber)
		delete []aWidthNumber;
	if(aWidth)
		delete []aWidth;

	if(aFontNumber)
		delete []aFontNumber;
	if(aFont)
		delete []aFont;

	if(aRow)
		delete []aRow;

	if(aMergeCells)
		delete []aMergeCells;

	nAllColumnCount=0;
	nAllRowCount=0;
	nRowCount=0;
	aRowNumber=0;
	nWidthCount=0;
	aWidthNumber=0;
	aWidth=0;
	nFontCount=0;
	aFontNumber=0;
	aFont=0;
	aRow=0;
	nMergeCells=0;
	aMergeCells=0;

	HorizSection.RemoveAll();
	VertSection.RemoveAll();
}
CMoxcel::~CMoxcel()
{
	Clear();
}
void CMoxcel::Load(char *buf)
{
	Clear();
	int nPos;
	//���������
	nPos=13;
	memcpy(&nAllColumnCount,&buf[nPos],4);//���������� ������� �����
	memcpy(&nAllRowCount,&buf[nPos+4],4);//���������� ����� ����� �����

	nPos=0x37;
	//������
	nFontCount=LoadNumberArray(buf,aFontNumber,nPos);
	nPos+=2;
	if(nFontCount>0)
	{
		aFont=new CString[nFontCount];
		for(int i=0;i<nFontCount;i++)
		{
			nPos+=28;
			int nLength=0x20;
			memcpy(aFont[i].GetBuffer(nLength),&buf[nPos],nLength);
			aFont[i].ReleaseBuffer();
			nPos+=0x40-4-28;
		}
	}

	nPos+=0x40;



	//������ ��������
	nWidthCount=LoadNumberArray(buf,aWidthNumber,nPos);
	nPos+=2;
	if(nWidthCount>0)
	{
		aWidth=new int[nWidthCount];
		for(int i=0;i<nWidthCount;i++)
		{
			nPos+=6;
			aWidth[i]=(unsigned char)buf[nPos]+256*(unsigned char)buf[nPos+1];
			aWidth[i]=aWidth[i]*90/100;
			nPos+=24;
		}
	}

	//������
	nRowCount=LoadNumberArray(buf,aRowNumber,nPos);
	nPos+=2;
	if(nRowCount>0)
	{
		aRow=new CRow[nRowCount];
		for(int i=0;i<nRowCount;i++)
		{
			//byte pArray[100];
			//memcpy(pArray,&buf[nPos],sizeof(pArray));

			aRow[i].nRowHeight=0;
			memcpy(&aRow[i].nRowHeight,&buf[nPos+4],2);
			aRow[i].nRowHeight=aRow[i].nRowHeight/3;

			nPos+=0x20-2;
			aRow[i].load(buf,nPos);
		}
	}

	//byte pArray[100];
	//buf[nPos] - ���������� ���. �������� ���� �������������, ��������, ���������, OLE-������
	//�������� ��� ��������
	int nCount=0;
	memcpy(&nCount,&buf[nPos],2);

	nPos+=2;
	int nOLEDelta=18;//������ ������ ��� ������� ��� ������� (������ �� ����, �� ��� ���)
	for(int i=0;i<nCount;i++)
	{
		//memcpy(pArray,&buf[nPos],sizeof(pArray));
		int nType=(unsigned char)buf[nPos+2];//��� �������
		//0-������� ������ (�����, �������, ���� � �������)
		//8-������� ������ (��������, ��������� 1�, ��� ������)
		BOOL bText=(unsigned char)buf[nPos+3]&0x80;//������� ����� � �������
		BOOL bFormula=(unsigned char)buf[nPos+3]&0x40;//������� ����� � ������������
		if(bText)
			bText=1;
		if(bFormula)
			bFormula=1;


		nPos+=30;
		CString csText;
		if(bText)
			GetText(buf,csText,nPos);//�����
		if(bFormula)
			GetText(buf,csText,nPos);//�����������
//		if(bText||bFormula)
//			nPos=-16;
		//memcpy(pArray,&buf[nPos],sizeof(pArray));
		int nKind=(unsigned char)buf[nPos];//��� �������
		//1-�����
		//2-�������
		//3-���� ������ (�� ��� ������)
		//4-��� ������ (� �.�. ��������� 1�)
		//5-��������

		if(nKind<=3||nType==0)
			nPos+=40;
		else
		if(nKind==4)//��� ������
		{
			int nLength=0;
			memcpy(&nLength,&buf[nPos+60+nOLEDelta],4);
			nPos+=64+nOLEDelta+nLength;
			nOLEDelta=0;
		}
		else//��������
		if(nKind==5)
		{
			//memcpy(pArray,&buf[nPos],sizeof(pArray));
			int nPictureType=(unsigned char)buf[nPos+16];//��� ��������

			int nLength=0;
			memcpy(&nLength,&buf[nPos+44],4);
			nPos+=nLength+48;
/*
			if(nLength==0)
				nPos+=48;//������ ��������
			else
			{
				int nLength=0;
				memcpy(&nLength,&buf[nPos+0x52],4);
				//memcpy(&nLength,&buf[nPos+44],4);

				int nLength2=0;
				memcpy(&nLength2,&buf[nPos+58],4);

				nPos+=nLength+nLength2+48;//0x52-34;//�� ������ ��������

			}
//*/
		}
		else//����������� ������
		{
			CString Str;
			Str.Format("���������� ����������� ������ N %d",nKind);
			AfxMessageBox(Str);
			return;

		}

	}

	//buf[nPos] - ���������� ������������ �����
	nCount=0;
	memcpy(&nCount,&buf[nPos],2);
	//��������� ������������ ������
	nMergeCells=LoadNumberArray(buf,aMergeCells,nPos,4)/4;

	//��������� ������:
	//������������
	LoadSection(VertSection,buf,nPos);
	//��������������
	LoadSection(HorizSection,buf,nPos);



	//��������� ����������� ������...
}

void CMoxcel::save(CFile &sf)
{
    sf.Write(MY_MOXEL_HEADER.GetBuffer(0), MY_MOXEL_HEADER_LENGTH);

	sf.Write(&nAllColumnCount,sizeof(int));//���������� ������� �����
	sf.Write(&nAllRowCount	,sizeof(int));//���������� ����� �����

	//������
	sf.Write(&nFontCount	,sizeof(int));//���������� �������
	if(nFontCount>0)
	{
		//...
	}


	//������ ��������
	sf.Write(&nWidthCount	,sizeof(int));
	if(nWidthCount)
	{
		sf.Write(&aWidthNumber[0],nWidthCount*sizeof(int));
		sf.Write(&aWidth[0]		,nWidthCount*sizeof(int));
	}

	//������
	sf.Write(&nRowCount		,sizeof(int));
	if(nRowCount)
	{
		//������� �������� �������� ����������� ���������� ������
		{
			int nCount=0;
			for(int i=0;i<nRowCount;i++)
			{
				nCount+=aRow[i].nColumnCount;
			}
			int n=sf.GetPosition();
			sf.SetLength(n+nCount*(sizeof(CCell)+50));
			sf.Seek(n,CFile::begin);
		}

		sf.Write(&aRowNumber[0]	,nRowCount*sizeof(int));
		for(int i=0;i<nRowCount;i++)
		{
			aRow[i].save(sf);
		}
	}
	//������������ ������:
	VertSection.save(sf);
	//��������������
	HorizSection.save(sf);

	//������������ ������
	sf.Write(&nMergeCells	,sizeof(int));
	if(nMergeCells)
	{
		sf.Write(&aMergeCells[0],4*nMergeCells*sizeof(int));
	}

	int nPos=sf.GetPosition();
	sf.SetLength(nPos);//�������� ������ �����
}

BOOL CMoxcel::Save(CString &csStr)
{
    CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);

	save(sf);
	DWORD dwLen = (DWORD) sf.GetLength();
	sf.SeekToBegin();

	char *p=csStr.GetBuffer(dwLen);
	if(!p)
		return 0;
	sf.Read(p,dwLen);
	csStr.ReleaseBuffer(dwLen);
	return 1;
}

void CMoxcel::load(CFile &sf)
{
	CString Str;
    sf.Read(Str.GetBuffer(MY_MOXEL_HEADER_LENGTH), MY_MOXEL_HEADER_LENGTH);
	Str.ReleaseBuffer(MY_MOXEL_HEADER_LENGTH);
	if(Str.Left(MY_MOXEL_HEADER_LENGTH)!=MY_MOXEL_HEADER)
		return;

	sf.Read(&nAllColumnCount,sizeof(int));//���������� ������� �����
	sf.Read(&nAllRowCount	,sizeof(int));//���������� ����� �����

	//������
	sf.Read(&nFontCount	,sizeof(int));//���������� �������
	if(nFontCount>0)
	{
		//...
	}


	//������ ��������
	sf.Read(&nWidthCount	,sizeof(int));
	if(nWidthCount)
	{
		aWidthNumber=new int[nWidthCount];
		aWidth		=new int[nWidthCount];

		sf.Read(&aWidthNumber[0],nWidthCount*sizeof(int));
		sf.Read(&aWidth[0]		,nWidthCount*sizeof(int));
	}

	//������
	sf.Read(&nRowCount		,sizeof(int));
	if(nRowCount)
	{
		aRowNumber	=new int[nRowCount];
		aRow		=new CRow[nRowCount];

		sf.Read(&aRowNumber[0]	,nRowCount*sizeof(int));
		for(int i=0;i<nRowCount;i++)
		{
			aRow[i].load(sf);
		}
	}

	//������������ ������:
	VertSection.load(sf);
	//��������������
	HorizSection.load(sf);

	//������������ ������
	sf.Read(&nMergeCells	,sizeof(int));
	if(nMergeCells)
	{
		aMergeCells	=new int[4*nMergeCells];

		sf.Read(&aMergeCells[0]	,4*nMergeCells*sizeof(int));
	}
}


BOOL CMoxcel::Load(CString csBuffer)
{
	int nLength=csBuffer.GetLength();
	if(nLength>38)
	if(csBuffer.Left(MY_MOXEL_HEADER_LENGTH)==MY_MOXEL_HEADER)
	{
		CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
		sf.SetLength(nLength);
		sf.Write(csBuffer.GetBuffer(0),nLength);

		sf.SeekToBegin();
		load(sf);
		return 1;
	}

	if(nLength>0x92)
	if(csBuffer.Left(MOXEL_HEADER_LENGTH)==MOXEL_HEADER)
	{
		Load(csBuffer.GetBuffer(0));
		return 1;
	}
	return 0;
}










//��� ������� �� ������ � 1� ��������
//������� ������ 1�
UINT GetPallete(int nIndex)
{

	UINT a1CPallete[]=
	{
//		0xRRGGBB,
		0x000000,
		0xFFFFFF,
		0xFF0000,
		0x00FF00,
		0x0000FF,
		0xFFFF00,
		0xFF00FF,
		0x00FFFF,

		0x800000,
		0x008000,
		0x808000,
		0x000080,
		0x800080,
		0x008080,
		0x808080,
		0xC0C0C0,

		0x8080FF,
		0x802060,
		0xFFFFC0,
		0xA0E0E0,
		0x600080,
		0xFF8080,
		0x0080C0,
		0xC0C0FF,

		0x00CFFF,
		0x69FFFF,
		0xE0FFED,
		0xDD9CB3,
		0xB38FEE,
		0x2A6FF9,
		0x3FB8CD,
		0x488436,

		0x958C41,
		0x8E5E42,
		0xA0627A,
		0x624FAC,
		0x1D2FBE,
		0x286676,
		0x004500,
		0x453E01,

		0x6A2813,
		0x85396A,
		0x4A3285,
		0xC0DCC0,
		0xA6CAF0,
		0x800000,
		0x008000,
		0x000080,

		0x808000,
		0x800080,
		0x008080,
		0x808080,
		0xFFFBF0,
		0xA0A0A4,
		0x313900,
		0xD98534
	};

	if(nIndex<0||nIndex>=sizeof(a1CPallete))
		return 0;

	UINT nRes=a1CPallete[nIndex];
	return RGB(nRes>>16&0xFF,nRes>>8&0xFF,nRes&0xFF);
}


int GetRamka(int nIndex)
{
	nIndex=nIndex&15;
	int aRamka[]=
	{
		10+PS_NULL,//0
		10+PS_DOT,//1-�����
		10+PS_SOLID,//2-������ �����
		20+PS_SOLID,//3-������� �����
		20+PS_SOLID,//4-����� ������� �����
		10+PS_SOLID,//5-������� �����
		10+PS_DASH,//6-�������1
		10+PS_DASHDOT,//7-�������2
		10+PS_SOLID,//8-�����2
		20+PS_DASH,//9-������� �������
	};

	if(nIndex<0||nIndex>=sizeof(aRamka))
		return PS_NULL;

//#define PS_SOLID            0
//#define PS_DASH             1       /* -------  */
//#define PS_DOT              2       /* .......  */
//#define PS_DASHDOT          3       /* _._._._  */
//#define PS_DASHDOTDOT       4       /* _.._.._  */
//#define PS_NULL             5


	return aRamka[nIndex];
}


int GetUzor(int nIndex)
{

	nIndex=nIndex&15;
	int aUzor[]=
	{
		33,
		0,
		1,
		2,
		22,
		17,
		7,
		6,
		14,
		15,
		10,
		11,
		31,
		30,
		21,
		20
	};

	if(nIndex<=0||nIndex>=sizeof(aUzor))
		return 33;
	return aUzor[nIndex];
}


