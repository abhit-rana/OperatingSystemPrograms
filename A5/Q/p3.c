#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define THREADS 5

sem_t semaphores[THREADS];
sem_t bowls[4];


void* routine(void* args){
	while(1){
	int random = rand()%4;
	int ph = *(int*)args;
	if(ph==0){
		sem_wait(&semaphores[0]);
		sem_wait(&semaphores[1]);
	}else if(ph==1){
		sem_wait(&semaphores[1]);
		sem_wait(&semaphores[2]);
	}else if(ph==2){
		sem_wait(&semaphores[2]);
		sem_wait(&semaphores[3]);
	}else if(ph==3){
		sem_wait(&semaphores[3]);
		sem_wait(&semaphores[4]);
	}else if(ph==4){
		sem_wait(&semaphores[0]);
		sem_wait(&semaphores[4]);
	}
	sem_wait(&bowls[random]);
	printf("Philospher%d Got the Fork and Bowl No:%d and eats the food!\n", ph, random);
	if(ph==0){
		sem_post(&semaphores[0]);
		sem_post(&semaphores[1]);
	}else if(ph==1){
		sem_post(&semaphores[1]);
		sem_post(&semaphores[2]);
	}else if(ph==2){
		sem_post(&semaphores[2]);
		sem_post(&semaphores[3]);
	}else if(ph==3){
		sem_post(&semaphores[3]);
		sem_post(&semaphores[4]);
	}else if(ph==4){
		sem_post(&semaphores[0]);
		sem_post(&semaphores[4]);
	}
	sem_post(&bowls[random]);
	}
}





int main(int argc, char* argv[]){
	srand(time(NULL));
	int i = 0;
	pthread_t th[THREADS];
	for(i = 0; i<THREADS; i++){
		sem_init(&semaphores[i], 0, 1);
		if(i<4){
			sem_init(&bowls[i], 0, 1);
		}
	}
	for(i=0; i<THREADS; i++){
		int *a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&th[i], NULL, &routine, a)!=0){
			perror("failed to create the thread\n");
		}
	}
	for(i =0 ; i<THREADS; i++){
		if(pthread_join(th[i], NULL)!=0){
			perror("unable to join the thread\n");
		}
	}
	for(i=0; i<THREADS; i++){
		sem_destroy(&semaphores[i]);
		if(i<4){
			sem_destroy(&bowls[i]);
		}
	}
	return 0;
}

