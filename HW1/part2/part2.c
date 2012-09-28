/**
 * Varun Kohli (vkohli), Rishabh Singh (rasingh), Carlos Gil (cgil)
 */

#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
  int i, j;
  int foundInner;
  int result = 0;
  
  for (i = 0; i < len; i++) {
    foundInner = 0;
    for (j = 0; j < len; j++) {
      if (i == j) {
	continue;
      }
      if (arr[i] == arr[j]) {
	foundInner = 1;
      }
    }
    if (foundInner != 1) {
      result = arr[i];
    }
  }
  
  return result;
}
#endif

#ifdef OPTIMIZE1
/**
 *  Basic algorithm: Use the arithmetic series summation
 *  equation to get the theoretical sum for all n numbers.
 *  multiply by 2, giving us what we'd have if we summed
 *  all 2n numbers and subtract each number in the array
 *  from this sum. This will return the one non-repeating 
 *  digit. 
 *
 *  Runtime: O(n), Space: O(1).
 */
int oddball(int *arr, int len) {
  int sum, n, i;

  // gives us the n value
  n = (len + 1)/2;
  
  // 2x the theoretical sum of all elements 1 to n
  sum = n*(n+1);

  //goes through subtracting each num from the sum
  for (i = 0; i < len; i++) {
    sum = sum - arr[i];
  }

  return sum;
}
#endif

#ifdef OPTIMIZE2
/**
 * This optimization uses the property of xor. If you
 * xor a number with itself, you get 0. Furthermore, if you
 * have three numbers, say 1, 2, and 2, and you xor them in
 * order, you get 1^2 =3, 3^2 = 1. So, if we extend this principle
 * we get our final number to be the singlet as it was the only
 * one that was not zeroed. 
 *
 * Runtime O(n), Space O(1).
 */
int oddball(int *arr, int len) {
  int a = 0;
  int i;

  //loop through xor-ing each num together
  for (i = 0; i < len; i++) {
    a = a ^ arr[i];
  }

  return a;
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */
#ifndef RNG
int randGenerator(int *arr, int len) {
  int i, j, r, rcount;
  for (i = 0; i < len; i++) {
    do {
      rcount = 0;
      r = rand()%(len/2 + 1) + 1;
      for (j = 0; j < i && rcount < 2; j++) {
	if (arr[j] == r) {
	  rcount++;
	}
      }
    } while(rcount >= 2);
    arr[i] = r;
    printf("%d ", r);
  }
  printf("\nDone generating\n");
  return 0;
}

#else
//used the given randGenerator...
int randGenerator(int *arr, int len) {
  /* Put your code here */
  int i, j, r, rcount;
  for (i = 0; i < len; i++) {
    do {
      rcount = 0;
      r = rand()%(len/2 + 1) + 1;
      for (j = 0; j < i && rcount < 2; j++) {
	if (arr[j] == r) {
	  rcount++;
	}
      }
    } while(rcount >= 2);
    arr[i] = r;
    printf("%d ", r);
  }
  printf("\nDone generating\n");
  return 0;
}
#endif


