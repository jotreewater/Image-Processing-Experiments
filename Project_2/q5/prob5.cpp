#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "image.h"


std::vector<std::vector<double>> prewitt_y = {
    { -1, -1, -1 },
    { 0, 0, 0 },
    { 1, 1, 1 }
};

std::vector<std::vector<double>> prewitt_x = {
    { -1, 0, 1 },
    { -1, 0, 1 },
    { -1, 0, 1 }
};

std::vector<std::vector<double>> sobel_y = {
    { -1, -2, -1 },
    { 0, 0, 0 },
    { 1, 2, 1 }
};

std::vector<std::vector<double>> sobel_x = {
    { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 }
};

std::vector<std::vector<double>> laplacian = {
    { 0, -1, 0 },
    { -1, 4, -1 },
    { 0, -1, 0 },
};


int readImage(const char *fname, ImageType& image);
int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type);
int writeImage(const char *fname, ImageType& image);

void apply_filter(ImageType & orig,
        ImageType & result,
        const std::vector<std::vector<double>> & filter);

void gradient_magnitude(ImageType & partial_y, ImageType & partial_x, ImageType & mag);

void sharpen(ImageType & orig, ImageType & edges, ImageType & result);

void normalize(ImageType & image);

int main(int argc, char * argv[]) {

    if (argc != 2) {
        std::cout << "usage: prob2.cpp image.pgm" << std::endl;
        return 1;
    }


    int N, M, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type);

    ImageType image(N, M, Q);
    readImage(argv[1], image);

    ImageType prewitt_y_im(image);
    ImageType prewitt_x_im(image);
    ImageType sobel_y_im(image);
    ImageType sobel_x_im(image);
    ImageType laplacian_im(image);

    apply_filter(image, prewitt_y_im, prewitt_y);
    apply_filter(image, prewitt_x_im, prewitt_x);
    apply_filter(image, sobel_y_im, sobel_y);
    apply_filter(image, sobel_x_im, sobel_x);
    apply_filter(image, laplacian_im, laplacian);

    ImageType prewitt_mag(image);
    ImageType sobel_mag(image);
    
    gradient_magnitude(prewitt_y_im, prewitt_x_im, prewitt_mag);
    gradient_magnitude(sobel_y_im, sobel_x_im, sobel_mag);

    ImageType prewitt_result(image);
    ImageType sobel_result(image);
    ImageType laplacian_result(image);

    sharpen(image, prewitt_mag, prewitt_result);
    sharpen(image, sobel_mag, sobel_result);
    sharpen(image, laplacian_im, laplacian_result);


    std::string filename;

    filename = std::string("prewitt_y_") + argv[1];
    normalize(prewitt_y_im);
    writeImage(filename.c_str(), prewitt_y_im);

    filename = std::string("prewitt_x_") + argv[1];
    normalize(prewitt_x_im);
    writeImage(filename.c_str(), prewitt_x_im);

    filename = std::string("sobel_y_") + argv[1];
    normalize(sobel_y_im);
    writeImage(filename.c_str(), sobel_y_im);

    filename = std::string("sobel_x_") + argv[1];
    normalize(sobel_x_im);
    writeImage(filename.c_str(), sobel_x_im);

    filename = std::string("laplacian_") + argv[1];
    normalize(laplacian_im);
    writeImage(filename.c_str(), laplacian_im);

    filename = std::string("prewitt_mag_") + argv[1];
    normalize(prewitt_mag);
    writeImage(filename.c_str(), prewitt_mag);

    filename = std::string("sobel_mag_") + argv[1];
    normalize(sobel_mag);
    writeImage(filename.c_str(), sobel_mag);

    filename = std::string("prewitt_result_") + argv[1];
    writeImage(filename.c_str(), prewitt_result);

    filename = std::string("sobel_result_") + argv[1];
    writeImage(filename.c_str(), sobel_result);

    filename = std::string("laplacian_result_") + argv[1];
    writeImage(filename.c_str(), laplacian_result);

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

void gradient_magnitude(ImageType & partial_y, ImageType & partial_x, ImageType & mag) {
    int N, M, Q;
    partial_x.getImageInfo(N, M, Q);

    int val_y, val_x;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            partial_y.getPixelVal(i, j, val_y);
            partial_x.getPixelVal(i, j, val_x);

            mag.setPixelVal(i, j, (int) std::sqrt(val_y*val_y + val_x*val_x));
        }
    }
}

void sharpen(ImageType & orig, ImageType & edges, ImageType & result) {
    int N, M, Q;
    orig.getImageInfo(N, M, Q);

    int val1, val2;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            orig.getPixelVal(i, j, val1);
            edges.getPixelVal(i, j, val2);
            result.setPixelVal(i, j, std::max(0, std::min(255, val1 + val2)));
        }
    }
}

void normalize(ImageType & image) {
    int N, M, Q;
    image.getImageInfo(N, M, Q);

    int min_val, max_val, val;
    image.getPixelVal(0,0,min_val);
    image.getPixelVal(0,0,max_val);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);

            if (val < min_val) min_val = val;

            if (val > max_val) max_val = val;

        }
    }

    double new_val;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.getPixelVal(i, j, val);
            new_val = 255 * (double) (val - min_val) / (double)(max_val - min_val);
            image.setPixelVal(i, j, (int) new_val);
        }
    }
}
