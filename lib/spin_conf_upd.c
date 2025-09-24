#ifndef SPIN_CONF_UPD_C
#define SPIN_CONF_UPD_C
#include "../include/macro.h"

#include "../include/random.h"
#include "../include/spin_conf.h"

// perform one site update with Metropolis
// return 1 if accepted, 0 otherwise
int Metropolis(Spin_Conf *SC, Geometry const *const geo,
               Params const *const params, long r) {
  int i, acc, s_r, S_r, k;

  S_r = 0;
  for (i = 0; i < DIM; i++) {
    S_r += SC->lattice[nnp(geo, r, i)];
    S_r += SC->lattice[nnm(geo, r, i)];
  }

  s_r = SC->lattice[r];
  k = s_r * S_r;
  if (k <= 0) {
    SC->lattice[r] *= -1;
    acc = +1;
  } else if (myrand() <= params->d_weights[k / 2 + DIM]) {
    SC->lattice[r] *= -1;
    acc = +1;
  } else {
    acc = 0;
  }
  return acc;
}

// perform a complete update with Metropolis
void update_Metropolis(Spin_Conf *SC, Geometry const *const geo,
                       Params *params) {
  long r, raux, acc_rate;

  acc_rate = 0;
  for (r = 0; r < geo->d_volume; r++) {
    raux = (long)((double)geo->d_volume * myrand());
    acc_rate += Metropolis(SC, geo, params, raux);
  }

  params->d_acc = (double)acc_rate * (double)geo->d_inv_vol;
}

// perform one site update with the heatbath algorithm
void heatbath(Spin_Conf *SC, Geometry const *const geo,
              Params const *const params, long r) {
  int i, s_r;

  s_r = 0;

  for (i = 0; i < DIM; i++) {
    s_r += SC->lattice[nnp(geo, r, i)];
    s_r += SC->lattice[nnm(geo, r, i)];
  }

  if (myrand() < params->d_weights[s_r / 2 + DIM]) {
    SC->lattice[r] = +1;
  } else {
    SC->lattice[r] = -1;
  }
}

// perform a complete update with heatbath
void update_heatbath(Spin_Conf *SC, Geometry const *const geo,  Params const *const params) {
  long r, raux;

  for (r = 0; r < geo->d_volume; r++) {
    raux = (long)((double)geo->d_volume * myrand());
    heatbath(SC, geo, params, raux);
  }
}

#endif