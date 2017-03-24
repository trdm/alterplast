
cvs -z9 -q update -P -d -C

cd Source
gawk -f ..\nightbuild.awk AddIn.rc > AddIn.rc.new
move /y AddIn.rc.new AddIn.rc
MSDEV.EXE AddIn.dsp /MAKE "AddIn - Win32 Release MinDependency" /REBUILD > ..\compile.log
MSDEV.EXE AddIn.dsp /MAKE "Documents - Win32 HTML" /REBUILD >> ..\compile.log
MSDEV.EXE AddIn.dsp /MAKE "Documents - Win32 CHM" /REBUILD >> ..\compile.log
cd ..

rem ночная сборка компоненты
7z u -tzip nightbuild.zip Source\ReleaseMinDependency\1cpp.dll
cp -u nightbuild.zip ../1cpp_web/files

rem ночная сборка chm документации
7z u -tzip 1cpp.chm.zip Documents\chm
cp -u 1cpp.chm.zip ../1cpp_web/docum

rem Ночная сборка html документации
cd Documents
call site_html.bat
cd ..
cp -u Documents/site/*.* ../1cpp_web/docum/html

rem ночная сборка файлов для Intellisence
7z u -tzip 1cpp.intl.zip Documents\Intell
cp -u 1cpp.intl.zip ../1cpp_web/docum

cd ..\1cpp_web

cvs -z9 -q commit -m "Обновил ночную сборку"

