
#include "Analyser.h"
#include "Types1C.h"
#include "CommonFunctions.h"



//===========================================================================
CExecutionContext::CExecutionContext(CCode* pCode)
:CCodeContainer(pCode)
{
	pParentContext = NULL;
};

CExecutionContext::CExecutionContext(CCode* pCode, CExecutionContext* pParentCont)
:CCodeContainer(pCode)
{
	pParentContext = pParentCont;
};

CCode* CExecutionContext::GetVarDeclList()
{
	CCodeIterator Iter(Code);
	for( CCode* Vars = Iter.First(); Vars; Vars = Iter.Next() )
	{
		if( Vars->CodeType == CT_VarDeclList )
			break;
	}
	return Vars;
}

CCode* CExecutionContext::GetStatements()
{
	CCodeIterator Iter(Code);
	for( CCode* Statements = Iter.First(); Statements; Statements= Iter.Next() )
	{
		if( Statements->CodeType == CT_Statements )
			return Statements;
	}
	return NULL;
}

bool CExecutionContext::SetDeclaredVarType(const char* VarName, const char* VarType)
{
	CVariable* pVar = DeclaredVars.GetVar(VarName);

	if( VarType && *VarType != '\0' )
	{
		if( pVar )
			pVar->Type = VarType;

	}

	if( pParentContext )
		return pParentContext->SetDeclaredVarType(VarName, VarType);
	else
		return (pVar != NULL);
}

void CExecutionContext::AddTypedVar(const char* VarName, const char* VarType)
{
	bool var_declared = SetDeclaredVarType(VarName, VarType);

	if( VarType && *VarType != '\0' )
	{
		CVariable* pAutoVar = AutomaticVars.GetVar(VarName);
		CVariable* pVar = DeclaredVars.GetVar(VarName);

		if( pVar )
			pVar->Type = VarType;
		if( pAutoVar )
			pAutoVar->Type = VarType;

		if( (!var_declared) && pAutoVar == NULL )
			AutomaticVars.Add(VarName, VarType);

	}
}

void CExecutionContext::AddVarProp(const char* VarName, const char* PropName, const char* PropType)
{
	CString csVarName = GetVarType(VarName);
	CType1C* pType = CTypes1C::GetType(csVarName);
	if( pType && pType->PropExists(PropName) )
		return; //свойство не динамическое (т.е. является свойством типа) - в переменную добавлять не нужно

	CVariable* pVar = GetVar(VarName);
	if( pVar )
	{
		pVar->AddProperty(PropName, PropType);
		Msg("AddVarProp %s, %s, %s", VarName, PropName, PropType);
	}
}

CVariable* CExecutionContext::GetVar(const char* VarName)
{
	CVariable* pVar = DeclaredVars.GetVar(VarName);
	if( pVar ) return pVar;
	
	pVar = AutomaticVars.GetVar(VarName);

	if( !pVar && pParentContext )
		return pParentContext->GetVar(VarName);

	return pVar;
}

const char* CExecutionContext::GetVarType(const char* VarName)
{
	CVariable* pVar = GetVar(VarName);
	if( pVar )
		return pVar->Type;
	else
		return NULL;
}

void CExecutionContext::ReadVarDeclarations(CCode* pCode)
{
	CCodeIterator Iter(pCode);
	for( CCode* code = Iter.First(); code; code = Iter.Next() )
	{
		if( code->CodeType == CT_VarDef )
		{
			CString VarName = code->GetFirst()->GetText();
			DeclaredVars.Add(VarName, "");
		}
		else
		{
			ReadVarDeclarations(code);
		}
	}
}

void CExecutionContext::AnalyseStatements(CCode* pCode)
{
	CStatements Stmt(pCode, this);
	Stmt.Analyse();
}

void CExecutionContext::Analyse(CCode* pCode)
{
	TypeFound = "";

	CCode* VarDeclList = GetVarDeclList();
	if( VarDeclList )
	{
		ReadVarDeclarations(VarDeclList);
	}

	CCode* Statements = GetStatements();
	if( Statements )
	{
		AnalyseStatements(Statements);
	}
}

void CExecutionContext::SetTypeFound(const char* type)
{
	TypeFound = type;
	if( pParentContext )
		pParentContext->SetTypeFound(type);
}

bool CExecutionContext::CheckDesiredPos(CCode* left, CCode* right, const char* Type)
{
	return false;

	if( DesiredLine < 0 || DesiredCol < 0 )
	{
		if( pParentContext )
			return pParentContext->CheckDesiredPos(left, right, Type);
		else
			return false;
	}

	CString LeftText = left->GetText();

	if( left->EndLine == DesiredLine && left->EndCol == DesiredCol )
	{
		Msg("DESIRED TYPE (1): %s   %i", Type, Code->EndCol);
		SetTypeFound(Type);
		return true;
	}

	if( right )
	{
		CString RightText = right->GetText();
		if( left->EndLine == DesiredLine && left->EndCol < DesiredCol &&
			(
				(DesiredLine <= right->EndLine) ||
				(DesiredLine == right->EndLine && DesiredCol <= right->EndCol)
			)
		)
		{
			SetTypeFound(Type);
			Msg("DESIRED TYPE (2): %s   %i", Type, Code->EndCol);
			return true;
		}
	}

	return false;
}


//===========================================================================
CModule::~CModule()
{
	CString MethName;
	void* meth;
	POSITION pos = Methods.GetStartPosition();
	while( pos != NULL )
	{
		Methods.GetNextAssoc(pos, MethName, meth);
		delete ((CMethod*)meth);
	}
}

void CModule::AddMethod(CString MethName, CMethod* Method)
{
	Methods.SetAt(MethName, Method);
}

CMethod* CModule::GetMethod(CString MethName, bool bScanIfNotInList)
{
	CMethod* Method = (CMethod*)(Methods[MethName]);
	if( Method == NULL && bScanIfNotInList )
	{
		Analyse((const char*)MethName);
		Method = (CMethod*)(Methods[MethName]);
	}

	if( Method == NULL )
	{
		Method = pParentContext->GetMethod(MethName, bScanIfNotInList);
	}

	return Method;
}

CCode* CModule::GetMethList()
{
	CCodeIterator Iter(Code);
	for( CCode* Methods = Iter.First(); Methods; Methods = Iter.Next() )
	{
		if( Methods->CodeType == CT_MethList )
			break;
	}
	return Methods;
}

const char* CModule::GetReturnedType(const char* MethodName)
{
	CMethod* Method = GetMethod(MethodName);
	if( Method )
		return Method->GetReturnedType();
	else
		return NULL;
}



void CModule::Analyse(const char* DesiredMethName)
{
	CCode* Methods = GetMethList();
	if( !Methods )
		return;

	CCodeIterator Iter(Methods);
	for( CCode* meth_code = Iter.First(); meth_code; meth_code = Iter.Next() )
	{
		CMethod* Method = new CMethod(meth_code, this);
		CString MethName = Method->GetName();
		if( MethName.CollateNoCase(DesiredMethName) != 0 )
		{
			delete Method;
			continue;
		}

		AddMethod(Method->GetName(), Method);
		Method->Analyse();
		return;
	}
}

void CModule::Analyse()
{
	CExecutionContext::Analyse(Code);


	CCode* Methods = GetMethList();
	if( !Methods )
		return;

	CCodeIterator Iter(Methods);
	for( CCode* meth_code = Iter.First(); meth_code; meth_code = Iter.Next() )
	{
		if( DesiredLine >= 0 && 
			(meth_code->StartLine > DesiredLine || meth_code->EndLine < DesiredLine) )
		{
			continue;
		}

		if( meth_code->CodeType == CT_Error )
			continue;

		CMethod* Method = new CMethod(meth_code, this);
		CString MethName = Method->GetName();
		if( MethName.IsEmpty() )
		{
			delete Method;
			continue; //something wrong with this method!
		}

		MethodFound = (LPCSTR)MethName;

		if( this->Methods[MethodFound] == NULL )
		{
			AddMethod(Method->GetName(), Method);
			Method->DesiredLine = DesiredLine;
			Method->DesiredCol = DesiredCol;
			Method->Analyse();
		}
		else
		{
			delete Method;
		}

		if( DesiredLine >= 0 )
			break;
	}
}

//===========================================================================
CMethod::CMethod(CCode* pCode, CModule* pMod)
:CExecutionContext(pCode, pMod)
{
	ParentModule = pMod;

	MethodName = NULL;
	ParamList = NULL;
	DesiredLine = -1; 
	DesiredCol = -1;
}

void CMethod::ParseHeader()
{
	CCodeIterator Iter(Code);
	CCode* code = Iter.First(); //type: func or proc
	CCode* MethHeader = Iter.Next();

	if( MethHeader == NULL ) //Ошибка разбора хедера
		return;

	Iter.Reset(MethHeader);
	MethodName = (CLexema*)(Iter.First());

	ParamList = Iter.Next();
}

CString CMethod::GetName()
{
	if( !MethodName )
		ParseHeader();

	if( MethodName == NULL )
		return "";
	else
		return MethodName->GetText();
}

void CMethod::FillParamList()
{
	int param_no = 0;
	CCodeIterator Iter(ParamList);
	for( CCode* param = Iter.First(); param; param = Iter.Next() )
	{
		if( param->CodeType != CT_Param ) continue;

		param_no++;

		CCodeIterator ParamIter(param);
		for( CCode* param_part = ParamIter.First(); param_part; param_part = ParamIter.Next() )
		{
			if( ((CLexema*)param_part)->KeyWord == val ) //Параметры по значению нас не интересуют
				break;

			if( ((CLexema*)param_part)->LexType == LT_Word )
			{
				CFormalParameter* Param = new CFormalParameter("");
				Param->Number = param_no;
				FormalParameters.Add(param_part->GetText(), Param);
				break;
			}
		}
	}
}


const char* CMethod::GetReturnedType(const char* MethodName)
{
	return ParentModule->GetReturnedType(MethodName);
}

void CMethod::AddTypedVar(const char* VarName, const char* VarType)
{
	CFormalParameter* param = (CFormalParameter*)FormalParameters.GetVar(VarName);
	if( param )
	{
		param->Type = VarType;
	}
	else
	{
		CExecutionContext::AddTypedVar(VarName, VarType);
	}
}

void CMethod::AddVarProp(const char* VarName, const char* PropName, const char* PropType)
{
	CFormalParameter* param = (CFormalParameter*)FormalParameters.GetVar(VarName);
	if( param )
	{
		param->AddProperty(PropName, PropType);
	}
	else
	{
		CExecutionContext::AddVarProp(VarName, PropName, PropType);
	}
}

CVariable* CMethod::GetVar(const char* VarName)
{
	CFormalParameter* param = (CFormalParameter*)FormalParameters.GetVar(VarName);
	if( param )
	{
		return param;
	}
	else
	{
		return CExecutionContext::GetVar(VarName);
	}
}


CString CMethod::GetFormalParameterType(int param_no)
{
	POSITION pos = FormalParameters.GetStartPosition();
	CString ParamName;
	CFormalParameter* Param;
	while( pos )
	{
		FormalParameters.GetNextAssoc(pos, ParamName, (void*&)Param);
		if( Param->Number == param_no )
			return Param->Type;
	}

	return "";
}

CString CMethod::GetFormalParameterType(CString& MethName, int param_no)
{
	CMethod* Method = ParentModule->GetMethod(MethName);
	if( Method )
		return Method->GetFormalParameterType(param_no);
	else
		return "";
}



void CMethod::Analyse()
{
	ParseHeader();
	FillParamList();

	CCode* StatementsCode = GetStatements();
	if( StatementsCode )
	{
		CStatements Statements(StatementsCode, this);
		Statements.Analyse();

		if( !Statements.ReturnedType.IsEmpty() )
		{
			Msg("RETURN: %s", Statements.ReturnedType);
		}

		PrintFormalParameters();
	}
}


void CMethod::PrintHeader()
{
	CCodeIterator Iter(Code);
	CCode* MethType = Iter.First();
	CCode* MethHeader = Iter.Next();

	Msg("\n//==================================================");
	Msg("%s %s", MethType->GetText(), MethHeader->GetText());
}

void CMethod::PrintFormalParameters()
{
	POSITION pos = FormalParameters.GetStartPosition();
	CString ParamName;
	CFormalParameter* Param;
	while( pos )
	{
		FormalParameters.GetNextAssoc(pos, ParamName, (void*&)Param);
		if( !Param->Type.IsEmpty() )
			Msg("\tParam %i: '%s', Type '%s'", Param->Number, ParamName, Param->Type);
	}
}


//===========================================================================
void CStatements::RecursiveAnalyse(CExecutionContext* Context, CCode* Code)
{
	CCodeIterator Iter(Code);
	CCode* code = Iter.First();
	while( code != NULL)
	{
		bool ScanSubtree = true;

		CString tmpText = code->GetText();

		if( code->CodeType == CT_Error )
		{
			Msg("\n============ err =============\n");
			code->PrintTree();
			Msg("\n==============================\n");
		}
		else if( code->CodeType == CT_Operator )
		{
			COperator op(code);
			if( op.IsAssignment() )
			{
				op.AnalyseAssignment(Context);
				ScanSubtree = false;
			}
			else
			{
				CString ReturnedType;
				if( op.IsReturn(Context, ReturnedType) )
				{
					if( !ReturnedType.IsEmpty() )
					{
						Context->SetReturnedType(ReturnedType);
						//Msg("RETURN %s", ReturnedType);
					}
					ScanSubtree = false;
				}
			}
		}
		else if( code->CodeType == CT_LValue )
		{
			CLValue lval(code);
			CString type = lval.GetType1C(Context);
			Msg("Expr %s; type = '%s'", code->GetText(), type);
			ScanSubtree = false;
		}
		else if( code->CodeType == CT_MethCall )
		{
			CMethCall mc(code);
			CString type = mc.GetType1C(Context);
			Msg("Expr %s; type = '%s'", code->GetText(), type);
			ScanSubtree = false;
		}

		if( ScanSubtree )
			RecursiveAnalyse(Context, code);


		code = Iter.Next();
	}
}

void CStatements::Analyse()
{
	RecursiveAnalyse(pParentContext, Code);
}


//===========================================================================
CString CMethCall::GetMethName()
{
	CCodeIterator Iter(Code);
	CCode* Name = Iter.First();
	if( Name->CodeType != CT_Lexema )
		return "";

	return ((CLexema*)Name)->GetText();
}

CCode* CMethCall::GetParams()
{
	if( Params == NULL )
	{
		CCodeIterator Iter(Code);
		for( CCode* code = Iter.Last(); code; code = Iter.Prev() )
		{
			if( code->CodeType == CT_CallParamList )
			{
				Params = code;
				break;
			}
			else if( code->CodeType == CT_MethCall )
			{
				Iter.Reset(code);
				Iter.Last();
			}
		}
	}

	return Params;
}

bool CMethCall::ExtractLeftmostString(CCode* pCode, CString& strRes)
{
	if( pCode )
	{
		CLexema* FirstLexema = pCode->GetLeftmostLexema();
		if( FirstLexema && FirstLexema->LexType == LT_String )
		{
			strRes = FirstLexema->GetText();
			UnquoteString(strRes);
			CutLastDot(strRes);
			return true;
		}
	}
	return false;
}

bool CMethCall::IsObjectCreatingMethod(CString& MethName, CString& Type)
{
	if(    MethName.CollateNoCase("СоздатьОбъект") == 0 
		|| MethName.CollateNoCase("CreateObject") == 0 
		|| MethName.CollateNoCase("ПолучитьПустоеЗначение") == 0 
		)
	{
		CString TypeName;
		if( ExtractLeftmostString(GetParams(), TypeName) )
		{
			Type = (LPCSTR)TypeName;
			return true;
		}
	}
	return false;
}

void CMethCall::ModifyExemplar(CVariable* pVar, CTypeExemplarModifyingMethod* pModMeth)
{
	CString PropName, PropType;
	CCode* pParams = GetParams();
	int nParam = 0;

	CCodeIterator Iter(GetParams());
	for( CCode* pParam = Iter.First(); pParam; pParam = Iter.Next() )
	{
		if( pParam->CodeType == CT_Lexema && pParam->GetText() == "," )
			continue;

		nParam++;
		if( nParam == pModMeth->nParam_PropName )
			ExtractLeftmostString(pParam, PropName);
		else if( nParam == pModMeth->nParam_PropType )
			ExtractLeftmostString(pParam, PropType);
	}

	if( !PropName.IsEmpty() )
		pVar->AddProperty(PropName, PropType);
}


CString CMethCall::GetType1C(CExecutionContext* Context)
{
	CCodeIterator Iter(Code);
	CCode* left = Iter.First();

	if( left->CodeType == CT_Lexema ) //Вызов метода в чистом виде
	{
		UsedVars(Context, CString(""));

		CString MethName = left->GetText();
		CString Type;
		if( IsObjectCreatingMethod(MethName, Type) )
		{
			return Type;
		}
		else
		{
			CString MethType = Context->GetReturnedType(MethName);
			if( !MethType.IsEmpty() )
				return MethType;
		}
	}
	else
	{
		CString LeftPartType;
		if( left->CodeType == CT_MethCall )
		{
			CMethCall mc(left);
			LeftPartType = mc.GetType1C(Context);
		}
		else if( left->CodeType == CT_LValue )
		{
			CLValue lval(left);
			LeftPartType = lval.GetType1C(Context);
		}
		else
		{
			return "";
		}

		if( Context->CheckDesiredPos(left, NULL, LeftPartType) )
		{
			//return LeftPartType;
		}

		Iter.Next(); //skip '.'
		CMethCall mcRight(Iter.Next());

		mcRight.UsedVars(Context, LeftPartType);

		if( Context->CheckDesiredPos(left, mcRight.Code, LeftPartType) )
		{
			//return LeftPartType;
		}

		CString RightPartType = mcRight.GetSubType(LeftPartType);

		//Слева - переменная. 
		if( left->CodeType == CT_LValue && left->GetPartsCount() == 1 )
		{
			CString VarName = left->GetText();
			CString MethodName = mcRight.GetMethName();

			//сообщим переменной, что у неё есть новое свойство, раз уж кто-то его поюзал
			Context->AddVarProp(VarName, MethodName, RightPartType);

			//Метод может добавлять свойства к экземпляру типа. Проверим, и добавим.
			if( !LeftPartType.IsEmpty() )
			{
				CType1C* pType = CTypes1C::GetType(LeftPartType);
				if( pType )
				{
					CTypeExemplarModifyingMethod* pModMeth = pType->GetExemplarModifyingMethod(MethodName);
					if( pModMeth )
					{
						CVariable *pVar = Context->GetVar(VarName);
						if( !pVar )
							pVar = Context->AddAutomaticVar(VarName, LeftPartType);
						mcRight.ModifyExemplar(pVar, pModMeth);
					}
				}
			}
		}

		return RightPartType;
	}


	return "";
}

CString CMethCall::GetSubType(CString& LeftPartType)
{
	CCodeIterator Iter(Code);
	CCode* left = Iter.First();
	
	if( left->CodeType == CT_Lexema )
	{
		return CTypes1C::GetSubType(LeftPartType, left->GetText());
	}

	return "";
}

//Анализируем параметры, проинициализированные в вызываемом методе,
//и добавляем их в список локальных переменных с типами
void CMethCall::UsedVars(CExecutionContext* Context, CString& LeftPartType)
{
	CCode* param_list = GetParams();
	if( param_list == NULL )
		return;

	CCodeIterator Iter(param_list);
	int param_number = 0;
	
	CCode* param = Iter.First();
	while( param )
	{
		if( param->CodeType == CT_CallParam )
		{
			param_number++;

			CCode* expr = param->GetFirst();
			if( expr->GetPartsCount() == 1 )
			{
				expr = expr->GetFirst();
				if( expr->CodeType == CT_LValue && expr->GetPartsCount() == 1 )
				{
					expr = expr->GetFirst();
					if( expr->CodeType == CT_Lexema )
					{
						CString ReturnedType = Context->GetFormalParameterType(GetMethName(), param_number);
						if( !ReturnedType.IsEmpty() )
							Context->AddTypedVar(expr->GetText(), ReturnedType);
					}
				}
			}
		}

		param = Iter.Next();
	}
}

//===========================================================================
CString CLValue::GetType1C(CExecutionContext* Context)
{
	CCodeIterator Iter(Code);
	CCode* left = Iter.First();
	CString LeftPartType;

	if( left->CodeType == CT_Lexema )
	{
		//самая левая переменная - её тип надо поискать в текущем контексте
		LeftPartType = Context->GetVarType(left->GetText());
		Msg("   LVal %s : %s", left->GetText(), LeftPartType);
	}
	else
	{
		if( left->CodeType == CT_MethCall )
		{
			CMethCall mc(left);
			LeftPartType = mc.GetType1C(Context);
		}
		else if( left->CodeType == CT_LValue )
		{
			CLValue lval(left);
			LeftPartType = lval.GetType1C(Context);
		}
	}

	if( Context->CheckDesiredPos(left, NULL, LeftPartType) )
	{
		//return LeftPartType;
	}


	CCode* code = Iter.Next();
	if( code == NULL )
		return LeftPartType;

	if( code->CodeType != CT_Lexema && ((CLexema*)code)->GetText() != '.' )
		return LeftPartType; //There must be a '.' but found something else

	CCode* right = Iter.Next();
	if( right == NULL )
		return LeftPartType; //Something wrong

	if( Context->CheckDesiredPos(left, right, LeftPartType) )
	{
		//return LeftPartType;
	}

	if( right->CodeType == CT_Lexema )
	{
		CString RightPartType = CTypes1C::GetSubType(LeftPartType, right->GetText());

		if( left->CodeType == CT_LValue ) //сообщим переменной, что у неё есть новое свойство, раз уж кто-то его поюзал
			Context->AddVarProp(left->GetText(), right->GetText(), RightPartType);

		return RightPartType;
	}
	else
	{
		CLValue lvalRight(right);
		return lvalRight.GetSubType(LeftPartType);
	}
}

CString CLValue::GetSubType(CString& LeftPartType)
{
	CCodeIterator Iter(Code);
	CCode* left = Iter.First();
	
	if( left->CodeType == CT_Lexema )
	{
		Msg("   subtype of %s = %s", left->GetText(), CTypes1C::GetSubType(LeftPartType, left->GetText()));
		return CTypes1C::GetSubType(LeftPartType, left->GetText());
	}

	return "";
}

//===========================================================================
CString CExpression::GetType1C(CExecutionContext* Context)
{
	CString Type1C = "";

	if( Code->CodeType == CT_MethCall )
	{
		CMethCall MethCall(Code);
		return MethCall.GetType1C(Context);
	}
	else if( Code->CodeType == CT_LValue )
	{
		CLValue LVal(Code);
		return LVal.GetType1C(Context);
	}
	else if( Code->CodeType == CT_Expr )
	{
		CCodeIterator Iter(Code);
		if( Iter.Count() == 1 ) //если > 1, то это выражения типа "А + Б" - про них мы сказать ничего не можем
		{
			CExpression SubExpr(Iter.First());
			return SubExpr.GetType1C(Context);
		}
	}

	return Type1C;
}

CString CExpression::GetSubType(CString& LeftPartType)
{
	if( Code->CodeType == CT_Lexema )
	{
		CTypes1C Types;
		CType1C* Type = Types.GetType(LeftPartType);
		if( Type == NULL )
			return "";
		CString PropType = Type->GetPropType(Code->GetText());
		return PropType;
	}
	else if( Code->CodeType == CT_LValue )
	{
		CLValue LVal(Code);
		return LVal.GetSubType(LeftPartType);
	}
	else if( Code->CodeType == CT_MethCall )
	{
		CMethCall mc(Code);
		return mc.GetSubType(LeftPartType);
	}

	return "";
}

//===========================================================================
bool COperator::IsAssignment()
{
	CCodeIterator Iter(Code);
	CCode* LValue = Iter.First();
	if( LValue == NULL || LValue->CodeType != CT_LValue )
		return false;

	CCode* Operator = Iter.Next();
	if( Operator == NULL || Operator->CodeType != CT_Lexema || Operator->GetText() != "=" )
		return false;

	return true;
}

bool COperator::IsReturn(CExecutionContext* Context, CString& ReturnedType)
{
	CCodeIterator Iter(Code);
	CCode* left = Iter.First();
	if( left->CodeType != CT_Lexema )
		return false;
	if( ((CLexema*)left)->KeyWord != return_ )
		return false;

	CCode* right = Iter.Next();
	if( right )
	{
		CExpression Expr(right);
		ReturnedType = Expr.GetType1C(Context);
	}

	return true;
}

void COperator::AnalyseAssignment(CExecutionContext* Context)
{
	CCodeIterator Iter(Code);
	CCode* LValue = Iter.First();
	CCode* Operator = Iter.Next(); // '='
	CExpression Expr(Iter.Next());

	CString RightPartType = Expr.GetType1C(Context);
	if( LValue->GetPartsCount() == 1 ) //слева - переменная
	{
		Context->AddTypedVar(LValue->GetText(), RightPartType);
	}
	else if( LValue->GetPartsCount() == 3 ) //слева - сложное выражение, возможно свойство переменной
	{
		Iter.Reset(LValue);
		CCode* left = Iter.First();
		Iter.Next(); // '.'
		CCode* right = Iter.Next();

		if( left->GetPartsCount() == 1 && right->CodeType == CT_Lexema )
		{
			CString VarName = left->GetText();
			CVariable* pVar = Context->GetVar(VarName);
			
			if( !pVar )
			{
				pVar = Context->AddAutomaticVar(VarName, "");
				pVar = Context->GetVar(VarName);
			}

			if( pVar )
			{
				pVar->AddProperty(right->GetText(), RightPartType);
			}
		}
	}
}

void COperator::PrintAssignment()
{
	CCodeIterator Iter(Code);
	CCode* LValue = Iter.First();
	CCode* Operator = Iter.Next();
	CExpression Expr(Iter.Next());

	char* err = "";
	if( Expr.HasErrors() ) err = "!ERROR!";
	else err = (char*)Expr.CodeTypeAsStr();


	Msg("(%s) = (%s) %s (%i,%i - %i,%i)", LValue->GetText(), Expr.GetText(), err,
		Expr.StartLine(), Expr.StartCol(), Expr.EndLine(), Expr.EndCol());
	fflush(stdout);
}

