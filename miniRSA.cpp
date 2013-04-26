#include "miniRSA.h"

using namespace std;

MiniRSA::MiniRSA() {}

MiniRSA::~MiniRSA() {}


long MiniRSA::coprime(long x) {
  //  srand(time(NULL));
  long num;
  while (1) {
    num = rand() % x + 1;
    if (GCD(num, x) == 1) {
      return num;
    }
  }
}

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

long MiniRSA::totient(long n) {
  long i;
  long tot = 0;
  for (i = 1; i < n; i++) {
    if (GCD(i, n) == 1) tot++; 
  }
  cout << "totient is " << tot << endl;
  return tot;
}
/**
long MiniRSA::mod_inverse(long base, long m) {
  long exp = totient(m);
  return ((long long)pow((long long)base, (long long)exp - 1)) % m;
}
**/

long MiniRSA::mod_inverse(long base, long m) {
  long exp = totient(m) - 1;
  return modulo(base, exp, m);

}

long MiniRSA::modulo(long a, long b, long c) {
  long i;
  long now = 1;
  for (i = 1; i <= b; i++) {
    now *= a;
    now = now % c;
  }
  return now;
}

long MiniRSA::endecrypt(int msg_or_cipher, long key, long c) {
  return modulo(msg_or_cipher, key, c);
}