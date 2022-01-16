// Project 2 - CS474 Image Processing - Joseph Trierweiler - Problem 1, Noise, Problem 4

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);

int main(int argc, char *argv[])
{
    std::cout << "Start." << std::endl;
    if (argc < 2)
    {
        std::cout << "Execution Format: './p2 <Image.pgm> <Mask.pgm> <OutputImage.pgm>'" << std::endl;
        std::cout << "The output will be named what the user inputs for argv[3]" << std::endl;
    }
    // Reads image header
    int image_N, image_M, image_Q, image_val;
    bool image_type;
    readImageHeader(argv[1], image_N, image_M, image_Q, image_type);
    ImageType image(image_N, image_M, image_Q);
    readImage(argv[1], image);

    // Reads mask
    int mask_N, mask_M, mask_Q, mask_val;
    bool mask_type;
    readImageHeader(argv[2], mask_N, mask_M, mask_Q, mask_type);
    ImageType mask(mask_N, mask_M, mask_Q);
    readImage(argv[2], mask);

    // Output image
    int output_N = image_N;
    int output_M = image_M;
    int output_Q = image_Q;
    ImageType output(output_N, output_M, output_Q);

    // Correlation
    // hm
    // Start by calculating correlation weights

    int weight_array[image_N][image_M];
    for (int image_i = 0; image_i < image_N; image_i++)
    {
        for (int image_j = 0; image_j < image_M; image_j++)
        {
            int sum = 0;
            for (int mask_i = (-1 * mask_N / 2); mask_i < (mask_N / 2); mask_i++)
            {
                for (int mask_j = ((-1) * mask_M / 2); mask_j < (mask_M / 2); mask_j++)
                {
                    if (image_i + mask_i < 0 || image_j + mask_j < 0 || image_i + mask_i >= image_N || image_j + mask_j >= image_M)
                    {
                        sum += 0;
                    }
                    else
                    {
                        image.getPixelVal(image_i + mask_i, image_j + mask_j, image_val);
                        mask.getPixelVal(mask_i + mask_N / 2, mask_j + mask_M / 2, mask_val);
                        sum += image_val * mask_val;
                    }
                }
            }
            weight_array[image_i][image_j] = sum;
        }
    }

    // Scale weights down to match image_Q

    int temp, max;
    int min = image_Q;
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            temp = weight_array[i][j];
            if(temp > max){max = temp;}
            if(temp < min){min = temp;}
        }
    }
    float adjusted_sum;
    float scale = (float)image_Q / ((float)max - (float)min);
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            adjusted_sum = weight_array[i][j] * scale;
            output.setPixelVal(i, j, (int)adjusted_sum);
        }
    }

    // Write image
    writeImage(argv[3], output);
    std::cout << "Finish." << std::endl;
    return (1);
}