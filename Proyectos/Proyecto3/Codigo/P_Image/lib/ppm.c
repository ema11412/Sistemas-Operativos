#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <structs.h>

/* write on ppm file */
void write_ppm(char* file, int width, int height, int max, const rgb *image)
{
    FILE *fd;
    fd = fopen(file, "w");

    /* write header */
    fprintf(fd, "P3\n");
    fprintf(fd, "%d %d\n%d\n", width, height, max);

    /* write image */
    for (int i = 0; i < height * width; i++)
    {
        const rgb *p = image + i;
        fprintf(fd, "%d %d %d ", p->r, p->g, p->b);
    }

    fclose(fd);
}

/* read ppm file */
rgb *read_ppm(char* file, int* width, int* height, int* max)
{
    FILE *fd;
    char c;
    int i, n;
    char b[100];
    int red, green, blue;

    /* check format */
    fd = fopen(file, "r");
    n = fscanf(fd, "%[^\n] ", b);

    if (b[0] != 'P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n", file);
        exit(0);
    }

    // printf("%s is a PPM file\n", file);
    n = fscanf(fd, "%c", &c);

    while (c == '#')
    {
        n = fscanf(fd, "%[^\n] ", b);
        printf("%s\n", b);
        n = fscanf(fd, "%c", &c);
    }

    ungetc(c, fd);
    n = fscanf(fd, "%d %d %d", width, height, max);
    assert(n == 3);

    // printf("%d x %d image, max value= %d\n", *width, *height, *max);

    int size = *width * (*height);

    rgb *image = (rgb*)malloc(size*sizeof(rgb));
    assert(image);

    for (i = 0; i < size; i++)
    {
        n =  fscanf(fd, "%d %d %d", &red, &green, &blue);
        assert(n == 3);
        image[i].r = red;
        image[i].g = green;
        image[i].b = blue;
    }

    return image;
}

