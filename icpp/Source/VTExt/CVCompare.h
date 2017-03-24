#ifndef CVCOMPARE_H_INCLUDED
#define CVCOMPARE_H_INCLUDED

// Способ сравнения значений
typedef enum { CmpByStringRepr, CmpByInnerRepr } TCompareType ;

// Колонка индекса с флагами сортировки
struct TVTIndexDescrRecord
{
	int ColumnNumber;
	int Direction;
	TCompareType CompareType;
	bool TrimStrings;
	bool NoCaseStringCompare;
	bool SumsBySubcontoGroups; //поле используется только для группировки ТЗ

	void ParseMod(CString const& ColName)
	{
		CompareType = ColName.Find('*') < 0 ? CmpByStringRepr : CmpByInnerRepr;
		Direction = ColName.Find('-') < 0 ? 1 : -1;
		TrimStrings = ColName.Find('#') >= 0;
		NoCaseStringCompare = ColName.Find('^') >= 0;
		SumsBySubcontoGroups = ColName.Find('&') >= 0;
	}
	static void TrimMod(CString& ColName)
	{
		ColName.Replace("-", "");
		ColName.Replace("*", "");
		ColName.Replace("#", "");
		ColName.Replace("^", "");
		ColName.Replace("&", "");
	}
};

class CVCompare
{
public:
	static int numeric_compare(int a, int b)
	{
		return a - b;
	}

	static int cv_numeric_compare(CValue const& left, CValue const& right)
	{
		return left.GetNumeric().Compare(right.GetNumeric());
	}

	static int string_compare(LPCSTR left, LPCSTR right, TVTIndexDescrRecord const& IdxRec)
	{
		if( IdxRec.NoCaseStringCompare )
		{
			if( IdxRec.TrimStrings )
				return FastCompareNoCase.CompareTrimSpc(left, right);
			else
				return FastCompareNoCase.Compare(left, right);
		}
		else if ( IdxRec.TrimStrings )
			return FastCompare.CompareTrimSpc(left, right);
		else
			return FastCompare.Compare(left, right);
	}

	static int safe_string_compare(LPCSTR left, LPCSTR right, TVTIndexDescrRecord const& IdxRec)
	{
		if (!left)
			return right ? -1 : 0;
		else if (!right)
			return 1;
		else
			return string_compare(left, right, IdxRec);
	}

	static int cv_string_compare(CValue const& left, CValue const& right, TVTIndexDescrRecord const& IdxRec)
	{
		return string_compare((LPCSTR)CString(left.Format()), (LPCSTR)CString(right.Format()), IdxRec);
	}

	static int date_compare(CDate const& d1, CDate const& d2)
	{
		int comp_res = numeric_compare(d1.GetYear(), d2.GetYear());
		if( comp_res ) return comp_res;
		comp_res = numeric_compare(d1.GetMonth(), d2.GetMonth());
		if( comp_res ) return comp_res;
		comp_res = numeric_compare(d1.GetMonthDay(), d2.GetMonthDay());
		return comp_res;
	}

	static int time_compare(CEventTime const& t1, CEventTime const& t2)
	{
		if( t1 < t2 ) return -1;
		else if( t1 > t2 ) return 1;
		return 0;
	}

	static inline DWORD gettypeid(CValue const &cv)
	{
		return cv.m_mdid ? cv.m_mdid : cv.ValTypeID;
	}

	static int inner_compare(CValue const& left, CValue const& right)
	{
		int diff;
		if( !(diff = gettypeid(left) - gettypeid(right)) )
		{
			if( !(diff = left.m_ObjID.GetlObjID() - right.m_ObjID.GetlObjID()) )
				diff = FastCompare.Compare(left.m_ObjID.DBSign.Sign, right.m_ObjID.DBSign.Sign);
		}
		return diff;
	}

	static int CValue_compare(CValue const& left, CValue const& right, TVTIndexDescrRecord const& IdxRec)
	{
		int nTypeCodeLeft = left.GetTypeCode();
		int nTypeCodeRight = right.GetTypeCode();
		if( nTypeCodeLeft == nTypeCodeRight )
		{
			switch( nTypeCodeLeft )
			{
			case UNDEFINE_TYPE_1C:
				return 0;
				break;
			case NUMBER_TYPE_1C:
				return cv_numeric_compare(left, right);
				break;
			case STRING_TYPE_1C:
				return string_compare(left.m_String.operator LPCSTR(), right.m_String.operator LPCSTR(), IdxRec);
				break;
			case DATE_TYPE_1C:
				{
					CDate d1 = left.GetDate(), d2 = right.GetDate();
					return date_compare(d1, d2);
				}
				break;
			case REFERENCE_TYPE_1C:
				{
					if( IdxRec.CompareType == CmpByInnerRepr )
						return inner_compare(left, right);

					// учтено время жизни указателей
					int comp_res = cv_string_compare(left, right, IdxRec);
					if( comp_res == 0 )
						comp_res = inner_compare(left, right);

					return comp_res;
				}
				break;
			case DOCUMENT_TYPE_1C:
				if( IdxRec.CompareType == CmpByInnerRepr )
				{
					return inner_compare(left, right);
				}
				else
				{
					CDate d1(0,0,0), d2(0,0,0);
					CEventTime t1, t2;
					GetDateTimeFromValue(left, d1, t1);
					GetDateTimeFromValue(right, d2, t2);
					int comp_res = date_compare(d1, d2);
					if( comp_res == 0 ) comp_res = time_compare(t1, t2);
					return comp_res;
				}
				break;

			case AGREGATE_TYPE_1C:
				{
					if( IdxRec.CompareType == CmpByInnerRepr )
						return left.m_Context - right.m_Context;

					int comp_res = cv_string_compare(left, right, IdxRec);
					if( comp_res == 0 )
						comp_res = left.m_Context - right.m_Context;

					return comp_res;
				}
				break;

			default:
				if( IdxRec.CompareType == CmpByInnerRepr )
					return inner_compare(left, right);
				else
					return cv_string_compare(left, right, IdxRec);
			}
		}
		else
		{
			return numeric_compare(nTypeCodeLeft, nTypeCodeRight);
		}
	}
};

#endif
