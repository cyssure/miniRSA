#include "miniRSA.h"

using namespace std;

MiniRSA::MiniRSA() {}

MiniRSA::~MiniRSA() {}

/**
 *Generate a coprime smaller than x
 */
long MiniRSA::coprime(long x) {
  long num;
  while (1) {
    num = rand() % x + 1;
    if (GCD(num, x) == 1) {
      return num;
    }
  }
}

/**
 *Get the greatest common devider of two numbers
 */
long MiniRSA::GCD(long a, long b) {
  long x, y;
  x = (a > b) ? a : b;
  y = (a > b) ? b : a;
  long div, mod;
  while (1) {
    //div = x / y;
    mod = x % y;
    if (mod == 0) return y;
    x = y;
    y = mod;
  }
}

/**
 *Compute the totient of a number
 */
long MiniRSA::totient(long n) {
  long i;
  long tot = 0;
  for (i = 1; i < n; i++) {
    if (GCD(i, n) == 1) tot++; 
  }
  return tot;
}

/**
 *Compute moduler inversion of a given base and number
 */
long MiniRSA::mod_inverse(long base, long m) {
  long exp = totient(m) - 1;
  return modulo(base, exp, m);

}

/**
 *Compute modulo
 */
long MiniRSA::modulo(long a, long b, long c) {
  long i;
  long now = 1;
  for (i = 1; i <= b; i++) {
    now *= a;
    now = now % c;
  }
  return now;
}

/**
 *Encrypt or decrypt of an integer
 */
long MiniRSA::endecrypt(int msg_or_cipher, long key, long c) {
  return modulo(msg_or_cipher, key, c);
}
