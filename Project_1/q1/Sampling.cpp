#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);

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

    std::cout << "Enter what factor you would like to sub-sample with: ";
    std::cin >> factor;

    // sample

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            if ((i % factor == 0) && (j % factor == 0))
            {
                image.getPixelVal(i, j, val);
                image.setPixelVal(i, j, val);
                temp = val;
                //std::cout << temp << std::endl;
            }
            else if ((i % factor != 0) && (j % factor == 0))
            {
                image.getPixelVal(i - (i % factor), j, val);
                image.setPixelVal(i, j, val);
                temp = val;
                //std::cout << temp << std::endl;
            }
            else
            {
                image.setPixelVal(i, j, temp);
                //std::cout << temp << std::endl;
            }
        }
    }

    // write image
    writeImage(argv[2], image);

    return (1);
}