#include <iostream>
#include <vector>
#include <string>
#include "image.h"

std::vector<std::vector<double>> gaussian7 = {
    { 1, 1, 2, 2, 2, 1, 1 },
    { 1, 2, 2, 4, 2, 2, 1 },
    { 2, 2, 4, 8, 4, 2, 2 },
    { 2, 4, 8, 16, 8, 4, 2 },
    { 2, 2, 4, 8, 4, 2, 2 },
    { 1, 2, 2, 4, 2, 2, 1 },
    { 1, 1, 2, 2, 2, 1, 1 }
};

std::vector<std::vector<double>> gaussian15 = {
    { 2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2 },
    { 2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2 },
    { 3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3 },
    { 4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4 },
    { 5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5 },
    { 5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5 },
    { 6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6 },
    { 6, 8, 11, 13, 16, 18, 19, 20, 19, 18, 16, 13, 11, 8, 6 },
    { 6, 8, 10, 13, 15, 17, 19, 19, 19, 17, 15, 13, 10, 8, 6 },
    { 5, 7, 10, 12, 14, 16, 17, 18, 17, 16, 14, 12, 10, 7, 5 },
    { 5, 7, 9, 11, 13, 14, 15, 16, 15, 14, 13, 11, 9, 7, 5 },
    { 4, 5, 7, 9, 10, 12, 13, 13, 13, 12, 10, 9, 7, 5, 4 },
    { 3, 4, 6, 7, 9, 10, 10, 11, 10, 10, 9, 7, 6, 4, 3 },
    { 2, 3, 4, 5, 7, 7, 8, 8, 8, 7, 7, 5, 4, 3, 2 },
    { 2, 2, 3, 4, 5, 5, 6, 6, 6, 5, 5, 4, 3, 2, 2 }
};

int readImage(const char *fname, ImageType& image);
int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type);
int writeImage(const char *fname, ImageType& image);

void averaging(ImageType & orig, ImageType & result, int k);
void apply_filter(ImageType & orig,
        ImageType & result,
        const std::vector<std::vector<double>> & filter);

int main(int argc, char * argv[]) {

    if (argc != 2) {
        std::cout << "usage: ./gaussian image.pgm" << std::endl;
        return 1;
    }


    int sum;

    // normalize the masks first
    sum = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            sum += gaussian7[i][j];
        }
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            gaussian7[i][j] /= sum;
        }
    }

    sum = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            sum += gaussian15[i][j];
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            gaussian15[i][j] /= sum;
        }
    }

    int N, M, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type);

    ImageType image(N, M, Q);
    readImage(argv[1], image);

    ImageType gaussian7_im(image);
    ImageType gaussian15_im(image);

    apply_filter(image, gaussian7_im, gaussian7);
    apply_filter(image, gaussian15_im, gaussian15);

    std::string filename;

    filename = std::string("gaussian7_") + argv[1];
    writeImage(filename.c_str(), gaussian7_im);

    filename = std::string("gaussian15_") + argv[1];
    writeImage(filename.c_str(), gaussian15_im);

    return 0;
}


// apply filter to orig, and store in result
void apply_filter(ImageType & orig,
        ImageType & result,
        const std::vector<std::vector<double>> & filter) {
    int N, M, Q;
    orig.getImageInfo(N, M, Q);

    int k = filter.size();

    for (int i =  0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int val;
            double sum = 0;
            for (int r = 0; r < k; r++) {
                for (int s = 0; s < k; s++) {
                    if (i+r-(k/2) >= 0 && i+r-(k/2) < N && j+s-(k/2) >= 0 && j+s-(k/2) < M) {
                        orig.getPixelVal(i+r-(k/2), j+s-(k/2), val);
                        sum += (double) val * filter[r][s];
                    }
                }
            }


            result.setPixelVal(i, j, (int) sum);
        }
    }
}

