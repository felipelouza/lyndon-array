# lyndon-array

This repository contains a set of algorithms to compute the **Lyndon-array** (LA) for a string T\[1,n\].

## Introduction

In particular, we introduce algorithm **SACA-K+LA** \[1\] that computes **LA** together with the **suffix array** (SA) for T\[1,n\] in linear time.

SACA-K+LA extends the optimal suffix sorting algorithm **SACA-K** \[2\] to also compute LA in linear time using O(\sigma) words of additional space, which is optimal for alphabets of constant size. 

Experimental results have shown that our algorithm is competitive in time and space, compared to solutions that compute only LA. 

We provide four versions of SACA-K+LA, the last version, option ``-A 10``, corresponds to the **optimal solution**.


## Build requirements

An ANSI C Compiler (e.g. GNU GCC)

## Example

**Compilation:**

```sh
make
```

**Available options:**

```sh
-A a  preferred algorithm to use (default 1)
-c    check output (for debug)
-p P  print the output arrays LA[1,P] and SA[1,P] (for debug)
-d D  use the first D documents of the INPUT
-v    verbose output
-o    output computed arrays to disk (INPUT.la and INPUT.sa)
-s    computes some statistics for LA
-h    this help message
```
_Notes:_ 
- Supported extensions are _.txt_, _.fasta_ and _.fastq_.

**Algorithms:**

| -A | Algorithm              | Output   |   Running time  | Total space           | Auxiliary arrays |
|:--:|------------------------|----------|:---------------:|-----------------------|:----------------:|
|  1 | Lyndon_NSV **\[3\]**   |    LA    |       O(n)      | 9n bytes + O(n)-words |    ISA+Stack     |
|  2 | GSACA_LA  **\[4\]**    |    LA    |       O(n)      | 17n bytes             |  GSIZE+PREV+ISA  |
|  3 | MAX_LYN **\[5\]**      |    LA    |      O(n^2)     | 5n bytes              |                  |
|  4 | Lyndon_BWT **\[6\]**   |    LA    |       O(n)      | 9n bytes + O(n)-words |     LF+Stack     |
|  5 | BWT_INPLACE_LA         | LA + BWT |      O(n^2)     | 5n bytes              |                  |
|  6 | GSACA_LA+SA **\[4\]**  |  LA + SA |       O(n)      | 17n bytes             |  GSIZE+PREV+ISA  |
|  7 | SACA-K+LA              |  LA + SA | O(n*avg_lyndon) | 9n bytes              |                  |
|  8 | SACA-K+LA\_17n         |  LA + SA |       O(n)      | 17n bytes             |     PREV+NEXT    |
|  9 | SACA-K+LA\_13n         |  LA + SA |       O(n)      | 13n bytes             |       PREV       |
| 10 | SACA-K+LA\_9n          |  LA + SA |       O(n)      | 9n bytes              |                  |

_Notes:_ 
- BWT_INPLACE_LYN is a modification of the BWT-Inplace algorithm by Crochemore et al. \[7\] to also compute LA. 

- SACA-K+LA \[1\] is a modification of the suffix sorting algorithm SACAK by Nong \[2\] to also compute LA.


**Run a test:**

```c
./main dataset/input.txt -A 10 -p 10
```

**Output:**

```c
d = 1
N = 7 bytes
sizeof(int) = 4 bytes
## SACAK_LYNDON 9n (linear) ##
TOTAL:
CLOCK = 0.000039 TIME = 0.000000
0.000039
########
i) LA	SA	suffixes
0) 1	6	banana#
1) 2	5	anana#
2) 1	3	nana#
3) 2	1	ana#
4) 1	0	na#
5) 1	4	a#
6) 1	2	#
########
malloc_count ### exiting, total: 24,478, peak: 21,272, current: 1,144
```

## References

\[1\] 
Louza, F. A., Mantaci, S., Manzini G., Sciortino, M., Telles, G. P.: Inducing the Lyndon Array, arXiv preprint [arXiv:XXXX.XXXX](https://arxiv.org/abs/XXXX.XXXX)

\[2\] Nong, G., Practical linear-time O(1)-workspace suffix sorting for constant alphabets, ACM Trans. Inform. Syst., vol. 31, no. 3, pp. 1-15, 2013

\[3\] Christophe Hohlweg, Christophe Reutenauer: Lyndon words, permutations and trees. Theor. Comput. Sci. 307(1): 173-178 (2003)

\[4\] Uwe Baier: Linear-time Suffix Sorting - A New Approach for Suffix Array Construction. CPM 2016: 23:1-23:12 [link](https://doi.org/10.4230/LIPIcs.CPM.2016.23)

\[5\] Frantisek Franek, A. S. M. Sohidull Islam, Mohammad Sohel Rahman, William F. Smyth:
Algorithms to Compute the Lyndon Array. Stringology 2016: 172-184

\[6\] Louza, F. A., Smyth W. F., Manzini G., Telles, G. P.: Lyndon Array Construction during Burrows-Wheeler Inversion, J. Discrete Algorithms, vol. 50, pp. 2-9, 2018 [link](https://www.sciencedirect.com/science/article/pii/S1570866718301254)

\[7\] Maxime Crochemore, Roberto Grossi, Juha Kärkkäinen, Gad M. Landau: Computing the Burrows-Wheeler transform in place and in small space. J. Discrete Algorithms 32: 44-52 (2015) [link](https://doi.org/10.1016/j.jda.2015.01.004)



## Thanks

Thanks to [Uwe Baier](https://github.com/waYne1337/) for kindly providing the source codes for GSACA_LA and GSACA_LA+SA: [link](https://github.com/felipelouza/sacak-lyndon/tree/master/external/gsaca_cl).
