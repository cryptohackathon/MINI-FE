#ifndef ORPROOFS_HEADER
#define ORPROOFS_HEADER
#define DEBUG_PROOFS 1
#ifndef RANGE_OF_GRADING
#define RANGE_OF_GRADING 3
#endif
typedef struct
{
  element_t a, b, e, z;		/* a=g^r, b=h^r, z=r+H(a,b)w, where h=g^w; e used in CDS's OR proofs */
} ChaumPedersenProof;

void
CDS3Prover (pairing_t * pairing, element_t * g, element_t * h, element_t * u,
	    element_t * v, element_t * w, int vote,
	    ChaumPedersenProof res[3]);
int CDS3Verifier (pairing_t * pairing, element_t * g, element_t * h,
		  element_t * u, element_t * v, ChaumPedersenProof res[3]);
//extern void
//ChaumPedersenSimulator (pairing_t * pairing, element_t * g, element_t * h,
//                      element_t * u, element_t * v,
//                      ChaumPedersenProof * res);
#endif
