#include <iostream>
#include <algorithm>
#include <complex>
#include <cmath>
#include <vector>
#include <cfloat>
#include "image.h"

int readImage(const char *fname, ImageType& image);
int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type);
int writeImage(const char *fname, ImageType& image);
void fft(float data[], unsigned long nn, int isign);

void fft2D(std::vector<std::vector<std::complex<float>>> & a, int N, int isign);
void toArray(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N);
void toImage(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N);
void fromFreqtoImage(ImageType &img, std::vector<std::vector<std::complex<float>>> &a, int N);
void rescale(std::vector<std::vector<std::complex<float>>> &a, int N);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        std::cout << "usage: ./q3 image.pgm" << std::endl;
        return -1;
    }

    int N, M, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type);

    if (N != M) {
        std::cout << "Image must have equal number of rows and columns" << std::endl;
        return -1;
    }

    ImageType img(N, M, Q);
    readImage(argv[1], img);


    std::vector<std::vector<std::complex<float>>> a(N, std::vector<std::complex<float>>(N));
    toArray(img, a, N);

    std::vector<std::vector<std::complex<float>>> a1(a);
    fft2D(a1, N, -1);
    std::vector<std::vector<std::complex<float>>> a2(a1);


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ( (i+j) % 2 == 1) {
                a[i][j] *= -1.0;
            }
        }
    }

    fft2D(a, N, -1);
    fromFreqtoImage(img, a, N);
    writeImage("fft.pgm", img);



    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a1[i][j] = std::complex<float>(
                    sqrt(a1[i][j].real()*a1[i][j].real() + a1[i][j].imag()*a1[i][j].imag()), 0);
        }
    }

    fft2D(a1, N, 1);
    toImage(img, a1, N);
    writeImage("no_phase.pgm", img);


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a2[i][j] != std::complex<float>(0)) {
                a2[i][j] /= sqrt(a2[i][j].real()*a2[i][j].real() + a2[i][j].imag()*a2[i][j].imag());
            }
        }
    }

    fft2D(a2, N, 1);
    rescale(a2, N);
    toImage(img, a2, N);
    writeImage("no_mag.pgm", img);


    return 0;
}

void fft2D(std::vector<std::vector<std::complex<float>>> & a, int N, int isign) {
    float data[2*N+1];

    // dft on the rows
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            data[2*j + 1] = a[i][j].real();
            data[2*j + 2] = a[i][j].imag();
        }

        fft(data, N, isign);

        for (int j = 0; j < N; j++) {
            a[i][j] = std::complex<float>(data[2*j + 1], data[2*j + 2]);
        }
    }

    // dft on the columns
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            data[2*j + 1] = a[j][i].real();
            data[2*j + 2] = a[j][i].imag();
        }

        fft(data, N, isign);

        for (int j = 0; j < N; j++) {
            a[j][i] = std::complex<float>(data[2*j + 1], data[2*j + 2]) / (float) N;
        }
    }
}

void toArray(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N) {
    int val;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            img.getPixelVal(i, j, val);

            a[i][j] = val;
        }
    }
}


// assumes array is back in the spatial domain and only has real components
void toImage(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            img.setPixelVal(i, j, std::max(0, std::min(255, (int) a[i][j].real())));
        }
    }
}


// assumes array is the frequency domain
// visualizes as c * log(1 + magnitude), where c is such that the max magnitude gets mapped to 255
void fromFreqtoImage(ImageType &img, std::vector<std::vector<std::complex<float>>> &a, int N) {
    std::vector<std::vector<float>> mags(N, std::vector<float>(N, 0));

    float max_mag = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mags[i][j] = log(1 + a[i][j].real()*a[i][j].real() + a[i][j].imag()*a[i][j].imag());

            if (mags[i][j] > max_mag) {
                max_mag = mags[i][j];
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (max_mag > 0) mags[i][j] *= (255/max_mag);


            img.setPixelVal(i, j, (int) mags[i][j]);
        }
    }
}


void rescale(std::vector<std::vector<std::complex<float>>> &a, int N) {

    float min_val = FLT_MAX, max_val = FLT_MIN;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j].real() < min_val) min_val = a[i][j].real();
            if (a[i][j].real() > max_val) max_val = a[i][j].real();
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = std::complex<float>( (255.0f/(max_val - min_val)) * (a[i][j].real() - min_val));
        }
    }
}
