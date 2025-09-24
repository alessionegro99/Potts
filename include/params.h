#ifndef GPARAM_H
#define GPARAM_H

#include "macro.h"

#include <stdio.h>
#include <time.h>

typedef struct Params {
  // lattice dimensions
  int d_insize[DIM];

  // simulation parameters
  double d_beta;

  // simulation details
  long d_sample;
  long d_measevery;
  int d_updater;
  double d_weights[2 * DIM + 1];
  double d_padd;
  double d_acc;

  // initialization & saving
  int d_start;

  // output file names
  char d_data_file[STD_STRING_LENGTH];
  char d_log_file[STD_STRING_LENGTH];

  // random seed
  unsigned long int d_randseed;

} Params;

void remove_white_lines_and_comments(FILE *input);
void readinput(char *in_file, Params *params);

void init_data_file(FILE **dataf, Params const *const params);
void print_parameters_local(Params const *const params, time_t time_start,
                            time_t time_end);

#endif