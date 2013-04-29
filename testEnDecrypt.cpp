/**
 *This program demos the process of encrypt and decrypt
 *Author: Yushu, April 28, 2013
 */
#include "miniRSA.h"
#include "Encrypt.h"
#include "Encryption.h"
#include <string.h>
#include <iostream>

using namespace std;

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
    cout << "Enter next char cipher value as an int, type quit to quit" << endl\
      ;                                                                               
    getline(cin, line);                                                         
    if (strcmp(line.c_str(), "quit") == 0) {                                    
      break;                                                                    
    }                                                                           
    int value = encrypt.decrypt_char(atoi(line.c_str()));                                   
    cout << (char) value << " " << value << endl;
  }                           

}
