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

	s0 = sem_open("first_sem" , O_CREAT | O_EXCL, 0777, 1);
	s1 = sem_open("second_sem", O_CREAT | O_EXCL, 0777, 0);
	
		
	for( i = 1 ; i < 11 ; ++i ) {
		sem_wait(s1);
	    printf("String # %i from child\n",i);
		sem_post(s0);
	}

	sem_unlink("first_sem");
	sem_unlink("second_sem");
	return(0);
}
