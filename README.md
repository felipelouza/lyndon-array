# lyndon-array

This code implements a set of algorithms to compute the Lyndon-array (LA) of a string.

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
-h    this help message
```
_Notes:_ 
- Supported extensions are _.txt_, _.fasta_ and _.fastq_.

**Algorithms:**

| -A | Algorithm              | Output   |   Running time  | Working space | Auxiliary arrays |
|:--:|------------------------|----------|:---------------:|:-------------:|:----------------:|
|  1 | Lyndon_NSV             |    LA    |       O(n)      |   O(n)-words  |       Stack      |
|  2 | GSACA_LYN **\[2\]**    |    LA    |       O(n)      |   12n bytes   |  GSIZE+PREV+ISA  |
|  3 | MAX_LYN                |    LA    |      O(n^2)     |      O(1)     |                  |
|  4 | Lyndon_BWT **\[1\]**   | LA + BWT |       O(n)      |   O(n)-words  |       Stack      |
|  5 | BWT_INPLACE_LYN        | LA + BWT |      O(n^2)     |      O(1)     |                  |
|  6 | GSACA_LYN_SA **\[2\]** |  LA + SA |       O(n)      |   12n bytes   |  GSIZE+PREV+ISA  |
|  7 | SACAK_LYN              |  LA + SA | O(n*avg_lyndon) |      O(1)     |                  |
|  8 | SACAK_LYN_17n          |  LA + SA |       O(n)      |    8n bytes   |     PREV+NEXT    |
|  9 | SACAK_LYN_13n          |  LA + SA |       O(n)      |    4n bytes   |       PREV       |
| 10 | SACAK_LYN_9n           |  LA + SA |       O(n)      |      O(1)     |                  |

**Run a test:**

```c
./main dataset/input.txt -c -p 10 -A 10
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
LA: isLyndonArray!!
SA: isSorted!!
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
Louza, F. A., & Smyth W. F., Manzini G., Telles, G. P.: Lyndon Array Construction during Burrows-Wheeler Inversion, J. Discrete Algorithms, vol. 50, pp. 2-9, 2018 [link](https://www.sciencedirect.com/science/article/pii/S1570866718301254)

\[2\] Uwe Baier: Linear-time Suffix Sorting - A New Approach for Suffix Array Construction. CPM 2016: 23:1-23:12 [link](https://doi.org/10.4230/LIPIcs.CPM.2016.23)

\[3\] Maxime Crochemore, Roberto Grossi, Juha Kärkkäinen, Gad M. Landau: Computing the Burrows-Wheeler transform in place and in small space. J. Discrete Algorithms 32: 44-52 (2015) [link](https://doi.org/10.1016/j.jda.2015.01.004)

\[4\] Nong, G., Practical linear-time O(1)-workspace suffix sorting for constant alphabets, ACM Trans. Inform. Syst., vol. 31, no. 3, pp. 1-15, 2013


## Thanks

Thanks to Uwe Baier for kindly providing the source code of algorithm GSACA_LYN_SA: [link](https://github.com/felipelouza/sacak-lyndon/tree/master/external/gsaca_cl).
