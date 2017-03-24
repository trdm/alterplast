// CreateMetaInfoClasses.h: interface for the CCreateMetaInfoClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEMETAINFOCLASSES_H__832FA866_F53B_4B46_B9D4_AEC5B26F32B8__INCLUDED_)
#define AFX_CREATEMETAINFOCLASSES_H__832FA866_F53B_4B46_B9D4_AEC5B26F32B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "MSXML.DLL" named_guids
#include "StringArrayEx.h"

class CCreateMetaInfoClasses : public CBLContext  
{
  // Construction
DECLARE_DYNCREATE(CCreateMetaInfoClasses);
public:
  void ProcessMethod(MSXML::IXMLDOMElement *node, CString &strDest);
  void ProcessAttribut(MSXML::IXMLDOMElement *node, CString &strDest);
  void ProcessClass(MSXML::IXMLDOMElement *node, CString &strDest);
	void ProcessNamespace(MSXML::IXMLDOMElement *node, CString& strDest);
  void ProcessElement(MSXML::IXMLDOMElement *node, CString& strDest);
	void CreateALS(CString& strPlace, CString& strCompany);
	void PrepareXMLPart(CString& strSource);
	CCreateMetaInfoClasses();
	virtual ~CCreateMetaInfoClasses();  
  static struct paramdefs {
		char * Names[2];
		int HasReturnValue;
		int NumberOfParams;
	}  defFnNames[];
  virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//935
	virtual int  CallAsProc(int,class CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	virtual int  GetDestroyUnRefd(void)const;	//1657
	virtual void  GetExactValue(class CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,class CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual class CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	virtual void  InitObject(class CType const &);	//2721
	virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(class CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,class CValue const &);	//3694

	static class CObjID ObjID;
private:
  void ProcessChildNode(MSXML::IXMLDOMElement *node, CString &strDest);

  class CExeptionUser
  {
    CString err;
    void Format(MSXML::IXMLDOMElement *node)
    {
      CComBSTR sXml;
      node->get_xml(&sXml);
      CString text;
      text.Format("\r\n(%s)\r\n", CString(sXml));
      err += text;
    }
  public:
    CExeptionUser(const char* p, MSXML::IXMLDOMElement *node) : err(p) {Format(node);}
    CExeptionUser(const CString& p, MSXML::IXMLDOMElement *node) : err(p) {Format(node);}
    CString GetErrorText() {return err;}
    operator CString() {return err;}
    operator LPCSTR() {return err;}
  };

  MSXML::IXMLDOMDocument* pDoc;
  CComBSTR m_strNameSpace;
  CComBSTR m_strNameClass;
  CComBSTR m_strNameMethod;
  CComBSTR m_strNameProperty;
  CComBSTR m_strNameNames; //название атрибута по которому можно определить имя элемента
  CComBSTR m_strNameParam;
  CComBSTR m_strNameReturns;
  
  CStringArrayEx m_filterClasses;
  CStringArrayEx m_filterNamespace;
};

#endif // !defined(AFX_CREATEMETAINFOCLASSES_H__832FA866_F53B_4B46_B9D4_AEC5B26F32B8__INCLUDED_)
