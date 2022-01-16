#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include "image.h"

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);
void createHistogram(ImageType &, int *);
void getEqualizedHistogram(ImageType &, int *histogram, int *result);
void getSpecifiedTransformation(int *T, int *G, int *result);
ImageType applyTransformation(ImageType &image, int *H);
void writeHistogram(char[], int *);


int main(int argc, char *argv[])
{
    int i, j;
    int M, N, Q;
    bool type;
    int val;
    int factor;
    int temp;

    // read image to transform
    readImageHeader(argv[1], N, M, Q, type);
    ImageType input_image(N, M, Q);
    readImage(argv[1], input_image);

    // read image which specifies the histogram
    readImageHeader(argv[2], N, M, Q, type);
    ImageType specified_image(N, M, Q);
    readImage(argv[2], specified_image);


    std::cout << "Making a histogram of: " << argv[1] << std::endl;

    int histogram[256] = {0};
    createHistogram(input_image, histogram);

    std::cout << "Making a histogram of: " << argv[2] << std::endl;

    int specified_hist[256] = {0};
    createHistogram(specified_image, specified_hist);

    std::cout << "Getting the equalizing transformation for " << argv[1] << std::endl;

    int T[256] = {0};
    getEqualizedHistogram(input_image, histogram, T);

    std::cout << "Getting the equalizing transformation for " << argv[2] << std::endl;

    int G[256] = {0};
    getEqualizedHistogram(specified_image, specified_hist, G);

    std::cout << "Getting the final transformation, and applying it to " << argv[1] << std::endl;
    int H[256] = {0};
    getSpecifiedTransformation(T, G, H);
    ImageType result = applyTransformation(input_image, H);
    
    char output[] = "result.pgm";
    writeImage(output, result);

    char output1[] = "input_hist.txt";
    writeHistogram(output1, histogram);

    char output2[] = "specified_hist.txt";
    writeHistogram(output2, specified_hist);

    int result_hist[256] = {0};
    createHistogram(result, result_hist);
    char output3[] = "result_hist.txt";
    writeHistogram(output3, result_hist);

    return 0;
}


void createHistogram(ImageType &image, int *histogram)
{
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
}


void getEqualizedHistogram(ImageType &image, int *histogram, int *result)
{
    int N, M, Q, val;
    image.getImageInfo(N, M, Q);

    int number_of_pixels = N * M;
    int original_value = 0;
    float equalized_value = 0.0f;

    for (int i = 0; i < 256; i++)
    {
        original_value += histogram[i];
        equalized_value = (float)original_value / (float)number_of_pixels;
        equalized_value *= Q;
        equalized_value = round(equalized_value);
        result[i] = equalized_value;
    }
}


void getSpecifiedTransformation(int *T, int *G, int *result)
{
    for (int i = 0; i < 256; i++)
    {
        int best_j = 0;
        for (int j = 1; j < 256; j++)
        {
            if (abs(G[j] - T[i]) < abs(G[best_j] - T[i]))
            {
                best_j = j;
            }
        }
        result[i] = best_j;
    }
}


ImageType applyTransformation(ImageType &image, int *H)
{
    int N, M, Q, val;
    image.getImageInfo(N, M, Q);
    ImageType new_image(N, M, Q);

    int old_val, new_val;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            image.getPixelVal(i, j, old_val);
            new_val = H[old_val];
            new_image.setPixelVal(i, j, new_val);
        }
    }

    return new_image;
}


void writeHistogram(char filename[], int * hist)
{
    std::ofstream file;
    file.open(filename);

    for (int i = 0; i < 256; i++)
    {
        file << hist[i] << std::endl;
    }


    file.close();
}
