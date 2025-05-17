#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

typedef struct {
  char str[100];
  int server, client, res;
} sharedMemory;

void myHandler(int signum) { int n = signum; }

void main() {
  signal(SIGCONT, myHandler);

  int shmid = shmget(2832, sizeof(sharedMemory), 0666 | IPC_CREAT);
  sharedMemory *shared = shmat(shmid, NULL, 0);
  shared->client = getpid();

  char word[100];
  while (1) {
    printf("\nEnter the string : ");
    fgets(word, sizeof(word), stdin);
    word[strlen(word) - 1] = '\0';
    strcpy(shared->str, word);

    if (!strcmp(word, "quit")) {
      kill(shared->server, SIGCONT);
      break;
    }

    printf("\nSending the string to the server and waiting for response\n\n");
    kill(shared->server, SIGCONT);
    pause();

    printf("Received response from the server\n");
    if (shared->res == 0)
      printf("%s is not a palindrome\n\n", word);
    else
      printf("%s is a palindrome\n\n", word);
  }
  shmdt(shared);
  shmctl(shmid, IPC_RMID, NULL);
}