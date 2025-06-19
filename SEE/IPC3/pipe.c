#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main (int argc, char *argv[]) {
	if(argc==1) {
		fprintf(stderr, "Invalid number of inputs.\n");
		return EXIT_FAILURE;
	}
	int pfd[2];
	if(pipe(pfd)!=0) {
		fprintf(stderr, "Error opening pipe.\n");
		return EXIT_FAILURE;
	}
	int child=fork();
	if(child==-1) {
		fprintf(stderr, "Fork failure.\n");
		return EXIT_FAILURE;
	}
	else if(child==0) {
		close(pfd[0]);
		printf("%d: Child\n", (int)getpid());
		printf("%d: Child writing to pipe: %s\n", (int)getpid(), argv[1]);
		write(pfd[1], argv[1], strlen(argv[1]));
		close(pfd[1]);
	}
	else {
		close(pfd[1]);
		printf("%d: Parent\n", (int)getpid());
		char message[100];
		int n=read(pfd[0], message, sizeof(message)-1);
		message[n]='\0';
		close(pfd[0]);
		printf("%d: Parent read from pipe: %s\n", (int)getpid(), message);
	}
	int corpse, status;
	while(corpse=wait(&status)>0)
		printf("%d: Child PID: %d exited with status 0x%.4x\n", (int)getpid(), corpse, status);
	return EXIT_SUCCESS;
}
