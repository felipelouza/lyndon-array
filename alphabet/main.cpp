/*
 * Alphabet reordering
 *
 * Authors: Felipe A. Louza and Marcelo K. Albertini
 * contact: louza@ufu.br
 * 30/03/2020
 *
 */

#include <cstdio>
#include <cerrno>
#include <ctime>
#include <climits>

#include <iostream>
#include <vector>

using namespace std;

#include "lib/utils.h"
#include "lib/file.h"
#include "../external/malloc_count/malloc_count.h"
#include "lib/algorithms.h"
#include "lib/duval.hpp"

#ifndef DEBUG
  #define DEBUG 0 
#endif

#ifndef CAT 
  #define CAT 0 
#endif

/*******************************************************************/
unsigned char* cat_char(unsigned char** R, size_t d, size_t *n){

  (*n)++; //add 0 at the end

  size_t i, j;
  size_t l=0;
  unsigned char *str = (unsigned char*) malloc((*n)*sizeof(unsigned char));

  for(i=0; i<d; i++){
    size_t m = strlen((char*)R[i]);
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
  puts("\t-p\tprint Lyndon Factors (stdio)");
  puts("\t-A a\tpreferred algorithm to use (default 10)");
  puts("\t-d D\tuse the first D documents of the INPUT");
  puts("\t-b\tread INPUT as binary input (default)");
  puts("\t-f\tread INPUT as formatted input (txt, fasta or fastq)");
  puts("\t-o\toutput converted file");
  puts("\t-s\tcompute some statistics for Lyndon Factors");
  puts("\t-l\toutput the lyndon-factors (start positions) to (INPUT.pos)");
  puts("\t-L\toutput the lyndon-factors (substrings) to (INPUT.lyn)");
  puts("\t-v\tverbose output");
  puts("Debug options:");
  puts("\t-h\tthis help message");
  exit(EXIT_FAILURE);
}

/*******************************************************************/

int main(int argc, char** argv){


  extern char *optarg;
  extern int optind; //, opterr, optopt;

  int c=0, verbose=0, time=0, print=0, output=0, stats=1, factors=0, pos=0;
  //input options
  int bin=0;// bin or formatted (default) input (txt, fasta and fastq)
  char *c_file=NULL;

  size_t  d=0; //number of documents
  int ALG=0;//Algorithm

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
      case 'l':
        pos++; break; //outputs Lyndon factors positions
      case 'L':
        factors++; break; //outputs Lyndon factors
      case 's':
        stats++; break;
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
        printf("%zu) %s (%zu)\n", i, R[i], strlen((char*)R[i]));
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
    size_t e = fseek(f,0,SEEK_END);
    if(e)  {perror("Cannot seek"); exit(1);}
    n = ftell(f);
    rewind(f);

    str = (unsigned char*) malloc((n+1)*sizeof(unsigned char));
    if(str==NULL) {perror("Cannot alloc"); exit(1);}

    e = fread(str,1,n,f);
    if(e!=n) if(f==NULL) {perror("Cannot read from input file"); exit(1);}
    str[n++] = 0; // terminator 

    (void) d;
    printf("N = %zu bytes\n", n);
    printf("sizeof(int) = %zu bytes\n", sizeof(int_t));  
  }

  char ext[20];

  switch(ALG){

    /****/
    case 0:  printf("## NONE ##\n");//no alphabet reordering
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

    case 5:  printf("## 2MER (break) ##\n"); 
      break_2mer(str, n);
      sprintf(ext, "2mer.break");
      break;

    case 6:  printf("## 2MER (extend) ##\n"); 
      extend_2mer(str, n);
      sprintf(ext, "2mer.extend");
      break;
  }

  if(stats){

    #if DEBUG
    printf("%s\n", str);
    #endif

    vector<size_t> R = duval(str, n);
    size_t prev=0, max=0, count=0;

    if(print)
      str[n-1]=END_MARKER;

    for(auto it = R.begin(); it!=R.end(); it++){
      #if DEBUG
        printf("%zu\t%zu\t(%zu)\n", count+1, *it, *it-prev);
      #endif
      if(*it-prev>max) max= *it-prev;
      count++;

      if(print){
        #if DEBUG
        printf("s[%zu..%zu] = \t", prev, *it-1);
        #endif
        for(size_t i=prev; i<*it;i++){
          printf("%c", str[i]);
        }
        printf("\n");
      }

      prev = *it;
    }

    if(print)
      str[n-1]=0;

    printf("##\n");
    printf("Number of Lyndon factors: %zu\n", count);
    printf("Average length: %.2lf\n", (double)n/(double)count);
    printf("Maximum length: %zu\n", max);
    printf("##\n");

    fprintf(stderr, "%zu\t%.2lf\t%zu\n", count, (double)n/(double)count, max);
  }

  if(pos || factors || output)
    printf("OUTPUT:\n");

  if(pos){
    char c_out[255];
    FILE *f_out = NULL;

    if(ALG)
      sprintf(c_out, "%s.%s.pos", c_file, ext);
    else
      sprintf(c_out, "%s.pos", c_file);

    printf("%s\n", c_out);
    f_out = file_open(c_out, "wb");
    vector<size_t> R = duval(str, n);
    
    char tmp[64];
    sprintf(tmp, "%u\n", 0);
    fwrite(tmp, sizeof(char), strlen(tmp), f_out);        

    for(auto it = R.begin(); it!=R.end()-1; it++){
        pos = *it;
        sprintf(tmp, "%lu\n", *it);
        fwrite(tmp, sizeof(char), strlen(tmp), f_out);        
    }
    file_close(f_out);
  }

  if(factors){
    char c_out[255];
    FILE *f_out = NULL;

    if(ALG)
      sprintf(c_out, "%s.%s.lyn", c_file, ext);
    else
      sprintf(c_out, "%s.lyn", c_file);

    printf("%s\n", c_out);
    f_out = file_open(c_out, "wb");
    vector<size_t> R = duval(str, n);
    char endl = '\n';
    size_t prev=0;
    for(auto it = R.begin(); it!=R.end(); it++){
        for(size_t i=prev; i<*it;i++)
          fwrite(&str[i], sizeof(char), 1, f_out);
        fwrite(&endl, sizeof(char), 1, f_out);
      prev = *it;
    }
    file_close(f_out);
  }

  if(output && ALG){

    char c_out[255];
    FILE *f_out = NULL;
    sprintf(c_out, "%s.%s.txt", clean_filename_ext(c_file), ext);
    printf("%s\n", c_out);
    f_out = file_open(c_out, "wb");
    fwrite(str, sizeof(unsigned char), n, f_out);
    file_close(f_out);
  }


  free(str);

return 0;
}

