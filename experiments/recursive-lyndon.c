/*
 * Lyndon Array Decomposition
 *
 * Author: Felipe A. Louza
 * contact: louza@usp.br
 * 29/04/2019
 *
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

#include "../lib/utils.h"
#include "../lib/file.h"
#include "../lib/lyndon-array.h"
#include "../external/malloc_count/malloc_count.h"
#include "../sacak-lyndon.h"

#ifndef DEBUG
  #define DEBUG 0 
#endif

#ifndef CAT 
  #define CAT 1 
#endif

/*******************************************************************/
int recursive_decomposition(unsigned char *str, int_t *LA, size_t start, size_t end, int level, size_t *total, size_t *large, size_t mem, int print){

  if(print) printf("level = %d\n", level);

  int next=level;
  size_t i=start;

  while(i<end){
//  for(i=0;i<end;i++){

    if(print) if(LA[i]>mem) printf("## ");
    if(print) printf("%zu)\t%d\n", i, LA[i]);
    if(LA[i]>mem){

      (*large)++;
      /*
      int j;
      for(j=0; j<level+1; j++) printf("##");
      printf(" %zu\t%d\n", i, LA[i]+1);
      */

      next = recursive_decomposition(str, LA, i+1, i+LA[i]-1, level+1, total, large, mem, print);
      if(print) printf("level = %d\n", level);
    }

    (*total)++;
    i+=LA[i];
  }


return next;
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
      if(R[i][j]<255) str[l++] = R[i][j]+1;
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
  printf("recursive-lyndon:\n\tUsage: %s [options] FILE \n\n",name);
  puts("Recursive Lyndon-decomposition\n");
  puts("Available options:");
  puts("\t-d D\tuse the first D documents of the INPUT");
  puts("\t-b\tread INPUT as binary input (default)");
  puts("\t-f\tread INPUT as formatted input (txt, fasta or fastq)");
  puts("\t-v\tverbose output");
  puts("\t-s\tcomputes some statistics for LA");
  puts("\t-M m\tmemory is equal to m");
  puts("\t-P p\tmemory is equal to n/2^p");
  puts("Debug options:");
  puts("\t-p\tprint lyndon decomposition");
  puts("\t-h\tthis help message");
  exit(EXIT_FAILURE);
}

/*******************************************************************/

int main(int argc, char** argv){

time_t t_start=0;
clock_t c_start=0;

  extern char *optarg;
  extern int optind, opterr, optopt;

  int c=0, time=0, verbose=0, stats=0, print=0;
  //input options
  int bin=1;// bin or formatted input (txt, fasta and fastq)
  size_t  mem=SIZE_MAX;
  char *c_file=NULL;

  size_t  d=0; //number of documents
  int power=0;

  while ((c=getopt(argc, argv, "vthd:sM:pP:bf")) != -1) {
    switch (c)
    {
      case 'v':
        verbose++; break;
      case 't':
        time++; break;
      case 'h':
        usage(argv[0]); break;       // show usage and stop
      case 'd':
        d=(size_t)atoi(optarg); break;
      case 's':
        stats++; break;
      case 'M':
        mem=(size_t)atoi(optarg); break;
      case 'p':
        print++; break;
      case 'P':
        power=atoi(optarg); break;
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

    str = malloc((n+1)*sizeof(char));
    if(str==NULL) {perror("Cannot alloc"); exit(1);}

    e = fread(str,1,n,f);
    if(e!=n) if(f==NULL) {perror("Cannot read from input file"); exit(1);}
    str[n++] = 0; // terminator 

    (void) d;
    printf("N = %zu bytes\n", n);
    printf("sizeof(int) = %zu bytes\n", sizeof(int_t));  
  }


  /********/


  //lyndon array
  int_t *LA = (int_t*) malloc(n*sizeof(int_t));
  for(i=0; i<n; i++) LA[i]=0;

  int_t *SA = (int_t*) malloc(n*sizeof(int_t));
  for(i=0; i<n; i++) SA[i]=0;
  
  printf("## SACAK_LYNDON 9n (linear) ##\n"); 
  time_start(&t_start, &c_start);
  sacak_lyndon_9n_linear(str, (int_t*)SA, (int_t*)LA, n);
  printf("TOTAL:\n");
  fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  /**/
  int level = 0;
  size_t total = 0, large = 0;
  if(power){
    mem = n/pow(2,power);
    printf("power = %.0f\n", pow(2,power));
  }
  if(mem==SIZE_MAX) printf("MEM = unlimited\n");
  else printf("MEM = %zu bytes\n", mem);
  level = recursive_decomposition(str, LA, 0, n, level, &total, &large, mem, print);

  printf("##\n");
  printf("total = %zu\n", total);
  printf("level = %d\n", level);
  printf("large = %zu\n", large);
  printf("##\n");

  fprintf(stderr,"%zu\t%d\t%zu\n", total, level, large);
  /**/


  if(LA && stats){
    int_t max=0, j=0;
    size_t sum=0;
    
    for(i=0;i<n;i++){
      sum+=LA[i];
      if(LA[i]>max){max=LA[i];j=i;}
    }

    printf("##\n");
    printf("Max(LA[%d]): % "PRIdN"\n", j, max);
    printf("Avg(LA[i]): %.2lf\n", (double)sum/(double)n);

    i=0;
    int_t count=0;
    max=0;
    while(i<n){
      #if DEBUG
      printf("%d\t%d\n", i, LA[i]);
      #endif
      count++;
      if(LA[i]>max) max=LA[i];
      i+=LA[i];
    }

    printf("##\n");
    printf("Number of Lyndon factors: % "PRIdN"\n", count);
    printf("Average length: %.2lf\n", (double)n/(double)count);
    printf("Maximum length: % "PRIdN"\n", max);
    printf("##\n");
  }

  free(str);
  if(LA) free(LA);
  if(SA) free(SA);

return 0;
}

