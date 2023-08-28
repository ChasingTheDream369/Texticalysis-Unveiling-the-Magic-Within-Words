#include<stdio.h>
#include<stdlib.h>

int compare (const void* p1, const void* p2);
void printArr(int arr[], int n);

  
// Driver program to test above function 
int main() 
{ 
    int arr[] = {22, 34, 12, 2, 3, 9, 5, 7, 8,10,490,4934029,2343,3949,43}; 
  
    int size = sizeof(arr) / sizeof(arr[0]); 
    qsort((void*)arr, size, sizeof(arr[0]), compare); 
  
    printf("Output array is\n"); 
    printArr(arr, size); 
  
    return 0; 
} 

int compare (const void* p1, const void* p2)
{ 
   int i1 = *(int*) p1;
   int i2 = *(int*) p2;
   if (i1 < i2) return -1;
   else if (i1 == i2) return 0;
   else return 1;
   /* or simply: return i1 - i2; */
 }

 void printArr(int arr[], int n) 
{ 
    int i; 
    for (i = 0; i < n; ++i) 
        printf("%d ", arr[i]); 
} 