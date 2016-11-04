#define MQUEUESIZE 200
#define true 1
#define false 0

typedef int (*queue_f0)(mqueue * que);              /* only 1 item */       
typedef int (*queue_f1)(void *item, mqueue * que);  /* not need &return */       
typedef int (*queue_f2)(void **item, mqueue * que); 

struct node
{
    void *value;
    struct node *nextNode; 
};

typedef struct 
{  
    int size;      
    int number;
    struct node *head;   
    struct node *tail;  
    struct node *cur;
    struct node *prev;
    queue_f0 isempty;
    queue_f1 enqueue;
    queue_f2 dequeue;
    queue_f0 iterator;
    queue_f2 next;
    queue_f0 remove;
} mqueue;  

void initqueue(mqueue ** que);                
void closequeue(mqueue *que);
