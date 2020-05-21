#include "lyndon-array.h"
#include "../external/sacak-lyndon.h"

//get bwt[i] 
#define bwt(i) ((SA[i])?s[SA[i]-1]:0)

#ifndef TIME
	#define TIME 1
#endif

#ifndef STEP_TIME
	#define STEP_TIME 0
#endif

#define SIGMA 256

#define END_MARKER '$'

//0 = Lyndon-array (LA)
//1 = permuted LA (in suffix-ordering)
#ifndef PERMUTED
	#define PERMUTED 0
#endif

#ifndef PRINT
	#define PRINT 0
#endif

#ifndef SAVE_SPACE
	#define SAVE_SPACE 1
#endif

#ifndef COMPUTE_BWT 
	#define COMPUTE_BWT 1
#endif

#define STACK_SIZE 10240/(sizeof(int_t)*2) //10KB

/**********************************************************************/

typedef struct _pair{
        int_t i;
        int_t j;
} s_pair;

typedef struct{
        s_pair *array;
        int_t top;
        int_t size;
} stack_pair;

int stack_pair_init(stack_pair *S, int_t size){

	S->array = (s_pair*) malloc(size*sizeof(s_pair));	
	if(S->array==NULL) return 0;

	S->size = size;
	S->top = 0;

return 0;
}

void stack_pair_push(stack_pair *S, int_t i, int_t j){

	if(S->top==S->size){
		S->size += STACK_SIZE;
		S->array = (s_pair*) realloc(S->array, S->size*sizeof(s_pair));
	} 

	S->array[S->top].i = i;
	S->array[S->top].j = j;

	(S->top)++;
}

s_pair stack_pair_top(stack_pair *S){

	s_pair aux;
	aux.i = S->array[S->top-1].i;
	aux.j = S->array[S->top-1].j; 

return aux;
}

void stack_pair_pop(stack_pair *S){
	(S->top)--;
}

void stack_pair_print(stack_pair *S){

	int_t i;

	for(i=S->top-1; i>0; i--)
		printf("%d\t(%d, %d)\n", i, S->array[i].i, S->array[i].j);
	
	printf("%d\t(%d, %d)\n", i, S->array[i].i, S->array[i].j);

}

/****************************************************************************/

typedef struct{
        int_t *array;
        int_t top;
        int_t size;
} stack;

int stack_init(stack *S, int_t size){

	S->array = (int_t*) malloc(size*sizeof(int_t));	
	if(S->array==NULL) return 0;

	S->size = size;
	S->top = 0;

return 0;
}

void stack_push(stack *S, int_t i){

	if(S->top==S->size){
		S->size += STACK_SIZE;
		S->array = (int_t*) realloc(S->array, S->size*sizeof(int_t));
	} 

	S->array[S->top] = i;

	(S->top)++;
}

int_t stack_top(stack *S){

return S->array[S->top-1];
}

void stack_pop(stack *S){
  if(S->top) (S->top)--;
}

void stack_print(stack *S){

	int_t i;

	for(i=S->top-1; i>0; i--)
		printf("%d\t(%d)\n", i, S->array[i]);
	
	printf("%d\t(%d)\n", i, S->array[i]);

}

/****************************************************************************/

void tstart(time_t *t_time, clock_t *c_clock){
        *t_time = time(NULL);
        *c_clock =  clock();
}

double tstop(time_t t_time, clock_t c_clock){
        double aux1 = (clock() - c_clock) / (double)(CLOCKS_PER_SEC);
        double aux2 = difftime (time(NULL),t_time);
        printf("CLOCK = %lf TIME = %lf\n", aux1, aux2);
        return aux1;
}

/**********************************************************************/

int compute_lyndon_bwt_10n(unsigned char *s, int_t *A, int_t n){

int_t i;

	#if TIME
		time_t t_total= 0;clock_t c_total= 0;
		tstart(&t_total, &c_total); 
	#endif

	#if STEP_TIME
		time_t t_time = 0;clock_t c_time = 0;
		tstart(&t_time, &c_time); 
		printf("1. SA construction:\n");
	#endif
	
	// 1. sort
	#if PERMUTED  	//1. uses 1 array for SA -> LF -> pLA
		int_t *SA = A;
	#else		//0. uses 2 arrays, SA -> LF, and LA
		int_t *SA = (int_t*) malloc(n*sizeof(int_t));
	#endif

	sacak(s, SA, n);

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("2. Obtain BWT:\n"); 
	#endif

	//2. compute BWT
	unsigned char* bwt = malloc(n*sizeof(unsigned char));
	for(i=0; i<n; i++){
		bwt[i] = (SA[i])?s[SA[i]-1]:0;
	}	

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("3. Compute LF:\n"); 
	#endif

	//3. compute LF-array (in the space of SA[0,n-1])
	int_t *LF = SA;
	int_t *C = (int_t*) malloc(SIGMA*sizeof(int_t));

	//counter
	for(i=0; i<SIGMA; i++) C[i]=0;
	for(i=0; i<n; i++) C[bwt[i]]++;

	int_t sum=0;
	for(i=0; i<SIGMA; i++){
		sum+=C[i]; C[i]=sum-C[i];
	}

	for(i=0; i<n; i++){
		LF[i] = C[bwt[i]]++;
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("4. Compute Lyndon:\n");
	#endif

	//BWT-inversion and Lyndon array construction
	stack_pair S;
	S.top=0; S.size=STACK_SIZE;
	stack_pair_init(&S,STACK_SIZE);
	stack_pair_push(&S, 0, 0);//(0, 0)

	#if PRINT
		printf("step\tpos\tT^{rev}\tLyndon\n");
	#endif

	int_t *LA = A;

	int_t pos = 0;
	int_t step = 1;//(n-1)-i

	for(i=n-1; i >= 0; i--){	

		while(stack_pair_top(&S).i > pos) stack_pair_pop(&S);

		int_t next = LF[pos];

		#if PERMUTED
			LA[pos] = step-stack_pair_top(&S).j;
		#else
			LA[i] = step-stack_pair_top(&S).j;
		#endif

		#if PRINT
			printf("%d\t%d\t%c\t%d\n", step, pos, bwt[pos], step-stack_pair_top(&S).j);
		#endif

		stack_pair_push(&S, pos, step++);

		pos = next; //pos = LF(pos)
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
	#endif

	#if TIME
		printf("TOTAL:\n");
		fprintf(stderr,"%.6lf\n", tstop(t_total, c_total)); 
	#endif

	free(S.array);
	free(C);
	free(bwt);

	#if PERMUTED == 0
		free(SA);
	#endif

return 0;
}

/**********************************************************************/

unsigned char find_symbol(int_t pos, int_t* F, unsigned char* Alpha, int_t n){

	//binary search
	int_t a=0;
	int_t b=n-2;

	int_t i=0;
	while(a<=b){
		i=a+(b-a)/2;
		if(pos>=F[i] && pos<F[i+1]){
			return Alpha[i];
		}
		if(pos>F[i]) a=i+1;
		else b=i-1;
	}

	return 0;
}

int compute_lyndon_bwt_9n(unsigned char *s, int_t *A, int_t n){

int_t i;

	#if TIME
		time_t t_total= 0;clock_t c_total= 0;
		tstart(&t_total, &c_total); 
	#endif

	#if STEP_TIME
		time_t t_time = 0;clock_t c_time = 0;
		tstart(&t_time, &c_time); 
		printf("1. SA construction:\n");
	#endif
	
	// 1. sort
	int_t *SA = A;
	sacak(s, SA, n);

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("2. Obtain BWT:\n"); 
	#endif

	//2. compute BWT
	unsigned char* tmp = (unsigned char*) malloc(n*sizeof(unsigned char));
	for(i=0; i<n; i++)
		tmp[i] = s[i];		

	unsigned char* bwt = s;
	//unsigned char* bwt = malloc(n*sizeof(unsigned char));
	for(i=0; i<n; i++)
		bwt[i] = (SA[i])?tmp[SA[i]-1]:0;

	free(tmp);

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("3. Compute LF:\n"); 
	#endif

	//3. compute LF-array (in the space of SA[0,n-1])
	int_t *LF = (int_t*) malloc(n*sizeof(int_t));
	int_t *C = (int_t*) malloc(SIGMA*sizeof(int_t));

	int_t sigma = 0;

	//counter
	for(i=0; i<SIGMA; i++) C[i]=0;
	for(i=0; i<n; i++){
		if(!C[bwt[i]]) sigma++;
		C[bwt[i]]++;
	}

	sigma++;
	int_t *F = (int_t*) malloc(sigma*sizeof(int_t));
	unsigned char *Alpha = (unsigned char*) malloc(sigma*sizeof(unsigned char));
	for(i=0; i<sigma; i++) F[i]=0;

	//sentinel
	F[sigma-1]=n;
	Alpha[sigma-1]=0;

	int_t sum=0;
	int_t j=0;
	for(i=0; i<SIGMA; i++){
		sum+=C[i]; 
		if(C[i]){ 
			C[i]=sum-C[i];
			F[j]=C[i];
			Alpha[j]=i;
			j++;
		}
	}

	for(i=0; i<n; i++){
		LF[i] = C[bwt[i]]++;
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("4. Compute Lyndon:\n");
	#endif

	//BWT-inversion and Lyndon array construction
	stack_pair S;
	S.top=0; S.size=STACK_SIZE;
	stack_pair_init(&S,STACK_SIZE);
	stack_pair_push(&S, 0, 0);//(0, 0)

	#if PRINT
		printf("step\tpos\tT^{rev}\tLyndon\n");
	#endif

	int_t *LA = A;

	int_t pos = 0;
	int_t step = 1;//(n-1)-i

	for(i=n-1; i >= 0; i--){	

		s[i] = find_symbol(pos, F, Alpha, sigma);
		
		while(stack_pair_top(&S).i > pos) stack_pair_pop(&S);

		int_t next = LF[pos];

		#if PERMUTED
			LA[pos] = step-stack_pair_top(&S).j;
		#else
			LA[i] = step-stack_pair_top(&S).j;
		#endif

		#if PRINT
			printf("%d\t%d\t%c\t%d\n", step, pos, bwt[pos], step-stack_pair_top(&S).j);
		#endif

		stack_pair_push(&S, pos, step++);

		pos = next; //pos = LF(pos)
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
	#endif

	#if TIME
		printf("TOTAL:\n");
		fprintf(stderr,"%.6lf\n", tstop(t_total, c_total)); 
	#endif


	free(S.array);
	free(C);
	free(LF);

	free(F);
	free(Alpha);

return 0;
}

//Giovanni's version
int compute_lyndon_text_9n(unsigned char *s, int_t *A, int_t n){

int_t i;

	#if TIME
		time_t t_total= 0;clock_t c_total= 0;
		tstart(&t_total, &c_total); 
	#endif

	#if STEP_TIME
		time_t t_time = 0;clock_t c_time = 0;
		tstart(&t_time, &c_time); 
		printf("1. SA construction:\n");
	#endif
	
	// 1. sort
	int_t *SA = (int_t*) malloc(n*sizeof(int_t));
	sacak(s, SA, n);

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("2. Compute BWT and C:\n"); 
	#endif

	//2. compute BWT
	int_t* bwt = SA; 
	for(i=0; i<n; i++){
		bwt[i] = (SA[i])?s[SA[i]-1]:0;
	}	

	//2. compute C 

	int_t *C = (int_t*) malloc(SIGMA*sizeof(int_t));
	for(i=0; i<SIGMA; i++) C[i]=0;
	for(i=0; i<n; i++) C[s[i]]++;

	int_t sum=0;
	for(i=0; i<SIGMA; i++){
		sum+=C[i]; C[i]=sum-C[i];
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("3. Compute LF:\n"); 
	#endif

	//3. compute LF-array (in the space of SA[0,n-1])
	int_t *LF = SA; 
	for(i=0; i<n; i++) LF[i] = C[bwt[i]]++;

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("4. Compute Lyndon:\n");
	#endif

	//Lyndon array construction
	stack_pair S;
	S.top=0; S.size=STACK_SIZE;
	stack_pair_init(&S,STACK_SIZE);
	stack_pair_push(&S, 0, 0);//(0, 0)

	#if PRINT
		printf("step\tpos\tT^{rev}\tLyndon\n");
	#endif

	int_t *LA = A;

	int_t pos = 0;
	int_t step = 1;//(n-1)-i

	for(i=n-1; i >= 0; i--){	

		while(stack_pair_top(&S).i > pos) stack_pair_pop(&S);
		int_t next = LF[pos];

		#if PERMUTED
			LA[pos] = step-stack_pair_top(&S).j;
		#else
			LA[i] = step-stack_pair_top(&S).j;
		#endif

		stack_pair_push(&S, pos, step++);
		pos = next; //pos = LF(pos)
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
	#endif

	#if TIME
		printf("TOTAL:\n");
		fprintf(stderr,"%.6lf\n", tstop(t_total, c_total)); 
	#endif

	free(S.array);
	free(C);
	free(LF);

return 0;
}


int compute_lyndon_bwt(unsigned char *s, int_t *A, int_t n){

	#if SAVE_SPACE
		#if COMPUTE_BWT
			compute_lyndon_bwt_9n(s, A, n);
		#else
			compute_lyndon_text_9n(s, A, n);
		#endif
	#else 
		compute_lyndon_bwt_10n(s, A, n);
	#endif
	return 0;
}


/**********************************************************************/

int lyndon_check(unsigned char *s, int_t *LA, int_t n, int print){

int check=1;
int_t i;

	int_t *SA = (int_t*) malloc(n*sizeof(int_t));
	sacak(s, SA, n);

	int_t *ISA = (int_t*) malloc(n*sizeof(int_t));
	for(i=0; i<n; i++) ISA[SA[i]]=i;

	// print output
	if(print){
		#if PERMUTED
			printf("i\tSA\tpLA\tBWT\tsuffixes\n");
		#else
			printf("i\tSA\tLA\tBWT\tsuffixes\n");
		#endif
	
		for(i = 0; i < n; ++i) {
	
			char j = (SA[i])? s[SA[i]-1]:END_MARKER;
			printf("%d\t%d\t%d\t%c\t",i, SA[i], LA[i],j-1);
			
			#if PERMUTED
			    int start=SA[i];
			#else
			    int start=i;
			#endif
	
			int_t k;	
			for(k = start; k < n; k++) {
				printf("%c", s[k]-1);
			}
			printf("$\n");
		}
	}

	//simple checker
	for(i=0; i<n; i++){
		
		#if PERMUTED
			int start=SA[i];
			int pos = i;
		#else
			int start=i;
			int pos = ISA[i];
		#endif

		if(start==n-1){
			if(LA[i]!=1){
				check=0;
				printf("*%d) %d != %d\n", i, LA[i], 1);
				break;
			}
			continue;			
		}

		int size=1;
		while(ISA[start+size]>pos)size++; //ISA[n-1]=0

		if(LA[i]!=size){
			check=0;
			printf("%d) %d != %d\n", i, LA[i], size);
			break;
		}
	}

	free(SA);
	free(ISA);
	
	return check;
}

/**********************************************************************/

int compute_nsv(int_t* NSV, int_t *array, int_t n){

	stack S;
	S.top=0; S.size=STACK_SIZE;
	stack_init(&S,STACK_SIZE);
	stack_push(&S, 0);//(idx, value)=(pos, step)

	int_t i;
  for(i=n-1; i>0; i--){

    NSV[i] = stack_top(&S);
		
    if(array[i-1]>array[i]){
      stack_push(&S, i);
    }
    else{
      //while(stack_top(&S).j >= array[i-1]) stack_pop(&S);
      while(array[stack_top(&S)] >= array[i-1]) stack_pop(&S);
    }
  }

  NSV[0] = stack_top(&S);
  free(S.array);

return 0;
}

/**********************************************************************/

int compute_lyndon_nsv(unsigned char *s, int_t *A, int_t n){

int_t i;

	#if TIME
		time_t t_total= 0;clock_t c_total= 0;
		tstart(&t_total, &c_total); 
	#endif

	#if STEP_TIME
		time_t t_time = 0;clock_t c_time = 0;
		tstart(&t_time, &c_time); 
		printf("1. SA construction:\n");
	#endif
	
	// 1. sort
	//int_t *SA = A;
	int_t *SA = (int_t*) malloc(n*sizeof(int_t));
	sacak(s, SA, n);

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("2. Compute ISA:\n"); 
	#endif

	//2. compute ISA
	int_t *ISA = A;
	for(i=0; i<n; i++) ISA[SA[i]]=i;

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("3. Compute NSV-array:\n"); 
	#endif

	//3. compute NSV
	int_t *NSV = SA; //(in the space of SA[0,n-1])
	compute_nsv(NSV, ISA, n);
	NSV[n-1]=n;

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
		tstart(&t_time, &c_time);
		printf("4. Compute Lyndon:\n");
	#endif


	#if PRINT
		printf("step\tpos\tT^{rev}\tLyndon\n");
	#endif

	int_t *LA = A;
	for(i=0; i < n; i++){	
		LA[i] = NSV[i]-i;
	}

	#if STEP_TIME
		fprintf(stderr,"%.6lf\n", tstop(t_time, c_time));
	#endif

	#if TIME
		printf("TOTAL:\n");
		fprintf(stderr,"%.6lf\n", tstop(t_total, c_total)); 
	#endif

	free(SA);
	//free(ISA);
	//free(NSV);

return 0;
}

/**********************************************************************/

int compute_lyndon_max_lyn(unsigned char *s, int_t *A, int_t n){

int_t j;

	#if TIME
		time_t t_total= 0;clock_t c_total= 0;
		tstart(&t_total, &c_total); 
	#endif

	int_t *LA = A;
	for(j=0; j < n-1; j++){	

		int_t i = j+1;
		int_t max = j+1;

		while(i<n){

			int_t k = 0;
			while(s[j+k]==s[i+k]) k++;

			if(s[j+k]<s[i+k]){
				i+=k+1;
				max=i;
			}
			else{
				LA[j]=max-j;	
				break;
			}
		}
	}
	LA[n-1]=1;

	#if TIME
		printf("TOTAL:\n");
		fprintf(stderr,"%.6lf\n", tstop(t_total, c_total)); 
	#endif

return 0;
}

/**********************************************************************/

