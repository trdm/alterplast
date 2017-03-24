// yccconv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static const char* shablons[]={
	"\tDECLARE_TERMINALS\r\n",
	"\tEND_TERMINALS\r\n",
	"\tDECLARE_NON_TERMINALS\r\n",
	"\tEND_NON_TERMINALS\r\n",
	"\t\t%-20s, // %i\r\n",				//name
	"\tDECLARE_GRAMMAR(%i,%i)\r\n",		//iNumOfStates,iNumOfRules
	"IMPLEMENT_ACTIONS(%s)\r\n",		//ClassName
	"\tBEGIN_STATE\t// %i\r\n",			//state
	"\tEND_STATE\t// error\r\n",
	"\t\tSHT(%i)\t\t// %s\r\n",			//par term
	"\t\tRDC(%i)\t\t// %s\r\n",			//par term
	"\t\tACC \t\t// %s\r\n",			//term
	"\t\tERR \t\t// %s\r\n",			//term
	"END_IMPLEMENT_ACTIONS\r\n\r\n",
	"IMPLEMENT_GOTO(%s)\r\n",			//ClassName
	"\tBEGIN_GOTO\t// %i\r\n",			//state
	"\tEND_GOTO\r\n",
	"\t\tGOTO(%i)\t//%s\r\n"	,		//par nonterm
	"END_IMPLEMENT_GOTO\r\n\r\n",
	"IMPLEMENT_RULE(%s)\r\n",			//ClassName
	"\tRULE(%i,\t%s)\t// %i\r\n",		//iLen,iNonTerm
	"END_IMPLEMENT_RULE\r\n\r\n"
};
enum Shablons{
	declTerminal=0,
	endTerminal,
	declNonTerm,
	endNonTerm,
	name,
	declGrammar,
	implAction,
	beginState,
	endState,
	shift,
	reduce,
	Acc,
	error,
	endActions,
	implGoto,
	beginGoto,
	endGoto,
	_goto,
	endImplGoto,
	implRule,
	rule,
	endRule
};

int main(int argc, char* argv[])
{
	HANDLE hH,hC,hI;
	hI=CreateFile("y.output",GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hI==HANDLE(-1))
		return 0;
	DWORD inputSize=GetFileSize(hI,NULL);
	char *pInputBuf=new char[inputSize+1];
	LPCTSTR pRead=pInputBuf,pMem;
	DWORD d;
	ReadFile(hI,pInputBuf,inputSize,&d,NULL);
	pInputBuf[inputSize]=0;
	CloseHandle(hI);

	char buf[MAX_PATH];
	strcpy(buf,argv[1]);
	strcat(buf,".h");
	hH=CreateFile(buf,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hH==HANDLE(-1))
		return 0;
	strcpy(buf,argv[1]);
	strcat(buf,".cpp");
	hC=CreateFile(buf,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hC==HANDLE(-1))
		return 0;
	// Read rule, make map of terminal and non terminal, IMPLEMENT_RULE
	int CountOfTerminal=0,CountOfNonTerminal=0,countStates=0,countRules=0;
	typedef map<string,int> strmap;
	typedef strmap::iterator strit;
	char LastNonTerm[100];

	strmap AllNames,NonTerminal,Terminal;
	sprintf(buf,shablons[implRule],argv[1]);
	WriteFile(hC,buf,strlen(buf),&d,NULL);
	while(*pRead!=0x0C)
	{
		while(*pRead<'A')
			*pRead++;
		if(*pRead!='|')
		{
			pMem=pRead;
			while(*pRead>' ')
				pRead++;
			strncpy(LastNonTerm,pMem,pRead-pMem);
			LastNonTerm[pRead-pMem]=0;
			char *pBuf=new char[pRead-pMem+1];
			strcpy(pBuf,LastNonTerm);
			string str=pBuf;
			NonTerminal[str]=CountOfNonTerminal++;
		}
		else
			pRead++;
		int CountWord=0;
		if(!countRules)
		{
			while(*++pRead!='\r');
			CountWord=1;
		}
		else
		{
			if(*pRead!='\r')
			{
				while(*pRead<'A')
					pRead++;
			}
		}
		while(*pRead!='\r')
		{
			pMem=pRead;
			while(*pRead>' ')
				pRead++;
			strncpy(buf,pMem,pRead-pMem);
			buf[pRead-pMem]=0;
			string str=buf;
			strit it=AllNames.find(str);
			if(it==AllNames.end())
			{
				char *p=new char[pRead-pMem+1];
				strcpy(p,buf);
				string str=p;
				AllNames[str]=1;
			}
			CountWord++;
			while(*pRead<'A' && *pRead!='\r')
				pRead++;

		}
		sprintf(buf,shablons[rule],CountWord,LastNonTerm,countRules++);
		WriteFile(hC,buf,strlen(buf),&d,NULL);
		pRead+=2;
		if(*pRead=='\r')
			pRead+=2;
	}
	WriteFile(hC,shablons[endRule],strlen(shablons[endRule]),&d,NULL);
	WriteFile(hH,shablons[declTerminal],strlen(shablons[declTerminal]),&d,NULL);
	strit it1,it2;
	int iNumber=1;
	for(it1=AllNames.begin();it1!=AllNames.end();it1++)
	{
		it2=NonTerminal.find(it1->first);
		if(it2==NonTerminal.end())
		{
			Terminal[it1->first]=0;
			sprintf(buf,shablons[name],it1->first.c_str(),iNumber);
			iNumber++;
			WriteFile(hH,buf,strlen(buf),&d,NULL);
		}
	}
	WriteFile(hH,shablons[endTerminal],strlen(shablons[endTerminal]),&d,NULL);
	WriteFile(hH,shablons[declNonTerm],strlen(shablons[declNonTerm]),&d,NULL);
	NonTerminal.erase("accept");
	iNumber=-1;
	for(it1=NonTerminal.begin();it1!=NonTerminal.end();it1++)
	{
		sprintf(buf,shablons[name],it1->first.c_str(),iNumber);
		iNumber--;
		WriteFile(hH,buf,strlen(buf),&d,NULL);
	}
	WriteFile(hH,shablons[endNonTerm],strlen(shablons[endNonTerm]),&d,NULL);

	pRead+=3;
	LPCTSTR pStates=pRead;
	Terminal["__end"]=0;
	sprintf(buf,shablons[implAction],argv[1]);
	WriteFile(hC,buf,strlen(buf),&d,NULL);
	while(pRead=strstr(pRead,"state "))
	{
		for(strit it=Terminal.begin();it!=Terminal.end();it++)
			it->second=0;
		pRead=strstr(pRead,"\r\n\r\n")+4;
		while(*pRead!='\r')
		{
			bool all=false;
			while(!(*pRead>='A' || *pRead=='$' || *pRead=='.'))
				pRead++;
			if(*pRead=='$')
			{
				strcpy(buf,"__end");
				pRead+=4;
			}
			else if(*pRead!='.')
			{
				pMem=pRead;
				while(*pRead>' ')pRead++;
				strncpy(buf,pMem,pRead-pMem);
				buf[pRead-pMem]=0;
			}
			else
			{
				all=true;
				pRead++;
			}
			while(*pRead<=' ')pRead++;
			int action=0,iNum=0;
			if(*pRead=='s')
				action=1;
			else if(*pRead=='r')
				action=2;
			else if(*pRead=='a')
				action=4;
			while(*pRead>='A')pRead++;
			
			if(action==1 || action==2)
			{
				while(*pRead<'0')pRead++;
				while(*pRead>='0')
					iNum=iNum*10+*pRead++-'0';
			}
			if(action)
			{
				if(!all)
					Terminal[buf]=(action<<16)|iNum;
				else
				{
					for(it=Terminal.begin();it!=Terminal.end();it++)
					{
						if(!it->second)
							it->second=(action<<16)|iNum;
					}
				}
			}
			pRead+=2;
		}
		sprintf(buf,shablons[beginState],countStates++);
		WriteFile(hC,buf,strlen(buf),&d,NULL);
		for(it=Terminal.begin();it!=Terminal.end();it++)
		{
			int action=it->second>>16;
			int iNum=it->second & 0xFFFF;
			switch(action){
			case 0:
				sprintf(buf,shablons[error],it->first.c_str());
				break;
			case 1:
				sprintf(buf,shablons[shift],iNum,it->first.c_str());
				break;
			case 2:
				sprintf(buf,shablons[reduce],iNum,it->first.c_str());
				break;
			case 4:
				sprintf(buf,shablons[Acc],it->first.c_str());
			}
			WriteFile(hC,buf,strlen(buf),&d,NULL);
		}
		WriteFile(hC,shablons[endState],strlen(shablons[endState]),&d,NULL);
	}
	WriteFile(hC,shablons[endActions],strlen(shablons[endActions]),&d,NULL);
	sprintf(buf,shablons[declGrammar],countStates,countRules);
	WriteFile(hH,buf,strlen(buf),&d,NULL);
	pRead=pStates;
	countStates=0;

	sprintf(buf,shablons[implGoto],argv[1]);
	WriteFile(hC,buf,strlen(buf),&d,NULL);
	while(pRead=strstr(pRead,"state "))
	{
		pRead=strstr(pRead,"\r\n\r\n")+4;
		pRead=strstr(pRead,"\r\n\r\n")+4;
		for(strit it=NonTerminal.begin();it!=NonTerminal.end();it++)
			it->second=0;
		while(*pRead!='\r')
		{
			while(*pRead<'A')pRead++;
			pMem=pRead;
			while(*pRead>' ')pRead++;
			strncpy(buf,pMem,pRead-pMem);
			buf[pRead-pMem]=0;
			int iNum=0;
			while(*pRead<'0' || *pRead>'9')pRead++;
			while(*pRead>='0')
				iNum=iNum*10+*pRead++-'0';
			NonTerminal[buf]=iNum;
			pRead+=2;
		}
		sprintf(buf,shablons[beginGoto],countStates++);
		WriteFile(hC,buf,strlen(buf),&d,NULL);
		for(it=NonTerminal.begin();it!=NonTerminal.end();it++)
		{
			sprintf(buf,shablons[_goto],it->second,it->first.c_str());
			WriteFile(hC,buf,strlen(buf),&d,NULL);
		}
		WriteFile(hC,shablons[endGoto],strlen(shablons[endGoto]),&d,NULL);
	}
	WriteFile(hC,shablons[endImplGoto],strlen(shablons[endImplGoto]),&d,NULL);
	CloseHandle(hH);
	CloseHandle(hC);
	return 0;
}
