#ifndef BUILD_RESOURCE
int CLexer::m_SwitchTable[STATE_LAST][CLS_LAST+1];
DWORD CLexer::m_ClassesOfSymbol[256];

#else

int CLexer::m_SwitchTable[STATE_LAST][CLS_LAST+1]={
	{	// STATE_NONE
		0,				//CLS_WHTSPC,		// ' ' \t
		STATE_DIV,		//CLS_REMARK,		// '/'
		STATE_QUOT1,	//CLS_QUOTE,		// '"'
		STATE_QUOT1,	//CLS_QUOTECONT,	// '|'
		STATE_LPAR,		//CLS_LPAR,			// '('
		STATE_RPAR,		//CLS_RPAR,			// ')'
		STATE_LBRACKET,	//CLS_LBRACKET,		// '['
		STATE_RBRACKET,	//CLS_RBRACKET,		// ']'
		STATE_COMMA,	//CLS_COMMA,		// ','
		STATE_SEMICOLON,//CLS_SEMICOLON,	// ';'
		STATE_EQUAL,	//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		STATE_PLUS,		//CLS_PLUS,			// '+'
		STATE_MINUS,	//CLS_MINUS,		// '-'
		STATE_MULT,		//CLS_MULT,			// '*'
		STATE_PERCENT,	//CLS_PERCENT,		// '%'
		STATE_TILDA,	//CLS_TILDA,		// '~'
		STATE_COLON,	//CLS_COLON,		// ':'
		STATE_LESS,		//CLS_LESS,			// '<'
		STATE_GRAT,		//CLS_GRAT,			// '>'
		STATE_DAT1,		//CLS_APOSTROF,		// '''
		STATE_PERIOD,	//CLS_PERIOD,		// '.'
		STATE_NUM,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_A,		//CLS_A,
		STATE_B,		//CLS_B,
		STATE_C,		//CLS_C,
		STATE_D,		//CLS_D,
		STATE_E,		//CLS_E,
		STATE_F,		//CLS_F,
		STATE_G,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_I,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_N,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_P,		//CLS_P,
		STATE_R,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_T,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_V,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_W,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_R_V,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_D,		//CLS_R_D,	// Ä
		STATE_R_E,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_R_Z,		//CLS_R_Z,	// Ç
		STATE_R_I,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_K,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_N,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_R_P,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_S,		//CLS_R_S,	// Ñ
		STATE_R_T,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_C,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_R_F,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_R_e,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		STATE_WHAT,		//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_V
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_VO,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_VO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_R_VOZ,	//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_VOZ
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_R_VOZV,	//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_VOZV
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_VOZVR,	//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_VOZVR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_VOZVRA,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_VOZVRA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_RETURN,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_D
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_DA,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_DL,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_DA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_DAL,	//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_DAL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_DALE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_DALE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_FORWARD,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_DL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_R_DLYA,	//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_DLYA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_E
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_ES,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ES
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_ESL,	//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ESL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_IF,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_Z
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_ZN,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ZN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_ZNA,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ZNA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_VAL,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_I
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_IL,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_IN,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_IS,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_IL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_OR,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_IN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_INA,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_R_INA4,	//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA4
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_INA4E,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA4E
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_INA4EE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA4EE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_INA4EES,//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA4EEC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_INA4EESL,//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_INA4EESL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_ELSIF,	//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_IS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_ISK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_ISKL,	//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_R_ISKLU,	//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKLU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_R_ISKLU4,	//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKLU4
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_ISKLU4E,//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKLU4E
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_ISKLU4EN,//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKLU4EN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_ISKLU4ENI,//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_ISKLU4ENI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_EXCEPT,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_K
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_KO,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_KON,	//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KON
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_KONE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_KONT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_KONEC,	//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONEC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_KONECE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_R_KONECP,	//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_KONECC,	//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_R_KONECF,	//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_KONECES,//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECES
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_KONECESL,//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECESL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_ENDIF,	//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_KONECPO,//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_KONECPR,//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_R_KONECPOP,//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPOP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_R_KONECPOPY,//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPOPY
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_KONECPOPYT,//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPOPYT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_KONECPOPYTK,//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPOPYTK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_ENDTRY,	//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_KONECPRO,//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPRO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_KONECPROC,//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPROC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_KONECPROCE,//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPROCE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_KONECPROCED,//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPROCED
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_R_KONECPROCEDU,//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPROCEDU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_KONECPROCEDUR,//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECPROCEDUR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_ENDPROCEDURE,//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECF
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_R_KONECFU,//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECFU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_KONECFUN,//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECFUN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_KONECFUNK,//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECFUNK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_KONECFUNKC,//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECFUNKC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_KONECFUNKCI,//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECFUNKCI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_ENDFUNCTION,//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_KONECCI,//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECCI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_KONECCIK,//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECCIK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_KONECCIKL,//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONECCIKL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_ENDDO,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_N
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NOT,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_P
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_PE,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_PO,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_PR,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_PER,	//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PER
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_PERE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PERE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_R_PEREY,	//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_VAR,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PEREY
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_PEREYT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PEREYT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_GOTO,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_POK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_R_POP,	//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_POK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_WHILE,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_POP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_R_POPY,	//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_POPY
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_POPYT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_POPYT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_POPYTK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_POPYTK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_TRY,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_PRE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_PRO,	//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_PRER,	//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRER
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_R_PRERV,	//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRERV
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_PRERVA,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRERVA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_PRERVAT,//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRERVAT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_BREAK,	//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_PROD,	//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_PROC,	//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PROD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_PRODO,	//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRODO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_R_PRODOL,	//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRODOL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_R_PRODOLZH,//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRODOLZH
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_PRODOLZHI,//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRODOLZHI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_PRODOLZHIT,//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PRODOLZHIT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_CONTINUE,	//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PROC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_PROCE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PROCE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_PROCED,	//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PROCED
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_R_PROCEDU,//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_PROCEDU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_PROCEDUR,//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_RROCEDUR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_PROCEDURE,//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_T
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_TO,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_TO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_R_TOG,	//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_TOG
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_TOGD,	//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_TOGD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_THEN,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_F
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_R_FU,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_FU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_R_FUN,	//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_FUN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_FUNK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_FUNK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_R_FUNKC,	//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_FUNKC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_FUNKCI,	//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_FUNKCI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_FUNCTION,	//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_C
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_R_CI,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_CI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_CIK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_CIK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_DO,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_e
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_EK,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_EK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_EKS,	//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_EKS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_R_EKSP,	//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_EKSP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_EKSPO,	//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_EKSPO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_R_EKSPOR,	//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_EKSPOR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_EXPORT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_A
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_AN,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_AN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_AND,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_B
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_BR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_BR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_BRE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_BRE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_BREA,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_BREA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_BREAK,	//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_C
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_CO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_CR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_CON,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CON
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_CONT,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_CONTE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_CONTI,	//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_CONTIN,	//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTIN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_CONTINU,	//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTINU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_CONTINUE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_D
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_DO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_E
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_EL,		//CLS_L,
		STATE_EN,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_EX,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_ELS,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ELS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_ELSE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_ELSI,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ELSI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_ELSIF,	//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_END,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_END
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_ENDD,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_ENDF,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_ENDI,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_ENDP,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_ENDT,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_ENDDO,	//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_ENDIF,	//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_ENDPR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_ENDPRO,	//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPRO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_ENDPROC,	//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPROC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_ENDPROCE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPRCE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_ENDPROCED,//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPROCED
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_ENDPROCEDU,//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPROCEDU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_ENDPROCEDUR,//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPROCEDUR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_ENDPROCEDURE,//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDF
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_ENDFU,	//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_ENDFUN,	//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_ENDFUNC,	//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUNC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_ENDFUNCT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUNCT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_ENDFUNCTI,//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUNCTI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_ENDFUNCTIO,//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUNCTIO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_ENDFUNCTION,//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_ENDTR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDTR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_ENDTRY,	//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EX
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_EXC,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_EXP,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_EXCE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXCE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_EXCEP,	//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXCEP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_EXCEPT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXP
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_EXPO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXPO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_EXPOR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXPOR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_EXPORT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_F
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_FO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_FU,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_FOR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FOR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_FORW,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FORW
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_FORWA,	//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FORWA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_FORWAR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FORWAR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_FORWARD,	//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_FUN,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_FUNC,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUNC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_FUNCT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUNCT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_FUNCTI,	//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUNCTI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_FUNCTIO,	//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUNCTIO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_FUNCTION,	//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_G
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_GO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_GO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_GOT,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_GOT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_GOTO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_I
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_IF,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_N
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_NO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NOT,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_O
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_OR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_P
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_PR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_PRO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PRO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_PROC,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_PROCE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROCE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_PROCED,	//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROCED
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_PROCEDU,	//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROCEDU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_PROCEDUR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROCEDUR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_PROCEDURE,//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_RE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_RE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_RET,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_RET
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_RETU,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_RETU
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_RETUR,	//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_RETUR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_RETURN,	//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_T
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_TH,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_TO,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_TR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_TH
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_THE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_THE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_THEN,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_TR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_TRY,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_V
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_VA,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_VA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_VAL,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_VAR,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_W
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_WH,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_WH
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_WHI,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_WHI
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_WHIL,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_WHIL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_WHILE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_S,
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_SO,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_R_SOZ,	//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZ
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_R_SOZD,	//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_R_SOZDA,	//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDA,
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_R_SOZDAT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDAT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_R_SOZDATT,//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_R_SOZDATTO,	//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATTO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_R_SOZDATTOB,	//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATTOB
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_R_SOZDATTOBB,	//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATTOBB
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_SOZDATTOBBE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATTOBBE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_SOZDATTOBBEC,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_SOZDATTOBBEC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_CREATEOBJECT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_CRE,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CRE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_CREA,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_CREAT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREAT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_CREATE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_CREATEO,	//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_CREATEOB,	//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEOB
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_CREATEOBJ,//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEOBJ
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_CREATEOBJE,	//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEOBJE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_CREATEOBJEC,	//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEOBJEC
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_CREATEOBJECT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CREATEOBJECT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_R_KONTE,	//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONTE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_R_KONTEK,	//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONTEK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_R_KONTEKS,//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_R_KONTEKS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_CONTEXT,	//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_CONTEX,	//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTEX
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_CONTEXT,	//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_AND
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_RETURN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FORWARD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_IF
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_VAL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_OR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ELSE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ELSIF
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXCEPT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDIF
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDTRY
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDPROCEDURE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDFUNCTION
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_ENDDO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_NOT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_VAR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_GOTO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_TO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_WHILE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_TRY
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_BREAK
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTINUE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_PROCEDURE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_THEN
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_FUNCTION
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_EXPORT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_DO
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_CONTEXT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_NAME
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NAME,		//CLS_NUMBER,		//
		STATE_NAME,		//CLS_SYMBOL,
		STATE_NAME,		//CLS_A,
		STATE_NAME,		//CLS_B,
		STATE_NAME,		//CLS_C,
		STATE_NAME,		//CLS_D,
		STATE_NAME,		//CLS_E,
		STATE_NAME,		//CLS_F,
		STATE_NAME,		//CLS_G,
		STATE_NAME,		//CLS_H,
		STATE_NAME,		//CLS_I,
		STATE_NAME,		//CLS_K,
		STATE_NAME,		//CLS_L,
		STATE_NAME,		//CLS_N,
		STATE_NAME,		//CLS_O,
		STATE_NAME,		//CLS_P,
		STATE_NAME,		//CLS_R,
		STATE_NAME,		//CLS_S,
		STATE_NAME,		//CLS_T,
		STATE_NAME,		//CLS_U,
		STATE_NAME,		//CLS_V,
		STATE_NAME,		//CLS_Y,
		STATE_NAME,		//CLS_W,
		STATE_NAME,		//CLS_X,
		STATE_NAME,		//CLS_R_A,	// À
		STATE_NAME,		//CLS_R_V,	// Â
		STATE_NAME,		//CLS_R_G,	// Ã
		STATE_NAME,		//CLS_R_D,	// Ä
		STATE_NAME,		//CLS_R_E,	// Å
		STATE_NAME,		//CLS_R_ZH,	// Æ
		STATE_NAME,		//CLS_R_Z,	// Ç
		STATE_NAME,		//CLS_R_I,	// È
		STATE_NAME,		//CLS_R_IY,	// É
		STATE_NAME,		//CLS_R_K,	// Ê
		STATE_NAME,		//CLS_R_L,	// Ë
		STATE_NAME,		//CLS_R_M,	// Ì
		STATE_NAME,		//CLS_R_N,	// Í
		STATE_NAME,		//CLS_R_O,	// Î
		STATE_NAME,		//CLS_R_P,	// Ï
		STATE_NAME,		//CLS_R_R,	// Ð
		STATE_NAME,		//CLS_R_S,	// Ñ
		STATE_NAME,		//CLS_R_T,	// Ò
		STATE_NAME,		//CLS_R_U,	// Ó
		STATE_NAME,		//CLS_R_C,	// Ö
		STATE_NAME,		//CLS_R_4,	// ×
		STATE_NAME,		//CLS_R_F,	// Ô
		STATE_NAME,		//CLS_R_MZ,	// Ü
		STATE_NAME,		//CLS_R_Y,	// Û
		STATE_NAME,		//CLS_R_e,	// Ý
		STATE_NAME,		//CLS_R_YU	// Þ
		STATE_NAME,		//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_NAME,		//CLS_J,
		STATE_NAME,		//CLS_R_B,
		STATE_NAME,		//CLS_R_TZ,
	},
	{	// STATE_NUM
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		STATE_NUMPERIOD,//CLS_PERIOD,		// '.'
		STATE_NUM,		//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_DIV
		0,				//CLS_WHTSPC,		// ' ' \t
		STATE_REMARK,	//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_REMARK
		STATE_REMARK,	//CLS_WHTSPC,		// ' ' \t
		STATE_REMARK,	//CLS_REMARK,		// '/'
		STATE_REMARK,	//CLS_QUOTE,		// '"'
		STATE_REMARK,	//CLS_QUOTECONT,	// '|'
		STATE_REMARK,	//CLS_LPAR,			// '('
		STATE_REMARK,	//CLS_RPAR,			// ')'
		STATE_REMARK,	//CLS_LBRACKET,		// '['
		STATE_REMARK,	//CLS_RBRACKET,		// ']'
		STATE_REMARK,	//CLS_COMMA,		// ','
		STATE_REMARK,	//CLS_SEMICOLON,	// ';'
		STATE_REMARK,	//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		STATE_REMARK,	//CLS_PLUS,			// '+'
		STATE_REMARK,	//CLS_MINUS,		// '-'
		STATE_REMARK,	//CLS_MULT,			// '*'
		STATE_REMARK,	//CLS_PERCENT,		// '%'
		STATE_REMARK,	//CLS_TILDA,		// '~'
		STATE_REMARK,	//CLS_COLON,		// ':'
		STATE_REMARK,	//CLS_LESS,			// '<'
		STATE_REMARK,	//CLS_GRAT,			// '>'
		STATE_REMARK,	//CLS_APOSTROF,		// '''
		STATE_REMARK,	//CLS_PERIOD,		// '.'
		STATE_REMARK,	//CLS_NUMBER,		//
		STATE_REMARK,	//CLS_SYMBOL,
		STATE_REMARK,	//CLS_A,
		STATE_REMARK,	//CLS_B,
		STATE_REMARK,	//CLS_C,
		STATE_REMARK,	//CLS_D,
		STATE_REMARK,	//CLS_E,
		STATE_REMARK,	//CLS_F,
		STATE_REMARK,	//CLS_G,
		STATE_REMARK,	//CLS_H,
		STATE_REMARK,	//CLS_I,
		STATE_REMARK,	//CLS_K,
		STATE_REMARK,	//CLS_L,
		STATE_REMARK,	//CLS_N,
		STATE_REMARK,	//CLS_O,
		STATE_REMARK,	//CLS_P,
		STATE_REMARK,	//CLS_R,
		STATE_REMARK,	//CLS_S,
		STATE_REMARK,	//CLS_T,
		STATE_REMARK,	//CLS_U,
		STATE_REMARK,	//CLS_V,
		STATE_REMARK,	//CLS_Y,
		STATE_REMARK,	//CLS_W,
		STATE_REMARK,	//CLS_X,
		STATE_REMARK,	//CLS_R_A,	// À
		STATE_REMARK,	//CLS_R_V,	// Â
		STATE_REMARK,	//CLS_R_G,	// Ã
		STATE_REMARK,	//CLS_R_D,	// Ä
		STATE_REMARK,	//CLS_R_E,	// Å
		STATE_REMARK,	//CLS_R_ZH,	// Æ
		STATE_REMARK,	//CLS_R_Z,	// Ç
		STATE_REMARK,	//CLS_R_I,	// È
		STATE_REMARK,	//CLS_R_IY,	// É
		STATE_REMARK,	//CLS_R_K,	// Ê
		STATE_REMARK,	//CLS_R_L,	// Ë
		STATE_REMARK,	//CLS_R_M,	// Ì
		STATE_REMARK,	//CLS_R_N,	// Í
		STATE_REMARK,	//CLS_R_O,	// Î
		STATE_REMARK,	//CLS_R_P,	// Ï
		STATE_REMARK,	//CLS_R_R,	// Ð
		STATE_REMARK,	//CLS_R_S,	// Ñ
		STATE_REMARK,	//CLS_R_T,	// Ò
		STATE_REMARK,	//CLS_R_U,	// Ó
		STATE_REMARK,	//CLS_R_C,	// Ö
		STATE_REMARK,	//CLS_R_4,	// ×
		STATE_REMARK,	//CLS_R_F,	// Ô
		STATE_REMARK,	//CLS_R_MZ,	// Ü
		STATE_REMARK,	//CLS_R_Y,	// Û
		STATE_REMARK,	//CLS_R_e,	// Ý
		STATE_REMARK,	//CLS_R_YU	// Þ
		STATE_REMARK,	//CLS_R_YA,	// ß
		STATE_REMARK,	//CLS_WHAT	// ?
		STATE_REMARK,	//CLS_J,
		STATE_REMARK,	//CLS_R_B,
		STATE_REMARK,	//CLS_R_TZ,
	},
	{	// STATE_QUOT1
		STATE_QUOT1,	//CLS_WHTSPC,		// ' ' \t
		STATE_QUOT1,	//CLS_REMARK,		// '/'
		STATE_QUOTE,	//CLS_QUOTE,		// '"'
		STATE_QUOT1,	//CLS_QUOTECONT,	// '|'
		STATE_QUOT1,	//CLS_LPAR,			// '('
		STATE_QUOT1,	//CLS_RPAR,			// ')'
		STATE_QUOT1,	//CLS_LBRACKET,		// '['
		STATE_QUOT1,	//CLS_RBRACKET,		// ']'
		STATE_QUOT1,	//CLS_COMMA,		// ','
		STATE_QUOT1,	//CLS_SEMICOLON,	// ';'
		STATE_QUOT1,	//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		STATE_QUOT1,	//CLS_PLUS,			// '+'
		STATE_QUOT1,	//CLS_MINUS,		// '-'
		STATE_QUOT1,	//CLS_MULT,			// '*'
		STATE_QUOT1,	//CLS_PERCENT,		// '%'
		STATE_QUOT1,	//CLS_TILDA,		// '~'
		STATE_QUOT1,	//CLS_COLON,		// ':'
		STATE_QUOT1,	//CLS_LESS,			// '<'
		STATE_QUOT1,	//CLS_GRAT,			// '>'
		STATE_QUOT1,	//CLS_APOSTROF,		// '''
		STATE_QUOT1,	//CLS_PERIOD,		// '.'
		STATE_QUOT1,	//CLS_NUMBER,		//
		STATE_QUOT1,	//CLS_SYMBOL,
		STATE_QUOT1,	//CLS_A,
		STATE_QUOT1,	//CLS_B,
		STATE_QUOT1,	//CLS_C,
		STATE_QUOT1,	//CLS_D,
		STATE_QUOT1,	//CLS_E,
		STATE_QUOT1,	//CLS_F,
		STATE_QUOT1,	//CLS_G,
		STATE_QUOT1,	//CLS_H,
		STATE_QUOT1,	//CLS_I,
		STATE_QUOT1,	//CLS_K,
		STATE_QUOT1,	//CLS_L,
		STATE_QUOT1,	//CLS_N,
		STATE_QUOT1,	//CLS_O,
		STATE_QUOT1,	//CLS_P,
		STATE_QUOT1,	//CLS_R,
		STATE_QUOT1,	//CLS_S,
		STATE_QUOT1,	//CLS_T,
		STATE_QUOT1,	//CLS_U,
		STATE_QUOT1,	//CLS_V,
		STATE_QUOT1,	//CLS_Y,
		STATE_QUOT1,	//CLS_W,
		STATE_QUOT1,	//CLS_X,
		STATE_QUOT1,	//CLS_R_A,	// À
		STATE_QUOT1,	//CLS_R_V,	// Â
		STATE_QUOT1,	//CLS_R_G,	// Ã
		STATE_QUOT1,	//CLS_R_D,	// Ä
		STATE_QUOT1,	//CLS_R_E,	// Å
		STATE_QUOT1,	//CLS_R_ZH,	// Æ
		STATE_QUOT1,	//CLS_R_Z,	// Ç
		STATE_QUOT1,	//CLS_R_I,	// È
		STATE_QUOT1,	//CLS_R_IY,	// É
		STATE_QUOT1,	//CLS_R_K,	// Ê
		STATE_QUOT1,	//CLS_R_L,	// Ë
		STATE_QUOT1,	//CLS_R_M,	// Ì
		STATE_QUOT1,	//CLS_R_N,	// Í
		STATE_QUOT1,	//CLS_R_O,	// Î
		STATE_QUOT1,	//CLS_R_P,	// Ï
		STATE_QUOT1,	//CLS_R_R,	// Ð
		STATE_QUOT1,	//CLS_R_S,	// Ñ
		STATE_QUOT1,	//CLS_R_T,	// Ò
		STATE_QUOT1,	//CLS_R_U,	// Ó
		STATE_QUOT1,	//CLS_R_C,	// Ö
		STATE_QUOT1,	//CLS_R_4,	// ×
		STATE_QUOT1,	//CLS_R_F,	// Ô
		STATE_QUOT1,	//CLS_R_MZ,	// Ü
		STATE_QUOT1,	//CLS_R_Y,	// Û
		STATE_QUOT1,	//CLS_R_e,	// Ý
		STATE_QUOT1,	//CLS_R_YU	// Þ
		STATE_QUOT1,	//CLS_R_YA,	// ß
		STATE_QUOT1,	//CLS_WHAT	// ?
		STATE_QUOT1,	//CLS_J,
		STATE_QUOT1,	//CLS_R_B,
		STATE_QUOT1,	//CLS_R_TZ,
	},
	{	// STATE_QUOTE
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		STATE_QUOT1,	//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_DAT1
		STATE_DAT1,		//CLS_WHTSPC,		// ' ' \t
		STATE_DAT1,		//CLS_REMARK,		// '/'
		STATE_DAT1,		//CLS_QUOTE,		// '"'
		STATE_DAT1,		//CLS_QUOTECONT,	// '|'
		STATE_DAT1,		//CLS_LPAR,			// '('
		STATE_DAT1,		//CLS_RPAR,			// ')'
		STATE_DAT1,		//CLS_LBRACKET,		// '['
		STATE_DAT1,		//CLS_RBRACKET,		// ']'
		STATE_DAT1,		//CLS_COMMA,		// ','
		STATE_DAT1,		//CLS_SEMICOLON,	// ';'
		STATE_DAT1,		//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		STATE_DAT1,		//CLS_PLUS,			// '+'
		STATE_DAT1,		//CLS_MINUS,		// '-'
		STATE_DAT1,		//CLS_MULT,			// '*'
		STATE_DAT1,		//CLS_PERCENT,		// '%'
		STATE_DAT1,		//CLS_TILDA,		// '~'
		STATE_DAT1,		//CLS_COLON,		// ':'
		STATE_DAT1,		//CLS_LESS,			// '<'
		STATE_DAT1,		//CLS_GRAT,			// '>'
		STATE_DATE,		//CLS_APOSTROF,		// '''
		STATE_DAT1,		//CLS_PERIOD,		// '.'
		STATE_DAT1,		//CLS_NUMBER,		//
		STATE_DAT1,		//CLS_SYMBOL,
		STATE_DAT1,		//CLS_A,
		STATE_DAT1,		//CLS_B,
		STATE_DAT1,		//CLS_C,
		STATE_DAT1,	 	//CLS_D,
		STATE_DAT1,		//CLS_E,
		STATE_DAT1,		//CLS_F,
		STATE_DAT1,		//CLS_G,
		STATE_DAT1,		//CLS_H,
		STATE_DAT1,		//CLS_I,
		STATE_DAT1,		//CLS_K,
		STATE_DAT1,		//CLS_L,
		STATE_DAT1,		//CLS_N,
		STATE_DAT1,		//CLS_O,
		STATE_DAT1,		//CLS_P,
		STATE_DAT1,		//CLS_R,
		STATE_DAT1,		//CLS_S,
		STATE_DAT1,		//CLS_T,
		STATE_DAT1,		//CLS_U,
		STATE_DAT1,		//CLS_V,
		STATE_DAT1,		//CLS_Y,
		STATE_DAT1,		//CLS_W,
		STATE_DAT1,		//CLS_X,
		STATE_DAT1,		//CLS_R_A,	// À
		STATE_DAT1,		//CLS_R_V,	// Â
		STATE_DAT1,		//CLS_R_G,	// Ã
		STATE_DAT1,		//CLS_R_D,	// Ä
		STATE_DAT1,		//CLS_R_E,	// Å
		STATE_DAT1,		//CLS_R_ZH,	// Æ
		STATE_DAT1,		//CLS_R_Z,	// Ç
		STATE_DAT1,		//CLS_R_I,	// È
		STATE_DAT1,		//CLS_R_IY,	// É
		STATE_DAT1,		//CLS_R_K,	// Ê
		STATE_DAT1,		//CLS_R_L,	// Ë
		STATE_DAT1,		//CLS_R_M,	// Ì
		STATE_DAT1,		//CLS_R_N,	// Í
		STATE_DAT1,		//CLS_R_O,	// Î
		STATE_DAT1,		//CLS_R_P,	// Ï
		STATE_DAT1,		//CLS_R_R,	// Ð
		STATE_DAT1,		//CLS_R_S,	// Ñ
		STATE_DAT1,		//CLS_R_T,	// Ò
		STATE_DAT1,		//CLS_R_U,	// Ó
		STATE_DAT1,		//CLS_R_C,	// Ö
		STATE_DAT1,		//CLS_R_4,	// ×
		STATE_DAT1,		//CLS_R_F,	// Ô
		STATE_DAT1,		//CLS_R_MZ,	// Ü
		STATE_DAT1,		//CLS_R_Y,	// Û
		STATE_DAT1,		//CLS_R_e,	// Ý
		STATE_DAT1,		//CLS_R_YU	// Þ
		STATE_DAT1,		//CLS_R_YA,	// ß
		STATE_DAT1,		//CLS_WHAT	// ?
		STATE_DAT1,		//CLS_J,
		STATE_DAT1,		//CLS_R_B,
		STATE_DAT1,		//CLS_R_TZ,
	},
	{	// STATE_DATE
		0,		//CLS_WHTSPC,		// ' ' \t
		0,		//CLS_REMARK,		// '/'
		0,		//CLS_QUOTE,		// '"'
		0,		//CLS_QUOTECONT,	// '|'
		0,		//CLS_LPAR,			// '('
		0,		//CLS_RPAR,			// ')'
		0,		//CLS_LBRACKET,		// '['
		0,		//CLS_RBRACKET,		// ']'
		0,		//CLS_COMMA,		// ','
		0,		//CLS_SEMICOLON,	// ';'
		0,		//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,		//CLS_PLUS,			// '+'
		0,		//CLS_MINUS,		// '-'
		0,		//CLS_MULT,			// '*'
		0,		//CLS_PERCENT,		// '%'
		0,		//CLS_TILDA,		// '~'
		0,		//CLS_COLON,		// ':'
		0,		//CLS_LESS,			// '<'
		0,		//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,		//CLS_PERIOD,		// '.'
		0,		//CLS_NUMBER,		//
		0,		//CLS_SYMBOL,
		0,		//CLS_A,
		0,		//CLS_B,
		0,		//CLS_C,
		0,	 	//CLS_D,
		0,		//CLS_E,
		0,		//CLS_F,
		0,		//CLS_G,
		0,		//CLS_H,
		0,		//CLS_I,
		0,		//CLS_K,
		0,		//CLS_L,
		0,		//CLS_N,
		0,		//CLS_O,
		0,		//CLS_P,
		0,		//CLS_R,
		0,		//CLS_S,
		0,		//CLS_T,
		0,		//CLS_U,
		0,		//CLS_V,
		0,		//CLS_Y,
		0,		//CLS_W,
		0,		//CLS_X,
		0,		//CLS_R_A,	// À
		0,		//CLS_R_V,	// Â
		0,		//CLS_R_G,	// Ã
		0,		//CLS_R_D,	// Ä
		0,		//CLS_R_E,	// Å
		0,		//CLS_R_ZH,	// Æ
		0,		//CLS_R_Z,	// Ç
		0,		//CLS_R_I,	// È
		0,		//CLS_R_IY,	// É
		0,		//CLS_R_K,	// Ê
		0,		//CLS_R_L,	// Ë
		0,		//CLS_R_M,	// Ì
		0,		//CLS_R_N,	// Í
		0,		//CLS_R_O,	// Î
		0,		//CLS_R_P,	// Ï
		0,		//CLS_R_R,	// Ð
		0,		//CLS_R_S,	// Ñ
		0,		//CLS_R_T,	// Ò
		0,		//CLS_R_U,	// Ó
		0,		//CLS_R_C,	// Ö
		0,		//CLS_R_4,	// ×
		0,		//CLS_R_F,	// Ô
		0,		//CLS_R_MZ,	// Ü
		0,		//CLS_R_Y,	// Û
		0,		//CLS_R_e,	// Ý
		0,		//CLS_R_YU	// Þ
		0,		//CLS_R_YA,	// ß
		0,		//CLS_WHAT	// ?
		0,		//CLS_J,
		0,		//CLS_R_B,
		0,		//CLS_R_TZ,
	},
	{	// STATE_LPAR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_RPAR
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_COMMA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_SEMICOLON
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_EQUAL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_LBRACKET
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_RBRACKET
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_PLUS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_MINUS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_MULT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_PERCENT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_TILDA
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		STATE_LABEL,	//CLS_SYMBOL,
		STATE_LABEL,	//CLS_A,
		STATE_LABEL,	//CLS_B,
		STATE_LABEL,	//CLS_C,
		STATE_LABEL,	//CLS_D,
		STATE_LABEL,	//CLS_E,
		STATE_LABEL,	//CLS_F,
		STATE_LABEL,	//CLS_G,
		STATE_LABEL,	//CLS_H,
		STATE_LABEL,	//CLS_I,
		STATE_LABEL,	//CLS_K,
		STATE_LABEL,	//CLS_L,
		STATE_LABEL,	//CLS_N,
		STATE_LABEL,	//CLS_O,
		STATE_LABEL,	//CLS_P,
		STATE_LABEL,	//CLS_R,
		STATE_LABEL,	//CLS_S,
		STATE_LABEL,	//CLS_T,
		STATE_LABEL,	//CLS_U,
		STATE_LABEL,	//CLS_V,
		STATE_LABEL,	//CLS_Y,
		STATE_LABEL,	//CLS_W,
		STATE_LABEL,	//CLS_X,
		STATE_LABEL,	//CLS_R_A,	// À
		STATE_LABEL,	//CLS_R_V,	// Â
		STATE_LABEL,	//CLS_R_G,	// Ã
		STATE_LABEL,	//CLS_R_D,	// Ä
		STATE_LABEL,	//CLS_R_E,	// Å
		STATE_LABEL,	//CLS_R_ZH,	// Æ
		STATE_LABEL,	//CLS_R_Z,	// Ç
		STATE_LABEL,	//CLS_R_I,	// È
		STATE_LABEL,	//CLS_R_IY,	// É
		STATE_LABEL,	//CLS_R_K,	// Ê
		STATE_LABEL,	//CLS_R_L,	// Ë
		STATE_LABEL,	//CLS_R_M,	// Ì
		STATE_LABEL,	//CLS_R_N,	// Í
		STATE_LABEL,	//CLS_R_O,	// Î
		STATE_LABEL,	//CLS_R_P,	// Ï
		STATE_LABEL,	//CLS_R_R,	// Ð
		STATE_LABEL,	//CLS_R_S,	// Ñ
		STATE_LABEL,	//CLS_R_T,	// Ò
		STATE_LABEL,	//CLS_R_U,	// Ó
		STATE_LABEL,	//CLS_R_C,	// Ö
		STATE_LABEL,	//CLS_R_4,	// ×
		STATE_LABEL,	//CLS_R_F,	// Ô
		STATE_LABEL,	//CLS_R_MZ,	// Ü
		STATE_LABEL,	//CLS_R_Y,	// Û
		STATE_LABEL,	//CLS_R_e,	// Ý
		STATE_LABEL,	//CLS_R_YU	// Þ
		STATE_LABEL,	//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_LABEL,		//CLS_J,
		STATE_LABEL,		//CLS_R_B,
		STATE_LABEL,		//CLS_R_TZ,
	},
	{	// STATE_LABEL
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_LABEL,	//CLS_NUMBER,		//
		STATE_LABEL,	//CLS_SYMBOL,
		STATE_LABEL,	//CLS_A,
		STATE_LABEL,	//CLS_B,
		STATE_LABEL,	//CLS_C,
		STATE_LABEL,	//CLS_D,
		STATE_LABEL,	//CLS_E,
		STATE_LABEL,	//CLS_F,
		STATE_LABEL,	//CLS_G,
		STATE_LABEL,	//CLS_H,
		STATE_LABEL,	//CLS_I,
		STATE_LABEL,	//CLS_K,
		STATE_LABEL,	//CLS_L,
		STATE_LABEL,	//CLS_N,
		STATE_LABEL,	//CLS_O,
		STATE_LABEL,	//CLS_P,
		STATE_LABEL,	//CLS_R,
		STATE_LABEL,	//CLS_S,
		STATE_LABEL,	//CLS_T,
		STATE_LABEL,	//CLS_U,
		STATE_LABEL,	//CLS_V,
		STATE_LABEL,	//CLS_Y,
		STATE_LABEL,	//CLS_W,
		STATE_LABEL,	//CLS_X,
		STATE_LABEL,	//CLS_R_A,	// À
		STATE_LABEL,	//CLS_R_V,	// Â
		STATE_LABEL,	//CLS_R_G,	// Ã
		STATE_LABEL,	//CLS_R_D,	// Ä
		STATE_LABEL,	//CLS_R_E,	// Å
		STATE_LABEL,	//CLS_R_ZH,	// Æ
		STATE_LABEL,	//CLS_R_Z,	// Ç
		STATE_LABEL,	//CLS_R_I,	// È
		STATE_LABEL,	//CLS_R_IY,	// É
		STATE_LABEL,	//CLS_R_K,	// Ê
		STATE_LABEL,	//CLS_R_L,	// Ë
		STATE_LABEL,	//CLS_R_M,	// Ì
		STATE_LABEL,	//CLS_R_N,	// Í
		STATE_LABEL,	//CLS_R_O,	// Î
		STATE_LABEL,	//CLS_R_P,	// Ï
		STATE_LABEL,	//CLS_R_R,	// Ð
		STATE_LABEL,	//CLS_R_S,	// Ñ
		STATE_LABEL,	//CLS_R_T,	// Ò
		STATE_LABEL,	//CLS_R_U,	// Ó
		STATE_LABEL,	//CLS_R_C,	// Ö
		STATE_LABEL,	//CLS_R_4,	// ×
		STATE_LABEL,	//CLS_R_F,	// Ô
		STATE_LABEL,	//CLS_R_MZ,	// Ü
		STATE_LABEL,	//CLS_R_Y,	// Û
		STATE_LABEL,	//CLS_R_e,	// Ý
		STATE_LABEL,	//CLS_R_YU	// Þ
		STATE_LABEL,	//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		STATE_LABEL,	//CLS_J,
		STATE_LABEL,	//CLS_R_B,
		STATE_LABEL,	//CLS_R_TZ,
	},
	{	// STATE_COLON
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_LESS
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_GRAT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_PERIOD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NUMPERIOD,//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_WHAT
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		0,				//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
	{	// STATE_NUMPERIOD
		0,				//CLS_WHTSPC,		// ' ' \t
		0,				//CLS_REMARK,		// '/'
		0,				//CLS_QUOTE,		// '"'
		0,				//CLS_QUOTECONT,	// '|'
		0,				//CLS_LPAR,			// '('
		0,				//CLS_RPAR,			// ')'
		0,				//CLS_LBRACKET,		// '['
		0,				//CLS_RBRACKET,		// ']'
		0,				//CLS_COMMA,		// ','
		0,				//CLS_SEMICOLON,	// ';'
		0,				//CLS_EQUAL,		// '='
		0,				//CLS_CR,			//
		0,				//CLS_LF,			//
		0,				//CLS_PLUS,			// '+'
		0,				//CLS_MINUS,		// '-'
		0,				//CLS_MULT,			// '*'
		0,				//CLS_PERCENT,		// '%'
		0,				//CLS_TILDA,		// '~'
		0,				//CLS_COLON,		// ':'
		0,				//CLS_LESS,			// '<'
		0,				//CLS_GRAT,			// '>'
		0,				//CLS_APOSTROF,		// '''
		0,				//CLS_PERIOD,		// '.'
		STATE_NUMPERIOD,//CLS_NUMBER,		//
		0,				//CLS_SYMBOL,
		0,				//CLS_A,
		0,				//CLS_B,
		0,				//CLS_C,
		0,				//CLS_D,
		0,				//CLS_E,
		0,				//CLS_F,
		0,				//CLS_G,
		0,				//CLS_H,
		0,				//CLS_I,
		0,				//CLS_K,
		0,				//CLS_L,
		0,				//CLS_N,
		0,				//CLS_O,
		0,				//CLS_P,
		0,				//CLS_R,
		0,				//CLS_S,
		0,				//CLS_T,
		0,				//CLS_U,
		0,				//CLS_V,
		0,				//CLS_Y,
		0,				//CLS_W,
		0,				//CLS_X,
		0,				//CLS_R_A,	// À
		0,				//CLS_R_V,	// Â
		0,				//CLS_R_G,	// Ã
		0,				//CLS_R_D,	// Ä
		0,				//CLS_R_E,	// Å
		0,				//CLS_R_ZH,	// Æ
		0,				//CLS_R_Z,	// Ç
		0,				//CLS_R_I,	// È
		0,				//CLS_R_IY,	// É
		0,				//CLS_R_K,	// Ê
		0,				//CLS_R_L,	// Ë
		0,				//CLS_R_M,	// Ì
		0,				//CLS_R_N,	// Í
		0,				//CLS_R_O,	// Î
		0,				//CLS_R_P,	// Ï
		0,				//CLS_R_R,	// Ð
		0,				//CLS_R_S,	// Ñ
		0,				//CLS_R_T,	// Ò
		0,				//CLS_R_U,	// Ó
		0,				//CLS_R_C,	// Ö
		0,				//CLS_R_4,	// ×
		0,				//CLS_R_F,	// Ô
		0,				//CLS_R_MZ,	// Ü
		0,				//CLS_R_Y,	// Û
		0,				//CLS_R_e,	// Ý
		0,				//CLS_R_YU	// Þ
		0,				//CLS_R_YA,	// ß
		0,				//CLS_WHAT	// ?
		0,				//CLS_J,
		0,				//CLS_R_B,
		0,				//CLS_R_TZ,
	},
};

DWORD CLexer::m_ClassesOfSymbol[256]={
CLS_LAST,//  0 
0,//  1 
0,//  2 
0,//  3 
0,//  4 
0,//  5 
0,//  6 
0,//  7 
0,//  8 
CLS_WHTSPC,//  9 
CLS_LF,//  10 
0,//  11 
0,//  12 
CLS_CR,//  13 
0,//  14 
0,//  15 
0,//  16 
0,//  17 
0,//  18 
0,//  19 
0,//  20 
0,//  21 
0,//  22 
0,//  23 
0,//  24 
0,//  25 
0,//  26 
0,//  27 
0,//  28 
0,//  29 
0,//  30 
0,//  31 
CLS_WHTSPC,	//  32 
CLS_WHTSPC,	//  33 !
CLS_QUOTE,	//  34 "
CLS_WHTSPC,	//  35 #
CLS_WHTSPC,	//  36 $
CLS_PERCENT,	//  37 %
CLS_WHTSPC,	//  38 &
CLS_APOSTROF,	//  39 '
CLS_LPAR,		//  40 (
CLS_RPAR,		//  41 )
CLS_MULT,		//  42 *
CLS_PLUS,		//  43 +
CLS_COMMA,	//  44 ,
CLS_MINUS,	//  45 -
CLS_PERIOD,	//  46 .
CLS_REMARK,	//  47 /
CLS_NUMBER,	//  48 0
CLS_NUMBER,	//  49 1
CLS_NUMBER,	//  50 2
CLS_NUMBER,	//  51 3
CLS_NUMBER,	//  52 4
CLS_NUMBER,	//  53 5
CLS_NUMBER,	//  54 6
CLS_NUMBER,	//  55 7
CLS_NUMBER,	//  56 8
CLS_NUMBER,	//  57 9
CLS_COLON,	//  58 :
CLS_SEMICOLON,//  59 ;
CLS_LESS,		//  60 <
CLS_EQUAL,	//  61 =
CLS_GRAT,		//  62 >
CLS_WHAT,	//  63 ?
CLS_WHTSPC,	//  64 @
CLS_A,		//  65 A
CLS_B,		//  66 B
CLS_C,		//  67 C
CLS_D,		//  68 D
CLS_E,		//  69 E
CLS_F,		//  70 F
CLS_G,		//  71 G
CLS_H,		//  72 H
CLS_I,		//  73 I
CLS_J,	//  74 J
CLS_K,		//  75 K
CLS_L,		//  76 L
CLS_SYMBOL,	//  77 M
CLS_N,		//  78 N
CLS_O,		//  79 O
CLS_P,		//  80 P
CLS_SYMBOL,	//  81 Q
CLS_R,		//  82 R
CLS_S,		//  83 S
CLS_T,		//  84 T
CLS_U,		//  85 U
CLS_V,		//  86 V
CLS_W,		//  87 W
CLS_X,		//  88 X
CLS_Y,		//  89 Y
CLS_SYMBOL,	//  90 Z
CLS_LBRACKET,	//  91 [
CLS_WHTSPC,	//  92 '\'
CLS_RBRACKET,	//  93 ]
CLS_WHTSPC,	//  94 ^
CLS_SYMBOL,	//  95 _
0,//  96 `
CLS_A,		//  97 a
CLS_B,		//  98 b
CLS_C,		//  99 c
CLS_D,		//  100 d
CLS_E,		//  101 e
CLS_F,		//  102 f
CLS_G,		//  103 g
CLS_H,		//  104 h
CLS_I,		//  105 i
CLS_J,		//  106 j
CLS_K,		//  107 k
CLS_L,		//  108 l
CLS_SYMBOL,	//  109 m
CLS_N,		//  110 n
CLS_O,		//  111 o
CLS_P,		//  112 p
CLS_SYMBOL,	//  113 q
CLS_R,		//  114 r
CLS_S,		//  115 s
CLS_T,		//  116 t
CLS_U,		//  117 u
CLS_V,		//  118 v
CLS_W,		//  119 w
CLS_X,		//  120 x
CLS_Y,		//  121 y
CLS_SYMBOL,	//  122 z
CLS_WHTSPC,	//  123 {
CLS_QUOTECONT,//  124 |
CLS_WHTSPC,	//  125 }
CLS_TILDA,	//  126 ~
0,//  127 
CLS_SYMBOL,//  128 €
CLS_SYMBOL,//  129 
0,//  130 ‚
CLS_SYMBOL,//  131 ƒ
0,//  132 „
0,//  133 …
0,//  134 †
0,//  135 ‡
0,//  136 ˆ
0,//  137 ‰
CLS_SYMBOL,//  138 Š
0,//  139 ‹
CLS_SYMBOL,//  140 Œ
CLS_SYMBOL,//  141 
CLS_SYMBOL,//  142 Ž
CLS_SYMBOL,//  143 
CLS_SYMBOL,//  144 
0,//  145 ‘
0,//  146 ’
0,//  147 “
0,//  148 ”
0,//  149 •
0,//  150 –
CLS_MINUS,//  151 —
0,//  152 ˜
0,//  153 ™
CLS_SYMBOL,//  154 š
0,//  155 ›
CLS_SYMBOL,//  156 œ
CLS_SYMBOL,//  157 
CLS_SYMBOL,//  158 ž
CLS_SYMBOL,//  159 Ÿ
0,//  160  
CLS_SYMBOL,//  161 ¡
CLS_SYMBOL,//  162 ¢
CLS_SYMBOL,//  163 £
0,//  164 ¤
CLS_SYMBOL,//  165 ¥
0,//  166 ¦
0,//  167 §
CLS_SYMBOL,//  168 ¨
0,//  169 ©
CLS_SYMBOL,//  170 ª
0,//  171 «
0,//  172 ¬
0,//  173 ­
0,//  174 ®
CLS_SYMBOL,//  175 ¯
0,//  176 °
0,//  177 ±
CLS_SYMBOL,//  178 ²
CLS_SYMBOL,//  179 ³
CLS_SYMBOL,//  180 ´
CLS_SYMBOL,//  181 µ
0,//  182 ¶
0,//  183 ·
CLS_SYMBOL,//  184 ¸
0,//  185 ¹
CLS_SYMBOL,//  186 º
0,//  187 »
CLS_SYMBOL,//  188 ¼
CLS_SYMBOL,//  189 ½
CLS_SYMBOL,//  190 ¾
CLS_SYMBOL,//  191 ¿
CLS_R_A,		//  192 À
CLS_R_B,		//  193 Á
CLS_R_V,		//  194 Â
CLS_R_G,		//  195 Ã
CLS_R_D,		//  196 Ä
CLS_R_E,		//  197 Å
CLS_R_ZH,		//  198 Æ
CLS_R_Z,		//  199 Ç
CLS_R_I,		//  200 È
CLS_R_IY,		//  201 É
CLS_R_K,		//  202 Ê
CLS_R_L,		//  203 Ë
CLS_R_M,		//  204 Ì
CLS_R_N,		//  205 Í
CLS_R_O,		//  206 Î
CLS_R_P,		//  207 Ï
CLS_R_R,		//  208 Ð
CLS_R_S,		//  209 Ñ
CLS_R_T,		//  210 Ò
CLS_R_U,		//  211 Ó
CLS_R_F,		//  212 Ô
CLS_SYMBOL,	//  213 Õ
CLS_R_C,		//  214 Ö
CLS_R_4,		//  215 ×
CLS_SYMBOL,	//  216 Ø
CLS_SYMBOL,	//  217 Ù
CLS_R_TZ,	//  218 Ú
CLS_R_Y,		//  219 Û
CLS_R_MZ,		//  220 Ü
CLS_R_e,		//  221 Ý
CLS_R_YU,		//  222 Þ
CLS_R_YA,		//  223 ß
CLS_R_A,		//  224 à
CLS_R_B,		//  225 á
CLS_R_V,		//  226 â
CLS_R_G,		//  227 ã
CLS_R_D,		//  228 ä
CLS_R_E,		//  229 å
CLS_R_ZH,		//  230 æ
CLS_R_Z,		//  231 ç
CLS_R_I,		//  232 è
CLS_R_IY,		//  233 é
CLS_R_K,		//  234 ê
CLS_R_L,		//  235 ë
CLS_R_M,		//  236 ì
CLS_R_N,		//  237 í
CLS_R_O,		//  238 î
CLS_R_P,		//  239 ï
CLS_R_R,		//  240 ð
CLS_R_S,		//  241 ñ
CLS_R_T,		//  242 ò
CLS_R_U,		//  243 ó
CLS_R_F,		//  244 ô
CLS_SYMBOL,	//  245 õ
CLS_R_C,		//  246 ö
CLS_R_4,		//  247 ÷
CLS_SYMBOL,	//  248 ø
CLS_SYMBOL,	//  249 ù
CLS_R_TZ,	//  250 ú
CLS_R_Y,		//  251 û
CLS_R_MZ,		//  252 ü
CLS_R_e,		//  253 ý
CLS_R_YU,		//  254 þ
CLS_R_YA,		//  255 ÿ
};
#endif