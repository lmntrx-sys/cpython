#include <stdio.h>

void selectionSort(int arr[], int n){

    for (int i = 0; i < n; i++){
        int min_index = i;
        for (int j = i+1; j < n; j++){
            if (arr[j] < arr[min_index]){
                min_index = j;
            }

        }

    int temp = arr[i];
    arr[i] = arr[min_index];
    arr[min_index] = temp;
    }
    
}

void insertionSort(int arr[], int n){
    for (int i = 0; i < n; i++){
        int x = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > x){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = x;
    }
}

int PrintArray(int *array, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
 
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high){
    int pivot = arr[high];
    int i = low -1;

    for (int j = low; j < high; j++){
        if (arr[j] <= pivot){
            i++;
            arr[i], arr[j] = arr[j], arr[j];
        }
    }
    arr[i+1], arr[high] = arr[high], arr[i+1];
    return i+1;
}

int main(){
    int arr[] = {64, 56, 32, 7, 0, 23, 45, 6, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    selectionSort(arr, n);
    PrintArray(arr, n);

    printf("______\n");

    insertionSort(arr, n);
    PrintArray(arr, n);

    return 0;
}