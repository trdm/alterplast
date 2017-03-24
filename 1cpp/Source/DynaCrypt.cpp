// DynaCrypt.cpp: implementation of the CDynaCrypt class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "addin.h"
#include "dynacrypt.h"
#include "RC6.h"
#include "dml.h"

#undef DEBUG_THIS

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

//#define DEBUG_THIS

#ifdef DEBUG_THIS
	static char dbg[512];
	#define DBG OutputDebugString(dbg)
	#define DBG_DYNACRYPT_CONSTRUCT
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDynaCrypt, CBLContext);


static unsigned char num2str[512];
static int num2strI = 0;

class CObjID CDynaCrypt::ObjID;

enum {
methEncryptString,
methDecryptString,
lastMethod
};

struct CDynaCrypt::st_paramdefs CDynaCrypt::defFnNames[] = {	
	{{"EncryptString","ЗашифроватьСтроку"}, 1, 2},
	{{"DecryptString","ДешифроватьСтроку"}, 1, 2},	
	};

CDynaCrypt::CDynaCrypt():CBLContext(1)
{ Init(); }

CDynaCrypt::CDynaCrypt(int param):CBLContext(param)
{ Init(); }

void CDynaCrypt::Init()
{
	cryptBuff = 0;
	cbSize = 0;
}



CDynaCrypt::~CDynaCrypt()
{

	if(cryptBuff)
	{
		free(cryptBuff);
		cryptBuff=0;cbSize=0;
	}
}

void CDynaCrypt::AllocBuffer(int size)
{
	if(cbSize < size)
	{
		if(cryptBuff)
		{
			free(cryptBuff);
		}
		if(cryptBuff = (char*)malloc(size))
		{
			cbSize=size;
		}
		else
		{
			cbSize=0;
			pBkEndUI->DoMessageLine("Not enough memory for encryption buffer!",mmRedErr);
		}
	}
}

void CDynaCrypt::EncryptString(const class CString &str, const class CString &key, CString &rValue)
{	
	int kl,sl,dl;
	unsigned char *ps,*pt,*p;
	char *target;
	unsigned long *crc;

	if((sl = str.GetLength())==0)
	{
		rValue.Empty();
	}
	else
	{		
		if(!key.IsEmpty())
		{
			dl=+sl*2
				+31//rc6 padding
				+1//.
				+16//crc
				+1//0
				+256//на всякий случай
				;
			AllocBuffer(sl+dl);
			if(cryptBuff)
			{
				//crypting source string
				memcpy(cryptBuff,str,sl);
				memset(cryptBuff+sl,0,15);
				kl = sl>>4;
				if(sl&0xf) kl++;
				sl = (kl<<4);
				{
					//encryption
					RC6Encryption *enc = new RC6Encryption((unsigned char*)key.operator LPCTSTR(),key.GetLength(),20);
					ps = (unsigned char*)cryptBuff;
					while(kl--)
					{
						enc->ProcessBlock(ps);
						ps += 16;
					}
					crc = (unsigned long*)ps;
					*crc = sl;
					adler32((unsigned char*)cryptBuff,sl,crc);
					sl += 4;
					delete enc;
				}
				//target string
				if(num2strI == 0)
				{
					pt=num2str;
					for(int i=0;i<256;i++)
					{
						sprintf((char*)pt,"%02X",i);
						pt += 2;
					}
					num2strI++;
				}
				ps = (unsigned char*)cryptBuff;
				pt = ps+sl;
				target = (char*)pt;
				while(sl--)
				{
					p = num2str + (*ps <<1);
					ps++;
					*pt++ = *p++;
					*pt++ = *p;

				}
				*pt=0;
				{
					rValue = target;
				}
			}
			else
				rValue.Empty();
		}
		else
		{
			pBkEndUI->DoMessageLine("CDynaCrypt::EncryptString() enctyption key not specified!",mmRedErr);
			rValue.Empty();
		}
	}
}

int CDynaCrypt::DecryptString(const CString& strSource, const CString& key, CString& strDest)
{
	const char *str;
	int i,sl;
	unsigned char *ps,*pt,c;
	unsigned long adler;  
	str = strSource;
	sl = strSource.GetLength();
	if(sl)
	{			
		ps = (unsigned char*)str;			
		
			if(!key.IsEmpty())
			{					
				if(sl)
				{
					AllocBuffer(sl);
					if(pt = (unsigned char*)cryptBuff)
					{
            sl = sl>>1;
						i=sl;
						while(i--)
						{
							c=*ps++;
							if(c<='F')
							{
								if(c>='A')
								{
									c -= ('A'-10);
								}
								else
								{
									c -= '0';
									if(c>10)
										return -2;//invalid string
								}
							}
							else 
                return -2;//invalid string
							*pt = c<<4;
							c=*ps++;
							if(c<='F')
							{
								if(c>='A')
								{
									c -= ('A'-10);
								}
								else
								{
									c -= '0';
									if(c>10)
										return -2;//invalid string
								}
							}
							else 
                return -2;//invalid string
							*pt++ |= c;
						}
						//adler32 check
						i = sl - 4;
						adler = i;
						adler32((unsigned char*)cryptBuff,i,&adler);
						if(adler == *((unsigned long*)(cryptBuff+i)))
						{
							//decrypt string
							*((unsigned int*)(cryptBuff+i)) = 0;
							i = sl>>4;
							pt = (unsigned char*)cryptBuff;
							{
								RC6Decryption *dec = new RC6Decryption((unsigned char*)key.operator LPCTSTR(),key.GetLength(),20);
								while(i--)
								{
									dec->ProcessBlock(pt);
									pt += 16;
								}
								delete dec;
							}
							strDest = cryptBuff;
							return 1;//ok
						}
						else
						{
							return -3;//crc error
						}
					}
					else
						return -998;//out of mem						
        }
        else
          return -2;					
			}
			else
			{
				return 0;//key not found
			}
		
	}
	else
		return -1;//empty string
}

int  CDynaCrypt::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	int rc=1;

	try
	{
		switch (iMethNum)
		{
	
		case methEncryptString:
			{
        CString strDest;
				EncryptString(ppValue[0]->GetString(),ppValue[1]->GetString(), strDest);
        rValue = strDest;
				break;
			}

		case methDecryptString:
			{
				CString strDest;
        int res = DecryptString(ppValue[0]->GetString(), ppValue[1]->GetString(), strDest);
        
        if (res == 1)
          *ppValue[0] = strDest;

        rValue = res;

				break;
			}
		default:
			rc = -1;
		}
	}
	catch(...)
	{
		//pBkEndUI->DoMessageLine("Error: exception",mmRedErr);
		rc = 0;
	};
	return rc;
}

int  CDynaCrypt::FindMethod(char const * lpMethodName)const
{
	int i;

	
	for (i = 0;i<lastMethod;i++)
	{
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}

	return -1;
}

char const *  CDynaCrypt::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CDynaCrypt::GetNMethods(void)const
{
	return(lastMethod);
}

int  CDynaCrypt::HasRetVal(int iMethodNum)const
{
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CDynaCrypt::GetNParams(int iMethodNum)const
{
	return defFnNames[iMethodNum].NumberOfParams;
}

int  CDynaCrypt::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	return 0;
}


int  CDynaCrypt::CallAsProc(int iMethNum,class CValue * * ppValue)
{
  return -1;
}

void  CDynaCrypt::DecrRef(void)
{
	CBLContext::DecrRef();
}


int  CDynaCrypt::FindProp(char const *propname)const
{
	return 0;
}

char const *  CDynaCrypt::GetCode(void)const
{
	return 0;
}

int  CDynaCrypt::GetDestroyUnRefd(void)const
{
	return CBLContext::GetDestroyUnRefd();
}

void  CDynaCrypt::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CDynaCrypt::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CDynaCrypt::GetInternalData(void)
{
	return NULL;

}



int  CDynaCrypt::GetNProps(void)const
{
	return 0;
}

char const *  CDynaCrypt::GetPropName(int,int)const
{
	return NULL;
}

int  CDynaCrypt::GetPropVal(int i,class CValue &rValue)const
{
	return NULL;
}

long  CDynaCrypt::GetTypeID(void)const
{
	return 100;
}

char const *  CDynaCrypt::GetTypeString(void)const
{
	return "DynaCrypt";
}

class CType   CDynaCrypt::GetValueType(void)const
{
	return CType(100);
}


void  CDynaCrypt::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CDynaCrypt::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CDynaCrypt::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CDynaCrypt::IsExactValue(void)const
{
	return 0;
}

int  CDynaCrypt::IsOleContext(void)const
{
	return 0;
}

int  CDynaCrypt::IsPropReadable(int iPropNum)const
{
	return 0;
}

int  CDynaCrypt::IsPropWritable(int iPropNum)const
{
	return 0;
}

int  CDynaCrypt::IsSerializable(void)
{
	return 0;
}

int  CDynaCrypt::SaveToString(class CString & csStr)
{	
	return 1;
}

void  CDynaCrypt::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}

int  CDynaCrypt::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return 0;
}