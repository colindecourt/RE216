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



// ------------ FONCTIONS ------------ //

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20

void pseudo(int display,char*user_name,int s);
void whois_client(char*server_input,char*user_input,int s);
void who_client(char*user_input,char*server_input,int s);
void create_channel_client(char *user_input, char *server_input, int s);
void leave_channel_client(int s,int display);

