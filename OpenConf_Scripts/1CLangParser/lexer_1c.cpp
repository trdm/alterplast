
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer_1c.h"
#include "parser.h"
#include "CommonFunctions.h"

char CLexAnalyzer_1C::operators[] = "?,;()=[]+-*/%<>.:";
char CLexAnalyzer_1C::word[] = "àáâãäåæçèéêëìíîïðñòóôõö÷øùýþÿ_ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÝÞß¸¨úÚüÜ";
CLexAnalyzer_1C::TKeyWord CLexAnalyzer_1C::keywords[] = {
	{"Ïðîöåäóðà", proc},
	{"Procedure", proc},
	{"Ôóíêöèÿ", func},
	{"Function", func},
	{"È", and_},
	{"and", and_},
	{"Öèêë", do_},
	{"do", do_},
	{"Èíà÷å", else_},
	{"else", else_},
	{"Èíà÷åÅñëè", elsif},
	{"elsif", elsif},
	{"ÊîíåöÖèêëà", enddo},
	{"enddo", enddo},
	{"ÊîíåöÔóíêöèè", endfunc},
	{"endfunction", endfunc},
	{"ÊîíåöÅñëè", endif},
	{"endif", endif},
	{"ÊîíåöÏðîöåäóðû", endproc},
	{"endprocedure", endproc},
	{"ÊîíåöÏîïûòêè", endtry},
	{"endtry", endtry},
	{"Èñêëþ÷åíèå", except_},
	{"except", except_},
	{"Ýêñïîðò", export_},
	{"export", export_},
	{"Äëÿ", for_},
	{"for", for_},
	{"Äàëåå", forward},
	{"forward", forward},
	{"Åñëè", if_},
	{"if", if_},
	{"ÍÅ", not_},
	{"not", not_},
	{"ÈËÈ", or_},
	{"or", or_},
	{"Âîçâðàò", return_},
	{"return", return_},
	{"Òîãäà", then},
	{"then", then},
	{"Ïî", to_},
	{"to", to_},
	{"Ïîïûòêà", try_},
	{"try", try_},
	{"Çíà÷", val},
	{"val", val},
	{"Ïåðåì", var},
	{"var", var},
	{"Ïîêà", while_},
	{"while", while_},
	{"Ïðåðâàòü", break_},
	{"break", break_},
	{"Ïðîäîëæèòü", continue_},
	{"continue", continue_},
	{"Ïåðåéòè", goto_},
	{"goto", goto_},
	{"Êîíòåêñò", context},
	{"context", context},
	{NULL, 0}
};

int __cdecl CLexAnalyzer_1C::compare(const void *elem1, const void *elem2)
{
	TKeyWord *e1 = (TKeyWord*)elem1;
	TKeyWord *e2 = (TKeyWord*)elem2;
	return stricmp(e1->word, e2->word);
}

int CLexAnalyzer_1C::kw_count = -1;


//========================================================================
//=====   CLexAnalyzer_1C  methods   ==========================================
//========================================================================
CLexAnalyzer_1C::CLexAnalyzer_1C(const char* aText)
{
	Text = (char*)aText;
	ptr = Text;
	token = NULL;
	tok_ptr = token;
	tok_len = 0;
	MemAllocated = 0;
	TokenAlloc(1);

	FirstRun = true;

	if( kw_count < 0 )
	{
		kw_count = 0;
		while( keywords[kw_count].word != NULL ) kw_count++;
		qsort(keywords, kw_count, sizeof(TKeyWord), compare);
	}
}

CLexAnalyzer_1C::~CLexAnalyzer_1C()
{
	if( token )
		free(token);
}


void CLexAnalyzer_1C::TokenAlloc(int len)
{
	if( (len+1) < MemAllocated ) return;
	token = (char*)realloc(token, MemAllocated + LEXEMA_ALLOCATION_GRANULARITY);
	memset(token + MemAllocated, 0, LEXEMA_ALLOCATION_GRANULARITY);
	MemAllocated += LEXEMA_ALLOCATION_GRANULARITY;
	tok_ptr = token + tok_len;
}

void CLexAnalyzer_1C::ajust_location(char c, YYLTYPE* yylloc)
{
	if( c == '\n' )
	{	
		yylloc->last_line++;
		yylloc->last_column = 1;
	}
	else
	{
		yylloc->last_column++;
	}
}
		 
void CLexAnalyzer_1C::AddTokenChar(char c, YYLTYPE* yylloc)
{
	tok_len++;
	TokenAlloc(tok_len);
	*tok_ptr++ = c;
	*tok_ptr = '\0';

	ajust_location(*ptr, yylloc);
}

#ifndef FALSE
#define FALSE (1 == 0)
#define TRUE (0 == 0)
#endif

int CLexAnalyzer_1C::IsWord(unsigned char c)
{
	if( isalpha(c) ) return TRUE;
	if( strchr(word, c) != NULL ) return TRUE;
	return FALSE;
}

int CLexAnalyzer_1C::IsKeyWord(char* token)
{
	TKeyWord key;
	key.word = token;
	TKeyWord *found = (TKeyWord*)bsearch(&key, keywords, kw_count, sizeof(TKeyWord), compare);
	if( found == NULL ) return name;
	return found->id;
}

void CLexAnalyzer_1C::SkipSpaces(YYLTYPE* yylloc)
{
	//printf("space: '");
	while( *ptr != '\0' && isspace((unsigned char)*ptr) ) 
	{
		ajust_location(*ptr, yylloc);
		//printf("%c",*ptr);
		ptr++;
	}
	
	if( *ptr == '/' && *(ptr+1) == '/' )
	{
		while( *ptr != '\0' && *ptr != '\n' ) ptr++;
		SkipSpaces(yylloc);
	}
	//printf("'\n");
}

int CLexAnalyzer_1C::yylex(YYSTYPE* lvalp, YYLTYPE* yylloc)
{
	if( FirstRun )
	{
		yylloc->first_line = 1;
		yylloc->first_column = 1;
		yylloc->last_line = 1;
		yylloc->last_column = 1;
		FirstRun = false;
	}

	tok_len = 0;
	tok_ptr = token;
	*lvalp = NULL;
	
	SkipSpaces(yylloc);
	if( *ptr == '\0' ) return 0;

	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column;
	
	AddTokenChar(*ptr, yylloc);
	
	if( strchr(operators, *ptr) != NULL )
	{
		*lvalp = new CLexema(*yylloc, token, LT_Other);
		return *ptr++;
	}
	
	if( *ptr == '"' )
	{
		while( *++ptr != '\0' )
		{
			AddTokenChar(*ptr, yylloc);
			if( *ptr == '"' )
			{
				if( *(ptr+1) != '"' )
					break;
				else
					AddTokenChar(*++ptr, yylloc);
			}
				
		}
		ptr++;

		*lvalp = new CLexema(*yylloc, token, LT_String);
		return literal;
	}
	
	if( isdigit((unsigned char)*ptr) )
	{
		while( *++ptr != '\0' && (isdigit((unsigned char)*ptr) || *ptr == '.') )
		{
			AddTokenChar(*ptr, yylloc);
		}

		*lvalp = new CLexema(*yylloc, token, LT_Number);
		return number;
	}
	
	if( *ptr == '\'' )
	{
		while( *++ptr != '\0' )
		{
			AddTokenChar(*ptr, yylloc);
			if( *ptr == '\'' ) break;
		}
		ptr++;

		*lvalp = new CLexema(*yylloc, token, LT_Date);
		return date;
	}

	if( *ptr == '~' )
	{
		while( *++ptr != '\0' && IsWord(*ptr) )
		{
			AddTokenChar(*ptr, yylloc);
		}
		*lvalp = new CLexema(*yylloc, token, LT_Label);
		return label;
	}

	if( IsWord(*ptr) )
	{
		while( *++ptr != '\0' && (isdigit((unsigned char)*ptr) || IsWord(*ptr)) )
		{
			AddTokenChar(*ptr, yylloc);
		}
		
		int WordType = IsKeyWord(token);
		CLexema* lex = new CLexema(*yylloc, token, LT_Word);
		if( WordType != name )
		{
			lex->LexType = LT_Keyword;
			lex->KeyWord = WordType;
		}
		*lvalp = lex;

		return WordType;
	}
	
	Msg("Unknown: '%c'", *ptr);
	*lvalp = new CLexema(*yylloc, token, LT_Other);
	return -1;
}

