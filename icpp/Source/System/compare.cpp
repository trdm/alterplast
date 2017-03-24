//______________________________________________________________________________
//
// Design and implementation by kms, 2006
// Fast string comparator class (NLS version)
//______________________________________________________________________________
//
#include "stdafx.h"
#include "compare.h"

int CCompare::Compare(LPCSTR key1, LPCSTR key2)
{
	int result;
	UINT n1, n2;

	while (
		// именно в этом порядке
		n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))],
		n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))],
		!(result = n1 - n2) && n2);

	return result;
}

int CCompare::Compare(LPCSTR key1, LPCSTR key2, size_t nLen)
{
	int result;
	UINT n1, n2;

	while (
		n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))],
		n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))],
		!(result = n1 - n2) && n2 && --nLen);

	return result;
}

//______________________________________________________________________________
//
// сравнение строк без учета пробелов слева и справа
//______________________________________________________________________________
//
int CCompare::CompareTrimSpc(LPCSTR key1, LPCSTR key2)
{
	int result;
	UINT n1, n2;
	const UINT spc = 0x01;

	// пропуск пробелов слева
	do n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))]; while(n2 & spc);
	do n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))]; while(n1 & spc);

	// сравнение
	while ((n1 == n2) && n2)
	{
		n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))];
		n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))];
	};

	// пропуск пробелов справа
	int n0 = n2;
	while(n2 & spc) n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))];
	if (n2)
		n2 = n0;

	n0 = n1;
	while(n1 & spc) n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))];
	if (n1)
		n1 = n0;
	
	// финал
	result = n1 - n2;
	return result;
}
//______________________________________________________________________________
//
// сравнение без учета пробелов вообще
//______________________________________________________________________________
//
int CCompare::CompareIgnoreSpc(LPCSTR key1, LPCSTR key2)
{
	int result;
	UINT n1, n2;
	const UINT spc = 0x01;

	for(;;) {
		// простое сравнение
		n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))];
		n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))];
		if ((n1 == n2) && n2)
			continue;

		// пропуск пробелов
		while(n2 & spc) n2 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key2++))];
		while(n1 & spc)	n1 = m_tbl[static_cast<UINT>(static_cast<BYTE>(*key1++))];

		// финал
		if ((result = n1 - n2) || !n2)
			break;
	};

	return result;
}

UINT CCompare::GetWeight(LCID lcid, char ch)
{
	const int dst_len = 20;
	char dst[dst_len];

	BYTE w_case = 0x0;
	BYTE w_diacritic = 0x0;
	WORD w_unicode = 0xFFFF;

	if (LCMapStringA(lcid, LCMAP_SORTKEY|SORT_STRINGSORT, &ch, 1, dst, dst_len))
	{
		char* pc = dst;
		char c;
		while ((c = *pc++) && (c != 0x01))
			w_unicode = MAKEWORD(*pc++, c);

		assert(c);
		if (c) // always valid
		{
			while ((c = *pc++) && (c != 0x01))
				w_diacritic = c;
		}

		assert(c);
		if (c) // always valid
		{
			while ((c = *pc++) && (c != 0x01))
				w_case = c;
		}
	}
	// порядок сортировки регистра: младшие w_case, старшие w_case (т.е. прописные сначала)
	UINT weight = (w_unicode << 16) | (w_diacritic << 8) | (w_case ^ 0xFF);
	return weight;
}

//______________________________________________________________________________
//
// инит карты весов
// параметры: mask:
//		0xFFFF0000 : Unicode sort weights
//		0x0000FF00 : Diacritic weights
//		0x000000FF : Case weights
//______________________________________________________________________________
//
BOOL CCompare::Init(LCID lcid, UINT mask)
{
	const int src_len = 256;
	const int dst_len = src_len * 2;

	char src[src_len];
	char dst[dst_len];

	// source string prep
	char *pc = src;
	for (int i =1; i < src_len; ++i)
		*pc++ = (char)i;
	*pc =0;

	if (!GetStringTypeA(lcid, CT_CTYPE1, src, src_len, (WORD *)dst))
	{
		for (int i = 0; i < src_len; ++i)
			m_tbl[i] = i;

		return FALSE;
	}

	// weight table constructing
	m_tbl[0] = 0;
	for (int i = 1; i < src_len; ++i)
	{
		UINT weight = GetWeight(lcid, (char)i) & mask;
		// здесь теряется старший бит регистра
		UINT w_case = weight & 0xFF;
		weight ^= w_case;
		weight |= (w_case << 1) & 0xFF;

		// здесь добавляется младший бит - space flag
		DWORD flags = ((WORD *)dst)[i - 1];
		if (flags & (C1_SPACE | C1_CNTRL | C1_BLANK))
			weight |= 1;

		m_tbl[i] = weight;
	}
	// space special case
	m_tbl[' '] = 1;

	return TRUE;
}
