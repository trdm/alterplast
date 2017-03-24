Dim glSavedClass

Sub TestFlagGroupByType()
'	message Visual1Cpp.FlagGroupByType
'	Visual1Cpp.FlagGroupByType = "new value"
'	message Visual1Cpp.FlagGroupByType

	message "FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
End Sub

Sub TestEnableFlagGroupByType()
	message "до FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
	Visual1Cpp.FlagGroupByType = 1
	message "после FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
End Sub

Sub TestDisableFlagGroupByType()
	message "до FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
	Visual1Cpp.FlagGroupByType = 0
	message "после FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
End Sub

Sub TestSetFlagGroupByTypeFromErrorValue()
	message "до FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
	Visual1Cpp.FlagGroupByType = 10
	message "после FlagGroupByType = " & CStr(Visual1Cpp.FlagGroupByType)
End Sub

Sub TestMethod1()
	Visual1Cpp.Method1()
End Sub

Sub Visual1Cpp_OnRefreshAll()
	message "Visual1Cpp_OnRefreshAll"
End Sub

Sub TestRefreshAll()
	Visual1Cpp.RefreshAll
End Sub

Sub TestGetClasses()
	Set Classes = Visual1Cpp.GetClasses
	Set Classes = Visual1Cpp.GetClasses
	message "TypeName(Classes) = " & CStr(TypeName(Classes))
	message "Classes.Count = " & CStr(Classes.Count)
	Set Class1 = Classes.Item(0)
	Set Class1 = Classes.Item(0)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))
	'Exit Sub
	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)

	Set Class1 = Classes.Item(Classes.Count-1)
	Set Class1 = Classes.Item(Classes.Count-1)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))
	'Exit Sub
	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)

	on error resume next
	Set Class2 = CreateObject("Vis1cpp.CoClassInfo")
	message "TypeName(Class2) = " & CStr(TypeName(Class2))
	'Exit Sub
	message "Class2.Name = " & CStr(Class2.Name)
	message "Class2.Path = " & CStr(Class2.Path)

	Set Classes1 = CreateObject("Vis1cpp.CoClasses")
	message "TypeName(Classes1) = " & CStr(TypeName(Classes1))

	on Error goto 0
	for i = 0 to Classes.Count-1
		Set Class1 = Classes.Item(i)
		message "Class1.Name = " & CStr(Class1.Name)
	next
End Sub

Sub TestGetLastClass()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	Set Class1 = Classes.Item(Classes.Count-1)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)
	message "Class1.DefFilePath = " & CStr(Class1.DefFilePath)

	Set DefFiles = Visual1Cpp.GetDefFiles
	message "TypeName(DefFiles) = " & CStr(TypeName(DefFiles))
	message "DefFiles.Count = " & CStr(DefFiles.Count)

	Set DefFile = Visual1Cpp.GetDefFiles(Class1.DefFilePath)
	message "TypeName(DefFile) = " & CStr(TypeName(DefFile))

	message "DefFile.Name = " & CStr(DefFile.Name)
	message "DefFile.Path = " & CStr(DefFile.Path)
End Sub

Sub TestSavedClassAfterRefreshAll()
	Set Classes = Visual1Cpp.GetClasses
	'message "TypeName(Classes) = " & CStr(TypeName(Classes))
	message "Classes.Count = " & CStr(Classes.Count)

	Set SavedClass = Classes.Item(Classes.Count-1)
	message "до TypeName(SavedClass) = " & CStr(TypeName(SavedClass))
	message "до SavedClass.Name = " & CStr(SavedClass.Name)
	'message "до SavedClass.Path = " & CStr(SavedClass.Path)

	Visual1Cpp.RefreshAll

	message "после TypeName(SavedClass) = " & CStr(TypeName(SavedClass))
	message "после SavedClass.Name = " & CStr(SavedClass.Name)
	'message "после SavedClass.Path = " & CStr(SavedClass.Path)

End Sub

Sub TestSaveSavedClassBeforeRefreshAll()
	Set Classes = Visual1Cpp.GetClasses
	'message "TypeName(Classes) = " & CStr(TypeName(Classes))
	message "Classes.Count = " & CStr(Classes.Count)

	Set glSavedClass = Classes.Item(0)
	message "до TypeName(glSavedClass) = " & CStr(TypeName(glSavedClass))
	message "до glSavedClass.Name = " & CStr(glSavedClass.Name)
	'message "до glSavedClass.Path = " & CStr(glSavedClass.Path)

	message "теперь удалите первый класс в иерархии классов и вызовите макрос TestSaveSavedClassAfterRefreshAll"
End Sub

Sub TestSaveSavedClassAfterRefreshAll()
	Visual1Cpp.RefreshAll

	message "после TypeName(glSavedClass) = " & CStr(TypeName(glSavedClass))
	message "после glSavedClass.Name = " & CStr(glSavedClass.Name)
	'message "после glSavedClass.Path = " & CStr(glSavedClass.Path)

End Sub

Sub TestOpenClass()
	Set Classes = Visual1Cpp.GetClasses

	Set Class1 = Classes.Item(Classes.Count-1)
	'message "Class1.Name = " & CStr(Class1.Name)
	Class1.Open
End Sub

Sub TestGetClassByName()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	Set Class1 = Classes.Item(0)
	ClassName = Class1.Name
	message "поиск класса " & CStr(Class1.Name)

	Set Class1 = Classes.Item(ClassName)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)

	Set Class1 = Classes(ClassName)
	message "TypeName(Classes(ClassName)) = " & CStr(TypeName(Class1))

	message "Classes(ClassName).Name = " & CStr(Class1.Name)
	message "Classes(ClassName).Path = " & CStr(Class1.Path)

	Set Class1 = Classes(0)
	message "TypeName(Classes(0)) = " & CStr(TypeName(Class1))

	message "Classes(0).Name = " & CStr(Class1.Name)
	message "Classes(0).Path = " & CStr(Class1.Path)

	message "Classes(0).Name = " & CStr(Classes(0).Name)
End Sub

Sub TestGetClassByNameVerifyCase()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	Set Class1 = Classes.Item(0)
	ClassName = Class1.Name
	message "поиск класса " & CStr(Class1.Name)

	Set Class1 = Classes.Item(ClassName)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)

	ClassNameUpper = UCase(ClassName)
	Set ClassUpper = Classes(ClassNameUpper)
	message "TypeName(ClassUpper) = " & CStr(TypeName(ClassUpper))

	message "ClassUpper.Name = " & CStr(ClassUpper.Name)
	message "ClassUpper.Path = " & CStr(ClassUpper.Path)

	ClassNameLower = LCase(ClassName)
	Set ClassLower = Classes(ClassNameLower)
	message "TypeName(ClassLower) = " & CStr(TypeName(ClassLower))

	message "ClassLower.Name = " & CStr(ClassLower.Name)
	message "ClassLower.Path = " & CStr(ClassLower.Path)
End Sub

Sub TestEnumClasses()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	for each Class1 in Classes
		message "TypeName(Class1) = " & CStr(TypeName(Class1))

		message "Class1.Name = " & CStr(Class1.Name)
		message "Class1.Path = " & CStr(Class1.Path)
		set Class1 = Nothing
	next
End Sub

Sub TestClassesFindByPath()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	Set Class1 = Classes.Item(Classes.Count-1)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)
	message "Class1.DefFilePath = " & CStr(Class1.DefFilePath)

	'Set DefFiles = Visual1Cpp.GetDefFiles
	Set Class2 = Classes.FindByPath(Class1.Path)
	message "Class2.Name = " & CStr(Class2.Name)
	message "Class2.Path = " & CStr(Class2.Path)
	message "Class2.DefFilePath = " & CStr(Class2.DefFilePath)

	Set Class3 = Classes.FindByPath("W:\1C\Testing 1C++\UnitTests\DebugTest.ert")
	message "Class3.Name = " & CStr(Class3.Name)
	message "Class3.Path = " & CStr(Class3.Path)
	message "Class3.DefFilePath = " & CStr(Class3.DefFilePath)
	'if Class1 <> Class2 then
	'	message "Ошибка равенства после поиска"
	'end if
End Sub

Sub TestClassInfoSourceText()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	Set Class1 = Classes.Item(Classes.Count-1)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)
	message "Class1.DefFilePath = " & CStr(Class1.DefFilePath)
	message "Class1.SourceText = <" & CStr(Class1.SourceText) & ">"
End Sub

Sub TestClassInfoBaseClasses()
	Set Classes = Visual1Cpp.GetClasses
	message "Classes.Count = " & CStr(Classes.Count)

	for each Class1 in Classes
		Set BaseClasses = Class1.BaseClasses
		if BaseClasses.Count <> 0 then

		message "TypeName(Class1) = " & CStr(TypeName(Class1))

		message "Class1.Name = " & CStr(Class1.Name)
		message "Class1.Path = " & CStr(Class1.Path)
		message "Class1.DefFilePath = " & CStr(Class1.DefFilePath)

		message "BaseClasses.Count = " & CStr(BaseClasses.Count)

		Set BaseClass1 = BaseClasses.Item(BaseClasses.Count-1)
		message "TypeName(BaseClass1) = " & CStr(TypeName(BaseClass1))
		set BaseClasses = Nothing
		set Class1 = Nothing
		set Classes = Nothing

		message "BaseClass1.Name = " & CStr(BaseClass1.Name)
		message "BaseClass1.Path = " & CStr(BaseClass1.Path)
		message "BaseClass1.DefFilePath = " & CStr(BaseClass1.DefFilePath)
		exit Sub
		end if
	next

End Sub

Sub TestGetDefFiles()
	Set DefFiles = Visual1Cpp.GetDefFiles
	Set DefFiles = Visual1Cpp.GetDefFiles
	message "TypeName(DefFiles) = " & CStr(TypeName(DefFiles))
	message "DefFiles.Count = " & CStr(DefFiles.Count)
	Set DefFile1 = DefFiles.Item(0)
	Set DefFile1 = DefFiles.Item(0)
	message "TypeName(DefFile1) = " & CStr(TypeName(DefFiles1))
	'Exit Sub
	message "DefFile1.Name = " & CStr(DefFile1.Name)
	message "DefFile1.Path = " & CStr(DefFile1.Path)
End Sub

Sub TestGetDefFiles2()
	Set DefFiles = Visual1Cpp.GetDefFiles
	Set DefFiles = Visual1Cpp.GetDefFiles
	message "TypeName(DefFiles) = " & CStr(TypeName(DefFiles))
	message "DefFiles.Count = " & CStr(DefFiles.Count)
	Set DefFile1 = DefFiles.Item(0)
	Set DefFile1 = DefFiles.Item(0)
	message "TypeName(DefFile1) = " & CStr(TypeName(DefFiles1))
	'Exit Sub
	message "DefFile1.Name = " & CStr(DefFile1.Name)
	message "DefFile1.Path = " & CStr(DefFile1.Path)

	Set DefFile1 = DefFiles(0)
	Set DefFile1 = DefFiles(0)
	message "TypeName(DefFile1) = " & CStr(TypeName(DefFiles1))
	'Exit Sub
	message "DefFile1.Name = " & CStr(DefFile1.Name)
	message "DefFile1.Path = " & CStr(DefFile1.Path)

	Set DefFile1 = DefFiles.Item(DefFiles.Count-1)
	Set DefFile1 = DefFiles.Item(DefFiles.Count-1)
	message "TypeName(DefFile1) = " & CStr(TypeName(DefFile1))
	'Exit Sub
	message "DefFile1.Name = " & CStr(DefFile1.Name)
	message "DefFile1.Path = " & CStr(DefFile1.Path)

	for i = 0 to DefFiles.Count-1
		message "index = " & CStr(i)
		Set DefFile1 = DefFiles(i)
		message "for DefFile1.Name = " & CStr(DefFile1.Name)
	next
	for each DefFile1 in DefFiles
		message "for each DefFile1.Name = " & CStr(DefFile1.Name)
	next

	on error resume next
	Set DefFile2 = CreateObject("Vis1cpp.CoDefFile")
	message "TypeName(DefFile2) = " & CStr(TypeName(DefFile2))
	'Exit Sub
	message "DefFile2.Name = " & CStr(DefFile2.Name)
	message "DefFile2.Path = " & CStr(DefFile2.Path)

	Set DefFiles1 = CreateObject("Vis1cpp.CoDefFiles")
	message "TypeName(DefFiles1) = " & CStr(TypeName(DefFiles1))

	on Error goto 0
End Sub

Sub TestGetDefFilesIntoFor()
	Set DefFiles = Visual1Cpp.GetDefFiles
	for i = 0 to DefFiles.Count-1
		message "index = " & CStr(i)
		Set DefFile1 = DefFiles(i)
		message "for DefFile1.Name = " & CStr(DefFile1.Name)
	next
	for each DefFile1 in DefFiles
		message "for each DefFile1.Name = " & CStr(DefFile1.Name)
	next
End Sub

Sub TestDefFileOpen()
	Set DefFiles = Visual1Cpp.GetDefFiles

	Set DefFile1 = DefFiles(0)
	message "for DefFile1.Name = " & CStr(DefFile1.Name)
	message "for DefFile1.Path = " & CStr(DefFile1.Path)
	DefFile1.Open
End Sub

Sub TestDefFileGetClasses()
	Set DefFiles = Visual1Cpp.GetDefFiles
	message "DefFiles.Count = " & CStr(DefFiles.Count)
	Set DefFile1 = DefFiles.Item(0)
	message "TypeName(DefFile1) = " & CStr(TypeName(DefFiles1))
	message "DefFile1.Name = " & CStr(DefFile1.Name)
	message "DefFile1.Path = " & CStr(DefFile1.Path)

	Set Classes = DefFile1.Classes
	message "TypeName(Classes) = " & CStr(TypeName(Classes))
	message "Classes.Count = " & CStr(Classes.Count)
	Set Class1 = Classes.Item(0)
	message "TypeName(Class1) = " & CStr(TypeName(Class1))

	message "Class1.Name = " & CStr(Class1.Name)
	message "Class1.Path = " & CStr(Class1.Path)
	message "Class1.DefFilePath = " & CStr(Class1.DefFilePath)
End Sub

Sub Configurator_AllPluginsInit()
'	message "Configurator_AllPluginsInit"
End Sub


set p = Plugins("Visual1Cpp")
    If Not p Is Nothing Then
			SelfScript.AddNamedItem "Visual1Cpp", p, False
			'execute("sub Visual1Cpp_OnMethod1() "& vbCrLF &" Visual1Cpp_OnMethod1 "& vbCrLF &" End Sub")
		else
			message "Неудача загрузки скрипта"
		end if
