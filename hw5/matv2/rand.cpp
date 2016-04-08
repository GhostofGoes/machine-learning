#include "rand.h"

///////////////////////////////////////////////////
//
// Simple 64 bit random number generators based on
// a RNG called flea by Bob Jenkins.   Thanks, Bob!
//
// http://burtleburtle.net/bob/rand/talksmall.html
//

// these constants are supplied to make the RNG return numbers
// on the open interval [0, 1)
#define PIDIV2	    1.570796326794896619231321691639751442099L
#define PI  	    3.141592653589793238462643383279502884197L

static double unitizer64 = 1.0/18446744073709551616.0;
static double unitizer64_2 = 2.0/18446744073709551616.0;
static double unitizer64_pi = PI/18446744073709551616.0;

///////////////////////////////////////////////////
//
// basic fast 64 bit random number generation
//
static unsigned long long int V, W, X, Y, Z;

// initialize the random number generator using a pair of seeds
void initRand(unsigned long long int a, unsigned long long int b)
{
    V = 1415926535897932ULL;
    W = 3846264338327950ULL;
    X = 2884197169399375ULL;
    Y = 105820974944592ULL;
    Z = 3078164062862089ULL;
    Z += a;
    X += b;
    for (unsigned long long int i=0; i<((a+b) % 1000)+1000; i++) randULL();
}


// initialize the random number generator using process id and time
void initRand()
{
    initRand((unsigned long long int)getpid()*1405321245300013ULL,
               (unsigned long long int)(time(NULL)<<1)|0x1ULL);
}


// return a 64 bit unsigned uniformly distributed random number
inline unsigned long long int randULL()
{
    V = W;
    W = X;
    X = ((Y<<41) + (Y>>23)) + Z;
    Y = Z ^ W;
    Z = V + X;

    return Y;
}

// return a uniformly distributed random number between 0 and 1
// NOTE: this can be relatively slow since the multiply is a 64 bit
// multiply.
double randUnit()
{
    return randULL()*unitizer64;
}



// return a uniformly distributed random number between -1 and 1
// NOTE: this can be relatively slow since the multiply is a 64 bit
// multiply.
double randPMUnit()
{
    return randULL()*unitizer64_2 - 1.0;
}


// return a uniformly distributed random number between 0 and m-1
int randMod(int m) {
    return randULL()%m;
}


// return two uniformly distributed random numbers in the rand 0 to m-1
// where they are not equal.
void randMod2(int m, int &a, int &b) 
{
    a = randMod(m);
    b = a + randMod(m-1) + 1;
    if (b>=m) b-=m;
}


// return a uniformly distributed random number whose bits lie in the 
//   masked bits.  If the mask is a right justified set of ones this
//   is far faster to generate mod of 2^k than using mod function.
int randMask(unsigned long long int mask) {
    return randULL()&mask;
}


// return true with a probability of prob
bool choose(double prob)
{
    return randUnit()<prob;
}


// returns zero or nonzero 50% of the time
unsigned long long randCoinToss()
{
    return (randULL()&0x40ULL);
}


// return true with a probability of eigth/8
// can be faster than using choose with a double probability
bool choose8(int eigth)
{
    return (randULL()&0x7ULL)<(unsigned long long int)eigth;
}

// for use in returning true with a probability of prob/(2^k)
// Mask must select the lower k bits
bool chooseMask(unsigned long long int mask, int prob)
{
    return (randULL()&mask)<(unsigned long long int)prob;
}


// Random number generator with normal (Gaussian) distribution
// from p 117 of Knuth vol 2 2nd ed.
// Note: this is slow.   Should use the Ziggurat method some day.
static bool gotSpare64=false;
static double spare64;
double randNorm(double stddev)
{
    double u, v, s;

    if (gotSpare64) {
        gotSpare64=false;
        return spare64;
    }
    else {
        do {
            u = 2*randUnit() - 1;
            v = 2*randUnit() - 1;
            s = u*u + v*v;
        } while (s>=1.0 || s==0.0);
    }

    s = sqrt(-2*log(s)/s)*stddev;
    spare64 = v*s;
    gotSpare64 = true;

    return u*s;
}



// Random number generators with a Cauchy distribution
// based on the inversion method using CDF F(x) = .5 + atan(x)/pi
// which yeilds  tan(pi F(x) - .5) as a the tranformation.
// To incorporate a mean and scale: scale*randCauchy()+mean
// 


double randCauchy()
{
    unsigned long long int r;

    do r=randULL(); while (r==0);

    return tan(unitizer64_pi*r - PIDIV2);
}


double randCauchy(double mean, double scale)
{
    unsigned long long int r;

    do r=randULL(); while (r==0);

    return scale*tan(unitizer64_pi*r - PIDIV2) + mean;
}





/*

#include <stdio.h>
#include <stdlib.h>

// Return a uniformly distributed random number between 0 and m-1.
// Suitable for 0 < m < 2^16.
// Uses rejection sampling to avoid the computation of a mod
// Requires a mask be set that of the form (1<<k)-1 that most
// closely contains the value m.   e.g.  randMod(63ULL, m);
// for 32<m<=64.
//
// IMPORTANT: if you are taking something mod m then the mask must be
// m-1 or contain m-1!  For example: if m=4 then mask=3 which is the
// smallest right justified all ones mask containing m-1.  If m=6 then
// mask=7.
int randMod(unsigned long long int mask, int m) {
    int ans;
    do {
        V = W;
        W = X;
        X = ((Y<<41) + (Y>>23)) + Z;
        Y = Z ^ W;
        Z = V + X;

        ans = Y&mask;
        if (ans<m) return ans;

        ans = (Y>>16)&mask;
        if (ans<m) return ans;

        ans = (Y>>32)&mask;
        if (ans<m) return ans;

        ans = (Y>>48)&mask;
    } while (ans>=m);

    return ans;
}


static unsigned long long int smallestMask(unsigned long long int x) {
    x |= x>>1;
    x |= x>>2;
    x |= x>>4;
    x |= x>>8;
    x |= x>>16;
    x |= x>>32;

    return x;
}


int main(int argc, char *argv[]) {
    initRand();
    int m;
    int z;
    unsigned long long int mask;
    z = 0;
    m = atoi(argv[1]);
    mask = smallestMask(m-1);
    for (int i=0; i<100000000; i++) {
//        printf("%d\n", randMod(63ULL, 17));
        z+=randMod(mask, m);
//       z+=randMod(m);
    }
    return z;
}
*/
