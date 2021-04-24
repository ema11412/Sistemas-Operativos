#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// Usada escribir el archivo de registro de cliente
void readCounter(int *clientCounter)
{
  FILE *file = fopen("../tmp/server.client", "r");

  if (file != NULL)
  {
    char line[3];
    fgets(line, sizeof line, file);
    *clientCounter = atoi(line);

    fclose(file);
  }
  else
  {
    printf("Counter File does not exists\n");
  }
}

// Usada leer el archivo de registro de cliente
void writeCounter(int clientCounter)
{
  FILE *file = fopen("../tmp/server.client", "w+");
  char line[3];
  sprintf(line, "%d", clientCounter);
  fputs(line, file);
  fclose(file);
}

// Funcion de split usada en varias funciones
int split(char *str, char c, char ***arr)
{
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
      count++;
    p++;
  }

  *arr = (char **)malloc(sizeof(char *) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
    {
      (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
      if ((*arr)[i] == NULL)
        exit(1);

      token_len = 0;
      i++;
    }
    p++;
    token_len++;
  }
  (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
  if ((*arr)[i] == NULL)
    exit(1);

  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0')
  {
    if (*p != c && *p != '\0')
    {
      *t = *p;
      t++;
    }
    else
    {
      *t = '\0';
      i++;
      t = ((*arr)[i]);
    }
    p++;
  }

  return count;
}

// Revisa que existan los directorios, sino los crea
void checkDir(char *path)
{
  struct stat st = {0};
  if (stat(path, &st) == -1)
  {
    mkdir(path, 0700);
    printf("Directory %s created\n", path);
  }
  else
  {
    printf("Directory %s already exists\n", path);
  }
}

// Crea las rutas para la clasificaion de imagenes
void checkColorsRoute(char *path)
{
  char line[256];
  sprintf(line, "%srojas", path);
  checkDir(line);

  sprintf(line, "%sazules", path);
  checkDir(line);

  sprintf(line, "%sverdes", path);
  checkDir(line);
}

// Parsea el archivo de configuracion
void getData(int *port, char **colors, char **histo, char **log)
{
  FILE *file = fopen("/home/ema/Documents/Operativos/T1/CServer/config.conf", "r");

  char line[256];
  char **arr = NULL;
  char **arr2 = NULL;
  /* or other suitable maximum line size */
  while (fgets(line, sizeof line, file) != NULL) /* read a line */
  {
    split(line, ':', &arr);
    split(arr[1], '\n', &arr2);

    if (!strcmp(arr[0], "Port"))
    {
      *port = atoi(arr2[0]);
    }
    else if (!strcmp(arr[0], "DirColors"))
    {
      checkDir(arr2[0]);
      checkColorsRoute(arr2[0]);
      *colors = arr2[0];
    }
    else if (!strcmp(arr[0], "DirHisto"))
    {
      checkDir(arr2[0]);
      *histo = arr2[0];
    }
    else if (!strcmp(arr[0], "DirLog"))
    {
      checkDir(arr2[0]);
      *log = arr2[0];
    }

    free(arr);
    free(arr2);
  }
  fclose(file);
}
