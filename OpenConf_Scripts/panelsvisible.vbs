' Пример скрипта, демонстрирующий добавленное свойство PanelVisible
' Данные макросы переключают видимость различных панелей.
' В качестве имени панели используется ее заголовок.
' Также можно менять видимость обычных панелей инструментов

Sub TogglePanel(PanelName)
	Windows.PanelVisible(PanelName)=Not Windows.PanelVisible(PanelName)
End Sub

Sub ToggleSyntaxHelper()
	TogglePanel "Синтакс-Помощник"
End Sub

Sub ToggleOutPutWindow()
	TogglePanel "Окно сообщений"
End Sub

Sub TogleSearchWindow()
	TogglePanel "Список найденных вхождений"
End Sub

Sub TogleStdToolbar()
	TogglePanel "Стандартная"
End Sub
