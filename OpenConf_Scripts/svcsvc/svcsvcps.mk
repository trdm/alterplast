
svcsvcps.dll: dlldata.obj svcsvc_p.obj svcsvc_i.obj
	link /dll /out:svcsvcps.dll /def:svcsvcps.def /entry:DllMain dlldata.obj svcsvc_p.obj svcsvc_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del svcsvcps.dll
	@del svcsvcps.lib
	@del svcsvcps.exp
	@del dlldata.obj
	@del svcsvc_p.obj
	@del svcsvc_i.obj
