# Multi-Inputs Non-Interactive Functional Encryption (MINI-FE) without Trusted Authorities and Applications to Secure Grading
## Overview
The goal is to extend the [CiFEr][cifer] library with new functionalities that implement a sort of multi-input FE without trusted parties. The setting is the following. 
There are N participants, and each of them generates a pair of public and secret-keys. Each participant knows the public-keys of all other participants. 
The i-th participant can encode an input Xi with her/his own secret-key and the public-keys of the others to generate a ciphertext CTi. There is a public Eval function that takes all ciphertexts and compute f(x1,...,xn) where f is a given function (we explain below the functions we achieve).

[cifer]: https://github.com/fentec-project/CiFEr

## Applications to Secure Grading 
We show the practicality  of this approach by presenting a demo for an application of MINI-FE to an online Hackaton.
In an online Hackaton there are N Judges who have to assign a grade to each Hackaton's candidate. 
Each judge can cast his/her grade, where the grade is an integer from 0 to 2 (our system can be easily extended to allow larger ranges), with 0 being reject, 1 borderline, 2 accept.  
From each Judge's ciphertext the average grade for the candidate can be computed. Each ciphertext individually does not reveal the grade of the judge so the judge's privacy is preserved.
The demo also allows to perform Decisions by Unanimity and "Dead or Alive" Decisions. In the first case the candidate's project gets accepted if all Judges cast YES and in the latter case the project gets accepted if at least one Judges cast YES.
### Verifiability
The MINI-FE schemes we implement are verifiable. That is, the system can detect if a Judge casts an invalid grade/decision. To achieve this, we also implement NIZK range proofs.
### Security
Our MINI-FE systems are provably secure from the [Decision Linear Assumption][dlin] over bilinear groups.

[dlin]: http://crypto.stanford.edu/~dabo/papers/groupsigs.pdf
## Compatibility with Stanford's pbc library
In achieving our new functionality and demo we also implement an abstraction of pairing-based cryptography (PBC). Our abstraction allows to construct PBC systems in a way that makes them compatible both with the CiFEr library and the Stanford [pbc] library.

[pbc]: https://crypto.stanford.edu/pbc/

In particular the programmer can implement PBC systems using the same set of APIS from the Stanford's pbc library and simply setting a flag the code can be linked either to the Stanford's pbc or to the CiFEr library. So all applications of Stanford's pbc become compatible with CiFEr as well.

# Installation
```bash
chmod u+x install.sh
./install.sh
```
It will ask you whether you want to additionally install the version of our library and demo for the Stanford's pbc library. This is not required. If you choose to do that it will ask you for the path to the pbc's header files in your system. 

It will create the files:
* `minife.so` containing the MINI-FE library. 
* `demo_cifer` A demo of a secure grading system linked to the CiFEr library.
* `demo_pbc` Same as before but linked to the Stanford's pbc library.
* `testpairings_cifer` A program to perform some tests with pairing functions inked to the CiFEr library. The output of the program should be equal to the content of file output_test in the main directory.
* `testpairings_pbc` Same as before but linked to the Stanford's pbc library.
# Library's APIs
The library's APIs can be divided in routines for pairing computations and routines to implement MINI-FE systems.
The pairing APIs are a subset of the Stanford's pbc and are better described by the following example program `test.c`.
```bash
#include <stdio.h>
#include "pairings.h"
int main(void){
element_t a,b,a2,b2,y,T,T4,_T4; // all elements are of type element_t
pairing_t p; // declare a pairing instance
pairing_init_set_str(p,Param); // Param is a static global constant
element_init_G1(a,p); // a is an element of G1 - the element is associated to the pairing instance p
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
Assuming `libminife.so` has been installed (e.g. in `/usr/local/lib`), the program can be compiled as:

```bash
gcc -o test test.c -lgmp -lcifer -lminife -I ./include/
```
It can be also compiled with the Stanford's PBC library (assuming the pbc's header are in `/usr/include/pbc`) as follows: 
```bash
gcc -o test test.c src/pairings.c -lgmp -lpbc -I ./include/ -I /usr/include/pbc -DPBC_OR_CIFER=0
```
