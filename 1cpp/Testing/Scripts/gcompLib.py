#coding: Windows-1251
from os import *
from shutil import *
import filecmp
from time import *
import sys

ScriptPath=path.dirname(sys.argv[0])
gcompFile=path.join(ScriptPath, 'gcomp.exe')

def compileERT(sourceFolder, ertName):
    cmdLine = '"' + gcompFile + '" -c -DD "' + sourceFolder + '" -F "' + ertName + '"'
    #print cmdLine
    #system('"' + cmdLine + '"')
    #Думаю, что это извращение, лучше так:
    spawnl(P_WAIT, gcompFile, ' -c -DD "' + sourceFolder + '" -F "' + ertName + '"')
    atime=0
    mtime=0
    for fname in listdir(sourceFolder):
        subFileName=path.join(sourceFolder, fname)
        if path.isdir(subFileName):
            continue
        atime=max(atime, path.getatime(subFileName))
        mtime=max(mtime, path.getmtime(subFileName))
    utime(ertName, (atime, mtime))

def decompileERT(ertName, sourceFolder):
#    print "ertName "+ertName
#    print "sourceFolder "+sourceFolder

#    system(gcompFile + ' -d -v -F "' + ertName + '" -DD "' + sourceFolder + '" --truncate-mms')
    spawnl(P_WAIT, gcompFile, ' -d -v -F "' + ertName + '" -DD "' + sourceFolder + '" --no-empty-mxl')
    atime=path.getatime(ertName)
    mtime=path.getmtime(ertName)
    for fname in listdir(sourceFolder):
        subFileName=path.join(sourceFolder, fname)
        if path.isdir(subFileName): continue
        if path.getmtime(subFileName)>mtime:
            utime(subFileName, (atime, mtime))

def copySingle(srcName, dstName):
    if path.exists(dstName):
        if filecmp.cmp(srcName, dstName, 0): return
    copy2(srcName, dstName)

def createFolder(folderName):
    if not path.exists(folderName):
        mkdir(folderName)
    if path.isfile(folderName):
        remove(folderName)
        mkdir(folderName)

def compileFolder(srcFolder, dstFolder):
    createFolder(dstFolder)
    for fname in listdir(srcFolder):
        if fname == 'CVS': continue
        if fname == '.svn': continue
        if fname == 'Scripts': continue
        if fname[:2] == ".#" : continue
        compileItem(path.join(srcFolder, fname), path.join(dstFolder, fname))

def compileItem(srcName, dstName):
    if path.isfile(srcName):
        copySingle(srcName, dstName)
        return
    if path.exists(path.join(srcName, 'МодульФормы.1s')):
        if path.exists(path.join(srcName, 'Форма.frm')):
            compileERT(srcName, dstName+'.ert')
            return
    compileFolder(srcName, dstName)

def decompileFolder(srcFolder, dstFolder):
    createFolder(dstFolder)
    for fname in listdir(srcFolder):
        fullName=path.join(srcFolder, fname)
        if fname[:2] == ".#" :
            remove(fullName)
        else:
            decompileItem(fullName, path.join(dstFolder, fname))

def decompileItem(srcName, dstName):
    if path.isdir(srcName):
        decompileFolder(srcName, dstName)
    elif path.splitext(srcName)[1].upper() == ".ERT":
        decompileERT(srcName, path.splitext(dstName)[0])
    else:
        copySingle(srcName, dstName)
