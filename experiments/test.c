#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sacak-lyndon.h"
#include "external/malloc_count/malloc_count.h" //memory counter
#include "external/lib/lyndon-array.h"

#ifndef CHECK
	#define CHECK 1
#endif

int main(int argc, char *argv[]){

	printf("sizeof(int_t) = %zu bytes\n", sizeof(int_t));
	unsigned char *Text;

	// intput data
	if(argc==2){
		Text = malloc((strlen(argv[1])+1)*sizeof(unsigned char));
		sscanf(argv[1], "%s", Text);	
	}
	else{
		Text = (unsigned char*) "graindraining";
	}	
	
	printf("Text = %s$\n", Text);
	int n = strlen((const char*)Text)+1;
	int i;
	
	// allocate
	uint_t *LA = (uint_t *)malloc(n * sizeof(uint_t));
	compute_lyndon_bwt((unsigned char *)Text, (uint_t*)LA, n);
	
	uint_t *SA = (uint_t *)malloc(n * sizeof(uint_t));
	sacak((unsigned char *)Text, (uint_t*)SA, n);

	// output
	printf("pLA = LA[SA[i]] (permuted LA)\n");
	printf("i\tSA\tpLA\tBWT\tsuffixes\n");
	for(i = 0; i < n; ++i) {

		char j = (SA[i])? Text[SA[i]-1]:'$';
		printf("%d\t%d\t%d\t%c\t",i, SA[i], LA[SA[i]],j);
		
		int start=SA[i];

		uint_t k;	
		for(k = start; k < n; ++k) {
			printf("%c", Text[k]);
		}
		printf("$\n");
	}

	#if CHECK
		if(!lyndon_check(Text, LA, n, 0)) printf("isNOTLyndonArray!!\n");
		else printf("isLyndonArray!!\n");
	#endif

	// deallocate
	free(LA);
	free(SA);
	if(argc==2) free(Text);
	
return 0;
}
