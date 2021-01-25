#include "nife.h"
#include <unistd.h>
void
EncodeGrade (element_t * g, pairing_pp_t * pp, pairing_t * pairing,
	     element_t * hash, element_t * secret_key, const long int *grade,
	     element_t * Y, element_t * Res,
	     ChaumPedersenProof Proof[RANGE_OF_GRADING])
{
  element_t temp1, temp2, temp3, temp4;
  element_t pk, Pk, Base, C1;
  mpz_t v;
  mpz_init (v);
  mpz_set_si (v, *grade);
  element_init_Zr (temp3, *pairing);
  element_set_mpz (temp3, v);
  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_G2 (temp4, *pairing);
  element_init_GT (*Res, *pairing);
  element_init_G2 (pk, *pairing);
  element_init_GT (Pk, *pairing);
  element_init_GT (Base, *pairing);
  element_init_GT (C1, *pairing);
  element_pow_zn (temp4, *Y, *secret_key);
  pairing_pp_apply (temp1, temp4, *pp);
  pairing_pp_apply (temp2, *g, *pp);
  element_pow_zn (temp2, temp2, temp3);
  element_mul (*Res, temp1, temp2);
  mpz_clear (v);
  element_pairing (Base, *hash, *g);
  element_pow_zn (pk, *g, *secret_key);
  element_pairing (Pk, *hash, *Y);
  element_pairing (C1, *hash, pk);
  if (*grade >= 0 && *grade <= RANGE_OF_GRADING - 1)
    CDS3Prover (pairing, &Base, &Pk, &C1, Res, secret_key, *grade, Proof);
  else
    CDS3Prover (pairing, &Base, &Pk, &C1, Res, secret_key, 0, Proof);
}

int
EvalTallyGrade (const int N, pairing_t * pairing, element_t * g,
		element_t * hash, element_t pk[], element_t Y[],
		element_t CT[], element_t * Res,
		ChaumPedersenProof Proofs[][RANGE_OF_GRADING])
{
  int i;
  element_t temp, temp2, Pk, C1, Base;

  element_init_GT (temp, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_GT (Pk, *pairing);
  element_init_GT (C1, *pairing);
  element_init_GT (Base, *pairing);
  element_set1 (temp);
  element_pairing (Base, *hash, *g);

  for (i = 1; i <= N; i++)
    {
      element_pairing (Pk, *hash, Y[i - 1]);
      element_pairing (C1, *hash, pk[i - 1]);
      element_set (temp2, CT[i - 1]);
#if DEBUG_PROOFS == 1
      printf ("%sVerifying Ciphertext #%d%s: ", KBLU, i, KWHT);
#endif
      if (CDS3Verifier (pairing, &Base, &Pk, &C1, &temp2, &Proofs[i - 1][0])
	  == 0)
	{
#if DEBUG_PROOFS == 1
	  printf ("%sCiphertext #%d is invalid\n%s\n", KRED, i, KWHT);
#endif
	  return 0;
	}
      else
	{
#if DEBUG_PROOFS == 1
	  printf ("%sOK%s\n", KGRN, KWHT);
#endif
	}
      element_mul (temp, temp, CT[i - 1]);
      {
	element_t T;
	element_init_GT (T, *pairing);
	element_pairing (T, *hash, *g);
      }
    }

  element_pairing (temp2, *hash, *g);
  if (element_is1 (temp))
    {
      element_set0 (*Res);
#if PBC_OR_CIFER == 1
      mpz_set_si ((*Res)[0].z, 0);
#endif
    }
  else
    element_dlog_brute_force (*Res, temp2, temp);
  return 1;
}

void
EncodeDeadOrAlive (element_t * g, pairing_pp_t * pp,
		   pairing_t * pairing, element_t * hash,
		   element_t * secret_key, const long int *decision,
		   element_t * Y, element_t * Res)
{
  element_t temp1, temp2, temp3, temp4;
  mpz_t v;
  mpz_init (v);
  element_init_Zr (temp3, *pairing);
  element_init_GT (*Res, *pairing);
  if (*decision == 0)
    {
      mpz_set_si (v, *decision);
      element_set_mpz (temp3, v);
    }
  else
    element_random (temp3);

  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_G2 (temp4, *pairing);
  element_pow_zn (temp4, *Y, *secret_key);
  pairing_pp_apply (temp1, temp4, *pp);
  pairing_pp_apply (temp2, *g, *pp);
  element_pow_zn (temp2, temp2, temp3);
  element_mul (*Res, temp1, temp2);
  mpz_clear (v);
}

void
EvalTallyDeadOrAlive (const int N, pairing_t * pairing, element_t * g,
		      element_t * hash, element_t CT[], int *res)
{
  int i;
  element_t temp, temp2;

  element_init_GT (temp, *pairing);
  element_init_GT (temp2, *pairing);
  element_set1 (temp);

  for (i = 1; i <= N; i++)
    {
      element_mul (temp, temp, CT[i - 1]);
      printf ("%sVerifying CT #%d%s: OK%s\n", KBLU, i, KGRN, KWHT);
    }
  if (element_is1 (temp))
    *res = 1;
  else
    *res = 0;

}

void
EncodeUnanimity (element_t * g, pairing_pp_t * pp,
		 pairing_t * pairing, element_t * hash,
		 element_t * secret_key, const long int *decision,
		 element_t * Y, element_t * Res)
{
  element_t temp1, temp2, temp3, temp4;
  mpz_t v;
  mpz_init (v);
  element_init_Zr (temp3, *pairing);
  element_init_GT (*Res, *pairing);
  if (*decision == 1)
    {
      mpz_set_si (v, 0);
      element_set_mpz (temp3, v);
    }
  else
    element_random (temp3);

  element_init_GT (temp1, *pairing);
  element_init_GT (temp2, *pairing);
  element_init_G2 (temp4, *pairing);
  element_pow_zn (temp4, *Y, *secret_key);
  pairing_pp_apply (temp1, temp4, *pp);
  pairing_pp_apply (temp2, *g, *pp);
  element_pow_zn (temp2, temp2, temp3);
  element_mul (*Res, temp1, temp2);
  mpz_clear (v);
}

void
EvalTallyUnanimity (const int N, pairing_t * pairing, element_t * g,
		    element_t * hash, element_t CT[], int *res)
{
  int i;
  element_t temp, temp2;

  element_init_GT (temp, *pairing);
  element_init_GT (temp2, *pairing);
  element_set1 (temp);

  for (i = 1; i <= N; i++)
    {
      element_mul (temp, temp, CT[i - 1]);
      printf ("%sVerifying CT #%d%s: OK%s\n", KBLU, i, KGRN, KWHT);
    }
  if (element_is1 (temp))
    *res = 1;
  else
    *res = 0;

}

void
GenerateSecretKey (element_t * secret_key, pairing_t * p)
{

  element_init_Zr (*secret_key, *p);
  element_random (*secret_key);

}

void
GenerateSecretKeyFromInt (element_t * secret_key, int pin, pairing_t * p)
{
  element_init_Zr (*secret_key, *p);

  mpz_t z;
  mpz_init (z);
  mpz_set_si (z, pin);
  element_set_mpz (*secret_key, z);
  mpz_clear (z);

}

void
ComputeGenerator (element_t * g, pairing_t * p)
{
  element_init_G2 (*g, *p);
  element_random (*g);

}

void
ComputePublicKey (element_t * public_key, element_t * g,
		  element_t * secret_key, pairing_t * p)
{
  // element_init_G2 (*g, *p);
  //element_random (*g);
  element_init_G2 (*public_key, *p);
  element_pow_zn (*public_key, *g, *secret_key);
}


void
ComputeY (element_t * Y, int N, int i, element_t public_key[], pairing_t * p)
{
  int j;
  element_t temp;
  element_init_G1 (temp, *p);
  element_set1 (temp);

  for (j = 1; j <= N; j++)
    {
      if (j < i)
	element_add (*Y, *Y, public_key[j - 1]);
      else if (j > i)
	element_sub (*Y, *Y, public_key[j - 1]);

    }

}

void
ComputeId (element_t * hash, char *str, pairing_t * p)
{
  element_init_G1 (*hash, *p);
  element_from_hash (*hash, str, strlen (str));
}


#if PBC_OR_CIFER == 1
int
write_ciphertextwproofs_to_file (element_t CT,
				 ChaumPedersenProof Proof[RANGE_OF_GRADING],
				 char *filename)
{
  FILE *f_ct;
  int i;
  if ((f_ct = fopen (filename, "w+")) == NULL)
    return -1;
  _write_element_to_file (CT, f_ct);
  for (i = 0; i < RANGE_OF_GRADING; i++)
    {
      _write_element_to_file (Proof[i].a, f_ct);
      _write_element_to_file (Proof[i].b, f_ct);
      _write_element_to_file (Proof[i].e, f_ct);
      _write_element_to_file (Proof[i].z, f_ct);
    }
  if (fclose (f_ct) != 0)
    return -1;
  truncate (filename, 2048 * 16);	// we need this to make all ciphertexts of the same length - this may be needed when all ciphertexts are broacasted and the receiver receives the concatenation of all ciphertexts without knowing the corresponding length 
  return 0;
}

int
write_ciphertext_to_file (element_t CT, char *filename)
{
  FILE *f_ct;
  if ((f_ct = fopen (filename, "w+")) == NULL)
    return -1;
  _write_element_to_file (CT, f_ct);
  if (fclose (f_ct) != 0)
    return -1;
  truncate (filename, 2048 * 16);	// we need this to make all ciphertexts of the same length - this may be needed when all ciphertexts are broacasted and the receiver receives the concatenation of all ciphertexts without knowing the corresponding length 
  return 0;
}

int
read_ciphertextwproofs_from_file (element_t CT,
				  ChaumPedersenProof Proof[RANGE_OF_GRADING],
				  char *filename)
{
  FILE *f_ct;
  int i;
  if ((f_ct = fopen (filename, "r")) == NULL)
    return -1;
  _read_element_from_file (CT, f_ct);
  for (i = 0; i < RANGE_OF_GRADING; i++)
    {
      _read_element_from_file (Proof[i].a, f_ct);
      _read_element_from_file (Proof[i].b, f_ct);
      _read_element_from_file (Proof[i].e, f_ct);
      _read_element_from_file (Proof[i].z, f_ct);
    }
  if (fclose (f_ct) != 0)
    return -1;
  return 0;
}

int
read_ciphertext_from_file (element_t CT, char *filename)
{
  FILE *f_ct;
  if ((f_ct = fopen (filename, "r")) == NULL)
    return -1;
  _read_element_from_file (CT, f_ct);
  if (fclose (f_ct) != 0)
    return -1;
  return 0;
}
#endif
