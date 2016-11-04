#include "mqueue.h"
#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>   

/* private methods */
int isempty(mqueue * que);                  
int enqueue(void *item, mqueue * que);        
int dequeue(void **item, mqueue * que);
int iterator(mqueue * que);
int next(void **item, mqueue *que);
int getitem(void **item, mqueue *que);
static struct node *cur;            /* current */

void initqueue(mqueue **que)  
{  
    (*que) = (mqueue *)malloc(sizeof(mqueue));            
    (*que)->head = NULL;        
    (*que)->tail = NULL;  
    (*que)->num = 0;
    
    /*Bind methods*/
    (*que)->isempty = isempty;
    (*que)->enqueue = enqueue;
    (*que)->dequeue = dequeue;
    (*que)->iterator = iterator;
    (*que)->next = next;
    (*que)->getitem = getitem;
    return;  
}  
/* 
enqueue item into queue
 */  
int enqueue(void *item, mqueue *que)  
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
        que->num++;
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
int dequeue(void **item, mqueue *que)  
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

int next(void **item, mqueue *que)
{
	int i;
	struct node *p;
	if(isempty(que))  
    {  
        return false;  
    } 
}
/* 
if queue is empty
 */  
int isempty(mqueue * que)  
{  
    if(que->head == NULL)  
        return true;  
    else  
        return false;  
}

