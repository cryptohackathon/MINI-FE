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

