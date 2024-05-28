n=int(input("Enter the size of the array: "))
arr=[]
for i in range(n):
    arr.append(int(input("Enter the element: ")))
def isAtEnd(arr:list,x:int):
    length=len(arr)
    if arr[0]==x or arr[n-1]==x:
        return True
    return False
def highest(arr:list):
    p=arr.sort()
    index=arr.where(arr==x)
    return arr[len(arr)-1]
while(len(arr)>0):
    if arr==arr.sort():
        print("Player1 takes the cake for being the greediest menace this world ever saw")

    else:
        x=highest(arr)
        i=0
        while i<n-1:
            if arr[i]==x:
                break
            i=i+1
            continue
        