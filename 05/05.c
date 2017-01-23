#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void threadOnExitFunction(void *param){
    write( 1 , "NOOOOOOOO\n" , 10  );
}
	
void *threadBody(void *param)
{
    pthread_cleanup_push( threadOnExitFunction , (void*)NULL );
    while(1) {
        write(1,"I will kill you, Darth Vader!\n",30);
        usleep(2500000);
    }
    pthread_cleanup_pop(0);
    pthread_exit(NULL);   
}

int main( int argc , char **argv )
{
    pthread_t child;
    int code;
    
    pthread_create( &child , (pthread_attr_t*)NULL , threadBody , (void*)NULL );
    
    sleep(2);
    
    pthread_cancel(child);
    
    write( 1 , "I'm your father, Luke!\n" , 23 );
    
    pthread_exit(NULL);
    return (EXIT_SUCCESS);
}
