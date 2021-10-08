extern int isempty(queue_t queue) // checks if the queue is empty
{
    // Name: Qisempty
    // Input Parameters: a queue
    // Output: a integer stating if the queue is empty or not 1 if empty 0 if not
    // Purpose: lets calling function know if the queue is empty
    return (queue.front == NULL && queue.rear == NULL); // if front and rear are NULL the queue is empty
}


extern queue_node* Qgetnewnode(char item, int *flag) // gets a new node for a queue
{
    // Name: Qgetnewnode
    // Input Parameters: a character and a flag
    // Output: a intialized new node for a queue
    // Purpose: create a new node for a queue
    queue_node *newnode = (queue_node *)malloc(sizeof(queue_node)); // allocate memory for the new node
    newnode->item = item; // add item to new node
    newnode->number = 0;
    newnode->next = NULL; // intialize next and prev
    return newnode; // return the new node to calling function
}


extern void enqueue(queue_t *queue, char item) // adds an element to the queue
{
    // Name: enqueue
    // Input Parameters: a queue and a character
    // Output: places an integer on the queue
    // Purpose: to add elements in the queue
    int flag; // flag to check if memory is full
    queue_node *newnode = Qgetnewnode(item, &flag); // get new node to be inserted into queue
    if(flag != 1) // if memory is not full insert on to queue
    {
        if (Qisempty(*queue)) queue->front = newnode; // if list is empty set front pointer to new node
        else queue->rear->next = newnode; // if list is not empty change rear next to new node
        queue->rear = newnode; // set rear to new node either way
    }
    else cout << "Memory is full had to stop breadth-first traversal at " << item << endl; // if memory is full notify user
}

extern char dequeue(queue_t *queue) // removes an element from the queue
{
    // Name: dequeue
    // Input Parameters: a queue
    // Output: removes a character from a queue
    // Purpose: to remove elements from a queue
    char item; // item removed from the queue
    if(!Qisempty(*queue)) // if the list is not empty remove an element
    {
        queue_node *remove = queue->front; // node to be removed from the queue
        item = remove->item; // item removed from the queue and returned to calling function
        if (queue->front == queue->rear) queue->rear = NULL; // if front equals rear last element on the list set rear to NULL
        queue->front = queue->front->next; // set front to front next
        free(remove); // free memory allocated for the node
    }
    else item = '\0'; // if the list is empty let the calling function know
    return item; // send item to calling function
}
