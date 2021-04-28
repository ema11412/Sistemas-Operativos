#ifndef _UTILITIES_H
#define _UTILITIES_H

/* Library to used various general functions */

int split(char *str, char c, char ***arr);
int checkDir(char *path);
void createDirs(char *buffer_name);
char *concat(const char *s1, const char *s2);
int checkBinDir(void);
int isNumber(char *text);
int isFloat(char *s);

#endif
