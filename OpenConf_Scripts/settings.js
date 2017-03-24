$NAME УправлениеНастройками

var regEntries = new Array();

regEntries[0] = "HKEY_CURRENT_USER\\Software\\1C\\1Cv7\\7.7\\OpenConf";
//regEntries[1] = "HKEY_CURRENT_USER\\Software\\1C\\1Cv7\\7.7\\Пустая\\Config\\ToolbarSystem";

function selectFile()
{
    with (new ActiveXObject("Svcsvc.Service"))
        return SelectFile(true, "", "Файлы реестра (*.reg)|*.reg", false);
}

function ask(msg)
{
    with (new ActiveXObject("WScript.Shell"))
        return (Popup(msg, 0, 'Settings.js', 4 | 32) == 6); // Yes == 6       
}

function fileExists(file)
{
    with (new ActiveXObject("Scripting.FileSystemObject"))
        return FileExists(file);
}

function exportEntry(regPath)
{
    var tmpfile, cmdLine, rawData; 
    var wsh = new ActiveXObject("WScript.Shell"); 
    var fso = new ActiveXObject("Scripting.FileSystemObject");
   
    tmpfile = wsh.ExpandEnvironmentStrings(
        fso.BuildPath("%TEMP%", fso.GetTempName()));   
    
    cmdLine = '%SystemRoot%\\regedit.exe /ea "' + 
        tmpfile + '" "' + regPath + '"';
   
    with (wsh)
        Run(ExpandEnvironmentStrings(cmdLine), 7, true);
    
    if (!fso.FileExists(tmpfile))
        throw("Произошла ошибка экспорта!");
    
    rawData = fso.OpenTextFile(tmpfile).ReadAll();
    
    //fso.DeleteFile(tmpfile);

    return rawData.replace(/^REGEDIT4\r\n/, '');
}

function saveSettings(file)
{
    var msg, f, rawData = '';
    
   // try {
        for (var i in regEntries)
            rawData += exportEntry(regEntries[i], true);
      
        with (new ActiveXObject("Scripting.FileSystemObject")) {
            f = CreateTextFile(file, true);
            f.WriteLine('REGEDIT4'); 
            f.WriteLine(rawData);
            f.Close();
        }
        msg = 'Настройки успешно экспортированы!';
   // }
   // catch (e) {
        msg = "Во время экспорта настроек произошла ошибка:\n" ; //+ e.description;
    //}
    
   // with  (new ActiveXObject("WScript.Shell"))
     //   Popup(msg, 0, 'Settings.js');
}

function ЗагрузитьНастройки()
{
    //if (!ask(''))
     //   return;
}

function СохранитьНастройки()
{
    var file;

    if (!(file = selectFile()))
        return;
    
    if (fileExists(file)) 
        if (!ask('Файл "' + file + '" существует! Перезаписать?'))
            return;   
  
    saveSettings(file);   
}


/* возвращает объект TextDoc, если текущим открытым документом является
    текстовый документ, в противном случае - null */
function getTextDoc(_)
{
    var win, doc;

    if (!(win = Windows.ActiveWnd))
        return null;

    if ((doc = win.Document) == docWorkBook)
        doc = doc.Page(1);

    return (doc == docText)?doc:null;
}

function CopyString()
{
  var
    d = getTextDoc()
  ;
  
  if (d == null) return;
  
  if (d.SelStartLine == d.SelEndLine) {
    d.Range(d.SelStartLine) += '\r\n' + d.Range(d.SelStartLine);
  }
  else {
    d.Range(d.SelStartLine, 0, d.SelEndLine + 1, 0) +=
      d.Range(d.SelStartLine, 0, d.SelEndLine + 1, 0);
  }
}