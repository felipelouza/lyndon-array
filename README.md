# lyndon-array

This code implements a set of algorithms to compute the Lyndon-array (LA) of a string.

## Build requirements

An ANSI C Compiler (e.g. GNU GCC)

## Example

**Compilation:**

```sh
make
```

**Run a test:**

```c
./main dataset/input.txt -A 8 -c
```

**Output:**

```c
./main dataset/input.txt -A 7 -c -p 10
d = 1
N = 7 bytes
sizeof(int) = 4 bytes
## SACAK_LYNDON 9n (non-linear) ##
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

\[2\] Nong, G., Practical linear-time O(1)-workspace suffix sorting for constant alphabets, ACM Trans. Inform. Syst., vol. 31, no. 3, pp. 1-15, 2013


## Thanks

Thanks to Uwe Baier for kindly providing the source code of algorithm Baier-Lyndon: [link](https://github.com/felipelouza/lyndon-array/tree/master/experiments/gsaca\_cl).
