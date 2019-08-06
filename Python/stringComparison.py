#
# Written fully by Andrew Wells
# Done as a purely functional package to be used in a few other python works of varying complexity
# Written late 2015
#

def compare(ideal, usrInput, ignoreArray=[]):
	ideal = str(ideal).lower()
	usrInput = str(usrInput).lower()
	for x in range(len(ignoreArray)):
		ignoreArray[x] = ignoreArray[x].lower()
	if(ideal == usrInput):
		return True
	idealWords = splitIntoWords(ideal, ignoreArray)
	usrWords = splitIntoWords(usrInput, ignoreArray)
	for x in range(len(idealWords)):
		for y in range(len(usrWords)):
			if(idealWords[x] == usrWords[y]):
				return True

def splitIntoWords(phrase, ignore=[]):
	phrase = str(phrase).lower()
	returnWords = []
	buildString = ""
	for x in range(len(phrase)):
		if(phrase[x] != " "):
			buildString += phrase[x]
		else:
			if(ignore.count(buildString) == 0 and len(buildString) > 0):
				#not in array and is a word
				returnWords.append(buildString)
			buildString = ""
	if(buildString != ""): #last word
		if(ignore.count(buildString) == 0):
			returnWords.append(buildString)
	return returnWords
