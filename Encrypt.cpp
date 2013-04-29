/**
 *This is the class encrypt messages
 *Author: Yushu Cao, April 28, 2013
 */
#include "miniRSA.h"
#include "Encrypt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

Encrypt::Encrypt() {}

Encrypt::Encrypt(long e, long c) {
  E = e;
  C = c;
}

/**
 *Generate keys according the given numbers, which refer to the nth prime
 */
int Encrypt::generate_keys(int x, int y) {
  int first = x;
  int second = y;
  encryption.generate_primes(&first, &second);

  C = encryption.compute_c(first, second);
  M = encryption.compute_m(first, second);
  E = encryption.compute_key(M);
  D = encryption.compute_d(E, M);

  return 0;
}

/**
 *Encrypt a char
 */
int Encrypt::encrypt_char(int msg) {
  return rsa.modulo(msg, E, C);
}

/**
 *Encrypt a char with additional given keys
 */
int Encrypt::encrypt_char(int msg, long e, long c) {
  MiniRSA rsa;
  return rsa.modulo(msg, e, c);
}

/**
 *Decrypt a char
 */
int Encrypt::decrypt_char(int msg) {
  MiniRSA rsa;
  int res = rsa.modulo(msg, D, C);
  return res;
}

/**
 *Encrypt a sentence
 */
int * Encrypt::encrypt_sen(string msg) {
  int i;
  int * result = new int[msg.size()];
  for (i = 0; i < msg.size(); i++) {
    result[i] = encrypt_char((int)msg.at(i), E, C);
    cout << result[i] << endl;
  }
  return result;
  
}

/**
 *Set the public key
 */
void Encrypt::set_public(long e, long c) {
  E = e;
  C = c;
}

/**
 *Set the private key
 */
void Encrypt::set_private(long d, long c) {
  D = d;
  C = c;
}

/**
 *Get the public key
 */
int * Encrypt::get_public() {
  int * publics = new int[2];
  publics[0] = E;
  publics[1] = C;
  return publics;
}
 
