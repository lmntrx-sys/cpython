#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1024

int main()
{   
    ssize_t nread;
    int dest_fd;
    dest_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);

    if (dest_fd==-1)
    {
        write(2, "Error opening file \n", 19);
        return 1;
    }

    char buffer[BUFFSIZE];
    
    while ((nread = read(0, buffer, sizeof(buffer))) > 0)
    {
        /* code */
        if (write(dest_fd, buffer, nread) == -1)
        {
            write(2, "Write error \n", 12);
            close(dest_fd);
            return 1;
        }
    }
    
    close(dest_fd);
    return 0;
}