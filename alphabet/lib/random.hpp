#ifndef RANDOM_H
#define RANDOM_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <random>

#ifndef DEBUG
  #define DEBUG 0
#endif

using namespace std;

int random(unsigned char* str, size_t n, int verbose);

#endif
