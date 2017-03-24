$NAME Мастер 1С++
'      Скрипт для управления плагином "Мастер 1С++" (c) artbear,  2004
'
'         Мой e-mail: artbear@bashnet.ru
'         Мой ICQ: 265666057

Dim Wizard1CPP

Sub TestCreateExampleClass
  Wizard1CPP.ClassName = "Новый_Класс"

 Wizard1CPP.FolderPath = "C:\" ' можно работать с абсолютным путем к папке
'  Wizard1CPP.FolderPath = "ExtForms"  ' можно работать с относительным путем к папке
'  Wizard1CPP.FolderPath = "#Классы\Общие" ' можно работать с алиасом из oxy.ini

  Wizard1CPP.ClassFileName = "Файл_нового_класса.ert" ' без пробелов вводить можно
'  Wizard1CPP.ClassFileName = "Файл_нового_ класса.ert" ' должен выдать сообщение об ошибке

  Wizard1CPP.EnableConstructor = 1
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
  Wizard1CPP.BaseClasses = ""

  Wizard1CPP.CreateClass

End Sub ' TestCreateExampleClass

Sub Wizard1CPP_OnCreateClass(ModuleText)
' можно поменять код самого класса,  например:
  ModuleText = "// начало класса Wizard1C++ "+vbCRLF +ModuleText + vbCRLF +" // завершение класса Wizard1C++ !!"

'  Echo ModuleText
End Sub

Sub Wizard1CPP_OnCreateDefinitionOfClass(DefinitionText)
' можно поменять код определения класса,  например:
  DefinitionText = "// начало определения класса "+vbCRLF +DefinitionText + vbCRLF +" // завершение определения класса !!"

'  Echo "Определение класса = "+vbCrLF+DefinitionText
End Sub

Sub TestCreateExampleClassWithDebugMessages

  Wizard1CPP.EnableDebugMessages = -1 ' включить подробные отладочные сообщения

  TestCreateExampleClass

  'Wizard1CPP.EnableDebugMessages = 0 ' выключить подробные отладочные сообщения

End Sub ' TestCreateExampleClassWithDebugMessages

Sub TestHistory
  Echo "История созданных классов - " + Wizard1CPP.ClassNameHistory

  Echo "История использованных каталогов для хранения классов - " + Wizard1CPP.FolderPathHistory
  Echo "История использованных базовых классов - " + Wizard1CPP.BaseClassesHistory

End Sub ' TestHistory

Sub ClearAll
  Wizard1CPP.ClassName = ""

  Wizard1CPP.FolderPath = ""
  Wizard1CPP.ClassFileName = ""
  Wizard1CPP.EnableConstructor = 0
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
  Wizard1CPP.BaseClasses = ""
  Wizard1CPP.EnableBaseClassesVariable = 0

End Sub ' ClearAll

Sub Min
  Wizard1CPP.EnableConstructor = 0
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
End Sub 'Min

Sub Max
  Wizard1CPP.EnableConstructor = 1
  Wizard1CPP.EnableDestructor = 1
  Wizard1CPP.EnableDebug = 1
  Wizard1CPP.UsedAsVisualForm = 1
End Sub 'Max

Sub Echo(text)
  Message CStr(text), mNone
End Sub'Echo

Function GetPluginByName(PluginName)
  set GetPluginByName = Nothing
'echo "PluginName = " & PluginName
  err.Clear
'Echo CStr(err.Number)

  on error resume next
  Wizard1CPP = Plugins(CStr(PluginName))
  Err_My = err.Number
'Echo "1-" & CStr(Err_My)
'  on error goto 0
'Echo "2-" &  CStr(Err_My)
'Echo CStr(GetPluginByName)
  if Err_My = 438 then ' такой плагин действительно существует
    set GetPluginByName = Plugins(CStr(PluginName))
'Echo "3-" & CStr(err.Number)
'Echo "4-" & CStr(GetPluginByName)
    Exit Function
  end if
End Function

'' Инициализация скрипта. param - пустой параметр,
'' чтобы не попадал в макросы
''
'Sub Init(param)
'' set Wizard1CPP = GetPluginByName("Мастер 1С++")
' set Wizard1CPP = Plugins("Мастер 1С++")
' if not Wizard1CPP Is Nothing Then
'     ' Привязываем скрипт к событиям плагина
'		SelfScript.AddNamedItem "Wizard1CPP", Wizard1CPP, False
'		Wizard1CPP.EnableDebugMessages = 0 ' выключить подробные отладочные сообщения
'  else
'      ' Плагин не загружен. Выгрузим и скрипт
'      Scripts.Unload SelfScript.Name
'  End If
'End Sub


' Инициализация скрипта. param - пустой параметр,
' чтобы не попадал в макросы
'
'Sub Init(Script, ScriptName, PrefixForEvents, ByRef ScriptVariable)
Sub Init(Script, ScriptName, PrefixForEvents, ScriptVariable)
  set ScriptVariable = GetPluginByName(CStr(ScriptName))
  if not ScriptVariable Is Nothing Then
      ' Привязываем скрипт к событиям плагина
      Script.AddNamedItem PrefixForEvents, ScriptVariable, False
  else
      ' Плагин не загружен. Выгрузим и скрипт
      Scripts.Unload Script.Name
  End If
End Sub

' При загрузке скрипта инициализируем его
Init SelfScript, "Мастер 1С++", "Wizard1CPP", Wizard1CPP
on error resume next
Wizard1CPP.EnableDebugMessages = 0 ' выключить подробные отладочные сообщения

'' При загрузке скрипта инициализируем его
'Init 0