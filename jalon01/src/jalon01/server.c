#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include <arpa/inet.h>



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
  portno   = atoi(port);

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
    //fprintf(stderr,"Error : bind failed\n");
    perror("error bind : ");
    exit(EXIT_FAILURE);
  }
  printf("bind ok \n");
}



//Accept the connection

int do_accept(int sock, struct sockaddr_in adr){
    int adr_len = sizeof(adr);
    int connection = accept(sock,(struct sockaddr *)&adr,(socklen_t*)&adr_len);
    if (connection == -1)
        error("accept ERROR\n");
    else if (connection >0)
      printf("Connection ok");

    //return connection;
    printf("%i\n", connection);
}

//Read message

ssize_t do_read(int sock, void *data_ptr, size_t to_recv){
    int recv = 0;
    do{
      recv += read(sock, &sock + recv, to_recv - recv);

    } while(recv != to_recv);

    if (recv == -1){
      error("Data not read");
    }
    else{
      return recv;
    }
}

ssize_t do_write(int sock, void *data_ptr, size_t to_send){
  int sent = 0;
  do{
    sent += write(sock, &sock + sent, to_send - sent);
  } while(sent != to_send);

  if (sent == -1){
    error("Data not send");
  }
  else {
    return sent;
  }
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

  //create message pointer
  char msg_ser[50000];
  char end[] = "End of message\n";
  memset(msg_ser,0,sizeof(msg_ser));

  //init the serv_add structure
  struct sockaddr_in serv_addr;
  init_serv_addr(port, &serv_addr);

  //perform the binding
  //we bind on the tcp port specified
  do_bind(s_server, serv_addr);



  //specify the socket to be a server socket and listen for at most 20 concurrent client
  int backlog = 20;

  listen(s_server, backlog);




  for (;;)
  {
  //accept connection from client
  int s_client = do_accept(s_server,serv_addr);

  while(strcmp(msg_ser,end) != 0){
    do_read(s_client, msg_ser,sizeof(msg_ser));
    do_write(s_client, msg_ser,sizeof(msg_ser));
  }
  //read what the client has to say
  //do_read(sock,msg,)

  //we write back to the client
  //do_write()

  //clean up client socket
  memset(msg_ser,0,sizeof(msg_ser));
  close(s_client);
}

//clean up server socket
  close(s_server);
return 0;
}
