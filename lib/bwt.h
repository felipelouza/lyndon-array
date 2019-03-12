#ifndef BWTH
#define BWTH

#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "utils.h"


//0 = Lyndon-array (LA)
//1 = permuted LA (in suffix-ordering)
#ifndef PERMUTED
	#define PERMUTED 0
#endif

/*******************************************************************/

int rank(char *T, char c, int i);

int bwt_lcp_inplace(char *T, int n, int *LCP);

int bwt_inplace(char *T, int n);

char* bwt_reverse(char *T, int n);

//computes the Lyndon array during BWT in quadratic time
int bwt_lyndon_inplace(char *T, int_t *LA, int n);

/*******************************************************************/

#endif
