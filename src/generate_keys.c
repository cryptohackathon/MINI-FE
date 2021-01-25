// MINI-FE.
//
// Vincenzo Iovino
#include "pairings.h"
#include "orproofs.h"
#include "nife.h"
#include <stdio.h>
void
InputSecretKey (element_t * secret_key, pairing_t * pairing)
{
  GenerateSecretKey (secret_key, pairing);
}

int
main (int argc, char **argv)
{
  pairing_t p;

  element_t secret_key, public_key, g;
  element_t x;
  if (argc != 4)
    {
      fprintf (stderr,
	       "usage: %s sk g pk\nThe program will create a secret-key in the file sk and public-key in the file pk. The file g is the file output by the program compute_generator\n",
	       argv[0]);
      exit (1);
    }
  element_init_Zr (x, p);
  InputSecretKey (&secret_key, &p);
  if (write_element_to_file (secret_key, argv[1]) != 0)
    {
      fprintf (stderr, "error in creating or writing to file %s\n", argv[1]);
      exit (1);
    }
  if (read_element_from_file (g, argv[2]) != 0)
    {
      fprintf (stderr, "error in opening or reading from file %s\n", argv[2]);
      exit (1);
    }
  ComputePublicKey (&public_key, &g, &secret_key, &p);
  mpz_init (public_key[0].z);
  if (write_element_to_file (public_key, argv[3]) != 0)
    {
      fprintf (stderr, "error in creating or writing to file %s\n", argv[3]);
      exit (1);
    }

//element_dlog_brute_force (x, g, public_key);
//printf("%d\n",mpz_get_ui(x[0].z));
  return 0;
}
