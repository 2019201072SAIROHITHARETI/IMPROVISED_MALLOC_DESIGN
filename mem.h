#include "header.h"
using namespace std;


void *pointer=NULL;
int counter_freenode=1;	
int counter_allocnodes=0;
int remaining_space;	
int garbage_size=0;

struct node_memory
{
	void *data;
	int size;
};
node_memory *end_free=NULL;
node_memory *end_alloc=NULL;

struct node_memory *starthead_free = NULL;
struct node_memory *starthead_alloc = NULL;
int size_realloc_data=0;
int size_data=0;


//returns 1 if falls inn range else retun 0
int Mem_IsValid(void *user_pointer)
{
	node_memory *node_current=starthead_alloc;
	unsigned long long int low,high;
	for(int i=0;i<counter_allocnodes;i++)
	{
		low=(uintptr_t)node_current->data;
		high=low + node_current->size;
		if((uintptr_t)low<=(uintptr_t)user_pointer && (uintptr_t)user_pointer<(uintptr_t)high)
			return 1;
		node_current++;
	}
	return 0;
}

int Mem_Init(int total_region)
{	
if(total_region<=0)
    {	
    	cout<<"invalid size of region";
    	return -1;
    	
    }
    garbage_size=total_region;
    int fd=open("/dev/zero", O_RDWR);
    pointer=mmap(NULL,7*total_region,PROT_READ | PROT_WRITE ,MAP_PRIVATE,fd,0);
    if (pointer == MAP_FAILED) { //error checking
       cout<<"mmap failed to execute";
        return -1;
    } 
    remaining_space=total_region;
    starthead_free=(node_memory*)(pointer+7*total_region - sizeof(node_memory));
    starthead_alloc=(node_memory*)(pointer + total_region);
    starthead_alloc->size=0;
    starthead_free->data=pointer;
    starthead_free->size=total_region;
    end_free=starthead_free;
    end_alloc=starthead_alloc;
    close(fd); 
    return 1;    
}
 

void *Mem_Alloc(int size)
{
	if(size<=0){
		printf("your size is invalid");
		return NULL;
	}
	if(size>remaining_space)
	{
		cout<<"not enough memory\n";
		return NULL;
	}	
	
	node_memory *node_biggest=starthead_free;
	node_memory *node_current=node_biggest;
	void *userpointer=node_biggest->data;
	int biggestsize=node_biggest->size;

	int temporary=rand()%2;
	printf("%d",temporary);
	//worst fit
	if(temporary==0){
	printf("using worst fit");
	printf("\n");
	for(int i=1;i<=counter_freenode;i++)
	{	
		if(node_biggest->size < node_current->size)
			node_biggest=node_current;
		
		node_current--;
	}
    }
    //first fit
    else{
    printf("using first fit");
    printf("\n");
	for(int i=1;i<=counter_freenode;i++)
	{	
		if(node_biggest->size < node_current->size){
			node_biggest=node_current;
			break;
		}
		
		node_current--;
	}
    }
	
	userpointer=node_biggest->data;
	node_memory *newallo;
	
	if(size<node_biggest->size)
	{	
		
		node_biggest->size-=size;
		node_biggest->data+=size;
		
		if(starthead_alloc->size!= 0)
			newallo=end_alloc +1;
		
		else
			newallo=starthead_alloc;
		
		newallo->size=size;
		newallo->data=userpointer;
		counter_allocnodes++;
		end_alloc=newallo;
		remaining_space-=size;
	}
	else if(size==node_biggest->size)
	{
		
		if(starthead_alloc->size!= 0)
			newallo=end_alloc+1;
		else
			newallo=starthead_alloc;

		newallo->size=size;
		newallo->data=userpointer;
		counter_allocnodes++;
		end_alloc=newallo;
		remaining_space-=size;
		
		if(node_biggest!=end_free)
		{
			node_biggest->data=end_free->data;
			node_biggest->size=end_free->size;
		}
		
		end_free++;	
		if(counter_freenode>1)
			counter_freenode--;
		else if(counter_freenode==1)
		{
			starthead_free->data=NULL;
			starthead_free->size=0;
		}	

		
	}	
	else	
	{
		cout<<"not enough memory\n";
		return NULL;
	}	
	return userpointer;
}
