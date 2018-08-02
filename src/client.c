#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <pthread.h>

#define IP "127.0.0.1"
#define PORTA 6789

void *sender(void *vargp)
{
    send( *(int*)vargp , "-Cliente Conectado", 22, 0 );    

    char buffer[1024] = {0};
    do {
        printf("\n");
        fgets(buffer, 1024, stdin);
        send( *(int*)vargp , buffer, strlen(buffer), 0 );
        memset(buffer, 0, 1024);
    }
    while(strcmp(buffer, "-b\n"));

    return NULL;
}

int main(int nargs, char* args[])
{
    struct sockaddr_in serv_addr;
    int err = 0;
    int new_socket = 0;

    new_socket=0;
   
    puts("Client");

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( PORTA );

    if( (new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        return err-1;
    if( inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0 ) 
        return err-2;
    if( connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
        return err-3;

    pthread_t snd=0;
    pthread_create(&snd, NULL, sender, &new_socket);

    char buffer[1024] = {0};
    int valread = 0;
    int r = 0;
    do {
        memset(buffer, 0, 1024);
        r = read( new_socket , buffer, 1024);
        printf(">> ");
        puts(buffer);
    }
    while(strcmp(buffer, "-b\n"));
    send( new_socket, "-b\n", 3, 0 );

    puts("Recebeu Saida");

    pthread_cancel(snd);
    close(new_socket);

    return err;
}

