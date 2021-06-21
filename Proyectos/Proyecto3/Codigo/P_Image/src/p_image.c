/*
Based on code by https://github.com/PhilGabardo/PPM-Filtering
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <structs.h>
#include <ppm.h>
#include <process.h>
#include <print.h>

int main(int argc, char** argv)
{
		
		rgb *noisy_image, *filtered_image;

		// image attributes
		int total_width, total_height;
		int width, height, max;
		dimension *dim;

		double start_time = 0.0, run_time;

		int offset;

		int name_len;


		/* Prints initialize process */
		printc("Starting process \n", 2);

		int tag = 0;

		char ppm_convert_in[100];
		char ppm_convert_out[100];

		/* read image and sends images part info */
		

		sprintf(ppm_convert_in, "convert %s tmp_in.ppm", argv[1]);

		system(ppm_convert_in);

		system("convert tmp_in.ppm -compress none tmp_in.ppm");


		noisy_image = read_ppm("tmp_in.ppm", &total_width, &total_height, &max);

		
		/* Process image */
		
		int key = (int)(argv[3]);

		filtered_image = process_image(total_width, total_height, noisy_image, key);
		/* Prints initialize process */
		printc("Image filtered Done! \n", 1);

		write_ppm("tmp_out.ppm", total_width, total_height, max, filtered_image);

		free(noisy_image);

		sprintf(ppm_convert_out, "convert tmp_out.ppm %s", argv[2]);
		system(ppm_convert_out);

		remove("tmp_out.ppm");
		remove("tmp_in.ppm");
		

		/* Prints initialize process */
		printc("End process \n", 6);

		return(0);
}