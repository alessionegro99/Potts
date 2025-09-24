#ifndef GEOMETRY_C
#define GEOMETRY_C

#include "../include/macro.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/geometry.h"

long (*cart_to_si)(int const *const cartcoord, Geometry const *const geo) =
    &cart_to_lexeo; // cartesian coordinates -> single index
void (*si_to_cart)(int *cartcoord, long si, Geometry const *const geo) =
    &lexeo_to_cart; // single index -> cartesian coordinates
long (*lex_to_si)(long lex, Geometry const *const geo) =
    &lex_to_lexeo; // lexicographic -> single index
long (*si_to_lex)(long si, Geometry const *const geo) =
    &lexeo_to_lex; // lexicographic -> single index

void init_geometry(Geometry *geo, int insize[DIM]) {
  int i, value, valuep, valuem, err;
  long r, rm, rp;
  int cartcoord[DIM];

  for (i = 0; i < DIM; i++) {
    geo->d_size[i] = insize[i];
  }

  // derived constants
  geo->d_volume = 1;
  for (i = 0; i < DIM; i++) {
    (geo->d_volume) *= (geo->d_size[i]);
  }

  geo->d_inv_vol = 1.0 / ((double)geo->d_volume);

  // allocate memory
  err = posix_memalign((void **)&(geo->d_nnp), (size_t)INT_ALIGN,
                       (size_t)geo->d_volume * sizeof(long *));
  if (err != 0) {
    fprintf(stderr, "Problems in allocating geometry! (%d)\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  err = posix_memalign((void **)&(geo->d_nnm), (size_t)INT_ALIGN,
                       (size_t)geo->d_volume * sizeof(long *));
  if (err != 0) {
    fprintf(stderr, "Problems in allocating geometry! (%d)\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  for (r = 0; r < (geo->d_volume); r++) {
    err = posix_memalign((void **)&(geo->d_nnp[r]), (size_t)INT_ALIGN,
                         (size_t)DIM * sizeof(long));
    if (err != 0) {
      fprintf(stderr, "Problems in allocating geometry! (%d)\n", __LINE__);
      exit(EXIT_FAILURE);
    }
    err = posix_memalign((void **)&(geo->d_nnm[r]), (size_t)INT_ALIGN,
                         (size_t)DIM * sizeof(long));
    if (err != 0) {
      fprintf(stderr, "Problems in allocating geometry! (%d)\n", __LINE__);
      exit(EXIT_FAILURE);
    }
  }

  // initialize
  for (r = 0; r < geo->d_volume; r++) {
    si_to_cart(cartcoord, r, geo);

    for (i = 0; i < DIM; i++) {
      value = cartcoord[i];

      valuep = value + 1;
      // PBC+
      if (valuep >= geo->d_size[i]) {
        valuep -= geo->d_size[i];
      }
      cartcoord[i] = valuep;
      rp = cart_to_si(cartcoord, geo);
      geo->d_nnp[r][i] = rp;

      valuem = value - 1;
      // PBC -
      if (valuem < 0) {
        valuem += geo->d_size[i];
      }
      cartcoord[i] = valuem;
      rm = cart_to_si(cartcoord, geo);
      geo->d_nnm[r][i] = rm;

      cartcoord[i] = value;
    }
  }

#ifdef DEBUG
  test_geometry(geo);
#endif
}

// free memory
void free_geometry(Geometry *geo) {
  long r;

  for (r = 0; r < geo->d_volume; r++) {
    free(geo->d_nnp[r]);
    free(geo->d_nnm[r]);
  }
  free(geo->d_nnp);
  free(geo->d_nnm);
}

void test_geometry(Geometry const *const geo) {
  long si, ris_test, si_bis;
  int dir, cart[DIM];

  // test of lex_to_cart <-> cart_to_lex
  for (si = 0; si < geo->d_volume; si++) {
    lex_to_cart(cart, si, geo);
    ris_test = cart_to_lex(cart, geo);

    if (si != ris_test) {
      fprintf(stderr, "Problems while testing geometry! (%d)\n", __LINE__);

      exit(EXIT_FAILURE);
    }
  }

  // test of lexeo_to_cart <-> cart_to_lexeo
  for (si = 0; si < geo->d_volume; si++) {
    lexeo_to_cart(cart, si, geo);
    ris_test = cart_to_lexeo(cart, geo);

    if (si != ris_test) {
      fprintf(stderr, "Problems while testing geometry! (%d)\n", __LINE__);
      exit(EXIT_FAILURE);
    }
  }

  // test of nnp <-> nnm
  for (si = 0; si < geo->d_volume; si++) {
    for (dir = 0; dir < DIM; dir++) {
      si_bis = nnp(geo, si, dir);
      ris_test = nnm(geo, si_bis, dir);

      if (si != ris_test) {
        fprintf(stderr, "Problems while testing geometry! (%d)\n", __LINE__);
        exit(EXIT_FAILURE);
      }
    }
  }
}

//------------ these are not to be used outside geometry.c ----------------

// cartesian coordinates -> lexicographic index
long cart_to_lex(int const *const cartcoord, Geometry const *const geo) {
  int i;
  long ris, aux;

  ris = 0;
  aux = 1;
  for (i = 0; i < DIM; i++) {
    ris += cartcoord[i] * aux;
    aux *= geo->d_size[i];
  }

  // ris = cartcoord[0]
  //      +cartcoord[1]*size[0]
  //      +cartcoord[2]*size[0]*size[1]
  //      +...
  //      +cartcoord[DIM-1]*size[0]*size[1]*...*size[DIM-2]

  return ris;
}

// lexicographic index -> cartesian coordinates
void lex_to_cart(int *cartcoord, long lex, Geometry const *const geo) {
  int i;
  long aux[DIM];

  aux[0] = 1;
  for (i = 1; i < DIM; i++) {
    aux[i] = aux[i - 1] * geo->d_size[i - 1];
  }
  // aux[0]=1
  // aux[1]=size[0]
  // aux[2]=size[0]*size[1]
  // ...
  // aux[DIM-1]=size[0]*size[1]*...*size[DIM-2]

  for (i = DIM - 1; i >= 0; i--) {
    cartcoord[i] = (int)(lex / aux[i]);
    lex -= aux[i] * cartcoord[i];
  }
}

// cartesian coordinates -> lexicographic eo index
long cart_to_lexeo(int const *const cartcoord, Geometry const *const geo) {
  long lex;
  int i, eo;

  lex = cart_to_lex(cartcoord, geo);

  eo = 0;
  for (i = 0; i < DIM; i++) {
    eo += cartcoord[i];
  }

  if (eo % 2 == 0) {
    return lex / 2;
  } else {
    return (lex + geo->d_volume) / 2;
  }
  // even sites are written first
}

// lexicographic eo index -> cartesian coordinates
void lexeo_to_cart(int *cartcoord, long lexeo, Geometry const *const geo) {
  long lex;
  int i, eo;

  if (geo->d_volume % 2 == 0) {
    if (lexeo < geo->d_volume / 2) {
      lex = 2 * lexeo;
    } else {
      lex = 2 * (lexeo - geo->d_volume / 2);
    }
    lex_to_cart(cartcoord, lex, geo);

    eo = 0;
    for (i = 0; i < DIM; i++) {
      eo += cartcoord[i];
    }
    eo = eo % 2;

    // this is to take care of the case d_volume is even but not
    // all the lattice extents are even
    if ((eo == 0 && lexeo >= geo->d_volume / 2) ||
        (eo == 1 && lexeo < geo->d_volume / 2)) {
      lex += 1;
      lex_to_cart(cartcoord, lex, geo);
    }
  } else {
    if (lexeo <= geo->d_volume / 2) {
      lex = 2 * lexeo;
    } else {
      lex = 2 * (lexeo - geo->d_volume / 2) - 1;
    }
    lex_to_cart(cartcoord, lex, geo);
  }
}

//  lexicographic index -> lexicographic eo index
long lex_to_lexeo(long lex, Geometry const *const geo) {
  int cartcoord[DIM];

  lex_to_cart(cartcoord, lex, geo);

  return cart_to_lexeo(cartcoord, geo);
}

//  lexicographic eo index -> lexicographic index
long lexeo_to_lex(long lexeo, Geometry const *const geo) {
  int cartcoord[DIM];

  lexeo_to_cart(cartcoord, lexeo, geo);

  return cart_to_lex(cartcoord, geo);
}
#endif