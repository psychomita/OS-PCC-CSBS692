#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int pid, pid1, ppid1;
	pid = fork();
	if (pid == 0)
	{
		pid1 = getpid();
		ppid1 = getppid();
		printf("CHILD: PID = %d\tPPID = %d\n", pid1, ppid1);
		sleep(5);
		printf("CHILD DIES.\n");
	}
	if (pid > 0)
	{
		pid1 = getpid();
		ppid1 = getppid();
		printf("PARENT: PID = %d\tPPID = %d\n",pid1, ppid1);
		sleep(10);
		wait(NULL);
	}
	return 0;
}
