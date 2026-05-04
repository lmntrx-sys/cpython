#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define WAIT_TIME 5

int main()
{
    pid_t proc_id = fork();

    if (proc_id == 0){
        // wait for some time before creating a process
        sleep(WAIT_TIME);
        write(1, "Child process created after waiting for 5 seconds\n", 50);
        exit(0);
    } else if (proc_id > 0) {
        int status;
        wait(&status);
        write(1, "Parent process completed\n", 25);
        return 0;
    } else {
        write(2, "Process creation failed\n", 24);
        return 1;
    }
}