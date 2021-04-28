#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/utilities.h"

/* split function */
int split(char *str, char c, char ***arr) {
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0') {
    if (*p == c)
      count++;
    p++;
  }

  *arr = (char **)malloc(sizeof(char *) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0') {
    if (*p == c) {
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
  while (*p != '\0') {
    if (*p != c && *p != '\0') {
      *t = *p;
      t++;
    } else {
      *t = '\0';
      i++;
      t = ((*arr)[i]);
    }
    p++;
  }

  return count;
}

/* check if the directory exits if not, it creates it */
int checkDir(char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    mkdir(path, 0700);
    printf("Directory %s created\n", path);
    return 0;
  } else {
    printf("Directory %s already exists\n", path);
    return 1;
  }
}

/* concats two strings */
char *concat(const char *s1, const char *s2) {
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);

  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

/* check if program is executed from bin directory */
int checkBinDir(void) {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    printf("Can't get current directory\n");
    exit(0);
  }

  char **array;
  int size = split(cwd, '/', &array);

  if (!strcmp(array[size - 1], "bin")) {
    return 1;
  }

  return 0;
}

/* creates files for shared memory and semaphores */
void createDirs(char *buffer_name) {
  if (checkBinDir()) {
    char *route = concat("../share_files/", buffer_name);

    if (checkDir(route) == 0) {
      mkdir("../share_files", 0700);
      mkdir("../share_files/global", 0700);
      mkdir("../share_files/sem", 0700);

      mkdir(route, 0700);
    }
  } else {
    char *route = concat("share_files/", buffer_name);

    if (checkDir(route) == 0) {
      mkdir("share_files", 0700);
      mkdir("share_files/global", 0700);
      mkdir("share_files/sem", 0700);

      mkdir(route, 0700);
    }
  }
}

/* check if a string is a number */
int isNumber(char *text) {
  int j;
  j = strlen(text);
  while (j--) {
    if (text[j] > 47 && text[j] < 58)
      continue;

    return 0;
  }
  return 1;
}

/* check if a string is a float */
int isFloat(char *s) {
  const char *ptr = s;
  double x = strtod(ptr, &s);

  if (*s == 0) {
    return 1;
  }

  return 0;
}