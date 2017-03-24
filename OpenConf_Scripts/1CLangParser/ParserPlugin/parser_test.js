// Версия: $Revision: 1.19 $

var Parser;
var SA = new ActiveXObject("OpenConf.SyntaxAnalysis");
var DataDir = BinDir + "Config\\Intell";
var CPP_DataDir = DataDir + "\\1С++";
var DataDir2 = BinDir + "Config\\Intell2";
var CPP_DataDir2 = DataDir2 + "\\1С++";

var WSH = new ActiveXObject("WScript.Shell");
var FSO = new ActiveXObject("Scripting.FileSystemObject");
var CrLf = '\r\n';
var SelObj = new ActiveXObject("Svcsvc.Service");

var GlobalVariables;

var CurrentTextDoc;

var picProperty = '', picMethod = '', picObjField = '', picObjTableField = '', picFormAttr = '';


//Constructor for type TExprInfo
function TExprInfo(_)
{
	this.Parts = new Array();
	this.PropFilter = '';
	this.Type = '';
	this.PropList = new ActiveXObject("Scripting.Dictionary");

	this.TestProp = function (PropName)
	{
		var len = this.PropFilter.length;
		if( len == 0 ) return true;
		return PropName.substr(0, len) == this.PropFilter;
	}
}



//----------------------------------------------------------------------
function СписокСвойств()
{
	d1 = new Date();

	CurrentTextDoc = CommonScripts.GetTextDoc(true, 2);
	if( CurrentTextDoc == null )
		return false;

	ExprInfo = GetTypeInPos();
	if( ExprInfo != null )
	{
		//Message(ExprInfo.Type);
		arrTypeProps = GetTypeProperties(ExprInfo);

		//d2 = new Date();
		//Message('Определение типа: ' + (d2.getTime() - d1.getTime()), 0);

		if( arrTypeProps.length == 0 ) return false;
		Prop = SelectProperyFromList(ExprInfo, arrTypeProps);
		if( Prop.length == 0 ) return true;

		InsertTextInPos(ExprInfo, Prop);
		return true;
	}

	return false;
}

function GM()
{
	//docGM = Documents("Глобальный модуль");
	Parser.AnalyseGlobalModule();
}

//----------------------------------------------------------------------
function TelepatOnShowMemberList(Line, Col)
{
	if( Parser != null )
	{
		if( СписокСвойств() ) return;
	}

	Intellisence = Scripts("Intellisence");
	Ret = Intellisence.MethodsList();
	/*
	Select Case Ret
		Case -1: ' нажата ESC, пользователь передумал, значит и Dots вызывать смысла нет
			exit Sub
		Case  1: ' Все Ок, выбор из списка автодополнения произведен
			ShowTooltip()
			exit Sub
		Case Else: ' Интел не смог составить список для автодополнения, придется работать Dots'у
	End Select
	*/
}

//======================================================================
//=====  Parsing methods  ==============================================
//======================================================================

function GetTypeInPos(_)
{
	AnalyseDialogControls();

	Parser.GetTypeInPos();
	//Parser.AnalyseModule(CurrentTextDoc.Text, CurrentTextDoc.SelEndLine, CurrentTextDoc.SelEndCol);
	//Message("Parsing - " + Parser.TimeSpent + "ms");
	if( Parser.Type != '' )
	{
		//Message("Type = '" + Parser.Type + "', time spent = " + Parser.TimeSpent + "ms");
		//return Parser.Type;
	}

	var CurType;
	var ExprInfo = SplitExpressionUnderCursor();
	if( ExprInfo == null )
		return null;


	var nParts = ExprInfo.Parts.length;
	if( nParts <= 0 )
		return null;
	var VarName = ExprInfo.Parts[0];
	var VarName_UC = VarName.toUpperCase();
	var ExplicitlyDeclaredVars = AnalyseDeclaredVars(CurrentTextDoc.Text, false, CurrentTextDoc.SelEndLine);

	if( ExplicitlyDeclaredVars != null && ExplicitlyDeclaredVars.Exists(VarName_UC) )
	{
		CurType = ExplicitlyDeclaredVars.Item(VarName_UC);
	}
	else if( GlobalVariables != null && GlobalVariables.Exists(VarName_UC) )
	{
		CurType = GlobalVariables.Item(VarName_UC);
	}
	else if( Parser.FindVar(VarName) )
	{
		CurType = Parser.Type;
		//Message('type_ = '+CurType);
	}
	else
	{
		return null;
	}
	//Message(VarName+': '+CurType);

	var i;
	for( i = 1; i < nParts; i++ )
	{
		var PropName = ExprInfo.Parts[i];

		if( i == 1 ) //свойство переменной надо сначала поискать в самой переменной, и только потом уже в типе
		{
			if( Parser.FindVarProp(PropName) )
			{
				CurType = Parser.VarPropType;
				//Message('type 1 = '+PropName+': '+CurType);
				continue;
			}
		}

		if( !Parser.FindType(CurType) )
			return null;

		//Message('1'+PropName);
		if( !Parser.FindTypeProp(PropName) )
		{
			return null;
		}
		CurType = Parser.TypePropertyType;
		//Message('type = '+CurType);
	}

	//Message('type = '+CurType);
	var re = /[ \t]*(.*)/;
	var arr;
	if( (arr = re.exec(CurType)) != null )
	{
		CurType = arr[1];
	}
	ExprInfo.Type = CurType;
	return ExprInfo;
}

function SplitExpressionUnderCursor(_)
{
	var line = CurrentTextDoc.SelEndLine;
	var col = CurrentTextDoc.SelEndCol;


	var Expr = SA.AnalyseExpression(CurrentTextDoc, line, 0, line, col);
	var ExprParts = Expr.Parts;

	var nParts = ExprParts.Size();

	if( nParts == 0 )
		return null;

	var ExprInfo = new TExprInfo();

	//Если в левой части есть '=', то анализировать нужно только то, что после '='
	var nFirstPart = 0, i;
	for( i = 0; i < nParts; i++ )
	{
		var part = ExprParts.Item(i);
		var part_text = ExprParts.Item(i).Text.toLowerCase();
		if( part.Type == 'delimiter' && part_text != '.' )
		{
			nFirstPart = i + 1;
		}
		else if( part.Type == 'equal_sign' )
		{
			nFirstPart = i + 1;
		}
		else if( part.Type == 'identifier' )
		{
			if( part_text == 'если' || part_text == 'иначеесли' || part_text == 'пока' || part_text == 'для' )
			{
				nFirstPart = i + 1;
			}
		}
	}

	for( i = nFirstPart; i < nParts; i++ )
	{
		var part = ExprParts.Item(i);
		if( part.Type == 'identifier' || part.Type == 'method' )
		{
			ExprInfo.Parts.push(part.Text);
			//Message('push part: '+part.Text);
		}
	}

	if( CurrentTextDoc.Range(line, col-1, line, col) != '.' )
	{
		ExprInfo.PropFilter = ExprInfo.Parts.pop();
	}

	return ExprInfo;
}

function StandartBaseType(Type)
{
	if( Type.substr(0, 11).toUpperCase() == 'СПРАВОЧНИК.' )
		return Type.substr(0, 10);

	if( Type.substr(0, 9).toUpperCase() == 'ДОКУМЕНТ.' )
		return Type.substr(0, 8);

	if( Type.substr(0, 13).toUpperCase() == 'ПЕРЕЧИСЛЕНИЕ.' )
		return Type.substr(0, 12);

	if( Type.substr(0, 8).toUpperCase() == 'РЕГИСТР.' )
		return Type.substr(0, 7);

	return "";
}

function AddMetadataToPropList(arrProps, TypeName, PropsName, picture)
{
	var arrTypeNameParts = TypeName.split('.');
	var Type = MetaData.TaskDef.Childs(arrTypeNameParts[0]);
	
	try {
		var Kind = Type(arrTypeNameParts[1]);
		var TypeProps = Kind.Childs(PropsName);

		for( j = 0; j < TypeProps.Count; j++ )
		{
			Prop = TypeProps(j);
			arrProps.push(picture + Prop.Name);
		}
	} catch(err) {}
}

function GetTypeProperties(ExprInfo)
{
	//Message('"'+ExprInfo.Type+'"');
	var arrProps = ReadMethodsFromFile(ExprInfo.Type, null, true);
	//Message('' + arrProps.length);

	var BaseType = StandartBaseType(ExprInfo.Type);
	if( BaseType.length > 0 )
	{
		var arrBaseProps = ReadMethodsFromFile(BaseType, arrProps, true);
		arrProps = arrBaseProps.concat(arrProps);
		AddMetadataToPropList(arrProps, ExprInfo.Type, "Реквизит", picObjField);
		AddMetadataToPropList(arrProps, ExprInfo.Type, "РеквизитШапки", picObjField);
		AddMetadataToPropList(arrProps, ExprInfo.Type, "РеквизитТабличнойЧасти", picObjTableField);
		AddMetadataToPropList(arrProps, ExprInfo.Type, "Измерение", picObjField);
		AddMetadataToPropList(arrProps, ExprInfo.Type, "Ресурс", picObjTableField);
	}
	else if( Parser.FindType(ExprInfo.Type) )
	{
		Parser.StartPropsIteration();
		while( (PropName = Parser.NextProperty()) != "" )
		{
			//Message(PropName);
			if( FindInArray(arrProps, PropName) != -1 ) continue;
			arrProps.push(PropName);
		}
	}


	if( ExprInfo.Parts.length == 1 )
	{
		//Для переменной добавим в список её динамические свойства
		if( Parser.FindVar(ExprInfo.Parts[0]) )
		{
			Parser.StartVarPropsIteration();
			while( (PropName = Parser.NextVarProp()) != "" )
			{
				if( FindInArray(arrProps, PropName) == -1 ) arrProps.push(PropName);
			}
		}
	}

	return arrProps;
}

function SelectProperyFromList(ExprInfo, arrTypeProps)
{
	var strTypeProps = arrTypeProps.join('\r\n');

	var selection = SelObj.FilterValue(strTypeProps, 1 | 4 | 16 | 64 | 512, ExprInfo.PropFilter);
	if( selection == ExprInfo.PropFilter )
		selection = "";
	return selection;
}

function InsertTextInPos(ExprInfo, Prop)
{
	var re_prop = /^([^\(\)]+)(\(.*\))*.*$/;
	var IsMethod = false;
	var arr_parts = re_prop.exec(Prop);
	if( arr_parts != null )
	{
		var Prop = arr_parts[1];
		if( arr_parts[2].length > 0 )
		{
			IsMethod = true;
		}
	}
	var CursorOffset = Prop.length;

	var line = CurrentTextDoc.SelEndLine;
	var LineLen = CurrentTextDoc.LineLen(line);
	var strLine = CurrentTextDoc.Range(line, 0, line, LineLen);
	LineLen = strLine.length;

	var col1 = CurrentTextDoc.SelEndCol - 1;
	while( col1 >= 0 && strLine.substr(col1, 1) != '.' )
	{
		//Message(''+col1+': '+strLine.substr(col1, 1));
		col1--;
	}
	col1++;
	//*
	var col2 = col1;
	var re_letter = /[a-zа-я0123456789_]/i;
	while( col2 < LineLen && re_letter.test(strLine.substr(col2, 1)) ) col2++;
	//Message(''+col2+', '+LineLen+' "'+strLine+'"'+' "'+strLine.substr(col2, 1)+'"');
	//*/
	//var col2 = CurrentTextDoc.SelEndCol;
	if( IsMethod )
	{
		if( col2 == LineLen )
		{
			CursorOffset = Prop.length + 1;
			Prop += '();';
		}
		else if( strLine.substr(col2, 1) != '(' )
		{
			CursorOffset = Prop.length + 1;
			Prop += '()';
		}
	}
	

	CurrentTextDoc.Range(line, col1, line, col2) = Prop;
	CurrentTextDoc.MoveCaret(line, col1 + CursorOffset);
}

//====================================================================
//==== чтение явно объявленных переменных ============================
//====================================================================
function ReadGlobalVariables()
{
	GlobalVariables = new ActiveXObject("Scripting.Dictionary");
	var docGM = Documents("Глобальный модуль");
	GlobalVariables = AnalyseDeclaredVars(docGM.Text, true);
}

function AnalyseDeclaredVars(ModuleText, bOnlyGlobalVars, stop_line)
{
	var Variables = new ActiveXObject("Scripting.Dictionary");
	if( Variables == null )
	{
		Message('Не могу создать объект типа "Scripting.Dictionary"!', mRedErr);
		//return Variables;
	}

	var arrLines = ModuleText.split(/[\r\n]+/);

	var re_vardecl = /\s*(перем|var)\s+([_a-zа-яё][_0-9a-zа-яё]*)(\s+(export|экспорт))?\s*;\s*\/\/\s*:\s*([_a-zа-яё][_0-9a-zа-яё.]*)/i;
	var re_vardecl_comment = /^\s*\/\/\s*(перем|var)?\s*([_a-zа-яё][_0-9a-zа-яё]*)(\s+(export|экспорт))?\s*;?\s*\/\/\s*:\s*([_a-zа-яё][_0-9a-zа-яё.]*)/i;
	var re_meth = /\s*(процедура|функция|procedure|function)\s+/i;

	if( stop_line == null ) stop_line = arrLines.length - 1;

	var i;
	for( i = 0; i <= stop_line; i++ )
	{
		var line = arrLines[i];

		if( bOnlyGlobalVars && re_meth.test(line) ) break;

		var arrMatches = re_vardecl.exec(line);
		if( arrMatches == null )
		{
			arrMatches = re_vardecl_comment.exec(line);
		}
		
		if( arrMatches != null )
		{
			var VarName = arrMatches[2].toUpperCase();
			var VarType = arrMatches[5];
			if( Variables.Exists(VarName) )
			{
				Variables.Remove(VarName);
			}
			Variables.Add(VarName, VarType);

			// UCase(Matches(0).SubMatches(1)),  LCase(Matches(0).SubMatches(4))
		}
	}

	return Variables;
}



//====================================================================
//==== методы для работы с ints файлами ==============================
//====================================================================
function Get1CppIntsFolder(_)
{
	return DataDir + '\\1С++';
}

function FindInArray(arr, val)
{
	var i, str, pos;
	for( i = 0; i < arr.length; i++ )
	{
		str = arr[i];
		pos = str.indexOf('#');
		if( pos >= 0 ) str = str.substr(pos+1);
		pos = str.indexOf('(');
		if( pos >= 0 ) str = str.substr(0, pos);
		
		if( str == val )
			return i;
	}

	return -1;
}


//== Загрузка текущей формы в парсер =================
function AnalyseDialogControls(_)
{
	var DocFrm = Windows.ActiveWnd.Document;
	var Dialog;

	try
	{
		Dialog = DocFrm.Page(0);
	}
	catch(err)
	{
		return false;
	}
	
	/*
	if( DocFrm == docWorkBook )
	{
		Dialog = DocFrm.Page(0);
	}
	else if( DocFrm == docText )
	{
		//попытаемcя получить форму для модуля, загруженного с помощью ЗагрузитьИзФайла
		// Phoenix === begin ===
		// добавляем анализ на нахождение в ГМ, если это ГМ вернуть false
		// при попытке вызова функции: GetDocFromPath с путем от ГМ выдает ошибку.
		if (DocFrm.Path.indexOf("ModuleText_Number1") != -1)
		{
			return false;
		}
		else
		{
			Dialog = GetDocFromPath(DocFrm.Path)
			if( DocFrm == docWorkBook )
				Dialog = DocFrm.Page(0);
			else
				return false;
		}
		// Phoenix === end ===
	}
	*/
	
	Parser.LC_AddVar('Форма', 'Форма');
	Parser.LC_AddVar('Контекст', 'ГрупповойКонтекст');

	var arrLines = Dialog.Stream.split(CrLf);

	var State = '';
	var nLines = arrLines.length;
	var i, ControlName, TypeLetter, TypeID;
	for( i = 0; i < nLines; i++ )
	{
		var line = arrLines[i];

		//определяем, в какой части формы мы находимся
		if( line == '{"Fixed",' )
		{
			State = 'Table';
			continue;
		}
		if( line == '{"Controls",' )
		{
			State = 'Controls';
			continue;
		}

		if( State == '' ) continue;

		//Message(line, 0);

		//--- начинаем разбор контрола ---
		var arrControl = line.split('","');

		if( State == 'Table' )
		{
			ControlName = arrControl[7];
			TypeLetter = arrControl[9];
			TypeID = Number(arrControl[12]);
		}
		else if( State == 'Controls' )
		{
			ControlName = arrControl[12];
			TypeLetter = arrControl[14];
			TypeID = Number(arrControl[17]);
		}
		else
		{
			ControlName = null;
		}

		if( ControlName == null ) continue; //Не распарсилось чего-то...
		if( ControlName == '0' ) continue;  //фиг знает, чего это, но явно что-то не то

		if( ControlName.length > 0 )
		{
			var WndClass = arrControl[1];

			if( WndClass == 'TABLE' )
			{
				TypeID = 'ТаблицаЗначений';
			}
			else if( WndClass == 'COMBOBOX' || WndClass == 'LISTBOX' )
			{
				TypeID = 'СписокЗначений';
			}
			else
			{
				if( TypeID > 0 )
				{
					MD = MetaData.FindObject(TypeID);
					if( MD != null )
						TypeID = MD.FullName;
					else
						TypeID = '';
				}
				else
				{
					TypeID = '';
				}
			}

			if( TypeID == '' )
			{
				if( TypeLetter == 'B' )
				{
					TypeID = 'Справочник';
				}
				else if( TypeLetter == 'O' )
				{
					TypeID = 'Документ';
				}
			}

			//Добавим контрол в локальный контекст
			Parser.LC_AddVarProperty('Форма', ControlName, 'ФормаАтрибут');
			if( TypeID.length > 0 )
				Parser.LC_AddVar(ControlName, TypeID);

			//Message('Control: '+ControlName+', type '+TypeLetter+':'+TypeID, 0);
		}


		//проверим завершение блока контролов
		if( State == 'Table' && line.substr(line.length - 5) == '"}}},' )
		{
			State = '';
		}
		if( State == 'Controls' && line.substr(line.length - 4) == '"}},' )
		{
			State = '';
		}
	}
}

//----------------------------------------------------------------------
// сначала ищем в папке Intell, а уже затем в папке Intell\1С++
function GetIntsFilePath(TypeName)
{
	var FileName;

	FileName = DataDir2 + '\\' + TypeName + '.ints';
	if( !FSO.FileExists(FileName) )
		FileName = DataDir2 + '\\1C++\\' + TypeName + '.ints';
		
	if( !FSO.FileExists(FileName) )
		FileName = DataDir + '\\' + TypeName + '.ints';
		
	if( !FSO.FileExists(FileName) )
		FileName = Get1CppIntsFolder() + '\\' + TypeName + '.ints';
		
	if( !FSO.FileExists(FileName) )
		return '';

	return FileName;
}

//----------------------------------------------------------------------
//returns Array of strings
function ReadMethodsFromFile(Type, arrMethods, bAddPictures)
{
	var arrRes = new Array;

	var FileName = GetIntsFilePath(Type);
	if( FileName.length == 0 )
		return arrRes;

	var FileStream = FSO.OpenTextFile(FileName, 1, false);
	var strMethods = FileStream.ReadAll();
	FileStream.Close();

	var re_line = /^(\d+)\s+([^\t\r\n]*)(\t+[^\r\n]*)?[\r\n]*$/mig;
	var arrLine;
	while( (arrLine = re_line.exec(strMethods)) != null )
	{
		if( arrMethods != null )
		{
			if( FindInArray(arrMethods, arrLine[2]) != -1 ) continue;
		}
		AttributeName = arrLine[2];
		if( bAddPictures == true )
		{
			if( AttributeName.indexOf('(') >= 0 )
				AttributeName = picMethod + AttributeName;
			else
				AttributeName = picProperty + AttributeName;
		}
		arrRes.push(AttributeName);
	}

	return arrRes;
}

function Update1CppInts()
{
	var CPPAnalyser = new ActiveXObject("OpenConf._1CPP");

	// интс-файлы будем генерить в специальной папке, чтобы не захламлять общую папку
	if( !FSO.FolderExists(CPP_DataDir2) )
	{
		FSO.CreateFolder(CPP_DataDir2);
	}

	CPPAnalyser.SetConfig(Configurator);
	CPPAnalyser.UpdateClassesInfo();
	var i;
	for( i = 0; i < CPPAnalyser.ClassCount; i++ )
	{
		var Info = CPPAnalyser.ClassInfo(i);
		var FileName = CPP_DataDir2 + '\\' + Info.Name + '.ints';
		Status(FileName);

		var IntsFile = FSO.CreateTextFile(FileName);
		AddCppMethods(Info, IntsFile);
		IntsFile.Close();
	}
}

function AddCppMethods(ClassInfo, IntsFile)
{
	var BaseClasses = '', comma = '';
	var i;
	for( i = 0; i < ClassInfo.BaseClasses.Size(); i++ )
	{
		BaseClasses += comma;
		BaseClasses += ClassInfo.BaseClasses.Item(i);
		comma = ',';
	}
	BaseClasses += comma + '1С++';
	IntsFile.WriteLine('БазовыеКлассы:\t' + BaseClasses);

	if( ClassInfo.Methods.Size() == 0 ) ClassInfo.FillMethodList();
	//add exported vars
	for( i = 0; i < ClassInfo.Vars.Size(); i++ )
	{
		var VarName = ClassInfo.Vars.Item(i);
		IntsFile.WriteLine('0000 ' + VarName);
	}
	//add exported methods
	for( i = 0; i < ClassInfo.Methods.Size(); i++ )
	{
		var method = ClassInfo.Methods.Item(i);
		func = '';
		if( method.substr(0, 1) == 'f' ) func = 'f';
		method = method.substr(2);
		IntsFile.WriteLine('0000 ' + method + '(' + func + ')');
	}
}


//====================================================================
//=====  Setting methods  ============================================
//====================================================================
function _LoadTypeFromFile(Type, FileName)
{
	var FileStream = FSO.OpenTextFile(FileName, 1, false);
	if( FileStream.AtEndOfStream )
	{
		FileStream.Close();
		return;
	}

	var strMethods = FileStream.ReadAll();
	FileStream.Close();

	var BaseTypes = ''; //TODO: в ini-файле нужно как-то указывать имя базового типа

	//Message(Type+': '+FileName);
	//var re_line = /^(БазовыеКлассы:\s*([^\t\s\r\n]*))|((\d+)\s+([^\t\r\n]*)\t+([^\t\s\r\n]*))[\r\n]*$/mig;
	//var re_line = /^(\d+)\s+([^\t\r\n]*)\t+([^\t\s\r\n]*)[\r\n]*$/mig;
	var re_line = /^(?:(БазовыеКлассы:)[\s\t]*([^\r\n]*))|(?:(\d+)\s+([^\t\r\n]*)(\t+[^\r\n]*)?)[\r\n]*$/mig;
	var re_base_delim = /\s*,\s*/;
	var re_prop = /^([^\(\)]+)(\(.*\))*$/;
	var arrLine, ToAdd = true;
	while( (arrLine = re_line.exec(strMethods)) != null )
	{
		//Message(' -- '+arrLine.length+', '+arrLine[1]);
		if( arrLine[1] == 'БазовыеКлассы:' )
		{
			BaseTypes = arrLine[2];
			continue;
		}

		if( ToAdd )
		{
			Parser.AddType(Type);
			arrBaseTypes = arrLine[2].split(re_base_delim);
			var i;
			for( i = 0; i < arrBaseTypes.length; i++ )
				Parser.AddBaseType(arrBaseTypes[i]);
			ToAdd = false;
		}

		var PropName = arrLine[4];
		var PropType = arrLine[5];
		var arrProp = re_prop.exec(PropName);
		if( arrProp != null )
		{
			PropName = arrProp[1];
		}
		//Message("'"+Type+"': '"+PropName+"', '"+PropType+"'");
		Parser.AddTypeProperty(PropName, PropType);
	}
}

function LoadTypeFromFile(Type)
{
	FileName = GetIntsFilePath(Type);
	if( FileName.length == 0 )
		return "";

	_LoadTypeFromFile(Type, FileName);
}

function LoadAllTypesFromFiles(IntsDir)
{
	if( !FSO.FolderExists(IntsDir) )
		return;

	Folder = FSO.GetFolder(IntsDir);
	for( fc = new Enumerator(Folder.files); !fc.atEnd(); fc.moveNext() )
	{
		var FileName = fc.item();
		var ext = FSO.GetExtensionName(FileName);
		if( ext.toUpperCase() != 'INTS' ) continue;
		var Type = FSO.GetBaseName(FileName);

		if( Parser.FindType(Type) ) //already loaded - skip it
			continue;

		_LoadTypeFromFile(Type, FileName);
	}
}

function AddMetadataToParserTypes(TypeName, PropsName, HaveType)
{
	var Types = MetaData.TaskDef.Childs(TypeName)

	for( i = 0; i < Types.Count; i++ )
	{
		Type = Types(i);

		if( TypeName == 'Перечисление' )
		{
			Parser.AddType(TypeName);
			Parser.AddTypeProperty(Type.Name, 'Перечисление.'+Type.Name);
		}

		var FullTypeName = TypeName + "." + Type.Name;
		Parser.AddType(FullTypeName);
		Parser.AddBaseType(TypeName);


		TypeProps = Type.Childs(PropsName);

		for( j = 0; j < TypeProps.Count; j++ )
		{
			Prop = TypeProps(j);
			strPropType = "";
			if( HaveType )
			{
				strPropType = Prop.Props("Тип");
				strPropKind = Prop.Props("Вид");
				if( strPropKind.length > 0 )
					strPropType = strPropType + "." + strPropKind
				//Message( "    " + FullTypeName+': '+Prop.Name + ": " + strPropType);
			}
			Parser.AddTypeProperty(Prop.Name, strPropType);
		}
	}
}

function LoadAllTypes()
{
	Parser.RemoveAllTypes();
	d1 = new Date();

	//базовые типы для метаданных
	LoadTypeFromFile("Справочник");
	LoadTypeFromFile("Документ");
	LoadTypeFromFile("Перечисление");
	//метаданные
	AddMetadataToParserTypes("Справочник", "Реквизит", true);
	AddMetadataToParserTypes("Документ", "РеквизитШапки", true);
	AddMetadataToParserTypes("Документ", "РеквизитТабличнойЧасти", true);
	AddMetadataToParserTypes("Регистр", "Измерение", true);
	AddMetadataToParserTypes("Перечисление", 0, false);

	d2 = new Date();
	//Message('Загрузка метаданных: ' + (d2.getTime() - d1.getTime()), 0);

	LoadAllTypesFromFiles(DataDir2);
	LoadAllTypesFromFiles(CPP_DataDir2);
	LoadAllTypesFromFiles(DataDir);
	//LoadAllTypesFromFiles(CPP_DataDir);

	d2 = new Date();
	Message('Загрузка типов: ' + (d2.getTime() - d1.getTime()), 0);

	Parser.AddType('ТаблицаЗначений');
	Parser.AddExemplarModifyingMethod("НоваяКолонка", 1, 2);
	Parser.AddExemplarModifyingMethod("ВставитьКолонку", 1, 3);
	Parser.AddType('ИндексированнаяТаблица');
	Parser.AddExemplarModifyingMethod("НоваяКолонка", 1, 0);
	Parser.AddType("Структура");
	Parser.AddExemplarModifyingMethod("Вставить", 1, 0);
	Parser.AddType("DynaValue");
	Parser.AddExemplarModifyingMethod("ДобавитьСвойство", 1, 0);

	Parser.AddType("КолонкиТабличногоПоля");
	Parser.AddExemplarModifyingMethod("Добавить", 1, 0);
	Parser.AddExemplarModifyingMethod("Вставить", 1, 0);

	Parser.AddType("ПоляДанныхODBC");
	Parser.AddExemplarModifyingMethod("Добавить", 1, 0);
}

function Configurator_MetaDataSaved()
{
	Message('Save');
	Parser.RemoveAllTypes();
	LoadAllTypes();
	Parser.AnalyseGlobalModule();
}


/* так обходится особенность работы JScript'а с
	подключением обработчиков событий объектов, добавляемых
	в пространство имен скрипта run-time -- a13x */
function addTelepatEventHandler(handler)
{
	eval('function Telepat::' + handler + '{ return Telepat' + handler + '}');
}

function Init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{
	try {
		Parser = Plugins("ParserPlugin.CodeAnalyser");
	} catch(err)
	{
		return;
	}
	
	SA.SetConfig(Configurator);

	var Telepat, OCS = new ActiveXObject("OpenConf.CommonServices");
	OCS.SetConfig(Configurator);
	SelfScript.AddNamedItem("CommonScripts", OCS, false);
	OCS.AddPluginToScript(SelfScript, "Телепат", "Telepat", Telepat);

	addTelepatEventHandler("OnShowMemberList(Line, Col)");

	LoadAllTypes();

	Parser.AnalyseGlobalModule();
	ReadGlobalVariables();
	
		
	if( SelObj.Version >= 1008 )
	{
		picProperty			= '3#';
		picMethod			= '7#';
		picObjField			= '4#';
		picObjTableField	= '5#';
		picFormAttr			= '6#';
	}
}

Init();
