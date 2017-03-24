'--------------------------------------------------------------------------------------
' Файловый диалог Excel (Скрипт для OpenConf'а)
' Скрипт меняет 1С-овский диалог выбора файла на наворочный диалог из Excel,
' в котором присутствует масса удобных примочек (он сохраняет свое положение и размер,
' в отличии от 1С-овского, можно добавлять свои папки в панель быстрого доступа и проч.)
' Вызывается при открытии файлов TXT,MXL,ERT,SPL,PRM и сохранении TXT,MXL и ERT.
' К сожалению вызов в остальных случаях реализовать не удалось. Из-за некоторых
' свойств OpenConfa 1.0.1.8 (Beta) пришлось пренебречь возможностью т. н. мультиселекта
' в диалоге выбора файла, но все равно очень прикольно.
'
' Для работы необходимо, чтобы на компьютере был установлен Excel.
'
' Установка: скопировать в ...\BIN\CONFIG\Scrips
'
' Скрипт разрабатывался с следующих условиях: WinXP Sp2, 1Cv7.70.025, MS Offise 2003
' OpenConf 1.0.1.8 (Beta)
'
' aphanas@list.ru 
'--------------------------------------------------------------------------------------

Dim Excel
Dim WshShell
Dim FSO

Dim a
Dim s
Const HKEY_LOCAL_MACHINE = &H80000002


Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
	FilterIndex = 0
	ModeName = ""
	ExcelFilter = "Все файлы,*.*"
    IF Filter = "Текст (*.txt)|*.txt|Текст - MS DOS Формат (*.txt)|*.txt|Таблицы (*.mxl)|*.mxl|Внешний отчет(обработка) (*.ert)|*.ert|Файлы прописи (*.spl)|*.spl|Файлы сценария запуска (*.prm)|*.prm|1С файлы (*.txt, *.mxl, *.ert, *.spl, *.prm)|*.txt;*.mxl;*.ert;*.spl;*.prm|" Then
		ExcelFilter = ExcelFilter+",1С файлы,*.txt;*.mxl;*.ert;*.spl;*.prm,Внешние отчеты и обработки,*.ert,Таблицы,*.mxl,Текстовые файлы,*.txt,Файлы прописи,*.spl,Файлы сценария запуска,*.prm"
		ModeName = "Open file"
		FilterIndex = 2
	ElseIF Filter = "Текст (*.txt)|*.txt|Текст - MS DOS Формат (*.txt)|*.txt|Все файлы (*.*)|*.*|" Then
		ExcelFilter = ExcelFilter+",Текстовые файлы,*.txt"
		ModeName = "Save TXT"
		FilterIndex = 2
	ElseIF Filter = "Таблицы (*.mxl)|*.mxl|Таблица Excel (*.xls)|*.xls| HTML Документ (*.htm;*.html)|*.htm; *.html|Текстовый файл (*.txt)|*.txt|" Then
		ExcelFilter = ExcelFilter+",Таблицы,*.mxl,Таблицы Excel,*.xls,HTML Документы,*.htm; *.html,Текстовые файлы,*.txt"
		ModeName = "Save MXL"
		FilterIndex = 2
	ElseIF Filter = "Внешний отчет(обработка) (*.ert)|*.ert|" Then
		ExcelFilter = ExcelFilter+",Внешние отчеты и обработки,*.ert"
		ModeName = "Save ERT"
		FilterIndex = 2
	Else
		Exit Sub
	End If
	
	If Saved Then
		FileName = Excel.GetSaveAsFilename(FileName,ExcelFilter,FilterIndex,Caption)
		If FileName = False Then
			Answer = mbaCancel
			Exit Sub
		End If
		
		If FSO.FileExists(FileName) Then
			Name = FSO.GetFileName(FileName)
			intButton = WshShell.Popup("Файл """+Name+""" уже существует, продолжить?",,"ExcelFileDialog (OpenConf script)",36)
			If intButton = 7 Then 'No
				Answer = mbaCancel
				Exit Sub
			End If
		End If
		
		Answer = mbaOk
	Else
		If ModeName = "" Then
			Exit Sub
		End If
		
		LastDir = ""
		ExcelDefaultPath = ""
		On Error Resume Next
		ExcelDefaultPath = WshShell.RegRead("HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath")
		LastDir = WshShell.RegRead("HKCU\Software\1C\1Cv7\7.7\ExcelFileDialog\"+ModeName+"\LastDir")
		On Error GoTo 0
		
		If LastDir <> "" Then
			WshShell.RegWrite "HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath",LastDir,"REG_EXPAND_SZ"
		End If
		
		If ModeName = "Open file" Then
			FileName = Excel.GetOpenFileName(ExcelFilter,FilterIndex,Caption,, False)
			Answer = mbaCancel
			If FileName = False Then
				Exit Sub
			End If
			Documents.Open(FileName)
		End If
		
		SlashInd = 0
		Poz = InStr(FileName,"\")
		do Until Poz = 0
			SlashInd = Poz
			Poz = InStr(Poz+1,FileName,"\")
		Loop
		DirName = Left(FileName,SlashInd)
		
		WshShell.RegWrite "HKCU\Software\1C\1Cv7\7.7\ExcelFileDialog\"+ModeName+"\LastDir",DirName,"REG_EXPAND_SZ"
		WshShell.RegWrite "HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath",ExcelDefaultPath,"REG_EXPAND_SZ"
		
	End If
	
End Sub

Sub Init(Param)
	Set Excel = CreateObject("Excel.Application")
	Excel.Visible = True
	Excel.Visible = False
	
	Set FSO = CreateObject("Scripting.FileSystemObject")
	Set WshShell = CreateObject("WScript.Shell")
End Sub

Init 0
