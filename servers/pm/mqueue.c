#include "mqueue.h"
#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>   

int main()  
{  
    mqueue *que; 
    char *str="test1";
    char *str1="test2";
	char *str2="test3";
	char *str3="test4";
	char *str4="test5";
    void *value;

    initQueue(&que);  
    push(str, que);
    push(str1, que);
	push(str2, que);
	push(str3, que);
	push(str4, que);
	
    //
    pull(&value, que);
    printf("%s",value);
	pullindex(&value, que, 3);
	printf("%s",value);

    getchar();
    return 0;  
}  

void initQueue(mqueue **que)  
{  
    (*que) = (mqueue *)malloc(sizeof(mqueue));            
    (*que)->head = NULL;        
    (*que)->tail = NULL;  
    (*que)->num = 0;
    return;  
}  
/* 
push item into queue
 */  
int push(void *item, mqueue *que)  
{  
    struct node *newNode;
    newNode=(struct node *)malloc(sizeof(struct node));
    if(newNode==NULL)
    {
        printf("Fail！ ");
        exit(1);
    }
    if(isFull(que))  
    {  
        printf("Queue is full!\n");  
        return false;  
    }  
    else  
    {  
        newNode->data=item; 
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
    return 0;
}  
/* 
pull out first item from the queue
 */  
int pull(void **item, mqueue *que)  
{  
    struct node *p;
    if(isEmpty(que))  
    {  
        printf("Queue is empty!\n");  
        return false;  
    }  
    else  
    {  
        *item = que->head->data;    
        p=que->head;
        que->head=p->nextNode;  
        if(que->head==NULL)
        {
            que->tail=NULL;
        }
        return true;  
    }  
}  
int pullindex(void **item, mqueue *que, int index)
{
	int i;
	struct node *p;
	if(isEmpty(que))  
    {  
        printf("Queue is empty!\n");  
        return false;  
    } 
	else if(index>(que->num))
	{
		printf("wrong index!");
		return false;
	}
	else
	{
		for(i=0; i<index; i++)
		{
			*item = que->head->data;    
			p=que->head;
			que->head=p->nextNode;
			if(que->head==NULL)
			{
				que->tail=NULL;
			}
		}
	}
}
/* 
if queue is full 
 */  
int isFull(mqueue * que)  
{  
    if((que->num) ==MQUEUESIZE)     
        return true;  
    else  
        return false;  
}  
/* 
if queue is empty
 */  
int isEmpty(mqueue * que)  
{  
    if(que->head == NULL)  
        return true;  
    else  
        return false;  
}
