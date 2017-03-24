// JScript source code
var fso=new ActiveXObject("Scripting.FileSystemObject")
var Title=""
var inFileName=""
var intDir, outDir

var hhcFile
var CreatedFiles=Array()	// Имена всех создаваемых файлов

function ShowText(txt){WScript.Echo(txt)}
function FatalError(txt)
{
	ShowText(txt)

	if(hhcFile)
		hhcFile.Close()

	for(var i in CreatedFiles)
		fso.DeleteFile(i, true)

	WScript.Quit(1)
}
function CreateFile(name)
{
	try{
		var file=fso.CreateTextFile(name)
		if(file)
		{
			CreatedFiles[name]=1
			return file
		}
	}catch(e){}
	return null
}

// Подготовка входного файла
function PrepeareFile(path)
{
	// Проверим существование файла
	try{
		var f=fso.GetFile(path)
	}catch(e)
	{
		FatalError("can't get file "+path+'\n'+e.Description)
	}

	// Запомним его каталог, это будет и каталог промежуточных файлов
	intDir=f.ParentFolder.Path
	if(!intDir.match(/\\$/))
		intDir+='\\'
	
	inFileName=f.Name.replace(/\.[^\.]*$/, "")
	f=null
	
	// Далее надо вытащить title
    var InFile = fso.OpenTextFile(path)
    var txt=InFile.ReadAll()
    var ttt=txt.match(/<title>([^<]*)</i)
    if(ttt)
		Title=ttt[1]
	else
		Title=inFileName
    InFile.Close()
}
function norm(text)
{
	return text.replace(/&/g,"&amp;").replace(/"/g,"&quot;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
}

function MakeHHPFiles()
{
	//debugger
    var fName=intDir+inFileName+".hhc"
    hhcFile=CreateFile(fName)
    if(!hhcFile)
		FatalError("can't create content file: "+fName)
	hhcFile.Write('<HTML><UL><LI><OBJECT type="text/sitemap">\r\n'+
		'<param name="Name" value="'+norm(Title)+'">\r\n'+
		'<param name="Local" value="'+inFileName+'.html">\r\n'+
		'</OBJECT></UL></HTML>')
	hhcFile.Close();
	hhcFile=null
	
    output = CreateFile(intDir+inFileName + ".hhp")
    if(!output)
		FatalError("Don't create hhp file")
    output.WriteLine("[Options]")
    output.WriteLine("Auto Index = Yes")
    output.WriteLine("Binary TOC = No")
    output.WriteLine("Compatibility = 1.1 Or later")
    output.WriteLine("Compiled File = " + outDir + inFileName + ".chm")
    output.WriteLine("Contents File = " + inFileName + ".hhc")
    output.WriteLine("Default Window = Main")
    output.WriteLine("Default topic = " + inFileName+".html")
    output.WriteLine("Display compile progress=No")
    output.WriteLine("Full-text search=Yes")
    output.WriteLine("Language=0x419 Русский")
    output.WriteLine("Title=" + Title)
    output.WriteLine("")
    output.WriteLine("[Windows]")
    output.WriteLine('main="'+Title+'","' + inFileName + '.hhc","","'+
		inFileName + '.html","' + inFileName + '.html",,,,,0x73420,,0x387e,[8,13,731,589],,,,,,,0')
    output.WriteLine("")
    output.WriteLine("[Files]")
	output.WriteLine(inFileName+".html")
    output.Close()
}

function main()
{
 	var argc=WScript.Arguments.Count()
	if(!argc)
		FatalError("usage: cscript.exe simplechm.js InFileName [outFilesDir]")
	// Готовим входной файл
	PrepeareFile(WScript.Arguments.item(0))
	
	outDir=intDir
	// Возможно задан выходной каталог в параметрах
	if(argc>1)
	{
		try{
			var fOutDir=fso.GetFolder(WScript.Arguments.item(1))
		}catch(e)
		{
			FatalError("can't get outdir "+WScript.Arguments.item(1)+'\n'+e.Description)
		}
		outDir=fOutDir.Path
	}
	// Дополним по необходимости слэшем
	if(!outDir.match(/\\$/))
		outDir+='\\'
	
	MakeHHPFiles()

	// И напоследок файл зачистки
	var erasefile=fso.CreateTextFile(intDir+inFileName+"del.bat")
	erasefile.WriteLine("@echo off")
	for(var i in CreatedFiles)
		erasefile.WriteLine('del "'+i+'"')
	erasefile.Close()
}

main()
WScript.Quit(0)
