import os
import dbfpy.dbf
import dbfpy.dbfnew
import csv
import filecmp
import shutil

dbf_ext = ".dbf"
cdx_ext = ".cdx"
hdr_ext = ".hdr"
body_ext = ".csv"


def passNameFilterTXT(fname):

	if (fname[-4:].lower() != ".hdr"):
		return 0

	return 1



class txt2dbfApp:

	def __init__(self, pathFrom, pathTo):
		self._pathFrom = pathFrom
		self._pathTo = pathTo

	def txt2dbf(self, fname):

		TempName = self._pathFrom + "\\" + "my_really_temp_file_name.tmp"
		hdrName = self._pathFrom + "\\" + fname + hdr_ext
		csvName = self._pathFrom + "\\" + fname + body_ext
		dbfName = self._pathTo + "\\" + fname + dbf_ext
		cdxName = self._pathTo + "\\" + fname + cdx_ext

		fldNames = []
		fldTypes = []
		dbn = dbfpy.dbfnew.dbf_new()

		hdr = file(hdrName, "r")
		hdr_reader = csv.reader(hdr)
		for row in hdr_reader:
			fldNames.append(row[0])
			fldTypes.append(row[1])
			dbn.add_field(row[0], row[1], int(row[2]), int(row[3]))
		dbn.write(TempName)

		db = dbfpy.dbf.Dbf()
		db.openFile(TempName, readOnly = 0)

		txt = file(csvName, "r")
		txt_reader = csv.reader(txt)
		for row in txt_reader:
			dbRecord = dbfpy.dbf.DbfRecord(db)
			fldNumber = 0
			for fldName in fldNames:
				if fldTypes[fldNumber] == 'N':
					dbRecord[fldName] = float(row[fldNumber])
				elif fldTypes[fldNumber] == 'D':
					dbRecord[fldName] = eval(row[fldNumber])
				else:
					dbRecord[fldName] = row[fldNumber]
				fldNumber += 1
	
			if row[len(fldNames)] == "*":
				dbRecord.isDeleted = True
			dbRecord.store()

		db.close()

		# espetially for 1C
		db = file(TempName, "a+")
		db.write(chr(0x1A))
		db.close()

		txt.close()
		hdr.close()

		if not os.path.exists(dbfName) or not filecmp.cmp(TempName, dbfName, 0):
			print dbfName
			shutil.copyfile(TempName, dbfName)
			if os.path.exists(cdxName):
				os.remove(cdxName)

	def go(self):
		ListDir = os.listdir(self._pathFrom)
		for fname in ListDir:
			FullName = self._pathFrom + "\\" + fname
			if os.path.isdir(FullName):
				continue
			if passNameFilterTXT(fname):
				self.txt2dbf(fname[:-4])

		ListDir = os.listdir(self._pathTo)
		for fname in ListDir:
			if (fname[:2].lower() == 'ra') and (fname[-4:].lower() == '.dbf'):
				os.remove(self._pathTo + "\\" + fname)
			if (fname[:2].lower() == 'ra') and (fname[-4:].lower() == '.cdx'):
				os.remove(self._pathTo + "\\" + fname)
			if (fname[:2].lower() == 'rg') and (fname[-4:].lower() == '.dbf'):
				os.remove(self._pathTo + "\\" + fname)
			if (fname[:2].lower() == 'rg') and (fname[-4:].lower() == '.cdx'):
				os.remove(self._pathTo + "\\" + fname)
				
