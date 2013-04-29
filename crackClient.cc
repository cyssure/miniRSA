/**
 *This is the program for a client program who can crack server's key.
 *Author: Yushu Cao, April 28, 2013
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <netdb.h>
#include "miniRSA.h"
#include <time.h>
#include "Encrypt.h"
#include <iostream>


using namespace std;

#define MAXLINE 1024

Encrypt c_encrypt;
Encrypt c_decrypt;
Encrypt crack;


/**
 *Decrypt with the server's private key
 */
void crack_key(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, MAXLINE);
  char temp[33];
  int de_value;
  char * ptr;
  ptr = strtok(input, " ");
  while (ptr != NULL) {
    de_value = crack.decrypt_char(atoi(ptr));//decrypt an integer
    sprintf(temp, "%c", de_value);
    strcat(buf, temp);
    ptr = strtok(NULL, " ");
  }
  cout << buf;
  fflush(stdout);
}


/**
 *Read from the socket connecting to the server
 */
void* readFrom2(void * arg) {
  int connfd = *(int*)arg;
  int n;
  char input[MAXLINE];
  char buf[MAXLINE];
  memset(input, 0, MAXLINE);
  while (1) {
    n = read(connfd, input, MAXLINE);
    if (n < 0) break;
    crack_key(input, buf);
    if (strcmp(buf, ".bye\n") == 0) break;
    memset(input, 0, n);
  }
  close(connfd);
  exit(0);
}

/**
 *Try to crack m
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
 *Crack the key by brute force
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
 *Parse out the keys sent from the server
 */
int parse_keys(char input[]) {
  string input_str(input);//convert the char array into a C++ string
  int p1 = input_str.find(" ", 0);
  if (p1 == string::npos) {
    return -1;
  }
  long e = atoi(input_str.substr(0, p1).c_str());//convert to long int         
  long c = atoi(input_str.substr(p1 + 1, string::npos).c_str());
  crack.set_public(e, c);
  long d = bruteforce(e, c);//get the private key
  crack.set_private(d, c);
  return 0;
}


/**
 *Read the public keys sent from the server
 */
void* read_public(void * arg) {
  int connfd = *(int *)arg;
  int n;
  char input[MAXLINE];

  memset(input, 0, MAXLINE);
  n = read(connfd, input, MAXLINE);
  cout << "Read in " << input << endl;
  if (parse_keys(input) == -1) {
    cout << "Error when parsing public keys" << endl;
    exit(1);
  }
  pthread_exit(0);
}


/**
 *Send public key to the server
 */
void * send_public (void * arg) {
  int connfd = *(int *) arg;

  char input[MAXLINE];
  int * publics = c_decrypt.get_public();
  char temp[33];

  sprintf(input, "%d ", publics[0]);//copy E into the temp buffer              
  sprintf(temp, "%d", publics[1]);//copy C into the temp buffer                
  strcat(input, temp);

  cout << "My public keys are " << input << endl;
  write(connfd, input, strlen(input));//send public keys to the client
  cout << "Send successfully" << endl;
  pthread_exit(0);

}

/**
 *Encrypt the sentence entered from the command line with my own public key
 */
void en_sentence(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, sizeof(buf));
  char temp[33];

  int i;
  for (i = 0; i < msg_length; i++) {
    int en_value = c_decrypt.encrypt_char(input[i]);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d ", en_value);
    strcat(buf, temp);
  }

  printf("Encrypted TO: %s\n", buf);
}

/**
 *This is where the program starts from
 */
int main (int argc, char** argv) {
  if (argc < 3) {
    printf("usage: ./client <http_server_addr(localhost)> <port_num>\n");
    exit(0);
  }
  char* server_add = argv[1];
  int server_port = atoi(argv[2]);
  if (server_port <= 0) {
    printf("usage: the port number is illegal\n");
    exit(0);
  }



  int sockfd = 0, n = 0;
  struct sockaddr_in serv_addr;

  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);

  int rv;
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (inet_pton(AF_INET, server_add, &serv_addr.sin_addr) <= 0) {
    //The host is not IP address, instead it is a host name                    
    //Do DNS then                                                              
    if ((rv = getaddrinfo(server_add, argv[2], &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      exit(1);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        continue;
      }
      if (connect (sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        perror("connect");
	continue;
      }
      break;
    }
    if (p == NULL) {
      fprintf(stderr, "failed to connect\n");
      exit(2);
    }
    freeaddrinfo(servinfo);

  } else {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("ERROR: Could not create socket \n");
      return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0\
	) {
      printf("\n Error: Connect Failed\n");
      return 1;
    }

  }

  c_decrypt.generate_keys(rand() % 20 + 5, rand() % 20 + 5);//initialize its own keys

  pthread_t threads[2];
  pthread_create(&threads[0], NULL, read_public, (void *)&sockfd);
  pthread_create(&threads[1], NULL, send_public, (void *)&sockfd);
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);//wait for the public key transfers finished  

  pthread_t thread;
  pthread_create(&thread, NULL, readFrom2, (void*)&sockfd);


  char input[MAXLINE];
  memset(input, 0, MAXLINE);
  char buf[MAXLINE];
  while (1) {
    fgets(input, MAXLINE, stdin);
    en_sentence(input, buf);
    write(sockfd, buf, strlen(buf));
    if (strcmp(input, ".bye\n") == 0) break;

  }

  close(sockfd);

  fflush(stdout);
  exit(0);
} 
