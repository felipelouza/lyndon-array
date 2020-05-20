/*
 * Alphabet reordering
 *
 * Authors: Felipe A. Louza and Marcelo K. Albertini
 * contact: louza@ufu.br
 * 30/03/2020
 *
 */

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

int compare_rle(const void * left, const void * right) {
    const t_rle * a = (const t_rle *) left;
    const t_rle * b = (const t_rle *) right;
    if (a->run > b->run) {
        return -1;
    } else if (a->run < b->run) {
        return 1;
    } else {
        return 0;
    }
}
/*******************************************************************/

typedef struct{
  unsigned char c1, c2;
  int freq;
} t_2mer;

int compare_2mer(const void * left, const void * right) {
    const t_2mer * a = (const t_2mer*) left;
    const t_2mer * b = (const t_2mer *) right;
    if (a->freq > b->freq) {
        return -1;
    } else if (a->freq < b->freq) {
        return 1;
    } else {
        return 0;
    }
}

/*******************************************************************/

typedef struct{
  unsigned char symbol;
  int freq;
} t_symbol;

int compare(const void * left, const void * right) {
    const t_symbol * a = (const t_symbol *) left;
    const t_symbol * b = (const t_symbol *) right;
    if (a->freq > b->freq) {
        return -1;
    } else if (a->freq < b->freq) {
        return 1;
    } else {
        return 0;
    }
}

/*******************************************************************/
int rle(unsigned char* str, int n, t_rle *A, int *C){

  int i;

  //init
  for(i=0; i<255;i++){
    A[i].run=0;
    A[i].symbol=i;
  }

  //count
  i=0;
  while(i<n-1){
    int rle=1;
    while(str[i]==str[i+rle]) rle++;
//    printf("%d%c\n", rle, str[i]);
    if(A[str[i]].run<rle) A[str[i]].run =rle;
    i=i+rle;
  }

  int c=0;
  for(i=0; i<255;i++){
    if(A[i].run){
//      printf("%d\t%c\t%d\n", i, A[i].symbol, A[i].run);
      C[c++]=A[i].symbol;
    }
  }

  //sort
  qsort(A, 255, sizeof(t_rle), compare_rle);

  #if DEBUG
  for(i=0; i<255;i++)
    if(A[i].run)
      printf("%d\t%d\t%d\n", i, A[i].symbol, A[i].run);
  #endif

return c;
}

/*******************************************************************/
int count(unsigned char* str, int n, t_symbol *A, int *C){

  int i;

  //init
  for(i=0; i<255;i++){
    A[i].freq=0;
    A[i].symbol=i;
  }

  //count
  for(i=0; i<n-1;i++){
    A[str[i]].freq++;
  }

  int c=0;
  for(i=0; i<255;i++){
    if(A[i].freq){
      #if DEBUG
      printf("%d\t%d\t%d\n", i, A[i].symbol, A[i].freq);
      #endif
      C[c++]=A[i].symbol;
    }
  }

  //sort
  qsort(A, 255, sizeof(t_symbol), compare);

return c;
}

/*******************************************************************/
int count_2mer(unsigned char* str, int n, t_2mer A[255][255], int *C){

  int i,j;

  //init
  for(i=0; i<255;i++){
    for(j=0; j<255; j++){
      A[i][j].freq=0;
      A[i][j].c1=i;
      A[i][j].c2=j;
    }
  }

  //count
  for(i=0; i<n-1;i++){
    A[str[i]][str[i+1]].freq++;
  }

  int c=0;
  for(i=0; i<255;i++){
    int sum=0;
    for(j=0; j<255; j++){
      if(A[i][j].freq){
        sum++;
//        printf("%d\t%c%c\t%d\n", i, A[i][j].c1, A[i][j].c2, A[i][j].freq);
      }
    }
    if(sum)
      C[c++]=i; //A[i].symbol;
  }

  //sort
  qsort(A, 255*255, sizeof(t_2mer), compare_2mer);

  for(i=0; i<255;i++){
    for(j=0; j<255; j++){
      if(A[i][j].freq==0) break;
      #if DEBUG
      printf("%d\t%c%c\t%d\n", A[i][j].c2, A[i][j].c1, A[i][j].c2, A[i][j].freq);
      #endif
    }
    if(A[i][j].freq==0) break;
  }

return c;
}

/*******************************************************************/
int most_frequent(unsigned char* str, int n){

  t_symbol A[255];
  int C[255];

  int c = count(str, n, A, C);

  int i;
  int B[255];
  B[0]=0;//terminator symbol
  for(i=0; i<c;i++){
    B[A[i].symbol]=C[i];
    #if DEBUG
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}
/*******************************************************************/
int less_frequent(unsigned char* str, int n){

  t_symbol A[255];
  int C[255];

  int c = count(str, n, A, C);

  int i;
  int B[255], b=0;
  B[0]=0;//terminator symbol
  for(i=c-1; i>=0;i--){
    B[A[i].symbol]=C[b++];
    #if DEBUG
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}

/*******************************************************************/
int break_2mer(unsigned char* str, int n){

  t_2mer A[255][255];
  int B[255];
  int C[255];
  int D[255];

  int i,j;
  for(i=0; i<255;i++)
    B[i]=0;

  int c = count_2mer(str, n, A, C);
  #if DEBUG
  printf("Sigma = %d\n", c);
  #endif

  int d=0;
  for(i=0; i<255;i++){
    for(j=0; j<255; j++){
      if(A[i][j].freq==0) break;

      if(A[i][j].c1 == A[i][j].c2) continue; //ignore AA, CC, GG, TT

      if(!B[A[i][j].c2] && A[i][j].c2){
        D[d++] = A[i][j].c2;
        B[A[i][j].c2]=1;
      }
      
      if(!B[A[i][j].c1] && A[i][j].c1){
        D[d++] = A[i][j].c1;
        B[A[i][j].c1]=1;
      }
    }
    if(A[i][j].freq==0) break;
  }

  if(c!=d){
    for(i=0; i<c;i++){
      if(!B[C[i]]) D[d++] = C[i];
    }
  }
/*
  for(i=0; i<d;i++){
    printf("D['%d'] = '%c' (%d)\n", i, D[i], D[i]);
  }
  printf("==\n");
*/
  int b=0;
  B[0]=0;//terminator symbol
  for(i=0; i<d;i++){
    B[D[i]]=C[b++];
    #if DEBUG
    printf("B['%c'] = '%c' (%d)\n", D[i], B[D[i]], B[D[i]]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}
/*******************************************************************/
int extend_2mer(unsigned char* str, int n){

  t_2mer A[255][255];
  int B[255];
  int C[255];
  int D[255];

  int i,j;
  for(i=0; i<255;i++)
    B[i]=0;

  int c = count_2mer(str, n, A, C);
  #if DEBUG
  printf("%d\n", c);
  #endif

  int d=0;
  for(i=0; i<255;i++){
    for(j=0; j<255; j++){
      if(A[i][j].freq==0) break;

      if(A[i][j].c1 == A[i][j].c2) continue; //ignore AA, CC, GG, TT

      if(!B[A[i][j].c2] && A[i][j].c2){
        D[d++] = A[i][j].c2;
        B[A[i][j].c2]=1;
      }
      if(!B[A[i][j].c1] && A[i][j].c1){
        D[d++] = A[i][j].c1;
        B[A[i][j].c1]=1;
      }
    }
    if(A[i][j].freq==0) break;
  }
  if(c!=d){
    for(i=0; i<c;i++){
      if(!B[C[i]]) D[d++] = C[i];
    }
  }
  #if DEBUG
  for(i=0; i<d;i++){
    printf("D['%d'] = '%c' (%d)\n", i, D[i], D[i]);
  }
  printf("==\n");
  #endif

  int b=0;
  B[0]=0;//terminator symbol
  for(i=d-1; i>=0;i--){
    B[D[i]]=C[b++];
    #if DEBUG
    printf("B['%c'] = '%c' (%d)\n", D[i], B[D[i]], B[D[i]]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}

/*******************************************************************/
int most_rle(unsigned char* str, int n){

  t_rle A[255];
  int C[255];

  int c = rle(str, n, A, C);
  int i;
  int B[255];
  B[0]=0;//terminator symbol
  for(i=0; i<c;i++){
    B[A[i].symbol]=C[i];
    #if DEBUG
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}
/*******************************************************************/
int less_rle(unsigned char* str, int n){

  t_rle A[255];
  int C[255];

  int c = rle(str, n, A, C);
  int i;
  int B[255], b=0;
  B[0]=0;//terminator symbol
  for(i=c-1; i>=0;i--){
    B[A[i].symbol]=C[b++];
    #if DEBUG
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
    #endif
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}
/*******************************************************************/
