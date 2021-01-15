#include "pairings.h"
#include "orproofs.h"

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

void EncodeGrade (element_t * g, pairing_pp_t * pp, pairing_t * pairing,
	   element_t * hash, element_t * secret_key, const long int *grade,
	   element_t * Y, element_t * CT, ChaumPedersenProof Proof[3]);
int
EvalTallyGrade (const int N, pairing_t * pairing, element_t * g,
		element_t * hash, element_t pk[], element_t Y[],
		element_t CT[], element_t * Res,
		ChaumPedersenProof Proofs[][3]);
void
EncodeDeadOrAlive (element_t * g, pairing_pp_t * pp,
		 pairing_t * pairing, element_t * hash,
		 element_t * secret_key, const long int *decision, element_t * Y,
		 element_t * Res);
void
EvalTallyDeadOrAlive (const int N, pairing_t * pairing, element_t * g,
		      element_t * hash, element_t CT[], int *res);
void
EncodeUnanimity (element_t * g, pairing_pp_t * pp,
	       pairing_t * pairing, element_t * hash, element_t * secret_key,
	       const long int *decision, element_t * Y, element_t * Res);
void
EvalTallyUnanimity (const int N, pairing_t * pairing, element_t * g,
		    element_t * hash, element_t CT[], int *res);

void GenerateSecretKeyFromInt(element_t *secret_key,int pin);
void ComputeY(element_t *Y,int N,int i,element_t public_key[],pairing_t *p);
void ComputePublicKey(element_t *public_key,element_t *g, element_t *secret_key);
