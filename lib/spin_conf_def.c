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

  int i;

  // initializing weights
  if (params->d_updater == 0) {
    for (i = 0; i <= 2 * DIM; i++) {
      params->d_weights[i] =
          exp(-2.0 * params->d_beta * ((double)(2 * (i - DIM))));
    }
  } else if (params->d_updater == 1) {
    for (i = 0; i <= 2 * DIM; i++) {
      params->d_weights[i] =
          1.0 / (exp(-2.0 * params->d_beta * (double)(2 * (i - DIM))) + 1.0);
    }
  } else if (params->d_updater == 2) {
    params->d_padd = 1 - exp(-2 * params->d_beta);
  }
}

void free_spin_conf(Spin_Conf *SC) { free(SC->lattice); }
#endif