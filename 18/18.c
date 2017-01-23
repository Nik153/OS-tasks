#include	<stdio.h>
#include	<string.h>
#include	<malloc.h>
#include	<unistd.h>
#include	<pthread.h>

#define		BUFF_SIZE 80

typedef struct node_struct {
	struct node_struct* next;
	char* data;
	pthread_mutex_t mtx;
} node;

pthread_mutex_t head_mtx;
node* head;

node* push_front(node* p, char* buf) {

	int len = strlen (buf);
	
	node* q = (node*) malloc (sizeof (node));
	pthread_mutex_init (&(q -> mtx), 0);
	q -> data = (char*) malloc (len);
	
	strncpy (q->data,buf,len);
	q -> data [len] = 0;
	
	pthread_mutex_lock(&head_mtx);	
	q -> next = p;		
	head = q;
	pthread_mutex_unlock(&head_mtx);
}

void destroy (node* p) {

	node* q;
	while (p) {
		q = p;
		p = p -> next;
		pthread_mutex_destroy(&q -> mtx);
		free (q->data);
		free (q);
	}
}

void print_all (node* p) {

	fprintf (stderr, "Curr elements:\n");
	while (p) {
		pthread_mutex_lock(&(p -> mtx));
		printf ("- %s\n", p->data);
		node* q = p;
		p = p -> next;
		pthread_mutex_unlock(&(q -> mtx));
	}
}

void sort (node* p) {

	node* i;
	node* j;
	char* tmp_str;
	for (i = p; i; i = i -> next)
		for (j = i -> next; j; j = j -> next)
			if( strcmp(i->data,j->data) > 0) {
				pthread_mutex_lock(&(i -> mtx));
				pthread_mutex_lock(&(j -> mtx));

				tmp_str = i -> data;
				i -> data = j -> data;
				j -> data = tmp_str;

				pthread_mutex_unlock(&(i -> mtx));
				pthread_mutex_unlock(&(j -> mtx));
			
				sleep(1);
			}
}

void* sorter (void* ptr) {
	while (1) {
		sleep (5);
		sort (head);		
	}
}


int main () {
	
	int input_size;
	char buf [BUFF_SIZE];
	pthread_t thread;

	pthread_mutex_init(&head_mtx, 0);
	pthread_create (&thread, 0, sorter, 0);
	
	while ((input_size = read (0, buf, BUFF_SIZE)) > 0) {

		buf [input_size - 1] = 0;
				
		if (input_size == 1)
			print_all (head);
		else
			push_front (head, buf);
		
	}
	
	pthread_cancel(thread);
	pthread_join(thread, 0);
	
	pthread_mutex_destroy (&head_mtx);
	destroy (head);
	return 0;
}
