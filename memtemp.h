#include "headertemp.h"
#include<string>
using namespace std;


void *pointer=NULL;
int counter_freenode=1;	
int counter_allocnodes=0;
int counter_varaiblenodes=0;
int remaining_space;	
int garbage_size=0;

//structure of node_memory it is used for empty list store and allocated list store
struct node_memory
{
	void *data;
	int size;
};

//structure that is used for variable list that we are using it for double pointers 
struct variablelist
{
	string name;
	void *ptr;
	int size;
	int type_size;
};

node_memory *end_free=NULL;
node_memory *end_alloc=NULL;
variablelist *end_variablelist=NULL;

struct node_memory *starthead_free = NULL;
struct node_memory *starthead_alloc = NULL;
struct variablelist *starthead_variablelist=NULL;
int size_realloc_data=0;
int size_data=0;


//Is pointer in given range of variable or not..
int memory_isvalid(void *user_pointer)
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





int memory_initalization(int total_region)
{	
if(total_region<=0)
    {	
    	cout<<"invalid size of region";
    	cout<<endl;
    	return -1;
    	
    }
    garbage_size=total_region;
    int fd=open("/dev/zero", O_RDWR);
    pointer=mmap(NULL,8*total_region,PROT_READ | PROT_WRITE ,MAP_PRIVATE,fd,0);
    if (pointer == MAP_FAILED) { //error checking
       cout<<"mmap failed to execute \n";
        return -1;
    } 
    remaining_space=total_region;
    starthead_free=(node_memory*)(pointer+7*total_region - sizeof(node_memory));
    starthead_alloc=(node_memory*)(pointer + total_region);

    starthead_variablelist=(variablelist*)(pointer+8*total_region-sizeof(variablelist));

    starthead_alloc->size=0;
    starthead_free->data=pointer;
    starthead_free->size=total_region;
    end_free=starthead_free;
    end_alloc=starthead_alloc;
    end_variablelist=starthead_variablelist;
    close(fd); 
    return 1;    
}
 

void *memory_allocation(int size)
{
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




int memory_free(void *pointer1)
{
	if(pointer1==NULL)
		return -1;


	if(!memory_isvalid(pointer1))
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
	uintptr_t high1;
	for (int i = 0; i < counter_freenode; ++i)
	{
		high1=(uintptr_t)node_current->data + node_current->size;
		if(high1 == low)	
		{
			node_current->size+=pointer->size;	
			end_free++;
			counter_freenode--;
			cout<<counter_freenode;
			break;
		}	
		node_current--;
	}
    //merging rights
	pointer=(node_memory*)newfree;
	low=(uintptr_t)pointer->data;
	high=low + pointer->size;
	node_current=starthead_free;
	uintptr_t low1;
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
			cout<<counter_freenode;
			break;
		}	
		node_current--;
	}
	
	counter_freenode=0;
	 node_memory *temp=starthead_free;
	while(temp!=end_free){
		counter_freenode++;
		temp--;
	}
	if(temp==end_free){
		counter_freenode=1;
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
	void *newpointer=memory_allocation(size_realloc_data);
	memcpy(newpointer,ptr,size_realloc_data/2);
	memory_free(ptr);

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
              printf("\n");
			  exit(1);
              
			}

		
		}

		current_node++;
	}

	if(flag==false){
			printf("segment_fault");
			cout<<endl;
		}	
}

void *callo(int number,int size){

	void *ptr=memory_allocation(number*size);
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

int size_of_all(void *ptr){
   node_memory *current_node=starthead_alloc;

	for(int i=0;i<counter_allocnodes;i++){
		uintptr_t n1=(uintptr_t)current_node->data;
		if(n1==(uintptr_t)ptr)	
		{
			size_realloc_data=current_node->size;
			break;
		}	
		current_node++;
	}
	return size_realloc_data;
}

//improvised for double pointer handling..

void *memory_allocation2(string name,int number_of_bytes,int type_size){
	   void *temp=memory_allocation(number_of_bytes);
	    counter_varaiblenodes++;
	    end_variablelist->name=name;
		end_variablelist->size=number_of_bytes;
		end_variablelist->ptr=temp;
		end_variablelist->type_size=type_size;
		end_variablelist--;
		return temp;
}



void  memory_free2(string name,void *ptr){
	variablelist *temp=starthead_variablelist;
	void *pointer=NULL;
	int size=0;

	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp->name==name){
			pointer=temp->ptr;
			size=temp->size;
			break;
		}
		temp--;
	}

	if(pointer==NULL){
		cout<<"no such pointer"<<endl;
	}
	else{
		void* low=pointer;

	    void* high=low +size ;
	    // printf("low:%u high:%u\n",low,high);
	    // printf("size:%d\n",size);
	    // printf("ptr:%u\n",ptr);
	    if((uintptr_t)ptr==(uintptr_t)low){
	    	// printf("hello");
	    	variablelist *temp=starthead_variablelist;

	    	//iterate over the list and skip if same nodes are there in  list from end otherwise replace the temp with endvariable are there and 
	    	while(temp>end_variablelist){

	    		if(temp->ptr==ptr ){
	    			end_variablelist++;
	    			if(end_variablelist->ptr==ptr){
	    				continue;
	    			}
	    			else{
	    			temp->ptr=end_variablelist->ptr;
			        temp->size=end_variablelist->size;
			        temp->name=end_variablelist->name;
			        temp->type_size=end_variablelist->type_size;
			        temp--;
			      }
			     }
			     else{
			     	temp--;
			     }
	    	}

	    	//making count_variablenodes according to the changed list
	    	counter_varaiblenodes=0;
	    	temp=starthead_variablelist;
	    	while(temp!=end_variablelist){
	    		counter_varaiblenodes++;
	    		temp--;
	    	}
            
            //free the pointer..
	    	memory_free(ptr);
	    }
	    //we are checking in given range and then change the respective sizes and delete and free the pointer
	    if(ptr>low && ptr<high){
	    	int size_new=(char*)ptr-(char*)low;
	    	variablelist *temp=starthead_variablelist;
	    	while(temp!=end_variablelist){
	    		if((char*)temp->ptr==(char*)low){
	    			temp->size=size_new;
	    		}
	    		temp--;
	    	}
	    	memory_free(ptr);
	    }

	    //pointer is given higer than the range of that variable then error is displayed
	    if(ptr>=high)
	    {
	    	printf("u r trying to del some data which is not in ur boundaries..\n");
	    	return;
	    }


	}


}
void checklist(){
	variablelist *temp=starthead_variablelist;
	int i=0;
	//here we are just printing the data in allocated list
	for( i=0;i<counter_varaiblenodes;i++){
		cout<<temp->name<<" ";
		cout<<temp->ptr<<" ";
		cout<<temp->type_size<<" ";
		cout<<temp->size<<" ";
		temp--;
		cout<<endl;
	}

	if(i==0){
		printf("no allocation yet\n");
	}
}

bool ispointervalid(string name,void *ptr){
	variablelist *temp=starthead_variablelist;

	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp->name==name && memory_isvalid(ptr)){
		  return true;
	  }
	  temp--;
	}

	return false;

}


void *copy(string name,void *source_address){


	variablelist *temp=starthead_variablelist;
	variablelist *temp2=starthead_variablelist;
	void *dummypointer=NULL;
	variablelist *temp3=starthead_variablelist;
	int count=0;

    
    //we are finding the pointer of the given name in the list of variablenodes
	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp->name==name){
			// printf("hi\n");
			dummypointer=temp->ptr;
			break;
		}
		temp--;
	}

    //If pointer is not NULL we have an entry in it so we proceed
    if(dummypointer!=NULL){

    //now we are checking that given pointer is present then we are replacing all data in the oldnode with newnode data
	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp2->ptr==source_address){
			temp->size=temp2->size;
			temp->ptr=temp2->ptr;
			temp->type_size=temp2->type_size;
			break;
		}
		temp2--;

	}


	//if count=0 then we are removing the old pointer pointing address this ensures that 
	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp3->ptr==dummypointer){
			count++;
		}
		temp3--;
	}
	if(count==0){
		// printf("hello\n");
		memory_free(dummypointer);
	}
	return temp->ptr;
  }


  //if it is null we are just adding to endlist..
  else{
  	for(int i=0;i<counter_varaiblenodes;i++){
		if(temp2->ptr==source_address){
			end_variablelist->ptr=temp2->ptr;
			end_variablelist->size=temp2->size;
			end_variablelist->type_size=temp2->type_size;
			end_variablelist->name=name;
			counter_varaiblenodes++;
			end_variablelist--;
			break;
		}
		temp2--;

  }
return source_address;
}
}

template <typename T>
void write(string name,void *ptr,T data){
	variablelist *temp=starthead_variablelist;
	int flag=0;

	//while writing just iterate in variable list find name 
	while(temp!=end_variablelist){
		if(temp->name==name){
			flag=1;
			break;
		}
		temp--;
	}
	if(flag==0){
		cout<<"There is no such pointer to write"<<endl;
		return;
	}

	//if flag is made 1 then it means that we got a pointer with that name now check if ptr+type_size<=temp->ptr+temp->size
	if(flag==1){
		if(ptr+temp->type_size<=temp->ptr+temp->size){
			writ<T>(ptr,data);

		}
		else{
			cout<<"writing out of boundary access"<<endl;
		}
	}
	return;
}

bool read(string name,void *ptr){
	variablelist *temp=starthead_variablelist;
	int flag=0;
	//while writing just iterate in variable list find name 
	while(temp!=end_variablelist){
		if(temp->name==name){
			flag=1;
			break;

		}
		temp--;
	}

	//if flag is made 1 then it means that we got a pointer with that name now check if ptr+type_size<=temp->ptr+temp->size and return true if readable
	if(flag==0){
		cout<<"no such pointer to read"<<endl;
	}
	else{
		if(ptr+temp->type_size<=temp->size+temp->ptr){
			return true;
		}
		else{
			cout<<"reading out of boundary"<<endl;
		}
	}

	// now  return false
	return false; 

}


//memory leak code..
int allocnodes_data(){
	return counter_allocnodes;
}

void checklist_allocnodes(){
	node_memory *temp=starthead_alloc;
	for(int i=0;i<counter_allocnodes;i++){
		cout<<temp->data<<" ";
		cout<<temp->size<<" ";
		temp--;
	}
}

void clear_nodes_after_function(int before,int after){
	node_memory *temp=end_alloc;
	temp--;
	for(int i=after;i<before;i--){
		memory_free(temp->data);
		temp--;
	}
	temp++;
	end_alloc=temp;
	counter_allocnodes=before;

	return ;
}

void free_list_data(){
	node_memory *temp=starthead_free;
	int i=0;
	for( i=0;i<counter_freenode;i++){
		if(counter_freenode==0){
		cout<<"no data in list";
		break;
	   }
		cout<<temp->data<<" ";
	 	cout<<temp->size<<" ";
		temp++;

	}


}










