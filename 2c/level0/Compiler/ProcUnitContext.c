CRunContext *AfxGetCurrentRunContext()
{
	if(afxRunContext.size()==0)
		return 0;
	return afxRunContext.back();
}
CRunContext *AfxGetPrevRunContext()
{
	if(afxRunContext.size()<2)
		return 0;
	return afxRunContext[afxRunContext.size()-2];
}
int AfxGetCountRunContext()
{
	return afxRunContext.size();
}
CByteCode *AfxGetCurrentByteCode()
{
	CRunContext *pContext=AfxGetCurrentRunContext();
	if(pContext)
	{
		if(pContext->pCompileContext)
			if(pContext->pCompileContext->pModule)
				return &pContext->pCompileContext->pModule->cByteCode;
	}
	return 0;
}

inline void AfxBeginByteCode(CRunContext *pCode)
{
//	if(afxMessageFromConfig)//||afxSimpleErrorMessage)
//		return;
	afxRunContext.push_back(pCode);
} 
inline int AfxEndByteCode()
{
//	if(afxMessageFromConfig)//||afxSimpleErrorMessage)
//		return 1;
	int n=afxRunContext.size();
	if(n>0)
	{
		afxRunContext.pop_back();
	}
	n--;

	if(n<=0)
		return 0;
	else
		return 1;
}
//Обнуление стека
void ResetByteCode()
{
//	if(afxMessageFromConfig||afxSimpleErrorMessage)
//		return;
	while(AfxEndByteCode());
}

#define MAX_REC_COUNT	200

//CString FindErrorCodeLine(CString &Buffer,int nCurPos,int nOnlyCodeLine)
int nRecCount=0;//контроль зацикливания
class CSmartStack
{
public:
	CRunContext *pCurrentContext;
	CSmartStack(CRunContext *pContext)
	{
		if(nRecCount>MAX_REC_COUNT)//критическая ошибка
		{
			OutToDebug("Переполнение стека вызовов...\n");
			CString csError;
			for(int i=0;i<afxRunContext.size();i++)
			{
				CRunContext *pLastContext=afxRunContext[i];
				CByteCode *pByteCode=&pLastContext->pCompileContext->pModule->cByteCode;
				CString csModuleName=pByteCode->csModuleName;
				int nLine=pByteCode->CodeList[pLastContext->nCurLine].nNumberLine+1;

				CString csError2;
				csError2.Format("\n%s (#Строка %d)",csModuleName,nLine);
				csError=csError+csError2;
			}


			CString s="Количество рекурcивных вызовов превысило максимально допустимое значение!\nСтек вызова:";
			csError=s+csError;

			//прорисовка окон
			MSG msg;
			while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if( ! ::GetMessage( &msg, NULL, 0, 0 ) )
					break;
				::DispatchMessage( &msg );
			}
			Sleep(100);
			Error(csError);
		}

		nRecCount++;
		pCurrentContext=pContext;
		AfxBeginByteCode(pContext);
	};
	~CSmartStack()
	{
		nRecCount--;
		//ASSERT(pCurrentContext==AfxGetCurrentRunContext());
		AfxEndByteCode();
	};
};

