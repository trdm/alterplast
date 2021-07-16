#ifndef __SPELL_H__
#define __SPELL_H__

enum SpellPatrimony_{patrMan=0,patrFem=1,patrInd=2
}; //–од ("наследственность")
struct SpellPatrimony{
	int patr;
    SpellPatrimony(int c);
    SpellPatrimony();
	operator int();
};


class spell_string{
protected:
	CString str; //"Ќачальна€ форма"
public:
	spell_string();
	spell_string(spell_string&);
	spell_string(CString);
	virtual CString& operator[](int);
	virtual CString& operator[](double);
	virtual SpellPatrimony operator =(int);
	virtual operator SpellPatrimony(); 
	CString operator=(CString);
	CString operator=(LPCTSTR);
	spell_string& operator=(spell_string&);
	inline operator CString&(){return str;};
	friend class spell_num;
};

class spell_unit: public spell_string{
protected:
	//Ќач. форма - ед. число
	CString unit2,unit5;
	SpellPatrimony patr;
public:
	spell_unit();
	spell_unit(spell_unit&);
	spell_unit(CString,int=patrMan);
	spell_unit(CString,CString,CString,int=patrMan);
	virtual CString& operator[] (int n);
	virtual CString& operator[] (double d);
	virtual SpellPatrimony operator =(int);
	friend class spell_num;
};

class spell_patr: public spell_string{
protected:
	// Ќач. форма - мужск. род
	CString sFem,sInd;
public:
	spell_patr();
	spell_patr(spell_patr&);
	spell_patr(CString);
	spell_patr(CString,CString,CString);
	virtual CString& operator[] (int n);
	virtual CString& operator[] (double d);
	friend class spell_num;
	//virtual CString& operator=()=NULL;
};

class spell_num{

	CString	plus,minus;
	spell_string	num0;
	spell_patr num1_2[2];
	spell_string num3_19[19-3+1];
	spell_string num20_90[(90-20)/10+1];
	spell_string num100_900[(900-100)/100+1];
	spell_unit numexp[5]; //10^0 - всегда пустое


	int Decode999(CString &rez,int arg,int patr);

public:
	spell_num();
	spell_num(spell_num&);
	spell_string& operator[] (int n);
	spell_string& operator[] (double d);


	CString operator() (int n,int patr=patrMan);
	CString operator() (double d,int patr=patrMan);
	CString operator() (int n,spell_unit &su);
	CString operator() (double d,spell_unit &su);
	CString operator() (double d,spell_unit &su,spell_unit &fu);
	void SetSpell(LPCTSTR []);
	static void SetDefaultSpell(spell_num&);

};

class spell_date{
public:
	CString Months_And_Other[24]
	//12мес + 12мес в р.п.,
	,Quart,SemiYear,Month_9,Year,SmallYear;
	//+ кв + 1/2г + 9мес + год + сокр. год. =29 - и все идет подр€д
	spell_date();
	CString DayOfWeek[7];
	enum Interval{InM=12,InQ=25,InS=26,InMonths_9=27
		,InYear=28,InSmallYear=29};
	CString& operator[] (int n);
	CString operator() (int n);
};

struct Speller{
	Speller();
	Speller(Speller&);
	spell_unit UnitBig,UnitSmall;
	spell_date DatePart;
	spell_num num_Speller;
	static Speller& GetDefaultSpeller();
	void SetAsDefaultSpeller();
	void CopyToDefaultSpeller();
	~Speller();

	//friend class SpellPtr;
/*protected:
	~Speller();*/
	
	static CString FloatFormat(double d,int width,int prec,int leadzero,char zero,char dot,char coma);
	static void FirstCharBig(CString&); //устанавливает 1-ю букву строки большой, остальные-маленькие
	static CString GetFract(double d,int prec);

};

/*struct SpellPtr{
	operator Speller&();
	Speller *spell;
	SpellPtr();
	operator 
};*/

#endif __SPELL_H__
