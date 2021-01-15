#ifndef PAIRING_HEADER
#define PAIRING_HEADER 1
//#define PBC_OR_CIFER 0 /* 1=CIFER 0=PBC */
#ifndef PBC_OR_CIFER
#define PBC_OR_CIFER 1
#endif
#if PBC_OR_CIFER == 0
extern const char *Param;
#include <pbc.h>
#include <pbc_test.h>
#else
extern const char *Param;
#include <stdlib.h>
#include <string.h>
#include <amcl/big_256_56.h>
#include <amcl/pair_BN254.h>
#include <amcl/pbc_support.h>
#include "cifer/internal/common.h"
#include "cifer/internal/hash.h"
#include "cifer/internal/big.h"
#include "cifer/sample/uniform.h"


typedef struct
{
  ECP_BN254 g1;
  ECP2_BN254 g2;
  FP12_BN254 gT;
  BIG_256_56 x;
  mpz_t z;
  mpz_t p;
  int type;
} _element_t;

typedef _element_t element_t[1];
typedef struct
{
  int p;
} pairing_t;

typedef element_t pairing_pp_t;


void element_init_G1 (element_t g, pairing_t p);
void element_init_G2 (element_t g, pairing_t p);
void element_init_GT (element_t g, pairing_t p);
void element_init_Zr (element_t g, pairing_t p);
void element_random (element_t e);
void element_pow_zn (element_t res, element_t g, element_t y);
void element_set1 (element_t e);
void element_set (element_t e, element_t a);
void element_add (element_t c, element_t a, element_t b);
void element_pow_zn (element_t c, element_t a, element_t b);
void element_div (element_t c, element_t a, element_t b);
int element_cmp (element_t a, element_t b);
void element_set0 (element_t a);
void element_set_mpz (element_t a, mpz_t b);
void pairing_pp_init (pairing_pp_t pp, element_t x, pairing_t p);
void pairing_pp_clear (pairing_pp_t pp);
void element_pairing (element_t c, element_t a, element_t b);
void pairing_pp_apply (element_t c, element_t b, pairing_pp_t pp);
int element_is1 (element_t a);
void element_from_hash (element_t hash, char *str, unsigned int length);
int element_snprintf (char *str, unsigned int length, char *format,
		      element_t g, element_t h, element_t u, element_t v);
int element_snprintf_publickey (char *str, int i, element_t g);
void element_dlog_brute_force (element_t x, element_t g, element_t h);
void element_to_mpz (mpz_t z, element_t e);
void element_mul (element_t c, element_t a, element_t b);
void element_sub (element_t c, element_t a, element_t b);
void pairing_init_set_str (pairing_t p, const char *param);
int element_cmp_ui (element_t a, unsigned long int b);
int element_cmp_si (element_t a, signed long int b);
#endif
#endif
