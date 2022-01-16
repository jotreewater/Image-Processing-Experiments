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
        std::cout << "Execution Format: './p3 <Image.pgm> <X% Noise> <OutputImage.pgm>'" << std::endl;
        std::cout << "The output will be named what the user inputs for argv[3]" << std::endl;
    }
    // Reads image header
    int image_N, image_M, image_Q, image_val;
    bool image_type;
    readImageHeader(argv[1], image_N, image_M, image_Q, image_type);
    ImageType image(image_N, image_M, image_Q);
    readImage(argv[1], image);

    // Noise generation
    
    float X = std::stof(argv[2]) / 100;
    std::cout << "X:" << X << std::endl;
    int area = image_N * image_M;
    std::cout << "Image area: " << area << std::endl;
    int pixels_affected = X * area;
    std::cout << "Pixels affected: " << pixels_affected << std::endl;
    int x, y;
    for (int i = 0; i < pixels_affected; i++)
    {
        x = std::rand() % image_N;
        y = std::rand() % image_M;
        if (x % 2 == 0)
        {
            image.setPixelVal(x, y, 255);
        }
        else
        {
            image.setPixelVal(x, y, 0);
        }
    }

    // Write image
    writeImage(argv[3], image);
    std::cout << "Finish." << std::endl;
    return (1);
}