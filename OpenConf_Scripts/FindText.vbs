$NAME Поиск текста в модуле регулярными выражениями
'----------------------------------------------------------------
' Автор: Ушаков Сергей (Phoenix)
' Email: PhoenixUSA#yandex(dot)ru
'
' поиск текста в текущем модуле
' с установкой закладок
' используются регулярные выражения
'
' добавления функционала: Артур Аюханов aka artbear
' Email: artbear@bashnet.ru
'
' на всякий случай: 
' шаблон для поиска выражения, которое не встречается в комментариях 1C 
' ^/?([^/]/?)*Выражение
' или 
' ^(?!.*//).*Выражение
' Примечание: за уточнение спасибо mikeA и Номер 6 c Т1С
'
' шаблон для поиска выражения, которое, напротив, стоит в комментариях 1C 
' //+.*Выражение
' 
' TODO:
'     1) можно весь массив поиска сохранять и по отдельному макросу выводить окно выбора
'     2) также можно сохранять массив выражений поиска. 
'         и вместо ИнпутБокс использовать также ФильтерВалуе с выбором или ручным вводом нового выражения (?пока нет такой фичи)
'
'======= Инициализация =============================
Set RE = New RegExp

PrevRegPattern = "" ' кэш последнего поиска      
' список последнего поиска
Dim RegPatternCollection  ' "OpenConf.Collection"

'
' Процедура инициализации скрипта
'
Sub Init(dummy) ' Фиктивный параметр, чтобы процедура не попадала в макросы
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
	
    On Error Resume Next
	set RegPatternCollection = CreateObject("OpenConf.Collection")
    On Error GoTo 0
    If RegPatternCollection Is Nothing Then
        Message "Не могу создать объект OpenConf.Collection", mRedErr
        Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
	RegPatternCollection.Add("^/?([^/]/?)*ВыражениеТолькоНеКомментариях")
	RegPatternCollection.Add("//+.*ВыражениеТолькоВКомментариях")
End Sub
 
Init 0 ' При загрузке скрипта выполняем инициализацию

'----------------------------------------------------------------
sub GoFindText(DoBookmark)
  Dim strDoc, fStr
  
  Set Doc = CommonScripts.GetTextDocIfOpened(0)
  If Doc Is Nothing Then Exit Sub

  if DoBookmark <> -1 then ' если не удаляем закладки 
    if RegPatternCollection.Size > 0 then 
      PrevRegPattern = RegPatternCollection.Item(RegPatternCollection.Size-1)
    end if
  end if

  vDefaultWord = doc.CurrentWord
  vDefaultWord = Trim(vDefaultWord)
  if DoBookmark <> -1 then ' если не удаляем закладки 
  	if vDefaultWord <> "" then
	  RegPatternCollection.Add(vDefaultWord)
    end if

	arrForSelect = RegPatternCollection.SaveToString(vbCrLf)

	fStr = CommonScripts.SelectValue(arrForSelect, "Введите значение для поиска", "", true, true)
	if fStr <> vDefaultWord then
      if vDefaultWord <> "" then
		RegPatternCollection.Remove(vDefaultWord)
      end if
	end if
	
  else
	fStr = PrevRegPattern
  end if
  if fStr = "" then
  	Exit Sub
  end if

    if DoBookmark = 1 then
	  if doc.NextBookmark(0) > 0 then
	  	'if fStr <> PrevRegPattern then ' TODO ? если поиск повторяется, вопрос про удаление закладок не нужен
          if MsgBox("Убрать закладки предыдущего поиска?",vbYesNo,"Поиск") = vbYes then
            'doc.ClearAllBookMark
		    ClearAllPreviousFindBookmarks() ' возможны небольшие тормоза на глобальных модулях
          end if
        'end if
      end if
    end if

   RegPatternCollection.Add(fStr)

  	
    RE.Pattern = fStr
    RE.IgnoreCase = true

	vals = ""
	
    For i = 0 To doc.LineCount - 1
      strDoc = doc.Range(i, 0)
      if RE.Test(strDoc) then
        strDoc = Trim(strDoc)
        vals = vals & vbCrLf & "(" & CStr(i+1) & ") " & Replace(strDoc,vbTab,"")

        if DoBookmark = 1 then
          doc.Bookmark(i) = true
        end if
        if DoBookmark = -1 then
          doc.Bookmark(i) = false
        end if
      End If
    Next
    if DoBookmark = -1 then
	  exit sub
    end if
    
	if Trim(Vals) = "" then
	  exit sub 
	end if
	' если найдена всего одна позиция, сразу и перехожу на нее
	arrVals = split(Vals, vbCrLf)
	if UBound(arrVals) - LBound(arrVals) = 1 then
	  fStr = arrVals(1)
	else
	  fStr = CommonScripts.SelectValue(Vals)
	end if
    if fStr <> "" then
      strDoc = Mid( fStr, InStr(fStr, "(")+1 , InStr(fStr, ")")-2)
      doc.MoveCaret CInt(strDoc)-1,0
    end if

End sub

'----------------------------------------------------------------
sub FindWithAddBookmarks()
  GoFindText(1)
end sub

'----------------------------------------------------------------
sub FindWithoutAddBookmarks()
  GoFindText(0)
end sub

'----------------------------------------------------------------
' возможность стереть все закладки, созданные предыдущим поиском
sub ClearAllPreviousFindBookmarks()
  GoFindText(-1)
end sub