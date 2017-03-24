Sub ManagePRM()
	
	Set FSO = CreateObject("Scripting.FileSystemObject")
	
	If FSO.FileExists(IBDir & "turbomd.prm") Then

		
		'Резервное копирование исходного dd
		FSO.CopyFile IBDir & "turbomd.prm", IBDir & "turbomd.prm.bak", True
		FSO.DeleteFile(IBDir & "turbomd.prm")
		'MsgBox "turbomd.prm переименован в turbomd.prm.bak"
	End If 
	
End Sub
                                                                   
'После сохранения - переделка prm
'------------------------------------------------------------------
Sub Configurator_MetaDataSaved(FileNAme)
	ManagePRM()
End Sub


