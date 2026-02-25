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
 

int main(){
    int arr[] = {64, 56, 32, 7, 0, 23, 45, 6, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    selectionSort(arr, n);
    PrintArray(arr, n);

    printf("______");

    insertionSort(arr, n);
    PrintArray(arr, n);

    return 0;
}