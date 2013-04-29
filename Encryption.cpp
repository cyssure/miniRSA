/**
 *This class realizes the encryption process
 *Author: Yushu Cao, April 28, 2013
 */
#include "miniRSA.h"
#include "Encryption.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

Encryption::Encryption() {

}

Encryption::~Encryption() {

}

/**
 *To see whether a number is prime
 */
int Encryption::isPrime(int n) {
  if (n == 1) return 0;
  if (n == 2 || n == 3) return 1;
  if (n % 2 == 0) return 0;
  int end = (int) (sqrt(n) + 0.5);
  int i;
  for (i = 3; i <= end; i += 2) {
    if (n % i == 0) return 0;
  }
  return 1;
}


/**
 *Generate the nth and the mth primes
 */
int Encryption::generate_primes(int* nth, int *mth) {
  int first, second;
  first = *nth < *mth ? *nth : *mth;
  second = *nth < *mth ? *mth : *nth;
  int i = 0;
  int temp = 1;
  long result = 0;
  MiniRSA rsa;
  
  while (i <= first) {
    if (isPrime(temp) == 1) {
      i++;
    }
    temp++;
  }
  first = temp - 1;

  while (i <= second) {
    if (isPrime(temp) == 1) {
      i++;
    }
    temp++;
  }
  second = temp - 1;

  *nth = first;
  *mth = second;
  return 0;
}

/**
 *Compute C
 */
long Encryption::compute_c(int a, int b) {
  return a * b;
}

/**
 *Compute M
 */
long Encryption::compute_m(int a, int b) {
  return (a - 1) * (b - 1);
}

/**
 *Generate a key
 */
long Encryption::compute_key(long m) {
  MiniRSA rsa;
  return rsa.coprime(m);
}

/**
 *Compute D
 */
long Encryption::compute_d(long e, long m) {
  MiniRSA rsa;
  return rsa.mod_inverse(e, m);
}

