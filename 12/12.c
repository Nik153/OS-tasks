#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
   
pthread_mutex_t mutx;
pthread_cond_t cond;

void * thread_body(void * param) {
    int i;
    pthread_mutex_lock(&mutx);
    pthread_cond_signal(&cond);
    for(i = 1; i < 11; ++i) {
        pthread_cond_wait(&cond, &mutx);
        printf("String # %i from child\n",i);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutx);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    int i;
    
    pthread_mutex_init(&mutx, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_mutex_lock(&mutx);
    
    if (pthread_create(&thread, NULL, thread_body, NULL) != 0) {
        perror("thread create error\n");
        exit(1);
    }

    for(i = 1; i < 11; ++i) {
        pthread_cond_wait(&cond, &mutx);
        printf("String # %i from parent\n",i);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutx);
    
    pthread_exit(NULL);   
    return (EXIT_SUCCESS);
}
