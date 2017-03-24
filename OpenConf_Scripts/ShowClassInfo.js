
var Vis1CPP;
var CommonScripts;

// Возвращает полное имя объекта под курсором. В качестве имени берётся диапазон, включающий указанную позицию, и
// ограниченный символами в Delimiters
function GetObjectName(Line, CursorPos)
{
	var Delimiters = "\t ,;:|#=+-*/%?<>\\()[]{}!~@$^&'\"";
	var Col1, Col2, TextLen;

	ObjectName = "";

	TextLen = Line.length;
	Col1 = CursorPos;
	while( Col1 > 0 )
	{
		if( Delimiters.indexOf(Line.substr(Col1-1, 1)) > 0 )
		{
			Col1++;
			break
		}
		Col1--;
	}
	
	if( Col1 == 0 ) Col1 = 1;
	Col2 = Col1;
	while( Col2 <= TextLen )
	{
		if( Delimiters.indexOf(Line.substr(Col2-1, 1)) > 0 )
		{
			Col2--;
			break;
		}
		Col2++;
	}
	
	if( Col2 > TextLen ) Col2 = TextLen;

	//Message "" & Col1 & ":" & Col2, mNone
	return Line.substr(Col1-1, Col2 - Col1 + 1);
	//Message GetObjectName, mNone
}

function GetWordUnderCursor(_)
{
	CommonScripts.SetConfig(Configurator);

	var doc = CommonScripts.GetTextDocIfOpened(false, true);
	if( doc == null )
		return "";

	Line = doc.SelStartLine;
	Col = doc.SelStartCol;
	return GetObjectName(doc.Range(Line), Col);
}

function ShowBase(Name, Offset, ClassList)
{
	var Classes = Vis1CPP.GetClasses();
	var Class = Classes.Item(Name);
	for( i = 0; i < Class.BaseClasses.Count; i++ )
	{
		BaseClass = Class.BaseClasses.Item(i);
		if( ClassList == null )
		{
			Message(Offset + BaseClass.Name);
		}
		else
		{
			ClassList.List += '\n' + BaseClass.Name;
			//Message('-'+ClassList);
		}
		ShowBase(BaseClass.Name, Offset + "   ", ClassList);
	}
}

function TList(_)
{
	this.List = '';
}

function BaseClasses()
{
	var ClassName = GetWordUnderCursor();
	if( ClassName == '' ) return;
	
	var Classes = Vis1CPP.GetClasses();
	var Class = Classes.Item(ClassName);
	Message(Class.Name);
	ShowBase(Class.Name, "   ");
}

function GoBaseClasses()
{
	var ClassName = GetWordUnderCursor();
	if( ClassName == '' ) return;
	
	var Classes = Vis1CPP.GetClasses();
	var Class = Classes.Item(ClassName);
	var ClassList = new TList();
	ShowBase(Class.Name, "   ", ClassList);
	
	name = CommonScripts.SelectValue(ClassList.List);
	if( name != '' )
	{
		var NavScript = Scripts('Навигация');
		NavScript.TryToOpenClass(name);
	}
}


function IsDerived(BaseName, Class, Path)
{
	var i;
	Path.Add(Path.Count, ""+Class.Name);
	for( i = 0; i < Class.BaseClasses.Count; i++ )
	{
		var BaseClass = Class.BaseClasses.Item(i);
		if( BaseClass.Name == BaseName )
		{
			return true;
		}
		else
		{
			if( IsDerived(BaseName, BaseClass, Path) ) 
			{
				return true;
			}
		}
	}
	return false;
}

function ShowPath(Path)
{
	var i = Path.Count-1;
	strPath = "  ";
	while( i >= 0 )
	{
		var Class = Path.Item(i);
		strPath += Class;
		if( i > 0 ) strPath += " --> ";
		i--;
	}
	Message(strPath);
}

function _DerivedClasses(ClassList)
{
	var BaseName = GetWordUnderCursor();
	if( BaseName == '' ) return;
	
	if( ClassList == null )
		Message(BaseName);

	var Classes = Vis1CPP.GetClasses();
	
	var i;
	for( i = 0; i < Classes.Count; i++ )
	{
		var Class = Classes.Item(i);
		var Path = new ActiveXObject("Scripting.Dictionary");
		if( IsDerived(BaseName, Class, Path) )
		{
			if( ClassList == null )
			{
				ShowPath(Path);
			}
			else
			{
				ClassList.List += '\n' + Path.Item(0);
			}
		}
	}
	
}

function DerivedClasses()
{
	_DerivedClasses();
}

function GoDerivedClasses()
{
	var ClassList = new TList();
	_DerivedClasses(ClassList);
	
	name = CommonScripts.SelectValue(ClassList.List);
	if( name != '' )
	{
		var NavScript = Scripts('Навигация');
		NavScript.TryToOpenClass(name);
	}
}

function Init(_) // Фиктивный параметр, чтобы процедура не попадала в макросы
{
	Vis1CPP = Plugins("Visual1Cpp");
	CommonScripts = new ActiveXObject("OpenConf.CommonServices");
}

Init();
