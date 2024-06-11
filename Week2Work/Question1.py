import json
import copy  # use it for deepcopy if needed
import math  # for math.inf
import logging

logging.basicConfig(format='%(levelname)s - %(asctime)s - %(message)s', datefmt='%d-%b-%y %H:%M:%S',
                    level=logging.INFO)

# Global variables in which you need to store player strategies (this is data structure that'll be used for evaluation)
# Mapping from histories (str) to probability distribution over actions
strategy_dict_x = {}
strategy_dict_o = {}

utility={}

class History:
    def __init__(self, history=None):
        """
        # self.history : Eg: [0, 4, 2, 5]
            keeps track of sequence of actions played since the beginning of the game.
            Each action is an integer between 0-8 representing the square in which the move will be played as shown
            below.
              ___ ___ ____
             |_0_|_1_|_2_|
             |_3_|_4_|_5_|
             |_6_|_7_|_8_|

        # self.board
            empty squares are represented using '0' and occupied squares are either 'x' or 'o'.
            Eg: ['x', '0', 'x', '0', 'o', 'o', '0', '0', '0']
            for board
              ___ ___ ____
             |_x_|___|_x_|
             |___|_o_|_o_|
             |___|___|___|

        # self.player: 'x' or 'o'
            Player whose turn it is at the current history/board

        :param history: list keeps track of sequence of actions played since the beginning of the game.
        """
        if history is not None:
            self.history = history
            self.board = self.get_board()
        else:
            self.history = []
            self.board = ['0', '0', '0', '0', '0', '0', '0', '0', '0']
        self.player = self.current_player()

    def current_player(self):
        """ Player function
        Get player whose turn it is at the current history/board
        :return: 'x' or 'o' or None
        """
        total_num_moves = len(self.history)
        if total_num_moves < 9:
            if total_num_moves % 2 == 0:
                return 'x'
            else:
                return 'o'
        else:
            return None

    # I don't like the way this function has been written
    def get_board(self):
        """ Play out the current self.history and get the board corresponding to the history in self.board.

        :return: list Eg: ['x', '0', 'x', '0', 'o', 'o', '0', '0', '0']
        """
        board = ['0', '0', '0', '0', '0', '0', '0', '0', '0']
        for i in range(len(self.history)):
            if i % 2 == 0:
                board[self.history[i]] = 'x'   #all even index moves (list is zero indexed) are x's moves
            else:
                board[self.history[i]] = 'o'
        return board

    def is_win(self):
        # check if the board position is a win for either players
        # Feel free to implement this in anyway if needed
        if len(self.history<=5):
            return 0   

        winCases=[{0,1,2},{0,3,6},{0,4,8},{1,4,7},{2,5,8},{2,4,6},{3,4,5},{6,7,8}]
        x={}
        o={}
        for i in len(self.history):
            if i%2==0:
                x.add(self.history[i])
            else:
                o.add(self.history[i])
        
        for samp in winCases:
            if samp.issubset(x):
                return (1)
            elif samp.issubset(o):
                return (-1)
            else:
                return 0  #not necessarily draw

    def is_draw(self):
        # check if the board position is a draw
        # Feel free to implement this in anyway if needed
        if len(self.history)==9:
            if self.is_win()==0:
                return True
            
        return False

    def get_valid_actions(self):
        # get the empty squares from the board
        # Feel free to implement this in anyway if needed
        pass

    def stringRepresent(self):
        return ''.join(self.get_board)

    def is_terminal_history(self):
        # check if the history is a terminal history
        # Feel free to implement this in anyway if needed
        if self.is_win!=0 or self.is_draw():
            self.get_utility_given_terminal_history
            return True
        return False

    def get_utility_given_terminal_history(self):
        # Feel free to implement this in anyway if needed
        global utility 
        z=self.is_win()
        if z==1:
            utility[self.stringRepresent]=(1,None)
        elif z==-1:
            utility[self.stringRepresent]=(-1,None)
        elif self.draw:
            utility[self.stringRepresent]=(0,None)

    def update_history(self, action):
        # In case you need to create a deepcopy and update the history obj to get the next history object.
        # Feel free to implement this in anyway if needed
        altHistory = History()
        altHistory.history = copy.deepcopy(self.history)
        altHistory.history.append(action)
        return altHistory
    
    def getPossibleActions(self):
        possActions=[]
        for i in range(9):
            if i not in self.history:
                possActions.append(i)

    #def utilityAtState(self): #calculating the utility at each state
        #if len(self.history)==9:
            #self.utilityAtState()
    #      
        ## whenever 2 x's in a row, utility=2/3, 1 x utility=0,
        #else:
            #winCases=[{0,1,2},{0,3,6},{0,4,8},{1,4,7},{2,5,8},{2,4,6},{3,4,5},{6,7,8}]
            #board=self.get_board()
            #x={}
            #o={}
            #for i in range(len(board)):
                #if board[i]=='x':
                    #x.add(i)
                #elif board[i]=='o':
                    #o.add(i)
            #for 
        #pass


def getDictionary(action):
    prob_dist = dict()
    for i in range(9):
        prob_dist[str(i)] = int(action == i)
    return prob_dist

def backward_induction(history_obj:History):
    """
    :param history_obj: History class object
    :return: best achievable utility (float) for th current history_obj
    """
    global strategy_dict_x, strategy_dict_o
    strategy_dict_x={(''.join(history_obj.history)):{}}
    strategy_dict_o={(''.join(history_obj.history)):{}}
    
    #for 
    # TODO implement
    # (1) Implement backward induction for tictactoe
    # (2) Update the global variables strategy_dict_x or strategy_dict_o which are a mapping from histories to
    # probability distribution over actions.
    # (2a)These are dictionary with keys as string representation of the history list e.g. if the history list of the
    # history_obj is [0, 4, 2, 5], then the key is "0425". Each value is in turn a dictionary with keys as actions 0-8
    # (str "0", "1", ..., "8") and each value of this dictionary is a float (representing the probability of
    # choosing that action). Example: {”0452”: {”0”: 0, ”1”: 0, ”2”: 0, ”3”: 0, ”4”: 0, ”5”: 0, ”6”: 1, ”7”: 0, ”8”:
    # 0}}
    # (2b) Note, the strategy for each history in strategy_dict_x and strategy_dict_o is probability distribution over
    # actions. But since tictactoe is a PIEFG, there always exists an optimal deterministic strategy (SPNE). So your
    # policy will be something like this {"0": 1, "1": 0, "2": 0, "3": 0, "4": 0, "5": 0, "6": 0, "7": 0, "8": 0} where
    # "0" was the one of the best actions for the current player/history.
    
    if not history_obj.is_terminal_history():
        if history_obj.current_player() == 'x':
            best_utility = -2
            best_move = -1
            # print(history_obj.get_valid_actions())
            # time.sleep(1)
            for move in history_obj.getPossibleActions():
                child = history_obj.update_history(move)
                # print(str(child))
                board_string = child.stringRepresent()
                # if str(child)[:-1] == '0412':
                #     print(str(child), board_string in utility, utility[board_string])
                if board_string in utility:
                    child_utility, cmove = utility[board_string]
                #     if cmove is not None:
                #         strategy_dict_o[str(child)] = get_dict(move)
                else:
                    child_utility = backward_induction(child)
                if child_utility > best_utility:
                    best_move = move
                    best_utility = child_utility
            strategy_dict_x[str(history_obj)] = getDictionary(best_move)
        else:
            best_utility = 2
            best_move = -1
            for move in history_obj.get_valid_actions():
                child = history_obj.update_history(move)
                board_string = child.stringRepresent()
                if str(child) in utility:
                    child_utility, cmove = utility[board_string]
                #     if cmove is not None:
                #         strategy_dict_x[str(child)] = get_dict(move)
                else:
                    child_utility = backward_induction(child)
                # print(child_utility)
                if child_utility < best_utility:
                    best_move = move
                    best_utility = child_utility
            # print('best', best_move)
            strategy_dict_o[str(history_obj)] = getDictionary(best_move)
        utility[history_obj.stringRepresent()] = (best_utility, best_move)
    else:
        best_utility = utility[history_obj.stringRepresent()][0]
        # print(best_utility)
    return best_utility



def solve_tictactoe():
    backward_induction(History())
    with open('./policy_x.json', 'w') as f:
        json.dump(strategy_dict_x, f)
    with open('./policy_o.json', 'w') as f:
        json.dump(strategy_dict_o, f)
    return strategy_dict_x, strategy_dict_o


if __name__ == "__main__":
    logging.info("Start")
    solve_tictactoe()
    logging.info("End")
