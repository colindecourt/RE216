#ifndef SERVER_TOOLS_H
#define SERVER_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>

// -------------- STRUCTURE --------------//
#define PSEUDO_LEN_MAX 100
struct user_table {
  int id_client;
  char pseudo[PSEUDO_LEN_MAX];
  int n_socket;
  char ip[20];
  int port;
  struct user_table * next_user;
};

struct user_table * UserInit();
struct user_table * addUser(struct user_table * UserTable, int id_client, char * pseudo, int n_socket, char * ip, int port);
void deleteUser(struct user_table * UserTable);
struct user_table * searchUser(struct user_table * UserTable, int id_client, int nb_clients, struct user_table * wanted_user);
void init_serv_addr(const char* port, struct sockaddr_in* serv_addr);
void do_bind(int sock, struct sockaddr_in adr);
int do_accept(int sock, struct sockaddr_in adr,int id_client);


#endif /* SERVER_TOOLS_H*/