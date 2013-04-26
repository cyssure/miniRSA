#ifndef __RSA_H
#define __RSA_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cmath>

using namespace std;

class MiniRSA {
public:
  MiniRSA();
  ~MiniRSA();
  
  long coprime(long x);
  long endecrypt(int msg_or_cipher, long key, long c);
  long GCD(long a, long b);
  long mod_inverse(long base, long m);
  long modulo(long a, long b, long c);
  long totient(long n);
};

#endif
