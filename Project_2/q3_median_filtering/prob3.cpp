#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "image.h"

int readImage(const char *fname, ImageType& image);
int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type);
int writeImage(const char *fname, ImageType& image);

void median_filter(ImageType & orig, ImageType & result, int k);

int main(int argc, char * argv[]) {

    if (argc != 2) {
        std::cout << "usage: prob3.cpp image.pgm" << std::endl;
        return 1;
    }

    int N, M, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type);

    ImageType image(N, M, Q);
    readImage(argv[1], image);

    ImageType median7_im(image);
    ImageType median15_im(image);

    median_filter(image, median7_im, 7);
    median_filter(image, median15_im, 15);

    std::string filename;

    filename = std::string("median7_") + argv[1];
    writeImage(filename.c_str(), median7_im);

    filename = std::string("median15_") + argv[1];
    writeImage(filename.c_str(), median15_im);

    return 0;
}

// apply averaging to orig with a k x k filter 
void median_filter(ImageType & orig, ImageType & result, int k) {
    int N, M, Q;
    orig.getImageInfo(N, M, Q);

    for (int i =  0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int val;
            std::vector<int> vals;
            for (int r = -k/2; r < (k/2) + 1; r++) {
                for (int s = -k/2; s < (k/2) + 1; s++) {
                    if (i+r >= 0 && i+r < N && j+s >= 0 && j+s < M) {
                        orig.getPixelVal(i+r, j+s, val);
                        vals.push_back(val);
                    }
                }
            }

            std::sort(vals.begin(), vals.end());
            result.setPixelVal(i, j, vals[vals.size()/2]);
        }
    }
}

