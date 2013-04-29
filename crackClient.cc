/**
 *This is the program for a HTTP client.
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

//#define HTTP_P 80

using namespace std;

#define MAXLINE 1024

Encrypt c_encrypt;
Encrypt c_decrypt;
Encrypt crack;

int read_line(int fd, char * input) {
  int n;
  char c;
  char * buf = input;
  int read_n;
  for (n = 1; n < MAXLINE; n++) {
    if ((read_n = read(fd, &c, 1)) == 1) {
      *buf++ = c;
      if (c == '\n') break;
    } else if (read_n == 0) {
      if (n == 1) return 0;
      else break;
    } else return -1;
  }
  *buf = 0;
  return n;
}

void de_sentence(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, MAXLINE);
  char temp[33];
  int de_value;
  char * ptr;
  ptr = strtok(input, " ");
  while (ptr != NULL) {
    de_value = c_decrypt.decrypt_char(atoi(ptr));//decrypt an integer
    sprintf(temp, "%c", de_value);
    //    cout << "temp: " << temp ;
    //    cout << temp;
    strcat(buf, temp);
    ptr = strtok(NULL, " ");
  }
  printf("%s", buf);
}

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
    //    cout << "temp: " << temp ;
    //    cout << temp;
    strcat(buf, temp);
    ptr = strtok(NULL, " ");
  }
  //  printf("%s", buf);
  cout << buf;
  fflush(stdout);

  
}

void* readFrom2(void * arg) {
  int connfd = *(int*)arg;
  int n;
  char input[MAXLINE];
  char buf[MAXLINE];
  memset(input, 0, MAXLINE);
  while (1) {
    n = read(connfd, input, MAXLINE);
    if (n < 0) break;
    //    cout << input;
    crack_key(input, buf);
    //    de_sentence(input, buf);
    if (strcmp(buf, ".bye\n") == 0) break;
    memset(input, 0, n);
  }
  //  cout << "I break out of the reading loop " << endl;
  close(connfd);
  exit(0);
}

void* readFrom(void * arg) {
  //  struct args * my_arg = (struct args * ) arg;
  int connfd = *(int*)arg;
  int n;
  char input[MAXLINE];
  char buf[MAXLINE];

  while (1) {
    n = read_line(connfd, input);
    if (n <= 0) break;  
    cout << input;
    de_sentence(input, buf);
    if (strcmp(input, ".bye\n") == 0) break;
  }
  cout << "I break out of the reading loop" << endl;
  close(connfd);
  exit(0);
}

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



int parse_keys(char input[]) {
  string input_str(input);//convert the char array into a C++ string
  cout << "The string is " << input_str <<endl; 
  int p1 = input_str.find(" ", 0);
  if (p1 == string::npos) {
    return -1;
  }
  long e = atoi(input_str.substr(0, p1).c_str());//convert to long int         
  long c = atoi(input_str.substr(p1 + 1, string::npos).c_str());
  crack.set_public(e, c);
  long d = bruteforce(e, c);//get the private key
  //  c_encrypt.set_public(e, c);
  crack.set_private(d, c);
  return 0;
}



void* read_public(void * arg) {
  //  struct args * my_arg = (struct args *) arg;
  int connfd = *(int *)arg;
  int n;
  char input[MAXLINE];
  //  string input;                                                            
  //  n = read_line(connfd, input);//read the first line, containing keys  
  memset(input, 0, MAXLINE);
  n = read(connfd, input, MAXLINE);
  cout << "Read in " << input << endl;
  if (parse_keys(input) == -1) {
    cout << "Error when parsing public keys" << endl;
    exit(1);
  }
  pthread_exit(0);
}



void * send_public (void * arg) {
  int connfd = *(int *) arg;

  char input[MAXLINE];
  int * publics = c_decrypt.get_public();
  char temp[33];
  //  sprintf(temp, "%d ", publics[0]);//copy E into the temp buffer           
  sprintf(input, "%d ", publics[0]);//copy E into the temp buffer              
  sprintf(temp, "%d", publics[1]);//copy C into the temp buffer                
  strcat(input, temp);
  //  strcat(input, "\n");
  cout << "My public keys are " << input << endl;
  write(connfd, input, strlen(input));//send public keys to the client
  cout << "Send successfully" << endl;
  pthread_exit(0);

}

void en_sentence(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, sizeof(buf));
  char temp[33];

  int i;
  for (i = 0; i < msg_length; i++) {
    int en_value = c_decrypt.encrypt_char(input[i]);
    //    int en_value = c_encrypt.encrypt_char(input[i]);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d ", en_value);
    //itoa(en_value, temp, 10);                                                
    //    temp[strlen(temp)] = ' ';                                            
    //    temp[strlen(temp)] = 0;                                              
    strcat(buf, temp);
  }
  //  buf[strlen(buf)] = 0;                                                    
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
  //  memset(buf, '0', sizeof(recvBuff));                                      
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
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -\
	  1) {
        //      perror("socket");                                              
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
    //printf("\n inet_pton error occurred\n");                                 
    //return 1;                                                                
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
  //  pthread_create(&thread, NULL, readFrom, (void*)&sockfd);
  pthread_create(&thread, NULL, readFrom2, (void*)&sockfd);

  //  string input;
  char input[MAXLINE];
  memset(input, 0, MAXLINE);
  char buf[MAXLINE];
  while (1) {
    fgets(input, MAXLINE, stdin);
    //    if (strcmp(input, ".bye") == 0) break;
    //    input[strlen(input) - 1] = 0;
    en_sentence(input, buf);
    write(sockfd, buf, strlen(buf));
    if (strcmp(input, ".bye\n") == 0) break;
    //Rio_writen(clientfd, input, strlen(input));
  }
  //  printf("Buf is: %s", buf);
  
  //  create_new_cache(file_name, buf, &rio);
  //  pthread_cancel(thread);
  close(sockfd);
  //  printf("\n");
  fflush(stdout);
  exit(0);
} 
