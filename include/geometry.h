#ifndef GEOMETRY_H
#define GROMETRY_H

#include "macro.h"

typedef struct Geometry {
  int d_size[DIM];

  long d_volume;
  double d_inv_vol;

  long **d_nnp;
  long **d_nnm;
} Geometry;

// functions to be used when switching between different indices
extern long (*cart_to_si)(
    int const *const cartcoord,
    Geometry const *const geo); // cartesian coordinates -> single index
extern void (*si_to_cart)(
    int *cartcoord, long si,
    Geometry const *const geo); // single index -> cartesian coordinates
extern long (*lex_to_si)(
    long lex, Geometry const *const geo); // lexicographic -> single index
extern long (*si_to_lex)(
    long si, Geometry const *const geo); // lexicographic -> single index

// general functions
void init_geometry(Geometry *geo, int insize[DIM]);
void free_geometry(Geometry *geo);

// next neighbour in + direction
inline long nnp(Geometry const *const geo, long r, int i) {
  return geo->d_nnp[r][i];
}

// next neighbour in - direction
inline long nnm(Geometry const *const geo, long r, int i) {
  return geo->d_nnm[r][i];
}

// for debug
void test_geometry(Geometry const *const geo);

//------------ these are not to be used outside geometry.c ----------------

long cart_to_lex(
    int const *const cartcoord,
    Geometry const *const geo); // cartesian coordinates -> lexicographic index
void lex_to_cart(
    int *cartcoord, long lex,
    Geometry const *const geo); // lexicographic index -> cartesian coordinates

long cart_to_lexeo(int const *const cartcoord,
                   Geometry const *const
                       geo); // cartesian coordinates -> lexicographic eo index
void lexeo_to_cart(int *cartcoord, long lexeo,
                   Geometry const *const
                       geo); // lexicographic eo index -> cartesian coordinates

long lex_to_lexeo(long lex,
                  Geometry const *const
                      geo); //  lexicographic index -> lexicographic eo index
long lexeo_to_lex(long lexeo,
                  Geometry const *const
                      geo); //  lexicographic eo index -> lexicographic index

#endif
