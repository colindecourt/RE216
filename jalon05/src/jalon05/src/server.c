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
          char *ip = malloc(sizeof(char) * 20);
          s_client = do_accept(s_server, serv_addr, i);
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
          UserTable = addUser(UserTable, i, pseudo, fds[i].fd, ip, atoi(port));
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
            struct user_table *to_delete = NULL;
            struct user_table *temp = NULL;
            to_delete = searchUser(UserTable, i, nb_clients, to_delete);
            printf("i = %i", nb_clients);
            close(fds[i].fd);
            fds[i].fd = -1;
            fds[i].events = -1;
            printf("Client n°%i close connection\n", i);
            //deleteUser(UserTable, temp, to_delete);
            nb_clients--;
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

            struct user_table *pseudo_user = NULL;
            struct user_table *curUser = NULL;
            char *whois = buffer + strlen("/whois ");
            char msg_whois[5000];
            int cur_id = search_user_pseudo(UserTable, whois, nb_clients, pseudo_user);
            curUser = searchUser(UserTable, cur_id, nb_clients, curUser);
            strcat(msg_whois, curUser->pseudo);
            strcat(msg_whois, " connected since ");
            strcat(msg_whois, curUser->time);
            strcat(msg_whois, " with IP adress ");
            strcat(msg_whois, curUser->ip);
            strcat(msg_whois, " and port number ");
            strcat(msg_whois, port);
            printf("%s\n", msg_whois);
            memset(buffer, '\0', MSG_SIZE);
            do_send(fds[i].fd, msg_whois, strlen(msg_whois));
            memset(msg_whois, '\0', strlen(msg_whois));
          }

          else if (strncmp(buffer, "/who", 4) == 0)
          {
            char msg_who[PSEUDO_LEN_MAX * 20];
            strcpy(msg_who, "\n");

            for (int k = 1; k <= nb_clients; k++)
            {
              struct user_table *curUser = malloc(sizeof(struct user_table));
              curUser = searchUser(UserTable, k, nb_clients, curUser);
              //if (curUser != 0)
              //{
              strcat(msg_who, "-");
              strcat(msg_who, curUser->pseudo);
              strcat(msg_who, "\n");
              //}
            }
            printf("sock %i\n", fds[i].fd);
            do_send(fds[i].fd, msg_who, strlen(msg_who));
            memset(msg_who, '\0', strlen(msg_who));
          }

          else if (strncmp(buffer, "/msgall", 7) == 0)
          {
            char msg_all[BUFF_LEN_MAX];
            strcpy(msg_all, "/msgall ");
            struct user_table *curUser = malloc(sizeof(struct user_table));
            curUser = searchUser(UserTable, i, nb_clients, curUser);
            strcat(msg_all, "[");
            strncat(msg_all, curUser->pseudo, strlen(curUser->pseudo) - strlen("\n"));
            strcat(msg_all, "] ");

            for (int k = 1; k <= nb_clients; k++)
            {
              if (k != i)
              {
                strcpy(buffer, buffer + 8);
                strcat(msg_all, buffer);
                do_send(fds[k].fd, msg_all, BUFF_LEN_MAX);
                memset(msg_all, '\0', BUFF_LEN_MAX);
              }
            }
          }

          else if (strncmp(buffer, "/msg", 4) == 0)
          {
            int j;
            j = strlen("/msg ");
            char *unicast = malloc(sizeof(char) * BUFF_LEN_MAX);
            char *pseudo = malloc(sizeof(char) * PSEUDO_LEN_MAX);
            memset(pseudo, '\0', sizeof(pseudo));
            struct user_table *pseudo_user = malloc(sizeof(struct user_table));
            struct user_table *user_to_send = malloc(sizeof(struct user_table));
            struct user_table *cur_user = malloc(sizeof(struct user_table));
            cur_user = searchUser(UserTable, i, nb_clients, cur_user);
            printf("%s\n", cur_user->pseudo);
            while (buffer[j] != ' ')
            {
              strncat(pseudo, buffer + j, 1);
              j++;
            }
            strcat(pseudo, "\n");
            int id_to_send = search_user_pseudo(UserTable, pseudo, nb_clients, pseudo_user);
            user_to_send = searchUser(UserTable, id_to_send, nb_clients, user_to_send);
            char *temp_pseudo = malloc(sizeof(char) * PSEUDO_LEN_MAX);
            memset(temp_pseudo, '\0', sizeof(temp_pseudo));
            strncpy(temp_pseudo, cur_user->pseudo, strlen(cur_user->pseudo) - strlen("\n"));
            printf("%s\n", temp_pseudo);
            strcat(pseudo, "\n");
            strcpy(unicast, "_$$_");
            strcat(unicast, "[ ");
            strcat(unicast, temp_pseudo);
            strcat(unicast, " ] : ");
            strcat(unicast, buffer + j + 1);
            printf("%s\n", unicast);
            do_send(user_to_send->n_socket, unicast, BUFF_LEN_MAX);
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
              printf("actual number : %i\n", channel_table->actual_number);
              printf("id channel : %i\n", channel_table->id_channel);
              printf("channel name : %s\n", channel_table->channel_name);
              id_channel++;
            }
          }

          else if (strncmp(buffer, "/join", 5) == 0)
          {
            struct user_table *curUser = malloc(sizeof(struct user_table));
            curUser = searchUser(UserTable, i, nb_clients, curUser);
            struct channel *to_join = malloc(sizeof(struct channel));
            char *channel_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
            memset(channel_name, '\0', sizeof(channel_name));
            channel_name = buffer + strlen("/join ");
            to_join = search_channel(channel_table, channel_name, to_join);

            if (to_join->id_channel != -1)
            {
              printf("curuser : %s\n", curUser->pseudo);
              printf("%d\n", to_join->actual_number);
              printf("%s\n", channel_name);
              join_channel(to_join, curUser->pseudo, to_join->actual_number, channel_name, fds[i].fd, curUser);
              printf("%d\n", to_join->actual_number);
              do_send(fds[i].fd, channel_name, strlen(channel_name));
            }
            else
            {
              do_send(fds[i].fd, "server : Channel doesn't exist", strlen("server : Channel doesn't exist"));
            }
          }

          else if (strncmp(buffer, "/leave", 6) == 0)
          {
            struct user_table *curUser = malloc(sizeof(struct user_table));
            curUser = searchUser(UserTable, i, nb_clients, curUser);
            char *channel_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
            memset(channel_name, '\0', sizeof(channel_name));
            channel_name = buffer + strlen("/leave ");
            struct channel *to_quit = malloc(sizeof(struct channel));
            to_quit = search_channel(channel_table, channel_name, to_quit);
            printf("channel:%s", to_quit->channel_name);
            printf("channel:%s", curUser->pseudo);
            quit_channel(to_quit, curUser->pseudo, fds[i].fd);
          }

          else
          {
            struct user_table *cur_user = malloc(sizeof(struct user_table));
            cur_user = searchUser(UserTable, i, nb_clients, cur_user);

            if (cur_user->channel != 0)
            {
              char msg_all[BUFF_LEN_MAX];
              do_recv(fds[i].fd, msg_all);
              printf("Message reçu : %s\n", msg_all);

              struct channel *to_send = malloc(sizeof(struct channel));
              to_send = search_channel_id(channel_table, cur_user, to_send);
              char pseudo_send[BUFF_LEN_MAX];
              strncpy(pseudo_send,cur_user->pseudo,strlen(cur_user->pseudo)-strlen("\n"));
              strcat(pseudo_send," : ");
              strcat(pseudo_send,msg_all);
              for (int k = 1; k <= nb_clients; k++)
              {
                if (UserTable->channel == to_send->id_channel)
                {
                  if (k != i)
                  {
                    do_send(fds[k].fd, pseudo_send, BUFF_LEN_MAX);
                    memset(pseudo_send, '\0', BUFF_LEN_MAX);
                  }
                }
                else
                {
                  UserTable = UserTable->next_user;
                }
              }
            }
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
