
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <time.h>

// Archivo main

int main()
{
  char recv_data[1024];
  char fileName[20];
  char imageRoute[50];
  char logFileName[50];
  char clientInfo[100];

    FILE *logFile = fopen(logFileName, "a");

    fputs("Star processing all images...\n", logFile);
  return 0;
}

