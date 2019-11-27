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
 
