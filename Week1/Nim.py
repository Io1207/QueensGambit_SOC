import random as rand

COMPUTER = 1 #an attempt to appease AI in case it takes over the world
HUMAN = 2

# move has two attributes-
# 1) pileIndex = The index of pile from which ctick(s) is
#				 going to be removed
# 2) stickszRemoved = Number of sticks removed from the
#					 pile indexed = pileIndex 

class move:

	def __init__(self):

		self.pileIndex = 0
		self.sticksTaken = 0



# A function to see the current game state.
def showPiles(piles):  #initial state of the game(no of sticks in the pile)
	print(*piles)


def gameEnd(piles, n):
	for i in range(n):
		if (piles[i] != 0):
			return False #game goes on

	return True #game ended

#call this function only if sticks in each pile is over
def winnerWinnerChickerDinner(whoseTurn): #I am a vegetarian and I still wrote this
	if (whoseTurn == COMPUTER):
		print("You won given that you are not the computer")
	else:
		print("Computer won")
	return

#calculating nim-sum
def calculateNimSum(piles, n):
	nimsum = piles[0]
	for i in range(1, n):
		nimsum = nimsum ^ piles[i]   #it helps to remember your school comp textbook at times. ^ is XOR
	return nimsum

# A function to make moves of the Nim Game
def makeMove(piles, n, moves):
	nimSum = calculateNimSum(piles, n)

	# Trying to make the num-sum 0
	if (nimSum != 0):
		for i in range(n):

			#If can move, move
			if ((piles[i] ^ nimSum) < piles[i]): #removing the sticks st nim sum still zero

				moves.pileIndex = i
				moves.stones_removed = piles[i]-(piles[i] ^ nimSum)
				piles[i] = (piles[i] ^ nimSum)
				break

	# Dear player on the losing side, this is for you
	else:
		
		filledPileIndex = [None for _ in range(n)]
		count = 0
		for i in range(n):
			if (piles[i] > 0):
				filledPileIndex[count] = i
				count += 1

		moves.pileIndex = int(rand.random() * (count))
		moves.stones_removed = 1 + \
			int(rand.random() * (piles[moves.pileIndex]))
		piles[moves.pileIndex] -= moves.stones_removed

		if (piles[moves.pileIndex] < 0):
			piles[moves.pileIndex] = 0

	return

#In case you wish to play the game, I am a very compectitive person, I will keep computer as the first player
def playGame(piles, n, whoseTurn):
	print("staring the game now")
	moves = move()

	while (gameEnd(piles, n) == False):
		showPiles(piles, n)
		makeMove(piles, n, moves)

		if (whoseTurn == COMPUTER):

			print("COMPUTER removes", moves.stones_removed,
				"stones from pile at index ", moves.pileIndex)
			whoseTurn = HUMAN

		else:
			print("HUMAN removes", moves.stones_removed,
				"stones from pile at index", moves.pileIndex)
			whoseTurn = COMPUTER

	showPiles(piles, n)
	winnerWinnerChickerDinner(whoseTurn)
	return

def predictWinner(piles, n, whoseTurn):
	print("Prediction before playing the game -> ", end="")
	if (calculateNimSum(piles, n) != 0):

		if (whoseTurn == COMPUTER):
			print("COMPUTER will win")
		else:
			print("HUMAN will win")

	else:

		if (whoseTurn == COMPUTER):
			print("HUMAN will win")
		else:
			print("COMPUTER will win")

	return

##Mic Testing 1,2,3

#piles = [1, 3, 5, 7]
#n = len(piles)

#predictWinner(piles, n, COMPUTER)
