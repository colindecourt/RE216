#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <error.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include "include/server_tools.h"
#include "include/common_tools.h"
#include "include/lines.h"
#include "include/main_server_functions.h"

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20
#define CHANNEL_LEN_MAX 100

// ------- MAIN -------- //

int main(int argc, char **argv)
{
  struct user_table *UserTable = UserInit();
  struct channel *channel_table = channel_init();
  int id_channel = 1; //for the first channel
  int n;
  if (argc != 2)
  {
    fprintf(stderr, "usage: RE216_SERVER port\n");
    return 1;
  }

  const char *port = argv[1];

  //create the socket, check for validity!
  int s_server = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int s_client;

  //create message pointer
  void *msg = malloc(BUFF_LEN_MAX * sizeof(char));
  char *msg_connection = "server_client";
  char buffer[BUFF_LEN_MAX];

  //init the serv_add structure
  struct sockaddr_in serv_addr;
  init_serv_addr(port, &serv_addr);

  //perform the binding
  //we bind on the tcp port specified
  do_bind(s_server, serv_addr);

  listen(s_server, BACKLOG);

  //complete the pollfd structure
  struct pollfd fds[BACKLOG + 1];

  int nb_clients = 0;
  memset(fds, -1, sizeof(fds));
  fds[0].fd = s_server;
  fds[0].events = POLLIN;

  for (;;)
  {

    printf("Waiting on poll...\n");
    int rs = poll(fds, BACKLOG + 1, -1);
    if (rs < 0)
    {
      printf("Error, poll failed \n");
    }

    //Detect which socket try to talk
    char *msg_close = "_kill_";

    for (int i = 1; i <= BACKLOG; i++)
    {

      if (fds[0].revents == POLLIN)
      { //c'est lui qui a déclenché l'évènement

        if (fds[i].fd == -1)
        {
          const struct sockaddr_in sin;
          char *ip = malloc(sizeof(char) * 20);
          s_client = do_accept(s_server, sin, i);
          printf("kaka %i\n", htons(sin.sin_port));
          nb_clients++;
          if (nb_clients >= BACKLOG)
          {
            do_send(s_client, msg_close, strlen(msg_close));
            close(s_client); //fermer la socket du client en trop
            nb_clients--;
            printf("Too many connection, client closed\n");
            break;
          }
          do_send(s_client, msg_connection, strlen(msg_connection));
          fds[i].fd = s_client;
          fds[i].events = POLLIN;
          do_recv(s_client, ip);
          char *pseudo = malloc(sizeof(char) * PSEUDO_LEN_MAX);
          memset(pseudo, '\0', sizeof(pseudo));
          strcpy(pseudo, "user");
          char *s = malloc(sizeof(int));
          sprintf(s, "%d", i);
          strcat(pseudo, s);

          UserTable = addUser(UserTable, i, pseudo, fds[i].fd, ip, sin.sin_port);
          printf("port%i\n", UserTable->port);
          do_send(fds[i].fd, pseudo, strlen(pseudo));
          break;
        }
      }

      else
      {

        if (fds[i].revents == POLLIN)
        {
          char *msg_cli = malloc(BUFF_LEN_MAX * sizeof(char));
          //do_recv(fds[i].fd, msg);
          msg_cli = (char *)msg;
          //accept connection from client
          //do_send(fds[i].fd, msg_cli, strlen(msg_cli));

          memset(buffer, 0, MSG_SIZE);
          n = read_line(fds[i].fd, buffer, MSG_SIZE);
          if (n < 0)
          {
            perror("ERROR reading from socket");
            break;
          }

          if (strncmp(buffer, "/quit", 5) == 0)
          {
            leave_chat(UserTable, nb_clients, fds, i);
            break;
          }

          else if (strncmp(buffer, "/nick", 5) == 0)
          {
            char *pseudo = buffer + strlen("/nick ");
            struct user_table *curUser = NULL;
            curUser = searchUser(UserTable, i, nb_clients, curUser);
            strcpy(curUser->pseudo, pseudo);
            printf("Welcome on the chat %s\n", pseudo);
            send_line(fds[i].fd, pseudo, PSEUDO_LEN_MAX);
          }

          else if (strncmp(buffer, "/whois", 6) == 0 && strncmp(buffer, "/who", 4) == 0)
          {
            whois(buffer, UserTable, nb_clients, fds, i, serv_addr);
          }

          else if (strncmp(buffer, "/who", 4) == 0)
          {
            who(buffer, UserTable, nb_clients, i, fds);
          }

          else if (strncmp(buffer, "/msgall", 7) == 0)
          {
            broadcast(buffer, UserTable, i, nb_clients, fds);
          }

          else if (strncmp(buffer, "/msg", 4) == 0)
          {
            unicast(buffer, UserTable, i, nb_clients, fds);
          }

          else if (strncmp(buffer, "/create", 7) == 0)
          {
            char *channel_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
            memset(channel_name, '\0', sizeof(channel_name));
            struct channel *temp_channel = NULL;
            temp_channel = channel_table;
            channel_name = buffer + strlen("/create ");
            int possible_create = 0;

            while (temp_channel->next_channel != NULL)
            {
              if (strncmp(temp_channel->channel_name, channel_name, strlen(channel_name)) == 0)
              {
                do_send(fds[i].fd, "Can't create this channel : this channel already exist\n", strlen("Can't create this channel : this channel already exist\n"));
                possible_create = 1;
                break;
              }
              else
              {
                temp_channel = temp_channel->next_channel;
              }
            }

            if (possible_create == 0)
            {
              channel_table = create_channel(channel_table, id_channel, channel_name);
              do_send(fds[i].fd, channel_name, strlen(channel_name));
              id_channel++;
            }
          }

          else if (strncmp(buffer, "/join", 5) == 0)
          {
            main_join_channel(UserTable,i,nb_clients,channel_table,buffer,fds);
          }

          else if (strncmp(buffer, "/leave", 6) == 0)
          {
            main_quit_channel(UserTable,i,nb_clients,channel_table,buffer,fds);
          }

          else
          {
            send_canal_msg(UserTable,i,nb_clients,channel_table,buffer,fds);
          }
        }
      }
    }
  }

  //clean up server socket
  printf("Close socket server\n");
  close(s_server);
  return 0;
}
