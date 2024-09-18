//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
      output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}
  //Function to greyscale pixels of an image by taking the average of the RGB values
  void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
    for (int x = 0; x < BMP_WIDTH; x++)
    {
      for (int y = 0; y < BMP_HEIGTH; y++)
      {
        unsigned char average = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
        for (int c = 0; c < BMP_CHANNELS; c++)
        {
        output_image[x][y][c] = average;
        }
      }
    }
  }

    //Apply binary threshold to an image with a given threshold of 90
    void binary_threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
      for (int x = 0; x < BMP_WIDTH; x++)
      {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
          unsigned char average = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
          for (int c = 0; c < BMP_CHANNELS; c++)
          {
            if (average > 90)
            {
              output_image[x][y][c] = 255;
            }
            else
            {
              output_image[x][y][c] = 0;
            }
          }
        }
      }
    }

  //Apply binary erosion to an image
  //the structuring element is a 3x3 matrix with 0 in the corners and 1 in the center and sides
  //if any of the neighboring pixels are 0, the center pixel is set to 0
  //where the neighboring pixels are the pixels above, below, left, and right of the center pixel
  //if any neighboring pixel is outside the image, it is considered to be 0
  //print the output image of every single iteration of the erosion process to the folder "test_erosion" 
  //with the name "erosion_x.bmp" where x is the iteration number 
  // (e.g. erosion_0.bmp, erosion_1.bmp, erosion_2.bmp, etc.)
  // stop the process when the output image is all black (all pixels are 0)
  void binary_erosion(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
    int iteration = 0;
    int black = 0;
    while (black == 0)
    {
      black = 1;
      for (int x = 0; x < BMP_WIDTH; x++)
      {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
          for (int c = 0; c < BMP_CHANNELS; c++)
          {
            if (x > 0 && x < BMP_WIDTH - 1 && y > 0 && y < BMP_HEIGTH - 1)
            {
              if (input_image[x - 1][y][c] == 0 || input_image[x + 1][y][c] == 0 || input_image[x][y - 1][c] == 0 || input_image[x][y + 1][c] == 0)
              {
                output_image[x][y][c] = 0;
              }
              else
              {
                output_image[x][y][c] = 255;
                black = 0;
              }
            }
            else
            {
              output_image[x][y][c] = 0;
            }
          }
        }
      }
      char filename[100];
      sprintf(filename, "test_erosion/erosion_%d.bmp", iteration);
      write_bitmap(output_image, filename);
      iteration++;
      for (int x = 0; x < BMP_WIDTH; x++)
      {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
          for (int c = 0; c < BMP_CHANNELS; c++)
          {
            input_image[x][y][c] = output_image[x][y][c];
          }
        }
      }
    }
  }


  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image1[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image2[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image3[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];


//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
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

// Run binary erosion
binary_erosion(output_image2, output_image3);

// Save image to file
write_bitmap(output_image3, argv[2]);

printf("Done!\n");
return 0;
}
