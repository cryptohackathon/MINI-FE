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
  return;
  for (i = 0; i <= 100; i++)
    printf ("\n");
}

void
InputSecretKey (int voter, element_t * secret_key)
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
  GenerateSecretKeyFromInt (secret_key, pin);
  clearscreen ();
}

int
main (int argc, char **argv)
{
  int i, election, N;
  pairing_t pairing;
  element_t g;			//, h;
  element_t temp, temp1, temp2, zero, res;
  pairing_pp_t pp;
  //pbc_demo_pairing_init(pairing, argc, argv);
  pairing_init_set_str (pairing, Param);
  element_init_G2 (g, pairing);
  element_init_G1 (temp, pairing);
  element_init_Zr (zero, pairing);
  element_init_Zr (res, pairing);
  //generate public parameters
  element_random (g);
  //element_printf("public parameter g = %B\n", g);

  clearscreen ();
  while (1)
    {
      int n, c;
      printf
	("%sWelcome to our NIFE system. You can use this to grade candidates for the Crypto Hackaton%s\n",
	 KCYN, KWHT);
      printf ("To begin input the number of participants: ");
      n = scanf ("%d", &N);
      while (1)
	{
	  if ((c = getchar ()) != '\n' && c != EOF)
	    n = -1;
	  else
	    break;
	}

      if (n != 1)
	{
	  printf ("\n Error: the input must be numerical\n");
	}
      else
	break;
    }


  {
    mpz_t z;
    float average;
    char str[10];
    int r;
    element_t public_key[N], hash, Y[N];
    element_t secret_key[N];
    element_t CT[N];
    ChaumPedersenProof Proofs[N][3];
    long int vote[N];
    mpz_init (z);
    element_init_G1 (hash, pairing);
/* setup */
    for (i = 1; i <= N; i++)
      {
	element_init_G2 (public_key[i - 1], pairing);
	element_init_G2 (Y[i - 1], pairing);
	element_init_GT (temp1, pairing);
	element_init_GT (temp2, pairing);
	element_init_Zr (secret_key[i - 1], pairing);
	element_set1 (Y[i - 1]);


	/*
	   generate private key
	 */
	InputSecretKey (i, &secret_key[i - 1]);

	/*
	   element_printf("private key %d = %B\n\n", i, secret_key[i-1]);
	 */
	/* compute corresponding public key */
	ComputePublicKey (&public_key[i - 1], &g, &secret_key[i - 1]);

      }
/* print all PKs */
    {
#if PBC_OR_CIFER == 1
      char string[512];
#endif
      for (i = 1; i <= N; i++)
	{
#if PBC_OR_CIFER == 0
	  element_printf ("public key of Judge %d= %B\n\n", i,
			  public_key[i - 1]);
#else
	  element_snprintf_publickey (string, i, public_key[i - 1]);
	  printf ("public key of Judge %d=", i);
	  ECP2_BN254_output (&public_key[i - 1][0].g2);
	  printf ("\n");
#endif
	}
    }
/* end setup */
/*compute the Y_i's */
    for (i = 1; i <= N; i++)
      ComputeY (&Y[i - 1], N, i, public_key, &pairing);
    /*
       for (i = 1; i <= N; i++)
       {
       element_set1 (temp);

       for (j = 1; j <= N; j++)
       {
       if (j < i)
       element_add (Y[i - 1], Y[i - 1], public_key[j - 1]);
       else if (j > i)
       element_sub (Y[i - 1], Y[i - 1], public_key[j - 1]);

       }
       }
     */
/*end of computation of the Y_i's */
    for (election = 1;; election++)
      {
	sprintf (str, "%d\n", election);
	printf ("\n\nEvaluating the Project of Candidate #%s\n\n", str);
	element_from_hash (hash, str, 2);
	pairing_pp_init (pp, hash, pairing);	/* we are going to do a lot of pairings with this value */
	printf
	  ("How do you want to judge the project's candidate? press %s1 for computing an Average Grade,%s 2 for Dead or Alive elections,%s 3 for Victory by unanimity,%s q to quit%s ",
	   KCYN, KMAG, KYEL, KGRN, KWHT);
	while (1)
	  {
	    scanf ("%2s", str);
	    if (str[1] == '\0')
	      break;
	    printf
	      ("Invaid choice. How do you want to judge the project's candidate? press %s1 for computing an Average Grade,%s 2 for Dead or Alive elections,%s 3 for Victory by unanimity,%s q to quit%s ",
	       KCYN, KMAG, KYEL, KGRN, KWHT);
	    flush ();
	  }
	switch (str[0])
	  {
	  case '1':
	    for (i = 1; i <= N; i++)
	      {
		InputSecretKey (i, &secret_key[i - 1]);
		printf
		  ("Judge #%d, give a grade to the candidate #%d [0 for reject, 1 for borderline, 2 for accept]: %s",
		   i, election, KBLACK);
		scanf ("%ld", &vote[i - 1]);
		printf ("%s", KWHT);
		clearscreen ();
//element_printf("uuuu#%B\n",public_key[0]);
		EncodeGrade (&g, &pp, &pairing, &hash, &secret_key[i - 1],
			     &vote[i - 1], &Y[i - 1], &CT[i - 1],
			     Proofs[i - 1]);
//element_printf("uuuu#%B\n",public_key[0]);
	      }
	    printf
	      ("%sEvaluating the result for candidate #%d...pls wait%s\n",
	       KBLU, election, KWHT);
	    if (EvalTallyGrade
		(N, &pairing, &g, &hash, &public_key[0], Y, CT, &res,
		 Proofs) == 0)
	      {
		printf
		  ("%sOne of the Judges cast invalid grade or used wrong secret PIN\nAborting...%s\n",
		   KRED, KWHT);
		break;
	      }
	    element_to_mpz (z, res);
	    average = mpz_get_si (z);
	    average /= N;
	    printf ("%sAverage grade for candidate #%d = %s%f%s%s\n\n\n",
		    KCYN, election, BLINK, average, NOBLINK, KWHT);
	    break;
	  case '2':
	    for (i = 1; i <= N; i++)
	      {
		InputSecretKey (i, &secret_key[i - 1]);
		printf ("Judge #%d, insert your decision [0 or 1]: ", i);
		scanf ("%ld", &vote[i - 1]);
		clearscreen ();
		EncodeDeadOrAlive (&g, &pp, &pairing, &hash,
				   &secret_key[i - 1], &vote[i - 1],
				   &Y[i - 1], &CT[i - 1]);
	      }

	    printf
	      ("%sEvaluating the result for candidate #%d...pls wait%s\n",
	       KBLU, election, KWHT);
	    EvalTallyDeadOrAlive (N, &pairing, &g, &hash, &CT[0], &r);
	    if (r == 1)
	      printf
		("%sResult of Dead or Alive Decision #%d = %s%sDead%s%s\n\n\n",
		 KMAG, election, BLINK, KRED, KWHT, NOBLINK);
	    else
	      printf
		("%sResult of Dead or Alive Decision #%d = %s%sAlive%s%s\n\n\n",
		 KMAG, election, BLINK, KGRN, KWHT, NOBLINK);


	    break;

	  case '3':
	    for (i = 1; i <= N; i++)
	      {
		InputSecretKey (i, &secret_key[i - 1]);
		printf ("Judge #%d, insert your decision [0 or 1]: ", i);
		scanf ("%ld", &vote[i - 1]);
		clearscreen ();
		EncodeUnanimity (&g, &pp, &pairing, &hash,
				 &secret_key[i - 1], &vote[i - 1], &Y[i - 1],
				 &CT[i - 1]);
	      }

	    printf
	      ("%sEvaluating the result for candidate #%d...pls wait%s\n",
	       KBLU, election, KWHT);
	    EvalTallyUnanimity (N, &pairing, &g, &hash, &CT[0], &r);
	    if (r == 1)
	      printf
		("%sResult of Decision by Unanimity for candidate  #%d = %s%sVictory by Unanimity%s%s\n\n\n",
		 KYEL, election, BLINK, KGRN, KWHT, NOBLINK);
	    else
	      printf
		("%sResult of Decision by Unanimity for candidate #%d = %s%sNot approved by unanimity%s%s\n\n\n",
		 KYEL, election, BLINK, KRED, KWHT, NOBLINK);
	    break;
	  case 'q':
	    printf ("%sSee you at the next Crypto Hackaton%s\n", KBLU, KWHT);
	    return 0;
	  default:
	    printf ("%sChoice not valid%s\n\n\n", KRED, KWHT);
	    break;
	  }
	pairing_pp_clear (pp);

      }


    mpz_clear (z);
  }

  return 0;
}
