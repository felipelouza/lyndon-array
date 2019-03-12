/* 
 * Author: Felipe A. Louza
 * contact: louza@ic.unicamp.br
 * 25/03/2017
 *
 */

#ifndef LYNDON_H
#define LYNDON_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

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


/** @brief computes the lyndon array of string s[0, n-1] during the BWT inversion 
 *
 *  @param s	input string with s[n-1]=0
 *  @param A 	lyndon array 
 *  @param n	string length
 *  @return -1 if an error occured, otherwise the depth of the recursive calls.
 */
int compute_lyndon_bwt(unsigned char *s, int_t *A, int_t n);

/** @brief computes the lyndon array of string s[0, n-1] from SA, ISA and NSV 
 *
 *  @param s	input string with s[n-1]=0
 *  @param A 	lyndon array 
 *  @param n	string length
 *  @return -1 if an error occured, otherwise the depth of the recursive calls.
 */
int compute_lyndon_nsv(unsigned char *s, int_t *A, int_t n);

/** @brief computes the lyndon array of string s[0, n-1] 
 *
 *  @param s	input string with s[n-1]=0
 *  @param A 	lyndon array 
 *  @param n	string length
 *  @return -1 if an error occured, otherwise the depth of the recursive calls.
 */
int compute_lyndon_max_lyn(unsigned char *s, int_t *A, int_t n);


/** @brief checks if A[0, n-1] is the lyndon array of string s[0, n-1]
 *
 *  @param s	input string with s[n-1]=0
 *  @param A 	lyndon array 
 *  @param n	string length
 *  @param print 1 = print s, SA, BWT, LA and suffixes
 *  @return -1 if an error occured, otherwise the depth of the recursive calls.
 */
int lyndon_check(unsigned char *s, int_t *A, int_t n, int print);

#endif
