/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     label = 258,
     number = 259,
     date = 260,
     literal = 261,
     name = 262,
     and_ = 263,
     do_ = 264,
     else_ = 265,
     elsif = 266,
     enddo = 267,
     endfunc = 268,
     endif = 269,
     endproc = 270,
     endtry = 271,
     except_ = 272,
     export_ = 273,
     for_ = 274,
     forward = 275,
     func = 276,
     if_ = 277,
     not_ = 278,
     or_ = 279,
     proc = 280,
     return_ = 281,
     then = 282,
     to_ = 283,
     try_ = 284,
     val = 285,
     var = 286,
     while_ = 287,
     break_ = 288,
     continue_ = 289,
     goto_ = 290,
     context = 291
   };
#endif
/* Tokens.  */
#define label 258
#define number 259
#define date 260
#define literal 261
#define name 262
#define and_ 263
#define do_ 264
#define else_ 265
#define elsif 266
#define enddo 267
#define endfunc 268
#define endif 269
#define endproc 270
#define endtry 271
#define except_ 272
#define export_ 273
#define for_ 274
#define forward 275
#define func 276
#define if_ 277
#define not_ 278
#define or_ 279
#define proc 280
#define return_ 281
#define then 282
#define to_ 283
#define try_ 284
#define val 285
#define var 286
#define while_ 287
#define break_ 288
#define continue_ 289
#define goto_ 290
#define context 291




/* Copy the first part of user declarations.  */
#line 11 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"

	#include "parser.h"

	#define YYERROR_VERBOSE 1

	int yylex(YYSTYPE* lvalp, YYLTYPE* yylloc, CLexAnalyzer_1C* Lexer);
	void yyerror(YYLTYPE* location, CCode** parse_res, CLexAnalyzer_1C* Lexer, char const *s);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 199 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   513

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNRULES -- Number of states.  */
#define YYNSTATES  223

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    15,     2,     2,
       6,     7,    13,    11,     4,    12,    18,    14,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    53,     5,
      16,     8,    17,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     9,     2,    10,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,     9,    12,    13,    17,    19,    23,
      25,    28,    33,    39,    41,    44,    45,    51,    58,    63,
      69,    74,    78,    82,    85,    89,    93,    96,    98,   102,
     104,   107,   110,   114,   118,   123,   127,   129,   132,   133,
     137,   139,   143,   145,   150,   152,   156,   161,   163,   167,
     168,   172,   174,   176,   178,   180,   182,   184,   186,   189,
     191,   193,   196,   198,   203,   207,   211,   215,   220,   224,
     228,   232,   237,   239,   241,   245,   247,   249,   250,   256,
     262,   269,   276,   284,   289,   292,   295,   301,   311,   315,
     317,   322,   326,   331,   335,   339,   341,   343,   345,   348,
     351,   354,   356,   358,   362,   367,   371,   373,   375,   377,
     379,   388,   392,   395,   398,   400,   402,   404,   406,   408,
     410,   413,   415,   417,   419,   421,   424
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    56,    60,    70,    -1,    57,    -1,    56,
      57,    -1,    -1,    47,    58,     5,    -1,    59,    -1,    58,
       4,    59,    -1,    23,    -1,    23,    34,    -1,    23,     9,
      20,    10,    -1,    23,     9,    20,    10,    34,    -1,    61,
      -1,    60,    61,    -1,    -1,    41,    62,    66,    70,    31,
      -1,    41,    62,    66,    70,     1,    31,    -1,    41,    62,
       1,    31,    -1,    37,    62,    66,    70,    29,    -1,    37,
      62,     1,    29,    -1,    41,    62,    36,    -1,    37,    62,
      36,    -1,    23,    63,    -1,    23,    63,    34,    -1,     6,
      64,     7,    -1,     6,     7,    -1,    65,    -1,    64,     4,
      65,    -1,    23,    -1,    46,    23,    -1,    23,     8,    -1,
      46,    23,     8,    -1,    23,     8,    94,    -1,    46,    23,
       8,    94,    -1,    23,     9,    10,    -1,    67,    -1,    66,
      67,    -1,    -1,    47,    68,     5,    -1,    69,    -1,    68,
       4,    68,    -1,    23,    -1,    23,     9,    20,    10,    -1,
      71,    -1,    70,     5,    71,    -1,    70,     1,     5,    71,
      -1,    72,    -1,    19,    53,    72,    -1,    -1,    73,     8,
      88,    -1,    73,    -1,    74,    -1,    79,    -1,    80,    -1,
      83,    -1,    84,    -1,    42,    -1,    42,    88,    -1,    49,
      -1,    50,    -1,    51,    19,    -1,    23,    -1,    23,     9,
      88,    10,    -1,    73,    18,    76,    -1,    73,    18,     1,
      -1,    74,    18,    76,    -1,    23,     6,    77,     7,    -1,
      74,    18,    75,    -1,    74,    18,     1,    -1,    73,    18,
      75,    -1,    76,     6,    77,     7,    -1,    23,    -1,    78,
      -1,    77,     4,    78,    -1,    88,    -1,    52,    -1,    -1,
      45,    70,    33,    70,    32,    -1,    38,    85,    43,    70,
      30,    -1,    38,    85,    43,    70,    82,    30,    -1,    38,
      85,    43,    70,    81,    30,    -1,    38,    85,    43,    70,
      81,    82,    30,    -1,    27,    85,    43,    70,    -1,    81,
      81,    -1,    26,    70,    -1,    48,    85,    25,    70,    28,
      -1,    35,    23,     8,    88,    44,    88,    25,    70,    28,
      -1,    88,    87,    88,    -1,    86,    -1,    88,    87,     1,
      88,    -1,     6,    85,     7,    -1,    39,     6,    85,     7,
      -1,    86,    24,    86,    -1,    86,    40,    86,    -1,     8,
      -1,    16,    -1,    17,    -1,    17,     8,    -1,    16,     8,
      -1,    16,    17,    -1,    89,    -1,    90,    -1,    88,    91,
      89,    -1,    88,    91,     1,    89,    -1,    88,     1,    89,
      -1,    74,    -1,    73,    -1,    93,    -1,    20,    -1,     3,
       6,    85,     4,    88,     4,    88,     7,    -1,     6,    88,
       7,    -1,    11,    89,    -1,    12,    89,    -1,    11,    -1,
      12,    -1,    13,    -1,    14,    -1,    15,    -1,    22,    -1,
      92,    22,    -1,    92,    -1,    21,    -1,    93,    -1,    20,
      -1,    11,    20,    -1,    12,    20,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   160,   160,   169,   171,   173,   176,   179,   181,   184,
     186,   188,   190,   193,   195,   197,   200,   206,   212,   219,
     221,   228,   230,   233,   235,   238,   240,   243,   245,   248,
     250,   252,   254,   256,   258,   260,   264,   266,   268,   271,
     274,   276,   279,   281,   284,   286,   295,   305,   307,   310,
     320,   322,   324,   326,   328,   330,   332,   336,   338,   340,
     342,   344,   347,   349,   351,   353,   355,   358,   360,   362,
     364,   367,   382,   412,   414,   417,   419,   422,   430,   433,
     435,   437,   439,   442,   444,   447,   450,   453,   456,   458,
     459,   465,   467,   469,   471,   474,   475,   476,   477,   478,
     479,   481,   482,   483,   488,   493,   500,   501,   503,   505,
     507,   512,   520,   522,   525,   526,   527,   528,   529,   540,
     541,   549,   550,   552,   553,   554,   561
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'?'", "','", "';'", "'('", "')'", "'='",
  "'['", "']'", "'+'", "'-'", "'*'", "'/'", "'%'", "'<'", "'>'", "'.'",
  "label", "number", "date", "literal", "name", "and_", "do_", "else_",
  "elsif", "enddo", "endfunc", "endif", "endproc", "endtry", "except_",
  "export_", "for_", "forward", "func", "if_", "not_", "or_", "proc",
  "return_", "then", "to_", "try_", "val", "var", "while_", "break_",
  "continue_", "goto_", "context", "':'", "$accept", "MODUL", "VARS",
  "VAR", "VARIABLES", "VARIABLE", "METHODS", "METHOD", "METHEADER",
  "PARAMLIST", "PARAMS", "PARAM", "MVARS", "MVAR", "MVARIABLES",
  "MVARIABLE", "STATEMENTS", "STATEMENT", "OPERATOR", "LVAL", "METHCALL",
  "SUBMETHCALL", "MEMBNAME", "REALPARAMS", "REALPARAM", "TRYBLOCK",
  "IFBLOCK", "ELSEIFBLOCK", "ELSEBLOCK", "WHILEBLOCK", "FORBLOCK", "LOGIC",
  "LOGICP", "COMPARE", "EXPRESSION", "EXPRBIN", "EXPRUN", "OPERATION",
  "LITERAL", "CONST_", "CONSTANT", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    63,    44,    59,    40,    41,    61,    91,
      93,    43,    45,    42,    47,    37,    60,    62,    46,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    56,    56,    56,    57,    58,    58,    59,
      59,    59,    59,    60,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    66,    66,    66,    67,
      68,    68,    69,    69,    70,    70,    70,    71,    71,    71,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    73,    73,    73,    73,    74,    74,    74,
      74,    75,    76,    77,    77,    78,    78,    78,    79,    80,
      80,    80,    80,    81,    81,    82,    83,    84,    85,    85,
      85,    86,    86,    86,    86,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    88,    89,    89,    89,    89,
      89,    89,    90,    90,    91,    91,    91,    91,    91,    92,
      92,    93,    93,    94,    94,    94,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     2,     0,     3,     1,     3,     1,
       2,     4,     5,     1,     2,     0,     5,     6,     4,     5,
       4,     3,     3,     2,     3,     3,     2,     1,     3,     1,
       2,     2,     3,     3,     4,     3,     1,     2,     0,     3,
       1,     3,     1,     4,     1,     3,     4,     1,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     2,     1,     4,     3,     3,     3,     4,     3,     3,
       3,     4,     1,     1,     3,     1,     1,     0,     5,     5,
       6,     6,     7,     4,     2,     2,     5,     9,     3,     1,
       4,     3,     4,     3,     3,     1,     1,     1,     2,     2,
       2,     1,     1,     3,     4,     3,     1,     1,     1,     1,
       8,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     0,    15,     3,     9,     0,     7,     1,     0,
       0,     4,    49,    13,     0,    10,     0,     6,     0,     0,
       0,     0,    62,     0,     0,    57,    49,     0,    59,    60,
       0,    14,     0,    44,    47,    51,    52,    53,    54,    55,
      56,     0,     8,     0,    23,     0,    22,     0,    49,    36,
       0,    21,    49,     0,    77,     0,     0,     0,     0,     0,
       0,   109,   122,   119,     0,   107,   106,     0,    89,     0,
     101,   102,   121,   108,     0,     0,     0,     0,    61,     0,
      49,     0,     0,     0,    11,    26,    29,     0,     0,    27,
      24,    20,    42,     0,    40,    37,     0,    18,     0,    48,
      76,     0,    73,     0,     0,     0,     0,     0,     0,   112,
     113,     0,    49,     0,     0,     0,    95,   114,   115,   116,
     117,   118,    96,    97,     0,     0,   120,     0,    49,    49,
      49,    45,     0,    65,    72,    70,    64,    69,    68,    66,
      12,    31,     0,    30,     0,    25,     0,     0,    39,    19,
       0,    16,    77,    67,    63,     0,     0,    91,   111,     0,
       0,     0,    93,    94,   105,    99,   100,    98,     0,     0,
       0,   103,     0,     0,    46,    77,     0,     0,   124,   123,
      33,    35,    32,    28,     0,    41,    17,    74,     0,     0,
      92,    49,     0,    79,     0,     0,     0,   104,    78,    86,
       0,   125,   126,    34,    43,     0,     0,     0,     0,    81,
      84,     0,    80,    71,    49,     0,    49,    82,     0,     0,
       0,    87,   110
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,     6,     7,    12,    13,    19,    44,
      88,    89,    48,    49,    93,    94,    32,    33,    34,    65,
      66,   135,   136,   101,   102,    37,    38,   210,   195,    39,
      40,   107,    68,   124,    69,    70,    71,   125,    72,    73,
     180
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -141
static const yytype_int16 yypact[] =
{
     -22,     4,    37,   114,  -141,    38,   137,  -141,  -141,    50,
      50,  -141,   279,  -141,    58,  -141,     4,  -141,    93,   139,
     193,    44,   105,   101,    63,   404,   313,    63,  -141,  -141,
      94,  -141,    91,  -141,  -141,    10,   113,  -141,  -141,  -141,
    -141,   133,  -141,     3,   120,   134,  -141,   136,   296,  -141,
     138,  -141,   296,   331,    42,   404,   165,   172,    63,   414,
     414,  -141,  -141,  -141,   179,   153,   113,   148,    64,   448,
    -141,  -141,   181,  -141,   404,   246,    54,   188,  -141,   196,
     313,   404,    56,    66,   183,  -141,   191,   192,   163,  -141,
    -141,  -141,   197,   205,  -141,  -141,    76,  -141,    75,  -141,
    -141,   176,  -141,   465,   440,   404,    63,   223,   431,  -141,
    -141,    63,   313,     7,     7,   414,  -141,  -141,  -141,  -141,
    -141,  -141,    90,   225,   391,   233,  -141,   474,   313,   313,
     313,  -141,   280,  -141,  -141,  -141,   231,  -141,  -141,   231,
    -141,   127,   235,   254,    -8,  -141,   243,   136,  -141,  -141,
      19,  -141,    42,  -141,  -141,     8,   260,  -141,  -141,   258,
     222,    63,  -141,  -141,  -141,  -141,  -141,  -141,   404,   121,
     414,  -141,    29,   122,  -141,    42,   247,   248,  -141,  -141,
    -141,  -141,   127,  -141,   256,  -141,  -141,  -141,   404,   404,
    -141,   313,    63,  -141,   130,   239,   207,  -141,  -141,  -141,
     189,  -141,  -141,  -141,  -141,   376,   489,   107,   227,  -141,
    -141,   241,  -141,  -141,   313,   404,   313,  -141,   125,   498,
     348,  -141,  -141
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,  -141,   281,  -141,   266,  -141,   271,   276,  -141,
    -141,   143,   268,   -36,   142,  -141,   -19,   -74,   237,   -12,
      -9,   213,   214,   124,   149,  -141,  -141,   140,   109,  -141,
    -141,   -16,   112,  -141,   -23,   -55,  -141,  -141,  -141,  -140,
     123
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -91
static const yytype_int16 yytable[] =
{
      35,   179,    75,    36,   109,   110,   131,    76,    67,   115,
      85,    77,    95,   161,    35,    86,    95,    36,    81,   117,
     118,   119,   120,   121,   130,     1,    86,     5,    82,    96,
      79,   103,   104,    98,    80,   108,    35,     8,    87,    36,
      35,    35,   179,    36,    36,    57,    64,    14,    74,    87,
     186,   127,   188,    59,    60,    79,   174,   133,   132,    80,
     164,   198,    61,    62,    63,    22,    57,   137,    35,    58,
     171,    36,    15,    18,    59,    60,   150,    79,    41,   134,
      80,    80,   155,    61,    62,    63,    22,   128,   113,   134,
     156,    -2,    79,   160,   100,   159,    80,    53,   165,    43,
      35,   169,    64,    36,   114,   149,   151,   166,    79,   172,
     173,    54,    80,    78,    55,   197,    35,    35,    35,    36,
      36,    36,   115,    79,    56,   -88,    79,    80,   -88,   103,
      80,    83,   117,   118,   119,   120,   121,   -85,   176,   177,
      45,    16,    17,    84,   -38,   196,   -88,   178,    62,    63,
     199,     9,   103,   221,    90,    10,   191,   192,   -38,    92,
     209,     1,   -38,    91,   -88,   205,   206,   144,   -38,    97,
     145,    82,   207,   105,   -38,    46,   208,   -38,   106,    35,
     152,   -38,    36,   153,   -38,   111,    47,   -38,   -38,   -38,
     -38,   112,   219,   152,    50,   218,   213,   220,   -38,   141,
     142,   130,    35,   126,    35,    36,   146,    36,   115,   147,
     148,   -90,   -38,   129,   -90,   143,   -38,   140,   117,   118,
     119,   120,   121,    79,   -38,   162,   163,    80,   -38,    51,
     157,   -38,   -90,   167,   170,   -38,    57,   175,   -38,    74,
      47,   -38,   -38,   -38,   -38,   181,   -58,   115,   191,   192,
     -90,   -58,   193,    61,    62,    63,    22,   117,   118,   119,
     120,   121,   182,   184,   189,   190,   204,   201,   202,   212,
     216,   217,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -50,   115,    42,    31,    11,   -50,    20,   183,    52,   185,
      99,   117,   118,   119,   120,   121,   138,   139,    21,   200,
     194,   187,    22,   211,     0,   203,   -50,   -50,   -50,   -50,
     -50,   -50,   -50,   -50,    23,    21,     9,    24,     0,    22,
      10,    25,     0,     0,    26,     0,     0,    27,    28,    29,
      30,    23,    21,     0,    24,     0,    22,     0,    25,     0,
       0,    26,     0,    47,    27,    28,    29,    30,    23,    79,
       0,    24,     0,    80,    22,    25,     0,     0,    26,     0,
       0,    27,    28,    29,    30,     0,    23,     0,     0,    24,
       0,     0,     0,    25,   -83,   -83,    26,   115,   -83,    27,
      28,    29,    30,     0,     0,     0,     0,   117,   118,   119,
     120,   121,   168,     0,    57,     0,     0,    74,     0,     0,
       0,   214,    59,    60,     0,     0,     0,    57,     0,     0,
      74,    61,    62,    63,    22,    59,    60,    57,     0,     0,
      74,     0,     0,     0,    61,    62,    63,    22,     0,     0,
       0,     0,   115,     0,    61,    62,    63,    22,   158,   116,
       0,   115,   117,   118,   119,   120,   121,   122,   123,   115,
     154,   117,   118,   119,   120,   121,   116,     0,     0,   117,
     118,   119,   120,   121,   122,   123,   115,     0,     0,   -75,
       0,     0,   -75,     0,     0,   115,   117,   118,   119,   120,
     121,   158,     0,     0,     0,   117,   118,   119,   120,   121,
     115,     0,     0,   215,     0,     0,     0,     0,     0,   115,
     117,   118,   119,   120,   121,   222,     0,     0,     0,   117,
     118,   119,   120,   121
};

static const yytype_int16 yycheck[] =
{
      12,   141,    25,    12,    59,    60,    80,    26,    24,     1,
       7,    27,    48,     6,    26,    23,    52,    26,     8,    11,
      12,    13,    14,    15,     5,    47,    23,    23,    18,    48,
       1,    54,    55,    52,     5,    58,    48,     0,    46,    48,
      52,    53,   182,    52,    53,     3,    39,     9,     6,    46,
      31,    74,    44,    11,    12,     1,   130,     1,    81,     5,
     115,    32,    20,    21,    22,    23,     3,     1,    80,     6,
     125,    80,    34,    23,    11,    12,     1,     1,    20,    23,
       5,     5,   105,    20,    21,    22,    23,    33,    24,    23,
     106,     0,     1,   112,    52,   111,     5,    53,     8,     6,
     112,   124,    39,   112,    40,    29,    31,    17,     1,   128,
     129,     6,     5,    19,     9,   170,   128,   129,   130,   128,
     129,   130,     1,     1,    23,     4,     1,     5,     7,   152,
       5,    18,    11,    12,    13,    14,    15,    30,    11,    12,
       1,     4,     5,    10,     5,   168,    25,    20,    21,    22,
      28,    37,   175,    28,    34,    41,    26,    27,    19,    23,
      30,    47,    23,    29,    43,   188,   189,     4,    29,    31,
       7,    18,   191,     8,    35,    36,   192,    38,     6,   191,
       4,    42,   191,     7,    45,     6,    47,    48,    49,    50,
      51,    43,   215,     4,     1,   214,     7,   216,     5,     8,
       9,     5,   214,    22,   216,   214,     9,   216,     1,     4,
       5,     4,    19,    25,     7,    23,    23,    34,    11,    12,
      13,    14,    15,     1,    31,   113,   114,     5,    35,    36,
       7,    38,    25,     8,     1,    42,     3,     6,    45,     6,
      47,    48,    49,    50,    51,    10,     0,     1,    26,    27,
      43,     5,    30,    20,    21,    22,    23,    11,    12,    13,
      14,    15,     8,    20,     4,     7,    10,    20,    20,    30,
      43,    30,    26,    27,    28,    29,    30,    31,    32,    33,
       0,     1,    16,    12,     3,     5,    10,   144,    20,   147,
      53,    11,    12,    13,    14,    15,    83,    83,    19,   175,
     160,   152,    23,   194,    -1,   182,    26,    27,    28,    29,
      30,    31,    32,    33,    35,    19,    37,    38,    -1,    23,
      41,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    35,    19,    -1,    38,    -1,    23,    -1,    42,    -1,
      -1,    45,    -1,    47,    48,    49,    50,    51,    35,     1,
      -1,    38,    -1,     5,    23,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    -1,    35,    -1,    -1,    38,
      -1,    -1,    -1,    42,    26,    27,    45,     1,    30,    48,
      49,    50,    51,    -1,    -1,    -1,    -1,    11,    12,    13,
      14,    15,     1,    -1,     3,    -1,    -1,     6,    -1,    -1,
      -1,    25,    11,    12,    -1,    -1,    -1,     3,    -1,    -1,
       6,    20,    21,    22,    23,    11,    12,     3,    -1,    -1,
       6,    -1,    -1,    -1,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,     1,    -1,    20,    21,    22,    23,     7,     8,
      -1,     1,    11,    12,    13,    14,    15,    16,    17,     1,
      10,    11,    12,    13,    14,    15,     8,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,     1,    -1,    -1,     4,
      -1,    -1,     7,    -1,    -1,     1,    11,    12,    13,    14,
      15,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
       1,    -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,     1,
      11,    12,    13,    14,    15,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    47,    55,    56,    57,    23,    58,    59,     0,    37,
      41,    57,    60,    61,     9,    34,     4,     5,    23,    62,
      62,    19,    23,    35,    38,    42,    45,    48,    49,    50,
      51,    61,    70,    71,    72,    73,    74,    79,    80,    83,
      84,    20,    59,     6,    63,     1,    36,    47,    66,    67,
       1,    36,    66,    53,     6,     9,    23,     3,     6,    11,
      12,    20,    21,    22,    39,    73,    74,    85,    86,    88,
      89,    90,    92,    93,     6,    88,    70,    85,    19,     1,
       5,     8,    18,    18,    10,     7,    23,    46,    64,    65,
      34,    29,    23,    68,    69,    67,    70,    31,    70,    72,
      52,    77,    78,    88,    88,     8,     6,    85,    88,    89,
      89,     6,    43,    24,    40,     1,     8,    11,    12,    13,
      14,    15,    16,    17,    87,    91,    22,    88,    33,    25,
       5,    71,    88,     1,    23,    75,    76,     1,    75,    76,
      34,     8,     9,    23,     4,     7,     9,     4,     5,    29,
       1,    31,     4,     7,    10,    88,    85,     7,     7,    85,
      70,     6,    86,    86,    89,     8,    17,     8,     1,    88,
       1,    89,    70,    70,    71,     6,    11,    12,    20,    93,
      94,    10,     8,    65,    20,    68,    31,    78,    44,     4,
       7,    26,    27,    30,    81,    82,    88,    89,    32,    28,
      77,    20,    20,    94,    10,    88,    88,    70,    85,    30,
      81,    82,    30,     7,    25,     4,    43,    30,    70,    88,
      70,    28,     7
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, parse_res, Lexer, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, Lexer)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, parse_res, Lexer); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, CCode** parse_res, CLexAnalyzer_1C* Lexer)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parse_res, Lexer)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    CCode** parse_res;
    CLexAnalyzer_1C* Lexer;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (parse_res);
  YYUSE (Lexer);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, CCode** parse_res, CLexAnalyzer_1C* Lexer)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, parse_res, Lexer)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    CCode** parse_res;
    CLexAnalyzer_1C* Lexer;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parse_res, Lexer);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, CCode** parse_res, CLexAnalyzer_1C* Lexer)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, parse_res, Lexer)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    CCode** parse_res;
    CLexAnalyzer_1C* Lexer;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , parse_res, Lexer);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, parse_res, Lexer); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, CCode** parse_res, CLexAnalyzer_1C* Lexer)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, parse_res, Lexer)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    CCode** parse_res;
    CLexAnalyzer_1C* Lexer;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parse_res);
  YYUSE (Lexer);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "'?'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1382 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 4: /* "','" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1387 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 5: /* "';'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1392 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 6: /* "'('" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1397 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 7: /* "')'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1402 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 8: /* "'='" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1407 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 9: /* "'['" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1412 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 10: /* "']'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1417 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 11: /* "'+'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1422 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 12: /* "'-'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1427 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 13: /* "'*'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1432 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 14: /* "'/'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1437 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 15: /* "'%'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1442 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 16: /* "'<'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1447 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 17: /* "'>'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1452 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 18: /* "'.'" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1457 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 19: /* "label" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1462 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 20: /* "number" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1467 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 21: /* "date" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1472 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 22: /* "literal" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1477 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 23: /* "name" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1482 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 24: /* "and_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1487 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 25: /* "do_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1492 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 26: /* "else_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1497 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 27: /* "elsif" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1502 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 28: /* "enddo" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1507 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 29: /* "endfunc" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1512 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 30: /* "endif" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1517 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 31: /* "endproc" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1522 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 32: /* "endtry" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1527 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 33: /* "except_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1532 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 34: /* "export_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1537 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 35: /* "for_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1542 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 36: /* "forward" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1547 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 37: /* "func" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1552 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 38: /* "if_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1557 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 39: /* "not_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1562 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 40: /* "or_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1567 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 41: /* "proc" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1572 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 42: /* "return_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1577 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 43: /* "then" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1582 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 44: /* "to_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1587 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 45: /* "try_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1592 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 46: /* "val" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1597 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 47: /* "var" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1602 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 48: /* "while_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1607 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 49: /* "break_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1612 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 50: /* "continue_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1617 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 51: /* "goto_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1622 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 52: /* "context" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1627 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 61: /* "METHOD" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1632 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 62: /* "METHEADER" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1637 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 63: /* "PARAMLIST" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1642 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 64: /* "PARAMS" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1647 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 65: /* "PARAM" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1652 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 66: /* "MVARS" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1657 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 67: /* "MVAR" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1662 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 68: /* "MVARIABLES" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1667 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 69: /* "MVARIABLE" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1672 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 70: /* "STATEMENTS" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1677 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 71: /* "STATEMENT" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1682 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 72: /* "OPERATOR" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1687 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 73: /* "LVAL" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1692 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 74: /* "METHCALL" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1697 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 75: /* "SUBMETHCALL" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1702 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 76: /* "MEMBNAME" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1707 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 77: /* "REALPARAMS" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1712 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 78: /* "REALPARAM" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1717 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 79: /* "TRYBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1722 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 80: /* "IFBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1727 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 81: /* "ELSEIFBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1732 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 82: /* "ELSEBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1737 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 83: /* "WHILEBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1742 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 84: /* "FORBLOCK" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1747 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 85: /* "LOGIC" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1752 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 86: /* "LOGICP" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1757 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 87: /* "COMPARE" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1762 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 88: /* "EXPRESSION" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1767 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 89: /* "EXPRBIN" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1772 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 90: /* "EXPRUN" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1777 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 91: /* "OPERATION" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1782 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 92: /* "LITERAL" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1787 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 93: /* "CONST_" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1792 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;
      case 94: /* "CONSTANT" */
#line 92 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
	{ delete (*yyvaluep); };
#line 1797 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (CCode** parse_res, CLexAnalyzer_1C* Lexer);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (CCode** parse_res, CLexAnalyzer_1C* Lexer)
#else
int
yyparse (parse_res, Lexer)
    CCode** parse_res;
    CLexAnalyzer_1C* Lexer;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 161 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc));
				(yyval)->CodeType = CT_Module;
				(yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));
				*parse_res = (yyval);
			;}
    break;

  case 3:
#line 170 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDeclList); (yyval)->AddTail(1, (yyvsp[(1) - (1)])); ;}
    break;

  case 4:
#line 172 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (2)])->AddTail(1, (yyvsp[(2) - (2)])); (yyvsp[(1) - (2)])->SetLoc((yyloc)); ;}
    break;

  case 5:
#line 174 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = NULL; ;}
    break;

  case 6:
#line 177 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDecl); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 7:
#line 180 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDefList); (yyval)->AddTail(1, (yyvsp[(1) - (1)])); ;}
    break;

  case 8:
#line 182 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (3)])->AddTail(2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); (yyvsp[(1) - (3)])->SetLoc((yyloc)); ;}
    break;

  case 9:
#line 185 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail(1, (yyvsp[(1) - (1)])); ;}
    break;

  case 10:
#line 187 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 11:
#line 189 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 12:
#line 191 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail(4, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 13:
#line 194 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethList); (yyval)->AddTail(1, (yyvsp[(1) - (1)])); ;}
    break;

  case 14:
#line 196 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (2)])->AddTail((yyvsp[(2) - (2)])); (yyvsp[(1) - (2)])->SetLoc((yyloc)); ;}
    break;

  case 15:
#line 198 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = NULL; ;}
    break;

  case 16:
#line 201 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_Procedure);
				(yyval)->AddTail(5, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)]));
			;}
    break;

  case 17:
#line 207 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_Procedure);
				(yyval)->AddTail(5, (yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); 
			;}
    break;

  case 18:
#line 213 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_Error);
				delete (yyvsp[(1) - (4)]);
				delete (yyvsp[(2) - (4)]);
				delete (yyvsp[(4) - (4)]); 
			;}
    break;

  case 19:
#line 220 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Function); (yyval)->AddTail(5, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)]));;}
    break;

  case 20:
#line 222 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_Error);
				delete (yyvsp[(1) - (4)]);
				delete (yyvsp[(2) - (4)]);
				delete (yyvsp[(4) - (4)]); 
			;}
    break;

  case 21:
#line 229 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Procedure); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 22:
#line 231 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Function); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 23:
#line 234 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethHeader); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 24:
#line 236 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethHeader); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 25:
#line 239 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = (yyvsp[(2) - (3)]); (yyval)->AddHead((yyvsp[(1) - (3)])); (yyval)->AddTail((yyvsp[(3) - (3)])); (yyvsp[(1) - (3)])->SetLoc((yyloc)); ;}
    break;

  case 26:
#line 241 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_ParamList); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 27:
#line 244 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_ParamList); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 28:
#line 246 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (3)])->AddTail(2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); (yyvsp[(1) - (3)])->SetLoc((yyloc)); ;}
    break;

  case 29:
#line 249 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 30:
#line 251 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 31:
#line 253 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 32:
#line 255 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 33:
#line 257 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 34:
#line 259 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 35:
#line 261 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Param); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 36:
#line 265 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDeclList); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 37:
#line 267 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (2)])->AddTail((yyvsp[(2) - (2)])); (yyvsp[(1) - (2)])->SetLoc((yyloc)); ;}
    break;

  case 38:
#line 269 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = NULL; ;}
    break;

  case 39:
#line 272 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDecl); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 40:
#line 275 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDefList); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 41:
#line 277 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (3)])->AddTail(2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); (yyvsp[(1) - (3)])->SetLoc((yyloc)); ;}
    break;

  case 42:
#line 280 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 43:
#line 282 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_VarDef); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 44:
#line 285 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Statements); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 45:
#line 287 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				CCode* PrevStmt = (yyvsp[(1) - (3)])->GetLast();
				PrevStmt->AddTail((yyvsp[(2) - (3)]));
				PrevStmt->EndLine = (yylsp[(2) - (3)]).last_line;
				PrevStmt->EndCol = (yylsp[(2) - (3)]).last_column - 1;
				(yyvsp[(1) - (3)])->AddTail((yyvsp[(3) - (3)]));
				(yyvsp[(1) - (3)])->SetLoc((yyloc));
			;}
    break;

  case 46:
#line 296 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				CCode* PrevStmt = (yyvsp[(1) - (4)])->GetLast();
				PrevStmt->AddTail((yyvsp[(3) - (4)]));
				PrevStmt->EndLine = (yylsp[(3) - (4)]).last_line;
				PrevStmt->EndCol = (yylsp[(3) - (4)]).last_column - 1;
				(yyvsp[(1) - (4)])->AddTail((yyvsp[(4) - (4)]));
				(yyvsp[(1) - (4)])->SetLoc((yyloc));
			;}
    break;

  case 47:
#line 306 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Statement); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 48:
#line 308 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Statement); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 49:
#line 310 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				//меняем позицию, а то она берётся от предыдущей конструкции
				//@$.first_line = @$.last_line;
				//@$.first_column = @$.last_column - 1;
				CLexema* lex = new CLexema((yyloc), "", LT_Other); //пустая лексема - для однородности
				(yyval) = new CCode((yyloc)); 
				(yyval)->AddTail(lex);
			;}
    break;

  case 50:
#line 321 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 51:
#line 323 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 52:
#line 325 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 53:
#line 327 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 54:
#line 329 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 55:
#line 331 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 56:
#line 333 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 57:
#line 337 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 58:
#line 339 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 59:
#line 341 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 60:
#line 343 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 61:
#line 345 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Operator); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 62:
#line 348 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LValue); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 63:
#line 350 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LValue); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 64:
#line 352 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LValue); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 65:
#line 354 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = (yyvsp[(1) - (3)]); delete (yyvsp[(2) - (3)]); ;}
    break;

  case 66:
#line 356 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LValue); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 67:
#line 359 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethCall); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 68:
#line 361 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethCall); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 69:
#line 363 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = (yyvsp[(1) - (3)]); delete (yyvsp[(2) - (3)]); ;}
    break;

  case 70:
#line 365 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethCall); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 71:
#line 368 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_MethCall); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 73:
#line 413 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_CallParamList); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 74:
#line 415 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(1) - (3)])->AddTail(2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); (yyvsp[(1) - (3)])->SetLoc((yyloc)); ;}
    break;

  case 75:
#line 418 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_CallParam); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 76:
#line 420 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_CallParam); (yyval)->AddTail((yyvsp[(1) - (1)])); ;}
    break;

  case 77:
#line 422 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyloc).first_line = (yyloc).last_line;
				(yyloc).first_column = (yyloc).last_column - 1;
				CLexema* lex = new CLexema((yyloc), "", LT_Other);
				(yyval) = new CCode((yyloc), CT_CallParam);
				(yyval)->AddTail(lex);
			;}
    break;

  case 78:
#line 431 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Try); (yyval)->AddTail(5, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 79:
#line 434 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_If); (yyval)->AddTail(5, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 80:
#line 436 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_If); (yyval)->AddTail(6, (yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 81:
#line 438 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_If); (yyval)->AddTail(6, (yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 82:
#line 440 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_If); (yyval)->AddTail(7, (yyvsp[(1) - (7)]), (yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 83:
#line 443 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_ElseIf); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 84:
#line 445 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_ElseIf); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 85:
#line 448 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_Else); (yyval)->AddTail(2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 86:
#line 451 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_While); (yyval)->AddTail(5, (yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 87:
#line 454 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_For); (yyval)->AddTail(9, (yyvsp[(1) - (9)]), (yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(5) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 88:
#line 457 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LogicalExpr); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 90:
#line 460 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_LogicalExpr);
				(yyval)->AddTail(3, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)]));
			;}
    break;

  case 91:
#line 466 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LogicalExpr); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 92:
#line 468 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LogicalExpr); (yyval)->AddTail(4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 93:
#line 470 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LogicalExpr); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 94:
#line 472 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CCode((yyloc), CT_LogicalExpr); (yyval)->AddTail(3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 98:
#line 477 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CLexema((yyloc), ">=", LT_Other); delete (yyvsp[(1) - (2)]); delete (yyvsp[(2) - (2)]); ;}
    break;

  case 99:
#line 478 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CLexema((yyloc), "<=", LT_Other); delete (yyvsp[(1) - (2)]); delete (yyvsp[(2) - (2)]); ;}
    break;

  case 100:
#line 479 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyval) = new CLexema((yyloc), "<>", LT_Other); delete (yyvsp[(1) - (2)]); delete (yyvsp[(2) - (2)]); ;}
    break;

  case 103:
#line 484 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyvsp[(1) - (3)])->AddTail(2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));
				(yyvsp[(1) - (3)])->SetLoc((yyloc));
			;}
    break;

  case 104:
#line 489 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyvsp[(1) - (4)])->AddTail(2, (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)]));
				(yyvsp[(1) - (4)])->SetLoc((yyloc));
			;}
    break;

  case 105:
#line 494 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = (yyvsp[(1) - (3)]);
				(yyvsp[(1) - (3)])->AddTail((yyvsp[(3) - (3)]));
				(yyval)->SetLoc((yyloc));
			;}
    break;

  case 107:
#line 502 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {(yyval) = new CCode((yyloc)); (yyval)->AddTail((yyvsp[(1) - (1)]));;}
    break;

  case 108:
#line 504 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {(yyval) = new CCode((yyloc)); (yyval)->AddTail((yyvsp[(1) - (1)]));;}
    break;

  case 109:
#line 506 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {(yyval) = new CCode((yyloc)); (yyval)->AddTail((yyvsp[(1) - (1)]));;}
    break;

  case 110:
#line 508 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyval) = new CCode((yyloc), CT_Expr);
				(yyval)->AddTail(8, (yyvsp[(1) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)]));
			;}
    break;

  case 111:
#line 513 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				(yyvsp[(2) - (3)])->AddHead((yyvsp[(1) - (3)]));
				(yyvsp[(2) - (3)])->AddTail((yyvsp[(3) - (3)]));
				(yyvsp[(2) - (3)])->SetLoc((yyloc));
				(yyval) = (yyvsp[(2) - (3)]);
			;}
    break;

  case 112:
#line 521 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    { (yyvsp[(2) - (2)])->AddHead((yyvsp[(1) - (2)])); (yyval) = (yyvsp[(2) - (2)]); ;}
    break;

  case 113:
#line 523 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {(yyvsp[(2) - (2)])->AddHead((yyvsp[(1) - (2)])); (yyval) = (yyvsp[(2) - (2)]);;}
    break;

  case 120:
#line 542 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				CLexema* lex = (CLexema*)(yyvsp[(1) - (2)]);
				lex->Text += (yyvsp[(2) - (2)])->GetText();
				(yyvsp[(2) - (2)])->SetLoc((yyloc));
				delete (yyvsp[(2) - (2)]);
			;}
    break;

  case 125:
#line 555 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				CLexema* lex = (CLexema*)(yyvsp[(1) - (2)]);
				lex->Text += (yyvsp[(2) - (2)])->GetText();
				lex->SetLoc((yyloc));
				delete (yyvsp[(2) - (2)]);
			;}
    break;

  case 126:
#line 562 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"
    {
				CLexema* lex = (CLexema*)(yyvsp[(1) - (2)]);
				lex->Text += (yyvsp[(2) - (2)])->GetText();
				lex->SetLoc((yyloc));
				delete (yyvsp[(2) - (2)]);
			;}
    break;


/* Line 1267 of yacc.c.  */
#line 2741 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, parse_res, Lexer, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, parse_res, Lexer, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, parse_res, Lexer, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, parse_res, Lexer);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, parse_res, Lexer);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, parse_res, Lexer, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, parse_res, Lexer);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, parse_res, Lexer);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 570 "\\OpenConf_Scripts\\\321\350\361\362\345\354\355\373\345\324\340\351\353\373\\1CLangParser\\lang1c.y"



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

