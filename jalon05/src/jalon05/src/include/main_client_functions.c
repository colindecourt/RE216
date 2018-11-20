#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client_tools.h"
#include "common_tools.h"
#include "lines.h"

// ------------ FONCTIONS ------------ //

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20

void pseudo(int display, char *user_name, int s)
{
    display = 0;
    char *pseudo = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(pseudo, '\0', sizeof(pseudo));
    do_recv(s, pseudo);
    memset(user_name, '\0', sizeof(user_name));
    strncpy(user_name, pseudo, strlen(pseudo) - strlen("\n"));
}

void whois_client(char *server_input, char *user_input, int s)
{
    memset(server_input, '\0', MSG_SIZE);
    memset(user_input, '\0', MSG_SIZE);
    char msg_whois[5000];
    do_recv(s, msg_whois);
    printf("%s \n", msg_whois);
    fflush(stdout);
    memset(msg_whois, '\0', 2000 * sizeof(char));
}

void who_client(char *user_input, char *server_input, int s)
{
    memset(server_input, '\0', MSG_SIZE);
    memset(user_input, '\0', MSG_SIZE);
    char msg_who[PSEUDO_LEN_MAX * 20];
    do_recv(s, msg_who);
    printf("Online users are : %s\n", msg_who);
    fflush(stdout);
    memset(msg_who, '\0', PSEUDO_LEN_MAX * 20);
}

void create_channel_client(char *user_input, char *server_input, int s)
{
    char *msg = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(msg, '\0', sizeof(msg));
    do_recv(s, msg);
    printf("%s\n", msg);
}

void leave_channel_client(int s,int display)
{

    char *msg = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(msg, '\0', sizeof(msg));
    do_recv(s, msg);
    printf("%s\n", msg);
    display = 0;
}
