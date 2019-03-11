/*
 * Lyndon Array Construction
 *
 * Author: Felipe A. Louza
 * contact: louza@usp.br
 * 11/03/2019
 *
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

#include "lib/utils.h"
#include "lib/file.h"
#include "lib/bwt.h"
#include "lib/lyndon-array.h"
#include "lib/suffix-array.h"
#include "external/gsaca_cl/gsaca.h"
#include "external/malloc_count/malloc_count.h"
#include "sacak-lyndon.h"

#ifndef DEBUG
  #define DEBUG 0 
#endif

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
    //str[l++] = 1; //add 1 as separator
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
  printf("\n\tUsage: %s [options] FILE \n\n",name);
  puts("Computes the Lyndon-array of FILE");
  puts("Output:\tLyndon-array\n");
  puts("Available options:");
  puts("\t-h\tthis help message");
  puts("\t-t\ttime");
  puts("\t-v\tverbose");
  puts("\t-v\tcheck\n");
  exit(EXIT_FAILURE);
}

/*******************************************************************/

int main(int argc, char** argv){

time_t t_start=0;
clock_t c_start=0;

  extern char *optarg;
  extern int optind, opterr, optopt;

  int c=0, time=0, verbose=0, check=0, print=0;
  char *c_file=NULL;

  size_t  d=0; //number of documents
  int ALG=1;//Algorithm

  while ((c=getopt(argc, argv, "vthp:d:A:c")) != -1) {
    switch (c)
    {
      case 'v':
        verbose++; break;
      case 't':
        time++; break;
      case 'p':
        print=(size_t)atoi(optarg); break;
      case 'h':
        usage(argv[0]); break;       // show usage and stop
      case 'd':
        d=(size_t)atoi(optarg); break;
      case 'A':
        ALG=(size_t)atoi(optarg); break;
      case 'c':
        check++; break;
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

  unsigned char **R;
  size_t i, n=0;

  //disk access
  R = (unsigned char**) file_load_multiple(c_file, &d, &n);
  if(!R){
    fprintf(stderr, "Error: less than %zu strings in %s\n", d, c_file);
    return 0;
  }

  //concatenate strings
  unsigned char *str = NULL;
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

  char* copy = NULL;
  if(check && ALG==5){
      copy = (char*) malloc((n+1)*sizeof(char));
      strcpy(copy, (char*)str);
  }

  //sorted array
  uint_t *LA = (uint_t*) malloc(n*sizeof(int_t));
  for(i=0; i<n; i++) LA[i]=0;

  uint_t *SA = NULL;
  if(ALG>=6) //also computes SA[1,n]
      SA = (uint_t*) malloc(n*sizeof(int_t));

  switch(ALG){

    /****/
    case 1: printf("## LYNDON_BWT ##\n"); 
      compute_lyndon_bwt(str, (uint_t*)LA, n);
      break;

    case 2:  printf("## LYNDON_NSV ##\n"); 
      compute_lyndon_nsv(str, (uint_t*)LA, n);
      break;

    case 3:  printf("## GSACA_LYN ##\n"); 
      time_start(&t_start, &c_start);
      gsaca_cl(str, (uint_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;

    case 4:  printf("## MAX_LYN ##\n"); 
      compute_lyndon_max_lyn(str, (uint_t*)LA, n);
      break;

    case 5:  printf("## BWT_INPLACE_LYN ##\n"); 
      bwt_lyndon_inplace((char*)str, (uint_t*)LA, n);
      break;

    /****/
    case 6:  printf("## GSACA_LYN_SA ##\n"); 
      time_start(&t_start, &c_start);
      gsaca_cl_la(str, (uint_t*)SA, (uint_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;
    
    case 7:  printf("## SACAK_LYNDON 9n (non-linear) ##\n"); 
      time_start(&t_start, &c_start);
      sacak_lyndon_9n_non_linear(str, (uint_t*)SA, (int_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;

    case 8:  printf("## SACAK_LYNDON 17n (linear) ##\n"); 
      time_start(&t_start, &c_start);
      sacak_lyndon_17n_linear(str, (uint_t*)SA, (int_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;
    
    case 9:  printf("## SACAK_LYNDON 13n (linear) ##\n"); 
      time_start(&t_start, &c_start);
      sacak_lyndon_13n_linear(str, (uint_t*)SA, (int_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;
    
    case 10:  printf("## SACAK_LYNDON 9n (linear) ##\n"); 
      time_start(&t_start, &c_start);
      sacak_lyndon_9n_linear(str, (uint_t*)SA, (int_t*)LA, n);
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      break;
    
    /****/

    default: break;
  }

  // validate  
  if(check){
    if(ALG==5){
      free(str);
      str = (unsigned char*) copy;
    }

    printf("LA: ");
    if(!lyndon_check(str, LA, n, 0)){
      printf("isNOTLyndonArray!!\n");
      fprintf(stderr, "ERROR\n");
    }
    else {
      printf("isLyndonArray!!\n");
    }
    if(SA){
      printf("SA: ");
      if(!suffix_array_check((unsigned char*)str, (int_t*)SA, n, sizeof(char), 0)) fprintf(stderr,"isNotSorted!!\n");//compares until the separator=1
			else printf("isSorted!!\n");
    }
  }

  if(print){ 
    printf("########\n");
    printf("i) LA\t");
    if(SA) printf("SA\t");
    printf("suffixes\n");
    for(i=0; i<min(n,print); i++){
      printf("%zu) %" PRIdN "\t", i, LA[i]);
      if(SA) printf("%" PRIdN "\t", SA[i]);
      int_t j=i;
      for(j=i; j<(int_t) min(i+10,n); j++){
        char c = (str[j]==0)?'#':(str[j]>1?str[j]-1:'$');
        printf("%c", c);
      }
      printf("\n");
    }
    printf("########\n");
  }

  free(LA);
  free(str);

  if(SA)
    free(SA);

return 0;
}

