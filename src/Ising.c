#ifndef ISING_C
#define ISING_C

#include "macro.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/params.h"
#include "../include/random.h"
#include "../include/spin_conf.h"

void real_main(char *in_file) {
  Spin_Conf SC;
  Geometry geo;
  Params params;

  int count;
  FILE *datafile;
  time_t time1, time2;

  // read input file
  readinput(in_file, &params);

  // initialize random generator
  myrand_init(&params);

  // open data_file
  init_data_file(&datafile, &params);

  // initialize geometry
  init_geometry(&geo, params.d_insize);

  // initialize spin configuration
  init_spin_conf(&SC, &geo, &params);

  long *cluster;
  int err = posix_memalign((void **)&cluster, INT_ALIGN,
                           (size_t)geo.d_volume * sizeof(double long));

  if (err != 0) {
    fprintf(stderr, "Problems in allocating a vector (%s, %d)\n", __FILE__,
            __LINE__);
    exit(EXIT_FAILURE);
  }

  // montecarlo start
  time(&time1);
  for (count = 1; count < params.d_sample + 1; count++) {
    // update
    if (params.d_updater == 0) {
      update_Metropolis(&SC, &geo, &params);
    } else if (params.d_updater == 1) {
      update_heatbath(&SC, &geo, &params);
    } else if (params.d_updater == 2) {
      update_single_cluster(&SC, &geo, &params, cluster);
    }

    if (count % params.d_measevery == 0) {
      // measure
      perform_measures(&SC, &geo, &params, datafile);
    }
  }
  time(&time2);
  // montecarlo end

  // close data file
  fclose(datafile);

  // print simulation details
  print_parameters_local(&params, time1, time2);

  // free spin configuration
  free_spin_conf(&SC);

  // free gauge configuration
  free_geometry(&geo);

  // free cluster vector
  free(cluster);
}

void print_template_input(void) {
  FILE *fp;

  fp = fopen("template_input.in", "w");

  if (fp == NULL) {
    fprintf(stderr, "Error in opening the file template_input.in (%s, %d)\n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  } else {
    fprintf(fp, "size 16 16\n");
    fprintf(fp, "\n");
    fprintf(fp, "beta 0.42\n");
    fprintf(fp, "\n");
    fprintf(fp, "sample    10\n");
    fprintf(fp, "measevery 5\n");
    fprintf(fp, "updater 2 # 0=Metropolis 1=heat-bath 2=single cluster\n");
    fprintf(fp, "\n");
    fprintf(fp, "start                   0  # 0=ordered  1=random\n");
    fprintf(fp, "\n");
    fprintf(fp, "#output files\n");
    fprintf(fp, "data_file  dati.dat\n");
    fprintf(fp, "log_file   log.dat\n");
    fprintf(fp, "\n");
    fprintf(fp, "randseed 0    #(0=time)\n");
    fclose(fp);
  }
}

int main(int argc, char **argv) {
  char in_file[50];

  if (argc != 2) {
    printf("Alessio Negro %s\n", PACKAGE_BUGREPORT);
    printf("Usage: %s input_file\n\n", argv[0]);

    printf("Compilation details:\n");
    printf("\tINT_ALIGN: %s\n", QUOTEME(INT_ALIGN));

    print_template_input();

    return EXIT_SUCCESS;
  } else {
    if (strlen(argv[1]) >= STD_STRING_LENGTH) {
      fprintf(
          stderr,
          "File name too long. Increse STD_STRING_LENGTH in include/macro.h\n");
    } else {
      strcpy(in_file, argv[1]);
    }
  }

  real_main(in_file);

  return EXIT_SUCCESS;
}

#endif