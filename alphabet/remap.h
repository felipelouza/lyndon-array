#ifndef REMAP_H
#define REMAP_H

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

#include "lib/utils.h"
#include "lib/file.h"

#ifndef DEBUG
  #define DEBUG 0 
#endif

#ifndef CAT 
  #define CAT 0 
#endif

/*******************************************************************/

typedef struct{
  unsigned char symbol;
  int run;
} t_rle;

int compare_rle(const void * left, const void * right);

typedef struct{
  unsigned char c1, c2;
  int freq;
} t_2mer;

int compare_2mer(const void * left, const void * right);

typedef struct{
  unsigned char symbol;
  int freq;
} t_symbol;

int compare(const void * left, const void * right);

int rle(unsigned char* str, int n, t_rle *A, int *C);

/*******************************************************************/

int count(unsigned char* str, int n, t_symbol *A, int *C);
int count_2mer(unsigned char* str, int n, t_2mer A[255][255], int *C);

int most_frequent(unsigned char* str, int n);
int less_frequent(unsigned char* str, int n);

int break_2mer(unsigned char* str, int n);
int extend_2mer(unsigned char* str, int n);

int most_rle(unsigned char* str, int n);
int less_rle(unsigned char* str, int n);

/*******************************************************************/

#endif
