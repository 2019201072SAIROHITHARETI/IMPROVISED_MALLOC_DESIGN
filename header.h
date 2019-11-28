#include <bits/stdc++.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
int Mem_IsValid(void *user_pointer);
int Mem_Init(int total_region);
void *Mem_Alloc(int size);
int Mem_Free(void *pointer1);
void garbage_collector();
void *reallo(void *ptr);
void *callo(int number,int size);
int Mem_GetSize(void *ptr);
void merging_right(void *pointer1);
void merging_left(void *pointer1);
