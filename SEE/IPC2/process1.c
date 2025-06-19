#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "myfifo"
#define FILE_NAME "input.txt"

int main() {
    int fd;
    char buffer[1024];

    // Create the named pipe (FIFO)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    // Open file for reading
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // Read file content and write to pipe
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        write(fd, buffer, strlen(buffer));
    }

    printf("[Sender] Data sent to receiver via FIFO.\n");

    close(fd);
    fclose(fp);
    return 0;
}
