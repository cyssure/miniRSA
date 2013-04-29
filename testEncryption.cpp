#include "miniRSA.h"
#include "Encryption.h"
#include "Encrypt.h"

using namespace std;

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
