'
'Файл: Templates.vbs
'Версия: $Revision: 1.1 $
'Автор: Александр Кунташов <kuntashov@yandex.ru>
'
'Расширение типового механизма шаблонов конфигуратора 1С:Предприятия.
'
'В состав расширения входят файлы:
'        
'        OpenConf_Scripts\СистемныеФайлы\Templates.wsc 
'            библиотека, реализующая работу расширения
'
'        OpenConf_Scripts\Скрипты\Редактирование\Templates.vbs
'            скрипт для OpenConf, загружающий библиотеку и обрабатывающий 
'            событие OnTemplate плагина "Телепат 2" Александра Орефкова.
'
'Полное описание скрипта см. на http://npj.ru/kuntashov/open_conf/templates
'
'Эта программа является свободным программным обеспечением. Вы можете
'распространять и (или) модифицировать ее на условиях GNU Generic Public License.
'
'Данная программа распространяется с надеждой оказаться полезной, но
'БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, в том числе без гарантий пригодности для продажи или
'каких-либо других практических целей.
'
'С полным текстом лицензии на английском языке можно ознакомитсья по адресу
'http://www.gnu.org/licenses/gpl.txt
'
'С русским переводом лицензии можно ознакомиться по адресу
'http://gnu.org.ru/gpl.html  
'

Sub Telepat_OnTemplate(Name, Text, Cancel)

    Set Template = TemplatesManager.CreateTemplate(Name, Text, true)        
    Cancel = Not TemplatesManager.ParseInteractively(Template)
    If Not Cancel Then
        Text = TemplatesManager.ParsingResult
    End If

End Sub

Private Sub CreateObjectOrDie(ProgId, Name)
    
    Set Object = Nothing    
    On Error Resume Next
    Set Object = CreateObject(ProgId)
    On Error GoTo 0
    
    If Object Is Nothing Then
        Message "Не могу создать объект """ & ProgId & """", mRedErr
        Message "Скрипт " & SelfScript.Name & " не загружен", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    
    Object.SetConfig(Configurator)
    SelfScript.AddNamedItem Name, Object, False

End Sub

Private Sub Init()

    Set t = Plugins("Телепат")
    If t Is Nothing Then
        Message "Скрипт работает только при установленном плагине Телепат!", mRedErr
        Scripts.Unload SelfScript.Name
    End If
    SelfScript.AddNamedItem "Telepat", t, False

    'CreateObjectOrDie "OpenConf.CommonServices", "CommonScripts" 
    CreateObjectOrDie "OpenConf.TemplatesManager", "TemplatesManager"

End Sub

Init
