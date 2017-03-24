' -----------------------------------------------------------------------------
'		Набор процедур/функций для работы с реестром
'		Тимошин Вячеслав aka slavka
'		05/12/2004
'		e-mail: webmastert@mail.ru
'		ICQ:
' -----------------------------------------------------------------------------
Dim Names
Dim WshShell
Set WshShell = CreateObject("WScript.Shell")
'Записать в реестр
'параметры:
'name - куда записать. Name должен начинаться с одного из корневых ключей: HKEY_CURRENT_USER - HKCU,
' HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG. напр. "HKCU\Software\1C\1cv7\7.7\Test\script name"
'Value -  значение параметра
'strType - тип параметра, возможные значения "REG_SZ", "REG_EXPAND_SZ", "REG_DWORD", "REG_BINARY" 
Sub RegWrite(name,Value,strType)
WshShell.RegWrite name,Value,strType
End Sub

'Получить значение параметра
'strName - значение параметра. strName должен начинаться с одного из корневых ключей: HKEY_CURRENT_USER - HKCU,
' HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG. напр."HKCU\Software\1C\1cv7\7.7\Test\script name"
Function RegRead(strName)
strName = WshShell.RegRead(strName)
End Function

'Удалить раздел/параметр
'strName - значение раздела/параметра. strName должен начинаться с одного из корневых ключей:
' HKEY_CURRENT_USER - HKCU, HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG.
'для удаления раздела StrName должен заканчиваться "\" (напр."HKCU\Software\1C\1cv7\7.7\Test\")
'для удаления параметра strName должен быть в виде "HKCU\Software\1C\1cv7\7.7\Test\script name"
Sub RegDelete(strName)
WshShell.RegDelete(strName)
End Sub

' -----------------------------------------------------------------------------
'Соответсвие значений корней реестра:
'HKEY_CLASSES_ROOT &H80000000
'HKEY_CURRENT_USER &H80000001
'HKEY_LOCAL_MACHINE &H80000002
'HKEY_USERS &H80000003
'HKEY_CURRENT_CONFIG &H80000005
'т.е. в нижеследующих функциях можно в параметре "rasdel" указывать (например
'если нужно обратиться к корню HKEY_CURRENT_USER) то можно указать
'или HKEY_CURRENT_USER или HKCU или &H80000001
' -----------------------------------------------------------------------------

'возвращает в переменную Names все подразделы в указанном разделе
'в виде строки с разделителем ";"
'напр.: "AddTemplate;AutoCompleteSymb;DisableTemplate;EnumMethDlg;ParamInfo;"
Function EnumSubKey(rasdel, strKeyPath, Names)
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		rasdel = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		rasdel = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		rasdel = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		rasdel = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumKey rasdel, strKeyPath, arrSubKeys
On Error Resume Next
For Each subkey In arrSubKeys
	Names =Names & subkey & ";"
Next
End Function

'возвращает в переменную Names параметры в указанном разделе
'в виде строки с разделителем ";"
'напр.: "lang;components;"
Function EnumValues(rasdel, strKeyPath, Names)
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		rasdel = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		rasdel = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		rasdel = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		rasdel = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumValues rasdel, strKeyPath, arrValueNames
On Error Resume Next
For Each ValueNames In arrValueNames
	Names =Names & ValueNames & ";"
Next
End Function

'возвращает в переменную Names параметры ,их значения и типы в указанном разделе
'в виде строки "lang=2,REG_SZ;components=3,REG_DWORD;", т.е. 
'параметру lang соответствует значение 2 тип REG_SZ, а параметру components
'соответствует значение 3 тип REG_DWORD
Function EnumParamValues(rasdel, strKeyPath, Names)
const REG_SZ = 1
const REG_EXPAND_SZ = 2
const REG_BINARY = 3
const REG_DWORD = 4
const REG_MULTI_SZ = 7	
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		ras = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		ras = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		ras = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		ras = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumValues ras, strKeyPath, arrValueNames, arrValueTypes
On Error Resume Next
For i=0 To UBound(arrValueNames)
	Names =Names & arrValueNames(i) & "="
	Select Case arrValueTypes(i)
        Case REG_SZ
            tip = "REG_SZ"
        Case REG_EXPAND_SZ
            tip = "REG_EXPAND_SZ"
        Case REG_BINARY
            tip = "REG_BINARY"
        Case REG_DWORD
            tip = "REG_DWORD"
        Case REG_MULTI_SZ
            tip = "REG_MULTI_SZ"
    End Select 
	str1 = rasdel & "\" & strKeyPath & "\" & arrValueNames(i)
	str = RegRead(str1)
	Names = Names & str1 & "," & tip & ";"
Next
End Function