
/*
Грамматика языка 1С 7.7
(с) А.С. Орефков
Пока вроде учтено все, что было найдено.
Принято несколько упрощений, разбор которых, будем считать,
относиться уже к семантике
*/


%{
	#include "parser.h"

	#define YYERROR_VERBOSE 1

	int yylex(YYSTYPE* lvalp, YYLTYPE* yylloc, CLexAnalyzer_1C* Lexer);
	void yyerror(YYLTYPE* location, CCode** parse_res, CLexAnalyzer_1C* Lexer, char const *s);
%}


%defines
%locations
%pure_parser
%parse-param {CCode** parse_res}
%parse-param {CLexAnalyzer_1C* Lexer}
%lex-param {CLexAnalyzer_1C* Lexer}
%debug

%token
	'?'
	','
	';'
	'('
	')'
	'='
	'['
	']'
	'+'
	'-'
	'*'
	'/'
	'%'
	'<'
	'>'
	'.'
	label		/* Метка ('~ИмяМетки') */
	number		/* Число (без + -) */
	date		/* Дата (любая однострочная последовательность символов между двух апострофов ) */
	literal		/* Строка (для простоты '|asdhsd"' тоже считается строкой) */
	name		/* Любой идентификатор */

	/* Ключевые слова */
	and_			/* И */
	do_			/* Цикл */
	else_		/* Иначе */
	elsif		/* ИначеЕсли */
	enddo		/* КонецЦикла */
	endfunc		/* КонецФункции */
	endif		/* КонецЕсли */
	endproc		/* КонецПроцедуры */
	endtry		/* КонецПопытки */
	except_		/* Исключение */
	export_		/* Экспорт */
	for_		/* Для */
	forward		/* Далее */
	func		/* Функция */
	if_			/* Если */
	not_			/* НЕ */
	or_			/* ИЛИ */
	proc		/* Процедура */
	return_		/* Возврат */
	then		/* Тогда */
	to_			/* По */
	try_		/* Попытка */
	val			/* Знач */
	var			/* Перем */
	while_		/* Пока */
	break_
	continue_
	goto_
	context		/* Контекст */

%left or_ and_
%left '='
%left not_
%left ',' elsif
%left '.'
%left '+' '-'
%left '*' '/' '%'
%left var proc func

%destructor { delete $$; } 
	'?'
	','
	';'
	'('
	')'
	'='
	'['
	']'
	'+'
	'-'
	'*'
	'/'
	'%'
	'<'
	'>'
	'.'
	label		/* Метка ('~ИмяМетки') */
	number		/* Число (без + -) */
	date		/* Дата (любая однострочная последовательность символов между двух апострофов ) */
	literal		/* Строка (для простоты '|asdhsd"' тоже считается строкой) */
	name		/* Любой идентификатор */

	/* Ключевые слова */
	and_			/* И */
	do_			/* Цикл */
	else_		/* Иначе */
	elsif		/* ИначеЕсли */
	enddo		/* КонецЦикла */
	endfunc		/* КонецФункции */
	endif		/* КонецЕсли */
	endproc		/* КонецПроцедуры */
	endtry		/* КонецПопытки */
	except_		/* Исключение */
	export_		/* Экспорт */
	for_		/* Для */
	forward		/* Далее */
	func		/* Функция */
	if_			/* Если */
	not_			/* НЕ */
	or_			/* ИЛИ */
	proc		/* Процедура */
	return_		/* Возврат */
	then		/* Тогда */
	to_			/* По */
	try_		/* Попытка */
	val			/* Знач */
	var			/* Перем */
	while_		/* Пока */
	break_
	continue_
	goto_
	context		/* Контекст */

	//MODUL VARS VAR VARIABLES VARIABLE METHODS 
	METHOD METHEADER PARAMLIST PARAMS PARAM
	MVARS MVAR MVARIABLES MVARIABLE
	STATEMENTS STATEMENT OPERATOR
	LVAL METHCALL SUBMETHCALL MEMBNAME
	REALPARAMS REALPARAM
	TRYBLOCK IFBLOCK ELSEIFBLOCK ELSEBLOCK WHILEBLOCK FORBLOCK
	LOGIC LOGICP COMPARE
	EXPRESSION EXPRBIN EXPRUN OPERATION
	LITERAL CONST_ CONSTANT



%%
MODUL		:VARS METHODS STATEMENTS
			{
				$$ = new CCode(@$);
				$$->CodeType = CT_Module;
				$$->AddTail(3, $1, $2, $3);
				*parse_res = $$;
			}
			;

VARS		:VAR
			{ $$ = new CCode(@$, CT_VarDeclList); $$->AddTail(1, $1); }
			|VARS VAR
			{ $1->AddTail(1, $2); $1->SetLoc(@$); }
			|/*empty*/%prec ',' 
			{ $$ = NULL; }
			;
VAR			:var VARIABLES ';'
			{ $$ = new CCode(@$, CT_VarDecl); $$->AddTail(3, $1, $2, $3); }
			;
VARIABLES	:VARIABLE
			{ $$ = new CCode(@$, CT_VarDefList); $$->AddTail(1, $1); }
			|VARIABLES ',' VARIABLE
			{ $1->AddTail(2, $2, $3); $1->SetLoc(@$); }
			;
VARIABLE	:name
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail(1, $1); }
			|name export_
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail(2, $1, $2); }
			|name '[' number ']'
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail(4, $1, $2, $3, $4); }
			|name '[' number ']' export_
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail(4, $1, $2, $3, $4, $5); }
			;
METHODS		:METHOD
			{ $$ = new CCode(@$, CT_MethList); $$->AddTail(1, $1); }
			|METHODS METHOD
			{ $1->AddTail($2); $1->SetLoc(@$); }
			|/*empty*/%prec ',' 
			{ $$ = NULL; }
			;
METHOD		:proc METHEADER MVARS STATEMENTS endproc
			{
				$$ = new CCode(@$, CT_Procedure);
				$$->AddTail(5, $1, $2, $3, $4, $5);
			}
			///*
			|proc METHEADER MVARS STATEMENTS error endproc
			{
				$$ = new CCode(@$, CT_Procedure);
				$$->AddTail(5, $1, $2, $3, $4, $6); 
			}
			//*/
			|proc METHEADER error endproc
			{
				$$ = new CCode(@$, CT_Error);
				delete $1;
				delete $2;
				delete $4; 
			}
			|func METHEADER MVARS STATEMENTS endfunc
			{ $$ = new CCode(@$, CT_Function); $$->AddTail(5, $1, $2, $3, $4, $5);}
			|func METHEADER error endfunc
			{
				$$ = new CCode(@$, CT_Error);
				delete $1;
				delete $2;
				delete $4; 
			}
			|proc METHEADER forward
			{ $$ = new CCode(@$, CT_Procedure); $$->AddTail(3, $1, $2, $3); }
			|func METHEADER forward
			{ $$ = new CCode(@$, CT_Function); $$->AddTail(3, $1, $2, $3); }
			;
METHEADER	:name PARAMLIST 
			{ $$ = new CCode(@$, CT_MethHeader); $$->AddTail(2, $1, $2); }
			|name PARAMLIST export_
			{ $$ = new CCode(@$, CT_MethHeader); $$->AddTail(3, $1, $2, $3); }
			;
PARAMLIST	:'(' PARAMS ')'
			{ $$ = $2; $$->AddHead($1); $$->AddTail($3); $1->SetLoc(@$); }
			|'(' ')'
			{ $$ = new CCode(@$, CT_ParamList); $$->AddTail(2, $1, $2); }
			;
PARAMS		:PARAM
			{ $$ = new CCode(@$, CT_ParamList); $$->AddTail($1); }
			|PARAMS ',' PARAM
			{ $1->AddTail(2, $2, $3); $1->SetLoc(@$); }

PARAM		:name
			{ $$ = new CCode(@$, CT_Param); $$->AddTail($1); }
			|val name
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(2, $1, $2); }
			|name '='			/* Как ни странно, 1С не ругается на такую конструкцию */
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(2, $1, $2); }
			|val name '='
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(3, $1, $2, $3); }
			|name '=' CONSTANT
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(3, $1, $2, $3); }
			|val name '=' CONSTANT
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(4, $1, $2, $3, $4); }
			|name '[' ']'
			{ $$ = new CCode(@$, CT_Param); $$->AddTail(3, $1, $2, $3); }
			;
/* В методе нельзя использовать Экспорт */
MVARS		:MVAR
			{ $$ = new CCode(@$, CT_VarDeclList); $$->AddTail($1); }
			|MVARS MVAR
			{ $1->AddTail($2); $1->SetLoc(@$); }
			|/*empty*/%prec ',' 
			{ $$ = NULL; }
			;
MVAR		:var MVARIABLES ';'
			{ $$ = new CCode(@$, CT_VarDecl); $$->AddTail(3, $1, $2, $3); }
			;
MVARIABLES	:MVARIABLE
			{ $$ = new CCode(@$, CT_VarDefList); $$->AddTail($1); }
			|MVARIABLES ',' MVARIABLES
			{ $1->AddTail(2, $2, $3); $1->SetLoc(@$); }

MVARIABLE	:name
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail($1); }
			|name '[' number ']'
			{ $$ = new CCode(@$, CT_VarDef); $$->AddTail(4, $1, $2, $3, $4); }
			;
STATEMENTS	:STATEMENT
			{ $$ = new CCode(@$, CT_Statements); $$->AddTail($1); }
			|STATEMENTS ';' STATEMENT
			{
				CCode* PrevStmt = $1->GetLast();
				PrevStmt->AddTail($2);
				PrevStmt->EndLine = @2.last_line;
				PrevStmt->EndCol = @2.last_column - 1;
				$1->AddTail($3);
				$1->SetLoc(@$);
			}
			|STATEMENTS error ';' STATEMENT
			{
				CCode* PrevStmt = $1->GetLast();
				PrevStmt->AddTail($3);
				PrevStmt->EndLine = @3.last_line;
				PrevStmt->EndCol = @3.last_column - 1;
				$1->AddTail($4);
				$1->SetLoc(@$);
			}
			;
STATEMENT	:OPERATOR
			{ $$ = new CCode(@$, CT_Statement); $$->AddTail($1); }
			|label ':' OPERATOR
			{ $$ = new CCode(@$, CT_Statement); $$->AddTail(3, $1, $2, $3); }
			| //empty
			{
				//меняем позицию, а то она берётся от предыдущей конструкции
				//@$.first_line = @$.last_line;
				//@$.first_column = @$.last_column - 1;
				CLexema* lex = new CLexema(@$, "", LT_Other); //пустая лексема - для однородности
				$$ = new CCode(@$); 
				$$->AddTail(lex);
			}
			;

OPERATOR	:LVAL '=' EXPRESSION
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail(3, $1, $2, $3); }
			|LVAL
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|METHCALL
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|TRYBLOCK
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|IFBLOCK
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|WHILEBLOCK
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|FORBLOCK
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			/* для простоты запишем так, будем считать, что допустимость этих
			   операторов проверяется семантически */
			|return_
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|return_ EXPRESSION
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail(2, $1, $2); }
			|break_
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|continue_
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail($1); }
			|goto_ label
			{ $$ = new CCode(@$, CT_Operator); $$->AddTail(2, $1, $2); }
			;
LVAL		:name 
			{ $$ = new CCode(@$, CT_LValue); $$->AddTail($1); }
			|name '[' EXPRESSION ']'
			{ $$ = new CCode(@$, CT_LValue); $$->AddTail(4, $1, $2, $3, $4); }
			|LVAL '.' MEMBNAME
			{ $$ = new CCode(@$, CT_LValue); $$->AddTail(3, $1, $2, $3); }
			|LVAL '.' error
			{ $$ = $1; delete $2; }
			|METHCALL '.' MEMBNAME
			{ $$ = new CCode(@$, CT_LValue); $$->AddTail(3, $1, $2, $3); }
			;
METHCALL	:name '(' REALPARAMS ')' 
			{ $$ = new CCode(@$, CT_MethCall); $$->AddTail(4, $1, $2, $3, $4); }
			|METHCALL '.' SUBMETHCALL
			{ $$ = new CCode(@$, CT_MethCall); $$->AddTail(3, $1, $2, $3); }
			|METHCALL '.' error
			{ $$ = $1; delete $2; }
			|LVAL '.' SUBMETHCALL
			{ $$ = new CCode(@$, CT_MethCall); $$->AddTail(3, $1, $2, $3); }
			;
SUBMETHCALL :MEMBNAME '(' REALPARAMS ')'
			{ $$ = new CCode(@$, CT_MethCall); $$->AddTail(4, $1, $2, $3, $4); }

/*
Оказывается, в качестве имени метода/свойства объекта можно использовать
любое ключевое слово.
Например,
а=таб.Если;
б=таб.Для();
Потенциальный источник ошибок.
Например,
а=тз.
Возврат;
не вызовет синтаксическую ошибку
*/				
MEMBNAME	:name
			/*
			|and_
			|do_
			|else_
			|elsif
			|enddo
			//|endfunc
			|endif
			//|endproc
			|endtry
			|except_
			|export_
			|for_
			|forward
			|func
			//|if_
			|not_
			|or_
			|proc
			|return_
			|then
			|to_
			|try_
			|val
			|var
			|while_
			|context
			*/
			;
REALPARAMS	:REALPARAM
			{ $$ = new CCode(@$, CT_CallParamList); $$->AddTail($1); }
			|REALPARAMS ',' REALPARAM
			{ $1->AddTail(2, $2, $3); $1->SetLoc(@$); }
			;
REALPARAM	:EXPRESSION
			{ $$ = new CCode(@$, CT_CallParam); $$->AddTail($1); }
			|context
			{ $$ = new CCode(@$, CT_CallParam); $$->AddTail($1); }
			|//empty
			{
				@$.first_line = @$.last_line;
				@$.first_column = @$.last_column - 1;
				CLexema* lex = new CLexema(@$, "", LT_Other);
				$$ = new CCode(@$, CT_CallParam);
				$$->AddTail(lex);
			}
			;
TRYBLOCK	:try_ STATEMENTS except_ STATEMENTS endtry
			{ $$ = new CCode(@$, CT_Try); $$->AddTail(5, $1, $2, $3, $4, $5); }
			;
IFBLOCK		:if_ LOGIC then STATEMENTS endif
			{ $$ = new CCode(@$, CT_If); $$->AddTail(5, $1, $2, $3, $4, $5); }
			|if_ LOGIC then STATEMENTS ELSEBLOCK endif
			{ $$ = new CCode(@$, CT_If); $$->AddTail(6, $1, $2, $3, $4, $5, $6); }
			|if_ LOGIC then STATEMENTS ELSEIFBLOCK endif
			{ $$ = new CCode(@$, CT_If); $$->AddTail(6, $1, $2, $3, $4, $5, $6); }
			|if_ LOGIC then STATEMENTS ELSEIFBLOCK ELSEBLOCK endif
			{ $$ = new CCode(@$, CT_If); $$->AddTail(7, $1, $2, $3, $4, $5, $6, $7); }
			;
ELSEIFBLOCK	:elsif LOGIC then STATEMENTS
			{ $$ = new CCode(@$, CT_ElseIf); $$->AddTail(4, $1, $2, $3, $4); }
			|ELSEIFBLOCK ELSEIFBLOCK %prec '+'
			{ $$ = new CCode(@$, CT_ElseIf); $$->AddTail(2, $1, $2); }
			;
ELSEBLOCK	:else_ STATEMENTS
			{ $$ = new CCode(@$, CT_Else); $$->AddTail(2, $1, $2); }
			;
WHILEBLOCK	:while_ LOGIC do_ STATEMENTS enddo
			{ $$ = new CCode(@$, CT_While); $$->AddTail(5, $1, $2, $3, $4, $5); }
			;
FORBLOCK	:for_ name '=' EXPRESSION to_ EXPRESSION do_ STATEMENTS enddo
			{ $$ = new CCode(@$, CT_For); $$->AddTail(9, $1, $2, $3, $4, $5, $6, $7, $8, $9); }
			;
LOGIC		:EXPRESSION COMPARE EXPRESSION
			{ $$ = new CCode(@$, CT_LogicalExpr); $$->AddTail(3, $1, $2, $3); }
			|LOGICP
			|EXPRESSION COMPARE error EXPRESSION
			{
				$$ = new CCode(@$, CT_LogicalExpr);
				$$->AddTail(3, $1, $2, $4);
			}
			;
LOGICP		:'(' LOGIC ')'
			{ $$ = new CCode(@$, CT_LogicalExpr); $$->AddTail(3, $1, $2, $3); }
			|not_ '(' LOGIC ')'
			{ $$ = new CCode(@$, CT_LogicalExpr); $$->AddTail(4, $1, $2, $3, $4); }
			|LOGICP and_ LOGICP
			{ $$ = new CCode(@$, CT_LogicalExpr); $$->AddTail(3, $1, $2, $3); }
			|LOGICP or_ LOGICP
			{ $$ = new CCode(@$, CT_LogicalExpr); $$->AddTail(3, $1, $2, $3); }
			;
COMPARE		:'='
			|'<'
			|'>'
			|'>' '=' { $$ = new CLexema(@$, ">=", LT_Other); delete $1; delete $2; }
			|'<' '=' { $$ = new CLexema(@$, "<=", LT_Other); delete $1; delete $2; }
			|'<' '>' { $$ = new CLexema(@$, "<>", LT_Other); delete $1; delete $2; }
			;
EXPRESSION	:EXPRBIN
			|EXPRUN
			|EXPRESSION OPERATION EXPRBIN
			{
				$1->AddTail(2, $2, $3);
				$1->SetLoc(@$);
			}
			|EXPRESSION OPERATION error EXPRBIN
			{
				$1->AddTail(2, $2, $4);
				$1->SetLoc(@$);
			}
			|EXPRESSION error EXPRBIN
			{
				$$ = $1;
				$1->AddTail($3);
				$$->SetLoc(@$);
			}
			;
EXPRBIN		:METHCALL
			|LVAL
			{$$ = new CCode(@$); $$->AddTail($1);}
			|CONST_ 
			{$$ = new CCode(@$); $$->AddTail($1);}
			|number 
			{$$ = new CCode(@$); $$->AddTail($1);}
			|'?' '(' LOGIC ',' EXPRESSION ',' EXPRESSION ')'
			{
				$$ = new CCode(@$, CT_Expr);
				$$->AddTail(8, $1, $2, $3, $4, $5, $6, $7, $8);
			}
			|'(' EXPRESSION ')'
			{
				$2->AddHead($1);
				$2->AddTail($3);
				$2->SetLoc(@$);
				$$ = $2;
			}
			;
EXPRUN		:'+' EXPRBIN 
			{ $2->AddHead($1); $$ = $2; }
			|'-' EXPRBIN
			{$2->AddHead($1); $$ = $2;}
			;
OPERATION	:'+'
			|'-'
			|'*'
			|'/'
			|'%'
			;
/*
Что бы там не писалось в ЖКК, идущие друг за другом строки,
независимо, на разных они строках или нет, объединяются.
т.е.
Сообщить("ааааа"  "ббббб");
выдаст
ааааа
ббббб
*/
LITERAL		:literal
			|LITERAL literal 
			{
				CLexema* lex = (CLexema*)$1;
				lex->Text += $2->GetText();
				$2->SetLoc(@$);
				delete $2;
			}
			;
CONST_		:LITERAL
			|date
			;
CONSTANT	:CONST_
			|number
			|'+' number 
			{
				CLexema* lex = (CLexema*)$1;
				lex->Text += $2->GetText();
				lex->SetLoc(@$);
				delete $2;
			}
			|'-' number 
			{
				CLexema* lex = (CLexema*)$1;
				lex->Text += $2->GetText();
				lex->SetLoc(@$);
				delete $2;
			}
			;
			
%%


/* The lexical analyzer returns quoted string (without quotes) or single token.
   It skips all blanks and tabs, and returns 0 for end-of-input. 
*/
int yylex(YYSTYPE* lvalp, YYLTYPE* yylloc, CLexAnalyzer_1C* Lexer)
{
	return Lexer->yylex(lvalp, yylloc);
}

 // Called by yyparse on error.
void yyerror(YYLTYPE* location, CCode** parse_res, CLexAnalyzer_1C* Lexer, char const *s)
{
	printf ("%s - Line %i Column %i\n", s, location->first_line, location->first_column);
}
