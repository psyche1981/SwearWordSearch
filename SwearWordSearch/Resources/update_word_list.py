words = []
moreWords = []

def loadWords():
	with open('words.txt') as f:
		for l in f.readlines():
			words.append(l.rstrip())

	f.close()


def loadMoreWordsFromFile():
	with open('more_words.txt') as f:
		for l in f.readlines():
			moreWords.append(l.rstrip())
	f.close()


def addWord(w):
	with open('words.txt', 'a') as f:
		f.write(w + '\n')
	f.close()


def compareWordWithList(w):
	if w in words:
		return True
	return False


def getWord():
	#get word from user
	ans = input('Please enter the word: ')
	if not compareWordWithList(ans):
		addWord(ans)
		print(ans + ' added\n')


def getWordsFromFile():
	#read in words from file
	ans = input('Is there a file named more_words.txt in the same directory as this script?(Y/N): ')
	if ans == 'y' or ans == 'Y':
		loadMoreWordsFromFile()
		for w in moreWords:
			if not compareWordWithList(w):
				addWord(w)
				print(w + ' added\n')


if __name__ == '__main__':
	loadWords()
	done = False
	while not done:
		ans = input('Select an option\n' +
			'1. Input a word\n' + 
			'2. Load words froom file\n' +
			'Q. Quit\n')

		if ans == str(1):
			getWord()
		elif ans == str(2):
			getWordsFromFile()
		elif ans == 'Q' or ans == 'q':
			done = True
		else:
			print('Enter a valid option')

