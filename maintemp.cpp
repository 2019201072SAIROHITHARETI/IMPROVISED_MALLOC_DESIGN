#include "memtemp.h"
using namespace std;

// void function_new(){
// 	int *ptr3=(int*)memory_allocation2("ptr3",100,sizeof(int));
// 	int *ptr2=(int*)memory_allocation2("ptr2",100,sizeof(int));
// 	checklist();
// 	cout<<allocnodes_data()<<endl;

// }


int main()
{
	if(!memory_initalization(4096))
	{
		cout<<"init failed";
		return 1;
	}

     // int *ptr1=(int*)memory_allocation2("ptr1",100);
     // int *ptr2=(int*)memory_allocation2("ptr2",100);
     // int *ptr3=(int*)memory_allocation2("ptr3",100);
     // checklist();
     // cout<<endl;
     // memory_free2("ptr1",ptr1);
     // checklist();


	 //dangling pointer testcase..
	 // int *ptr1=(int*)memory_allocation2("ptr1",100,sizeof(int));
	 // int *ptr2=(int*)memory_allocation2("ptr2",100,sizeof(int));
	 // void *ptr3=copy("ptr3",ptr2);
	 // checklist();
	 // ptr2=(int*)copy("ptr2",ptr1);
	 // ptr3=(int*)copy("ptr3",ptr1);
	 // checklist();
	 // cout<<endl;
	 // memory_free2("ptr1",ptr1);
	 // checklist();
 

     //dangling pointer testcase 2
     // int *ptr1=(int*)memory_allocation2("ptr1",100,sizeof(int));
     //  void *ptr3=copy("ptr3",ptr1);
     //  void *ptr4=copy("ptr4",ptr1);
     //   void*ptr2=memory_allocation2("ptr2",100,sizeof(int));
     //  void *ptr5=copy("ptr5",ptr2);
     //  checklist();
     //  cout<<endl;
     //  memory_free2("ptr5",ptr5);
     //  checklist();
     //  printf("after del\n");
     //  memory_free2("ptr1",ptr1);
     //  checklist();
     //  memory_free2("ptr1",ptr1);
     //  checklist();


	//testcacse 3--
	// int *ptr1=(int*)memory_allocation2("ptr1",sizeof(int)*4,sizeof(int));
	// void*ptr2=memory_allocation2("ptr2",sizeof(int),sizeof(int));
	// void*ptr3=memory_allocation2("ptr3",100,sizeof(int));
	// void *ptr4=copy("ptr4",ptr1);
	// checklist();
	// cout<<endl;
	// memory_free2("ptr1",ptr1+2);
	// checklist();




	//testcacse for write checking and read checking...
	// int *ptr1=(int*)memory_allocation2("ptr1",sizeof(int)*3,sizeof(int));
	//void*ptr2=memory_allocation2("ptr2",sizeof(int),sizeof(int));
	//int *ptr3=(int*)memory_allocation2("ptr3",sizeof(int),sizeof(int));
	//int *ptr4=(int *)copy("ptr4",ptr1);
	// checklist();
	//cout<<endl;
	//write("ptr3",ptr3,20);
	// if(read("ptr3",ptr3)){
	// 	cout<<*ptr3;
	// }

	// write("ptr3",ptr3+1,25);
	// if(read("ptr3",ptr3+1)){
	// 	cout<<*(ptr3+1);
	// }

 //    if(read("ptr1",ptr1+3)){
 //    	cout<<*(ptr1+3);
 //    }
	// checklist();
	// printf("next..");
	// memory_free2("ptr1",ptr1+2);
	// checklist();


 //memory leak test case...
	// int *ptr1=(int *)memory_allocation2("ptr1",sizeof(int)*3,sizeof(int));
	// int before=allocnodes_data();
	// function_new();
	// int after=allocnodes_data();
	// clear_nodes_after_function(before,after);
	// cout<<allocnodes_data()<<endl;
	// checklist_allocnodes();










	
	// int *ptr1=(int*)memory_allocation(0);
	// Mem_Free(ptr1);
	// writ<int>(ptr1,10);
	// void *ptr2=reallo(ptr1);
	// int temp=Mem_GetSize(ptr2);
	// printf("%d",*((int *)ptr2+0));
	// int *a=(int *)memory_allocation(sizeof(int)*4);
	// writ<int>(a,20);
	// writ<int>(a+1,30);
	// writ<int>(a+2,40);
	// writ<int>(a+3,40);
	// printf("%d",a[0]);
	// printf("%d %d %d",a[1],a[2],a[3]);
	// writ<int>(a+4,30);
	// printf("%d",a[4]);


	

	int i;
	char *v1=(char*)memory_allocation(sizeof(char)*8);
	char *v2=(char*)memory_allocation(sizeof(char)*8);
	char *v3=(char*)memory_allocation(sizeof(char)*8);
	char *v4=(char*)memory_allocation(sizeof(char)*(4096-24));
	for(i=0;i<8;i++)
	{
		v1[i]=v2[i]=v3[i]=12;
	}
	for(i=0;i<4096-24;i++)
	{
		v4[i]=12;
	}
	memory_free(v1);memory_free(v2);memory_free(v3);
	free_list_data();
	// char *abc=(char *)memory_allocation(sizeof(char)*24);
	// for(i=0;i<24;i++)
	// {
	// 	abc[i]=23;
	// 	printf("%d",abc[i]);
	// }
	// cout<<i;
	


	// printf("hii\n");
	// int *ar1=(int *)memory_allocation(sizeof(int));
	// writ<int>(ar1,12);
	// char *arr=(char *)memory_allocation(sizeof(char)*5);
	// writ<int>(arr+2,12);
	// printf("hii\n");

	//malloc
	//multithreads
	//security
	
	
	return 0;
}