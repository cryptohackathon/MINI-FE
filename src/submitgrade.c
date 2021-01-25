// MINI-FE demo.
//
// Vincenzo Iovino
#include <stdio.h>
#include "pairings.h"
#include "orproofs.h"
#include "nife.h"
#define KNRM  "\x1B[0m"
#define BLINK  "\x1B[5m"
#define NOBLINK  "\x1B[m"
#define KBLACK  "\x1B[30m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
void
flush (void)
{
  int c;
  while ((c = getchar ()) != '\n' && c != EOF);
}




void
clearscreen (void)
{
  int i;
  for (i = 0; i <= 100; i++)
    printf ("\n");
}

void
InputSecretKey (int voter, element_t * secret_key, pairing_t * pairing)
{
  int pin;

  while (1)
    {
      int n, c;
      printf ("Judge %d, input you secret PIN: %s", voter, KBLACK);
      n = scanf ("%d", &pin);
      printf ("%s", KWHT);
      while (1)
	{
	  if ((c = getchar ()) != '\n' && c != EOF)
	    n = -1;
	  else
	    break;
	}
      if (n != 1)
	{
	  printf ("\n Error: your PIN must be numerical\n");
	}
      else
	break;
    }
  GenerateSecretKeyFromInt (secret_key, pin, pairing);
  clearscreen ();
}

void
error (char *s)
{
  fprintf (stderr,
	   "usage: %s N sk CT g i pk1 ... pkN\nThe program will generate a ciphertext into the file CT from a secret-key in the file sk, generator in the file g and public-keys in the files pk1 ... pkN, where N is the number of participants and i is the the identifier of the party.\n",
	   s);
  exit (1);
}

int
main (int argc, char **argv)
{
  int i, election, N;
  pairing_t pairing;
  element_t g;
  element_t temp, temp1, temp2, zero, res;
  pairing_pp_t pp;
  pairing_init_set_str (pairing, Param);
  element_init_G1 (temp, pairing);
  element_init_Zr (zero, pairing);
  element_init_Zr (res, pairing);
  {
    int P;
    if (argc == 1)
      error (argv[0]);
    if (argv[1] == NULL)
      error (argv[0]);
    else
      N = atoi (argv[1]);
    if (argv[5] == NULL)
      error (argv[0]);
    else
      P = atoi (argv[5]) - 1;


    {
      mpz_t z;
      char str[10];
      element_t public_key[N], hash, Y;
      element_t secret_key;
      element_t CT;
      ChaumPedersenProof Proofs[RANGE_OF_GRADING];
      long int vote;
      mpz_init (z);
      if (argv[4] == NULL)
	error (argv[0]);
      else
	read_element_from_file (g, argv[4]);

      element_init_G2 (Y, pairing);
      element_init_GT (temp1, pairing);
      element_init_GT (temp2, pairing);
      element_set1 (Y);



      if (argv[2] == NULL)
	error (argv[0]);
      else
	read_element_from_file (secret_key, argv[2]);
      for (i = 0; i < N; i++)
	{
	  if (argv[6 + i] == NULL)
	    error (argv[0]);
	  else
	    read_element_from_file (public_key[i], argv[6 + i]);
	}



/* print all PKs */
      {
#if PBC_OR_CIFER == 1
	char string[512];
#endif
	for (i = 1; i <= N; i++)
	  {
#if PBC_OR_CIFER == 0
	    element_printf ("public key of participant %d= %B\n\n", i,
			    public_key[i - 1]);
#else
	    element_snprintf_publickey (string, i, public_key[i - 1]);
	    printf ("public key of participant %d=", i);
	    ECP2_BN254_output (&public_key[i - 1][0].g2);
	    printf ("\n");
#endif
	  }
      }
/*compute the Y_P */
      ComputeY (&Y, N, P + 1, public_key, &pairing);
      //  printf ("Y=");
      //ECP2_BN254_output (&Y[0].g2);


      printf ("Insert the identifier of the election: ");
      scanf ("%d", &election);
      sprintf (str, "%d\n", election);

      ComputeId (&hash, str, &pairing);

      pairing_pp_init (pp, hash, pairing);	/* we are going to do a lot of pairings with this value */
      printf
	("Press %s1 for computing an Average Grade,%s 2 for Dead or Alive,%s 3 for Victory by unanimity,%s q to quit%s ",
	 KCYN, KMAG, KYEL, KGRN, KWHT);
      while (1)
	{
	  scanf ("%2s", str);
	  if (str[1] == '\0')
	    break;
	  printf
	    ("Invaid choice. Press %s1 for computing an Average Grade,%s 2 for Dead or Alive elections,%s 3 for Victory by unanimity,%s q to quit%s ",
	     KCYN, KMAG, KYEL, KGRN, KWHT);
	  flush ();
	}
      switch (str[0])
	{
	case '1':
	  printf
	    ("Participant #%d, give a grade [from 0 to %d]: %s",
	     P + 1, RANGE_OF_GRADING - 1, KBLACK);
	  scanf ("%ld", &vote);
	  printf ("%s", KWHT);
	  EncodeGrade (&g, &pp, &pairing, &hash, &secret_key, &vote, &Y, &CT,
		       Proofs);
	  if (argv[3] == NULL)
	    error (argv[3]);
	  else
	    write_ciphertextwproofs_to_file (CT, Proofs, argv[3]);
	  break;
	case '2':
	  printf ("Participant #%d, insert your decision [0 or 1]: ", P + 1);
	  scanf ("%ld", &vote);
	  EncodeDeadOrAlive (&g, &pp, &pairing, &hash, &secret_key, &vote, &Y,
			     &CT);
	  if (argv[3] == NULL)
	    error (argv[3]);
	  else
	    write_ciphertext_to_file (CT, argv[3]);
	  break;
	case '3':
	  printf ("Participant #%d, insert your decision [0 or 1]: ", P + 1);
	  scanf ("%ld", &vote);
	  EncodeUnanimity (&g, &pp, &pairing, &hash, &secret_key, &vote, &Y,
			   &CT);
	  if (argv[3] == NULL)
	    error (argv[3]);
	  else
	    write_ciphertext_to_file (CT, argv[3]);
	  break;
	case 'q':
	  return 0;
	  break;
	default:
	  printf ("%sChoice not valid%s\n\n\n", KRED, KWHT);
	  break;
	  pairing_pp_clear (pp);
	}
      mpz_clear (z);

    }
  }

  return 0;
}
