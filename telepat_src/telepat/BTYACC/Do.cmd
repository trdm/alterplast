del %1.cpp
del %1.h
del %1.output
btyacc -v %1
yccconv %1
del y_tab.c
ren y.output %1.output
exit
