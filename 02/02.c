#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
	
void * thread_body(void * param) {
    int i;
    for(i = 1; i < 11; ++i) 
        printf("String # %i from child\n",i);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    int i;
 
    if (pthread_create(&thread, NULL, thread_body, NULL) != 0) {
       perror("thread create error\n");
       exit(1);
    }
			    
    pthread_join(thread,NULL);

    for(i = 1; i < 11; ++i) 
        printf("String # %i from parent\n",i);
    
    pthread_exit(NULL);   
    return (EXIT_SUCCESS);
}
