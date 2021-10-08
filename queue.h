typedef struct qnode // type definition for a queue's node
{
    char token[20]; // token on the queue
    qnode *next; // pointers to the next element on the queue
}queue_node;

typedef struct // type definition for a whole queue
{
    queue_node *front = NULL, *rear = NULL; // pointers to the front an rear elements of the queue
}queue_t;


extern int isempty(queue_t); // checks if the queue is empty
extern void enqueue(queue_t *, char); // adds an element to the queue
extern char[20] dequeue(queue_t *); // removes an element from the queue
