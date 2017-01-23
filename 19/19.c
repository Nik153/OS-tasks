#include	<stdio.h>
#include	<string.h>
#include	<malloc.h>
#include	<unistd.h>
#include	<pthread.h>

#define		BUFF_SIZE 80

typedef struct node_struct {
	struct node_struct* next;
	char* data;
} node;

pthread_rwlock_t rwlock;
node* head;

node* push_front(node* p, char* buf) {

	pthread_rwlock_wrlock (&rwlock);	

	int len = strlen (buf);
	
	node* q = (node*) malloc (sizeof (node));
	q -> data = (char*) malloc (len);
	
	strncpy (q->data,buf,len);
	q -> data [len] = 0;
	
	q -> next = p;		
	pthread_rwlock_unlock (&rwlock);

	return q;
}

void destroy (node* p) {

	node* q;
	while (p) {
		q = p;
		p = p -> next;
		free (q->data);
		free (q);
	}
}

void print_all (node* p) {

	pthread_rwlock_rdlock (&rwlock);
	fprintf (stderr, "Curr elements:\n");
	while (p) {
		printf ("- %s\n", p->data);
		p = p -> next;
	}
	pthread_rwlock_unlock (&rwlock);

}

void sort (node* p) {

	node* i;
	node* j;
	char* tmp_str;
	for (i = p; i; i = i -> next)
		for (j = i -> next; j; j = j -> next)
			if( strcmp(i->data,j->data) > 0) {
				tmp_str = i -> data;
				i -> data = j -> data;
				j -> data = tmp_str;
			}
}

void* sorter (void* ptr) {
	while (1) {
		sleep (5);

		pthread_rwlock_wrlock (&rwlock);		
		sort (head);		
		pthread_rwlock_unlock (&rwlock);
	}
}


int main () {
	
	int input_size;
	char buf [BUFF_SIZE];
	pthread_t thread;

	pthread_rwlock_init (&rwlock, 0);
	pthread_create (&thread, 0, sorter, 0);
	
	while ((input_size = read (0, buf, BUFF_SIZE)) > 0) {

		buf [input_size - 1] = 0;
		
		
		if (input_size == 1) {
			print_all (head);
		}
		else {
			head = push_front (head, buf);
		}
		
	}
	
	pthread_cancel(thread);
	pthread_join(thread, 0);
	
	pthread_rwlock_destroy (&rwlock);
	destroy (head);
	return 0;
}
