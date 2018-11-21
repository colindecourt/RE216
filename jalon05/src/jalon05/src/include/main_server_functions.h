#ifndef MAIN_SERVER_FUNCTIONS_H
#define MAIN_SERVER_FUNCTIONS_H

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


#define BUFF_LEN_MAX 1000
#define BACKLOG 21
#define PSEUDO_LEN_MAX 100


void leave_chat(struct user_table *UserTable, int nb_clients, struct pollfd fds[BACKLOG + 1],int i);
void whois(char * buffer, struct user_table *UserTable, int nb_clients, struct pollfd fds[BACKLOG + 1], int i, struct sockaddr_in6 serv_addr);
void who(char *buffer, struct user_table *UserTable, int nb_clients, int i, struct pollfd fds[BACKLOG+1]);
void broadcast(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1]);
void unicast(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1]);
void main_create_channel(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1],struct channel *channel_table, int id_channel);
void main_join_channel(struct user_table*UserTable,int i, int nb_clients, struct channel * channel_table,char *buffer,struct pollfd fds[BACKLOG+1]);
void main_quit_channel(struct user_table *UserTable, int i, int nb_clients, struct channel *channel_table, char *buffer, struct pollfd fds[BACKLOG + 1]);
void send_canal_msg(struct user_table *UserTable, int i, int nb_clients, struct channel *channel_table, char *buffer, struct pollfd fds[BACKLOG + 1]);
#endif /*MAIN_SERVER_FUNCTIONS_H*/