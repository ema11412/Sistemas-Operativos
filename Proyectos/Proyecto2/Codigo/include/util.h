#ifndef _UTIL_H
#define _UTIL_H

int chkDir(const char *path);

int poisson(int lambda);

double exponencial(double lambda);

int createDir(const char *path);

char *concat(const char *s1, const char *s2);

int isNumber(char *text);

void printc(char *msg, int color);

char *int2str(int num);

double expRand(double mean);

#endif 