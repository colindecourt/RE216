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

#include "include/client_tools.h"
#include "include/common_tools.h"
#include "include/lines.h"
#include "include/server_tools.h"
#include "include/main_client_functions.h"

#define BUFF_LEN_MAX 1000
#define BACKLOG 21
#define PSEUDO_LEN_MAX 100

// ----------- MAIN ------------- //

int main(int argc, char **argv)
{

  char *death_msg = "_kill_";

  char server_input[MSG_SIZE];
  char user_input[MSG_SIZE];
  memset(server_input, 0, MSG_SIZE);
  memset(user_input, 0, MSG_SIZE);

  char *ip = argv[1];
  int port = atoi(argv[2]);

  if (argc != 3)
  {
    fprintf(stderr, "usage: RE216_CLIENT hostname port\n");
    return 1;
  }

  //get address info from the server
  struct sockaddr_in cli_addr;
  //cli_addr = malloc(sizeof(struct sockaddr_in));

  get_addr_info(argv[2], &cli_addr, ip);

  //get the socket
  int s = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  //connect to remote socket
  char *msg_connection = malloc(100 * sizeof(char));

  int connect = do_connect(s, cli_addr);

  do_recv(s, msg_connection);

  if (strncmp(msg_connection, death_msg, 7) == 0)
  {
    printf("Too many connection. Server close connection \n");
    close(s);
  }
  else if (strncmp(msg_connection, "server_client", 14) == 0)
  {
    char *user_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(user_name, '\0', sizeof(user_name));

    do_send(s, ip, strlen(ip));
    do_recv(s, user_name);
    printf("Connection with server ok \n");
    printf(">> %s : Please login with /nick <your pseudo> \n", user_name);
    int display = 0;
    char *channel_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(channel_name, '\0', sizeof(channel_name));
    strcpy(channel_name, "");
    printf("My port is :%i\n", cli_addr.sin_port);

    for (;;)
    {

      struct pollfd fds[BACKLOG + 1];
      memset(fds, -1, sizeof(fds));
      fds[0].fd = s;
      fds[0].events = POLLIN;
      fds[1].fd = STDIN_FILENO;
      fds[1].events = POLLIN;

      to_display(display, channel_name, user_name);

      int rs = poll(fds, BACKLOG + 1, -1);
      if (rs < 0)
      {
        printf("Error, poll failed \n");
      }

      if (fds[1].revents == POLLIN)
      {

        read_line(STDIN_FILENO, user_input, MSG_SIZE);

        send_line(s, user_input, strlen(user_input));

        printf("User input : %s\n", user_input);

        memset(server_input, '\0', MSG_SIZE);

        if (strcmp(user_input, "/quit\n") == 0)
        {
          break;
        }

        else if (strncmp(user_input, "/nick", 5) == 0)
        {
          pseudo(display,user_name,s);
        }

        else if (strncmp(user_input, "/whois", 6) == 0 && strncmp(user_input, "/who", 4) == 0)
        {
         whois_client(server_input,user_input,s); 
        }

        else if (strcmp(user_input, "/who\n") == 0)
        {
          who_client(user_input,server_input,s);
        }

        else if (strncmp(user_input, "/create", 7) == 0)
        {
          create_channel_client(user_input,server_input,s);
        }

        else if (strncmp(user_input, "/leave", 6) == 0)
        {
          leave_channel_client(s,display);
        }

        else if (strncmp(user_input, "/join", 5) == 0)
        {

          char *msg = malloc(sizeof(char) * PSEUDO_LEN_MAX);
          memset(msg, '\0', sizeof(msg));
          do_recv(s, msg);
          if (strncmp(msg, "This channel doesn't exist", strlen("This channel doesn't exist")) == 0)
          {
            printf("%s\n", msg);
          }
          else if (strncmp(msg, "You already joined this channel", strlen("You already joined this channel")) == 0)
          {
            printf("%s\n", msg);
          }
          else if (strncmp(msg, "server : Channel doesn't exist", strlen("server : Channel doesn't exist")) == 0)
          {
            printf("%s\n", msg);
          }
          else
          {
            strncat(channel_name, msg, strlen(msg) - strlen("\n"));
            printf("You join the channel %s\n", channel_name);
            printf("Write something to send to other users. To leave the channel please write '/leave %s'\n", channel_name);
            display = 1;
          }
          fflush(stdout);
        }

        else
        {
          if (display == 1)
          {
            do_send(s, user_input, strlen(user_input));
          }
        }
      }

      else if (fds[0].revents == POLLIN)
      {

        char msg_all[BUFF_LEN_MAX];
        do_recv(s, msg_all);

        if (strncmp(msg_all, "/msgall", 7) == 0)
        {
          char *broadcast = msg_all + strlen("/msgall ");
          printf("il y a un message all \n");
          memset(server_input, '\0', MSG_SIZE);
          memset(user_input, '\0', MSG_SIZE);
          printf("%s\n", broadcast);
          fflush(stdout);
          memset(msg_all, '\0', BUFF_LEN_MAX * sizeof(char));
        }

        else if (strncmp(msg_all, "_$$_", 4) == 0)
        {
          char *unicast = msg_all + strlen("_$$_");
          printf("%s\n", unicast);
        }
        else if (strcmp(msg_all, "A user wants you to accept the transfer of the file") == 0)
        {
          do_bind(s, cli_addr);
          listen(s, 1);
          //int s2 = accept(s,(struct sockaddr *)&serv_addr, (socklen_t *)&serv_addr);
        }
        else
        {
          if (display == 1)
          {
            printf("%s\n", msg_all);
          }
        }
      }
    }
  }

  //close Connection
  printf("Close connection\n");
  close(s);
  return 0;
}
