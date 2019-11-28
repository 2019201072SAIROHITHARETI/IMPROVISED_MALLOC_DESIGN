#include <bits/stdc++.h>
#include <stdint.h>
#include"header.h"
using namespace std;
int main()
{
	if(!Mem_Init(4096))
	{
		cout<<"init failed";
		return 1;
	}

	//testcase to show partial free
	int i;
	int *ptr1=(int*)Mem_Alloc(sizeof(int)*4);
	for(i=0;i<=3;i++)
		ptr1[i]=i+1;
	ptr1[i]=20;
	for(i=0;i<=3;i++)
	printf("%d\n",ptr1[i]);
	Mem_Free(ptr1+4);//it doesnt allow u to free unallocated memory..
	printf("initial size:%d\n",Mem_GetSize(ptr1));
	Mem_Free(ptr1+2);//It frees from 2nd location and updates size of ptr1 as 8bytes from 16 bytes...
	printf("size after free:%d\n",Mem_GetSize(ptr1));
	Mem_Free(ptr1);

	//test case to show complete free..
	int *ptr3=(int*)Mem_Alloc(sizeof(int)*10);
	for(i=0;i<=9;i++)
		ptr3[i]=i+1;
	for(i=0;i<=9;i++)
	printf("%d\n",ptr3[i]);
	printf("initial size:%d\n",Mem_GetSize(ptr3));
	Mem_Free(ptr3);//It frees entire memory
	printf("size after free:%d\n",Mem_GetSize(ptr3));//returns -1 saying that there is no such memory pointing by that pointer any more

	




	garbage_collector();
	return 0;
}
