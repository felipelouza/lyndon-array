/* 
 * Authors: Felipe A. Louza
 * contact: louza@ufu.br
 * 01/06/2019
 */

#ifndef SACA_K_H
#define SACA_K_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

#ifndef DEBUG
        #define DEBUG 0 
#endif

#ifndef M64
        #define M64 0
#endif

#if M64
        typedef int64_t  int_t;
        typedef uint64_t uint_t;
        #define PRIdN    PRId64
        #define U_MAX    UINT64_MAX
        #define I_MAX    INT64_MAX
        #define I_MIN    INT64_MIN
#else
        typedef int32_t  int_t;
        typedef uint32_t uint_t;
        #define PRIdN    PRId32
        #define U_MAX    UINT32_MAX
        #define I_MAX    INT32_MAX
        #define I_MIN    INT32_MIN
#endif

/** @brief computes the suffix array of string s[0..n-1] 
 *
 *  @param s	input string with s[n-1]=0
 *  @param SA 	suffix array 
 *  @param n	string length
 *  @return -1 if an error occured, otherwise the depth of the recursive calls.
 */
int sacak(unsigned char *s, int_t *SA, int_t n);

/** @brief computes the suffix and LA arrays of string s[0..n-1]
 *  @param k	alphabet size
 */
int sacak_lyndon_9n_non_linear(unsigned char *s, int_t *SA, int_t* LA, int_t n);

int sacak_lyndon_17n_linear(unsigned char *s, int_t *SA, int_t* LA, int_t n);
int sacak_lyndon_13n_linear(unsigned char *s, int_t *SA, int_t* LA, int_t n);
int sacak_lyndon_9n_linear(unsigned char *s, int_t *SA, int_t* LA, int_t n);

#endif
