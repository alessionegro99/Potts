#ifndef SPIN_CONF_UPD_C
#define SPIN_CONF_UPD_C
#include "../include/macro.h"

#include "../include/random.h"
#include "../include/spin_conf.h"

// perform one site update with the heatbath algorithm
void heatbath(Spin_Conf *SC, Geometry const *const geo,
              Params const *const params, long r) {

  int i, nn[NSTATES];
  double tmp, prob[NSTATES];

  // nn[i] = numbers of sites with state "i"
  // the energy will be -nn[lattice[r]]
  for (i = 0; i < NSTATES; i++) {
    nn[i] = 0;
  }
  for (i = 0; i < DIM; i++) {
    nn[SC->lattice[nnp(geo, r, i)]] += 1;
    nn[SC->lattice[nnm(geo, r, i)]] += 1;
  }

  tmp = 0.0;
  for (i = 0; i < NSTATES; i++) {
    tmp += 1.0 / params->d_aux_prob[nn[i]];
    prob[i] = tmp;
  }
  for (i = 0; i < NSTATES; i++) {
    prob[i] /= tmp;
  }

  tmp = myrand();
  for (i = 0; i < NSTATES; i++) {
    if (tmp < prob[i]) {
      SC->lattice[r] = i;
      i = NSTATES;
    }
  }
}

// perform a complete update with heatbath
void update_heatbath(Spin_Conf *SC, Geometry const *const geo,
                     Params const *const params) {
  long r, raux;

  for (r = 0; r < geo->d_volume; r++) {
    raux = (long)((double)geo->d_volume * myrand());
    heatbath(SC, geo, params, raux);
  }
}

#endif