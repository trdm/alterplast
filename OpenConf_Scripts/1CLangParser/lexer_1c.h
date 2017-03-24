
#ifndef _LEXER_1C_H_
#define _LEXER_1C_H_

#include "parser.h"
#include "lang1c.tab.h"

#define LEXEMA_ALLOCATION_GRANULARITY 512

class CLexAnalyzer_1C
{
private:
	typedef struct {
		char* word;
		int id;
	} TKeyWord;
	static bool TablesInitialized;
	static char operators[];
	static char word[];
	static TKeyWord keywords[];
	static int kw_count;
	static int __cdecl compare(const void *elem1, const void *elem2);

	char *Text;
	char *ptr, *token, *tok_ptr;
	int tok_len;
	int MemAllocated;
	bool FirstRun;

public:
	CLexAnalyzer_1C(const char* aText);
	~CLexAnalyzer_1C();
	int yylex(YYSTYPE* lvalp, YYLTYPE* yylloc);

private:
	void TokenAlloc(int len);
	void ajust_location(char c, YYLTYPE* yylloc);
	void AddTokenChar(char c, YYLTYPE* yylloc);
	int IsWord(unsigned char c);
	int IsKeyWord(char* token);
	void SkipSpaces(YYLTYPE* yylloc);
};

#endif