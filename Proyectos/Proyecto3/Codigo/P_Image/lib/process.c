#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <structs.h>




rgb *process_image(int width, int height, rgb *noisy_image, int key)
{

    // Process pixel by pixel
    rgb *filtered_image = (rgb*)malloc(height * width * sizeof(rgb));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            
            filtered_image[i * width + j].r = (noisy_image[i * width + j].r) ^key;
            filtered_image[i * width + j].g = (noisy_image[i * width + j].g) ^key;
            filtered_image[i * width + j].b = (noisy_image[i * width + j].b) ^key;

    }
    }


    return filtered_image;
}