// Project 4 - CS474 Image Processing - Joseph Trierweiler - Experiment 4

#include <iostream>
#include <algorithm>
#include <complex>
#include <cmath>
#include <vector>
#include <cfloat>

#include "image.h"

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

using std::complex; 
using std::vector;

complex<float> H(int u, int v) {
    double gamma_h = 1.75f;
    double gamma_l = 0.25f;
    double d_0 = 1.8f;
    return (gamma_h - gamma_l) * (1.0 - exp(-(double)((u*u)+(v*v))/(d_0*d_0)))+gamma_l;
}

int readImageHeader(char[], int &, int &, int &, bool &);
int readImage(char[], ImageType &);
int writeImage(char[], ImageType &);

void fft(float data[], unsigned long nn, int isign);

void fft2D(std::vector<std::vector<std::complex<float>>> & a, int N, int isign);
void toArray(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N);
void toImage(ImageType & img, std::vector<std::vector<std::complex<float>>> & a, int N);
void fromFreqtoImage(ImageType &img, std::vector<std::vector<std::complex<float>>> &a, int N);

int main(int argc, char *argv[])
{
    std::cout << "Start." << std::endl;
    if (argc < 2)
    {
        std::cout << "Execution Format: './p2 <Image.pgm> <Result.pgm>'" << std::endl;
        std::cout << "Should apply high-frequency emphasis filter with parameters set in H(u,v) on line 17" << std::endl;
    }
    // Reads image header
    int image_N, image_M, image_Q, image_val;
    bool image_type;
    readImageHeader(argv[1], image_N, image_M, image_Q, image_type);
    ImageType image(image_N, image_M, image_Q);
    readImage(argv[1], image);

    // Resulting Image
    int result_N = image_N;
    int result_M = image_M;
    int result_Q = image_Q;
    ImageType result(result_N, result_M, result_Q);

    vector<vector<complex<float>>> a(2*image_N, vector<complex<float>>(2*image_N, 0));

    toArray(image, a, image_N);

    // Decorrelate
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            a[i][j] = log(a[i][j]);
        }
    }

    fft2D(a, 2*image_N, -1);

    for (int u = 0; u < image_N; u++) {
        for (int v = 0; v < image_N; v++) {
            a[u][v] *= H(u,v);
        }
    }

    fft2D(a, 2*image_N, 1);

    // Recorrelate
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            a[i][j] = exp(a[i][j]);
        }
    }

    toImage(image, a, image_N);

    int temp;
    for(int i = 0; i < image_N; i++){
        for(int j = 0; j < image_M; j++){
            image.getPixelVal(i,j,temp);
            result.setPixelVal(i,j,temp);
        }
    }

    // Write Result
    writeImage(argv[2], result);

    std::cout << "Finish." << std::endl;
    return (1);
}

void fft(float data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
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
            img.setPixelVal(i, j, (int) a[i][j].real());
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