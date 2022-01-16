// Project 2 - CS474 Image Processing - Joseph Trierweiler - Problem 1, Noise, Problem 4

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

#define MASK_N 7
#define MASK_M 7

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);

int main(int argc, char *argv[])
{
    std::cout << "Start." << std::endl;
    if (argc < 2)
    {
        std::cout << "Execution Format: './p2 <Image.pgm> <Unsharp.pgm> <Sharp.pgm> <High.pgm>'" << std::endl;
        std::cout << "The unsharp will be named what the user inputs for argv[3]" << std::endl;
    }
    // Reads image header
    int image_N, image_M, image_Q, image_val;
    bool image_type;
    readImageHeader(argv[1], image_N, image_M, image_Q, image_type);
    ImageType image(image_N, image_M, image_Q);
    readImage(argv[1], image);

    // 7x7 Gaussian mask
    int mask[MASK_N][MASK_M] =
        {{1, 1, 2, 2, 2, 1, 1},
         {1, 2, 2, 4, 2, 2, 1},
         {2, 2, 4, 8, 4, 2, 2},
         {2, 4, 8, 16, 8, 4, 2},
         {2, 2, 4, 8, 4, 2, 2},
         {1, 2, 2, 4, 2, 2, 1},
         {1, 1, 2, 2, 2, 1, 1}};

    // Unsharp image
    int unsharp_val;
    int unsharp_N = image_N;
    int unsharp_M = image_M;
    int unsharp_Q = image_Q;
    ImageType unsharp(unsharp_N, unsharp_M, unsharp_Q);

    // Sharp image
    int sharp_val;
    int sharp_N = image_N;
    int sharp_M = image_M;
    int sharp_Q = image_Q;
    ImageType sharp(sharp_N, sharp_M, sharp_Q);

    // High image
    int high_val;
    int high_N = image_N;
    int high_M = image_M;
    int high_Q = image_Q;
    ImageType high(high_N, high_M, high_Q);

    // Unsharp + High Pass

    // Start by calculating unsharp weights

    int weight_array[image_N][image_M];
    for (int image_i = 0; image_i < image_N; image_i++)
    {
        for (int image_j = 0; image_j < image_M; image_j++)
        {
            int sum = 0;
            for (int mask_i = (-1 * MASK_N / 2); mask_i < (MASK_N / 2); mask_i++)
            {
                for (int mask_j = ((-1) * MASK_M / 2); mask_j < (MASK_M / 2); mask_j++)
                {
                    if (image_i + mask_i < 0 || image_j + mask_j < 0 || image_i + mask_i >= image_N || image_j + mask_j >= image_M)
                    {
                        sum += 0;
                    }
                    else
                    {
                        image.getPixelVal(image_i + mask_i, image_j + mask_j, image_val);
                        sum += image_val * mask[mask_i + MASK_N / 2][mask_j + MASK_M / 2];
                    }
                }
            }
            weight_array[image_i][image_j] = sum;
        }
    }

    // Scale weights down to match image_Q

    int temp, max;
    int min = image_Q;
    for (int i = 0; i < image_N; i++)
    {
        for (int j = 0; j < image_M; j++)
        {
            temp = weight_array[i][j];
            if (temp > max)
            {
                max = temp;
            }
            if (temp < min)
            {
                min = temp;
            }
        }
    }
    float adjusted_sum;
    float scale = (float)image_Q / ((float)max - (float)min);
    for (int i = 0; i < image_N; i++)
    {
        for (int j = 0; j < image_M; j++)
        {
            adjusted_sum = weight_array[i][j] * scale;
            unsharp.setPixelVal(i, j, (int)adjusted_sum);
        }
    }

    // Write unsharp image
    writeImage(argv[2], unsharp);

    // Calculate sharp image
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            image.getPixelVal(i,j,image_val);
            unsharp.getPixelVal(i,j,unsharp_val);
            sharp_val = image_val - unsharp_val;
            sharp.setPixelVal(i, j, sharp_val);
        }
    }

    // Write sharp image
    writeImage(argv[3], sharp);

    // Calculate high image
    int k = 2;
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            image.getPixelVal(i,j,image_val);
            sharp.getPixelVal(i,j,sharp_val);
            high_val = image_val + k * sharp_val;
            if(high_val > image_Q){
                high_val = 255;
            }
            else if(high_val < 0){
                high_val = 0;
            }
            high.setPixelVal(i, j, high_val);
        }
    }

    // Write high image
    writeImage(argv[4], high);

    std::cout << "Finish." << std::endl;
    return (1);
}