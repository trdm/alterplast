

//проверка доступности переменной
#define CHECK_READONLY(Operation)\
if(Value1.bReadOnly)\
{\
	CValue Val;\
	Operation(Val,Value2,Value3);\
	Value1.SetValue(Val);\
	return;\
}\
if(Value1.nType==TYPE_REFFER)\
	Value1.pRef->DecrRef();\


//Функции для быстрой работы с типом CValue
inline void AddValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(AddValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
			Value1.fData=Value2.GetNumber()+Value3.GetNumber();
		break;
		case TYPE_DATE:
			if(Value3.nType==TYPE_DATE)//дата+дата -> число
			{
				Value1.nType=TYPE_NUMBER;
				Value1.fData=Value2.GetDate()+Value3.GetDate();
			}
			else
				Value1.dData=Value2.GetDate()+Value3.GetDate();
		break;
		default:
			{
				Value1.nType=TYPE_STRING;
				Value1.sData=Value2.GetString()+Value3.GetString();
			}
	};
}

inline void AddValue(CValue &Value1,CValue &Value2,__int64 Value3)
{
	CHECK_READONLY(AddValue);

	
	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
			Value1.fData=Value2.GetNumber()+Value3;
		break;
		case TYPE_DATE:
			Value1.dData=Value2.GetDate()+Value3;
		break;
		default:
			{
				Value1.nType=TYPE_STRING;
				Value1.sData.Format("%s%d",Value2.GetString(),Value3);
			}
	};
}
inline void SubValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(SubValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			Value1.fData=Value2.GetNumber()-Value3.GetNumber();
		break;
		case TYPE_DATE:
			if(Value3.nType==TYPE_DATE)//дата-дата -> число
			{
				Value1.nType=TYPE_NUMBER;
				Value1.fData=Value2.GetDate()-Value3.GetDate();
			}
			else
				Value1.dData=Value2.GetDate()-Value3.GetDate();
		break;
		default:
			Error(CString("Операция вычитания не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}
inline void SubValue(CValue &Value1,CValue &Value2,__int64 Value3)
{
	CHECK_READONLY(SubValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			Value1.fData=Value2.GetNumber()-Value3;
		break;
		case TYPE_DATE:
			Value1.dData=Value2.GetDate()-Value3;
		break;
		default:
			Error(CString("Операция вычитания не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}

inline void MultValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(MultValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			Value1.fData=Value2.GetNumber()*Value3.GetNumber();
		break;
		case TYPE_DATE:
			if(Value3.nType==TYPE_DATE)//дата*дата -> число
			{
				Value1.nType=TYPE_NUMBER;
				Value1.fData=Value2.GetDate()*Value3.GetDate();
			}
			else
				Value1.dData=Value2.GetDate()*Value3.GetDate();
		break;
		default:
			Error(CString("Операция умножения не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}
inline void MultValue(CValue &Value1,CValue &Value2,__int64 Value3)
{
	CHECK_READONLY(MultValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			Value1.fData=Value2.GetNumber()*Value3;
		break;
		case TYPE_DATE:
			Value1.dData=Value2.GetDate()*Value3;
		break;
		default:
			Error(CString("Операция умножения не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}

inline void DivValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(DivValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			{
				NUMBER f=Value3.GetNumber();
				if(f==0)
					Error("Деление на ноль");
				Value1.fData=Value2.GetNumber()/f;
			}
		break;
		default:
			Error(CString("Операция деления не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}
inline void DivValue(CValue &Value1,CValue &Value2,__int64 Value3)
{
	CHECK_READONLY(DivValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			{
				if(Value3==0)
					Error("Деление на ноль");
				Value1.fData=Value2.GetNumber()/Value3;
			}
		break;
		default:
			Error(CString("Операция деления не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}
inline void ModValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(ModValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType) 
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			{
				__int64 f=__int64(Value3.GetNumber());
				if(f==0)
					Error("Деление на ноль");
				Value1.fData=__int64(Value2.GetNumber())%f;
			}
		break;
		default:
			Error(CString("Операция деления по модулю не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}
inline void ModValue(CValue &Value1,CValue &Value2,__int64 Value3)
{
	CHECK_READONLY(ModValue);
	

	Value1.nType=Value2.GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
//		case TYPE_STRING:
//		case TYPE_REFFER:
			{
				if(Value3==0)
					Error("Деление на ноль");

				Value1.fData=__int64(Value2.GetNumber())%Value3;
			}
		break;
		default:
			Error(CString("Операция деления по модулю не может применяться для данного типа (")+Value2.GetTypeString()+")");
	};
}

//Реализация операторов сравнения
#define CompareValueDef(name,x)\
inline void CompareValue##name(CValue &Value1,CValue &Value2,CValue &Value3)\
{\
	CHECK_READONLY(CompareValue##name);\
	Value1.nType=TYPE_NUMBER;\
	int nMode=Value2.nType;\
	if(Value3.nType!=nMode)\
		nMode=0;\
	switch(nMode)\
	{\
		case TYPE_NUMBER:\
			Value1.fData=Value2.GetNumber() x Value3.GetNumber();\
		break;\
		case TYPE_DATE:\
			Value1.fData=Value2.GetDate() x Value3.GetDate();\
		break;\
		default:\
			{\
				Value1.fData=Value2.GetString() x Value3.GetString();\
			}\
	};\
}


CompareValueDef(GT,>)//CompareValueGT
CompareValueDef(EQ2,==)//CompareValueEQ
CompareValueDef(LS,<)//CompareValueLS
CompareValueDef(GE,>=)//CompareValueGE
CompareValueDef(LE,<=)//CompareValueLE
CompareValueDef(NE2,!=)//CompareValueNE




inline void CompareValueEQ(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(CompareValueEQ);
	Value1.nType=TYPE_NUMBER; 
	int nType2=Value2.GetType();
	if(nType2!=Value3.GetType())
	{
		Value1.fData=0;
	}
	else
	{
#ifndef SMALL_TRANSLATE
		if(nType2==TYPE_REFFER)
		{
			if(Value2.pRef->GetRuntimeClass()==RUNTIME_CLASS(CValueObject))
			{
				if(Value3.pRef->GetRuntimeClass()!=RUNTIME_CLASS(CValueObject))
				{
					Value1.fData=0;
					return;
				}
				CValueObject *p2=(CValueObject *)Value2.pRef;
				CValueObject *p3=(CValueObject *)Value3.pRef;
				Value1.fData=(p2->CallFunction("ПолучитьИдентификаторОбъекта").GetNumber()==p3->CallFunction("ПолучитьИдентификаторОбъекта").GetNumber());
				return;
			}
		}
#endif
		CompareValueEQ2(Value1,Value2,Value3);
	}
}

inline void CompareValueNE(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CHECK_READONLY(CompareValueNE);
	Value1.nType=TYPE_NUMBER; 
	int nType2=Value2.GetType();
	if(Value2.GetType()!=Value3.GetType())
	{
		Value1.fData=1;
	}
	else
	{
#ifndef SMALL_TRANSLATE
		if(nType2==TYPE_REFFER)
		{
			if(Value2.pRef->GetRuntimeClass()==RUNTIME_CLASS(CValueObject))
			{
				if(Value3.pRef->GetRuntimeClass()!=RUNTIME_CLASS(CValueObject))
				{
					Value1.fData=1;
					return;
				}
				CValueObject *p2=(CValueObject *)Value2.pRef;
				CValueObject *p3=(CValueObject *)Value3.pRef;
				Value1.fData=!(p2->CallFunction("ПолучитьИдентификаторОбъекта").GetNumber()==p3->CallFunction("ПолучитьИдентификаторОбъекта").GetNumber());
				return;
			}
		}
#endif
		CompareValueNE2(Value1,Value2,Value3);
	}
}

inline void CopyValue(CValue &Value1,CValue &Value2)
{
	if(&Value1==&Value2)
		return;

	//проверка доступности переменной и контроль ссылок
	if(Value1.bReadOnly)
	{
		Value1.SetValue(Value2);
		return;
	}
	else//Reset
	{
		if(Value1.pRef&&Value1.nType==TYPE_REFFER)
			Value1.pRef->DecrRef();
		Value1.nType=0;
		Value1.fData=0;
		Value1.sData.Empty();
	}

	if(Value2.nType==TYPE_REFFER)
	{
		Value1=Value2.GetValue();
		return;
	}


	Value1.nType=Value2.nType;//GetType();
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
			Value1.fData=Value2.fData;
			return;
		case TYPE_STRING:
			Value1.sData=Value2.sData;
			return;
		case TYPE_DATE:
			Value1.dData=Value2.dData;
			return;
		case TYPE_REFFER:
			Value1.pRef=Value2.pRef;
			Value1.pRef->IncrRef();
			return;
		case 100:
			Value1=ValueByRef(&Value2);
			return;
		case TYPE_EMPTY:
			return;
		default:
			Value1.nType=0;
	}
}

inline BOOL IsEmptyValue(CValue &Value1)
{
	switch(Value1.nType)
	{
		case TYPE_NUMBER:
			return !Value1.fData;//GetNumber();
		case TYPE_STRING:
			return Value1.sData.IsEmpty();//GetString().IsEmpty();
		case TYPE_DATE:
			return !Value1.dData;//GetDate();//Value1.dData!=Value1.GetDate();
		case TYPE_REFFER:
			if(Value1.pRef)
				return Value1.pRef->IsEmpty();
			else
				return 1;
		break;
	};
	return !Value1.fData;
//	return TRUE;
}
#define IsHasValue(Value1) (!IsEmptyValue(Value1))


inline void SetTypeNumber(CValue &Value1,__int64 nValue)
{
	//проверка доступности переменной и контроль ссылок
	if(Value1.bReadOnly)
	{
		Value1.SetValue(CValue(nValue));
		return;
	}
	Value1.Reset();

	Value1.nType=TYPE_NUMBER;
	Value1.fData = nValue;
}
/*
void CheckAndError(CValue &Variable,CString csName)
{
	CString Str;
	if(Variable.nType!=TYPE_REFFER)
		Str.Format("Не найден атрибут или метод '%s' - переменная не является агрегатным объектом",csName);
	else
		Str.Format("Поле агрегатного объекта не обнаружено '%s'",csName);
	Error(Str.GetBuffer(0));
}
*/
#define CheckAndError(Variable,csName)\
{\
	if(Variable.nType!=TYPE_REFFER)\
		Error("Не найден атрибут или метод '%s' - переменная не является агрегатным объектом",csName);\
	else\
		Error("Поле агрегатного объекта не обнаружено '%s'",csName);\
}
//	OutToDebug("Модуль: %s, строка: %d\n",pByteCode->csModuleName,pByteCode->CodeList[nCodeLine].nNumberLine);\

//проверяет является ли данная переменная массивом, если нет, то создает массив
inline void CheckArray(CValue &Value1)
{
	if(Value1.GetType()!=TYPE_ARRAY)//создаем массив
	{
		Value1.Reset();
		Value1.nType=TYPE_REFFER;
		Value1.pRef=new CValueArray();
		Value1.pRef->nRef=1;
	}
}
inline void SetArrayValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CheckArray(Value1);
	if(Value2.GetType()==TYPE_NUMBER)
		((CValueArray*)Value1.pRef)->SetAt(Value2.GetNumber(),Value3);
	else
		((CValueArray*)Value1.pRef)->SetAt(Value2.GetString(),Value3);
}
inline void GetArrayValue(CValue &Value1,CValue &Value2,CValue &Value3)
{
	CheckArray(Value2);
	if(Value3.GetType()==TYPE_NUMBER)
		CopyValue(Value1,((CValueArray*)Value2.pRef)->GetAt(Value3.GetNumber()));
	else
		CopyValue(Value1,((CValueArray*)Value2.pRef)->GetAt(Value3.GetString()));
}

inline void CheckArray(CValue &Value1,CValue &Value2)
{
	CheckArray(Value1);

	if(Value2.GetType()==TYPE_NUMBER)
		CheckArray(((CValueArray*)Value1.pRef)->GetAt(Value2.GetNumber()));
	else
		CheckArray(((CValueArray*)Value1.pRef)->GetAt(Value2.GetString()));
}

inline void SetSizeArray(CValue &Value1,int nSize)
{
	CheckArray(Value1);
	if(nSize>0)
	{
		((CValueArray*)Value1.pRef)->SetSizeArray(nSize);
	}
}

//Численно-индексные массивы
inline void SetArrayValue(CValue &Value1,int Value2,CValue &Value3)
{
	CheckArray(Value1);
	((CValueArray*)Value1.pRef)->SetAt(Value2,Value3);
}
inline void GetArrayValue(CValue &Value1,CValue &Value2,int Value3)
{
	CheckArray(Value2);
	CopyValue(Value1,((CValueArray*)Value2.pRef)->GetAt(Value3));
}

//Строково-индексные массивы
inline void SetArrayValue(CValue &Value1,CString Value2,CValue &Value3)
{
	CheckArray(Value1);
	((CValueArray*)Value1.pRef)->SetAt(Value2,Value3);
}
inline void GetArrayValue(CValue &Value1,CValue &Value2,CString Value3)
{
	CheckArray(Value2);
	CopyValue(Value1,((CValueArray*)Value2.pRef)->GetAt(Value3));
}


inline CValue GetValue(CValue &Value1)
{
	if(Value1.bReadOnly&&Value1.nType!=TYPE_REFFER)
//	if(Value1.bReadOnly)
	{
		CValue Val;
		CopyValue(Val,Value1);
		return Val;
	}
	return Value1;
}
