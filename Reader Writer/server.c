#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

typedef struct {
  char str[100];
  int server, client, res;
} sharedMemory;

int isPalindrome(char str[]) {
  int n = strlen(str);
  for (int i = 0; i <= n / 2; i++)
    if (str[i] != str[n - i - 1]) return 0;
  return 1;
}

void myHandler(int signum) { int n = signum; }

void main() {
  signal(SIGCONT, myHandler);
  int shmid = shmget(2832, sizeof(sharedMemory), 0666 | IPC_CREAT);
  sharedMemory *shared = shmat(shmid, NULL, 0);
  shared->server = getpid();

  while (1) {
    printf("\nWaiting for receiving a string from the client\n\n");
    pause();

    char word[100];
    strcpy(word, shared->str);

    printf("Received string is : %s\n\n", word);
    if (!strcmp(word, "quit")) break;

    shared->res = isPalindrome(word);

    printf("Sending response to the client\n\n");
    kill(shared->client, SIGCONT);
  }

  shmdt(shared);
  shmctl(shmid, IPC_RMID, NULL);
}