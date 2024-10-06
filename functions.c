#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "cbmp.h"
#include <assert.h>
#include <dirent.h>
#include <time.h>
//To compile (linux/mac): gcc cbmp.c functions.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c functions.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

// To compile test (win): gcc cbmp.c functions.c test_main.c -o test_main.exe -std=c99
// To run test (win): test_main.exe

#define Max_celler 1000
int coordinate_x_cells[Max_celler];
int coordinate_y_cells[Max_celler];
int cells = 0;
clock_t start, end;
double cpu_time_used;

//Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  start = clock();
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
  end = clock();
  cpu_time_used= end - start;
  printf("Total time for invert: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
}
  //Function to greyscale pixels of an image by taking the average of the RGB values
  void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
    start = clock();
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
    end = clock();
    cpu_time_used= end - start;
    printf("Total time for grayscale: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
  }

    //Apply binary threshold to an image with a given threshold of 90
    void binary_threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
      start = clock();
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
      end = clock();
      cpu_time_used= end - start;
      printf("Total time for binary_threshold: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
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
    start = clock();
    cell_detection_function(output_image);
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
      cell_detection_function(output_image);
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

    printf("Number of cells: %d\n", cells);
    end = clock();
    cpu_time_used= end - start;
    printf("Total time for binary_erosion: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
  }

void cell_detection_function(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  start = clock();
  //Checks the whole image, by create a 12x12 area to check for white cells
  int checking_area = 12;
  for (int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
      int white = 0;
      for (int i = 0; i <= checking_area; i++){
        for (int j = 0; j <= checking_area; j++){
          if (input_image[x + i][y + j][0] == 255){
            white = 1;
            break;
          }
        }
        if (white == 1) break;
      }
      if (white==0){ //skipper resten af 12x12 hvis der ingen hvide pixels er
        y+=11;
        continue;
      }
      int outer_white = 0;

        //Checks if there are any white cells on the outer layer
      for (int i = -1; i <= checking_area+1; i++){
        if (input_image[x+i][y-1][0]==255 || input_image[x+i][y+checking_area+1][0]==255){
          outer_white = 1;
          break;
        }
        for (int j = -1; j <= checking_area+1; j++){
          if(input_image[x-1][y+j][0]==255 || input_image[x+checking_area+1][y+j][0]==255){
            outer_white = 1;
            break;
          }
        }
      }

      //If there are no white cells in the outer layer, it adds 1 to cells and makes the 12x12 black
      //This also checks if coordinates are already in the array
      if (outer_white == 0){
        int white_cell_exists = 0;
        for (int k=0;k<=Max_celler;k++){
          if (coordinate_x_cells[k]==x && coordinate_y_cells[k]==y){
            white_cell_exists = 1;
            break;
          }
        }
        if (!white_cell_exists){
          cells++;
          coordinate_x_cells[cells] = x;
          coordinate_y_cells[cells] = y;
          for (int i = 0; i <= checking_area; i++){
            for (int j = 0; j <= checking_area; j++){
              input_image[x + i][y + j][0] = 0;
            }
          }
        }
      }
    }
  }
  end = clock();
  cpu_time_used= end - start;
  printf("Total time for cell_detection_function: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
}

void draw_crosses_on_image(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
    start = clock();
    int cross_size = 8; 
    // Copying input image to output image
    for (int x = 0; x < BMP_WIDTH; x++) {
      for (int y = 0; y < BMP_HEIGTH; y++) {
        for (int c = 0; c < BMP_CHANNELS; c++) {
          output_image[x][y][c] = input_image[x][y][c];
        }
      }
    }
    for (int i = 1; i < cells; i++) {
      int x = coordinate_x_cells[i];
      int y = coordinate_y_cells[i];
      //Draws the cross in the middle of the cell (Actually its just the middle of the 12x12 area, but does
      //the trick)
      x+=6;
      y+=6;
      // Draws horizontal line of the cross
      for (int j = -cross_size; j <= cross_size; j++) {
        if (x + j >= 0 && + j < BMP_WIDTH) {
            output_image[x + j][y][0] = 255;
            output_image[x + j][y][1] = 0;
            output_image[x + j][y][2] = 0;
        }
      }
      // Draws vertical line of the cross
      for (int j = -cross_size; j <= cross_size; j++) {
        if (y + j >= 0 && y + j < BMP_HEIGTH) {
          output_image[x][y + j][0] = 255;
          output_image[x][y + j][1] = 0;
          output_image[x][y + j][2] = 0;
      }
    }
  }
  end = clock();
  cpu_time_used= end - start;
  printf("Total time for draw_crosses_on_image: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
}


// Function to compare two images
void assert_equal(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char expected_output[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], const char* test_name) {
    int passed = 1;
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            for (int c = 0; c < BMP_CHANNELS; c++) {
                if (output_image[x][y][c] != expected_output[x][y][c]) {
                    passed = 0;
                    break;
                }
            }
            if (!passed) break;
        }
        if (!passed) break;
    }
    if (passed) {
        printf("%s: images are equal\n", test_name);
    } else {
        printf("%s: images are different\n", test_name);
    }
}

void test_assert_equal() {
    printf("Running test_assert_equal...\n");

    // Allocate memory for images on the heap
    unsigned char (*image1)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*image2)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*image3)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);

    if (!image1 || !image2 || !image3) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Test case where the images are equal
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            for (int c = 0; c < BMP_CHANNELS; c++) {
                image1[x][y][c] = 0;
                image2[x][y][c] = 0;
                image3[x][y][c] = 255;
            }
        }
    }
    assert_equal(image1, image2, "test_assert_equal_equal");

    // Test case where the images are not equal
    assert_equal(image1, image3, "test_assert_equal_not_equal");

    // Free allocated memory
    free(image1);
    free(image2);
    free(image3);

    printf("Completed test_assert_equal.\n");
}

void test_invert() {
    printf("Running test_invert...\n");

    // Allocate memory for images on the heap
    unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*expected_output)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*output_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);

    if (!input_image || !expected_output || !output_image) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Test case where the images are equal
    // Initialize input_image and expected_output
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            input_image[x][y][0] = 0;
            input_image[x][y][1] = 127;
            input_image[x][y][2] = 255;

            expected_output[x][y][0] = 255;
            expected_output[x][y][1] = 128;
            expected_output[x][y][2] = 0;
        }
    }

    // Perform inversion
    invert(input_image, output_image);

    // Assert equality
    assert_equal(output_image, expected_output, "test_invert_equal");

    // Test case where the images are not equal
    // Reset output_image to ensure it matches the expected_output for the second test case
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            output_image[x][y][0] = 255;
            output_image[x][y][1] = 128;
            output_image[x][y][2] = 0;
        }
    }

    // Modify expected_output to make it different
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            expected_output[x][y][0] = 100;
            expected_output[x][y][1] = 100;
            expected_output[x][y][2] = 100;
        }
    }

    // Assert inequality
    assert_equal(output_image, expected_output, "test_invert_not_equal");

    // Free allocated memory
    free(input_image);
    free(expected_output);
    free(output_image);

    printf("Completed test_invert.\n");
}

void test_greyscale() {
    printf("Running test_greyscale...\n");

    // Allocate memory for images on the heap
    unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*expected_output)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*output_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);

    if (!input_image || !expected_output || !output_image) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Test case where the images are equal
    // Initialize input_image and expected_output
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            input_image[x][y][0] = 0;
            input_image[x][y][1] = 127;
            input_image[x][y][2] = 255;

            expected_output[x][y][0] = 127;
            expected_output[x][y][1] = 127;
            expected_output[x][y][2] = 127;
        }
    }

    // Perform greyscale conversion
    greyscale(input_image, output_image);

    // Assert equality
    assert_equal(output_image, expected_output, "test_greyscale_equal");

    // Test case where the images are not equal
    // Reset output_image to ensure it matches the expected_output for the second test case
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            output_image[x][y][0] = 85;
            output_image[x][y][1] = 85;
            output_image[x][y][2] = 85;
        }
    }

    // Modify expected_output to make it different
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            expected_output[x][y][0] = 100;
            expected_output[x][y][1] = 100;
            expected_output[x][y][2] = 100;
        }
    }

    // Assert inequality
    assert_equal(output_image, expected_output, "test_greyscale_not_equal");

    // Free allocated memory
    free(input_image);
    free(expected_output);
    free(output_image);

    printf("Completed test_greyscale.\n");
}

void test_binary_threshold() {
    printf("Running test_binary_threshold...\n");

    // Allocate memory for images on the heap
    unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*expected_output)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    unsigned char (*output_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);

    if (!input_image || !expected_output || !output_image) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Test case where the images are equal
    // Initialize input_image and expected_output
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            input_image[x][y][0] = 0;
            input_image[x][y][1] = 127;
            input_image[x][y][2] = 255;

            expected_output[x][y][0] = 255;
            expected_output[x][y][1] = 255;
            expected_output[x][y][2] = 255;
        }
    }

    // Perform greyscale conversion to prepare the image for binary threshold
    greyscale(input_image, output_image);


    // Perform binary threshold
    binary_threshold(output_image, output_image);

    // Assert equality
    assert_equal(output_image, expected_output, "test_binary_threshold_equal");

    // Test case where the images are not equal
    // Modify expected_output to make it different
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            expected_output[x][y][0] = 100;
            expected_output[x][y][1] = 100;
            expected_output[x][y][2] = 100;
        }
    }

    // Assert inequality
    assert_equal(output_image, expected_output, "test_binary_threshold_not_equal");

    // Free allocated memory
    free(input_image);
    free(expected_output);
    free(output_image);

    printf("Completed test_binary_threshold.\n");
}
