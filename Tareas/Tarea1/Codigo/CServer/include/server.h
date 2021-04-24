#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <time.h>
#include <utilities.h>
#include <images.h>

// Header del servidor

int socket_desc, client_sock, c, *new_sock;
struct sockaddr_in server, client;

void init_server();
int run_server();
void *connection_handler(void *);
int parse_header(int sock);
void readAndProcess(char *colors, char *histo, FILE *logfile);

#endif
