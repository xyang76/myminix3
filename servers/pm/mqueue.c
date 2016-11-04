#include "mqueue.h"
#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>   

/* private methods */
static int isempty(mqueue * que);                  
static int enqueue(void *item, mqueue * que);        
static int dequeue(void **item, mqueue * que);
static int iterator(mqueue * que);
static int next(void **item, mqueue *que);
static int removeitem(mqueue *que);
static int hasvalue(void *item, mqueue * que);

/* Bind methods */
queue_func.isempty = isempty;
queue_func.enqueue = enqueue;
queue_func.dequeue = dequeue;
queue_func.iterator = iterator;
queue_func.next = next;
queue_func.removeitem = removeitem;
queue_func.hasvalue = hasvalue;

void initqueue(mqueue **que)  
{  
    (*que) = (mqueue *)malloc(sizeof(mqueue));            
    (*que)->head = NULL;        
    (*que)->tail = NULL;  
    (*que)->size = 0;
    return;  
}  
/* 
enqueue item into queue
 */  
static int enqueue(void *item, mqueue *que)  
{  
    struct node *newNode;
    newNode=(struct node *)malloc(sizeof(struct node));
    if(newNode==NULL)
    {
        return false;
    }
    else  
    {  
        newNode->value=item; 
        newNode->nextNode = NULL; 
        que->size++;
    }  
    if(que->tail==NULL)
    {
        que->head=que->tail=newNode;
    }else
    {
        que->tail=que->tail->nextNode=newNode;
    }
    return true;
}  
/* 
dequeue out first item from the queue
 */  
static int dequeue(void **item, mqueue *que)  
{  
    struct node *p;
    if(isempty(que))  
    {  
        return false;  
    }  
    else  
    {  
        *item = que->head->value;    
        p=que->head;
        que->head=p->nextNode;  
        if(que->head==NULL)
        {
            que->tail=NULL;
        }
        return true;  
    }  
}  

static int iterator(mqueue * que){
    que->cur = que->head;
    que->prev = NULL;
}

static int next(void **item, mqueue *que)
{
	int i;
	struct node *p;
    
	if(que->cur == NULL){  
        (*item) = NULL;
        return false;  
    } 
    (*item) = que->cur->value;
    que->prev = que->cur;
    que->cur = que->cur->nextNode;
    return true;
}

static int removeitem(mqueue *que){
    if(que->cur == NULL){
        return false;
    }
    if(que->prev != NULL){
        que->prev->nextNode = que->cur->nextNode;
    }
    free(que->cur);
    return true;
}

static int hasvalue(void *item, mqueue *que){
    struct node *p = que->head;
    while(p != NULL){
        if(p->value == item){
            return true;
        }
        p++;
    }
    return false;
}

/* 
if queue is empty
 */  
static int isempty(mqueue * que)  
{  
    if(que->head == NULL)  
        return true;  
    else  
        return false;  
}

void closequeue(mqueue *que){
    struct node *c, *p;
    
    c = que->head;
    while(c != NULL){
        p = c;
        c = c->nextNode;
        free(p);
    }
    free(que);
}
