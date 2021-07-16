#include "stdafx.h"
#include "spell.h"

SpellPatrimony::SpellPatrimony(int c){
	switch(c){
		case (char)(int)patrFem:
		case '0'+patrFem:
		case '�':
		case '�':
		case 'f':
		case 'F':
			patr=patrFem;
			return;

		case (char)(int)patrInd:
		case '0'+patrInd:
		case '�':
		case '�':
		case 'i':
		case 'I':
			patr=patrInd;
			return;
		default:
			patr=patrMan;
			return;
	}
}

SpellPatrimony::operator int(){
	return patr;
}

SpellPatrimony::SpellPatrimony(){
	patr=0;
}

//////////////////////////////////////////////////////////////////////////
CString spell_string::operator =(CString tstr){
	str=tstr;
	return str;
}

CString spell_string::operator =(LPCTSTR tstr){
	str=tstr;
	return str;
}

spell_string&  spell_string::operator =(spell_string &tst){
	str=tst.str;
	return *this;
}


CString& spell_string::operator[](int){
	return str;
}

spell_string::spell_string(CString tstr){
	str=tstr;
}

spell_string::spell_string(){
	str.Empty();
}

spell_string::spell_string(spell_string &tst){
	str=tst.str;
}

CString& spell_string::operator[](double){
	return str;
}

SpellPatrimony spell_string::operator =(int s_p){
return s_p;
}

spell_string::operator SpellPatrimony (){
	return patrMan;
}

//////////////////////////////////////////////////////////////////////////
spell_unit::spell_unit() : spell_string(){
	unit2.Empty();
	unit5.Empty();
	patr=0;
}

spell_unit::spell_unit(CString s1,int p) : spell_string(s1){
	unit2=s1;
	unit5=s1;
	patr=p;
}

spell_unit::spell_unit(CString s1,CString s2,CString s5,int p) : spell_string(s1){
	unit2=s2;
	unit5=s5;
	patr=p;
}

spell_unit::spell_unit(spell_unit& unit){
	str=unit.str;
	unit2=unit.unit2;
	unit5=unit.unit5;
	patr=unit.patr;
}

SpellPatrimony spell_unit::operator=(int p){
	patr=p;
	return patr;
}

CString& spell_unit::operator [](int n){
	if (0>n)n=-n;
	n%=100;
	if(11<=n && n<=20)return unit5;
	switch(n%10){
		case 1:
			return str;
		case 2:case 3:case 4:
			return unit2;
		default:
			return unit5;
	}
}

CString& spell_unit::operator [](double d){
	if (0.0 > d)d=-d;
	int n=(int)fmod(d,100.0);
	if(11<=n && n<=20)return unit5;
	switch(n%10){
		case 1:
			return str;
		case 2:case 3:case 4:
			return unit2;
		default:
			return unit5;
	}
}

//////////////////////////////////////////////////////////////////////////
spell_patr::spell_patr():spell_string(){
	sFem.Empty();
	sInd.Empty();
}

spell_patr::spell_patr(CString ts):spell_string(ts){
	sFem=ts;
	sInd=ts;
}



spell_patr::spell_patr(spell_patr &pt){
	str=pt.str;
	sFem=pt.sFem;
	sInd=pt.sInd;
}


spell_patr::spell_patr(CString tMan,CString tFem,CString tInd) : spell_string(tMan){
	sFem=tFem;
	sInd=tInd;
}
CString& spell_patr::operator[] (int n){
	switch(n) {
	case patrFem:
		return sFem;
	case patrInd:
		return sInd;
	default:
		return str;
	}
}

CString& spell_patr::operator[] (double d){
	return operator[] ((int) d);
}

//////////////////////////////////////////////////////////////////////////

spell_string &spell_num::operator [](int n){
	if(n<0)n=-n;
	if(!n)return num0;
	if(999<n){
		if(999999<n){
			if(999999999<n)return numexp[3];
			else return numexp[2];
		}
		else return numexp[1];
	}
	else{
		if(100<=n)return num100_900[n/100-1];
		if(20<=n)return num100_900[n/10-2];
		if(2<n)return num3_19[n-3];
		return num1_2[n-1];
	}
}

spell_string &spell_num::operator [](double d){
	if(d<0)d=-d;
	d=floor(d);
	if(1e6 <= d ){
		if(1e12 <= d)return numexp[4];
		else return numexp[3];
	}
	else return operator [](int(d));
}


int spell_num::Decode999(CString &rez,int arg,int patr){
	if(!arg)return 0;

	ASSERT(0<arg && arg<1000);
	if(!rez.IsEmpty())rez+=' ';
	if(99< arg){
		rez+=(CString&)num100_900[arg/100-1];
		arg%=100;
		if(!arg)return 1;
		rez+=' ';
	}

	if(19<arg){
		rez+=(CString&)num20_90[arg/10-2];
		arg%=10;
		if(!arg)return 1;
		rez+=' ';
	}
	if(2<arg)
		rez+=(CString&)num3_19[arg-3];
	else 
		rez+=num1_2[arg-1][patr];
	return 1;
}

CString spell_num::operator ()(int n,int patr){
	CString rez;
	//3	2/6	1/3	0/0
	//1 000 000 000
	int triad[4]={0}
		,t_patr[4]={0};
	if(!n) return num0;
	if( n<0 ){
		n=-n;
		rez=minus;
	}

	int i;
	
	for(i=0;n;i++){
		triad[i]=n%1000;
		t_patr[i]=numexp[i].patr;
		n/=1000;
	}

	t_patr[0]=patr;
	for(i--;0<=i;i--){
		if( Decode999(rez,triad[i],t_patr[i]) )
			if(i){
				CString tr(numexp[i][triad[i]]);
				if(!tr.IsEmpty()){
					rez+=' ';
					rez+=tr;
				}
				
			
			}
			
	}
	return rez;
}

CString spell_num::operator ()(int n,spell_unit &su){
	CString rez;
	//3	2/6	1/3	0/0
	//1 000 000 000
	int triad[4]={0};
	if(!n){
		rez=num0;
		if(su.unit5.IsEmpty())
		{
			rez+=' ';
			rez+=su.unit5;
		}
		return rez;
	}
	if( n<0 ){
		n=-n;
		rez=minus;
	}
	
	spell_unit *spl[4];

	int i;

	for(i=0;n;i++){
		spl[i]=&numexp[i];
		triad[i]=n%1000;
		n/=1000;
	}
	spl[0]=&su;
	for(i--;0<=i;i--){
		if( Decode999(rez,triad[i],spl[i]->patr) ){
			CString &s=(*spl[i])[triad[i]];
			if(!s.IsEmpty()){
				rez+=' ';
				rez+=s;
			}
			
		}
	}
	return rez;
}

CString spell_num::operator ()(double d,spell_unit &su){
	CString rez;
	if( d<0.0 ){
		d=-d;
		rez=minus;
	}
	d=floor(d);
	if(0.0 == d){
		rez=num0;
		if(!su.unit5.IsEmpty()){
			rez+=' ';
			rez+=su.unit5;
		}
		return rez;
	}
	
	int i;
	spell_unit *spl[sizeof(numexp)/sizeof(*numexp)];
	int triad[sizeof(numexp)/sizeof(*numexp)]={0};
	for(i=0;d && i<sizeof(numexp)/sizeof(*numexp);i++){
		double ost = fmod(d,1000.0);
		triad[i]=ost;
		d-=ost;
		spl[i]=&numexp[i];
		d /= 1000.0;
	}
	spl[0]=&su;
	for(i--;0<=i;i--){
		if( Decode999(rez,triad[i],spl[i]->patr) ){
			CString &descript=(*spl[i])[triad[i]];
			if(!descript.IsEmpty()){
				rez+=' ';
				rez+=descript;
			}
		}
		
	}
	return rez;
}

CString spell_num::operator() (double d,int patr){
	spell_unit tsu("",patr);
	return (*this)(d,tsu);
}


CString spell_num::operator ()(double d,spell_unit &su,spell_unit &ku){
	int fract;
	double t_d;
	if( d<0.0 ){
		t_d=ceil(d);
		if((d*100.0-int(d*100.0))>=0.5)
			fract=(d-int(d))*100.0+0.5;
		else
			fract=(d-int(d))*100.0+1;
	}
	else {
		t_d=floor(d);
		
		if((d*100.0-int(d*100.0))>=0.5)
			fract=(d-int(d))*100.0+1;
		else
			fract=(d-int(d))*100.0+0.5;
	}
	CString rez;
	rez.Format("%s %02d %s",operator ()(t_d,su),fract,CString(ku[fract]) );
	return rez;

}


void spell_num::SetSpell(LPCTSTR alist[]){

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(*arr))
#define GET_STR(arg) do {\
	if(! *(list) )return;\
	arg=*list;\
	list++;\
	} while(0);
	
	int i;
	i=-1;
	LPCTSTR *list=alist;
	GET_STR(num0);
	GET_STR(num1_2[0][patrMan]);GET_STR(num1_2[0][patrFem]);GET_STR(num1_2[0][patrInd]);
	GET_STR(num1_2[1][patrMan]);GET_STR(num1_2[1][patrFem]);GET_STR(num1_2[1][patrInd]);
	for(i=0;i<ARRAY_SIZE(num3_19);i++)
		GET_STR(num3_19[i]);
	

	for(i=0; i<ARRAY_SIZE(num20_90) ;i++)
	{
		GET_STR(num20_90[i]);
	}
	
	for(i=0;i<ARRAY_SIZE(num100_900);i++)
		GET_STR(num100_900[i]);
	
	for(i=1;i<ARRAY_SIZE(numexp);i++){ //1-� ����� - �� ������
		GET_STR(numexp[i][1]);
		GET_STR(numexp[i][2]);
		GET_STR(numexp[i][5]);
		LPCTSTR cc;
		GET_STR(cc);
		numexp[i].patr=int(*cc);
	}
	GET_STR(minus);
	GET_STR(plus);
#undef GET_STR
#undef ARRAY_SIZE
}

spell_num::spell_num(){
	SetDefaultSpell(*this);
}

void spell_num::SetDefaultSpell(spell_num&sn){
	static LPCTSTR def_spells[]={
		"����"
		,"����","����","����"
		,"���","���","���"
		,"���","������","����","�����","����","������","������"
		,"������","�����������","����������","����������","������������"
		,"����������","�����������","����������","������������","������������"
		,"��������","��������","�����","���������","����������","��������"
		,"�����������","���������"
		,"���","������","������","���������"
		,"�������","��������","�������","���������","���������"
		,"������","������","�����","�"
		,"�������","��������","���������","�"
		,"��������","���������","����������","�"
		,"��������","���������","����������","�"
		,"�����","����"
		,NULL
	};
	sn.SetSpell(def_spells);
}

static CString EmptyString;
CString& spell_date::operator[] (int n){
	n--;
	if(n<0 || 30<n){
		EmptyString.Empty();
		return EmptyString;
	}
	return Months_And_Other[n];
}

CString spell_date::operator() (int n){
	return operator[](n);
}

spell_date::spell_date(){
	LPCTSTR Mnths_[]={
		"������","�������","����","������","���","����","����","������","��������","�������","������","�������"
		,"������","�������","�����","������","���","����","����","�������","��������","�������","������","�������"
		,"�������","���������","9 �������","���","�."};
	int i;
	for(i=0;i< sizeof(Mnths_)/sizeof(LPCTSTR);i++ ){
		Months_And_Other[i]=Mnths_[i];
	}
	LPCTSTR DoW_[]={"�����������","�������","�����","�������","�������","�������","�����������"};
	for(i=0;i<7;i++){
		 DayOfWeek[i]=DoW_[i];
	}
}

static Speller DefSpeller;
static Speller *PDefSpeller=NULL;

Speller& Speller::GetDefaultSpeller(){
	if(PDefSpeller)return *PDefSpeller;
	return DefSpeller;
}

Speller::~Speller(){
	if(PDefSpeller == this)PDefSpeller=NULL;
}

void Speller::SetAsDefaultSpeller(){
	PDefSpeller=this;
}

void Speller::CopyToDefaultSpeller(){
	DefSpeller=*this;
}


Speller::Speller(){
	UnitBig=spell_unit("�����","�����","������",patrMan);
	//UnitBig=spell_unit("������","������","������",patrFem);
	UnitSmall=spell_unit("�������","�������","������",patrFem);
	spell_date DatePart;
	spell_num num_Speller;

}


CString Speller::GetFract(double d,int prec){
	
	if(!prec)return "";
	d-=floor(d);
	int i,j;
	LPTSTR ts;
	if(prec<0){
		ts=fcvt(d,20,&i,&j);
		if('0'==*ts)return "";
		for(LPTSTR tail=ts+19;ts<tail;tail--){
			if('0' == *tail)*tail='\0';	//������� �������� ����
			else break; //�� 1-�� ���������� �������.
		}
		CString rez('0',i);
		rez+=ts;
		return rez;
	}
	
	d+=1;
	ts=fcvt(d,prec,&i,&j);
	CString rez(ts+1);
	return rez;
	
}

CString Speller::FloatFormat(double d,int width,int prec,int leadzero,char zero,char dot,char coma){

	CString rez;
	int t_width=width;
	if(0<width && coma)
		{
		if(prec<0)prec=0;
		if(prec)t_width -= prec+1; //��� ����� � �����
		t_width=width-t_width/4; //��������� ���. ����� �� ����������� ��� �����.
	}


	if(t_width<=0){
		if(0<=prec) 
			rez.Format( "%.*f",prec,d);
		else 
			rez.Format( "%f",d);
	}

	else if(prec<0){
		rez.Format( (leadzero?"%*f":"%0*f"),t_width,d);
	}
	else {
		rez.Format((leadzero?"%0*.*f":"%*.*f"),t_width,prec,d);
	}
	if(zero){
		if(-1 == rez.FindOneOf("123456789")){
			rez.Format("%*c",-width,zero);
			return rez;
		}
	}

	if(coma || dot){
		CString t_str;
		int dot_pos=rez.Find('.');

		if(dot && -1<dot_pos)
			rez.SetAt(dot_pos,dot);
		if(coma){
			int len=rez.GetLength();
			if(-1==dot_pos)
				dot_pos=len;
			LPTSTR buf=new char[dot_pos*4/3+3];
			int i;
			LPTSTR cpos=buf;
			int estl=0;
			for (i=dot_pos-1;i>=0;i--){
				char c=rez[i];
				estl++;
				if(4 == estl){
					estl=1;
					if(' '==c || '-'==c){
						*(cpos++)=c;
						*(cpos++)=' ';
						continue;
					}
					else {
						*(cpos++)=coma;
					}
				}
				*(cpos++)=c;
			}
			int deltabuf=cpos-buf;
			LPTSTR bufrez=rez.GetBuffer(len-dot_pos+deltabuf);
			memmove(&bufrez[deltabuf],&bufrez[dot_pos],len-dot_pos+1);
			i=0;
			for(cpos--;cpos>=buf;cpos--)
				bufrez[i++] = *cpos;
			rez.ReleaseBuffer();
			delete []buf;
		}
	}
	return rez;
}



#define STAT_LOCALE 

#ifdef STAT_LOCALE
//� ������ ������ ����������, 
//��� ����������� �������, �� �������� � ������� ������� (����. ���. �)


inline BOOL isalpha2(char c)
{
   return (	'A'<=c && c<='Z'
         || 'a'<=c && c<='z'
		 || '�'<=c && c<='�'
		 ||	'�'<=c && c<='�');
}



void Speller::FirstCharBig(CString& s){
	LPTSTR bf=s.LockBuffer();
	for(;*bf;bf++){
		if( isalpha2(*bf) ){
			*bf &= ~0x20;
			bf++;
			break;
		}
	}
	_tcslwr(bf); //������� ������ - ����
	s.UnlockBuffer();
}



#else

#include<locale>   //��� ��� isalpha,toupper
//�� ���� ������ �������� ��� �������.
locale loc_rus (".1251");

void Speller::FirstCharBig(CString& s){
	LPTSTR bf=s.LockBuffer();
	for(;*bf;bf++){
		if(isalpha(*bf,loc_rus)){
			*bf=toupper(*bf,loc_rus);
			for(bf++;*bf;bf++){
				if(isalpha(*bf,loc_rus))
					*bf=tolower(*bf,loc_rus);
			}
			break;
		}
	}
	s.UnlockBuffer();
}

#endif
 

/*
inline BOOL isalpha2(char c)
{
	if(
		(c>='A'&&c<='Z')||
		(c>='a'&&c<='z')||
		(c>='�'&&c<='�')||
		(c>='�'&&c<='�')
		)
		return TRUE;
	return FALSE;
}

void Speller::FirstCharBig(CString& s){
	s.MakeLower();
	LPTSTR bf=s.LockBuffer();
	for(;*bf;bf++){
		if(isalpha2(*bf))
		{
			CString csFirst=*bf;
			csFirst.MakeUpper();
			*bf=csFirst.GetAt(0);
			break;
		}
	}
	s.UnlockBuffer();
}

/*
#include<locale>   //��� ��� isalpha,toupper

locale loc_rus ("Russian_Russia.1251");
void Speller::FirstCharBig(CString& s){
	s.MakeLower();
	LPTSTR bf=s.LockBuffer();
	for(;*bf;bf++){
		if(isalpha(*bf,loc_rus)){
			*bf=toupper(*bf,loc_rus);
			break;
		}
	}
	s.UnlockBuffer();
}
*/
