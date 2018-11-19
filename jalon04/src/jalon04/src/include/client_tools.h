#ifndef CLIENT_TOOLS_H
#define CLIENT_TOOLS_H_

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
#define PATH_NAME_MAX 2000

struct sockaddr_in get_addr_info(const char* port, struct sockaddr_in* serv_addr1, char* ip);
int do_connect(int sockfd, const struct sockaddr_in addr);
char * handle_client_message(int sockfd, char*buffer, int port);
int to_display(int display, char * channel_name, char * pseudo);

#endif /* CLIENT_TOOLS_H */