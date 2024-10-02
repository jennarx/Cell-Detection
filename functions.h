#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define BMP_WIDTH 950
#define BMP_HEIGTH 950
#define BMP_CHANNELS 3

void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void binary_threshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void binary_erosion(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void cell_detection_function(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void draw_crosses_on_image(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);

void test_assert_equal();
void test_invert();
void test_greyscale();
void test_binary_threshold();
void test_binary_erosion();

#endif // FUNCTIONS_H