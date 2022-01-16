#include <iostream>
#include <algorithm>
#include <complex>
#include <cmath>
#include <vector>
#include <cfloat>
#include "image.h"

using std::complex, std::vector;

int readImage(const char *fname, ImageType& image);
int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type);
int writeImage(const char *fname, ImageType& image);
void fft(float data[], unsigned long nn, int isign);

void fft2D(vector<vector<complex<float>>> & a, int N, int isign);
void toArray(ImageType & img, vector<vector<complex<float>>> & a, int N);
void toImage(ImageType & img, vector<vector<complex<float>>> & a, int N);
void fromFreqtoImage(ImageType &img, vector<vector<complex<float>>> &a, int N);

const double PI = std::acos(-1);

// H applies motion blur when multiplied in the frequency domain
complex<float> H(int u, int v) {
    if (u + v == 0) return 1;

    float tmp = PI *(0.1*u + 0.1*v);
    complex<float> result = (1/tmp) * std::sin(tmp) * std::exp(complex<float>(0, -1.0f * tmp));

    if (std::sqrt(result.real()*result.real() + result.imag()*result.imag()) < 1e-7) return 1;

    return result;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Usage: ./inverse image.pgm <radius>" << std::endl;
        return -1;
    }


    /* image initialization */

    int N, M, Q;
    bool type;
    readImageHeader(argv[1], N, M, Q, type);
    
    if (N != M) {
        std::cout << "Image must have equal number of rows and columns" << std::endl;
        return -1;
    }

    ImageType img(N, M, Q);
    readImage(argv[1], img);


    /* fft stuff */

    std::cout << "Taking fft" << std::endl;
    
    // initialize array
    vector<vector<complex<float>>> a(N, vector<complex<float>>(N, 0));

    // convert the image to a complex array
    toArray(img, a, N);

    // multiply by (-1)^(i+j) to center resulting spectra
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((i+j) % 2 == 1) {
                a[i][j] *= -1.0f;
            }
        }
    }

    // apply 2d fft on a
    fft2D(a, N, -1);

    int n = N/2;
    float radius = atof(argv[2]);
    // divide by H(u,v) to get the inverse
    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            // check if inside the specified radius
            if ((u-n)*(u-n) + (v-n)*(v-n) <= radius*radius) {
                a[u][v] /= H(u - n, v - n);
            }
        }
    }

    // take inverse fft 
    fft2D(a, N, 1);

    // undo centering
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((i+j) % 2 == 1) {
                a[i][j] *= -1.0f;
            }
        }
    }

    /* output */

    ImageType output(n, n, Q);

    toImage(output, a, n);

    writeImage("output.pgm", output);
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