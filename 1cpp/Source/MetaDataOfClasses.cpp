// MetaDataOfClasses.cpp: implementation of the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "MetaDataOfClasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMetaDataOfClasses::CMetaDataOfClasses() : m_bInsideDynamicAdd(false)
{

}

CMetaDataOfClasses::~CMetaDataOfClasses()
{
//   POSITION pos;
//   for (pos = m_MapClassNamesToMethodsMap.GetStartPosition(); pos != NULL;)
//   {
//     CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;//CMapStringToOb* pMapFunc = NULL;
//     CString key;
//     m_MapClassNamesToMethodsMap.GetNextAssoc( pos, key, pMapFunc);
//     
//     for (POSITION pos2 = pMapFunc->GetStartPosition(); pos2 != NULL;)
//     {
//       CArrayOfMethodsParamData_PTR param;// = NULL;
//       pMapFunc->GetNextAssoc( pos2, key, (CObject*&)param);      
//       delete param; // TODO неявная работа с памятью в двух модулях, проще исправить на смарт-пойнтер
//     }
//   }  
}

// Метод разбирает строку текста и заполняет внутренние структуры класса. 
// Строка должна состоять из объявлений классов с привязкой к именам файлов 
// хранящих реализацию и иерархическую структуру этих классов

//pathBase1C
//m_AliasOfPaths
//m_ClassBindFile
//m_ClassesHierarchyMap
//m_MapClassNamesToMethodsMap
//ARR_TYPE_NAME
//CDataOfParamForUserMethods
//m_ValParamInMeth
//m_UnlimitsParams

void CMetaDataOfClasses::ParsingString(CString& StringForParse, CRangesOfDescr& RangesDescr)
{   
  if (StringForParse.IsEmpty())
    return;
  int posComment = 0;
  
  // уберем комментарии
  int nStart = 0;
  int nLength = StringForParse.GetLength();

  while ((posComment = StringForParse.Find("//", nStart)) != -1)
  {    
    int nCount = 0;    
    if ((nLength != posComment+2)&&(StringForParse[posComment+2] == '#'))
    {
      nCount = 3;
    }
    else
    {
      int i = posComment;
      while(i != nLength && StringForParse[i] != '\r')
      {
        ++i;
        ++nCount;
      }
    }    
    char* p = const_cast<char*>(StringForParse.operator LPCTSTR());
    memset(p+posComment, 1, nCount); 
  }
  //pBkEndUI->DoMessageLine(StringForParse, mmExclamation);

  int st = 0;
 
  CString token;
  for (int i = 0; i < nLength; i++)
  {
    CHAR symbol = StringForParse[i];
    
    if (symbol == ' ' || symbol == '\n' || symbol == '\r' || symbol == 1)
      continue;

    // далее необходимо найти ключевое слово class или класс
    token.Empty();
    while(symbol != ' ' && symbol != '\r' && symbol != '\t' && symbol != '\n')
    {            
      token += symbol;
      i++;
      if (i == nLength)
          break;
      symbol = StringForParse[i];
    }
    if (!token.IsEmpty())
    {
      if (token.CompareNoCase("class") == 0 || token.CompareNoCase("класс") == 0)
      {
        //pBkEndUI->DoMessageLine(token, mmExclamation);
        // начинается определение класса
        CString DefClass;
        st = 0;
        bool isClasDef = false;
        for (;i < nLength; i++)
        {
          symbol = StringForParse[i];                    
          if (symbol == '}')
          {
            //st = 1;
            isClasDef = true;
            DefClass += symbol;
            break;
          }
          /*if (symbol == ';' && st == 1)
          {
            isClasDef = true;
            break;
          }*/
          if (symbol != 1)
            DefClass += symbol;
        }
        if (isClasDef && !DefClass.IsEmpty())
        {
          // определение класса находиться в DefClass
          // Разделим на заголовок класса, где определено имя класса, файл реализации и на 
          // определение функций и процедур находящееся в фигурных скобках

          int nPosFigOpen  = DefClass.Find("{");
          int nPosFigClose = DefClass.Find("}");
          if (nPosFigClose == -1 || nPosFigOpen == -1)
          {
			MessageBox(NULL, DefClass, "Ошибка: не хватает скобки }",MB_ICONERROR);
            break;
          }
          CString strHead    = DefClass.Left(nPosFigOpen+1);
          CString strDefMeth = DefClass.Mid(nPosFigOpen+1, nPosFigClose - nPosFigOpen - 1);
          
          strHead.TrimRight();
          strHead.TrimLeft();
          CString NameOfClass;

          if (!strHead.IsEmpty())
          {
            CString NameOfFile;
            CString strAlies;
            CString NameOfBaseClass;
            CStringList_SharedPtr pList(new CStringList);
            strHead.Replace(" ", NULL); // убираем все пробелы // TODO artbear может быть, все-таки нужно убирать пробелы

            int nLen = strHead.GetLength();
            st = 0;
            for (int i2 = 0; i2 < nLen; i2++)
            {            
              if (strHead[i2] == ' ' || strHead[i2] == '\n' || strHead[i2] == '\t' || strHead[i2] == 9 || strHead[i2] == '\r')
                continue;
              
              if (strHead[i2] == '=')
              {
                st = 1;
                continue;
              }
              if (strHead[i2] == ':' && strHead[i2+1==nLen?i2:i2+1] != '\\')
              {
                st = 2;
                continue;
              }
              if (strHead[i2] == ',' || strHead[i2] == '{')
                st = 3;
              
              if (strHead[i2] == '#' && st == 1)
              {
                st = 4;
                continue;
              }
              
              if (strHead[i2] == '\\' && st == 4)
                st = 5;

              switch (st)
              {
              case 0:
                NameOfClass += strHead[i2];
                break;               
              case 1:
                NameOfFile += strHead[i2];
                break;
              case 2:
                NameOfBaseClass += strHead[i2];
                break;
              case 3:
                NameOfBaseClass.TrimLeft();
                NameOfBaseClass.TrimRight();
                if (!NameOfBaseClass.IsEmpty())
                {
                  pList->AddTail(NameOfBaseClass);             
                  NameOfBaseClass.Empty();
                }
                st = 2;
                break;
              case 4:
                strAlies += strHead[i2];
                break;
              case 5:
                st = 1;
                if (!strAlies.IsEmpty())
                {
                  CString strPathOfAlies = m_AliasOfPaths[strAlies];
                  if (!strPathOfAlies.IsEmpty())
                  {
                    NameOfFile += strPathOfAlies;
                    if (strPathOfAlies[strPathOfAlies.GetLength()-1] != '\\')
                      NameOfFile += '\\';                      
                  }
                  else
                  {
					MessageBox(NULL, strAlies, "Ошибка: в ini файле нет такого псевдонима", MB_ICONERROR);
                  }
                }
                break;
              default:
                break;
              } 
            }
            NameOfClass.TrimLeft();
            NameOfClass.TrimRight();
            NameOfFile.TrimLeft();
            NameOfFile.TrimRight();
            if (!NameOfClass.IsEmpty())
            {
              /*CString text;
              text.Format("%d", i);
              pBkEndUI->DoMessageLine(text, mmExclamation);*/
              char drive[_MAX_DRIVE] = {0};
              char dir[_MAX_DIR] = {0};
              char fname[_MAX_FNAME] = {0};
              char ext[_MAX_EXT] = {0};
              CString path_fileDescr(RangesDescr.GetNameOfFileDescrOnPos(i));              
              if (!path_fileDescr.IsEmpty())
              {
                _splitpath(path_fileDescr, drive, dir, fname, ext );
                m_FullFileNameOfDescrFile[NameOfClass] = path_fileDescr;
              }

              //pBkEndUI->DoMessageLine(NameOfClass, mmExclamation);
              //pBkEndUI->DoMessageLine(RangesDescr.GetNameOfFileDescrOnPos(i), mmExclamation);
              
              if (!NameOfFile.IsEmpty())
              {
                if (!path_fileDescr.IsEmpty())
// artbear
/*                  if (NameOfFile.Find(":") == -1)
                  {
                    if (NameOfFile.Find("@MD") == -1)
                    if (NameOfFile.Find("@md") == -1)
                    {
                      if (NameOfFile.GetLength() > 1 && (NameOfFile.GetAt(0) != '\\' || NameOfFile.GetAt(0) != '.') && NameOfFile.GetAt(1) != '\\')
                      {
                        CString path(drive);
                        path+=dir;
                        NameOfFile.Insert(0, path);
                      }
                    }
                  }
*/
			// получу полное имя файла или из каталога ИБ или относительно каталога текущего defcls.prm
			{
				CString sDefFileFolderPath = ::ExtractFileDir(path_fileDescr);
				CSetOfHierarchy::MakeFullFileName(&NameOfFile, sDefFileFolderPath);
			}
// end
                //pBkEndUI->DoMessageLine(NameOfFile, mmExclamation);

                CString strResult;

                if (m_ClassBindFile.Lookup(NameOfClass, strResult))
                {
                  CString error = "Класс с именем: ";
                  error += NameOfClass; error += " уже был определен ранее";
				  MessageBox(NULL, error, "Ошибка определения класса", MB_ICONERROR);
                }
                else{
                  m_ClassBindFile[NameOfClass] = NameOfFile;
				  m_DynamicAddedClassBindFile[NameOfClass] = NameOfFile;
				}
              }
            }
            if ((!pList->IsEmpty())&&(!NameOfClass.IsEmpty()))
              m_ClassesHierarchyMap[NameOfClass] = pList;
          }
          else
          {
			MessageBox(NULL, DefClass, "Ошибка: неопределено название класса", MB_ICONERROR);
            break;
          }

          // Определение методов
          //strDefMeth.TrimRight();
          //strDefMeth.TrimLeft();
          
          if (!strDefMeth.IsEmpty() && !NameOfClass.IsEmpty())
          {
            int nLen = strDefMeth.GetLength();
            CStringList ListStrMeth;
            CString str;
            bool nQuotes = false;
            bool bIsExistCloseBroken = false;

            for (int pos = 0; pos < nLen; pos++)
            {              
			        if (strDefMeth[pos] == '\"' && !nQuotes)
                nQuotes = true;
              else if (strDefMeth[pos] == '\"' && nQuotes)
                nQuotes = false;
              
              if (strDefMeth[pos] == ';' && !nQuotes)
              {                
                str += strDefMeth[pos];
                ListStrMeth.AddTail(str);
                str.Empty();                
              }
              else
              {              
                str += strDefMeth[pos];
              }
            }
			
			CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapMeth; //boost::shared_ptr<CIStringMap<CParamDefs::CArrayOfMethodsParamData_PTR, CParamDefs::CArrayOfMethodsParamData_PTR&> > pMapMeth; //CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapMeth;
            for(POSITION posList = ListStrMeth.GetHeadPosition(); posList != NULL; )
            {              
              str = ListStrMeth.GetNext( posList );
              str.TrimLeft();
              int nLen = str.GetLength();
              st = 1;
              int nSemicolon = -1;

              int nPosOpenBracket  = str.Find("(");              
              bool nQuotes = false;
			        for (int n = nLen - 1; n >= nPosOpenBracket ; --n)
			        {			          
			          if (str[n] == '\"' && !nQuotes)
                  nQuotes = true;
                else if (str[n] == '\"' && nQuotes)
                  nQuotes = false;
			          
                if (str[n] == ';' && !nQuotes)
                {
                  nSemicolon = n;
				          break;
                }

			        }               
              if (nSemicolon == -1)
              {
				MessageBox(NULL, str, "Ошибка: в объявлении метода не найдена ;", MB_ICONERROR);
                continue;
              }
              int nPosCloseBracket = -1;
              for (int nIter = nLen; nIter != 0; --nIter)
              {
                if (str[nIter-1] == ')')
                {
                  nPosCloseBracket = nIter-1;
                  break;
                }
              }
              
              if (nPosOpenBracket == -1)
              {
				MessageBox(NULL, str, "Ошибка: в объявлении метода не найдена (", MB_ICONERROR);
                continue;
              }
              if (nPosCloseBracket == -1)
              {
				MessageBox(NULL, str, "Ошибка: в объявлении метода не найдена )", MB_ICONERROR);
                continue;
              }

              if (!pMapMeth)//if(pMapMeth == NULL)
              {
				  pMapMeth = CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData(new CParamDefs::CMapMethodsNameToArrayOfMethodsParamData);
                m_MapClassNamesToMethodsMap[NameOfClass] = pMapMeth;
              }              

              CString token;
              CString strNameOfMeth;
			  CParamDefs::CArrayOfMethodsParamData_PTR pArr(CParamDefs::Create_ArrayOfMethodsParamData() ); //new CParamDefs::ARR_TYPE_NAME); // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
              st = 0;
              // Обработаем первую часть объявления метода, где указаны имя и тип возвращаемой
              // переменой.
              bool IsRetVal = true;
              for (int pos = 0; pos < nPosOpenBracket; pos++)
              {

                if (str[pos] != ' ' && str[pos] != '/n' && str[pos] != 9)
                  token += str[pos];
                else
                  st = 1;

                if (st == 1 && !token.IsEmpty() && IsRetVal)
                {
                  if (token.Compare("void") != 0)
                  {
					  pArr->Add(CMethodsParamData::Create(CString("#return"), token));
                  }
                  token.Empty();
                  st = 0;
                  IsRetVal = false;
                }
              }
              if (!token.IsEmpty())
              {
                pMapMeth-> operator [](token) = pArr;                
                strNameOfMeth = token;
                token.Empty();
              }
              else
              {
                MessageBox(NULL, str, "Ошибка: в объявлении метода не найдено название", MB_ICONERROR);
                //delete pArr; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
                continue;
              }
            
              // Обработаем выражение в скобках
              CStringList ListStrOfParam;
              nQuotes = false;
              for(pos = nPosOpenBracket+1; pos < nPosCloseBracket; pos++)
              {
                if (str[pos] == '\"' && !nQuotes)
                  nQuotes = true;
                else if (str[pos] == '\"' && nQuotes)
                  nQuotes = false;

                if (str[pos] == ',' && !token.IsEmpty() && !nQuotes)
                {
                  token.TrimLeft();
                  token.TrimRight();
                  if (!token.IsEmpty())
                  {
                    ListStrOfParam.AddTail(token);                    
                    token.Empty();
                  }
                }
                else
                {
                  token += str[pos];
                }
                
              }
              if (!token.IsEmpty())
              {
                token.TrimLeft();
                token.TrimRight();
                ListStrOfParam.AddTail(token);            
                token.Empty();
              }
              // Переменная определяет существует ли параметры передаваемые по значению
              bool bExistValPar = false;
              
              // Переменная определяет существует ли параметр ...
              bool bExistUnLimits = false;

              for(POSITION posList2 = ListStrOfParam.GetHeadPosition(); posList2 != NULL; )
              {
                CString strParam = ListStrOfParam.GetNext( posList2 );
                RemoveNeedlessBlank(strParam);
                
                int nStrLen = str.GetLength();
                
                if (bExistUnLimits)
                  continue;                

                if (!bExistUnLimits && "..." == strParam)                
                  bExistUnLimits = true;                                  
                
                int nPosEq = strParam.Find("=");
                
                if (nPosEq != -1)
                { // Уберем пробелы, если они есть, вокруг знака равно
                  if ((nPosEq+1 < nStrLen)&&strParam[nPosEq+1] == ' ')
                  {
                    strParam.Delete(nPosEq+1);
                  }
                  if ((nPosEq-1 >= 0)&&strParam[nPosEq-1] == ' ')
                  {
                    strParam.Delete(nPosEq-1);
                  }
                }                
               
                int nPosSpace = strParam.Find(" ");
                if (nPosSpace == -1)
                {
                  pArr->Add(CMethodsParamData::Create(CString(""), strParam));
                }
                else
                {
                  CString masStr[3] = {CString(""),CString(""), CString("")};
                  nStrLen = strParam.GetLength();
                  bool isVal = false;
                  int nPosBlank = 0;
                  int Index = 0;
                  CString strToken;
                  st = 1;
                  for (int i = 0; i < nStrLen; i++)
                  {
                    if (strParam[i] == '\"' && !st)
                      st = 1;
                    if (strParam[i] == '\"')
                      st = 0;
                    
                    if (strParam[i] == ' ' && st)
                    {
                      strToken.TrimRight();
                      if (!strToken.IsEmpty())
                      {                        
                        if ((strToken.CompareNoCase("Val") == 0)||(strToken.CompareNoCase("Знач") == 0))
                        {                          
                          isVal = true;
                          bExistValPar = true;
                        }
                        else if (Index < 2)
                        {
                          masStr[Index++] = strToken;
                        }                        
                        strToken.Empty();
                      }                        
                      continue;
                    }
                    strToken += strParam[i];
                  }
                  if (!strToken.IsEmpty())
                  {
                    masStr[Index++] = strToken;
                  }                                    
                  int nPosEq = masStr[1].Find("=");
                  if (nPosEq != -1)
                  {
                    CString strTemp     = masStr[1].Mid(0, nPosEq);
                    CString strParamDef = masStr[1].Mid(nPosEq+1);                    
                    strTemp.TrimLeft();
                    strTemp.TrimRight();
                    strParamDef.TrimLeft();
                    strParamDef.TrimRight();
                    masStr[1] = strTemp;
                    masStr[2] = strParamDef;
                  }
                  pArr->Add(CMethodsParamData::Create(masStr[1], masStr[0], masStr[2], isVal));
                }                                                
              }

              if ((bExistValPar || bExistUnLimits) && !NameOfClass.IsEmpty() && !strNameOfMeth.IsEmpty())
              { 
                CString strNameOfClassAndMeth = NameOfClass;

                strNameOfClassAndMeth += ":";
                strNameOfClassAndMeth += strNameOfMeth;
                if (bExistValPar)
                {
                  // Запишем название класса и название метода для ускорения поиска тех методов
                  // классов, в которых есть параметры передаваемые по значению, их нам необходимо
                  // скопировать перед вызовом, а потом восстановить.                  
                  //m_ValParamInMeth[strNameOfClassAndMeth] = pArr;
					AddMethHaveParVal(strNameOfClassAndMeth, pArr);
                }
                if (bExistUnLimits)
                {
                  // Запишем атрибуты для метода, который содержит неопределенное количество
                  // параметров (...)                  
                  m_UnlimitsParams[strNameOfClassAndMeth] = pArr;
                }
              }                              
            }
          }
        }
        else
        {
          MessageBox(NULL, DefClass, "Ошибка: неверно определен класс", MB_ICONERROR);
          break;
        }
      }
      else
      {
        token.Empty();
        continue;
      }

    }
  }  
}

void CMetaDataOfClasses::RemoveNeedlessBlank(CString &str)
{ 
  str.TrimLeft(); str.TrimRight();
  int nStrLen = str.GetLength();  
  int nFindBlank = str.Find(" ");
  int st = 0;
  for (int j = 0; j < nStrLen; j++)
  {
    int nCountBlank = 0;
    
    if (str[j] == '\"' && st == 0)
      st = 1;
    else if (str[j] == '\"' && st == 1)
      st = 0;

    if (str[j] == ' ' && st == 0)
    {
      for (int i = j + 1; i < nStrLen; i++)
      {
        if (str[i] == ' ')
        {
          nCountBlank++;          
        }
        else
          break;
      }
      if (nCountBlank > 0)
      {
        str.Delete(j + 1, nCountBlank);
      }
      nStrLen = str.GetLength();      
    }
  }
}

bool CMetaDataOfClasses::ThisIsUnLimMethod(const CString& strName) const
{
	CParamDefs::CArrayOfMethodsParamData_PTR param;  
	if (m_UnlimitsParams.Lookup(strName, param))
		return true;
	else
		return false;
}

bool CMetaDataOfClasses::LookupMethHaveParVal(const CString& strName, CParamDefs::CArrayOfMethodsParamData_PTR& param) const
{
	if (m_ValParamInMeth.Lookup(strName, param))
		return true;
	else
		return false;
}

void CMetaDataOfClasses::AddMethHaveParVal(const CString& strNameOfClassAndMeth, CParamDefs::CArrayOfMethodsParamData_PTR param)
{
	m_ValParamInMeth[strNameOfClassAndMeth] = param;
}

bool CMetaDataOfClasses::IsClassExist(const char *pcstrName) const
{
  CString strResult;
  return m_ClassBindFile.Lookup(pcstrName, strResult) != 0;
}

int CMetaDataOfClasses::GetCountClasses() const
{
  return m_ClassBindFile.GetCount();
}

int CMetaDataOfClasses::GetCountMethods() const
{
  CString key;
  POSITION pos;
  int res = 0;
  for (pos = m_MapClassNamesToMethodsMap.GetStartPosition(); pos != NULL;)
  {
    CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
    CString key;
    m_MapClassNamesToMethodsMap.GetNextAssoc( pos, key, pMapFunc);
    res += pMapFunc->GetCount();
    
  }  
  return res;
}

CString CMetaDataOfClasses::GetAllClassesNames(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_ClassBindFile.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_ClassBindFile.GetNextAssoc( pos, key, val);    
    res += key;
    res += " = ";
    res += val;
    res += sep;        
  }  
  return res;
}

CString CMetaDataOfClasses::GetAllAliasAndPath(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_AliasOfPaths.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_AliasOfPaths.GetNextAssoc( pos, key, val);    
    res += key;
    res += " = ";
    res += val;
    res += sep;
  }  
  return res;
}

CString CMetaDataOfClasses::GetBaseClasses(const char* strNameCl, const char* sep) const
{
  CString str;
  CStringList_SharedPtr pList;
  m_ClassesHierarchyMap.Lookup(strNameCl, pList);
  if (pList)
  {
    for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
    {
      str += pList->GetNext( posList );
      str += sep;
    }    
  }
  return str;
}

CString CMetaDataOfClasses::GetNamesMethOfClass(const char *pNameClass, const char *sep) const
{
  CString res;
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if (m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
    if (pMapFunc)
    {
      for (POSITION pos = pMapFunc->GetStartPosition(); pos != NULL;)
      {
        CParamDefs::CArrayOfMethodsParamData_PTR param; // = NULL;
        CString key;
        pMapFunc->GetNextAssoc( pos, key, param);
        res += key;
        res += sep;
      }
    }
    return res;
}

CParamDefs::CArrayOfMethodsParamData_PTR CMetaDataOfClasses::GetInfoParamMeth(const char *pNameClass, const char *pNameMeth) const
{  
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if (m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
    if (pMapFunc)
    {
      CParamDefs::CArrayOfMethodsParamData_PTR arr; // = NULL;
      if (pMapFunc->Lookup(pNameMeth, arr))
        return arr;      
    }
    return CParamDefs::CArrayOfMethodsParamData_PTR(); //NULL;
}

bool CMetaDataOfClasses::IsMethExist(const char *pNameClass, const char *pNameMeth) const
{
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if(m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
  {
    CParamDefs::CArrayOfMethodsParamData_PTR arr; // = NULL;
    if (pMapFunc->Lookup(pNameMeth, arr))
      return true;
  }
  return false;
}

// чтобы название класса для ТипЗначенияСтр(КОП) писалось не как пользователь напишет в СоздатьОбъект(ТипОбъекта),
// а бралось из файла описания - artbear
// используется в CComponentClass::InitObject - т.е. когда в 1С пишем СоздатьОбъект(ТипОбъекта)
//
CString CMetaDataOfClasses::GetClassNameFromDefinition(const CString& ClassName) const
{
	LPSTR strResult;
	m_ClassBindFile.LookupKey(ClassName, strResult);
	
	return strResult;
}

CString CMetaDataOfClasses::GetPathFromAlias(const CString& strAls) const
{
  //return m_AliasOfPaths[strAls];
	CString Result;
	m_AliasOfPaths.Lookup(strAls, Result);
	return Result;
}

// artbear для проверки рекурсии
// 1 - рекурсия, 0 - нет
bool CMetaDataOfClasses::VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy) const
{
//   CStringList* pList = (CStringList*)m_ClassesHierarchyMap[BaseClassName];
//   if (pList)
  CStringList_SharedPtr pList;
  if (m_ClassesHierarchyMap.Lookup(BaseClassName, pList) && pList)
  {
	sRecursiveHierarchy = BaseClassName+ " - "+sRecursiveHierarchy;
    for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
    {
		CString strBaseClassName = pList->GetNext( posList );

		if (!ClassName.CompareNoCase(strBaseClassName))
		{
			sRecursiveHierarchy = strBaseClassName + " - "+sRecursiveHierarchy;
			return true;
		}

		bool bHaveRecursive = VerifyRecursiveInBaseClasses(ClassName, strBaseClassName, sRecursiveHierarchy);	

		if (bHaveRecursive)
			return true;

    }
  }
  return false;
}

CString CMetaDataOfClasses::GetNameOfFile(const CString &strNameOfMod) const
{
	CString Result;
	m_ClassBindFile.Lookup(strNameOfMod, Result); //return m_ClassBindFile[strNameOfMod];
	return Result;
}

bool CMetaDataOfClasses::TheClassIsDerive(const CString& derive, const CString& base) const
{
  //CStringList *pStrList = (CStringList*)(const_cast<CIStringMap<CStringList*, CStringList*>*>(&m_Meta.m_ClassesHierarchyMap)->operator[](derive));
  CStringList_SharedPtr pStrList;
  if (m_ClassesHierarchyMap.Lookup(derive, pStrList) && pStrList)
  {
    //CString str;
    for(POSITION posList = pStrList->GetHeadPosition(); posList != NULL; )
    {
		if (IsObject(pStrList->GetNext( posList ), base))
			return true;
    }
    
  }
  return false;
}

bool CMetaDataOfClasses::IsObject(const CString& sClassName, const CString& sTryName) const
{
	if (!sClassName.CompareNoCase(sTryName))
		return true;
	else
		return TheClassIsDerive(sClassName, sTryName);
}  
