
cvs -z9 -q update -P -d -C

rem Ночная сборка html документации

cd Documents
call site_html.bat

cp -u site/*.* ../../1cpp_web/docum/icpp/html
cd ..

cd ..\1cpp_web
cvs -z9 -q commit -m "Обновил ночную сборку"

