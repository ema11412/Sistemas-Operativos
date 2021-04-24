#include <server.h>

// Estrcutura usada para los parametros del hilo
typedef struct
{
  int sock;
  int clientCounter;
  char *colors;
  char *histo;
  char *logs;
} threadArgs;

// Inicia el servidor
void init_server(int port)
{
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }
  puts("Socket created");
  printf("Running on port %d\n", port);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("bind failed. Error");
    exit(EXIT_FAILURE);
  }
  puts("bind done");

  listen(socket_desc, 3);

  c = sizeof(struct sockaddr_in);

  puts("Waiting for incoming connections...");
}

// Funcion que espera y acepta las conexiones
int run_server()
{
  // Parseo del archivo de configuracion

  int port = 1717;
  int clientCounter = 0;
  char *colors = "/home/ema/Documents/Operativos/T1/CServer";
  char *histo = "/home/ema/Documents/Operativos/T1/CServer";
  char *logs = "/home/ema/Documents/Operativos/T1/CServer";

  getData(&port, &colors, &histo, &logs);
  readCounter(&clientCounter);
  checkDir("/home/ema/Documents/Operativos/T1/CServer/tmp/");

  init_server(port);

  while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
  {
    puts("Connection accepted");

    pthread_t sniffer_thread;
    new_sock = (int *)malloc(1);
    *new_sock = client_sock;

    clientCounter++;

    threadArgs *args = malloc(sizeof *args);

    args->clientCounter = clientCounter;
    args->sock = client_sock;
    args->colors = malloc(sizeof(char) * 100);
    args->histo = malloc(sizeof(char) * 100);
    args->logs = malloc(sizeof(char) * 100);

    memcpy(args->colors, colors, sizeof(char) * 100);
    memcpy(args->histo, histo, sizeof(char) * 100);
    memcpy(args->logs, logs, sizeof(char) * 100);

    if (pthread_create(&sniffer_thread, NULL, connection_handler, args) < 0)
    {
      free(args);
      free(args->colors);
      free(args->histo);
      free(args->logs);

      perror("could not create thread");
      return 1;
    }

    puts("Handler assigned");
    //pthread_join(sniffer_thread, NULL);
  }

  if (client_sock < 0)
  {
    perror("accept failed");
    return 1;
  }
}

// Funcion para parsear el header el request
int parse_header(int sock)
{
  char c;
  char buff[1024] = "", *ptr = buff + 4;
  int bytes_received, status;
  printf("Begin HEADER ..\n");
  while (bytes_received = recv(sock, ptr, 1, 0))
  {
    if (bytes_received == -1)
    {
      perror("Parse Header");
      exit(1);
    }

    if (
        (ptr[-3] == '\r') && (ptr[-2] == '\n') &&
        (ptr[-1] == '\r') && (*ptr == '\n'))
      break;
    ptr++;
  }

  *ptr = 0;
  ptr = buff + 4;

  if (bytes_received)
  {
    ptr = strstr(ptr, "Content-Length:");
    if (ptr)
    {
      sscanf(ptr, "%*s %d", &bytes_received);
    }
    else
      bytes_received = -1; 

    printf("Content-Length: %d\n", bytes_received);
  }
  printf("End HEADER ..\n");
  return bytes_received;
}

// Funcion usada para procesar las imagenes por tamaño
void readAndProcess(char *colors, char *histo, FILE *logfile)
{
  FILE *file = fopen("/home/ema/Documents/Operativos/T1/CServer/tmp/tmpfile.txt", "r");
  char line[256];
  char averageName[50];
  char medianName[50];
  char imageInfo[50];
  char **arr = NULL;

  int counter = 0;

  fputs("Star processing all images...\n", logfile);

  while (fgets(line, sizeof line, file) != NULL) /* read a line */
  {
    split(line, '\n', &arr);

    if (counter > 0)
    {
      sprintf(imageInfo, "Star classify Image: %s\n", arr[0]);
      fputs(imageInfo, logfile);

      classify("/home/ema/Documents/Operativos/T1/CServer/tmp/", arr[0], colors);

      sprintf(averageName, "%saverage_filter", histo);
      sprintf(medianName, "%smedian_filter", histo);

      sprintf(imageInfo, "Applying Average Filter to Image: %s\n", arr[0]);
      fputs(imageInfo, logfile);

      average_filter("/home/ema/Documents/Operativos/T1/CServer/tmp/", arr[0], averageName);

      sprintf(imageInfo, "Applying Median Filter to Image: %s\n", arr[0]);
      fputs(imageInfo, logfile);

      median_filter("/home/ema/Documents/Operativos/T1/CServer/tmp/", arr[0], medianName);
    }

    free(arr);
    counter++;
  }

  fputs("Finished processing all images\n", logfile);
  fputs("Cleaning....\n", logfile);
  fputs("----------------\n", logfile);

  system("rm /home/ema/Documents/Operativos/T1/CServer/tmp/image*");

  fclose(file);
}

// Handler de los hilos
void *connection_handler(void *args)
{
  threadArgs *actual_args = args;

  // int sock = *(int *)socket_desc;
  int contentlengh, bytes_received;

  char recv_data[1024];
  char fileName[20];
  char imageRoute[50];
  char logFileName[50];
  char clientInfo[100];

  sprintf(logFileName, "%sserver.log", actual_args->logs);
  FILE *logFile = fopen(logFileName, "a");

  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char date[64];
  strftime(date, sizeof(date), "%c", tm);

  sprintf(clientInfo, "Client %d connected, processing file image%d, at %s\n",
          actual_args->clientCounter, actual_args->clientCounter, date);
  fputs(clientInfo, logFile);

  if (contentlengh = parse_header(actual_args->sock))
  {
    fputs("Start to recieve image\n", logFile);

    sprintf(fileName, "image%d.png", actual_args->clientCounter);
    sprintf(imageRoute, "/home/ema/Documents/Operativos/T1/CServer/tmp/%s", fileName);

    int bytes = 0;
    FILE *fd = fopen(imageRoute, "wb");

    printf("Saving data...\n\n");

    while (bytes_received = recv(actual_args->sock, recv_data, 1024, 0))
    {
      if (bytes_received == -1)
      {
        perror("recieve");
        exit(3);
      }

      if (!strcmp(recv_data, "init"))
      {
        fputs("Process command Received\n", logFile);
        printf("Process command Received\n");

        system("ls /home/ema/Documents/Operativos/T1/CServer/tmp -pSr -1 | grep -vE '/|.txt|.client' > /home/ema/Documents/Operativos/T1/CServer/tmp/tmpfile.txt");

        fputs("Images Sorted\n", logFile);
        printf("Images Sorted\n");
        readAndProcess(actual_args->colors, actual_args->histo, logFile);

        printf("Image Processed...\n");

        break;
      }

      fwrite(recv_data, 1, bytes_received, fd);
      bytes += bytes_received;
      printf("Bytes recieved: %d from %d\n", bytes, contentlengh);
      if (bytes == contentlengh)
        break;
    }
    fclose(fd);

    fputs("Image Received\n", logFile);
  }
  else
  {
    fputs("Failed to receive data\n", logFile);
    fputs("----------------\n", logFile);
  }

  writeCounter(actual_args->clientCounter);

  fclose(logFile);
  close(actual_args->sock);

  free(actual_args->colors);
  free(actual_args->histo);
  free(actual_args->logs);
  free(args);
}
