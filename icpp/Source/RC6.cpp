/* RC6.c
*/

#include "stdafx.h"
//#include <windows.h>
//#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <Nb30.h>
#include "RC6.h"

/*typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;

ASTAT Adapter;
unsigned long mac1,mac2;

void dc_getMAC (void)
{
    NCB ncb;
    UCHAR uRetCode;

    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0;

    uRetCode = Netbios( &ncb );

    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0;

    strcpy( (char*)ncb.ncb_callname,  "*               " );
    ncb.ncb_buffer = (unsigned char *) &Adapter;
    ncb.ncb_length = sizeof(Adapter);

    uRetCode = Netbios( &ncb );
    if ( uRetCode == 0 )
    {
		mac1 = (Adapter.adapt.adapter_address[0]<<16)|(Adapter.adapt.adapter_address[1]<<8)|Adapter.adapt.adapter_address[2];
		mac2 = (Adapter.adapt.adapter_address[3]<<16)|(Adapter.adapt.adapter_address[4]<<8)|Adapter.adapt.adapter_address[5];

    }
}*/

//---------------- Adler32 ------------------
void adler32(unsigned char *input, unsigned int length, unsigned long *adler)
{
	const unsigned long BASE = 65521;

	unsigned long s1 = (*adler>>16)&0xFFFF;
	unsigned long s2 = (*adler)&0xFFFF;

	while (length % 8 != 0)
	{
		s1 += *input++;
		s2 += s1;
		length--;
	}

	while (length > 0)
	{
		s1 += input[0]; s2 += s1;
		s1 += input[1]; s2 += s1;
		s1 += input[2]; s2 += s1;
		s1 += input[3]; s2 += s1;
		s1 += input[4]; s2 += s1;
		s1 += input[5]; s2 += s1;
		s1 += input[6]; s2 += s1;
		s1 += input[7]; s2 += s1;

		length -= 8;
		input += 8;

		if (s1 >= BASE)
			s1 -= BASE;
		if (length % 0x8000 == 0)
			s2 %= BASE;
	}

	*adler = ((s1&0xFFFF)<<16) | (s2&0xFFFF);
}

//------------------------------ RC6 -------------------------
RC6Base::RC6Base(const byte *k, unsigned int keylen, unsigned int rounds)
	: r(rounds), sTable((2*r)+4)
{
	//assert(keylen == KeyLength(keylen));

	static const RC6_WORD MAGIC_P = 0xb7e15163L;    // magic constant P for wordsize
	static const RC6_WORD MAGIC_Q = 0x9e3779b9L;    // magic constant Q for wordsize
	static const int U=sizeof(RC6_WORD);

	const unsigned int c=(keylen+U-1)/U;
	SecBlock<RC6_WORD> l(c);

	GetUserKeyLittleEndian(l.ptr, c, k, keylen);

	sTable[0] = MAGIC_P;
	for (unsigned j=1; j<sTable.size;j++)
		sTable[j] = sTable[j-1] + MAGIC_Q;

	RC6_WORD a=0, b=0;
	const unsigned n = 3*STDMAX(sTable.size,c);

	for (unsigned h=0; h < n; h++)
	{
		a = sTable[h % sTable.size] = rotlFixed((sTable[h % sTable.size] + a + b), 3);
		b = l[h % c] = rotlMod((l[h % c] + a + b), (a+b));
	}
}

//#ifdef DMSLG
void RC6Encryption::ProcessBlock(const byte *in, byte *out) const
{
	const RC6_WORD *sptr = sTable;
	RC6_WORD a, b, c, d, t, u;

	GetBlockLittleEndian(in, a, b, c, d);
	b += sptr[0];
	d += sptr[1];
	sptr += 2;

	for(unsigned i=0; i<r; i++)
	{
		t = rotlFixed(b*(2*b+1), 5);
		u = rotlFixed(d*(2*d+1), 5);
		a = rotlMod(a^t,u) + sptr[0];
		c = rotlMod(c^u,t) + sptr[1];
		t = a; a = b; b = c; c = d; d = t;
		sptr += 2;
	}

	a += sptr[0];
	c += sptr[1];

	PutBlockLittleEndian(out, a, b, c, d);
}
//#endif

void RC6Decryption::ProcessBlock(const byte *in, byte *out) const
{
	const RC6_WORD *sptr = sTable+sTable.size;
	RC6_WORD a, b, c, d, t, u;

	GetBlockLittleEndian(in, a, b, c, d);

	sptr -= 2;
	c -= sptr[1];
	a -= sptr[0];

	for (unsigned i=0; i < r; i++)
	{
		sptr -= 2;
		t = a; a = d; d = c; c = b; b = t;
		u = rotlFixed(d*(2*d+1), 5);
		t = rotlFixed(b*(2*b+1), 5);
		c = rotrMod(c-sptr[1], t) ^ u;
		a = rotrMod(a-sptr[0], u) ^ t;
	}

	sptr -= 2;
	d -= sTable[1];
	b -= sTable[0];

	PutBlockLittleEndian(out, a, b, c, d);
}