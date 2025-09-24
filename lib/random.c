#ifndef RANDOM_C
#define RANDOM_C

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#include "../include/pcg32min.h"
#include "../include/random.h"

// initialization
void myrand_init(Params *params) {
  unsigned long int seed = params->d_randseed;
  if (seed == 0) {
    seed = ((unsigned int)time(NULL) + 10) % UINT_MAX;
    if (seed == 0) {
      seed = 1;
    }
  }
  pcg32_srandom_r(&pcg32_random_state, (uint64_t)seed, (uint64_t)seed + 123);
  params->d_randseed = seed;
}

// number in [0,1)
double myrand(void) {
  return (double)pcg32_random_r(&pcg32_random_state) / (pow(2.0, 32.0));
}
#endif