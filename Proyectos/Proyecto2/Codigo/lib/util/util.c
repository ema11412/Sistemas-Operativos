#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

/**
 * Check directory if it not exists it will be created
*/
int chkDir(const char *path) {
    struct stat sb;
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Create Directory on path
*/
int createDir(const char *path) {
    if (mkdir(path, 0700) == -1) {
        return 0;
    }
    return 1;
}

/**
 *  Generate a random number with a Exponencial distribution. 
 *  Lambda is the average number.
 *  Based on this code: https://stackoverflow.com/questions/34558230/generating-random-numbers-of-exponential-distribution  
*/
double exponencial(double mean) {
    double u;
    double result;
    u = rand() / (RAND_MAX + 1.0);
    result = -log(1 - u) * mean;
    return result;
}

/**
 *  Generate a random number with a Poisson distribution. 
 *  Lambda is the average number.
 *  Based on this code: https://topic.alibabacloud.com/a/use-c-language-to-generate-poisson-distribution-random-number-instance-source-code_1_31_20012031.html
*/
int poisson(int lambda) {
    int k = 0;
    double p = 1.0;
    double l = exp(-lambda); //It is defined as long double for precision, and exp (-Lambda) is a decimal near 0
    while (p >= l) {
        double f, u;
        srand((unsigned) time(NULL));
        f = (float) (rand() % 100); // Generate a Random number between 0-1
        u = f / 100;
        p *= u;
        k++;
    }
    return k - 1;
}

/**
 * Concat String S1 and s2
*/
char *concat(const char *s1, const char *s2) {
    char *result = (char *) malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/** 
 * Verify id string is a number.
*/
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

/**
 * Prints text in color
 * 1: Bold Red
 * 2: Bold Blue
 * 3: Bold Green
 * 4: Bold Yellow
 * 5: Bold Magenta
 * 6: Bold Cyan
*/
void printc(char *msg, int color) {
    if (color == 1) {
        printf("\033[1;31m");
        printf("%s", msg);
        printf("\033[0m");
    } else if (color == 2) {
        printf("\033[1;34m");
        printf("%s", msg);
        printf("\033[0m");
    } else if (color == 3) {
        printf("\033[1;32m");
        printf("%s", msg);
        printf("\033[0m");
    } else if (color == 4) {
        printf("\033[1;33m");
        printf("%s", msg);
        printf("\033[0m");
    } else if (color == 5) {
        printf("\033[1;35m");
        printf("%s", msg);
        printf("\033[0m");
    } else if (color == 6) {
        printf("\033[1;36m");
        printf("%s", msg);
        printf("\033[0m");
    }
}

double expRand(double mean) {
    double u = rand() / (RAND_MAX + 1.0);
    double lambda = 1 / mean;
    double out = -log(u) / lambda;

    return out;
}