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

#include "../lib/utils.h"
#include "../lib/file.h"
#include "../external/malloc_count/malloc_count.h"

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
  char symbol;
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
  for(i=0; i<255;i++)
    if(A[i].run)
      printf("%d\t%d\t%d\n", i, A[i].symbol, A[i].run);

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
      printf("%d\t%d\t%d\n", i, A[i].symbol, A[i].freq);
      C[c++]=A[i].symbol;
    }
  }

  //sort
  qsort(A, 255, sizeof(t_symbol), compare);

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
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
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
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
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
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
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
    printf("B['%d'] = '%d'\n", A[i].symbol, B[A[i].symbol]);
  }

  for(i=0; i<n-1;i++){
    str[i] = B[str[i]];
  }

return 0;
}
/*******************************************************************/
unsigned char* cat_char(unsigned char** R, size_t d, size_t *n){

  (*n)++; //add 0 at the end

  int_t i, j;
  int_t l=0;
  unsigned char *str = (unsigned char*) malloc((*n)*sizeof(unsigned char));

  for(i=0; i<d; i++){
    int_t m = strlen((char*)R[i]);
    for(j=0; j<m; j++){
      if(R[i][j]<255) str[l++] = R[i][j];
    }
#if CAT == 1
    str[l++] = 1; //add 1 as separator
#endif
  }

  str[l++]=0;
  if(*n>l){
    str = (unsigned char*) realloc(str, (l)*sizeof(unsigned char)); 
  }
  *n = l;

return str;
}

/*******************************************************************/


void usage(char *name){
//  printf("lyndon-array:\n\tUsage: %s [options] FILE \n\n",name);
//  puts("Computes the Lyndon-array of FILE");
//  puts("Output:\tLyndon-array (and Suffix-array)\n");
  puts("Available options:");
  puts("\t-A a\tpreferred algorithm to use (default 10)");
  puts("\t-d D\tuse the first D documents of the INPUT");
  puts("\t-b\tread INPUT as binary input (default)");
  puts("\t-f\tread INPUT as formatted input (txt, fasta or fastq)");
  puts("\t-v\tverbose output");
  puts("Debug options:");
  puts("\t-h\tthis help message");
  exit(EXIT_FAILURE);
}

/*******************************************************************/

int main(int argc, char** argv){


  extern char *optarg;
  extern int optind, opterr, optopt;

  int c=0, verbose=0, time=0, print=0, output=1;
  //input options
  int bin=0;// bin or formatted (default) input (txt, fasta and fastq)
  char *c_file=NULL;

  size_t  d=0; //number of documents
  int ALG=1;//Algorithm

  while ((c=getopt(argc, argv, "vthpd:A:cosbflL")) != -1) {
    switch (c)
    {
      case 'v':
        verbose++; break;
      case 't':
        time++; break;
      case 'p':
        print++; break;
      case 'h':
        usage(argv[0]); break;       // show usage and stop
      case 'd':
        d=(size_t)atoi(optarg); break;
      case 'A':
        ALG=(size_t)atoi(optarg); break;
      case 'o':
        output++; break;
      case 'b':
        bin=1; break;
      case 'f':
        bin=0; break;
      case '?':
        exit(EXIT_FAILURE);
    }
  }
  free(optarg);
  
  if(optind+1==argc) {
    c_file=argv[optind++];
  }
  else  usage(argv[0]);

  if(verbose>0) {
    puts("Command line:");
    int i;
    for(i=0;i<argc;i++)
      printf(" %s",argv[i]);
    puts("");
  }

  /********/
  unsigned char *str = NULL;
  size_t i, n=0;

  if(bin==0){
    // reading the input as a collection of documents
    unsigned char **R;

    //disk access
    R = (unsigned char**) file_load_multiple(c_file, &d, &n);
    if(!R){
      fprintf(stderr, "Error: less than %zu strings in %s\n", d, c_file);
      return 0;
    }

    //concatenate strings
    str = cat_char(R, d, &n);

    printf("d = %zu\n", d);
    printf("N = %zu bytes\n", n);
    printf("sizeof(int) = %zu bytes\n", sizeof(int_t));


    #if DEBUG
      printf("R:\n");
      for(i=0; i<d; i++)
        printf("%" PRIdN ") %s (%zu)\n", i, R[i], strlen((char*)R[i]));
    #endif

    //free memory
    for(i=0; i<d; i++)
      free(R[i]);
    free(R);
  }
  else{ //bin
    // reading the input file as a single document
    FILE *f = fopen(c_file,"r");
    if(f==NULL) {perror("Cannot open input file"); exit(1);}
    int e = fseek(f,0,SEEK_END);
    if(e)  {perror("Cannot seek"); exit(1);}
    n = ftell(f);
    rewind(f);

    str = malloc((n+1)*sizeof(unsigned char));
    if(str==NULL) {perror("Cannot alloc"); exit(1);}

    e = fread(str,1,n,f);
    if(e!=n) if(f==NULL) {perror("Cannot read from input file"); exit(1);}
    str[n++] = 0; // terminator 

    (void) d;
    printf("N = %zu bytes\n", n);
    printf("sizeof(int) = %zu bytes\n", sizeof(int_t));  
  }

  char ext[10];

  switch(ALG){

    /****/
    case 0:  printf("## NORMAL ##\n");
      sprintf(ext, "normal");
      break;

    case 1:  printf("## FREQ (less) ##\n"); 
      less_frequent(str, n);
      sprintf(ext, "freq.less");
      break;

    case 2:  printf("## FREQ (most) ##\n"); 
      most_frequent(str, n);
      sprintf(ext, "freq.most");
      break;

    case 3:  printf("## RLE (less) ##\n"); 
      less_rle(str, n);
      sprintf(ext, "rle.less");
      break;

    case 4:  printf("## RLE (most) ##\n"); 
      most_rle(str, n);
      sprintf(ext, "rle.most");
      break;
  }

  if(print){
    for(i=0; i<n-1;i++){
      printf("%c", str[i]);
    }
    printf("\n");
  }

  if(output){

    char c_out[255];
    FILE *f_out = NULL;
    printf("OUTPUT:\n");
    sprintf(c_out, "%s.%s.txt", c_file, ext);
    printf("%s\n", c_out);
    f_out = file_open(c_out, "wb");
    fwrite(str, sizeof(unsigned char), n, f_out);
    file_close(f_out);
  }

  free(str);

return 0;
}

