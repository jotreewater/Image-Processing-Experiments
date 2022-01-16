/*
 * generate_square.cpp
 *
 * Author: Logan Leavitt
 *
 * Generates the images used for experiment 2, which contain a square in the middle of the image.
 *
 */

#include <iostream>
#include <string>
#include "image.h"

void writeImage(const char* fname, ImageType & image);

const int N = 512, M = 512, Q = 255;


int main() {
    ImageType img(N, M, Q);
    
    const int sizes[] = { 32, 64, 128 };
    std::string filename;
    for (int t = 0; t < 3; t++) {

        // create square
        for (int i = N/2 - sizes[t]/2; i < N/2 + sizes[t]/2; i++) {
            for (int j = M/2 - sizes[t]/2; j < M/2 + sizes[t]/2; j++) {
                img.setPixelVal(i, j, Q);
            }
        }

        // write image to file
        filename = "square" + std::to_string(sizes[t]) + ".pgm";
        writeImage(filename.c_str(), img);
    }

    return 0;
}
