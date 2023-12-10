#include <stdio.h>
#include <stdlib.h>

//int* ptr = malloc(20 * sizeof(int));

int main() 
{
  int* ptr1 = (int*)malloc(6 * sizeof(int));
  int* ptr2 = (int*)malloc(6 * sizeof(int));
  int* ptrx = ptr1;
  for(int i = 0; i < 6; i++)
    printf("%x ", ptr1[i]);
  free(ptr1);
  printf("\n");
  for(int i = 0; i < 6; i++)
    printf("%x ", ptrx[i]);
  int* ptr3 = (int*)malloc(6 * sizeof(int));
  
  printf("%x\n%x\n%x\n", ptr1, ptr2, ptr3);
}
