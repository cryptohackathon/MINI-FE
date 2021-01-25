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
InputSecretKey (int voter, element_t * secret_key,pairing_t *pairing)
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
  GenerateSecretKeyFromInt (secret_key, pin,pairing);
  clearscreen ();
}
void error(char *s){
fprintf(stderr,"usage: %s N g pk1 ... pkN ct1 ... ctN\nThe program will evaluate the tally for the N participants given their public-keys pk1 ... pkN, ciphertexts ct1 ... ctN and the generator g.\n",s);
exit(1);
}
int
main (int argc, char **argv)
{
  int i, election, N,r;
  float average;
  pairing_t pairing;
  element_t g;			//, h;
  element_t temp, temp1, temp2, zero, res;
  mpz_t z;
  pairing_pp_t pp;
  pairing_init_set_str (pairing, Param);
  element_init_G1 (temp, pairing);
  element_init_Zr (zero, pairing);
  element_init_Zr (res, pairing);
if (argc==1) error(argv[0]);
      if (argv[1]==NULL) error(argv[0]); else 
	      N = atoi(argv[1]);


  {
    char str[10];
    element_t public_key[N], hash, Y[N];
    element_t CT[N];
element_t tx;
    ChaumPedersenProof Proofs[N][RANGE_OF_GRADING];
    if (argv[2]==NULL) error(argv[0]);
    else read_element_from_file(g,argv[2]);
	
	element_init_GT (temp1, pairing);
	element_init_GT (temp2, pairing);

element_init_Zr(tx,pairing);

   for (i=0;i<N;i++){
    if (argv[3+i]==NULL) error(argv[0]);
    else read_element_from_file(public_key[i],argv[3+i]);
element_dlog_brute_force (tx, g, public_key[i]);
printf("%lu\n",mpz_get_ui(tx[0].z));
   }
    for (i = 1; i <= N; i++){
	element_init_G2 (Y[i - 1], pairing);
	element_set1 (Y[i-1]);
    }
/*compute the Y_i's */
    for (i = 1; i <= N; i++)      ComputeY (&Y[i - 1], N, i, public_key, &pairing);

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
//	  printf ("Y[%d]=", i);
//	  ECP2_BN254_output (&Y[i - 1][0].g2);
	  printf ("\n");
#endif
	}
    }
	    printf("Insert the identifier of the election: ");
	    scanf("%d",&election);
	sprintf (str, "%d\n", election);
	
	ComputeId(&hash,str,&pairing);
	
	pairing_pp_init (pp, hash, pairing);	/* we are going to do a lot of pairings with this value */
	printf
	  ("Press %s1 for verifying an Average Grade ceremony,%s 2 for Dead or Alive,%s 3 for Victory by unanimity,%s q to quit%s ",
	   KCYN, KMAG, KYEL, KGRN, KWHT);
while(1) {
	scanf ("%2s", str);
	    if (str[1] == '\0')break;
	    printf ("Invaid choice. Press %s1 for verifying an Average Grade ceremony,%s 2 for Dead or Alive,%s 3 for Victory by unanimity,%s q to quit%s ", KCYN, KMAG, KYEL, KGRN, KWHT);
	    flush ();
  }
	switch (str[0])
	  {
	  case '1':
		  mpz_init(z);
   for (i=0;i<N;i++){
    if (argv[3+N+i]==NULL) error(argv[0]);
    else read_ciphertextwproofs_from_file(CT[i],Proofs[i],argv[3+N+i]);
   }
	      printf ("%sEvaluating the result for...pls wait%s\n",
	       KBLU, KWHT);
	    if (EvalTallyGrade
		(N, &pairing, &g, &hash, &public_key[0], Y, CT, &res,
		 Proofs) == 0)
	      {
		printf
		  ("%sOne of the ciphertext is invalid\nAborting...%s\n",
		   KRED, KWHT);
		break;
	      }
	    else {
	    element_to_mpz (z, res);
	    average = mpz_get_si (z);
	    average /= N;
	    printf ("%sAverage grade for candidate #%d = %s%f%s%s\n\n\n",
		    KCYN, election, BLINK, average, NOBLINK, KWHT);
	    }
    break;
	  case '2':
   for (i=0;i<N;i++){
    if (argv[3+N+i]==NULL) error(argv[0]);
    else read_ciphertext_from_file(CT[i],argv[3+N+i]);
   }
	    printf
	      ("%sEvaluating the result...pls wait%s\n",
	       KBLU, KWHT);
	    EvalTallyDeadOrAlive (N, &pairing, &g, &hash, &CT[0], &r);
	    if (r == 1)
	      printf
		("%sResult of Dead or Alive Decision = %s%sDead%s%s\n\n\n",
		 KMAG, BLINK, KRED, KWHT, NOBLINK);
	    else
	      printf
		("%sResult of Dead or Alive Decision = %s%sAlive%s%s\n\n\n",
		 KMAG, BLINK, KGRN, KWHT, NOBLINK);
	    break;
	  case '3':
   for (i=0;i<N;i++){
    if (argv[3+N+i]==NULL) error(argv[0]);
    else read_ciphertext_from_file(CT[i],argv[3+N+i]);
   }
	    printf
	      ("%sEvaluating the result...pls wait%s\n",
	       KBLU, KWHT);
	    EvalTallyUnanimity (N, &pairing, &g, &hash, &CT[0], &r);
	    if (r == 1)
	      printf
		("%sResult of Decision by Unanimity for candidate = %s%sVictory by Unanimity%s%s\n\n\n",
		 KYEL, BLINK, KGRN, KWHT, NOBLINK);
	    else
	      printf
		("%sResult of Decision by Unanimity for candidate = %s%sNot approved by unanimity%s%s\n\n\n",
		 KYEL, BLINK, KRED, KWHT, NOBLINK);
	    break;
	  case 'q':
	    return 0;
	    break;
	  default:
	    printf ("%sChoice not valid%s\n\n\n", KRED, KWHT);
	    break;
	pairing_pp_clear (pp);
      }

    }

  return 0;
}
