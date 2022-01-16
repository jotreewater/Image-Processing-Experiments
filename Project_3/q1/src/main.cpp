// Project 3, Question 1

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#define SWAP(a, b) \
    tempr = (a);   \
    (a) = (b);     \
    (b) = tempr
void fft(float data[], unsigned long nn, int isign);
void print_i(float array[], int size);
void print_real(float array[], int size);
int main()
{
    std::cout << "Start." << std::endl;

    // 1 a

    int a_size = 9;
    int a_real_numbers = 4;
    float a[a_size] = {0, 2, 0, 3, 0, 4, 0, 4, 0};
    std::cout << "\n1a. Demonstrate FFT given (2,3,4,4)" << std::endl;
    std::cout << "\nReal:" << std::endl;
    print_real(a, a_size);
    std::cout << "\nImaginary:" << std::endl;
    print_i(a, a_size);
    std::cout << std::endl;

    // Start FFT
    fft(a, a_real_numbers, -1);
    std::cout << "\nAfer Forward Transform" << std::endl;
    std::cout << "\nReal:" << std::endl;
    print_real(a, a_size);
    std::cout << "\nImaginary:" << std::endl;
    print_i(a, a_size);
    std::cout << std::endl;

    // Normalize
    for (int i = 1; i < a_size; i++)
    {
        a[i] = a[i] * 1 / a_real_numbers;
    }
    std::cout << "\nAfer Normalizing" << std::endl;
    std::cout << "\nReal:" << std::endl;
    print_real(a, a_size);
    std::cout << "\nImaginary:" << std::endl;
    print_i(a, a_size);
    std::cout << std::endl;

    // Start Inverse FFT
    fft(a, a_real_numbers, 1);
    //std::cout << "Afer Inverting" << std::endl;
    //std::cout << "Real:" << std::endl;
    print_real(a, a_size);
    //std::cout << "Imaginary:" << std::endl;
    print_i(a, a_size);
    //std::cout << std::endl;

    //std::cout << "Real numbers should be the same: (2,3,4,4)." << std::endl;

    // 1 b
    std::cout << "\n1b. Create a cosine wave with 128 samples and 8 cycles per period." << std::endl;
    float step = 1.0 / 128.0;
    float fx[257] = {0}; // 128*2 + 1
    for (int i = 0; i < 128; i++)
    {
        fx[i] = cos(2.0 * M_PI * 8.0 * (float)(i * step));
        //std::cout << fx[i] << std::endl;
    }
    // std::cout << "Shift Magnitude to the center." << std::endl;
    for (int i = 0; i < 128; i++)
    {
        if (i % 2 == 0)
        {
            fx[i] = fx[i] * 1;
        }
        else
        {
            fx[i] = fx[i] * -1;
        }
        //std::cout << fx[i] << std::endl;
    }
    for (int i = 0; i < 257; i++)
    {
        //std::cout << i << "    " << fx[i] << std::endl;
    }
    // std::cout << "Reformat for FFT function." << std::endl;
    float temp[257] = {0};
    int copy_index = 0;
    for (int i = 0; i < 257; i++)
    {
        if (i % 2 == 1)
        {
            temp[i] = fx[copy_index];
            copy_index++;
        }
        else
        {
            temp[i] = 0;
        }
    }
    for (int i = 0; i < 257; i++)
    {
        fx[i] = temp[i];
    }
    // std::cout << "Applying FFT." << std::endl;
    fft(fx, 128, -1);
    // Normailizing
    for (int i = 1; i < 257; i++)
    {
        fx[i] = fx[i] * 1 / 128;
    }
    std::cout << "\nReal:" << std::endl;
    print_real(fx, 257);
    std::cout << "\nImaginary:" << std::endl;
    print_i(fx, 257);

    // 1c
    std::cout << "\n1c. Perform FFT on a rectangle function." << std::endl;
    float rect[128] = {0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000,
                       1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000,
                       0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000};
    // std::cout << "Shift Magnitude to the center." << std::endl;
    for (int i = 0; i < 128; i++)
    {
        if (i % 2 == 0)
        {
            rect[i] = rect[i] * 1;
        }
        else
        {
            rect[i] = rect[i] * -1;
        }
        //std::cout << rect[i] << std::endl;
    }
    for (int i = 0; i < 257; i++)
    {
        //std::cout << i << "    " << fx[i] << std::endl;
    }
    // std::cout << "Reformat for FFT function." << std::endl;
    temp[257] = {0};
    copy_index = 0;
    for (int i = 0; i < 257; i++)
    {
        if (i % 2 == 1)
        {
            temp[i] = rect[copy_index];
            copy_index++;
        }
        else
        {
            temp[i] = 0;
        }
    }
    for (int i = 0; i < 257; i++)
    {
        //std::cout << temp[i] << std::endl;
    }
    // std::cout << "Applying FFT." << std::endl;
    fft(temp, 128, -1);
    // Normalizing
    for (int i = 1; i < 257; i++)
    {
        temp[i] = temp[i] * 1 / 128;
    }
    std::cout << "\nReal:" << std::endl;
    print_real(temp, 257);
    std::cout << "\nImaginary:" << std::endl;
    print_i(temp, 257);

    std::cout << "\nStop." << std::endl;

    return 0;
}
// This function is sadness
void fft(float data[], unsigned long nn, int isign)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2)
    {
        if (j > i)
        {
            SWAP(data[j], data[i]);
            SWAP(data[j + 1], data[i + 1]);
        }
        m = n >> 1;
        while (m >= 2 && j > m)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax)
    {
        istep = mmax << 1;
        theta = isign * (6.28318530717959 / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp * wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2)
        {
            for (i = m; i <= n; i += istep)
            {
                j = i + mmax;
                tempr = wr * data[j] - wi * data[j + 1];
                tempi = wr * data[j + 1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}
#undef SWAP
void print_i(float array[], int size)
{
    for (int i = 2; i < size; i += 2)
    {
        std::cout << array[i] << std::endl;
    }
}
void print_real(float array[], int size)
{
    for (int i = 1; i < size; i += 2)
    {
        std::cout << array[i] << std::endl;
    }
}