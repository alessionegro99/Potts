#ifndef SPIN_CONF_MEAS_C
#define SPIN_CONF_MEAS_C

#include "../include/macro.h"

#include "../include/spin_conf.h"

// magnetization per site
double magn(Spin_Conf const *const SC, Geometry const *const geo) {
  int r, sum;

  sum = 0;
  for (r = 0; r < geo->d_volume; r++) {
    sum += SC->lattice[r];
  }

  return (double)sum * geo->d_inv_vol;
}

// energy per site
double energy(Spin_Conf const *const SC, Geometry const *const geo) {
  long int r, sum;
  int i;

  sum = 0;
  for (r = 0; r < geo->d_volume; r++) {
    for (i = 0; i < DIM; i++) {
      sum += -SC->lattice[r] * SC->lattice[nnp(geo, r, i)];
    }
  }

  return (double)sum * geo->d_inv_vol;
}

void perform_measures(Spin_Conf const *const SC, Geometry const *const geo,
                      Params const *const params, FILE *datafile) {
  double mag, ener;

  mag = magn(SC, geo);
  ener = energy(SC, geo);

  fprintf(datafile, "%.12g %.12g", mag, ener);

  if (params->d_updater == 0) {
    fprintf(datafile, " %.12g", params->d_acc);
  }
  fprintf(datafile, "\n");

  fflush(datafile);
}

#endif