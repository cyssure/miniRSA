#include "miniRSA.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv) {
  string line;
  cout << "Enter the public key value" << endl;
  getline(cin, line);
  int p1 = line.find(" ", 0);
  if (p1 == string::npos) {
    cout << "Please enter a whitespace between two numbers" << endl;
    exit(0);
  }
  string first_num = line.substr(0, p1);
  string first_num = line.substr(p1 + 1, string::npos);
  

}
