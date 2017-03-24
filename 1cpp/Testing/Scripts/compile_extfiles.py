#coding: Windows-1251
from os import *
from gcompLib import *

def compileProjectItem(fileName):
    compileItem(destFName(fileName), releaseFName(fileName))

def releaseFName(fileName):
    return path.join(ReleaseFolder, fileName)

def destFName(fileName):
    return path.join(DestFolder, fileName)

def compileProject():
    compileProjectItem('ExtForms')
    compileProjectItem('UnitTests')

#Пути для сборки проекта относительно папки Scripts
ReleaseFolder='../..'
ReleaseFolder=path.abspath(path.join(ScriptPath, ReleaseFolder))
DestFolder = '..'
DestFolder =path.abspath(path.join(ScriptPath, DestFolder))

compileProject()
