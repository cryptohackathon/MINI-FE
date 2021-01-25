// MINI-FE.
//
// Vincenzo Iovino
#include "pairings.h"
#include "orproofs.h"
#include "nife.h"
#include <stdio.h>


int
main (int argc, char **argv)
{
  pairing_t p;

  element_t g;
  if (argc != 2)
    {
      fprintf (stderr,
	       "usage: %s file\nThe program will create a group element and write it into file\n",
	       argv[0]);
      exit (1);
    }
  ComputeGenerator (&g, &p);
  if (write_element_to_file (g, argv[1]) != 0)
    {
      fprintf (stderr, "error in creating or writing to file %s\n", argv[1]);
      exit (1);
    }
  return 0;
}
