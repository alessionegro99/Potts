#ifndef SPIN_CONF_H
#define SPIN_CONF_H

#include "macro.h"

#include <stdio.h>

#include "geometry.h"
#include "params.h"

typedef struct Spin_Conf {
  int *lattice;
} Spin_Conf;

// in spin_conf_def.c
void init_spin_conf(Spin_Conf *SC, Geometry const *const geo, Params *params);
void free_spin_conf(Spin_Conf *SC);

// in spin_conf_meas.c
double magn(Spin_Conf const *const SC, Geometry const *const geo);
double energy(Spin_Conf const *const SC, Geometry const *const geo);
void perform_measures(Spin_Conf const *const SC, Geometry const *const geo,
                      Params const *const params, FILE *datafile);

// in spin_conf_upd.c
int Metropolis(Spin_Conf *SC, Geometry const *const geo,
               Params const *const params, long r);
void update_Metropolis(Spin_Conf *SC, Geometry const *const geo,
                       Params *params);
void heatbath(Spin_Conf *SC, Geometry const *const geo,
              Params const *const params, long r);
void update_heatbath(Spin_Conf *SC, Geometry const *const geo,
                     Params const *const params);
void single_cluster(Spin_Conf *SC, Geometry const *const geo,
                    Params const *const params, long *cluster, long r);
void update_single_cluster(Spin_Conf *SC, Geometry const *const geo,
                           Params const *const params, long *cluster);

#endif
