#include<stdio.h>  
#include<malloc.h>     
#include"mqueue.h"

void initQueue(mqueue **que)  
{  
    (*que) = (mqueue *)malloc(sizeof(mqueue));            
    (*que)->head = 0;        
    (*que)->tail = 0;  
    (*que)->list = (void **)malloc(sizeof(void*)*MQUEUESIZE);
    return;  
}  
/* 
push item into queue
 */  
int enqueue(void *item, mqueue *que)  
{  
    if(isFull(que))  
    {  
        printf("Queue is full!\n");  
        return -1;  
    }  
    else  
    {  
        que->list[que->tail] = item;  
        que->tail = que->tail+1;  
        return 0;  
    }  
}  
/* 
pull out first item from the queue
 */  
int dequeue(void **item, mqueue *que)  
{  
    if(isEmpty(que))  
    {  
        printf("Queue is empty!\n");  
        return -1;  
    }  
    else  
    {  
        *item = que->list[que->head];        
        que->head = que->head+1;       
        return 0;  
    }  
}  

/* 
if queue is full 
 */  
int isFull(mqueue * que)  
{  
    if((que->tail+1) ==MQUEUESIZE)     
        return 1;  
    else  
        return 0;  
}  
/* 
if queue is empty
 */  
int isEmpty(mqueue * que)  
{  
    if(que->head == que->tail)  
        return 1;  
    else  
        return 0;  
}

void closeQueue(mqueue * que){
    free(que->list);
    free(que);
}