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
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20





// ---------- FUNCTIONS ------------- //

char * get_time(){
  char date [40];
  char buff[20];
  char buff1[20];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(buff, 20, "%F", tm);
  strftime(buff1,20,"%T",tm);
  strcat(date,buff);
  strcat(date," @ ");
  strcat(date,buff1);
}

// ----------------------------------- //

struct user_table * UserInit(){
  struct user_table* user = malloc(sizeof(struct user_table));
  if(!user) exit(EXIT_FAILURE);
  user->next_user = NULL;
  return user;
}

// -------------------------------------------------------------- //

struct user_table * addUser(struct user_table * UserTable, int id_client, char *pseudo, int n_socket, char * ip, int port){
  struct user_table * new_user; //ajout à gauche
  new_user = malloc(sizeof(struct user_table));
  new_user->id_client = id_client;
  new_user->pseudo = malloc(sizeof(char)*PSEUDO_LEN_MAX);
  strcpy(new_user->pseudo, pseudo);
  new_user->n_socket = n_socket;
  strcpy(new_user->ip,ip);
  new_user->port = port;
  strcpy(new_user->time, get_time());
  new_user->next_user = UserTable;
  return new_user;
}

// --------------------------------------------------- //

void deleteUser(struct user_table * UserTable,struct user_table * temp, struct user_table * to_delete){
  temp = UserTable;
  /*if(temp->next_user != to_delete){
    temp = temp->next_user;
  }
  else if(to_delete->next_user == NULL){
      temp->next_user = NULL;
    }
  else{
    temp->next_user = to_delete->next_user;
  }*/
  if (temp == to_delete){
    free(to_delete);
  }
  else {
    while(temp->next_user != to_delete){
      temp = temp->next_user;
    }
    temp->next_user = to_delete->next_user;
    free(to_delete);
  }
}

// -------------------------------------------------------------- //

struct user_table * searchUser(struct user_table * UserTable, int id_client, int nb_clients, struct user_table * wanted_user){
  int k=0;
  wanted_user = UserTable;
  while(k<nb_clients){
    if(wanted_user->id_client == id_client)
      return wanted_user;
    k++;
    wanted_user = wanted_user->next_user;
  }
  return 0;
}

// ----------------------------------//

int search_user_pseudo(struct user_table * UserTable, char * pseudo, int nb_clients, struct user_table * wanted_user){
  int k=0;
  wanted_user = UserTable;
  while(k<nb_clients){
    if(strcmp(wanted_user->pseudo,pseudo)==0){
      return wanted_user->id_client;
    }
    k++;
    wanted_user = wanted_user->next_user;
  }
  return 0;
}

// ------------------------ //

void init_serv_addr(const char* port, struct sockaddr_in* serv_addr) {

  int portno;

  //clean the serv_add structure
  memset( serv_addr, '\0', sizeof(struct sockaddr_in));

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

void do_bind(int sock, struct sockaddr_in adr){

  int retbind; //retour de la fonction bind()

  retbind = bind (sock, (struct sockaddr *) &adr, sizeof(struct sockaddr_in));

  if (retbind == -1){
    perror("error bind : ");
    exit(EXIT_FAILURE);
  }
}

// ------- Accept the connection ------- //

int do_accept(int sock, struct sockaddr_in adr,int id_client){
  int adr_len = sizeof(adr);
  int connection = accept(sock,(struct sockaddr *)&adr,(socklen_t*)&adr_len);
  if (connection == -1)
  perror("accept ERROR\n");
  else if (connection >0)
  printf("Connection ok with client n°%i\n",id_client );

  return connection;
}
