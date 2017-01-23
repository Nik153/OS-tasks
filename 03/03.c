#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

char* first[4]  = { "thread # 1 string # 1" , "thread # 1 string # 2" , "thread # 1 string # 3" , "thread # 1 string # 4" };
char* second[4] = { "thread # 2 string # 1" , "thread # 2 string # 2" , "thread # 2 string # 3" , "thread # 2 string # 4" };
char* third[4]  = { "thread # 3 string # 1" , "thread # 3 string # 2" , "thread # 3 string # 3" , "thread # 3 string # 4" };
char* fourth[4] = { "thread # 4 string # 1" , "thread # 4 string # 2" , "thread # 4 string # 3" , "thread # 4 string # 4" };

void *thread_body(void * param)
{
    int i;
    char** t = (char**) param;

    for(i = 0; i < 4; ++i)
        printf("%s\n", t[i] );

    pthread_exit(NULL);   
}

int main(int argc, char *argv[])
{
    pthread_t child_thread;
    int code;
    int i;
    
	if (pthread_create(&child_thread, NULL, thread_body, (void*)first)) {
        perror("thread create error\n");
        exit(1);
    }
    if (pthread_create(&child_thread, NULL, thread_body, (void*)second)) {
        perror("thread create error\n");
        exit(1);
    }
    if (pthread_create(&child_thread, NULL, thread_body, (void*)third)) {
        perror("thread create error\n");
        exit(1);
    }
    if (pthread_create(&child_thread, NULL, thread_body, (void*)fourth)) {
        perror("thread create error\n");
        exit(1);
    }


    pthread_exit(NULL);
    return (EXIT_SUCCESS);
}
