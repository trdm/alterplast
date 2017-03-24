// TypesCont.cpp: implementation of the CTypesCont class.
#include "stdafx.h"
#include "TypesCont.h"
#include "syntax.h"
#include "option.h"
#include "telegroup.h"
#include "resource.h"
#include "txttrap.h"
#include "typegroup.h"

CTypesCont* CTypesCont::m_pOneCont=NULL;
static CStringArray* stringsArray;
static BYTE LoadedComponents=0;

static CMapStringToPtr mapNames;

static int GetStringNum(LPCTSTR txt)
{
	if(!txt || !*txt)
		return 0;
	int num;
	if(mapNames.Lookup(txt,(void*&)num))
		return num;
	num=stringsArray->GetSize();
	stringsArray->Add(txt);
	mapNames[txt]=(void*)num;
	return num;
}

void SplitStrToArray(LPCTSTR str,CStringArray& arr,char delim)
{
	if(!str || !*str)
		return;
	LPCTSTR pRead=str,pStart=pRead;
	while(1)
	{
		while(*pRead && *pRead!=delim)
			pRead++;
		CString key=CString(pStart,pRead-pStart);
		key.TrimLeft(" \t\r\n");
		key.TrimRight(" \t\r\n");
		arr.Add(key);
		if(!*pRead)
			break;
		pStart=++pRead;
	}
}

CTypesCont::CTypesCont()
{
	m_pOneCont=this;
	m_MainType=new STypeBlock;
	m_components=-1;
	m_IndexData=NULL;
	LoadTypes((BYTE)COption::m_pOption->GetComponents());
}

CTypesCont::~CTypesCont()
{
	if(m_IndexData)
		delete [] m_IndexData;
}

/*
Загрузка типа.
- Считываем файл.
- создаем массив для новых номеров строк.
- Считываем строки в общий массив, запоминая их НОВЫЕ номера.
- получаем указатель на типы, методы, атрибуты, параметры, допбазы, доптипы, доппараметры
- проходим типы-атрибуты-методы, проверяя на компоненты, считаем количества.
- выделяем память. 
- Считываем все в память, проставляя новые номера строк.
- Регистрируем типы, атрибуты, методы.
- после обработки всех файлов, обходим типы, заменяя номера строк базовых типов
  на указатели на типы. Для ненайденных уменьшаем количество базовых типов и
  сдвигаем номера в доп массиве.
*/

void CTypesCont::LoadTypes(BYTE Components)
{
	if(!Components)
		Components=16;
	if(Components==m_components)
		return;
	m_components=LoadedComponents=Components;
	// Очищаем строки
	m_strings.RemoveAll();
	m_strings.Add("");
	stringsArray=&m_strings;
	// Очищаем "быстрые" индексы
	m_Name2Type.RemoveAll();
	m_Name2Methods.RemoveAll();
	m_Name2Attribs.RemoveAll();
	if(m_IndexData)
		delete [] m_IndexData;
	// Очистим создаваемые типы
	m_Name2Create.RemoveAll();
	m_CreatableNames.RemoveAll();
	m_CreatableTypes.RemoveAll();

	// Загружаем основные типы
	m_MainType->Load("");
	// Обойдем ранее загруженные дополнительные блоки
	for(POSITION pos=m_Path2Block.GetStartPosition();pos;)
	{
		STypeBlock* pBlock;
		CString		path;
		m_Path2Block.GetNextAssoc(pos,path,(void*&)pBlock);
		pBlock->Load(path);
	}
	// Обшарим bin на предмет новых файлов tls
	WIN32_FIND_DATA fnd;
	HANDLE hFnd=FindFirstFile(pConfSvc->BinDir()+"*.tls",&fnd);
	if(hFnd!=HANDLE(-1))
	{
		do
		{
			if(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			CString path=pConfSvc->BinDir()+fnd.cFileName;
			path.MakeLower();
			if(path.Right(4)!=".tls")
				continue;
			void* val;
			if(!m_Path2Block.Lookup(path,val))
			{
				STypeBlock* pNewBlock=new STypeBlock;
				if(pNewBlock->Load(path))
					m_Path2Block[path]=pNewBlock;
				else
					delete pNewBlock;
			}
		}while(FindNextFile(hFnd,&fnd));
		FindClose(hFnd);
	}
	// Теперь все загружено, обработаем загруженные блоки
	m_MainType->PostLoad();
	for(pos=m_Path2Block.GetStartPosition();pos;)
	{
		STypeBlock* pBlock;
		CString		path;
		m_Path2Block.GetNextAssoc(pos,path,(void*&)pBlock);
		pBlock->PostLoad();
	}
	mapNames.RemoveAll();

	// Строим индексы
	// Необходимо перебрать все типы, подсчитав одинаковые атрибуты
	// и методы. Расчитать объем памяти и выделить ее.
	// вписать в карту указатели на области памяти.
	// Снова обойти типы, вписав в области памяти указатели на
	// атрибуты и методы
	PrepeareTypeIndex(m_MainType);
	for(pos=m_Path2Block.GetStartPosition();pos;)
	{
		STypeBlock* pBlock;
		CString		path;
		m_Path2Block.GetNextAssoc(pos,path,(void*&)pBlock);
		PrepeareTypeIndex(pBlock);
	}
	int total=0;
	CStringArray keysatr,keysmet;
	CDWordArray cntAtr,cntMet;
	CString key;
	for(pos=m_Name2Attribs.GetStart();pos;)
	{
		int cnt=(int)m_Name2Attribs.GetNext(pos,key);
		keysatr.Add(key);
		cntAtr.Add(cnt);
		total+=cnt+1;
	}
	for(pos=m_Name2Methods.GetStart();pos;)
	{
		int cnt=1+(int)m_Name2Methods.GetNext(pos,key);
		keysmet.Add(key);
		cntMet.Add(cnt);
		total+=cnt;
	}
	m_IndexData=new DWORD[total];
	memset(m_IndexData,0,total<<2);
	DWORD* ptr=(DWORD*)m_IndexData;
	for(int i=0;i<keysatr.GetSize();i++)
	{
		m_Name2Attribs.InsertKey(keysatr[i],ptr);
		ptr+=cntAtr[i];
	}
	for(i=0;i<keysmet.GetSize();i++)
	{
		m_Name2Methods.InsertKey(keysmet[i],ptr);
		ptr+=cntMet[i];
	}
	IndexTypes(m_MainType);
	for(pos=m_Path2Block.GetStartPosition();pos;)
	{
		STypeBlock* pBlock;
		CString		path;
		m_Path2Block.GetNextAssoc(pos,path,(void*&)pBlock);
		IndexTypes(pBlock);
	}
	CTypeGroup::ReFillAll();
}

static char* LoadAndUnzip(CString path)
{
	HANDLE hFile=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==HANDLE(-1))
		return NULL;
	DWORD size=GetFileSize(hFile,NULL);
	if(!size)
		return NULL;
	IStreamPtr pStreamIn,pStreamOut;
	if (S_OK!=CreateStreamOnHGlobal(NULL,TRUE,&pStreamIn)  || pStreamIn==NULL ||
		S_OK!=CreateStreamOnHGlobal(NULL,TRUE,&pStreamOut) || pStreamOut==NULL)
		return NULL;
	char buf[0x1000];
	DWORD read;
	while(size)
	{
		ReadFile(hFile,buf,sizeof(buf),&read,NULL);
		pStreamIn->Write(buf,read,NULL);
		size-=read;
	}
	CloseHandle(hFile);
	IzlibEnginePtr zlibeng;
	if(S_OK!=zlibeng.CreateInstance(CLSID_zlibEngine))
		return NULL;
	LARGE_INTEGER liZero;
	liZero.QuadPart=0;
	pStreamIn->Seek(liZero,STREAM_SEEK_SET,NULL);
	if(S_OK!=zlibeng->pkDecompress(pStreamIn,pStreamOut))
		return NULL;
	STATSTG stg;
	if(S_OK!=pStreamOut->Stat(&stg,0))
		return NULL;
	HANDLE hStream;
	if(S_OK!=GetHGlobalFromStream(pStreamOut,&hStream) || !hStream)
		return NULL;
	char* pData=new char[stg.cbSize.QuadPart];
	if(!pData)
		return NULL;
	void* pMemBlock=GlobalLock(hStream);
	if(!pMemBlock)
	{
		delete [] pData;
		return NULL;
	}
	memcpy(pData,pMemBlock,stg.cbSize.QuadPart);
	GlobalUnlock(hStream);
	return pData;
}

bool STypeBlock::Load(CString path)
{
	if(data)
	{
		delete [] data;
		data=NULL;
	}
	cntTypes=0;
	LPTSTR pData=NULL;
	if(path.IsEmpty())
	{
		#ifdef BUILD_RESOURCE
			CString prdir=__FILE__;
			pData=LoadAndUnzip(prdir.Left(prdir.ReverseFind('\\')+1)+"1cv7lang.bin");
		#else
			pData=(char*)GetZipResource(IDR_1CV7LANG);
		#endif
	}
	else
		pData=LoadAndUnzip(path);
	if(!pData)
		return false;
	static const char magic[16]={(char)0xad,(char)0xe5,(char)0xba,(char)0xca,(char)0x95,(char)0x00,
		(char)0x47,(char)0x58,(char)0x88,(char)0x81,(char)0xc8,(char)0xe7,(char)0x50,
		(char)0x6c,(char)0x20,(char)0x7e};
	for(int i=0;i<16;i++)
	{
		if(pData[i]!=magic[i])
			break;
	}
	if(i<16)
	{
		delete [] pData;
		return false;
	}
	SHeadType*	pHead=(SHeadType*)(pData+16);
	LPCTSTR		pStrings=(LPCTSTR)((char*)pHead)+sizeof(SHeadType);
	STypeInfo*	pTypes=(STypeInfo*)(pStrings+pHead->StrSize),*pCurTypes=pTypes;
	SAttrInfo*	pAttrs=(SAttrInfo*)(pTypes+pHead->TypeCount),*pCurAttrs=pAttrs;
	SMethInfo*	pMeths=(SMethInfo*)(pAttrs+pHead->AttrCount),*pCurMeths=pMeths;
	SParaInfo*	pParam=(SParaInfo*)(pMeths+pHead->MethCount),*pCurParam=pParam;
	DWORD*		pAddBase=(DWORD*)(pParam+pHead->ParaCount),*pCurAddBase=pAddBase;
	DWORD*		pAddRets=pAddBase+pHead->AddBaseCount,*pCurAddRets=pAddRets;
	DWORD*		pAddPars=pAddRets+pHead->AddRetCount,*pCurAddPars=pAddPars;
	DWORD*		pCrObjs=pAddPars+pHead->AddPTypeCount,*pCurCrObjs=pCrObjs;

	int TypeCnt=0,AttrCnt=0,MethCnt=0,ParaCnt=0,
		AddBaseCnt=0,AddRetCnt=0,AddParCnt=0;
	DWORD* pNewStrings=new DWORD[pHead->StrCount];
	memset(pNewStrings,0,pHead->StrCount*4);
	CMapPtrToPtr usedTypes;

	// Проход 1.
	// Подсчитываем статистику с учетом установленных компонент
	// и отмечаем используемые строки.
	for(i=0;i<pHead->TypeCount;i++)
	{
		bool TypeLoaded=(pCurTypes->Components & LoadedComponents)!=0;
		if(TypeLoaded)
		{
			LPCTSTR ptr=pStrings;
			for(int n=0;n<pCurTypes->Name;n++)
			{
				while(*ptr++);
			}
			if(CTypesCont::m_pOneCont->GetTypeInfo(ptr))
			{
				DoMsgLine("Телепат: тип %s уже зарегистрирован.",mmRedErr,ptr);
				TypeLoaded=false;
			}
		}
		// Обработаем данные о типе
		if(TypeLoaded)
		{
			TypeCnt++;
			usedTypes  [(void*)pCurTypes->Name] =(void*)1;
			pNewStrings[pCurTypes->Name] =1;
			pNewStrings[pCurTypes->Alias]=1;
		}
		if(pCurTypes->BaseCnt==1)
		{
			if(TypeLoaded)
				pNewStrings[pCurTypes->pBases]=1;
		}
		else
		{
			for(int k=0;k<pCurTypes->BaseCnt;k++)
			{
				if(TypeLoaded)
				{
					AddBaseCnt++;
					pNewStrings[*pCurAddBase]=1;
				}
				pCurAddBase++;
			}
		}
		// Обработаем атрибуты
		for(int k=0;k<pCurTypes->AttrCnt;k++)
		{
			bool AttrLoaded=TypeLoaded && ((pCurAttrs->Components & LoadedComponents)!=0);
			if(AttrLoaded)
			{
				AttrCnt++;
				pNewStrings[pCurAttrs->NameRus]=1;
				pNewStrings[pCurAttrs->NameEng]=1;
				pNewStrings[pCurAttrs->Descr]=1;
				pNewStrings[pCurAttrs->strTypeName]=1;
			}
			pCurAttrs++;
		}
		// Обработаем методы
		for(k=0;k<pCurTypes->MethCnt;k++)
		{
			bool MethLoaded=TypeLoaded && ((pCurMeths->Components & LoadedComponents)!=0);
			if(MethLoaded)
			{
				MethCnt++;
				pNewStrings[pCurMeths->NameRus]=1;
				pNewStrings[pCurMeths->NameEng]=1;
				pNewStrings[pCurMeths->Descr]=1;
				pNewStrings[pCurMeths->retDescr]=1;
			}
			// Обработаем типы возвращаемых значений
			if(pCurMeths->retTypeCnt==1)
			{
				if(MethLoaded)
					pNewStrings[pCurMeths->strRetTypeName]=1;
			}
			else
			{
				for(int j=0;j<pCurMeths->retTypeCnt;j++)
				{
					if(MethLoaded)
					{
						AddRetCnt++;
						pNewStrings[*pCurAddRets]=1;
					}
					pCurAddRets++;
				}
			}
			// Обработаем параметры
			for(int j=0;j<pCurMeths->ParamCnt;j++)
			{
				if(MethLoaded)
				{
					ParaCnt++;
					pNewStrings[pCurParam->Name]=1;
					pNewStrings[pCurParam->DefVal]=1;
					pNewStrings[pCurParam->Descr]=1;
				}
				if(pCurParam->TypeCnt==1)
				{
					if(MethLoaded)
						pNewStrings[pCurParam->strTypeName]=1;
				}
				else
				{
					for(int n=0;n<pCurParam->TypeCnt;n++)
					{
						if(MethLoaded)
						{
							AddParCnt++;
							pNewStrings[*pCurAddPars]=1;
						}
						pCurAddPars++;
					}
				}
				pCurParam++;
			}
			pCurMeths++;
		}
		pCurTypes++;
	}
	// Обойдем создаваемые объекты, проверяя на загруженность типов
	for(i=0;i<pHead->CrObCount;i++)
	{
		DWORD num;
		if(usedTypes.Lookup((void*)*pCurCrObjs,(void*&)num))
		{
			// Тип используется, пометим строку с создаваемым именем
			pNewStrings[pCurCrObjs[1]]=1;
		}
		pCurCrObjs+=2;
	}

	/*
	DoMsgLine(
		"Типов: %i, %i\r\n"
		"Атрибутов: %i, %i\r\n"
		"Методов: %i, %i\r\n"
		"Параметров: %i, %i\r\n"
		"Допбаз: %i, %i\r\n"
		"Допметодов: %i, %i\r\n"
		"Доппараметров: %i, %i",mmNone,
		pHead->TypeCount,TypeCnt,
		pHead->AttrCount,AttrCnt,
		pHead->MethCount,MethCnt,
		pHead->ParaCount,ParaCnt,
		pHead->AddBaseCount,AddBaseCnt,
		pHead->AddRetCount,AddRetCnt,
		pHead->AddPTypeCount,AddParCnt);
	*/

	cntTypes=TypeCnt;
	// Добавим в общий массив используемые строки
	// запомнив их новые номера.
	LPCTSTR pStr=pStrings;
	for(i=0;i<pHead->StrCount;i++)
	{
		if(pNewStrings[i])
			pNewStrings[i]=GetStringNum(pStr);
		while(*pStr++);
	}
	// Выделим блок памяти подо все
	int total =	TypeCnt*sizeof(STTypeInfo)	+ AttrCnt*sizeof(STAttrInfo)+
				MethCnt*sizeof(STMethInfo)	+ ParaCnt*sizeof(SParaInfo) +
				AddBaseCnt*sizeof(DWORD)	+ AddRetCnt*sizeof(DWORD)   +
				AddParCnt*sizeof(DWORD);
	data=new char [total];//[0x20000];
	memset(data,0,total);
	// Настроим указатели
	STTypeInfo* pTypeW=(STTypeInfo*)data;
	STAttrInfo* pAttrW=(STAttrInfo*)(pTypeW+TypeCnt);
	STMethInfo* pMethW=(STMethInfo*)(pAttrW+AttrCnt);
	SParaInfo*  pParaW=(SParaInfo*) (pMethW+MethCnt);
	DWORD*	pAddBaseW=(DWORD*)(pParaW+ParaCnt);
	DWORD*	pAddRettW=(DWORD*)(pAddBaseW+AddBaseCnt);
	DWORD*	pAddParaW=(DWORD*)(pAddRettW+AddRetCnt);

	// Проход 2.
	// Считываем инфу в подготовленный блок, ставя новые номера строк
	for(i=0;i<pHead->TypeCount;i++)
	{
		bool TypeLoaded=(pTypes->Components & LoadedComponents)!=0;
		int tAttrCnt=0,tMethCnt=0;
		if(TypeLoaded)
		{
			LPCTSTR ptr=pStrings;
			for(int n=0;n<pTypes->Name;n++)
			{
				while(*ptr++);
			}
			if(CTypesCont::m_pOneCont->GetTypeInfo(ptr))
				TypeLoaded=false;
		}
		// Обработаем данные о типе
		if(TypeLoaded)
		{
			pTypeW->Name	= pNewStrings[pTypes->Name];
			pTypeW->Alias	= pNewStrings[pTypes->Alias];
			pTypeW->AttrCnt	= pTypes->AttrCnt;
			pTypeW->MethCnt	= pTypes->MethCnt;
			pTypeW->BaseCnt	= pTypes->BaseCnt;
			pTypeW->Flags	= pTypes->Flags;
			pTypeW->pAttribs= pAttrW;
			pTypeW->pMethods= pMethW;
			pTypeW->BaseCnt	= pTypes->BaseCnt;
		}
		if(pTypes->BaseCnt==1)
		{
			if(TypeLoaded)
				pTypeW->pBases=(STTypeInfo*)pNewStrings[pTypes->pBases];
		}
		else
		{
			if(TypeLoaded)
				pTypeW->pBases=(STTypeInfo*)pAddBaseW;
			for(int k=0;k<pTypes->BaseCnt;k++)
			{
				if(TypeLoaded)
					*pAddBaseW++=pNewStrings[*pAddBase];
				pAddBase++;
			}
		}
		// Обработаем атрибуты
		for(int k=0;k<pTypes->AttrCnt;k++)
		{
			bool AttrLoaded=TypeLoaded && ((pAttrs->Components & LoadedComponents)!=0);
			if(AttrLoaded)
			{
				tAttrCnt++;
				pAttrW->NameRus		= pNewStrings[pAttrs->NameRus];
				pAttrW->NameEng		= pNewStrings[pAttrs->NameEng];
				pAttrW->Descr		= pNewStrings[pAttrs->Descr];
				pAttrW->strTypeName	= pNewStrings[pAttrs->strTypeName];
				pAttrW->Flags		= pAttrs->Flags;
				pAttrW->ParentType	= pTypeW;
				pAttrW++;
			}
			pAttrs++;
		}
		// Обработаем методы
		for(k=0;k<pTypes->MethCnt;k++)
		{
			bool MethLoaded=TypeLoaded && ((pMeths->Components & LoadedComponents)!=0);
			if(MethLoaded)
			{
				tMethCnt++;
				pMethW->NameRus		= pNewStrings[pMeths->NameRus];
				pMethW->NameEng		= pNewStrings[pMeths->NameEng];
				pMethW->Descr		= pNewStrings[pMeths->Descr];
				pMethW->retDescr	= pNewStrings[pMeths->retDescr];
				pMethW->flags		= pMeths->flags;
				pMethW->ParamCnt	= pMeths->ParamCnt;
				pMethW->ParentType	= pTypeW;
				pMethW->pParams		= pParaW;
				pMethW->retTypeCnt	= pMeths->retTypeCnt;
			}
			// Обработаем типы возвращаемых значений
			if(pMeths->retTypeCnt==1)
			{
				if(MethLoaded)
					pMethW->strRetTypeName=pNewStrings[pMeths->strRetTypeName];
			}
			else
			{
				if(MethLoaded)
					pMethW->strRetTypeName=(DWORD)pAddRettW;
				for(int j=0;j<pMeths->retTypeCnt;j++)
				{
					if(MethLoaded)
						*pAddRettW++=pNewStrings[*pAddRets];
					pAddRets++;
				}
			}
			// Обработаем параметры
			for(int j=0;j<pMeths->ParamCnt;j++)
			{
				if(MethLoaded)
				{
					pParaW->Name	= pNewStrings[pParam->Name];
					pParaW->DefVal	= pNewStrings[pParam->DefVal];
					pParaW->Descr	= pNewStrings[pParam->Descr];
					pParaW->TypeCnt	= pParam->TypeCnt;
				}
				if(pParam->TypeCnt==1)
				{
					if(MethLoaded)
						pParaW->strTypeName=pNewStrings[pParam->strTypeName];
				}
				else
				{
					if(MethLoaded)
						pParaW->strTypeName=(DWORD)pAddParaW;
					for(int n=0;n<pParam->TypeCnt;n++)
					{
						if(MethLoaded)
							*pAddParaW++=pNewStrings[*pAddPars];
						pAddPars++;
					}
				}
				pParam++;
				if(MethLoaded)
					pParaW++;
			}
			pMeths++;
			if(MethLoaded)
				pMethW++;
		}
		pTypes++;
		if(TypeLoaded)
		{
			pTypeW->AttrCnt=tAttrCnt;
			pTypeW->MethCnt=tMethCnt;
			CTypesCont::m_pOneCont->RegisterType(pTypeW++);
		}
	}

	for(i=0;i<pHead->CrObCount;i++)
	{
		DWORD num;
		if(usedTypes.Lookup((void*)*pCrObjs,(void*&)num))
		{
			CTypesCont::m_pOneCont->RegisterCreatable(pNewStrings[*pCrObjs],pNewStrings[pCrObjs[1]]);
		}
		pCrObjs+=2;
	}

	delete [] pNewStrings;
	delete [] pData;
	return true;
}

void STypeBlock::PostLoad()
{
	// Пройдем базовые классы, заменив номера имен на указатели
	STTypeInfo* pType=(STTypeInfo*)data;
	CTypesCont* pCont=CTypesCont::m_pOneCont;
	for(int i=0;i<cntTypes;i++)
	{
		if(pType->BaseCnt==1)
		{
			CString& name=pCont->GetString((int)pType->pBases);
			pType->pBases=pCont->GetTypeInfo(name);
			if(!pType->pBases)
			{
				DoMsgLine("Тип \"\": базовый тип \"\" неопределен",mmNone,
					pCont->GetString(pType->Name),name);
				pType->BaseCnt=0;
			}
		}
		else
		{
			STTypeInfo** pBases=(STTypeInfo**)pType->pBases;
			for(int k=0,l=0;k<pType->BaseCnt;k++)
			{
				CString& name=pCont->GetString((int)pBases[k]);
				STTypeInfo* pBase=pCont->GetTypeInfo(name);
				if(pBase)
					pBases[l++]=pBase;
				else
				{
					DoMsgLine("Тип \"\": базовый тип \"\" неопределен",mmNone,
						pCont->GetString(pType->Name),name);
					pType->BaseCnt--;
				}
			}
		}
		pType++;
	}
}

void CTypesCont::RegisterType(STTypeInfo *pType)
{
	m_Name2Type.InsertKey(m_strings[pType->Name],pType);
}

void CTypesCont::PrepeareTypeIndex(STypeBlock* pBlock)
{
	for(int t=0;t<pBlock->cntTypes;t++)
	{
		STTypeInfo* pType=((STTypeInfo*)pBlock->data)+t;
		for(int i=0;i<pType->AttrCnt;i++)
		{
			if(pType->pAttribs[i].NameRus)
			{
				CString name=m_strings[pType->pAttribs[i].NameRus];
				int cnt=(int)m_Name2Attribs.GetKey(name);
				m_Name2Attribs.InsertKey(name,(void*)++cnt);
			}
			if(pType->pAttribs[i].NameEng)
			{
				CString name=m_strings[pType->pAttribs[i].NameEng];
				int cnt=(int)m_Name2Attribs.GetKey(name);
				m_Name2Attribs.InsertKey(name,(void*)++cnt);
			}
		}
		for(i=0;i<pType->MethCnt;i++)
		{
			if(pType->pMethods[i].NameRus)
			{
				CString name=m_strings[pType->pMethods[i].NameRus];
				int cnt=(int)m_Name2Methods.GetKey(name);
				m_Name2Methods.InsertKey(name,(void*)++cnt);
			}
			if(pType->pMethods[i].NameEng)
			{
				CString name=m_strings[pType->pMethods[i].NameEng];
				int cnt=(int)m_Name2Methods.GetKey(name);
				m_Name2Methods.InsertKey(name,(void*)++cnt);
			}
		}
	}
}

void CTypesCont::IndexTypes(STypeBlock *pBlock)
{
	for(int t=0;t<pBlock->cntTypes;t++)
	{
		STTypeInfo* pType=((STTypeInfo*)pBlock->data)+t;
		for(int i=0;i<pType->AttrCnt;i++)
		{
			if(pType->pAttribs[i].NameRus)
			{
				CString name=m_strings[pType->pAttribs[i].NameRus];
				STAttrInfo** pAtr=(STAttrInfo**)m_Name2Attribs.GetKey(name);
				while(*pAtr)
					pAtr++;
				*pAtr=pType->pAttribs+i;
			}
			if(pType->pAttribs[i].NameEng)
			{
				CString name=m_strings[pType->pAttribs[i].NameEng];
				STAttrInfo** pAtr=(STAttrInfo**)m_Name2Attribs.GetKey(name);
				while(*pAtr)
					pAtr++;
				*pAtr=pType->pAttribs+i;
			}
		}
		for(i=0;i<pType->MethCnt;i++)
		{
			if(pType->pMethods[i].NameRus)
			{
				CString name=m_strings[pType->pMethods[i].NameRus];
				STMethInfo** pMet=(STMethInfo**)m_Name2Methods.GetKey(name);
				while(*pMet)
					pMet++;
				*pMet=pType->pMethods+i;
			}
			if(pType->pMethods[i].NameEng)
			{
				CString name=m_strings[pType->pMethods[i].NameEng];
				STMethInfo** pMet=(STMethInfo**)m_Name2Methods.GetKey(name);
				while(*pMet)
					pMet++;
				*pMet=pType->pMethods+i;
			}
		}
	}
}

void CTypesCont::RegisterCreatable(DWORD tName, DWORD cName)
{
	STTypeInfo* pType=GetTypeInfo(m_strings[tName]);
	CString tt=m_strings[tName],
		ct=m_strings[cName];
	if(pType)
	{
		m_CreatableNames.Add(cName);
		m_CreatableTypes.Add(pType);
		m_Name2Create.InsertKey(m_strings[cName],pType);
	}
}
