#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H_

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


int do_socket(int domain, int type, int protocol);
int do_recv(int sockfd, char *buff);
int do_send(int sockfd, char* msg, int len);
void sig_handler(int signum);
void traitement();

#endif /* COMMON_TOOLS_H */
