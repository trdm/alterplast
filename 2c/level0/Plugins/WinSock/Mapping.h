// Mapping.h: interface for the CMapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPPING_H__98B47844_66E1_47A8_9826_B4F8F09075F8__INCLUDED_)
#define AFX_MAPPING_H__98B47844_66E1_47A8_9826_B4F8F09075F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define net_msg

class __declspec(novtable) CNetworkMessageMap
{
public:
	virtual bool ProcessNetworkMessage(DWORD dwMsgMapID) = 0;
};

#define BEGIN_NETWORK_MSG_MAP(theClass) \
public: \
		bool ProcessNetworkMessage(DWORD dwMsgMapID = 0) \
		{ \
			bool bRet = false; 

#define NETWORK_MESSAGE_HANDLER(msg, func) \
			if (msg & dwMsgMapID) \
				bRet = func(); 

#define END_NETWORK_MSG_MAP() \
		return bRet; \
	}


//////////////////////////////////////////////////////////////////////

#define app_msg



class __declspec(novtable) CApplicationMessageMap
{
public:
	virtual bool ProcessApplicationMessage(DWORD dwMsgMapID, PBYTE pData) = 0;
};

#define BEGIN_APP_MSG_MAP(theClass) \
public: \
		bool ProcessApplicationMessage(DWORD dwMsgMapID = 0, PBYTE pData = NULL) \
		{ \
			switch(dwMsgMapID) \
			{ \
			case 0:

#define APP_MESSAGE_HANDLER(msg, func) \
			case msg: \
			{ \
				func(pData); \
			} \
			break;


#define END_APP_MSG_MAP() \
			break; \
		default: \
			TRACE(_T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ASSERT(FALSE); \
			break; \
		} \
		return FALSE; \
	}





#endif // !defined(AFX_MAPPING_H__98B47844_66E1_47A8_9826_B4F8F09075F8__INCLUDED_)
