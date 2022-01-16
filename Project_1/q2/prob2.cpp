#include <iostream>
#include <string>
#include "image.h"


int readImage(const char *, ImageType &);
int readImageHeader(const char *, int &, int &, int &, bool &);
int writeImage(const char *, ImageType &);

void reduceQuantization(ImageType & image, int L);


int main() {
    int N, M, Q;
    bool type;

    readImageHeader("lenna.pgm", N, M, Q, type);
    ImageType lenna(N, M, Q);
    readImage("lenna.pgm", lenna);

    readImageHeader("peppers.pgm", N, M, Q, type);
    ImageType pepper(N, M, Q);
    readImage("peppers.pgm", pepper);

    for (int L = 128; L > 1; L /= 2) {
        ImageType lennaCopy(lenna);
        ImageType pepperCopy(pepper);

        reduceQuantization(lennaCopy, L);
        reduceQuantization(pepperCopy, L);

        writeImage( ("lenna"+std::to_string(L)+".pgm").c_str(), lennaCopy);
        writeImage( ("pepper"+std::to_string(L)+".pgm").c_str(), pepperCopy);
    }
    
    return 0;
}


// Reduces quantization levels of image to L levels
//
// this function makes the assumption that the original
// quantization level is divisible by L
void reduceQuantization(ImageType & image, int L) {
    int N, M, Q;

    image.getImageInfo(N, M, Q);

    int k = Q / L;

    int val;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);
            image.setPixelVal(i, j, val - val%k);
        }
    }


}

