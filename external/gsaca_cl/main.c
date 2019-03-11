#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gsaca.h"
#include "../../external/malloc_count/malloc_count.h"


int main() {

	//Test Arrays
	const char *S = "graindraining";
//	const char *S = "ababababa";
	int n = strlen(S) + 1;
	unsigned int *CLEN = (unsigned int*) malloc(n*sizeof(unsigned int));

	//Compute context lengths
	gsaca_cl( (const unsigned char *)S, CLEN, n);

	//output result
	int i;
	for (i = 0; i < n; i++) {
		printf("%d\t", CLEN[i]);
		int j;
		for (j = 0; j < (int) CLEN[i]; j++) {
			printf("%c", S[i+j]);
		}
		printf("\n");
	}

	//free(CLEN);

	return 0;
}
