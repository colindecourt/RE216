#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>

#define BUFF_LEN_MAX 1000
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20


// -------------- STRUCTURE --------------//

struct user_table {
  int id_client;
  char * pseudo;
  int n_socket;
  char *ip;
  int port;
  struct user_table * next_user;
};



// ---------- FUNCTIONS ------------- //

struct user_table * UserInit(){
  struct user_table* user = malloc(sizeof(struct user_table));
  if(!user) exit(EXIT_FAILURE);
  user->next_user = NULL;
  return user;
}

// -------------------------------------------------------------- //

void addUser(struct user_table * UserTable, int id_client, char * pseudo, int n_socket, char *ip, int port){
  struct user_table * new_user = UserTable;
  while(new_user->next_user != NULL){
    new_user = new_user->next_user;
  }
  new_user->id_client = id_client;
  new_user->pseudo = pseudo;
  new_user->n_socket = n_socket;
  new_user->ip = ip;
  new_user->port = port;
  new_user->next_user = NULL;
}

// --------------------------------------------------- //

void deleteUser(struct user_table * UserTable){
  struct user_table * temp = UserTable;
  while(temp->next_user != UserTable){
    temp = temp->next_user;
  }
  UserTable = temp;
}

// -------------------------------------------------------------- // 

struct user_table * searchUser(struct user_table * UserTable, int id_client, int nb_clients){
  int k;
  k = 1;
  struct user_table * temp = malloc(sizeof(struct user_table));
  temp = UserTable;
  while(k<=nb_clients){
    if(temp->id_client == id_client){
      return UserTable;
    }
    else{
      temp = UserTable->next_user;
    }
    k++;
  }

}

// ---------------------------------- //

void editPseudo(struct user_table * UserTable,char*pseudo){
  UserTable->pseudo = pseudo;
}

// ----------------------------------//


void error(const char *msg)
{
  perror(msg);
  exit(1);
}

// ------------------------- //

int do_socket(int domain, int type, int protocol) {
  int sockfd;
  int yes = 1;
  //create the socket
  sockfd = socket( domain, type, protocol );

  //check for socket validity
  if (sockfd == -1){
    fprintf(stderr,"Error : Unvalid server socket\n");
  }

  // set socket option, to prevent "already in use" issue when rebooting the server right on

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  fprintf(stderr,"ERROR setting server socket options\n");

  return sockfd;
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
  error("accept ERROR\n");
  else if (connection >0)
  printf("Connection ok with client n°%i\n",id_client );

  return connection;
}

// ------- Read message -------- //

void do_recv(int sockfd, char *buff){
  int msg_recv;
  void * buffer = (void*)buff;
  msg_recv = recv(sockfd, buffer, BUFF_LEN_MAX, 0);
  if (msg_recv == -1)
  printf("reception error");
}

// ------ Send message ------- //

void do_send(int sockfd, char* msg, int len) {
  int retour;
  size_t taille = (size_t)len;
  void * cast_msg = (void*)msg;
  retour = send(sockfd, cast_msg, taille, 0);
  if (retour == -1)
  printf("error : send");
}

// ------- MAIN -------- //

int main(int argc, char** argv)
{
  struct user_table * UserTable = UserInit();

  if (argc != 2)
  {
    fprintf(stderr, "usage: RE216_SERVER port\n");
    return 1;
  }

  const char * port = argv[1];

  //create the socket, check for validity!
  int s_server = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int s_client;

  //create message pointer
  void* msg = malloc(BUFF_LEN_MAX*sizeof(char));
  char * msg_connection ="server_client";
  //init the serv_add structure
  struct sockaddr_in serv_addr;
  init_serv_addr(port, &serv_addr);

  //perform the binding
  //we bind on the tcp port specified
  do_bind(s_server, serv_addr);

  listen(s_server, BACKLOG);

  //complete the pollfd structure
  struct pollfd fds[BACKLOG+1];

  int nb_clients = 0;
  memset(fds,-1,sizeof(fds));
  fds[0].fd = s_server;
  fds[0].events = POLLIN;


  for (;;){

    printf("Waiting on poll...\n");
    int rs = poll(fds,BACKLOG+1,-1);
    if(rs<0){
      printf("Error, poll failed \n");
    }

    //Detect which socket try to talk
    char* msg_close = "_kill_";


    for(int i=1; i<=BACKLOG; i++){

      if(fds[0].revents == POLLIN) {//c'est lui qui a déclenché l'évènement

      if(fds[i].fd == -1){
        char * ip = malloc(sizeof(char));
        s_client = do_accept(s_server,serv_addr,i);
        nb_clients++; 
        if (nb_clients>=BACKLOG){
          do_send(s_client, msg_close, strlen(msg_close));
          close(s_client);//fermer la socket du client en trop
          nb_clients--;
          printf("Too many connection, client closed\n");
          break;
        }
        do_send(s_client,msg_connection, strlen(msg_connection));
        fds[i].fd = s_client;
        fds[i].events = POLLIN;
        do_recv(s_client, ip);
        addUser(UserTable,i, "", fds[i].fd,ip,atoi(port));

        break;

      }
    }

    else{

      if(fds[i].revents == POLLIN){

        char* msg_cli = malloc(BUFF_LEN_MAX*sizeof(char));
        do_recv(fds[i].fd, msg);
        msg_cli = (char*)msg;
        //accept connection from client
        do_send(fds[i].fd, msg_cli, strlen(msg_cli));

        if(strncmp(msg_cli,"/quit",5) == 0){
          struct user_table * curUser = malloc(sizeof(struct user_table));
          curUser = searchUser(UserTable,i,nb_clients);
          close(fds[i].fd);
          nb_clients--;
          fds[i].fd = -1;
          fds[i].events = -1;
          printf("Client n°%i close connection\n", i);
          deleteUser(curUser);
          break;
        }

        else if(strncmp(msg_cli,"/nick",5)==0){
          char * pseudo = msg_cli+strlen("/nick ");
          struct user_table * curUser = malloc(sizeof(struct user_table));
          curUser = searchUser(UserTable,i,nb_clients);
          editPseudo(curUser, pseudo);
          printf("Welcome on the chat %s\n", pseudo);
          /*printf("id %i\n", curUser->id_client);
          printf("%s\n", curUser->pseudo );
          printf("%i\n", curUser->n_socket);
          printf("%s\n", curUser->ip);
          printf("%i\n", curUser->port);*/
          free(curUser);
        }

        else if(strncmp(msg_cli, "/who",4)==0){
          printf("[SERVER : ] Online users are :\n");
          
          for (int k=1; k<=nb_clients; k++){
            struct user_table * curUser = malloc(sizeof(struct user_table));
            curUser = searchUser(UserTable,k,nb_clients);
            printf("- %s\n", curUser->pseudo);
            free(curUser);
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
