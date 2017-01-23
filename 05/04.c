#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
	
void *thread_body(void * param)
{
    write(1,"N",1);
    while(1){
        write(1,"OOO", 3);
        sleep(1);
    }
    pthread_exit(NULL);   
}

int main(int argc, char *argv[])
{
    pthread_t child_thread;
    int code;
    int i;

    write(1, "I'M YOUR FATHER, LUKE!\n",24);
    
    code=pthread_create(&child_thread, NULL, thread_body, NULL );
    if (code!=0)
    {
        perror("thread create error\n");
        exit(1);
    }


    sleep(5);

    pthread_cancel(child_thread);
    write(1,"\nNOW YOR'RE DEAD\n",19);
    pthread_exit(NULL);
    return (EXIT_SUCCESS);
}
