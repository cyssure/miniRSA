/**
 *This program demos a process of cracking keys
 *Author: Yushu Cao, April 28, 2013
 */
#include "Encrypt.h"
#include "Encryption.h"
#include "miniRSA.h"
#include <string>
#include <string.h>

using namespace std;

/**
 *Look for an appropriate m 
 */
long try_m(int a, int b, long e) {
  long m = (a - 1) * (b - 1);
  MiniRSA rsa;
  Encryption en;
  if (rsa.GCD(m, e) == 1) {//e and m should be coprime
    long d = rsa.mod_inverse(e, m);
    cout << "a is " << a << ", b is " << b << endl;
    cout << "The totient is " << rsa.totient(m) << endl;
    return d;
    
  } else {
    return -1;
  }
  
}

/**
 *Crack the keys by bruteforce
 */
long bruteforce(long e, long c) {
  Encryption en;
  int i = 2;
  for (i = 2; i < c; i++) {
    if (c % i == 0) {
      if (en.isPrime(i) == 1 && en.isPrime(c / i) == 1) {
	int assume_a = i;
	int assume_b = c / i;
	long d = try_m(assume_a, assume_b, e);
	if (d != -1) {
	  return d;
	}
      }
    }
  }
  return -1;
}


/**
 *This is where the program starts to run
 */
int main(int argc, char** argv) {
  cout << "Enter the public key value" << endl;
  string input;
  getline(cin, input);
  int p1 = input.find(" ", 0);
  if (p1 == string::npos) {
    cout << "Please enter a whitespace between two numbers" << endl;
    exit(0);
  }
  long e = atoi(input.substr(0, p1).c_str());
  int c = atoi(input.substr(p1 + 1, string::npos).c_str());
  long d = bruteforce(e, c);
  MiniRSA rsa;
  //  MiniRSA rsa;
  //cout << "The totient is " << 
  cout << "D is " << d << endl;
  int decrypt;
  while (1) {
    cout << "Enter a letter to encrypt/decrypt, or quit to exit" << endl;
    getline(cin, input);
    if (strcmp(input.c_str(), "quit") == 0) break;
    decrypt = rsa.modulo(atoi(input.c_str()), d, c);
    cout << "The number decrypted to " << decrypt << endl;
    cout << "It is " << (char) decrypt << endl;

  }
  cout << "Done" << endl;
  exit(0);
}
