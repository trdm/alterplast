import os
import dbconvert
import dbfpy.dbf
import dbfpy.dbfnew
import datetime

def getCurrentQuorterBegin():
	Today = datetime.date(2000, 1, 1).today()
	qwBegin = Today.replace(day=1, month=((Today.month-1) / 3 ) * 3 + 1)
	
	return (qwBegin.year, qwBegin.month, qwBegin.day)

def CreateSystemDBF():
	fileName = r'..\..\1ssystem.dbf'
	
	dbn = dbfpy.dbfnew.dbf_new()

	dbn.add_field('CURDATE',   'D', 8)
	dbn.add_field('CURTIME',   'C', 6)
	dbn.add_field('EVENTIDTA', 'C', 9)
	dbn.add_field('DBSIGN',    'C', 3)
	dbn.add_field('DBSETUUID', 'C', 36)
	dbn.add_field('SNAPSHPER', 'C', 1)
	dbn.add_field('ACCDATE',   'D', 8)
	dbn.add_field('FLAGS',     'N', 10, 0)
	dbn.write(fileName)

	db = dbfpy.dbf.Dbf()
	db.openFile(fileName, readOnly = 0)

	dbRecord = dbfpy.dbf.DbfRecord(db)
	dbRecord['CURDATE'] = eval('(2000, 1, 1)')
	dbRecord['CURTIME'] = '     0'
	dbRecord['EVENTIDTA'] = '     0   '
	dbRecord['DBSIGN'] = 'fez' # TODO ini['DBSign']
	dbRecord['DBSETUUID'] = '00000000-0000-0000-0000-000000000000'
	dbRecord['SNAPSHPER'] = 'M'
	dbRecord['ACCDATE'] = getCurrentQuorterBegin()
	dbRecord['FLAGS'] = 0
	dbRecord.store()

	db.close()

	# espetially for 1C
	db = file(fileName, "a+")
	db.write(chr(0x1A))
	db.close()

theConverter = dbconvert.Converter('txt2dbf', r'..\TestDB', r'..\..')
theConverter.go()

CreateSystemDBF()
