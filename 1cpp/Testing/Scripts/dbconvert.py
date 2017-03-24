import os
from dbc_dbf2txt import *
from dbc_txt2dbf import *

class Converter:
	def __init__(self, Direction, SrcPath, DstPath):

		if not os.path.exists(SrcPath):
			self.theApp = helperApp("Source path " + SrcPath + " doesn't exist")
			return
		if not os.path.exists(DstPath):
			self.theApp = helperApp("Destination path " + DstPath + " doesn't exist")
			return

		if Direction == 'dbf2txt':
			self.theApp = dbf2txtApp(SrcPath, DstPath)
			return
		elif Direction == 'txt2dbf':
			self.theApp = txt2dbfApp(SrcPath, DstPath)
			return
		else:
			self.theApp = helperApp("Incorrect first argument")
			return

	def go(self):
		self.theApp.go()


class helperApp:

	def __init__(self, firstMessage):
		self._firstMessage = firstMessage

	def go(self):
		print "\n" + self._firstMessage

		print "\nUsage: dbconvert.py <Direction> <Source> <Destination>"
		print "\n\t<Direction> - string \"dbf2txt\" or \"txt2dbf\""
		print "\t<Source> - Full path to folder with source files"
		print "\t<Destination> - Full path to folder with destination files"


def dbf2txt():
	theConverter = Converter('dbf2txt', r'..\..', r'..\TestDB')
	theConverter.go()

def txt2dbf():
	theConverter = Converter('txt2dbf', r'..\TestDB', r'..\..')
	theConverter.go()

