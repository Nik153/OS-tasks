#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

sem_t sem1;
sem_t sem2;

void * thread_body(void * param) {
    
    int i;
    for(i = 1; i < 11; ++i) {
        sem_wait(&sem2);
        printf("String # %i from child\n",i);
        sem_post(&sem1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    int i;

    sem_init( &sem1 , 0 , 1 );
    sem_init( &sem2 , 0 , 0 );
       
    if (pthread_create(&thread, NULL, thread_body, NULL) != 0) {
        perror("thread create error\n");
        exit(1);
    }
    
    for(i = 1; i < 11; ++i) {
        sem_wait(&sem1);
        printf("String # %i from parent\n",i);
        sem_post(&sem2);
    }
        
    pthread_join(thread,NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
	
    pthread_exit(NULL);
}
