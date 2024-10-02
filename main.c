#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "cbmp.h"
#include <time.h>


//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image1[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image2[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image3[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image4[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

//Main function
int main(int argc, char** argv){
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3){
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }
  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  // invert(input_image, output_image);

  // Run greyscale
  greyscale(input_image, output_image1);

  // Run binary threshold
  binary_threshold(output_image1, output_image2);

  // Run binary erosion and cell detection
  binary_erosion(output_image2, output_image3);

  //Draws the crosses
  draw_crosses_on_image(input_image, output_image4);
  // Save image to file
  
  write_bitmap(output_image4, argv[2]);
  end = clock();
  cpu_time_used= end - start;
  printf("Total time for overall: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
  printf("Done!\n");
  return 0;
}