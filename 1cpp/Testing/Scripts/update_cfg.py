import os

def parseIni():
    Result = {}

    f = file('scripts.ini', 'r')
    lines = f.readlines()
    f.close()

    for line in lines:
        if line[0] == r'#':
            continue

        delimPos = line.find('=')
        if delimPos == -1:
            continue

        key = line[:delimPos].strip()
        value = line[delimPos+1:-1].strip()
        Result[key] = value

    return Result


def printDecoded(theText):

    linesTotal = len(theText)
    for i in range(1, linesTotal+1):
        line = theText[linesTotal - i]
        if line[0] == '?':
            del theText[linesTotal - i]

    for line in theText:
        print line[:-1].decode('cp1251', 'ignore')


def loadNewMD(DBPath, MdName):

    args = []
    args.append(' config')
    args.append(' /d"'+DBPath+'"')

    if ini['1CUser']:
        args.append(' /n' + ini['1CUser'])

    if ini['1CPasswd']:
        args.append(' /p' + ini['1CPasswd'])

#    args.append(' /q '+ MdName)

    os.spawnv(os.P_WAIT, ini['1CExe'], args)

def cvsUpdate():
    os.chdir('..')

    cvsOut = os.popen('cvs -z9 -q update -P -d', 'r')
    Result = cvsOut.readlines()
    cvsOut.close()

    os.chdir('scripts')

    printDecoded(Result)


## Main

ini = parseIni()

#cvsUpdate()
os.system('gcomp.exe -c -D ..\\Config --no-profiles')
DBPath = os.path.normpath(os.getcwd() + r'\..\..')
MdName = os.getcwd() + r'\1cv7.new.md'
loadNewMD(DBPath, MdName)
