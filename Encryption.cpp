#include "miniRSA.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

int isPrime(int n) {
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

int generate_primes(int* nth, int *mth) {
  int first, second;
  first = *nth < *mth ? *nth : *mth;
  second = *nth < *mth ? *mth : *nth;
  cout << first << "th prime = ";
  int i = 0;
  int temp = 1;
  long result = 0;
  MiniRSA rsa;
  
  while (i <= first) {
    if (isPrime(temp) == 1) {
      //      cout << "Find a prime " << temp << endl;
      i++;
    }
    temp++;
  }
  first = temp - 1;
  cout << first << ", ";
  cout << second << "th prime = ";
  //  temp++;
  while (i <= second) {
    if (isPrime(temp) == 1) {
      //      cout << "Find a prime " << temp << endl;
      i++;
    }
    temp++;
  }
  second = temp - 1;
  cout << second << ", ";
  //  cout << "Two prime numbers: " << first << ", " << second << endl;
  *nth = first;
  *mth = second;
  return 0;
}

long compute_c(int a, int b) {
  return a * b;
}

long compute_m(int a, int b) {
  return (a - 1) * (b - 1);
}

long compute_key(long m) {
  MiniRSA rsa;
  return rsa.coprime(m);
}

long compute_d(long e, long m) {
  MiniRSA rsa;
  return rsa.mod_inverse(e, m);
}
/**
int main(int argc, char** argv) {
  cout << "Enter the nth prime and the mth prime to compute" << endl;
  string line;
  getline(cin, line);
  int p1 = line.find(" ", 0);
  if (p1 == string::npos) {
    cout << "Please enter a white space between two numbers" << endl;
    exit(0);
  }
  int first_num = atoi(line.substr(0, p1).c_str());
  int second_num = atoi(line.substr(p1 + 1, string::npos).c_str());
  generate_primes(&first_num, &second_num);
  long c = compute_c(first_num, second_num);
  cout << "c = " << c << ", ";
  long m = compute_m(first_num, second_num);
  cout << "m = " << m  << ", ";
  long e = compute_key(m);
  cout << "e = " << e << ", ";
  long d = compute_d(e, m);
  cout << "d = " << d << ", ";
  cout << "Public Key = (" << e << ", " << c << "), ";
  cout << "Private Key = (" << d << ", " << c << ")" << endl;
}
**/
