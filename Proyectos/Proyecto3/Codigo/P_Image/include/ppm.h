#ifndef _PPM_H
#define _PPM_H

/* library to read/write ppm files */
rgb *read_ppm(char* file, int* width, int* height, int* max);
void write_ppm(char* file, int width, int height, int max, const rgb *image);

#endif