#ifndef __ENCRYPT_H
#define __ENCRYPT_H

#include <iostream>

using namespace std;

class Encrypt {
 public:
  Encrypt();
  Encrypt(long e, long c);
  void set_public(long e, long c);
  void set_private(long d, long c);
  int encrypt_char(int msg, long e, long c);
  int decrypt_char(int msg);
  int * encrypt_sen(string msg);
  
 private:
  long E;
  long C;
  long D;
  
  
};
#endif
