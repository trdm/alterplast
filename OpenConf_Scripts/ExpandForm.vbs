' Скрипт ExpandForm
' Версия: $Revision: 1.1 $
' Скрипт позволяет посмотреть все контролы формы в развернутом виде.
' Автор trdm 2005.07

Set fso = CreateObject("Scripting.FileSystemObject")
dim objIE
dim HtmlFile
dim HtmlFileName
HtmlFileName = BinDir + "Config\form.htm"
HtmlFile = NULL
Dim dcPropMy
set dcPropMy = CreateObject("Scripting.Dictionary")
dcPropMy.Add cpStrID , "Идентификатор"
dcPropMy.Add cpFormul , "Формула"
dcPropMy.Add cpTitle , "Заголовок"
dcPropMy.Add cpHelpString , "Описание"
dcPropMy.Add cpToolTip , "Подсказка"
dcPropMy.Add cpHotKey , "Хоткей"
dcPropMy.Add cpLayer , "Слой"
dcPropMy.Add cpLinkWith , "Связан с"
dcPropMy.Add cpMetaID , "ID метаданных реквизита"
dcPropMy.Add cpPictMode , "Положение картинки"
dcPropMy.Add cpLeft , "Левая координата"
dcPropMy.Add cpTop , "Правая координата"
dcPropMy.Add cpHeight , "Высота"
dcPropMy.Add cpWidth , "Ширина"
dcPropMy.Add cpFontColor , "Цвет шрифта"
dcPropMy.Add cpFontItalic , "Шрифт наклонный"
dcPropMy.Add cpFontName , "Имя шрифта"
dcPropMy.Add cpFontSize , "Размер шрифта"
dcPropMy.Add cpFontUnderline, "Шрифт подчернутый"
dcPropMy.Add cpFontWeight , "Толщина шрифта"
dcPropMy.Add cpFlags , "Основной набор флагов"
dcPropMy.Add cpFlagsEx , "Дополнительный набор флагов"
private function HtmlFileCreate()
  Set HtmlFile = fso.CreateTextFile(HtmlFileName)
  HtmlFileAddCaption()
end function
private function HtmlFileAddCaption()
  HtmlFile.WriteLine("<HTML>")
  HtmlFile.WriteLine("<HEAD>")
  HtmlFile.WriteLine("<TITLE></TITLE>")
  HtmlFile.WriteLine("<META HTTP-EQUIV=""Content-Type"" content=""text/html; charset=windows-1251"">")
  HtmlFile.WriteLine("</HEAD>")
  HtmlFile.WriteLine("<BODY bgcolor=silver>")
end function
private function HtmlFileAddFutter()
  HtmlFile.WriteLine("</BODY>")
  HtmlFile.WriteLine("</HTML>")
end function
private function GetColorForCtrl( TypeCtrl )
  GetColorForCtrl = "#ffffcc"
  'exit function
  select case (TypeCtrl)
    case "Текст" GetColorForCtrl = "yellowgreen"
    case "Реквизит" GetColorForCtrl = "lightgreen"
    case "ПолеСписок" GetColorForCtrl = "mediumpurple"
    case "Кнопка" GetColorForCtrl = "silver"
    case "ТабличнаяЧасть" GetColorForCtrl = "plum"
    case "Рамка" GetColorForCtrl = "wheat"
    case "Флажок" GetColorForCtrl = "yellow"
    case else GetColorForCtrl = "#f5f5f5"
  End Select
end function
sub ShowExpandForm()
  DocWB = ""
  if not CheckWorkBook(doc) Then Exit sub
  HtmlFile = NULL
  HtmlFileCreate()
  set Form = Windows.ActiveWnd.Document.Page(0)
  Dim TableTop
  Dim TableFut

  TableTop = "<table border=0 cellpadding=1 cellspacing=1 cols="""" width="""" height="""" align=""left right"" bgcolor=#000033 bordercolor="""" bordercolorlight=black bordercolordark="""" background="""" title="""">"
  HtmlFile.WriteLine(TableTop)
  strToFile = "<tr bgcolor=#ffcc66 align=center valign=middle>"
  HtmlFile.WriteLine(strToFile)

  strToFile = "<td>Тип элемента</td>" + vbCrLf
  HtmlFile.WriteLine(strToFile)
  strToFile = ""
  Arr = dcPropMy.Items
  For i=0 To dcPropMy.Count-1
    if Arr(i) = "Описание" Then
      strToFile = strToFile + "<td>" + Arr(i) + "</td>" + vbCrLf
    Else
      strToFile = strToFile + "<td>" + Arr(i) + "</td>" + vbCrLf
    End IF
  next
  strToFile = strToFile + "</tr>"
  HtmlFile.WriteLine(strToFile)

  Keys = dcPropMy.Keys
  for i = 0 to Form.ctrlCount - 1
    strToFile = "<tr>"
    strToFile = " <tr bgcolor= " + GetColorForCtrl(Form.ctrlType(i)) + ">" ' #f5f5f5
    strToFile = strToFile + "<td>" + Form.ctrlType(i) + "</td>"
    For ii=0 To dcPropMy.Count-1
      on error resume next
      str = CStr(Form.ctrlProp(i,Keys(ii)))
      if err.number<>0 then str = "&nbsp;"
      on error goto 0
      if len(str) = 0 Then str = "&nbsp;"
      if cpHelpString = Keys(ii) Then
        strToFile = strToFile + "<td><NoBR>" + Trim(str) + "</NoBR></td>" + vbCrLf
      else
        strToFile = strToFile + "<td><NoBR>" + str + "</NoBR></td>" + vbCrLf
      End If
    next
    strToFile = strToFile + "</tr>"
    HtmlFile.WriteLine(strToFile)
  Next
  TableFut = "</table>"
  HtmlFile.WriteLine(TableFut)
  HtmlFileAddFutter()
  HtmlFile.Close
  set objIE = CreateObject("InternetExplorer.Application")
  objIE.Navigate("about:blank")
  While objIE.Busy=-1
  Wend
  Set f = fso.OpenTextFile(HtmlFileName,1)
  tText = f.ReadAll
  objIE.Document.Write(tText)
  objIE.Visible = 1
End Sub
'=============================================================================================
' Проверяет активное окно на принадлежность к WorkBook и возвращает
' Windows.ActiveWnd.Document.Page(0)
Function CheckWorkBook(doc)
  CheckWorkBook = False
  If Windows.ActiveWnd Is Nothing Then
     Exit Function
  End If
  Set doc = Windows.ActiveWnd.Document
  If doc<>docWorkBook Then Exit Function
  Set doc=doc.Page(0)
  CheckWorkBook = True
End Function