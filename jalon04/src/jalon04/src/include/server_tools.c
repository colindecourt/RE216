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
  for(int i = 0; i < CAPACITY_CHANNEL; i++){
    new_channel->connected_people[i] = ""; 
  }
  new_channel->next_channel = channel_table;
  return new_channel;
}

// ----------------------------------------------------------------------------------- //

void join_channel(struct channel *channel_table, char *pseudo, int actual_number)
{
  channel_table->connected_people[actual_number] = malloc(sizeof(strlen(pseudo)));
  strcpy(channel_table->connected_people[actual_number],pseudo);
  channel_table->actual_number++;
  printf("Nouveau client : %s\n",channel_table->connected_people[actual_number]);
}

// ----------------------------------------------------------------------------------- //

void quit_channel(struct channel *channel_table, char *pseudo){
  int i = 0;
  while(strcmp(channel_table->connected_people[i],"") != 0){
    /*if (strcmp(channel_table->connected_people[i], pseudo) ==0){
      free(channel_table->connected_people[i]);
      channel_table->actual_number--;
    }*/
    if(strcmp(channel_table->connected_people[i],pseudo)==0){
      strcpy(channel_table->connected_people[i], "\0");
      channel_table->actual_number--;
    }
    printf("People on channel %s\n", channel_table->connected_people[i]);
    i++;
  }
}


struct channel *search_channel(struct channel *channel_table, char * channel_name, struct channel *wanted_channel)
{
  wanted_channel = channel_table;
  if (strcmp(wanted_channel->channel_name,channel_name)==0)
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
  serv_addr->sin_addr.s_addr = INADDR_ANY;

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
