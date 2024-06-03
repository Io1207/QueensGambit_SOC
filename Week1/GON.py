def optimal_strategy(n, arr):
    mapping = [[0] * n for _ in range(n)]

    for i in range(n): #Whenever the subarray is just an element long
        mapping[i][i] = arr[i]

    for length in range(2, n+1): # Fill the table for sublists of increasing lengths
        for i in range(n-length+1):
            j = i + length - 1
            mapping[i][j] = max(arr[i] - mapping[i+1][j], arr[j] - mapping[i][j-1])

    for x in mapping:
        print(x)

    # Determine the winner
    if mapping[0][n-1] > 0:
        return "Player 1 wins"
    elif mapping[0][n-1] < 0:
        return "Player 2 wins"
    else:
        return "It's a draw"

n = int(input("Enter the number of elements: "))
arr = list(map(float,input("Enter the elements: ").split()))


result = optimal_strategy(n, arr)
print(result)
