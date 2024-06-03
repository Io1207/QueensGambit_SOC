# A Python3 program to implement Game of Nim. The program
# assumes that both players are playing optimally
import random

COMPUTER = 1
HUMAN = 2

# A Structure to hold the two parameters of a move

# move has two parameters-
# 1) pile_index = The index of pile from which stone is
#				 going to be removed
# 2) stones_removed = Number of stones removed from the
#					 pile indexed = pile_index */


class move:

	def __init__(self):

		self.pile_index = 0
		self.stones_removed = 0


# piles[] -> Array having the initial count of stones/coins
#		 in each piles before the game has started.
# n	 -> Number of piles

# The piles[] are having 0-based indexing

# A function to output the current game state.
def showPiles(piles, n):
	print("Current Game Status -> ")
	print(*piles)

# A function that returns True if game has ended and
# False if game is not yet over
def gameOver(piles, n):
	for i in range(n):
		if (piles[i] != 0):
			return False

	return True

# A function to declare the winner of the game
def declareWinner(whoseTurn):
	if (whoseTurn == COMPUTER):
		print("\nHUMAN won")
	else:
		print("\nCOMPUTER won")
	return


# A function to calculate the Nim-Sum at any point
# of the game.
def calculateNimSum(piles, n):
	nimsum = piles[0]
	for i in range(1, n):
		nimsum = nimsum ^ piles[i]
	return nimsum

# A function to make moves of the Nim Game
def makeMove(piles, n, moves):
	nim_sum = calculateNimSum(piles, n)

	# The player having the current turn is on a winning
	# position. So he/she/it play optimally and tries to make
	# Nim-Sum as 0
	if (nim_sum != 0):
		for i in range(n):

			# If this is not an illegal move
			# then make this move.
			if ((piles[i] ^ nim_sum) < piles[i]):

				moves.pile_index = i
				moves.stones_removed = piles[i]-(piles[i] ^ nim_sum)
				piles[i] = (piles[i] ^ nim_sum)
				break

	# The player having the current turn is on losing
	# position, so he/she/it can only wait for the opponent
	# to make a mistake(which doesn't happen in this program
	# as both players are playing optimally). He randomly
	# choose a non-empty pile and randomly removes few stones
	# from it. If the opponent doesn't make a mistake,then it
	# doesn't matter which pile this player chooses, as he is
	# destined to lose this game.

	# If you want to input yourself then remove the rand()
	# functions and modify the code to take inputs.
	# But remember, you still won't be able to change your
	# fate/prediction.
	else:
		# Create an array to hold indices of non-empty piles
		non_zero_indices = [None for _ in range(n)]
		count = 0
		for i in range(n):
			if (piles[i] > 0):
				non_zero_indices[count] = i
				count += 1

		moves.pile_index = int(random.random() * (count))
		moves.stones_removed = 1 + \
			int(random.random() * (piles[moves.pile_index]))
		piles[moves.pile_index] -= moves.stones_removed

		if (piles[moves.pile_index] < 0):
			piles[moves.pile_index] = 0

	return

# A C function to play the Game of Nim
def playGame(piles, n, whoseTurn):
	print("\nGAME STARTS")
	moves = move()

	while (gameOver(piles, n) == False):
		showPiles(piles, n)
		makeMove(piles, n, moves)

		if (whoseTurn == COMPUTER):

			print("COMPUTER removes", moves.stones_removed,
				"stones from pile at index ", moves.pile_index)
			whoseTurn = HUMAN

		else:
			print("HUMAN removes", moves.stones_removed,
				"stones from pile at index", moves.pile_index)
			whoseTurn = COMPUTER

	showPiles(piles, n)
	declareWinner(whoseTurn)
	return

def knowWinnerBeforePlaying(piles, n, whoseTurn):
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

# Driver program to test above functions

# Test Case 1
piles = [1, 3, 5, 7]
n = len(piles)

# We will predict the results before playing
# The COMPUTER starts first
knowWinnerBeforePlaying(piles, n, COMPUTER)

# Let us play the game with COMPUTER starting first
# and check whether our prediction was right or not
#playGame(piles, n, COMPUTER)

# This code is contributed by phasing17
