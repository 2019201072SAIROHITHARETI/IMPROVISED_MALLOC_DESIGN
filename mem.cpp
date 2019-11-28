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
	    int i;
	for(i=1;i<=counter_freenode;i++)
	{	
		if(node_biggest->size>=size){
			break;
		}
		
		node_biggest--;
	}
	    if(i==counter_freenode+1)
	    {
		    cout<<"No big slot"<<endl;
		    return NULL;
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
int Mem_Free(void *pointer1)
{
	if(pointer1==NULL){
		printf("invalid free memory pointer");
		return -1;
	}


	if(!Mem_IsValid(pointer1))
	{	
		cout<<"Not a valid pointer\n";
		return -1;
	}	
	node_memory *node_current=starthead_alloc;
	unsigned long long int low,high;
	
	for(int i=0;i<counter_allocnodes;i++)
	{
		
		low=(uintptr_t)node_current->data;
		high=low + node_current->size;

		if((uintptr_t)low<=(uintptr_t)pointer1 && (uintptr_t)pointer1<(uintptr_t)high)
			break;
		node_current++;	
	}
	int actual_size=node_current->size;
	node_current->size=(uintptr_t)pointer1 - (uintptr_t)low;
	
	node_memory *newfree;
	if(node_current->size>=1)
	{	
		newfree=end_free;											
		newfree--;
		newfree->size = actual_size - node_current->size;
	}

	else
	{
		newfree=end_free-1; 
		newfree->size = actual_size;
		node_current->size=end_alloc->size;
		node_current->data=end_alloc->data;
		end_alloc--;
		counter_allocnodes--;
	}	
	newfree->data=pointer1;
	counter_freenode++;
	end_free=newfree;
	remaining_space+=newfree->size;

    //merging_left
    node_memory *pointer=(node_memory*)newfree;
	low=(uintptr_t)pointer->data;
	high=low + pointer->size;
	node_current=starthead_free;
	int high1;
	for (int i = 0; i < counter_freenode; ++i)
	{
		high1=(uintptr_t)node_current->data + node_current->size;
		if(high1 == low)	
		{
			node_current->size+=pointer->size;	
			end_free++;
			counter_freenode--;
			break;
		}	
		node_current--;
	}
    //merging rights
	pointer=(node_memory*)newfree;
	low=(uintptr_t)pointer->data;
	high=low + pointer->size;
	node_current=starthead_free;
	int low1;
	for (int i=0; i<counter_freenode; ++i)
	{
		low1=(uintptr_t)node_current->data;
		if(high == low1)	
		{
			pointer->size+=node_current->size;
			node_current->size=end_free->size;
			node_current->data=end_free->data;
			end_free++;
			counter_freenode--;
			break;
		}	
		node_current--;
	}
	counter_freenode=0;
	node_memory *temp=starthead_free;
	if(temp==end_free){
		counter_freenode=1;
	}
	else
	{
	while(temp!=end_free){
		counter_freenode++;
		temp--;
	}
	}

	return 1;
}

void garbage_collector()
{
	sbrk(-garbage_size);
	cout<<"Memory allocated clear."<<endl;
}

void *reallo(void *ptr){

	node_memory *current_node=starthead_alloc;
	for(int i=0;i<counter_allocnodes;i++){
		uintptr_t n1=(uintptr_t)current_node->data;
		if(n1==(uintptr_t)ptr)	
		{
			size_realloc_data=2*current_node->size;
			break;
		}	
		current_node++;
	}
	void *newpointer=Mem_Alloc(size_realloc_data);
	memcpy(newpointer,ptr,size_realloc_data/2);
	Mem_Free(ptr);

	return newpointer;

}

//using templates to write for all data types
template <typename T>
void writ(void *ptr,T data){

	node_memory *current_node=starthead_alloc;
	size_data=sizeof(data);
	bool flag=false;
	
	for(int i=0;i<counter_allocnodes;i++){
       

		char* low=(char*)current_node->data;
		char* high=low+current_node->size;
		char* ptr1=(char*)ptr;
		if(low<=ptr1 && ptr1<high){
			T *ptr2;
			if(size_data<=(high-ptr1)*sizeof(char)){
              ptr2=(T *)ptr;
			  flag=true;
			  *ptr2=data;
              
			}
			else{
              printf("ur croosing ur boundry");
			  exit(1);
              
			}

		
		}

		current_node++;
	}

	if(flag==false){
			printf("segment_fault");
		}	
}

void *callo(int number,int size){

	void *ptr=Mem_Alloc(number*size);
	if(ptr==NULL){
         return NULL;
	}
	char *low=(char*)ptr;
	char *high=low+number*size;
	for(int i=0;i<high-low;i++){
		*(low+i)=0;
	}
	return ptr;
}

int Mem_GetSize(void *ptr){
   node_memory *current_node=starthead_alloc;
   bool flag=false;

	for(int i=0;i<counter_allocnodes;i++){
		uintptr_t n1=(uintptr_t)current_node->data;
		if(n1==(uintptr_t)ptr)	
		{
			size_realloc_data=current_node->size;
			flag=true;
			break;
		}	
		current_node++;
	}
	if(flag==false){
		return -1;
	}
	return size_realloc_data;
}

