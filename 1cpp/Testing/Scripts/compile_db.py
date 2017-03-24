import dbconvert
import config_starter
import os

dbconvert.txt2dbf()

recalc = file(r'..\..\1srecalc.cmd', 'w+')
recalc.write('AccSel    0\n')
recalc.write('AccTotals 0\n')
recalc.write('CrossDoc  0\n')
recalc.close()

config_starter.startConfig(os.path.normpath(os.getcwd() + r'\..\..'))
