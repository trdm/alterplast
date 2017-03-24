'$NAME OLE-ActiveX *.ints Generator

'	intsOLEGenerator.vbs - генератор *.ints файлов для Intellisence
'	© trdm 2005
'
' Версия: $Revision: 1.4 $
'
'Автор Трошин Дмитрий, ака trdm 2005 год
'	trdm@mail333.com
'	ICQ 308-779-620
'
' Для работы данного скрипта требуется библиотека TLBINF32.DLL
' По последней информации она входит в состав 6-й Визуал студии от microsoft, однако
' Скачать её можно: не нашел, а вот хелп по ней: 2 линка
' http://download.microsoft.com/download/vstudio60pro/doc/1/win98/en-us/tlbinf32.exe
' http://support.microsoft.com/default.aspx?scid=kb;en-us;224331

Dim FileNameProgIDDumped
Dim DataDir
Dim WSH
Dim FSO

DataDir = BinDir & "\Config\Intell"
FileNameProgIDDumped		= DataDir + "\ProgIDDumped.txt"			' Имя файла, содержащего прог-иды по которым данные сформированы


DataDirAls = BinDir ' Сам Bin....

Dim AlsToFile


'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Добавить в конец массива nArray элемент nElement
' А то что-то при редиме убивались напроч элементы........
' Параметры:
' nArray - Массив куда добавляем
' nElement - Элемент что мы добавляем в массив
Private Sub AppendArray ( nArray, nElement )
	On Error Resume Next
	Size = 1 + UBound(nArray)
	NewArr = Array()
	ReDim NewArr( Size )
	if Size>0 Then
		For i = 0 To Size-1
			If IsObject( nElement ) Then
				Set NewArr(i) = nArray(i)
			Else
				NewArr(i) = nArray(i)
			End If
		Next
	End If
	ReDim nArray( Size )
	if Size>0 Then
		For i = 0 To Size-1
			If IsObject( nElement ) Then
				Set nArray(i) = NewArr(i)
			Else
				nArray(i) = NewArr(i)
			End If
		Next
	End If
	Set nArray ( Size ) = nElement

	if Err.number<>0 Then
		Message " Ошибка при добавлении в массив...", mExclamation3
		Message err.Description, mExclamation3
		On Error goto 0
	End If
	On Error goto 0

End Sub

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Ищем путь к библиотеке по ProgID-у
Private Function GetPathFromProgID ( ProgID )
	GetPathFromProgID = Null
	RKey = "HKCR\"+ProgID+"\CLSID\"
	dim PathFile
	Dim val
	PathFile = Null
	On Error Resume Next
	val = WSH.RegRead(RKey)
	if Not IsEmpty(val) Then
		RKey = "HKCR\CLSID\"+val+"\InprocServer32\"
		KeyVal = WSH.RegRead(RKey)
		if IsEmpty(KeyVal) Then
			RKey = "HKCR\CLSID\"+val+"\LocalServer\"
			KeyVal = WSH.RegRead(RKey)
			if IsEmpty(KeyVal) Then
				RKey = "HKCR\CLSID\"+val+"\LocalServer32\"
				KeyVal = WSH.RegRead(RKey)
			End IF
		End IF
		if Not IsEmpty(KeyVal) Then
			PathFile = KeyVal
		End IF
	Else
		Message "Не найден ключ реестра с данным класидом" + RKey + " для объекта: " + ProgID
	End IF
	if IsNull(PathFile) Then
		On Error goto 0
		Exit Function
	End IF
	GetPathFromProgID = PathFile
	On Error goto 0
End Function

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Функция проверяет прог-ид по реестру и на содержание ProgID
' ProgID = должен быть представлен английскими литералами, цыфрами и точкой.
Function IsProgID( ProgID )
	nProgID = Trim(ProgID)
	'Проверим что нам в	ProgID преподнесли....
	IsProgID = True
	For i=1 To Len (nProgID)
		Char = Mid (nProgID,i,1)
		ChKd = Asc(Char)
		if Not ((ChKd = Asc(".")) or ( (Asc("A")<=ChKd) And (Asc("Z")>=ChKd)) or ( (Asc("a")<=ChKd) And (Asc("z")>=ChKd)) or ( (Asc("0")<=ChKd) And (Asc("9")>=ChKd)) ) Then
			IsProgID = False
			'message "недопустимый символ в ProgID: " + Char
			Exit Function
		End IF
	Next
	PathFile = GetPathFromProgID ( ProgID )
	If PathFile = Null Then
		IsProgID = False
		Exit Function
	End If
	IsProgID = True
End Function

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Ищет nProgID в файле (BinDir+"\Config\Intell\ProgIDDumped.txt"), если находит,
' то считается, что прогид уже проанализирован и файлы по нему сгенерированы.
Function ProgIDIsDumped( nProgID )
	ProgIDIsDumped = False
	FileName = FileNameProgIDDumped
	if fso.FileExists( FileName ) Then
		Set File0 = fso.GetFile( FileName )
		Set File = File0.OpenAsTextStream(1)
		if File0.Size > 0 Then
			TextStream = File.ReadAll()
			If InStr(1, lCase(TextStream), lcase(nProgID))>0 Then
				ProgIDIsDumped = True
			End IF
		End IF
		File.Close
	End IF
End Function

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Класс для записей
'
' ИНТЕРФЕЙС:
' ---------------------------
' Init (nName*, nAlias*)	- Присваиваем имя и английский псевдоним...
' Dump ()				- Выливаем данные в файл "AlsToFile" в соответствии с форматом.
' ---------------------------
'  mWriting		- Написание*
'  mWritingEng	- Написание английское
'
'  mSyntax		- Синтаксис*
'  mAssignment	- Назначение
'  mParams		- параметры
'  mNotes		- Примечания
'_______________________________________
'  Примечание: mMamber*	- Обязательный атрибут
Class ALSItem ' Класс для элементов
	Public mName	' Имя элемента
	Public mAlias	' Синоним...

	Public mWriting		' Написание*
	Public mWritingEng	' Написание английское

	Public mSyntax		' Синтаксис....
	Public mAssignment	' Назначение*
	Public mParams		' параметры
	Public mNotes		' Примечания

	Public mLevel	' Уровень вложенности....

	Sub Init (nName, nAlias)
		mName	= nName
		mAlias	= nlias
	End Sub

	Private Sub Class_Initialize
		mName		=	"!!!Name!!!"
		mAlias		= "!!!Alias!!!"
		mWriting	= "!!!Writing!!!"
		mWritingEng = ""
		mSyntax		= ""
		mAssignment	= ""
		mParams		= ""
		mNotes		= ""
	End Sub



	' Типа привести в чуство - убрать кавычки, заменить апострофами...
	Private Function Normalize()
		Normalize = True

		' Убираем кавычки, заменяем на апостофы
		mName		= Replace( mName, """", "''")
		mAlias		= Replace( mAlias, """", "''")
		mWriting	= Replace( mWriting , """", "''")
		mWritingEng	= Replace( mWritingEng , """", "''")
		mSyntax		= Replace( mSyntax , """", "''")
		mAssignment	= Replace( mAssignment , """", "''")
		mParams		= Replace( mParams , """", "''")
		mNotes		= Replace( mNotes , """", "''")

	End Function

	' Выгружаем в файл.....................
	Function Dump()

		Dump	= Normalize
		Indent	= String(mLevel," ")

		' Начало секции
		StrToDump = Indent + "{""Item"",""AST"",""" + mName + """,""" + mAlias + ""","""+ mWriting + ""","""+ mWritingEng + ""","
		AllDump = AllDump + StrToDump + vbCrLf
		AlsToFile.WriteLine StrToDump
		AlsToFile.WriteLine """@Синтаксис:"
		if Len(mSyntax) = 0 Then
			AlsToFile.WriteLine "-------------------"
		Else
			AlsToFile.WriteLine mSyntax
		End IF

		if Len(mAssignment)>0 Then
			AlsToFile.WriteLine "@Назначение:"
			AlsToFile.WriteLine mAssignment
		Else
			'AlsToFile.WriteLine "----------------"
		End IF
		if Len(mParams) Then
			AlsToFile.WriteLine "@Параметры:"
			AlsToFile.WriteLine mParams
		End IF
		if Len(mNotes) Then
			AlsToFile.WriteLine "@Замечание:"
			AlsToFile.WriteLine mNotes
		End IF

		AlsToFile.WriteLine """"
		StrToDump = Indent + "}," ' Есть у Item закрывающая запятая...
		AlsToFile.WriteLine StrToDump
	End Function

End Class 'ALSItem

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Класс для разделов
'
' ИНТЕРФЕЙС:
' ---------------------------
' Init (nName*, nAlias*)	- Присваиваем имя и английский псевдоним...
' Dump ()				- Выливаем данные в файл "AlsToFile" в соответствии с форматом.
' AddSubFolder ( nFolderName, nFolderAlias )	- добавляет "Раздел"
' AddSubItem( nItemName, nItemAlias )			- добавляет "Запись"
'_______________________________________
'  Примечание: mMamber*	- Обязательный атрибут
Class ALSFolder ' Класс для папок
	Public mName		' Имя папки
	Public mAlias	' Синоним...
	Public mLevel	' Уровень вложенности....

	Public mArrSubFolder	'Массив подпапок
	Public mArrSubItem	'Массив под элементов



	Private Sub Class_Initialize
		mName = ""
		mArrSubFolder = Array()
		mArrSubItem = Array()
		Level = 0
	End Sub

	Sub Init (nFolderName, nFolderAlias)
		mName	= nFolderName
		mAlias	= nFolderAlias
		mName = Replace(mName, """", "''")
		mAlias = Replace(mAlias, """", "''")
	End Sub


	Function AddSubFolder( nFolderName, nFolderAlias )

		On Error Resume Next
		Set NewFolder = New ALSFolder
		NewFolder.mLevel = mLevel + 1
		NewFolder.Init nFolderName, nFolderAlias

		AppendArray mArrSubFolder, NewFolder
		if Err.number<>0 Then
			Message "не удалось создать папку: " + nFolderName, mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
			Exit Function
		End If
		On Error goto 0


		Set AddSubFolder = NewFolder
	End Function

	Function AddSubItem( nItemName, nItemAlias )

	On Error Resume Next
		Set NewALSItem = New ALSItem
		NewALSItem.mLevel = mLevel + 1
		NewALSItem.Init nItemName, nItemAlias
		AppendArray mArrSubItem, NewALSItem
		Set AddSubItem = NewALSItem
		if Err.number<>0 Then
			Message "не удалось создать папку: " + nItemName, mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
		End If
		On Error goto 0

	End Function

	Function Dump(  )
		Dump = False
		If (UBound(mArrSubFolder) + UBound (mArrSubItem)) >-2 Then
			' Отсчитать уровень...
			Indent = String(mLevel," ")
			' Начало секции
			StrToDump = Indent + "{""Folder"",""AST"",""" + mName + """,""" + mAlias + ""","
			AlsToFile.WriteLine StrToDump

			For i=0 To UBound (mArrSubItem)
				Set nALSItem = mArrSubItem(i)
				Dump = nALSItem.Dump
			Next

			For i=0 To UBound (mArrSubFolder)
				Set nFolder = mArrSubFolder(i)
				Dump = nFolder.Dump
			Next
			' Конец секции
			StrToDump = Indent + "},"
			AlsToFile.WriteLine StrToDump
		End If

	End Function

End Class 'ALSFolder

'::::::::::::::::::::::::::::::::::::::::::::::::::::::
' Класс для генерации als-файлов совместно с *.ints файлами
'
' ИНТЕРФЕЙС:
' ---------------------------
' Init(nFileName*)		- Устанавливаем ПОЛНОЕ!! имя файла для генерации
' Dump ()				- Выливаем данные в файл "AlsToFile" в соответствии с форматом.
' CreateRootFolder( nFolderName*, nFolderAlias*)	- добавляет корневой "Раздел",
'		а к корневому разделу уже можно обращаться как к ALSFolder-у
'_______________________________________
'  Примечание: mMamber*	- Обязательный атрибут

'ALSGenerator
Class ALSGenerator ' Сам класс генератор als-файлов....
	Public Redy				' Класс готов к работе...
	Public RootFolder		' Корневая папка...
	Public mFileName		' имя файла

	' Идет разрушение класса
	Private Sub Class_Terminate	' Setup Terminate event.
		'Закрываем файл...
		On Error Resume Next
		if IsObject(AlsToFile) Then AlsToFile.Close

		if Err.number<>0 Then
			Message "Не удалось закрыть файл" + mFileName, mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
		End If
		On Error goto 0
	End Sub

	Private Sub Class_Initialize
		Redy		= False
		mFileName	= ""
	End Sub

	' Пользовательская инициализация...
	' Параметр: nFileName - имя файла, которое предстоит создать
	Function Init(nFileName)
		Init = True
		mFileName = nFileName

		Redy = True
	End Function

	Function CreateRootFolder( nFolderName, nFolderAlias) '(ParentFolder, nFolderName,  nFolderName)
		RootFolder = Null
		On Error Resume Next
		Set RootFolder = New ALSFolder
		RootFolder.mLevel = 1
		Set CreateRootFolder = RootFolder
		RootFolder.Init nFolderName, nFolderAlias

		if Err.number<>0 Then
			Message "не удалось создать папку: " + nFolderName, mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
		End If
		On Error goto 0

	End Function

	Function Dump()
		Dump = False

		StrQuestion = "Готовы данные для генерации als-файла!" + vbCr +_
		mFileName + vbCr +_
		"Генерировать?"
		Dim MyAnswer
		MyAnswer = MsgBox (StrQuestion, vbOKCancel+vbQuestion, "OLE-ActiveX *.ints Generator")
		if MyAnswer = vbCancel Then
			Dump = True
			Exit Function
		End If
		On Error Resume Next
		if fso.FileExists(mFileName) Then fso.DeleteFile mFileName
		Set AlsToFile = fso.CreateTextFile( mFileName )
		'if Exept("Не удалось удалиить файл" + mFileName) Then Exit Function

		if Err.number<>0 Then
			Message "Не удалось удалиить файл" + mFileName, mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
			Exit Function
		End If
		On Error goto 0


		DeleteFile = false
		if not IsEmpty(RootFolder) Then
			AlsToFile.WriteLine "{""Shell"","
			if Not IsNull(RootFolder) Then Dump = RootFolder.Dump
			AlsToFile.WriteLine "}"
		else
			DeleteFile = true
		End if
		AlsToFile.Close
		AlsToFile = Null
		If len(mFileName)>0 And DeleteFile Then fso.DeleteFile mFileName
		if Err.number<>0 Then
			Message "Генерация als-файла " + mFileName + "завершилась ошибкой...", mExclamation3
			Message err.Description, mExclamation3
			On Error goto 0
			Dump = False
		End If
		On Error goto 0


		if Dump Then Message "Сгенерирован als-file: " + mFileName

	End Function

End Class


' Класс для анализа библиотеки.
Class ITypeLib
	Public LibName 'Имя библитеки
	Public LibPath 'Имя библитеки
	Public Library 'библитека (TypeLibInfo)
	Public DefaultInterfaseName ' интерфейс по умолчанию
	Public ThisProgID 'ProgID библиотеки
	Public ThisProgIDForIntel 'ProgID библиотеки, преобразованный для OtherTypesDefine:  Excel.Application>>excel_application

	Public LibraryIsLoad ' Библиотека считана и готова к работе.

	Public DebugFlag 'Флаг отладки
	Public flOutNameAndTypeParameters 'Выводить ли информацию о параметрах функции (имена и типы)
	Public AllDumpInterfases 'Все выведенные интерфейсы.

	Dim DataDir							' Каталог, где хранятся *.ints файлы
	Dim FileNameTypesStructureExt		' Имя файла из которого будет подгружаться словарь Intellisence "TypesStructureExt"
	Dim FileNameTypesStructureExtDef	' Имя файла из которого будет подгружаться словарь Intellisence "TypesStructureExt"
	' Содержит наименование интерфейса для записи из TypesStructureExt, для быстрого поиска в синтакс-помощнике
	Dim FileNameOtherTypesDefine		' Имя файла из которого будет подгружаться словарь Intellisence "OtherTypesDefine"
	Dim FileNameProgIDDumped			' Имя файла, содержащего прог-иды по которым данные сформированы
	' из-за того, что объект, созданный по прог-иду может быть опасен
	' или уже есть сформированные *.ints файлы

	Public TypeStr

	Public ErrorNumber				' Номер последней ошибки загрузки.

	'Public ALSCreate				' нужно ли генерировать als-файл?
	Public mALSGenerator			' элемент класса для генерации als-файла....
	Public mALSParentF				' Родительская папка...

	Public TLITypeLibInfo
	Public TLIApplication

	Dim INVOKE_UNKNOWN
	Dim INVOKE_FUNC
	Dim INVOKE_PROPERTYGET
	Dim INVOKE_PROPERTYPUT
	Dim INVOKE_PROPERTYPUTREF
	Dim INVOKE_EVENTFUNC
	Dim INVOKE_CONST

	Dim VT_EMPTY
	Dim VT_NULL
	Dim VT_I2
	Dim VT_I4
	Dim VT_R4
	Dim VT_R8
	Dim VT_CY
	Dim VT_DATE
	Dim VT_BSTR
	Dim VT_DISPATCH
	Dim VT_ERROR
	Dim VT_BOOL
	Dim VT_VARIANT
	Dim VT_UNKNOWN
	Dim VT_DECIMAL
	Dim VT_I1
	Dim VT_UI1
	Dim VT_UI2
	Dim VT_UI4
	Dim VT_I8
	Dim VT_UI8
	Dim VT_INT
	Dim VT_UINT
	Dim VT_VOID
	Dim VT_HRESULT
	Dim VT_PTR
	Dim VT_SAFEARRAY
	Dim VT_CARRAY
	Dim VT_USERDEFINED
	Dim VT_LPSTR
	Dim VT_LPWSTR
	Dim VT_RECORD
	Dim VT_FILETIME
	Dim VT_BLOB
	Dim VT_STREAM
	Dim VT_STORAGE
	Dim VT_STREAMED_OBJECT
	Dim VT_STORED_OBJECT
	Dim VT_BLOB_OBJECT
	Dim VT_CF
	Dim VT_CLSID
	Dim VT_VECTOR
	Dim VT_ARRAY
	Dim VT_BYREF
	Dim VT_RESERVED

	Dim TKIND_ENUM
	Dim TKIND_RECORD
	Dim TKIND_MODULE
	Dim TKIND_INTERFACE
	Dim TKIND_DISPATCH
	Dim TKIND_COCLASS
	Dim TKIND_ALIAS
	Dim TKIND_UNION
	Dim TKIND_MAX



	' Считать библиотеку
	' CreateObj - создавать объект, если по прог-иду не нашли
	' ProgID - прог-ид объекта
	Function LoadLibrary( ProgID, CreateObj )
		LoadLibrary = False
		LibraryIsLoad = False
		if ProgIDIsDumped( ProgID ) Then
			Message "Файлы для " + ProgID + " уже сформированы. Если нужно заново переформировать их, удалитте строку: "+ProgID+" из файла: " + FileNameProgIDDumped
			Exit Function
		End If

		ThisProgID = ProgID

		' Ищем прог-ид в реестре.
		LibPath = GetPathFromProgID ( ProgID )
		if IsNull(LibPath) Then
			ErrorNumber = 1
			SaveDumpedProgID lCase(ThisProgID)
			Message "Прог-ид " + ThisProgID + " не найден в реестре и помечен как обработанный для исключения повторной генерации.", mExclamation
			Exit Function
		End IF
		LibPath = Replace(LibPath,"/automation","")
		LibPath = Trim(LibPath)

		if not FSO.FileExists(LibPath) Then
			ErrorNumber = 1
			SaveDumpedProgID lCase(ThisProgID)
			Message "Прог-ид " + ThisProgID + "Нет файла:" + LibPath	, mExclamation
			LibPath = Null
		End If
		If not IsNull(LibPath) then
			On Error Resume Next
			Set Library = TLIApplication.TypeLibInfoFromFile(LibPath)
			if Err.number = 0 Then
				If not IsNull(Library) And Not IsEmpty( Library ) Then
					LoadLibrary = true
				End if
			End if
			On Error Goto 0
		End if
		LoadLibrary = False


		StrQuestion = "ВНИМАНИЕ! Сейчас будет произведено создание объекта """+ProgID+""""+vbCr+_
		"Для генерации файлов Intellisence. Некоторые объекты не поддерживают "+vbCr+_
		"динамическое создание и могут привести к завершению программы."+vbCr+_
		"Рекомендуется сохранить данные." + vbCr +_
		"Продолжить?"
		Dim MyAnswer
		MyAnswer = MsgBox (StrQuestion, vbOkCancel+vbQuestion, "OLE-ActiveX *.ints Generator")
		if MyAnswer = vbCancel Then
			LoadLibrary = False
			LibraryIsLoad = LoadLibrary
			StrQuestion = "Пометить объект """+ProgID+""" как обработанный?"+ vbCr+_
			"По помеченным объектам генерация не производится."

			MyAnswer = MsgBox (StrQuestion, vbYesNo+vbQuestion, "OLE-ActiveX *.ints Generator")
			if MyAnswer = vbYes Then
				SaveDumpedProgID lCase(ThisProgID)
				ErrorNumber = 1
			End If
			Exit Function
		End If

		' Нужно создать объект из которого извлечем дефолтный интерфейс ииспользуем его
		' Если мне кто подскажет как получить дефолтный интерфейс без создания объекта буду признателен.
		On Error Resume Next
		Set Obj = CreateObject(ProgID)
		if Err.number<>0 Then
			LoadLibrary = false
			LibraryIsLoad = false
			ErrorNumber = 1
			SaveDumpedProgID lCase(ThisProgID)
			Message "По прог-ид " + ThisProgID + " создать объект не удалось", mExclamation
			On Error goto 0
			Exit Function
		End If
		On Error goto 0

		if not IsEmpty(Obj) Then
			On Error Resume Next
			Set DefaultInterfase = TLIApplication.InterfaceInfoFromObject(Obj)
			if Err.number<>0 Then
				LoadLibrary = false
				LibraryIsLoad = false
				ErrorNumber = 1
				SaveDumpedProgID lCase(ThisProgID)
				Message "По прог-ид " + ThisProgID + " не удалось получить главного интерфейса", mExclamation
				On Error goto 0
				Exit Function
			End If
			On Error goto 0

			if Not IsEmpty( DefaultInterfase ) Then
				On Error Resume Next
				LibPath = DefaultInterfase.Parent.ContainingFile
				Set Library = TLIApplication.TypeLibInfoFromFile(LibPath)
				DefaultInterfaseName = DefaultInterfase.Name
				LoadLibrary = true
				LibraryIsLoad = true
				if Err.number<>0 Then
					LoadLibrary = false
					LibraryIsLoad = false

					ErrorNumber = 1
					SaveDumpedProgID lCase(ThisProgID)
					Message "По прог-ид " + ThisProgID + " информацию из библиотеки типов..", mExclamation
					On Error goto 0
					Exit Function
				On Error goto 0
				End If
			On Error goto 0
			End IF
			Obj = Null
		End IF
		LibraryIsLoad = LoadLibrary
		ThisProgIDForIntel = lCase(ThisProgID)
		ThisProgIDForIntel = Replace(ThisProgIDForIntel,".", "_")
	'Stop
		OtherTypesDefineString = lCase(ThisProgID)+","+ThisProgIDForIntel
		DumpStringToFile OtherTypesDefineString, FileNameOtherTypesDefine
		SaveDumpedProgID ThisProgID
	End Function


	' Уникальное добавление строки в файл.
	Private Sub DumpStringToFile (DumpString, FileName)
		if Not fso.FileExists( FileName ) Then
			Set File = fso.CreateTextFile( FileName, False)
		Else
			Set File0 = fso.GetFile( FileName )
			Set File = File0.OpenAsTextStream(1)
			if File0.Size > 0 Then
				TextStream = File.ReadAll()
				If InStr(1, lCase(TextStream), lcase(DumpString))>0 Then
					File.Close
					Exit Sub
				End IF
			End IF
			Set File = File0.OpenAsTextStream(8)

		End IF
		File.WriteLine DumpString
		File.Close
	End Sub


	' Запоминаем обработанный ProgID, по которому сгенерированы файлы *.ints
	'private ' Пока не приватничаем, отлаживаем
	Sub SaveDumpedProgID( DumpedProgID )
		DumpStringToFile DumpedProgID, FileNameProgIDDumped
	End Sub


	Private  Sub DMessage( mess )
		if DebugFlag Then
			Message mess
		End If
	End Sub

	function CheskClass() 'Проверим клас, как он инициализировался...
		CheskClass = true
		If IsEmpty(TLITypeLibInfo) Then
			CheskClass = false
		ElseIf IsEmpty(TLIApplication) Then
			CheskClass = false
		ElseIf IsEmpty(WSH) Then
			CheskClass = false
		End IF
	End Function

	Private Sub Class_Initialize
		LibName = null
		On Error Resume Next
		Set TLITypeLibInfo = CreateObject("TLI.TypeLibInfo")
		Set TLIApplication = CreateObject("TLI.TLIApplication")
		if Err.number<>0 then
			Message "Класс: ITypeLib не инициализировался, библиотека: TLBINF32.DLL не зарегистрирована на данном рабочем месте!"
		End If
		On Error Goto 0
		Set TypeStr = CreateObject("Scripting.Dictionary")
		Set AllDumpInterfases = CreateObject("Scripting.Dictionary")
		DefaultInterfase = Null
		DebugFlag = false
		flOutNameAndTypeParameters = False

		DataDir = BinDir & "\Config\Intell"
		FileNameTypesStructureExt		= DataDir + "\TypesStructureExt.txt"	' Имя файла из которого будет подгружаться словарь Intellisence "TypesStructureExt"
		FileNameTypesStructureExtDef	= DataDir + "\TypesStructureExtDef.txt"	' Имя файла из которого будет подгружаться словарь Intellisence "TypesStructureExtDef"
		FileNameOtherTypesDefine		= DataDir + "\OtherTypesDefine.txt"		' Имя файла из которого будет подгружаться словарь Intellisence "OtherTypesDefine"
		FileNameProgIDDumped			= DataDir + "\ProgIDDumped.txt"			' Имя файла, содержащего прог-иды по которым данные сформированы
		ErrorNumber = 0



		INVOKE_UNKNOWN = 0
		INVOKE_FUNC = 1
		INVOKE_PROPERTYGET = 2
		INVOKE_PROPERTYPUT = 4
		INVOKE_PROPERTYPUTREF = 8
		INVOKE_EVENTFUNC = 16
		INVOKE_CONST = 32

		VT_EMPTY = 0
		VT_NULL = 1
		VT_I2 = 2
		VT_I4 = 3
		VT_R4 = 4
		VT_R8 = 5
		VT_CY = 6
		VT_DATE = 7
		VT_BSTR = 8
		VT_DISPATCH = 9
		VT_ERROR = 10
		VT_BOOL = 11
		VT_VARIANT = 12
		VT_UNKNOWN = 13
		VT_DECIMAL = 14
		VT_I1 = 16
		VT_UI1 = 17
		VT_UI2 = 18
		VT_UI4 = 19
		VT_I8 = 20
		VT_UI8 = 21
		VT_INT = 22
		VT_UINT = 23
		VT_VOID = 24
		VT_HRESULT = 25
		VT_PTR = 26
		VT_SAFEARRAY = 27
		VT_CARRAY = 28
		VT_USERDEFINED = 29
		VT_LPSTR = 30
		VT_LPWSTR = 31
		VT_RECORD = 36
		VT_FILETIME = 64
		VT_BLOB = 65
		VT_STREAM = 66
		VT_STORAGE = 67
		VT_STREAMED_OBJECT = 68
		VT_STORED_OBJECT = 69
		VT_BLOB_OBJECT = 70
		VT_CF = 71
		VT_CLSID = 72
		VT_VECTOR = 4096
		VT_ARRAY = 8192
		VT_BYREF = 16384
		VT_RESERVED = 32768

		TypeStr.Add VT_EMPTY ,"VT_EMPTY"
		TypeStr.Add VT_NULL ,"VT_NULL"
		TypeStr.Add VT_I2 ,"Число (VT_I2)"
		TypeStr.Add VT_I4 ,"Число (VT_I4)"
		TypeStr.Add VT_R4 ,"Число (VT_R4)"
		TypeStr.Add VT_R8  ,"Число (VT_R8)"
		TypeStr.Add VT_CY ,"VT_CY"
		TypeStr.Add VT_DATE  ,"Дата (VT_DATE)"
		TypeStr.Add VT_BSTR ,"Строка (VT_BSTR)"
		TypeStr.Add VT_DISPATCH ,"VT_DISPATCH"
		TypeStr.Add VT_ERROR ,"VT_ERROR"
		TypeStr.Add VT_BOOL ,"Булево (VT_BOOL)"
		TypeStr.Add VT_VARIANT ,"VT_VARIANT"
		TypeStr.Add VT_UNKNOWN ,"VT_UNKNOWN"
		TypeStr.Add VT_DECIMAL ,"Число (VT_DECIMAL)"
		TypeStr.Add VT_I1 ,"Число (VT_I1)"
		TypeStr.Add VT_UI1 ,"Число (VT_UI1)"
		TypeStr.Add VT_UI2 ,"Число (VT_UI2)"
		TypeStr.Add VT_UI4 ,"Число (VT_UI4)"
		TypeStr.Add VT_I8 ,"Число (VT_I8)"
		TypeStr.Add VT_UI8 ,"Число (VT_UI8)"
		TypeStr.Add VT_INT ,"Число (VT_INT)"
		TypeStr.Add VT_UINT ,"Число (VT_UINT)"
		TypeStr.Add VT_VOID,"VT_VOID"
		TypeStr.Add VT_HRESULT,"VT_HRESULT"
		TypeStr.Add VT_PTR,"VT_PTR"
		TypeStr.Add VT_SAFEARRAY ,"VT_SAFEARRAY"
		TypeStr.Add VT_CARRAY ,"VT_CARRAY"
		TypeStr.Add VT_USERDEFINED,"VT_USERDEFINED"
		TypeStr.Add VT_LPSTR,"VT_LPSTR"
		TypeStr.Add VT_LPWSTR,"VT_LPWSTR"
		TypeStr.Add VT_RECORD,"VT_RECORD"
		TypeStr.Add VT_FILETIME,"VT_FILETIME"
		TypeStr.Add VT_BLOB,"VT_BLOB"
		TypeStr.Add VT_STREAM,"VT_STREAM"
		TypeStr.Add VT_STORAGE,"VT_STORAGE"
		TypeStr.Add VT_STREAMED_OBJECT,"VT_STREAMED_OBJECT"
		TypeStr.Add VT_STORED_OBJECT,"VT_STORED_OBJECT"
		TypeStr.Add VT_BLOB_OBJECT,"VT_BLOB_OBJECT"
		TypeStr.Add VT_CF,"VT_CF"
		TypeStr.Add VT_CLSID,"VT_CLSID"
		TypeStr.Add VT_VECTOR,"VT_VECTOR"
		TypeStr.Add VT_ARRAY,"VT_ARRAY"
		TypeStr.Add VT_BYREF,"VT_BYREF"
		TypeStr.Add VT_RESERVED,"VT_RESERVED"

		TKIND_ENUM = 0
		TKIND_RECORD = 1
		TKIND_MODULE = 2
		TKIND_INTERFACE = 3
		TKIND_DISPATCH = 4
		TKIND_COCLASS = 5
		TKIND_ALIAS = 6
		TKIND_UNION = 7
		TKIND_MAX = 8


	End Sub

	Private Function OstDel(Pa1, Pa2)
		OstDel = Pa1 - ((Pa1\Pa2)*Pa2)
	End Function

	Private Sub Class_Terminate	' Setup Terminate event.
		TLITypeLibInfo = Null
		TLIApplication = Null
		DefaultInterfaseName = Null
	End Sub

	Function AttrIsEnum ( InvokeKinds ) ' Атрибут есть перечисление
		IsEnumAttr = False
		if OstDel(Fix(InvokeKinds/INVOKE_CONST),2) = 1 Then
			IsEnumAttr = True
		End IF
	End Function

	Function AttrIsEvent ( InvokeKinds ) ' Атрибут есть событие
		AttrIsEvent = False
		if OstDel(Fix(InvokeKinds/INVOKE_EVENTFUNC),2) = 1 Then
			AttrIsEvent = True
		End IF
	End Function

	Function AttrIsFunc ( InvokeKinds ) ' Атрибут есть метод
		AttrIsFunc = False
		if OstDel(Fix(InvokeKinds/INVOKE_FUNC),2) = 1 Then
			AttrIsFunc = True
		End IF
	End Function

	Function AttrIsProps ( InvokeKinds ) ' Атрибут есть свойство
		AttrIsProps = False
		if OstDel(Fix(InvokeKinds/INVOKE_PROPERTYGET),2) = 1 Then
			AttrIsProps = True
		Elseif OstDel(Fix(InvokeKinds/INVOKE_PROPERTYPUT),2) = 1 Then
			AttrIsProps = True
		Elseif OstDel(Fix(InvokeKinds/INVOKE_PROPERTYPUTREF),2) = 1 Then
			AttrIsProps = True
		End IF
	End Function


	Function GetObjectByName( InterfaseName )
		GetObjectByName = Null
		if IsObject(Library) Then
			set GetObjectByName = Library.Interfaces.NamedItem(""+ InterfaseName )
		End If
	End Function
	Function GetObjectDefault(  )
		GetObjectDefault = Null
		if IsObject(Library) Then
			set GetObjectDefault = Library.Interfaces.NamedItem(""+ DefaultInterfaseName )
		End If
	End Function

	'Функция ПостроитьДанныеПоиска(Знач НомерИнформацииОТипе,Знач ТипыПоиска,НомерБиблиотеки=0,Скрытые = Ложь)
	private Function NoMyMakeSearchData (TypeInfoNumber) ',127,0,false)
		NoMyMakeSearchData = 127 * 16777216 + (4096 +  TypeInfoNumber - 4096 * Fix(TypeInfoNumber/4096))
	end Function

	' перебрать свойства, функции и методы интерфейса или сокласа легко,
	' а вот получить более расширенную инфу по ним без этой процедуринки и
	' без NoMyMakeSearchData у меня не получилось (((, ну ничего ))) ведь получилось......
	Function GetMemberOfObjectInfo ( Member, Object )
		GetMemberOfObjectInfo = null
		if IsObject( Member ) And IsObject( Object ) Then
			SearchData = NoMyMakeSearchData( Object.TypeInfoNumber )
			set GetMemberOfObjectInfo = Library.GetMemberInfo ( SearchData, Member.InvokeKinds,  Member.MemberId, Member.Name)
		End if
	end Function


	' Преобразовываем параметр типа вариант в строковое представление
	Function VarTypeToString ( VarType )
		VarTypeToString = ""
		If TypeStr.Exists( VarType ) Then
			VarTypeToString = TypeStr.Item( VarType )
		End If
	end Function

	' Имеет ли параметр значение по умолчанию?
	Function ParamHaveDefValue ( Param )
		ParamHaveDefValue = False
		If Param.Default And Param.Optional Then
			ParamHaveDefValue = True
		End If
	end Function

	' Это обязательный параметр
	Function ParamIsBinding ( Param )
		ParamIsBinding = False
		If Param.Default Or Param.Optional Then
			ParamIsBinding = True
		End If
	end Function

	' Обработан ли интерфейс с пом. DumpInterfaseObject
	Function InterfaseIsDump ( Name )
		InterfaseIsDump = AllDumpInterfases.Exists(Trim(LCase(Name)))
	end Function

	' Отметить что интерфейс выведен. DumpInterfaseObject
	Function InterfaseIsDumpMark ( Name )
		If Not InterfaseIsDump(Trim(LCase(Name))) Then
			AllDumpInterfases.Add Trim(LCase(Name)), Trim(LCase(Name))
		end if
	end Function

	' Возвращает строковое представление типа параметра
	Function GetParamTypeStr ( Param )
		ParamTipeStr = ""
		Set ParaVarTypeInfo = Param.VarTypeInfo
		ParaVarType  = ParaVarTypeInfo.VarType
		Fixed = Fix ( ParaVarType / 4096)
		FlagVT_ARRAY = OstDel(Fixed, 2)
		FlagVT_VECTOR = OstDel( Fix( Fixed / 2),2)
		TypeVars = Fix(Fixed/4)*4*4096+(ParaVarType - Fixed)
		If TypeVars = 0 Then ' Неопределено.....
			TKind = Null
			Do ' Будем выполнять пока нет ошибок
				On Error Resume Next
				set InfoOfType = ParaVarTypeInfo.TypeInfo
				set TIType =  ParaVarTypeInfo.TypeInfo

				TIResolved = ParaVarTypeInfo.TypeInfo
				'set TIResolved = ParaVarTypeInfo.TypeInfo
				TKind = TIResolved.TypeKind
				if Err.Number <> 0 Then
					TKind = ParaVarTypeInfo.TypeInfo.TypeKind
					on error goto 0
				End If
			Loop Until True
			While TKind = TKIND_ALIAS
				TKind = TKIND_MAX
				On Error Resume Next
				set TIResolved = TIResolved.ResolvedType
				TKind = TIResolved.TypeKind
				on error goto 0
			Wend

			If TKind = Null Then
				ParamTipeStr = "?"
			else
				IF ParaVarTypeInfo.IsExternalType Then
					ParamTipeStr = ParaVarTypeInfo.TypeLibInfoExternal.Name+"."+TIType.Name
				Else
					ParamTipeStr = TIType.Name
				End If
			End If

		Else
			If ParaVarTypeInfo.VarType <> VT_VARIANT Then
				ParamTipeStr = VarTypeToString(Param.VarTypeInfo.VarType)
			Else
				ParamTipeStr = "VT_VARIANT"
			End IF

		End If
		If IsEmpty( ParamTipeStr ) Or (ParamTipeStr = "")  Then
			if Param.VarTypeInfo.TypeInfoNumber <> -1 Then
				ParamTipeStr = Param.VarTypeInfo.TypeInfo.TypeKindString
			else
				ParamTipeStr = VarTypeToString(Param.VarTypeInfo.VarType)
			End IF
		End IF
		GetParamTypeStr = ParamTipeStr
	end Function

	' Преобразовываем имя интерфейса для Интела
	' Дефолтный интерфейс у нас идет как просто прог-ид, остальные с приставкой из прог-ида.
	Private Function MakeIName( iName )
		nMakeIName = lcase(iName)
		If LCase(nMakeIName) = LCase(DefaultInterfaseName) Then nMakeIName = ""
		MakeIName = ThisProgIDForIntel + nMakeIName
	end Function

	' Сохраняем данные для словаря из Intellisence "TypesStructureExt"
	Sub SaveForTypesStructureExt (Interf1, FuncPropName, Interf2)
		SaveStr = MakeIName(""+Interf1)+ "." + Lcase(FuncPropName)+"," + "VALUE|"+MakeIName(""+Interf2)
		DumpStringToFile SaveStr, FileNameTypesStructureExt
		DumpStringToFile MakeIName(""+Interf2)+","+Interf2, FileNameTypesStructureExtDef
	end Sub

	' Вываливаем информацию по интерфейсу в окно сообщений...
	' и за одно не забываем генерить файлы для интеллиценза...
	' Name - имя интерфейса
	' AllStrIterf - пройденный путь к этому интерфейсу
	Sub DumpInterfaseObject ( Name , AllStrIterf )
		'Message "%% " + AllStrIterf + "." + Name
		'Message "Информация по интерфейсу: " + Name
		FindInterfases = "" ' найденные "по пути" интерфейсы...
		On Error Resume Next
		Set Object = GetObjectByName( "" + Name )
		set TrueMembers = Object.Members.GetFilteredMembers
		IF err.number<>0 Then
			On Error goto 0
			Exit Sub
		End If
		IntsFileName = DataDir+"\" + MakeIName(Name)+".ints"
		if fso.FileExists(IntsFileName) Then fso.DeleteFile IntsFileName,true
		Set IntsFile = fso.CreateTextFile(IntsFileName)
		Status "Генерируем: " + IntsFileName

		Set ALSIntefaseFolder = mALSParentF.AddSubFolder( NAme,	"" )
		Set ALSFolderProp = ALSIntefaseFolder.AddSubFolder( "Атрибуты",	"" )
		Set ALSFolderFunc = ALSIntefaseFolder.AddSubFolder( "Методы",		"" )

		for i=1 To TrueMembers.Count
			set Member = TrueMembers.item(i)
			Set MemberInfo = GetMemberOfObjectInfo ( Member, Object )

			MemberIK = Member.InvokeKinds
			If AttrIsProps( MemberIK) Then 'INVOKE_EVENTFUNC  событие
				Set ALSProp = ALSFolderProp.AddSubItem( Member.Name,"" )
				ALSProp.mWriting = Member.Name ': ALSProp.mWritingEng = Member.Name
				ALSProp.mSyntax = Member.Name
				ALSProp.mAssignment = MemberInfo.HelpString

				if MemberInfo.ReturnType.TypeInfoNumber <> -1 Then
					If MemberInfo.ReturnType.TypeInfo.TypeKindString = "coclass" Then
						' Если это соклас, тогда ищем интерфейс
						'Message " prop - " + Member.Name + " dispinterface " + MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name
						FindInterfases = FindInterfases + vbCrLf + MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name
						SaveForTypesStructureExt Name, Member.Name, MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name
						ALSProp.mParams = "Возвращает: """ + MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name+""""
					ElseIf MemberInfo.ReturnType.TypeInfo.TypeKindString = "dispinterface" Then
						'Message " prop - " + Member.Name + " " + MemberInfo.ReturnType.TypeInfo.TypeKindString + " " + MemberInfo.ReturnType.TypeInfo.Name
						FindInterfases = FindInterfases + vbCrLf + MemberInfo.ReturnType.TypeInfo.Name
						SaveForTypesStructureExt Name, Member.Name, MemberInfo.ReturnType.TypeInfo.Name
						ALSProp.mParams = "Возвращает: """ + MemberInfo.ReturnType.TypeInfo.Name + """"
					Else
						'Message " prop - " + Member.Name
					End If
				Else

					ALSProp.mParams = "Возвращает: " + VarTypeToString ( MemberInfo.ReturnType.VarType )
					'Message " prop - " + Member.Name
				End If

				IntsFile.WriteLine "0000 "+ Member.Name
			'ElseIf AttrIsFunc ( MemberIK) And False Then 'INVOKE_FUNC  функция/метод
			ElseIf AttrIsFunc ( MemberIK) Then 'INVOKE_FUNC  функция/метод
				' Формируем строку метода....
				Set ALSProp = ALSFolderFunc.AddSubItem( Member.Name,"" )
				ALSProp.mAssignment = MemberInfo.HelpString

				StrForMessage = ""
				StrForAls = Member.Name+"("
				StrForAlsParams = ""
				if MemberInfo.ReturnType.TypeInfoNumber = -1 Then
					StrForMessage = " meth - " + Member.Name
				Else
					StrForMessage = " func - " + Member.Name
				end if
				ForFileString = Member.Name + "("
				For ParCnt = 1 To MemberInfo.Parameters.Count
					Set Param = MemberInfo.Parameters.Item(ParCnt)
					' Param.Default = true -> Имеет значение по умолчанию = необязательный
					'if flOutNameAndTypeParameters Then
						if Param.Default Then
							StrForMessage = StrForMessage + "["
							StrForAls = StrForAls + "["
						End if
						StrForMessage = StrForMessage + Param.Name + " as " + GetParamTypeStr ( Param )
						StrForAls = StrForAls + Param.Name
						StrForAlsParams = StrForAlsParams + "<"+ Param.Name +"> - " + GetParamTypeStr ( Param )
						if Param.Default Then
							StrForMessage = StrForMessage + "]"
							StrForAls = StrForAls + "]"
						end if
					'end if
					if ParCnt <> MemberInfo.Parameters.Count Then ForFileString = ForFileString + ", " : StrForAlsParams = StrForAlsParams + vbCrLf
					if ParCnt <> MemberInfo.Parameters.Count Then StrForMessage = StrForMessage + ", "
					if ParCnt <> MemberInfo.Parameters.Count Then StrForAls = StrForAls + ", "
					StrForMessage = Replace(StrForMessage," ","") ' Убираем пробелы
					ForFileString = Replace(ForFileString," ","") ' Убираем пробелы
				Next
				ForFileString = ForFileString + ")"
				StrForMessage = StrForMessage + ")"
				StrForAls = StrForAls + ")"
				ALSProp.mWriting = ForFileString
				ALSProp.mSyntax = StrForAls
				ALSProp.mParams = StrForAlsParams
				' Надо выяснить тип возвращаемого значения
				if MemberInfo.ReturnType.TypeInfoNumber <> -1 Then
					if MemberInfo.ReturnType.TypeInfo.TypeKindString = "dispinterface" Then
						FindInterfases = FindInterfases + vbCrLf + MemberInfo.ReturnType.TypeInfo.Name
						StrForMessage = StrForMessage + " as Interfase " + MemberInfo.ReturnType.TypeInfo.Name
						SaveForTypesStructureExt Name, Member.Name, MemberInfo.ReturnType.TypeInfo.Name
						ALSProp.mParams = ALSProp.mParams + vbCrLf + "Возвращает: """ + MemberInfo.ReturnType.TypeInfo.Name+""""
					Elseif MemberInfo.ReturnType.TypeInfo.TypeKindString = "coclass" Then
						FindInterfases = FindInterfases + vbCrLf + MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name
						SaveForTypesStructureExt Name, Member.Name, MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name
						FindInterfases = FindInterfases + vbCrLf + MemberInfo.ReturnType.TypeInfo.DefaultEventInterface.Name
						SaveForTypesStructureExt Name, Member.Name, MemberInfo.ReturnType.TypeInfo.DefaultEventInterface.Name
						ALSProp.mParams = ALSProp.mParams + vbCrLf + "Возвращает: """ + MemberInfo.ReturnType.TypeInfo.DefaultInterface.Name+""""
					end if
				end if
				'if MemberInfo.HelpString <> "" Then StrForMessage = StrForMessage + " Help -> " + MemberInfo.HelpString
				'Message StrForMessage
				IntsFile.WriteLine "0000 " + ForFileString
			End IF
		Next
		IntsFile.Close
		InterfaseIsDumpMark(Name)

		ArrNameOfIntrf = Split(FindInterfases, vbcrlf)
		for i=0 To UBound(ArrNameOfIntrf)
			iName = trim(ArrNameOfIntrf(i))
			if iName <> "" Then
				if Not InterfaseIsDump(iName) Then
					DumpInterfaseObject iName, AllStrIterf + "." + Name
				End IF
			End IF
		next
	end Sub
End class 'ITypeLib



' Ключевая процедура генерации
Function IntsGenerator( ProgID )
'Stop
	IntsGenerator = False
	if ProgIDIsDumped( ProgID ) Then Exit Function
	if not IsProgID( ProgID )  Then Exit Function

	Set li = new ITypeLib
	li.DebugFlag = true
	li.flOutNameAndTypeParameters = false
	if li.LoadLibrary( ProgId , True) Then
		Set li.mALSGenerator = New ALSGenerator
		li.mALSGenerator.Init DataDirAls+ ProgID +".als"
		Set li.mALSParentF = li.mALSGenerator.CreateRootFolder( ProgID, "")

		IntsGenerator = True
		messageStr =				"######################################################################" + vbCrLf
		messageStr = messageStr +	"Библиотека: " + li.LibPath + " считана по прог-иду: " + li.ThisProgID + vbCrLf
		messageStr = messageStr +	"Интерфейс по умолчанию:" + li.DefaultInterfaseName + vbCrLf
		AllStrIterf = ""
		li.DumpInterfaseObject  li.DefaultInterfaseName , AllStrIterf
		messageStr = messageStr +	"Сгенерировали данные по: " + ProgID
		message messageStr, mExclamation3
		Set ALSProp			= li.mALSParentF.AddSubItem( ProgID,"" )

		ALSProp.mWriting	= ProgID ': ALSProp.mWritingEng = Member.Name
		ALSProp.mSyntax		= "Объект = СоздатьОбъект(""" + ProgID+ """);" + vbCrLf + "Объект.<?>;"
		ALSProp.mWriting	= "Объект = СоздатьОбъект(""" + ProgID+ """);" + vbCrLf + "Объект.<?>;"
		ALSProp.mNotes		= "@Данные_библиотеки:"	+ vbCrLf +_
								"Help: "		+ li.Library.HelpString + vbCrLf +_
								"Name: "		+ li.Library.Name + vbCrLf +_
								"GUID: "		+ li.Library.GUID + vbCrLf +_
								"LibPath: """	+ li.LibPath +""""+ vbCrLf +_
								"===================================================="	 + vbCrLf +_
								"als-file-path:"	+ li.mALSGenerator.mFileName + vbCrLf +_
								"Файл сгенерирован c помощью: intsOLEGenerator.vbs" + vbCrLf +_
								"openconf рулит!" + vbCrLf +_
								"Автор: Трошин Д. ака trdm, 2005 г. trdm©fromru.com"	 + vbCrLf +_
								"===================================================="

		li.mALSGenerator.Dump
		message "######################################################################",mExclamation3
		message "Синтакс-Помощник: вызвать динамическое меню, выбрать пункт ""Обновить""", mExclamation3
	ElseIF li.ErrorNumber<>0 Then
		message "Ошибка генерации файлов по прог-иду: " + li.ThisProgID, mExclamation
	End IF
End Function

' Генерирует *.ints-файлы по произвольному прог-иду
Sub Generator()
	ProgID = ""
	ProgID = InputBox ("Введите ProgID", "OLE-ActiveX *.ints Generator",ProgID)
	ProgID = Trim(ProgID)
	If Len(ProgID)>0 Then
		if IntsGenerator( ProgID ) Then
			on Error resume next
			Scripts("Intellisence").ReloadDictionary
			if err.number<>0 Then
				message "В Intellisence не найдена процедура ReloadDictionary!!!", mExclamation3
				message "Подгрузка дополнительных словарей невозможна!", mExclamation3
			End If
			on Error goto 0
		End If
	End IF
End Sub

' В случае возникновения ситуации когда библиотека содержащая объект,
' соотвестствующий прог-иду, не была зарегистрирована на компьютере,
' а по данному объекту уже была произведена неудачная попытка генерации
' *.ints-файлов, объект запоминался в файле ProgIDDumped.txt и все попытки
' генерации по нему пресекались в зародыше.
'
' Данная процедура позволяет произвести регенерацию файлов по данному объекту.
' Для регенерации его надо выбрать из предложенного списка.
Sub ReGenerator()
	If FSO.FileExists(FileNameProgIDDumped) Then
		Set File0 = fso.GetFile( FileNameProgIDDumped )
		Set File = File0.OpenAsTextStream(1)
		if File0.Size > 0 Then
			TextStream = File.ReadAll()
			Set ch = CreateObject("svcsvc.service")
			needProgID = ch.SelectValue(TextStream,"Выберите объект регенерации")
			File.Close
			if Len(needProgID)>0 Then
				fso.DeleteFile  FileNameProgIDDumped, true
				Set File = fso.CreateTextFile(FileNameProgIDDumped)

				AllProgID = Split(TextStream, vbCrLf)
				for i=0 To UBound (AllProgID)
					if needProgID<>AllProgID(i) And Len(Trim(AllProgID(i)))>0 Then
						File.WriteLine AllProgID(i)
					End If
				Next
				File.Close
				if IntsGenerator( needProgID ) Then
					on Error resume next
					Scripts("Intellisence").ReloadDictionary
					if err.number<>0 Then
						message "В Intellisence не найдена процедура ReloadDictionary!!!", mExclamation3
						message "Подгрузка дополнительных словарей невозможна!", mExclamation3
					End If
					on Error goto 0
				End If

			End If
		Else
			File.Close
			Message "Файл: " + FileNameProgIDDumped + " пуст!", mExclamation
			Exit Sub
		End IF
	Else
		Message "Файл: " + FileNameProgIDDumped + " не существует!", mExclamation
	End IF
End Sub


' Нам надо отдампить что:
'1. ints-файлы с именем интерфейса
'2. Для TypesStructureExt из Intellisence файл для вытяжки следующих данных
'	TypesStructureExt.Add "excel_application.range", "VALUE|excel_applicationrange"
'	TypesStructureExt.Add "excel_applicationrange.cells", "VALUE|excel_applicationcells"
'3 для OtherTypesDefine из из Intellisence файл для вытяжки следующих данных
'	OtherTypesDefine.Add "word.application", "word_application"
'4 Перечень прог-идов по которым уже сформированны данные или тех по которым нельзя формировать
' такую информацию, из-за того, что объект, созданный по прог-иду может быть опасен.

Sub CommonGenerator( )
	IntsGenerator( "Scripting.FileSystemObject" )
	IntsGenerator( "ADODB.Connection" )
	IntsGenerator( "WScript.Shell" )
	IntsGenerator( "MSXML2.DOMDocument" )

Exit sub ' остальные ради прикола.......
	IntsGenerator( "Excel.Application" )
	IntsGenerator( "Word.application" )
	IntsGenerator( "SYSINFO.SysInfo" )
	IntsGenerator( "Svcsvc.Service" )
	IntsGenerator( "v77.Application" )
	IntsGenerator( "VBScript.RegExp" ) ' нету там собирающего интерфейса (((( придется ручками....
	IntsGenerator( "SHPCE.Profiler" )
	IntsGenerator( "ODBCSQL.RarusSQL" )
	IntsGenerator( "TLI.TypeLibInfo" )
	IntsGenerator( "TLI.TLIApplication" )
	IntsGenerator( "Rarus.ApiExtender" )
End Sub

Sub TestLibrary()
	On Error Resume Next
	Set TLITypeLibInfo = CreateObject("TLI.TypeLibInfo")
	Set TLIApplication = CreateObject("TLI.TLIApplication")
	if Err.number<>0 then
		Message "Библиотека: TLBINF32.DLL не зарегистрирована на данном рабочем месте!"
	Else
		Message "Библиотека: TLBINF32.DLL зарегистрирована! Все в порядке!"
	End If
	On Error Goto 0
End Sub
' Инициализация....
Sub InitScript( para )
	set WSH = CreateObject("WScript.Shell")
	Set FSO = CreateObject("Scripting.FileSystemObject")
End Sub

' Тестовая процедура, показывающая возможности генерации.....
Private Sub TestAlsCreation()
	'Stop
	Set AG = New ALSGenerator
	AG.Init DataDirAls+"aaa.als"
	Set PaFo = AG.CreateRootFolder("Папка 1-го уровня", "Синоним")
	If IsObject(PaFo) Then
		for i=0 To 10
			Set F2L= PaFo.AddSubFolder("Папка 2-го уровня","--")
			for ii = 0 To 10
				Set F3L = F2L.AddSubFolder ( "Папка 3-го уровня","---")
				Set Si3L = F2L.AddSubItem ( "Элемент 3-го уровня","---" )
				Si3L.mWriting = "фывлафывлаоылвафтмл ЛЫ ЛЫВФЫ"
				Si3L.mWritingEng = "KHDSs sdvsdm wefke"
				For iii = 0 To 10
					F3L.AddSubItem  "Элемент 4-го уровня","----"
				Next
			Next
		Next
	End If
	AG.Dump
End Sub


InitScript 0