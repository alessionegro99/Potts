#ifndef PARAM_C
#define PARAM_C

#include "../include/macro.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/params.h"

// remove from input file white/empty lines and comments
// comments start with the character #

void remove_white_lines_and_comments(FILE *input) {
  int temp_i;

  temp_i = getc(input);
  if (temp_i == '\n' || temp_i == ' ' ||
      temp_i == '\043') // scan for white lines and comments
  {
    ungetc(temp_i, input);

    temp_i = getc(input);
    if (temp_i == '\n' || temp_i == ' ') // white line
    {
      do {
        temp_i = getc(input);
      } while (temp_i == '\n' || temp_i == ' ');
    }
    ungetc(temp_i, input);

    temp_i = getc(input);
    if (temp_i == '\043') // comment, \043 = ascii oct for #
    {
      do {
        temp_i = getc(input);
      } while (temp_i != '\n');
    } else {
      ungetc(temp_i, input);
    }

    remove_white_lines_and_comments(input);
  } else {
    ungetc(temp_i, input);
  }
}

void readinput(char *in_file, Params *params) {
  FILE *input;
  char str[STD_STRING_LENGTH], temp_str[STD_STRING_LENGTH];
  double temp_d;
  int temp_i, i;
  int err, end = 1;
  unsigned int temp_ui;

  input = fopen(in_file, "r"); // open the input file
  if (input == NULL) {
    fprintf(stderr, "Error in opening the file %s (%s, %d)\n", in_file,
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  } else {
    while (end == 1) // slide the file
    {
      remove_white_lines_and_comments(input);

      err = fscanf(input, "%s", str);
      if (err != 1) {
        fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                __FILE__, __LINE__);
        printf("err=%d\n", err);
        exit(EXIT_FAILURE);
      }

      if (strncmp(str, "size", 4) == 0) {
        for (i = 0; i < DIM; i++) {
          err = fscanf(input, "%d", &temp_i);
          if (err != 1) {
            fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
          }
          params->d_insize[i] = temp_i;
        }
      }

      // simulation paramseters
      else if (strncmp(str, "beta", 4) == 0) {
        err = fscanf(input, "%lf", &temp_d);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_beta = temp_d;
      }

      // simulation details
      else if (strncmp(str, "sample", 6) == 0) {
        err = fscanf(input, "%d", &temp_i);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_sample = temp_i;
      } else if (strncmp(str, "measevery", 9) == 0) {
        err = fscanf(input, "%d", &temp_i);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_measevery = temp_i;
      } else if (strncmp(str, "updater", 7) == 0) {
        err = fscanf(input, "%d", &temp_i);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_updater = temp_i;
      }

      // initialization & saving
      else if (strncmp(str, "start", 5) == 0) {
        err = fscanf(input, "%d", &temp_i);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_start = temp_i;
      }

      // output file names
      else if (strncmp(str, "data_file", 9) == 0) {
        err = fscanf(input, "%s", temp_str);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        strcpy(params->d_data_file, temp_str);
      } else if (strncmp(str, "log_file", 8) == 0) {
        err = fscanf(input, "%s", temp_str);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        strcpy(params->d_log_file, temp_str);
      }

      // random seed
      else if (strncmp(str, "randseed", 8) == 0) {
        err = fscanf(input, "%u", &temp_ui);
        if (err != 1) {
          fprintf(stderr, "Error in reading the file %s (%s, %d)\n", in_file,
                  __FILE__, __LINE__);
          exit(EXIT_FAILURE);
        }
        params->d_randseed = temp_ui;
      }

      else {
        fprintf(stderr,
                "Error: unrecognized option %s in the file %s (%s, %d)\n", str,
                in_file, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
      }

      remove_white_lines_and_comments(input);

      // check if the read line is the last one
      temp_i = getc(input);
      if (temp_i == EOF) {
        end = 0;
      } else {
        ungetc(temp_i, input);
      }
    }

    fclose(input);

    err = 0;
    if (params->d_updater != 0 && params->d_updater != 1 &&
        params->d_updater != 2) {
      err = 1;
    }
    if (err == 1) {
      fprintf(stderr,
              "Error: 0 for Metropolis, 1 for heat-bath, 2 for single cluster "
              "(%s, %d)\n",
              __FILE__, __LINE__);
    }
  }
}

// initialize data file
void init_data_file(FILE **dataf, Params const *const params) {
  int i;
  *dataf = fopen(params->d_data_file, "w");
  fprintf(*dataf, "%d ", DIM);
  for (i = 0; i < DIM; i++) {
    fprintf(*dataf, "%d ", params->d_insize[i]);
  }
  fprintf(*dataf, "\n");

  fflush(*dataf);
}

// print simulation parameters
void print_parameters_local(Params const *const params, time_t time_start,
                            time_t time_end) {
  FILE *fp;
  int i;
  double diff_sec;

  fp = fopen(params->d_log_file, "w");
  fprintf(fp, "+------------------------------+\n");
  fprintf(fp, "| Simulation details for Ising |\n");
  fprintf(fp, "+------------------------------+\n\n");

  fprintf(fp, "space dimensionality: %d\n\n", DIM);

  fprintf(fp, "lattice: %d", params->d_insize[0]);
  for (i = 1; i < DIM; i++) {
    fprintf(fp, "x%d", params->d_insize[i]);
  }
  fprintf(fp, "\n\n");

  fprintf(fp, "beta: %.10lf\n", params->d_beta);
  fprintf(fp, "\n");

  fprintf(fp, "sample:    %ld\n", params->d_sample);
  fprintf(fp, "measevery: %ld\n", params->d_measevery);
  fprintf(fp, "\n");

  fprintf(fp, "start: %d\n", params->d_start);
  fprintf(fp, "updater: %d\n", params->d_updater);
  fprintf(fp, "\n");

  fprintf(fp, "randseed: %lu\n", params->d_randseed);
  fprintf(fp, "\n");

  diff_sec = difftime(time_end, time_start);
  fprintf(fp, "Simulation time: %.3lf seconds\n", diff_sec);
  fprintf(fp, "\n");

  fclose(fp);
}

#endif