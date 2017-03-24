
#include "parser.h"
#include "CommonFunctions.h"

int ObjCount = 0;

CMapPtrToPtr CMemCollector::MemMap;
long CMemCollector::maxRecords = -1;

void CMemCollector::Add(CCode* code)
{
	return;
	if( maxRecords == -1 )
	{
		MemMap.InitHashTable(33735);
		maxRecords = 0;
	}

	MemMap.SetAt(code, NULL);

	int cnt = MemMap.GetCount();
	if( cnt > maxRecords ) maxRecords = cnt;

}

void CMemCollector::Remove(CCode* code)
{
	return;
	MemMap.RemoveKey(code);
}

void CMemCollector::Free()
{
	return;
	void *code, *ptr;
	POSITION pos = MemMap.GetStartPosition();
	while( pos != NULL )
	{
		MemMap.GetNextAssoc(pos, code, ptr);
		MemMap.RemoveKey(code);
		delete ((CCode*)code);

		POSITION pos = MemMap.GetStartPosition();
	}
	MemMap.RemoveAll();
}


CCode::CCode(YYLTYPE& yyloc, TCodeType ct)
{
	SetLoc(yyloc);
	CodeType = ct;

	ObjCount++;
	CMemCollector::Add(this);
}

void CCode::SetLoc(YYLTYPE& yyloc)
{
	StartLine = yyloc.first_line;
	StartCol = yyloc.first_column;
	EndLine = yyloc.last_line;
	EndCol = yyloc.last_column - 1;
}

void CCode::TypeNode(int level)
{
	//for( int j = 0; j < level; j++ ) printf("|  ");
	//if( code->CodeType == CT_LValue )
	//	Msg("+- %s  (%s) (%i,%i - %i,%i)\n", code->GetText(), CodeTypeAsStr(code->CodeType), code->StartLine, code->StartCol, code->EndLine, code->EndCol);
	//else
	//	Msg("+- %s  (%s) (%i,%i - %i,%i)", code->GetText(), CodeTypeAsStr(code->CodeType), code->StartLine, code->StartCol, code->EndLine, code->EndCol);
	//fflush(stdout);

	debug(" %s  (%s) (%i,%i - %i,%i)", GetText(), CodeTypeAsStr(CodeType), StartLine, StartCol, EndLine, EndCol);
}

void CCode::PrintTree(int level)
{
	POSITION pos;
	pos = Parts.GetHeadPosition();
	for( int i = 0; i < Parts.GetCount(); i++ )
	{
		CCode* code = (CCode*)Parts.GetAt(pos);

		code->TypeNode(level);

		code->PrintTree(level+1);

		Parts.GetNext(pos);
	}
}

void CCode::DestroyNodes()
{
	while( !Parts.IsEmpty() )
	{
		CCode* code = (CCode*)Parts.RemoveHead();
		//code->TypeNode();
		delete code;
	}
}

CCode::~CCode()
{
	CMemCollector::Remove(this);
	DestroyNodes();
	ObjCount--;
}

void CCode::AddHead(CCode* part)
{
	if( part != NULL )
		Parts.AddHead((void*)part);
}

void CCode::AddTail(CCode* part)
{
	if( part != NULL )
		Parts.AddTail((void*)part);
}

void CCode::AddTail(int nParts, ...)
{
	va_list ap;
	va_start(ap, nParts);

	while( nParts-- > 0 )
	{
		AddTail(va_arg(ap, CCode*));
	}
	
	va_end(ap);
}

CCode* CCode::GetFirst()
{
	return (CCode*)Parts.GetHead();
}

CCode* CCode::GetLast()
{
	return (CCode*)Parts.GetTail();
}

CLexema* CCode::GetLeftmostLexema()
{
	if( CodeType == CT_Lexema )
		return (CLexema*)this;

	CCode* code = GetFirst();
	while( code && code->CodeType != CT_Lexema )
	{
		code = code->GetFirst();
	}
	return (CLexema*)code;
}



CString CCode::GetText()
{
	CString Text;
	CCodeIterator Iter(this);
	CCode* code = Iter.First();
	while( code != NULL )
	{
		Text += code->GetText();
		//Text += " ";
		code = Iter.Next();

	}
	
	return Text;
}

const char* CCode::CodeTypeAsStr(TCodeType type)
{
	switch( type )
	{
	case CT_Error: return "CT_Error";
	case CT_Lexema: return "CT_Lexema";
	case CT_VarDef: return "CT_VarDef";
	case CT_VarDefList: return "CT_VarDefList";
	case CT_VarDecl: return "CT_VarDecl";
	case CT_VarDeclList: return "CT_VarDeclList";
	case CT_Expr: return "CT_Expr";
	case CT_Operator: return "CT_Operator";
	case CT_Statement: return "CT_Statement";
	case CT_Statements: return "CT_Statements";
	case CT_LogicalExpr: return "logical";
	case CT_LValue: return "CT_LValue";
	case CT_If: return "CT_If";
	case CT_Else: return "CT_Else";
	case CT_ElseIf: return "CT_ElseIf";
	case CT_While: return "CT_While";
	case CT_For: return "CT_For";
	case CT_Try: return "CT_Try";
	case CT_MethCall: return "CT_MethCall";
	case CT_CallParam: return "CT_CallParam";
	case CT_CallParamList: return "CT_CallParamList";
	case CT_MethHeader: return "CT_MethHeader";
	case CT_ParamList: return "CT_ParamList";
	case CT_Param: return "CT_Param";
	case CT_Procedure: return "CT_Procedure";
	case CT_Function: return "CT_Function";
	case CT_MethList: return "CT_MethList";
	case CT_Module: return "CT_Module";
	}
	return "";
}

bool CCode::HasErrors()
{
	if( CodeType == CT_Error ) return true;

	POSITION pos;
	pos = Parts.GetHeadPosition();
	for( int i = 0; i < Parts.GetCount(); i++ )
	{
		CCode* code = (CCode*)Parts.GetAt(pos);
		if( code->CodeType == CT_Error )
		{
			return true;
		}
		else
		{
			if( code->HasErrors() ) return true;
		}


		Parts.GetNext(pos);
	}

	return false;
}


CLexema::CLexema(YYLTYPE& yyloc, const char* text, TLexemaType type)
:CCode(yyloc)
{
	CodeType = CT_Lexema;
	Text = text;
	LexType = type;
	KeyWord = -1;
}


CString CLexema::GetText()
{
	return (LPCSTR)Text;
}


CCode* CCodeIterator::First()
{
	head_pos = Parts->GetHeadPosition();
	tail_pos = Parts->GetTailPosition();
	pos = head_pos;
	if( pos == NULL ) return NULL;
	return (CCode*)Parts->GetAt(pos);
}

CCode* CCodeIterator::Last()
{
	head_pos = Parts->GetHeadPosition();
	tail_pos = Parts->GetTailPosition();
	pos = tail_pos;
	if( pos == NULL ) return NULL;
	return (CCode*)Parts->GetAt(pos);
}

CCode* CCodeIterator::Next()
{
	if( pos == tail_pos ) return NULL;
	Parts->GetNext(pos);
	return (CCode*)Parts->GetAt(pos);
}

CCode* CCodeIterator::Prev()
{
	if( pos == head_pos ) return NULL;
	Parts->GetPrev(pos);
	return (CCode*)Parts->GetAt(pos);
}

