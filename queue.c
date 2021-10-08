#include "queue.h"


extern queue_t* newQueue() // initalizes the values of a queue
{
    // Name: newQueue
    // Input Parameters: none
    // Output: intilizes a new queue
    // Purpose: intilize a queue
    queue_t *returnvalue;
    returnvalue = (queue_t *)malloc(sizeof(queue_t));
    if(returnvalue != NULL){
        returnvalue->front = returnvalue->rear = NULL;
    }
    else{
        printf("Memory is full, cannot create queue \n");
    }
    return returnvalue;
}


extern int isempty(queue_t queue) // checks if the queue is empty
{
    // Name: Qisempty
    // Input Parameters: a queue
    // Output: a integer stating if the queue is empty or not 1 if empty 0 if not
    // Purpose: lets calling function know if the queue is empty
    return (queue.front == NULL && queue.rear == NULL); // if front and rear are NULL the queue is empty
}


extern queue_node* newNode(char *token) // gets a new node for a queue
{
    // Name: Qgetnewnode
    // Input Parameters: a token
    // Output: a intialized new node for a queue
    // Purpose: create a new node for a queue
    queue_node *newnode;
    newnode = (queue_node *)malloc(sizeof(queue_node)); // allocate memory for the new node
    if (newnode != NULL){
        newnode->token = token;
        newnode->next = NULL; // intialize next and prev
    }
    return newnode; // return the new node to calling function
}


extern void enqueue(queue_t *queue, char *token) // adds an element to the queue
{
    // Name: enqueue
    // Input Parameters: a queue and a token
    // Output: places an token on the queue
    // Purpose: to add elements in the queue
    queue_node *newnode = newNode(token); // get new node to be inserted into queue
    if(newnode != NULL) // if memory is not full insert on to queue
    {
        if (isempty(*queue)) queue->front = newnode; // if list is empty set front pointer to new node
        else queue->rear->next = newnode; // if list is not empty change rear next to new node
        queue->rear = newnode; // set rear to new node either way
    }
    else printf("Memory is full could not enqueue token %s", token); // if memory is full notify user
}

extern char* dequeue(queue_t *queue) // removes an element from the queue
{
    // Name: dequeue
    // Input Parameters: a queue
    // Output: removes a token from a queue
    // Purpose: to remove elements from a queue
    char *token; // token removed from the queue
    if(!isempty(*queue)) // if the list is not empty remove an element
    {
        queue_node *remove = queue->front; // node to be removed from the queue
        token = remove->token; // item removed from the queue and returned to calling function
        if (queue->front == queue->rear) queue->rear = NULL; // if front equals rear last element on the list set rear to NULL
        queue->front = queue->front->next; // set front to front next
        free(remove); // free memory allocated for the node
    }
    else{
         token = (char *)malloc(sizeof(1)); // if the list is empty let the calling function know
         token[0] = '\0';
    }
    return token; // send item to calling function
}
