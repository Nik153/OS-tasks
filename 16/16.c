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

pthread_mutex_t mx;
node* head;

void push_front(char* buf) {

	pthread_mutex_lock (&mx);

	int len = strlen (buf);
	
	node* q = (node*) malloc (sizeof (node));
	q -> data = (char*) malloc (len);
	
	strncpy (q->data,buf,len);
	q -> data [len] = 0;
	
	q -> next = head;		

	head = q;

	pthread_mutex_unlock (&mx);
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

	pthread_mutex_lock (&mx);

	fprintf (stderr, "Curr elements:\n");
	while (p) {
		printf ("- %s\n", p->data);
		p = p -> next;
	}
	
	pthread_mutex_unlock (&mx);
}

void sort (node* p) {

	pthread_mutex_lock (&mx);		
	
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
	pthread_mutex_unlock (&mx);		

}

void* sorter (void* ptr) {
	while (1) {
		sleep (5);
		sort (head);		
	}
	pthread_exit(NULL);
}


int main () {
	
	int i;
	int input_size;
	char buf [BUFF_SIZE];
	pthread_t thread;

	pthread_mutex_init (&mx, 0);
	pthread_create (&thread, 0, sorter, 0);
	
	while ((input_size = read (STDIN_FILENO, buf, BUFF_SIZE)) > 0) {

		buf [input_size - 1] = 0;
		
		if (input_size == 1)
			print_all (head);
		else {
			for (i = 0; i < input_size - 1; ++i) {
				if(buf[i] == 27) {
					fprintf(stderr, "Invalid symbols detected\n");
					i = 100;
					break;
				}
			}
			if(i < 100)
				push_front (buf);
		}

	}
	
	pthread_cancel(thread);
	pthread_join(thread, 0);
	
	pthread_mutex_destroy (&mx);
	destroy (head);
	pthread_exit(NULL);
}
