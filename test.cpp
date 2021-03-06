#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "miniRSA.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc <= 1) cout << "usage illegal" << endl;
  MiniRSA testRSA;
  if (argc == 2) {
    int find_cop = atoi(argv[1]);
    cout << "Coprime num of " << find_cop << "is " << testRSA.coprime(find_cop) << endl;
    cout << "Euler totient is " << testRSA.totient(find_cop) << endl;
  }
  if (argc == 3) {
    cout << "GCD of them is " << testRSA.GCD(atoi(argv[1]), atoi(argv[2])) << endl;
    cout << "mod inverse is " << testRSA.mod_inverse(atoi(argv[1]), atoi(argv[2])) << endl;
    //    cout << "Encrypt is " << testRSA.endecrypt()
  }

  
}
