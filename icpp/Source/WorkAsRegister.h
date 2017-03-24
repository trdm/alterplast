// WorkAsRegister.h: interface for the CWorkAsRegister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKASREGISTER_H__D63A6FC2_3FAD_4134_A66F_D2F34B044DC8__INCLUDED_)
#define AFX_WORKASREGISTER_H__D63A6FC2_3FAD_4134_A66F_D2F34B044DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkAsRegister : public CBLContext  
{
DECLARE_DYNCREATE(CWorkAsRegister);
public:
	CWorkAsRegister();
	virtual ~CWorkAsRegister();
	
public:
	void ConnectRegistry(HKEY key, LPCTSTR lpMachineName);
	enum {
	methEnableDebug,
	methCreateKey,
	methOpenKey,
	methDeleteKey,
	methDeleteValue,
	methSetValue,
	methQueryValue,
	methChooseKeys,
	methGetKey,
	methChooseValues,
	methGetValue,
	methFlash,
	methConnectRegistry,
	lastMethod
	};

	static struct paramdefs {
		char * Names[2];
		int HasReturnValue;
		int NumberOfParams;
	}  defFnNames[];
	
	virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	virtual int  GetDestroyUnRefd(void)const;	//1657
	virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	//virtual void  InitObject(CType const &);	//2721
	//virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,CValue const &);	//3694
	//////////////////////////////////////////////////////////////////////
	// functions CWorkAsRegister
	//////////////////////////////////////////////////////////////////////
	struct SExcept {
		SExcept(long err) : lCodeError(err) {}
		long lCodeError;
	};
	void OpenKey(HKEY key,LPCTSTR lpcName,REGSAM samDesired);
	void CreateKey(HKEY key, LPCTSTR lpcNameValue, REGSAM samDesired, LPDWORD lpdwDisposition);
	void SetValue(LPCTSTR lpcNameValue, REGSAM samDesired, BYTE* pValue, DWORD dwSize);
	void QueryValue(LPCTSTR lpcNameValue, LPDWORD lpType, BYTE* pValue, LPDWORD pDwSize);
	void DeleteKey(LPCTSTR lpSubKey);
	void DeleteValue(LPCTSTR lpValueName);
	void Flash();
	bool EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, LPTSTR lpClass, 
		         LPDWORD lpcClass, PFILETIME lpftLastWriteTime);
	bool EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpType, 
				   LPBYTE lpData, LPDWORD lpcbData);
	void InfoKey(LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, 
		         LPDWORD lpcMaxValueLen, PFILETIME lpftLastWriteTime);
	operator HKEY();
	static void ShowError(SExcept& e, bool bEnableDebug);
	//////////////////////////////////////////////////////////////////////
	static class CObjID ObjID;
	class CBLModule * pBLModule;
private:
	HKEY hKey;
	bool bEnableDebug;
	int nPosOfKey;
	int nPosOfVal;
	DWORD dwCount;
	DWORD dwMaxLen;
};

#endif // !defined(AFX_WORKASREGISTER_H__D63A6FC2_3FAD_4134_A66F_D2F34B044DC8__INCLUDED_)
