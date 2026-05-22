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
    
    def Partion(self, high, low, arr):
        pivot = arr[high]
        i = low - 1

        for j in range(low, high):
            if arr[j] <= pivot:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]

        arr[i+1], arr[high] = arr[high], arr[i+1]
        return i+1
    
    def quickSort(self, high=None, low=0, arr=[]):
        if high is None:
            high = len(arr) - 1

        if low < high: 
            pivot_index = self.Partion(high, low, arr)
            self.quickSort(pivot_index-1, low, arr)
            self.quickSort(high, pivot_index+1, arr)
        return arr




sort = Sort()  
arr = [64, 56, 32, 7, 0, 23, 45, 6, 11]
print(f"selection sort {sort.selectionSort(arr)}")
print(f"Insertion sort {sort.InsertionSort(arr)}")
print(f"quick sort {sort.quickSort(len(arr)-1, 0, arr)}")
#print(f"Insertion sort {sort.InsertionSort(arr)}")