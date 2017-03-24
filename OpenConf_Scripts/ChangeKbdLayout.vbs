'активизация нужной раскладки клавиатуры (рус\лат)
Set Wrapper = CreateObject("DynamicWrapper")
Wrapper.Register "USER32.DLL", "ActivateKeyboardLayout", "I=ll", "f=s", "r=l" 
Wrapper.Register "USER32.DLL", "LoadKeyboardLayoutA",    "I=sl", "f=s", "r=l" 
rus = Wrapper.LoadKeyboardLayoutA ("00000419",0)
eng = Wrapper.LoadKeyboardLayoutA ("00000409",0)

'=======================================================================================
Sub ActivateRusLayout()
	Wrapper.ActivateKeyboardLayout rus,0
End Sub

'=======================================================================================
Sub ActivateEngLayout()
	 Wrapper.ActivateKeyboardLayout eng,0
End Sub