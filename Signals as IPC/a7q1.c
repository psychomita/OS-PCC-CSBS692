#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void myhandler(int signo)
{
	printf("\nReceived signal %d\n",signo);
	return;
}

int main()
{
	signal(2,myhandler);
	while(1)
	{
		printf("My PID is %d\n",getpid());
		sleep(2);
	}
	return 0;
}
