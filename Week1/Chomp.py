def get_moves(position):
    moves = []
    for i in range(len(position)):
        for j in range(position[i]):
            new_position = [min(position[k], j) for k in range(i + 1)]
            moves.append(new_position)
    return moves

def is_losing_position(position, memo):
    if tuple(position) in memo:
        return memo[tuple(position)]
    
    for move in get_moves(position):
        if not is_losing_position(move, memo):
            memo[tuple(position)] = False
            return False
    
    memo[tuple(position)] = True
    return True

def find_winning_moves(position, memo):
    for move in get_moves(position):
        if is_losing_position(move, memo):
            return True, move
    return False, None

def get_winning_sequence(position):
    memo = {}
    sequence = []
    current_position = position
    
    while True:
        winning, next_move = find_winning_moves(current_position, memo)
        if not winning:
            break
        sequence.append(next_move)
        current_position = next_move
    
    return sequence

# Input
n = int(input("Enter the number of rows: "))
position = []
for _ in range(n):
    row = int(input(f"Enter the number of blocks in row {_+1}: "))
    position.append(row)

# Get the winning sequence of moves
winning_sequence = get_winning_sequence(position)

# Output the sequence of moves
print("Winning sequence of moves for the first player:")
for move in winning_sequence:
    print(move)

'''import numpy as np
x=np.random.rand(50)
x=x*50
for y in x:
    print(y,end=" ")'''
