' Показать список методов редактируемого скрипта (можно *.vb, *.js, *.wsc)
'
'	макрос "ShowMethodsList" - в случае нахождения в окне скрипта (vbs,js,wsc) вызывает 
'		макрос "ScriptMethodsList" для показа методов скрипта, 
'		а иначе просто показывает окно "Методы модуля" от Опенконф.
'	Этот макрос удобно посадить на Ctrl+1 (вместо типового от Опенконф)
'
'Версия: $Revision: 1.6 $ 
'                   
' Авторы:
'	MetaEditor
'	Артур Аюханов aka artbear
'
'Расширения файлов, которые являются скриптами
Const SupportedExtensions = ".vbs.js.wsc"

Sub ShowMethodsList()
	
	set doc = CommonScripts.GetTextDocIfOpened(0)
	if doc is nothing then exit Sub
	
	if CommonScripts.CheckDocOnExtension(doc, SupportedExtensions) Then 
		ScriptMethodsList
	else
		SendCommand(33298) ' ОпенКонфПоказатьСписокМетодовМодуля
	end if
End Sub

re_proc = "^\s*(private\s*)?((?:sub)|(?:function))\s+([\wА-я¬и\d]+)\s*\(([\wА-я¬и\d\s,.=""\']*)\)\s*"

'=======================================================================================
Sub ScriptMethodsList()

	set doc = CommonScripts.GetTextDocIfOpened(0)
	if doc is Nothing then Exit Sub

	Set MethodsDict = CreateObject("Scripting.Dictionary")

	for i=0 to doc.LineCount-1 
		sLine = lTrim(doc.Range(i))
		set Matches = CommonScripts.RegExpExecute(re_proc, sLine)
		if not Matches is Nothing then 
			
			for each Match in Matches   
				MethodsDict.Add Match.SubMatches(2), i
			next
		end if
	next       
	if MethodsDict.Count = 0 then Exit Sub
		
  	num = CommonScripts.SelectValue(MethodsDict)
  	if num = "" then exit sub

	doc.MoveCaret num,0,num,0
End Sub

'
' Процедура инициализации скрипта
'
Private Sub Init() ' Фиктивный параметр, чтобы процедура не попадала в макросы
	
    Set c = Nothing
    On Error Resume Next
    Set c = CreateObject("OpenConf.CommonServices")
    On Error GoTo 0
    If c Is Nothing Then
        Message "Не могу создать объект OpenConf.CommonServices", mRedErr
        Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    c.SetConfig(Configurator)
	SelfScript.AddNamedItem "CommonScripts", c, False
	
End Sub ' Init

Init