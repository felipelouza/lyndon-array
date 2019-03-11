#include "sacak-lyndon.h"

// set only the highest bit as 1, i.e. 1000...
//const unsigned int EMPTY_k=((unsigned int)1)<<(sizeof(unsigned int)*8-1); 
const uint_t EMPTY_k=((uint_t)1)<<(sizeof(uint_t)*8-1); 

// get s[i] at a certain level
#define chr(i) (cs==sizeof(int_t)?((int_t*)s)[i]:((unsigned char *)s)[i])

#ifndef SAVE_SPACE 
  #define SAVE_SPACE 1 
#endif

#define true 1
#define false 0

#define DEPTH 0
#define TIME  0

/**/

void t_start(time_t *t_time, clock_t *c_clock){

	*t_time = time(NULL);
	*c_clock =  clock();
}

double t_stop(time_t t_time, clock_t c_clock){

	double aux1 = (clock() - c_clock) / (double)(CLOCKS_PER_SEC);
	double aux2 = difftime (time(NULL),t_time);
	printf("CLOCK = %lf TIME = %lf\n", aux1, aux2);
	return aux1;
}

/**/

int compare (const void * a, const void * b){
  if(*(const uint_t *)a < *(const uint_t *)b) return -1;
  if(*(const uint_t *)a > *(const uint_t *)b) return 1;
return 0;
}

int type_cmp(void *a, void *b){ return (*(uint_t*)a)-(*(uint_t*)b); }

/*****************************************************************************/

void getBuckets_k(int_t *s, 
  uint_t *bkt, uint_t n,
  unsigned int K, int end, int cs) { 
  uint_t i, sum=0;
  
  // clear all buckets .
  for(i=0; i<K; i++) bkt[i]=0; 
  
  // compute the size of each bucket .
  for(i=0; i<n; i++) bkt[chr(i)]++; 
  
  for(i=0; i<K; i++) { 
    sum+=bkt[i]; 
    bkt[i]=end ? sum-1 : sum-bkt[i]; 
  }
}

void putSuffix0(uint_t *SA, 
  int_t *s, uint_t *bkt, 
  uint_t n, unsigned int K, int_t n1, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  // put the suffixes into their buckets.
  for(i=n1-1; i>0; i--) {
    j=SA[i]; SA[i]=0;
    SA[bkt[chr(j)]--]=j;
  }
  SA[0]=n-1; // set the single sentinel suffix.
}

/*****************************************************************************/

void induceSAl0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the head of each bucket.
  getBuckets_k(s, bkt, n, K, false, cs);

  bkt[0]++; // skip the virtual sentinel.
  for(i=0; i<n; i++)
    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)>=chr(j+1)) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]++;
        if(!suffix && i>0) SA[i]=0;
      }
    }
}

/*****************************************************************************/

void induceSAs0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  for(i=n-1; i>0; i--)
    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
}

/*****************************************************************************/

void induceSAs0_LA(uint_t *SA, int_t *LA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  for(i=n-1; i>0; i--){

		j=SA[i];
		if(!LA[j]){// LA[j]=1;
				uint_t l=j+1;
				//while(LA[l++]!=0);
				//LA[j]=l-j-1;
				while(LA[l]!=0) l+=LA[l];//jumping
				LA[j]=l-j;
		}

    if(SA[i]>0) {

      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
	}

	LA[n-1]=1;
}
/*****************************************************************************/

void induceSAs0_LA_17n(uint_t *SA, int_t *LA, int_t *PREV, int_t *NEXT,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  for(i=n-1; i>0; i--){

		j=SA[i];

		LA[j]=NEXT[j]-j;	
		NEXT[PREV[j]]=NEXT[j];
		PREV[NEXT[j]]=PREV[j];

    if(SA[i]>0) {

      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
	}

	LA[n-1]=1;
}
/*****************************************************************************/

void induceSAs0_LA_13n(uint_t *SA, int_t *LA, int_t *PREV,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  for(i=n-1; i>0; i--){

		j=SA[i];

		int_t next, prev=PREV[j];
		if(LA[j+1]==0) next=j+1;
		else next=PREV[j+1];

		LA[j]=next-j;	

		PREV[next]=prev;
		PREV[prev+1]=next;

    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
	}

	LA[n-1]=1;
}
/*****************************************************************************/

void induceSAs0_LA_9n(uint_t *SA, int_t *LA, 
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

//  int_t *PREV=LA;

  for(i=n-1; i>1; i--){

		j=SA[i];

		int_t next, prev;

    if(SA[i]>0) {


      if(LA[j-1]==0) prev=j-1;
      else prev=LA[j-1];

		  if(LA[j+1]==0) next=j+1;
	    else next=LA[j];

		  LA[next-1]=prev;
		  LA[prev]=next;
//		LA[j]=NEXT[j]-j;	
//		NEXT[PREV[j]]=NEXT[j];
//		PREV[NEXT[j]]=PREV[j];


      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) { //Inducing S-type
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
    /*
    else{
		  if(LA[j+1]==0) next=j+1;
	    else next=LA[j];

		  LA[next-1]=j;
		  LA[j]=next;
    
    }
    */
	}

	LA[n-1]=1;
}

/*****************************************************************************/

void putSubstr0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int cs) {
  uint_t i, cur_t, succ_t;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  // set each item in SA as empty.
  for(i=0; i<n; i++) SA[i]=0;

  succ_t=0; // s[n-2] must be L-type.
  for(i=n-2; i>0; i--) {
    cur_t=(chr(i-1)<chr(i) ||
           (chr(i-1)==chr(i) && succ_t==1)
          )?1:0;
    if(cur_t==0 && succ_t==1) SA[bkt[chr(i)]--]=i;
    succ_t=cur_t;
  }

  // set the single sentinel LMS-substring.
  SA[0]=n-1;
}

/*****************************************************************************/

void putSuffix1(int_t *SA, int_t *s, int_t n1, int cs) {
  int_t i, j, pos=n1, cur, pre=-1;
  
  for(i=n1-1; i>0; i--) {
    j=SA[i]; SA[i]=EMPTY_k;
    cur=chr(j);
    if(cur!=pre) {
      pre=cur; pos=cur;
    }
    SA[pos--]=j;
  }
}

void induceSAl1(int_t *SA, int_t *s, 
  int_t n, int_t suffix, int cs) {
  int_t h, i, j, step=1;
  
  for(i=0; i<n; i+=step) {
    step=1; j=SA[i]-1;
    if(SA[i]<=0) continue;
    int_t c=chr(j), c1=chr(j+1);
    int_t isL=c>=c1;
    if(!isL) continue;

    // s[j] is L-type.

    int_t d=SA[c];
    if(d>=0) {
      // SA[c] is borrowed by the left
      //   neighbor bucket.
      // shift-left the items in the
      //   left neighbor bucket.
      int_t foo, bar;
      foo=SA[c];
      for(h=c-1; SA[h]>=0||SA[h]==EMPTY_k; h--)
      { bar=SA[h]; SA[h]=foo; foo=bar; }
      SA[h]=foo;
      if(h<i) step=0;

      d=EMPTY_k;
    }

    if(d==EMPTY_k) { // SA[c] is empty.
      if(c<n-1 && SA[c+1]==EMPTY_k) {
        SA[c]=-1; // init the counter.
        SA[c+1]=j;
      }
      else        
        SA[c]=j; // a size-1 bucket.
    }
    else { // SA[c] is reused as a counter.
        int_t pos=c-d+1;
        if(pos>n-1 || SA[pos]!=EMPTY_k) {
          // we are running into the right
          //   neighbor bucket.
          // shift-left one step the items
          //   of bucket(SA, S, j).
          for(h=0; h<-d; h++)
            SA[c+h]=SA[c+h+1];
          pos--;
          if(c<i) step=0;
        }
        else
          SA[c]--;

        SA[pos]=j;
    }

    int_t c2;
    int_t isL1=(j+1<n-1) && (c1>(c2=chr(j+2)) || (c1==c2 && c1<i));  // is s[SA[i]] L-type?
    if((!suffix || !isL1) && i>0) {
      int_t i1=(step==0)?i-1:i;
      SA[i1]=EMPTY_k;
    }
  }

  // scan to shift-left the items in each bucket 
  //   with its head being reused as a counter.
  for(i=1; i<n; i++) {
    j=SA[i];
    if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
      for(h=0; h<-j; h++)
        SA[i+h]=SA[i+h+1];
      SA[i+h]=EMPTY_k;
    }
  }
}

void induceSAs1(int_t *SA, int_t *s, 
  int_t n, int_t suffix, int cs) {
  int_t h, i, j, step=1;
  
  for(i=n-1; i>0; i-=step) {
    step=1; j=SA[i]-1;
    if(SA[i]<=0) continue;
    int_t c=chr(j), c1=chr(j+1);
    int_t isS=(c<c1) || (c==c1 && c>i);
    if(!isS) continue;

    // s[j] is S-type

    int_t d=SA[c];
    if(d>=0) {
      // SA[c] is borrowed by the right
      //   neighbor bucket.
      // shift-right the items in the
      //   right neighbor bucket.
      int_t foo, bar;
      foo=SA[c];
      for(h=c+1; SA[h]>=0||SA[h]==EMPTY_k; h++)
      { bar=SA[h]; SA[h]=foo; foo=bar; }
      SA[h]=foo;
      if(h>i) step=0;

      d=EMPTY_k;
    }

    if(d==EMPTY_k) { // SA[c] is empty.
      if(SA[c-1]==EMPTY_k) {
        SA[c]=-1; // init the counter.
        SA[c-1]=j;
      }
      else
        SA[c]=j; // a size-1 bucket.
    }
    else { // SA[c] is reused as a counter.
        int_t pos=c+d-1;
        if(SA[pos]!=EMPTY_k) {
          // we are running into the left
          //   neighbor bucket.
          // shift-right one step the items 
          //   of bucket(SA, S, j).
          for(h=0; h<-d; h++)
            SA[c-h]=SA[c-h-1];
          pos++;
          if(c>i) step=0;
        }
        else
          SA[c]--;

        SA[pos]=j;
    }

    if(!suffix) {
      int_t i1=(step==0)?i+1:i;
      SA[i1]=EMPTY_k;
    }
  }

  // scan to shift-right the items in each bucket
  //   with its head being reused as a counter.
  if(!suffix)
    for(i=n-1; i>0; i--) {
      j=SA[i];
      if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
        for(h=0; h<-j; h++)
          SA[i-h]=SA[i-h-1];
        SA[i-h]=EMPTY_k;
      }
    }
}

void putSubstr1(int_t *SA, int_t *s, int_t n, int cs) {
  int_t h, i, j;

  for(i=0; i<n; i++) SA[i]=EMPTY_k;

  int_t c, c1, t, t1;
  c1=chr(n-2);
  t1=0; 
  for(i=n-2; i>0; i--) {
    c=c1; t=t1; 
    c1=chr(i-1);
    t1=c1<c || (c1==c && t);
    if(t && !t1) {
      if(SA[c]>=0) {
        // SA[c] is borrowed by the right
        //   neighbor bucket.
        // shift-right the items in the
        //   right neighbor bucket.
        int_t foo, bar;
        foo=SA[c];
        for(h=c+1; SA[h]>=0; h++)
        { bar=SA[h]; SA[h]=foo; foo=bar; }
        SA[h]=foo;

        SA[c]=EMPTY_k;
      }

      int_t d=SA[c];
      if(d==EMPTY_k) { // SA[c] is empty.
        if(SA[c-1]==EMPTY_k) {
          SA[c]=-1; // init the counter.
          SA[c-1]=i;
        }
        else
          SA[c]=i; // a size-1 bucket.
      }
      else { // SA[c] is reused as a counter
          int_t pos=c+d-1;
          if(SA[pos]!=EMPTY_k) {
            // we are running into the left
            //   neighbor bucket.
            // shift-right one step the items 
            //   of bucket(SA, S, i).
            for(h=0; h<-d; h++)
              SA[c-h]=SA[c-h-1];
            pos++;
          }
          else
            SA[c]--;

          SA[pos]=i;
      }
    }
  }

  // scan to shift-right the items in each bucket
  //   with its head being reused as a counter.
  for(i=n-1; i>0; i--) {
    j=SA[i];
    if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
      for(h=0; h<-j; h++)
        SA[i-h]=SA[i-h-1];
      SA[i-h]=EMPTY_k;
    }
  }

  // put the single sentinel LMS-substring.
  SA[0]=n-1;
}

uint_t getLengthOfLMS(int_t *s, 
  uint_t n, int level, uint_t x, int cs) {
  if(x==n-1) return 1;  
  
  uint_t dist=0, i=1;  
  while(1) {
    if(chr(x+i)<chr(x+i-1)) break;
    i++;
  }  
  while(1) {
    if(x+i>n-1 || chr(x+i)>chr(x+i-1)) break;
    if(x+i==n-1 || chr(x+i)<chr(x+i-1)) dist=i;
    i++;
  }
  
  return dist+1;
}

/*****************************************************************************/

uint_t nameSubstr(uint_t *SA, 
  int_t *s, uint_t *s1, uint_t n, 
  uint_t m, uint_t n1, int level, int cs) {
  uint_t i, j, cur_t, succ_t;

  // init the name array buffer
  for(i=n1; i<n; i++) SA[i]=EMPTY_k;

  // scan to compute the interim s1
  uint_t name=0, name_ctr=0;
  uint_t pre_pos=n-1, pre_len=0;
  for(i=0; i<n1; i++) {
    int diff=false;
    uint_t len, pos=SA[i];

    uint_t d;
    len=getLengthOfLMS(s, n, level, pos, cs);
    if(len!=pre_len) diff=true;
    else
      for(d=0; d<len; d++)
        if(pos+d==n-1 || pre_pos+d==n-1 ||
           chr(pos+d)!=chr(pre_pos+d)) {
          diff=true; break;
        }

    if(diff) {
      name=i; name_ctr++;
      SA[name]=1; // a new name.
      pre_pos=pos; pre_len=len;
    }
    else
      SA[name]++; // count this name.

    SA[n1+pos/2]=name;
  }

  // compact the interim s1 sparsely stored 
  //   in SA[n1, n-1] into SA[m-n1, m-1].
  for(i=n-1, j=m-1; i>=n1; i--)
    if(SA[i]!=EMPTY_k) SA[j--]=SA[i];

  // rename each S-type character of the
  //   interim s1 as the end of its bucket
  //   to produce the final s1.
  succ_t=1;
  for(i=n1-1; i>0; i--) {
    int_t ch=s1[i], ch1=s1[i-1];
    cur_t=(ch1< ch || (ch1==ch && succ_t==1))?1:0;
    if(cur_t==1) {
      s1[i-1]+=SA[s1[i-1]]-1;
    }
    succ_t=cur_t;
  }

  return name_ctr;
}

/*****************************************************************************/
// find lms position and stores it on s1 (in text order)

void getSAlms(uint_t *SA, 
  int_t *s, 
  uint_t *s1, uint_t n, 
  uint_t n1, int level, int cs) {
  uint_t i, j, cur_t, succ_t;

  j=n1-1; s1[j--]=n-1;
  succ_t=0; // s[n-2] must be L-type
  for(i=n-2; i>0; i--) {
    cur_t=(chr(i-1)<chr(i) ||
          (chr(i-1)==chr(i) && succ_t==1))?1:0;
    if(cur_t==0 && succ_t==1) s1[j--]=i;
    succ_t=cur_t;
  }
}

/*****************************************************************************/

int_t SACA_K(int_t *s, uint_t *SA,
  uint_t n, unsigned int K,
  uint_t m, int cs, int level) {
  uint_t i;
  uint_t *bkt=NULL;

  #if TIME
    time_t t_time = 0; 
    clock_t c_time = 0; 
  #endif

  #if DEPTH
  time_t t_start = time(NULL);
  clock_t c_start =  clock();
  #endif

  // stage 1: reduce the problem by at least 1/2.
  if(level==0) {

    bkt=(uint_t *)malloc(sizeof(int_t)*K);
    putSubstr0(SA, s, bkt, n, K, cs);

    #if DEBUG
    printf("bucket LMS-subs\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif
    induceSAl0(SA, s, bkt, n, K, false, cs);

    #if DEBUG
    printf("L-type\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif

    induceSAs0(SA, s, bkt, n, K, false, cs);

    #if DEBUG
    printf("S-type\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif
  }
  else {
    putSubstr1((int_t *)SA, (int_t *)s,(int_t)n, cs);
    induceSAl1((int_t *)SA, (int_t *)s, n ,false, cs);
    induceSAs1((int_t *)SA, (int_t *)s, n, false, cs);
  }

  // now, all the LMS-substrings are sorted and 
  //   stored sparsely in SA.

  // compact all the sorted substrings into
  //   the first n1 items of SA.
  // 2*n1 must be not larger than n.
  uint_t n1=0;
  for(i=0; i<n; i++) 
    if((!level&&SA[i]>0) || (level&&((int_t *)SA)[i]>0))
      SA[n1++]=SA[i];

  #if DEBUG
  if(level==0) {
    printf("SA1\n");
    for(i=0; i<n1; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
  }
  #endif

  uint_t *SA1=SA, *s1=SA+m-n1;
  uint_t name_ctr;
  name_ctr=nameSubstr(SA,s,s1,n,m,n1,level,cs);

  // stage 2: solve the reduced problem.
  int_t depth=1;
  // recurse if names are not yet unique.
  if(name_ctr<n1)
    depth += SACA_K((int_t*)s1, SA1, 
          n1, 0, m-n1, sizeof(int_t), level+1);
  else // get the suffix array of s1 directly.
    for(i=0; i<n1; i++) SA1[s1[i]]=i;

  // stage 3: induce SA(S) from SA(S1).

  #if DEBUG
  if(level==0) {
    printf("SA1\n");
    for(i=0; i<n1; i++)
          printf("%" PRIdN "\t", SA1[i]);
    printf("\n");
    printf("\nstage 3:\n\n");
  }
  #endif
  
  getSAlms(SA, s, s1, n, n1, level, cs);

  for(i=0; i<n1; i++) SA[i]=s1[SA[i]];
  for(i=n1; i<n; i++) SA[i]=level?EMPTY_k:0; 

  #if DEBUG
  if(level==0) {
    printf("SA\n");
    for(i=0; i<n; i++)
      printf("%" PRIdN "\t", SA[i]);
    printf("\n");
  }
  #endif

  if(level==0) {

    #if TIME
      t_start(&t_time, &c_time);
    #endif

    putSuffix0(SA, s, bkt, n, K, n1, cs);

    #if TIME
      printf("SA (mapped):\n");
      t_stop(t_time, c_time);
    #endif

    #if DEBUG
    printf("SA (mapped)\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]+1);
    printf("\n");
    #endif

    #if TIME
      t_start(&t_time, &c_time);
    #endif

    induceSAl0(SA, s, bkt, n, K, true, cs);

    #if TIME
      printf("L-type:\n");
      t_stop(t_time, c_time);
    #endif

	  #if DEBUG
	  printf("L-type\n");
	  for(i=0; i<n; i++)
	        printf("%" PRIdN "\t", SA[i]+1);
	  printf("\n");
	  #endif

    #if TIME
      t_start(&t_time, &c_time);
    #endif
    induceSAs0(SA, s, bkt, n, K, true, cs);

		#if DEBUG
  	printf("S-type\n");
  	for(i=0; i<n; i++)
  	      printf("%" PRIdN "\t", SA[i]+1);
  	printf("\n");
  	#endif

    #if TIME
      printf("S-type:\n");
      t_stop(t_time, c_time);
    #endif

    free(bkt);
  }
  else {
    putSuffix1((int_t *)SA, (int_t *)s, n1, cs);
    induceSAl1((int_t *)SA, (int_t *)s, n, true, cs);
    induceSAs1((int_t *)SA, (int_t *)s, n, true, cs);
  }

  #if DEPTH
  printf("depth %" PRIdN ":\nname_ctr = %" PRIdN ", n1 =%" PRIdN ", n = %" PRIdN "\n", depth, name_ctr, n1, n);
  t_stop(t_start, c_start);
  #endif

return depth;
}
/*****************************************************************************/

int_t SACA_K_LA(int_t *s, uint_t *SA, int_t *LA,
  uint_t n, unsigned int K,
  uint_t m, int cs, int level, 
  int MODE) {  //MODE: 1 (17n linear), 2 (13n linear), 3 (9n linear), 0 (9n non-linear), 

  uint_t i;
  uint_t *bkt=NULL;

	int_t *PREV=NULL;
	int_t *NEXT=NULL;


	if(MODE==1)
		NEXT = (int_t*) malloc(n*sizeof(int_t));

	if(MODE==1 || MODE==2)
		PREV = (int_t*) malloc(n*sizeof(int_t));

  for(i=0;i<n;i++)
		SA[i]=LA[i]=0;
  
	if(MODE==1){
		for(i=0;i<n-1;i++){
				NEXT[i]=i+1;
		}
		NEXT[n-1]=n-1;
	}

	if(MODE==1 || MODE==2){
		PREV[0]=0;
		for(i=1;i<n-1;i++){
				PREV[i]=i-1;
		}
		PREV[n-1]=n-2;
	}

/*
	if(MODE==3){
    for(i=0;i<n-1;i++) LA[i]=i+1;
    LA[n-1]=n-1;
  }
*/

  #if TIME
    time_t t_time = 0; 
    clock_t c_time = 0; 
  #endif

  #if DEPTH
  time_t t_start = time(NULL);
  clock_t c_start =  clock();
  #endif

  // stage 1: reduce the problem by at least 1/2.
  bkt=(uint_t *)malloc(sizeof(int_t)*K);
  putSubstr0(SA, s, bkt, n, K, cs);

  #if DEBUG
  printf("bucket LMS-subs\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]);
  printf("\n");
  #endif
  induceSAl0(SA, s, bkt, n, K, false, cs);

  #if DEBUG
  printf("L-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]);
  printf("\n");
  #endif

  induceSAs0(SA, s, bkt, n, K, false, cs);

  #if DEBUG
  printf("S-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]);
  printf("\n");
  #endif

  // now, all the LMS-substrings are sorted and 
  //   stored sparsely in SA.

  // compact all the sorted substrings into
  //   the first n1 items of SA.
  // 2*n1 must be not larger than n.
  uint_t n1=0;
  for(i=0; i<n; i++) 
    if((!level&&SA[i]>0) || (level&&((int_t *)SA)[i]>0))
      SA[n1++]=SA[i];

  #if DEBUG
  printf("SA1\n");
  for(i=0; i<n1; i++)
        printf("%" PRIdN "\t", SA[i]);
  printf("\n");
  #endif

  uint_t *SA1=SA, *s1=SA+m-n1;
  uint_t name_ctr;
  name_ctr=nameSubstr(SA,s,s1,n,m,n1,level,cs);

  // stage 2: solve the reduced problem.
  int_t depth=1;
  // recurse if names are not yet unique.
  if(name_ctr<n1)
    depth += SACA_K((int_t*)s1, SA1, 
          n1, 0, m-n1, sizeof(int_t), level+1);
  else // get the suffix array of s1 directly.
    for(i=0; i<n1; i++) SA1[s1[i]]=i;

  // stage 3: induce SA(S) from SA(S1).

  #if DEBUG
  printf("SA1\n");
  for(i=0; i<n1; i++)
        printf("%" PRIdN "\t", SA1[i]);
  printf("\n");
  printf("\nstage 3:\n\n");
  #endif
  
  getSAlms(SA, s, s1, n, n1, level, cs);

  for(i=0; i<n1; i++) SA[i]=s1[SA[i]];
  for(i=n1; i<n; i++) SA[i]=level?EMPTY_k:0; 

  #if DEBUG
  printf("SA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]);
  printf("\n\n");
  #endif

  #if TIME
    t_start(&t_time, &c_time);
  #endif

  putSuffix0(SA, s, bkt, n, K, n1, cs);

  #if TIME
    printf("SA (mapped):\n");
    t_stop(t_time, c_time);
  #endif

  #if DEBUG
  printf("SA (mapped)\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
	printf("LA\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LA[i]);
  printf("\n\n");
  #endif

  #if TIME
    t_start(&t_time, &c_time);
  #endif

  induceSAl0(SA, s, bkt, n, K, true, cs);

  #if TIME
    printf("L-type:\n");
    t_stop(t_time, c_time);
  #endif

	#if DEBUG
	printf("L-type\n");
	for(i=0; i<n; i++)
		printf("%" PRIdN "\t", SA[i]+1);
	printf("\n");
	printf("LA\n");
  for(i=0; i<n; i++)
		printf("%" PRIdN "\t", LA[i]);
	printf("\n\n");
	if(MODE==1){
		printf("NEXT\n");
		for(i=0; i<n; i++)
			printf("%" PRIdN "\t", NEXT[i]+1);
	  printf("\n\n");
	}
	if(MODE==1 || MODE==2){
		printf("PREV\n");
		for(i=0; i<n; i++)
			printf("%" PRIdN "\t", PREV[i]+1);
	  printf("\n\n");
	}
	#endif

  #if TIME
    t_start(&t_time, &c_time);
  #endif

	if(MODE==1)
	  induceSAs0_LA_17n(SA, LA, PREV, NEXT, s, bkt, n, K, true, cs);
	else if(MODE==2)
	  induceSAs0_LA_13n(SA, LA, PREV, s, bkt, n, K, true, cs);
	else if(MODE==3){
	  induceSAs0_LA_9n(SA, LA, s, bkt, n, K, true, cs);
    //overwrite the pointer list with LA
    for(i=0; i<n; i++) 
      if(LA[i]>i) LA[i]=LA[i]-i;
      else  LA[i]=1;
  }
	else
	  induceSAs0_LA(SA, LA, s, bkt, n, K, true, cs);

	#if DEBUG
  printf("S-type\n");
  for(i=0; i<n; i++)
		printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
	printf("LA\n");
  for(i=0; i<n; i++)
		printf("%" PRIdN "\t", LA[i]);
	printf("\n\n");
	if(MODE==1){
		printf("NEXT\n");
		for(i=0; i<n; i++)
			printf("%" PRIdN "\t", NEXT[i]+1);
	  printf("\n\n");
	}
	if(MODE==1 || MODE==2){
		printf("PREV\n");
		for(i=0; i<n; i++)
			printf("%" PRIdN "\t", PREV[i]+1);
	  printf("\n\n");
	}
  #endif

  #if TIME
    printf("S-type:\n");
    t_stop(t_time, c_time);
  #endif

  free(bkt);
  
	if(MODE==1)
		free(NEXT);

	if(MODE==1 || MODE==2)
		free(PREV);

  #if DEPTH
  printf("depth %" PRIdN ":\nname_ctr = %" PRIdN ", n1 =%" PRIdN ", n = %" PRIdN "\n", depth, name_ctr, n1, n);
  t_stop(t_start, c_start);
  #endif

return depth;
}

/*****************************************************************************/

int sacak(unsigned char *s, uint_t *SA, uint_t n){
  if((s == NULL) || (SA == NULL) || (n < 0)) return -1;
  return SACA_K((int_t*)s, (uint_t*)SA, n, 256, n, sizeof(char), 0);
}

int sacak_lyndon_9n_non_linear(unsigned char *s, uint_t *SA, int_t* LA, uint_t n){
  if((s == NULL) || (SA == NULL) || (LA == NULL) || (n < 0)) return -1;
  return SACA_K_LA((int_t*)s, (uint_t*)SA, (int_t*)LA, n, 256, n, sizeof(char), 0, 0);
}

int sacak_lyndon_17n_linear(unsigned char *s, uint_t *SA, int_t* LA, uint_t n){
  if((s == NULL) || (SA == NULL) || (LA == NULL) || (n < 0)) return -1;
  return SACA_K_LA((int_t*)s, (uint_t*)SA, (int_t*)LA, n, 256, n, sizeof(char), 0, 1);
}

int sacak_lyndon_13n_linear(unsigned char *s, uint_t *SA, int_t* LA, uint_t n){
  if((s == NULL) || (SA == NULL) || (LA == NULL) || (n < 0)) return -1;
  return SACA_K_LA((int_t*)s, (uint_t*)SA, (int_t*)LA, n, 256, n, sizeof(char), 0, 2);
}

int sacak_lyndon_9n_linear(unsigned char *s, uint_t *SA, int_t* LA, uint_t n){
  if((s == NULL) || (SA == NULL) || (LA == NULL) || (n < 0)) return -1;
  return SACA_K_LA((int_t*)s, (uint_t*)SA, (int_t*)LA, n, 256, n, sizeof(char), 0, 3);
}


/*****************************************************************************/
