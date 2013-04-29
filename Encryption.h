#ifndef __ENCRYPTION_H
#define __ENCRYPTION_H

#include "miniRSA.h"
#include <iostream>
using namespace std;

class Encryption {
 public:
  Encryption();
  ~Encryption();
  int generate_primes(int * nth, int * mth);
  long compute_c(int a, int b);
  long compute_m(int a, int b);
  long compute_key(long m);
  long compute_d(long e, long m);
  int isPrime(int n);

};

#endif
