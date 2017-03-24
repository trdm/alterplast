var modules = new Array(
"Обработка.defcls"
);
//------------------------------------------------------------------------------

function processModule(moduleName)
{
var m= Module(moduleName);
var re = /^\s*[/]{3,3}\s+/i;
var xmlDoc= new ActiveXObject("Msxml2.DOMDocument");
var xml= "";
var lines= new Array();
var xmlLineNum= 0;

xmlDoc.async = false;
xmlDoc.resolveExternals= false;

for (var i = 0; i < m.LineCount; i++)
{
   if (re.test(m.Line(i)))
   {
   xml += (m.Line(i).replace(re, ""));
   lines[++xmlLineNum] = i + 1;
   }
}

xmlDoc.loadXML(xml);

if (xmlDoc.parseError.errorCode == 0)
{
Message(moduleName + ":    Ok!", 0);
xmlDoc.save(moduleName + "-comments.xml");
}
else
{
Message(
"{" + moduleName + " (" + lines[xmlDoc.parseError.line] + ")}: " +
xmlDoc.parseError.srcText, mExclamation
);

Message(
xmlDoc.parseError.reason, mExclamation
);

//m.Open();
}
}
//------------------------------------------------------------------------------

function ValidateXmlComments()
{
for (var i = 0; i < modules.length; i++)
{
processModule(modules[i]);
}
}