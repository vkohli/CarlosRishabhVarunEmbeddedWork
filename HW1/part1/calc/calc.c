/**
 * Group 14: Varun Kohli (vkohli), Rishabh Singh 
 * (rasingh), Carlos Gil (cgil). 
 * Calc.c - Parses input and calls Math functions to
 * calculate the output.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

/**
 * parseOp - takes in the input and parses the string
 * into a formula. Also makes sure that the expression
 * is valid.
 */
void parseOp(char* inputbuf) {
  int i = 0; 
  int a, b;
  char* word;
  char *buf[3];
  int ret;
 
  //for loop that splits string by the spaces
  for(word = strtok(inputbuf, " ");
      word != NULL;
      word = strtok(NULL, " "))
    {
      //exit if more than three args
      if (i == 3) exit(0);
      buf[i] = word;
      i++;
    }
  //exit if less than 3 args provided
  if (i < 3) exit(0);
  
  a = atoi(buf[0]);
  b = atoi(buf[2]);

  //exit if non-numbers passed through
  if ((a == 0 && !strcmp(buf[0],"0")) ||
      (b == 0 && !strcmp(buf[2],"0"))) {
    exit(0);
  }
  
  if (strcmp(buf[1],"+") == 0) {
    ret = add(a,b);
    printf("%d\n", ret);
    return;
  }
  else if (strcmp(buf[1], "-") == 0) {
    ret = sub(a,b);
    printf("%d\n", ret);
    return;
  }
  else if (strcmp(buf[1], "*") == 0) {
    ret = mul(a,b);
    printf("%d\n", ret);
    return;
  }
  else if (strcmp(buf[1], "/") == 0 && b != 0) {
    ret = mydiv(a, b);
    printf("%d\n", ret);
    return;
  }
  else if (strcmp(buf[1], "%") == 0 && b != 0) {
    ret = mod(a,b);
    printf("%d\n", ret);
    return;
  }
  /*	exit if non-valid operator passed through
	or if divide by 0 error                */
  else
    exit(0);
}

int main() {
  while(1) {
    char inputbuf[32];
    read(0, inputbuf, 32);
    parseOp(inputbuf);
  }
  return 0;
}
