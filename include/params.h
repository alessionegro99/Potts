#ifndef GPARAM_H
#define GPARAM_H

#include "macro.h"

#include <stdio.h>
#include <time.h>

typedef struct Params {
  // lattice dimensions
  int d_insize[DIM];
  double d_instates;

  // simulation parameters
  double d_beta;

  // simulation details
  long d_sample;
  long d_measevery;
  double d_aux_prob[2 * DIM + 1];

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