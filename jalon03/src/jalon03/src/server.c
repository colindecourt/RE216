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


#include "include/server_tools.h"
#include "include/common_tools.h"

#define BUFF_LEN_MAX 1000
#define PSEUDO_LEN_MAX 100
//specify the socket to be a server socket and listen for at most 20 concurrent client
#define BACKLOG 21 //Maximum clients = 20


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
        char * ip = malloc(sizeof(char)*20);
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
       
        UserTable = addUser(UserTable,i, "", fds[i].fd,ip,atoi(port));
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
          struct user_table * to_delete=NULL;
          struct user_table * temp = NULL;
          to_delete = searchUser(UserTable,i,nb_clients,to_delete);
          close(fds[i].fd);
          nb_clients--;
          fds[i].fd = -1;
          fds[i].events = -1;
          printf("Client n°%i close connection\n", i);
          deleteUser(UserTable,temp,to_delete);
          break;
        }

        else if(strncmp(msg_cli,"/nick",5)==0){
          char * pseudo = msg_cli+strlen("/nick ");
          struct user_table * curUser=NULL;
          curUser = searchUser(UserTable,i,nb_clients,curUser);
          strcpy(curUser->pseudo, pseudo);
          printf("Welcome on the chat %s\n", pseudo);
        }

        else if(strncmp(msg_cli, "/who",4)==0  && strncmp(msg_cli, "/whois",6)!=0){
          char msg_who[PSEUDO_LEN_MAX*20];
          strcpy(msg_who, " ");
          for(int k=1; k<=nb_clients; k++){
            struct user_table * curUser = NULL;
            curUser = searchUser(UserTable, k, nb_clients, curUser);
            strcat(msg_who, "-");
            strcat(msg_who, curUser->pseudo);
            strcat(msg_who, "\n");
          }
          printf("sock %i\n",fds[i].fd);
          do_send(fds[i].fd,msg_who,strlen(msg_who));
        }

        else if(strncmp(msg_cli, "/whois",6)==0 && strncmp(msg_cli, "/who",4)==0){
          struct user_table * pseudo_user = NULL;
          struct user_table * curUser = NULL;
          char * whois = msg_cli + strlen("/whois ");
          char msg_whois [5000];
          int cur_id = search_user_pseudo(UserTable,whois,nb_clients,pseudo_user);
          curUser = searchUser(UserTable,cur_id,nb_clients,curUser);
          strcat(msg_whois,curUser->pseudo);
          strcat(msg_whois," connected since ");
          strcat(msg_whois,curUser->time);
          strcat(msg_whois," with IP adress ");
          strcat(msg_whois,curUser->ip);
          strcat(msg_whois," and port number ");
          strcat(msg_whois,port);
          do_send(fds[i].fd,msg_whois,strlen(msg_whois));  
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
