#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORTA 6789

int server_cnt(int);
int client_cnt(char*, int);
int sock_close();
int msg_snd(char*, int);
int msg_recv(char*, int);

