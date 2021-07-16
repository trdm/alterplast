
ServerCOMps.dll: dlldata.obj ServerCOM_p.obj ServerCOM_i.obj
	link /dll /out:ServerCOMps.dll /def:ServerCOMps.def /entry:DllMain dlldata.obj ServerCOM_p.obj ServerCOM_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ServerCOMps.dll
	@del ServerCOMps.lib
	@del ServerCOMps.exp
	@del dlldata.obj
	@del ServerCOM_p.obj
	@del ServerCOM_i.obj
