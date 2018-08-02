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
    send( *(int*)vargp , "-Conectado ao Servidor", 22, 0 );    

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
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int err = 0, sock=0;
    int new_socket = 0;

    new_socket=0;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORTA );
   
    puts("Server");

    if( (sock = socket (AF_INET, SOCK_STREAM, 0)) == 0 )
    {
        puts("[-]Erro na criação do socket");
        return err-1;
    }
    if( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT,  &(int){ 1 }, sizeof(int)) )
    {
        puts("[-]Erro na parametrização do socket");
        return err-2;
    }
    if( bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0 )
    {
        puts("[-]Erro no bind do socket");
        return err-3;
    }  
    if( listen(sock, 3) < 0 )
    {
        puts("[-]Erro na espera do socket");
        return err-4;
    }
    if( (new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0 )
    {
        puts("[-]Erro na aceitação do socket");
        return err-5;
    }

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

