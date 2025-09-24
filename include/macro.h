#ifndef MACRO_H
#define MACRO_H

#define DIM 2
#define UPDATER 2

#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define MAX(A, B) (((A) > (B)) ? (A) : (B))

#define MIN_VALUE 1.0e-13

#define INT_ALIGN 16

static const double PI =
    3.141592653589793238462643383279502884197169399375105820974944;
static const double PI2 =
    6.283185307179586476925286766559005768394338798750211641949889;
static const double HALF_PI =
    1.570796326794896619231321691639751442098584699687552910487472;

#define STD_STRING_LENGTH 50 // standarg lenght of unknown strings

// way to print a macro: if
// #define val1 val2
// then QUOTEME(val1) give the string "val2"
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

// to activate posix_memalign in stdlib.h
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define PACKAGE_BUGREPORT "alessio.negro1999@gmail.com"

#endif