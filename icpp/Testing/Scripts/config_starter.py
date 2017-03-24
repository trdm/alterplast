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

def loadNewMD(DBPath, MdName):
	
	ini = parseIni()

	args = []
	args.append(' config')
	args.append(' /d"'+DBPath+'"')

	if ini['1CUser']:
		args.append(' /n' + ini['1CUser'])

	if ini['1CPasswd']:
		args.append(' /p' + ini['1CPasswd'])

	args.append(' /q '+ MdName)

	os.spawnv(os.P_WAIT, ini['1CExe'], args)

def startConfig(DBPath):

	ini = parseIni()
	
	args = []
	args.append(' config')
	args.append(' /d"'+DBPath+'"')

	if ini['1CUser']:
		args.append(' /n' + ini['1CUser'])

	if ini['1CPasswd']:
		args.append(' /p' + ini['1CPasswd'])

	os.spawnv(os.P_NOWAIT, ini['1CExe'], args)

