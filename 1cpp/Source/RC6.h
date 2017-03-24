#if !defined(AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7324234234AFS9333__INCLUDED_)
#define AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7324234234AFS9333__INCLUDED_
/* RC6.h
*/
typedef unsigned char byte;
typedef unsigned long word32;
typedef unsigned __int32 word;
typedef unsigned __int64 dword;

const unsigned int WORD_SIZE = sizeof(word);
const unsigned int WORD_BITS = WORD_SIZE * 8;


template <class _Tp>
inline const _Tp& STDMIN(const _Tp& __a, const _Tp& __b) {
  return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& STDMAX(const _Tp& __a, const _Tp& __b) {
  return  __a < __b ? __b : __a;
}

// ************** secure memory allocation ***************

#define SecAlloc(type, number) (new type[(number)])
#define SecFree(ptr, number) (memset((ptr), 0, (number)*sizeof(*(ptr))), delete [] (ptr))

template <class T> struct SecBlock
{
	explicit SecBlock(unsigned int size=0)
		: size(size) {ptr = SecAlloc(T, size);}
	SecBlock(const SecBlock<T> &t)
		: size(t.size) {ptr = SecAlloc(T, size); memcpy(ptr, t.ptr, size*sizeof(T));}
	SecBlock(const T *t, unsigned int len)
		: size(len) {ptr = SecAlloc(T, len); memcpy(ptr, t, len*sizeof(T));}
	~SecBlock()
		{SecFree(ptr, size);}

	operator const T *() const
		{return ptr;}
	operator T *()
		{return ptr;}

	const T* Begin() const
		{return ptr;}
	T* Begin()
		{return ptr;}
	const T* End() const
		{return ptr+size;}
	T* End()
		{return ptr+size;}

	void Assign(const T *t, unsigned int len)
	{
		New(len);
		memcpy(ptr, t, len*sizeof(T));
	}

	void Assign(const SecBlock<T> &t)
	{
		New(t.size);
		memcpy(ptr, t.ptr, size*sizeof(T));
	}

	SecBlock& operator=(const SecBlock<T> &t)
	{
		Assign(t);
		return *this;
	}

	bool operator==(const SecBlock<T> &t) const
	{
		return size == t.size && memcmp(ptr, t.ptr, size*sizeof(T)) == 0;
	}

	bool operator!=(const SecBlock<T> &t) const
	{
		return !operator==(t);
	}

	void New(unsigned int newSize)
	{
		if (newSize != size)
		{
			T *newPtr = SecAlloc(T, newSize);
			SecFree(ptr, size);
			ptr = newPtr;
			size = newSize;
		}
	}

	void CleanNew(unsigned int newSize)
	{
		if (newSize != size)
		{
			T *newPtr = SecAlloc(T, newSize);
			SecFree(ptr, size);
			ptr = newPtr;
			size = newSize;
		}
		memset(ptr, 0, size*sizeof(T));
	}

	void Grow(unsigned int newSize)
	{
		if (newSize > size)
		{
			T *newPtr = SecAlloc(T, newSize);
			memcpy(newPtr, ptr, size*sizeof(T));
			SecFree(ptr, size);
			ptr = newPtr;
			size = newSize;
		}
	}

	void CleanGrow(unsigned int newSize)
	{
		if (newSize > size)
		{
			T *newPtr = SecAlloc(T, newSize);
			memcpy(newPtr, ptr, size*sizeof(T));
			memset(newPtr+size, 0, (newSize-size)*sizeof(T));
			SecFree(ptr, size);
			ptr = newPtr;
			size = newSize;
		}
	}

	void Resize(unsigned int newSize)
	{
		if (newSize != size)
		{
			T *newPtr = SecAlloc(T, newSize);
			memcpy(newPtr, ptr, STDMIN(newSize, size)*sizeof(T));
			SecFree(ptr, size);
			ptr = newPtr;
			size = newSize;
		}
	}

	void swap(SecBlock<T> &b);

	unsigned int size;
	T *ptr;
};

template <class T> void SecBlock<T>::swap(SecBlock<T> &b)
{
	std::swap(size, b.size);
	std::swap(ptr, b.ptr);
}

typedef SecBlock<byte> SecByteBlock;
typedef SecBlock<word> SecWordBlock;

//---------------------
template <class T>
inline void GetUserKeyLittleEndian(T *out, unsigned int outlen, const byte *in, unsigned int inlen)
{
	const unsigned int U = sizeof(T);
	memcpy(out, in, inlen);
	memset((byte *)out+inlen, 0, outlen*U-inlen);
}

// Fetch 2 words from user's buffer into "a", "b" in LITTLE-endian order
template <class T>
inline void GetBlockLittleEndian(const byte *block, T &a, T &b)
{
	a = ((T *)block)[0];
	b = ((T *)block)[1];
}

// Put 2 words back into user's buffer in LITTLE-endian order
template <class T>
inline void PutBlockLittleEndian(byte *block, T a, T b)
{
	((T *)block)[0] = a;
	((T *)block)[1] = b;
}

// Fetch 4 words from user's buffer into "a", "b", "c", "d" in LITTLE-endian order
template <class T>
inline void GetBlockLittleEndian(const byte *block, T &a, T &b, T &c, T &d)
{
	a = ((T *)block)[0];
	b = ((T *)block)[1];
	c = ((T *)block)[2];
	d = ((T *)block)[3];
}

// Put 4 words back into user's buffer in LITTLE-endian order
template <class T>
inline void PutBlockLittleEndian(byte *block, T a, T b, T c, T d)
{
	((T *)block)[0] = a;
	((T *)block)[1] = b;
	((T *)block)[2] = c;
	((T *)block)[3] = d;
}

// ************** rotate functions ***************

template <class T> inline T rotlFixed(T x, unsigned int y)
{
	return (x<<y) | (x>>(sizeof(T)*8-y));
}

template <class T> inline T rotrFixed(T x, unsigned int y)
{
	return (x>>y) | (x<<(sizeof(T)*8-y));
}

template <class T> inline T rotlVariable(T x, unsigned int y)
{
	return (x<<y) | (x>>(sizeof(T)*8-y));
}

template <class T> inline T rotrVariable(T x, unsigned int y)
{
	return (x>>y) | (x<<(sizeof(T)*8-y));
}

template <class T> inline T rotlMod(T x, unsigned int y)
{
	y %= sizeof(T)*8;
	return (x<<y) | (x>>(sizeof(T)*8-y));
}

template <class T> inline T rotrMod(T x, unsigned int y)
{
	y %= sizeof(T)*8;
	return (x>>y) | (x<<(sizeof(T)*8-y));
}

template<> inline word32 rotlFixed<word32>(word32 x, unsigned int y)
{
	return y ? _lrotl(x, y) : x;
}

template<> inline word32 rotrFixed<word32>(word32 x, unsigned int y)
{
	return y ? _lrotr(x, y) : x;
}

template<> inline word32 rotlVariable<word32>(word32 x, unsigned int y)
{
	return _lrotl(x, y);
}

template<> inline word32 rotrVariable<word32>(word32 x, unsigned int y)
{
	return _lrotr(x, y);
}

template<> inline word32 rotlMod<word32>(word32 x, unsigned int y)
{
	return _lrotl(x, y);
}

template<> inline word32 rotrMod<word32>(word32 x, unsigned int y)
{
	return _lrotr(x, y);
}

//---------------------
extern unsigned long mac1,mac2;
extern void dc_getMAC (void);

extern void adler32(unsigned char *input, unsigned int length, unsigned long *adler);

class RC6Base
{
public:
	typedef word32 RC6_WORD;

	// values of KEYLENGTH and ROUNDS are defaults only
	enum {KEYLENGTH=16, BLOCKSIZE = sizeof(RC6_WORD)*4, ROUNDS=20};
	unsigned int BlockSize() const {return BLOCKSIZE;}
	static unsigned int KeyLength(unsigned int keylength)
		{return STDMIN(keylength, 255U);}

protected:
	RC6Base(const byte *key, unsigned int keyLen, unsigned int rounds);

	const unsigned int r;       // number of rounds
	SecBlock<RC6_WORD> sTable;  // expanded key table
};

#ifdef DMSLG
class RC6Encryption : public RC6Base
{
public:
	RC6Encryption(const byte *key, unsigned int keyLen=KEYLENGTH, unsigned int rounds=ROUNDS)
		: RC6Base(key, keyLen, rounds) {}

	void ProcessBlock(byte * inoutBlock) const
		{RC6Encryption::ProcessBlock(inoutBlock, inoutBlock);}
	void ProcessBlock(const byte *inBlock, byte * outBlock) const;
};
#endif

class RC6Decryption : public RC6Base
{
public:
	RC6Decryption(const byte *key, unsigned int keyLen=KEYLENGTH, unsigned int rounds=ROUNDS)
		: RC6Base(key, keyLen, rounds) {}

	void ProcessBlock(byte * inoutBlock) const
		{RC6Decryption::ProcessBlock(inoutBlock, inoutBlock);}
	void ProcessBlock(const byte *inBlock, byte * outBlock) const;
};
#endif // AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7324234234AFS9333__INCLUDED_