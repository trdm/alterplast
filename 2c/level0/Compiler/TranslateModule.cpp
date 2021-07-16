// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// TranslateModule.cpp: implementation of the CTranslateModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TranslateModule.h"
#include "Definition.h"

extern BOOL afxMessageFromConfig;
CString LoadFromFile(CString csFileName);
CString GetModuleText(CString csPath,BOOL &bError);
extern CMapStringToPtr	aHelpDescription;//�������� �������� ���� � ��������� �������
extern CMapStringToPtr	aHashKeywordList;
extern CDefList glDefList;//���������� ������ �����������

//#ifdef MYDEBUG
//	#define MYDEBUGOUT
//#endif

//#include "\\Gena\YLINTERPRETE\InstructionList.h"
//#include "InstructionList.h"
 
//////////////////////////////////////////////////////////////////////
// Global array
//////////////////////////////////////////////////////////////////////
struct aKeyWordsDef aKeyWords[] = 
{
	{"����","if"},
	{"�����","then"},
	{"�����","else"},
	{"���������","elseif"},
	{"���������","endif"},
	{"���","for"},
	{"��","to"},
	{"����","do"},
	{"����������","enddo"},
	{"����","while"},
	{"�������","goto"},
	{"��","not"},
	{"�","and"},
	{"���","or"},
	{"���������","procedure"},
	{"��������������","endprocedure"},
	{"�������","function"},
	{"������������","endfunction"},
	{"�����","forward"},
	{"�������","export"},
	{"����","val"},
	{"�������","return"},
	{"�������","try"},
	{"����������","except"},
	{"������������","endtry"},
	{"����������","continue"},
	{"��������","break"},
	{"�����������������","raise"},
	{"�����","var"},
	//���������� ����� 1�:
	//	{"�����","class"},
	//	{"�����������","endclass"},
	{"List","List"},
	{"����������","virtual"},
//	{"��������","static"},
	{"",""},
	//{"#�������������","#import"},
	{"#���������","#include","#��������� \"���\" - �������� ������ ����� ��� ������������� ������"},
	{"#����������������","#LoadFromFile","#���������������� \"���\" - �������� ������ ����� ��� ������������� ������"},
	{"#������","#define","#������ ������������� <���������> - ���������� �������������� ��������� � �������� �������� ��������������"},
	{"#�������","#undef","#������� ������������� - ������ ���������� ��������������"},
	{"#����","#ifdef","#���� ������������� - �������������� ������ ��� ������� ����� ������������ ��������������"},
	{"#������","#ifndef","#������ ������������� - �������������� ������ ��� ����������� ����� ������������ ��������������"},
	{"#�����","#else"},
	{"#���������","#endif","#��������� - ����� ��������� ��������������"},

	//��������� ����������
/*	{"�����","Number"},
	{"������","String"},
	{"����","Date"},
	{"������","Array"},*/

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTranslateModule::CTranslateModule()
{
	//���������� ������ �������� ����
	if(HashKeyWords.GetCount()==0)//������ ���� ���
		LoadKeyWords();
	Clear();

	aDefList=0;
	bAutoDeleteDefList=0;
	nModePreparing=LEXEM_ADD;
}

CTranslateModule::~CTranslateModule()
{
	if(bAutoDeleteDefList&&aDefList)
		delete aDefList;

}

//���������� ������ �������� ����
void CTranslateModule::LoadKeyWords()
{
	HashKeyWords.RemoveAll();
	for(int i=0;i<LastKeyWord;i++)
	{
		CString csRus=aKeyWords[i].Rus;
		csRus.MakeUpper();
		CString csEng=aKeyWords[i].Eng;
		csEng.MakeUpper();

		HashKeyWords[csRus]=(void*)(i+1);
		HashKeyWords[csEng]=(void*)(i+1);

		//��������� � ������ ��� ��������������� �����������
		aHashKeywordList[mUpper(csRus)]=(void*)1;
		aHashKeywordList[mUpper(csEng)]=(void*)1;

		aHelpDescription[csRus]=aKeyWords[i].sShortDescription;
		aHelpDescription[csEng]=aKeyWords[i].sShortDescription;
	}
}

//////////////////////////////////////////////////////////////////////
// Translating
//////////////////////////////////////////////////////////////////////


//____________________________________________________________________
//Clear
//����������:
//����������� ���������� ��� ������ ����������
//������������ ��������:
//���
void CTranslateModule::Clear()
{
	Buffer="";
	BUFFER="";
	nSizeText=0;
	nPos=0;
	nCurNumberLine=0;
}


//____________________________________________________________________
//LoadFile
//����������:
//��������� ����� �������� ������� �� ����� + ����������� ���������� ��� ����������
//������������ ��������:
//���
void CTranslateModule::LoadFile(char *sFileName)
{
//	Clear();
//	Load(LoadFromFileBin(sFileName,nSizeText));
//	BUFFER=Buffer;
//	BUFFER.MakeUpper();
}

//____________________________________________________________________
//SaveFile
//����������:
//��������� ������ ����-����� � ����
//������������ ��������:
//���
void CTranslateModule::SaveToFile(char *sFileName)
{
	HANDLE hFile=CreateFile(sFileName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,NULL,NULL);
	if(!hFile)
		Error(ERROR_FILE_OPEN,sFileName);
	else
	{
		//��������� �����
		CString csStr="Lexem";
//		DWORD dwWrite;
//		WriteFile(hFile,csStr.GetBuffer(0),csStr.GetLength(),&dwWrite,NULL);
/*
		for(int i=0;i<cByteCode.GetSize();i++)
		{
			CLexem *b=&cByteCode.Get(i);
			if(!b->SaveToFile(hFile))
			{
				Error(ERROR_FILE_WRITE,sFileName);
				break;
			}
		}
*/
		CloseHandle (hFile);
	}
}


//____________________________________________________________________
//Load
//����������:
//��������� ����� �������� ������� + ����������� ���������� ��� ����������
//������������ ��������:
//���
void CTranslateModule::Load(CString s)
{
	Clear();
	Buffer=s;
	Buffer.TrimRight();
	nSizeText=Buffer.GetLength();
	if(nSizeText==0)
		return;
	BUFFER=Buffer;
	BUFFER.MakeUpper();
}



//____________________________________________________________________
//SetError
//����������:
//��������� ������ ���������� � ������� ����������
//������������ ��������:
//����� �� ���������� ����������!
void CTranslateModule::SetError(int nErr,int nCurPos,CString s)
{
	CString strError;
	if(afxMessageFromConfig)
		strError=FindErrorCodeLine(Buffer,nCurPos,0);
	else
		strError.Format("%s %s",csModuleName,FindErrorCodeLine(Buffer,nCurPos,0));

	Error(nErr,strError.GetBuffer(0),s.GetBuffer(0));

}



//____________________________________________________________________
//SkipSpaces
//����������:
//���������� ��� ���������� ������� �� ������ �����
//���� ����������� �� ������ ����� �������������� � ����� ������
//������������ ��������:
//���
void CTranslateModule::SkipSpaces(void)
{
	for(int i=nPos;i<nSizeText;i++)
	{
		char c=BUFFER[i];
		if(c!=' '&&c!='\t'&&c!='\n'&&c!='\r')
		{
			if(c=='/')//����� ��� �����������
			{
				if(i+1<nSizeText)
					if(BUFFER[i+1]=='/')//���������� �����������
					{
						for(int j=i;j<nSizeText;j++)
						{
							nPos=j;
							if(BUFFER[j]=='\n'||BUFFER[j]==13)
							{
								//������������ ��������� ������
								SkipSpaces();
								return;
							}
						}
						i=nPos+1;
					}
			}
			nPos=i;
			break;
		}
		else
		{
			if(c=='\n')
				nCurNumberLine++;
		}
	}
	if(i==nSizeText)
		nPos=nSizeText;
}



//____________________________________________________________________
//IsByte
//����������:
//��������� �������� �� ��������� ���� (��� ����� ��������) ������
//��������� �����
//������������ ��������:
//true,false
bool CTranslateModule::IsByte(char c)
{
	SkipSpaces();
	if(nPos>=nSizeText)
		return false;
	if(BUFFER[nPos]==c)
		return true;
	return false;
}


//____________________________________________________________________
//GetByte
//����������:
//�������� �� ������� ���� (��� ����� ��������)
//���� ������ ����� ���, �� ��������� ����������
//������������ ��������:
//���� �� ������
char CTranslateModule::GetByte()
{
	SkipSpaces();
	if(nPos<nSizeText)
	{
		char c=Buffer[nPos];
		nPos++;
		return c;
	}
	SetError(ERROR_TRANSLATE_BYTE,nPos);
	return 0;
}


//____________________________________________________________________
//IsWord
//����������:
//��������� (�� ������� ������� �������� ������)
//�������� �� ��������� ����� ���� ������ (��������� ������� � ��.)
//������������ ��������:
//true,false
bool CTranslateModule::IsWord(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
	if(
		(BUFFER[nPos]=='_')||
		(BUFFER[nPos]>='A'&&BUFFER[nPos]<='Z')||
		(BUFFER[nPos]>='�'&&BUFFER[nPos]<='�')||
		(BUFFER[nPos]=='#')
		)
		return true;
	return false;
}


//____________________________________________________________________
//GetWord
//����������:
//������� �� ������ ��������� �����
//���� ����� ��� (�.�. ��������� ����� ���� �� �������� ������), �� ��������� ����������
//��������: bGetPoint
//true - ��������� ����� ��� ��������� ����� ����� (��� ��������� ����� ���������)
//������������ ��������:
//����� �� ������
CString CTranslateModule::GetWord(bool bOrigin,bool bGetPoint,CString *psOrig)
{
	SkipSpaces();
	if(nPos>=nSizeText)
		SetError(ERROR_TRANSLATE_WORD,nPos);
	int nNext=nPos;
	for(int i=nPos;i<nSizeText;i++)
	{
		if(
			(BUFFER[i]=='_')||
			(BUFFER[i]>='A'&&BUFFER[i]<='Z')||
			(BUFFER[i]>='�'&&BUFFER[i]<='�')||
			(BUFFER[i]>='0'&&BUFFER[i]<='9')||
			(BUFFER[i]=='#'&&i==nPos)||//���� ������ ������ # - ��� ��������� �����
			(BUFFER[i]=='.'&&bGetPoint)
			)
		{
			if(BUFFER[i]=='.'&&bGetPoint)
				bGetPoint=false;////����� ������ ����������� ������ ���� ���
			nNext=i+1;
		}
		else
			break;
	}
	int nFirst=nPos;
	nPos=nNext;
	if(nFirst==nNext)
		SetError(ERROR_TRANSLATE_WORD,nFirst);
	if(bOrigin)
		return Buffer.Mid(nFirst,nNext-nFirst);
	else
	{
		if(psOrig)
			*psOrig=Buffer.Mid(nFirst,nNext-nFirst);
		return BUFFER.Mid(nFirst,nNext-nFirst);
	}
}



//____________________________________________________________________
//GetStrToEndLine
//����������:
//�������� ��� ������ �� ����� (������� 13 ��� ����� ���� ���������)
//������
CString CTranslateModule::GetStrToEndLine(void)
{
	int nStart=nPos;
	for(int i=nPos;i<nSizeText;i++)
	if(Buffer[i]=='\r'||Buffer[i]=='\n')
	{
		i++;
		//nCurNumberLine++;
		break;
	}
	nPos=i;
	int nEnd=nPos;
	return Buffer.Mid(nStart,nEnd-nStart);
}

//____________________________________________________________________
//IsNumber
//����������:
//��������� (�� ������� ������� �������� ������)
//�������� �� ��������� ����� ���� ������-���������� (��������� ������� � ��.)
//������������ ��������:
//true,false
bool CTranslateModule::IsNumber(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
	if(
		(BUFFER[nPos]>='0'&&BUFFER[nPos]<='9')
//		||(BUFFER[nPos]=='.')
		)
		return true;
	return false;
}

//____________________________________________________________________
//GetNumber
//����������:
//�������� �� ������� �����
//���� �������� ����� ���, �� ��������� ����������
//������������ ��������:
//����� �� ������
CString CTranslateModule::GetNumber(void)
{
	if(!IsNumber())
		SetError(ERROR_TRANSLATE_NUMBER,nPos);
	return GetWord(false,true);
}

//____________________________________________________________________
//IsString
//����������:
//��������� �������� �� ��������� ����� �������� (��� ����� ��������) �������-����������, ����������� � �������
//������������ ��������:
//true,false
bool CTranslateModule::IsString(void)
{
	return IsByte('\"');
/*	if(IsByte('\"'))
		return 1;

	//������� - ����� ����� ���� �� ����� - ������ ��� �� ������� ���� - � ����� ��������� ���������
	nPos++;
	bool bRes=IsNumber();
	nPos--;
	return bRes;
*/
}

//____________________________________________________________________
//GetString
//����������:
//�������� �� ������� ������, ����������� � �������
//���� ����� ������ ���, �� ��������� ����������
//������������ ��������:
//������ �� ������
CString CTranslateModule::GetString(void)
{
	CString csRes;
	if(!IsString())
		SetError(ERROR_TRANSLATE_STRING,nPos);
	SkipSpaces();
	int nStart=0;
	for(int i=nPos+1;i<nSizeText-1;i++)
	{
		char c=Buffer[i];
		if(c=='\"')
		{
			i++;
			c=Buffer[i];
			if(c!='\"')
				break;
		}
		else//��������� �������������� ������
		if(c=='\n')
			nStart=csRes.GetLength();
		else
		if(c=='|'&&nStart>0)
		{//�������� �������� �����
			csRes=csRes.Left(nStart);
			nStart=0;
			continue;
		}
		else
		if(c=='/'&&nStart>0)
		{
			if(Buffer[i+1]=='/')//��� ����������� - ����������
			{
				i=Buffer.Find('\n',i+1);
				if(i<0)
				{
					i=nSizeText;
					break;
				}
				continue;
			}
		}
		else
		if(c!='\t'&&c!=' ')
			nStart=0;

		csRes+=CString(c);
	}
	nPos=i;
	return csRes;
}

//____________________________________________________________________
//IsDate
//����������:
//��������� �������� �� ��������� ����� �������� (��� ����� ��������) �����-����������, ����������� � ���������
//������������ ��������:
//true,false
bool CTranslateModule::IsDate(void)
{
	return IsByte('\'');
}

//____________________________________________________________________
//GetDate
//����������:
//�������� �� ������� ����, ����������� � ���������
//���� ����� ���� ���, �� ��������� ����������
//������������ ��������:
//���� �� ������, ����������� � �������
CString CTranslateModule::GetDate(void)
{
	if(!IsDate())
		SetError(ERROR_TRANSLATE_DATE,nPos);

	int nCount=0;
	SkipSpaces();
	if(nPos>=nSizeText)
		SetError(ERROR_TRANSLATE_WORD,nPos);
	int nNext=nPos;
	for(int i=nPos;i<nSizeText;i++)
	{
	if(
		(nCount<2)
		)
	{
		if(BUFFER[i]=='\'')
			nCount++;
		nNext=i+1;
	}
		else
			break;
	}
	int nFirst=nPos;
	nPos=nNext;
	if(nFirst==nNext)
		SetError(ERROR_TRANSLATE_DATE,nFirst);
	CString sRes=Buffer.Mid(nFirst+1,nNext-nFirst-2);
	return sRes; 
}


//____________________________________________________________________
//IsEnd
//����������:
//��������� ������� ��������� �������������� (�.�. ����� �� ����� ������)
//������������ ��������:
//true,false
bool CTranslateModule::IsEnd(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
		return false;
	else
		return true;
}



//____________________________________________________________________
//IsKeyWord
//����������:
//���������� �������� �� �������� ����� ��������� ����������
//������������ ��������,����:
//-1: ���
//������ ��� ����� 0: ����� � ������ ��������� ����
int CTranslateModule::IsKeyWord(CString s)
{
	return ((int)HashKeyWords[s])-1;
}

/*
//____________________________________________________________________
//TranslateModule
//����������:
//���������� ��������� ������ � ���� ���
//������������ ��������:
//true,false
bool CTranslateModule::PrepareCode()
{
	PrepareLexem();
	return true;
}
*/
//____________________________________________________________________
//PrepareLexem
//������1 - �������� ������ ��� ������������ �������� ������� ��� �������������
bool CTranslateModule::PrepareLexem()
{
	if(!aDefList)
	{
		aDefList=new CDefList();
		aDefList->SetParent(&glDefList);
		bAutoDeleteDefList=1;//�������, ��� ������ � ������������� ������� �� (� �� ������� � �������� ���������� �����������)
	}

	CString s;
	LexemList.RemoveAll();

	while(!IsEnd())
	{
		CLexem bytecode;
		bytecode.nStringNumber=nPos;//���� � ���������� ���������� ������, �� ������ ��� ������ ����� ������ ������������
		bytecode.nNumberLine=nCurNumberLine;
		bytecode.csModuleName=csModuleName;
		if(IsWord())
		{
			CString csOrig;
			s=GetWord(0,0,&csOrig);

			//��������� ����������� ������������ (#define)
			if(aDefList->HasDef(s))
			{
				CLexemList *pDef=aDefList->GetDef(s);
				for(int i=0;i<pDef->GetSize();i++)
				{
					CLexem lex=pDef->GetAt(i);
					lex.nStringNumber=nPos;
					lex.nNumberLine=nCurNumberLine;//��� ����� ��������
					lex.csModuleName=csModuleName;
					LexemList.Add(lex);
				}

				continue;
			}

			int n=IsKeyWord(s);
			bytecode.vData=String(csOrig);
			if(n>=0)
			{
				bytecode.nType=KEYWORD;
				bytecode.nData=n;
			}
			else
			{
				bytecode.nType=IDENTIFIER;
			}
		}
		else
		if(IsNumber()||IsString()||IsDate())
		{
			bytecode.nType=CONSTANT;
			if(IsNumber())
			{
				bytecode.vData.SetNumber(GetNumber());

				int n=LexemList.GetSize()-1;
				if(n>=0)
				if(LexemList[n].nType==DELIMETER&&(LexemList[n].nData=='-'||LexemList[n].nData=='+'))
				{
					n--;
					if(n>=0)
					if(LexemList[n].nType==DELIMETER&&(LexemList[n].nData=='['||LexemList[n].nData=='('||LexemList[n].nData==','||LexemList[n].nData=='<'||LexemList[n].nData=='>'||LexemList[n].nData=='='))
					{
						n++;
						if(LexemList[n].nData=='-')
							bytecode.vData.fData=-bytecode.vData.fData;
						LexemList[n]=bytecode;
						continue;
					}
				}
			}
			else
			if(IsString())
				bytecode.vData.SetString(GetString());
			else
			if(IsDate())
				bytecode.vData.SetDate(GetDate());

			LexemList.Add(bytecode);
			continue;
		}
		else
		if(IsByte('~'))
		{
			s="";
			GetByte();//���������� ����������� � �������. ������ ����� (��� ������)
			continue;
		}
		else
		{
			s="";
			bytecode.nType=DELIMETER;
			bytecode.nData=GetByte();
			if(bytecode.nData<=13)
				continue;
		}
		bytecode.sData=s;


		if(bytecode.nType==KEYWORD)
		{
			if(bytecode.nData==KEY_INCLUDE||bytecode.nData==KEY_INCLUDE2)//����������� ������������� ������ (�����)
			{
				if(!IsString())
					SetError(ERROR_TRANSLATE_STRING,nPos);

				BOOL bError;
				CString csPath=GetString();
				csPath.Replace("/","\\");
				CString csCompile=GetModuleText(csPath,bError);

				if(bError)
				{
					SetError(ERROR_MODULE_NOT_FOUND,nPos);
				}

				if(!csCompile.IsEmpty())
				{
					CTranslateModule translate;
					translate.aDefList=aDefList;//������������� ������ �����������
					translate.csModuleName=csPath;
					translate.Load(csCompile);
					translate.PrepareLexem();
					for(int i=0;i<translate.LexemList.GetSize()-1;i++)//��� ����� ENDPROGRAM
						LexemList.Add(translate.LexemList[i]);
				}
				continue;
			}
			else
			if(bytecode.nData==KEY_DEFINE&&nModePreparing!=LEXEM_ADDDEF)//������� ������������� ��������������
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);

				CString csName=GetWord();
//				if(aDefList->HasDef(csName))
//					SetError(ERROR_IDENTIFIER_DUPLICATE,nPos);

				//��������� �������������� ��������� � ������ �����������
				if(LEXEM_ADD==nModePreparing)
					PrepareFromCurrent(LEXEM_ADDDEF,csName);
				else
					PrepareFromCurrent(LEXEM_IGNORE,csName);
				continue; 
			}
			else
			if(bytecode.nData==KEY_UNDEF)//�������� ��������������
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);

				CString csName=GetWord();
				aDefList->RemoveDef(csName);
				continue;
			}
			else
			if(bytecode.nData==KEY_IFDEF||bytecode.nData==KEY_IFNDEF)//�������� ��������������
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);
				CString csName=GetWord();
				BOOL bHasDef=aDefList->HasDef(csName);
				if(bytecode.nData==KEY_IFNDEF)
					bHasDef=!bHasDef;

				
				//����������� ���� ���� ���� �� ���������� #else ��� #endif
				int nMode=0;

				if(bHasDef)
					nMode=LEXEM_ADD;//��������� �������������� ��������� � ������ ������
				else
					nMode=LEXEM_IGNORE;//����� ����������
				PrepareFromCurrent(nMode);

				if(!IsWord())
					SetError(ERROR_USE_ENDDEF,nPos);

				CString csWord=GetWord();
				if(IsKeyWord(csWord)==KEY_ELSEDEF)//����� #else
				{
					//��� ��� �����������
					if(!bHasDef)
						nMode=LEXEM_ADD;//��������� �������������� ��������� � ������ ������
					else
						nMode=LEXEM_IGNORE;//����� ����������
					PrepareFromCurrent(nMode);

					if(!IsWord())
						SetError(ERROR_USE_ENDDEF,nPos);
					csWord=GetWord();
				}

				//������� #endif
				if(IsKeyWord(csWord)!=KEY_ENDIFDEF)
					SetError(ERROR_USE_ENDDEF,nPos);

				continue;
			}
			else
			if(bytecode.nData==KEY_ENDIFDEF)//����� ��������� ��������������
			{
				nPos=bytecode.nStringNumber;//����� �� ��������� ���������� ��������
				break;
			}
			else
			if(bytecode.nData==KEY_ELSEDEF)//"�����" ��������� ��������������
			{
				//������������ �� ������ ��������� ���������
				nPos=bytecode.nStringNumber;//����� �� ��������� ���������� ��������
				break;
			}
		}

		LexemList.Add(bytecode);
	}
//	if(LexemList.GetSize()==0)
//		return false;

	CLexem bytecode;
	bytecode.nType=ENDPROGRAM;
	bytecode.nData=0;
	bytecode.nStringNumber=nPos;
	LexemList.Add(bytecode);

#ifdef MYDEBUGOUT
	ToDebug();
#endif
	return true;
}

void CTranslateModule::ToDebug()
{
#ifdef _DEBUG
	int i;
	OutToDebug("\nLexemList: %d\n",LexemList.GetSize());
	for(i=0;i<LexemList.GetSize();i++)
	{
		OutToDebug("%d.\t%d",i,LexemList[i].nType);
		OutToDebug("\t%c(%d)",LexemList[i].nData,LexemList[i].nData);
		OutToDebug("\t%s",LexemList[i].vData.GetString());
		OutToDebug("\t%s",LexemList[i].sData);
		OutToDebug("\t%d",LexemList[i].nStringNumber);
		OutToDebug("\n");
	}
#endif
}


CDefList::~CDefList()
{
	CLexemList *pArray;
	POSITION pos;
	CString csKey;
	for( pos = DefList.GetStartPosition(); pos != NULL; )
	{
		DefList.GetNextAssoc( pos, csKey, (void*&)pArray );
		if(pArray)
			pArray->RemoveAll();
	}
}
void CDefList::RemoveDef(CString &csName)
{
	DefList.RemoveKey(mUpper(csName));
}
BOOL CDefList::HasDef(CString &csName)
{
	CLexemList *p;
	BOOL bRes=DefList.Lookup(mUpper(csName),(void*&)p);
	if(bRes)
		return 1;

	static int nLevel=0;
	nLevel++;
	if(nLevel>MAX_OBJECTS_LEVEL)
		Error("����������� ����� ������� (#3)");

	//���� � ���������
	bRes=0;
	if(pParent)
	{
		bRes=pParent->HasDef(csName);
	}

	nLevel--;
	return bRes;
}
CLexemList *CDefList::GetDef(CString &csName)
{
	CLexemList *p=(CLexemList *)DefList[mUpper(csName)];
	if(!p)
	{
		//���� � ���������
		if(pParent)
			if(pParent->HasDef(csName))
				return pParent->GetDef(csName);

		p=new CLexemList();
		DefList[mUpper(csName)]=p;
	}
	return p;
}
void CDefList::SetDef(CString &csName,CLexemList *pDef)
{
	CLexemList *pList=GetDef(csName);
	pList->RemoveAll();
	if(pDef)
	for(int i=0;i<pDef->GetSize();i++)
		pList->Add(pDef->GetAt(i));
}
void CDefList::SetDef(CString &csName,CString csValue)
{
	CLexemList List;
	if(csValue.GetLength()>0)
	{
		CLexem Lex;
		Lex.nType=CONSTANT;
		if(csValue[0]=='-'||csValue[0]=='+'||(csValue[0]>='0'&&csValue[0]<='9'))//�����
			Lex.vData.SetNumber(csValue);
		else
			Lex.vData.SetString(csValue);
		List.Add(Lex);
		SetDef(mUpper(csName),&List);
	}
	else
	{
		SetDef(mUpper(csName),NULL);
	}
}

void CDefList::SetParent(CDefList *p)
{
	pParent=p;
}

//�������� ������ ������� � ������� �������
void CTranslateModule::PrepareFromCurrent(int nMode,CString csName)
{

	CTranslateModule translate;
	translate.aDefList=aDefList;
	translate.nModePreparing=nMode;
	translate.csModuleName=csModuleName;
	translate.Load(Buffer);
	
	//��������� ����� ������
	translate.nCurNumberLine=nCurNumberLine;
	translate.nPos=nPos; 

	//�������� ����� ������
	if(nMode==LEXEM_ADDDEF)
	{
		GetStrToEndLine();
		translate.nSizeText=nPos;
	}

	translate.PrepareLexem();
	if(nMode==LEXEM_ADDDEF)
	{
		if(translate.LexemList.GetSize())
			translate.LexemList.SetSize(translate.LexemList.GetSize()-1);//��� ����� ENDPROGRAM

		aDefList->SetDef(csName,&translate.LexemList);
		//nCurNumberLine++;
		nCurNumberLine=translate.nCurNumberLine;
	}
	else
	if(nMode==LEXEM_ADD)
	{
		for(int i=0;i<translate.LexemList.GetSize()-1;i++)//��� ����� ENDPROGRAM
			LexemList.Add(translate.LexemList[i]);
		nPos=translate.nPos;
		nCurNumberLine=translate.nCurNumberLine;
	}
	else
	{
		nPos=translate.nPos;
		nCurNumberLine=translate.nCurNumberLine;
	}
}


void CTranslateModule::OnSetParent(CTranslateModule *pSetParent)
{
	if(!aDefList)
	{
		aDefList=new CDefList();
		aDefList->SetParent(&glDefList);
		bAutoDeleteDefList=1;//������� ������������
	}
	if(pSetParent)
		aDefList->pParent=pSetParent->aDefList;
	else
		aDefList->pParent=&glDefList;
}
