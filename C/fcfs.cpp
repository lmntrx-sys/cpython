#include <iostream>

void calculaTeWaitingTime(int ProcessQueue[], int n, int wt[], int bt[]){
    wt[0] = 0;
    for (int i = 1; i < n; i++){
        wt[i] = bt[i-1] + wt[i-1];
    }
}

void calculateTurnAroundTime(int ProcessQueue[], int n, int wt[], int bt[], int tat[]){

    for (int i = 1; i < n; i++){
        tat[i] = wt[i] + bt[i];
    }
}

void FindAverageTime(int ProcessQueue[], int n, int bt[]){
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    calculaTeWaitingTime(ProcessQueue, n, wt, bt);
    calculateTurnAroundTime(ProcessQueue, n, wt, bt, tat);

    wt[0] = 0;
    tat[0] = bt[0];

    for (int i = 1; i < n; i++){
        total_wt = total_wt + wt[i];
        total_tat = total_tat + bt[i];
        std::cout << "   " << i+1 << "\t\t" << bt[i] <<"\t "<< wt[i] <<"\t\t  " << tat[i] <<std::endl;

    }
    std::cout << "Average waiting time = " 
         << (float)total_wt / (float)n;
    std::cout << "\nAverage turn around time = " 
         << (float)total_tat / (float)n;
}

int main(){
    int ProcessQueue[] = {1, 2, 3, 4, 5};

    int bt[] = {6, 3, 4, 8, 9};

    int n = sizeof(ProcessQueue) / sizeof(ProcessQueue[0]);
    FindAverageTime(ProcessQueue, n, bt);
    return 0;
}