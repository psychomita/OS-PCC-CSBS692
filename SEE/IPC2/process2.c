#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define FIFO_NAME "myfifo"

int main() {
    int fd;
    char buffer[1024];
    ssize_t bytesRead;

    // Open the FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO");
        exit(EXIT_FAILURE);
    }

    printf("[Receiver] Reading data and printing in UPPERCASE:\n");

    // Read and convert to uppercase
    while ((bytesRead = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytesRead] = '\0';
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper((unsigned char) buffer[i]);
        }
        printf("%s", buffer);
    }

    if (bytesRead == -1) {
        perror("Error reading from FIFO");
    }

    close(fd);
    return 0;
}
