#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *filepath = "test.txt";
    
    // 1. OPEN (Must be O_RDWR for PROT_WRITE mapping)
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 2. ENSURE FILE IS NOT EMPTY (mmap fails on 0-length files)
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file status");
        close(fd);
        return 1;
    }
    
    if (st.st_size == 0) {
        const char *initial_text = "Foundation";
        write(fd, initial_text, strlen(initial_text));
        fstat(fd, &st); // Update the size after writing
    }

    // 3. MAP (The handshake with the Kernel)
    char *map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (map == MAP_FAILED) {
        perror("mmap failed"); 
        close(fd);
        return 1;
    }

    // 4. ACCESS AND MODIFY
    printf("Original first char: %c\n", map[0]);
    map[0] = 'J'; // Changing 'F'oundation to 'J'oundation
    printf("New first char: %c\n", map[0]);

    // 5. CLEAN UP
    if (munmap(map, st.st_size) == -1) {
        perror("Error unmapping");
    }
    close(fd);
    return 0;
}