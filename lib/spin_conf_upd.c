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

// build a cluster of neighbouring spins with the same orientation
void single_cluster(Spin_Conf *SC, Geometry const *const geo,
                    Params const *const params, long *cluster, long r) {
  long x;
  int n_old, n_new, l_c, s_x, s_r;

  s_r = SC->lattice[r];
  cluster[0] = r;
  n_old = 0;
  n_new = 1;
  l_c = 1;

  while (n_new > n_old) {
    int p;

    for (p = n_old; p < n_new; p++) {
      int i;

      // sweeping all nearaest neighbors of cluster[p]
      for (i = 0; i < DIM; i++) {
        int j, flag;

        flag = 0;

        // nearest neighbor + dir of cluster[p]
        x = nnp(geo, cluster[p], i);

        // check if x is already in the cluster
        for (j = 0; j < l_c; j++) {
          if (cluster[j] == x) {
            flag = 1;
            break;
          }
        }

        // add x if it is not in the cluster
        if (flag == 0) {
          s_x = SC->lattice[x];
          if (s_x == s_r) {
            if (myrand() < params->d_padd) {
              cluster[l_c] = x;
              l_c++;
            }
          }
        }

        // nearest neighbor - dir
        flag = 0;

        x = nnm(geo, cluster[p], i);

        for (j = 0; j < l_c; j++) {
          if (cluster[j] == x) {
            flag = 1;
            break;
          }
        }
        if (flag == 0) {
          s_x = SC->lattice[x];
          if (s_x == s_r) {
            if (myrand() < params->d_padd) {
              cluster[l_c] = x;
              l_c++;
            }
          }
        }
      }
    }
    n_old = n_new;
    n_new = l_c;
  }

  int i;
  for (i = 0; i < l_c; i++) {
    x = cluster[i];
    SC->lattice[x] *= -1;
  }
}

// performs update with single cluster
void update_single_cluster(Spin_Conf *SC, Geometry const *const geo,
                           Params const *const params, long *cluster) {
  long r;

  r = (long)((double)geo->d_volume * myrand());

  single_cluster(SC, geo, params, cluster, r);
}

#endif