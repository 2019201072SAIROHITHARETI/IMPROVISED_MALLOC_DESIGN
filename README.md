# IMPROVISED_MALLOC_DESIGN
OS PROJECT

TEAM MEMBERS:
AKLESH MISHRA(2019201073)
HARIDATTA(2019201010)
KURUVA JAYA KRISHNA(2019201076)
SAI ROHITH ARETI(2019201072)

DEFINED MODULES:

int Mem_Init(int total_region):
Mem_Init is called one time by a process using your routines. total_region is the number of bytes that you should request from the OS using mmap.

void *Mem_alloc(int size):
Mem_Alloc() is similar to the library function malloc(). Mem_Alloc takes as input the size in bytes of the object to be allocated and returns a pointer to the start of that object. The function returns NULL if there is not enough free space within total_region allocated by Mem_Init to satisfy this request. 

void *reallo(void *ptr):
we double the given pointer size and free the already allocated pointer remove from alloclist and add to free list.And return the newly allocated pointer to user.Its entry is maintained in the alloclist.

void writ(void *ptr,T data):
To handle the case of writing out of bound of allocated variable size.Its is basically api that handles such cases..

void *callo(int number,int size):
Similar to that of Mem_alloc and also Initialize allocated memory with zeros. 

int Mem_GetSize(void *ptr):
If ptr falls within the range of a currently allocated object, then this function returns the size in bytes of that object; otherwise, the function returns -1.

int Mem_IsValid:
This function returns 1 if ptr falls within a currently allocated object and 0 if it does not.

int Mem_Free:
Mem_Free frees the memory object that ptr falls within, according to the rules described above. Just like with the standard free(), if ptr is NULL, then no operation is performed. The function returns 0 on success and -1 if ptr to does not fall within a currently allocated object


memtemp.h contains all the extended fuctionalites that handles dangling pointer and memory leak(function type).And we used random fit which is collection of worst fit and first fit.It has APIs to read,write,copy.All error handling are done. 
