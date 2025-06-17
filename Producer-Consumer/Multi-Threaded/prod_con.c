#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFSIZE 10

typedef struct {
	int *data, front, rear;
}CQ;

int init_buff(CQ *bp, int size){
	bp->data = (int *)malloc(size * sizeof(int));
	bp->front = bp->rear = -1;
	return (bp->data != NULL);
}

void buff_insert (CQ *bp, int item) {
	if (bp->rear == -1)
		bp->rear = bp->front = 0;
	else
		bp->rear = (bp->rear+1) % BUFFSIZE;
	bp->data[bp->rear] = item;
	return;
}

int buff_delete(CQ *bp){
	int item = bp->data[bp->front];
	if(bp->rear == bp->front)
		bp->rear = bp->front = -1;
	else
		bp->front = (bp->front + 1) % BUFFSIZE;
	return item;
}

void buff_destroy(CQ *bp){
	bp->front = bp->rear = -1;
	free(bp->data);
}

CQ buff;
sem_t mutex, full, empty;

void *produce(void *arg){
	int item = 0;
	while(1){
		sem_wait(&empty);
		sem_wait(&mutex);
		item++;
		buff_insert(&buff, item);
		printf("Item produced is : %d\n",item);
		sem_post(&mutex);
		sem_post(&full);
		sleep(4);
	}
	pthread_exit(NULL);
}
void *consume(void *arg){
	int item;
	while(1){
		sem_wait(&full);
		sem_wait(&mutex);
		item = buff_delete(&buff);
		printf("Item consumed is : %d\n",item);
		sem_post(&mutex);
		sem_post(&empty);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(){
	init_buff(&buff, BUFFSIZE);
	sem_init(&mutex,0,1);
	sem_init(&full, 0,0);
	sem_init(&empty, 0, BUFFSIZE);

	pthread_t th1,th2;
	pthread_create(&th1, NULL, produce, NULL);
	pthread_create(&th2, NULL, consume, NULL);

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	buff_destroy(&buff);
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);

	return 0;
}
