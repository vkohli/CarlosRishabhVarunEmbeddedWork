#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int oddball(int *arr, int len);
int randGenerator(int *arr, int len);
void verify(int *arr, int len);

#ifndef N
	#define N (40)
#endif

#define ARRLEN (2*(N) - 1)

int main()
{
  /* Keep this line here */
  srand(time(NULL));
  
  int *arr = malloc(ARRLEN*sizeof(int));
  int num;
  
  if (!arr) {
    return -1;
  }
  
  randGenerator(arr, ARRLEN);
  verify(arr, ARRLEN);
  num = oddball(arr, ARRLEN);

  printf("Oddball is %d\n", num);

  return 0;
}

/** 
 *  Basically used to confirm that the array generated was
 *  in fact correct, and then also find the single method
 *  via brute force to compare to our optimized outputs.
 */
void verify(int *arr, int len) {
  int i, j;
  int *temparr = calloc(N+1,sizeof(int));
  int singleCount = 0, singleMem = 0;

  for (i = 0; i < len; i++) {
    temparr[arr[i]]++;
  }
  for (j = 1; j < N+1; j++) {
    if (temparr[j] == 1) {
      singleCount++;
      singleMem = j;
    }
    else if (temparr[j] == 2) 
      continue;
    else { 
      printf("Incorrectly generated array 1\n");
      return;
    }
  }
  if (singleCount != 1) {
    printf("Incorrectly generated array 2\n");
    return;
  }
  printf("The single number is %d\n", singleMem);
  return;
}
