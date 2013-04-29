/**
 *This program test the process of encryption
 */
#include "miniRSA.h"
#include "Encryption.h"
#include <iostream>
#include <string>

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
  Encryption en;
  int first_b = first_num;
  int second_b = second_num;
  en.generate_primes(&first_num, &second_num);                                     
  cout << first_b << "th prime = " << first_num << ", " << second_b << "th prime = " << second_num << ", ";
  long c = en.compute_c(first_num, second_num);                                    
  cout << "c = " << c << ", ";                                                  
  long m = en.compute_m(first_num, second_num);                                    
  cout << "m = " << m  << ", ";                                                 
  long e = en.compute_key(m);                                                      
  cout << "e = " << e << ", ";                                                  
  long d = en.compute_d(e, m);                                                     
  cout << "d = " << d << ", ";                                                  
  cout << "Public Key = (" << e << ", " << c << "), ";                          
  cout << "Private Key = (" << d << ", " << c << ")" << endl;                   
}                                                                               


