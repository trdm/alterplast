//metaparser.h
#pragma once

class MetaParser
{
public:
	void processSql(CString& sql);
	void setTextParam(const CString& name, const CValue* pValue)
	{
		textParams[name] = *pValue;
	}
	void reset()
	{
		textParams.RemoveAll();
	}
protected:
	BOOL makeString(const CString& paramName, int mod, CString& paramString);
	void processParam(struct parsingData& pd);
	void processMetaName(struct parsingData& pd);
	CNoCaseMap<CValue> textParams;
};
