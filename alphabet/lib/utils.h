#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cstdint>
#include <cinttypes>
#include <cmath>

#ifndef UCHAR_SIZE
#define UCHAR_SIZE 256
#endif

#define END_MARKER '$'

#ifndef M64
	#define M64 0
#endif

#ifndef DEBUG
	#define DEBUG 0 
#endif

#if M64
	typedef int64_t  int_t;
	typedef uint64_t uint_t;
	#define PRIdN	 PRId64
	#define U_MAX 	 UINT64_MAX
	#define I_MAX 	 INT64_MAX
	#define I_MIN 	 INT64_MIN
#else
	typedef int32_t  int_t;
	typedef uint32_t uint_t;
	#define PRIdN	 PRId32
	#define U_MAX 	 UINT32_MAX
	#define I_MAX	 INT32_MAX
	#define I_MIN	 INT32_MIN
#endif

typedef uint32_t int_text;


/**********************************************************************/

void   time_start(time_t *t_time, clock_t *c_clock);
double time_stop(time_t t_time, clock_t c_clock);

void die(const char* where);
void dies(const char* where, char* format, ...);

int_t print_int(int_t* A, int_t n);
int_t print_char(char* A, int_t n);
int_t min_range(int_t* A, int_t l, int_t r);

/**********************************************************************/

#endif
