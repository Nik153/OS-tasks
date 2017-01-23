#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILO 5
#define DELAY 30000
#define FOOD 50

pthread_mutex_t forks[PHILO];

pthread_t phils[PHILO];

pthread_mutex_t foodlock;

int eated[5];
int sleep_seconds = 3000;

void *philosopherRoutine(void*);
int get_food();
void get_fork(int,int,char*);
void put_forks(int,int);

int main ( int argc , char **argv ) {

    int i;

    if( argc == 2 )
        sleep_seconds = atoi( argv[1] );

    pthread_mutex_init(&foodlock, NULL);

    for (i = 0; i < PHILO; i++) {   
        pthread_mutex_init(&forks[i], NULL);

        pthread_create(&phils[i], NULL, philosopherRoutine, (void *)i);
    }
    for (i = 0; i < PHILO; i++) {   
        pthread_join(phils[i], NULL);
    }

    for(i = 0; i < 5; ++i) {
        printf("%d eated %d\n",i, eated[i] );
    }

    printf( "\nDONE.\n\n");
    return 0;
}

void *philosopherRoutine(void *num)
{
    int id = (int)num;
    int left_fork, right_fork, f;

    printf ("Philosopher # %d has come.\n", id);
    
    right_fork = id;
    left_fork = (id + 1) % PHILO;

    int counter = 1;

    while( f = get_food()) {


        printf( "Philosopher #%d is trying to lock forkes. #%d.\n" , id , f );
        if( id % 2) {   
            get_fork( id , left_fork , "left" );
            get_fork( id , right_fork , "right" );
        }
        else {   
            get_fork( id , right_fork , "right" );            
            get_fork( id , left_fork , "left" );
        }
        printf( "Philosopher #%d is eating.\n" , id );
        eated[id]++;
       // usleep( DELAY * (FOOD - f + 1) );
        if(id % 2)
            put_forks( left_fork , right_fork );
        else
            put_forks( right_fork , left_fork );
    }

    printf ("Philosopher #%d has exploded.\n", id);
    return(NULL);
}

int get_food() {
    static int total_food = FOOD;
    int curr_food;

    pthread_mutex_lock(&foodlock);
    if (total_food > 0)
        --total_food;
    curr_food = total_food;
    pthread_mutex_unlock(&foodlock);
    return curr_food;
}

void get_fork( int phil ,  int fork , char *hand )
{
    pthread_mutex_lock( &forks[fork] );
    printf ("Philosopher #%d fork #%d in %s arm.\n", phil, fork, hand);
}

void put_forks( int f1 , int f2 ) {
    pthread_mutex_unlock( &forks[f1] );
    pthread_mutex_unlock( &forks[f2] );
}
