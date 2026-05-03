#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#define WAIT_TIME 5

int main()
{
    pid_t proc_id = fork();

    if (proc_id == 0){
        // wait for some time before creating a process
        printf("Process created, waiting to start \n");
        sleep(WAIT_TIME);
        exit(0);
    } else if (proc_id > 0) {
        int status;
        wait(&status);
        printf("Child process completed \n");
        return 0;
    } else {
        printf("process creation failed \n");
        return 1;
    }
}