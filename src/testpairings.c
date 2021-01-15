#include <stdio.h>
#include <string.h>
#include "pairings.h"

int
main (void)
{
  element_t a, b, c, x;
  element_t a2, b2, c2, d2, e2, f2, g2, one, a3, b3, c3, d3;
  element_t A2, B2, C2, D2, E2, F2, G2, B;
  element_t gT, gT2, gT3, GT3, GT4;
  pairing_t p;
  mpz_t D;
  mpz_init (D);
  pairing_init_set_str (p, Param);
  element_init_G1 (a, p);
  element_random (a);
  element_init_G1 (c, p);
  element_init_Zr (B, p);
  element_set0 (B);
  element_pow_zn (c, a, B);
  printf ("%d\n", element_is1 (c));	// should return 1
  element_init_G1 (a2, p);
  element_init_G1 (b, p);
  element_init_G1 (b2, p);
  element_init_G1 (c2, p);
  element_init_G1 (d2, p);
  element_init_G1 (e2, p);
  element_init_G1 (f2, p);
  element_init_G1 (g2, p);
  element_set (a2, a);
  element_set (b2, a);
  element_set (b, a);
  element_mul (c, a, b);	// c=g^2
  element_mul (c, c, c);	// c=g^4
  element_init_Zr (x, p);
  element_dlog_brute_force (x, a, c);
  element_to_mpz (D, x);
  printf ("brute force of c=%lu\n", mpz_get_ui (D));
  element_mul (c2, a2, b2);	// c2=g^2
  element_mul (d2, c2, a2);	// d2=g^3
  element_mul (e2, d2, a2);	// e2=g^4
  printf ("%d\n", element_cmp (c, e2));	// should return 0
  element_mul (f2, e2, a2);	// f2=g^5
  printf ("%d\n", element_cmp (c, f2));	// should return 1
  element_div (g2, f2, a2);
  printf ("%d\n", element_cmp (c, g2));	// should return 0
  element_init_G2 (A2, p);
  element_init_G2 (B2, p);
  element_init_G2 (C2, p);
  element_init_G2 (D2, p);
  element_init_G2 (E2, p);
  element_init_G2 (F2, p);
  element_init_G2 (G2, p);
  element_mul (C2, A2, B2);
  element_mul (D2, C2, A2);
  element_random (A2);
  element_set (C2, A2);
  element_init_GT (gT, p);
  element_init_GT (gT2, p);
  element_init_GT (gT3, p);
//element_set1(c2);
  element_pairing (gT, c2, C2);	// gT=gT^2
//element_set1(e2);
  element_pairing (gT2, e2, A2);	//
  element_dlog_brute_force (x, gT, gT2);
  element_to_mpz (D, x);
  printf ("brute force of gT2=%lu\n", mpz_get_ui (D));
  printf ("%d\n", element_cmp (gT, gT2));
  element_pairing (gT, c2, D2);
  printf ("%d\n", element_cmp (gT, gT2));
  element_pairing (gT3, a2, C2);
  element_div (gT, gT, gT3);
  printf ("%d\n", element_cmp (gT, gT2));
  element_init_G1 (a, p);
  element_init_G1 (a2, p);
  element_from_hash (a, "abc", 3);
  element_from_hash (a2, "abc", 3);
  printf ("cmp hash1  %d\n", element_cmp (a, a2));
  element_from_hash (a2, "abd", 3);
  printf ("cmp hash2 %d\n", element_cmp (a, a2));
  element_init_Zr (a, p);
  element_init_Zr (b, p);
  element_from_hash (a, "abc", 3);
  element_from_hash (b, "abd", 3);
  printf ("%d\n", element_cmp (a, b));
  element_from_hash (b, "abc", 3);
  printf ("%d\n", element_cmp (a, b));

  element_init_Zr (b3, p);
  element_init_Zr (d3, p);
  element_init_Zr (one, p);
  element_init_G2 (a3, p);
  element_init_G2 (c3, p);
  element_init_G2 (d3, p);
  element_random (a3);
#if PBC_OR_CIFER == 1
  mpz_from_BIG_256_56 (b3[0].z, (int64_t *) CURVE_Order_BN254);
#else
  element_set0 (b3);
#endif
  element_pow_zn (c3, a3, b3);
  printf ("%d\n", element_is1 (c3));
  element_set1 (one);
  element_add (b3, b3, one);
  element_pow_zn (d3, a3, b3);
  printf ("%d\n", element_cmp (d3, a3));
  printf ("%d\n", element_is1 (d3));
  printf ("%d\n", element_is1 (b3));

#if PBC_OR_CIFER == 1
//mpz_from_BIG_256_56(b3[0].z, (int64_t *) CURVE_Order_BN254);
//mpz_add_ui(b3[0].z,b3[0].z,1);
//mpz_mod(b3[0].z,b3[0].z,b3[0].p);
  element_set0 (b3);
#else
  element_set0 (b3);
#endif
  element_init_GT (GT3, p);
  element_init_GT (GT4, p);
// FP12_BN254_output(&(GT3[0].gT));
// FP12_BN254_output(&(GT4[0].gT));
  element_pow_zn (GT4, GT3, b3);
//BIG_256_56_from_mpz(s,b3[0].z);
//FP12_BN254_pow(&(GT4[0].gT),&(GT3[0].gT),s);
// FP12_BN254_output(&(GT4[0].gT));
//printf("%lu\n",mpz_get_ui(b3[0].z));
  printf ("%d\n", element_is1 (GT4));

  {
    element_t H1, H2;
    char str[2049];
    strcpy (str, "abc");
    element_init_Zr (H1, p);
    element_init_Zr (H2, p);
    element_from_hash (H1, str, 2048);
    element_from_hash (H2, str, 2048);
    printf ("H1==H2? %d\n", element_cmp (H1, H2));

  }
}
