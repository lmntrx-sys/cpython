#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFSIZE 1024


int main()
{   
    ssize_t nread;
    int src_fd, dest_fd;
    src_fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);

    char buffer[BUFFSIZE];    
    nread = read(0, buffer, sizeof(buffer));

    if (nread > 0){ write(dest_fd, buffer, nread); }

    close(src_fd);
    return 0;
}