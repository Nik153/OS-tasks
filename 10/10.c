#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
   
pthread_mutex_t first;
pthread_mutex_t second;
pthread_mutex_t third;

#define lock1 pthread_mutex_lock(&first);
#define lock2 pthread_mutex_lock(&second);
#define lock3 pthread_mutex_lock(&third);

#define unlock1 pthread_mutex_unlock(&first);
#define unlock2 pthread_mutex_unlock(&second);
#define unlock3 pthread_mutex_unlock(&third);

void * thread_body(void * param) {
    int i;
    lock1;
    for(i = 1; i < 5; ++i) {
        lock2;
        printf("String # %i from child\n",i);
        unlock1;
        lock3;
        unlock2;
        lock1;
        unlock3;
    }

    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t thread;
    int i;

    pthread_mutex_init(&first, NULL);
    pthread_mutex_init(&second, NULL);
    pthread_mutex_init(&third, NULL);
    
    lock2;
    if (pthread_create(&thread, NULL, thread_body, NULL) != 0) {
        perror("thread create error\n");
        exit(1);
    }
    sleep(1); 
    for(i = 1; i < 5; ++i) {
        lock3;
        printf("String # %i from parent\n",i);
        unlock2;
        lock1;
        unlock3;
        lock2;
        unlock1;
    }
    
   pthread_exit(NULL);   
}
