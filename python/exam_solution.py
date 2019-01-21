#!/usr/bin/python

import os
import datetime

class PostcardList():
  
	def __init__(self):
		self._file = None
		self._postcards = [] 
		self._date = {}
		self._from = {}
		self._to = {}

	def readFile(self, from_file_path):
		'''
		This function initialize the PostcardList with _file = from_file_path
		Only for initialization
		''' 
		self._file = from_file_path
		self._postcards = []
		with open(from_file_path) as file: 
			for pc in file:  
				self._postcards.append(pc)
		self.parsePostcards()


	def updateLists(self, from_file_path):
		'''
		Updates object to include new Postcards from another file 
		'''
		pc_list = []
		with open(from_file_path) as file: 
			for pc in file:  
				self._postcards.append(pc)
				pc_list.append(pc)
		self.parsePostcards()
		# print(pc_list)
		self.updateFile(pc_list)

	def updateFile(self, pc_list):
		'''
		Update _file associated with PostcardLists.
		Meant to be a private function called by updateLists(). Do not run by it self!!
		'''
		with open(self._file, 'a') as file:
			for pc in pc_list:
				file.write(pc)

			
	def writeFile(self, to_file_path):
		'''
		Write postcardList into new file
		'''
		with open(to_file_path, 'w') as file:
			for pc in self._postcards:
				file.write(pc)


	def parsePostcards(self):
		prev_len = len(self._date) 
		for record, pc in enumerate(self._postcards[prev_len:self.getNumberOfPostcards()],0):
			record += prev_len
			date_key =  pc.split(';')[0].split(':')[1]
			from_key = pc.split(';')[1].split(':')[1]
			to_key = pc.split(';')[2].split(':')[1]

			if date_key not in self._date:
				self._date[ date_key ] = []

			if from_key not in self._from:
				self._from[ from_key ] = []

			if to_key  not in self._to:
				self._to[ to_key ] = []

			self._date[ date_key ].append(record)  
			self._from[ from_key ].append(record)  
			self._to[ to_key  ].append(record)   

	def getPostcardsBySender(self, sender):
		pcs = []
		if sender in self._from.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 0) if i in self._from.get(sender, [-1])]
		return pcs	

	def getPostcardsByReceiver(self, receiver):
		pcs = []
		if receiver in self._to.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 0) if i in self._to.get(receiver, [-1])]
		return pcs	

	def getPostcardsByDateRange(self, date_range): # returns the postcards within a date_range
		datesInRange = []
		date_ini = date_range[0]
		date_end = date_range[1]
		for key in self._date.keys():
			datetime_key = datetime.datetime.strptime(key, "%Y-%m-%d") 
			if ( datetime_key >= date_ini  and  datetime_key <= date_end ):
				datesInRange += self._date[key]
		pcInRange = [ self._postcards[i] for i in datesInRange ]
		return pcInRange

	def getNumberOfPostcards(self):
		return len(self._postcards)

if __name__ == '__main__':

	curret_directory = os.getcwd()

	# file_name = 'exam_postcard_list0.txt'

	file_name = 'test0.txt'
	other_file_name = 'test1.txt'
	new_file_name = 'test2.txt'

	file_path = curret_directory + "/" + file_name
	other_file_path = curret_directory + "/" + other_file_name
	new_file_path = curret_directory + "/" + new_file_name

	postL = PostcardList()

	postL.readFile(file_path)
	print("antes \n", postL._from)
	postL.updateLists(other_file_path)
	print("despues \n", postL._from)
	postL.writeFile(new_file_path)