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
#define BACKLOG 2

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

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


void do_bind(int sock, struct sockaddr_in adr){

  int retbind; //retour de la fonction bind()

  retbind = bind (sock, (struct sockaddr *) &adr, sizeof(struct sockaddr_in));

  if (retbind == -1){
    perror("error bind : ");
    exit(EXIT_FAILURE);
  }
}

//Accept the connection

int do_accept(int sock, struct sockaddr_in adr,int i, int nb_clients){
  int adr_len = sizeof(adr);
  int connection = accept(sock,(struct sockaddr *)&adr,(socklen_t*)&adr_len);
  if (connection == -1)
    error("accept ERROR\n");
  else if (nb_clients>BACKLOG) // teste si il n'y a pas trop de clients connectés
    printf("Too many clients, connection failed\n");
  else if (connection >0)
    printf("Connection ok with client n°%i\n",i);

  return connection;
}

//Read message

void do_recv(int sockfd, void*buff){
  int msg_recv;
  msg_recv = recv(sockfd, buff, BUFF_LEN_MAX, 0);
  if (msg_recv == -1)
  printf("reception error");
}

//Send message

void do_send(int sockfd, char* msg, int len) {
  int retour;
  size_t taille = (size_t)len;
  void * cast_msg = (void*)msg;
  retour = send(sockfd, cast_msg, taille, 0);
  if (retour == -1)
  printf("error : send");
}


int main(int argc, char** argv)
{

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

  //init the serv_add structure
  struct sockaddr_in serv_addr;
  init_serv_addr(port, &serv_addr);

  //perform the binding
  //we bind on the tcp port specified
  do_bind(s_server, serv_addr);

  listen(s_server, BACKLOG);

  //complete the pollfd structure
   struct pollfd fds[BACKLOG+5];
   int nfds = 1;
   int current_size = 0; //number of wait sockets
   int nb_clients = 0;
   memset(fds,0,sizeof(fds));
   fds[0].fd = s_server;
   fds[0].events = POLLIN;



  for (;;){

    printf("Waiting on poll...\n");
    int rs = poll(fds,nfds,-1);
    if(rs<0){
      printf("Error, poll failed \n");
    }

    //Detect which socket try to talk
    current_size = nfds;
    char* msg_close = "close";
    for(int i=0; i<current_size; i++){
      if(fds[i].revents == POLLIN) {//c'est lui qui a déclenché l'évènement
        if(fds[i].fd == s_server){
          nb_clients ++;
          s_client = do_accept(s_server,serv_addr,current_size, nb_clients);
          if (nb_clients==BACKLOG){
            do_send(s_client, msg_close, strlen(msg_close));
            close(s_client);//fermer la socket du client en trop
            printf("connection fermée\n");
            nb_clients--;
          }
          else {
            fds[nfds].fd = s_client;
            fds[nfds].events = POLLIN;
            nfds++;
          }
        }

        else{
          char* msg_cli = malloc(BUFF_LEN_MAX*sizeof(char));
          do_recv(fds[i].fd, msg);
          msg_cli = (char*)msg;
          //accept connection from client
          do_send(fds[i].fd, msg_cli, strlen(msg_cli));
          printf("Client n°%i say :%s\n",i, msg_cli);
          if(strncmp(msg_cli,"/quit",5) == 0){
            close(fds[i].fd);
            nb_clients--;
            fds[i].fd = -1;
            //fds[i].events = -1;
            //fds[i].revents = -1;
            printf("Client n°%i close connection\n", i);
            for (int i = 0; i < nfds; i++){ //on remplace les socket inutilisées
              if (fds[i].fd == -1){
                for(int j = i; j < nfds; j++){//memmove
                  fds[j].fd = fds[j+1].fd;
                }
                i--;
                nfds--;
              }
            }
          }
        }
       }
      }
    }
  //clean up server socket
  //close(s_server);
  return 0;
}
