#ifndef SPIN_CONF_MEAS_C
#define SPIN_CONF_MEAS_C

#include "../include/macro.h"

#include <math.h>

#include "../include/spin_conf.h"

// magnetization per site
double magn(Spin_Conf const *const SC, Geometry const *const geo,
            Params const *const params) {
  long r;
  double tmp1, tmp2, sum1, sum2;

  sum1 = 0.0;
  sum2 = 0.0;
  for (r = 0; r < geo->d_volume; r++) {
    tmp1 = cos(SC->lattice[r] * 2.0 * PI * params->d_instates);
    tmp2 = sin(SC->lattice[r] * 2.0 * PI * params->d_instates);

    sum1 += tmp1;
    sum2 += tmp2;
  }

  sum1 *= (double)geo->d_inv_vol;
  sum2 *= (double)geo->d_inv_vol;

  return sqrt(sum1 * sum1 + sum2 * sum2);
}

// energy per site
double energy(Spin_Conf const *const SC, Geometry const *const geo) {
  long int r, sum;
  int i;

  sum = 0;
  for (r = 0; r < geo->d_volume; r++) {
    for (i = 0; i < DIM; i++) {
      if (SC->lattice[r] == SC->lattice[nnp(geo, r, i)]) {
        sum--;
      }
    }
  }

  return (double)sum * geo->d_inv_vol;
}

void perform_measures(Spin_Conf const *const SC, Geometry const *const geo,
                      Params const *const params, FILE *datafile) {
  double mag, ener;

  mag = magn(SC, geo, params);
  ener = energy(SC, geo);

  fprintf(datafile, "%.12g %.12g", mag, ener);

  fprintf(datafile, "\n");

  fflush(datafile);
}

#endif