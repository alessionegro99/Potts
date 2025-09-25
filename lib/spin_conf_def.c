#ifndef SPIN_CONF_DEF_C
#define SPIN_CONF_DEF_C
#include "../include/macro.h"

#include <math.h>
#include <stdlib.h>

#include "../include/params.h"
#include "../include/random.h"
#include "../include/spin_conf.h"

void init_spin_conf(Spin_Conf *SC, Geometry const *const geo, Params *params) {
  long r;
  int err;

  // allocate the local lattice
  err = posix_memalign((void **)&(SC->lattice), (size_t)INT_ALIGN,
                       (size_t)geo->d_volume * sizeof(long));
  if (err != 0) {
    fprintf(stderr, "Problems in allocating the lattice! (%d)\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  // initialize lattice
  if (params->d_start == 0) // ordered start
  {
    for (r = 0; r < (geo->d_volume); r++) {
      SC->lattice[r] = 1;
    }
  }
  if (params->d_start == 1) // random start
  {
    for (r = 0; r < (geo->d_volume); r++) {
      if (myrand() >= 0.5)
        SC->lattice[r] = 1;
      else
        SC->lattice[r] = -1;
    }
  }

  // initializing parameters
  params->d_instates = 1.0 / (double)NSTATES;

  int i;

  // initialize auxilliary vector for acceptance probability
  for (i = 0; i <= 2 * DIM; i++) {
    params->d_aux_prob[i] = exp(-params->d_beta * ((double)i));
  }
}

void free_spin_conf(Spin_Conf *SC) { free(SC->lattice); }
#endif