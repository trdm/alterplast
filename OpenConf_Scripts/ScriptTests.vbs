Sub TestScriptsMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
        iScriptCount=Scripts.Count-1
        for i=0 to iScriptCount
                Message Scripts.Name(i),mMetaData
                Set script=Scripts(i)
                arr=e.EnumMacros(script)                        ' Получение массива макросов объекта
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' Вызов макроса: объект, имя макроса
                Next
        Next
End Sub

Sub TestPluginsMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
        iScriptCount=Plugins.Count-1
        for i=0 to iScriptCount
                Message Plugins.Name(i),mMetaData
                Set script=Plugins.Item(i)
                arr=e.EnumMacros(script)                        ' Получение массива макросов объекта
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' Вызов макроса: объект, имя макроса
                Next
        Next
End Sub

' -----------------------------------------------------------------------------
' получить плагин по названию
' если плагин отсутствует, возвращается Nothing
'
' почему-то конструкция
'          set GetPlugin = Plugins(PluginName)
' не хочет работать,  если PluginName - обычная строковая переменная
' или плагина под таким названием не существует
'
Function GetPluginByName(PluginName)
	set GetPluginByName = Nothing

	on error resume next
	temp = Plugins(PluginName)
	on error goto 0
	if err.Number = 438 then ' такой плагин действительно существует
		set GetPluginByName = Plugins(PluginName)
		Exit Function
	end if
End Function ' GetPluginByName

Sub TestWizard1CPPMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
'        iScriptCount = 0
'        for i=0 to iScriptCount
'                Message Plugins.Name(i),mMetaData
                Set script=GetPluginByName("Мастер 1С++")


                arr=e.EnumMacros(script)                        ' Получение массива макросов объекта
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' Вызов макроса: объект, имя макроса
                Next
'        Next
End Sub