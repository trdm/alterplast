rem @echo off
dir /b /s *.dsp *.dsw *.clw > temp.$$$
FOR /F %%i in (temp.$$$) do attrib -R %%i && perl -pi"*.bak" -e 's/\r/\n\r/' %%i 
del temp.$$$ 
