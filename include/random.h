#ifndef RANDOM_H
#define RANDOM_H

#include "../include/params.h"

// initialize the random number generator
void myrand_init(Params *params);

// return a random number in [0,1)
double myrand(void);

#endif
