@echo off

for %%i in (*.rst) do call txt2chm.bat %%i

call kb2chm_all.bat
