import dbconvert
import os

os.system('decompile_cfg.bat')

dbconvert.dbf2txt()

os.system('decompile_extfiles.py')