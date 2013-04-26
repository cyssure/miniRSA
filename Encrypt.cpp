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

int Encrypt::encrypt_char(int msg, long e, long c) {
  MiniRSA rsa;
  return rsa.modulo(msg, e, c);
}

int Encrypt::decrypt_char(int msg) {
  MiniRSA rsa;
  int res = rsa.modulo(msg, D, C);
  cout << (char) res << " " << res;
  return res;
}

int * Encrypt::encrypt_sen(string msg) {
  int i;
  int * result = new int[msg.size()];
  for (i = 0; i < msg.size(); i++) {
    result[i] = encrypt_char((int)msg.at(i), E, C);
    cout << result[i] << endl;
  }
  return result;
  
}

void Encrypt::set_public(long e, long c) {
  E = e;
  C = c;
}

void Encrypt::set_private(long d, long c) {
  D = d;
  C = c;
}
int main(int argc, char** argv) {
  cout << "Please enter the public key (e, c): first e, then c" << endl;
  string line;
  getline(cin, line);
  int p1 = line.find(" ", 0);
  if (p1 == string::npos) {
    cout << "Please enter a white space between two numbers" << endl;
    exit(0);
  }

  int first_num = atoi(line.substr(0, p1).c_str());
  int second_num = atoi(line.substr(p1 + 1, string::npos).c_str());
  Encrypt encrypt;
  encrypt.set_public(first_num, second_num);
  cout << "Please enter a sentence to encrypt" << endl;
  getline(cin, line);
  encrypt.encrypt_sen(line);
  cout << "Please enter the private key (d, c): first d, then c" << endl;
  getline(cin, line);

  p1 = line.find(" ", 0);
  if (p1 == string::npos) {
    cout << "Please enter a white space between two numbers" << endl;
    exit(0);
  }
  first_num = atoi(line.substr(0, p1).c_str());
  second_num = atoi(line.substr(p1 + 1, string::npos).c_str());
  encrypt.set_private(first_num, second_num);
  while (1) {
    cout << "Enter next char cipher value as an int, type quit to quit" << endl;
    getline(cin, line);
    if (strcmp(line.c_str(), "quit") == 0) { 
      break;
    }
    encrypt.decrypt_char(atoi(line.c_str()));
  }
}
