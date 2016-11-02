#define MQUEUESIZE 200
#define true 1
#define false 0
struct node
{
    void *data;
    struct node *nextNode; 

};

typedef struct 
{  
    int num;      
    struct node* head;   
    struct node* tail;     
} mqueue;  

void initQueue(mqueue ** que);                
int isEmpty(mqueue * que);                   
int isFull(mqueue * que);                  
int push(void *item, mqueue * que);        
int pull(void **item, mqueue * que);   