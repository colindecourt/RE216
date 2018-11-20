#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include "server_tools.h"
#include "common_tools.h"

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
#define CHANNEL_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20

// ---------- FUNCTIONS ------------- //

char *get_time()
{
  char date[40];
  char buff[20];
  char buff1[20];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(buff, 20, "%F", tm);
  strftime(buff1, 20, "%T", tm);
  strcat(date, buff);
  strcat(date, " @ ");
  strcat(date, buff1);
}

// ----------------------------------- //

struct channel *channel_init()
{
  struct channel *channel = malloc(sizeof(struct channel));
  if (!channel)
    exit(EXIT_FAILURE);
  channel->next_channel = NULL;
  return channel;
}

// -------------------------------------------------------------------- //

struct channel *create_channel(struct channel *channel_table, int id_channel, char *channel_name)
{
  struct channel *new_channel; //ajout à gauche
  new_channel = malloc(sizeof(struct channel));
  new_channel->id_channel = id_channel;
  new_channel->channel_name = malloc(sizeof(char) * CHANNEL_LEN_MAX);
  strcpy(new_channel->channel_name, channel_name);
  new_channel->actual_number = 0;
  for (int i = 0; i < CAPACITY_CHANNEL; i++)
  {
    new_channel->connected_people[i] = "";
  }
  new_channel->next_channel = channel_table;
  return new_channel;
}

// ----------------------------------------------------------------------------------- //

int join_channel(struct channel *channel_table, char *pseudo, int actual_number,char * channel_name, int socket, struct user_table * cur_user)
{
  channel_table->connected_people[actual_number] = malloc(sizeof(strlen(pseudo)));
  struct channel * temp = NULL;
  temp = channel_table;
  if(channel_table->next_channel==NULL)
  {
    do_send(socket,"This channel doesn't exist\n", strlen("This channel doesn't exist\n"));
    return 0;
  }
  else
  {
    while(strncmp(temp->channel_name,channel_name, strlen(channel_name))!=0)
    {
      if(temp->next_channel == NULL){
        do_send(socket,"This channel doesn't exist\n", strlen("This channel doesn't exist\n"));
        return 0;
      }
      else
      {
        temp = temp->next_channel;
      }
    }
  }
  for(int i = 0; i<CAPACITY_CHANNEL; i++)
  {
    if(strncmp(pseudo, channel_table->connected_people[i], strlen(pseudo))==0){
      do_send(socket,"You already joined this channel\n",strlen("You already joined this channel\n"));
      return 0;
    }
  }
  strncpy(channel_table->connected_people[actual_number], pseudo, strlen(pseudo));
  channel_table->actual_number++;
  cur_user->channel = channel_table->id_channel;
  printf("Nouveau client : %s\n", channel_table->connected_people[actual_number]);
  //do_send(socket,"You join a channel\n",strlen("You join a channel\n"));
  return 0;
}

// ----------------------------------------------------------------------------------- //

void quit_channel(struct channel *channel_table, char *pseudo, int socket)
{
  int exist_channel = 0;
  char * msg_quit = malloc(sizeof(BUFF_LEN_MAX));
  memset(msg_quit,'\0',sizeof(msg_quit));
  strcpy(msg_quit,"You leave the channel ");
  int i = 0;
  int number = channel_table->actual_number;
  while (i < number)
  {
    if (strncmp(channel_table->connected_people[i], pseudo, strlen(pseudo)) == 0)
    {
      strncat(msg_quit,channel_table->channel_name, strlen(channel_table->channel_name));
      strncpy(channel_table->connected_people[i], "", strlen(""));
      do_send(socket, msg_quit,strlen(msg_quit));
      memset(msg_quit,'\0',sizeof(msg_quit));
      channel_table->actual_number--;
      exist_channel = 1;
    }
    printf("People on channel %s\n", channel_table->connected_people[i]);
    i++;
  }
  if ( exist_channel == 0 ){
    do_send(socket,"Not an existing channel\n", strlen("Not an existing channel\n"));
  }
  else if (channel_table->actual_number == 0)
  {
    do_send(socket,"Empty channel, channel will be destroy\n", strlen("Empty channel, channel will be destroy\n"));
    strncpy(channel_table->channel_name, "//", strlen("//"));
  }
}

// --------------------------------------------------------------------------------------- //

struct channel *search_channel(struct channel *channel_table, char *channel_name, struct channel *wanted_channel)
{
  wanted_channel = channel_table;
  while (wanted_channel->next_channel != NULL){
    if (strncmp(wanted_channel->channel_name, channel_name, strlen(channel_name)) == 0)
    {
      return wanted_channel;
    }
    else
    {
      wanted_channel = wanted_channel->next_channel;
    }
  }
  wanted_channel->id_channel = -1;
  return wanted_channel;
}

// ------------------------------------------------------------------------ //


struct channel *search_channel_id(struct channel *channel_table, struct user_table* cur_user, struct channel *wanted_channel)
{
  wanted_channel = channel_table;
  if (wanted_channel->id_channel == cur_user->channel)
  {
    return wanted_channel;
  }
  else
  {
    wanted_channel = wanted_channel->next_channel;
  }
  return wanted_channel;
}

// ------------------------------------------------------------------------ //

struct user_table *UserInit()
{
  struct user_table *user = malloc(sizeof(struct user_table));
  if (!user)
    exit(EXIT_FAILURE);
  user->next_user = NULL;
  return user;
}

// -------------------------------------------------------------- //

struct user_table *addUser(struct user_table *UserTable, int id_client, char *pseudo, int n_socket, char *ip, int port)
{
  struct user_table *new_user; //ajout à gauche
  new_user = malloc(sizeof(struct user_table));
  new_user->id_client = id_client;
  new_user->pseudo = malloc(sizeof(char) * PSEUDO_LEN_MAX);
  strcpy(new_user->pseudo, pseudo);
  new_user->n_socket = n_socket;
  strcpy(new_user->ip, ip);
  new_user->port = port;
  strcpy(new_user->time, get_time());
  new_user->channel=0;
  new_user->next_user = UserTable;
  return new_user;
}

// --------------------------------------------------- //

void deleteUser(struct user_table *UserTable, struct user_table *temp, struct user_table *to_delete)
{

  temp = UserTable;

  if (temp->id_client == to_delete->id_client)
  {
    if (temp->next_user->next_user == NULL)
    {
      temp->next_user = NULL;
      free(temp);
    }
    else
    {
      temp->next_user = to_delete->next_user;
      free(to_delete);
    }
  }

  else
  {
    while (temp->next_user->id_client != to_delete->id_client)
    {
      temp = temp->next_user;
    }
    temp->next_user = to_delete->next_user;
    free(to_delete);
  }
}

// -------------------------------------------------------------- //

struct user_table *searchUser(struct user_table *UserTable, int id_client, int nb_clients, struct user_table *wanted_user)
{
  int k = 0;
  wanted_user = UserTable;
  while (k < nb_clients)
  {
    if (wanted_user->id_client == id_client)
      return wanted_user;
    k++;
    wanted_user = wanted_user->next_user;
  }
  return wanted_user;
}

// ----------------------------------//

int search_user_pseudo(struct user_table *UserTable, char *pseudo, int nb_clients, struct user_table *wanted_user)
{
  int k = 0;
  wanted_user = UserTable;
  while (k < nb_clients)
  {
    if (strcmp(wanted_user->pseudo, pseudo) == 0)
    {
      return wanted_user->id_client;
    }
    k++;
    wanted_user = wanted_user->next_user;
  }
  return 0;
}

// ------------------------------------ //

int pseudo_to_socket(struct user_table *UserTable, char *pseudo, int nb_clients, struct user_table *wanted_user)
{
  int k = 0;
  wanted_user = UserTable;
  while (k < nb_clients)
  {
    if (strcmp(wanted_user->pseudo, pseudo) == 0)
    {
      return wanted_user->n_socket;
    }
    k++;
    wanted_user = wanted_user->next_user;
  }
  return 0;
}

// ------------------------ //

void init_serv_addr(const char *port, struct sockaddr_in *serv_addr)
{

  int portno;

  //clean the serv_add structure
  memset(serv_addr, '\0', sizeof(struct sockaddr_in));

  //cast the port from a string to an int
  portno = atoi(port);

  //internet family protocol
  serv_addr->sin_family = AF_INET;

  //we bind to any ip form the host
  serv_addr->sin_addr.s_addr = htons(INADDR_ANY);

  //we bind on the tcp port specified
  serv_addr->sin_port = htons(portno);
}

// ----------------------- //

void do_bind(int sock, struct sockaddr_in adr)
{

  int retbind; //retour de la fonction bind()

  retbind = bind(sock, (struct sockaddr *)&adr, sizeof(struct sockaddr_in));

  if (retbind == -1)
  {
    perror("error bind : ");
    exit(EXIT_FAILURE);
  }
}

// ------- Accept the connection ------- //

int do_accept(int sock, struct sockaddr_in adr, int id_client)
{
  int adr_len = sizeof(adr);
  int connection = accept(sock, (struct sockaddr *)&adr, (socklen_t *)&adr_len);
  if (connection == -1)
    perror("accept ERROR\n");
  else if (connection > 0)
    printf("Connection ok with client n°%i\n", id_client);

  return connection;
}
