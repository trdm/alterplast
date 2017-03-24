#coding: Windows-1251
from os import *
from gcompLib import *

def decompileProjectItem(fileName):
    decompileItem(releaseFName(fileName), destFName(fileName))

def releaseFName(fileName):
    return path.join(ReleaseFolder, fileName)

def destFName(fileName):
    return path.join(DestFolder, fileName)

def decompileProject():
    decompileProjectItem("ExtForms")
    decompileProjectItem("UnitTests")

#Путь, где находится собранный проект относительно папки Scripts
ReleaseFolder= '../..'
ReleaseFolder= path.abspath(path.join(ScriptPath, ReleaseFolder))
DestFolder   = '..'
DestFolder   = path.abspath(path.join(ScriptPath, DestFolder))

decompileProject()
