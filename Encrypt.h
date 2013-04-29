#ifndef __ENCRYPT_H
#define __ENCRYPT_H

#include <iostream>
#include "Encryption.h"

using namespace std;

class Encrypt {
 public:
  Encrypt();
  Encrypt(long e, long c);
  void set_public(long e, long c);
  void set_private(long d, long c);
  int encrypt_char(int msg);
  int encrypt_char(int msg, long e, long c);
  int decrypt_char(int msg);
  int * encrypt_sen(string msg);
  int generate_keys(int x, int y);
  int * get_public();
  
 private:
  long E;
  long C;
  long D;
  long M;
  MiniRSA rsa;
  Encryption encryption;
};
#endif
