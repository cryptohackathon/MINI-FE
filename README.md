# Multi-Inputs Non-Interactive Functional Encryption (MINI-FE) without Trusted Authorities and Applications to Private Grading
## Overview
The goal is to extend the [CiFEr][cifer] library with new functionalities that implement a sort of multi-input FE without trusted parties. The setting is the following. 

* There are N participants, and each of them generates a pair of public- and secret-keys. Each participant knows the public-keys of all other participants. 

* The i-th participant can encode an input Xi with her/his own secret-key and the public-keys of the others to generate a ciphertext CTi. 

* There is a public Eval function that takes all ciphertexts and compute f(x1,...,xN) where f is a given function (we explain below the functions we achieve).

[cifer]: https://github.com/fentec-project/CiFEr

### Applications to Private Grading 
We show the practicality of our systems by presenting a demo application to online Hackatons.
In an online Hackaton there are N Judges who have to assign a grade to each Hackaton's candidate. 
Each judge encodes his/her grade for each candidate in a MINI-FE ciphertext, where the grade is an integer from 0 to 2 (our system can be easily extended to allow larger ranges), with 0 being reject, 1 borderline, 2 accept.

From all ciphertexts the average grade for each candidate can be computed. Each ciphertext individually does not reveal the grade the Judge assigned to a candidate so the Judge's privacy is preserved.

The demo also allows to perform Decisions by Unanimity and "Dead or Alive" (or Accept/Reject) Decisions. In the first case the candidate's project gets accepted if all Judges cast YES and in the latter case the project gets accepted if at least one Judges cast YES.
### Verifiability
The MINI-FE schemes we implement are verifiable. That is, the system can detect if a Judge casts an invalid grade/decision. To achieve this, we also implement NIZK range proofs.
### Security
Our MINI-FE systems are provably secure from the [Bilinear Decision Diffie-Hellman Assumption][bddh] over bilinear groups.

[bddh]: http://cyber.biu.ac.il/wp-content/uploads/2017/01/Boneh-basics-of-pairings-4.pdf

### Compatibility with Stanford's pbc library
In achieving our new functionality and demo we also implement an abstraction of pairing-based cryptography (PBC). Our abstraction allows to construct PBC systems in a way that makes them compatible both with the CiFEr library and the Stanford [pbc] library.

[pbc]: https://crypto.stanford.edu/pbc/

In particular the programmer can implement PBC systems using the same set of APIs from the Stanford's pbc library and simply setting a flag the code can be linked either to the Stanford's pbc or to the CiFEr library. So all applications of Stanford's pbc become compatible with CiFEr as well.

## Installation
```bash
chmod u+x install.sh
./install.sh
```
It will ask you whether you want to additionally install the version of our library and demo for the Stanford's pbc library. This is not required. If you choose to do that it will ask you for the path to the pbc's header files in your system. 

It will create the files:
* `minife.so` containing the MINI-FE library. 
* A set of command-line programs to run private grading ceremonies. 
* `demo_cifer` A demo of a secure grading system linked to the CiFEr library.
* `demo_pbc` Same as before but linked to the Stanford's pbc library.
* `testpairings_cifer` A program to perform some tests with pairing functions inked to the CiFEr library. The output of the program should be equal to the content of file output_test in the main directory.
* `testpairings_pbc` Same as before but linked to the Stanford's pbc library.
## APIs
The APIs can be divided in routines for pairing computations and routines to implement MINI-FE systems.
The pairing APIs are a subset of the Stanford's pbc and are better described by the following example program `test.c`.
### Pairing APIs
```C
#include <stdio.h>
#include "pairings.h"
int main(void){
element_t a,b,a2,b2,y,T,T4,_T4; // all elements are of type element_t
pairing_t p; // pairing instance
pairing_init_set_str(p,Param); // Param is a static global constant
element_init_G1(a,p); // a is an element of G1 - all the following elements are associated to the pairing instance p
element_init_G1(a2,p); 
element_init_G2(b,p); // b is an element of G2
element_init_G2(b2,p);
element_random(a); // choose a random a in the group where a has been initialized 
element_random(b); // same for b
element_mul(a2,a,a); // a2=a^2
element_mul(b2,b,b); // b2=b^2
element_init_GT(T,p); // T belongs to the target group
element_init_GT(T4,p);
element_init_GT(_T4,p);
element_pairing(T,a,b); //T=e(a,b)
element_pairing(T4,a2,b2); //T4=e(a2,b2)=e(a^2,b^2)=e(a,b)^4=T^4
element_init_Zr(y,p); // y belongs to Zr with r order of the groups
element_set1(y); // y=1
element_add(y,y,y); // y=y+y=2
element_add(y,y,y);// y=y+y+=4
element_pow_zn(_T4,T,y); // _T4=T^4
printf("%d\n",element_cmp(T4,_T4)); // test if T4 == _T4 - should output 0
return 0;
}
```
Assuming `libminife.so` has been installed in one of the system paths (e.g. in `/usr/local/lib`), the program can be compiled as:

```bash
gcc -o test test.c -lgmp -lcifer -lminife -I ./include/
```
It can be also compiled with the Stanford's PBC library (assuming the pbc's header are in `/usr/include/pbc`) as follows: 
```bash
gcc -o test test.c src/pairings.c -lgmp -lpbc -I ./include/ -I /usr/include/pbc -DPBC_OR_CIFER=0
```

Since the pairing routines are a subset of the Stanford's pbc library, one can find the documentation of each routine in `include/pairings.h` in the pbc [manual][pbcmanual]

[pbcmanual]: https://crypto.stanford.edu/pbc/manual/

Moreover, existing programs designed for the Stanford's pbc library can be compiled with the minife library if they use pbc's routines also present in minife.


### MINI-FE APIs
The following two functions generate resp. a random secret-key and a secret-key from an integer (e.g., a numerical pin) in the obvious way:
```C
void GenerateSecretKey(element_t *secret_key,pairing_t *p);
void GenerateSecretKeyFromInt(element_t *secret_key,int pin,pairing_t *p);
```

The following function
```C
void ComputeGenerator(element_t *g, pairing_t *p);
```
computes a public generator ``g`` that will be used by all parties to compute public-keys and by other routines. It is assumed that the same pairing instance ``p`` is used by all parties.

The following function
```C
void ComputePublicKey(element_t *public_key,element_t *g, element_t *secret_key, pairing_t *p);
```
computes a public-key ``public_key`` from the secret-key ``secret_key`` and from a public generator ``g`` and from a pairing instance ``p`` with which all elements will be initialized.

The following function
```C
void ComputeY(element_t *Y,int N,int i,element_t public_key[],pairing_t *p);
```
can be executed as in the following code
```C
    ComputeY(&Y[i-1],N,i,public_key,&pairing);
```
by the i-th participant to generate a value ``Y`` that depends on the public-keys of all other participants. The value ``public_key`` is in the previous code is indeed an array of the N public-keys of all participants. 
The value ``pairing`` is the pairing instance with which all group elements are generated.


The following function
```C
void EncodeGrade (element_t * g, pairing_pp_t * pp, pairing_t * pairing,
	   element_t * hash, element_t * secret_key, const long int *grade,
	   element_t * Y, element_t * CT, ChaumPedersenProof Proof[3]);
```
computes a ciphertext ``CT`` from the grade ``grade``, the secret-key and the so computed value ``Y`` that depends on all other public-keys of other participants. 

In addition, it computes a NIZK proof that can be used to verify the ciphertext.

The value ``pairing`` is the pairing instance with which all group elements are generated. 

The element ``hash`` is a value that is supposed to depend on the grade ceremony. For security to hold it has to be different in each ceremony and can be chosen as the hash of a fixed string (e.g., the identifier of the ceremony).

The following function can be used to compute such ``hash`` value. It takes as input the ``hash`` element (that must not be initialized before), a null-terminated string denoting the identifier of the election, a pairing instance, and sets ``hash`` to be an element that can be used by the previous and following procedures.
```C
void ComputeId(element_t *hash,char *str,pairing_t p);
```

The function
```C
int
EvalTallyGrade (const int N, pairing_t * pairing, element_t * g,
		element_t * hash, element_t pk[], element_t Y[],
		element_t CT[], element_t * Res,
		ChaumPedersenProof Proofs[][3]);
```
takes as input the number of participants, the pairing instance, the generator ``g`` used by all participants, the hash ``hash` explained before, the public-key array ``pk``, the ciphertext array ``CT and the array ``Y`` of all participants and compute as follow. It returns ``true`` iff the proofs are verified. Moreover it sets the result of the grading in the element pointed by ``Res``.

The header file ``nife.h`` exposes all other similar routines for the functionalities 'Dead or Alive' and 'Unanimity'. In these case, there is no need for NIZK proofs and verifications since, by design of the systems, there are no invalid ciphertexts (except for ciphertexts not representing valid group elements).
## Command-line utilities
The programs ``compute_generator``, `generate_keys``, ``generate_keys_from_pin``,``submitgrade`` and ``evaltally`` can be used to run private grading ceremonies from command-line. Run the programs without arguments to know the usage.
## Demo
The demo ``demo_cifer`` (and ``demo_pbc`` as well in the case you also installed the Stanford pbc's version)
can be executed just with the command:
```bash
./demo_cifer
```
 The demo is supposed to simulate locally an evaluation ceremony for the candidates/projects of the Crypto Hackaton.
* The demo starts by asking you how many Judges there are.
* For each candidate of the Hackaton, the demo starts his/her evaluation.
	* For each Judge, it asks a secret pin from which the secret-key is computed.
	* It asks which kind of ceremony you want to do - average grading, dead or alive, or unanimity.
	* For each Judge it asks the secret pin and a grade/decision to assign to the candidate - a number from 0 to 2 for average grade ceremonies, and a bit for the other types.
	* The evaluation of the candidate is computed along with the verification of the NIZK proofs. The verification step is done only for average grade elections and  in the case if a Judge input an invalid grade the invalid ciphertext is detected and ceremony aborts. 
