#ifndef RANDH
#define RANDH

///////////////////////////////////////////////////
//
// This header file is for use with rand.cpp, randf.cpp, and randmt.cpp
//
//
// WARNING: INIT REQUIRED before use!!!
//

// WARNING: the following includes may vary by OS, since these
// libraries may be in different places on different machines.
//#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// init
void     initRand();                       // WARNING: init required before use!!!
void     initRand(unsigned long long int a, unsigned long long int b);  // init with 2 seeds

// simple calls
unsigned long long int randULL();          // 64 bits random number
double   randUnit();                       // random [0,1)
double   randPMUnit();                     // random [-1,1)
int      randMod(int m);                   // random int in [0,m-1]
void     randMod2(int m, int &a, int &b);  // two random numbers [0,m-1] that are not equal
int      randMask(unsigned long long int mask);  // random in bit mask
bool     choose(double prob);              // true with probability prob
unsigned long long randCoinToss();         // 50:50 true or false
bool     choose8(int eigth);               // fast choose based on prob in 1/8 increments
bool     chooseMask(unsigned long long int mask, int prob); 
double   randNorm(double stddev);          // normal distribution with (mean=0)
double   randCauchy();                     // Cauchy distribution (mean=0, scale=1)
double   randCauchy(double mean, double scale);
#endif
