$NAME Extract All Reporst Into External Files

'"Сохранение отчетов/обработок во внешние файлы"
' MetaEditor (shotfire@inbox.ru)

Dim FName ' имя файла для автоподстановки в диалог сохранения
Dim flAutoSave ' флаг - признак автоматического сохранения только из этого скрипта
flAutoSave = false

'========================================================================
Sub SaveToERT()
Set srv = CreateObject("Svcsvc.Service")
SaveFolder = srv.SelectFolder("Сохранить в:")
If SaveFolder = "" Then Exit Sub : End If

SaveObjects = srv.SelectValue("Отчеты|c" & vbCrLf & "Обработки|c","Что сохраняем...",True)

If SaveObjects = "" Then Exit Sub : End If

Set GotoTreeScript = scripts("NavigationTools")
flAutoSave = true

If InStr(SaveObjects,"Отчеты") > 0 Then
Set Reps = MetaData.TaskDef.Childs(5) ' отчеты
For i = 0 To Reps.Count - 1
	RepName = "Отчет." & Reps(i).Name
	FName = SaveFolder & "\" & RepName & ".ert"
	GotoTreeScript.GoToMDTreeItem RepName,33239, false
	message FName & " обработан...", mInformation
Next                  
End If

If InStr(SaveObjects,"Обработки") > 0 Then
Set Calcs = MetaData.TaskDef.Childs(6) ' обработки
For i = 0 To Calcs.Count - 1
	CalcName = "Обработка." & Calcs(i).Name
	FName = SaveFolder & "\" & CalcName & ".ert"
	GotoTreeScript.GoToMDTreeItem CalcName,33239, false
	message FName & " обработан...", mInformation
Next                           
End If

message ""
message "ОБРАБОТКА ЗАВЕРШЕНА...", mInformation

flAutoSave = false
End Sub  

'========================================================================
Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer) 
	If flAutoSave = false then Exit Sub
	If (Instr(Caption,"Сохранить как") = 1) and (InStr(UCase(Filter), "*.ERT") > 0) Then 
		FileName = FName
		Answer = mbaOK
	End If
End Sub
