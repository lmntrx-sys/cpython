class Sort:
    def __init__(self):
        pass
        
    def selectionSort(arr):
        n = len(arr)
        for i in range(n):
            min_index = i
            for j in range(i+1, n):
                min_index = j
        arr[i], arr[min_index] = arr[min_index], arr[i]
        return arr
    
arr = [64, 56, 32, 7, 0, 23, 45, 6, 11]
print(Sort.selectionSort(arr))
