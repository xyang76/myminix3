#include<stdio.h>  
#include<malloc.h>     
#include"mqueue.h"


int main(void)  
{  
    mqueue *que; 
    char *str="test1";
    char *str1="test2";
    void *value;

    initQueue(&que);  
    push(str, que);
    push(str1, que);
	
    pull(&value, que);
    printf("%s\n", value);
    pull(&value, que);
    printf("%s\n", value);
  
    getchar();  
    return 0;  
}  

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
int push(void *item, mqueue *que)  
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
int pull(void **item, mqueue *que)  
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