
#include <afxcoll.h>

#ifdef PARSER_PLUGIN
#include "ParserPlugin\stdafx.h"
#include "ParserPlugin\ParserPlugin.h"
#include "ParserPlugin\CodeAnalyser.h"
#endif

#define QUIET_MODE 1

//------------------------------------------------------------
void UnquoteString(CString& Str)
{
	if( Str.Right(1) == '"' )
		Str = Str.Left(Str.GetLength()-1);
	if( Str.Left(1) == '"' )
		Str = Str.Mid(1);
}

void CutLastDot(CString& str)
{
	int len = str.GetLength();
	if( len > 0 && str[len-1] == '.' )
		str.SetAt(len-1, '\0');
}

//------------------------------------------------------------
void Msg(char *msg, ...)
{
	CString ConvertedMsg;
	va_list ap;
	va_start(ap, msg);
#ifndef QUIET_MODE
	ConvertedMsg.FormatV(msg, ap);
#endif
	va_end(ap);

#ifndef QUIET_MODE
#ifdef PARSER_PLUGIN
	CComBSTR str(ConvertedMsg);
	CCodeAnalyser::pConfigurator->Message(str, mNone);
	str.Empty();
#else
	ConvertedMsg.AnsiToOem();
	printf((LPCSTR)ConvertedMsg);
	printf("\n");
	fflush(stdout);
#endif
#endif

}



//------------------------------------------------------------
void debug(char *msg, ...)
{
	CString ConvertedMsg;
	va_list ap;
	va_start(ap, msg);
	ConvertedMsg.FormatV(msg, ap);
	va_end(ap);

	/*
	FILE* f;
	f = fopen("c:\\plugin.txt", "at");
	fprintf(f, (LPCSTR)ConvertedMsg);
	fprintf(f, "\n");
	fclose(f);
	*/
}
