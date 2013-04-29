/**
 *This is the program for a RSA chat server.
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
#include "Encrypt.h"
#include <time.h>
#include <iostream>

using namespace std;

#define MAXLINE 1024
/**
 *This is the argument struct passed to each thread
 */
struct args {
  int connfd;
  char * ip;
  int port_num;
};

pthread_mutex_t mutex;//Prevent race condition when writing log
Encrypt s_encrypt;
Encrypt s_decrypt;
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


void crack_key(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, MAXLINE);
  char temp[33];
  int de_value;
  char * ptr;
  ptr = strtok(input, " ");
  while (ptr != NULL) {
    de_value = crack.decrypt_char(atoi(ptr));//decrypt an integer         \
                                                                               
    sprintf(temp, "%c", de_value);
    //    cout << "temp: " << temp ;                                          \
                                                                               
    //    cout << temp;                                                       \
                                                                               
    strcat(buf, temp);
    ptr = strtok(NULL, " ");
  }
  cout << buf;
  fflush(stdout);
  //  printf("%s", buf);

}

void de_sentence(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, MAXLINE);
  char temp[33];
  int de_value;
  char * ptr;
  ptr = strtok(input, " ");
  while (ptr != NULL) {
    de_value = s_decrypt.decrypt_char(atoi(ptr));//decrypt an integer                                                          
    sprintf(temp, "%c", de_value);
    //    cout << "temp: " << temp ;                                                                                           
    //    cout << temp;                                                                                                        
    strcat(buf, temp);
    ptr = strtok(NULL, " ");
  }
  printf("%s", buf);

}

void* readFrom2(void * arg) {
  struct args * my_arg = (struct args *) arg;
  int n;
  char input[MAXLINE];
  char buf[MAXLINE];
  memset(input, 0, MAXLINE);
  while (1) {
    n = read(my_arg->connfd, input, MAXLINE);
    if (n < 0) break;
    crack_key(input, buf);
    //    de_sentence(input, buf);
    if (strcmp(buf, ".bye\n") == 0) break;
    memset(input, 0, n);
  }
  close(my_arg->connfd);
  exit(0);
}

void* readFrom(void * arg) {
  struct args * my_arg = (struct args * ) arg;
  //  char buf[MAXLINE];
  //rio_t rio;
  //(&rio, my_arg->connfd);//initialize
  int n;
  char input[MAXLINE];
  char buf[MAXLINE];

  while (1) {
    n = read_line(my_arg->connfd, input);
    if (n <= 0) break;
    cout << input;
    de_sentence(input, buf);
    if (strcmp(buf, ".bye\n") == 0) break;
  }
  exit(0);
}

long try_m(int a, int b, long e) {
  long m = (a - 1) * (b - 1);
  MiniRSA rsa;
  Encryption en;
  if (rsa.GCD(m, e) == 1) {//e and m should be coprime                       \
                                                                              
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
  int p1 = input_str.find(" ", 0);
  if (p1 == string::npos) {
    return -1;
  }
  long e = atoi(input_str.substr(0, p1).c_str());//convert to long int
  long c = atoi(input_str.substr(p1 + 1, string::npos).c_str());
  crack.set_public(e, c);
  long d = bruteforce(e, c);
  crack.set_private(d, c);
  //  s_encrypt.set_public(e, c);
  return 0;
}


void en_sentence(char input[], char buf[]) {
  int msg_length = strlen(input);
  memset(buf, 0, sizeof(buf));
  char temp[33];
  
  int i;
  for (i = 0; i < msg_length; i++) {
    //    int en_value = s_encrypt.encrypt_char(input[i]);
    int en_value = s_decrypt.encrypt_char(input[i]);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d ", en_value);
    strcat(buf, temp);
  }
  printf("ENCRYPTED TO: %s\n", buf);

}

void* read_public(void * arg) {
  struct args * my_arg = (struct args *) arg;
  int n;
  char input[MAXLINE];
  memset(input, 0, MAXLINE);

  n = read(my_arg->connfd, input, MAXLINE);
  cout << "Read public" << input << endl;
  if (parse_keys(input) == -1) {
    cout << "Error when parsing public keys" << endl;
    exit(1);
  }
  pthread_exit(0);
}

void * send_public (void * arg) {
  struct args * my_arg = (struct args *) arg;

  char input[MAXLINE];
  int * publics = s_decrypt.get_public();
  char temp[33];

  sprintf(input, "%d ", publics[0]);//copy E into the temp buffer
  sprintf(temp, "%d", publics[1]);//copy C into the temp buffer
  strcat(input, temp);

  cout << "My public keys are " << input << endl;

  write(my_arg->connfd, input, strlen(input));//send public keys to the client
  cout << "Send successfully" << endl;
  pthread_exit(0);

}

/**
 *This is the routine that each thread will run
 */
void* routine (void * arg) {
  struct args * my_arg = (struct args *) arg;

  char input[MAXLINE];

  pthread_t threads[2];
  pthread_create(&threads[0], NULL, read_public, (void *)arg);
  pthread_create(&threads[1], NULL, send_public, (void *)arg);
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);//wait for the public key transfers finished

  pthread_t thread;//start the reading thread
  pthread_create(&thread, NULL, readFrom2, (void*)arg);

  char buf[MAXLINE];
  memset(input, 0, MAXLINE);

  while (1) {
    fgets(input, MAXLINE, stdin);
    en_sentence(input, buf);
    write(my_arg->connfd, buf, strlen(buf));
    if (strcmp(input, ".bye\n") == 0) break;
  }
  close(my_arg->connfd);

  free(my_arg->ip);
  free(my_arg);
  exit(0);


}

/**
 *Initial its own public keys and private keys
 */
int initial_encrypt(Encrypt * en) {
  en->generate_keys(rand() % 20 + 5, rand() % 20 + 5);
}

/**
 *This is where the program starts from
 */
int main (int argc, char** argv) {
  if (argc < 2) {
    printf("usage: server <port_num>\n");
    return 0;
  }

 
  int listenfd = 0, connfd = 0;
  int  port, clientlen;
  
  struct sockaddr_in serv_addr;
  

  struct hostent *hp;
  char *haddrp;
  
  port = atoi(argv[1]);
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  //  memset()
  //  listenfd = open_listenfd(port);
  cout << "Binding to port " << port << ", please wait ..." << endl;
  bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  listen(listenfd, 10);

  cout << "Server started: localport " << port << endl;
  cout << "Waiting for a client ..." << endl;
  printf("The server is waiting for a request\n");
  struct sockaddr_in client_addr;
  clientlen = sizeof(client_addr);
  

  initial_encrypt(&s_decrypt);//initial my public keys and private keys
  
  connfd = accept(listenfd, (struct sockaddr*)&client_addr, (socklen_t*)&clientlen);
  //while (1) {
  //clientlen = sizeof(clientaddr);
  
  //    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
  //  cout << "Client accepted: ";
    //    printf("The server accepted a request\n");
    hp = gethostbyaddr((const char *) &client_addr.sin_addr.s_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
    haddrp = inet_ntoa(client_addr.sin_addr);
    cout << "Client accepted: Socket[addr=/" << haddrp << ", port=" << client_addr.sin_port << "]" << endl;//print the client address 
    printf("server connected to %s (%s)\n", hp->h_name, haddrp);
    struct args * para = (struct args *) malloc(sizeof(struct args));
    para->connfd = connfd;
    para->ip = (char * )malloc( sizeof (char) * (strlen(haddrp) + 1));//copy ip addr
    strcpy(para->ip, haddrp);
    para->port_num = client_addr.sin_port;
    pthread_t  * thread = (pthread_t *) malloc(sizeof(pthread_t));

    pthread_create(thread, NULL, routine, (void *)para);
    
    pthread_join(*thread, NULL);
    //  }

  return 0;
} 
