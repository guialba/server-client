#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <pthread.h>

#include "comm.h"


void *sender(void *vargp)
{
    char buffer[1024] = {0};
    do {
        printf("\n");
        fgets(buffer, 1024, stdin);
        msg_snd(buffer, strlen(buffer));
        memset(buffer, 0, 1024);
    }
    while(strcmp(buffer, "-b\n"));

    return NULL;
}

void Server()
{
    if(server_cnt(PORTA))
        return;
}
void Client()
{
    if(client_cnt(IP, PORTA))
        return;
}

int menuPrincipal()
{
    int opc = 0;

    while (opc>2 || opc<1)
    {
        system("clear");
        printf("Selecione o Modo: \n\n");
        printf("1- Servidor \n");
        printf("2- Cliente \n");
        printf("\n> ");
        
        scanf("%d", &opc);
        system("pause");

        if (opc>2 || opc<1) 
        {
            system("clear");
            printf("Opção Invalida!\n");
            system("pause");
        }
    }
    system("clear");
    return opc;
}

void main()
{
  
    switch(menuPrincipal())
    {
        case 1: printf("Servidor!\n\n"); Server(); break;
        case 2: printf("Cliente!\n\n"); Client(); break;
    }

    pthread_t snd=0;
    pthread_create(&snd, NULL, sender, NULL);

    char buffer[1024] = {0};
    int valread = 0;
    int r = 0;
    do {
        memset(buffer, 0, 1024);
        r = msg_recv(buffer, 1024);
        if(strcmp(buffer, "\n") && strcmp(buffer, "-b\n") )
        {
            printf(">> ");
            puts(buffer);
        }
    }
    while(strcmp(buffer, "-b\n"));
    msg_snd("-b\n", 3);

    pthread_cancel(snd);
    sock_close();
}
