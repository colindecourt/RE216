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

#include "common_tools.h"
#include "lines.h"
#include "server_tools.h"

#define BUFF_LEN_MAX 1000
#define BACKLOG 21
#define PSEUDO_LEN_MAX 100



void leave_chat(struct user_table *UserTable, int nb_clients, struct pollfd fds[BACKLOG + 1], int i)
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
}

void whois(char *buffer, struct user_table *UserTable, int nb_clients, struct pollfd fds[BACKLOG + 1], int i, struct sockaddr_in serv_addr)
{
    struct user_table *pseudo_user = NULL;
    struct user_table *curUser = NULL;
    char *whois = buffer + strlen("/whois ");
    char msg_whois[5000];
    int cur_id = search_user_pseudo(UserTable, whois, nb_clients, pseudo_user);
    curUser = searchUser(UserTable, cur_id, nb_clients, curUser);
    char *port_client = malloc(sizeof(char) * 10);
    sprintf(port_client, "%i", serv_addr.sin_port);
    strcat(msg_whois, curUser->pseudo);
    strcat(msg_whois, " connected since ");
    strcat(msg_whois, curUser->time);
    strcat(msg_whois, " with IP adress ");
    strcat(msg_whois, curUser->ip);
    strcat(msg_whois, " and port number ");
    strcat(msg_whois, port_client);
    printf("%s\n", msg_whois);
    memset(buffer, '\0', MSG_SIZE);
    do_send(fds[i].fd, msg_whois, strlen(msg_whois));
    memset(msg_whois, '\0', strlen(msg_whois));
}

void who(char *buffer, struct user_table *UserTable, int nb_clients, int i, struct pollfd fds[BACKLOG + 1])
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

void broadcast(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1])
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

void unicast(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1])
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

void main_create_channel(char *buffer, struct user_table *UserTable, int i, int nb_clients, struct pollfd fds[BACKLOG + 1], struct channel *channel_table, int id_channel)
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

void main_join_channel(struct user_table *UserTable, int i, int nb_clients, struct channel *channel_table, char *buffer, struct pollfd fds[BACKLOG + 1])
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

void main_quit_channel(struct user_table *UserTable, int i, int nb_clients, struct channel *channel_table, char *buffer, struct pollfd fds[BACKLOG + 1])
{
    struct user_table *curUser = malloc(sizeof(struct user_table));
    curUser = searchUser(UserTable, i, nb_clients, curUser);
    char *channel_name = malloc(sizeof(char) * PSEUDO_LEN_MAX);
    memset(channel_name, '\0', sizeof(channel_name));
    channel_name = buffer + strlen("/leave ");
    struct channel *to_quit = malloc(sizeof(struct channel));
    to_quit = search_channel(channel_table, channel_name, to_quit);
    quit_channel(to_quit, curUser->pseudo, fds[i].fd);
}

void send_canal_msg(struct user_table *UserTable, int i, int nb_clients, struct channel *channel_table, char *buffer, struct pollfd fds[BACKLOG + 1])
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
        strncpy(pseudo_send, cur_user->pseudo, strlen(cur_user->pseudo) - strlen("\n"));
        strcat(pseudo_send, " : ");
        strcat(pseudo_send, msg_all);
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
