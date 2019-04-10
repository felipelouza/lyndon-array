#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef DEBUG
	#define DEBUG 0
#endif

int fraenkel(unsigned char* T, size_t i, size_t j, size_t alpha){

  size_t m = (i+j)/2;
  T[m] = alpha;

  if(i==j) return 0;

  alpha++;
  //printf("%zu) %zu\t%zu\t%zu\n", alpha, i, m, j);

  fraenkel(T, i, m-1, alpha);
  fraenkel(T, m+1, j, alpha);

return 0;
}

int main(int argc, char *argv[]){

	unsigned char *T;
  size_t n, size;

	sscanf(argv[1], "%zu", &n);
	char* c_output = argv[2];

	if(argc!=3){
		fprintf(stderr,"./fraenkel n OUTPUT\n");
    return 1;
	}
  
  size = pow(2.0,n);
  printf("size = %zu\t (%.2lf MB)\n", size, (double)size/pow(2.0,20));

	T = (unsigned char*) malloc((size+1)*sizeof(unsigned char));

  size_t alpha = 1;

  fraenkel(T, 0, size-2, alpha);

  T[size-1]='\n';
  size_t i=0;
  for(;i<size-1;i++){
    //T[i]=n-T[i]+1;
    T[i]='A'+n-T[i]+1;
    #if DEBUG
      printf("%d|", T[i]);
    #endif
  }
  #if DEBUG
    printf("%d|", T[i]);
    printf("\n");
  #endif

	FILE *f_out = fopen(c_output, "w");
	if (!f_out) perror ("file_open");

	fwrite(T, sizeof(unsigned char), size, f_out);

  fclose(f_out);
  free(T);


return 0;
}
