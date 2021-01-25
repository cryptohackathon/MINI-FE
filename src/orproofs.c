#include "pairings.h"
#include "orproofs.h"


void
ChaumPedersenProverFixedChallenge (pairing_t * pairing, element_t * g,
				   element_t * h, element_t * u,
				   element_t * v, element_t * hash,
				   element_t * w, ChaumPedersenProof * res)
{
  /* a=g^r, b=h^r, z=r+H(a,b)w, where h=g^w */
  element_t r;
  element_init_Zr (r, *pairing);
  element_init_GT (res->a, *pairing);
  element_init_GT (res->b, *pairing);
  element_init_Zr (res->z, *pairing);
  element_init_Zr (res->e, *pairing);
  element_random (r);
  element_set (res->e, *hash);
  element_pow_zn (res->a, *g, r);
  element_pow_zn (res->b, *h, r);
  element_mul (res->z, res->e, *w);
  element_add (res->z, res->z, r);
}

/* not used */
void
ChaumPedersenProver (pairing_t * pairing, element_t * g, element_t * h,
		     element_t * u, element_t * v, element_t * w,
		     ChaumPedersenProof * res)
{
  /* a=g^r, b=h^r, z=r+H(a,b)w, where h=g^w */
  char str[2048];
  element_t r, hash;
  element_init_Zr (r, *pairing);
  element_init_Zr (hash, *pairing);
  element_init_GT (res->a, *pairing);
  element_init_GT (res->b, *pairing);
  element_init_Zr (res->z, *pairing);
  element_init_Zr (res->e, *pairing);
  element_random (r);
  element_pow_zn (res->a, *g, r);
  element_pow_zn (res->b, *h, r);
  element_snprintf (str, 2048, "%B%B%B%B", *g, *h, *u, *v);
  element_from_hash (hash, str, 2048);
  element_set (res->e, hash);
  element_mul (res->z, hash, *w);
  element_add (res->z, res->z, r);
}

void
ChaumPedersenSimulator (pairing_t * pairing, element_t * g, element_t * h,
			element_t * u, element_t * v,
			ChaumPedersenProof * res)
{
  /* a=g^r, b=h^r, z=r+H(a,b)w, where h=g^w */
  char str[2048];
  element_t hash, temp1, temp2, temp3, temp4;
  element_init_Zr (hash, *pairing);
  element_init_GT (temp3, *pairing);
  element_init_GT (temp4, *pairing);
  element_init_GT (res->a, *pairing);
  element_init_GT (res->b, *pairing);
  element_init_Zr (res->z, *pairing);
  element_init_Zr (res->e, *pairing);
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_random (res->z);
  element_pow_zn (temp1, *g, res->z);
  element_pow_zn (temp2, *h, res->z);
  element_snprintf (str, 2048, "%B%B%B%B", *g, *h, *u, *v);
  element_from_hash (hash, str, 2048);
  element_set (res->e, hash);
  element_pow_zn (temp3, *u, hash);
  element_pow_zn (temp4, *v, hash);
  element_div (res->a, temp1, temp3);
  element_div (res->b, temp2, temp4);
}

int
ChaumPedersenVerifierFixedChallenge (pairing_t * pairing, element_t * g,
				     element_t * h, element_t * u,
				     element_t * v, ChaumPedersenProof * CP)
{
  element_t temp1, temp2, temp3, temp4;
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_GT (temp3, *pairing);
  element_init_GT (temp4, *pairing);
  element_pow_zn (temp1, *g, CP->z);
  element_pow_zn (temp2, *u, CP->e);
  element_mul (temp2, temp2, CP->a);
  element_pow_zn (temp3, *h, CP->z);
  element_pow_zn (temp4, *v, CP->e);
  element_mul (temp4, temp4, CP->b);
  if (element_cmp (temp1, temp2) == 0 && element_cmp (temp3, temp4) == 0)
    return 1;
  else
    return 0;

}

/* not used */
int
ChaumPedersenVerifier (pairing_t * pairing, element_t * g, element_t * h,
		       element_t * u, element_t * v, ChaumPedersenProof * CP)
{
  char str[2048];
  element_t hash;
  element_t temp1, temp2, temp3, temp4;
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_GT (temp3, *pairing);
  element_init_GT (temp4, *pairing);
  element_init_Zr (hash, *pairing);
  element_snprintf (str, 2048, "%B%B%B%B", *g, *h, *u, *v);
  element_from_hash (hash, str, 2048);
  element_pow_zn (temp1, *g, CP->z);
  element_pow_zn (temp2, *u, hash);
  element_mul (temp2, temp2, CP->a);
  element_pow_zn (temp3, *h, CP->z);
  element_pow_zn (temp4, *v, hash);
  element_mul (temp4, temp4, CP->b);
  if (element_cmp (temp1, temp2) == 0 && element_cmp (temp3, temp4) == 0)
    return 1;
//if (element_cmp(temp1,temp2)==0 && element_cmp(temp3,temp4)==0) return 1;
  else
    return 0;

}

void
CDS3Prover (pairing_t * pairing, element_t * g, element_t * h, element_t * u,
	    element_t * v, element_t * w, int vote, ChaumPedersenProof res[RANGE_OF_GRADING])
{
  /* prove that either (g,h,u,v) is a DH tuple, or (g,h,u,v/g) is a DH or (g,h,u,v/g^2) is a DH tuple. Here vote is in {0,1,2} */
  element_t temp1, temp2, temp3, temp4, hash;
  int i;
  char str[2048];
  mpz_t V;
  mpz_init (V);

  element_init_Zr (temp3, *pairing);
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_GT (temp4, *pairing);
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_Zr (hash, *pairing);
  element_snprintf (str, 2048, "%B%B%B%B", *g, *h, *u, *v);
  element_from_hash (hash, str, 2048);
  for (i = 0; i < RANGE_OF_GRADING; i++)	// the user can choose one out of RANGE_OF_GRADING values for her/his selection 
    {
      mpz_set_si (V, i);
      element_set_mpz (temp3, V);
      element_pow_zn (temp1, *g, temp3);
      element_div (temp2, *v, temp1);
      if (i != vote)
	{
	  ChaumPedersenSimulator (pairing, g, h, u, &temp2, &res[i]);
	  element_sub (hash, hash, res[i].e);
	}
      else
	element_set (temp4, temp2);
    }
  ChaumPedersenProverFixedChallenge (pairing, g, h, u, &temp4, &hash, w,
				     &res[vote]);
}

int
CDS3Verifier (pairing_t * pairing, element_t * g, element_t * h,
	      element_t * u, element_t * v, ChaumPedersenProof res[RANGE_OF_GRADING])
{
  /* prove that either (g,h,u,v) is a DH tuple, or (g,h,u,v/g) is a DH or (g,h,u,v/g^2) is a DH tuple. Here vote is in {0,1,2} */
  char str[2048];
  int i;
  mpz_t V;
  element_t hash, temp, temp1, temp2, temp3, temp4;
  mpz_init (V);
  element_init_Zr (hash, *pairing);
  element_init_Zr (temp, *pairing);
  element_init_Zr (temp3, *pairing);
  element_init_GT (temp1, *pairing);
  element_init_GT (temp4, *pairing);
  element_init_GT (temp2, *pairing);
  element_set0 (temp);
  element_snprintf (str, 2048, "%B%B%B%B", *g, *h, *u, *v);
  element_from_hash (hash, str, 2048);
  for (i = 0; i < RANGE_OF_GRADING; i++)
    {
      mpz_set_si (V, i);
      element_set_mpz (temp3, V);
      element_pow_zn (temp1, *g, temp3);
      element_div (temp2, *v, temp1);
      if (ChaumPedersenVerifierFixedChallenge
	  (pairing, g, h, u, &temp2, &res[i]) == 0)
	return 0;
      element_add (temp, temp, res[i].e);
    }
  if (element_cmp (temp, hash) != 0)
    return 0;
  return 1;
}
