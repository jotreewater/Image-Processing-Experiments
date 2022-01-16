#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include "image.h"

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);
void createHistogram(ImageType &, int *);
void equalizeHistogram(ImageType &, int *histogram);

int main(int argc, char *argv[])
{
    int i, j;
    int M, N, Q;
    bool type;
    int val;
    int factor;
    int temp;

    // read image header
    readImageHeader(argv[1], N, M, Q, type);

    // allocate memory for the image array

    ImageType image(N, M, Q);

    // read image
    readImage(argv[1], image);

    std::cout << "Making a histogram of:" << argv[1] << std::endl;

    int histogram[255] = {0};

    // sample
    createHistogram(image, histogram);
    std::cout << "hi" << std::endl;
    equalizeHistogram(image, histogram);

    return (1);
}
void createHistogram(ImageType &image, int *histogram)
{
    std::cout << "Histograming" << std::endl;
    int N, M, Q, val;
    image.getImageInfo(N, M, Q);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, val);
            histogram[val] += 1;
        }
    }
    std::ofstream myfile;
    myfile.open("histogram.txt");
    for (int k = 0; k < 256; k++)
    {
        myfile << histogram[k] << std::endl;
    }
    myfile.close();
}
void equalizeHistogram(ImageType &image, int *histogram)
{
    std::cout << "Equalizing" << std::endl;
    int N, M, Q, val;
    N = 256;
    M = 256;
    Q = 255;
    int equalized_histogram[256];
    int number_of_pixels = N * M;
    int original_value = 0;
    float equalized_value = 0.0f;
    std::ofstream myfile2;
    myfile2.open("equalized.txt");
    for (int i = 0; i < 256; i++)
    {
        original_value += histogram[i];
        equalized_value = (float)original_value / (float)number_of_pixels;
        equalized_value *= Q;
        equalized_value = round(equalized_value);
        myfile2 << equalized_value << "\n";
        equalized_histogram[i] = equalized_value;
    }
    myfile2.close();
    ImageType equalized_image(N, M, Q);
    int old_val, new_val;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, old_val);
            new_val = equalized_histogram[old_val];
            equalized_image.setPixelVal(i, j, new_val);
        }
    }
    char string[16] = {"equal.pgm"};
    writeImage(string, equalized_image);
}