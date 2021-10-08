#include <stdio.h>
#include <stdlib.h>


typedef struct qnode // type definition for a queue's node
{
    char *token; // token on the queue
    struct qnode *next; // pointers to the next element on the queue
}queue_node;

typedef struct // type definition for a whole queue
{
    queue_node *front, *rear; // pointers to the front an rear elements of the queue
    int num_of_elements;
}queue_t;

extern queue_t* newQueue(); // initalizes the values of a queue
extern queue_node* newNode(char *); // gets a new node for a queue
extern int isempty(queue_t); // checks if the queue is empty
extern void enqueue(queue_t *, char *); // adds an element to the queue
extern char* dequeue(queue_t *); // removes an element from the queue
