#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <semaphore.h>

int
main(int argc, char* argv[])
{	
	sem_t *s0;
	sem_t *s1;
	int i = 0;
	int err = 0;
	
	s0 = sem_open("0", O_CREAT | O_EXCL, 0777, 1);
	s1 = sem_open("1", O_CREAT | O_EXCL, 0777, 0);
	
	if(fork()) {	
		for( i = 1 ; i < 11 ; ++i ) {
			sem_wait(s1);
	        printf("String # %i from child\n",i);
			sem_post(s0);
		}
	}

	else {
		for( i = 1 ; i < 11 ; ++i )  {
			sem_wait(s0);
	        printf("String # %i from parent\n",i);
			sem_post(s1);		
		}
	}

	sem_unlink("0");
	sem_unlink("1");
	return(0);
}
