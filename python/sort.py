class Sort:
    def __init__(self):
        pass
        
    def selectionSort(self, arr):
        n = len(arr)
        for i in range(n-1):
            min_index = i
            for j in range(i+1, n):
                if arr[j] < arr[min_index]:
                    min_index = j
        arr[i], arr[min_index] = arr[min_index], arr[i]
        return arr
    
    def InsertionSort(self, arr):
        n = len(arr)
        for i in range(1, n):
            x = arr[i]
            j = i - 1
            while j >= 0 and arr[j] > x:
                arr[j+1] = arr[j]
                j -= 1
            arr[j+1] = x
        return arr



sort = Sort()  
arr = [64, 56, 32, 7, 0, 23, 45, 6, 11]
print(f"selection sort {sort.selectionSort(arr)}")
# TODO Fix selection sort, it is not working as expected. It is not sorting the array correctly.
#print(f"Insertion sort {sort.InsertionSort(arr)}")