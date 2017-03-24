import os
import csv
import dbfpy.dbf
import filecmp
import shutil

dbf_ext = ".dbf"
cdx_ext = ".cdx"
hdr_ext = ".hdr"
body_ext = ".csv"

class _1S_Dialect(csv.Dialect):
	_name = "_1s"
	quoting = csv.QUOTE_NONNUMERIC
	delimiter = ','
	quotechar = '"'
	lineterminator = '\r\n'
	escapechar = None
	doublequote = True
	skipinitialspace = False

def passNameFilterDBF(fname):

	NameExclusions = []
	NameExclusions.append('1cv7$7.dbf')

	NameExclusions.append('1ssystem.dbf')
	NameExclusions.append('1susers.dbf')
	NameExclusions.append('1sstream.dbf')

	NameExclusions.append('1suidctl.dbf')
	NameExclusions.append('1sdnlock.dbf')
	NameExclusions.append('1scrdoc.dbf')

	NameExclusions.append('1sbkttlc.dbf')
	NameExclusions.append('1sbkttl.dbf')
	NameExclusions.append('1ssbsel.dbf')
	NameExclusions.append('1saccsel.dbf')

	NameExclusions.append('1sdbset.dbf')
	NameExclusions.append('1sdwnlds.dbf')
	NameExclusions.append('1supdts.dbf')

	NameExclusions.append('t0.dbf')

	if (fname[-4:].lower() != ".dbf"):
		return 0
	if (fname.lower() in NameExclusions):
		return 0
	if (fname[:2].lower() == 'ra'):
		return 0
	if (fname[:2].lower() == 'rg'):
		return 0

	return 1


class dbf2txtApp:

	def __init__(self, pathFrom, pathTo):
		self._pathFrom = pathFrom
		self._pathTo = pathTo

	def dbf2hdr(self, db, NameTo):

		# two results
		VerStampNum = 0
		ActCntNum = 0

		TempName = "my_really_temp_file_name.tmp"
		
		hdr = file(TempName, "w+b")
		hdr_writer = csv.writer(hdr, '_1s')
		fdCounter = 0

		for FieldDef in db.header.fieldDefs:
			TempList = []
			TempList.append(FieldDef.name)
			TempList.append(FieldDef.typeCode())
			TempList.append(FieldDef.length)
			TempList.append(FieldDef.decimalCount)

			fdCounter += 1
			if FieldDef.name == 'VERSTAMP':
				VerStampNum = fdCounter
			if FieldDef.name == 'ACTCNT':
				ActCntNum = fdCounter

			hdr_writer.writerow(TempList)

		hdr.close()

		fileexist = os.path.exists(NameTo + hdr_ext)
		if not fileexist or not filecmp.cmp(TempName, NameTo + hdr_ext, 0):
			print NameTo + hdr_ext
			shutil.copyfile(TempName, NameTo + hdr_ext)

		os.remove(TempName)
		
		return VerStampNum, ActCntNum


	def dbf2body(self, db, NameTo, VerStampNum, ActCntNum):
		TempName = "my_really_temp_file_name.tmp"

		txt = file(TempName, "w+b")	
		txt_writer = csv.writer(txt, '_1s')
		for Record in db:
			if Record.isDeleted:
				continue

			RaL = Record.asList()

			if VerStampNum:
				RaL[VerStampNum - 1] = "     0"
			if ActCntNum:
				RaL[ActCntNum - 1] = "     0"

			RaL.append(chr(0x20)) # isDeleted field
			
			txt_writer.writerow(RaL)

		txt.close()

		fileexist = os.path.exists(NameTo + body_ext)
		if not fileexist or not filecmp.cmp(TempName, NameTo + body_ext, 0):
			print NameTo + body_ext
			shutil.copyfile(TempName, NameTo + body_ext)

		os.remove(TempName)

	def dbf2txt(self, fname):

		NameFrom = self._pathFrom + "\\" + fname
		NameTo = self._pathTo + "\\" + fname

		csv.register_dialect("_1s", _1S_Dialect())

		db = dbfpy.dbf.Dbf()
		db.openFile(NameFrom + dbf_ext)

		VerStampNum, ActCntNum = self.dbf2hdr(db, NameTo)
		self.dbf2body(db, NameTo, VerStampNum, ActCntNum)

		db.close()

		csv.unregister_dialect("_1s")

	def go(self):
		fileList = []

		ListDir = os.listdir(self._pathFrom)
		for fname in ListDir:
			FullName = self._pathFrom + "\\" + fname
			if os.path.isdir(FullName):
				continue
			if passNameFilterDBF(fname):
				self.dbf2txt(fname[:-4])
				
				fileList.append((fname[:-4] + hdr_ext).lower())
				fileList.append((fname[:-4] + body_ext).lower())


		#print fileList

		# delete missing files
		ListDir = os.listdir(self._pathTo)
		for fname in ListDir:
			if fname.lower() not in fileList:
				if fname == 'CVS':
					continue
				print('File removed: '+fname)
				os.remove(self._pathTo + '\\' + fname)
